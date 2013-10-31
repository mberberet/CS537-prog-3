RedBlackTree: main.o rangeTree.o
	gcc -pedantic -Wall -o RedBlackTree main.o rangeTree.o

main.o: main.c rangeTree.h
	gcc -pedantic -Wall -c main.c rangeTree.h

rangeTree.o: rangeTree.c 
	gcc -pedantic -Wall -c rangeTree.c

clean:
	rm -rf RedBlackTree *.o *.gch 
