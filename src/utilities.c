#include "../include/utilities.h"

void *default_allocate(size_t size) {
    void *ptr = NULL;
    size_t attempts = 0;
    while (ptr == NULL && attempts < MAX_ATTEMPTS) {
        ptr = malloc(size);

        if (!ptr) {
            perror("Memory allocation failed\n");
            ++attempts;
        }
    }

    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed after %zu attemps\n", attempts);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void default_deallocate(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

allocator_t default_allocator = { default_allocate, default_deallocate };

void remove_char(char remove,char *str) {
    int j = 0;
    for_loop(i, strlen(str)) {
        if (str[i] != remove) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; 
}
