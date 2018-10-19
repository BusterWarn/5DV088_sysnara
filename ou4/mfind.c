#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
// #include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mfind.h"
#include "parseMfind.h"
#include "saferMemHandler.h"

int main (int argc, char *argv[]) {

	args a;
	argsInit(&a);
	parseArgs(&a, argc, argv);
	search(&a);
	// argsKill(&a);
	return errno;
}

object *objectNew (char *name, char type) {

	object *o = smalloc(sizeof(*o));
	o -> name = name;
	o -> type = type;
	return o;
}

char objectGetType (object *o) {

	return o -> type;
}

int objectCmp (object *o, struct dirent *entry, struct stat *buf) {

	if (o -> type == 'd' && (buf -> st_mode & S_IFMT) == S_IFDIR) {

		return strcmp(o -> name, entry -> d_name) == 0;

	} else if (o -> type == 'f' && (buf -> st_mode & S_IFMT) == S_IFREG) {

		return strcmp(o -> name, entry -> d_name) == 0;

	} else if (o -> type == 'l' && (buf -> st_mode & S_IFMT) == S_IFLNK) {

		return strcmp(o -> name, entry -> d_name) == 0;
	}
	return 0;
}

int objectFileCmp (object *file, struct dirent *entry) {

	if (strcmp(file -> name, entry -> d_name) == 0) {

		return 1;
	}
	return 0;
}

int objectDirCmp (object *dir, struct dirent *entry) {

	char *suffix = NULL;
	int nameLength = 0;

	if (dir -> name[0] == '/') {

		for (int i = 0; dir -> name[i] != '\0'; i++) {

			if (dir -> name[i] == '/' && dir -> name[i + 1] != '\0') {

				suffix = &(dir -> name[i + 1]);
				nameLength = 0;
			}
			nameLength++;
		}
	}

	if (strncmp(suffix, entry -> d_name, nameLength) == 0) {

		return 1;
	}
	return 0;
}

char *objectAddSuffix (object *o, char *suffix) {

	int newLength = getStrLen(o -> name) + getStrLen(suffix);
	char *newPath = smalloc(sizeof(char) * (newLength + 2));
	newPath[0] = '\0';
	strcat(newPath, o -> name);
	strcat(newPath, suffix);
	newPath[newLength] = '\0';
	newPath[newLength + 1] = '\0';

	return newPath;
}

void stringAddForwardslashSuffix (char *str) {

	int len = getStrLen(str);
	str[len] = '/';
}

void objectKill (object *o) {

	sfree(o -> name);
	sfree(o);
}

void changeDirectory (char *path) {

	if (chdir(getenv("HOME")) < 0) {

		perror("Cannot acces home directory");
	}
	if (path != NULL) {

		if (chdir(path) < 0) {

			fprintf(stderr, "%s - ", path);
			perror(path);
		}
	}
}

void search (args *a) {

	changeDirectory(NULL);
	queue *q = queueEmpty();
	object *target;
	if (a -> type == 'd') {

		target = objectNew(a -> name, 'd');

	} else if (a -> type == 'f') {

		target = objectNew(a -> name, 'f');
	}

	for (int i = 0; i < a -> nrStart; i++) {

		object *o = objectNew(a -> start[i], 'd');
		queueEnqueue(q, (void *)o);
	}

	while (!queueIsEmpty(q)) {

		object *o = (object *)queueFront(q);
		queueDequeue(q);
		searchDirectory(q, o, target);
		objectKill(o);
	}
	queueKill(q);
}

void searchDirectory (queue *q, object *o, object *target) {

	DIR *dir;
	if (o -> name[0] == '/') {

		dir = opendir(o -> name + 1);
	} else {

		dir = opendir(o -> name);
	}

	if (dir == NULL) {

		perror(o -> name);
	} else {

		struct dirent *entry;
		struct stat buf;
		while ((entry = readdir(dir)) != NULL) {

			char *newPath = objectAddSuffix(o, entry -> d_name);
			fprintf(stderr, "%s\n", newPath);
			if (entry -> d_name[0] != '.' && lstat(newPath, &buf) < 0) {

				perror(newPath);
			} else if (entry -> d_name[0] != '.') {

				if (objectCmp(target, entry, &buf)) {

					printf("%s\n", o -> name);
				}
				fprintf(stderr, "KOM HIT\n");
				if ((buf.st_mode & S_IFMT) == S_IFDIR) {
					stringAddForwardslashSuffix(newPath);
					if (newPath != NULL) {

						object *newObj = objectNew(newPath, 'd');
						queueEnqueue(q, (void *)newObj);
					}
				}
			}
		}
		closedir(dir);
	}
}
