#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define for_loop(i, n) for(int i = 0; i < (n); i++)
#define MAX_ATTEMPTS 10
#define SIZE_256 256
#define SIZE_1024 1024

typedef struct allocator {
    void*(*allocate)(size_t size);
    void(*deallocate)(void* ptr);
} allocator_t;

extern allocator_t default_allocator;

void *default_allocate(size_t);
void default_deallocate(void*);
void remove_char(char, char*);

#endif