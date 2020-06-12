#include "ft_ping.h"

void	check_loop_out(int deadline)
{
	if (ping->stats.msg_cnt == 0)
		return ;
	if (ping->set.count)
	{
		if (ping->set.count <= ping->stats.msg_cnt)
			sig_int(SIGINT);
	}
	if (ping->set.deadline)
	{
		if (ping->set.deadline + deadline <= ping->stats.msg_cnt)
			sig_int(SIGINT);
	}
	return ;
}

void	loop_ping(void)
{
	gettimeofday(&ping->time.start, NULL);
	sig_alrm(SIGALRM);
	while (42)
	{
		recv_pkt();
		check_loop_out(1);
	}
}
