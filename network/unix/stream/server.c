#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

//定义用于通信的文件名

#define UNIX_DOMAIN "/tmp/UNIX.domain"

int main()
{

    socklen_t clt_addr_len;
    int listen_fd;
    int com_fd;
    int ret;
    int i;
    static char recv_buf[1024];
    int len;
    struct sockaddr_un clt_addr;
    struct sockaddr_un srv_addr;

    //创建用于通信的套接字，通信域为UNIX通信域

    listen_fd=socket(PF_UNIX,SOCK_STREAM,0);
    if(listen_fd<0){
        perror("cannot create listening socket");
        return 1;
    }

    //设置服务器地址参数
    srv_addr.sun_family=AF_UNIX;
    strncpy(srv_addr.sun_path,UNIX_DOMAIN,sizeof(srv_addr.sun_path)-1);

    //绑定套接字与服务器地址信息
    ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret==-1){
        perror("cannot bind server socket");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;

    }

    //对套接字进行监听，判断是否有连接请求
    ret=listen(listen_fd,1);
    if(ret==-1){
        perror("cannot listen the client connect request");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;

    }


    //当有连接请求时，调用accept函数建立服务器与客户机之间的连接
    len=sizeof(clt_addr);
    com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);
    if(com_fd<0){
        perror("cannot accept client connect request");
        close(listen_fd);
        unlink(UNIX_DOMAIN);

        return 1;
    }

    //读取并输出客户端发送过来的连接信息
    printf("\n=====info=====\n");
    for(i=0;i<4;i++){
        memset(recv_buf,0,1024);
        int num=read(com_fd,recv_buf,sizeof(recv_buf));
        printf("Message from client (%d)) :%s\n",num,recv_buf);
    }

    close(com_fd);
    close(listen_fd);

    unlink(UNIX_DOMAIN);

    return 0;
}
