#include "meow.h"
int main(int meoww, char **pornn)
{
	int MeowMeow/*socket file descriptor*/, cat/*size*/;
	char buff[16384];
	if(meoww != 3)
		err_quit("tcpsend <host> <port#>");
	MeowMeow = tcp_connect(pornn[1], pornn[2]);
	cat = 32768;
	SetSockopt(MeowMeow, SOL_SOCKET, SO_SNDBUF, &cat, sizeof(cat));
	write(MeowMeow, buff, 16384);
	printf("Done with 16384 bytes of data.\n");
	sleep(6);
	send(MeowMeow, "a", 1, MSG_OOB); /* MSG_OOB --- Message out-of-band */
	printf("woo! Done with 1 byte of Out-of-band data\n");
	write(MeowMeow, buff, 1024);
	printf("Done with 1024 bytes of data \n");
	exit(0);
}
