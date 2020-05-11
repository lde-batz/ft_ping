
#include "ft_ping.h"

void	parsing_options_with_value(int argc, char **argv,
									t_ping *ping, t_parcing *p)
{
	double value;

	if (argv[p->i][p->j + 1])
	{
		printf("%s\n", p->msg);
		exit(EXIT_FAILURE);
	}
	else
	{
		p->finish = 1;
		if (++p->i >= argc)
			print_help();
		if (p->c == 'c')
		{
			if (!ft_atof_strict(argv[p->i], &value) || value < 1)
			{
				printf("%s\n", p->msg);
				print_help();
			}
			else
			{
				ping->options.c.actived = 1;
				ping->options.c.value = (int)value;
			}
		}
		else if (p->c == 'i')
		{
			if (!ft_atof_strict(argv[p->i], &value) || value < 0)
			{
				printf("%s\n", p->msg);
				print_help();
			}
			else
			{
				ping->options.i.actived = 1;
				ping->options.i.value = value;
			}
		}
		else if (p->c == 'w')
		{
			if (!ft_atof_strict(argv[p->i], &value)
						|| (value > -1 && value < 1))
				ping->options.w.actived = 0;
			else if (value <= -1)
			{
				printf("%s\n", p->msg);
				print_help();
			}
			else
			{
				ping->options.w.actived = 1;
				ping->options.w.value = value;
			}
		}
	}
}

void	parsing_options(int argc, char **argv, t_ping *ping, t_parcing *p)
{
	p->j = 0;
	p->finish = 0;
	while (!p->finish && argv[p->i][++p->j])
	{
		p->c = argv[p->i][p->j];
		if (p->c == 'c')
		{
			p->msg = "ping: bad number of packets to transmit.";
			parsing_options_with_value(argc, argv, ping, p);
		}
		else if (p->c == 'D')
			ping->options.d = 1;
		else if (p->c == 'f')
		{
			ping->options.f = 1;
			if (ping->options.i.actived == 0)
			{
				ping->options.i.actived = 1;
				ping->options.i.value = 0;
			}
		}
		else if (p->c == 'i')
		{
			p->msg = "ping: bad timing interval.";
			parsing_options_with_value(argc, argv, ping, p);
		}
		else if (p->c == 'h')
			print_help();
		else if (p->c == 'v')
			ping->options.v = 1;
		else if (p->c == 'w')
		{
			p->msg = "ping: bad wait time.";
			parsing_options_with_value(argc, argv, ping, p);
		}
		else
		{
			printf("ping : option invalide -- '%c'\n", p->c);
			print_help();
		}
	}
}

/*
int     checkIP(char *ip) {
    char    **arrayIP = ft_strsplit(ip, '.');
    int     i = -1;
    int     arrayLen = ft_arraystrlen(arrayIP);
    int     octet;

    while (++i < arrayLen) {
        octet = -1;
        if (!ft_atoi_strict(arrayIP[i], &octet) || octet < 0) {
            freeDoubleChar(arrayIP);
            return 0;
        }
    }
    freeDoubleChar(arrayIP);
    return 1;
}
*/

void	parsing(int argc, char **argv, t_ping *ping)
{
	t_parcing p;

	p.i = 0;
	while (++p.i < argc)
	{
		if (argv[p.i][0] == '-')
			parsing_options(argc, argv, ping, &p);
		else if (ping->dst == NULL)
			ping->dst = argv[p.i];
		else
		{
			printf("Error! Only one destination\n");
			print_help();
		}
	}
	if (ping->dst == NULL)
		print_help();
}
