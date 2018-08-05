#ifndef PRINT_H_
#define PRINT_H_
#include "display.h"

void print_char(int* x, int y, char msg, tile_t color);
void print_string(int* x, int y, const char* msg, tile_t color);
void print_int(int* x, int y, int msg, tile_t color);
void print_finish(int* x, int y);

#endif