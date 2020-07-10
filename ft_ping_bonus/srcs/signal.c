/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:06:15 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:06:16 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	sig_int(int signum)
{
	if (signum != SIGINT)
		return ;
	stat_calcul();
	printf("\n--- %s ping statistics ---\n", g_ping->hostname);
	printf("%i packets transmitted, %i received,",
			g_ping->stats.msg_cnt, g_ping->stats.recv_cnt);
	printf(" %.0f%c packet loss, time %.0fms\n",
			g_ping->stats.pck_loss, '%', g_ping->stats.time);
	if (g_ping->stats.pck_loss < 100 && !g_ping->set.verbose)
	{
		printf("rtt min/avg/max/mdev = %g/%g/%g/%.3f ms\n", g_ping->stats.min,
				g_ping->stats.avg, g_ping->stats.max, g_ping->stats.mdev);
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
