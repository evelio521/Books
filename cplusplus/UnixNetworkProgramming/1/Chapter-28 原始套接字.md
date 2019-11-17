Chapter-28 原始套接字

----

### 基础功能

1. 原始套接字，进程可以读写ICMPv4\IGMPv4和ICMPv6等分组。
2. 进程可以读写内核不处理协议字段的IPv4数据包。
3. 进程可以使用IP_HDRINGCL套接字选项自行构造IPv4首部。
4. 原始套接字的bind和connect比较少使用。

### 原始套接字的创建 

1. int socket = socket(AF_INET, SOCK_RAW, protocol);
2. const int on = 1; setsockopt(socket, IPPROTO_IP, IP_HDRINCL, &on, sizeif(on));
3. 原始套接字可以使用bind函数。
4. 原始套接字可以使用connect函数。

### 原始套接字的输出

1. 普通的输出通过调用sendto 或sendmsg并指定目的IP地址完成，已连接可以使用write writev或send。
2. 内核会对超出外出接口的MTU的原始分组执行分片。
3. IPV6_CHECKSUM 是独有的，对于ICMPv6。

### 原始套接字输入

1. 接收到的UDP分组和TCP分组绝不传递任何原始套接字，如果进程想要读取TCP或UDP的数据包，那么必须在数据链路层完成。
2. 大多数的ICMP分组子内核处理完其中的ICMP消息后，传递到原始套接字。
3. 所有的IGMP分组在内核完成处理其中的IGMP消息后传递到原始套接字。
4. 内核不认识其协议字段的所有IP数据包传递到原始套接字。
5. 如果某个数据包以片段形式到达，那么在它的所有片段均到达且重组出该数据报之前，不传递任何片段分组到原始套接字。
6. 如果创建这个原始套接字时指出了非0的协议参数，那么接到的数据包协议字段必须匹配该值，否则数据报不递送到这个套接字。
7. 如果原始套接字已经bind某个IP，接收到的数据报目的地IP必须匹配这个地址。
8. 如果原始套接字已由connect 调用指定某个外地ip，那么接收到数据报的源ip必须匹配这个地址。

```c++
// ICMPv6 Type Filtering
#include <netinet/icmp6.h>
void ICMP6_FILTER_SETPASSALL (struct icmp6_filter *filt);
void ICMP6_FILTER_SETBLOCKALL (struct icmp6_filter *filt);
void ICMP6_FILTER_SETPASS (int msgtype, struct icmp6_filter *filt);
void ICMP6_FILTER_SETBLOCK (int msgtype, struct icmp6_filter *filt);
int ICMP6_FILTER_WILLPASS (int msgtype, const struct icmp6_filter *filt); int ICMP6_FILTER_WILLBLOCK (int msgtype, const struct icmp6_filter *filt);

struct icmp6_filter myfilt;
fd = socket (AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
ICMP6_FILTER_SETBLOCKALL (&myfilt);
ICMP6_FILTER_SETPASS (ND_ROUTER_ADVERT, &myfilt);
Setsockopt (fd, IPPROTO_ICMPV6, ICMP6_FILTER. &myfilt, sizeof (myfilt));
```



### ping

####ping.h

```c++
#include	"unp.h"
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>
#include	<netinet/ip_icmp.h>

#define	BUFSIZE		1500

			/* globals */
char	 sendbuf[BUFSIZE];

int		 datalen;			/* # bytes of data following ICMP header */
char	*host;
int		 nsent;				/* add 1 for each sendto() */
pid_t	 pid;				/* our PID */
int		 sockfd;
int		 verbose;

			/* function prototypes */
void	 init_v6(void);
void	 proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void	 proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void	 send_v4(void);
void	 send_v6(void);
void	 readloop(void);
void	 sig_alrm(int);
void	 tv_sub(struct timeval *, struct timeval *);

struct proto {
  void	 (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
  void	 (*fsend)(void);
  void	 (*finit)(void);
  struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
  struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
  socklen_t	    salen;		/* length of sockaddr{}s */
  int	   	    icmpproto;	/* IPPROTO_xxx value for ICMP */
} *pr;

#ifdef	IPV6

#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>

#endif

```



####main.c

