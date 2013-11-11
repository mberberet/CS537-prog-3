/*/////////////////////////////////////////////////////////////////////////////
// File:             rangeTree.c
// Semester:         CS537 Fall 2013
//
// Author:           Adam Vander Pas
// Email:            azvanderpas@wisc.edu
// CS Login:         vander-p
// Lecturer's Name:  Barton Miller
//
// Partner:           Michael Berberet
//
// This is the implementation of a range binary search tree for use in
// keeping track of dynamic memory. The tree is kept balanced by a red black
// tree implementation.
/////////////////////////////////////////////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rangeTree.h"

#define RIGHT children[0]
#define LEFT children[1]

static rangeTree *head;

/**
 * Finds a node in the tree that fully contains the 
 * (address, length) tuple, if one exists. The tree's head
 * pointer's color gets set double black if the range is only
 * partially included. 
 *
 * @param address - integer representing starting address
 * @param length - length the of address block being looked for
 * @return Returns rangeTree pointer of found node. NULL if not found in tree
 */	
static rangeTree *getRangeNode(uintptr_t address, int length)
{
	rangeTree *tmp;
	if(length<=0)
	{
		return NULL;
	}

	tmp = head;
	while(tmp)
	{
		if(address == tmp->addr && address != tmp->start)
		{
			
		}
		if(address > tmp->start + tmp->len-1)
		{
			tmp= tmp->RIGHT;
		}
		else if (address + length-1 < tmp->start)
		{
			tmp = tmp->LEFT;
		}
		else
		{
			break;
		}
	}
	if(!tmp)
	{
		return NULL;
	}
	else
	{
		if(address >= tmp->start && address+length-1 <= tmp->start + tmp->len-1) 
		{
			return tmp;
		}
		else
		{
			head->black = 2;
			return tmp;
		}
	}
	return head;
}

/**
 * Returns whether or not an address block is freed.
 *
 * @param address - starting address pointer
 * @return Returns 1 if freed, 0 if allocated, -1 if node doesn't exist
 */	
int isFreed(void* address)
{
	rangeTree *x = getRangeNode((uintptr_t)address, 1);
	if(x)
	{
		return x->free;
	}
	else
	{
		return -1;
	}
}

/**
 * Gets the parent of a rangeTree node if one exists.
 *
 * @param child - child rangeTree node
 * @return pointer to parent node, or NULL if none exists
 */	
static rangeTree* getParent(rangeTree *child)
{
	rangeTree *tmp;
	tmp = head;
	if(!child || head == child )
	{
		return NULL;
	}
	while(tmp && tmp->RIGHT != child && tmp->LEFT != child)
	{
		if(child->start > tmp->start + tmp->len -1)
		{
			/*Go right*/
			tmp=tmp->RIGHT;
		}
		else if (child->start + child->len-1 < tmp->start)
		{
			/*Go left*/
			tmp = tmp->LEFT;
		}
	}
	return tmp;
}


/**
 * Helper function to fix double black nodes after removing nodes from the tree
 *
 * @param left - 1 if double black not is parent's left child, 0 if it is right child
 * @param parent - pointer to double black node's parent
 * @param root - pointer to parent's parent for use of connecting back to tree.
 */	
