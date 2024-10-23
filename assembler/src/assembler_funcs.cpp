#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assembler_funcs.h"
#include "error_processing.h"
#include "general.h"

#include "./../general_output/inc/general_output.h"

struct reg_t
{
    int id;
    const char *name;
};
reg_t reg_list[] =
{
    {0, "AX"},
    {1, "BX"},
    {2, "CX"},
    {3, "DX"},
};
const size_t reg_list_sz = sizeof(reg_list) / sizeof(reg_t);

int get_reg_id(const char name[]) {
    for (size_t reg_idx = 0; reg_idx < reg_list_sz; reg_idx++) {
        if (strcmp(reg_list[reg_idx].name, name) == 0) {
            return reg_list[reg_idx].id;
        }
    }
    return -1;
}

const int DEFAULT_label_VALUE = -1;
const size_t label_list_max_sz = 16;
const size_t max_label_name_sz = 16;

struct label_t
{
    int addr;
    char name[max_label_name_sz];
};
size_t empty_label_idx = 0;
label_t label_list[label_list_max_sz] = {};

void label_list_dump(FILE *stream) {
    fprintf_title(stream, "label_LIST", '-', border_size);
    fprintf(stream, "empty_label_idx: {%lu}\n", empty_label_idx);
    for (size_t label_idx = 0; label_idx < empty_label_idx; label_idx++) {
        fprintf(stream, "label[%2lu]: '%10s' addr = %d\n", label_idx, label_list[label_idx].name, label_list[label_idx].addr);
    }
    fprintf_border(stream, '-', border_size, true);
}

void fill_label_list(int default_addr) {
    for (size_t label_idx = 0; label_idx < label_list_max_sz; label_idx++) {
        label_list[label_idx].addr = default_addr;
    }
}

int add_label_to_list(int addr, const char *name) {
    assert(empty_label_idx < label_list_max_sz);

    label_list[empty_label_idx].addr = addr;
    strcpy(label_list[empty_label_idx].name, name);
    empty_label_idx++;

    return (int) empty_label_idx - 1;
}

int get_label_addr_from_list(const char *name) {
    for (size_t label_idx = 0; label_idx < empty_label_idx; label_idx++) {
        // printf("cmp: '%s' vs '%s' = {%d}", label_list[label_idx].name, name, strcmp(label_list[label_idx].name, name));
        if (strcmp(label_list[label_idx].name, name) == 0) {
            return (int) label_list[label_idx].addr;
        }
    }
    return -1;
}

struct fix_up_obj_t
{
    size_t label_bin_code_idx;
    char label_name[max_label_name_sz];
};

const size_t fix_up_table_max_sz = 16;
size_t empty_fix_up_idx = 0;
fix_up_obj_t fix_up_table[fix_up_table_max_sz] = {};

void fix_up_table_add(const size_t label_bin_code_idx, const char name[]) {
    assert(empty_fix_up_idx < fix_up_table_max_sz);

    fix_up_table[empty_fix_up_idx].label_bin_code_idx = label_bin_code_idx;
    strcpy(fix_up_table[empty_fix_up_idx].label_name, name);
    empty_fix_up_idx++;
}

void fix_up_table_pull_up(int bin_code[], asm_err *return_err) {
    for (size_t fix_idx = 0; fix_idx < empty_fix_up_idx; fix_idx++) {
        int label_addr = get_label_addr_from_list(fix_up_table[fix_idx].label_name);
        if (label_addr == -1) {
            asm_add_err(return_err, ASM_ERR_INVALID_LABEL);
            DEBUG_ERROR(ASM_ERR_INVALID_LABEL);
            debug("Invalid label : '%s'", fix_up_table[fix_idx].label_name);
        }
        bin_code[fix_up_table[fix_idx].label_bin_code_idx] = label_addr;
    }
}

// bool upd_reg(const char name[], const int value) {
//     for (size_t reg_idx = 0; reg_idx < reg_list_sz; reg_idx++) {
//         if (strcmp(reg_list[reg_idx].name, name) == 0) {
//             reg_list[reg_idx].value = value;
//             return true;
//         }
//     }
//     return false;
// }

bool asm_getline(FILE* stream, char line[], const size_t n) {
    assert(stream != NULL);
    assert(line != NULL);
    assert(n != 0);

    size_t idx = 0;
    bool EOF_state = true;
    bool write_state = true;

    while (int cur = fgetc(stream)) {
        if (idx >= n || cur == EOF || cur == '\n') {
            return !EOF_state || (cur == '\n');
        }

        if (cur == ';') {
            write_state = false;
        }

        if (write_state) {
            line[idx++] = (char) cur;
        }

        EOF_state = false;

    }
    return write_state;
}

