#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rangeTree.h"

#define RIGHT children[0]
#define LEFT children[1]

static rangeTree *head;

	
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

static void fixDoubleBlack(int left, rangeTree* parent, rangeTree *root)
{
	int topLeft = root ? (root->LEFT == parent) : 2;
	if(left<2)
	{	/*red sibling*/
		if(parent->children[!left] && !parent->children[!left]->black)
		{
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
		else if(!parent->children[!left])
		{
		}
		else
		{
			/*black sibling with red child on same side*/			
			if(parent->children[!left]->children[!left] && !parent->children[!left]->children[!left]->black)
			{
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
			/*black sibling with red child on other side*/
			else if(parent->children[!left]->children[left] && !parent->children[!left]->children[left]->black)
			{
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

static void rebalance(rangeTree *P, rangeTree *G, rangeTree *top, int newLeft, int left, int topLeft)
{
	/*P is red*/
	if(!P->black && left<2)
	{
		/*P's sibling is red */
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
		/*P's sibling is black (or NULL) */
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

int addRange(void* a, int length)
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
			return -2;
		}	
		head->addr = address;
		head->start = address;
		head->len = length;
		head->free = 0;
		/*First node will be black */
		head->black=1;
		head->LEFT = NULL;
		head->RIGHT = NULL;
		return 1;
	}
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
		else if(!tmp->free)
		{
			printf("Address space are ready allocated.\n");
			return -1;
		}
		else
		{
			if(address <= tmp->start)
			{
				tmp->len-= (int)((uintptr_t)length - tmp->start + address);
				tmp->start = address + (uintptr_t)length;
				
				if(tmp->len <=0)
				{
					removeRange(tmp);
					return addRange((void*) address, length);
				}
				else
				{
					x=1;
				}
			}
			else
			{
				if(address + (uintptr_t)length < tmp->start + (uintptr_t)tmp->len)
				{
					rangeTree *p;
					int t = tmp->len;
					tmp->len = (int)address - (int) tmp->start ;
					x=0;
					
				}
				else
				{
					tmp->len = (int)address - (int) tmp->start ;
					x= 0;
				}
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
			tmp->children[x] = (rangeTree *) malloc(sizeof(rangeTree));
			if(!tmp->children[x])
			{
				printf("Failed to allocate memory for the dynamic memory range tree");
				return -2;
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
	rebalance(tmp, tmpParent, top, newLeft, left, topLeft);
	return 1;
}


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
			return -4;//double free or misaligned address
		}
		if(x->addr != (uintptr_t) address)
		{
			return -2; //misaligned address
		}
		else
		{
			return -3; //double free
		}
	
	}
	else
	{
		return -1;//doesn't exist
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
		return dynamicMemoryBytesNode(x->LEFT) + dynamicMemoryBytesNode(x->RIGHT) + (x->free ? 0 : x->len);
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
		i=0;
		curIndex--;
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
	