static void fixDoubleBlack(int left, rangeTree* parent, rangeTree *root)
{
	int topLeft = root ? (root->LEFT == parent) : 2;
	if(left<2)
	{	/*red sibling*/
		if(parent->children[!left] && !parent->children[!left]->black)
		{
			/*parent isn't head*/
			if(topLeft<2)
			{
				parent->children[!left]->black = parent->black;
				root->children[topLeft] = parent->children[!left];
				parent->children[!left] = parent->children[!left]->children[left];
				root->children[topLeft]->children[left] = parent;
				parent->black=0;
				if(root->children[topLeft])
					root->children[topLeft]->black=1;
				fixDoubleBlack(left, parent, root->children[topLeft]);
			}
			/*parent is head*/
			else
			{
				parent->children[!left]->black = parent->black;
				head = parent->children[!left];
				parent->children[!left] = parent->children[!left]->children[left];
				head->children[left] = parent;
				parent->black=0;
				head->black=1;
				fixDoubleBlack(left, parent, head);
			}
		}
		else
		{
			/*black sibling with red child on opposite side*/			
			if(parent->children[!left]->children[!left] && !parent->children[!left]->children[!left]->black)
			{
				/*parent isn't head*/
				if(topLeft<2)
				{ 	
					parent->children[!left]->black = parent->black;
					parent->black = 1;
					root->children[topLeft] = parent->children[!left];
					parent->children[!left] = root->children[topLeft]->children[left];
					root->children[topLeft]->children[left] = parent;
					if(root->children[topLeft]->children[!left])
						root->children[topLeft]->children[!left]->black=1;
					if(parent->children[left])
						parent->children[left]->black=1;
				}
				/*parent is head*/
				else
				{
					parent->children[!left]->black = parent->black;
					parent->black = 1;
					head = parent->children[!left];
					parent->children[!left] = head->children[left];
					head->children[left] = parent;
					if(head->children[!left])
						head->children[!left]->black=1;
					if(parent->children[left])
						parent->children[left]->black=1;
				}
			}
			/*black sibling with red child on same side*/
			else if(parent->children[!left]->children[left] && !parent->children[!left]->children[left]->black)
			{
				/*parent isn't head*/
				if(topLeft<2)
				{
					parent->children[!left]->black = parent->black;
					parent->black = 1;
					root->children[topLeft] = parent->children[!left]->children[left];
					parent->children[!left]->children[left] = root->children[topLeft]->children[!left];
					root->children[topLeft]->children[!left] = parent->children[!left];
					parent->children[!left] = root->children[topLeft]->children[left];
					root->children[topLeft]->children[left] = parent;
					if(root->children[topLeft])
						root->children[topLeft]->black=1;
					if(parent->children[left])
						parent->children[left]->black=1;
				}
				/*parent is head*/
				else
				{
					parent->children[!left]->black = parent->black;
					parent->black = 1;
					head = parent->children[!left]->children[left];
					parent->children[!left]->children[left] = head->children[!left];
					head->children[!left] = parent->children[!left];
					parent->children[!left] = head->children[left];
					head->children[left] = parent;
					head->black=1;
					if(parent->children[left])
						parent->children[left]->black=1;
				}
			}
			/*black sibling with black children*/
			else
			{
				/*Recolor and recurse*/
				(parent->black)++;
				(parent->children[!left]->black)--;
				if(parent->black==2)
				{
					rangeTree *y = getParent(parent);
					if(!y)
					{
						parent->black=1;
						return;
					}
					left = y->LEFT == parent;
					parent->black=1;
					fixDoubleBlack(left, y, getParent(y));
				}
			}
		}
	}
	else
	{
		printf("Double black node is top of tree. Something went wrong");
		exit(2);
	}
}

/**
 * Function to remove a rangeTree node from the tree.
 *
 * @param x - node to remove
 */	

void removeRange(rangeTree *x)
{
	
	rangeTree* tmp;
	rangeTree* parent;
	int left;
	int deletedColor;
	if(x)
	{	
		if(x->addr == head->addr && !x->LEFT && !x->RIGHT)
		{
			free(head);
			head = NULL;
			return;
		}
		/*remove node and follow red black tree algorithm*/
		/*If node has less than 2 children*/
		if(!x->LEFT || !x->RIGHT)
		{			
			parent = getParent(x);
			left = (parent && parent->LEFT == x);
			/*replace node with child (null or otherwise)*/
			if(parent)
			{
				parent->children[left] = (x->LEFT) ? x->LEFT : x->RIGHT;
				tmp = parent->children[left];
				deletedColor = x->black;
				free(x);
				x = NULL;
			}
			/*At most two nodes in tree and removing head node*/
			else
			{
				head = (x->LEFT) ? x->LEFT : x->RIGHT;
				head->black=1;
				free(x);
				x = NULL;
				return;
			}
		}
		else
		{
			/*switch node with in order successor(and switch colors as well)*/
			tmp = x;
			x = x->LEFT;
			while(x->RIGHT)
			{
				x = x->RIGHT;
			}
			parent = getParent(x);
			tmp->start = x->start;
			tmp->free = x->free;
			tmp->addr = x->addr;
			tmp->len = x->len;
			left = (parent && parent->LEFT == x);
			
			/*replace node with child*/
			if(parent)
			{
				parent->children[left] = (x->LEFT) ? x->LEFT : x->RIGHT;
				tmp = parent->children[left];
				deletedColor = x->black;
				free(x);
				x=NULL;
			}
			/*Should never happen*/
			else
			{
				printf("Something went wrong");
				return;
			}		
		}
		/*If node deleted was red*/
		if(!deletedColor)
		{
			/*color black*/
			if(tmp)
			tmp->black = 1;
		}
		else
		{
			/*double black...*/
			fixDoubleBlack(left, parent, getParent(parent));
		}
	}
	else
	{
		printf("Memory is not allocated. Do not free.");
	}
}

