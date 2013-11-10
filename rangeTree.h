#include <stdint.h>

typedef struct rangeTree
{
	/*original starting point of the block*/
	uintptr_t addr;
	/*current starting point of block*/
	uintptr_t start;
	int len;
	/*children[0] is right, children[1] is left */
	struct rangeTree *children[2];
	int black;
	int free;
}rangeTree;

/* Every call will be a new node, no extending contiguous blocks*/
/*index off of start*/
int addRange(void* a, int length);
/*Might change return type to int for class security, but right now returns 
node that fits the range if it exists*/
/*index off of start*/
int rangeQuery(void* address, int length);
/*Only allow removing at start address
change to just set freed flag

Error cases
1. not allocated
2. not start address
3. double free



*/
/*index off address, must not be freed*/
int freeRange(void* address);

/*index off start*/
int isFreed(void* address);
void printLinear();
void deleteTree();
/*Gives total amount of dynamic memory allocated*/
int dynamicMemoryBytes();
void printRangeTree();
