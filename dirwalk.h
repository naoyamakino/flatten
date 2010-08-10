/*
 *Reference: Kernighan; Dennis M. Ritchie (March 1988). The C Programming Language (2nd ed.). 
 *				Englewood Cliffs, NJ: Prentice Hall. ISBN 0-13-110362-8. http://cm.bell-labs.com/cm/cs/cbook/.
 *note: I have gained a basic understanding of recursive file directory search by reading this textbook p160 - 164
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX_PATH 1024

void dirwalk(char *dir, void(*fcn)(char *))
{
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;
	
	if ((dfd = opendir(dir)) == NULL){
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while((dp = readdir(dfd)) != NULL){
		if (strcmp(dp->d_name, ".") == 0 ||
			strcmp(dp->d_name, "..") == 0)
			continue; //skip self and parent
		if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name)) 
			fprintf(stderr, "dirwalk: name %s %s too long\n",
					dir, dp->d_name);
		else { 
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}