/**
 * Helper function to rebalance the rangeTree after adding a node
 *
 * @param P - parent node of added node
 * @param G - parent node of P
 * @param top - parent of G, used to connect back to tree
 * @param newLeft - Direction from P to new node. 1 if left, 0 if right
 * @param left - Direction from G to P. 1 if left, 0 if right
 * @param topLeft - Direction from top to G. 1 if left, 0 if right
 */	
static void rebalance(rangeTree *P, rangeTree *G, rangeTree *top, int newLeft, int left, int topLeft)
{
	/*P is red*/
	if(!P->black && left<2)
	{
		/*P's sibling is red, recolor and recurse*/
		if(G->children[!left] && !G->children[!left]->black)
		{
			P->black=1;
			G->black=0;
			G->children[!left]->black=1;
			if(topLeft<2)
			{
				if(!top->black)
				{
					rangeTree *x = getParent(top);
					rangeTree *y = getParent(x);
					if(y)
					{
						left =(y->LEFT == x) ? 1 :0;
					}
					else
					{
						left = 2;
					}
					rebalance(top, x, y, topLeft, ((x->LEFT == top) ? 1 : 0), left);
				}
			}
			else
			{
				/*G is head*/
				G->black=1;
			}
		}
		/*P's sibling is black (or NULL), restructure*/
		else if(!G->children[!left] || G->children[!left]->black)
		{
			if(newLeft == left)
			{
				if(topLeft<2)
				{
					top->children[topLeft] = P;
					P->black = 1;
					G->black = 0;
					G->children[left] = P->children[!left];
					P->children[!left] = G;
				}
				else
				{
					head = P;
					head->black = 1;
					G->black = 0;
					G->children[left] = P->children[!left];
					P->children[!left] = G;
				}
			}
			else
			{
				if(topLeft<2)
				{
					top->children[topLeft] = P->children[newLeft];
					G->children[left] = P->children[newLeft]->children[newLeft];
					P->children[newLeft] = P->children[newLeft]->children[!newLeft];
					top->children[topLeft]->children[left] = P;
					top->children[topLeft]->children[!left] = G;
					G->black = 0;
					top->children[topLeft]->black = 1;
				}
				else
				{
					head = P->children[newLeft];
					G->children[left] = P->children[newLeft]->children[newLeft];
					P->children[newLeft] = P->children[newLeft]->children[!newLeft];
					head->children[left] = P;
					head->children[!left] = G;
					G->black = 0;
					head->black = 1;
				}
			}
		}
		/*else P is head pointer, so do nothing*/
	}
	/*else P is black, so do nothing*/
}
/**
 * Add's node to tree. If we cover up a freed node, then remove that node before adding
 *
 * @param a - start address of added range
 * @param length - size of the range
 */	
 
