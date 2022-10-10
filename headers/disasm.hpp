#ifndef DISASM_HPP_INCLUDED
#define DISASM_HPP_INCLUDED









#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "common.hpp"


//-------------------- SETTINGS --------------------
//#define work_folder               work/
#define default_source_file_name  "work/out.txt"
#define default_output_file_name  "work/original.txt"
//--------------------------------------------------


Return_code disassembler (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);









#endif