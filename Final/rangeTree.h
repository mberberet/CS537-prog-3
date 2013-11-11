/*/////////////////////////////////////////////////////////////////////////////
// File:             rangeTree.h
// Semester:         CS537 Fall 2013
//
// Author:           Adam Vander Pas
// Email:            azvanderpas@wisc.edu
// CS Login:         vander-p
// Lecturer's Name:  Barton Miller
//
// Partner:           Michael Berberet
//
// This is the header file of a range binary search tree for use in
// keeping track of dynamic memory. The tree is kept balanced by a red black
// tree implementation.
/////////////////////////////////////////////////////////////////////////////*/
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
	/*black = 1, red = 0*/
	int black;
	/*1 if deallocated, 0 if allocated on heap*/
	int free;
}rangeTree;

void addRange(void* a, int length);
int rangeQuery(void* address, int length);
int freeRange(void* address);
int isFreed(void* address);
void printLinear();
void deleteTree();
int dynamicMemoryBytes();
void printRangeTree();
