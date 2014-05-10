#include "meow.h"
int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_un cliaddr, servaddr;
	sockfd=Socket(AF_LOCAL, SOCK_DGRAM, 0);
	bzero(&cliaddr,sizeof(cliaddr)); /*binding*/
	cliaddr.sun_family=AF_LOCAL;
	strcpy(cliaddr.sun_path,tmpnam(NULL));
	bind(sockfd,(SA *) &cliaddr,sizeof(cliaddr));
	bzero(&servaddr,sizeof(servaddr)); /*server's info*/
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path,unixdg_path);
	dg_cli(stdin,sockfd,(SA *) &servaddr,sizeof(servaddr));
	exit(0);
}

