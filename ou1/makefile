CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:			mpasswd

mpasswd:		mpasswd.o linkedlist.o
	$(CC) mpasswd.o linkedlist.o -o mpasswd

mpasswd.o:		mpasswd.c mpasswd.h linkedlist.h
	$(CC) $(CFLAGS) -c mpasswd.c

linkedlist.o: 		linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

run:			mpasswd
	./mpasswd pass.txt

valgrind: 		mpasswd
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mpasswd pass.txt

clean:
	rm -f linkedlist *.o core
