#include <stdlib.h>
#include <stdio.h>

struct net_packet
{
    int protocol;
    int len;
    int payload[0];
};


int main()
{
    struct net_packet * p_dev =
        (struct net_packet *) malloc (sizeof(struct net_packet) + sizeof(int)*25);

    p_dev->payload[0] = 100;
    p_dev->payload[24] = 0;

    printf("p_dev->payload[0] = %d\n", p_dev->payload[0]);
    printf("p_dev->payload[24] = %d\n", p_dev->payload[24]);
    printf("sizeof(struct net_packet) = %d\n",sizeof(struct net_packet));

    return 0;
}
