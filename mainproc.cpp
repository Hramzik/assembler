

#include "headers/proc.hpp"


int main (int argc, char** argv)
{
    Return_code return_code = SUCCESS;
    if (isinteractive (argc - 1, argv + 1)) { return_code = processor_run_i (); }
    else                                    { return_code = processor_run   (); }
    printf ("%d", return_code);


    return 0;
}

