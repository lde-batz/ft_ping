#include "ft_ping.h"

void	init_ping(void)
{
	if (!(ping = (t_ping*)malloc(sizeof(t_ping))))
		exit_ping("Error malloc of ping");
	ft_bzero(ping, sizeof(ping));
	ping->hostname = NULL;
	ping->host = NULL;
	ping->fd = -1;
	ping->loop = 1;
	ping->res = NULL;
	ping->rtt_list = NULL;
}

int		main(int argc, char **argv)
{
	if (argc <= 1)
		exit_help();
	init_ping();
	parsing(argc, argv);
	signal(SIGINT, sig_int);
	signal(SIGALRM, sig_alrm);
	start_ping();
	loop_ping();
	exit_ping(NULL);
	return (0);
}
