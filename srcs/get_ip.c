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
