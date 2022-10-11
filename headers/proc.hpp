#ifndef PROC_HPP_INCLUDED
#define PROC_HPP_INCLUDED









#include "common.hpp"


//-------------------- SETTINGS --------------------
//#define work_folder               work/
#define default_source_file_name  "work/out.txt"
#define default_output_file_name  "work/answer.txt"

#define wanted_signature "WW"
//--------------------------------------------------


#define interactive_flag "-i"


Return_code processor     (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code processor_i   (const char* out_name = default_output_file_name);
Return_code greetings     (double version, char signature_first_letter, char signature_second_letter, const char* out_name = default_output_file_name);
bool        isinteractive (int num_str, char** string_array);









#endif