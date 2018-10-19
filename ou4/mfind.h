#ifndef __MFIND__
#define __MFIND__

#include "queue.h"

typedef struct object {

	char *name;
	char type;
} object;

object *objectNew (char *name, char type);

char objectGetType (object *o);

int objectCmp (object *o, struct dirent *entry, struct stat *buf);

int objectFileCmp (object *file, struct dirent *entry);

int objectDirCmp (object *o, struct dirent *entry);

char *objectAddSuffix (object *o, char *suffix);

void stringAddForwardslashSuffix (char *str);

void objectKill (object *o);

void searchDirectory (queue *q, object *o, object *target);

void changeDirectory (char *path);

#endif	//__MFIND__
