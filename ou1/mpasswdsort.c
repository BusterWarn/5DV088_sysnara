/*
* Program that reads user info from stdin or file. The user info is then sorted
* and printed to stdout.
*
* Program uses a the interface of a linkedlist (linkedlist.h).
*
* Format of each line should be as following.
* username:password:UID:GID:GECOS:directory:shell
*
* These fields are mandatory and must follow format:
* username - Can be max 32 characthers long.
* UID - Must be a string representing an integer.
* GID - Must be a string representing an integer.
* directory - No format required.
* shell - No format required.
*
* Author: Buster Hultgren Wärn <dv17bhn@cs.umu.se>
*
* Final build: 2018-09-26
*/

#include "linkedlist.h"
#include "mpasswdsort.h"

struct user {

	unsigned char *username;
	int UID;
};

int main (int argc, const char *argv[]) {

	int fileType = fileValidation(argc, argv);

	if (fileType == ERROR) {

	        fprintf(stderr, " - quitting program\n");
			errno = 1;
	        return errno;
	 }

	 linkedlist *list = buildList(fileType, argv);
	 sortList(list);
	 printList(list);
	 killUserList(list);

	return errno;
}

/*
* description: Builds a list containing user data.
* param[in]: fileType - The type of stream data is read from. 1 For stdin, 2
* from file.
* param[in]: argv[] - The arguments for the executable program.
* return: The list containing the user data.
*/
linkedlist *buildList (int fileType, const char *argv[]) {

	FILE *fp;
	if (fileType == STDIN) {

		fp = stdin;
	} else {

		fp = fopen(argv[1], "r");
	}

	linkedlist *list = listEmpty();
	unsigned char *line = readLine(fp);
	int stage = 1;

	for (int i = 0; stage >= 0; i++) {

		stage = lineValidation(line, i + 1);
		if (stage == 1) {

			listInsert(list, (void *)buildUser(line));
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

/*
* description: Builds a user from a string of user data. Allocates memory for
* the user struct.
* param[in]: line - The string of user data. Should be a line from the input
* stream.
* return: Pointer to the user struct.
*/
user *buildUser (unsigned char *line) {

	int i = 0;
	user *u = malloc(sizeof(*u));
	checkAlloc((void *)u);
	u -> username = getNextField(line, &i);
	//Skip password
	unsigned char *password = getNextField(line, &i);
	unsigned char *UID = getNextField(line, &i);
	u -> UID = convertStringToInt(UID);

	if (password != NULL) {

		free(password);
	}
	if (UID != NULL) {

		free(UID);
	}

	return u;
}

/*
* description: Checks if a string is a number or not.
* param[in]: str - The string.
* return: If str is a numer; 1, else 0.
*/
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
* description: Converts a string to an integer.
* param[in]: str - The string.
* return: The integer.
*/
int convertStringToInt (unsigned char *str) {

	int i = 0;
	int num = 0;

	while (str[i] != '\0') {

		num = (num * 10) + str[i] - '0';
		i++;
	}
	return num;
}

/*
* description: Compares two users to see if which one has the lower UID.
* param[in]: user1 - The first user to be compared, will be compared to user2.
* param[in]: user2 - The second user to be compared, will be compared to user1.
* return: 0 If UID's of both users are same, -1 if user1's UID is smaller, 1 if
* user2's UID is smaller.
*/
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

/*
* description: Sorts the list of userdata.
* param[in]: list - The list.
*/
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

/*
* description: Sorts an array of user data - used in sortList().
* param[in]: users[] - Array containing pointers to user structs.
* param[in]: size - Size of the array.
*/
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

/*
* description: Validates that input arguments to the program are correct and
* that file can be read.
* param[in]: argc - Number of input arguments to program.
* param[in]: argv[] - The input arguments.
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
		}
		fclose(fp);
	}
    return fileStatus;
}

/*
* description: Validates a string of userinfo to see if it follows the
* specified format of the input userdata.
* param[in]: line - The string containing the user data.
* param[in]: lineNr - The number of which line is being validated.
* return: 1 if string is valid, 0 if string is not, -1 if input string is NULL.
*/
int lineValidation (unsigned char *line, int lineNr) {


	int validLine = 1;
	int i = 0;

	if (line != NULL) {

		unsigned char *username = getNextField(line, &i);
		int nameLength = i;
		unsigned char *password = getNextField(line, &i);
		unsigned char *UID = getNextField(line, &i);
		unsigned char *GID = getNextField(line, &i);
		unsigned char *GECOS = getNextField(line, &i);
		unsigned char *directory = getNextField(line, &i);
		unsigned char *shell = getNextField(line, &i);


		validLine = lineFieldsValidation(lineNr, nameLength, username, UID,
										 GID, directory, shell);
		if (password != NULL) {

			free(password);
		}

		if (GECOS != NULL) {

			free(GECOS);
		}
	} else {

		validLine = -1;
	}
	return validLine;
}

/*
* description: Help function for lineValidation(). Validates all fields that
* are read from lineValidation.
* param[in]: lineNr - The number of which line is being validated (same as in
* lineValidation()).
* param[in]: nameLength - The length of the string 'username'.
* param[in]: username - String to username found in lineValidation().
* param[in]: username - String to UID found in lineValidation().
* param[in]: username - String to GID found in lineValidation().
* param[in]: username - String to directory found in lineValidation().
* param[in]: username - String to shell found in lineValidation().
* return: If fields are valid; 1, else 0.
*/
int lineFieldsValidation (int lineNr, int nameLength, unsigned char *username,
	 					  unsigned char *UID, unsigned char *GID,
						  unsigned char *directory, unsigned char *shell) {

	int validLine = 1;
	//Validating username
	if (username == NULL) {

		fprintf(stderr, "Line %d: field username is empty\n", lineNr);
		validLine = 0;
	} else if (nameLength >= 32) {

		fprintf(stderr, "Line %d: username is to long\n", lineNr);
		free(username);
		validLine = 0;
	} else {

		free(username);
	}
	//Validating UID
	if (UID == NULL) {

		fprintf(stderr, "Line %d: field UID is empty\n", lineNr);
		validLine = 0;
	} else if (!strIsNum(UID)) {

		fprintf(stderr, "Line %d: field UID is not an integer\n", lineNr);
		free(UID);
		validLine = 0;
	} else if (convertStringToInt(UID) < 0){

		fprintf(stderr, "Line %d: field UID must be positive\n", lineNr);
		free(UID);
		validLine = 0;
	} else {

		free(UID);
	}
	//Validating GID
	if (GID == NULL) {

		fprintf(stderr, "Line %d: field GID is empty\n", lineNr);
		validLine = 0;
	} else if (!strIsNum(GID)) {

		fprintf(stderr, "Line %d: field GID is not an integer\n", lineNr);
		free(GID);
		validLine = 0;
	} else if (convertStringToInt(GID) < 0){

		fprintf(stderr, "Line %d: field GID must be positive\n", lineNr);
		free(GID);
		validLine = 0;
	} else {

		free(GID);
	}
	//Validating directory
	if (directory == NULL) {

		fprintf(stderr, "Line %d: field directory is empty\n", lineNr);
		validLine = 0;
	} else {

		free(directory);
	}
	//Validating shell
	if (shell == NULL) {

		fprintf(stderr, "Line %d: field shell is empty\n", lineNr);
		validLine = 0;
	} else {

		free(shell);
	}
	if (!validLine) {

		errno = 1;
	}
	return validLine;
}

/*
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - File pointer to input stream where line is read from.
* returns: A pointer to the allocated memory for the char array.
*/
unsigned char *readLine (FILE *fp) {

    int length = 0;
    int buffer = 100;
    unsigned char *line = malloc(sizeof(char) * buffer);
	checkAlloc((void *)line);
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
* description: Gets the next field from a string of userdata.
* param[in]: line - The string containing userdata.
* param[in]: i - Pointer to an external iterator. Is used to keep track where
* the line is being read from. If i = 0, getNextField() will get the first
* field and i will increase to where the first char of the next field begins.
* return: If it can be read; the field, else NULL.
*/
unsigned char* getNextField (unsigned char *line, int *i) {

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
			(*i)--;
		}

		if (wordFound == 0) {

			wordFound = 1;
			startOfWord = *i;

    	} else if (line[*i] == ':' || line[*i] == '\0') {

			if (wordFound == 1) {

				wordFound = -1;
				word = malloc(sizeof(char) * (*i - startOfWord + 1));
				checkAlloc((void *)word);
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

/*
* description: Prints the list containg user info.
* param[in]: list - the list containing the users.
*/
void printList (linkedlist *list) {

	listFirst(list);
	int size = listGetSize(list);

	for (int i = 0; i < size; i++) {

		user *tempUser = listInspect(list);
		printUser(tempUser);
		listNext(list);
	}
}

/*
* description: Prints a user struct.
* param[in]: u - Pointer to the user struct.
*/
void printUser (user *u) {

	if (u != NULL) {

		printf("%d:%s\n", u -> UID, u -> username);
	}
}

/*
* description: Free's all memory allocated in the user list. Also calls in
* listKill() which free's all memory in the list.
* param[in]: list - The list containing the userinfo.
*/
void killUserList (linkedlist *list) {

	int listSize = listGetSize(list);
	listFirst(list);

	for (int i = 0; i < listSize; i++) {

		free(((user *)listInspect(list)) -> username);
		listNext(list);
	}
	listKill(list);
}
