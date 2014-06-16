pcap: pcap.o
	gcc -o pcap pcap.o -lpcap
sniff: sniff.o
	gcc -o sniff sniff.o -lpcap
socket_raw: socket_raw.o
socket_raw_promisc: socket_raw_promisc.o
socket_raw_filter: socket_raw_filter.o
all: pcap sniff socket_raw socket_raw_promisc socket_raw_filter
clean:
	rm pcap
	rm sniff
	rm socket_raw
	rm socket_raw_promisc
	rm socket_raw_filter
	rm *.o
