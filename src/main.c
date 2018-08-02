#include "display.h"
#include "stdmem.h"

void dmain(int key, void* data) {
	int fun = (int)data;

	if (key == VK_DRAW) {
		for (int x = 0; x < WIDTH; ++x) {
			for (int y = 0; y < HEIGHT; ++y) {
				dtile(x, y, TILE_BG, fun << 8);
			}
		}
	}

	if (key == VK_SPACE) {
		dscene(dmain, (void*)((fun + 1) & 0xF));
	}

	if (key == VK_ESCAPE) {
		dscene(NULL, NULL);
	}
}