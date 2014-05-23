/* Web kinda thingy with sockets mix */
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
Yet, another better scenario,
Complete first connection, followed by mutiple connections in parallel.
Like...The first connection is done by itself (The wife does cooking by herself.)...
And thereby followed by,
multiple connections for the references found in the data from that first connection..
(followed by husband who can find food in kitchen cooked by wife...)
So, Without accomplishing first process, the second process can't be done.

For optimization of sequences, the client can start parsing the data that is returned for 1st connection before it does completion.
Since we are into multiple non-blocking connects at the same time, we can't use our "connect_nonb function"...
Whoa! connect_nonb function?? Check our repository --- File name : "nonblock_connect.c"
We will be specifying our program as command line arguments where it has maximum number of parallel connections, the server's hostname and each of file names to fetch from the server.
The command line arguments specify three simultaneous connections,
--- The server's hostname
--- The filename for the homepage (the server's root page)
--- The files which will be referenced to homepage.
--- Then, a web client that would read the homepage and parse the HTML to obtain these filenames.
And instead of complicating it with HTML parsing, let's just simply go on specifying the filenames on the command line.
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
	/* Processing command line arguments
	The file structures are filled in with the relevant information from the command line arguments.
	*/
	nfiles = min(argc - 4, MAXFILES);
	for(i=0;i<nfiles;i++)
	{
		file[i].f_name=argv[i+4];
		file[i].f_host=argv[2];
		file[i].f_flags=0;
	}
	printf("nfiles = %d \n", nfiles);
	/* reading homepage
	The home_page function which creates a TCP connection where it sends a command to the server.
	Like... Let's take client-server computing concept..
	Client request from the server and the server responds to the request sent by the client.
	Just like that here... The first connection is done by itself before we start establishing multiple connections in parallel.
	*/
	home_page(argv[2], argv[3]);
	/* Initializing globals
	There are two descriptor sets ---> One for reading (rset) and one for writing (wset), where they are initialized.
	maxfd---> Maximum File Descriptor, which is used for 'select' operation, where we initialize to -1 since descriptor is non-negative.
	nlefttoread---> Number of Files remaining to be read. And when this function 'nlefttoread' reaches 0, we are done.
	nlefttoconn---> Number of files that still fucking need a TCP connection. It's also the number of connections that are currently open which can never exceed the 1st command line argument.
	*/
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	maxfd = -1;
	nlefttoread = nlefttoconn = nfiles;
	nconn = 0;
	/* Homepage function */
	void home_page(const char *host, const char *fname) /* function prototype */
	{
		int fd, n;
		char line[MAXLINE];
		/* Establishing connection with server
		Our tcp_connect establishes a connection with the server.
		*/
		fd = tcp_connect(host, SERV); /* blocking connect() */
		/* Sending HTTP command to server, reading reply
		HTTP 'GET' command is issued for the homepage. The reply is read & then, connection is closed.
		*/
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
		/* Creating socket and setting to non-blocking
		We will be calling our host_serv function to look up & convert the hostname and service name where pointer is returned to array of addrinfo structures.
		Only 1st structure is used.
		A TCP socket is created & the socket is set to non-blocking.
		*/
		ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
		fd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		fptr->f_fd = fd;
		printf("start_connect for %s, fd %d \n", fptr->f_name, fd);
		/* Set socket non-blocking */
		flags = Fcntl(fd, F_GETFL, 0);
		Fcntl(fd, F_SETFL, flags | O_NONBLOCK);
		/* Initiate non-blocking connect to the server
		The non-blocking connect is initiated & file's flag is set to F_CONNECTING.
		read set (rset) and write set (wset) are turned on. { socket descriptors }
		Then, maxfd is updated.
		*/
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
		/* Building command and sending it 
		The command is built and written to the socket.
		*/
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

/* AshuMeow */
/* NOTE: The program and the related ones will be re-updated soon. */
