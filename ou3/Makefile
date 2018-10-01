CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:			mish

mish:		mish.o
	$(CC) mish.o -o mish

mish.o:		mish.c mish.h
	$(CC) $(CFLAGS) -c mish.c

run:			mish
	./mish wutwut

valgrind: 		mpasswdsort
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mish wutwut

clean:
	rm -f mish *.o core
