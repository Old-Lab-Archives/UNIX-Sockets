/* Internet checksum algorithm */
uint16_t in_cksum(uint16_t * addr, int len)
{
	int nleft = len;
	uint32_t sum = 0;
	uint16_t *w = addr;
	uint16_t answer = 0;
	/* algorithm is simple which is done using 32-bit accumulator where sequential 16-bit words are added */
	while(nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}
	/* swapping off odd bytes if so! */
	if(nleft == 1)
	{
		*(unsigned char *) (&answer) = *(unsigned char *) w;
		sum += answer;
	}
	/* calculating from top 16-bits to low 16-bits */
	sum = (sum >> 16) + (sum & 0Xffff); /* adding 16 to low 16 */
	sum += (sum >> 16); /* adding carry */
	answer = ~sum; /* Truncating to 16 bits */
	return(answer);
}
