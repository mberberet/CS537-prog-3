#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
