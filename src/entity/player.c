#include "player.h"
#include "entity.h"
#include "../map.h"
#include "../print.h"

#define PLAYER_TILE (0x01 | COLOR_WHITE)

static void player_attack(entity_t* entity, item_t* item, int dx, int dy) {
	int range = 1;
	if (item->type == ITEM_BOW) {
		range = 8;
	}
	for (int i = 1; i <= range; ++i) {
		for (int j = 0; j < ENTITY_LIMIT; ++j) {
			entity_t* e = entity_get(j);
			if (e != NULL && e->alive) {
				if (e->x == entity->x + i * dx && e->y == entity->y + i * dy) {
					item_attack(item, entity, e);
					return;
				}
			}
		}
	}
}

static void player_dir(entity_t* entity, char state, int dx, int dy) {
	switch (state) {
		case 'Z':
			player_attack(entity, &entity->weapon1, dx, dy);
			break;
		case 'X':
			player_attack(entity, &entity->weapon2, dx, dy);
			break;
		case 'C':
			entity_move(entity, dx, dy);
			break;
	}
}

static void player_keys(entity_t* entity) {
	char state = 'C';
	for (;;) {
		int key = display_key();
		switch (key) {
			case VK_LEFT:
				player_dir(entity, state, -1, 0);
				return;
			case VK_RIGHT:
				player_dir(entity, state, 1, 0);
					return;
			case VK_UP:
				player_dir(entity, state, 0, -1);
				return;
			case VK_DOWN:
				player_dir(entity, state, 0, 1);
				return;
			case VK_SPACE:
				return;

			case 'Z':
			case 'X':
				display_set(entity->x, entity->y, key);
				state = key;
				break;
			case 'C':
				if (state == 'C') {
					for (int i = 0; i < ENTITY_LIMIT; ++i) {
						entity_t* e = entity_get(i);
						if (e != NULL && !e->alive) {
							if (e->x == entity->x && e->y == entity->y) {
								e->update(e);
							}
						}
					}
					return;
				} else {
					display_set(entity->x, entity->y, entity->tile & 0xFF);
					state = 'C';
				}
				break;
		}
	}
}

static void player_update(entity_t* entity) {
	entity->armor.damage = entity->health / PLAYER_ARMOR;
	item_print(MAP_HEIGHT, "[Z] ", &entity->weapon1, NULL);
	item_print(MAP_HEIGHT + 1, "[X] ", &entity->weapon2, NULL);
	item_print(MAP_HEIGHT + 2, "    ", &entity->armor, NULL);
	int x = 0;
	print_string(&x, MAP_HEIGHT + 3, "    ", 0);
	if (entity->health == 0) {
		print_string(&x, MAP_HEIGHT + 3, "Vulnerable", COLOR_RED);
	} else {
		elem_print(&x, MAP_HEIGHT + 3, &entity->elem, NULL);
	}
	print_finish(&x, MAP_HEIGHT + 3);

	player_keys(entity);
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* e = entity_get(i);
		if (e != NULL && !e->alive) {
			if (e->x == entity->x && e->y == entity->y) {
				entity->tile = 'C' | COLOR_WHITE;
				return;
			}
		}
	}
	entity->tile = PLAYER_TILE;
}

static entity_t* player_get() {
	entity_t* entity = entity_get(0);
	if (entity == NULL) {
		entity = entity_create();
	}
	return entity;
}

void player_init() {
	entity_t* entity = player_get();
	entity->tile = PLAYER_TILE;
	entity->alive = 1;

	entity->weapon1.type = ITEM_SWORD;
	entity->weapon1.damage = 4;
	entity->weapon1.elem.type = ELEM_NONE;

	entity->weapon2.type = ITEM_BOW;
	entity->weapon2.damage = 2;
	entity->weapon2.elem.type = ELEM_NONE;

	entity->armor.type = ITEM_ARMOR;
	entity->armor.damage = 8;
	entity->armor.elem.type = ELEM_NONE;

	entity->health = entity->armor.damage * PLAYER_ARMOR;
	entity->prev_health = entity->health;
	entity->score = 0;
	entity->update = player_update;
}

void player_create(int x, int y) {
	entity_t* entity = player_get();
	entity->x = x;
	entity->y = y;
}