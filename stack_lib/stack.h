#ifndef STACK_H
#define STACK_H

#include <string.h>

typedef int stack_elem_t;

typedef unsigned long long canary_elem_t;


#define GRN "\e[0;32m"
#define WHT "\e[0;20m"
#define RED "\e[0;31m"
#define YEL "\e[0;33m"

#define printf_red(str_, ...) printf(RED str_ WHT, ##__VA_ARGS__) // TODO: дефайны КАПСОМ
#define printf_wht(str_, ...) printf(WHT str_ WHT, ##__VA_ARGS__)
#define printf_grn(str_, ...) printf(GRN str_ WHT, ##__VA_ARGS__)
#define printf_yel(str_, ...) printf(YEL str_ WHT, ##__VA_ARGS__)

#define fprintf_red(stream, str_, ...) fprintf(stream, RED str_ WHT, ##__VA_ARGS__)
#define fprintf_wht(stream, str_, ...) fprintf(stream, WHT str_ WHT, ##__VA_ARGS__)
#define fprintf_grn(stream, str_, ...) fprintf(stream, GRN str_ WHT, ##__VA_ARGS__)
#define fprintf_yel(stream, str_, ...) fprintf(stream, YEL str_ WHT, ##__VA_ARGS__)

#define FREE(ptr)              \
    do {                       \
        free(ptr); ptr = NULL; \
    } while(0);



#define ASSERT(error, end_instruction)                                                                                    \
    fprintf_red(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_descr(error));    \
    end_instruction;

#define CLEAR_MEMORY(mark) goto mark;

#ifdef _DEBUG
    #define debug(str_, ...) fprintf_red(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define DEBUG_ERROR(code) fprintf_red(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_descr(code));
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

#define LogErr(log_type, err) log_err_print(log_type, err, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);

#define LogVar(log_type, fmt, ...) log_var_print(log_type, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define LogStkPtrInfo(log_type, stk) log_ptr_stack_dump(log_type, stk, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);


struct opt_data {
    const char *short_name;
    const char *long_name;

    bool exist;

    const char *fmt;
    void *val_ptr;
};

struct main_config_t {
    bool exist;
    int log_file_stderr;
};

struct auto_testing_config_t {
    bool exist;
    int log_file_stderr;
    size_t n_tests;
};

enum err_code {
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
    ERR_STACK_NULLPTR              = 1 << 11,
    ERR_STACK_CONT_NULLPTR         = 1 << 12,
    ERR_STACK_OVERFLOW             = 1 << 13,
    ERR_STACK_POP                  = 1 << 14,
    ERR_REALLOC                    = 1 << 15,
    ERR_CANARY_LEFT                = 1 << 16,
    ERR_CANARY_MID                 = 1 << 17,
    ERR_CANARY_RIGHT               = 1 << 18,
    ERR_CANARY_STK_RIGHT           = 1 << 19,
    ERR_HASH_STACK_DATA_MISMATCH   = 1 << 20,
    ERR_CANARY_STK_LEFT            = 1 << 21,
    ERR_SYSTEM                     = 1 << 22,
    ERR_STACK_LAST_ELEM            = 1 << 23,
    ERR_HASH_STACK_STRUCT_MISMATCH = 1 << 24,
    ERR_SYNTAX                     = 1 << 25,
};

enum RETURN_STATES {
    ReturnErrOR  = -1,
    RETURN_FALSE =  0,
    RETURN_TRUE  =  1,
};

enum log_type_t {
    LOG_ANALYS = 0,
    LOG_DEBUG = 1,
    LOG_ERROR = 2,
};


const size_t max_opt_name_sz = 64;

const canary_elem_t CANARY_VALUE = 0xC0FFEEABACABABAC;
const size_t CANARY_NMEMB = sizeof(canary_elem_t);

const stack_elem_t POISON_STACK_VALUE = 0x0BAD0DED; // FIXME: использовать при resize
const size_t resize_up_coeff = 2;
const size_t resize_down_check_coeff = 4;
const size_t resize_down_coeff = 2;

const size_t dump_output_sz = 10;


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

void opt_data_ctor(opt_data *option, const char *const short_name_src, const char *const long_name_src,
    const char *const fmt_src, void *val_ptr_src);

void opt_data_dtor(opt_data *option);

opt_data *option_list_ptr(const char *name, opt_data opts[], const size_t n_opts);

void get_options(const int argc, const char* argv[], opt_data opts[], const size_t n_opts);

void main_mode_launch(main_config_t *conf, unsigned long long *return_err);

void main_testing_mode_launch(main_config_t *conf, unsigned long long *return_err);

void auto_testing_mode_launch(auto_testing_config_t *conf, unsigned long long *return_err);

void main_config_ctor(main_config_t *conf);

void main_config_dtor(main_config_t *conf);

void auto_testing_config_ctor(auto_testing_config_t *conf);

void auto_testing_config_dtor(auto_testing_config_t *conf);

const char *get_descr(unsigned long long err);

int fprintf_abort(const char file_name[], const int line, const char function_name[], const char error_descr[]);

const char *get_bit_descr(unsigned long long err);

const char *get_descr(enum log_type_t log_type);

void print_log_func_info(const char file_name[], const char func_name[], const int line_idx);

ON_HASH(void HASH_print(hash_t *HASH);)

void dump(stack_t *stk, const char file_name[], const char func_name[], const int line_idx);

void log_ptr_stack_dump(enum log_type_t log_type, stack_t *stk, const char file_name[], const char func_name[], const int line_idx);

void print_log_time();

void print_log_type(enum log_type_t log_type);

void print_log_border();

void log_err_print(enum log_type_t log_type, const unsigned long long err, const char file_name[], const char func_name[], const int line_idx);

void log_init(const char log_path[], unsigned long long *return_err);

const char *get_log_descr(enum log_type_t log_type);

void log_var_print(enum log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...);

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

#endif // STACK_H