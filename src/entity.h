#ifndef ENTITY_H_
#define ENTITY_H_
#include "display.h"

#define ENTITY_LIMIT 256

typedef struct entity_t entity_t;

typedef void (*entity_update_t)(entity_t* entity);

typedef struct entity_t {
	_Bool used;
	tile_t tile;
	int x;
	int y;
	entity_update_t update;
} entity_t;

void entity_clear();
entity_t* entity_create();
entity_t* entity_get(int i);

void entity_draw();
void entity_update();

void entity_move(entity_t* entity, int dx, int dy);

#endif