#ifndef PROC_OUTPUT_H
#define PROC_OUTPUT_H

#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>

void fprintf_bin_code(FILE *stream, int code[], const size_t n);

#endif // PROC_OUTPUT_H