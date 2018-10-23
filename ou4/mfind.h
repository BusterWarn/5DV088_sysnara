#ifndef __MFIND__
#define __MFIND__

#include "queue.h"

pthread_mutex_t qmtx;
pthread_mutex_t nrmtx;
pthread_cond_t qcond;
int RUNNINGTHR;

typedef struct object {

	char *name;
	char type;
} object;

typedef struct trdArgs {

	queue *q;
	object *target;
} trdArgs;

object *objectNew (char *name, char type);

char objectGetType (object *o);

int objectCmp (object *o, struct dirent *entry, struct stat *buf);

int objectFileCmp (object *file, struct dirent *entry);

int objectDirCmp (object *o, struct dirent *entry);

char *objectAddSuffix (object *o, char *suffix);

void stringAddForwardslashSuffix (char *str);

void objectKill (object *o);

int trdSearchDir (queue *q, object *o, object *target);

void *mfind (void *arg);

int acivateMutex (void);

// void *trdSearchDir (void *arg);

#endif	//__MFIND__
