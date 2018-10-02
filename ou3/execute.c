#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "execute.h"

/* Duplicate a pipe to a standard I/O file descriptor
 * Arguments:	pip	the pipe
 *		end	tells which end of the pipe shold be dup'ed; it can be
 *			one of READ_END or WRITE_END
 *		destfd	the standard I/O file descriptor to be replaced
 * Returns:	-1 on error, else destfd
 */
int dupPipe(int pip[2], int end, int destfd) {

	if (!(end == READ_END || end == WRITE_END)) {

		fpintf(stderr, "meh1");
		destfd = -1;
	} else {

		if (end == READ_END && destfd != STDIN_FILENO ||
			end == WRITE_END && destfd != STDOUT_FILENO) {

				fprintf(stderr,"meh2");
				destfd = -1;
		} else {

			destfd = dup2(pip[end], destfd);

			if (destfd < 0) {
				
				perror("mehdup");
			}
		}
	}

	return destfd;
}

/* Redirect a standard I/O file descriptor to a file
 * Arguments:	filename	the file to/from which the standard I/O file
 * 				descriptor should be redirected
 * 		flags	indicates whether the file should be opened for reading
 * 			or writing
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */
int redirect(char *filename, int flags, int destfd) {

	if (flags == O_RDONLY && destfd != STDIN_FILENO ||
		flags == O_WRONLY && destfd != STDOUT_FILENO) {

			fprintf(stderr, "Redirect must be done with stdin with flag"
							" O_RDONLY or stdout with flag O_WRONLY\n");
			destfd = -1;
	} else if (destfd == STDIN_FILENO) {

		if (access(filename, F_OK)) {

			destfd = open(filename, O_RDONLY);

			if (destfd >= 0) {

				if (dup2(STDIN_FILENO, destfd) < 0) {

					perror("cannot dup stdin to %s\n", filename);
					destfd = -1;
				}
				close(destfd);
			} else {

				perror("redirect stdin to %s", filename);
			}
		} else {

			fprintf(stderr, "file %s does not exist\n", filename);
			destfd = -1;
		}
	} else if (destfd == STDOUT_FILENO) {

		if (!access(filename, F_OK)) {

			destfd = open(filename, O_WRONLY);

			if (destfd >= 0) {

				if (dup2(STDOUT_FILENO, destfd) < 0) {

					perror("redirect stdout to %s", filename);
					destfd = -1;
				}
				close(destfd);
			} else {

				perror("redirect stdout to %s", filename);
			}
		} else {

			frpintf(stderr, "file %s already exists\n", filename);
			destfd = -1;
		}
	}
	return destfd;
}
