#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "error_processing.h"
#include "args_proc.h"
#include "conf_ctor.h"
#include "general.h"
#include "output.h"

int main(const int argc, const char *argv[]) {
    unsigned long long last_err = ERR_OK;

    assembler_config_t assembler_config = {}; assembler_config_ctor(&assembler_config);

    const size_t n_options = 1;
    opt_data options[n_options] = {};

    auto_testing_config_t auto_testing_config = {}; auto_testing_config_ctor(&auto_testing_config);

    const char log_path[] = "./logs/log.txt";

    log_init(log_path, &last_err);
    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err);
        CLEAR_MEMORY(exit_mark)
    }

    opt_data_ctor(&options[0], "-t-n_tests", "-testing-n_tests", "%ld", &auto_testing_config.n_tests);

    get_options(argc, argv, options, n_options);

    auto_testing_mode_launch(&auto_testing_config, &last_err);
    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err)
        CLEAR_MEMORY(exit_mark)
    }

    main_testing_mode_launch(&assembler_config, &last_err);
    if (last_err != ERR_OK) {
        DEBUG_ERROR(last_err)
        CLEAR_MEMORY(exit_mark)
    }

    return EXIT_SUCCESS;

    exit_mark:

    return EXIT_FAILURE;
}
