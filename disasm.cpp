

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


    Command_code command_code = UNKNOWN;
    Command_mode command_mode = 0;
    Argument     argument     = NAN;

    for (size_t bytes_read = 0; bytes_read < preamble.out_file_size; ) {

        bytes_read += fread (&command_code, 1, Command_code_size, source);


        switch (command_code) {

            case UNKNOWN:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            case HALT:

                fprintf (out, "halt");
                break;

            case OUT:

                fprintf (out, "out");
                break;

            case PUSH:

                fprintf (out, "push ");

                bytes_read += fread (&command_mode, 1, Command_mode_size, source);
                bytes_read += fread (&argument,     1, Argument_size,     source);

                fprintf (out, "%lf", argument); //KOSTIL!!!!!!!!!!! SHOULD CHECK COMMAND_MODE

                break;

            case POP:

                fprintf (out, "pop");
                break;

            case ADD:

                fprintf (out, "add");
                break;

            case SUBSTRACT:

                fprintf (out, "sub");
                break;

            case MULTIPLY:

                fprintf (out, "mult");
                break;

            case DIVIDE:

                fprintf (out, "divide");
                break;

            case JUMP:

                fprintf (out, "jump");
                break;

            case DUPLICATE:

                fprintf (out, "duplicate");
                break;

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


