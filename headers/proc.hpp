#ifndef PROC_HPP_INCLUDED
#define PROC_HPP_INCLUDED









#include "common.hpp"


//-------------------- SETTINGS --------------------
//#define work_folder               work/
#define default_source_file_name  "work/out.txt"
#define default_output_file_name  "work/answer.txt"
//--------------------------------------------------


Return_code processor (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);









#endif