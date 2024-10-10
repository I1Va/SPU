#include <assert.h>
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "output.h"
#include "error_processing.h"
#include "general.h"


typedef long long stack_elem_t;
#include "stack_funcs.h"

FILE* log_output_file_ptr = NULL;

const size_t date_nmemb = 16;
const size_t time_nmemb = 16;

void log_init(const char log_path[], unsigned long long *return_err) {
    if (log_path == NULL) {
        log_output_file_ptr = stderr;
        return;
    }

    log_output_file_ptr = fopen(log_path, "w");
    if (log_output_file_ptr == NULL) {
        *return_err = ERR_FILE_OPEN;
        DEBUG_ERROR(*return_err);
    }
    setbuf(log_output_file_ptr, NULL); //disabling buffering
}

const char *get_log_descr(enum log_type_t log_type) {
    #define DESCR_(log_type) case log_type: return #log_type;

    switch (log_type) {
        DESCR_(LOG_ANALYS)
        DESCR_(LOG_DEBUG)
        DESCR_(LOG_ERROR)
        default: return "STRANGE LOG_TYPE";
    }
    #undef DESCR_
}

void print_log_time() {
    time_t cur_time;
    time(&cur_time);
    struct tm *now = localtime(&cur_time);

    char date_str[date_nmemb] = {};
    char time_str[time_nmemb] = {};
    strftime(date_str, date_nmemb, "%m/%d/%Y", now);
    strftime(time_str, time_nmemb, "%T", now);

    double milliseconds = ((double) clock()) / CLOCKS_PER_SEC;
    fprintf(log_output_file_ptr, "Date: %s Time: %s Ms: {%f}\n", date_str, time_str, milliseconds);
}

void print_log_border() {
    fprintf(log_output_file_ptr, "----------------------------------------------------------------------------------------------------\n");
}

void print_log_func_info(const char file_name[], const char func_name[], const int line_idx) {
    fprintf(log_output_file_ptr, "file : {%s}; func: {%s}; line: {%d}\n", file_name, func_name, line_idx);
}

void print_log_type(enum log_type_t log_type) {
    fprintf(log_output_file_ptr, "log_type: {%s}\n", get_log_descr(log_type));
}

