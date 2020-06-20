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
	char				*host;
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct sockaddr_in	*host_sockaddr;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	if (getaddrinfo(ping->hostname, NULL, &hints, &res) != 0)
	{
		printf("ping: %s: name or service not known\n", ping->hostname);
		exit_ping(NULL);
	}
	host_sockaddr = (struct sockaddr_in *)res->ai_addr;
	host = inet_ntoa(host_sockaddr->sin_addr);
	ping->host = ft_strdup(host);
	freeaddrinfo(res);
}

char	*get_hostname_by_ip(struct in_addr ip)
{
	struct hostent *hostent;

	hostent = gethostbyaddr(&ip, sizeof(ip), AF_INET);
	if (hostent)
		return (hostent->h_name);
	return (inet_ntoa(ip));
}
