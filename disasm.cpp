

#include "headers/disasm.hpp"

#include "lib/onegin.hpp"
#include "lib/stack.hpp"


Return_code disassembler  (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "rb");
    FILE* out    = fopen (   out_name, "w");


    if (source == nullptr || out == nullptr) { return FILE_ERR; }



    Preamble preamble = {};
    fread (&preamble, 1, Preamble_size, source);


    Command_code command_code  = UNKNOWN_CODE;
    Command_mode command_mode  = 0;
    Argument     argument      = NAN;
    const char*  register_name = nullptr;

    for (size_t bytes_read = 0; bytes_read < preamble.out_file_size; ) {

        bytes_read += fread (&command_code, 1, Command_code_size, source);


        #define DEF_CMD(name, code, args, mode, ...)\
\
            case name:\
\
                fprintf (out, "%s", #name);\
                if (args) {\
\
                    fprintf (out, " ");\
                    if (mode) {\
\
                        bytes_read += fread (&command_mode, 1, Command_mode_size, source);\
\
                        if (command_mode >> 2) { register_name = _get_register_name (command_mode >> 2); }\
\
                        if (command_mode & 2)  { fprintf (out, "["); }\
                        if (command_mode >> 2) { fprintf (out, "%s", register_name); }\
                    }\
\
                    bytes_read += fread (&argument, 1, Argument_size, source);\
                    if ( (command_mode & 1) && (command_mode >> 2) )  { fprintf (out, " + "); }\
                    if (!isnan (argument)) { fprintf (out, "%lf", argument); }\
\
                    if (mode && (command_mode & 2) ) { fprintf (out, "]"); }\
                }\
\
                break;


        switch (command_code) {

            case UNKNOWN_CODE:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            #include "headers/cmd.h"
            default:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }


        if (bytes_read != preamble.out_file_size) { fprintf (out, "\n"); }
    }


    fclose (source);
    fclose (out);


    return SUCCESS;
}
#undef DEF_CMD




#define DEF_REG(name, num)\
    case num:\
        return #name;

const char*  _get_register_name  (Command_mode register_num) {

    switch (register_num) {

        #include "headers/reg.h"

        default: return nullptr;
    }
}
#undef DEF_REG




#define DEF_CMD(name, ...)\
    case name:\
        return #name;


const char*  _get_command_name  (Command_code command_code) {

    switch (command_code) {

        case UNKNOWN_CODE: return nullptr;

        #include "headers/cmd.h"

        default: return nullptr;
    }
}
#undef DEF_CMD

