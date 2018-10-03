#include "mish.h"

int main (void) {

	int comms = 0;
	char buffer[MAXWORDS];
	command comLine[MAXCOMMANDS];

	printf("mish%% ");
	while (fgets(buffer, MAXWORDS, stdin) != NULL &&
		   strncmp(buffer, "exit", 4) != 0) {

		if (buffer[strlen(buffer) - 1] == '\n') {

			buffer[strlen(buffer) - 1] = '\0';
		}
		comms = parse(buffer, comLine);

		executeCommands(comms, comLine);
		printf("mish%% ");
	}

	printf("Goodbye!\n");
	return errno;
}

void executeCommands (int comms, command comLine[comms]) {

	pid_t children[comms];
	int parent = 1;

	for (int i = 0; i < comms && parent == 1; i++) {

		children[i] = fork();
		if (children[i] < 0 ) {

			perror("You forked up!");
		} else if (children[i] > 0) {

			parent = 0;
			executeCommand(comLine[i]);
			printf("Child here!\n");
		} else {

			waitpid(children[i]); //Research man waitpid you forking dup
			printf("Me estoy gusta parent porkflavor!\n");
		}
	}
}

void executeCommand (command c) {

	if (execvp(c.argv[0], c.argv) < 0) {

		fprintf(stderr, "Invalid command - ");
		perror(c.argv[0]);
	}
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
