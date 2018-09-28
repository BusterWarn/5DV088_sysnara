CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:			mpasswdsort

mpasswdsort:		mpasswdsort.o linkedlist.o
	$(CC) mpasswdsort.o linkedlist.o -o mpasswdsort

mpasswdsort.o:		mpasswdsort.c mpasswdsort.h linkedlist.h
	$(CC) $(CFLAGS) -c mpasswdsort.c

linkedlist.o: 		linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

run:			mpasswdsort
	./mpasswdsort pass.txt

valgrind: 		mpasswdsort
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mpasswdsort pass.txt

clean:
	rm -f mpasswdsort *.o core
