#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <limits.h>
#include "proc_err.h"

#include "./../../binder_dir/bind_coms.h"

const size_t CONSOLE_HEIGHT = 50;
const size_t CONSOLE_WIDTH = CONSOLE_HEIGHT * 2;

const size_t RAM_sz = CONSOLE_WIDTH * CONSOLE_HEIGHT + (1u << 20);

size_t bin_code_read(const char path[], int code[], proc_err *return_err);

void execute_code(int code[], proc_err *return_err);

size_t get_bin_code_real_sz(int bin_code[], const size_t n);

#endif // PROCESSOR_FUNC_H