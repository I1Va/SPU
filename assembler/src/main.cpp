#include <cstdlib>
#include <stdio.h>

#include "error_processing.h"
#include "general.h"
#include "assembler_funcs.h"

const size_t max_bin_code_sz = 256;

int main() {
    unsigned long long last_err = ERR_OK;
    char asm_commands[max_asm_commands_n][max_asm_command_size] = {};

    size_t asm_commands_n = asm_code_read(asm_commands, "./asm_code.txt", &last_err);
    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err);
        return EXIT_FAILURE;
    }
    int bin_code[max_bin_code_sz];
    asm_commands_translate(bin_code, asm_commands, asm_commands_n, &last_err);

    bin_code_write("./bin_code.txt", bin_code, asm_commands_n, &last_err);


}