#include <stdio.h>
#include <stdlib.h>
#include "RedBlackTree/rangeTree.h"

void *malloc537(size_t size) {
    void *address = NULL;
    if (size <= 0) {
        printf("Can't have a negative size\n");
        return NULL;
    }
    if ((address = malloc(size)) == NULL) {
        printf("Unable to allocate memory\n");
        exit(-1);
    }
    addRange((int) address, size);
    return address;
}
