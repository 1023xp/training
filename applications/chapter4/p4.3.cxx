/* program p4.3.cxx: using chdir to change current working directory */
#include <unistd.h>
#include <iostream>

int main(void){
   long cur_path_len;
   char* cur_work_dir;
   
   /*���Ŀ¼��󳤶�*/
   if((cur_path_len=pathconf(".",_PC_PATH_MAX))==-1){
      perror("Couldn't get current working path length");
      return 1;
   }

  std::cout<<"Current Path Length Is "<<cur_path_len<<std::endl;

  /*���ݻ�õ�Ŀ¼��󳤶ȣ������ڴ�*/
  if((cur_work_dir=(char *)malloc(cur_path_len))==NULL){
      perror("Couldn't allocate memory for the pathname");
      return 1;
  }

  if(getcwd(cur_work_dir,cur_path_len)==NULL){
      perror("Couldn't get current working directory");
      return 1;
  }

  std::cout<<"Current Working Directory is"<<cur_work_dir<<std::endl;
   
  /*���ĵ�ǰ����Ŀ¼���ϼ�Ŀ¼*/
  if(chdir("..")==-1){
     perror("Couldn't change current working directory");
     return 1;
  }

  if(getcwd(cur_work_dir,cur_path_len)==NULL){
      perror("Couldn't get current working directory");
      return 1;
  }

  std::cout<<"After chdir function call,Current Working Directory is"<<cur_work_dir<<std::endl;

  return 0;
}
