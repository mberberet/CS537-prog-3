/*This is just to test the red-black tree*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rangeTree.h"

int main(int argc, char **argv)
{
	uintptr_t i;
	int y;
	uintptr_t x;
	i=0;
	while(i<atoi(argv[1]))
	{
		x=i*500;
		addRange((void*)x, 400);
		i++;
	}
	
	printf("Free result: %d\n", freeRange((void*)(i/2*500)));
	printRangeTree();
	addRange((void*)(i*500/2-25), 100); //Before to between
	printRangeTree();
	addRange((void*)(i*500/2+125), 2); //After to Between
	printRangeTree();
	printf("About to add %X,%X\n", i*500/2+128, i*500/2+128 + 500-1);
	fflush(stdout);
	addRange((void*)(i*500/2+128), 500); //after to after
	printRangeTree();
	printf("About to add %X,%X\n", i*500/2, i*500/2 +4);
	fflush(stdout);
	addRange((void*)(i*500/2), 5); //should already exist
	printRangeTree();
	printf("About to add %X,%X\n", 0x627, 0x627 +(0x658-0x627));
	fflush(stdout);
	addRange((void*)0x627, (0x658-0x627+1)); //should remove freed node
	printRangeTree();
	
	
	return 0;
}
