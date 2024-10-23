#ifndef ERROR_PROCESSING_H
#define ERROR_PROCESSING_H

#include <stdio.h>

enum asm_err
{
    ASM_ERR_OK                         = 0,
    ASM_ERR_NULLPTR                    = 1 << 0,
    ASM_ERR_CALLOC                     = 1 << 1,
    ASM_ERR_MEM                        = 1 << 2,
    ASM_ERR_UNKNOWN                    = 1 << 3,
    ASM_ERR_STAT                       = 1 << 4,
    ASM_ERR_INPUT_DATA                 = 1 << 5,
    ASM_ERR_FILE_OPEN                  = 1 << 6,
    ASM_ERR_FILE_CLOSE                 = 1 << 7,
    ASM_ERR_ARGS                       = 1 << 8,
    ASM_ERR_WRONG_COEF                 = 1 << 9,
    ASM_ERR_INIT                       = 1 << 10,
    ASM_ERR_SYNTAX                     = 1 << 11,
    ASM_ERR_INVALID_LABEL              = 1 << 12,
};

const char *get_descr(unsigned long long err);

int fprintf_abort(const char file_name[], const int line, const char function_name[], const char error_descr[]);

const char *get_bit_descr(unsigned long long err);

void asm_add_err(asm_err *src, asm_err add);

// asm_err fprintf_return(const char file_name[], const int line, const char function_name[], const asm_err code);

// #ifndef _REALIZE
//     #define asserted(code) ? 1 : printf_red(stderr, "{%s}; [%s: %d]; ASM_ERR_info: %s\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_descr(code));
// #else
//     #define asserted(code) ;
// #endif // _REALIZE

#endif // ERROR_PROCESSING_H
