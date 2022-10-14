

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


    Processor   processor   = {0, nullptr, NAN, NAN, NAN, NAN, nullptr};
    return_code             = initialize_processor (&processor,preamble.out_file_size);
    if (return_code) { LOG_ERROR (return_code); return return_code; }

    fread (processor.code, 1, preamble.out_file_size, source);


    FILE* out = fopen (out_name, "a");
    if (out == nullptr) { return FILE_ERR; }


    Command_code command_code = UNKNOWN;
    Command_mode command_mode = 0;
    Argument     argument     = NAN;

    bool halt_seen_flag = false;

    Stack stack;
    STACK_CTOR (&stack);

    for ( ; processor.ip < preamble.out_file_size; ) {

        command_code  = * (Command_code*) ( (char*) processor.code + processor.ip);
        processor.ip += Command_code_size;


        switch (command_code) {

            case UNKNOWN:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            case HALT:

                halt_seen_flag = true;
                break;

            case OUT:

                fprintf (out, "%lf\n", stack_pop (&stack).value ); //func
                break;

            case PUSH:

                command_mode = * (Command_mode*) ( (char*) processor.code + processor.ip); processor.ip += Command_mode_size;
                argument     = * (Argument*)     ( (char*) processor.code + processor.ip); processor.ip += Argument_size;

                stack_push (&stack, argument); //SHOULD CHECK COMMAND_MODE

                break;

            case POP:

                //TOO SOON TO REALIZE
                break;

            case ADD:

                stack_push (&stack, stack_pop (&stack).value + stack_pop (&stack).value);
                break;

            case SUBSTRACT:

                stack_push (&stack, - stack_pop (&stack).value + stack_pop (&stack).value);
                break;

            case MULTIPLY:

                stack_push (&stack, stack_pop (&stack).value * stack_pop (&stack).value);
                break;

            case DIVIDE:

                stack_push (&stack, 1 / stack_pop (&stack).value * stack_pop (&stack).value);
                break;

            case JUMP:

                processor.ip = (size_t) round ( * (Argument*) ( (char*) processor.code + processor.ip));
                break;

            case DUPLICATE:

                argument = stack_pop (&stack).value;
                stack_push (&stack, argument);
                stack_push (&stack, argument);
                break;

            default:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }


        if (halt_seen_flag) { break; }
    }


    fclose      (source);
    fclose      (out);
    STACK_DTOR (&stack);


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


Return_code  initialize_processor  (Processor* processor, size_t commands_size, size_t memory_size) {

    if (!processor) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    processor->ip = 0;

    processor->code = calloc (commands_size, 1);
    if (!processor->code) { LOG_ERROR (MEMORY_ERR); return MEMORY_ERR; }

    processor->RAX = NAN;
    processor->RBX = NAN;
    processor->RCX = NAN;
    processor->RDX = NAN;

    processor->memory = (Argument*) calloc (memory_size, 1);
    if (!processor->memory) { LOG_ERROR (MEMORY_ERR); return MEMORY_ERR; }


    return SUCCESS;
}