#include <stdio.h>
#include <string.h>
#include "asm.cpp"


int main () {

    char a [100] = "";
    while (scanf ("%s", a)) {
        printf ("%d\n", get_register_code (a));
    }
}