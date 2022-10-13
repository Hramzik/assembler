#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED









#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../lib/Return_code.hpp"


enum Command_code {

    UNKNOWN   = 0,
    HALT      = 1,
    OUT       = 2,
    PUSH      = 3,
    POP       = 4,
    ADD       = 5,
    SUBSTRACT = 6,
    MULTIPLY  = 7,
    DIVIDE    = 8,
    JUMP      = 9,
};


struct Preamble_struct {

    char   signature_first_letter;
    char   signature_second_letter;
    size_t out_file_size;
    size_t num_commands;
    double version;
};


typedef Preamble_struct Preamble;

typedef double Argument;
typedef char   Command_mode;


const size_t Command_code_size = sizeof (Command_code);
const size_t Command_mode_size = sizeof (Command_mode);
const size_t Argument_size     = sizeof (Argument);
const size_t Preamble_size     = sizeof (Preamble);









#endif