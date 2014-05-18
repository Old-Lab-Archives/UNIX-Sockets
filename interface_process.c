/* The implementation of get_ifi_info that uses the SIOCGIFCONF ioctl is done in this program */
/* get_ifi_info syntax (found in <interface.h> --- https://github.com/Geek-Research-Lab/UNIX-Sockets/blob/master/interface.h)
struct ifi_info *get_ifi_info(int, int);
struct ifi_info *get_ifi_info(int, int);
*/
#include "interface.h"
/* Part 1: Obtaining the interface configuration from the Kernel */
struct ifi_info *get_ifi_info(int family, int doaliases)
{
	struct ifi_info *ifi, *ifihead, **ifipnext;
	int sockfd, len, lastlen, flags, myflags, idx=0, hlen=0;
	char *ptr, *buf, lastname[IFNAMSIZ], *cptr, *haddr, *sdlname;
	struct ifconf ifc;
	struct ifreq *ifr, ifrcopy;
	struct sockaddr_in *sinptr;
	/* Creating internet socket */
	struct sockaddr_in6 *sin6ptr;
	/* Issuing SIOCGIFCONF request in a loop */
	sockfd=socket(AF_INET,SOCK_DGRAM,0); /* dynamic allocation of buffer */
	lastlen=0;
	len=100 * sizeof(struct ifreq); /*initial buffer size*/
	for(; ;)
	{
		buff=malloc(len);
		ifc.ifc_len=len;
		ifc.ifc_buf=buf;
		if(ioctl(sockfd,SIOCGIFCONF,&ifc) < 0)
		{
			if(errno!=EINVAL || lastlen!=0) /* error of EINVAL is returned by ioctl */
				err_sys("ioctl error");
		}
		else
		{
			if(ifc.ifc_len==lastlen) 
				/*if ioctl returns success & if it return length equivalent to lastlen, no problem. Or else we gotta break out of loop */
				break; 
			lastlen=ifc.ifc_len;
		}
		/* the buffer size is increased 10 times inorder to hold ifreq structures */
		len+=10 * sizeof(struct ifreq); /*increment*/
		free(buf);
	}
	/* Initializing linked list pointers */
	ifihead=NULL;
	ifipnext=&ifihead;
	lastname[0]=0;
	sdlname=NULL;
	/* Part 2: Main program */
	for(ptr=buf;ptr<buf+ifc.ifc_len;)
	{
		/* stepping towards next socket address structure */
		ifr=(struct ifreq *) ptr;
#ifdef HAVE_SOCKADDR_SA_LEN
		len=max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
		switch(ifr->ifr_addr.sa_family)
		{
#ifdef IPV6
		case AF_INET6:
			len=sizeof(struct sockaddr_in6);
			break;
#endif
		case AF_INET:
		default:
			len=sizeof(struct sockaddr);
			break;
		}
#endif
		/*HAVE_SOCKADDR_SA_LEN*/
		ptr+=sizeof(ifr->ifr_name) + len; /*for next one in buffer*/
		/* Handling AF_LINK */
#ifdef HAVE_SOCKADDR_DL_STRUCT
		/*assuming that AF_LINK precedes AF_INET or AF_INET6*/
		if(ifr->ifr_addr.sa_family==AF_LINK)
		{
			struct sockaddr_dl *sdl=(struct sockaddr_dl *) &ifr->ifr_addr;
			sdlname=ifr->ifr_name;
			idx=sdl->sdl_index;
			haddr=sdl->sdl_data + sdl->sdl_nlen;
			hlen=sdl->sdl_alen;
		}
#endif
		if(ifr->ifr_addr.sa_family!=family)
			continue; /*ignored if it's not desired address family*/
		/* Handling aliases */
		myflags=0;
		if((cptr=strchr(ifr->ifr_name,':'))!=NULL)
			*cptr=0;
		if(strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0)
		{
			if(doaliases == 0)
				continue; /* already processed this interface */
			myflags=IFI_ALIAS;
		}
		memcpy(lastname,ifr->ifr_name,IFNAMSIZ);
		/* Fetching interface flags */
		ifrcopy = *ifr;
		ioctl(sockfd,SIOCGIFFLAGS,&ifrcopy);
		flags=ifrcopy.ifr_flags;
		if((flags & IFF_UP) == 0)
			continue; /* Ignored if interface dick doesn't stand up */
		/* Part 3: Allocating and Initializing ifi_info structure */
		/* Allocating and initiliazing ifi_info structure */
		ifi=calloc(1,sizeof(struct,ifi_info));
		*ifipnext=ifi; /* struct ifi_info *ifi_next; ---refer <interface.h> header---*/
		ifipnext=&ifi->ifi_next;
		ifi->ifi_flags=flags; /* IFF_xxx values */
		ifi->ifi_myflags=myflags; /* IFI_xxx values */
#if defined(SIOCGIFMTU) && defined(HAVE_STRUCT_IFREQ_IFR_MTU)
		ioctl(sockfd,SIOCGIFMTU,&ifrcopy);
		ifi->ifi_mtu=ifrcopy.ifr_mtu;
#else
		ifi->ifi_mtu=0;
#endif
		memcpy(ifi->ifi_name,ifr->ifr_name,IFI_NAME);
		ifi->ifi_name[IFI_NAME - 1] = '\0';
		/* if the sockaddr_dl is from different interface, then ignore it */
		if(sdlname == NULL || strcmp(sdlname,ifr->ifr_name)!=0)
			idx=hlen=0;
		ifi->ifi_index=idx;
		ifi->ifi_hlen=hlen;
		if(ifi->ifi_hlen > IFI_HADDR)
			ifi->ifi_hlen = IFI_HADDR;
		if(hlen)
			memcpy(ifi->ifi_haddr,haddr,ifi->ifi_hlen);
		/* Part 4: Fetch and return interface addresses */
		switch(ifr->ifr_addr.sa_family)
		{
		case AF_INET:
			/* copying the IP address that was returned from our original SIOCGIFCONF */
			sinptr=(struct sockaddr_in *) &ifr->ifr_addr;
			ifi->ifi_addr=calloc(1,sizeof(struct sockaddr_in));
			memcpy(ifi->ifi_addr,sinptr,sizeof(struct sockaddr_in));
#ifdef SIOCGIFBRDADDR
			/* if the interface supports broadcasting, then we will be fetching the broadcast address */
			if(flags & IFF_BROADCAST)
			{
				ioctl(sockfd,SIOCGIFBRDADDR,&ifrcopy);
				sinptr=(struct sockaddr_in *) &ifrcopy.ifr_broadaddr;
				ifi->ifi_brdaddr=calloc(1,sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_brdaddr,sinptr,sizeof(struct sockaddr_in));
			}
#endif
#ifdef SIOCGIFDSTADDR
			if(flags & IFF_POINTOPOINT)
			{
				ioctl(sockfd,SIOCGIFDSTADDR,&ifrcpy);
				sinptr=(struct sockaddr_in *) &ifrcopy.ifr_dstaddr;
				ifi->ifi_dstaddr=calloc(1,sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_dstaddr,sinptr,sizeof(struct sockaddr_in));
			}
#endif
			break;
		case AF_INET6:
			sin6ptr=(struct sockaddr_in6 *) &ifr->ifr_addr;
			ifi->ifi_addr=calloc(1,sizeof(struct sockaddr_in6));
			memcpy(ifi->ifi_dstaddr,sinptr,sizeof(struct sockaddr_in));
#ifdef SIOCGIFDSTADDR
			if(flags & IFF_POINTOPOINT)
			{
				ioctl(sockfd,SIOCGIFDSTADDR,&ifrcopy);
				sin6ptr=(struct sockaddr_in6 *) &ifr->ifr_addr;
				ifi->ifi_addr=calloc(1,sizeof(struct sockaddr_in6));
				memcpy(ifi->ifi_dstaddr,sinptr,sizeof(struct sockaddr_in));
			}
#endif
			break;
		default:
			break;
		}
	}
		free(buf);
		return(ifihead); /*pointer to first structure in linked list*/
}
		/* Part 5: free_ifi_info function which will free the dynamic memory allocated by get_ifi_info */
		void free_ifi_info(struct ifi_info *ifihead) /* void free_ifi_info(struct ifi_info *); --- refer <interface.h> ---*/
		{
			struct ifi_info *ifi, *ifinext;
			for(ifi=ifihead;ifi!=NULL;ifi=ifinext)
			{
				if(ifi->ifi_addr!=NULL)
					free(ifi->ifi_addr);
				if(ifi->ifi_brdaddr!=NULL)
					free(ifi->ifi_brdaddr);
				if(ifi->ifi_dstaddr!=NULL)
					free(ifi->ifi_dstaddr);
				ifinext=ifi->ifi_next; /*unable to fetch ifi_next after free()*/
				free(ifi); /*the ifi_info{} itself*/
			}

/* Ashumeow */
/* NOTE: this program will be again re-updated soon. */
				

