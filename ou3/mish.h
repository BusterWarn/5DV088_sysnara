#ifndef MISH
#define MISH

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UP 1
#define DOWN -1
#define HOME 0

void readInputLine (const char *line);

void echo (int argc, const char *argv[]);

void changeDirectory (const char *dir);



#endif //MISH
