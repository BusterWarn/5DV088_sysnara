#ifndef SIGHANT
#define SIGHANT


typedef void Sigfunc(int);
Sigfunc *signal(int, Sigfunc *);


#endif //SIGHANT
