#ifndef __PARSER__
#define __PARSER__

// typedef struct args args;
typedef struct args {

	char type;
	char *name;
	char **start;
	int nrthr;
	int nrStart;
} args;

void parseArgs (args *a, int argc, char *argv[]);

char* getNextWord (char *line, int *i);

int strToInt (char *str);

int getStrLen (char *str);

void argsInit (args *a);

void argsPrint (args *a);

void argsKill (args *a);

void search (args *a);

#endif	//__PARSER__
