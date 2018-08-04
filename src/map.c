#include "map.h"
#include "entity.h"
#include "player.h"
#include "random.h"

#define SHADOW_LIMIT 6

typedef struct shadow_t {
	_Bool used;
	float left;
	float right;
} shadow_t;

static map_tile_t g_map[MAP_WIDTH * MAP_HEIGHT];

static void map_light(int dx, int dy, _Bool flip) {
	shadow_t shadows[SHADOW_LIMIT] = {0};
	entity_t* player = entity_get(0);
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col <= row; ++col) {
			int x = col;
			int y = row;
			if (flip) {
				x = row;
				y = col;
			}
			map_tile_t* tile = map_tile(player->x + x * dx, player->y + y * dy);

			float left = (float)col / (row + 2);
			float right = (float)(col + 1) / (row + 1);
			tile->visible = 1;
			for (int i = 0; i < SHADOW_LIMIT; ++i) {
				shadow_t* shadow = shadows + i;
				if (shadow->used) {
					shadow->used = 1;
					if (left >= shadow->left && right <= shadow->right) {
						tile->visible = 0;
						break;
					}

					if (tile->solid) {
						if (left >= shadow->left && left < shadow->right) {
							left = shadow->left;
							shadow->used = 0;
						}
						if (right > shadow->left && right <= shadow->right) {
							right = shadow->right;
							shadow->used = 0;
						}
					}
				}
			}

			if (tile->visible) {
				tile->revealed = 1;
			}

			if (tile->visible && tile->solid) {
				for (int i = 0; i < SHADOW_LIMIT; ++i) {
					shadow_t* shadow = shadows + i;
					if (!shadow->used) {
						shadow->left = left;
						shadow->right = right;
						shadow->used = 1;
						break;
					}
				}
			}
		}
	}
}

map_tile_t* map_tile(int x, int y) {
	if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
		static map_tile_t empty;
		empty.solid = 1;
		empty.visible = 0;
		empty.revealed = 0;
		return &empty;
	}
	return g_map + y * MAP_WIDTH + x;
}

void map_create(int level) {
	entity_clear();
	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			map_tile_t* tile = map_tile(x, y);
			tile->solid = 1;
			tile->revealed = 0;
		}
	}

	int ex = 0;
	int ey = 0;
	for (int i = 0; i < 32; ++i) {
		int rw = random() * 4 + 4;
		int rh = random() * 4 + 4;
		int rx = random() * (MAP_WIDTH - rw - 2) + 1;
		int ry = random() * (MAP_HEIGHT - rh - 2) + 1;

		_Bool overlap = 0;
		for (int x = rx - 1; x < rx + rw + 1; ++x) {
			for (int y = ry - 1; y < ry + rh + 1; ++y) {
				if (!map_tile(x, y)->solid) {
					overlap = 1;
					break;
				}
			}
		}
		if (overlap) {
			continue;
		}
		
		for (int x = rx; x < rx + rw; ++x) {
			for (int y = ry; y < ry + rh; ++y) {
				map_tile(x, y)->solid = 0;
			}
		}

		if (i == 0) {
			player_create(random() * rw + rx, random() * rh + ry);
		}

		int bx = random() * rw + rx;
		int by = random() * rh + ry;
		if (i > 0) {
			int dx = 1;
			if (bx > ex) {
				dx = -1;
			}
			for (int x = bx; x != ex; x += dx) {
				map_tile(x, by)->solid = 0;
			}

			int dy = 1;
			if (by > ey) {
				dy = -1;
			}
			for (int y = by; y != ey; y += dy) {
				map_tile(ex, y)->solid = 0;
			}
		}
		ex = bx;
		ey = by;
	}
}

void map_draw() {
	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			map_tile(x, y)->visible = 0;
		}
	}

	map_light(1, 1, 0);
	map_light(1, 1, 1);
	map_light(1, -1, 1);
	map_light(1, -1, 0);
	map_light(-1, -1, 0);
	map_light(-1, -1, 1);
	map_light(-1, 1, 1);
	map_light(-1, 1, 0);

	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			map_tile_t* tile = map_tile(x, y);
			if (tile->visible) {
				if (tile->solid) {
					stile(x, y, 0xDB | COLOR_LGRAY);
				} else {
					stile(x, y, 0xB1 | COLOR_GRAY);
				}
			} else if (tile->revealed) {
				if (tile->solid) {
					stile(x, y, 0xDB | COLOR_GRAY);
				} else {
					stile(x, y, 0xB0 | COLOR_GRAY);
				}
			} else {
				stile(x, y, 0);
			}
		}
	}
}