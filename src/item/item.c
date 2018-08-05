#include "item.h"
#include "../print.h"
#include "../random.h"
#include "../entity/entity.h"

static const char* g_names[] = {
	"Armor ",
	"Sword ",
	"Bow "
};

item_t item_create(int level, _Bool armor) {
	item_t item;
	if (armor) {
		item.type = ITEM_ARMOR;
	} else {
		item.type = random(2) + 1;
	}

	item.damage = random_high(level);
	if (item.type == ITEM_BOW) {
		item.damage /= 2;
	}
	item.elem = elem_create(level, item.damage);
	item.damage -= item.elem.damage * item.elem.level / 2;
	if (item.damage <= 0) {
		item.damage = 1;
	}
	return item;
}

void item_print(int y, const char* prefix, const item_t* msg, const item_t* cmp) {
	int x = 0;
	print_string(&x, y, prefix, COLOR_WHITE);

	tile_t color = COLOR_WHITE;
	if (cmp != NULL) {
		if (cmp->type > msg->type) {
			color = COLOR_GREEN;
		}
		if (cmp->type < msg->type) {
			color = COLOR_RED;
		}
	}
	print_string(&x, y, g_names[msg->type], color);

	color = COLOR_WHITE;
	if (cmp != NULL) {
		if (cmp->damage > msg->damage) {
			color = COLOR_GREEN;
		}
		if (cmp->damage < msg->damage) {
			color = COLOR_RED;
		}
	}
	print_int(&x, y, msg->damage, color);

	if (msg->elem.type != ELEM_NONE) {
		const elem_t* elem = NULL;
		if (cmp != NULL) {
			elem = &cmp->elem;
		}
		print_string(&x, y, " <", COLOR_WHITE);
		elem_print(&x, y, &msg->elem, elem);
		print_char(&x, y, '>', COLOR_WHITE);
	}
	print_finish(&x, y);
}

void item_attack(item_t* item, entity_t* entity, entity_t* target) {
	entity_damage(target, item->damage);
	if (target->health > 0) {
		elem_attack(&item->elem, entity, target);
	}
}