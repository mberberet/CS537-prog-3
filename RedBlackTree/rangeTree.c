#include <stdlib.h>
#include <stdio.h>
#include "rangeTree.h"

#define RIGHT children[0]
#define LEFT children[1]

static rangeTree *head;


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
		if(child->addr > tmp->addr + tmp->len -1)
		{
			/*Go right*/
			tmp=tmp->RIGHT;
		}
		else if (child->addr + child->len-1 < tmp->addr)
		{
			/*Go left*/
			tmp = tmp->LEFT;
		}
	}
	return tmp;
}

static void rebalance(rangeTree *P, rangeTree *G, rangeTree *top, int newLeft, int left, int topLeft)
{
	if(!P->black && left<2)
	{
		/*P's sibling is red */
		if(G->children[!left] && !G->children[!left]->black)
		{
			P->black=1;
			G->black=0;
			G->children[!left]->black=1;
			printf("%d\n", topLeft);
			if(topLeft<2)
			{
				top->children[topLeft] = G;
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
				head = G;
				head->black=1;
			}
		}
		/*P's sibling is black (or NULL) */
		else if(!G->children[!left] || G->children[!left]->black)
		{
			if(newLeft == left)
			{
				G->children[left] = P->children[!left];
				G->black=0;
				P->children[!left] = G;
				P->black=1;
				if(topLeft<2)
				{
					top->children[topLeft] = P;
				}
				else
				{
					head = P;
				}
			}
			else
			{
				P->children[!left]->children[left] = P;
				P->children[!left]->children[!left] = G;
				P->children[!left]->black=1;
				if(topLeft<2)
				{
					top->children[topLeft] = P->children[!left];
				}
				else
				{
					head = P->children[!left];
				}
				P->children[!left] = NULL;
				G->children[left] = NULL;
				G->black=0;
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
		head->black=1; 
		return;
	}
	while(1)
	{
		int x;
		if(address > tmp->addr + tmp->len -1)
		{
			x = 0;
		}
		else if (address + length-1 < tmp->addr)
		{
			x = 1;
		}
		else
		{
			printf("Address are ready allocated.");
			exit(2);
		}
			/*Go right*/
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
				exit(1);
			}		
			tmp->children[x]->addr = address;
			tmp->children[x]->len = length;
			tmp->children[x]->black=0;
			newLeft=x;
			break;
		}
	}
	rebalance(tmp, tmpParent, top, newLeft, left, topLeft);

}
/*If range is covered in tree, return the node it is covered by. 
  If range is partially covered, exit with error code,
  If range is not at all covered, return NULL
*/
static rangeTree* getRangeNode(int address, int length)
{
	rangeTree *tmp;
	tmp = head;
	while(tmp)
	{
		if(address > tmp->addr + tmp->len-1)
		{
			tmp= tmp->RIGHT;
		}
		else if (address + length-1 < tmp->addr)
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
		if(address >= tmp->addr && address+length-1 < tmp->addr + tmp->len) 
		{
			return tmp;
		}
		else
		{
			head->black=2;
			return tmp;
		}
	}
	return head;
}

int rangeQuery(int address, int length)
{
	rangeTree *x = getRangeNode(address, length);
	if(x && head->black==2)
	{
		head->black=1;
		return 2;
	}
	else 
	{
		return (x != NULL);
	}
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
				root->children[topLeft] = parent->RIGHT;
				parent->children[!left] = parent->children[!left]->children[left];
				root->children[topLeft]->children[left] = parent;
				parent->black=0;
				if(root->children[topLeft])
					root->children[topLeft]->black=1;
				fixDoubleBlack(left, parent, root->children[topLeft]);
			}
			else
			{
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
			/*black sibling with red child on same side*/			
			if(parent->children[!left]->children[left] && !parent->children[!left]->children[left]->black)
			{
				if(topLeft<2)
				{
					root->children[topLeft] = parent->children[!left];
					parent->children[!left] = root->children[topLeft]->children[!left];
					root->children[topLeft]->children[left] = parent;
					if(root->children[topLeft]->children[!left])
						root->children[topLeft]->children[!left]->black=1;
					if(parent->children[left])
						parent->children[left]->black=1;
				}
				else
				{
					head = parent->children[!left];
					parent->children[!left] = head->children[!left];
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
				parent->black++;
				parent->children[!left]->black--;
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

void removeRange(int address, int length)
{
	rangeTree* x;
	rangeTree* tmp;
	rangeTree* parent;
	int left;
	int deletedColor;
	x = getRangeNode(address, length);
	if(x)
	{
	/*reduce length*/
		if(x->addr == address && length < x->len)
		{
			x->len = length;
			return;
		}
		/*change start address*/
		if(x->addr < address && address + length == x->addr + x->len)
		{
			x->addr = address;
			x->len-=length;
			return;
		}
		/*split into two nodes*/
		if(x->addr < address && address + length != x->addr + x->len)
		{
			int tmp;
			tmp = x->len;
			x->len = address - x->addr;
			addRange(address+length-1, tmp - x->len - length); 
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
			}
			/*At most two nodes in tree and removing head node*/
			else
			{
				head = (x->LEFT) ? x->LEFT : x->RIGHT;
				head->black=1;
				free(x);
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
				tmp->black=1;
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

static int dynamicMemoryBytesNode(rangeTree* x)
{
	if(!x)
	{
		return 0;
	}
	else
	{
		return dynamicMemoryBytesNode(x->LEFT) + dynamicMemoryBytesNode(x->RIGHT) + x->len;
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
			printf("0x%X,%d,%d ", n->addr, n->len, n->black);
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
	free(curLevelQ);
	free(nextLevelQ);
	printf("\n");
}
