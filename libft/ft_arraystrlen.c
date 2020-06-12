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
