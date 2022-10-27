

#include "../headers/generate.hpp"
#include "../lib/logs.hpp"
#include "../lib/onegin.hpp"


Return_code generate (const char* source_name, const char* dest_name) {

    if (!source_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }

    FILE* source = fopen (source_name, "r");
    FILE* dest   = fopen (dest_name,   "w");
    if (!source || !dest) { LOG_ERROR (FILE_ERR); printf ("can't open file %s", source_name); return FILE_ERR; }


    Text* source_lines  = initialize_text (source_name); //


    fprintf (dest, "setoutbuf\n");


    size_t simbol_num = 0;
    size_t line_len   = 0;
    size_t i          = 0;
    for (size_t line_num = 0; line_num < source_lines->num_lines; line_num++) {

        if (is_split (source_lines->lines[line_num].ptr)) { 

            fprintf (dest, "FLUSHOUT\n");
            fprintf (dest, "sleep %lf\n", 0.5);


            for (i = 0; i < 40; i++) {
            
                fprintf (dest, "push %d\n", '\n');
                fprintf (dest, "pop [%zd]\n", i);
            }


            fprintf (dest, "show  %zd\n", i);
            continue;
        }


        simbol_num = 0;
        line_len = strlen (source_lines->lines[line_num].ptr);
        for (i = 0; i < line_len; i++) {

            fprintf (dest, "push %d\n", source_lines->lines[line_num].ptr[i]);
            fprintf (dest, "pop [%zd]\n", simbol_num);
            simbol_num++;
        }

        if ( line_num < (source_lines->num_lines - 1) && !is_split (source_lines->lines[line_num+1].ptr)) {

            fprintf (dest, "push %d\n", '\n');
            fprintf (dest, "pop [%zd]\n",  simbol_num);
            simbol_num += 1; // for '\n'
        }


        fprintf (dest, "show %zd\n\n\n", simbol_num);
    }

    fprintf (dest, "halt");


    fclose (source);
    fclose (dest);

    return SUCCESS;
}