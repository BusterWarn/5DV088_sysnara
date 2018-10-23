#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
// #include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mfind.h"
#include "parseMfind.h"
#include "saferMemHandler.h"


int main (int argc, char *argv[]) {


	fprintf(stderr, "\n");
	args a;
	argsInit(&a);
	pthread_mutex_lock(&qmtx);
	parseArgs(&a, argc, argv);
	// argsPrint(&a);
	search(&a);
	// argsKill(&a);
	pthread_mutex_unlock(&qmtx);
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
	} else if (o -> type == '\0') {

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

	int oldLength = getStrLen(o -> name);
	if (o -> name[oldLength - 1] != '/') {

		oldLength++;
	}
	int newLength =  oldLength + getStrLen(suffix);


	char *newPath = smalloc(sizeof(char) * (newLength + 2));
	newPath[0] = '\0';
	strcat(newPath, o -> name);
	newPath[oldLength - 1] = '/';
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

/*void search (args *a) {

	pthread_t trd [a -> nrthr];
	trdArgs trdArg;
	trdArg.q = queueEmpty();
	trdArg.o = NULL;
	trdArg.target = objectNew(a -> name, a -> type);
	int activeTrd = 0;

	for (int i = 0; i < a -> nrStart; i++) {

		object *o = objectNew(a -> start[i], 'd');
		queueEnqueue(trdArg.q, (void *)o);
	}
	for (int i = 0; i < a -> nrthr; i++) {

		trd[i] = 0;
	}

	while (!queueIsEmpty(trdArg.q)) {

		if (activeTrd < a -> nrthr - 1) {

			activeTrd++;
		} else {

			activeTrd = 0;
		}
		trdArg.o = (object *)queueFront(trdArg.q);
		queueDequeue(trdArg.q);
		if (trd[activeTrd] != 0) {

			pthread_join(trd[activeTrd], NULL);
		}
		fprintf(stderr, "Main thread %lu\n", pthread_self());
		int rc = pthread_create(&trd[activeTrd], NULL, trdSearchDir, &trdArg);
		if (rc != 0) {
			fprintf(stderr, "pthread_create failed with exit code %d\n", rc);
			exit(1);
		}
		// searchDirectory(q, o, target);
		objectKill(trdArg.o);
	}

	for (int i = 0; i < a -> nrthr; i++) {

		if (trd[i] == 0) {

			fprintf(stderr, "Noninitiated thread nr %d\n", i);
		} else {

			pthread_join(trd[i]);
		}
	}
	queueKill(trdArg.q);
}

void *trdSearchDir (void *arg) {

	fprintf(stderr, "Thread %lu\n", pthread_self());
	trdArgs *a = (trdArgs *)arg;
	DIR *dir = opendir(a -> o -> name);
	if (dir == NULL) {

		perror(a -> o -> name);
	} else {

		struct dirent *entry;
		struct stat buf;
		while ((entry = readdir(dir)) != NULL) {

			char *newPath = objectAddSuffix(a -> o, entry -> d_name);
			if (entry -> d_name[0] != '.' && lstat(newPath, &buf) < 0) {

				perror(newPath);
			} else if (entry -> d_name[0] != '.') {

				if (objectCmp(a -> target, entry, &buf)) {

					printf("%s\n", newPath);
				}
				if ((buf.st_mode & S_IFMT) == S_IFDIR) {
					stringAddForwardslashSuffix(newPath);
					if (newPath != NULL) {

						object *newObj = objectNew(newPath, 'd');
						pthread_mutex_lock(&qmtx);
						queueEnqueue(a -> q, (void *)newObj);
						pthread_mutex_lock(&qmtx);
					}
				}
			}
		}
		closedir(dir);
	}
	pthread_exit(NULL);
	return 0;
}
*/

void search (args *a) {

	int rc;
	if ((rc = acivateMutex()) != 0) {

		fprintf(stderr, "phtread_cond_init failed with error code %d\n", rc);
		exit(1);
	}

	RUNNINGTHR = 0;
	pthread_t trd[a -> nrthr];
	trdArgs trdArg;
	trdArg.q = queueEmpty();
	trdArg.target = objectNew(a -> name, a -> type);

	for (int i = 0; i < a -> nrStart; i++) {

		object *o = objectNew(a -> start[i], 'd');
		queueEnqueue(trdArg.q, (void *)o);
	}

	for (int i = 0; i < a -> nrthr; i++) {

		rc = pthread_create(&trd[i], NULL, mfind, &trdArg);
		if (rc != 0) {
			fprintf(stderr, "pthread_create failed with error code %d\n", rc);
			exit(1);
		}
	}

	int sum = 0;
	void *reads = NULL;
	for (int i = 0; i < a -> nrthr; i++) {

		pthread_join(trd[i], &reads);
		printf("Thread: %ld Reads: %d\n", trd[i], *(int *)reads);
		sum += *(int *)reads;
		sfree(reads);
	}
	printf("\nTotal reads: %d\n", sum);
	queueKill(trdArg.q);
}

void *mfind (void *arg) {

	int *reads = malloc(sizeof(*reads));
	*reads = 0;
	fprintf(stderr, "Thread %lu\n", pthread_self());
	trdArgs *a = (trdArgs *)arg;
	object *o = NULL;
	int runLoop = 1;

	while (runLoop) {

		fprintf(stderr, "Locking qmtx at thread %ld (mfind)\n", pthread_self());
		pthread_mutex_lock(&qmtx);
		if (!queueIsEmpty(a -> q)) {

			o = queueFront(a -> q);
			queueDequeue(a -> q);
		} else {

			fprintf(stderr, "Locking nrmtx at thread %ld (mfind)\n", pthread_self());
			pthread_mutex_lock(&nrmtx);
			if (RUNNINGTHR == 0) {

				runLoop = 0;
				pthread_cond_broadcast(&qcond);
			}
			fprintf(stderr, "	Unlocking nrmtx at thread %ld (mfind)\n", pthread_self());
			pthread_mutex_unlock(&nrmtx);
			if (runLoop) {

				fprintf(stderr, "Cond waiting from thread %ld (mfind)\n", pthread_self());
				pthread_cond_wait(&qcond, &qmtx);
			}
		}
		fprintf(stderr, "	Unlocking qmtx at thread %ld (mfind)\n", pthread_self());
		pthread_mutex_unlock(&qmtx);

		if (o != NULL) {

			*reads += trdSearchDir(a -> q, o, a -> target);
			o = NULL;
		}
	}

	return (void *)reads;
}

int trdSearchDir (queue *q, object *o, object *target) {

	fprintf(stderr, "Locking nrmtx at thread %ld (trdSearchDir)\n", pthread_self());
	pthread_mutex_lock(&nrmtx);
	RUNNINGTHR++;
	fprintf(stderr, "	Unlocking nrmtx at thread %ld (trdSearchDir)\n", pthread_self());
	pthread_mutex_unlock(&nrmtx);

	int succesfullRead = 0;
	DIR *dir = opendir(o -> name);
	if (dir == NULL) {

		perror(o -> name);
	} else {

		struct dirent *entry;
		struct stat buf;
		while ((entry = readdir(dir)) != NULL) {

			char *newPath = objectAddSuffix(o, entry -> d_name);
			if (entry -> d_name[0] != '.' && lstat(newPath, &buf) < 0) {

				perror(newPath);
			} else if (entry -> d_name[0] != '.') {

				if (objectCmp(target, entry, &buf)) {

					printf("%s\n", newPath);
				}
				if ((buf.st_mode & S_IFMT) == S_IFDIR) {

					stringAddForwardslashSuffix(newPath);
					if (newPath != NULL) {

						object *newObj = objectNew(newPath, 'd');
						fprintf(stderr, "Locking nrmtx at thread %ld (trdSearchDir)\n", pthread_self());
						pthread_mutex_lock(&qmtx);
						queueEnqueue(q, (void *)newObj);
						fprintf(stderr, "	Unlocking nrmtx at thread %ld (trdSearchDir)\n", pthread_self());
						pthread_mutex_unlock(&qmtx);
						fprintf(stderr, "Signaling qcond from thread %ld (trdSearchDir)\n", pthread_self());
						pthread_cond_signal(&qcond);
					}
				}
			}
		}
		succesfullRead = 1;
		closedir(dir);
	}
	fprintf(stderr, "Locking nrmtx at thread %ld (trdSearchDir - endoffunction)\n", pthread_self());
	pthread_mutex_lock(&nrmtx);
	RUNNINGTHR--;
	fprintf(stderr, "	Unlocking nrmtx at thread %ld (trdSearchDir - endoffunction)\n", pthread_self());
	pthread_mutex_unlock(&nrmtx);

	return succesfullRead;
}

int acivateMutex (void) {

	int rc = 0;

	if ((rc = pthread_mutex_init(&qmtx, NULL) != 0)) {

		return rc;
	}
	if ((rc = pthread_mutex_init(&nrmtx, NULL) != 0)) {

		return rc;
	}
	if ((rc = pthread_cond_init(&qcond, NULL) != 0)) {

		return rc;
	}
	return rc;
}
