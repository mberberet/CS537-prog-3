#include <stdio.h>
#include <stdlib.h>
#include "rangeTree.h"

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
