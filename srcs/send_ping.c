/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:06:05 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:06:08 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	init_pkt(void)
{
	int i;

	ft_bzero(&g_ping->pkt, sizeof(g_ping->pkt));
	g_ping->pkt.hdr.type = ICMP_ECHO;
	g_ping->pkt.hdr.un.echo.id = getpid();
	g_ping->pkt.hdr.un.echo.sequence = g_ping->stats.msg_cnt++;
	i = 0;
	while ((unsigned long)i++ < sizeof(g_ping->pkt.msg) - 2)
		g_ping->pkt.msg[i] = i + '0';
	g_ping->pkt.msg[i] = 0;
	g_ping->pkt.hdr.checksum =
		checksum_calcul(&g_ping->pkt, sizeof(g_ping->pkt));
}

void	print_send_verbose(void)
{
	struct in_addr	in_addr;
	struct timeval	time;

	if (g_ping->set.timestamp)
	{
		gettimeofday(&time, NULL);
		printf("[%li.%li] ", time.tv_sec, time.tv_usec);
	}
	if (inet_pton(AF_INET, g_ping->hostname, &in_addr.s_addr) < 1 &&
		inet_pton(AF_INET, g_ping->host, &in_addr.s_addr) == 1 &&
		!g_ping->set.numeric)
	{
		printf("%i bytes to %s (%s): ICMP type=%i code=%i\n",
			PING_PKT_LEN, get_hostname_by_ip(in_addr), g_ping->host,
			g_ping->pkt.hdr.type, g_ping->pkt.hdr.code);
	}
	else
	{
		printf("%i bytes to %s: ICMP type=%i code=%i\n", PING_PKT_LEN,
			g_ping->host, g_ping->pkt.hdr.type, g_ping->pkt.hdr.code);
	}
}

void	send_ping(void)
{
	check_loop_out(0);
	init_pkt();
	gettimeofday(&g_ping->time.send, NULL);
	g_ping->sending = 1;
	if (sendto(g_ping->fd, &g_ping->pkt, sizeof(g_ping->pkt), 0,
			(struct sockaddr *)&g_ping->s_addr, g_ping->addrlen) < 0)
	{
		perror("Error sendto()");
		g_ping->sending = 0;
	}
	else if (g_ping->set.verbose)
	{
		print_send_verbose();
	}
}
