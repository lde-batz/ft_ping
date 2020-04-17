


#include "ft_ping.h"

void    initializationPing(t_ping *ping) {
    ft_memset(&ping->options, 0, sizeof(ping->options));
    ping->dst = NULL;
}

int	main(int argc, char **argv)
{
    t_ping  ping;

	if (argc <= 1) {
        printHelp();
    }
    else {
        initializationPing(&ping);
        parsing(argc, argv, &ping);
    }
    send_ping(&ping);
	return (0);
}