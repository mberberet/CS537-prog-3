#include <stdlib.h>
#include <stdio.h>
#include "rangeTree.h"

static rangeTree *head;

static int isLeaf(rangeTree* x)
{
	return !(x->left || x->right);
}

static rangeTree* getParent(rangeTree *child)
{
	rangeTree *tmp;
	tmp = head;
	if(tmp ==child)
	{
		return NULL;
	}
	while(child && tmp && tmp->right != child && tmp->left != child)
	{
		if(child->addr > tmp->addr + tmp->len -1)
		{
			/*Go right*/
			tmp=tmp->right;
		}
		else if (child->addr + child->len-1 < tmp->addr)
		{
			/*Go left*/
			tmp = tmp->left;
		}
	}
	return tmp;
}

static void redBlackFix(rangeTree *P, rangeTree *G, rangeTree *top, int newLeft, int left, int topLeft)
{
	if(P->red)
	{
		/*P's sibling is red and P is a left child*/
		if(left==1 && G->right && G->right->red)
		{
			P->red=0;
			G->red = 1;
			G->right->red=0;
			printf("%d\n", topLeft);
			switch(topLeft)
			{
				case 0: 
					top->right = G;
					if(top->red)
					{
						rangeTree *x = getParent(top);
						rangeTree *y = getParent(x);
						if(y)
						{
							left =(y->left == x) ? 1 :0;
						}
						else
						{
							left = 2;
						}
						redBlackFix(top, x, y, topLeft, ((x->left == top) ? 1 : 0), left);
					}
					break;
				case 1:
					top->left = G;
					if(top->red)
					{
						rangeTree *x = getParent(top);
						rangeTree *y = getParent(x);
						if(y)
						{
							left =(y->left == x) ? 1 :0;
						}
						else
						{
							left = 2;
						}
						redBlackFix(top, x, y, topLeft, ((x->left == top) ? 1 : 0), left);
					}
					break;
				case 2:
					head = G;
					head->red=0;
					break;
			}
		}
		/*P's sibling is red and P is a right child*/
		else if(left==0 && G->left && G->left->red)
		{
			P->red=0;
			G->red = 1;
			G->left->red=0;
			switch(topLeft)
			{
				case 0: 
					top->right = G;
					if(top->red)
					{
						rangeTree *x = getParent(top);
						rangeTree *y = getParent(x);
						if(y)
						{
							left =(y->left == x) ? 1 :0;
						}
						else
						{
							left = 2;
						}
						redBlackFix(top, x, y, topLeft, ((x->left == top) ? 1 : 0), left);
					}
					break;
				case 1:
					top->left = G;
					if(top->red)
					{
						rangeTree *x = getParent(top);
						rangeTree *y = getParent(x);
						if(y)
						{
							left =(y->left == x) ? 1 :0;
						}
						else
						{
							left = 2;
						}
						redBlackFix(top, x, y, topLeft, ((x->left == top) ? 1 : 0), left);
					}
					break;
				case 2:
					head = G;
					head->red=0;
					break;
			}
		}
		/*P's sibling is black (or NULL) and P is left child*/
		else if(left==1 && (!G->right || !G->right->red))
		{
			if(newLeft)
			{
				G->left = P->right;
				G->red = 1;
				P->right = G;
				P->red = 0;
				switch(topLeft)
				{
					case 0: 
						top->right = P;
						break;
					case 1:
						top->left = P;
						break;
					case 2:
						head = P;
						break;
				}
			}
			else
			{
				P->right->left = P;
				P->right->right = G;
				P->right->red = 0;
				switch(topLeft)
				{
					case 0: 
						top->right = P->right;
						break;
					case 1:
						top->left = P->right;
						break;
					case 2:
						head = P->right;
						break;
				}
				P->right = NULL;
				G->left = NULL;
				G->red = 1;
			}
				
				
		}
		/*P's sibling is black (or NULL) and P is left child*/
		else if(left==0 && (!G->left || !G->left->red))
		{
			if(!newLeft)
			{
				G->right = P->left;
				G->red = 1;
				P->left = G;
				P->red = 0;
				switch(topLeft)
				{
					case 0: 
						top->right = P;
						break;
					case 1:
						top->left = P;
						break;
					case 2:
						head = P;
						break;
				}
			}
			else
			{
				P->left->right = P;
				P->left->left = G;
				P->left->red = 0;
				switch(topLeft)
				{
					case 0: 
						top->right = P->left;
						break;
					case 1:
						top->left = P->left;
						break;
					case 2:
						head = P->left;
						break;
				}
				P->left = NULL;
				G->right = NULL;
				G->red = 1;
			}
		}
		/*else P is head pointer, so do nothing*/
	}
	/*else P is black, so do nothing*/
}

