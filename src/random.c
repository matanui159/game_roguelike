#include "random.h"
#include <windows.h>

int random(int limit) {
	static unsigned __int64 state;
	static _Bool init = 0;
	if (!init) {
		init = 1;
		state = 0;
		random(1);
		state += GetTickCount();
		random(1);
	}

	unsigned __int64 old = state;
	state = state * 6364136223846793005ULL + 1442695040888963407ULL;
	unsigned __int32 xsh = ((old >> 18) ^ old) >> 27;
	unsigned __int32 rot = old >> 59;
	unsigned __int32 rnd = (xsh >> rot) | (xsh << (32 - rot));
	return rnd % limit;
}

int random_high(int limit) {
	int max = 0;
	for (int i = 0; i < 4; ++i) {
		int rnd = random(limit);
		if (rnd > max) {
			max = rnd;
		}
	}
	return max;
}