void fprint_asm_commands_list(FILE *stream, const char asm_commands[][max_asm_command_size], const size_t n_coms) {
    fprintf_title(stream, "COMMANDS_LIST", '-', border_size);
    size_t line_content_len = 0;

    for (size_t com_idx = 0; com_idx < n_coms; com_idx++) {
        size_t cur_len = strlen(asm_commands[com_idx]);
        if (cur_len + line_content_len > border_size) {
            fputc('\n', stream);
            line_content_len = 0;
        }
        line_content_len += cur_len + 5;
        fprintf(stream, "'%s'; ", asm_commands[com_idx]);
    }

    fputc('\n', stream);
    fprintf_border(stream, '#', border_size);
    fputc('\n', stream);
}

size_t asm_code_read(char asm_commands[][max_asm_command_size], const char path[], asm_err *return_err) {
    FILE *asm_file_ptr = fopen(path, "r");

    size_t com_idx = 0;

    if (asm_file_ptr == NULL) {
        asm_add_err(return_err, ASM_ERR_FILE_OPEN);
        DEBUG_ERROR(ASM_ERR_FILE_OPEN);
        CLEAR_MEMORY(exit_mark)
    }

    while (1) {
        char line[max_asm_line_sz] = {};
        char *cur_line_ptr = line;

        if (!asm_getline(asm_file_ptr, line, max_asm_line_sz)) {
            break;
        }

        int delta_ptr = 0;
        while (1) {
            sscanf(cur_line_ptr, "%s%n", asm_commands[com_idx], &delta_ptr);
            if (!delta_ptr) {
                break;
            }
            if (delta_ptr >= (int) max_asm_command_size) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(ASM_ERR_SYNTAX)
                debug("parsed command size is larger than max_asm_command_size. Command : {%s}, Max_asm_com_sz: {%lu}", cur_line_ptr, max_asm_command_size);
                CLEAR_MEMORY(exit_mark)
            }

            com_idx++;
            cur_line_ptr += delta_ptr;
            delta_ptr = 0;
        }
    }

    fclose(asm_file_ptr);
    fprint_asm_commands_list(stdout, asm_commands, com_idx);
    return com_idx;

    exit_mark:
    if (asm_file_ptr != NULL) {
        fclose(asm_file_ptr);
    }
    return 0;
}

// bool get_rec_com_len(const char asm_com, const char fmt[]) {
//         } else if (sscanf(asm_com, fmt, &label, &len_rec_com)) {
//         printf("len: {%d}, label: %d, com: {%s}\n", len_rec_com, label, asm_commands[com_idx]);
//         com_idx++;
//     }
// }

bool check_label_elem(const char com[]) {
    int len_rec_com = 0;
    char label[max_label_name_sz] = {};
    sscanf(com, "%s%n", label, &len_rec_com);
    if (label[len_rec_com - 1] == ':') {
        return true;
    }
    return false;
}

