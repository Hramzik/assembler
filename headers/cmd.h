#include "shortcuts.h"

//       name       code args mode asm proc
DEF_CMD (HALT,       1,   0,   0,   {;},
        { IP = 99999;
        break; })

DEF_CMD (OUT,        2,   0,   0,   {;},
        { fprintf (out, "%lf\n", POP (STACK));
        break; })

DEF_CMD (PUSH,       3,   1,   1,   {;},
        { try (_processor_case_push (&processor));
        break; })

DEF_CMD (POP,        4,   1,   1,   {;},
        { try (_processor_case_pop (&processor));
        break; })

DEF_CMD (ADD,        5,   0,   0,   {;},
        { PUSH (STACK, POP (STACK) + POP (STACK));
        break; })

DEF_CMD (SUB,        6,   0,   0,   {;},
        { Argument first = POP (STACK);
        Argument second = POP (STACK); 
        PUSH (STACK, - first + second);
        break; })

DEF_CMD (MUL,       7,   0,   0,   {;},
        { PUSH (STACK, POP (STACK) * POP (STACK));
        break; })

DEF_CMD (DIV,        8,   0,   0,   {;},
        { Argument first = POP (STACK);
        Argument second = POP (STACK); 
        PUSH (STACK, 1 / first * second);
        break; })

DEF_CMD (JUMP,       9,   1,   0,
    { _asm_case_jumping_cmd (SRC, label_list, commands, BF, JUMP); break; },
        { IP = (size_t) round ( * (Argument*) ( (char*) CODE + IP));
        break; })

DEF_CMD (DUP, 10,   0,   0,   {;},
        { argument = POP (STACK);
        PUSH (STACK, argument);
        PUSH (STACK, argument);
        break; })

DEF_CMD (CALL,      11,   1,   0,
    { _asm_case_jumping_cmd (SRC, label_list, commands, BF, CALL); break; },
        { PUSH (F_C_STACK, (double) IP + Argument_size);
        IP = (size_t) round ( * (Argument*) ( (char*) CODE + IP));
        break; })

DEF_CMD (RET,    12,   0,   0,   {;},
        { IP = (size_t) POP (F_C_STACK);
        break; })

#define DEF_JMP(name, num, test)\
DEF_CMD (name,     num,   1,   0,\
    { _asm_case_jumping_cmd (SRC, label_list, commands, BF, name); break; },\
        { if (DK (POP (STACK), POP (STACK)) test 0) {\
            IP = (size_t) round ( * (Argument*) ( (char*) CODE + IP));\
        }\
        else {\
            IP += Argument_size;\
        }\
        break; })
#include "jump.h"
#undef DEF_JMP

