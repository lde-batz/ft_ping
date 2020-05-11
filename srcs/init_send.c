
#include "ft_ping.h"

int		get_fqdn(const char *host_name, struct sockaddr *s_addr)
{
	int				ret;
	struct addrinfo	*res;
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_CANONNAME;

	ret = getaddrinfo(host_name, NULL, &hints, &res);
	if (ret != 0)
	{
		perror("Error getaddrinfo()");
		return 0;
	}
//    freeaddrinfo(res);
	*s_addr = *res->ai_addr;
	return 1;
}

char	init_s_addr(t_ping *ping, struct sockaddr_in *s_addr,
					char *fqdn, char s_ip[16])
{
	s_addr->sin_family = AF_INET;
	if (inet_pton(AF_INET, ping->dst, &s_addr->sin_addr.s_addr) < 1)
	{
		if (get_fqdn(ping->dst, (struct sockaddr *)s_addr) == 0)
			return 0;
		*fqdn = 1;
	}
	else
	{
		printf("Addresse à ping chargée: %i\n", s_addr->sin_addr.s_addr);
	}
	if (inet_ntop(AF_INET, &s_addr->sin_addr.s_addr, s_ip, 16) == NULL)
	{
		perror("Error inet_ntop()");
		return 0;
	}
	return 1;
}

void	init_settings_options(t_ping *ping, int *count, int *interval)
{
	*interval = 1000;
	*count = -1;
	if (ping->options.i.actived)
		*interval = ping->options.i.value * 1000;
	if (ping->options.c.actived)
		*count = ping->options.c.value;
}
