#include <stdlib.h>
#include <stdio.h>
#include "rangeTree.h"

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