void asm_commands_translate(int bin_code[], char asm_commands[][max_asm_command_size],
        const size_t asm_commands_n, asm_err *return_err)
{
    assert(bin_code != NULL);
    assert(asm_commands != NULL);
    assert(return_err != NULL);

    fill_label_list(DEFAULT_label_VALUE);

    size_t com_idx = 0;

    while (com_idx < asm_commands_n) {
        // printf("before com[%lu]: '%s'\n", com_idx, asm_commands[com_idx]);
        // label_list_dump(stdout);
        // printf("\n");


        if (strcmp(asm_commands[com_idx], "push") == 0) {
            bin_code[com_idx] = PUSH_COM;
            com_idx++;

            if (com_idx >= asm_commands_n) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(*return_err)
                debug("push hasn't required arg");
                return;
            }

            char *end_ptr = NULL;
            int num = (int) strtol(asm_commands[com_idx], &end_ptr, 10);
            if (*end_ptr != '\0') {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(ASM_ERR_SYNTAX);
                debug("Can't convert command arg to int. Arg: '%s'", asm_commands[com_idx - 1]);
                return;
            }
            bin_code[com_idx] = num;
            com_idx++;
            continue;
        }

        if (strcmp(asm_commands[com_idx], "pushr") == 0) {
            bin_code[com_idx] = PUSHR_COM;
            com_idx++;

            if (com_idx >= asm_commands_n) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(*return_err)
                debug("pushr hasn't required arg");
                return;
            }

            const char *reg_name = asm_commands[com_idx];

            int reg_id = get_reg_id(reg_name);

            if (reg_id == -1) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(*return_err)
                debug("invalid register name {%s}]", reg_name);
                return;
            }

            bin_code[com_idx] = reg_id;
            com_idx++;
            continue;
        }

        if (strcmp(asm_commands[com_idx], "popr") == 0) {
            bin_code[com_idx] = POPR_COM;
            com_idx++;

            const char *reg_name = asm_commands[com_idx];
            int reg_id = get_reg_id(reg_name);
            bin_code[com_idx] = reg_id;
            com_idx++;
            continue;
        }

        if (strcmp(asm_commands[com_idx], "jmp") == 0) {
            bin_code[com_idx] = JMP_COM;
            com_idx++;
            if (com_idx >= asm_commands_n) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(*return_err)
                debug("JMP hasn't required arg");
                return;
            }
            if (check_label_elem(asm_commands[com_idx])) {
                char label_name[max_label_name_sz] = {};

                sscanf(asm_commands[com_idx], "%s", label_name);
                int label_addr = get_label_addr_from_list(label_name);
                if (label_addr == -1) {
                    fix_up_table_add(com_idx, label_name);
                    com_idx++;
                    continue;
                }

                bin_code[com_idx++] = label_addr;
                continue;
            }

            char *end_ptr = NULL;
            int num = (int) strtol(asm_commands[com_idx], &end_ptr, 10);
            if (*end_ptr != '\0') {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(ASM_ERR_SYNTAX);
                debug("Can't convert command arg to int. Arg: '%s'", asm_commands[com_idx - 1]);
                return;
            }
            bin_code[com_idx++] = num;
            continue;
        }
        if (strcmp(asm_commands[com_idx], "ja") == 0) {
            bin_code[com_idx] = JA_COM;
            com_idx++;
            if (com_idx >= asm_commands_n) {
                asm_add_err(return_err, ASM_ERR_SYNTAX);
                DEBUG_ERROR(*return_err)
                debug("JA hasn't required arg");
                return;
            }
            if (check_label_elem(asm_commands[com_idx])) {
                char label_name[max_label_name_sz] = {};

                sscanf(asm_commands[com_idx], "%s", label_name);
                int label_addr = get_label_addr_from_list(label_name);
                if (label_addr == -1) {
                    fix_up_table_add(com_idx, label_name);
                    com_idx++;
                    continue;
                }

                bin_code[com_idx++] = label_addr;
                continue;
            }
        }

        if (strcmp(asm_commands[com_idx], "pop") == 0) {
            bin_code[com_idx] = POP_COM;
            com_idx++;
            continue;
        }

        if (strcmp(asm_commands[com_idx], "in") == 0) {
            bin_code[com_idx] = IN_COM;
            com_idx++;
            continue;
        }

        if (strcmp(asm_commands[com_idx], "out") == 0) {
            bin_code[com_idx++] = OUT_COM;
            continue;
        }
        if (strcmp(asm_commands[com_idx], "add") == 0) {
            bin_code[com_idx++] = ADD_COM;
            continue;
        }
        if (strcmp(asm_commands[com_idx], "sub") == 0) {
            bin_code[com_idx++] = SUB_COM;
            continue;
        }
        if (strcmp(asm_commands[com_idx], "mult") == 0) {
            bin_code[com_idx++] = MULT_COM;
            continue;
        }
        if (strcmp(asm_commands[com_idx], "hlt") == 0) {
            bin_code[com_idx++] = HLT_COM;
            continue;
        }
        if (check_label_elem(asm_commands[com_idx])) {
            char label_name[max_label_name_sz] = {};
            sscanf(asm_commands[com_idx], "%s", label_name);
            add_label_to_list((int) com_idx, label_name);
            bin_code[com_idx++] = LABEL_COM;
            continue;
        }

        asm_add_err(return_err, ASM_ERR_SYNTAX);
        DEBUG_ERROR(ASM_ERR_SYNTAX);
        debug("Unknown command: '%s'", asm_commands[com_idx]);
        return;
    }

    // IMPORTANT
    fix_up_table_pull_up(bin_code, return_err);
    if (*return_err != ASM_ERR_OK) {
        DEBUG_ERROR(*return_err);
        return;
    }
}

void bin_code_write(const char path[], int bin_code[], const size_t n_bin_coms, asm_err *return_err) {
    FILE *bin_code_file_ptr = fopen(path, "w");
    if (bin_code_file_ptr == NULL) {
        asm_add_err(return_err, ASM_ERR_SYNTAX);
        DEBUG_ERROR(ASM_ERR_FILE_OPEN);
        return;
    }

    for (size_t bin_code_idx = 0; bin_code_idx < n_bin_coms; bin_code_idx++) {
        fprintf(bin_code_file_ptr, "%d ", bin_code[bin_code_idx]);
    }
}
