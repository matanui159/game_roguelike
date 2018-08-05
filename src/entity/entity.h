#ifndef ENTITY_ENTITY_H_
#define ENTITY_ENTITY_H_
#include "../display.h"
#include "../item/item.h"

#define ENTITY_LIMIT 256

typedef struct entity_t entity_t;

typedef void (*entity_update_t)(entity_t* entity);

typedef struct entity_t {
	_Bool used;
	tile_t tile;
	int x;
	int y;
	_Bool alive;
	item_t weapon1;
	item_t weapon2;
	item_t armor;
	elem_t elem;
	int health;
	int prev_health;
	entity_update_t update;
} entity_t;

void entity_clear();
entity_t* entity_create();
entity_t* entity_get(int i);

void entity_draw();
void entity_update();
void entity_move(entity_t* entity, int dx, int dy);
void entity_damage(entity_t* entity, int damage);

#endif