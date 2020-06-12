#include "ft_ping.h"

void	get_addr(struct sockaddr *s_addr)
{
	*s_addr = *ping->res->ai_addr;
	ping->addrlen = ping->res->ai_addrlen;
}

int		open_socket(void)
{
	int	fd;
	int ttl;

	ttl = TTL_MAX;
	ft_bzero(&ping->hints, sizeof(ping->hints));
	ping->hints.ai_family = AF_INET;
	ping->hints.ai_socktype = SOCK_RAW;
	ping->hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(ping->host, 0, &ping->hints, &ping->res) < 0)
		exit_ping("Error getaddrinfo()");
	get_addr((struct sockaddr *)&ping->s_addr);
	fd = socket(ping->res->ai_family, ping->res->ai_socktype,
										ping->res->ai_protocol);
	if (fd < 0)
		exit_ping("Error socket()");
	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
		exit_ping("Error setsockopt()");
	return (fd);
}

void	start_ping(void)
{
	printf("PING %s (%s) 56(84) bytes of data.\n", ping->hostname, ping->host);
	if (getuid() != 0)
		exit_ping("Error user: only root is permitted");
	ping->fd = open_socket();
}
