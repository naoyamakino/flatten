/*
 *cmpt-300
 *Assignment 4
 *Naoya Makino
 *SFU email ID: nmakino@sfu.ca
 *SFU #: 301117541
 *November 19th, 2009
 *File: flatten.c
 *Description:for a command line utility flatten path that operates on the directory hierarchy rooted at path.
 *
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "dirwalk.h"
using namespace std;

char *destination;
vector<string> file_list;
vector<string> source_list;
	
void flatten(char *);
void test(char *);
void make_no_conflict(char *, char *);

main (int argc, char **argv) {	
	struct stat buf;
	if(argc == 1)
		printf("please specify path\n");
	else
		while(--argc >0)
		{
			struct stat buf;
			destination = *++argv;
			stat(destination, &buf);
			if((buf.st_mode & S_IFMT) == S_IFDIR)
				flatten(destination);
			else{
				printf("the argument needs to be a valid path\n");
				return 0;
			}
		}
			
	return 0;
} /* main */

void flatten(char *name)
{	
	struct stat buf;
	string str;
	char *cstr;
	char tmp[] = "/";
	char *new_dir;
	char *file_name;
	char charModifyCount[50];

	
	if(stat(name, &buf) == -1){
		fprintf(stderr, "can't access %s\n", name);
		return;
	}
	if((buf.st_mode & S_IFMT) == S_IFDIR)
	{
		dirwalk(name, flatten);//if there is a sub directory, call dirwalk to get all file names within that directory
		//printf("rmdir:name = %s\n", name);
		rmdir(name);//after getting all files, detele that directory
	}
	else
	{
		file_name = strrchr(name, '/');
		if(file_name != NULL)
		{
		
			str = file_name +1;
			file_list.push_back(str);
			source_list.push_back(str);

			if(!source_list.empty())
			{
				int i;
				int modifyCount =0;
				string destFileName = source_list.back();
				for(i=0;i < source_list.size() - 1; i++)// source_list.size() - 1 as it compares up to the last one 
									//which is itself	
					if(source_list[i].compare(destFileName) == 0)						
						++modifyCount;			
				if(modifyCount > 0)
				{
					sprintf(charModifyCount, "%d",modifyCount);
					destFileName.append(".");
					destFileName.append(charModifyCount);
					file_list.back() = destFileName;
				}

				cstr = new char[file_list.back().size()+1];
				strcpy(cstr, file_list.back().c_str());
				
				new_dir = (char*)calloc(strlen(destination)+strlen(tmp) + strlen(cstr) +1, sizeof(char));
				strcat(new_dir, destination);
				strcat(new_dir, tmp);//tmp = "/"
				strcat(new_dir, cstr);
				rename(name, new_dir);
			}	
		}
	}
}


