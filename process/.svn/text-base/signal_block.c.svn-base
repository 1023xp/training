#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
static void sig_quit(int);
int
main(void)
{
    sigset_t newmask, oldmask, pendingmask;
    //设置对信号SIGQUIT的处理函数
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
    {
        fprintf(stderr, "can't catch SIGQUIT\n");
        exit(1);
    }
    //设置一个空的信号集
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT); // 在这个信号集中增加SIGQUIT信号

    //在当前进程中增加newmask信号集作为屏蔽信号集，oldmask返回当前进程的信号集
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr, "SIG_BLOCK error\n");
        exit(1);
    }
    sleep(20);
    //返回当前进程信号集
    if (sigpending(&pendingmask) < 0)
    {
        fprintf(stderr, "sigpending error\n");
        exit(1);
    }
    //检查SIGQUIT信号是否在当前信号集中
    if (sigismember(&pendingmask, SIGQUIT))
        printf("\nSIGQUIT pending\n");
    //恢复进程的信号集
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "SIG_SETMASK error\n");
        exit(1);
    }
    printf("SIGQUIT unblocked\n");
    sleep(5);
    exit(0);
}
static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    //恢复进程对SIGQUIT的默认处理
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    {
        fprintf(stderr, "can't reset SIGQUIT\n");
        exit(0);
    }
}
