/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:06:23 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:06:27 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_rtt_list	*new_rtt(t_rtt_list *rtt_list, double rtt)
{
	t_rtt_list *new;

	new = (t_rtt_list*)malloc(sizeof(t_rtt_list));
	new->rtt = rtt;
	new->next = rtt_list;
	return (new);
}

double		rtt_calcul(struct timeval *time)
{
	double			rtt;
	struct timeval	time_rtt;

	gettimeofday(&time_rtt, NULL);
	rtt = (time_rtt.tv_usec +
			(time_rtt.tv_sec - time->tv_sec) * 1000000) - time->tv_usec;
	rtt /= 1000;
	return (rtt);
}

double		ping_duration_calcul(struct timeval *time)
{
	double	rtt;

	rtt = (g_ping->time.send.tv_usec + (g_ping->time.send.tv_sec -
		time->tv_sec) * 1000000) - time->tv_usec;
	rtt /= 1000;
	return (rtt);
}

void		stat_calcul(void)
{
	t_rtt_list	*free_list_rtt;

	g_ping->stats.time = ping_duration_calcul(&g_ping->time.start);
	g_ping->stats.pck_loss = 100 -
		((g_ping->stats.recv_cnt / g_ping->stats.msg_cnt) * 100);
	if (!g_ping->set.verbose)
	{
		free_list_rtt = g_ping->rtt_list;
		while (g_ping->rtt_list)
		{
			g_ping->rtt_list->rtt = g_ping->rtt_list->rtt - g_ping->stats.avg;
			g_ping->stats.mdev += ft_powf(g_ping->rtt_list->rtt, 2);
			free_list_rtt = g_ping->rtt_list;
			g_ping->rtt_list = g_ping->rtt_list->next;
			free(free_list_rtt);
		}
		g_ping->stats.mdev /= g_ping->stats.msg_cnt;
		g_ping->stats.mdev = ft_sqrtf(g_ping->stats.mdev);
	}
}

void		update_stats(double *rtt)
{
	g_ping->stats.recv_cnt++;
	*rtt = rtt_calcul(&g_ping->time.send);
	g_ping->rtt_list = new_rtt(g_ping->rtt_list, *rtt);
	if (g_ping->stats.msg_cnt > 1)
	{
		if (g_ping->stats.min > *rtt)
			g_ping->stats.min = *rtt;
		if (g_ping->stats.max < *rtt)
			g_ping->stats.max = *rtt;
		g_ping->stats.avg = ((g_ping->stats.avg *
							(g_ping->stats.msg_cnt - 1)) + *rtt) /
							g_ping->stats.msg_cnt;
	}
	else
	{
		g_ping->stats.min = *rtt;
		g_ping->stats.avg = *rtt;
		g_ping->stats.max = *rtt;
	}
}
