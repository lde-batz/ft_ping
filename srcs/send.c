

#include "ft_ping.h"


unsigned short  checksum_calcul(void *pkt, int len)
{
    unsigned short  *buf = pkt;
    unsigned int    sum = 0;
    unsigned short  res;

    while (len > 1)
    {
        sum += *buf++;  // somme de tous les octets du packet
        len -= 2;       // -2 car int = 2 * short
    }
    if (len == 1)
        sum += *(unsigned char *)buf; // -1 car int = 4 * char
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    res = ~sum;
    return res;
}

void    time_wait(struct timeval *time, int waiting_time)
{
    struct timeval wait;
    long int       time_usec;

    if (waiting_time <= 0)
        return ;

    time_usec = (time->tv_usec + (waiting_time % 1000) * 1000) % 1000000;
    if (time_usec < time->tv_usec)
        time->tv_sec++;
    gettimeofday(&wait, NULL);
    while (wait.tv_sec - time->tv_sec < waiting_time / 1000 || wait.tv_usec < time_usec)
        gettimeofday(&wait, NULL);
//    printf("rtt : %li  |  %li\n", rtt->tv_sec, rtt->tv_usec);
//    printf("wait: %li  |  %li\n", wait.tv_sec, wait.tv_usec);
}

t_rtt_list  *new_rtt(t_rtt_list *rtt_list, double rtt)
{
    t_rtt_list *new;

    new = (t_rtt_list*)malloc(sizeof(t_rtt_list));
    new->rtt = rtt;
    new->next = rtt_list;
    return new;
}

double     rtt_calcul(struct timeval *time)
{
    double rtt;
    struct timeval time_rtt;

    gettimeofday(&time_rtt, NULL);
    rtt = (time_rtt.tv_usec + (time_rtt.tv_sec - time->tv_sec) * 1000000) - time->tv_usec;
    rtt /= 1000;
    return rtt;
}

void    stat_calcul(t_ping_stat *stat, struct timeval *time, t_rtt_list *rtt_list)
{
    t_rtt_list *list_rtt = rtt_list;

    stat->time = rtt_calcul(time);
    stat->pck_loss = 100 - ((stat->recv_cnt / stat->msg_cnt) * 100);
    while (list_rtt)
    {
        list_rtt->rtt = list_rtt->rtt - stat->avg;
        stat->mdev += ft_powf(list_rtt->rtt, 2);
        list_rtt = list_rtt->next;
    }
    stat->mdev /= stat->msg_cnt;
    stat->mdev = ft_sqrtf(stat->mdev);
}

char    check_loop_out(t_ping *ping, t_ping_stat *stat)
{
    int interval;

    if (ping->options.c.actived)
        if ((int)ping->options.c.value <= stat->msg_cnt)
            return 0;
    if (ping->options.w.actived)
    {
        interval = 1000;
        if (ping->options.i.actived)
            interval = ping->options.i.value * 1000;
        if (ping->options.w.value * 1000 < (stat->msg_cnt + 1) * interval)
            return 0;
    }
    return 1;
}


int     send_ping(t_ping *ping)
{
    int i;
    int fd;
    int ttl_value = 64;
    char    s_ip[16];
    char    fqdn = 0;
    char    sent = 1;
    int     msg_cnt_lim;
    int     interval;
    double  rtt;

    struct sockaddr_in s_addr;

    struct timeval      time;
    struct timeval      time_start;
    struct timeval      opt_d;

    t_ping_pkt  ping_pkt;
    t_ping_stat stat;
    t_rtt_list  *rtt_list = NULL;

    stat.msg_cnt = 0;
    stat.recv_cnt = 0;
    stat.min = 0;
    stat.avg = 0;
    stat.max = 0;
    stat.mdev = 0;

    init_settings_options(ping, &msg_cnt_lim, &interval);

// Chargement de l'addresse à ping
    if (init_s_addr(ping, &s_addr, &fqdn, s_ip) == 0)
        return -1;
    printf("PING %s (%s) 56(84) bytes of data.\n", ping->dst, s_ip);
    if (getuid() != 0)
    {
        if (ping->options.i.actived && ping->options.i.actived < 0.2)
            printf("ping: cannot flood; minimal interval allowed for user is 200ms\n");
        else
            printf("Error user: only root is permitted\n");
        return -1;
    }
    
// Chargement du socket
    fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd < 0)
    {
        perror("Error socket()");
        return -1;
    }
    else
        printf("Socket changé sur le file descriptor %i\n", fd);

