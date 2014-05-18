/* This program uses proc_v4 function which processes ICMPv4 message */
/* proc_v4 syntax
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
(refer <ping.h> ) */
#include "ping.h"
void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	int hlen1, icmplen;
	double rtt;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;
	ip=(struct ip *) ptr; /* start of IP header */
	hlen1=ip->ip_h1 << 2; /* length of IP header */
	if(ip->ip_p!=IPPROTO_ICMP)
		return; /* not ICMP */
	icmp=(struct icmp *) (ptr + hlen1); /* ICMP header */
	if((icmplen=len-hlen1) < 8)
		return; /* bad packets */
	/* Checking for ICMP echo reply */
	if(icmp->icmp_type==ICMP_ECHOREPLY)
	{
		if(icmp->icmp_id!=pid)
			return; /* Not a response to ECHO_REQUEST */
		if(icmplen<16)
			return; /* not enough data to use */
		tvsend=(struct timeval *)
			icmp->icmp_data;
		tv_sub(tvrecv, tvsend);
		/* calculating rtt by subtracting the time the message was sent from current time.
		Then, rtt is converted from microseconds to milliseconds */
		rtt=tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
		printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f \n",
			icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
			icmp->icmp_seq, ip->ip_ttl, rtt);
	}
	/* printing all recived ICMP message only if verbose option is specified */
	else if(verbose)
	{
		printf("%d bytes from %s: type=%d, code=%d \n",
			icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
			icmp->icmp_type, icmp->icmp_code);
	}
}
/*AshuMeow*/
