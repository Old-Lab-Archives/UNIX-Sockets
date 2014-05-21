#include "meow.h" /* our main header file */
/* --- Defining file structures ---
The program reads up to MAXFILES files from the web server.
We maintain a file structure with information about each file:-
- It's name (copied from the command line argument)
- The hostname or IP address of the server to read the file, also socket descriptor being used for the file.
- Set of flags to specify what we are gonna do with this yummy file (connecting or reading or )
*/
#define MAXFILES 20
#define SERV "80" /* port number or service name */
struct file
{
	char *f_name; /*filename*/
	char *f_host; /*hostname or IPv4/IPv6 address*/
	int f_fd; /*descriptor*/
	int f_flags;
}
file[MAXFILES]
#define F_CONNECTING 1 /*connect() in progress*/
#define F_READING 2 /*connect() complete, now reading*/
#define F_DONE 4 /* all done */
#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"
/* globals */
int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set rset, wset;
/* function prototypes */
void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);
