#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include "general.h"
#include "error_processing.h"
#include "output.h"

typedef long long stack_elem_t;
#include "stack_funcs.h"

ON_HASH(

    void HASH_init(hash_t *HASH) {
        HASH->left_ptr = NULL;
        HASH->right_ptr = NULL;
        HASH->hash_mult = 257;
        HASH->hash_value = 0;
    }

    unsigned long long HASH_get(hash_t *HASH) {
        unsigned char *left_ptr = (unsigned char *) HASH->left_ptr;
        unsigned char *right_ptr = (unsigned char *) HASH->right_ptr;
        unsigned long long hash_value = 0;

        while (left_ptr < right_ptr) {
            hash_value += *left_ptr++ * HASH->hash_mult; // использую переполнение
        }

        return hash_value;
    }

    void HASH_rebuild_ptr(hash_t *HASH, char *left_ptr, char *right_ptr) {
        HASH->left_ptr = left_ptr;
        HASH->right_ptr = right_ptr;
    }

    void HASH_rebuild_value(hash_t *HASH) {
        HASH->hash_value = HASH_get(HASH);
    }

    bool HASH_check(hash_t *HASH) {
        return HASH->hash_value == HASH_get(HASH);
    }
)

ON_CANARY(
    canary_elem_t *stack_end_canary_getptr(stack_t *stk) {
        size_t stack_byte_size = stk->capacity * sizeof(stack_elem_t);
        char *canary_ptr = (char *) stk->data + stack_byte_size;
        unsigned long long alignment = (unsigned long long) canary_ptr % 8;
        canary_elem_t *canary_ptr_align = (canary_elem_t *) (canary_ptr + alignment);
    }

    void stack_end_canary_assign(stack_t *stk, const canary_elem_t value) {
        canary_elem_t *canary_ptr = stack_end_canary_getptr(stk);
        *canary_ptr = value;
    }

    const size_t LEFT_CANARY_INDENT = (CANARY_NMEMB + sizeof(stack_elem_t) - 1) / sizeof(stack_elem_t);
)

void stack_memset(stack_elem_t *data, const stack_elem_t value, const size_t n) { // FIXME: можно использовать memcpy (не жалуется на выравнивание)
    assert(data != NULL);

    for (size_t i = 0; i < n; i++) {
        *(data + i) = value;
    }
}

unsigned long long verify(stack_t *stk, unsigned long long *return_err, const char file_name[], const char func_name[], const int line_idx) {
    ON_CANARY(
        if (*stk->CANARIES.canary_left_ptr != CANARY_VALUE) {
            *return_err |= ERR_CANARY_LEFT;
            MY_ASSERT(*return_err, abort())
        }
        if (*stk->CANARIES.canary_mid_ptr != CANARY_VALUE) { // FIXME: что за canary_mid_ptr
            *return_err |= ERR_CANARY_MID;
            MY_ASSERT(*return_err, abort())
        }
        if (*stk->CANARIES.canary_right_ptr != CANARY_VALUE) {
            *return_err |= ERR_CANARY_RIGHT;
            MY_ASSERT(*return_err, abort())
        }
        if (*stk->CANARIES.canary_stk_left_ptr != CANARY_VALUE) {
            *return_err |= ERR_CANARY_STK_LEFT;
            MY_ASSERT(*return_err, abort())
        }
        if (*stk->CANARIES.canary_stk_right_ptr != CANARY_VALUE) {
            *return_err |= ERR_CANARY_STK_RIGHT;
            MY_ASSERT(*return_err, abort())
        }
    )

    ON_HASH(
        if (!HASH_check(&stk->HASH_STACK_STRUCT)) {
            HASH_print(&stk->HASH_STACK_STRUCT);
            *return_err |= ERR_HASH_STACK_STRUCT_MISMATCH;
            MY_ASSERT(*return_err, return *return_err)
        }
        if (!HASH_check(&stk->HASH_STACK_DATA)) {
            HASH_print(&stk->HASH_STACK_DATA);
            *return_err |= ERR_HASH_STACK_DATA_MISMATCH;
            MY_ASSERT(*return_err, return *return_err)
        }
    )


    if (stk == NULL) {
        *return_err |= ERR_STACK_NULLPTR;
        goto dump_mark;
    }

    if (stk->data == NULL) {
        *return_err |= ERR_STACK_CONT_NULLPTR;
        goto dump_mark;
    }

    if (stk->size > stk->capacity) { // TODO: проверку на то, что после size всё в POISON
        *return_err |= ERR_STACK_OVERFLOW;
        goto dump_mark;
    }

    return ERR_OK;

    dump_mark:

    DEBUG_ERROR(*return_err);
    dump(stk, file_name, func_name, line_idx);

    return *return_err;
}

