#ifndef CPSERVER_UTIL_H
#define CPSERVER_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define for_loop(i, n) for(int i = 0; i < (n); i++)
#define MAX_ATTEMPTS 10
#define SIZE_256 256
#define SIZE_1024 1024

typedef char* str;
typedef const char* c_str;

typedef struct allocator {
    void*(*allocate)(size_t size);
    void(*deallocate)(void* ptr);
} allocator_t;

extern allocator_t default_allocator;

void* default_allocate(size_t size);
void default_deallocate(void* ptr);
void remove_char(char remove, str str);

#endif