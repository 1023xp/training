#include <stdio.h>
#include <pcap.h>
#include <netinet/if_ether.h>

void deal_with_packet(u_char *user, const struct pcap_pkthdr *hdr, const u_char *packet)
{
      	static int count=0;
	struct ether_header *eth_header;
        u_char *ptr; 
	
	printf("Packet length: %d\n",hdr->len);
        printf("length of portion present: %d\n",hdr->caplen);

        eth_header=(struct ether_header*)packet;
        if(ntohs(eth_header->ether_type)!=ETHERTYPE_IP){
                printf("not ethernet packet\n ");
                return ;
        }

        ptr=eth_header->ether_dhost;
        int i;
        i=0;
        printf("destination address(MAC):");
        while(i<ETHER_ADDR_LEN){
                printf(" %x ",*ptr++);
                i++;
        }

        printf("\nsource address(MAC):");
        ptr=eth_header->ether_shost;
        i=0;
        while(i<ETHER_ADDR_LEN){
                printf(" %x ",*ptr++);
                i++;
        }

        printf("\n");
	
	printf("finish deal with %d packet\n",count);
	count++;
}

int main(void)
{
	pcap_t *sniffer_des;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *net_dev;
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	struct bpf_program fp;
	int ret;
	
	char filter_exp[]="port 22";

	net_dev=pcap_lookupdev(errbuf);
	if(net_dev==NULL){
		printf("cannot get the network device info: %s\n",errbuf);
		return 1;
	}	

	if(pcap_lookupnet(net_dev,&netp,&maskp,errbuf)==-1){
		printf("cannot get the network device ip info:%s\n",errbuf);
		return 1;
	}

	sniffer_des=pcap_open_live(net_dev, 65535, 1, 1000, errbuf);
	if(sniffer_des==NULL){
		printf("cannot open the network device: %s\n",errbuf);
		return 1;
	}

	if(pcap_compile(sniffer_des, &fp, filter_exp, 0, maskp)==-1){
		printf("cannot compile the filter rule\n");
		return 1;
	}

	if(pcap_setfilter(sniffer_des,&fp)==-1){
		printf("cannot set the filter to the network device\n");
		return 1;
	}
	
	ret=pcap_loop(sniffer_des,3,deal_with_packet,NULL);
	if(ret==-1 || ret==-2){
		printf("cannot get the packet\n");
		return 1;
	}

	printf("finish processing packets....\n");
	return 0;
}
	

