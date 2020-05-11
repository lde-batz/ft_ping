/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 10:00:41 by karakvra          #+#    #+#             */
/*   Updated: 2020/05/09 10:06:04 by karakvra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_ping.h"

void	stop_loop()
{
	loop_ping = 0;
}

void	init_ping(t_ping *ping)
{
	ft_memset(&ping->options, 0, sizeof(ping->options));
	ping->dst = NULL;
	loop_ping = 1;
}

int		main(int argc, char **argv)
{
	t_ping	ping;

	if (argc <= 1)
	{
		print_help();
	}
	else
	{
		init_ping(&ping);
		parsing(argc, argv, &ping);
	}
	signal(SIGINT, stop_loop);
	send_ping(&ping);
	return (0);
}
