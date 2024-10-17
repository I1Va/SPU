#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "error_processing.h"
#include "output.h"
#include "general.h"

const size_t max_asm_line_sz = 128;
const size_t max_asm_command_size = 16;
const size_t max_commands_n = 128;

const size_t border_size = 100;

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

// bool asm_parse_line(const char line[]) {
//     sscanf(line, "%s")
// }

void fprint_commands_list(FILE *stream, const char commands[][max_asm_command_size], const size_t n_coms) {
    fprintf_title(stream, "COMMANDS_LIST", '-', border_size);
    size_t line_content_len = 0;

    for (size_t com_idx = 0; com_idx < n_coms; com_idx++) {
        size_t cur_len = strlen(commands[com_idx]);
        if (cur_len + line_content_len > border_size) {
            fputc('\n', stream);
            line_content_len = 0;
        }
        line_content_len += cur_len + 5;
        fprintf(stream, "'%s'; ", commands[com_idx]);
    }

    fputc('\n', stream);
    fprintf_border(stream, '#', border_size);
    fputc('\n', stream);
}

void asm_code_read(const char path[], unsigned long long *return_err) {
    unsigned long long last_err = ERR_OK;

    FILE *asm_file_ptr = fopen(path, "r");
    if (asm_file_ptr == NULL) {
        *return_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(ERR_FILE_OPEN);
    }



    char commands[max_commands_n][max_asm_command_size] = {};
    size_t com_idx = 0;

    while (1) {
        char line[max_asm_line_sz] = {};
        if (!asm_getline(asm_file_ptr, line, max_asm_line_sz)) {
            break;
        }
        printf("line: '%s'\n", line);
        while (sscanf(line, "%s", commands[com_idx])) {
            com_idx++;
            break;
        }
    }
    printf("idx: %lu\n", com_idx);
    fprint_commands_list(stdout, commands, com_idx);






}