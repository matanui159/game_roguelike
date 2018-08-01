#include "paint.h"
#include "font.h"

static HDC g_dc;
static HDC g_memdc;
static tile_t g_tiles[WIDTH * HEIGHT];

static const COLORREF g_colors[] = {
	0x00000000,
	0x00AA0000,
	0x0000AA00,
	0x00AAAA00,
	0x000000AA,
	0x00AA00AA,
	0x000055AA,
	0x00AAAAAA,
	0x00555555,
	0x00FF5555,
	0x0055FF55,
	0x00FFFF55,
	0x005555FF,
	0x00FF55FF,
	0x0055FFFF,
	0x00FFFFFF
};

_Bool paint_init(HWND wnd) {
	g_dc = GetDC(wnd);

	HBITMAP bitmap = CreateBitmap(FONT_SIZE * 0xFF, FONT_SIZE, 1, 1, g_font);
	if (bitmap == NULL) {
		return 0;
	}

	g_memdc = CreateCompatibleDC(g_dc);
	SelectObject(g_memdc, bitmap);

	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			tile(x, y, TILE_ALL, 0);
		}
	}
	return 1;
}

void tile(int x, int y, tile_t tand, tile_t txor) {
	int index = y * WIDTH + x;
	tile_t tile = (g_tiles[index] & tand) ^ txor;
	g_tiles[index] = tile;

	SetTextColor(g_dc, g_colors[(tile & TILE_BG) >> 8]);
	SetBkColor(g_dc, g_colors[(tile & TILE_FG) >> 12]);
	StretchBlt(
		g_dc,
		x * SCALE,
		y * SCALE,
		SCALE,
		SCALE,
		g_memdc,
		(tile & TILE_CHAR) * FONT_SIZE,
		0,
		FONT_SIZE,
		FONT_SIZE,
		SRCCOPY
	);
}