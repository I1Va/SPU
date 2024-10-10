#ifndef ARGS_PROC_H
#define ARGS_PROC_H

#include <string.h>

#include "conf_ctor.h"
#include "error_processing.h"

struct opt_data {
    const char *short_name;
    const char *long_name;

    bool exist;

    const char *fmt;
    void *val_ptr;
};

const size_t max_opt_name_sz = 64;

void opt_data_ctor(opt_data *option, const char *const short_name_src, const char *const long_name_src,
    const char *const fmt_src, void *val_ptr_src);

void opt_data_dtor(opt_data *option);

opt_data *option_list_ptr(const char *name, opt_data opts[], const size_t n_opts);

void get_options(const int argc, const char* argv[], opt_data opts[], const size_t n_opts);

void main_mode_launch(assembler_config_t *conf, unsigned long long *return_err);

void main_testing_mode_launch(assembler_config_t *conf, unsigned long long *return_err);

void auto_testing_mode_launch(auto_testing_config_t *conf, unsigned long long *return_err);

#endif // ARGS_PROC_H