#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef int (*ValCmpFunc) (void *val1, void *val2);

typedef struct listnode listnode;

typedef struct linkedlist linkedlist;

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

void checkAlloc (void *mem);


#endif //LINKEDLIST
