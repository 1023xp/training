#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
 
void init()
{
   initscr();
   cbreak();
   nl();
   noecho();
   intrflush(stdscr,FALSE);
   keypad(stdscr,TRUE);
   refresh();
}
 
int getpasswd(char* passwd, int size)
{
   int c;
   int n = 0;
   
   printw("Please Input password:");

   do{
      c = getch();
      if (c != '\n'){
         echochar('*');//printw("*");
         passwd[n++] = c;
        }
   }while(c != '\n' && n < (size - 1));

   passwd[n] = '\0';
 
   return n;
}
 
int main()
{
   char passwd[20];
   int n;
   
   init();
   n=getpasswd(passwd, sizeof(passwd));
   
   printw("\nYour passwd is:%s\n", passwd);
   printw("Press any key continue ...\n");
   
   refresh();
   getchar();
   endwin();
   return 0;
}
