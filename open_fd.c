/* open_fd.c (open_filedescriptor) === opens a file and returns a descriptor */
#include "meow.h"
int open_fd(const char *pathname, int mode)
{
	int fd, sockfd[2], status;
	pid_t childpid;
	char c, argsockfd[10], argmode[10];
	Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
	if((childpid=Fork())==0)
	{
		/*Child process*/
		Close(sockfd[0]);
		snprintf(argsockfd,sizeof(argsockfd),"%d",sockfd[1]);
		snprintf(argmode,sizeof(argmode),"%d",mode);
		execl("./openfile","openfile",argsockfd,pathname,argmode,(char *) NULL);
		err_sys("error");
	}
	/*parent process --- Wait for the child to terminate*/
	Close(sockfd[1]);
	Waitpid(childpid, &status, 0);
	if(WIFEXITED(status) == 0)
		err_quit("child did not terminate");
	if((status=WEXITSTATUS(status)) == 0)
		Read_fd(sockfd[0], &c, 1, &fd);
	else {
		errno = status;
		fd = -1;
	}
	Close(sockfd[0]);
	return(fd);
}
