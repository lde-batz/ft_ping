#include "ft_ping.h"

double	rtt_calcul(struct timeval *time)
{
	double			rtt;
	struct timeval	time_rtt;

	gettimeofday(&time_rtt, NULL);
	rtt = (time_rtt.tv_usec +
			(time_rtt.tv_sec - time->tv_sec) * 1000000) - time->tv_usec;
	rtt /= 1000;
	return (rtt);
}

double	ping_duration_calcul(struct timeval *time)
{
	double	rtt;

	rtt = (ping->time.send.tv_usec +
			(ping->time.send.tv_sec - time->tv_sec) * 1000000) - time->tv_usec;
	rtt /= 1000;
	return (rtt);
}

void	stat_calcul(void)
{
	t_rtt_list	*free_list_rtt;

	ping->stats.time = ping_duration_calcul(&ping->time.start);
	ping->stats.pck_loss = 100 -
						((ping->stats.recv_cnt / ping->stats.msg_cnt) * 100);

	if (!ping->set.verbose)
	{
		free_list_rtt = ping->rtt_list;
		while (ping->rtt_list)
		{
			ping->rtt_list->rtt = ping->rtt_list->rtt - ping->stats.avg;
			ping->stats.mdev += ft_powf(ping->rtt_list->rtt, 2);
			free_list_rtt = ping->rtt_list;
			ping->rtt_list = ping->rtt_list->next;
			free(free_list_rtt);
		}
		ping->stats.mdev /= ping->stats.msg_cnt;
		ping->stats.mdev = ft_sqrtf(ping->stats.mdev);
	}
}

void	update_stats(double rtt)
{
	if (ping->stats.msg_cnt > 1)
	{
		if (ping->stats.min > rtt)
			ping->stats.min = rtt;
		if (ping->stats.max < rtt)
			ping->stats.max = rtt;
		ping->stats.avg = ((ping->stats.avg *
							(ping->stats.msg_cnt - 1)) + rtt) /
							ping->stats.msg_cnt;
	}
	else
	{
		ping->stats.min = rtt;
		ping->stats.avg = rtt;
		ping->stats.max = rtt;
	}
}