```c++
#include	"ping.h"

struct proto	proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };

#ifdef	IPV6
struct proto	proto_v6 = { proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif

int	datalen = 56;		/* data that goes with ICMP echo request */

int
main(int argc, char **argv)
{
	int				c;
	struct addrinfo	*ai;
	char *h;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbose++;
			break;

		case '?':
			err_quit("unrecognized option: %c", c);
		}
	}

	if (optind != argc-1)
		err_quit("usage: ping [ -v ] <hostname>");
	host = argv[optind];

	pid = getpid() & 0xffff;	/* ICMP ID field is 16 bits */
	Signal(SIGALRM, sig_alrm);

	ai = Host_serv(host, NULL, 0, 0);

	h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
	printf("PING %s (%s): %d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h,
			h, datalen);

		/* 4initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
#ifdef	IPV6
	} else if (ai->ai_family == AF_INET6) {
		pr = &proto_v6;
		if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)
								 ai->ai_addr)->sin6_addr)))
			err_quit("cannot ping IPv4-mapped IPv6 address");
#endif
	} else
		err_quit("unknown address family %d", ai->ai_family);

	pr->sasend = ai->ai_addr;
	pr->sarecv = Calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	readloop();

	exit(0);
}

```

#### readloop.c

```c++
#include	"ping.h"

void
readloop(void)
{
	int				size;
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t			n;
	struct timeval	tval;

	sockfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());		/* don't need special permissions any more */
	if (pr->finit)
		(*pr->finit)();

	size = 60 * 1024;		/* OK if setsockopt fails */
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	sig_alrm(SIGALRM);		/* send first packet */

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for ( ; ; ) {
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvmsg error");
		}

		Gettimeofday(&tval, NULL);
		(*pr->fproc)(recvbuf, n, &msg, &tval);
	}
}

```

#### tv_sub.c

```c++
#include	"unp.h"

void
tv_sub(struct timeval *out, struct timeval *in)
{
	if ( (out->tv_usec -= in->tv_usec) < 0) {	/* out -= in */
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

```

#### proc_v4.c

```c++
#include	"ping.h"

void
proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	int				hlen1, icmplen;
	double			rtt;
	struct ip		*ip;
	struct icmp		*icmp;
	struct timeval	*tvsend;

	ip = (struct ip *) ptr;		/* start of IP header */
	hlen1 = ip->ip_hl << 2;		/* length of IP header */
	if (ip->ip_p != IPPROTO_ICMP)
		return;				/* not ICMP */

	icmp = (struct icmp *) (ptr + hlen1);	/* start of ICMP header */
	if ( (icmplen = len - hlen1) < 8)
		return;				/* malformed packet */

	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		if (icmp->icmp_id != pid)
			return;			/* not a response to our ECHO_REQUEST */
		if (icmplen < 16)
			return;			/* not enough data to use */

		tvsend = (struct timeval *) icmp->icmp_data;
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

		printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
				icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
				icmp->icmp_seq, ip->ip_ttl, rtt);

	} else if (verbose) {
		printf("  %d bytes from %s: type = %d, code = %d\n",
				icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
				icmp->icmp_type, icmp->icmp_code);
	}
}

```

####init_v6.c

```c++
#include "ping.h"

void
init_v6()
{
#ifdef IPV6
	int on = 1;

	if (verbose == 0) {
		/* install a filter that only passes ICMP6_ECHO_REPLY unless verbose */
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilt);
		setsockopt(sockfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
		/* ignore error return; the filter is an optimization */
	}

	/* ignore error returned below; we just won't receive the hop limit */
#ifdef IPV6_RECVHOPLIMIT
	/* RFC 3542 */
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
#else
	/* RFC 2292 */
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_HOPLIMIT, &on, sizeof(on));
#endif
#endif
}

```

#### proc_v6.c

