#include "ft_ping.h"

void	sig_int(int signum)
{
	if (signum != SIGINT)
		return ;
	stat_calcul();
	printf("\n--- %s ping statistics ---\n", ping->hostname);
	printf("%i packets transmitted, %i received,",
			ping->stats.msg_cnt, ping->stats.recv_cnt);
	printf(" %.0f%c packet loss, time %.0fms\n",
			ping->stats.pck_loss, '%', ping->stats.time);
	if (ping->stats.pck_loss < 100)
	{
		printf("rtt min/avg/max/mdev = %g/%g/%g/%.3f ms\n", ping->stats.min,
				ping->stats.avg, ping->stats.max, ping->stats.mdev);
	}
	exit_ping(NULL);
}

void	sig_alrm(int signum)
{
	if (signum != SIGALRM)
		return ;
	send_ping();
	alarm(1);
}
