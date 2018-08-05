#include "print.h"

void print_char(int* x, int y, char msg, tile_t color) {
	display_set(*x, y, msg | color);
	++*x;
}

void print_string(int* x, int y, const char* msg, tile_t color) {
	for (const char* ptr = msg; *ptr != '\0'; ++ptr) {
		print_char(x, y, *ptr, color);
	}
}

void print_int(int* x, int y, int msg, tile_t color) {
	if (msg < 0) {
		print_char(x, y, '-', color);
		msg = -msg;
	}

	char c = (msg % 10) + '0';
	msg /= 10;
	if (msg > 0) {
		print_int(x, y, msg, color);
	}
	print_char(x, y, c, color);
}

void print_finish(int* x, int y) {
	while (*x < WIDTH) {
		print_char(x, y, 0, 0);
	}
}