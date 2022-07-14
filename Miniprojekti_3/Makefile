FLAGS = -Wall -Werror
CC = gcc
COMMON = wish-common.h 

wish: wish-main.o wish-builtin.o wish-common.o wish.o
	$(CC) -o wish wish-main.o wish-builtin.o wish-common.o wish.o

wish-main.o: wish-main.c wish.h $(COMMON) 
	$(CC) -c wish-main.c $(FLAGS)

wish-builtin.o: wish-builtin.c wish-builtin.h $(COMMON) 
	$(CC) -c wish-builtin.c $(FLAGS)

wish-common.o: wish-common.c $(COMMON)
	$(CC) -c wish-common.c $(FLAGS)

wish.o: wish.c wish.h $(COMMON) 
	$(CC) -c wish.c $(FLAGS)
