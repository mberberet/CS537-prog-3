#include <stdio.h>
#include <stdlib.h>
#include "RedBlackTree/rangeTree.h"
#include "malloc537.h"

void *realloc537(void *ptr, size_t size) {
    void *address = NULL;
    if (size == 0 && ptr == NULL) {
        // TODO Free Operation
        printf("Free\n");
        return NULL;
    } else if (ptr == NULL) {
        printf("Malloc\n");
        return malloc537(size);
    }
    if (rangeQuery((int) ptr, 0) != NULL) {
        // TODO Remove tuple
        printf("Existed. Freed.\n");
    }
    if ((address = realloc(ptr, size)) == NULL) {
        printf("Unable to allocate memory!\n");
        exit(-1);
    }
    addRange((int) address, size);
    return address;

}
