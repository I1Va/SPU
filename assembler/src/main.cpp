#include <cstdlib>
#include <stdio.h>

#include "error_processing.h"
#include "general.h"
#include "assembler_funcs.h"

int main() {
    unsigned long long last_err = ERR_OK;

    asm_code_read("./code.txt", &last_err);
    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err);
        return EXIT_FAILURE;
    }

}