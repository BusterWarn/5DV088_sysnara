/*
* A dynamic single-linked list. Each element (node) in the list holds up to one
* void pointer. The positon of the list is saved in the list's struct, and user
* will not need to keep track of position.
*
* Author: Buster Hultgren Wärn <dv17bhn@cs.umu.se>
*
* Final build: 2018-09-26
*/

#include "linkedlist.h"

struct listnode {

	void *value;
	listnode *next;
};

struct linkedlist {

	int size;
	listnode *first;
	listnode *pos;
};

/*
* description: Creates and allocates memory for an empty list.
* return: The list.
*/
linkedlist *listEmpty (void) {

	linkedlist *list = malloc(sizeof(*list));
	checkAlloc((void *)list);
	list -> first = NULL;
	list -> pos = NULL;
	list -> size = 0;
	return list;
}

/*
* description: Checks if list contains any elements.
* param[in]: list - The list.
* return: If true; 1, else 0.
*/
int listIsEmpty (linkedlist *list) {

	return list -> size == 0;
}

/*
* description: Gets the number of elements in list.
* param[in]: list - The list.
* return: The number of elements in list.
*/
int listGetSize (linkedlist *list) {

	return list -> size;
}

/*
* description: Inspects the value of the element in the current position in the
* list.
* param[in]: list - The list.
* return: Void pointer to the value in the list.
*/
void *listInspect (linkedlist *list) {

	if (list -> pos != NULL) {

		return list -> pos -> value;
	}
	return NULL;
}

/*
* description: Inserts an element into the list. The element will take
* the first position in the list.
* param[in]: list - The list.
* param[in]: value - Void pointer to the value the element will hold.
*/
void listInsert (linkedlist *list, void *value) {

	if (list -> first == NULL) {

		list -> first = malloc(sizeof(listnode));
		checkAlloc((void *)(list -> first));
		list -> pos = list -> first;
		list -> first -> next = NULL;
	} else {

		struct listnode *tempNode = list -> first;
		list -> first = malloc(sizeof(listnode));
		checkAlloc((void *)(list -> first));
		list -> first -> next = tempNode;
	}

	list -> first -> value = value;
	list -> size++;
}

/*
* description: Changes the value of the element in the current position in the
* list. Allocated memory for old value WILL NOT be freed.
* param[in]: list - The list.
* param[in]: newValue - The value that will replace the old.
*/
void listModifyValue (linkedlist *list, void *newValue) {

	list -> pos -> value = newValue;
}

/*
* description: Removes the next element from the current position in the list.
* Allocated memory for the value in the removed element WILL be freed.
* param[in]: list - The list.
*/
void listRemoveNext (linkedlist *list) {

	listnode* tempNode = NULL;

	if (list -> pos != NULL) {

		if(list -> pos -> next != NULL) {

			tempNode = list -> pos -> next -> next;
			free(list -> pos -> next -> value);
			free(list -> pos -> next);
			list -> pos -> next = tempNode;
			list -> size--;
		}
	}
}

/*
* description: Removes all elements in the list with a specific value.
* Allocated memory for the value(s) in the element(s) WILL be freed.
* param[in]: list - The list.
* param[in]: value - Pointer to the value. SHOULD be a pointer containg the
* the same value, NOT a pointer to the same adress where value is stored.
* param[in]: valueCmp - Value compare function that compares if two values
* are the same.
*/
void listRemoveFromValue (linkedlist *list, void *value, ValCmpFunc valueCmp) {

	struct listnode *pos = list -> first;
	struct listnode *lastPos = list -> first;
	while (pos != NULL) {

		struct listnode *nextPos = pos -> next;
		if (valueCmp(pos -> value, value) == 1) {

			if (pos -> value != NULL) {

				free(pos -> value);
			}

			if (pos == list -> first) {

				list -> first = pos -> next;
				free(pos);
				pos = list -> first;
			} else {

				lastPos -> next = nextPos;
				free(pos);
				pos = nextPos;
			}
			list -> size--;

		} else {

			lastPos = pos;
			pos = pos -> next;
		}
	}
}

/*
* description: Sets the list's current position to the lists first position.
* param[in]: list - The list.
*/
void listFirst (linkedlist *list) {

	list -> pos = list -> first;
}

/*
* description: Sets the list's current position to next positon of that (if
* there is one).
* param[in]: list - The list.
*/
void listNext (linkedlist *list) {

	if (list -> pos != NULL) {

		list -> pos = list -> pos -> next;
	}
}

/*
* description: Frees all memory allocated by the list, inluding the list.
* param[in]: list - The list
*/
void listKill (linkedlist *list) {

	struct listnode *pos = list -> first;
	while (pos != NULL) {

		list -> first = pos -> next;
		if (pos -> value != NULL) {

			free(pos -> value);
		}
		free(pos);
		pos = list -> first;
	}
	free(list);
}

/*
* description: Checks if memory succesfully got allocated. If not, pointer will
* contain NULL and program will exit with errno.
* param[in]: mem - pointer to the allocated memory.
*/
void checkAlloc (void *mem) {

	if (mem == NULL) {

		perror("Error");
		exit(errno);
	}
}
