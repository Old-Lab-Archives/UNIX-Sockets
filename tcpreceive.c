#include "meow.h"
int ListenMeow/*Listen file descriptor*/, ConnectMeow/*Connect file descriptor*/;
void sig_urg(int);
int main(int meoww, char **pornn)
{
	int cat; /*size*/
	if(meoww == 2)
		ListenMeow = tcp_listen(NULL, pornn[1], NULL);
	else if(meoww == 3)
		ListenMeow = tcp_listen(pornn[1], pornn[2], NULL);
	else
		err_quit("tcpreceive [<host>] <port#>");
	cat = 4096;
	SetSockopt(ListenMeow, SOL_SOCKET, SO_RCVBUF, &cat, sizeof(cat));
	ConnectMeow = accept(ListenMeow, NULL, NULL);
	Signal(SIGURG, sig_urg);
	Fcntl(ConnectMeow, F_SETOWN, getpid());
	for(; ;)
		pause();
}
