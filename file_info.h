
#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#define DEBUG 1
/*FileInfo: stores all the neccesary information needed for displaying a histogram*/
class FileInfo{
	public:
		int size;
		int num_blocks;
		int internal_frag;
		int group_num;
		char *name;
	FileInfo();
		
};
FileInfo::FileInfo()
{
	size =0;
	num_blocks =0;
	internal_frag=0;
	group_num=0;
}

#endif /* _FILE_INFO_H_ */
