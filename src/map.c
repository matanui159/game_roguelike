#include "map.h"

typedef struct info_t {
	_Bool solid:1;
	_Bool revealed:1;
	_Bool visible:1;
} info_t;

static info_t g_map[MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH];

void map_init() {
	for (int z = 0; z < MAP_DEPTH; ++z) {
		for (int y = 0; y < MAP_HEIGHT; ++y) {
			for (int x = 0; x < MAP_WIDTH; ++x) {
				int index = (z * MAP_HEIGHT + y * MAP_WIDTH) + x;
				if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) {
					g_map[index].solid = 1;
				}
			}
		}
	}
}

void map_draw() {
	
}