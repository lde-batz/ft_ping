/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arraystrlen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-batz <lde-batz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 16:08:41 by lde-batz          #+#    #+#             */
/*   Updated: 2020/06/20 16:08:44 by lde-batz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_arraystrlen(char **array)
{
	int i;

	i = -1;
	if (!array)
		return (-1);
	while (array[++i])
		;
	return (i);
}
