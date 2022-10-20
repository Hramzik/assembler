#include <stdio.h>
#include <string.h>
#include "proc.cpp"


int main () {

    Processor processor;
    initialize_processor (&processor, 100, 100);
    for (size_t i = 0; i < 4; i++) {
    
        printf ("%p\n", _get_register_adress (&processor, (Argument) i));
    }
}