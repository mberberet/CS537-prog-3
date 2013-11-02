#include "malloc537.h"
#include "realloc537.h"
#include <stdio.h>
#include "RedBlackTree/rangeTree.h"

int main() {
/*    char *array = malloc537(10 * sizeof(char));
    int addr = (int) array;
    if (rangeQuery(addr, 2) != NULL) {
        printf("Found it!\n");
    } else {
        printf("Not Found\n");
    }*/
    char *array = NULL;
    array = realloc537(array, 10);
    if (rangeQuery((int) array, 1) != NULL) {
        printf("Found again!\n");
    } else {
        printf("Not Found again\n");
    }

    return 0;
}
