/* Determination of host byte order */
#include "meow.h"
int main(int argc, char **argv)
{
	union
	{
		short s;
		char c[sizeof(short)];
	}
	un;
	un.s = 0X0102;
	printf("%s: ", CPU_VENDOR_OS);
	if(sizeof(short)==2)
	{
		if(un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if(un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	}
	else
		printf("sizeof(short) = %d\n", sizeof(short));
	exit(0);
}

/*--- Notes: The string CPU_VENDOR_OS is determined by the GNU autoconf program where the software is configured
and it identifies the CPU type, vendor and OS.
Here is an example using FreeBSD:-
freebsd4 % byteorder
i386-unknown-freebsd4.8: little-endian
*/
