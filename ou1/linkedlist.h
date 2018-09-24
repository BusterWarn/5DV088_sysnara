#ifndef linkedlist
#define linkedlist

#include <stdio.h>
#include <stdlib.h>

typedef int (*ValCmpFunc) (void *val1, void *val2);

typedef struct node {

	void *value;
	struct node *next;
} node;

typedef struct  list {

	int size;
	struct node *first;
	struct node *pos;
} list;

list *listEmpty (void);

int listIsEmpty (list *list);

int listGetSize (list *list);

void *listInspect (list *list);

void listInsert (list *list, void *value);

void listModifyValue (list *list, void *newValue);

void listRemove (list *list);

void listFirst (list *list);

void listNext (list *list);

void listKill (list *list);


#endif //linkedlist
