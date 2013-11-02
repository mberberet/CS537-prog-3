#include <stdlib.h>
#include <stdio.h>
#include "RedBlackTree/rangeTree.h"

void testcheck537(void *ptr, size_t size) {
    rangeTree *node = rangeQuery((int) ptr, size); // Never getting past here if size > range
    if (node == NULL) {
        printf("Memory at address not allocated\n");
        exit(-1);
    } else if (node->len < size) {
        printf("Size larger than range of memory allocated\n");
        exit(-1);
    }
    // Check if memory already freed and report if so and exit.*/
    return;
}
