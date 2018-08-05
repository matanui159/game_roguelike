#ifndef ITEM_ITEM_H_
#define ITEM_ITEM_H_
#include "elem.h"

#define ITEM_ARMOR 0
#define ITEM_SWORD 1
#define ITEM_BOW 2

typedef struct item_t {
	int type;
	int damage;
	elem_t elem;
} item_t;

item_t item_create(int level, _Bool armor);
void item_print(int y, const char* prefix, const item_t* msg, const item_t* cmp);
void item_attack(item_t* item, entity_t* entity, entity_t* target);

#endif