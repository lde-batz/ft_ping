
#include "ft_ping.h"

void	free_double_char(char **ptr)
{
	int i;

	if (ptr)
	{
		i = -1;
		while (ptr[++i])
			free(ptr[i]);
		free(ptr);
	}
}