```c++
#include	"ping.h"

void
proc_v6(char *ptr, ssize_t len, struct msghdr *msg, struct timeval* tvrecv)
{
#ifdef	IPV6
	double				rtt;
	struct icmp6_hdr	*icmp6;
	struct timeval		*tvsend;
	struct cmsghdr		*cmsg;
	int					hlim;

	icmp6 = (struct icmp6_hdr *) ptr;
	if (len < 8)
		return;				/* malformed packet */

	if (icmp6->icmp6_type == ICMP6_ECHO_REPLY) {
		if (icmp6->icmp6_id != pid)
			return;			/* not a response to our ECHO_REQUEST */
		if (len < 16)
			return;			/* not enough data to use */

		tvsend = (struct timeval *) (icmp6 + 1);
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

		hlim = -1;
		for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg)) {
			if (cmsg->cmsg_level == IPPROTO_IPV6 &&
				cmsg->cmsg_type == IPV6_HOPLIMIT) {
				hlim = *(u_int32_t *)CMSG_DATA(cmsg);
				break;
			}
		}
		printf("%d bytes from %s: seq=%u, hlim=",
				len, Sock_ntop_host(pr->sarecv, pr->salen),
				icmp6->icmp6_seq);
		if (hlim == -1)
			printf("???");	/* ancillary data missing */
		else
			printf("%d", hlim);
		printf(", rtt=%.3f ms\n", rtt);
	} else if (verbose) {
		printf("  %d bytes from %s: type = %d, code = %d\n",
				len, Sock_ntop_host(pr->sarecv, pr->salen),
				icmp6->icmp6_type, icmp6->icmp6_code);
	}
#endif	/* IPV6 */
}

```

#### sig_alrm.c

```c++
#include	"ping.h"

void
sig_alrm(int signo)
{
	(*pr->fsend)();

	alarm(1);
	return;
}

```

#### send_v4.c

````c++
#include	"ping.h"

void
send_v4(void)
{
	int			len;
	struct icmp	*icmp;

	icmp = (struct icmp *) sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = pid;
	icmp->icmp_seq = nsent++;
	memset(icmp->icmp_data, 0xa5, datalen);	/* fill with pattern */
	Gettimeofday((struct timeval *) icmp->icmp_data, NULL);

	len = 8 + datalen;		/* checksum ICMP header and data */
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

	Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}

````

#### send_v6.c

```c++
#include	"ping.h"

void
send_v6()
{
#ifdef	IPV6
	int					len;
	struct icmp6_hdr	*icmp6;

	icmp6 = (struct icmp6_hdr *) sendbuf;
	icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp6->icmp6_code = 0;
	icmp6->icmp6_id = pid;
	icmp6->icmp6_seq = nsent++;
	memset((icmp6 + 1), 0xa5, datalen);	/* fill with pattern */
	Gettimeofday((struct timeval *) (icmp6 + 1), NULL);

	len = 8 + datalen;		/* 8-byte ICMPv6 header */

	Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
		/* 4kernel calculates and stores checksum for us */
#endif	/* IPV6 */
}

```

#### in_cksum.c

```c++
#include "unp.h"

uint16_t
in_cksum(uint16_t *addr, int len)
{
	int				nleft = len;
	uint32_t		sum = 0;
	uint16_t		*w = addr;
	uint16_t		answer = 0;

	/*
	 * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	 * sequential 16 bit words to it, and at the end, fold back all the
	 * carry bits from the top 16 bits into the lower 16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

		/* 4mop up an odd byte, if necessary */
	if (nleft == 1) {
		*(unsigned char *)(&answer) = *(unsigned char *)w ;
		sum += answer;
	}

		/* 4add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return(answer);
}

```



### traceroute

#### trace.h

