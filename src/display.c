#include "display.h"
#include "../font/src.h"

#define CLASS_NAME "roguelike"

#define FONT_SIZE 8
#define SCALE (FONT_SIZE * 3)

#define CHAR_MASK 0x00FF
#define COLOR_MASK 0x0F00

void display_main();

static HWND g_window;
static HDC g_windc;
static HDC g_memdc;
static tile_t g_tiles[WIDTH * HEIGHT];
static _Bool g_sleep;

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

static void error() {
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
	char* error;
	if (FormatMessage(flags, NULL, GetLastError(), 0, (char*)&error, 0, NULL) != 0) {
		MessageBox(g_window, error, NULL, MB_ICONERROR);
		LocalFree(error);
	}
	ExitProcess(1);
}

static LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wpm, LPARAM lpm) {
	if (msg == WM_CLOSE) {
		ExitProcess(0);
	} else {
		return DefWindowProc(window, msg, wpm, lpm);
	}
}

void display__entry() {
	SetLastError(0);
	HINSTANCE instance = GetModuleHandle(NULL);

	HBITMAP bitmap = CreateBitmap(FONT_SIZE * 0xFF, FONT_SIZE, 1, 1, g_font);
	if (bitmap == NULL) {
		error();
	}

	WNDCLASS wndclass = {
		CS_OWNDC | CS_SAVEBITS,
		window_proc,
		0,
		0,
		instance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		NULL,
		NULL,
		CLASS_NAME
	};
	if (RegisterClass(&wndclass) == 0) {
		error();
	}

	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
	LONG width = WIDTH * SCALE;
	LONG height = HEIGHT * SCALE;
	LONG x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	LONG y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	RECT rect = {
		x,
		y,
		x + width,
		y + height
	};
	AdjustWindowRect(&rect, style, FALSE);

	g_window = CreateWindow(
		CLASS_NAME,
		"Roguelike",
		style,
		rect.left,
		rect.top,
		rect.right  - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		instance,
		NULL
	);
	if (g_window == NULL) {
		error();
	}

	g_windc = GetDC(g_window);
	g_memdc = CreateCompatibleDC(g_windc);
	SelectObject(g_memdc, bitmap);
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			display_set(x, y, 1);
			display_set(x, y, 0);
		}
	}

	display_main();
	ExitProcess(0);
}

int display_key() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		DispatchMessage(&msg);
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_OEM_3) {
				g_sleep = !g_sleep;
			} else {
				return msg.wParam;
			}
		}
	}
	error();
}

void display_set(int x, int y, tile_t tile) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}

	int index = y * WIDTH + x;
	tile_t told = g_tiles[index];
	tile_t tnew = tile;
	if ((tnew & COLOR_MASK) == COLOR_UNCHANGED) {
		tnew |= told & COLOR_MASK;
	}
	g_tiles[index] = tnew;

	if (tnew != told) {
		SetBkColor(g_windc, g_colors[(tnew & COLOR_MASK) >> 8]);
		StretchBlt(
			g_windc,
			x * SCALE,
			y * SCALE,
			SCALE,
			SCALE,
			g_memdc,
			(tnew & CHAR_MASK) * FONT_SIZE,
			0,
			FONT_SIZE,
			FONT_SIZE,
			SRCCOPY
		);
		if (g_sleep) {
			Sleep(2);
		}
	} else if (tile == 0) {
		tile = tile + 1;
	}
}

tile_t display_get(int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return COLOR_WHITE;
	}
	return g_tiles[y * WIDTH + x];
}