void addRange(void* a, int length)
{
	/*Make copy of head pointer*/
	uintptr_t address = (uintptr_t) a;
	rangeTree *top = NULL;
	rangeTree *tmp = NULL;
	rangeTree *tmpParent = NULL;
	int topLeft;
	int left;
	int newLeft;
	tmp=head;
	topLeft=2;
	left=2;
	newLeft=2;
	
	/*tree is empty*/
	if(!head )
	{
		head = (rangeTree *) malloc(sizeof(rangeTree));
		if(!head)
		{
			printf("Failed to allocate memory for the dynamic memory range tree");
			exit(-2);
		}	
		head->addr = address;
		head->start = address;
		head->len = length;
		head->free = 0;
		/*First node will be black */
		head->black=1;
		head->LEFT = NULL;
		head->RIGHT = NULL;
		return;
	}
	/*Traverse tree*/
	while(1)
	{
		int x;
		if(address > tmp->start + (uintptr_t) tmp->len - 1)
		{
			x = 0;
		}
		else if (address + (uintptr_t)length-1 < tmp->start)
		{
			x = 1;
		}
		/*Found an allocated block in the tree that contains at least part of our range*/
		else if(!tmp->free)
		{
			printf("Address space is already allocated.\n");
			exit(-1);
		}
		else
		{
			/*Overlaps front of freed range*/
			if(address <= tmp->start)
			{
				tmp->len-= (int)((uintptr_t)length - tmp->start + address);
				tmp->start = address + (uintptr_t)length;
				
				/*covers freed range completely*/
				if(tmp->len <=0)
				{
					removeRange(tmp);
					addRange((void*) address, length);
					return;
				}
				else
				{
					x=1;
				}
			}
			else
			{
				/*Overlaps freed range end or middle*/
				tmp->len = (int)address - (int) tmp->start ;
				x=0;
					
			}
			
		}
		if(tmp->children[x])
		{
			top = tmpParent;
			tmpParent=tmp;
			tmp = tmp->children[x];
			topLeft=left;
			left=x;
		}
		else
		{
			/*Allocate new node*/
			tmp->children[x] = (rangeTree *) malloc(sizeof(rangeTree));
			if(!tmp->children[x])
			{
				printf("Failed to allocate memory for the dynamic memory range tree");
				exit(-2);
			}		
			tmp->children[x]->addr = address;
			tmp->children[x]->start = address;
			tmp->children[x]->len = length;
			tmp->children[x]->black=0;
			tmp->children[x]->free=0;
			tmp->children[x]->LEFT = NULL;
			tmp->children[x]->RIGHT = NULL;
			newLeft=x;
			break;
		}
	}
	/*Fix any double red nodes*/
	rebalance(tmp, tmpParent, top, newLeft, left, topLeft);
	return;
}

/**
 * Checks whether a range is contained in the tree.
 *
 * @param address - starting address of range
 * @param length - length the of address block being looked for
 * @return Returns 
 *			1 if found in the tree
 *			-1 if not found int the tree
 *			-2 if partially found in the tree
 *			-3 if node is found and already freed
 */
int rangeQuery(void* address, int length)
{
	rangeTree *x = getRangeNode((uintptr_t)address, length);
	/*Add other cases, including whether or not it is freed*/
	if(x && head->black==2)
	{
		head->black=1;
		/*address + length -1 is out of range*/
		return -2;
	}
	else if(!x)
	{
		/*not within range*/
		return -1;
	}
	else if(x->free)
	{
		/*already freed*/
		return -3;
	}
	else
	{
		return 1;
	}
}


/**
 * Sets the free flag for the node if it is in the tree
 *
 * @param address - starting address of range
 * @return Returns 
 *			1 if free is successful
 *			-1 if not found int the tree
 *			-2 if address is not the starting address of the node
 *			-3 if node is already freed
 *			-4 if misaligned address for one node and double free for another
 */
int freeRange(void* address)
{
	rangeTree *x = getRangeNode((uintptr_t) address, 1);
	if(x)
	{
		if(x->addr == (uintptr_t) address && !x->free)
		{
			x->free = 1;
			return x->free;
		}
		if(x->addr != (uintptr_t) address && x->free)
		{
			/*double free or misaligned address*/
			return -4;
		}
		if(x->addr != (uintptr_t) address)
		{
			/*misaligned address*/
			return -2;
		}
		else
		{
			/*double free*/
			return -3; 
		}
	
	}
	else
	{
		/*address is not in the tree*/
		return -1;
	}
}

/**
 * Helper function for dynamicMemoryBytes
 *
 * @param x - starting node
 * @return Number of bytes allocated in the tree
 */
static int dynamicMemoryBytesNode(rangeTree* x)
{
	if(!x)
	{
		return 0;
	}
	else
	{
		return dynamicMemoryBytesNode(x->LEFT) + dynamicMemoryBytesNode(x->RIGHT) + (x->free ? 0 : x->len);
	}
}
/**
 * Outputs the total number of dynamic bytes allocated
 *
 * @return Returns the number of bytes allocated in the tree
 */
int dynamicMemoryBytes()
{
	return dynamicMemoryBytesNode(head);
}
/**
 * Prints out the tree in level order using a breadth first traversal, prints an "X" for null nodes
 * Used for debugging
 */
