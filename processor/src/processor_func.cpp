#include "proc_err.h"
#include <cstddef>
#include <stdio.h>

typedef int stack_elem_t;

#include "./../stack/inc/stack_output.h"
#include "./../stack/inc/error_processing.h"

#include "processor_func.h"
#include "proc_err.h"

const size_t reg_list_sz = 4;
int reg_list[reg_list_sz] = {};

size_t get_bin_code_real_sz(int bin_code[], const size_t n) {
    for (size_t bin_code_idx = 0; bin_code_idx < n; bin_code_idx++) {
        if (bin_code[bin_code_idx] == HLT_COM) {
            return bin_code_idx + 1;
        }
    }
    return n;
}

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
        int argv = 0;
        int argv1 = 0;
        int argv2 = 0;
        int argv3 = 0;

        int reg_id = -1;
        int addr = 0;

        int call_label = -1;

        bool hlt_state = false;

        switch (com) {
            case PUSH_COM:
                argv = code[ip++];
                stack_push(&stk, argv, &stk_last_err);
                break;
            case PUSHR_COM:
                reg_id = code[ip++];
                stack_push(&stk, reg_list[reg_id], &stk_last_err);
                break;
            case POPR_COM:
                reg_id = code[ip++];
                reg_list[reg_id] = stack_pop(&stk, &stk_last_err);
                break;
            case JMP_COM:
                addr = code[ip++];
                ip = addr;
                break;
            case JA_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                if (argv1 > argv2) {
                    ip = addr;
                }
                break;
            case JAE_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                if (argv1 >= argv2) {
                    ip = addr;
                }
                break;
            case JB_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                if (argv1 < argv2) {
                    ip = addr;
                }
                break;
            case JBE_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                // printf("argv1: {%d}, argv2: {%d}", argv1, argv2);
                if (argv1 <= argv2) {
                    ip = addr;
                }
                break;
            case JE_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                if (argv1 == argv2) {
                    ip = addr;
                }
                break;
            case JNE_COM:
                addr = code[ip++];
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                // printf("argv1: {%d}, argv2: {%d}\n", argv1, argv2);
                if (argv1 != argv2) {
                    ip = addr;
                }
                break;
            case IN_COM:
                argv = 0;
                scanf("%d", &argv);
                stack_push(&stk, argv, &stk_last_err);
                break;
            case OUT_COM:
                argv = stack_pop(&stk, &stk_last_err);
                printf("%d\n", argv);
                break;
            case ADD_COM:
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                argv3 = argv1 + argv2;

                stack_push(&stk, argv3, &stk_last_err);
                break;
            case SUB_COM:
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                argv3 = argv1 - argv2;

                stack_push(&stk, argv3, &stk_last_err);
                break;
            case MULT_COM:
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                argv3 = argv1 * argv2;

                stack_push(&stk, argv3, &stk_last_err);
                break;
            case LABEL_COM:
                break;
            case CALL_COM:
                stack_push(&call_stk, ip + 1, &stk_last_err);
                addr = code[ip++];
                ip = addr;
                break;
            case RET_COM:
                call_label = stack_pop(&call_stk, &stk_last_err);
                ip = call_label;
                break;
            case HLT_COM:
                hlt_state = true;
                break;
            default:
                proc_add_err(return_err, PROC_UNKNOWN_COM);
                break;
        }


        if (*return_err != PROC_ERR_OK) {
            // DEBUG_ERROR(*return_err)
            debug("proc_error: {%llu}", *return_err);
            return;
        }

        if (hlt_state) {
            break;
        }
    }

    stack_destroy(&stk);
    stack_destroy(&call_stk);

    return;
}