// Chargement des options du socket    
    if (setsockopt(fd, SOL_IP, IP_TTL, &ttl_value, sizeof(ttl_value)) != 0)
    {
        perror("Error setsockopt()");
        return -1;
    }
    else
        printf("Options du socket chargé\n");
    
// Chargement du paquet
    while (loop_ping && check_loop_out(ping, &stat))
    {
        sent = 1;
        
        ft_bzero(&ping_pkt, sizeof(ping_pkt));

        ping_pkt.hdr.type = ICMP_ECHO;
        ping_pkt.hdr.un.echo.id = getpid();
        // Remplissage du msg avec des caractère osef
        i = 0;

        while ((unsigned long)i++ < sizeof(ping_pkt.msg) - 1)
            ping_pkt.msg[i] = i + '0';
        ping_pkt.msg[i] = 0;

        ping_pkt.hdr.un.echo.sequence = stat.msg_cnt++;
        ping_pkt.hdr.checksum = checksum_calcul(&ping_pkt, sizeof(ping_pkt));
    //    printf("Parquet chargé\n");

        if (stat.msg_cnt > 1)
            time_wait(&time, interval);
        else
            gettimeofday(&time_start, NULL);
        gettimeofday(&time, NULL);
    // Envoie du ping
        if (sendto(fd, &ping_pkt, sizeof(ping_pkt), 0,
                    (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) <= 0)
        {
            perror("Error sento()");
            sent = 0;
        }
        else if (ping->options.f)
            ft_putchar('.');

    // Reception du ping
        struct msghdr msg;
        struct iovec    iov[1];
        ft_memset(&msg, '\0', sizeof(msg));
		ft_bzero(&ping_pkt, sizeof(ping_pkt));
		iov[0].iov_base = (char *)&ping_pkt;
        iov[0].iov_len = sizeof(ping_pkt);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
		msg.msg_name = &s_addr;
		msg.msg_namelen = sizeof(s_addr);
        
        int ret_msg;

//        ret_msg = recvmsg(fd, &msg, MSG_PEEK);
        ret_msg = recvmsg(fd, &msg, 0);
        perror("recvmsg()");
//        printf("ret_msg = %i\n", ret_msg);
        if (ret_msg > 0)
        {
//            printf("Ping recu!\n");
            if (sent)
            {
                stat.recv_cnt++;
                if (ping->options.f)
                    ft_putchar('\b');
                else
                {
                    if (ping_pkt.hdr.type == 69 && ping_pkt.hdr.code == 0)
                    {
                        rtt = rtt_calcul(&time);
                        rtt_list = new_rtt(rtt_list, rtt);
                        if (stat.msg_cnt > 1)
                        {
                            if (stat.min > rtt)
                                stat.min = rtt;
                            if (stat.max < rtt)
                                stat.max = rtt;
                            stat.avg = ((stat.avg * (stat.msg_cnt - 1)) + rtt) / stat.msg_cnt;
                        }
                        else
                        {
                            stat.min = rtt;
                            stat.avg = rtt;
                            stat.max = rtt;
                        }
                        
                        if (ping->options.d)
                        {
                            gettimeofday(&opt_d, NULL);
                            printf("[%li.%li] ", opt_d.tv_sec, opt_d.tv_usec);
                        }
                        if (fqdn)
                        {
                            printf("%i bytes from %s (%s): icmp_seq=%i ttl=%i time=%g ms\n",
                                    PING_PKT_LEN, ping->dst, s_ip, stat.msg_cnt, ttl_value, rtt);
                        }
                        else
                        {
                            printf("%i bytes from %s: icmp_seq=%i ttl=%i time=%g ms\n",
                                    PING_PKT_LEN, ping->dst, stat.msg_cnt, ttl_value, rtt);
                        }
                    }
                    else
                        printf("Error..Packet received with ICMP type %i code %i\n",
                                ping_pkt.hdr.type, ping_pkt.hdr.code);
                    
                }
                
            }
        }
    }

    stat_calcul(&stat, &time_start, rtt_list);
    printf("\n--- %s ping statistics ---\n", ping->dst);
    printf("%i packets transmitted, %i received, %.0f%c packet loss, time %.0fms\n",
            stat.msg_cnt, stat.recv_cnt, stat.pck_loss, '%', stat.time);
    if (stat.pck_loss < 100)
        printf("rtt min/avg/max/mdev = %g/%g/%g/%.3f ms\n", stat.min, stat.avg, stat.max, stat.mdev);
    return 1;
}