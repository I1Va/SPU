#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "output.h"

void fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz) {
    for (size_t i = 0; i < bord_sz; i++) {
        fprintf(stream, WHT);
        fputc(bord_char, stream);
    }
}

void fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz) {
    assert(tittle != NULL);
    size_t tittle_sz = strlen(tittle);
    if (bord_sz < tittle_sz) {
        return;
    }
    size_t len = bord_sz - tittle_sz;
    fprintf_border(stream, bord_char, len / 2);
    fprintf_red(stream, "%s", tittle);
    fprintf_border(stream, bord_char, (len + 1) / 2);
    fputc('\n', stream);
}

void fprintf_bin_code(FILE *stream, int code[], const size_t n) {
    fprintf_title(stream, "BIN_CODE", '-', border_sz);
    for (size_t com_idx = 0; com_idx < n; com_idx++) {
        fprintf(stream, "%d ", code[com_idx]);
    }
    fputc('\n', stream);
    fprintf_border(stream, '#', border_sz);
    fputc('\n', stream);
}