void stack_init(stack_t *stk, const size_t size, unsigned long long *return_err, const char born_file[], const int born_line, const char born_func[]) {
    assert(return_err != NULL);

    if (stk == NULL) {
        *return_err |= ERR_CALLOC;
        DEBUG_ERROR(ERR_CALLOC)
        CLEAR_MEMORY(exit_mark)
    }

    ON_CANARY(stk->CANARIES.canary_left_ptr = &stk->CANARY_LEFT;)
    ON_CANARY(stk->CANARIES.canary_mid_ptr = &stk->CANARY_MID;)
    ON_CANARY(stk->CANARIES.canary_right_ptr = &stk->CANARY_RIGHT;)

    stk->size = 0;
    stk->capacity = size;



    NOT_ON_CANARY(stk->data = (stack_elem_t *) calloc(stk->capacity, sizeof(stack_elem_t));)
    ON_CANARY    (stk->data = (stack_elem_t *) calloc(stk->capacity * sizeof(stack_elem_t) + 2 * CANARY_NMEMB + LEFT_CANARY_INDENT * sizeof(stack_elem_t), sizeof(char));)

    if (stk->data == NULL) {
        *return_err |= ERR_CALLOC;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }

    ON_CANARY(
        stk->CANARIES.canary_stk_left_ptr = (canary_elem_t *) stk->data;
        *(canary_elem_t *) stk->data = CANARY_VALUE;
        stk->data += LEFT_CANARY_INDENT;
        stack_end_canary_assign(stk, CANARY_VALUE);
        stk->CANARIES.canary_stk_right_ptr = stack_end_canary_getptr(stk);
    )
    stack_memset(stk->data, POISON_STACK_VALUE, stk->capacity);

    stk->born_file = born_file;
    stk->born_line = born_line;
    stk->born_func = born_func;

    ON_HASH(
        stk->HASH_STACK_STRUCT = {}; HASH_init(&stk->HASH_STACK_STRUCT);
        stk->HASH_STACK_DATA = {}; HASH_init(&stk->HASH_STACK_DATA);
        HASH_rebuild_ptr(&stk->HASH_STACK_STRUCT, (char *) &stk->size, (char *) (&stk->born_func + 1));
        HASH_rebuild_ptr(&stk->HASH_STACK_DATA, (char *) stk->data, ((char *) stk->data + stk->capacity * sizeof(stack_elem_t)));

        HASH_rebuild_value(&stk->HASH_STACK_STRUCT);
        HASH_rebuild_value(&stk->HASH_STACK_DATA);
    )


    return;

    exit_mark:
    ON_CANARY(
        if (stk->data != NULL) {
            stk->data -= LEFT_CANARY_INDENT;
        }
    )

    if (stk->data != NULL) {
        FREE(stk->data);
    }

    return;
}

void stack_destroy(stack_t *stk) {
    if (stk == NULL) {
        return;
    }

    ON_CANARY(
        if (stk->data != NULL) {
            stk->data -= LEFT_CANARY_INDENT;
        }
    )

    FREE(stk->data);
}

