#include "proc_err.h"
#include <cassert>
#include <climits>
#include <cstddef>
#include <stdio.h>

typedef int stack_elem_t;

#include "./../stack/inc/stack_output.h"
#include "./../stack/inc/error_processing.h"
#include "./../stack/inc/stack_funcs.h"
#include "processor_func.h"
#include "proc_err.h"

const size_t reg_list_sz = 4;
int reg_list[reg_list_sz] = {};

const stack_elem_t ACCURACY = 100;

int RAM[RAM_sz] = {};

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


    FILE *bin_code_file_ptr = fopen(path, "rb");
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

void fprintf_bin(FILE *stream, int mask) {
    int start_mask = mask;

    const size_t bit_len = sizeof(mask) * 8;
    char res_str[bit_len + 1] = {};
    size_t res_idx = 0;

    char sims[] = "01";

    while (mask > 0) {
        res_str[res_idx++] = sims[mask % 2];
        mask /= 2;
    }
    while (res_idx < bit_len) {
        res_str[res_idx++] = '0';
    }

    fprintf(stream, "bin[%10d]: '%s'\n", start_mask, res_str);
}

void get_bit_str(int mask, char res_str[], const size_t n_bits) {
    assert(res_str != NULL);

    size_t res_idx = 0;

    size_t cnt_chars = 0;
    while (mask > 0 && res_idx < n_bits) {
        res_str[res_idx++] = (char) (mask % 2) + '0';
        mask /= 2;
    }
    while (res_idx < n_bits) {
        res_str[res_idx++] = '0';
    }
}



// void stack_push_double(stack_t *stk, const double val, stk_err *return_err) {
//     stack_push(stk, (int) (val * ACCURACY), return_err);
// }

// stack_elem_t stack_pop_double(stack_t *stk, stk_err *return_err) {
//     return stack_pop(stk, return_err) / ACCURACY;
// }

void execute_code(int code[], proc_err *return_err) {
    int ip = 0;
    int com = 0;

    stk_err stk_last_err = STK_ERR_OK;
    stack_t stk = {}; STACK_INIT(&stk, 0, &stk_last_err);
    stack_t call_stk = {}; STACK_INIT(&call_stk, 0, &stk_last_err);
    // printf("filter_mask\n");
    // fprintf_bin(stdout, filter_mask);
    while (1) {
        com = code[ip++];
        // printf("com[%d]: '%d'\n", ip, com);
        // fprintf_bin(stdout, filter_mask & com);
        // printf("{%d} vs {%d}\n", com, filter_mask & com);
        int argv_sum = 0;
        double double_argv = 0.0;
        int argv = 0;
        int argv1 = 0;
        int argv2 = 0;
        int argv3 = 0;

        int reg_id = -1;
        int addr = 0;

        int call_label = -1;

        bool hlt_state = false;


        switch (com & filter_mask) {
            case JMP_COM:
                // printf("jump from {%d} to {%d}", ip, code[ip]);
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
                double_argv = 0;
                scanf("%lg", &double_argv);
                stack_push(&stk, double_argv, &stk_last_err);
                break;
            case OUT_COM:
                argv = stack_pop(&stk, &stk_last_err);
                printf("%d", argv);
                break;
            case OUTC_COM:
                argv = stack_pop(&stk, &stk_last_err);
                printf("%c", argv);
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
            case DIV_COM:
                argv1 = stack_pop(&stk, &stk_last_err);
                argv2 = stack_pop(&stk, &stk_last_err);
                argv3 = argv1 / argv2;

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
            case DRAW_COM:
                for (size_t i = 0; i < CONSOLE_HEIGHT; i++) {
                    for (size_t j = 0; j < CONSOLE_WIDTH; j++) {
                        printf("%c", RAM[i * CONSOLE_WIDTH + j]);
                        // printf("%c", RAM[i * CONSOLE_WIDTH + j]); // FIXME: чтобы круг был круглым, можно раскоментить эту строчку
                        // т.е. кажду. клетку дублировать
                    }
                    printf("\n");
                }
                break;
            case RET_COM:
                call_label = stack_pop(&call_stk, &stk_last_err);
                ip = call_label;
                break;

            case PUSH_COM:
                if (com & MASK_MEM) {
                    argv_sum = 0;
                    if (com & MASK_REG) {
                        reg_id = code[ip++];
                        argv_sum += reg_list[reg_id];
                    }
                    if (com & MASK_IMMC) {
                        argv = code[ip++];
                        argv_sum += argv;
                    }
                    stack_push(&stk, RAM[argv_sum], &stk_last_err);
                } else {
                    argv_sum = 0;
                    if (com & MASK_REG) {
                        reg_id = code[ip++];
                        argv_sum += reg_list[reg_id];
                    }
                    if (com & MASK_IMMC) {
                        argv = code[ip++];
                        argv_sum += argv;
                    }
                    stack_push(&stk, argv_sum, &stk_last_err);
                }
                break;
            case POP_COM:
                if (com & MASK_MEM) {
                    argv_sum = 0;
                    if (com & MASK_REG) {
                        reg_id = code[ip++];
                        argv_sum += reg_list[reg_id];
                    }
                    if (com & MASK_IMMC) {
                        argv = code[ip++];
                        argv_sum += argv;
                    }
                    RAM[argv_sum] = stack_pop(&stk, &stk_last_err);
                } else {
                    if (com & MASK_REG) {
                        reg_id = code[ip++];
                        reg_list[reg_id] = stack_pop(&stk, &stk_last_err);
                    } else {
                        stack_pop(&stk, &stk_last_err);
                    }
                }
                break;

            case HLT_COM:
                hlt_state = true;
                break;
            default:
                debug("ip = {%d}, unknown command '%d' (tip: if com eq 0, you miss 'hlt')\n", ip, com & filter_mask);
                proc_add_err(return_err, PROC_UNKNOWN_COM);
                break;
        }
        if (*return_err != PROC_ERR_OK) {
            // DEBUG_ERROR(*return_err)
            const size_t bit_len = sizeof(proc_err) * 8;
            char bit_str[bit_len + 1] = {};
            get_bit_str(*return_err, bit_str, bit_len);
            debug("ip: {%d}, proc_error: %s", ip, bit_str);
            stack_destroy(&stk);
            stack_destroy(&call_stk);
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

