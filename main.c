/*
 *cmpt-300
 *Assignment 4
 *Naoya Makino
 *SFU email ID: nmakino@sfu.ca
 *SFU #: 301117541
 *November 19th, 2009
 *File: main.c
 *Description: filecount shall recursively gather frequency statistics 
 *		of all regular files (i.e., no directories) in
 *             the directory tree rooted at startdir.
 *Reference: Kernighan; Dennis M. Ritchie (March 1988). The C Programming Language (2nd ed.). 
 *				Englewood Cliffs, NJ: Prentice Hall. ISBN 0-13-110362-8.
 * 				 http://cm.bell-labs.com/cm/cs/cbook/.
 * note: I have gotten a basic understanding of 
 *       recursive file directory search by reading this textbook p160 - 164
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <string>
#include <math.h>
#include "dirwalk.h"
#include "file_info.h"
using namespace std;

#define MAX_PATH 1024
#define BLOCK_SIZE 4096

void search_file(char *);
void draw_graph();
vector<FileInfo> file_list;/*stores all files that found in the directory*/

main (int argc, char **argv) {
	
	if(argc == 1)
		search_file("."); //default: current directory
	else
		while(--argc >0)
			search_file(*++argv);
	
	draw_graph();
	return 0;
} /* main */

void search_file(char *name)
{
	struct stat buf;

	if(stat(name, &buf) == -1){
		perror("stat");
		return;
	}
	if((buf.st_mode & S_IFMT) == S_IFDIR)//if there is a sub directory
		dirwalk(name, search_file);//search that sub directory
	if((buf.st_mode & 0xF000) == S_IFREG)
	{
		FileInfo file;/*stores neccesary info*/
		file.size = buf.st_size;
		file.name = name;
		file_list.push_back(file);
		//printf("reading file... %s\n", name);
	}
}

void draw_graph()
{
	int i,j;
	int total_size = file_list.size();
	int bin[65];
	int percentage[65];
	int oneBlock=0;
	int twoBlocks = 0;
	int fourBlocks=0;
	int tenBlocks=0;
	for(i=0;i<65;i++)
	{
		bin[i] = 0;
		percentage[i] =0;
	}
	for(j = 0; j<file_list.size(); j++)
	{
		file_list[j].num_blocks = ceil(file_list[j].size / (float)BLOCK_SIZE);
		if(file_list[j].num_blocks <= 1)
			oneBlock++;
		if(file_list[j].num_blocks <= 2)
			twoBlocks++;
		if(file_list[j].num_blocks <= 4)
			fourBlocks++;
		if(file_list[j].num_blocks <= 10)
			tenBlocks++;
		/*calculation*/
		/*internal_frag = (num_block * block_size) - file_size */		
		file_list[j].internal_frag = (file_list[j].num_blocks * BLOCK_SIZE) - file_list[j].size; 
		file_list[j].group_num = ceil(file_list[j].internal_frag / (float)64);
		bin[file_list[j].group_num] = bin[file_list[j].group_num] + 1;
		percentage[file_list[j].group_num] = ceil (((bin[file_list[j].group_num] / (float)total_size) * 100));
 	}

	oneBlock = ceil((oneBlock/(float)total_size) * 100);
	twoBlocks = ceil((twoBlocks/(float)total_size) * 100);
	fourBlocks = ceil((fourBlocks/(float)total_size) * 100);
	tenBlocks = ceil((tenBlocks/(float)total_size) * 100);
	
	printf("\n");
	printf("		       30x64 text histogram \n");
	//printf("the number of files: %d\n", total_size);
	
	for(i=30;i>0;i--) //virtical %
	{
		
		if((i % 5) == 0)
			printf("   %d%s",i,"%");
		printf("	|");
		for(j = 0; j < 65; j++) //horizontal 
		{
			if(bin[j] > 0 && percentage[j] >= i)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("	+---'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`---+\n");
	printf("        0               1K              2K              3K              4K\n");
	printf("                           Internal Fragmentation\n");
	printf("\n");
	printf("Percentage of files: 1 block or less = %d%s\n", oneBlock,"%");
	printf("                     2 block or less = %d%s\n", twoBlocks,"%");
	printf("                     4 block or less = %d%s\n", fourBlocks,"%");
	printf("                    10 block or less = %d%s\n", tenBlocks,"%");
}



