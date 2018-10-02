#ifndef MISH
#define MISH

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "parser.h"

#define UP 1
#define DOWN -1
#define HOME 0

//void readInputLine (const char *line);

void echo (int argc, const char *argv[]);

void changeDirectory (const char *dir);

void printCommands (int commands, command comLine[commands]);

void printCommand (command c);

#endif //MISH
