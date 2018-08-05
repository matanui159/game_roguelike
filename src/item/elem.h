#ifndef ITEM_ELEM_H_
#define ITEM_ELEM_H_
#include "../display.h"

#define ELEM_NONE 0
#define ELEM_CRITICAL 1
#define ELEM_FIRE 2
#define ELEM_ICE 3
#define ELEM_LIGHTNING 4
#define ELEM_VAMPIRE 5
#define ELEM_RANDOM 6

typedef struct entity_t entity_t;

typedef struct elem_t {
	int type;
	int damage;
	int level;
	tile_t color;
} elem_t;

elem_t elem_create(int level, int damage);
void elem_print(int* x, int y, const elem_t* msg, const elem_t* cmp);
void elem_attack(elem_t* elem, entity_t* entity, entity_t* target);

#endif