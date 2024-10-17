#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>

const size_t border_sz = 100;

void fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz);

void fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz);

void fprintf_bin_code(FILE *stream, int code[], const size_t n);