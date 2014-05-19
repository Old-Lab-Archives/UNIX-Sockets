struct arpreq
{
	struct sockaddr arp_pa; /*protocol address*/
	struct sockaddr arp_ha; /*hardware address*/
	int arp_flags; /*flags*/
};
#define ATF_INUSE 0X01 /*entry in use*/
#define ATF_COM 0X02 /*completed entry (hardware addr valid)*/
#define ATF_PERM 0X04 /*permanent entry*/
#define ATF_PUBL 0X08 /*published entry (respond for other host)*/
