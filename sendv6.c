/* This program creates send_v6 functionality which builds and sends ICMPv6 echo request message */
#include "ping.h"
void send_v6()
{
#ifdef IPV6
	int len;
	struct icmp6_hdr *icmp6;
	icmp6=(struct icmp6_hdr *) sendbuf;
	icmp6->icmp6_type=ICMP6_ECHO_REQUEST;
	icmp6->icmp6_code=0;
	icmp6->icmp6_id=pid;
	icmp6->icmp6_seq=nsent++;
	memset((icmp6 + 1), 0Xa5, datalen); /* filling with patterns */
	Gettimeofday((struct timeval *) (icmp6 + 1), NULL);
	len = 8 + datalen; /* 8-byte ICMP header */
	Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
	/* Kernel calculates and stores the checksum */
#endif
}
