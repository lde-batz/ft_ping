
#include "ft_ping.h"

void	print_help(void)
{
	printf("Usage: ft_ping [-Dfhv] [-c count] [-i interval] ");
	printf("[-w deadline] destination\n");
	exit(EXIT_SUCCESS);
}
