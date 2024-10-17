#ifndef ERROR_PROCESSING_H
#define ERROR_PROCESSING_H

#include <stdio.h>

enum err_code
{
    ERR_OK                         = 0,
    ERR_NULLPTR                    = 1 << 0,
    ERR_CALLOC                     = 1 << 1,
    ERR_MEM                        = 1 << 2,
    ERR_UNKNOWN                    = 1 << 3,
    ERR_STAT                       = 1 << 4,
    ERR_INPUT_DATA                 = 1 << 5,
    ERR_FILE_OPEN                  = 1 << 6,
    ERR_FILE_CLOSE                 = 1 << 7,
    ERR_ARGS                       = 1 << 8,
    ERR_WRONG_COEF                 = 1 << 9,
    ERR_INIT                       = 1 << 10,
    ERR_SYNTAX                     = 1 << 11,
};

const char *get_descr(unsigned long long err);

int fprintf_abort(const char file_name[], const int line, const char function_name[], const char error_descr[]);

const char *get_bit_descr(unsigned long long err);

// err_code fprintf_return(const char file_name[], const int line, const char function_name[], const err_code code);

// #ifndef _REALIZE
//     #define asserted(code) ? 1 : printf_red(stderr, "{%s}; [%s: %d]; err_info: %s\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_descr(code));
// #else
//     #define asserted(code) ;
// #endif // _REALIZE

#endif // ERROR_PROCESSING_H
