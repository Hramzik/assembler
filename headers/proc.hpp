#ifndef PROC_HPP_INCLUDED
#define PROC_HPP_INCLUDED









#include "common.hpp"
#include "../lib/types/Stack.hpp"


//-------------------- SETTINGS --------------------
//#define work_folder               work/
#define default_source_file_name  "work/out.txt"
#define default_output_file_name  "work/answer.txt"

#define default_memory_capacity 100

#define wanted_signature "WW"

#define EPSILON 0.001
//--------------------------------------------------


#define interactive_flag "-i"


#define DEF_REG(name, ...) Argument name; 
struct Processor_structure {

    size_t    ip;
    void*     code;
    #include "reg.h"
    Argument* memory;
    size_t    memory_capacity;
    Stack*    stack;
    Stack*    function_call_stack;
};
#undef DEF_REG

typedef struct Processor_structure Processor;


Return_code processor_run        (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code processor_run_i      (const char* out_name = default_output_file_name);
Return_code greetings            (double version, char signature_first_letter, char signature_second_letter, const char* out_name = default_output_file_name);
bool        isinteractive        (int num_str, char** string_array);
Return_code initialize_processor (Processor* processor, size_t commands_size, size_t memory_capacity = default_memory_capacity);
Argument*   _get_register_adress (Processor* processor, Argument _reg_num);
Return_code _poison_memory       (Processor* processor);
Return_code _processor_case_push (Processor* processor);
Return_code _processor_case_pop  (Processor* processor);
int          double_compare      (double first, double second);








#endif