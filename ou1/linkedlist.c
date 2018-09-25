#include "linkedlist.h"


linkedlist *listEmpty (void) {

	linkedlist *list = malloc(sizeof(*list));
	list -> first = NULL;
	list -> pos = NULL;
	list -> size = 0;
	return list;
}

int listIsEmpty (linkedlist *list) {

	return list -> size == 0;
}

int listGetSize (linkedlist *list) {

	return list -> size;
}

void *listInspect (linkedlist *list) {

	if (list -> pos != NULL) {

		struct listnode *node = list -> pos;
		void *value = node -> value;
		return value;
	}
	return NULL;
}

void listInsert (linkedlist *list, void *value) {

	if (list -> first == NULL) {

		list -> first = malloc(sizeof(listnode));
		list -> pos = list -> first;
		list -> first -> next = NULL;
	} else {

		struct listnode *tempNode = list -> first;
		list -> first = malloc(sizeof(listnode));
		list -> first -> next = tempNode;
	}

	list -> first -> value = value;
	list -> size++;
}

void listModifyValue (linkedlist *list, void *newValue) {

	list -> pos -> value = newValue;
}

void listRemove (linkedlist *list) {

	struct node* tempNode = NULL;

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

void listFirst (linkedlist *list) {

	list -> pos = list -> first;
}

void listNext (linkedlist *list) {

	if (list -> pos != NULL) {

		list -> pos = list -> pos -> next;
	}
}

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
