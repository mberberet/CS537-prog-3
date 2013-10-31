/*This is just to test the red-black tree*/

#include <stdlib.h>
#include <stdio.h>
#include "rangeTree.h"

int main(int argc, char **argv)
{
	int i;
	rangeTree* y;
	i=0;
	while(i<atoi(argv[1]))
	{
		void *x;
		x=malloc(400);
		if(!x)
		{
			break;
		}
		addRange(i*400, 350);
		y = rangeQuery(i*400, 200);
		if(y)
		{
			printf("Found in tree:%d\n", i*400);
		}
		else
		{
			printf("Not found in tree:%d\n", i*400);
		}
		printf("%d\n", dynamicMemoryBytes());
		i++;
	}
	y = rangeQuery(3200, 500);
	printRangeTree();
	return 0;
}
