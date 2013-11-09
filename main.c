#include "malloc537.h"
#include "realloc537.h"
#include "free537.h"
#include "testcheck537.h"
#include <string.h>
#include <stdio.h>
#include "rangeTree.h"

int main() {
    char *array = NULL;
    array = malloc537(10);
//    free537(array);
    memcheck537(array, 100);
    if (rangeQuery(array, 2) == 1) {
        printf("Found it!\n");
    } else {
        printf("Not Found\n");
    }
//    free537(array);
//    char *array = NULL;
/*    array = realloc537(array, 20);
    if (rangeQuery(array, 1) == 1) {
        printf("Found again!\n");
    } else {
        printf("Not Found again\n");
    }
//    free537(array);*/

/*    char *buff = malloc537(8);
    testcheck537(buff, 5);
    // This is never happening.
    testcheck537(buff, 12);
    return 0;*/
    return 0;
}
