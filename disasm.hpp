

#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


enum  Return_code  {

    SUCCESS    = 0,
    MEMORY_ERR = 1,
    BAD_ARGS   = 2,
    FILE_ERR   = 3,
};


enum Command_name {

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
    double version;
};


typedef Preamble_struct Preamble;

typedef double Argument;


const size_t MAX_COMMAND_LEN = 40;

const size_t  Command_size = sizeof (Command_name);
const size_t Argument_size = sizeof (Argument);
const size_t Preamble_size = sizeof (Preamble);


Command_name get_command_name (char* command);
Return_code  assembler        (const char* source_name = "code.txt", const char* out_name = "out.txt");

