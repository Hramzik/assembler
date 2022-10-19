

#include "headers/asm.hpp"

#include "lib/onegin.hpp"
#include "lib/stack.hpp"


Command_code  _get_command_code  (char* command) {

    if (command == nullptr) { return UNKNOWN_CODE; }


    if ( !stricmp (command, "unknown"))                                                              { return UNKNOWN_CODE; }
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
    if ( !stricmp (command, "call"))                                                                 { return CALL; }
    if ( !stricmp (command, "return")    || !stricmp (command, "ret"))                               { return RETURN; }

    return UNKNOWN_CODE;
}


Return_code  assembler  (const char* source_name, const char* out_name) {

    if (source_name == nullptr || out_name == nullptr) { return BAD_ARGS; }


    FILE* source = fopen (source_name, "r");
    FILE* out    = fopen (   out_name, "wb");


    if (source == nullptr || out == nullptr) { return FILE_ERR; }



    Text* source_lines = initialize_text (source_name); //КОСТЫЛЬ!!! ПЕРЕПИСАТЬ через FILE*

    Label label_list [MAX_COMMAND_LEN] = {};
    try (_collect_labels (label_list, source_lines));


    char              command [MAX_COMMAND_LEN] = "";
    size_t            commands_size             = source_lines->num_lines * (Command_code_size + Argument_size);
    void*             commands                  = calloc (1, commands_size);
    Argument_and_mode argument_and_mode         = {NAN, 0};

    size_t bytes_filled = 0;
    size_t command_ind  = 0;
    for (size_t line_ind = 0; line_ind < source_lines->num_lines; line_ind++) {

        sscanf (source_lines->lines[line_ind].ptr, "%s", command);


        Command_code command_code = _get_command_code (command);
        Argument     argument     = NAN;


        switch (command_code) {

            case UNKNOWN_CODE:

                if (_islabel (command)) {

                    command_ind--;
                    break;
                }

                else {

                    LOG_ERROR (BAD_ARGS);
                    return BAD_ARGS;
                }

            case HALT:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case OUT:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case PUSH:

                argument_and_mode = _parse_args (source_lines->lines[line_ind].ptr);

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);
                _binary_array_write (commands, &argument_and_mode.command_mode, Command_mode_size, &bytes_filled);
                _binary_array_write (commands, &argument_and_mode.argument,     Argument_size,     &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command, argument_and_mode.argument, argument_and_mode.command_mode));
                break;

            case POP:

                argument_and_mode = _parse_args (source_lines->lines[line_ind].ptr);

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);
                _binary_array_write (commands, &argument_and_mode.command_mode, Command_mode_size, &bytes_filled);
                _binary_array_write (commands, &argument_and_mode.argument,     Argument_size,     &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command, argument_and_mode.argument, argument_and_mode.command_mode));
                break;

            case ADD:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case SUBSTRACT:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case MULTIPLY:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case DIVIDE:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case JUMP:

                _asm_case_jump (source_lines->lines[line_ind].ptr, label_list, commands, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command, argument));
                break;

            case DUPLICATE:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
                break;

            case CALL:

                _asm_case_call (source_lines->lines[line_ind].ptr, label_list, commands, &bytes_filled, command_ind);
                break;

            case RETURN:

                _binary_array_write (commands, &command_code, Command_code_size, &bytes_filled);


                IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, command));
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


Return_code  _binary_array_write  (void* array, void* filler, size_t size, size_t* bytes_filled) {

    memcpy ( (char*) array + *bytes_filled, filler, size);
    *bytes_filled += size;


    return SUCCESS;
}


