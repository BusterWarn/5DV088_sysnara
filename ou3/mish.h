#ifndef MISH
#define MISH

#include "parser.h"

#define UP 1
#define DOWN -1
#define HOME 0

static pid_t CHILDPIDS[MAXCOMMANDS];
static int NRCHILDREN;

void executeExternalCommands (int comms, command comLine[comms]);

void executeCommand (command c);

int openPipes(int nrOfPipes, int pipes[nrOfPipes][2]);

void closePipes(int nrOfPipes, int pipes[nrOfPipes][2]);

void waitForChildren(int nrOfChildren, pid_t children[nrOfChildren]);

void executeEcho (command c);

void executeCd (command c);

int containsInternalCommands (int comms, command comLine[comms]);

void printWithoutQuotationMarks(char *str);

void printCommands (int comms, command comLine[comms]);

void printCommand (command c);

void terminateChildren (int signo);

#endif //MISH
