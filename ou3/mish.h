#ifndef MISH
#define MISH

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "parser.h"
#include "execute.h"

#define UP 1
#define DOWN -1
#define HOME 0

//void readInputLine (const char *line);

void executeCommands (int comms, command comLine[comms]);

void executeCommand (command c);

void echo (int argc, const char *argv[]);

void changeDirectory (const char *dir);

void printCommands (int comms, command comLine[comms]);

void printCommand (command c);

#endif //MISH
