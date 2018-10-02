CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:			mish

mish:			mish.o parser.o execute.h
	$(CC) mish.o parser.o execute.h -o mish

mish.o:			mish.c mish.h parser.h execute.h
	$(CC) $(CFLAGS) -c mish.c

parser.o:		parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

execute.o:		execute.c execute.h
	$(CC) $(CFLAGS) -c execute.c

run:			mish
	./mish

valgrind: 		mish
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mish

clean:
	rm -f mish *.o core
