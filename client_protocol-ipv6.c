/* This is "Protocol Independence" program using IPv6 for client protocol.
Like if you refer here---> https://github.com/Geek-Research-Lab/Operating-Systems/blob/UNIX/simple_client.cpp ---This is protocol dependent.
Protocol dependent concepts are used in IPv4. */
/*--So, it is cool to make a program as a protocol independent rather than dependent one... So, let's go here with IPv6 for client protocol-- */
#include "meow.h"
int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in6 servaddr;
	if(argc!=2)
		err_quit("usage: a.out <IPaddress>");
	if((sockfd=socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family=AF_INET6;
	servaddr.sin6_port=htons(13); /* simple_server --- refer: https://github.com/Geek-Research-Lab/Operating-Systems/blob/UNIX/simple_server.cpp */
	if(inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);
	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
	while((n=read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n]=0; /*null terminate*/
		if(fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if(n < 0)
		err_sys("read error");
	exit(0);
}
