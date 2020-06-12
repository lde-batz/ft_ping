#include "ft_ping.h"

void	init_pkt(void)
{
	int i;

	ft_bzero(&ping->pkt, sizeof(ping->pkt));
	ping->pkt.hdr.type = ICMP_ECHO;
	ping->pkt.hdr.un.echo.id = getpid();
	ping->pkt.hdr.un.echo.sequence = ping->stats.msg_cnt++;
	i = 0;
	while ((unsigned long)i++ < sizeof(ping->pkt.msg) - 2)
		ping->pkt.msg[i] = i + '0';
	ping->pkt.msg[i] = 0;
	ping->pkt.hdr.checksum = checksum_calcul(&ping->pkt, sizeof(ping->pkt));
}

void	send_ping(void)
{
	check_loop_out(0);
	init_pkt();
	gettimeofday(&ping->time.send, NULL);
	ping->sending = 1;
	if (sendto(ping->fd, &ping->pkt, sizeof(ping->pkt), 0,
			(struct sockaddr *)&ping->s_addr, ping->addrlen) < 0)
	{
		perror("Error sendto()");
		ping->sending = 0;
	}
}
