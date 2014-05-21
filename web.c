/* Web */
/* Let's do some theory first... */
/* What is done here may not be fully correct! If found anything wrong, then simply contribute. (^_^) */
/*
The web client using Non-blocking connect concept in UNIX.
Real-world example:- Netscape Web Client.
*/
/*
Netscape web client:-
--- The client establishes an HTTP connection with the web server and fetches the homepage.
--- And also frequently, that page has got several references to other pages.
--- So, instead of fetching these pages "serially" one at a time, the client can fetch more than 1 at a time using "non-blocking connects".

How multiple connections are established in parallel?
Case 1: Three connections done serially.
Case 2: Three connections done in parallel, maximum of two connections at a time.
Case 3: Three connections done in parallel, maximum of three connections at a time.

*/
#include "web.h"
/* our <web.h> file ---> https://github.com/Geek-Research-Lab/UNIX-Sockets/blob/master/web.h */
int main(int argc, char **argv)
{
	int i, fd, n, maxnconn, flags, error;
	char buf[MAXLINE];
	fd_set rs, ws;
	if(argc < 5)
		err_quit("web <#conns> <hostname> <homepage> <file1> ... ");
	maxnconn = atoi(argv[1]);
	/* Processing command line arguments */
	nfiles = min(argc - 4, MAXFILES);
	for(i=0;i<nfiles;i++)
	{
		file[i].f_name=argv[i+4];
		file[i].f_host=argv[2];
		file[i].f_flags=0;
	}
	printf("nfiles = %d \n", nfiles);
	/* reading homepage */
	home_page(argv[2], argv[3]);
	/* Initializing globals */
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	maxfd = -1;
	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;

	void home_page(const char *host, const char *fname) /* function prototype */
	{
		int fd, n;
		char line[MAXLINE];
		/* Establishing connection with server */
		fd = tcp_connect(host, SERV); /* blocking connect() */
		/* Sending HTTP command to server, reading reply */
		n = snprintf(line, sizeof(line), GET_CMD, fname);
		writen(fd, line, n);
		for(; ;)
		{
			if((n = read(fd, line, MAXLINE)) == 0)
				break; /* served closed connection */
			printf("read %d bytes of homepage \n", n);
		}
		printf("end of file on homepage \n");
		close(fd);
	}

	void start_connect(struct file *fptr) /* function prototype */
	{
		int fd, flags, n;
		struct addrinfo *ai;
		/* Creating socket and setting to non-blocking*/
		ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
		fd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		fptr->f_fd = fd;
		printf("start_connect for %s, fd %d \n", fptr->f_name, fd);
		/* Set socket non-blocking */
		flags = Fcntl(fd, F_GETFL, 0);
		Fcntl(fd, F_SETFL, flags | O_NONBLOCK);
		/* Initiate non-blocking connect to the server */
		if((n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0)
		{
			if(errno!=EINPROGRESS)
				err_sys("nonblocking connect error");
			fptr->f_flags = F_CONNECTING;
			FD_SET(fd, &rset); /* select for reading and writing */
			FD_SET(fd, &wset);
			if(fd > maxfd)
				maxfd = fd;
		}
		/* Handling connection completely */
		else if(n >= 0) /* connection is already done */
			write_get_cmd(fptr); /* write() the GET command */
	}

	void write_get_cmd(struct file *fptr) /* function prototype */
	{
		int n;
		char line[MAXLINE];
		/* Building command and sending it */
		n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
		writen(fptr->f_fd, line, n);
		printf("Done with %d bytes for %s \n", n, fptr->f_name);
		/* Setting flags */
		fptr->f_flags = F_READING; /* clears F_CONNECTING */
		FD_SET(fptr->f_fd, &rset); /* read server's reply */
		if(fptr->f_fd > maxfd)
			maxfd = fptr->f_fd;
	}
}
