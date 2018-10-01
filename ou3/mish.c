#include "mish.h"

int main (int argc, const char *argv[]) {

	if (argc > 1) {

		changeDirectory(argv[1]);
	}
	return 0;
}

void readInputLine (const char *line) {

	
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
