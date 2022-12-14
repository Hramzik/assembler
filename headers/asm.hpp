#ifndef ASM_HPP_INCLUDED
#define ASM_HPP_INCLUDED









#include "common.hpp"
#include "../lib/types/Text.hpp"

//-------------------- SETTINGS --------------------
#define CREATE_LISTING_FILE

#define default_listing_file_name "work/listing.txt"
#define default_source_file_name  "work/generated.txt"
#define default_output_file_name  "work/out.txt"

const size_t MAX_COMMAND_LEN   = 40;
const size_t MAX_LABEL_LEN     = 40;
const size_t MAX_LABEL_COUNT   = 40;
//--------------------------------------------------


#ifdef CREATE_LISTING_FILE
    #define IF_CREATING_LISTING_FILE(x) x
#else
    #define IF_CREATING_LISTING_FILE(x)
#endif


struct Label_structure {

    size_t command_ind;
    size_t ip;
    char*  name;
};
typedef Label_structure Label;


struct Argument_and_mode_structure {

    Argument     argument;
    Command_mode command_mode;
};
typedef struct Argument_and_mode_structure Argument_and_mode;


Command_code      _get_command_code       (char* command);
Return_code       assembler               (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code       _binary_array_write     (void* array, void* filler, size_t size, size_t* bytes_filled);
Return_code       _listing_write          (size_t bytes_filled, Command_code command_code, const char* command, Argument argument = NAN, Command_mode command_mode = -1);
bool              _islabel                (char* str);
Return_code       _collect_labels         (Label* label_list, const Text* source_lines);
Return_code       _asm_case_jumping_cmd   (char* source, Label* label_list, void* commands, size_t* bytes_filled, Command_code command_code);
size_t            _find_label             (Label* label_list, char* label_str);
Argument_and_mode _parse_args             (char* source);
Register          _get_register_code      (char* source);
Argument_and_mode _parse_args_in_brackets (char* argument);
Argument_and_mode _parse_solo_in_brackets (char* argument);
const char*       _get_command_name       (Command_code command_code);







#endif