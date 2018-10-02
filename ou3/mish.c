#include "mish.h"

int main (void) {

	int commands = 0;
	char inputStr[MAXWORDS];
	command comLine[MAXCOMMANDS];

	printf("mish%% ");
	fgets(inputStr, MAXWORDS, stdin);
	commands = parse(inputStr, comLine);

	while ((strcmp(comLine[0].argv[0], "exit")) != 0) {

		printCommands(commands, comLine);

		printf("mish%% ");
		fgets(inputStr, MAXWORDS, stdin);
		commands = parse(inputStr, comLine);
	}

	printf("Goodbye!\n");
	return errno;
}


void echo (int argc, const char *argv[]) {

	for (int i = 0; i < argc; i++) {

		fprintf(stdout, "%s\n", argv[i]);
	}
}

void changeDirectory (const char *dir) {

	char cwd [1000];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {

		printf("Current directory: %s\n", cwd);
	} else {

		fprintf(stderr, "Fail!\n");
	}
	printf("cd %s\n", dir);
	if (chdir(dir) != 0) {

		printf("Error!\n");
	}

	if (getcwd(cwd, sizeof(cwd)) != NULL) {

		printf("Current directory: %s\n", cwd);
	}
}

void printCommands (int commands, command comLine[commands]) {

	for (int i = 0; i < commands; i++) {

		printCommand(comLine[i]);
	}
}

void printCommand (command c) {

	printf("Commands: (%d)\n", c.argc);
	for (int i = 0; i < c.argc; i++) {

		printf("%s\n", c.argv[i]);
	}
	printf("infile: %s\n", c.infile == NULL ? "N/A" : c.infile);
	printf("outfile: %s\n", c.outfile == NULL ? "N/A" : c.outfile);
	printf("internal: %d\n\n", c.internal);
}
