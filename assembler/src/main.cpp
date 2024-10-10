#include <stdio.h>

#include "args_proc.h"
#include "assembler.h"

int main(const int argc, const char ** argv) {
    assembler_config_t assembler_config = {}; assembler_config_ctor(&assembler_config);

    const size_t n_options = 2;
    opt_data options[n_options] = {};
    opt_data_ctor(&options[0], "-i", "-in", "%s", &assembler_config.inp_file);
    opt_data_ctor(&options[1], "-o", "-out", "%s", &assembler_config.out_file);

    get_options(argc, argv, options, n_options);
    assembler_launch(assembler_config);
}