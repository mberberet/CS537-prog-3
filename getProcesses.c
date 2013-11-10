#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "hasUID.h"
#include "537malloc.h"

/**
 * This function parses through the /proc directory and returns a pointer to
 * all the PID directories in /proc that have the same UID as the program
 * calling getProcesses.
 *
 * @return - Returns a pointer to an array of integers that represent PID's of
 * processes that match this process's UID
 *
 */

int *getProcesses()
{
    const char* proc = "/proc";
    int *pid;
    int pidSize;
    DIR *handle;
    /*Get DIR struct for /proc*/
    struct dirent processes;
    /*Struct for each directory in /proc*/
    struct dirent *p;
    int x;
    int i;
    int isPID;
    handle = opendir(proc);
    if(!handle)
    {
    	return NULL;
    }
    x = 0;
    pidSize = 100;
    pid = (int *) malloc537(pidSize * sizeof(int));
    if(!pid)
    {
    	free537(pid);
        closedir(handle);
	return NULL;
    }
    p = readdir(handle);

    while(p)
    {
    	processes = *p;
    	i = 0;
	isPID = 1;
	/* Make sure the directory name is an integer*/
	while(processes.d_name[i] != '\0' && isPID)
	{
		if(!isdigit((int)(processes.d_name[i])))
		{
			isPID = 0;
		}
		i++;
	}
	/* If UID is correct, add it to the array*/
	if(isPID && hasUID(processes.d_name))
	{
		memcheck537(&pid[x], sizeof(int));
		pid[x] = atoi(processes.d_name);
		if(pid[x] == 0)
		{
			free537(pid);
            closedir(handle);
			return NULL;
		}
		x++;
		/*Double the size of the array if we hit the end */
		if(x >= pidSize)
		{
			pidSize*=2;
			pid = (int *) realloc537(pid, pidSize*sizeof(int));
			if(!pid)
			{
				free537(pid);
                closedir(handle);
				return NULL;
			}
		}
	}
	p = readdir(handle);
    }
    /*if there isn't an error, return the array*/
    if(errno == 0)
    {
	memcheck537(pid + x, sizeof(int));
	*(pid + x) = 0;
    closedir(handle);
	return pid;
    }
    else
    {
 	free537(pid);
    closedir(handle);
 	return NULL;
    }
}


