#include "meow.h"
int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_un servaddr, cliaddr;
	sockfd=Socket(AF_LOCAL, SOCK_DGRAM, 0);
	unlink(unixdg_path);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path, unixdg_path);
	bind(sockfd,(SA *) &servaddr,sizeof(servaddr));
	dg_echo(sockfd,(SA *) &cliaddr,sizeof(cliaddr));
}
