#ifndef ENGINE_H_
#define ENGINE_H_

#ifdef UNICODE
#	undef UNICODE
#endif

#include <windows.h>

#define WIDTH 40
#define HEIGHT 25

#define TILE_CHAR 0x00FF
#define TILE_BG 0x0F00
#define TILE_FG 0xF000
#define TILE_ALL 0xFFFF

#define BG_BLACK 0x0000
#define BG_BLUE 0x0100
#define BG_GREEN 0x0200
#define BG_CYAN 0x0300
#define BG_RED 0x0400
#define BG_MAGENTA 0x0500
#define BG_BROWN 0x0600
#define BG_LGRAY 0x0700
#define BG_GRAY 0x0800
#define BG_LBLUE 0x0900
#define BG_LGREEN 0x0A00
#define BG_LCYAN 0x0B00
#define BG_LRED 0x0C00
#define BG_LMAGENTA 0x0D00
#define BG_YELLOW 0x0E00
#define BG_WHITE 0x0F00

#define FG_BLACK 0x0000
#define FG_BLUE 0x1000
#define FG_GREEN 0x2000
#define FG_CYAN 0x3000
#define FG_RED 0x4000
#define FG_MAGENTA 0x5000
#define FG_BROWN 0x6000
#define FG_LGRAY 0x7000
#define FG_GRAY 0x8000
#define FG_LBLUE 0x9000
#define FG_LGREEN 0xA000
#define FG_LCYAN 0xB000
#define FG_LRED 0xC000
#define FG_LMAGENTA 0xD000
#define FG_YELLOW 0xE000
#define FG_WHITE 0xF000

typedef unsigned short tile_t;

void tile(int x, int y, tile_t tand, tile_t txor);

#endif