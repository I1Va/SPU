#include <cstdlib>
#include <stdio.h>


#include "processor_func.h"

#include "./../stack/inc/error_processing.h"
#include "./../stack/inc/general.h"
#include "proc_err.h"
#include "proc_output.h"

int main() {
    proc_err proc_last_err = PROC_ERR_OK;
    int code[max_bin_code_sz] = {};

    bin_code_read("./../bin_code.txt", code, &proc_last_err);

    execute_code(code, &proc_last_err);

    return EXIT_SUCCESS;
}