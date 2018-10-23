#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "parseMfind.h"
#include "saferMemHandler.h"


void parseArgs (args *a, int argc, char *argv[]) {

	int opt;
	int nrthr = 0;
	while ((opt = getopt(argc, argv, "t:p:")) != -1) {

		switch (opt) {

			case 't':
				if (optarg[0] != 'd' && optarg[0] != 'f' && optarg[0] != 'l') {

					fprintf(stderr, "Invalid argument: -t must be set to d, "
									 "f or l\n");
					argsKill(a);
					exit(-1);
				} else if (optarg[1] != '\0') {

					fprintf(stderr, "Invalid argument: -t takes exactly one "
									"argument\n");
					argsKill(a);
					exit(1);
				}
				a -> type = optarg[0];
				break;

			case 'p':
				if (optarg[0] == '\0') {

					fprintf(stderr, "Invalid argument: -p takes exactly one "
									 "argument\n");
					argsKill(a);
					exit(1);
				}
				nrthr = strToInt(optarg);
				if (nrthr > 0) {

					a -> nrthr = nrthr;
				}
				break;

			default:
				fprintf(stderr, "Invalid argument: %s\n", optarg);
				exit(1);
				break;
		}
	}

	a -> start = smalloc(sizeof(*a -> start) * argc);

	for (int i = optind; i < argc; i++) {

		if (i == argc - 1) {

			int strLen = getStrLen(argv[i]) + 1;
			a -> name = smalloc(sizeof(char) * strLen);
			strcpy(a -> name, argv[i]);

		} else {

			int strLen = getStrLen(argv[i]) + 1;
			a -> start[a -> nrStart] = smalloc(sizeof(char) * strLen);
			strcpy(a -> start[a -> nrStart], argv[i]);
			a -> nrStart++;
		}
	}
}

int strToInt (char *str) {

	int sum = 0;
	for (int i = 0; str[i] != '\0'; i++) {

		sum = sum * 10 + str[i] - '0';
	}
	return sum;
}

int getStrLen (char *str) {

	int i;
	for (i = 0; str[i] != '\0'; i++);
	return i;
}

void argsInit (args *a) {

	a -> type = '\0';
	a -> nrthr = 1;
	a -> name = NULL;
	a -> start = NULL;
	a -> nrStart = 0;
}

void argsPrint (args *a) {

	if (a != NULL) {

		fprintf(stderr, "----- Printing arguments -----\n");
		fprintf(stderr, "type:				%c\n", a -> type);
		fprintf(stderr, "nrthr:				%d\n", a -> nrthr);
		fprintf(stderr, "nrStart:			%d\n", a -> nrStart);
		fprintf(stderr, "start: 				%s\n", a -> name);
		for (int i = 0; i < a -> nrStart; i++) {

			fprintf(stderr, "start folder 1:	%s\n", a -> start[i]);
		}
	}
}

void argsKill (args *a) {

	if (a != NULL) {

		sfree(a -> name);

		if (a -> start != NULL) {

			for (int i = 0; i < a -> nrStart; i++) {

				sfree(a -> start[i]);
			}
			free(a -> start);
		}
	}
}
