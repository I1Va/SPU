#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>

#include "proc_err.h"

enum asm_coms_nums
{
    PUSH_COM = 1,
    POP_COM = 2,
    IN_COM = 3,
    OUT_COM = 4,
    ADD_COM = 5,
    SUB_COM = 6,
    MULT_COM = 7,
    PUSHR_COM = 8,
    POPR_COM = 9,
    JMP_COM = 10,
    LABEL_COM = 11,
    JA_COM = 12,
    JAE_COM = 13,
    JB_COM = 14,
    JBE_COM = 15,
    JE_COM = 16,
    JNE_COM = 17,
    CALL_COM = 18,
    RET_COM = 19,


    HLT_COM = -1,
    UNKNOWN_COM = -2,
};

size_t bin_code_read(const char path[], int code[], proc_err *return_err);

void execute_code(int code[], proc_err *return_err);

#endif // PROCESSOR_FUNC_H