#include "display.h"
#include "../font/src.h"
#include "stdmem.h"

#define CLASS_NAME "roguelike"

#define FONT_SIZE 8
#define SCALE (FONT_SIZE * 3)

void dmain();

static HWND g_window;
static HDC g_windc;
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
		CreateSolidBrush(0),
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
	dmain();
	ExitProcess(0);
}

int dkey() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		DispatchMessage(&msg);
		if (msg.message == WM_KEYDOWN) {
			return msg.wParam;
		}
	}
	error();
}

tile_t dtile(int x, int y, tile_t clr, tile_t flip) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return 0;
	}

	int index = y * WIDTH + x;
	tile_t told = g_tiles[index];
	tile_t tnew = (told & ~clr) ^ flip;
	g_tiles[index] = tnew;

	if (tnew != told) {
		SetTextColor(g_windc, g_colors[(tnew & TILE_BG) >> 8]);
		SetBkColor(g_windc, g_colors[(tnew & TILE_FG) >> 12]);
		StretchBlt(
			g_windc,
			x * SCALE,
			y * SCALE,
			SCALE,
			SCALE,
			g_memdc,
			(tnew & TILE_CHAR) * FONT_SIZE,
			0,
			FONT_SIZE,
			FONT_SIZE,
			SRCCOPY
		);
	}
	return told;
}