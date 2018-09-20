CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g

all:				mpasswordsort

mpasswordsort:		mpasswordsort.o linkedlist.o
	$(CC) mpasswordsort.o linkedlist.o -o mpasswordsort

mpasswordsort.o:	mpasswordsort.c mpasswordsort.h linkedlist.h
	$(CC) $(CFLAGS) -c mpasswordsort.c

linkedlist.o: 		linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

run:				mpasswordsort
	./mpasswordsort pass.txt

valgrind: 			mpasswordsort
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mpasswordsort pass.txt

clean:
	rm -f linkedlist *.o core
