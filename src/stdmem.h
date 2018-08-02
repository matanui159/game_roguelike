#ifndef STDMEM_H_
#define STDMEM_H_
#include <stddef.h>

void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

void* memset(void* ptr, int c, size_t size);
void* memcpy(void* dst, const void* src, size_t size);
void* memmove(void* dst, const void* src, size_t size);
int memcmp(const void* lhs, const void* rhs, size_t size);

#endif