/*
 * This application sniffs on whatever device is returned by pcap_lookupdev() by
 * putting it into promiscuous mode. It finds the first packet to come across port
 * 23 (telnet) and tells the user the size of the packet (in bytes).
 *
 *
 * The format of a pcap application
 *
 * 1. We begin by determining which interface we want to sniff on. In Linux this may be something like eth0,
 * in BSD it may be xl1, etc. We can either define this device in a string, or we can ask pcap to provide us
 * with the name of an interface that will do the job.
 * 2. Initialize pcap. This is where we actually tell pcap what device we are sniffing on. We can, if we want
 * to, sniff on multiple devices. How do we differentiate between them? Using file handles. Just like opening
 * a file for reading or writing, we must name our sniffing "session" so we can tell it apart from other such
 * sessions.
 * 3. In the event that we only want to sniff specific traffic (e.g.: only TCP/IP packets, only packets going
 * to port 23, etc) we must create a rule set, "compile" it, and apply it. This is a three phase process, all
 * of which is closely related. The rule set is kept in a string, and is converted into a format that pcap can
 * read (hence compiling it.) The compilation is actually just done by calling a function within our program;
 * it does not involve the use of an external application. Then we tell pcap to apply it to whichever session
 * we wish for it to filter.
 * 4. Finally, we tell pcap to enter it's primary execution loop. In this state, pcap waits until it has received
 * however many packets we want it to. Every time it gets a new packet in, it calls another function that we have
 * already defined. The function that it calls can do anything we want; it can dissect the packet and print it to
 * the user, it can save it in a file, or it can do nothing at all.
 * 5. After our sniffing needs are satisfied, we close our session and are complete. 
 *
 * This is actually a very simple process. Five steps total, one of which is optional (step 3, in case you were
 * wondering.) Let's take a look at each of the steps and how to implement them. 
 */
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	pcap_t *handle;			/* Session handle */
	char *dev;			/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct bpf_program fp;		/* The compiled filter */
	char filter_exp[] = "port 23";	/* The filter expression */
	bpf_u_int32 mask;		/* Our netmask */
	bpf_u_int32 net;		/* Our IP */
	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet;		/* The actual packet */

	/* Define the device */
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}
	/* Find the properties for the device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
	/* Open the session in promiscuous mode */
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return(2);
	}
	/* Compile and apply the filter */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	/* Grab a packet */
	packet = pcap_next(handle, &header);
	/* Print its length */
	printf("Jacked a packet with length of [%d]\n", header.len);
	/* And close the session */
	pcap_close(handle);
	return(0);
}

