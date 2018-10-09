 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "parser.h"
#include "execute.h"
#include "mish.h"
#include "sighant.h"

int main (void) {

	NRCHILDREN = 0;
	signal(SIGINT, terminateChildren);

	int comms = 0;
	char buffer[MAXWORDS];
	command comLine[MAXCOMMANDS];

	fprintf(stderr, "mish%% ");
	fflush(stderr);
	while (fgets(buffer, MAXWORDS, stdin) != NULL &&
		   strncmp(buffer, "exit", 4) != 0 &&
		   strncmp(buffer, "quit", 4) != 0) {

		if (buffer[strlen(buffer) - 1] == '\n') {

			buffer[strlen(buffer) - 1] = '\0';
		}
		comms = parse(buffer, comLine);

		if (containsInternalCommands(comms, comLine) == 0) {

			executeExternalCommands(comms, comLine);
		} else if (strncmp(comLine[0].argv[0], "echo", 4) == 0 && comms == 1) {

			executeEcho(comLine[0]);
		} else if (strncmp(comLine[0].argv[0], "cd", 2) == 0 && comms == 1) {

			executeCd(comLine[0]);
		} else {

			fprintf(stderr, "Invalid input - echo and / or cd cannot be"
			" piped\n");
		}
		fprintf(stderr, "mish%% ");
		fflush(stderr);
	}
	fprintf(stderr, "\n");
	return errno;
}

void executeExternalCommands (int comms, command comLine[comms]) {

	pid_t children[comms];
	int pipes[comms - 1][2];
	int parent = openPipes(comms - 1, pipes);

	for (int i = 0; i < comms && parent == 1; i++) {

		children[i] = fork();

		if (children[i] < 0 ) {					// FAILED FORK

			perror("You forked up!");
		} else if (children[i] > 0) {			// PARENT

			CHILDPIDS[NRCHILDREN] = children[i];
			NRCHILDREN++;
		} else {								// CHILD

			parent = 0;

			if (i < comms - 1) {

				dupPipe(pipes[i], WRITE_END, STDOUT_FILENO);
			}
			if (i > 0) {

				dupPipe(pipes[i - 1], READ_END, STDIN_FILENO);
			}
			if (i == comms - 1 && comLine[i].outfile != NULL) {

				redirect(comLine[i].outfile, WRITE_END, STDOUT_FILENO);
			}
			if (i == 0 && comLine[i].infile != NULL) {

				redirect(comLine[i].infile, READ_END, STDIN_FILENO);
			}

			closePipes(comms - 1, pipes);
			executeCommand(comLine[i]);
		}
	}

	if (parent == 1) {

		closePipes(comms - 1, pipes);
		waitForChildren(comms, children);
		NRCHILDREN = 0;
	}
}

void executeCommand (command c) {

	if (execvp(c.argv[0], c.argv) < 0) {

		fprintf(stderr, "Invalid command - ");
		perror(c.argv[0]);
		exit(1);
	}
}

int openPipes(int nrOfPipes, int pipes[nrOfPipes][2]) {

	int pipesNoError = 1;
	for (int i = 0; i < nrOfPipes && pipesNoError > 0; i++) {

		if (pipe(pipes[i]) < 0) {

			perror("Opening parent pipes");
			pipesNoError = -1;
		}
	}
	return pipesNoError;
}

void closePipes (int nrOfPipes, int pipes[nrOfPipes][2]) {

	for (int i = 0; i < nrOfPipes; i++) {

		if (close(pipes[i][READ_END])) {

			perror("Closing pipe's READ_END");
		}
		if (close(pipes[i][WRITE_END])) {

			perror("Closing pipe's WRITE_END");
		}
	}
}

void waitForChildren(int nrOfChildren, pid_t children[nrOfChildren]) {

	for (int i = 0; i < nrOfChildren; i++) {

		if (waitpid(children[i], NULL, 0) < 0) {

			perror("waitpid");
		}
	}
}

void executeEcho (command c) {

	for (int i = 1; i < c.argc; i++) {

		printWithoutQuotationMarks(c.argv[i]);
		if (i < c.argc - 1) {

			printf(" ");
		} else if (i == c.argc - 1) {

			printf("\n");
		}
	}
}

void printWithoutQuotationMarks(char *str) {

	int usedBackSlash = 0;
	for (int i = 0; str[i] != '\0'; i++) {

		if (str[i] == '"' && usedBackSlash == 1) {

			printf("%c", str[i]);
		} else if (usedBackSlash == 1) {

			usedBackSlash = 0;
		} else if (str[i] == '\\') {

			usedBackSlash = 1;
		} else if (str[i] != '"') {

			printf("%c", str[i]);
		}
	}
}

void executeCd (command c) {

	if (c.argc > 1) {

		if (chdir(c.argv[1]) < 0) {

			perror("cd");
		}
	} else if (c.argc == 1) {

		if (chdir(getenv("HOME")) < 0) {

			perror("cd");
		}
	}
}

int containsInternalCommands (int comms, command comLine[comms]) {

	int internalCommands = 0;
	for (int i = 0; i < comms && internalCommands == 0; i++) {

		if (strncmp(comLine[i].argv[0], "echo", 4) == 0 ||
			strncmp(comLine[i].argv[0], "cd", 2) == 0) {

				internalCommands = 1;
			}
	}
	return internalCommands;
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

void terminateChildren (int signo)  {

	for (int i = 0; i < NRCHILDREN; i++) {

		if (kill(CHILDPIDS[i], signo) < 0) {

			perror("Terminating child process");
		}
	}
	fprintf(stderr, "\n");
}
