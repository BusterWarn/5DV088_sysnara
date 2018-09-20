#include "linkedlist.h"
#include "mpasswordsort.h"

int main (int argc, const char *argv[]) {

	int fileType = fileValidation(argc, argv);

	if (fileType == ERROR) {

	        fprintf(stderr, " - quitting program\n");
	        return 0;
	 }
	 list *list = buildList(fileType, argv);
	 printList(list);

	return 1;
}

list *buildList (int fileType, const char *argv[]) {

	FILE *fp;
	if (fileType == STDIN) {

		fp = stdin;
		printf("Please insert a row with the following format:\n"
				"username:password:UID:GID:GECOS:directory:shell\n"
				"If no more lines are required, write a '-1'\n");
	} else {

		fp = fopen(argv[1], "r");
	}

	list *list = listEmpty();
	unsigned char *line = readLine(fp);
	int stage = 1;

	while (stage >= 0) {

		stage = lineValidation(line);
		if (stage == 1) {

			listInsert(list, (void *)buildUser(line));
		} else if (stage == 0) {

			printf("Invalid format of line: '%s'. Line will not be "
			"saved.\n", line);
		}

		if (line != NULL) {

			free(line);
		}
		line = readLine(fp);
	}

	if (fp != NULL && fileType != STDIN) {

		fclose(fp);
	}
	return list;
}

user *buildUser (unsigned char *line) {

	int i = 0;
	user *user = malloc(sizeof(*user));
	user -> username = getNextWord(line, &i);
	user -> password = getNextWord(line, &i);
	user -> UID = convertStringToInt(getNextWord(line, &i));
	user -> GID = convertStringToInt(getNextWord(line, &i));
	user -> GECOS = getNextWord(line, &i);
	user -> directory = getNextWord(line, &i);
	user -> shell = getNextWord(line, &i);

	return user;
}

int convertStringToInt (unsigned char *str) {

	int i = 0;
	int num = 0;

	while (str[i] != '\0') {

		num = (num * 10) + str[i] - '0';
		i++;
	}
	return num;
}

int lineValidation (unsigned char *line) {


	int validLine = 1;
	int i = 0;

	if (line != NULL) {

		printf("username - ");
		unsigned char *username = getNextWord(line, &i);

		if (i >= 32) {

			printf("Invalid line, username to long\n");
			validLine = 0;
		}
		printf("password - ");
		unsigned char *password = getNextWord(line, &i);
		printf("UID - ");
		unsigned char *UID = getNextWord(line, &i);
		printf("GID - ");
		unsigned char *GID = getNextWord(line, &i);
		printf("GECOS - ");
		unsigned char *GECOS = getNextWord(line, &i);
		printf("directory - ");
		unsigned char *directory = getNextWord(line, &i);
		printf("shell - ");
		unsigned char *shell = getNextWord(line, &i);

		if (username == NULL || UID == NULL || GID == NULL ||
			directory == NULL || shell == NULL) {

			printf("Invalid line, someone here is NULL\n");
			validLine = 0;
		}

		if (!strIsNum(UID) || !strIsNum(GID)) {

			printf("Invalid line, not a number\n");
			validLine = 0;
		}

		free(username);
		free(password);
		free(UID);
		free(GECOS);
		free(directory);
		free(shell);
	} else {

		validLine = -1;
	}
	return validLine;
}

/*
* description: Validates number of arguments and that textfile (argv[1]) can be
* read and written to.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 1 if arguments are valid, else 0.
*/
int fileValidation (int argc, const char *argv[]) {

    FILE *fp;
	int fileStatus;

    if (argc == 1) {

		fileStatus = STDIN;
	} else if (argc == 2) {

		fileStatus = FFILE;
	} else {

        fprintf(stderr, "To many/few argument");
        return ERROR;
    }

	if (fileStatus == FFILE) {

		fp = fopen(argv[1], "r");
		if (fp == NULL) {

			fprintf(stderr, "Cannot read '%s'", argv[1]);
			return ERROR;
		} else {

			char charV = fgetc(fp);
			if (charV < 1) {

				fileStatus = ERROR;
			}
		}
		fclose(fp);
	}
    return fileStatus;
}

int strIsNum (unsigned char *str) {

	int isNum = 1;
	int i = 0;

	while (str[i] > 0 && isNum) {

		if (str[i] < '0' || str[i] > '9') {

			isNum = 0;
		}
		i++;
	}
	return isNum;
}


/*
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
unsigned char *readLine (FILE *fp) {

    int length = 0;
    int buffer = 100;
    unsigned char *line = malloc(sizeof(char) * buffer);
    unsigned char currChar = fgetc(fp);

    while (currChar != '\n' && currChar != 4 && currChar != 255) {

  		//If a carriage return is read, skip it.
  		if (currChar == '\r') {

  			currChar = fgetc(fp);
  		} else {

  			if (length >= buffer - 2) {

  	            buffer = buffer * 2;
  	            line = realloc(line, sizeof(char) * buffer);
  	        }
  	        line[length] = currChar;

  	        length++;
  	        currChar = fgetc(fp);
  		}
    }

	if (length > 0) {

		line[length] = '\0';
		line = realloc(line, sizeof(char) * length + 1);
	} else {

		free(line);
		line = NULL;
	}

	printf("-----Line: '%s'\n\n\n", line);
    return line;
}

/*
* description: Find the next word (if any) in an array of chars and returns it.
* param[in]: line - A pointer to an array where the next word (if any) is to
* be found.
* param[in]: i - The index where the function should start looking in the
* array.
* return: If found; the word, else NULL.
*/
unsigned char* getNextWord (unsigned char *line, int *i) {

    int wordFound = 0;
    int startOfWord = 0;
    char *word = NULL;

    if (line[*i] == '\0') {
      wordFound = -1;
    }

    while (wordFound >= 0) {

		if (line[*i] == '\0' && startOfWord == 0) {

			wordFound = -1;
		}

		if (wordFound == 0) {

			wordFound = 1;
			startOfWord = *i;

    	} else if (line[*i] == ':' || line[*i] == '\0') {

			wordFound = -1;
			word = malloc(sizeof(char) * (*i - startOfWord + 1));
			char *loc = (char *)line;
			strncpy(word, &loc[startOfWord], *i - startOfWord + 1);
			word[*i - startOfWord] = '\0';

			if (line[*i] == '\0') {

				(*i)--;
			}
		}
		(*i)++;
	}
	printf("word: '%s'\n", word);
    return (unsigned char *)word;
}

void printList (list *list) {

	listFirst(list);
	int size = listGetSize(list);

	for (int i = 0; i < size; i++) {

		printf("i: %d/%d - ", i, size);
		user *user = listInspect(list);
		printUser(user);
		listNext(list);
	}
}

void printUser (user *user) {

	if (user != NULL) {

		printf("%s:%s:%d:%d:%s:%s:%s\n", user -> username, user -> password,
		user -> UID, user -> GID, user -> GECOS, user -> directory,
		user -> shell);
	} else {

		printf("\nNULLNULLNULLNULLNULL\n\n");
	}
}

int userCompare (void *user1, void *user2) {

	char *userName1 = ((user *)user1) -> username;
	char *userName2 = ((user *)user2) -> username;
	int difference = 0;
	int i = 0;

	while (userName1[i] != '\0' && userName2[i] != '\0' && difference == 0) {

		if (userName1[i] > userName2[i]) {

			difference = 1;
		} else if (userName1[i] < userName2[i]) {

			difference = -1
		}
		i++;
		return difference;
	}

}