void printRangeTree()
{
	int curIndex = 0;
	int nxtIndex = 0;
	int curSize = 10;
	int nxtSize = 10;
	rangeTree **curLevelQ = (rangeTree **) malloc(curSize*sizeof(rangeTree *));
	rangeTree **nextLevelQ = (rangeTree **) malloc(nxtSize*sizeof(rangeTree *));
	if(!curLevelQ)
	{
		printf("Failed to allocate queue to print range tree");
		exit(1);
	}
	if(!nextLevelQ)
	{
		printf("Failed to allocate queue to print range tree");
		exit(1);
	}
	if(!head)
	{
		return;
	}
	curLevelQ[0] = head;
	curIndex++;
	while(curIndex)
	{
		/*Pop node off of queue*/
		rangeTree *n = curLevelQ[0];
		int i;
		for(i=1; i<curIndex; i++)
		{
			curLevelQ[i-1] = curLevelQ[i];
		}
		i=0;
		curIndex--;
		/*print node*/
		if(n)
		{
			printf("%d,%d,%d,%d ", n->start, n->start+n->len-1, n->black, n->free);
			/*printf("%d,%d ", n->black, n->free);*/
			nextLevelQ[nxtIndex] = n->LEFT;
			nxtIndex++;
			if(nxtIndex==nxtSize)
			{
				nxtSize*=2;
				nextLevelQ = (rangeTree **) realloc(nextLevelQ, nxtSize*sizeof(rangeTree *));
				if(!nextLevelQ)
				{
					printf("Failed to allocate queue to print range tree");
					exit(1);
				}
			}
			nextLevelQ[nxtIndex] = n->RIGHT;
			nxtIndex++;
			if(nxtIndex==nxtSize)
			{
				nxtSize*=2;
				nextLevelQ = (rangeTree **) realloc(nextLevelQ, nxtSize*sizeof(rangeTree *));
				if(!nextLevelQ)
				{
					printf("Failed to allocate queue to print range tree");
					exit(1);
				}
			}
		}
		/*n is null, print "X"*/
		else
		{
			printf("X");
			nextLevelQ[nxtIndex] = NULL;
			nxtIndex++;
			if(nxtIndex==nxtSize)
			{
				nxtSize*=2;
				nextLevelQ = (rangeTree **) realloc(nextLevelQ, nxtSize*sizeof(rangeTree *));
				if(!nextLevelQ)
				{
					printf("Failed to allocate queue to print range tree");
					exit(1);
				}
			}
			fflush(stdout);
			nextLevelQ[nxtIndex] = NULL;
			nxtIndex++;
			if(nxtIndex==nxtSize)
			{
				nxtSize*=2;
				nextLevelQ = (rangeTree **) realloc(nextLevelQ, nxtSize*sizeof(rangeTree *));
				if(!nextLevelQ)
				{
					printf("Failed to allocate queue to print range tree");
					exit(1);
				}
			}
		}
		/*next level*/
		if(!curIndex)
		{
			int t;
			int i, x;
			rangeTree **tmp;
			printf("\n");
			x=0;
			for(i=0; i<nxtIndex; i++)
			{
				if(nextLevelQ[i] != NULL)
				{
					x=1;
					break;
				}
			}
			if(!x)
			{
				break;
			}
			
			t = nxtSize;
			nxtSize = curSize;
			curSize = t;
			curIndex = nxtIndex;
			nxtIndex = 0;
				
			tmp = curLevelQ;
			curLevelQ = nextLevelQ;
			nextLevelQ = tmp;
		}
		
		
	}
	for(curIndex=0; curIndex<curSize; curIndex++)
	{
		curLevelQ[curIndex]=NULL;
	}
		for(nxtIndex=0; nxtIndex<nxtSize; nxtIndex++)
	{
		nextLevelQ[nxtIndex]=NULL;
	}
	free(curLevelQ);
	free(nextLevelQ);
	printf("\n");

}

/**
 * Helper function for tree destructor
 *
 * @param x - subtree to deallocate
 */
void deleteSubTree(rangeTree *x)
{
	rangeTree *l;
	rangeTree *r;
	if(x)
	{
		r = x->LEFT;
		l = x->RIGHT;
		free(x);
		x=NULL;
		deleteSubTree(l);
		deleteSubTree(r);
	}
	else
	{
		return;
	}
}

/**
 * Tree destructor
 */
void deleteTree()
{
	rangeTree *l;
	rangeTree *r;
	if(head)
	{
		r = head->LEFT;
		l = head->RIGHT;
		free(head);
		head = NULL;
		deleteSubTree(l);
		deleteSubTree(r);
	}
	else
	{
		return;
	}
}

