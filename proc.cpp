

#include "headers/proc.hpp"

#include "lib/stack.hpp"


Return_code  processor_run  (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "rb");

    if (source == nullptr) { return FILE_ERR; }


    Preamble preamble = {};
    fread (&preamble, 1, Preamble_size, source);


    Return_code return_code = greetings (preamble.version, preamble.signature_first_letter, preamble.signature_second_letter);
    if (return_code) { LOG_ERROR (return_code); return return_code; }


    #define DEF_REG(name, ...) .name = NAN, 
    Processor processor = {
        .ip = 0, .code = nullptr,
        #include "headers/reg.h"
        .memory = nullptr, .memory_capacity = 0, .stack = nullptr, .function_call_stack = nullptr
    };
    #undef DEF_REG
    try (initialize_processor (&processor, preamble.out_file_size));


    fread (processor.code, 1, preamble.out_file_size, source);


    FILE* out = fopen (out_name, "a");
    if (out == nullptr) { return FILE_ERR; }


    Command_code command_code = UNKNOWN_CODE;
    Argument     argument     = NAN;


    #define DEF_CMD(name, code, args, mode, asm, proc, ...)\
        case name:\
            proc;

    for ( ; processor.ip < preamble.out_file_size; ) {

        command_code  = * (Command_code*) ( (char*) processor.code + processor.ip); //printf ("%d %zd\n", command_code, processor.ip);
        processor.ip += Command_code_size;


        switch (command_code) {

            case UNKNOWN_CODE:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            #include "headers/cmd.h"

            default:

                LOG_ERROR (BAD_ARGS); //printf ("%d %zd", command_code, processor.ip);
                return BAD_ARGS;
        }
    }


    fclose      (source);
    fclose      (out);
    STACK_DTOR (processor.stack);


    return SUCCESS;
}


Return_code  greetings  (double version, char signature_first_letter, char signature_second_letter, const char* out_name) {

    FILE* out = fopen (out_name, "w");
    if (out == nullptr) { return FILE_ERR; }

    if (signature_first_letter != wanted_signature[0] or signature_second_letter != wanted_signature[1]) { return BAD_ARGS; }


    fprintf (out, "Welcome to the processor module! (version %lf)\n\n", version);


    fclose (out);


    return SUCCESS;
}


Return_code  processor_run_i  (const char* out_name) {

    const char* fuck_off = out_name; fuck_off += 1;
    printf ("ayo!");
    return SUCCESS;
}


bool  isinteractive (int num_str, char** string_array) {

    assert (string_array != nullptr);


    bool return_value = false;
    for (int i = 0; i < num_str; i++) {

        if (string_array [i] != nullptr && !strcmp (string_array [i], interactive_flag)) { return_value = true; break; }
    }


    return return_value;
}


#define DEF_REG(name, ...) processor->name = NAN;
Return_code  initialize_processor  (Processor* processor, size_t commands_size, size_t memory_capacity) {

    if (!processor) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    processor->ip = 0;


    processor->code = calloc (commands_size, 1);
    if (!processor->code) { LOG_ERROR (MEMORY_ERR); return MEMORY_ERR; }


    #include "headers/reg.h"


    processor->memory = (Argument*) calloc (memory_capacity * Argument_size, 1);
    if (!processor->memory) { LOG_ERROR (MEMORY_ERR); return MEMORY_ERR; }
    processor->memory_capacity = memory_capacity;
    _poison_memory (processor);


    static Stack stack               = {}; //??
    static Stack function_call_stack = {}; 

    try (STACK_CTOR               (&stack));
    try (STACK_CTOR (&function_call_stack));

    processor->stack               = &stack;
    processor->function_call_stack = &function_call_stack;


    return SUCCESS;
}
#undef DEF_REG


#define DEF_REG(name, num) case num: return &processor->name;
Argument*  _get_register_adress  (Processor* processor, Argument _reg_num) {

    size_t reg_num = (size_t) round (_reg_num);
    switch (reg_num) {

        #include "headers/reg.h"
        default: return nullptr;
    }
}
#undef DEF_REG


Return_code  _poison_memory  (Processor* processor) {

    if (!processor) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (size_t i = 0; i < processor->memory_capacity; i++) {
    
        processor->memory [i] = NAN;
    }


    return SUCCESS;
}


Return_code  _processor_case_push  (Processor* processor) {

    Command_mode command_mode = * (Command_mode*) ( (char*) processor->code + processor->ip); processor->ip += Command_mode_size;
    Argument     argument     = * (Argument*)     ( (char*) processor->code + processor->ip); processor->ip += Argument_size;
    if (isnan (argument)) { argument = 0; }


    if (command_mode >> 2) {

        Argument* register_adress = _get_register_adress (processor, command_mode >> 2);
        if (!register_adress) {

            LOG_ERROR (BAD_ARGS);
            return BAD_ARGS;
        }

        if (isnan (*register_adress)) {

            LOG_MESSAGE ("register is filled with poison!");
            return BAD_ARGS;
        }

        argument += *register_adress;;
    }

    if (command_mode & 2) {

        if ( round (argument) < 0 || (size_t) round (argument) >= processor->memory_capacity) {

            LOG_MESSAGE ("segmentation fault");
            LOG_ERROR (BAD_ARGS);
            return BAD_ARGS;
        }

        argument = processor->memory [ (size_t) argument];
        if (isnan (argument)) {

            LOG_MESSAGE ("memory cell is filled with poison!"); //dump
            return BAD_ARGS;
        }
    }

    return stack_push (processor->stack, argument);
}


Return_code  _processor_case_pop  (Processor* processor) {

    Command_mode command_mode = * (Command_mode*) ( (char*) processor->code + processor->ip); processor->ip += Command_mode_size;
    Argument     argument     = * (Argument*)     ( (char*) processor->code + processor->ip); processor->ip += Argument_size;
    Argument*    write_target = nullptr;

    if (!(command_mode & 2) && !isnan (argument)) { LOG_MESSAGE ("can't write into the constant"); return BAD_ARGS; }
    if (isnan (argument)) { argument = 0; }


    if (command_mode >> 2) {

        write_target = _get_register_adress (processor, command_mode >> 2);
        if (!write_target) {

            LOG_ERROR (BAD_ARGS);
            return BAD_ARGS;
        }
    }

    if (command_mode & 2) {

        if (write_target) { argument += *write_target; }
        if ( round (argument) < 0 || (size_t) round (argument) >= processor->memory_capacity) {

            LOG_MESSAGE ("segmentation fault");
            LOG_ERROR (BAD_ARGS);
            return BAD_ARGS;
        }

        write_target = &processor->memory [ (size_t) argument];
    }


    Return_code pop_return_code = BAD_ARGS;
    *write_target = stack_pop (processor->stack, &pop_return_code).value;
    if (pop_return_code) { LOG_ERROR (pop_return_code); return pop_return_code; }


    return SUCCESS;
}


int  double_compare  (double first, double second) {

    assert (!isnan(first) && !isnan(second));

    if      (fabs (first - second) < EPSILON) { return 0;  }

    else if (first > second)                  { return 1;  }

    else  /*(first < second)*/                { return -1; }
}


