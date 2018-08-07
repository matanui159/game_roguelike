#include "map.h"
#include "entity/entity.h"
#include "entity/player.h"

void display_main() {
	player_init();
	map_init();
	for (;;) {
		map_draw();
		entity_draw();
		entity_update();
	}
}