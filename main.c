#include "malloc537.h"
#include "realloc537.h"
#include "free537.h"
#include "testcheck537.h"
#include <string.h>
#include "RedBlackTree/rangeTree.h"

int main() {
/*    char *array = malloc537(10 * sizeof(char));
    int addr = (int) array;
    if (rangeQuery(addr, 2) != NULL) {
        printf("Found it!\n");
    } else {
        printf("Not Found\n");
    }
    char *array = NULL;
    array = realloc537(array, 10);
    if (rangeQuery((int) array, 1) != NULL) {
        printf("Found again!\n");
    } else {
        printf("Not Found again\n");
    }
    free537(array);
*/
    char *buff = malloc537(8);
    testcheck537(buff, 5);
    // This is never happening.
    testcheck537(buff, 12);
    return 0;
}
