#ifndef ASSEMBLER_FUNCS_H
#define ASSEMBLER_FUNCS_H

#include <stdio.h>
#include <limits.h>

#include "error_processing.h"

const size_t max_asm_line_sz = 128;
const size_t max_asm_command_size = 32;
const size_t max_asm_commands_n = 128;
const size_t label_list_max_sz = 16;
const size_t max_label_name_sz = 32;
const size_t fix_up_table_max_sz = 16;
const size_t max_com_sz = 16;
const size_t register_max_sz = 8;
const size_t max_bin_code_sz = 256;

const int MASK_MEM  = 1 << 8;
const int MASK_REG  = 1 << 7;
const int MASK_IMMC = 1 << 6;
const int filter_mask = INT_MAX & ~MASK_IMMC & ~MASK_MEM & ~MASK_REG;

struct bin_code_t {
    int code[max_bin_code_sz] = {};
    size_t bin_idx = 0;
};

struct asm_code_t {
    char code[max_asm_commands_n][max_asm_command_size] = {};
    size_t asm_idx = 0;
    size_t code_sz = max_asm_commands_n;
};

asm_code_t asm_code_init();

bin_code_t bin_code_init();

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

asm_code_t asm_code_read(const char path[], asm_err *return_err);

bool asm_getline(FILE* stream, char line[], const size_t n);

void asm_commands_translate(bin_code_t *bin_code, asm_code_t *asm_code, asm_err *return_err);

void fprint_asm_commands_list(FILE *stream, const char asm_commands[][max_asm_command_size], const size_t n_coms);

void bin_code_write(const char path[], bin_code_t bin_code, asm_err *return_err);

#endif // ASSEMBLER_FUNCS_H