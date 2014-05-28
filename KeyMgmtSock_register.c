#include "meow.h"
#include "MeowKey.h"
/* Opening PF_KEY socket */
void sadb_register(int type)
{
	int s;
	char buf[4096];
	struct sadb_msg msg;
	int goteof;
	int mypid;
	s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);
	/* Storing PID */
	mypid = getpid();
	/* building and writing SADB_REGISTER request */
	/* Creating SADB_REGISTER message */
	bzero(&msg, sizeof(msg));
	msg.sadb_msg_version = PF_KEY_V2;
	msg.sadb_msg_register = SADB_REGISTER;
	msg.sadb_msg_satype = type;
	msg.sadb_msg_len = sizeof(msg);
	msg.sadb_msg_pid = mypid;
	/* displaying and writing message to socket */
	printf("Sending message... =) \n");
	print_sadb_msg(&msg, sizeof(msg));
	write(s, &msg, sizeof(msg));
	printf("\n reply returned... =) \n");
	/* read and print SADB_REGISTER reply, discarding others */
	/* waiting for reply */
	for(; ;)
	{
		int msglen;
		struct sadb_msg *msgp;
		msglen = read(s, &buf, sizeof(buf));
		msgp = (struct sadb_msg *) &buf;
		if(msgp->sadb_msg_pid == mypid && msgp->sadb_msg_type == SADB_REGISTER)
		{
			print_sadb_msg(msgp, msglen);
			break;
		}
	}
	close(s);
}

