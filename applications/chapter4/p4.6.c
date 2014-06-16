#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	struct stat file_stat;
	
	if(argc!=2){
		printf("Usage:%s filename\n",argv[0]);
		return 1;
	}

	if(stat(argv[1],&file_stat)==-1){
		perror("Cannot get the information of the file!\n");
		return 1;
	}

	if(S_ISREG(file_stat.st_mode))
		printf("%s is Regular File,Judged by S_ISREG\n",argv[1]);
		

	if(file_stat.st_mode & S_IFREG)
		printf("%s is Regular File,Judeged by bits calculate S_IFREG\n",argv[1]);

	if(S_ISDIR(file_stat.st_mode))
		printf("%s is Directory,Judged by S_ISDIR\n",argv[1]);

	if(file_stat.st_mode & S_IFDIR)
		printf("%s is Directory,Judged by bit calculate S_ISDIR\n",argv[1]);

	
	printf("Owner ID: %d, Group ID: %d\n",file_stat.st_uid,file_stat.st_gid);
        printf("Permission: %o\n",file_stat.st_mode & 0x1ff);
	printf("Last Access Time: %15s\n",ctime(&file_stat.st_atime));
	printf("Last Modification Time: %15s\n",ctime(&file_stat.st_mtime));
	printf("Last Status Change Time: %15s\n",ctime(&file_stat.st_ctime));
	
	return 0;
}
