Prog3: free537.o malloc537.o memcheck537.o realloc537.o main.o rangeTree.o
	gcc -Wall -pedantic -o Prog3 free537.o malloc537.o memcheck537.o realloc537.o main.o rangeTree.o
main.o: main.c memcheck537.h malloc537.h realloc537.h free537.h rangeTree.h
	gcc -c -Wall -pedantic main.c
realloc537.o: realloc537.c malloc537.h rangeTree.h
	gcc -c -Wall -pedantic realloc537.c
free537.o: free537.c rangeTree.h
	gcc -c -Wall -pedantic free537.c
malloc537.o: malloc537.c rangeTree.h
	gcc -c -Wall -pedantic malloc537.c
memcheck537.o: memcheck537.c rangeTree.h
	gcc -c -Wall -pedantic memcheck537.c
rangeTree.o: rangeTree.c rangeTree.h
	gcc -c -Wall -pedantic rangeTree.c
