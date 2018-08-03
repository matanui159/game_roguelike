#include "display.h"
#include "map.h"

void dmain(int key, void* data) {
	switch (key) {
		case VK_INIT:
			map_init();
			break;
		case VK_DRAW:
			map_draw();
			break;
	}
}