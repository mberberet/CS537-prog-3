
typedef struct rangeTree
{
	int addr; 
	/*Cast to int from void*, that way we can do addition correctly*/
	int len;
	struct rangeTree *left;
	struct rangeTree *right;
	int red;
}rangeTree;


void addRange(int address, int length);
rangeTree *rangeQuery(int address, int length);
void removeRange(int address, int length);
int dynamicMemoryBytes();
void printRangeTree();
