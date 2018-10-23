CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:				mfind

mfind:				mfind.o queue.o parseMfind.o saferMemHandler.o
	$(CC) -pthread mfind.o queue.o parseMfind.o saferMemHandler.o -o mfind

mfind.o:			mfind.c mfind.h queue.h parseMfind.h
	$(CC) $(CFLAGS) -c mfind.c

queue.o: 			queue.c queue.h saferMemHandler.h
	$(CC) $(CFLAGS) -c queue.c

parseMfind.o:		parseMfind.c parseMfind.h saferMemHandler.h
	$(CC) $(CFLAGS) -c parseMfind.c

saferMemHandler.o:	saferMemHandler.c saferMemHandler.h
	$(CC) $(CFLAGS) -c saferMemHandler.c

run:				mfind
	./mfind -t f -p 10 /home/sersmoothie/Documents slibberslabber.tx 2> test.txt

run1: 				mfind
	./mfind -t d -p 100 /home/ wutwut 2>test.txt

runfromlink:		mfind
	./mfind -p /home/sersmoothie/Documents/UMU/Kurser/sysnara/ou4/ou1/ linkedlist.h

findlink:			mfind
	./mfind -t l -p 123 /home/sersmoothie/Documents/ /home/Steam/ ou1

valgrind:			mfind
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mfind -t f -p 10 /home/sersmoothie/Documents slibberslabber.tx

clean:
	rm -f mfind *.o core
