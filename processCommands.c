#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "537malloc.h"

int readFile(char *, char **);
char *nthWord(char *, int);

/**
 * This function processes the commands for a process ID, resulting in a
 * string with the information requested by the commands from the /proc/[pid]
 * filesystem.
 *
 * Input:d
 *   pid: the process id.
 *   commands: the commands to be processed.
 *   processed: where the processed string is placed.
 * Output:
 *   1: Performed as expected.
 *   0: An error occurred in reading the files for the given PID.
 */
int processCommands(int pid, char *commands, char **processed) {

    /* the positions in the stat and statm files of the requested info */
    const int STATE = 3;
    const int UTIME = 14;
    const int STIME = 15;
    const int VMEMORY = 1;

    /* Paths of the stat, statm, and cmdline files. */
    char path[128];
    char statPath[128];
    char statmPath[128];
    char cmdPath[128];

    /* Holds the information in each file (as a string) */
    char *stat = NULL;
    char *statm = NULL;
    char *cmdline = NULL;

    int i;

    DIR *dirp = NULL;
    /* Used for concatenation to "processed" array (string) */
    char *word = NULL;
    int catLength = 0;

    /* Holds the word to be concatenated to the "processed" string */
    char *buffer = NULL;
    int bufferSize = 1024;

    /* Keeps track of memory for the processed array */
    int procCurLength = 0;
    int procMaxLength = 2048;

    /* Get path to the PID proc files. */
    sprintf(path, "/proc/%d", pid);

    /* Check one of the file paths to see if PID is valid */
    if (!(dirp = opendir(path))) {
        printf("Invalid PID\n");
        return 0;
    }

    /* Allocate and initialize processed so it can be concatenated to */
    *processed = (char *) malloc537(procMaxLength * sizeof(char));
	if (!processed) {
		exit(-1);
	}
	memcheck537(&((*processed)[procCurLength]), sizeof(char));
    (*processed)[procCurLength] = '\0';
    procCurLength++;
    /* Prepares the path to each file based upon the given PID */
    sprintf(statPath, "%s/stat", path);
    sprintf(statmPath, "%s/statm", path);
    sprintf(cmdPath, "%s/cmdline", path);

    /* Read the stat, statm, and cmdline file. If error occurs, returns 0 */
    if (readFile(statPath, &stat) == 0) {
        return 0;
    }
    if (readFile(statmPath, &statm) == 0) {
        return 0;
    }
    if (readFile(cmdPath, &cmdline) == 0) {
        return 0;
    }

    buffer = (char *) malloc537(bufferSize * sizeof(char));
    /* For each command, retrieves desired information from string obtained from
        from the stat/statm file if necessary. */
    for (i = 0; i < strlen(commands); i++) {
        switch (commands[i]) {
            case 's':
                word = nthWord(stat, STATE);
                /* Determine length of word + prefix */
                catLength = strlen(word);
                /* Check if the buffer needs to be larger */
                if (catLength > bufferSize) {
                    bufferSize *= 2;
                    buffer = (char *) realloc537(buffer, bufferSize * sizeof(char));
                }
                /* Concatenate word and prefix */
				memcheck537(buffer, strlen(word)+1);
                sprintf(buffer, "%s", word);
                free(word);
                break;
            case 'U':
                word = nthWord(stat, UTIME);
                /* + 6 for length of "utime=" prefix */
                catLength = strlen(word) + 6;
                if (catLength > bufferSize) {
                    bufferSize *= 2;
                    buffer = (char *) realloc537(buffer, bufferSize * sizeof(char));
                }
				memcheck537(buffer, strlen("utime=")+strlen(word)+1);
                sprintf(buffer, "utime=%s", word);
                free(word);
                break;
            case 'S':
                word = nthWord(stat, STIME);
                catLength = strlen(word) + 6;
                if (catLength > bufferSize) {
                    bufferSize *= 2;
                    buffer = (char *) realloc537(buffer, bufferSize * sizeof(char));
                }
				memcheck537(buffer, strlen("stime=")+strlen(word)+1);
                sprintf(buffer, "stime=%s", word);
                free(word);
                break;
            case 'v':
                word = nthWord(statm, VMEMORY);
                catLength = strlen(word) + 8;
                if (catLength > bufferSize) {
                    bufferSize *= 2;
                    buffer = (char *) realloc537(buffer, bufferSize * sizeof(char));
                }
				memcheck537(buffer, strlen("vmemory=")+strlen(word)+1);
                sprintf(buffer, "vmemory=%s", word);
                free(word);
                break;
            case 'c':
                catLength = strlen(cmdline);
                if (catLength > bufferSize) {
                    bufferSize *= 2;
                    buffer = (char *) realloc537(buffer, bufferSize * sizeof(char));
                }
				memcheck537(buffer, strlen(cmdline)+1);
                sprintf(buffer, "%s", cmdline);
                break;
        }
        /* Check memory for room to add string */
        procCurLength += catLength + 1;
        if (procCurLength >= procMaxLength) {
            procMaxLength *= 2;
            *processed = (char *) realloc537(*processed,
            procMaxLength * sizeof(char));
        }
        /* Concatenate string */
		memcheck537(*processed, strlen(*processed)+strlen(" ")+strlen(buffer)+1);
        sprintf(*processed, "%s %s", *processed, buffer);
     }
    free537(buffer);
    free537(stat);
    free537(statm);
    free537(cmdline);
    closedir(dirp);

    return 1;
}


/**
 * Reads from a given file.
 *
 * Input:
 *   file: The file to read.
 *   line: A pointer to the character array to store the string in.
 * Output:
 *   0: Could not open the file.
 *   1: Successfully read file.
 */
int readFile(char *file, char **line) {
    FILE *fp;
    char c;
    int lineLength = 1000;
    int index = 0;

    *line = (char *) malloc537(lineLength * sizeof(char));

    /* If the file won't open, return */
    if ((fp = fopen(file, "r")) == NULL) {
        fprintf(fp, "Could not open %s\n", file);
        return 0;
    } else {
        c = fgetc(fp);
        while (c != EOF && c != '\n') {
            /* Replaces \0 with spaces */
            if (c == '\0') {
                c = ' ';
            }
            /* Resize the array if necessary */
            if (index >= lineLength - 1) {
                lineLength *= 2;
                *line = (char *) realloc537(*line, lineLength * sizeof(char));
            }

			memcheck537(&((*line)[index]), sizeof(char));
            (*line)[index] = c;
            index++;
            c = fgetc(fp);
        }

        /* Ensures that the string is null terminated so that any output of the
           string does not overflow into the following buffer.*/
        if (index >= lineLength - 1) {
            *line = (char *) realloc537(*line, (lineLength + 1) * sizeof(char));
        }
		memcheck537(&((*line)[index]), sizeof(char));
        (*line)[index] = '\0';
        fclose(fp);
    }
    return 1;
}


/**
 * Retrieves the nth word from a string.
 * Input:
 *   string: The string to be read from.
 *   n: The word to be retrieved.
 * Output:
 *   The requested word
 */
char *nthWord(char *string, int n) {
    char *line = strdup(string); /* Copy string so it can be reused */
    char *tmp = line;
    char *token = strtok(line, " ");
    char *tok;
    /* Reads the next word until the desired word is reached */
    while (token && (--n > 0)) {
        token = strtok(NULL, " ");
    }
    tok = strdup(token);
    free(tmp);
    return tok;
}
