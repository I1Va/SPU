#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "error_processing.h"

const char *get_bit_descr(unsigned long long err) {
    #define BIT_DESCR_(code) case code : return #code;
    switch (err) {
        BIT_DESCR_(ERR_OK)
        BIT_DESCR_(ERR_UNKNOWN)
        BIT_DESCR_(ERR_CALLOC)
        BIT_DESCR_(ERR_NULLPTR)
        BIT_DESCR_(ERR_STAT)
        BIT_DESCR_(ERR_INPUT_DATA)
        BIT_DESCR_(ERR_MEM)
        BIT_DESCR_(ERR_FILE_CLOSE)
        BIT_DESCR_(ERR_FILE_OPEN)
        BIT_DESCR_(ERR_ARGS)
        BIT_DESCR_(ERR_WRONG_COEF)
        BIT_DESCR_(ERR_INIT)
        BIT_DESCR_(ERR_SYNTAX)

        default: return "VERY STRANGE ERROR:(";
    }
    #undef BIT_DESCR_
}

const char *get_descr(unsigned long long err) {
    #define DESCR_(code, err)           \
    {                                   \
        if (code & err) {               \
            return #err;                \
        }                               \
    }                                   \

    DESCR_(err, ERR_NULLPTR)
    DESCR_(err, ERR_CALLOC)
    DESCR_(err, ERR_UNKNOWN)
    DESCR_(err, ERR_STAT)
    DESCR_(err, ERR_INPUT_DATA)
    DESCR_(err, ERR_FILE_OPEN)
    DESCR_(err, ERR_FILE_CLOSE)
    DESCR_(err, ERR_ARGS)
    DESCR_(err, ERR_WRONG_COEF)
    DESCR_(err, ERR_INIT)

    if (err == ERR_OK) {
        return "ERR_OK";
    }

    return "STRANGE ERROR!";
}

#undef DESCR_