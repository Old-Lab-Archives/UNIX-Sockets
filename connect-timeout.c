#include "meow.h"
static void connect_alarm(int);
int connect-timeout(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc *sigfunc;
	int n;
	sigfunc=Signal(SIGALRM, connect_alarm);
	if(alarm(nsec) != 0)
		err_msg("alarm already set");
	if((n=connect(sockfd, saptr, salen)) < 0)
	{
		close(sockfd);
		if(errno == EINTR)
			errno=ETIMEDOUT;
	}
	alarm(0); /*turn off the alarm*/
	Signal(SIGALRM, sigfunc); /*restore previous signal handler*/
	return(n);
}
static void connect_alarm(int signo)
{
	return; /*interrupt the connect()*/
}
