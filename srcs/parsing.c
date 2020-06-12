#include "ft_ping.h"

char	*parcing_str_value(int argc, char **argv, t_parsing *p, char c)
{
	char	*str_value;

	if (argv[p->i][p->j + 1])
	{
		if (!(str_value = ft_strslice(argv[p->i], p->j)))
			exit_ping("Error malloc()");
	}
	else
	{
		if (++p->i >= argc)
		{
			printf("ping : option requires an argument -- %c\n", c);
			exit_help();
		}
		if (!(str_value =
					(char *)malloc(sizeof(char) * (ft_strlen(argv[p->i] + 1)))))
			exit_ping("Error malloc()");
		str_value = ft_strcpy(str_value, argv[p->i]);
	}
	return (str_value);
}

void	parsing_opt_c(int argc, char **argv, t_parsing *p)
{
	double	value;
	char	*str_value;

	p->finish = 1;
	str_value = parcing_str_value(argc, argv, p, 'c');
	if (!ft_atof_strict(str_value, &value) || value < 1)
	{
		if (ft_atoi(argv[p->i], 0) < 1)
			exit_ping("ping: bad number of packets to transmit.");
		else
			exit_help();
	}
	else
		ping->set.count = (int)value;
	free(str_value);
}

void	parsing_opt_w(int argc, char **argv, t_parsing *p)
{
	double	value;
	char	*str_value;

	p->finish = 1;
	str_value = parcing_str_value(argc, argv, p, 'w');
	if (!ft_atof_strict(str_value, &value)
					|| (value > -1 && value < 1))
		ping->set.deadline = 0;
	else if (value <= -1)
		exit_ping("ping: bad wait time.");
	else
		ping->set.deadline = (int)value;
	free(str_value);
}

void	parsing_options(int argc, char **argv, t_parsing *p)
{
	p->j = 0;
	p->finish = 0;
	while (!p->finish && argv[p->i][++p->j])
	{
		p->c = argv[p->i][p->j];
		if (p->c == 'c')
			parsing_opt_c(argc, argv, p);
		else if (p->c == 'D')
			ping->set.timestamp = 1;
		else if (p->c == 'h')
			exit_help();
		else if (p->c == 'n')
			ping->set.numeric = 1;
		else if (p->c == 'q')
			ping->set.quiet = 1;
		else if (p->c == 'v')
			ping->set.verbose = 1;
		else if (p->c == 'w')
			parsing_opt_w(argc, argv, p);
		else
		{
			printf("ping : option invalide -- '%c'\n", p->c);
			exit_help();
		}
	}
}

void	parsing(int argc, char **argv)
{
	t_parsing p;

	p.i = 0;
	while (++p.i < argc)
	{
		if (argv[p.i][0] == '-')
			parsing_options(argc, argv, &p);
		else if (ping->hostname == NULL)
			ping->hostname = argv[p.i];
		else
		{
			printf("Error! Only one destination\n");
			exit_help();
		}
	}
	if (ping->hostname == NULL)
		exit_help();
	get_ip_by_hostname();
}
