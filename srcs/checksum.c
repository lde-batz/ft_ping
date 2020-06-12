#include "ft_ping.h"

unsigned short	checksum_calcul(void *pkt, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	res;

	buf = pkt;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	res = ~sum;
	return (res);
}
