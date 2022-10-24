

#include "headers/generate.hpp"
#include "lib/logs.hpp"
#include "lib/onegin.hpp"


Return_code generate (const char* source_name, const char* dest_name) {

    if (!source_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }

    FILE* source = fopen (source_name, "r");
    FILE* dest   = fopen (dest_name,   "w");
    if (!source || !dest) { LOG_ERROR (FILE_ERR); printf ("can't open file %s", source_name); return FILE_ERR; }


    Text* source_lines  = initialize_text (source_name); //
    size_t max_line_len = 0;
    for (size_t i = 0; i < source_lines->num_lines; i++) {

        if (strlen (source_lines->lines[i].ptr) > max_line_len) { max_line_len = strlen (source_lines->lines[i].ptr); }
    }


    size_t simbol_num = 0;
    for (size_t i = 0; i < source_lines->num_lines; i++) {

        simbol_num = 0;
        for (size_t g = 0; g < strlen (source_lines->lines[i].ptr); g++) {

            fprintf (dest, "push %d\n", source_lines->lines[i].ptr[g]);
            fprintf (dest, "pop [%zd]\n", simbol_num);
            simbol_num++;
        }

        /*for (size_t g = strlen(source_lines->lines[i].ptr); g < max_line_len; g++) {

            fprintf (dest, "push %d\n", ' ');
            fprintf (dest, "pop [%zd]\n", simbol_num);
            simbol_num++;
        }*/ // заполнение пробелами до максимума

        fprintf (dest, "push %d\n", '\n');
        fprintf (dest, "pop [%zd]\n",  simbol_num);

        fprintf (dest, "show %zd\n\n\n", simbol_num + 1); // for '\n'
    }

    fprintf (dest, "halt");


    fclose (source);
    fclose (dest);

    return SUCCESS;
}