#ifndef SIGHANT
#define SIGHANT


typedef void Sigfunc(int);

/*
* description: Signal function that handles a signal with sigaction().
* param[in]: signo - The signal.
* param[in]: Sigfunc - Function pointer to a Signal handler function. Will be
* sa_handler for sigaction struct.
*/
void signalHandler (int signo, Sigfunc *func);

#endif //SIGHANT
