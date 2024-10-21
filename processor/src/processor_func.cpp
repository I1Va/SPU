#include <stdio.h>

typedef int stack_elem_t;

#include "./../stack/inc/stack_output.h"
#include "./../stack/inc/error_processing.h"

#include "processor_func.h"

size_t bin_code_read(const char path[], int code[], unsigned long long *return_err) {
    size_t com_idx = 0;

    FILE *bin_code_file_ptr = fopen(path, "r");
    if (bin_code_file_ptr == NULL) {
        *return_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(ERR_FILE_OPEN)
        CLEAR_MEMORY(exit_mark)
    }

    while (fscanf(bin_code_file_ptr, "%d", &code[com_idx]) != EOF) {
        com_idx++;
    }

    fclose(bin_code_file_ptr);

    return com_idx;

    exit_mark:
    if (bin_code_file_ptr != NULL) {
        fclose(bin_code_file_ptr);
    }

    return 0;
}

void execute_code(int code[], unsigned long long *return_err) {
    log_init(NULL, return_err);
    int ip = 0;
    int com = 0;

    stack_t stk = {}; STACK_INIT(&stk, 0, return_err);


    while (1) {
        com = code[ip++];
        if (com == PUSH_COM) {
            int argv = code[ip++];
            stack_push(&stk, argv, return_err);
            continue;
        }
        if (com == POP_COM) {
            stack_pop(&stk, return_err);
            continue;
        }
        if (com == IN_COM) {
            int argv = 0;
            scanf("%d", &argv);
            stack_push(&stk, argv, return_err);
            continue;
        }
        if (com == OUT_COM) {
            int argv = stack_pop(&stk, return_err);
            printf("%d\n", argv);
            continue;
        }
        if (com == ADD_COM) {
            int argv1 = stack_pop(&stk, return_err);
            int argv2 = stack_pop(&stk, return_err);
            int argv3 = argv1 + argv2;

            stack_push(&stk, argv3, return_err);
            continue;
        }
        if (com == SUB_COM) {
            int argv1 = stack_pop(&stk, return_err);
            int argv2 = stack_pop(&stk, return_err);
            int argv3 = argv1 - argv2;

            stack_push(&stk, argv3, return_err);
            continue;
        }
        if (com == MULT_COM) {
            int argv1 = stack_pop(&stk, return_err);
            int argv2 = stack_pop(&stk, return_err);
            int argv3 = argv1 * argv2;

            stack_push(&stk, argv3, return_err);
            continue;
        }
        if (com == HLT_COM) {
            break;
        }
        *return_err |= ERR_SYNTAX;
        stack_destroy(&stk);
        return;
    }

    stack_destroy(&stk);

    return;
}

