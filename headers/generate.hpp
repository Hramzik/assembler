

#include <stdio.h>
#include <string.h>

#include "../lib/Return_code.hpp"


#define default_source_name "work/pic.txt"
#define default_dest_name   "work/generated.txt"


const size_t MAX_FILE_NAME_LEN = 40;


Return_code generate (const char* source_name = default_source_name, const char* dest_name = default_dest_name);


