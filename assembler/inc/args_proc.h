#ifndef ARGS_PROC_H
#define ARGS_PROC_H

#include <string.h>

const size_t max_opt_name_sz = 64;

struct opt_data {
    const char *short_name;
    const char *long_name;

    bool exist;

    const char *fmt;
    void *val_ptr;
};

void opt_data_ctor(opt_data *option, const char *const short_name_src, const char *const long_name_src,
    const char *const fmt_src, void *val_ptr_src);

void opt_data_dtor(opt_data *option);

opt_data *option_list_ptr(const char *name, opt_data opts[], const size_t n_opts);

void get_options(const int argc, const char* argv[], opt_data opts[], const size_t n_opts);




const size_t max_assembler_config_inp_file_sz = 128;
const size_t max_assembler_config_out_file_sz = 128;

struct assembler_config_t {
    bool exist = true;
    char inp_file[max_assembler_config_inp_file_sz] = {};
    char out_file[max_assembler_config_out_file_sz] = {};
};


void assembler_config_ctor(assembler_config_t *conf);

void assembler_config_dtor(assembler_config_t *conf);


#endif // ARGS_PROC_H