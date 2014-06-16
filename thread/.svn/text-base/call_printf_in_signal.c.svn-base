/*
 * 像printf()和malloc()之类的标准库函数，它们会使用全局的数据（比如malloc()需要使用进程的堆数据结构）是不可重入的，即使加锁也无法解决，加锁可以使得库函数Tread-safe，但是无法解决可重入。
 */
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
print_by_write()
{
	write(/*STDOUT_FILENO*/1, "now got a signal\n", strlen("now got a signal\n"));
}
void handler(int s){
#if 0
	if(s==SIGBUS)printf(" now got a bus error\n");
	if(s==SIGSEGV)printf("now got segmentation fault\n");
	if(s==SIGILL)printf(" now got illegal instruction\n");
#endif
	print_by_write();
	exit(1);
}
main(){
	int *p=NULL;
	signal(SIGBUS,handler);
	signal(SIGSEGV,handler);
	signal(SIGILL,handler);
	*p=0;
	return 0;
}
