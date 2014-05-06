/* Own header */
#ifndef __meow_h 
#define __meow_h
/*Config for OS*/
#include "../config.h"
/*basic system data types*/
#include <sys/types.h>
/*basic socket definition*/
#include<sys/socket.h>
/* timeval{} for select() */
#include<sys/time.h>
/* timespec{} for pselect() */
#include<time.h>
/* sockaddr_in{} and other definitions */
#include<netinet/in.h>
/* inet(3) functions */
#include<arpa/inet.h>
#include<errno.h>
/* for non-blocking */
#include<fcntl.h>
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* for S_xxx file mode contains */
#include<sys/stat.h>
/* for iovec{} and readv/writev */
#include<sys/uio.h>
#include<unistd.h>
#include<sys/wait.h>
/*for UNIX domain sockets */
#include<sys/un.h>


#ifdef HAVE_SYS_SELECT_H
/*for convenience */
#include<sys/select.h>
#endif
#ifdef HAVE_POLL_H
/*for convenience */
#include<poll.h>
#endif
/*3 headers for socket/file ioctl : <sys/ioctl.h>, <sys/filio.h>, <sys/sockio.h> */
#ifdef HAVE_SYS_IOCTL_H
#include<sys/ioctl.h>
#endif
#ifdef HAVE_SYS_FILIO_H
#include<sys/filio.h>
#endif
#ifdef HAVE_SYS_SOCKIO_H
#include<sys/sockio.h>
#endif
#ifdef HAVE_PTHREAD_H
#include<pthread.h>
#endif
/*OSF/1 disables recv() and send() in <sys/socket.h> */
#ifdef __OSF__
#undef recv
#undef send
#define recv(a,b,c,d) recvfrom(a,b,c,d,0,0)
#define send(a,b,c,d) sendto(a,b,c,d,0,0)
#endif
#ifdef IN_ADDR
/* <netinet/in.h> */
#define IN_ADDR 0Xffffffff
#endif
/* 3 POSIX.lg */
#ifdef SHUT_RD
/* shut down for reading */
#define SHUT_RD 0
/* shut down for writing */
#define SHUT_RD 1
/* shut down for reading and writing */
#define SHUT_RD 2
#endif
#ifdef INET_ADDRSTRLEN
/* "ddd.ddd.ddd.ddd\0" */
#define INET_ADDRSTRLEN
#endif


#ifndef INET6_ADDRSTRLEN 
/* max size of IPv6 address string */
#define INET6_ADDRSTRLEN
#endif
/* bzero() as macro if not a std. C Lib. */
#ifndef HAVE_BZERO
#define bzero(ptr,n) memset(ptr,0,n)
#endif
/*older resolvers don't have gethostbyname() */
#ifndef HAVE_GETHOSTBYNAME2
#define gethostbyname2(host,family) gethostbyname((host))
#endif
/*structure returned by recvfrom_flags()*/
struct in_pktinfo
{
	/* dst IPv4 address */
	struct in_addr;
	/*recieved interface index */
};
/* CMSG_LEN() and CMSG_SPACE() macro, ALIGN() macro */
#ifndef CMSG_LEN
#define CMSG_LEN(size) (sizeof(struct cmsghdr) + (size))
#endif
#ifndef CMSG_SPACE
#define CMSG_SPACE(size) (sizeof(struct cmsghdr + (size))
#endif
/* Posix.lg requires SUN_LEN */
#ifndef SUN_LEN
#define SUN_LEN(su) \
	(sizeof(*(su)) - sizeof((su)->sun path) + strlen((su)->sun path))
#endif
/* Posix.lg renames "Unix Domain" as "local IPC", not all systems define AF_LOCAL and AF_LOCAL(yet) */
#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
#define PF_LOCAL PF_UNIX
#endif
/* Posix.lg requires #include <poll.h>, define INFTIM, <sys/stropts.h> */
#ifndef INFTIM
/*infinite poll timeout */
#define INFTIM (-1)
#ifdef HAVE_POLL_H
/*defining unpxti.h */
#define INFTIM_UNPH
#endif
#endif
/* <sys/socket.h> in SOMAXCONN */
/* 2nd argument to listen() */
#define LISTENQ 1024
/* Miscellaneous constants */
/*max text line length */
#define MAXLINE 4096
/* max socket address structure size */
#define MAXSOCKADDR 128
/*buffer size for reads and writes */
#define BUFFSIZE 8192
/*define port number used in client-server*/
/*TCP and UDP cient servers */
#define SERV_PORT 9877
/* TCP and UDP client servers */
#define SERV_PORT_STR "9877"
/* UNIX domain stream client server */
#define UNIXSTR_PATH "/tmp/unix.str"
/* UNIX domain datagram client server */
#define UNIXDG_PATH "/tmp/unix.dg"
/* shortens all type casts of pointer arguments */
#define SA struct sockaddr
/* default file access permissions for new files */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default permissions for new directories */
#define DIR_MODE
/* signal handlers */
typedef void Sigfunc(int);
#define min(a,b) ((a) < b ? (a) : (b))
#define max(a,b) ((a) < b ? (a) : (b))
#ifndef HAVE_ADDRINFO_STRUCT
#include "../lib/addrinfo.h"
#endif
#ifndef HAVE_IF_NAMEINDEX_STRUCT
struct if_nameindex
{
	/* 1,2, ..... */
	unsigned int if_index;
	/* null terminated name:"le0",...*/
	char *if_name;
};
#endif
#ifndef HAVE_TIMESPEC_STRUCT
struct timespec
{
	/*seconds*/
	time_t tv_sec;
	/*nanoseconds*/
	long tv_nsec;
};
#endif
