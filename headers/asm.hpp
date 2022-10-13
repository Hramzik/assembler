#ifndef ASM_HPP_INCLUDED
#define ASM_HPP_INCLUDED









#include "common.hpp"


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


struct Label_structure {

    int   pointingto;
    char* name;
};


typedef Label_structure Label;


Command_code get_command_name   (char* command);
Return_code  assembler          (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code  binary_array_write (void* array, void* filler, size_t size, size_t* bytes_filled);
Return_code  listing_write      (size_t command_ind, Command_code command_code, char* command, Argument argument = NAN, Command_mode command_mode = -1);
bool         islabel            (char* str);








#endif