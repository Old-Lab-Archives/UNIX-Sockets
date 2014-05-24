#include "meow.h"
#include "mcast.h"
#include<net/if.h>
/* Part 1: Joining multicast group ~~ IP version independent */
int mcast_join(int sockfd, const struct sockaddr *grp, socklen_t grplen, const char *ifname, u_int ifindex)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;
	/* Handling index */
	if(ifindex > 0)
	{
		req.gr_interface = ifindex;
	}
	else if(ifname != NULL)
	{
		if((req.gr_interface = if_nametoindex(ifname)) == 0)
		{
			errno = ENXIO; /* if name not found */
			return (-1);
		}
	}
	else
		req.gr_interface = 0;
	/*---- Copying address and calling setsockopt---- */
	/* caller's socket address is directly copied into request's group field */
	if(grplen > sizeof(req.gr_group))
	{
		errno = EINVAL;
		return (-1);
	}
	memcpy(&req.gr_group, grp, grplen);
	/* Performance of join function by setsockopt */
	return(setsockopt(sockfd, family_to_level(grp->sa_family), MCAST_JOIN_GROUP, &req, sizeof(req)));
#else
	/* Part 2: Joining a multicast group ~~ IPv4 socket */
	switch(grp->sa_family)
	{
	case AF_INET:
		{
			struct ip_mreq mreq;
			struct ifreq ifreq;
			memcpy(&mreq.imr_multiaddr, &((const struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
			/* Handling index */
			if(ifindex > 0)
			{
				if(if_indextoname(ifindex, ifreq.ifr_name) == NULL)
				{
					errno = ENXIO; /* if index not found */
					return (-1);
				}
				goto doioctl;
			}
			/* Handling name */
			else if(ifname != NULL)
			{
				strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
				if(ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					return (-1);
				memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
			}
			/* specifying default...
			if the index wasn't specified, then interface is set to wildcard address telling Kernel to choose interface..*/
			else
				mreq.imr_interface.s_addr = hton1(INADDR_ANY);
			/* Performance of join function by setsockopt */
			return (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)));
		}
		/* Part 3:-- Joining a multicast group ~~ IPv6 socket */
#ifdef IPV6
	case AF_INET6:
		{
			struct ipv6_mreq mreq6;
			/* copying address */
			memcpy(&mreq6.ipv6mr_multiaddr, &((const struct sockaddr_in6 *) grp)->sin6_addr, sizeof(struct in6_addr));
			/* Handling index, name, default */
			if(ifindex > 0)
			{
				mreq6.ipv6mr_interface = ifindex;
			}
			else if(ifname != NULL)
			{
				if((mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0)
				{
					errno = ENXIO; /* if name not found */
					return (-1);
				}
			}
			else
				mreq6.ipv6mr_interface = 0;
			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)));
		}
#endif
	default:
		errno = EAFNOSUPPORT;
		return (-1);
	}
#endif
}
