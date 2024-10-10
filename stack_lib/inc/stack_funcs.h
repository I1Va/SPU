#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include <stdio.h>
#include <string.h>

#include "error_processing.h"
#include "general.h"

typedef unsigned long long canary_elem_t;

const canary_elem_t CANARY_VALUE = 0xC0FFEEABACABABAC;
const size_t CANARY_NMEMB = sizeof(canary_elem_t);

const stack_elem_t POISON_STACK_VALUE = 0x0BAD0DED; // FIXME: использовать при resize

struct canaries_t {
    const canary_elem_t *canary_left_ptr;
    const canary_elem_t *canary_mid_ptr;
    const canary_elem_t *canary_right_ptr;
    const canary_elem_t *canary_stk_left_ptr;
    const canary_elem_t *canary_stk_right_ptr;
};

struct hash_t {
    void *left_ptr;
    void *right_ptr;

    unsigned long long hash_value;
    unsigned long long hash_mult;
};

struct stack_t {
    //TODO: можно сделать указатель на дубль тут

    ON_HASH(hash_t HASH_STACK_STRUCT;)
    ON_HASH(hash_t HASH_STACK_DATA;)

    ON_CANARY(const unsigned long long CANARY_LEFT = CANARY_VALUE;)

    ON_CANARY(canaries_t CANARIES;)

    size_t size;
    size_t capacity;

    ON_CANARY(const unsigned long long CANARY_MID = CANARY_VALUE;)

    stack_elem_t *data;

    const char *born_file;
    int born_line;
    const char *born_func;

    ON_CANARY(const unsigned long long CANARY_RIGHT = CANARY_VALUE;)
};

const size_t resize_up_coeff = 2;
const size_t resize_down_check_coeff = 4;
const size_t resize_down_coeff = 2;

const size_t dump_output_sz = 10;

ON_CANARY(
    void stack_end_canary_assign(stack_t *stk, const canary_elem_t value);

    canary_elem_t *stack_end_canary_getptr(stack_t *stk);
)

ON_HASH(
    void HASH_init(hash_t *HASH);

    unsigned long long HASH_get(hash_t *HASH);

    void HASH_rebuild_ptr(hash_t *HASH, char *left_ptr, char *right_ptr);

    void HASH_rebuild_value(hash_t *HASH);

    bool HASH_check(hash_t *HASH);
)

void stack_init(stack_t *stk, const size_t size, unsigned long long *return_err, const char born_file[] = NULL, const int born_line = 0, const char born_func[] = NULL);

void stack_destroy(stack_t *stk);

unsigned long long verify(stack_t *stk, unsigned long long *return_err, const char file_name[], const char func_name[], const int line_idx);

void stack_memset(stack_elem_t *data, const stack_elem_t value, const size_t n);

void resize(stack_t *stk, unsigned long long *return_err);

void stack_push(stack_t *stk, stack_elem_t value, unsigned long long *return_err);

stack_elem_t stack_pop(stack_t *stk, unsigned long long *return_err);

stack_elem_t stack_get_last(stack_t *stk, unsigned long long *return_err);

#endif // STACK_FUNCS_H