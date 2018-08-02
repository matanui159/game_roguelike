#include "stdmem.h"
#include <windows.h>

void* malloc(size_t size) {
	return realloc(NULL, size);
}

void* realloc(void* ptr, size_t size) {
	static HANDLE heap = NULL;
	if (heap == NULL) {
		heap = GetProcessHeap();
	}

	if (ptr == NULL) {
		return HeapAlloc(heap, 0, size);
	}
	if (size == 0) {
		HeapFree(heap, 0, ptr);
		return NULL;
	}
	return HeapReAlloc(heap, 0, ptr, size);
}

void free(void* ptr) {
	realloc(ptr, 0);
}



void* memset(void* ptr, int c, size_t size) {
	char* cptr = ptr;
	for (int i = 0; i < size; ++i) {
		cptr[i] = c;
	}
	return ptr;
}

void* memcpy(void* dst, const void* src, size_t size) {
	return memmove(dst, src, size);
}

void* memmove(void* dst, const void* src, size_t size) {
	char* cdst = dst;
	const char* csrc = src;
	if (cdst < csrc) {
		for (int i = 0; i < size; ++i) {
			cdst[i] = csrc[i];
		}
	} else {
		for (int i = size - 1; i >= 0; --i) {
			cdst[i] = csrc[i];
		}
	}
	return dst;
}

int memcmp(const void* lhs, const void* rhs, size_t size) {
	const char* clhs = lhs;
	const char* crhs = rhs;
	for (int i = 0; i < size; ++i) {
		if (clhs[i] != crhs[i]) {
			return clhs[i] - crhs[i];
		}
	}
	return 0;
}