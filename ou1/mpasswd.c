#include "linkedlist.h"
#include "mpasswd.h"

int main (int argc, const char *argv[]) {

	int fileType = fileValidation(argc, argv);

	if (fileType == ERROR) {

	        fprintf(stderr, " - quitting program\n");
	        return 0;
	 }
	 linkedlist *list = buildList(fileType, argv);

	 sortList(list);
	 printList(list);

	return 1;
}

linkedlist *buildList (int fileType, const char *argv[]) {

	FILE *fp;
	if (fileType == STDIN) {

		fp = stdin;
		printf("Please insert a row with the following format:\n"
				"username:password:UID:GID:GECOS:directory:shell\n"
				"If no more lines are required, write a '-1'\n");
	} else {

		fp = fopen(argv[1], "r");
	}

	linkedlist *list = listEmpty();
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
	user *u = malloc(sizeof(u));
	u -> username = getNextWord(line, &i);
	u -> password = getNextWord(line, &i);
	u -> UID = convertStringToInt(getNextWord(line, &i));
	u -> GID = convertStringToInt(getNextWord(line, &i));
	u -> GECOS = getNextWord(line, &i);
	u -> directory = getNextWord(line, &i);
	u -> shell = getNextWord(line, &i);

	return u;
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

void sortList (linkedlist *list) {

	int listSize  = listGetSize(list);
	user *users [listSize];
	listFirst(list);

	for (int i = 0; i < listSize; i++) {

		users[i] = (user *)listInspect(list);
		listNext(list);
	}

	sortArray(users, listSize);

	listFirst(list);
	for (int i = 0; i < listSize; i++) {

		listModifyValue(list, (void *) users[i]);
		listNext(list);
	}
}

void sortArray (user *users[], int size) {

	for (int i = 0; i < size; i++) {

		int sortUser = 1;
		for (int j = i; j > 0 && sortUser; j--) {

			sortUser = userCompare((void *) users[j - 1],(void *) users[j]);
			if (sortUser == 1) {

				user *tempUser = users[j];
				users[j] = users[j - 1];
				users[j - 1] = tempUser;
			} else {

			}
		}
	}
}

int lineValidation (unsigned char *line) {


	int validLine = 1;
	int i = 0;

	if (line != NULL) {

		unsigned char *username = getNextWord(line, &i);

		if (i >= 32) {

			validLine = 0;
		}
		// printf("username: %s\n", username);
		unsigned char *password = getNextWord(line, &i);
		// printf("password: %s\n", password);
		unsigned char *UID = getNextWord(line, &i);
		// printf("UID: %s\n", UID);
		unsigned char *GID = getNextWord(line, &i);
		// printf("GID: %s\n", GID);
		unsigned char *GECOS = getNextWord(line, &i);
		// printf("GECOS: %s\n", GECOS);
		unsigned char *directory = getNextWord(line, &i);
		// printf("directory: %s\n", directory);
		unsigned char *shell = getNextWord(line, &i);
		// printf("shell: %s\n", shell);


		if (username == NULL || UID == NULL || GID == NULL ||
			directory == NULL || shell == NULL) {

			printf("Invalid line, someone here is NULL\n");
			validLine = 0;
		}

		if (validLine) {

			if (!strIsNum(UID) || !strIsNum(GID)) {

				printf("Invalid line, not a number\n");
				validLine = 0;
			}
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

  	} else if (line[*i] == ':') {

		wordFound = 2;
	}


    while (wordFound >= 0) {

		if (line[*i] == '\0' && startOfWord == 0) {

			wordFound = -1;
		}

		if (wordFound == 0) {

			wordFound = 1;
			startOfWord = *i;

    	} else if (line[*i] == ':' || line[*i] == '\0') {

			if (wordFound == 1) {

				wordFound = -1;
				word = malloc(sizeof(char) * (*i - startOfWord + 1));
				char *loc = (char *)line;
				strncpy(word, &loc[startOfWord], *i - startOfWord + 1);
				word[*i - startOfWord] = '\0';

				if (line[*i] == '\0') {

					(*i)--;
				}
			} else {

				wordFound = -1;
			}
		}
		(*i)++;
	}
    return (unsigned char *)word;
}

void printList (linkedlist *list) {

	listFirst(list);
	int size = listGetSize(list);

	for (int i = 0; i < size; i++) {

		user *tempUser = listInspect(list);
		printUser(tempUser);
		listNext(list);
	}
}

void printUser (user *u) {

	if (u != NULL) {

		printf("%d:%s\n", u -> UID, u -> username);
	}
}

int userCompare (void *user1, void *user2) {

	int UID1 = ((user *)user1) -> UID;
	int UID2 = ((user *)user2) -> UID;
	int difference = 0;

	if (UID1 < UID2) {

		difference = -1;
	} else if (UID1 > UID2) {

		difference = 1;
	}

	return difference;
}
