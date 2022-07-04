FLAGS = -Wall -Werror
ALL = my-cat my-grep my-zip my-unzip
COMMON = common.h 
CC = gcc

all: $(ALL)

clean:
	rm -f $(ALL)

my-cat: my-cat.c $(COMMON)
	$(CC) -o my-cat my-cat.c $(FLAGS)

my-grep: my-grep.c $(COMMON)
	$(CC) -o my-grep my-grep.c $(FLAGS)

my-unzip: my-unzip.c $(COMMON)
	$(CC) -o my-unzip my-unzip.c $(FLAGS)

my-zip: my-zip.c $(COMMON)
	$(CC) -o my-zip my-zip.c $(FLAGS)