

#include "headers/asm.hpp"

#include "lib/onegin.hpp"
#include "lib/stack.hpp"


Command_code  get_command_name  (char* command) {

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


Return_code  assembler  (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "r");
    FILE* out    = fopen (   out_name, "wb");


    if (source == nullptr || out == nullptr) { return FILE_ERR; }



    Text* source_lines = initialize_text (source_name); //КОСТЫЛЬ!!! ПЕРЕПИСАТЬ!!!


    char   command [MAX_COMMAND_LEN] = "";
    size_t commands_size             = source_lines->num_lines * (Command_code_size + Argument_size);
    void*  commands                  = calloc (1, commands_size);

    size_t bytes_filled = 0;
    for (size_t line_ind = 0, command_ind = 0; line_ind < source_lines->num_lines; line_ind++) {

        sscanf (source_lines->lines[line_ind].ptr, "%s", command);


        Command_code command_code = get_command_name (command);
        Command_mode command_mode = 0;
        Argument     argument;


        switch (command_code) {

            case UNKNOWN:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;

            case HALT:

                binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case OUT:

                binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case PUSH:

                sscanf (source_lines->lines[line_ind].ptr, "%*s %lf", &argument);

                command_mode = 0;                     //KOSTIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);
                binary_array_write (commands, &command_mode, Command_mode_size, &bytes_filled);
                binary_array_write (commands, &argument,     Argument_size,     &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command, argument, command_mode));
                break;

            case POP:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case ADD:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case SUBSTRACT:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case MULTIPLY:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            case DIVIDE:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            default:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }

        command_ind++;
    }


    Preamble preamble = {'W', 'W', bytes_filled, 1.0};
    fwrite ( &preamble, Preamble_size, 1, out);

    fwrite ( commands, bytes_filled,   1, out);


    cleanmemory (source_lines);


    return SUCCESS;
}


Return_code  binary_array_write  (void* array, void* filler, size_t size, size_t* bytes_filled) {

    memcpy ( (char*) array + *bytes_filled, filler, size);
    *bytes_filled += size;


    return SUCCESS;
}


Return_code  listing_write  (size_t command_ind, Command_code command_code, char* command, Argument argument, Command_mode command_mode) {

    static bool  first_function_call_flag = true;

    FILE* listing_file = nullptr;
    if (first_function_call_flag) { listing_file = fopen (default_listing_file_name, "w"); } //static
    else                          { listing_file = fopen (default_listing_file_name, "a"); } //static


    if (command_ind == 0) {fprintf (listing_file, "num      code    mode    args    name    args\n    -----------------------------------------\n"); }
    else { fprintf (listing_file, "\n    |\n"); }

    fprintf (listing_file, "%04zd|    %04X", command_ind, command_code);

    if (command_mode != -1) { fprintf (listing_file, "    %04X",   command_mode); }
    else                    { fprintf (listing_file, "        "); }

    if ( !isnan (argument)) { fprintf (listing_file, "    %2.1lf", argument); }
    else                    { fprintf (listing_file, "        "); }

    fprintf                           (listing_file, "    %s",     command);
    if ( !isnan (argument)) { fprintf (listing_file, "    %2.1lf", argument); }


    fclose (listing_file); //КОСТЫЛЬ!!!!! ПЕРЕДЕЛАТЬ!!!!!!!!!!
    if (first_function_call_flag) { first_function_call_flag = false; }


    return SUCCESS;
}


