#include "monster.h"
#include "entity.h"
#include "../map.h"

static int monster_abs(int val) {
	if (val < 0) {
		val = -val;
	}
	return val;
}

static void monster_attack(entity_t* entity, entity_t* player) {
	item_attack(&entity->weapon1, entity, player);
	if (entity->elem.type == ELEM_NONE) {
		elem_attack(&player->armor.elem, player, entity);
	}
}

static void monster_update(entity_t* entity) {
	entity_t* player = entity_get(0);
	if (map_get(entity->x, entity->y)->visible) {
		int dx = player->x - entity->x;
		int dy = player->y - entity->y;
		int ax = monster_abs(dx);
		int ay = monster_abs(dy);
		if (ax == 0 || ay == 0) {
			if (entity->weapon1.type == ITEM_BOW || ax == 1 || ay == 1) {
				monster_attack(entity, player);
				return;
			}
		}

		if (ay == 0 || (ax > 0 && ax < ay)) {
			if (dx < 0) {
				entity_move(entity, -1, 0);
			} else {
				entity_move(entity, 1, 0);
			}
		} else if (dy < 0) {
			entity_move(entity, 0, -1);
		} else {
			entity_move(entity, 0, 1);
		}
	}
}

void monster_create(int x, int y, int level) {
	entity_t* entity = entity_create();
	entity->x = x;
	entity->y = y;
	entity->alive = 1;
	entity->weapon1 = item_create(level, 0);
	entity->armor = item_create(level, 1);
	entity->elem.type = ELEM_NONE;
	entity->health = entity->armor.damage;
	entity->prev_health = entity->health;
	if (entity->weapon1.type == ITEM_SWORD) {
		entity->tile = 'S' | COLOR_WHITE;
	} else {
		entity->tile = 'B' | COLOR_WHITE;
	}
	entity->update = monster_update;
}