Return_code  _listing_write  (size_t command_ind, Command_code command_code, const char* command, Argument argument, Command_mode command_mode) {

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


bool  _islabel  (char* str)  {

    if (str == nullptr) { return false; }

    if (isdigit (str[0]) && str [strlen (str) - 1] != ':') { return false; }

    /*for (size_t i = 0; str [i] != '\0'; i++) {

        if (!isalpha (str [i]) && !isdigit (str [i])) { return false; }
    }*/
    return true;
}


Return_code  _collect_labels  (Label* label_list, const Text* source_lines) {

    if (!label_list || !source_lines) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (size_t i = 0; i < MAX_LABEL_COUNT; i++) {

        label_list [i] = Label {0, 0, nullptr};
    }


    char command [MAX_COMMAND_LEN] = "";
    size_t bytes_filled = 0;
    size_t command_ind  = 0;
    size_t label_ind    = 0;
    for (size_t line_ind = 0; line_ind < source_lines->num_lines; line_ind++) {

        sscanf (source_lines->lines[line_ind].ptr, "%s", command);


        Command_code command_code = _get_command_code (command);


        switch (command_code) {

            case UNKNOWN_CODE:

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

            case CALL:

                bytes_filled += Command_code_size;
                bytes_filled += Argument_size;
                break;

            case RETURN:

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


Return_code  _asm_case_jump  (char* source, Label* label_list, void* commands, size_t* bytes_filled) {

    if (!source || !label_list || !commands || !bytes_filled) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Argument argument = NAN;
    char jump_text_argument [MAX_LABEL_LEN] = "";

    sscanf (source, "%*s %s", jump_text_argument);


    size_t i = _find_label (label_list, jump_text_argument);
    argument = (double) label_list [i].ip;


    Command_code command_code = JUMP;
    _binary_array_write (commands, &command_code, Command_code_size, bytes_filled);
    _binary_array_write (commands, &argument,     Argument_size,     bytes_filled);


    return SUCCESS;
}

//copypast - almost the same as _asm_case_jump - how to fix?
Return_code  _asm_case_call  (char* source, Label* label_list, void* commands, size_t* bytes_filled, size_t command_ind) {

    
    if (!source || !label_list || !commands || !bytes_filled) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Argument argument = NAN;
    char jump_text_argument [MAX_LABEL_LEN] = "";

    sscanf (source, "%*s %s", jump_text_argument);


    size_t i = _find_label (label_list, jump_text_argument);
    argument = (double) label_list [i].ip;


    Command_code command_code = CALL;
    _binary_array_write (commands, &command_code, Command_code_size, bytes_filled);
    _binary_array_write (commands, &argument,     Argument_size,     bytes_filled);


    IF_CREATING_LISTING_FILE (_listing_write (command_ind, command_code, "call", argument));
    return SUCCESS;
}


size_t  _find_label  (Label* label_list, char* label_str) {

    if (!label_list || !label_str) { LOG_ERROR (BAD_ARGS); return 0; }
    size_t i = 0;
    bool label_found = false;
    for ( ; i < MAX_LABEL_COUNT; i++) {

        if (!strcmp (label_list [i].name, label_str)) { label_found = true; break; }
    }

    if (!label_found) { LOG_ERROR (BAD_ARGS); return 0; }


    return i;
}


Argument_and_mode  _parse_args  (char* source) {

    assert (source);


    char* argument = nullptr;
    int i = 0;
    sscanf (source, "%*s %n", &i);
    
    argument = source + i;

    if (argument [0] == '(') {
    
        return _parse_args_in_brackets (argument);
    }
    
    if (argument [0] == '[') {
    
        return _parse_args_in_brackets (argument);
    }


    Register register_argument = _get_register_code (argument);
    if (register_argument) {
    
        return {NAN, (Command_mode) (register_argument << 2)};
    }

    Argument const_argument = NAN;
    char should_be_blank [strlen (source)] = "";
    if (sscanf (argument, "%lf %s", &const_argument, should_be_blank) && isblank (should_be_blank)) {

        return {const_argument, 1};
    }


    LOG_ERROR (BAD_ARGS); return {NAN, 0};
}


#define   DEF_REG(name, ...)\
    if ( !stricmp (cut_source, #name)) { return name; }
Register  _get_register_code  (char* source) {

    char cut_source      [strlen (source)] = "";
    char should_be_blank [1]               = "";

    sscanf (source, "%s %s", cut_source, should_be_blank);
    if (!isblank (should_be_blank)) {
    
        return UNKNOWN_REG;
    }

    #include "headers/reg.h"
    return UNKNOWN_REG;
}
#undef DEF_REG


Argument_and_mode  _parse_args_in_brackets  (char* argument) {

    assert (argument);


    bool memory_bite = 0;
    if (argument [0] == '[') memory_bite = 1;


    size_t i = 1;
    bool found_plus;
    for ( ; i < strlen (argument); i++) {
    
        if (argument [i] == '+') {

            found_plus = true;
            break;
        }
    }

    if (!found_plus) {

        Argument_and_mode parse_solo = _parse_solo_in_brackets (argument);
        parse_solo.command_mode |= (Command_mode) (memory_bite << 1);
        return parse_solo;
    }


    char     first_argument   [strlen (argument)] = "";
    char     should_be_blank1 [1]                 = "";

    char     second_argument  [strlen (argument)] = "";
    char     should_be_blank2 [1]                 = "";


    argument [i] = '\0';
    sscanf (argument + 1,     "%s",  first_argument);
    sscanf (argument + i + 1, "%s", second_argument);
        size_t second_argument_len = strlen(second_argument);
        if (second_argument [second_argument_len - 1] == ']' || second_argument [second_argument_len - 1] == ')') {

            second_argument [second_argument_len - 1] = '\0';
        }
    argument [i] = '+';


    Register register_code1 = _get_register_code  (first_argument);
    Register register_code2 = _get_register_code (second_argument);


    Argument first_const  = NAN;
    Argument second_const = NAN;
    sscanf ( first_argument, "%lf %s",  &first_const, should_be_blank1);
    sscanf (second_argument, "%lf %s", &second_const, should_be_blank2);
    if (!isnan (first_const) && isblank (should_be_blank1) && register_code2) {

        return { first_const,  (Command_mode) (((register_code2 << 2) + 1) | (memory_bite << 1)) };
    }

    if (!isnan (second_const) && isblank (should_be_blank2) && register_code1) {

        return { second_const, (Command_mode) (((register_code1 << 2) + 1) | (memory_bite << 1)) };
    }


    LOG_ERROR (BAD_ARGS); return {NAN, 0};
}


Argument_and_mode _parse_solo_in_brackets (char* argument) {

    assert (argument);


    char  first_bracket = argument [0];

    char second_bracket = 0;
    switch (first_bracket) {

        case '(':
            second_bracket = ')';
            break;
        case '[':
            second_bracket = ']';
            break;
        default:
            LOG_ERROR (BAD_ARGS); return {NAN, 0};
    }

    bool  found_bracket = false;
    size_t i            = 0;
    for ( ; i < strlen (argument); i++) {
    
        if (argument [i] == second_bracket) {
        
            found_bracket = true;
            break;
        }
    }

    char should_be_blank [1] = "";
    sscanf (argument + i + 1, "%s", should_be_blank);
    if (!found_bracket || !isblank (should_be_blank)) { LOG_ERROR (BAD_ARGS); return {NAN, 0}; }


    char     inner_argument [strlen (argument)] = "";

    argument [i] = '\0';
    sscanf (argument + 1, "%s", inner_argument);
    argument [i] = second_bracket;


    Register register_code = _get_register_code (inner_argument);


    Argument const_arg = NAN;
    sscanf (inner_argument, "%lf %s",  &const_arg,  should_be_blank);
    if (!isnan (const_arg) && isblank (should_be_blank)) {

        return { const_arg, 1};
    }

    if (register_code) {

        return { NAN, (Command_mode) (register_code << 2) };
    }


    LOG_ERROR (BAD_ARGS); return {NAN, 0};
}

