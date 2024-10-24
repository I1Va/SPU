#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <limits.h>
#include "proc_err.h"

const int MASK_MEM  = 1 << 8;
const int MASK_REG  = 1 << 7;
const int MASK_IMMC = 1 << 6;

const int filter_mask = INT_MAX & ~MASK_IMMC & ~MASK_MEM & ~MASK_REG;

enum asm_coms_nums
{
    NULL_COM = 0,

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

    UPUSH_COM = 20,
    UPOP_COM = 21,
    OUTC_COM = 22,


    HLT_COM = INT_MAX & filter_mask,
    UNKNOWN_COM = 31,
};

size_t bin_code_read(const char path[], int code[], proc_err *return_err);

void execute_code(int code[], proc_err *return_err);

size_t get_bin_code_real_sz(int bin_code[], const size_t n);

#endif // PROCESSOR_FUNC_H