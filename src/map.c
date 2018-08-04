#include "map.h"
#include "entity.h"
#include "player.h"
#include "random.h"

#define SHADOW_LIMIT 29

typedef struct shadow_t {
	_Bool used;
	float left;
	float right;
} shadow_t;

static map_tile_t g_map[MAP_WIDTH * MAP_HEIGHT];

static void map_light(int dx, int dy, _Bool flip) {
	shadow_t shadows[SHADOW_LIMIT] = {0};
	entity_t* player = entity_get(0);
	for (int row = 0; row < MAP_WIDTH; ++row) {
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
						tile->visible = 1;
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
		static map_tile_t empty = {0, 0};
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
		}
	}

	int ex = 0;
	int ey = 0;
	for (int i = 0; i < 100; ++i) {
		int w = random() * 4 + 4;
		int h = random() * 2 + 2;
		int x = random() * (MAP_WIDTH - w - 2) + 1;
		int y = random() * (MAP_HEIGHT - h - 2) + 1;

		_Bool empty = 1;
		for (int xx = x - 1; xx < x + w + 1; ++xx) {
			for (int yy = y - 1; yy < y + h + 1; ++yy) {
				map_tile_t* tile = map_tile(xx, yy);
				if (!tile->solid) {
					empty = 0;
				}
			}
		}
		if (!empty) {
			continue;
		}
		
		for (int xx = x; xx < x + w; ++xx) {
			for (int yy = y; yy < y + h; ++yy) {
				map_tile_t* tile = map_tile(xx, yy);
				tile->solid = 0;
			}
		}

		if (i == 0) {
			player_create(random() * w + x, random() * h + y);
		}

		int bx = random() * w + x;
		int by = random() * h + y;
		if (i > 0) {
			int dx = 1;
			if (bx > ex) {
				dx = -1;
			}
			for (int xx = bx; xx != ex; xx += dx) {
				map_tile_t* tile = map_tile(xx, by);
				tile->solid = 0;
			}

			int dy = 1;
			if (by > ey) {
				dy = -1;
			}
			for (int yy = by; yy != ey; yy += dy) {
				map_tile_t* tile = map_tile(ex, yy);
				tile->solid = 0;
			}
		}
		ex = bx;
		ey = by;
	}
}

void map_draw() {
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
					dtile(x, y, TILE_ALL, 0xDB | FG_LGRAY);
				} else {
					dtile(x, y, TILE_ALL, 0xB0 | FG_GRAY);
				}
			} else {
				dtile(x, y, TILE_ALL, 0);
			}
		}
	}
}