```c++
#include	"unp.h"
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>
#include	<netinet/ip_icmp.h>
#include	<netinet/udp.h>

#define	BUFSIZE		1500

struct rec {					/* format of outgoing UDP data */
  u_short	rec_seq;			/* sequence number */
  u_short	rec_ttl;			/* TTL packet left with */
  struct timeval	rec_tv;		/* time packet left */
};

			/* globals */
char	 recvbuf[BUFSIZE];
char	 sendbuf[BUFSIZE];

int		 datalen;			/* # bytes of data following ICMP header */
char	*host;
u_short	 sport, dport;
int		 nsent;				/* add 1 for each sendto() */
pid_t	 pid;				/* our PID */
int		 probe, nprobes;
int		 sendfd, recvfd;	/* send on UDP sock, read on raw ICMP sock */
int		 ttl, max_ttl;
int		 verbose;

			/* function prototypes */
const char	*icmpcode_v4(int);
const char	*icmpcode_v6(int);
int		 recv_v4(int, struct timeval *);
int		 recv_v6(int, struct timeval *);
void	 sig_alrm(int);
void	 traceloop(void);
void	 tv_sub(struct timeval *, struct timeval *);

struct proto {
  const char	*(*icmpcode)(int);
  int	 (*recv)(int, struct timeval *);
  struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
  struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
  struct sockaddr  *salast;	/* last sockaddr{} for receiving */
  struct sockaddr  *sabind;	/* sockaddr{} for binding source port */
  socklen_t   		salen;	/* length of sockaddr{}s */
  int			icmpproto;	/* IPPROTO_xxx value for ICMP */
  int	   ttllevel;		/* setsockopt() level to set TTL */
  int	   ttloptname;		/* setsockopt() name to set TTL */
} *pr;

#ifdef	IPV6

#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>

#endif

```

####main.c

```c++
#include	"trace.h"

struct proto	proto_v4 = { icmpcode_v4, recv_v4, NULL, NULL, NULL, NULL, 0,
							 IPPROTO_ICMP, IPPROTO_IP, IP_TTL };

#ifdef	IPV6
struct proto	proto_v6 = { icmpcode_v6, recv_v6, NULL, NULL, NULL, NULL, 0,
							 IPPROTO_ICMPV6, IPPROTO_IPV6, IPV6_UNICAST_HOPS };
#endif

int		datalen = sizeof(struct rec);	/* defaults */
int		max_ttl = 30;
int		nprobes = 3;
u_short	dport = 32768 + 666;

int
main(int argc, char **argv)
{
	int				c;
	struct addrinfo	*ai;
	char *h;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "m:v")) != -1) {
		switch (c) {
		case 'm':
			if ( (max_ttl = atoi(optarg)) <= 1)
				err_quit("invalid -m value");
			break;

		case 'v':
			verbose++;
			break;

		case '?':
			err_quit("unrecognized option: %c", c);
		}
	}

	if (optind != argc-1)
		err_quit("usage: traceroute [ -m <maxttl> -v ] <hostname>");
	host = argv[optind];

	pid = getpid();
	Signal(SIGALRM, sig_alrm);

	ai = Host_serv(host, NULL, 0, 0);

	h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
	printf("traceroute to %s (%s): %d hops max, %d data bytes\n",
		   ai->ai_canonname ? ai->ai_canonname : h,
		   h, max_ttl, datalen);

		/* initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
#ifdef	IPV6
	} else if (ai->ai_family == AF_INET6) {
		pr = &proto_v6;
		if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr)))
			err_quit("cannot traceroute IPv4-mapped IPv6 address");
#endif
	} else
		err_quit("unknown address family %d", ai->ai_family);

	pr->sasend = ai->ai_addr;		/* contains destination address */
	pr->sarecv = Calloc(1, ai->ai_addrlen);
	pr->salast = Calloc(1, ai->ai_addrlen);
	pr->sabind = Calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	traceloop();

	exit(0);
}

```

#### traceloop.c

```c++
#include	"trace.h"

void
traceloop(void)
{
	int					seq, code, done;
	double				rtt;
	struct rec			*rec;
	struct timeval		tvrecv;

	recvfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());		/* don't need special permissions anymore */

#ifdef	IPV6
	if (pr->sasend->sa_family == AF_INET6 && verbose == 0) {
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &myfilt);
		setsockopt(recvfd, IPPROTO_IPV6, ICMP6_FILTER,
					&myfilt, sizeof(myfilt));
	}
#endif

	sendfd = Socket(pr->sasend->sa_family, SOCK_DGRAM, 0);

	pr->sabind->sa_family = pr->sasend->sa_family;
	sport = (getpid() & 0xffff) | 0x8000;	/* our source UDP port # */
	sock_set_port(pr->sabind, pr->salen, htons(sport));
	Bind(sendfd, pr->sabind, pr->salen);

	sig_alrm(SIGALRM);

	seq = 0;
	done = 0;
	for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
		Setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int));
		bzero(pr->salast, pr->salen);

		printf("%2d ", ttl);
		fflush(stdout);

		for (probe = 0; probe < nprobes; probe++) {
			rec = (struct rec *) sendbuf;
			rec->rec_seq = ++seq;
			rec->rec_ttl = ttl;
			Gettimeofday(&rec->rec_tv, NULL);

			sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
			Sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);

			if ( (code = (*pr->recv)(seq, &tvrecv)) == -3)
				printf(" *");		/* timeout, no reply */
			else {
				char	str[NI_MAXHOST];

				if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
					if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str),
									NULL, 0, 0) == 0)
						printf(" %s (%s)", str,
								Sock_ntop_host(pr->sarecv, pr->salen));
					else
						printf(" %s",
								Sock_ntop_host(pr->sarecv, pr->salen));
					memcpy(pr->salast, pr->sarecv, pr->salen);
				}
				tv_sub(&tvrecv, &rec->rec_tv);
				rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
				printf("  %.3f ms", rtt);

				if (code == -1)		/* port unreachable; at destination */
					done++;
				else if (code >= 0)
					printf(" (ICMP %s)", (*pr->icmpcode)(code));
			}
			fflush(stdout);
		}
		printf("\n");
	}
}

```

