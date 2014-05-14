#include "meow.h"
static void recvfrom_alarm(int);
static int pipefd[2];
void dg_cli(FILE *fp,int sockfd,const SA *pservaddr,socklen_t servlen)
{
	int n, maxfdp1;
	const int on=1;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	fd_set rset;
	socklen_t len;
	struct sockaddr *preply_addr;
	preply_addr=malloc(servlen);
	SetSockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
	pipe(pipefd);
	maxfdp1=max(sockfd,pipefd[0]) + 1;
	FD_ZERO(&rset);
	Signal(SIGALRM,recvfrom_alarm);
	while(Fgets(sendline,MAXLINE,fp) != NULL)
	{
		SendTo(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		alarm(5);
		for(; ;)
		{
			FD_SET(sockfd, &rset);
			FD_SET(pipefd[0], &rset);
			if((n=select(maxfdp1,&rset,NULL,NULL,NULL)) < 0)
			{
				if(errno==EINTR)
					continue;
				else
					err_sys("error");
			}
			if(FD_ISSET(sockfd, &rset))
			{
				len=servlen;
				n=recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
				recvline[n]=0; /*null terminate*/
				printf("From %s: %s", Sock_ntop_host(preply_addr, len), recvline);
			}
			if(FD_ISSET(pipefd[0], &rset))
			{
				read(pipefd[0], &n, 1); /*Timer expired*/
				break;
			}
		}
	}
	free(preply_addr);
}
static void recvfrom_alarm(int signo)
{
	write(pipefd[1], " ", 1); /*write one null byte to pipe*/
	return;
}
