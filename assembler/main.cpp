#include <cstdlib>
#include <stdio.h>

#include "error_processing.h"
#include "general.h"
#include "assembler_funcs.h"




int main() {
    asm_err last_err = ASM_ERR_OK;

    asm_code_t asm_code = asm_code_read("./../asm_code.txt", &last_err);
    if (last_err != ASM_ERR_OK) {
        DEBUG_ERROR(last_err);
        return EXIT_FAILURE;
    }

    bin_code_t bin_code = bin_code_init();

    asm_commands_translate(&bin_code, &asm_code, &last_err);

    bin_code_write("./../bin_code.txt", bin_code, &last_err);
}