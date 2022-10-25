

#include "headers/proc.hpp"


int main (int argc, char** argv)
{
    Return_code return_code = SUCCESS;
    if (isinteractive (argc - 1, argv + 1)) { return_code = processor_run_i (); }

    else if (argc < 2)                      { return_code = processor_run   (); }

    else                                    {

        char file_name [MAX_FILE_NAME_LEN + 1] = "work/";
        strcat                      (file_name, argv[1]);
        strcat                      (file_name, ".txt");
        return_code = processor_run (file_name);
    }


    printf ("%d", return_code);


    return 0;
}

