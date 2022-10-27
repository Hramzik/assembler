

#include "../headers/disasm.hpp"


int main ()
{
    Return_code return_code = disassembler ();
    printf ("%d", return_code);


    return 0;
}

