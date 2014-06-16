#include <stdio.h> 
#include <unistd.h> 
 
int main () 
{
   FILE* fs = popen ("sort", "w");
   if(fs==NULL){
	   perror("call popen function fail");
	   return 1;
   } 
   
   fprintf (fs, "This is a simple program.\n"); 
   fprintf (fs, "Hello, Linux world.\n"); 
   fprintf (fs, "I like Linux.\n"); 
   fprintf (fs, "Only a test program.\n"); 

   pclose(fs);
   return 0; 
} 

