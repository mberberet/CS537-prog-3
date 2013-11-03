
typedef struct rangeTree
{
	int addr; 
	/*Cast to int from void*, that way we can do addition correctly*/
	int len;
	/*children[0] is right, children[1] is left */
	struct rangeTree *children[2];
	int black;
}rangeTree;


void addRange(int address, int length);
/*Might change return type to int for class security, but right now returns 
node that fits the range if it exists*/
int rangeQuery(int address, int length);
void removeRange(int address, int length);

/*Gives total amount of dynamic memory allocated*/
int dynamicMemoryBytes();
void printRangeTree();
