#include <cstdlib>
#include <stdio.h>


#include "processor_func.h"

#include "./../stack/inc/error_processing.h"
#include "./../stack/inc/general.h"
#include "proc_err.h"
#include "proc_output.h"

const size_t mx_code_sz = 128;

int main() {
    proc_err proc_last_err = PROC_ERR_OK;
    int code[mx_code_sz] = {};

    size_t n_coms = bin_code_read("./../bin_code.txt", code, &proc_last_err);

    fprintf(stdout, "\n------------------BIN_CODE-------------\n");
    fprintf_bin_code(stdout, code, n_coms);
    fprintf(stdout, "---------------------------------------\n");
    execute_code(code, &proc_last_err);

    return EXIT_SUCCESS;
}