#### recv_v4.c

```c++
#include	"trace.h"

extern int gotalarm;

/*
 * Return: -3 on timeout
 *		   -2 on ICMP time exceeded in transit (caller keeps going)
 *		   -1 on ICMP port unreachable (caller is done)
 *		 >= 0 return value is some other ICMP unreachable code
 */

int
recv_v4(int seq, struct timeval *tv)
{
	int				hlen1, hlen2, icmplen, ret;
	socklen_t		len;
	ssize_t			n;
	struct ip		*ip, *hip;
	struct icmp		*icmp;
	struct udphdr	*udp;

	gotalarm = 0;
	alarm(3);
	for ( ; ; ) {
		if (gotalarm)
			return(-3);		/* alarm expired */
		len = pr->salen;
		n = recvfrom(recvfd, recvbuf, sizeof(recvbuf), 0, pr->sarecv, &len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvfrom error");
		}

		ip = (struct ip *) recvbuf;	/* start of IP header */
		hlen1 = ip->ip_hl << 2;		/* length of IP header */
	
		icmp = (struct icmp *) (recvbuf + hlen1); /* start of ICMP header */
		if ( (icmplen = n - hlen1) < 8)
			continue;				/* not enough to look at ICMP header */
	
		if (icmp->icmp_type == ICMP_TIMXCEED &&
			icmp->icmp_code == ICMP_TIMXCEED_INTRANS) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq)) {
				ret = -2;		/* we hit an intermediate router */
				break;
			}

		} else if (icmp->icmp_type == ICMP_UNREACH) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			if (hip->ip_p == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq)) {
				if (icmp->icmp_code == ICMP_UNREACH_PORT)
					ret = -1;	/* have reached destination */
				else
					ret = icmp->icmp_code;	/* 0, 1, 2, ... */
				break;
			}
		}
		if (verbose) {
			printf(" (from %s: type = %d, code = %d)\n",
					Sock_ntop_host(pr->sarecv, pr->salen),
					icmp->icmp_type, icmp->icmp_code);
		}
		/* Some other ICMP error, recvfrom() again */
	}
	alarm(0);					/* don't leave alarm running */
	Gettimeofday(tv, NULL);		/* get time of packet arrival */
	return(ret);
}

```

#### recv_v6.c

