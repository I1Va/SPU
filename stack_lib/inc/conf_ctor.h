#ifndef CONF_CTOR_H
#define CONF_CTOR_H

#include <string.h>

struct assembler_config_t {
    bool exist = true;
};

struct auto_testing_config_t {
    bool exist;
    size_t n_tests;
};

void assembler_config_ctor(assembler_config_t *conf);

void assembler_config_dtor(assembler_config_t *conf);

void auto_testing_config_ctor(auto_testing_config_t *conf);

void auto_testing_config_dtor(auto_testing_config_t *conf);

#endif // CONF_CTOR_H