

//       name       code args mode asm proc
DEF_CMD (HALT,       1,   0,   0,   -,   -)
DEF_CMD (OUT,        2,   0,   0,   -,   -)
DEF_CMD (PUSH,       3,   1,   1,   -,   -)
DEF_CMD (POP,        4,   1,   1,   -,   -)
DEF_CMD (ADD,        5,   0,   0,   -,   -)
DEF_CMD (SUBSTRACT,  6,   0,   0,   -,   -)
DEF_CMD (MULTIPLY,   7,   0,   0,   -,   -)
DEF_CMD (DIVIDE,     8,   0,   0,   -,   -)
DEF_CMD (JUMP,       9,   1,   0,
    { _asm_case_jump (source_lines->lines[line_ind].ptr, label_list, commands, &bytes_filled); }
    )
DEF_CMD (DUPLICATE, 10,   0,   0,   -,   -)
DEF_CMD (CALL,      11,   1,   0,
    { _asm_case_call (source_lines->lines[line_ind].ptr, label_list, commands, &bytes_filled, command_ind); },
    )
DEF_CMD (RETURN,    12,   0,   0,   -,   -)
DEF_CMD (JE,        13,   1,   0,   -,   -)
DEF_CMD (JNE,       14,   1,   0,   -,   -)
DEF_CMD (JB,        15,   1,   0,   -,   -)
DEF_CMD (JBE,       16,   1,   0,   -,   -)
DEF_CMD (JA,        17,   1,   0,   -,   -)
DEF_CMD (JAE,       18,   1,   0,   -,   -)

