/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ip.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:05:00 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:05:04 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_ip_by_hostname(void)
{
	char				host[INET_ADDRSTRLEN];
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct sockaddr_in	*host_sockaddr;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	if (getaddrinfo(g_ping->hostname, NULL, &hints, &res) != 0)
	{
		printf("ping: %s: name or service not known\n", g_ping->hostname);
		exit_ping(NULL);
	}
	host_sockaddr = (struct sockaddr_in *)res->ai_addr;
	if (inet_ntop(AF_INET, &host_sockaddr->sin_addr, host, INET_ADDRSTRLEN)
																	== NULL)
		exit_ping("Error inet_ntop(): host couldn't be changed");
	g_ping->host = ft_strdup(host);
	freeaddrinfo(res);
}
