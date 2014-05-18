#include "ping.h"
void readloop(void)
{
	int size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;
	/* creating socket */
	sockfd=Socket(pr->sasend->sa_family,SOCK_RAW,pr->icmpproto);
	setuid(getuid()); /*special permissions not required*/
	/* Performing protocol specific initialization */
	if(pr->finit)
		(*pr->finit) ();
	/* Setting socket receive buffer size */
	size=60*1024; /* okay if setsockopt fails */
	setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size));
	sig_alrm(SIGALRM); /*sending first packet*/
	/* Setting up msghdr for recvmsg */
	iov.iov_base=recvbuf;
	iov.iov_len=sizeof(recvbuf);
	msg.msg_name=pr->sarecv;
	mas.msg_iov= &iov;
	msg.msg_iovlen=1;
	msg.msg_control=controlbuf;
	/* Infinite loop reading all ICMP messages */
	for(; ;)
	{
		msg.msg_namelen=pr->salen;
		msg.msg_controllen=sizeof(controlbuf);
		n=recvmsg(sockfd,&msg,0);
		if(n<0)
		{
			if(errno==EINTR)
				continue;
			else
				err_sys("error");
		}
		Gettimeofday(&tval,NULL);
		(*pr->fproc) (recvbuf,n,&msg,&tval);
	}
}
