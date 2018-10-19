/*
* A dynamic single-linked list. Each element (node) in the list holds up to one
* void pointer. The positon of the list is saved in the list's struct, and user
* will not need to keep track of position.
*
* Author: Buster Hultgren Wärn <dv17bhn@cs.umu.se>
*
* Final build: 2018-09-26
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "queue.h"
#include "saferMemHandler.h"

struct node {

	void *value;
	node *next;
};

struct queue {

	int size;
	node *first;
	node *last;
};

/*
* description: Creates and allocates memory for an empty list.
* return: The list.
*/
queue *queueEmpty (void) {

	queue *q = smalloc(sizeof(*q));
	checkAlloc((void *)q);
	q -> first = NULL;
	q -> last = NULL;
	q -> size = 0;
	return q;
}

/*
* description: Checks if list contains any elements.
* param[in]: list - The list.
* return: If true; 1, else 0.
*/
int queueIsEmpty (queue *q) {

	return q -> size == 0;
}

/*
* description: Gets the number of elements in list.
* param[in]: list - The list.
* return: The number of elements in list.
*/
int queueGetSize (queue *q) {

	return q -> size;
}

/*
* description:
* param[in]: list - The list.
* return: Void pointer to the value in the list.
*/
void *queueFront (queue *q) {

	if (q -> first != NULL) {

		return q -> first -> value;
	}
	return NULL;
}

/*
* description:
* param[in]: list - The list.
* param[in]: value - Void pointer to the value the element will hold.
*/
void queueEnqueue (queue *q, void *value) {

	if (q -> first == NULL) {

		q -> first = smalloc(sizeof(node));
		checkAlloc((void *)(q -> first));
		q -> first -> next = NULL;
		q -> last = q -> first;
	} else {

		q -> last -> next = smalloc(sizeof(node));
		checkAlloc((void *)(q -> last));
		q -> last = q -> last -> next;
		q -> last -> next = NULL;
	}

	q -> last -> value = value;
	q -> size++;
}

/*
* description:
* param[in]: list - The list.
*/
void queueDequeue (queue *q) {

	if (q -> first != NULL) {

		if (q -> size == 1) {

			// sfree(q -> first -> value);
			// sfree(q -> first);
			q -> first = NULL;
			q -> last = NULL;
		} else {

			node *tempNode = q -> first -> next;

			// sfree(q -> first -> value);
			// sfree(q -> first);
			q -> first = tempNode;
		}
		q -> size--;
	}
}

/*
* description: Frees all memory allocated by the list, inluding the list.
* param[in]: list - The list
*/
void queueKill (queue *q) {

	while (q -> first != NULL) {

		node *pos = q -> first -> next;
		q -> first = q -> first -> next;
		if (pos -> value != NULL) {

			free(pos -> value);
		}
		free(pos);
	}
	free(q);
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
