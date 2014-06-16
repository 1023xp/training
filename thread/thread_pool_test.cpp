#include <string>  
#include <iostream>  
using namespace std;  
#include "thread_pool.h"  

class CWorkTask: public CTask  
{  
public:  
 CWorkTask()  
 {}  
 int Run()  
 {  
  cout << (char*)this->m_ptrData << endl;  
  sleep(10);  
  return 0;  
 }  
};  
int main()  
{  
 CWorkTask taskObj;  
 char szTmp[] = "this is the first thread running,haha success";  
 taskObj.SetData((void*)szTmp);  
 CThreadPool threadPool(10);  
 for(int i = 0;i < 11;i++)  
 {  
  threadPool.AddTask(&taskObj);  
 }  
 while(1)  
 {  
  sleep(120);  
 }  
 return 0;  
}  
