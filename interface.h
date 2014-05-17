/* This header is created for the programs that requires interface configuration info */
#ifndef __interface_h
#define __interface_h
#include "meow.h"
#include<net/if.h>
#define IFI_NAME 16 /* it's actually a similar one which we could see in <net/if.h> header */
#define IFI_HADDR 8 /* allowing 64-bit in future */
struct ifi_info
{
	char ifi_name[IFI_NAME]; /* interface name, null terminated */
	short ifi_index; /* interface index */
	short ifi_mtu; /* interface MTU */
	u_char ifi_haddr[IFI_HADDR]; /* hardware address */
	u_short ifi_hlen; /* #bytes in hardware address : 0, 6, 8 */
	short ifi_flags; /* IFF_xxx constants from <net/if.h> header */
	short ifi_myflags; /* our own IFI_xxx flags */
	struct sockaddr *ifi_addr; /* primary address */
	struct sockaddr *ifi_brdaddr; /* Broadcast address */
	struct sockaddr *ifi_dstaddr; /* destination address */
	struct ifi_info *ifi_next; /* next part of the structure */
};
#define IFI_ALIAS 1 /* ifi_addr is an alias */
/* Function prototypes*/
struct ifi_info *get_ifi_info(int, int);
struct ifi_info *get_ifi_info(int, int);
void free_ifi_info(struct ifi_info *);
#endif
