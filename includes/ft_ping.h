#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <stdio.h>
# include <sys/time.h>

# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

# include <arpa/inet.h>

# include <netdb.h>		// getaddrinfo()

# include <signal.h>	// signal ctrl + C

# include <errno.h>

# define PING_PKT_LEN 64

char			loop_ping;

typedef struct	s_parcing
{
	int			i;
	int			j;
	char		c;
	char		finish;
	const char	*msg;
}				t_parcing;

typedef struct	s_rtt_list
{
	double				rtt;
	struct s_rtt_list	*next;
}				t_rtt_list;

typedef struct	s_ping_stat
{
	int		msg_cnt;
	int		recv_cnt;
	double	pck_loss;
	double	time;
	double	min;
	double	avg;
	double	max;
	double	mdev;
}				t_ping_stat;

typedef struct	s_ping_pkt
{
	struct icmphdr	hdr;
	char			msg[PING_PKT_LEN - sizeof(struct icmphdr)];
}				t_ping_pkt;

typedef struct	s_options_value
{
	char	actived;
	double	value;
}				t_options_value;

typedef struct	s_options
{
	t_options_value c;
	char			d;
	char			f;
	t_options_value	i;
	char			v;
	t_options_value	w;
}				t_options;

typedef struct	s_ping
{
	t_options	options;
	char		*dst;
}				t_ping;

void			print_help();
void			parsing(int argc, char **argv, t_ping *ping);
int				send_ping(t_ping *ping);

void			init_settings_options(t_ping *ping, int *count, int *interval);
char			init_s_addr(t_ping *ping, struct sockaddr_in *s_addr,
							char *fqdn, char s_ip[16]);

void			free_double_char(char **ptr);

double			*ft_atof_strict(char *str, double *nb);
double			ft_sqrtf(const double m);

#endif
