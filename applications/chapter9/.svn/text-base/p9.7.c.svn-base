#include <stdio.h>
#include <signal.h>

void signal_set(struct sigaction *act)
{
        switch(act->sa_flags){
		case (int)SIG_DFL:
			printf("using default hander\n");
			break;
		case (int)SIG_IGN:
			printf("ignore the signal\n"); 
			break;
		default: 
			printf("%0x\n",act->sa_handler);
     	}
}

int main(int argc,char** argv)
{
        int i;
        struct sigaction act,oldact;
        
	act.sa_handler = signal_set;
        act.sa_flags = SA_NODEFER| SA_RESETHAND;
        
	sigaction (SIGUSR1,&act,&oldact) ;

	for (i=1; i<12; i++){
                printf("signal %d handler is : ",i);
                sigaction (i,NULL,&oldact) ;
                signal_set (&oldact);
        }

        return 0;
}
