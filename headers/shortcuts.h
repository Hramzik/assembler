#ifndef ASSEMBLER_SHORTCUTS_H_INCLUDED
#define ASSEMBLER_SHORTCUTS_H_INCLUDED









#define SRC                source_lines->lines[line_ind].ptr
#define BF                 &bytes_filled
#define IND                command_ind
#define PUSH(stack, value) try (stack_push (stack, value))
#define POP(stack)         stack_pop(stack).value //checkreturn
#define STACK              processor.stack
#define F_C_STACK          processor.function_call_stack
#define IP                 processor.ip
#define CODE               processor.code
#define DK(a, b)           double_compare (a, b)









#endif