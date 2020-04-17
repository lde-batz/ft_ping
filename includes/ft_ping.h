#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <stdio.h>
# include <sys/time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <arpa/inet.h>

# include <netdb.h> //getaddrinfo()

# include <errno.h>


#define PING_PKT_LEN 64 



typedef struct  s_ping_pkt
{
    struct icmphdr hdr;
    char    msg[PING_PKT_LEN - sizeof(struct icmphdr)];
}               t_ping_pkt;

typedef struct  s_options_value
{
    char actived;
    double value;
}               t_options_value;

typedef struct  s_options
{
    t_options_value c;
    char            d;
    char            f;
    t_options_value i;
    char            v;
    t_options_value w;
}               t_options;

typedef struct  s_ping
{
    t_options   options;
    char        *dst;
}               t_ping;

void    printHelp();
void    parsing(int argc, char **argv, t_ping *ping);
int     send_ping(t_ping *ping);

void    freeDoubleChar(char **ptr);

int    *ft_atoi_strict(char *str, int *nb);
double  *ft_atof_strict(char *str, double *nb);
int         ft_arraystrlen(char **array);


#endif
