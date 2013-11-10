#include <stdio.h>
#include <stdlib.h>
#include "537malloc.h"
#include "rangeTree.h"

void *malloc537(size_t size) {
    void *address = NULL;
    if ((int) size < 0) {
        printf("Can't have a negative size.\n");
        exit(-1);
    } else if (size == 0) {
        printf("Allocating a range of size of 0.\n");
    }
    // Is this a case?
    if ((address = malloc(size)) == NULL && size > 0) {
        printf("Unable to allocate memory.\n");
        exit(-1);
    }
    if (size > 0) {
        addRange(address, size);
    }
    return address;
}

void free537(void *ptr) {
    int error;
    error = freeRange(ptr);
    switch (error) {
        case -1:
            printf("Attempting to free memory that was never allocated.\n");
            exit(-1);
            break;
        case -2:
            printf("Attempting to free memory that is not the first byte of its range.\n");
            exit(-1);
            break;
        case -3:
            printf("Attempting to free previously freed memory.\n");
            exit(-1);
            break;
        case -4:
            printf("Attempting to either free previously freed memory or free memory that is not the first byte of its range.\n");
            exit(-1);
            break;
    }
    // Free the actual memory
    free(ptr);
    printf("Freed\n");
}

void *realloc537(void *ptr, size_t size) {
    void *address = NULL;

    if (size == 0 && ptr != NULL) {
        free537(ptr);
        printf("Free\n");
        return NULL;
    } else if (ptr == NULL) {
        return malloc537(size);
    }
    if (rangeQuery(ptr, 1) == 1) {
        free537(ptr);
        printf("Existed. Freed.\n");
    }
    if ((address = realloc(ptr, size)) == NULL && size > 0) {
        printf("Unable to allocate memory!\n");
        exit(-1);
    }
    if (size > 0) {
        addRange(address, size);
    }
    return address;

}

void memcheck537(void *ptr, size_t size) {
    int error = rangeQuery(ptr, size);
    if (error == -1) {
        printf("Memcheck: Desired address is not allocated.\n");
        exit(-1);
    } else if (error == -2) {
        printf("Memcheck: Desired size falls outsides of bounds of range.\n");
        exit(-1);
    } else if (error == -3) {
        printf("Memcheck: Desired address has been freed.\n");
        exit(-1);
    }
    return;
}
