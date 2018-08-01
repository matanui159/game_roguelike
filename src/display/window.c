#include "../display.h"
#include "paint.h"

#define CLASS_NAME "roguelike"

static void error() {
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
	char* error;
	if (FormatMessage(flags, NULL, GetLastError(), 0, (char*)&error, 0, NULL) != 0) {
		MessageBox(NULL, error, NULL, MB_ICONERROR);
		LocalFree(error);
	}
	ExitProcess(1);
}

static LRESULT CALLBACK window_proc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	static int fun = 0;

	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(wnd, msg, wpm, lpm);
	}
	return 0;
}

void entry() {
	SetLastError(0);
	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wndclass = {CS_OWNDC | CS_SAVEBITS};
	wndclass.lpszClassName = CLASS_NAME;
	wndclass.hInstance = instance;
	wndclass.lpfnWndProc = window_proc;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
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

	HWND window = CreateWindow(
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
	if (window == NULL) {
		error();
	}

	if (!paint_init(window)) {
		error();
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		DispatchMessage(&msg);
	}

	ExitProcess(0);
}