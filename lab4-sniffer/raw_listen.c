#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
// net/ has lower lever stuff
// netinet/ has higher level stuff

int main(int argc, char **argv) {
	int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	struct sockaddr_ll addr, listenaddr;
	listenaddr.sll_family = AF_PACKET;  // if casted to generic sockaddr, remembers type
	listenaddr.sll_protocol = htons(ETH_P_ALL);
	listenaddr.sll_ifindex = if_nametoindex("h1-eth0");  // which interface. Has to be one listed in ifconfig
	bind(sockfd, (struct sockaddr *) &listenaddr, sizeof listenaddr);

	while (1) {
		int len = sizeof addr;
		char buf[5000];
		int n = recvfrom(sockfd, buf, 5000, 0, (struct sockaddr *) &addr, &len);

		if (addr.sll_pkttype != PACKET_OUTGOING) {
			printf("Got a packet\n");
			struct ether_header eh;
			memcpy(&eh, &buf[0], 14);
			printf("Destination: %s\n", ether_ntoa((struct ether_addr *) &eh.ether_dhost) );
		}
	}
}
