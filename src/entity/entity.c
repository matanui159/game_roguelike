#include "entity.h"
#include "item.h"
#include "../map.h"
#include "../random.h"

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
	return g_entities + ENTITY_LIMIT - 1;
}

entity_t* entity_get(int i) {
	entity_t* entity = g_entities + i;
	if (entity->used) {
		return entity;
	} else {
		return NULL;
	}
}

static void entity_draw_layer(_Bool alive) {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* entity = entity_get(i);
		if (entity != NULL && entity->alive == alive) {
			if (map_get(entity->x, entity->y)->visible) {
				tile_t tile = entity->tile;
				if (alive) {
					if (entity->health < entity->prev_health) {
						int damage = 10 - entity->health * 10 / entity->prev_health;
						if (damage > 9) {
							tile = (tile & 0xFF00) | '!';
						} else {
							tile = (tile & 0xFF00) | (damage + '0');
						}
					}
					entity->prev_health = entity->health;

					if (entity->health == 0) {
						tile = (tile & 0x00FF) | COLOR_RED;
					} else if (entity->elem.type != ELEM_NONE) {
						tile = (tile & 0x00FF) | entity->elem.color;
						if (entity->elem.level > 0) {
							if (entity->elem.type == ELEM_FIRE) {
								entity_damage(entity, entity->elem.damage);
							}
							--entity->elem.level;
						} else {
							entity->elem.type = ELEM_NONE;
						}
					}
				}
				display_set(entity->x, entity->y, tile);
			}
		}
	}
}

void entity_draw() {
	entity_draw_layer(0);
	entity_draw_layer(1);
}

void entity_update() {
	for (int i = 0; i < ENTITY_LIMIT; ++i) {
		entity_t* entity = entity_get(i);
		if (entity != NULL && entity->alive) {
			entity->update(entity);
		}
	}
}

void entity_move(entity_t* entity, int dx, int dy) {
	if (entity->elem.type != ELEM_ICE || entity->elem.level == 0) {
		entity->x += dx;
		entity->y += dy;

		_Bool collide = map_get(entity->x, entity->y)->solid;
		if (!collide) {
			for (int i = 0; i < ENTITY_LIMIT; ++i) {
				entity_t* e = entity_get(i);
				if (e != NULL && e->alive && e != entity) {
					if (e->x == entity->x && e->y == entity->y) {
						collide = 1;
						break;
					}
				}
			}
		}

		if (collide) {
			entity->x -= dx;
			entity->y -= dy;
		}
	}
}

void entity_damage(entity_t* entity, int damage) {
	if (entity->health > 0) {
		entity->health -= damage;
		if (entity->health <= 0) {
			entity->health = 0;
			entity->elem.type = ELEM_NONE;
		}
	} else if (entity->prev_health == 0) {
		item_t* item = &entity->weapon1;
		if (random(2) == 0) {
			item = &entity->armor;
		}
		if (item->elem.type != ELEM_NONE && random(16) == 0) {
			item->elem.type = ELEM_RANDOM;
			item->elem.color = COLOR_WHITE;
		}
		item_entity_create(entity->x, entity->y, *item);
		entity->used = 0;
	}
}