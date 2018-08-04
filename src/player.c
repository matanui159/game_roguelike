#include "player.h"
#include "entity.h"
#include "map.h"

static void player_update(entity_t* entity) {
	for (;;) {
		switch (dkey()) {
			case VK_LEFT:
				entity_move(entity, -1, 0);
				return;
			case VK_RIGHT:
				entity_move(entity, 1, 0);
				return;
			case VK_UP:
				entity_move(entity, 0, -1);
				return;
			case VK_DOWN:
				entity_move(entity, 0, 1);
				return;
			case VK_SPACE:
				return;
			case VK_OEM_3:
				map_create(1);
				return;
		}
	}
}

void player_create(int x, int y) {
	entity_t* entity = entity_create();
	entity->tile = 0x01 | FG_WHITE;
	entity->x = x;
	entity->y = y;
	entity->update = player_update;
}