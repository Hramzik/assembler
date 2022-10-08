

#include <sys\stat.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


//-------------------- SETTINGS --------------------
#define CREATE_LISTING_FILE

#define default_listing_file_name "listing.txt"
#define default_source_file_name  "code.txt"
#define default_output_file_name  "out.txt"

const size_t MAX_COMMAND_LEN = 40;
//--------------------------------------------------


#ifdef CREATE_LISTING_FILE
    #define IF_CREATING_LISTING_FILE(x) x
#else
    #define IF_CREATING_LISTING_FILE(x)
#endif

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
    double version;
};


typedef Preamble_struct Preamble;

typedef double Argument;
typedef char   Command_mode;


const size_t Command_size      = sizeof (Command_code);
const size_t Command_mode_size = sizeof (Command_mode);
const size_t Argument_size     = sizeof (Argument);
const size_t Preamble_size     = sizeof (Preamble);


Command_code get_command_name   (char* command);
Return_code  assembler          (const char* source_name = default_source_file_name, const char* out_name = default_output_file_name);
Return_code  binary_array_write (void* array, void* filler, size_t size, size_t* bytes_filled);
Return_code  listing_write      (size_t command_ind, Command_code command_code, char* command, Argument argument = NAN, Command_mode command_mode = -1);
