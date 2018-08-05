#include "map.h"
#include "entity/entity.h"

void display_main() {
	map_create();
	for (;;) {
		map_draw();
		entity_draw();
		entity_update();
	}
}