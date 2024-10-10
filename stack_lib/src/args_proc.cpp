#include <cstddef>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "conf_ctor.h"
#include "general.h"
#include "error_processing.h"
#include "output.h"

typedef long long stack_elem_t;
#include "stack_funcs.h"

#include "general.h"

#include "args_proc.h"

void opt_data_ctor(opt_data *option, const char *const short_name_src, const char *const long_name_src,
    const char *const fmt_src, void *val_ptr_src) {

    option->short_name = short_name_src;
    option->long_name = long_name_src;
    option->fmt = fmt_src;

    option->val_ptr = val_ptr_src;
    option->exist = false;
}

void opt_data_dtor(opt_data *option) {
    *option = {};
    return;
}

opt_data *option_list_ptr(const char *name, opt_data opts[], const size_t n_opts) {
    for (size_t i = 0; i < n_opts; i++) {
        if (strcmp(name, opts[i].short_name) == 0 || strcmp(name, opts[i].long_name) == 0) {
            return &opts[i];
        }
    }
    return NULL;
}

void get_options(const int argc, const char* argv[], opt_data opts[], const size_t n_opts) {
    assert(argc >= 0);

    for (int i = 1; i < argc; i++) {
        char name[max_opt_name_sz];
        char value[max_opt_name_sz];
        sscanf(argv[i], "%[^=]%s", name, value);

        opt_data *ptr = option_list_ptr(name, opts, n_opts);

        if (ptr != NULL) {
            ptr->exist = true;

            sscanf(value + 1, (ptr->fmt), ptr->val_ptr); // FIXME: исправить warning. Мб использовать __atribute__
            ptr->exist = true;
        }
    }
}

void main_testing_mode_launch(assembler_config_t *conf, unsigned long long *return_err) {
    assert(conf != NULL);
    assert(return_err != NULL);

    unsigned long long last_err = ERR_OK;
    last_err |= ERR_CANARY_MID;
    last_err |= ERR_STACK_LAST_ELEM;
    last_err |= ERR_ARGS;
    last_err |= ERR_MEM;

    LogErr(LOG_ERROR, last_err)

    last_err = ERR_OK;

    DEBUG_ERROR(last_err)

    stack_t stk = {};
    STACK_INIT(&stk, 10, &last_err);
    DUMP(&stk)


    for (size_t i = 0; i < 10; i++) {
        stack_push(&stk, rand(), &last_err);
        if (last_err != ERR_OK) {
            DEBUG_ERROR(last_err)
            CLEAR_MEMORY(exit_mark)
        }
    }
    DUMP(&stk)
    LogStkPtrInfo(LOG_ANALYS, &stk)
    for (size_t i = 0; i < 10; i++) {
        stack_pop(&stk, &last_err);
        if (last_err != ERR_OK) {
            DEBUG_ERROR(last_err)
            CLEAR_MEMORY(exit_mark)
        }
    }

    DUMP(&stk);

    // ptr_stack_dump(stdout, &stk);
    // if (!conf->exist) {
    //     return;
    // }

    stack_destroy(&stk);
    return;

    exit_mark:
    stack_destroy(&stk);
    return;
}


void auto_testing_mode_launch(auto_testing_config_t *conf, unsigned long long *return_err) {
    assert(conf != NULL);
    if (!conf->n_tests) {
        return;
    }

    assert(return_err != NULL);


    unsigned long long last_err = ERR_OK;

    // FIXME:
    // const char test_gen_path[] = "src/testing/tests_gen.py";
    // const char compiler[] = "python3";
    const char input_path[] = "src/testing/input.txt";
    const char create_test_command[] = "python3 src/testing/test_gen.py > src/testing/input.txt";
    const char create_answer_command[] = "python3 src/testing/brut.py < src/testing/input.txt > src/testing/answer.txt";
    const char output_path[] = "src/testing/output.txt";
    // FIXME:

    FILE* output_file = NULL;
    FILE* input_file = NULL;
    size_t n_input_coms = 0;

    stack_t stk = {};
    STACK_INIT(&stk, 0, &last_err)

    if (system(create_test_command)) {
        *return_err |= ERR_SYSTEM;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }

    if (system(create_answer_command)) {
        *return_err |= ERR_SYSTEM;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }

    input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        *return_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }

    output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        *return_err = ERR_FILE_OPEN;
        DEBUG_ERROR(*return_err)
        CLEAR_MEMORY(exit_mark)
    }
    for (size_t test_idx = 0; test_idx < conf->n_tests; test_idx++) {
        fscanf(input_file, "%lu", &n_input_coms);
        for (size_t com_idx = 0; com_idx < n_input_coms; com_idx++) {
            char com_str[10] = {};
            long long com_val = 0;

            fscanf(input_file, "%s", com_str);
            // printf_grn("cur command: %s\n", com_str);
            DUMP(&stk);

            LogErr(LOG_ERROR, last_err);
            if (strcmp(com_str, "push") == 0) {
                fscanf(input_file, "%lld", &com_val);
                stack_push(&stk, com_val, return_err);
            } else if (strcmp(com_str, "pop") == 0) {
                stack_pop(&stk, &last_err);
                if (last_err != ERR_OK) {
                    fprintf(output_file, "pop_error\n");
                }
            } else if (strcmp(com_str, "last") == 0) {
                stack_elem_t last_elem = stack_get_last(&stk, &last_err);
                if (last_err != ERR_OK) {
                    fprintf(output_file, "last_error\n");
                } else {
                    fprintf(output_file, "%lld\n", last_elem);
                }
            } else {
                assert(1);
            }
            last_err = ERR_OK; // спефика авто тестинга. Предыдущие ошибки должны быть аннулированы, чтобы не перетирать будущие
        }
        if (!system("diff src/testing/output.txt src/testing/answer.txt")) {
            debug("dismatch");
            abort();
        }
    }




    stack_destroy(&stk);

    fclose(input_file);
    fclose(output_file);

    return;

    exit_mark:

    stack_destroy(&stk);
    if (input_file != NULL) {
        fclose(input_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }
    return;
}

void main_mode_launch(assembler_config_t *conf, unsigned long long *return_err) {
    assert(conf != NULL);
    assert(return_err != NULL);
}
