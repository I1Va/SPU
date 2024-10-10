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
        BIT_DESCR_(ERR_STACK_NULLPTR)
        BIT_DESCR_(ERR_STACK_CONT_NULLPTR)
        BIT_DESCR_(ERR_STACK_OVERFLOW)
        BIT_DESCR_(ERR_STACK_POP)
        BIT_DESCR_(ERR_REALLOC)
        BIT_DESCR_(ERR_CANARY_LEFT)
        BIT_DESCR_(ERR_CANARY_MID)
        BIT_DESCR_(ERR_CANARY_RIGHT)
        BIT_DESCR_(ERR_CANARY_STK_RIGHT)
        BIT_DESCR_(ERR_HASH_STACK_DATA_MISMATCH)
        BIT_DESCR_(ERR_CANARY_STK_LEFT)
        BIT_DESCR_(ERR_SYSTEM)
        BIT_DESCR_(ERR_STACK_LAST_ELEM)
        BIT_DESCR_(ERR_HASH_STACK_STRUCT_MISMATCH)

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
    DESCR_(err, ERR_STACK_NULLPTR)
    DESCR_(err, ERR_STACK_CONT_NULLPTR)
    DESCR_(err, ERR_STACK_OVERFLOW)
    DESCR_(err, ERR_STACK_POP)
    DESCR_(err, ERR_REALLOC)

    DESCR_(err, ERR_CANARY_MID)
    DESCR_(err, ERR_CANARY_RIGHT)
    DESCR_(err, ERR_CANARY_STK_RIGHT)
    DESCR_(err, ERR_CANARY_STK_LEFT)

    DESCR_(err, ERR_HASH_STACK_DATA_MISMATCH)
    DESCR_(err, ERR_HASH_STACK_STRUCT_MISMATCH)
    DESCR_(err, ERR_STACK_LAST_ELEM)

    DESCR_(err, ERR_SYSTEM)

    if (err == ERR_OK) {
        return "ERR_OK";
    }

    return "STRANGE ERROR!";
}

#undef DESCR_