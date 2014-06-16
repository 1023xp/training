//p21.2.c ping程序实现
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/ip_icmp.h>

#define WAIT_TIME 3
#define BUFFER_SIZE 1024

u_short cal_cksum(const u_short *addr, register int len, u_short csum)
{
	register int nleft = len;
	const u_short *w = addr;
	register u_short answer;
	register int sum = csum;
	
	/**  使用32位累加器实现校验码的计算
	*  联系对16位数进行累加，最后将高16位与低16位进行累加
	*/
		while (nleft > 1)  {
			sum += *w++;
			nleft -= 2;
		}
		
		/* 如果最后剩余8位，补齐到16位（补零，同时注意网络字节与
	主机字节间的转换） */
		if (nleft == 1)
			sum += htons(*(u_char *)w << 8);
		
		/** 将高低16位相加，以消除累加时进位问题*/
		sum = (sum >> 16) + (sum & 0xffff);	/* 高低16位相加 */
		sum += (sum >> 16);			/* 消除高低位相加的进位问题 */
		answer = ~sum;				/* 取反 */
		return (answer);

}


//ICMP报头解析代码
int unpack_packet_header(char *buf,int len,struct sockaddr_in *from)
{       int i,iphdrlen;
        struct ip *ip;
        struct icmp *icmp;
        struct timeval *tvsend;
	struct timeval tvrecv;
        double rtt;

        ip=(struct ip *)buf;
        
        //ip报头长度= ip_hl*4
        iphdrlen=ip->ip_hl*4;    

        //得到ICMP报文信息
        icmp=(struct icmp *)(buf+iphdrlen); 

        //icmp报文总长度
        len-=iphdrlen; 

        //如果长度小于要求，退出程序           
        if( len<8)                
        {       printf("ICMP packets\'s length is less than 8\n");
                return -1;
        }

        if( icmp->icmp_type==ICMP_ECHOREPLY)
        {       tvsend=(struct timeval *)icmp->icmp_data;

		gettimeofday(&tvrecv,NULL);
                //得到接收时间，并计算两者时间差
                if((tvrecv.tv_usec-=tvsend->tv_usec)<0){
			tvrecv.tv_sec--;
			tvrecv.tv_usec+=1000000;  
		}
		
		tvrecv.tv_sec-=tvsend->tv_sec;

                rtt=tvrecv.tv_sec*1000+tvrecv.tv_usec/1000; 

                //显示时间信息
                printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n",
                        len,
                        inet_ntoa(from->sin_addr),
                        icmp->icmp_seq,
                        ip->ip_ttl,
                        rtt);
        } else    
                return -1;
}

		
int
main(int argc, char * argv[])
{
    int sock;
    struct sockaddr_in sin;
    unsigned short local_port;
    unsigned char protocol;
    char * buffer, * buffer2, * dnsdata;
    struct iphdr * ip_header;
    struct icmphdr * icmp_header;
    char * remote_ip_str;
    unsigned short buffer_size, buffer_size2;
    int tmp, len;
    short randomseq;
 
    //判断参数是否符合程序运行要求
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s port destination\n", argv[0]);
        return 1;
    }
 
    protocol = IPPROTO_ICMP;
 
    local_port = atoi(argv[1]);
    remote_ip_str = argv[2];

    //创建通信所需套接字，注意套接字类型为原始套接字
    if ((sock = socket(PF_INET, SOCK_RAW, protocol)) < 0) { 
        perror("socket");
        exit(1);
    }
    memset(& sin, 0, sizeof(sin));
    sin.sin_port = htons(local_port);
 
    //完成套接字与端口、地址信息的绑定
    if ((bind(sock, (struct sockaddr *)& sin, sizeof(sin))) < 0) {
        perror("bind");
        exit(1);
    }
 
    tmp = 1;
    setsockopt(sock, 0, IP_HDRINCL, & tmp, sizeof(tmp));
 
    //申请内存空间，并填写包头信息
    buffer_size = sizeof(struct iphdr) + sizeof(struct icmphdr)
		+ sizeof(struct timeval);
    buffer_size2 = BUFFER_SIZE;
    buffer = (char *) malloc(buffer_size);
    buffer2 = (char *) malloc(buffer_size2);
 
    memset(buffer, 0, sizeof(buffer));
 
    memset(& sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(remote_ip_str);
 
    ip_header = (struct iphdr *) buffer;
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = htons(buffer_size);
    ip_header->id = rand();
    ip_header->ttl = 64;
    ip_header->frag_off = 0x40;
    ip_header->protocol = protocol;
    ip_header->check = 0; 
    ip_header->daddr = inet_addr(remote_ip_str);
    ip_header->saddr = 0;
  
    icmp_header = (struct icmphdr *) (ip_header + 1);
    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0;
    icmp_header->un.echo.id = htons(local_port);
    icmp_header->un.echo.sequence = 0;

	struct timeval *tp = (struct timeval *)&buffer[28];
	gettimeofday(tp, NULL);
	
	icmp_header->checksum = cal_cksum((const u_short *) icmp_header, sizeof(struct icmphdr) + sizeof(struct timeval), 0);
				  
    //发送该IP数据包
    if (sendto(sock, buffer, buffer_size, 0, (struct sockaddr *) &sin,
sizeof(struct sockaddr_in)) < 0) 
    {
        perror("sendto");
	return 1;
    }
 
    printf("send ICMP inquery\n");
 
len = sizeof(sin);

    alarm(WAIT_TIME);
//接收对方回应的数据，并给出提示
    if (recvfrom(sock, buffer2, buffer_size2, 0, (struct sockaddr *) &sin, &
len) < 0) {
        perror("recvfrom");
        return 1;
    }
    printf("received Destionation response\n");
   
//显示报文信息
    unpack_packet_header(buffer2,buffer_size2,(struct sockaddr_in *)&sin);
 
    close(sock);
    return 0;
}

