#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>

enum asm_coms_nums
{
    PUSH_COM = 1,
    POP_COM = 2,
    IN_COM = 3,
    OUT_COM = 4,
    ADD_COM = 5,
    SUB_COM = 6,
    MULT_COM = 7,

    HLT_COM = -1,
};

size_t bin_code_read(const char path[], int code[], unsigned long long *return_err);

void execute_code(int code[], unsigned long long *return_err);

#endif // PROCESSOR_FUNC_H