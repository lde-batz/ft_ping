/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:05:56 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:05:58 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void		print_recv(double rtt, int ttl, int recv_len)
{
	struct in_addr	in_addr;
	struct timeval	time;

	if (g_ping->set.timestamp)
	{
		gettimeofday(&time, NULL);
		printf("[%li.%li] ", time.tv_sec, time.tv_usec);
	}
	if (inet_pton(AF_INET, g_ping->hostname, &in_addr) < 1 &&
		inet_pton(AF_INET, g_ping->host, &in_addr) == 1 &&
		!g_ping->set.numeric)
	{
		printf("%i bytes from %s (%s): icmp_seq=%i ttl=%i time=%g ms\n",
				recv_len, g_ping->hostname, g_ping->host,
				g_ping->stats.msg_cnt, ttl, rtt);
	}
	else
	{
		printf("%i bytes from %s: icmp_seq=%i ttl=%i time=%g ms\n",
				recv_len, g_ping->host, g_ping->stats.msg_cnt, ttl, rtt);
	}
}

void		print_recv_verbose(int recv_len, struct icmphdr *icmp)
{
	struct in_addr	in_addr;
	struct timeval	time;

	g_ping->stats.recv_cnt++;
	if (!g_ping->set.quiet)
	{
		if (g_ping->set.timestamp)
		{
			gettimeofday(&time, NULL);
			printf("[%li.%li] ", time.tv_sec, time.tv_usec);
		}
		if (inet_pton(AF_INET, g_ping->hostname, &in_addr.s_addr) < 1 &&
			inet_pton(AF_INET, g_ping->host, &in_addr.s_addr) == 1 &&
			!g_ping->set.numeric)
		{
			printf("%i bytes from %s (%s): ICMP type=%i code=%i\n",
				recv_len, g_ping->hostname, g_ping->host,
				icmp->type, icmp->code);
		}
		else
		{
			printf("%i bytes from %s: ICMP type=%i code=%i\n", recv_len,
				g_ping->host, icmp->type, icmp->code);
		}
	}
}

void		pkt_received(void *ptr, int recv_len)
{
	double			rtt;
	struct iphdr	*ip;
	struct icmphdr	*icmp;

	ip = ptr;
	icmp = ptr + 20;
	if (g_ping->sending)
	{
		if (ip->protocol != IPPROTO_ICMP
					|| icmp->un.echo.sequence != g_ping->stats.msg_cnt - 1)
			return ;
		if (icmp->type == 0 && icmp->code == 0
										&& !g_ping->set.verbose)
		{
			update_stats(&rtt);
			if (!g_ping->set.quiet)
				print_recv(rtt, ip->ttl, recv_len);
		}
		else if (icmp->type == 0 && icmp->code == 0
										&& g_ping->set.verbose)
			print_recv_verbose(recv_len, icmp);
		else if (icmp->type != 8)
			printf("Error..Packet received with ICMP type %i code %i\n",
					icmp->type, icmp->code);
	}
}

void		recv_pkt(void)
{
	int				ret_recv;
	struct msghdr	msg;
	struct iovec	iov[1];

	ft_memset(&msg, '\0', sizeof(msg));
	ft_bzero(&g_ping->pkt, sizeof(g_ping->pkt));
	iov[0].iov_base = (char *)&g_ping->pkt;
	iov[0].iov_len = sizeof(g_ping->pkt);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = &g_ping->res->ai_addr;
	msg.msg_namelen = sizeof(g_ping->res->ai_addrlen);
	msg.msg_flags = 0;
	alarm(1);
	ret_recv = recvmsg(g_ping->fd, &msg, 0);
	if (ret_recv > 0)
		pkt_received(msg.msg_iov[0].iov_base, ret_recv);
}
