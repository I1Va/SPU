#include "conf_ctor.h"
#include "error_processing.h"

#include <cstdlib>

void assembler_config_ctor(assembler_config_t *conf) {
    if (conf) {}
    return;
}

void assembler_config_dtor(assembler_config_t *conf) {
    *conf = {};
    return;
}

void auto_testing_config_ctor(auto_testing_config_t *conf) {
    conf->n_tests = 0;
    conf->exist = false;
    return;
}

void auto_testing_config_dtor(auto_testing_config_t *conf) {
    *conf = {};
    return;
}
