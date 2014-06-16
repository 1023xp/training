/*p4.4.cxx :using mkdir to create a directory*/
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

int main(void){
   char* pathname="/home/program/a";
   
   /*mode 设置为0700,  开始的0表示八进制*/
   if(mkdir(pathname,0700)==-1){
      perror("Couldn't create the directory");
      return 1;
   }

   return 0;
}