void addRange(int address, int length)
{
	/*Make copy of head pointer*/
	rangeTree *top;
	rangeTree *tmp;
	rangeTree *tmpParent;
	int topLeft;
	int left;
	int newLeft;
	tmp=head;
	topLeft=2;
	left=2;
	newLeft=2;
	
	/*tree is empty*/
	if(!head)
	{
		head = (rangeTree *) malloc(sizeof(rangeTree));
		if(!head)
		{
			printf("Failed to allocate memory for the dynamic memory range tree");
			exit(1);
		}	
		head->addr = address;
		head->len = length;
		/*First node will be black */
		head->red = 0; 
		return;
	}
	while(1)
	{
		if(address > tmp->addr + tmp->len -1)
		{
			/*Go right*/
			if(tmp->right)
			{
				top = tmpParent;
				tmpParent=tmp;
				tmp = tmp->right;
				topLeft=left;
				left=0;
				
			}
			else
			{
				tmp->right = (rangeTree *) malloc(sizeof(rangeTree));
				if(!tmp->right)
				{
					printf("Failed to allocate memory for the dynamic memory range tree");
					exit(1);
				}		
				tmp->right->addr = address;
				tmp->right->len = length;
				tmp->right->red = 1;
				newLeft=0;
				break;
			}
		}
		else if (address + length-1 < tmp->addr)
		{
			/*Go left*/
			if(tmp->right)
			{
				top = tmpParent;
				tmpParent=tmp;
				tmp = tmp->left;
				topLeft=left;
				left=1;
			}
			else
			{
				tmp->left = (rangeTree *) malloc(sizeof(rangeTree));
				if(!tmp->left)
				{
					printf("Failed to allocate memory for the dynamic memory range tree");
					exit(1);
				}	
				tmp->left->addr = address;
				tmp->left->len = length;
				tmp->left->red = 1;
				newLeft=1;
				break;
			}
		}
		else
		{
			printf("Address are ready allocated.");
			exit(2);
		}
	}
	redBlackFix(tmp, tmpParent, top, newLeft, left, topLeft);

}
/*If range is covered in tree, return the node it is covered by. 
  If range is partially covered, exit with error code,
  If range is not at all covered, return NULL
*/
rangeTree *rangeQuery(int address, int length)
{
	rangeTree *tmp;
	tmp = head;
	while(tmp)
	{
		if(address > tmp->addr + tmp->len-1)
		{
			tmp= tmp->right;
		}
		else if (address + length-1 < tmp->addr)
		{
			tmp = tmp->left;
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
		if(address >= tmp->addr && address+length-1 < tmp->addr + tmp->len) 
		{
			return tmp;
		}
		else
		{
			printf("Memory is partially allocated for queried block:\nAddress: 0x%X\nLength: %d\n", address, length);
			printf("Free or malloc of this block will cause a seg fault\n");
			exit(2);
		}
	}
	return head;
}
void removeRange(int address, int length)
{
	/*THIS MIGHT BE ROUGH*/
	rangeTree* x;
	rangeTree* tmp;
	rangeTree* parent;
	int left;
	x = rangeQuery(address, length);
	parent = getParent(x);
	if(x)
	{
	/*reduce length*/
	/*change start address*/
	/* split into two nodes*/
	/*  remove node and follow red black tree algorithm*/

			
	}
	else
	{
		printf("Memory is not allocated. Will not free.");
	}
}

static int dynamicMemoryBytesNode(rangeTree* x)
{
	if(!x)
	{
		return 0;
	}
	else
	{
		return dynamicMemoryBytesNode(x->left) + dynamicMemoryBytesNode(x->right) + x->len;
	}
}

int dynamicMemoryBytes()
{
	return dynamicMemoryBytesNode(head);
}

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
		curIndex--;
		if(n)
		{
			printf("0x%X,%d,%d ", n->addr, n->len, n->red);
			nextLevelQ[nxtIndex] = n->left;
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
			nextLevelQ[nxtIndex] = n->right;
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
		else
		{
			printf("X");
			nextLevelQ[nxtIndex] = NULL;
			/*if(n->left){printf("Added left: 0x%X", n->left->addr);}*/
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
			nextLevelQ[nxtIndex] = NULL;
			/*if(n->right){printf("Added right: 0x%X", n->right->addr);}*/
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
		if(!curIndex)
		{
			int t;
			int i, x;
			rangeTree **tmp;
			printf("\n");
			x=0;
			for(i=0; i<=nxtIndex; i++)
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
			t = nxtIndex;
			nxtIndex = curIndex;
			curIndex = t;
						
			tmp = curLevelQ;
			curLevelQ = nextLevelQ;
			nextLevelQ = tmp;
			/*free(tmp);*/	
		}
		
		
	}
	printf("\n");
}
