#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdio.h>
#include <stdlib.h>

typedef int (*ValCmpFunc) (void *val1, void *val2);

typedef struct listnode listnode;

typedef struct linkedlist linkedlist;

struct listnode {

	void *value;
	listnode *next;
};

struct linkedlist {

	int size;
	linkedlist *first;
	linkedlist *pos;
};

linkedlist *listEmpty (void);

int listIsEmpty (linkedlist *list);

int listGetSize (linkedlist *list);

void *listInspect (linkedlist *list);

void listInsert (linkedlist *list, void *value);

void listModifyValue (linkedlist *list, void *newValue);

void listRemove (linkedlist *list);

void listRemoveFromValue (linkedlist *list, void *value, ValCmpFunc valueCmp);

void listFirst (linkedlist *list);

void listNext (linkedlist *list);

void listKill (linkedlist *list);


#endif //LINKEDLIST
