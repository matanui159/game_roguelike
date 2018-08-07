#ifndef MAP_H_
#define MAP_H_

#define MAP_WIDTH WIDTH
#define MAP_HEIGHT (HEIGHT - 4)

typedef struct map_tile_t {
	_Bool solid:1;
	_Bool visible:1;
	_Bool revealed:1;
} map_tile_t;

map_tile_t* map_get(int x, int y);
void map_init();
void map_create();
void map_draw();

#endif