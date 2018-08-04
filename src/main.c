#include "map.h"
#include "entity.h"

void dmain() {
	map_create(1);
	for (;;) {
		map_draw();
		entity_draw();
		entity_update();
	}
}