#include <cstddef>
#include <stdio.h>

#include "proc_output.h"
#include "./../stack/inc/stack_output.h"


// void fprintf_bin_code(FILE *stream, int code[], const size_t n) {
//     fprintf_title(stream, "BIN_CODE", '-', border_size);
//     for (size_t com_idx = 0; com_idx < n; com_idx++) {
//         fprintf(stream, "%d ", code[com_idx]);
//     }
//     fputc('\n', stream);
//     fprintf_border(stream, '-', border_size, true);
// }

void fprintf_bin_code(FILE *stream, int code[], const size_t n) {
    for (size_t com_idx = 0; com_idx < n; com_idx++) {
        fprintf(stream, "%d ", code[com_idx]);
    }
    fputc('\n', stream);
}
