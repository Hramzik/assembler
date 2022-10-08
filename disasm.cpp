

#include "asm.hpp"

#include "onegin.hpp"
#include "stack.hpp"


Command_name get_command_name (char* command) {

    if (command == nullptr) { return UNKNOWN; }


    if ( !stricmp (command, "unknown"))                                 { return UNKNOWN; }
    if ( !stricmp (command, "halt")      || !stricmp (command, "hlt"))  { return HALT; }
    if ( !stricmp (command, "out"))                                     { return OUT; }
    if ( !stricmp (command, "push"))                                    { return PUSH; }
    if ( !stricmp (command, "pop"))                                     { return POP; }
    if ( !stricmp (command, "add"))                                     { return ADD; }
    if ( !stricmp (command, "substract") || !stricmp (command, "sub"))  { return SUBSTRACT; }
    if ( !stricmp (command, "multiply")  || !stricmp (command, "mult")) { return MULTIPLY; }
    if ( !stricmp (command, "divide")    || !stricmp (command, "div"))  { return DIVIDE; }

    return UNKNOWN;
}


Return_code disassembler (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "r");
    FILE* out    = fopen (   out_name, "w");


    if (source == nullptr || out == nullptr) { return FILE_ERR; }



    Preamble preamble = {'W', 'W', 1.0};
    fwrite ( &preamble, Preamble_size, 1, out);/*
    fwrite ( &preamble.signature_first_letter, sizeof preamble.signature, 1, out);
    fwrite ( &preamble.version,   sizeof preamble.version,   1, out);*/


    Text* source_lines = initialize_text (source_name); //КОСТЫЛЬ!!! ПЕРЕПИСАТЬ!!!


    char command [MAX_COMMAND_LEN] = "";
    for (size_t i = 0; i < source_lines->num_lines; i++) {

        sscanf (source_lines->lines[i].ptr, "%s", command); printf ("%s ", command);

        Command_name command_code = get_command_name (command); printf ("%d\n", command_code);
        Argument     argument;

        switch (command_code) {

            case UNKNOWN:
                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            case HALT:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case OUT:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case PUSH:
                sscanf (source_lines->lines[i].ptr, "%*s %lf", &argument);

                fwrite ( &command_code, Command_size, 1, out);
                fwrite ( &argument,    Argument_size, 1, out);
                break;

            case POP:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case ADD:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case SUBSTRACT:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case MULTIPLY:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            case DIVIDE:
                fwrite ( &command_code, Command_size, 1, out);
                break;

            default:
                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }
    }

    cleanmemory (source_lines);


    return SUCCESS;
}