#include <stdio.h>
#include <stdlib.h>
#include "RedBlackTree/rangeTree.h"

void free537(void *ptr) {
    if (rangeQuery((int) ptr, 1) == NULL) {
        printf("Unable to free memory!\n");
        exit(-1);
    }
    // Do a check to see if the memory was already freed and report/exit if so.

    // Free the actual memory
    free(ptr);
    printf("Freed\n");
    // Remove ptr from range tree into already freed tree (or some other structure).
}
