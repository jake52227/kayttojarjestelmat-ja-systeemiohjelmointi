reverse: main.o reverse.o
	gcc -o reverse main.c reverse.o
reverse.o: reverse.c reverse.h
	gcc -c reverse.c
main.o: main.c reverse.h
	gcc -c main.c