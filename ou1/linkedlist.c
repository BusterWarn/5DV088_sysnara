#include "linkedlist.h"

list *listEmpty () {

	list *list = malloc(sizeof(*list));
	list -> first = NULL;
	list -> pos = NULL;
	list -> size = 0;
	return list;
}

int listIsEmpty (list *list) {

	return list -> size == 0;
}

int listGetSize (list *list) {

	return list -> size;
}

void *listInspect (list *list) {

	if (list -> pos != NULL) {

		return list -> pos -> value;
	}
	return NULL;
}

void listInsert (list *list, void *value) {

	if (list -> first == NULL) {

		list -> first = malloc(sizeof(node));
		list -> pos = list -> first;
		list -> first -> next = NULL;
	} else {

		struct node *tempNode = list -> first;
		list -> first = malloc(sizeof(node));
		list -> first -> next = tempNode;
	}

	list -> first -> value = value;
	list -> size++;
}

void listRemove (list *list, node *pos) {

	struct node* tempNode = NULL;

	if (pos != NULL) {

		if(pos -> next != NULL) {

			tempNode = pos -> next -> next;
			free(pos -> next -> value);
			free(pos -> next);
			pos -> next = tempNode;
			list -> size--;
		}
	}
}

void listRemoveFromValue (list *list, void *value, ValCmpFunc valueCmp) {

	struct node *pos = list -> first;
	struct node *lastPos = list -> first;
	while (pos != NULL) {

		struct node *nextPos = pos -> next;
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

void listFirst (list *list) {

	list -> pos = list -> first;
}

void listNext (list *list) {

	if (list -> pos != NULL) {

		list -> pos = list -> pos -> next;
	}
}

void listKill (list *list) {

	struct node *pos = list -> first;
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
