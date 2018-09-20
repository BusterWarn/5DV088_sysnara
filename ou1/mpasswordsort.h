#ifndef mpaswordsort
#define mpasswordsort

#include <string.h>

#define ERROR -1
#define STDIN 1
#define FFILE 2


typedef struct user {

	unsigned char *username;
	unsigned char *password;
	int UID;
	int GID;
	unsigned char *GECOS;
	unsigned char *directory;
	unsigned char *shell;
} user;

list *buildList (int fileType, const char *argv[]);

user *buildUser (unsigned char *line);

int convertStringToInt (unsigned char *str);

int lineValidation (unsigned char *line);

int fileValidation (int argc, const char *argv[]);

int strIsNum (unsigned char *str);
/*
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
unsigned char *readLine (FILE *fp);

unsigned char* getNextWord (unsigned char *line, int *i);

void printList (list *list);

void printUser (user *user);

int userCompare (void *user1, void *user2);

#endif //mpasswordsort
