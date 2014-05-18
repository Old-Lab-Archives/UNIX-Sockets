/* this program is an initialization of proc_v6 which helps in initializing ICMPv6 socket */
void init_procv6()
{
#ifdef IPV6
	int on=1;
	if(verbose==0)
	{
		/* Installing a filter that only passes ICMP6_ECHO_REPLY unless verbose */
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilt);
		setsockopt(sockfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
		/*ignore error return, the filter is an optimization.*/
	}
	/* ignore the above error returned */
#ifdef IPV6_RECVHOPLIMIT
	/* RFC 3542 */
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
#endif
#endif
}
