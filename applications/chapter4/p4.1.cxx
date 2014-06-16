/*Program 4.1.cxx : get current working directory*/

#include <limits.h>
#include <iostream>
#include <unistd.h>

int main(void){
    char cur_work_dir[PATH_MAX];

    std::cout<<"Current Max Path Length Is "<<PATH_MAX<<std::endl;

    if(getcwd(cur_work_dir,PATH_MAX)==NULL){
      perror("Couldn't get current working directory!");
      return 1;
    }

    std::cout<<"Current Woring Directory is :"<<cur_work_dir<<std::endl;
    return 0;
}
