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

#ifndef MPASSWDSORT
#define MPASSWDSORT

#include <string.h>

#define ERROR -1
#define STDIN 1
#define FFILE 2

typedef struct user user;

/*
* description: Builds a list containing user data.
* param[in]: fileType - The type of stream data is read from. 1 For stdin, 2
* from file.
* param[in]: argv[] - The arguments for the executable program.
* return: The list containing the user data.
*/
linkedlist *buildList (int fileType, const char *argv[]);

/*
* description: Builds a user from a string of user data. Allocates memory for
* the user struct.
* param[in]: line - The string of user data. Should be a line from the input
* stream.
* return: Pointer to the user struct.
*/
user *buildUser (unsigned char *line);

/*
* description: Checks if a string is a number or not.
* param[in]: str - The string.
* return: If str is a numer; 1, else 0.
*/
int strIsNum (unsigned char *str);

/*
* description: Converts a string to an integer.
* param[in]: str - The string.
* return: The integer.
*/
int convertStringToInt (unsigned char *str);

/*
* description: Compares two users to see if which one has the lower UID.
* param[in]: user1 - The first user to be compared, will be compared to user2.
* param[in]: user2 - The second user to be compared, will be compared to user1.
* return: 0 If UID's of both users are same, -1 if user1's UID is smaller, 1 if
* user2's UID is smaller.
*/
int userCompare (void *user1, void *user2);

/*
* description: Sorts the list of userdata.
* param[in]: list - The list.
*/
void sortList (linkedlist *list);

/*
* description: Sorts an array of user data - used in sortList().
* param[in]: users[] - Array containing pointers to user structs.
* param[in]: size - Size of the array.
*/
void sortArray (user *users[], int size);

/*
* description: Validates that input arguments to the program are correct and
* that file can be read.
* param[in]: argc - Number of input arguments to program.
* param[in]: argv[] - The input arguments.
*/
int fileValidation (int argc, const char *argv[]);

/*
* description: Validates a string of userinfo to see if it follows the
* specified format of the input userdata.
* param[in]: line - The string containing the user data.
* param[in]: lineNr - The number of which line is being validated.
* return: 1 if string is valid, 0 if string is not, -1 if input string is NULL.
*/
int lineValidation (unsigned char *line, int lineNr);

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
						  unsigned char *directory, unsigned char *shell);

/*
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - File pointer to input stream where line is read from.
* returns: A pointer to the allocated memory for the char array.
*/
unsigned char *readLine (FILE *fp);

/*
* description: Gets the next field from a string of userdata.
* param[in]: line - The string containing userdata.
* param[in]: i - Pointer to an external iterator. Is used to keep track where
* the line is being read from. If i = 0, getNextField() will get the first
* field and i will increase to where the first char of the next field begins.
* return: If it can be read; the field, else NULL.
*/
unsigned char* getNextField (unsigned char *line, int *i);

/*
* description: Prints the list containg user info.
* param[in]: list - the list containing the users.
*/
void printList (linkedlist *list);

/*
* description: Prints a user struct.
* param[in]: u - Pointer to the user struct.
*/
void printUser (user *u);

/*
* description: Free's all memory allocated in the user list. Also calls in
* listKill() which free's all memory in the list.
* param[in]: list - The list containing the userinfo.
*/
void killUserList (linkedlist *list);

#endif //MPASSWDSORT
