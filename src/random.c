#include "random.h"
#include <windows.h>

static _Bool g_init;
static unsigned __int64 g_state;

float random() {
	if (!g_init) {
		g_init = 1;
		g_state = 0;
		random();
		g_state += GetTickCount();
		random();
	}

	unsigned __int64 old = g_state;
	g_state = g_state * 6364136223846793005ULL + 1442695040888963407ULL;
	unsigned __int32 xsh = ((old >> 18) ^ old) >> 27;
	unsigned __int32 rot = old >> 59;
	unsigned __int32 rnd = (xsh >> rot) | (xsh << (32 - rot));
	return (float)rnd / 0x100000000ULL;
}