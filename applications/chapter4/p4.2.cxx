/*program p4.2.cxx: using pathconf and getcwd together */

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
  
  /*�õ���ǰ����Ŀ¼*/
  if(getcwd(cur_work_dir,cur_path_len)==NULL){
      perror("Couldn't get current working directory");
      return 1;
  }

  std::cout<<"Current Working Directory IS"<<cur_work_dir<<std::endl;
  return 0;
}
