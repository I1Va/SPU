#include <cstdlib>
#include <stdio.h>


#include "processor_func.h"

#include "stack.h"
#include "output.h"

const size_t mx_code_sz = 128;

int main() {
    unsigned long long last_err = 0;
    int code[mx_code_sz] = {};

    size_t n_coms = bin_code_read("./../bin_code.txt", code, &last_err);
    fprintf_bin_code(stdout, code, n_coms);

    execute_code(code, &last_err);

    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}