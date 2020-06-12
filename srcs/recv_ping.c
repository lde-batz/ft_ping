#include "ft_ping.h"

t_rtt_list	*new_rtt(t_rtt_list *rtt_list, double rtt)
{
	t_rtt_list *new;

	new = (t_rtt_list*)malloc(sizeof(t_rtt_list));
	new->rtt = rtt;
	new->next = rtt_list;
	return (new);
}

char		*get_hostname_by_ip(struct in_addr ip)
{
	struct hostent *hostent;

	hostent = gethostbyaddr(&ip, sizeof(ip), AF_INET);
	if (hostent)
		return (hostent->h_name);
	return (inet_ntoa(ip));
}

void		print_recv(double rtt, int ttl)
{
	struct in_addr	in_addr;
	struct timeval	time;

	if (ping->set.timestamp)
	{
		gettimeofday(&time, NULL);
		printf("[%li.%li] ", time.tv_sec, time.tv_usec);
	}
	if (inet_pton(AF_INET, ping->hostname, &in_addr.s_addr) < 1 &&
		inet_pton(AF_INET, ping->host, &in_addr.s_addr) == 1 &&
		!ping->set.numeric)
	{
		printf("%i bytes from %s (%s): icmp_seq=%i ttl=%i time=%g ms\n",
				PING_PKT_LEN, get_hostname_by_ip(in_addr), ping->host,
				ping->stats.msg_cnt, ttl, rtt);
	}
	else
	{
		printf("%i bytes from %s: icmp_seq=%i ttl=%i time=%g ms\n",
				PING_PKT_LEN, ping->host, ping->stats.msg_cnt, ttl, rtt);
	}
}

void		pkt_received(char *ptr)
{
	double		rtt;
	struct ip	*ip;

	ip = (struct ip *)ptr;
	if (ping->sending)
	{
		if (ip->ip_p != IPPROTO_ICMP)
			return ;
		if (ping->pkt.hdr.type == 69 && ping->pkt.hdr.code == 0)
		{
			ping->stats.recv_cnt++;
			rtt = rtt_calcul(&ping->time.send);
			ping->rtt_list = new_rtt(ping->rtt_list, rtt);
			update_stats(rtt);
			if (!ping->set.quiet)
				print_recv(rtt, ip->ip_ttl);
		}
		else
			printf("Error..Packet received with ICMP type %i code %i\n",
					ping->pkt.hdr.type, ping->pkt.hdr.code);
	}
}

void		recv_pkt(void)
{
	int				ret_recv;
	struct msghdr	msg;
	struct iovec	iov[1];

	ft_memset(&msg, '\0', sizeof(msg));
	ft_bzero(&ping->pkt, sizeof(ping->pkt));
	iov[0].iov_base = (char *)&ping->pkt;
	iov[0].iov_len = sizeof(ping->pkt);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = &ping->res->ai_addr;
	msg.msg_namelen = sizeof(ping->res->ai_addrlen);
	msg.msg_flags = 0;
	alarm(1);
	ret_recv = recvmsg(ping->fd, &msg, 0);
	if (ret_recv > 0)
		pkt_received(msg.msg_iov[0].iov_base);
}
