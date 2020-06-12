#include "ft_ping.h"

void	exit_help(void)
{
	printf("Usage: ft_ping [-Dhnqv] [-c count] ");
	printf("[-w deadline] destination\n");
	exit(EXIT_SUCCESS);
}

void	exit_ping(const char *msg)
{
	if (msg)
		ft_putendl(msg);
	if (ping)
	{
		if (ping->host)
			free(ping->host);
		if (ping->res)
			freeaddrinfo(ping->res);
		if (ping->fd != -1)
			close(ping->fd);
		free(ping);
	}
	exit(EXIT_FAILURE);
}
