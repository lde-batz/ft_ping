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

# include <netdb.h>

# include <signal.h>

# include <errno.h>

# define PING_PKT_LEN 64
# define TTL_MAX 64

struct s_ping	*ping;

typedef struct	s_parsing
{
	int			i;
	int			j;
	char		c;
	char		finish;
	const char	*msg;
}				t_parsing;

typedef struct	s_rtt_list
{
	double				rtt;
	struct s_rtt_list	*next;
}				t_rtt_list;

typedef struct	s_time
{
	struct timeval	start;
	struct timeval	send;
}				t_time;

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

typedef struct	s_settings
{
	int		count;
	char	timestamp;
	char	numeric;
	char	quiet;
	char	verbose;
	int		deadline;
}				t_settings;

typedef struct	s_ping_pkt
{
	struct icmphdr	hdr;
	char			msg[PING_PKT_LEN - sizeof(struct icmphdr)];
}				t_ping_pkt;

typedef struct	s_ping
{
	char				*hostname;
	char				*host;

	int					fd;
	char				loop;
	struct sockaddr_in	s_addr;
	size_t				addrlen;
	char				sending;

	t_ping_pkt			pkt;

	t_settings			set;

	t_ping_stat			stats;

	t_time				time;

	t_rtt_list			*rtt_list;

	struct addrinfo		hints;
	struct addrinfo		*res;
}				t_ping;

void			parsing(int argc, char **argv);

void			get_ip_by_hostname(void);
char			*get_hostname_by_ip(struct in_addr ip);

void			start_ping(void);

void			loop_ping(void);
void			check_loop_out(int deadline);

void			send_ping(void);

void			recv_pkt(void);

void			stat_calcul(void);
void			update_stats(double rtt);
double			rtt_calcul(struct timeval *time);

unsigned short	checksum_calcul(void *pkt, int len);

void			sig_int(int signum);
void			sig_alrm(int signum);

void			exit_help(void);
void			exit_ping(const char *msg);

char			*ft_strslice(char *str, int slice);
double			*ft_atof_strict(char *str, double *nb);
double			ft_sqrtf(const double m);

#endif
