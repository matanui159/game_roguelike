#include "elem.h"
#include "../print.h"
#include "../random.h"
#include "../entity/entity.h"
#include "../entity/player.h"

static tile_t g_colors[] = {
	0,
	COLOR_YELLOW,
	COLOR_LRED,
	COLOR_LCYAN,
	COLOR_MAGENTA,
	COLOR_LGREEN,
	COLOR_WHITE
};

static const char* g_names[] = {
	"",
	"Critical ",
	"Fire ",
	"Ice ",
	"Lightning ",
	"Vampire ",
	"Random "
};

elem_t elem_create(int level, int damage) {
	elem_t elem;
	elem.type = random(6);
	elem.level = random(level / 8);
	if (elem.type == ELEM_NONE || elem.level == 0) {
		elem.type = ELEM_NONE;
		elem.damage = 0;
		elem.level = 0;
		elem.color = 0;
	} else {
		elem.damage = damage / 8;
		elem.color = g_colors[elem.type];
		if (elem.damage == 0) {
			elem.damage = 1;
		}
	}
	return elem;
}

void elem_print(int* x, int y, const elem_t* msg, const elem_t* cmp) {
	if (msg->type != ELEM_NONE) {
		print_string(x, y, g_names[msg->type], msg->color);
		if (msg->level > 0) {
			tile_t color = msg->color;
			if (cmp != NULL) {
				if (cmp->level > msg->level) {
					color = COLOR_GREEN;
				}
				if (cmp->level < msg->level) {
					color = COLOR_RED;
				}
			}
			print_int(x, y, msg->level, color);
		}
	}
}

void elem_attack(elem_t* elem, entity_t* entity, entity_t* target) {
	if (elem->type == ELEM_NONE) {
		return;
	}
	if (target->elem.type != ELEM_NONE && target->elem.level > 0) {
		return;
	}
	if (target->health == 0) {
		return;
	}
	
	_Bool player = entity == entity_get(0);
	target->elem = *elem;
	switch (elem->type) {
		case ELEM_CRITICAL:
			entity_damage(target, random(elem->damage * elem->level));
			target->elem.level = 0;
			break;
		case ELEM_LIGHTNING:
			if (player) {
				for (int i = 0; i < elem->level; ++i) {
					int count = 0;
					entity_t* t = NULL;
					for (int j = 0; j < ENTITY_LIMIT; ++j) {
						entity_t* e = entity_get(j);
						if (e != NULL && e->alive && e != entity && e != target) {
							if (e->elem.type == ELEM_NONE || e->elem.level == 0) {
								if (e->health > 0) {
									int dx = e->x - target->x;
									int dy = e->y - target->y;
									if (dx * dx + dy * dy < 16) {
										++count;
										if (random(count) == 0) {
											t = e;
										}
									}
								}
							}
						}
					}
					
					if (t != NULL) {
						t->elem = *elem;
						t->elem.level = 1;
						entity_damage(t, elem->damage);
					}
				}
				target->elem.level = 0;
			} else {
				target->elem.damage *= elem->level;
			}
			break;
		case ELEM_VAMPIRE:;
			entity_damage(entity, -elem->damage * elem->level);
			target->elem.level = 0;
			break;
		case ELEM_RANDOM:;
			target->elem.type = ELEM_NONE;
			elem->type = random(5) + 1;
			elem->color = g_colors[elem->type];
			elem_attack(elem, entity, target);
			elem->type = ELEM_RANDOM;
			elem->color = g_colors[elem->type];
			return;
	}
	++target->elem.level;
}