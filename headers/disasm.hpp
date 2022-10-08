#define DISASM_HPP_INCLUDED
#ifndef DISASM_HPP_INCLUDED









#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "common.h"


//-------------------- SETTINGS --------------------
#define CREATE_LISTING_FILE

//#define work_folder               work/
#define default_listing_file_name "work/listing.txt"
#define default_source_file_name  "work/code.txt"
#define default_output_file_name  "work/out.txt"

const size_t MAX_COMMAND_LEN = 40;
//--------------------------------------------------


#ifdef CREATE_LISTING_FILE
    #define IF_CREATING_LISTING_FILE(x) x
#else
    #define IF_CREATING_LISTING_FILE(x)
#endif


Command_code get_command_name   (char* command);
Return_code  assembler          (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code  binary_array_write (void* array, void* filler, size_t size, size_t* bytes_filled);
Return_code  listing_write      (size_t command_ind, Command_code command_code, char* command, Argument argument = NAN, Command_mode command_mode = -1);









#endif