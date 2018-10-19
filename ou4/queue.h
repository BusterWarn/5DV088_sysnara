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

typedef struct node node;

typedef struct queue queue;

/*
* description: Creates and allocates memory for an empty list.
* return: The list.
*/
queue *queueEmpty (void);

/*
* description: Checks if list contains any elements.
* param[in]: list - The list.
* return: If true; 1, else 0.
*/
int queueIsEmpty (queue *q);

/*
* description: Gets the number of elements in list.
* param[in]: list - The list.
* return: The number of elements in list.
*/
int queueGetSize (queue *q);

/*
* description:
* param[in]: list - The list.
* return: Void pointer to the value in the list.
*/
void *queueFront (queue *q);

/*
* description:
* param[in]: list - The list.
* param[in]: value - Void pointer to the value the element will hold.
*/
void queueEnqueue (queue *q, void *value);

/*
* description:
* param[in]: list - The list.
*/
void queueDequeue (queue *q);

/*
* description: Frees all memory allocated by the list, inluding the list.
* param[in]: list - The list
*/
void queueKill (queue *q);

/*
* description: Checks if memory succesfully got allocated. If not, pointer will
* contain NULL and program will exit with errno.
* param[in]: mem - pointer to the allocated memory.
*/
void checkAlloc (void *mem);


#endif //LINKEDLIST
