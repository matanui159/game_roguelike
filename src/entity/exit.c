#include "exit.h"
#include "entity.h"
#include "../map.h"

static void exit_update(entity_t* entity) {
	map_create();
}

void exit_create(int x, int y) {
	entity_t* entity = entity_create();
	entity->tile = 0x19 | COLOR_CYAN;
	entity->x = x;
	entity->y = y;
	entity->alive = 0;
	entity->update = exit_update;
}