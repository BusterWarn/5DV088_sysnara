#ifndef mpaswordsort
#define mpasswordsort

#include <string.h>

#define ERROR -1
#define STDIN 1
#define FFILE 2


typedef struct user user;

linkedlist *buildList (int fileType, const char *argv[]);

user *buildUser (unsigned char *line);

int convertStringToInt (unsigned char *str);

void sortList (linkedlist *list);

void sortArray (user *users[], int size);

int lineValidation (unsigned char *line);

int lineFieldsValidation (unsigned char *username, unsigned char *UID,
					   	  unsigned char *GID, unsigned char *directory,
					   	  unsigned char *shell);

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

void printList (linkedlist *userList);

void killUserList (linkedlist *list);

void killUserVariables (user *u);

void printUser (user *tempUser);

int userCompare (void *user1, void *user2);

#endif //mpasswordsort
