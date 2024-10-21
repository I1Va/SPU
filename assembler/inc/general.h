#ifndef GENERAL_H
#define GENERAL_H

#include <cstddef>
#include <time.h>
#include "./../general_output/inc/general_output.h"

#define FREE(ptr)              \
    do {                       \
        free(ptr); ptr = NULL; \
    } while(0);


/*
#define FREE(ptr)                \
    ({                           \
        typeof(ptr) ptr__ = ptr; \
        free(ptr);               \
        ptr = NULL;              \
        ptr__                    \
    })
*/

enum RETURN_STATES {
    ReturnErrOR  = -1,
    RETURN_FALSE =  0,
    RETURN_TRUE  =  1,
};

#define ASSERT(error, end_instruction)                                                                                    \
    fprintf_red(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_descr(error));    \
    end_instruction;

#define CLEAR_MEMORY(mark) goto mark;

#ifdef _DEBUG
    #define debug(str_, ...) fprintf_red(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define DEBUG_ERROR(code) fprintf_red(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_bit_descr(code));
    #define ON_DEBUG(...) __VA_ARGS__
    #define VERIFY(stk, return_err, exit_instruction)                                         \
        {                                                                                             \
            if (verify(stk, return_err, __FILE__, __PRETTY_FUNCTION__, __LINE__) != ERR_OK) { \
                exit_instruction;                                                                     \
            }                                                                                          \
        }
    #define STACK_INIT(stk, size, return_err) stack_init(stk, size, return_err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    #define MY_ASSERT(err_code, exit_instruction) {DEBUG_ERROR(err_code); exit_instruction;};

#else
    #define debug(str_, ...) ;
    #define DEBUG_ERROR(code) ;
    #define ON_DEBUG(...)
    #define VERIFY(stk, return_err, exit_instruction) ;
    #define STACK_INIT(stk, size, return_err) stack_init(stk, size, return_err);
    #define MY_ASSERT(err_code, exit_instruction) ;

#endif // _DEBUG

#ifdef _CANARY
    #define ON_CANARY(...) __VA_ARGS__
    #define NOT_ON_CANARY(...)
#else
    #define ON_CANARY(...)
    #define NOT_ON_CANARY(...) __VA_ARGS__
#endif // _CANARY

#ifdef _HASH
    #define ON_HASH(...) __VA_ARGS__
    #define NOT_ON_HASH(...)
#else
    #define NOT_ON_HASH(...) __VA_ARGS__
    #define ON_HASH(...)
#endif // _HASH


#define DUMP(stk) dump(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__);

#endif // GENERAL
