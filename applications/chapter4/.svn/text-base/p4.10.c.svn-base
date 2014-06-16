#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int N_BITS=3;

typedef struct dir_lnk{
	char d_name[256];
	struct dir_lnk * next;
}dirlnk;

typedef struct item_info{
	unsigned long inode;
 	char permission[11];
	int owner;
	int group;
	off_t size;
	time_t mod_time;
	char name[256];
}info;


dirlnk* get_dir_detail(char* dirname)
{
	  DIR* dir;
	  struct dirent* drt;
	  dir=opendir(dirname);
					   
	  if(dir==NULL){
		perror("Cannot open the desired directory");
		return NULL;
		}

          dirlnk* dir_head=NULL;
          dirlnk* cur_item=NULL;

	  while((drt=readdir(dir))!=NULL){
		if((strcmp(drt->d_name,".")==0)||(strcmp(drt->d_name,"..")==0))
			continue;
	 
		dirlnk *next_item=(dirlnk*)malloc(sizeof(dirlnk));
	 
		if(cur_item==NULL)
			cur_item=next_item;
		else
			cur_item->next=next_item;

		cur_item=next_item;

		if(dir_head==NULL)
			dir_head=cur_item;
	
 		strcpy(cur_item->d_name,drt->d_name);
	  } 

		cur_item->next=NULL;
		closedir(dir);
		
		return dir_head;
}

void print_file_info(dirlnk* dir_head)
{
	static char* perm[]={"---","--x","-w-","-wx",
		"r--","r-x","rw-","rwx"};
		unsigned int mask=0700;
	struct stat file_stat;
       	dirlnk* cur_dir=dir_head;

	while(cur_dir!=NULL){
		mask=0700;
		info file_info;
		if(stat(cur_dir->d_name,&file_stat)==-1){
			perror("Cannot get the information of the file!");
			cur_dir=cur_dir->next;
			continue;
		}
	 	
		if(S_ISREG(file_stat.st_mode))
			file_info.permission[0]='-';
    
		if(S_ISDIR(file_stat.st_mode))
			file_info.permission[0]='d';
 
	     	int i=3;
	      	int j=0;
	    	while(i>0)
		{
			file_info.permission[1+j*3]=perm[(file_stat.st_mode & mask)>>(i-1)*N_BITS][0];
			file_info.permission[2+j*3]=perm[(file_stat.st_mode & mask)>>(i-1)*N_BITS][1];
		       	file_info.permission[3+j*3]=perm[(file_stat.st_mode & mask)>>(i-1)*N_BITS][2];
    
			j++;
			i--;
		       	mask>>=N_BITS;
	       	}
               
                file_info.permission[10]='\0';
		file_info.owner=file_stat.st_uid;
		file_info.group=file_stat.st_gid;
	       	file_info.mod_time=file_stat.st_atime;
		file_info.size=file_stat.st_size;
		strcpy(file_info.name,cur_dir->d_name);
		file_info.inode=file_stat.st_ino;
	
        	printf("%u  ",file_info.inode);
		printf("%s  ",file_info.permission);
		printf("%d  ",file_info.owner);
		printf("%d  ",file_info.group);
		printf("%u  ",file_info.size);
		printf("%s  ",ctime(&file_info.mod_time));
		printf("%s  \n",file_info.name);
                
	 	cur_dir=cur_dir->next;
	}
}

int main(int argc, char* argv[]){
	
	dirlnk* head=get_dir_detail(".");
	print_file_info(head);

			
}