void resize(stack_t *stk, unsigned long long *return_err) {
    assert(stk != NULL);
    assert(return_err != NULL);

    bool resize_up_state = false;
    bool unit_length_state = false;
    size_t new_byte_size = 0;

    if (stk->capacity == 0) {
        ON_CANARY(stack_end_canary_assign(stk, 0));
        stk->capacity++;
        unit_length_state = true;
    } else if (stk->size >= stk->capacity) {
        ON_CANARY(stack_end_canary_assign(stk, 0));
        stk->capacity *= resize_up_coeff;
        resize_up_state = true;
    } else if (stk->size <= stk->capacity / resize_down_check_coeff) {
        ON_CANARY(stack_end_canary_assign(stk, 0));
        stk->capacity /= resize_down_coeff;
    } else {
        return;
    }

    NOT_ON_CANARY(
        stk->data = (stack_elem_t *) realloc(stk->data, stk->capacity * sizeof(stack_elem_t));
        new_byte_size = stk->capacity * sizeof(stack_elem_t);
    )

    ON_CANARY(
        stk->data -= LEFT_CANARY_INDENT;
        stk->data = (stack_elem_t *) realloc(stk->data, stk->capacity * sizeof(stack_elem_t) + 2 * CANARY_NMEMB + LEFT_CANARY_INDENT * sizeof(stack_elem_t));
        stk->data += LEFT_CANARY_INDENT;
        new_byte_size = stk->capacity * sizeof(stack_elem_t) + 2 * CANARY_NMEMB + LEFT_CANARY_INDENT * sizeof(stack_elem_t);
    )

    if (stk->data == NULL) {
        *return_err |= ERR_REALLOC;
        DEBUG_ERROR(*return_err);
        return;
    }


    if (resize_up_state) {
        size_t old_capacity = stk->capacity / resize_up_coeff;
        stack_memset(stk->data + old_capacity, POISON_STACK_VALUE, old_capacity * (resize_up_coeff - 1));
    }
    if (unit_length_state) {
        stack_memset(stk->data, POISON_STACK_VALUE, 1);
    }

    ON_HASH(
        HASH_rebuild_ptr(&stk->HASH_STACK_DATA, (char *) stk->data, (char *) stk->data + new_byte_size);
    )
    ON_CANARY(
        stack_end_canary_assign(stk, CANARY_VALUE);
        stk->CANARIES.canary_stk_right_ptr = stack_end_canary_getptr(stk);
        stk->CANARIES.canary_stk_left_ptr = (canary_elem_t *) (stk->data - LEFT_CANARY_INDENT);
    )
}

void stack_push(stack_t *stk, stack_elem_t value, unsigned long long *return_err) {
    assert(return_err != NULL);

    VERIFY(stk, return_err, return)

    unsigned long long last_err = ERR_OK;
    resize(stk, &last_err);
    if (last_err != ERR_OK) {
        *return_err |= last_err;
        DEBUG_ERROR(last_err);
        return;
    }

    stk->data[stk->size++] = value;

    ON_HASH(HASH_rebuild_value(&stk->HASH_STACK_STRUCT);)
    ON_HASH(HASH_rebuild_value(&stk->HASH_STACK_DATA);)

    VERIFY(stk, return_err, return)
}

stack_elem_t stack_pop(stack_t *stk, unsigned long long *return_err) {
    assert(return_err != NULL);

    unsigned long long last_err = ERR_OK;

    VERIFY(stk, return_err, CLEAR_MEMORY(exit_mark))

    stack_elem_t last_elem = POISON_STACK_VALUE;

    if (stk->size == 0) {
        *return_err |= ERR_STACK_POP;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }

    resize(stk, &last_err);
    if (last_err != ERR_OK) {
        *return_err |= last_err;
        DEBUG_ERROR(last_err)
        CLEAR_MEMORY(exit_mark)
    }

    last_elem = stk->data[--stk->size];
    stk->data[stk->size] = POISON_STACK_VALUE;

    ON_HASH(HASH_rebuild_value(&stk->HASH_STACK_STRUCT);)
    ON_HASH(HASH_rebuild_value(&stk->HASH_STACK_DATA);)

    return last_elem;

    exit_mark:

    return POISON_STACK_VALUE;
}

stack_elem_t stack_get_last(stack_t *stk, unsigned long long *return_err) {
    assert(stk != NULL);

    VERIFY(stk, return_err, )

    if (stk->size == 0) {
        *return_err |= ERR_STACK_LAST_ELEM;
        DEBUG_ERROR(*return_err)
        return POISON_STACK_VALUE;
    }

    return stk->data[stk->size - 1];
}
