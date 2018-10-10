#include <stdio.h>
#include <signal.h>
#include "sighant.h"

/*
* description: Signal function that handles a signal with sigaction().
* param[in]: signo - The signal.
* param[in]: Sigfunc - Function pointer to a Signal handler function. Will be
* sa_handler for sigaction struct.
*/
void signalHandler (int signo, Sigfunc *sigFunc) {

	struct sigaction act;
	act.sa_handler = sigFunc;
	act.sa_flags |= SA_RESTART;
	
	if (sigemptyset(&act.sa_mask) < 0) {

		perror("sigemptyset()");
	}
	if (sigaction(signo, &act, NULL) < 0) {

		perror("sicacton()");
	}
}
