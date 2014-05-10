#include "meow.h"
int main(int argc, char **argv)
{
	int listenfd,connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_un cliaddr, servaddr;
	void sig_chld(int);
	listenfd=Socket(AF_LOCAL, SOCK_STREAM, 0);
	unlink(unixstr_path);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path,unixstr_path);
	bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
	listen(listenfd,listenq);
	signal(sigchld,sig_chld);
	for(; ;)
	{
		clilen=sizeof(cliaddr);
		if((connfd=accept(listenfd,(SA *) &cliaddr, &clilen)) < 0)
		{
			if(errno == EINTR)
				continue;
			else
				err_sys("error");
		}
		if((childpid=Fork()) == 0)
		{
			/*child process*/
			close(listenfd); /*closing listening socket*/
			str_echo(connfd); /*process request*/
			exit(0);
		}
		close(connfd); /*parent closes connected socket*/
	}
}
