/* Aim: To print host's hardware addresses */
/*
Function used:- get_ifi_info --- helps in returning all host's IP addresses.
The ioctl which uses SIOCGARP for each IP address inorder to obtain and print the hardware addresses.
*/
/* AshuMeow */
/*
Syntax for get_ifi_info ---- refer our header file <interface.h>
struct ifi_info *get_ifi_info(int, int);
struct ifi_info *get_ifi_info(int, int);
*/
#include "interface.h"
#include "arp_req.h"
int main(int argc, char **argv)
{
	int sockfd;
	struct ifi_info *ifi;
	unsigned char *ptr;
	struct arpreq arpreq;
	struct sockaddr_in *sin;
	sockfd=Socket(AF_INET, SOCK_DGRAM, 0);
	/* Getting list of addresses and loop through each one. */
	/*
	We call get_ifi_info to obtain the host's IP addresses and then loop through each address.
	*/
	for(ifi=get_ifi_info(AF_INET, 0);ifi!=NULL;ifi->ifi_next)
	{
		/* Printing IP address */
		/*
		We will print the IP address using Sock_ntop.
		We have requested  get_ifi_info only to return IPv4 addresses since this ARP concept isn't used in IPv6.
		*/
		printf("%s: ", Sock_ntop(ifi->ifi_addr, sizeof(struct sockaddr_in)));
		/* Issuing ioctl and checking for error */
		/*
		We will fill the arp_pa structure as an IPv4 socket address structure containing the IPv4 address.
		Then, ioctl is called.
		If ioctl returns an error, then we will print error and we'll gradually jump to the next address.
		*/
		/*
		Syntax for arp_pa: --- refer our header <arp_req.h>
		struct sockaddr arp_pa;
		*/
		sin=(struct sockaddr_in *) &arpreq.arp_pa;
		memcpy(sin,ifi->ifi_addr,sizeof(struct sockaddr_in));
		if(ioctl(sockfd,SIOCGARP,&arpreq)<0)
		{
			err_ret("ioctl SIOCGARP");
			continue;
		}
		/* Printing hardware address */
		/*
		Returning successfully from the ioctl, the hardware address is thereby printed. :)
		*/
		ptr= &arpreq.arp_ha.sa_data[0];
		printf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
	}
	exit(0);
}
