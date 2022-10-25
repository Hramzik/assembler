

#include "headers/generate.hpp"
#include "lib/logs.hpp"
#include "lib/onegin.hpp"


Return_code generate (const char* source_name, const char* dest_name) {

    if (!source_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }

    FILE* source = fopen (source_name, "r");
    FILE* dest   = fopen (dest_name,   "w");
    if (!source || !dest) { LOG_ERROR (FILE_ERR); printf ("can't open file %s", source_name); return FILE_ERR; }


    Text* source_lines  = initialize_text (source_name); //


    fprintf (dest, "setoutbuf\n");


    size_t simbol_num = 0;
    size_t g          = 0;
    for (size_t i = 0; i < source_lines->num_lines; i++) {

        if (is_split (source_lines->lines[i].ptr)) { 

            fprintf (dest, "FLUSHOUT\n");
            fprintf (dest, "sleep %lf\n", 0.5);


            for (g = 0; g < 40; g++) {
            
                fprintf (dest, "push %d\n", '\n');
                fprintf (dest, "pop [%zd]\n", g);
            }


            fprintf (dest, "show  %zd\n", g);
            continue;
        }


        simbol_num = 0;
        for (g = 0; g < strlen (source_lines->lines[i].ptr); g++) {

            fprintf (dest, "push %d\n", source_lines->lines[i].ptr[g]);
            fprintf (dest, "pop [%zd]\n", simbol_num);
            simbol_num++;
        }

        if ( i < (source_lines->num_lines - 1) && !is_split (source_lines->lines[i+1].ptr)) {

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