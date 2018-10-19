CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:				mfind

mfind:				mfind.o queue.o parseMfind.o saferMemHandler.o
	$(CC) mfind.o queue.o parseMfind.o saferMemHandler.o -o mfind

mfind.o:			mfind.c mfind.h queue.h parseMfind.h
	$(CC) $(CFLAGS) -c mfind.c

queue.o: 			queue.c queue.h saferMemHandler.h
	$(CC) $(CFLAGS) -c queue.c

parseMfind.o:		parseMfind.c parseMfind.h saferMemHandler.h
	$(CC) $(CFLAGS) -c parseMfind.c

saferMemHandler.o:	saferMemHandler.c saferMemHandler.h
	$(CC) $(CFLAGS) -c saferMemHandler.c

run1:				mfind
	./mfind -t d -p 15 /Documents/ wut2

run2:				mfind
	./mfind /Documents/ slibberslabber.tx

valgrind:			mfind
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mfind /Documents/ slibberslabber.tx

clean:
	rm -f mfind *.o core
