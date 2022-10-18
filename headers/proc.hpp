#ifndef PROC_HPP_INCLUDED
#define PROC_HPP_INCLUDED









#include "common.hpp"
#include "../lib/stack.hpp"


//-------------------- SETTINGS --------------------
//#define work_folder               work/
#define default_source_file_name  "work/out.txt"
#define default_output_file_name  "work/answer.txt"

#define default_memory_size 100

#define wanted_signature "WW"
//--------------------------------------------------


#define interactive_flag "-i"


struct Processor_structure {

    size_t    ip;
    void*     code;
    Argument  RAX;
    Argument  RBX;
    Argument  RCX;
    Argument  RDX;
    Argument* memory;
    Stack*    stack;
    Stack*    function_call_stack;
};


typedef struct Processor_structure Processor;


Return_code processor_run         (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code processor_run_i       (const char* out_name = default_output_file_name);
Return_code greetings            (double version, char signature_first_letter, char signature_second_letter, const char* out_name = default_output_file_name);
bool        isinteractive         (int num_str, char** string_array);
Return_code initialize_processor (Processor* processor, size_t commands_size, size_t memory_size = default_memory_size);









#endif