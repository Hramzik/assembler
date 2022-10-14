

#include "headers/asm.hpp"

#include "lib/onegin.hpp"
#include "lib/stack.hpp"


Command_code  get_command_name  (char* command) {

    if (command == nullptr) { return UNKNOWN; }


    if ( !stricmp (command, "unknown"))                                                              { return UNKNOWN; }
    if ( !stricmp (command, "halt")      || !stricmp (command, "hlt"))                               { return HALT; }
    if ( !stricmp (command, "out"))                                                                  { return OUT; }
    if ( !stricmp (command, "push"))                                                                 { return PUSH; }
    if ( !stricmp (command, "pop"))                                                                  { return POP; }
    if ( !stricmp (command, "add"))                                                                  { return ADD; }
    if ( !stricmp (command, "substract") || !stricmp (command, "sub"))                               { return SUBSTRACT; }
    if ( !stricmp (command, "multiply")  || !stricmp (command, "mult") || !stricmp (command, "mul")) { return MULTIPLY; }
    if ( !stricmp (command, "divide")    || !stricmp (command, "div"))                               { return DIVIDE; }
    if ( !stricmp (command, "jump")      || !stricmp (command, "jmp"))                               { return JUMP; }
    if ( !stricmp (command, "duplicate") || !stricmp (command, "dup"))                               { return DUPLICATE; }

    return UNKNOWN;
}


Return_code  assembler  (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "r");
    FILE* out    = fopen (   out_name, "wb");


    if (source == nullptr || out == nullptr) { return FILE_ERR; }



    Text* source_lines = initialize_text (source_name); //КОСТЫЛЬ!!! ПЕРЕПИСАТЬ через FILE*

    Label label_list [MAX_COMMAND_LEN] = {};
    try (_collect_labels (label_list, source_lines));


    char     command              [MAX_COMMAND_LEN]         = "";
    Argument command_byte_numbers [source_lines->num_lines] = {};
    size_t   commands_size             = source_lines->num_lines * (Command_code_size + Argument_size);
    void*    commands                  = calloc (1, commands_size);

    size_t bytes_filled = 0;
    size_t command_ind  = 0;
    for (size_t line_ind = 0; line_ind < source_lines->num_lines; line_ind++) {

        sscanf (source_lines->lines[line_ind].ptr, "%s", command);

        command_byte_numbers [command_ind] = (double) bytes_filled;


        Command_code command_code = get_command_name (command);
        Command_mode command_mode = 0;
        Argument     argument     = NAN;
        char jump_text_argument [MAX_LABEL_LEN] = "";


        switch (command_code) {

            case UNKNOWN:

                if (_islabel (command)) {

                    command_ind--;
                    break;
                }

                else {

                    LOG_ERROR (BAD_ARGS);
                    return BAD_ARGS;
                }

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

            case JUMP: {

                sscanf (source_lines->lines[line_ind].ptr, "%*s %s", jump_text_argument);


                size_t i = 0;
                bool label_found = false;
                for ( ; i < MAX_LABEL_COUNT; i++) {

                    if (!strcmp (label_list [i].name, jump_text_argument)) { label_found = true; break; }
                }
                if (!label_found) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


                argument = (double) label_list [i].ip;
                binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);
                binary_array_write (commands, &argument,     Argument_size,     &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command, argument));
                break;
            }

            case DUPLICATE:

                binary_array_write       (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (listing_write (command_ind, command_code, command));
                break;

            default:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }

        command_ind++;
    }


    Preamble preamble = {'W', 'W', bytes_filled, command_ind, 1.0};
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
    if (first_function_call_flag) { listing_file = fopen (default_listing_file_name, "w"); }
    else                          { listing_file = fopen (default_listing_file_name, "a"); }


    if (command_ind == 0) {fprintf (listing_file, "num      code    mode    args    name    args\n    -----------------------------------------\n"); }
    else { fprintf (listing_file, "\n    |\n"); }

    fprintf (listing_file, "%04zd|    %04X", command_ind, command_code);

    if (command_mode != -1) { fprintf (listing_file, "    %04X",   command_mode); }
    else                    { fprintf (listing_file, "        "); }

    if ( !isnan (argument)) { fprintf (listing_file, "    %-4.1lf", argument); }
    else                    { fprintf (listing_file, "        "); }

    fprintf                           (listing_file, "    %s",     command);
    if ( !isnan (argument)) { fprintf (listing_file, "    %-4.1lf", argument); }


    fclose (listing_file);
    if (first_function_call_flag) { first_function_call_flag = false; }

    return SUCCESS;
}


bool  _islabel  (char* str) {

    if (str == nullptr) { return false; }

    if (isdigit (str[0]) && str [strlen (str) - 1] != ':') { return false; }

    /*for (size_t i = 0; str [i] != '\0'; i++) {

        if (!isalpha (str [i]) && !isdigit (str [i])) { return false; }
    }*/
    return true;
}


Return_code  _collect_labels  (Label* label_list, const Text* source_lines) {

    assert (label_list && source_lines);


    for (size_t i = 0; i < MAX_LABEL_COUNT; i++) {

        label_list [i] = Label {0, 0, nullptr};
    }


    char command [MAX_COMMAND_LEN] = "";
    size_t bytes_filled = 0;
    size_t command_ind  = 0;
    size_t label_ind    = 0;
    for (size_t line_ind = 0; line_ind < source_lines->num_lines; line_ind++) {

        sscanf (source_lines->lines[line_ind].ptr, "%s", command);


        Command_code command_code = get_command_name (command);


        switch (command_code) {

            case UNKNOWN:

                if (_islabel (command)) {

                    char should_be_blank [source_lines->lines[line_ind].len + 1] = "";
                    sscanf (source_lines->lines[line_ind].ptr, "%*s %s", should_be_blank);
                    if (strcmp (should_be_blank, "")) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


                    label_list [label_ind] = { command_ind, bytes_filled, nullptr };
                    label_list [label_ind].name = (char*) calloc (strlen (command), 1);
                    strcpy (label_list [label_ind].name, command);
                    command_ind--;
                    label_ind  ++;
                    break;
                }

                else {

                    LOG_ERROR (BAD_ARGS);
                    return BAD_ARGS;
                }

            case HALT:

                bytes_filled += Command_code_size;
                break;

            case OUT:

                bytes_filled += Command_code_size;
                break;

            case PUSH:

                bytes_filled += Command_code_size;
                bytes_filled += Command_mode_size;
                bytes_filled += Argument_size;
                break;

            case POP:

                bytes_filled += Command_code_size;
                break;

            case ADD:

                bytes_filled += Command_code_size;
                break;

            case SUBSTRACT:

                bytes_filled += Command_code_size;
                break;

            case MULTIPLY:

                bytes_filled += Command_code_size;
                break;

            case DIVIDE:

                bytes_filled += Command_code_size;
                break;

            case JUMP:

                bytes_filled += Command_code_size;
                bytes_filled += Argument_size;
                break;

            case DUPLICATE:

                bytes_filled += Command_code_size;
                break;

            default:

                LOG_ERROR (BAD_ARGS);
                return BAD_ARGS;
        }

        command_ind++;
    }


    return SUCCESS;
}


