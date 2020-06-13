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

void	print_send_verbose(void)
{
	struct in_addr	in_addr;
	struct timeval	time;

	if (ping->set.timestamp)
	{
		gettimeofday(&time, NULL);
		printf("[%li.%li] ", time.tv_sec, time.tv_usec);
	}
	if (inet_pton(AF_INET, ping->hostname, &in_addr.s_addr) < 1 &&
		inet_pton(AF_INET, ping->host, &in_addr.s_addr) == 1 &&
		!ping->set.numeric)
	{
		printf("%i bytes to %s (%s): ICMP type=%i code=%i\n",
			PING_PKT_LEN, get_hostname_by_ip(in_addr), ping->host,
			ping->pkt.hdr.type, ping->pkt.hdr.code);
	}
	else
	{
		printf("%i bytes to %s: ICMP type=%i code=%i\n",
			PING_PKT_LEN, ping->host, ping->pkt.hdr.type, ping->pkt.hdr.code);
	}
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
	else if (ping->set.verbose)
	{
		print_send_verbose();
	}
}
