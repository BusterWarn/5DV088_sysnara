#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "saferMemHandler.h"

void *smalloc (size_t size) {

	void *mem = malloc(size);
	if (mem == NULL) {

		perror("Malloc");
		exit errno;
	}
	return mem;
}

void *scalloc (size_t nmeb, size_t size) {

	void *mem = calloc(nmeb, size);
	if (mem == NULL) {

		perror("Calloc");
		exit errno;
	}
	return mem;
}

void *srealloc (void *ptr, size_t size) {

	void *mem = realloc(ptr, size);
	if (mem == NULL) {

		perror("Realloc");
		exit errno;
	}
	return mem;
}

void sfree (void *mem) {

	if (mem != NULL) {

		free(mem);
	}
}
