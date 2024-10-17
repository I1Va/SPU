#ifndef ASSEMBLER_FUNCS_H
#define ASSEMBLER_FUNCS_H

#include <stdio.h>

const size_t max_asm_line_sz = 128;
const size_t max_asm_command_size = 32;
const size_t max_asm_commands_n = 128;

enum asm_coms_nums
{
    PUSH_COM = 1,
    POP_COM = 2,
};

size_t asm_code_read(char asm_commands[][max_asm_command_size], const char path[], unsigned long long *return_err);

bool asm_getline(FILE* stream, char line[], const size_t n);

void asm_commands_translate(int bin_code[], char asm_commands[][max_asm_command_size],
        const size_t asm_commands_n, unsigned long long *return_err);

void fprint_asm_commands_list(FILE *stream, const char asm_commands[][max_asm_command_size], const size_t n_coms);

void bin_code_write(const char path[], int bin_code[], const size_t n_bin_coms, unsigned long long *return_err);

#endif // ASSEMBLER_FUNCS_H