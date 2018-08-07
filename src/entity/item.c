#include "item.h"
#include "entity.h"
#include "player.h"
#include "../map.h"
#include "../print.h"

static void item_pickup(entity_t* entity, item_t* slot) {
	item_t temp = *slot;
	*slot = entity->weapon1;
	if (temp.damage == 0) {
		entity->used = 0;
	} else {
		entity->weapon1 = temp;
	}
}

static void item_update(entity_t* entity) {
	entity_t* player = entity_get(0);
	item_print(MAP_HEIGHT, "Pickup: ", &entity->weapon1, NULL);
	if (entity->weapon1.type == ITEM_ARMOR) {
		item_print(MAP_HEIGHT + 1, "[Z] ", &player->armor, &entity->weapon1);
		int x = 0;
		print_finish(&x, MAP_HEIGHT + 2);
	} else {
		item_print(MAP_HEIGHT + 1, "[Z] ", &player->weapon1, &entity->weapon1);
		item_print(MAP_HEIGHT + 2, "[X] ", &player->weapon2, &entity->weapon1);
	}
	int x = 0;
	print_string(&x, MAP_HEIGHT + 3, "[C] Cancel", COLOR_WHITE);
	print_finish(&x, MAP_HEIGHT + 3);

	for (;;) {
		switch (display_key()) {
			case 'Z':
				if (entity->weapon1.type == ITEM_ARMOR) {
					item_pickup(entity, &player->armor);
					player->health = player->armor.damage * PLAYER_ARMOR;
					player->prev_health = player->health;
				} else {
					item_pickup(entity, &player->weapon1);
				}
				return;
			case 'X':
				if (entity->weapon1.type != ITEM_ARMOR) {
					item_pickup(entity, &player->weapon2);
					return;
				}
				break;
			case 'C':
				return;
		}
	}
}

void item_entity_create(int x, int y, item_t item) {
	entity_t* entity = entity_create();
	entity->tile = 'i' | COLOR_GREEN;
	entity->x = x;
	entity->y = y;
	entity->alive = 0;
	entity->weapon1 = item;
	entity->update = item_update;
}