/*
* A dynamic single-linked list. Each element (node) in the list holds up to one
* void pointer. The positon of the list is saved in the list's struct, and user
* will not need to keep track of position.
*
* Author: Buster Hultgren Wärn <dv17bhn@cs.umu.se>
*
* Final build: 2018-09-26
*/

#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef int (*ValCmpFunc) (void *val1, void *val2);

typedef struct listnode listnode;

typedef struct linkedlist linkedlist;

/*
* description: Creates and allocates memory for an empty list.
* return: The list.
*/
linkedlist *listEmpty (void);

/*
* description: Checks if list contains any elements.
* param[in]: list - The list.
* return: If true; 1, else 0.
*/
int listIsEmpty (linkedlist *list);

/*
* description: Gets the number of elements in list.
* param[in]: list - The list.
* return: The number of elements in list.
*/
int listGetSize (linkedlist *list);

/*
* description: Inspects the value of the element in the current position in the
* list.
* param[in]: list - The list.
* return: Void pointer to the value in the list.
*/
void *listInspect (linkedlist *list);

/*
* description: Inserts an element into the list. The element will take
* the first position in the list.
* param[in]: list - The list.
* param[in]: value - Void pointer to the value the element will hold.
*/
void listInsert (linkedlist *list, void *value);

/*
* description: Changes the value of the element in the current position in the
* list. Allocated memory for old value WILL NOT be freed.
* param[in]: list - The list.
* param[in]: newValue - The value that will replace the old.
*/
void listModifyValue (linkedlist *list, void *newValue);

/*
* description: Removes the next element from the current position in the list.
* Allocated memory for the value in the removed element WILL be freed.
* param[in]: list - The list.
*/
void listRemoveNext (linkedlist *list);

/*
* description: Removes all elements in the list with a specific value.
* Allocated memory for the value(s) in the element(s) WILL be freed.
* param[in]: list - The list.
* param[in]: value - Pointer to the value. SHOULD be a pointer containg the
* the same value, NOT a pointer to the same adress where value is stored.
* param[in]: valueCmp - Value compare function that compares if two values
* are the same.
*/
void listRemoveFromValue (linkedlist *list, void *value, ValCmpFunc valueCmp);

/*
* description: Sets the list's current position to the lists first position.
* param[in]: list - The list.
*/
void listFirst (linkedlist *list);

/*
* description: Sets the list's current position to next positon of that (if
* there is one).
* param[in]: list - The list.
*/
void listNext (linkedlist *list);

/*
* description: Frees all memory allocated by the list, inluding the list.
* param[in]: list - The list
*/
void listKill (linkedlist *list);

/*
* description: Checks if memory succesfully got allocated. If not, pointer will
* contain NULL and program will exit with errno.
* param[in]: mem - pointer to the allocated memory.
*/
void checkAlloc (void *mem);


#endif //LINKEDLIST
