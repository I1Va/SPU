#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

const size_t border_size = 100;

void fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz);

void fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz);


#endif // OUTPUT_H