void log_ptr_stack_dump(enum log_type_t log_type, stack_t *stk, const char file_name[], const char func_name[], const int line_idx) {
    print_log_border();
    print_log_type(log_type);
    print_log_time();
    print_log_func_info(file_name, func_name, line_idx);
    print_log_border();

    fprintf(log_output_file_ptr, RED "------------------------------------------------------------\n" WHT);
    fprintf(log_output_file_ptr, GRN "_________stk: [%p:%p)" RED " | " WHT "bytes: %2lu\n" GRN, stk, stk + 1, sizeof(*stk));
    fprintf(log_output_file_ptr, RED "------------------------------------------------------------\n" WHT);
    ON_HASH(
    fprintf(log_output_file_ptr, "________HASH: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->HASH_STACK_STRUCT, &stk->HASH_STACK_STRUCT + 1, sizeof(stk->HASH_STACK_STRUCT));
    fprintf(log_output_file_ptr, "________HASH: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->HASH_STACK_DATA, &stk->HASH_STACK_DATA + 1, sizeof(stk->HASH_STACK_DATA));
    )
    ON_CANARY(
    fprintf(log_output_file_ptr, "_CANARY_LEFT: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->CANARY_LEFT, &stk->CANARY_LEFT + 1, sizeof(stk->CANARY_LEFT))
    fprintf(log_output_file_ptr, "_CANARY_LEFT: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->CANARY_LEFT, &stk->CANARY_LEFT + 1, sizeof(stk->CANARY_LEFT))
    )
    fprintf(log_output_file_ptr, "________size: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->size, &stk->size + 1, sizeof(stk->size));
    fprintf(log_output_file_ptr, "____capacity: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->capacity, &stk->capacity + 1, sizeof(stk->capacity));
    ON_CANARY(
    fprintf(log_output_file_ptr, "__CANARY_MID: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->CANARY_MID, &stk->CANARY_MID + 1, sizeof(stk->CANARY_MID));
    )
    fprintf(log_output_file_ptr, "_______*data: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->data, &stk->data + 1, sizeof(stk->data));
    fprintf(log_output_file_ptr, "__*born_file: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->born_file, &stk->born_file + 1, sizeof(stk->born_file));
    fprintf(log_output_file_ptr, "__*born_line: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->born_line, &stk->born_line + 1, sizeof(stk->born_line));
    fprintf(log_output_file_ptr, "__*born_func: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->born_func, &stk->born_func + 1, sizeof(stk->born_func));
    ON_CANARY(
    fprintf(log_output_file_ptr, "CANARY_RIGHT: [%p:%p)" RED " | " WHT "bytes: %2lu\n", &stk->CANARY_RIGHT, &stk->CANARY_RIGHT + 1, sizeof(stl->CANARY_RIGHT))
    )
    fprintf(log_output_file_ptr, RED "------------------------------------------------------------\n" WHT);

    print_log_border();
    fprintf(log_output_file_ptr, "\n");
}

ON_HASH(
    void HASH_print(hash_t *HASH) {
        fprintf(log_output_file_ptr, "seg: [%p:%p)\n", HASH->left_ptr, HASH->right_ptr);
        fprintf(log_output_file_ptr, "hash_value: [%llu]\n", HASH->hash_value);
        fprintf(log_output_file_ptr, "get _value: [%llu]\n", HASH_get(HASH));
    }
)

void dump(stack_t *stk, const char file_name[], const char func_name[], const int line_idx) {
    print_log_border();
    print_log_type(LOG_DEBUG);
    print_log_time();
    print_log_func_info(file_name, func_name, line_idx);
    print_log_border();

    if (stk == NULL) {
        return;
    }

    fprintf_red(log_output_file_ptr, "stack_t [%p] at %s:%d born at %s:%d(%s)\n", \
    stk, file_name, line_idx, stk->born_file, stk->born_line, stk->born_func);

    fprintf_wht(log_output_file_ptr, "{\n");
    ON_CANARY(fprintf_yel(log_output_file_ptr, "canl_ptr[%p]\n", &stk->CANARY_LEFT);)
    fprintf_yel(log_output_file_ptr, "size = %lu\n", stk->size);
    // fprintf_yel(log_output_file_ptr, "size_ptr[%p]\n", &stk->size);
    fprintf_yel(log_output_file_ptr, "capacity = %lu\n", stk->capacity);
    // fprintf_yel(log_output_file_ptr, "capa_ptr[%p]\n", &stk->capacity);
    // fprintf_yel(log_output_file_ptr, "data_ptr[%p]\n", &stk->data);
    fprintf_yel(log_output_file_ptr, "stack[%p]\n", stk->data);

    fprintf_wht(log_output_file_ptr, "{\n");
    if (stk->data == NULL) {
        fprintf_red(log_output_file_ptr, "NULLPTR\n");
    } else {
        ON_CANARY(
            canary_elem_t left_canary_val = *(canary_elem_t *)(stk->data - LEFT_CANARY_INDENT);
            fprintf_grn(log_output_file_ptr, "[_left_canary] = %llx;\n", left_canary_val); //FIXME: %x? для unsigned long long работает?
        )
        for (size_t i = 0; i < stk->capacity; i++) {
            if (stk->data[i] == POISON_STACK_VALUE) {
                fprintf_grn(log_output_file_ptr, "[%lu] = POISON;\n", i);
            } else {
                fprintf_grn(log_output_file_ptr, "*[%lu] = %lld;\n", i, stk->data[i]);
            }
        }
        ON_CANARY(
            canary_elem_t right_canary_val = *stack_end_canary_getptr(stk);
            fprintf_grn(log_output_file_ptr, "[right_canary] = %llx;\n", right_canary_val); //FIXME: %x? для unsigned long long работает?
        )
    }
    fprintf_wht(log_output_file_ptr, "}\n");

    fprintf_wht(log_output_file_ptr, "}\n");

    print_log_border();
    fprintf(log_output_file_ptr, "\n");
}

void log_err_print(enum log_type_t log_type, const unsigned long long err, const char file_name[], const char func_name[], const int line_idx) {
    print_log_border();
    print_log_type(log_type);
    print_log_time();
    print_log_func_info(file_name, func_name, line_idx);
    print_log_border();

    fprintf(log_output_file_ptr, "-----------ERROR_LIST------------------\n");
    for (size_t err_bit = 0; err_bit < 32; err_bit++) {
        if ((err >> err_bit) & 1ull) {
            fprintf(log_output_file_ptr, "%s\n", get_bit_descr(1 << err_bit));
        }
    }
    print_log_border();
    fprintf(log_output_file_ptr, "\n");
}

void log_var_print(enum log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...) {
    print_log_border();
    print_log_type(log_type);
    print_log_time();
    print_log_func_info(file_name, func_name, line_idx);
    print_log_border();

    if (line_idx) {}

    va_list args;
    va_start(args, fmt);
    vfprintf(log_output_file_ptr, fmt, args);

    va_end(args);
    print_log_border();
    fprintf(log_output_file_ptr, "\n");
}

// TODO: как сделать макросы LogVar(),