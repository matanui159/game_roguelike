#include "entity.h"
#include "map.h"

static entity_t g_entities[ENTITY_LIMIT];

void entity_clear() {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		g_entities[i].used = 0;
	}
}

entity_t* entity_create() {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* entity = g_entities + i;
		if (!entity->used) {
			entity->used = 1;
			return entity;
		}
	}
	return NULL;
}

entity_t* entity_get(int i) {
	entity_t* entity = g_entities + i;
	if (entity->used) {
		return entity;
	} else {
		return NULL;
	}
}

void entity_draw() {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* entity = entity_get(i);
		if (entity != NULL && map_tile(entity->x, entity->y)->visible) {
			stile(entity->x, entity->y, entity->tile);
		}
	}
}

void entity_update() {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* entity = entity_get(i);
		if (entity != NULL) {
			entity->update(entity);
		}
	}
}

void entity_move(entity_t* entity, int dx, int dy) {
	entity->x += dx;
	entity->y += dy;
	if (map_tile(entity->x, entity->y)->solid) {
		entity->x -= dx;
		entity->y -= dy;
	}
}