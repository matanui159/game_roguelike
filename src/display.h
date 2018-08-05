#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef UNICODE
#	undef UNICODE
#endif

#include <windows.h>

#define WIDTH 40
#define HEIGHT 25

#define COLOR_UNCHANGED 0x0000
#define COLOR_BLUE 0x0100
#define COLOR_GREEN 0x0200
#define COLOR_CYAN 0x0300
#define COLOR_RED 0x0400
#define COLOR_MAGENTA 0x0500
#define COLOR_BROWN 0x0600
#define COLOR_LGRAY 0x0700
#define COLOR_GRAY 0x0800
#define COLOR_LBLUE 0x0900
#define COLOR_LGREEN 0x0A00
#define COLOR_LCYAN 0x0B00
#define COLOR_LRED 0x0C00
#define COLOR_LMAGENTA 0x0D00
#define COLOR_YELLOW 0x0E00
#define COLOR_WHITE 0x0F00

typedef unsigned short tile_t;

int display_key();
void display_set(int x, int y, tile_t tile);
tile_t display_get(int x, int y);

#endif