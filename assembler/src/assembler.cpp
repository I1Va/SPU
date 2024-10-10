#include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "args_proc.h"
#include "general.h"
#include "error_processing.h"


size_t get_file_sz(const char *const path, unsigned long long *return_err) {
    struct stat buf = {};
    if (stat(path, &buf) != 0) {
        *return_err |= ERR_STAT;
        DEBUG_ERROR(ERR_STAT)
        return 0;
    }

    return (size_t) buf.st_size;
}


void assembler_launch(assembler_config_t assembler_config, unsigned long long *return_err) {
    printf("input_file: %s\n", assembler_config.inp_file);
    printf("output_fule %s\n", assembler_config.out_file);

    FILE* inp_file_ptr = NULL;
    FILE* out_file_ptr = NULL;
    size_t file_sz = 0;

    unsigned long long last_err = ERR_OK;

    file_sz = get_file_sz(assembler_config.inp_file, return_err);

    inp_file_ptr = fopen(assembler_config.inp_file, "r");
    if (inp_file_ptr == NULL) {
        last_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(last_err)
        CLEAR_MEMORY(exit_mark)
    }

    out_file_ptr = fopen(assembler_config.out_file, "w");
    if (out_file_ptr == NULL) {
        last_err |= ERR_FILE_OPEN;
        DEBUG_ERROR(last_err)
        CLEAR_MEMORY(exit_mark)
    }

    char *src_code = (char *) calloc(file_sz, sizeof(char));
    if (src_code != NULL) {

    }

    while (1) {
        size_t line_sz = 0;

        char *line = NULL;
        if (getline(&line, &line_sz, inp_file_ptr) == -1) {
            break;
        }

    }








    return;

    exit_mark:
    if (inp_file_ptr != NULL) {
        fclose(inp_file_ptr);
    }
    if (out_file_ptr != NULL) {
        fclose(out_file_ptr);
    }

    *return_err |= last_err;

    return;

}