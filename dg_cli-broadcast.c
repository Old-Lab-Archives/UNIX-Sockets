#include "meow.h"
static void recvfrom_alarm(int);
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int n;
	const int on=1;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	socklen_t len;
	struct sockaddr *preply_addr;
	preply_addr=malloc(servlen);
	SetSockopt(sockopt, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	Signal(SIGALRM, recvfrom_alarm);
	while(Fgets(sendline,MAXLINE,fp) != NULL)
	{
		SendTo(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		alarm(5);
		for(; ;)
		{
			len=servlen;
			n=recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
			if(n < 0)
			{
				if(errno==EINTR)
					break;
				else
					err_sys("error");
			}
			else
			{
				recvline[n]=0; /*null terminate*/
				printf("from %s: %s", Sock_ntop_host(preply_addr, len), recvline);
			}
		}
	}
	free(preply_addr);
}
static void recvfrom_alarm(int signo)
{
	return; /*interrupt the recvfrom()*/
}
