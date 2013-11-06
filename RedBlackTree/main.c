/*This is just to test the red-black tree*/

#include <stdlib.h>
#include <stdio.h>
#include "rangeTree.h"

int main(int argc, char **argv)
{
	int i;
	int y;
	i=0;
	while(i<atoi(argv[1]))
	{
		void *x;
		x=malloc(400);
		if(!x)
		{
			break;
		}
		addRange(i*400, 200);
		y = rangeQuery(i*400, 200);
		printf("%d\n", y);
		i++;
	}
	printRangeTree();
	printf("Attempting to remove 0x%X\n", 0x0);
	printf("%d\n", dynamicMemoryBytes());
	removeRange(0x0, 200);
	//addRange(i*400, 200);
	printf("%d\n", dynamicMemoryBytes());
	printRangeTree();
	return 0;
}
