#include "proc_err.h"
#include <stdio.h>

typedef int stack_elem_t;

#include "./../stack/inc/stack_output.h"
#include "./../stack/inc/error_processing.h"

#include "processor_func.h"
#include "proc_err.h"

const size_t reg_list_sz = 4;
int reg_list[reg_list_sz] = {};

size_t bin_code_read(const char path[], int code[], proc_err *return_err) {
    size_t com_idx = 0;

    FILE *bin_code_file_ptr = fopen(path, "r");
    if (bin_code_file_ptr == NULL) {
        proc_add_err(return_err, PROC_ERR_FILE_OPEN);
        DEBUG_ERROR(STK_ERR_FILE_OPEN)
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

void execute_code(int code[], proc_err *return_err) {
    int ip = 0;
    int com = 0;

    stk_err stk_last_err = STK_ERR_OK;
    stack_t stk = {}; STACK_INIT(&stk, 0, &stk_last_err);
    stack_t call_stk = {}; STACK_INIT(&call_stk, 0, &stk_last_err);

    while (1) {
        com = code[ip++];
        if (com == PUSH_COM) {
            int argv = code[ip++];
            stack_push(&stk, argv, &stk_last_err);
            continue;
        }

        if (com == PUSHR_COM) {
            int reg_id = code[ip++];
            stack_push(&stk, reg_list[reg_id], &stk_last_err);
            continue;
        }

        if (com == POPR_COM) {
            int reg_id = code[ip++];
            reg_list[reg_id] = stack_pop(&stk, &stk_last_err);
            continue;
        }

        if (com == JMP_COM) {
            int addr = code[ip++];
            ip = addr;
            continue;
        }
        if (com == JA_COM) {
            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            if (argv1 > argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == JAE_COM) {
            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            if (argv1 >= argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == JB_COM) {
            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            if (argv1 < argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == JBE_COM) {
            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            // printf("argv1: {%d}, argv2: {%d}", argv1, argv2);
            if (argv1 <= argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == JE_COM) {

            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            if (argv1 == argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == JNE_COM) {
            int addr = code[ip++];
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            // printf("argv1: {%d}, argv2: {%d}\n", argv1, argv2);
            if (argv1 != argv2) {
                ip = addr;
            }
            continue;
        }
        if (com == IN_COM) {
            int argv = 0;
            scanf("%d", &argv);
            stack_push(&stk, argv, &stk_last_err);
            continue;
        }

        if (com == OUT_COM) {
            int argv = stack_pop(&stk, &stk_last_err);
            printf("%d\n", argv);
            continue;
        }

        if (com == ADD_COM) {
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            int argv3 = argv1 + argv2;

            stack_push(&stk, argv3, &stk_last_err);
            continue;
        }

        if (com == SUB_COM) {
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            int argv3 = argv1 - argv2;

            stack_push(&stk, argv3, &stk_last_err);
            continue;
        }

        if (com == MULT_COM) {
            int argv1 = stack_pop(&stk, &stk_last_err);
            int argv2 = stack_pop(&stk, &stk_last_err);
            int argv3 = argv1 * argv2;

            stack_push(&stk, argv3, &stk_last_err);
            continue;
        }

        if (com == LABEL_COM) {
            continue;
        }

        if (com == CALL_COM) {
            stack_push(&call_stk, ip + 1, &stk_last_err);
            int addr = code[ip++];
            ip = addr;
            continue;
        }

        if (com == RET_COM) {
            int call_label = stack_pop(&call_stk, &stk_last_err);
            ip = call_label;
            continue;
        }

        if (com == HLT_COM) {
            break;
        }





        proc_add_err(return_err, PROC_UNKNOWN_COM);
        if (*return_err != PROC_ERR_OK) {
            debug("proc_error: {%llu}", *return_err);
        }
        stack_destroy(&stk);
        stack_destroy(&call_stk);
        return;
    }

    stack_destroy(&stk);
    stack_destroy(&call_stk);

    return;
}

