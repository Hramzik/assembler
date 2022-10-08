#define COMMON_HPP_INCLUDED
#ifndef COMMON_HPP_INCLUDED









enum  Return_code  {

    SUCCESS    = 0,
    MEMORY_ERR = 1,
    BAD_ARGS   = 2,
    FILE_ERR   = 3,
};

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
};


struct Preamble_struct {

    char   signature_first_letter;
    char   signature_second_letter;
    size_t out_file_size;
    double version;
};


typedef Preamble_struct Preamble;

typedef double Argument;
typedef char   Command_mode;


const size_t Command_size      = sizeof (Command_code);
const size_t Command_mode_size = sizeof (Command_mode);
const size_t Argument_size     = sizeof (Argument);
const size_t Preamble_size     = sizeof (Preamble);









#endif