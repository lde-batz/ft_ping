/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:05:45 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:05:47 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_addr(struct sockaddr *s_addr)
{
	*s_addr = *g_ping->res->ai_addr;
	g_ping->addrlen = g_ping->res->ai_addrlen;
}

int		open_socket(void)
{
	int	fd;
	int ttl;

	ttl = TTL_MAX;
	ft_bzero(&g_ping->hints, sizeof(g_ping->hints));
	g_ping->hints.ai_family = AF_INET;
	g_ping->hints.ai_socktype = SOCK_RAW;
	g_ping->hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(g_ping->host, 0, &g_ping->hints, &g_ping->res) < 0)
		exit_ping("Error getaddrinfo()");
	get_addr((struct sockaddr *)&g_ping->s_addr);
	fd = socket(g_ping->res->ai_family, g_ping->res->ai_socktype,
										g_ping->res->ai_protocol);
	if (fd < 0)
		exit_ping("Error socket()");
	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
		exit_ping("Error setsockopt()");
	return (fd);
}

void	start_ping(void)
{
	printf("PING %s (%s) 56(84) bytes of data.\n",
		g_ping->hostname, g_ping->host);
	if (getuid() != 0)
		exit_ping("Error user: only root is permitted");
	g_ping->fd = open_socket();
}
