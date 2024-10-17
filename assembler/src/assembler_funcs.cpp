#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assembler_funcs.h"
#include "error_processing.h"
#include "output.h"
#include "general.h"

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

size_t asm_code_read(char asm_commands[][max_asm_command_size], const char path[], unsigned long long *return_err) {
    FILE *asm_file_ptr = fopen(path, "r");

    size_t com_idx = 0;

    if (asm_file_ptr == NULL) {
        *return_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(ERR_FILE_OPEN);
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
                *return_err |= ERR_SYNTAX;
                DEBUG_ERROR(ERR_SYNTAX)
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

void asm_commands_translate(int bin_code[], char asm_commands[][max_asm_command_size],
        const size_t asm_commands_n, unsigned long long *return_err)
{
    assert(bin_code != NULL);
    assert(asm_commands != NULL);
    assert(return_err != NULL);

    size_t com_idx = 0;
    size_t code_idx = 0;

    while (com_idx < asm_commands_n) {
        if (strcmp(asm_commands[com_idx], "push") == 0) {
            com_idx++;
            bin_code[code_idx++] = PUSH_COM;
            if (com_idx >= asm_commands_n) {
                *return_err |= ERR_SYNTAX;
                DEBUG_ERROR(*return_err)
                debug("push hasn't required arg");
                return;
            }

            char *end_ptr = NULL;
            int num = (int) strtol(asm_commands[com_idx++], &end_ptr, 10);
            if (*end_ptr != '\0') {
                *return_err |= ERR_SYNTAX;
                DEBUG_ERROR(ERR_SYNTAX);
                debug("Can't convert command arg to int. Arg: '%s'", asm_commands[com_idx - 1]);
                return;
            }
            bin_code[code_idx++] = num;
        } else if (strcmp(asm_commands[com_idx], "pop") == 0) {
            com_idx++;
            bin_code[code_idx++] = POP_COM;
        } else {
            *return_err |= ERR_SYNTAX;
            DEBUG_ERROR(ERR_SYNTAX);
            debug("Unknown command: '%s'", asm_commands[com_idx]);
            return;
        }
    }
}

void bin_code_write(const char path[], int bin_code[], const size_t n_bin_coms, unsigned long long *return_err) {
    FILE *bin_code_file_ptr = fopen(path, "w");
    if (bin_code_file_ptr == NULL) {
        *return_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(ERR_FILE_OPEN);
        return;
    }

    for (size_t bin_code_idx = 0; bin_code_idx < n_bin_coms; bin_code_idx++) {
        fprintf(bin_code_file_ptr, "%d ", bin_code[bin_code_idx]);
    }
}
