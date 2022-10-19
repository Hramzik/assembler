#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED









#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../lib/Return_code.hpp"


#define DEF_CMD(name, code, ...) name = code,
enum Command_code {

    UNKNOWN_CODE = 0,
    #include "cmd.h"
};
#undef DEF_CMD


#define DEF_REG(name, num) name = num,
enum Register {

    UNKNOWN_REG = 0,
    #include "reg.h"
};
#undef DEF_REG


typedef struct Preamble_struct Preamble;
struct         Preamble_struct {

    char   signature_first_letter;
    char   signature_second_letter;
    size_t out_file_size;
    size_t num_commands;
    double version;
};


typedef char   Command_mode;
typedef double Argument;


const size_t Command_code_size = sizeof (Command_code);
const size_t Command_mode_size = sizeof (Command_mode);
const size_t Argument_size     = sizeof (Argument);
const size_t Preamble_size     = sizeof (Preamble);









#endif