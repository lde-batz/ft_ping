

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

int     get_fqdn(const char *host_name, struct sockaddr *s_addr)
{

    int             ret;
	struct addrinfo *res;
	struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_RAW;
    hints.ai_flags = AI_CANONNAME;

    ret = getaddrinfo(host_name, NULL, &hints, &res);
    if (ret != 0)
    {
        perror("Error getaddrinfo()");
        return 0;
    }
//    freeaddrinfo(res);
    *s_addr = *res->ai_addr;
    return 1;
}


int     send_ping(t_ping *ping)
{
    int i;
    int msg_count = 0;
    int fd;
    int ttl_value = 64;
    int addr_len;
    char    s_ip[16];

    struct sockaddr_in s_addr;
    struct sockaddr_in r_addr;

    t_ping_pkt  ping_pkt;

// Chargement de l'addresse à ping
    s_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ping->dst, &s_addr.sin_addr.s_addr) < 1)
    {
        perror("Error inet_pton()");
        if (get_fqdn(ping->dst, (struct sockaddr *)&s_addr) == 0)
            return -1;
    }
    else
        printf("Addresse à ping chargée: %i\n", s_addr.sin_addr.s_addr);

    if (inet_ntop(AF_INET, &s_addr.sin_addr.s_addr, s_ip, 16) == NULL)
    {
        perror("Error inet_ntop()");
        return -1;
    }
    printf("PING %s (%s) 56(84) bytes of data.\n", ping->dst, s_ip);
    if (getuid() == 0)
    {
//        if (ping->options.i  && ping->options.i < 0.2)
    }
    
// Chargement du socket
    fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd < 0)
    {
//        if (errno == EACCES && ping->options.i < 0.2)
        if (errno == EACCES)
        {
            printf("ping: cannot flood; minimal interval allowed for user is 200ms\n");
        } 
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
    ft_bzero(&ping_pkt, sizeof(ping_pkt));
    ping_pkt.hdr.type = ICMP_ECHO;
    ping_pkt.hdr.un.echo.id = getpid();
    // Remplissage du msg avec des caractère osef
    i = 0;
    while ((unsigned long)i++ < sizeof(ping_pkt.msg) - 1)
        ping_pkt.msg[i] = i + '0';
    // inutile
    ping_pkt.msg[i] = 0;
    ping_pkt.hdr.un.echo.sequence = msg_count;
    ping_pkt.hdr.checksum = checksum_calcul(&ping_pkt, sizeof(ping_pkt));
    printf("Parquet chargé\n");


// Envoie du ping
    if (sendto(fd, &ping_pkt, sizeof(ping_pkt), 0,
                (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) <= 0)
    {
        perror("Error Sento()");
        return -1;
    }
    else
        printf("ping envoyé\n");

// Reception du ping
    if (1)
    {
        struct msghdr msg;
        struct iovec    iov[1];
        ft_memset(&msg, '\0', sizeof(msg));
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
        iov[0].iov_base = (char *)&ping_pkt;
        iov[0].iov_len = sizeof(ping_pkt);
        
        int ret_msg;
        ret_msg = recvmsg(fd, &msg, 0);
        printf("ret_msg = %i\n", ret_msg);
        if (ret_msg > 0)
            printf("Ping recu!\n");
        else
        {
            perror("Error recvmsg()");
            return -1;
        }
    }
    else
    {
        addr_len = sizeof(r_addr);
        recvfrom(fd, &ping_pkt, sizeof(ping_pkt), 0,
                    (struct sockaddr *)&r_addr, (socklen_t *)&addr_len);
        perror("Reception du ping");
    }
    return 1;
}