#include <stdio.h>
#include <string.h>
#include "lib/logs.hpp"

#define DEF_CMD(name, code, args, mode, asm, ...) printf (#asm); printf ("\n");

int main () {

    #include "headers/cmd.h"

}