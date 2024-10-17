#ifndef ASSEMBLER_FUNCS_H
#define ASSEMBLER_FUNCS_H

#include <stdio.h>

void asm_code_read(const char path[], unsigned long long *return_err);

bool asm_getline(FILE* stream, char line[], const size_t n);

#endif // ASSEMBLER_FUNCS_H