#include <stdio.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
	char *net_dev;
	char *ip_addr;
	char *net_mask;
	char errbuf[PCAP_ERRBUF_SIZE];

	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	struct in_addr addr;

	net_dev=pcap_lookupdev(errbuf);
	if(net_dev==NULL){
		printf("cannot get the network device info: %s\n",errbuf);
		return 1;
	}	
        printf("the network device is: %s\n",net_dev);

	if(pcap_lookupnet(net_dev,&netp,&maskp,errbuf)==-1){
		printf("cannot get the network device ip info:%s\n",errbuf);
		return 1;
	}

	addr.s_addr=netp;
	ip_addr=inet_ntoa(addr);

	if(ip_addr==NULL){
		printf("convert network address fail\n");
		return 1;
	}
        printf("ip address is : %s\n",ip_addr);

	addr.s_addr=maskp;
	net_mask=inet_ntoa(addr);

	if(net_mask==NULL){
		printf("convert network mask fail\n");
		return 1;
	}

	printf("network mask is : %s\n",net_mask);

	return 0;
}
