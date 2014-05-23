#include "meow.h"
int connect_nonb(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	int flags, n, error;
	socklen_t len;
	fd_set rset, wset;
	struct timeval tval;
	/* Setting up the socket --- blocking */
	/* We'll be calling Fcntl for setting the socket to non-blocking */
	flags = Fcntl(sockfd, F_GETFL, 0);
	Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	/*
	We'll be initiating the non-blocking connect, where the error which we will be expecting would be EINPROGRESS.
	Hence, that will actually indicate the initialization of connection, but not yet complete.
	Any error might be possible to return.
	*/
	error = 0;
	if((n = connect(sockfd, saptr, salen)) < 0)
		if(errno != EINPROGRESS)
			return(-1);
	/* During connectivity */
	/* Overlap processing with connection establishment */
	if(n == 0) /* Checking for immediate completion */
		goto done; /* connection established */
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;
	if((n = Select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) == 0)
	{
		/* Handling timeout*/
		close(sockfd);
		errno = ETIMEOUT;
		return(-1);
	}
	/* checking for readability or writability */
	if(FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			return(-1); /* pending error */
	}
	else
		err_quit("error --- sockfd not set ");
done:
	Fcntl(sockfd, F_SETFL, flags); /* restore file status flags */
	if(error)
	{
		close(sockfd);
		errno = error;
		return(-1);
	}
	return 0;
}
