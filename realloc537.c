#include <stdio.h>
#include <stdlib.h>
#include "rangeTree.h"
#include "malloc537.h"
#include "free537.h"

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
