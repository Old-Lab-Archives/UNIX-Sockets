/* Heartbeat control utility function from SCTP */
/* SCTP : http://en.wikipedia.org/wiki/Stream_Control_Transmission_Protocol */
/*
SCTP provides heartbeat mechanism 
*/
#include "meow.h"
int heartbeat_action(int MeowSockFD, struct sockaddr *sa, sockelen_t slen, u_int value)
{
	struct sctp_paddrparams sp;
	int size;
	/* zero sctp_paddrparams struct and copy interval */
	bzero(&sp,sizeof(sp));
	sp.spp_hbinterval = value;
	/* Setting up address */
	memcpy((caddr_t) & sp.spp_address, sa, slen);
	/* Performing action */
	SetSockopt(MeowSockFD, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS, &sp, sizeof(sp));
	return(0);
}
/*Next comes the concurrent SCTP server part*/
for(; ;)
{
	len = sizeof(struct sockaddr_in);
	/* Recieve and process first message from client */
	rd_sz = sctp_recvmsg(MeowSockFD, readbuf, sizeof(readbuf), (SA *) &cliaddr, &len, &sri, &msg_flags);
	sctp_sendmsg(MeowSockFD, readbuf, rd_sz, (SA *) &cliaddr, len, sr.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0);
	/* Translation address to association ID */
	assoc = sctp_address_to_associd(MeowSockFD, (SA *) &cliaddr, len);
	if((int) assoc == 0)
	{
		err_ret("unable to get association id");
		continue;
	}
	/* Extract association */
	connfd = sctp_peeloff(MeowSockFD, assoc);
	if(connfd == -1)
	{
		err_ret("error");
		continue;
	}
	/* Delegate work to child */
	if((childpid=fork()) == 0)
	{
		close(MeowSockFD);
		str_echo(connfd);
		exit(0);
	}
	else
	{
		close(connfd);
	}
}
