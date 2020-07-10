/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:05:12 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:05:18 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	check_loop_out(int deadline)
{
	if (g_ping->stats.msg_cnt == 0)
		return ;
	if (g_ping->set.count)
	{
		if (g_ping->set.count <= g_ping->stats.msg_cnt)
			sig_int(SIGINT);
	}
	if (g_ping->set.deadline)
	{
		if (g_ping->set.deadline + deadline <= g_ping->stats.msg_cnt)
			sig_int(SIGINT);
	}
	return ;
}

void	loop_ping(void)
{
	gettimeofday(&g_ping->time.start, NULL);
	sig_alrm(SIGALRM);
	while (42)
	{
		recv_pkt();
		check_loop_out(1);
	}
}