```c++
#include	"trace.h"

extern int gotalarm;

/*
 * Return: -3 on timeout
 *		   -2 on ICMP time exceeded in transit (caller keeps going)
 *		   -1 on ICMP port unreachable (caller is done)
 *		 >= 0 return value is some other ICMP unreachable code
 */

int
recv_v6(int seq, struct timeval *tv)
{
#ifdef	IPV6
	int					hlen2, icmp6len, ret;
	ssize_t				n;
	socklen_t			len;
	struct ip6_hdr		*hip6;
	struct icmp6_hdr	*icmp6;
	struct udphdr		*udp;

	gotalarm = 0;
	alarm(3);
	for ( ; ; ) {
		if (gotalarm)
			return(-3);		/* alarm expired */
		len = pr->salen;
		n = recvfrom(recvfd, recvbuf, sizeof(recvbuf), 0, pr->sarecv, &len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvfrom error");
		}

		icmp6 = (struct icmp6_hdr *) recvbuf; /* ICMP header */
		if ( ( icmp6len = n ) < 8)
			continue;				/* not enough to look at ICMP header */
	
		if (icmp6->icmp6_type == ICMP6_TIME_EXCEEDED &&
			icmp6->icmp6_code == ICMP6_TIME_EXCEED_TRANSIT) {
			if (icmp6len < 8 + sizeof(struct ip6_hdr) + 4)
				continue;			/* not enough data to look at inner header */

			hip6 = (struct ip6_hdr *) (recvbuf + 8);
			hlen2 = sizeof(struct ip6_hdr);
			udp = (struct udphdr *) (recvbuf + 8 + hlen2);
			if (hip6->ip6_nxt == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq))
				ret = -2;		/* we hit an intermediate router */
				break;

		} else if (icmp6->icmp6_type == ICMP6_DST_UNREACH) {
			if (icmp6len < 8 + sizeof(struct ip6_hdr) + 4)
				continue;			/* not enough data to look at inner header */

			hip6 = (struct ip6_hdr *) (recvbuf + 8);
			hlen2 = sizeof(struct ip6_hdr);
			udp = (struct udphdr *) (recvbuf + 8 + hlen2);
			if (hip6->ip6_nxt == IPPROTO_UDP &&
				udp->uh_sport == htons(sport) &&
				udp->uh_dport == htons(dport + seq)) {
				if (icmp6->icmp6_code == ICMP6_DST_UNREACH_NOPORT)
					ret = -1;	/* have reached destination */
				else
					ret = icmp6->icmp6_code;	/* 0, 1, 2, ... */
				break;
			}
		} else if (verbose) {
			printf(" (from %s: type = %d, code = %d)\n",
					Sock_ntop_host(pr->sarecv, pr->salen),
					icmp6->icmp6_type, icmp6->icmp6_code);
		}
		/* Some other ICMP error, recvfrom() again */
	}
	alarm(0);					/* don't leave alarm running */
	Gettimeofday(tv, NULL);		/* get time of packet arrival */
	return(ret);
#endif
}

```

#### sig_alrm.c

```c++
#include	"trace.h"

int gotalarm;

void
sig_alrm(int signo)
{
	gotalarm = 1;	/* set flag to note that alarm occurred */
	return;			/* and interrupt the recvfrom() */
}

```

#### icmlcode_v4.c

```c++
#include	"trace.h"

const char *
icmpcode_v4(int code)
{
	static char errbuf[100];
	switch (code) {
	case  0:	return("network unreachable");
	case  1:	return("host unreachable");
	case  2:	return("protocol unreachable");
	case  3:	return("port unreachable");
	case  4:	return("fragmentation required but DF bit set");
	case  5:	return("source route failed");
	case  6:	return("destination network unknown");
	case  7:	return("destination host unknown");
	case  8:	return("source host isolated (obsolete)");
	case  9:	return("destination network administratively prohibited");
	case 10:	return("destination host administratively prohibited");
	case 11:	return("network unreachable for TOS");
	case 12:	return("host unreachable for TOS");
	case 13:	return("communication administratively prohibited by filtering");
	case 14:	return("host recedence violation");
	case 15:	return("precedence cutoff in effect");
	default:	sprintf(errbuf, "[unknown code %d]", code);
				return errbuf;
	}
}

```

#### icmpcode_v6.c

```c++
#include	"trace.h"

const char *
icmpcode_v6(int code)
{
#ifdef	IPV6
	static char errbuf[100];
	switch (code) {
	case  ICMP6_DST_UNREACH_NOROUTE:
		return("no route to host");
	case  ICMP6_DST_UNREACH_ADMIN:
		return("administratively prohibited");
	case  ICMP6_DST_UNREACH_NOTNEIGHBOR:
		return("not a neighbor");
	case  ICMP6_DST_UNREACH_ADDR:
		return("address unreachable");
	case  ICMP6_DST_UNREACH_NOPORT:
		return("port unreachable");
	default:
		sprintf(errbuf, "[unknown code %d]", code);
		return errbuf;
	}
#endif
}

```

