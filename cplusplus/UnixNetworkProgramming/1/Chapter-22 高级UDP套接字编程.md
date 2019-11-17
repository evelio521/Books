Chapter-22 高级UDP套接字编程

----

### 概述

1. TCP是一个字节流协议，使用滑动窗口，因此没有记录边界或发送者数据发送能力超过接受者数据接收能力的问题。
2. UDP有记录边界或发送者数据发送能力超过接受者数据接收能力的问题。

### recvfromflags.c

```c++
/* include recvfrom_flags1 */
#include	"unp.h"
#include	<sys/param.h>		/* ALIGN macro for CMSG_NXTHDR() macro */

ssize_t
recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp)
{
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t			n;

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	struct cmsghdr	*cmptr;
	union {
	  struct cmsghdr	cm;
	  char				control[CMSG_SPACE(sizeof(struct in_addr)) +
								CMSG_SPACE(sizeof(struct unp_in_pktinfo))];
	} control_un;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	msg.msg_flags = 0;
#else
	bzero(&msg, sizeof(msg));	/* make certain msg_accrightslen = 0 */
#endif

	msg.msg_name = sa;
	msg.msg_namelen = *salenptr;
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if ( (n = recvmsg(fd, &msg, *flagsp)) < 0)
		return(n);

	*salenptr = msg.msg_namelen;	/* pass back results */
	if (pktp)
		bzero(pktp, sizeof(struct unp_in_pktinfo));	/* 0.0.0.0, i/f = 0 */
/* end recvfrom_flags1 */

/* include recvfrom_flags2 */
#ifndef	HAVE_MSGHDR_MSG_CONTROL
	*flagsp = 0;					/* pass back results */
	return(n);
#else

	*flagsp = msg.msg_flags;		/* pass back results */
	if (msg.msg_controllen < sizeof(struct cmsghdr) ||
		(msg.msg_flags & MSG_CTRUNC) || pktp == NULL)
		return(n);

	for (cmptr = CMSG_FIRSTHDR(&msg); cmptr != NULL;
		 cmptr = CMSG_NXTHDR(&msg, cmptr)) {

#ifdef	IP_RECVDSTADDR
		if (cmptr->cmsg_level == IPPROTO_IP &&
			cmptr->cmsg_type == IP_RECVDSTADDR) {

			memcpy(&pktp->ipi_addr, CMSG_DATA(cmptr),
				   sizeof(struct in_addr));
			continue;
		}
#endif

#ifdef	IP_RECVIF
		if (cmptr->cmsg_level == IPPROTO_IP &&
			cmptr->cmsg_type == IP_RECVIF) {
			struct sockaddr_dl	*sdl;

			sdl = (struct sockaddr_dl *) CMSG_DATA(cmptr);
			pktp->ipi_ifindex = sdl->sdl_index;
			continue;
		}
#endif
		err_quit("unknown ancillary data, len = %d, level = %d, type = %d",
				 cmptr->cmsg_len, cmptr->cmsg_level, cmptr->cmsg_type);
	}
	return(n);
#endif	/* HAVE_MSGHDR_MSG_CONTROL */
}
/* end recvfrom_flags2 */

ssize_t
Recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp)
{
	ssize_t		n;

	n = recvfrom_flags(fd, ptr, nbytes, flagsp, sa, salenptr, pktp);
	if (n < 0)
		err_quit("recvfrom_flags error");

	return(n);
}

```



### dgechoaddr.c

```c++
#include	"unpifi.h"

#undef	MAXLINE
#define	MAXLINE	20		/* to see datagram truncation */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int						flags;
	const int				on = 1;
	socklen_t				len;
	ssize_t					n;
	char					mesg[MAXLINE], str[INET6_ADDRSTRLEN],
							ifname[IFNAMSIZ];
	struct in_addr			in_zero;
	struct unp_in_pktinfo	pktinfo;

#ifdef	IP_RECVDSTADDR
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on)) < 0)
		err_ret("setsockopt of IP_RECVDSTADDR");
#endif
#ifdef	IP_RECVIF
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0)
		err_ret("setsockopt of IP_RECVIF");
#endif
	bzero(&in_zero, sizeof(struct in_addr));	/* all 0 IPv4 address */

	for ( ; ; ) {
		len = clilen;
		flags = 0;
		n = Recvfrom_flags(sockfd, mesg, MAXLINE, &flags,
						   pcliaddr, &len, &pktinfo);
		printf("%d-byte datagram from %s", n, Sock_ntop(pcliaddr, len));
		if (memcmp(&pktinfo.ipi_addr, &in_zero, sizeof(in_zero)) != 0)
			printf(", to %s", Inet_ntop(AF_INET, &pktinfo.ipi_addr,
										str, sizeof(str)));
		if (pktinfo.ipi_ifindex > 0)
			printf(", recv i/f = %s",
				   If_indextoname(pktinfo.ipi_ifindex, ifname));
#ifdef	MSG_TRUNC
		if (flags & MSG_TRUNC)	printf(" (datagram truncated)");
#endif
#ifdef	MSG_CTRUNC
		if (flags & MSG_CTRUNC)	printf(" (control info truncated)");
#endif
#ifdef	MSG_BCAST
		if (flags & MSG_BCAST)	printf(" (broadcast)");
#endif
#ifdef	MSG_MCAST
		if (flags & MSG_MCAST)	printf(" (multicast)");
#endif
		printf("\n");

		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}

```

### 数据包截断

1. recvmsg在msghdr结构的msg_flags成员上设置MSG_TRUNC标志表示数据包截断，并非所有的系统都实现这种方式。
2. 丢弃超出部分的字节并向应用进程返回MSG_TRUNC标志(POSIX)。
3. 丢弃超出部分字节，但是不告知应用程序。
4. 保留超出部分的字节并在同一套接字上后续的读操作中返回它们。

### UDP的优势

1. UDP支持广播和多播。
2. UDP没有连接的建立和拆除。
3. TCP 连接时间 2* RTT + SPT 
4. UDP 连接时间 RTT + SPT

### TCP 优势

1. 正面确认，丢失分组重传，重复分组检测，给被网络打乱次序的分组排序。
2. 窗口式流量控制。
3. 慢启动和拥塞避免

### UDP使用建议

1. 对于广播和多播使用UDP。
2. 对于简单的请求应答使用UDP。
3. 对于海量数据不应该使用UDP（TFTP 使用UDP传输海量数据，这是个例外）。

###使用UDP在请求应该应用程序的条件

1. 超时和重传，用于处理丢失数据。
2. 序列号，供客户端验证一个应该是否匹配相应的请求。
3. DNS解析器、SNMP代理、TFTP和RPC使用UDP

### dg_cli.c

```c++
#include	"unp.h"

ssize_t	Dg_send_recv(int, const void *, size_t, void *, size_t,
				   const SA *, socklen_t);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	ssize_t	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		n = Dg_send_recv(sockfd, sendline, strlen(sendline),
						 recvline, MAXLINE, pservaddr, servlen);

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}

```



### dg_send_recv.c

```c++
/* include dgsendrecv1 */
#include	"unprtt.h"
#include	<setjmp.h>

#define	RTT_DEBUG

static struct rtt_info   rttinfo;
static int	rttinit = 0;
static struct msghdr	msgsend, msgrecv;	/* assumed init to 0 */
static struct hdr {
  uint32_t	seq;	/* sequence # */
  uint32_t	ts;		/* timestamp when sent */
} sendhdr, recvhdr;

static void	sig_alrm(int signo);
static sigjmp_buf	jmpbuf;

ssize_t
dg_send_recv(int fd, const void *outbuff, size_t outbytes,
			 void *inbuff, size_t inbytes,
			 const SA *destaddr, socklen_t destlen)
{
	ssize_t			n;
	struct iovec	iovsend[2], iovrecv[2];

	if (rttinit == 0) {
		rtt_init(&rttinfo);		/* first time we're called */
		rttinit = 1;
		rtt_d_flag = 1;
	}

	sendhdr.seq++;
	msgsend.msg_name = destaddr;
	msgsend.msg_namelen = destlen;
	msgsend.msg_iov = iovsend;
	msgsend.msg_iovlen = 2;
	iovsend[0].iov_base = &sendhdr;
	iovsend[0].iov_len = sizeof(struct hdr);
	iovsend[1].iov_base = outbuff;
	iovsend[1].iov_len = outbytes;

	msgrecv.msg_name = NULL;
	msgrecv.msg_namelen = 0;
	msgrecv.msg_iov = iovrecv;
	msgrecv.msg_iovlen = 2;
	iovrecv[0].iov_base = &recvhdr;
	iovrecv[0].iov_len = sizeof(struct hdr);
	iovrecv[1].iov_base = inbuff;
	iovrecv[1].iov_len = inbytes;
/* end dgsendrecv1 */

/* include dgsendrecv2 */
	Signal(SIGALRM, sig_alrm);
	rtt_newpack(&rttinfo);		/* initialize for this packet */

sendagain:
#ifdef	RTT_DEBUG
	fprintf(stderr, "send %4d: ", sendhdr.seq);
#endif
	sendhdr.ts = rtt_ts(&rttinfo);
	Sendmsg(fd, &msgsend, 0);

	alarm(rtt_start(&rttinfo));	/* calc timeout value & start timer */
#ifdef	RTT_DEBUG
	rtt_debug(&rttinfo);
#endif

	if (sigsetjmp(jmpbuf, 1) != 0) {
		if (rtt_timeout(&rttinfo) < 0) {
			err_msg("dg_send_recv: no response from server, giving up");
			rttinit = 0;	/* reinit in case we're called again */
			errno = ETIMEDOUT;
			return(-1);
		}
#ifdef	RTT_DEBUG
		err_msg("dg_send_recv: timeout, retransmitting");
#endif
		goto sendagain;
	}

	do {
		n = Recvmsg(fd, &msgrecv, 0);
#ifdef	RTT_DEBUG
		fprintf(stderr, "recv %4d\n", recvhdr.seq);
#endif
	} while (n < sizeof(struct hdr) || recvhdr.seq != sendhdr.seq);

	alarm(0);			/* stop SIGALRM timer */
		/* 4calculate & store new RTT estimator values */
	rtt_stop(&rttinfo, rtt_ts(&rttinfo) - recvhdr.ts);

	return(n - sizeof(struct hdr));	/* return size of received datagram */
}

static void
sig_alrm(int signo)
{
	siglongjmp(jmpbuf, 1);
}
/* end dgsendrecv2 */

ssize_t
Dg_send_recv(int fd, const void *outbuff, size_t outbytes,
			 void *inbuff, size_t inbytes,
			 const SA *destaddr, socklen_t destlen)
{
	ssize_t	n;

	n = dg_send_recv(fd, outbuff, outbytes, inbuff, inbytes,
					 destaddr, destlen);
	if (n < 0)
		err_quit("dg_send_recv error");

	return(n);
}

```



### unprtt.h

```c++
#ifndef	__unp_rtt_h
#define	__unp_rtt_h

#include	"unp.h"

struct rtt_info {
  float		rtt_rtt;	/* most recent measured RTT, in seconds */
  float		rtt_srtt;	/* smoothed RTT estimator, in seconds */
  float		rtt_rttvar;	/* smoothed mean deviation, in seconds */
  float		rtt_rto;	/* current RTO to use, in seconds */
  int		rtt_nrexmt;	/* # times retransmitted: 0, 1, 2, ... */
  uint32_t	rtt_base;	/* # sec since 1/1/1970 at start */
};

#define	RTT_RXTMIN      2	/* min retransmit timeout value, in seconds */
#define	RTT_RXTMAX     60	/* max retransmit timeout value, in seconds */
#define	RTT_MAXNREXMT 	3	/* max # times to retransmit */

				/* function prototypes */
void	 rtt_debug(struct rtt_info *);
void	 rtt_init(struct rtt_info *);
void	 rtt_newpack(struct rtt_info *);
int		 rtt_start(struct rtt_info *);
void	 rtt_stop(struct rtt_info *, uint32_t);
int		 rtt_timeout(struct rtt_info *);
uint32_t rtt_ts(struct rtt_info *);

extern int	rtt_d_flag;	/* can be set to nonzero for addl info */

#endif	/* __unp_rtt_h */

```



### rtt.c

### 

```c++
/* include rtt1 */
#include	"unprtt.h"

int		rtt_d_flag = 0;		/* debug flag; can be set by caller */

/*
 * Calculate the RTO value based on current estimators:
 *		smoothed RTT plus four times the deviation
 */
#define	RTT_RTOCALC(ptr) ((ptr)->rtt_srtt + (4.0 * (ptr)->rtt_rttvar))

static float
rtt_minmax(float rto)
{
	if (rto < RTT_RXTMIN)
		rto = RTT_RXTMIN;
	else if (rto > RTT_RXTMAX)
		rto = RTT_RXTMAX;
	return(rto);
}

void
rtt_init(struct rtt_info *ptr)
{
	struct timeval	tv;

	Gettimeofday(&tv, NULL);
	ptr->rtt_base = tv.tv_sec;		/* # sec since 1/1/1970 at start */

	ptr->rtt_rtt    = 0;
	ptr->rtt_srtt   = 0;
	ptr->rtt_rttvar = 0.75;
	ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
		/* first RTO at (srtt + (4 * rttvar)) = 3 seconds */
}
/* end rtt1 */

/*
 * Return the current timestamp.
 * Our timestamps are 32-bit integers that count milliseconds since
 * rtt_init() was called.
 */

/* include rtt_ts */
uint32_t
rtt_ts(struct rtt_info *ptr)
{
	uint32_t		ts;
	struct timeval	tv;

	Gettimeofday(&tv, NULL);
	ts = ((tv.tv_sec - ptr->rtt_base) * 1000) + (tv.tv_usec / 1000);
	return(ts);
}

void
rtt_newpack(struct rtt_info *ptr)
{
	ptr->rtt_nrexmt = 0;
}

int
rtt_start(struct rtt_info *ptr)
{
	return((int) (ptr->rtt_rto + 0.5));		/* round float to int */
		/* 4return value can be used as: alarm(rtt_start(&foo)) */
}
/* end rtt_ts */

/*
 * A response was received.
 * Stop the timer and update the appropriate values in the structure
 * based on this packet's RTT.  We calculate the RTT, then update the
 * estimators of the RTT and its mean deviation.
 * This function should be called right after turning off the
 * timer with alarm(0), or right after a timeout occurs.
 */

/* include rtt_stop */
void
rtt_stop(struct rtt_info *ptr, uint32_t ms)
{
	double		delta;

	ptr->rtt_rtt = ms / 1000.0;		/* measured RTT in seconds */

	/*
	 * Update our estimators of RTT and mean deviation of RTT.
	 * See Jacobson's SIGCOMM '88 paper, Appendix A, for the details.
	 * We use floating point here for simplicity.
	 */

	delta = ptr->rtt_rtt - ptr->rtt_srtt;
	ptr->rtt_srtt += delta / 8;		/* g = 1/8 */

	if (delta < 0.0)
		delta = -delta;				/* |delta| */

	ptr->rtt_rttvar += (delta - ptr->rtt_rttvar) / 4;	/* h = 1/4 */

	ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
}
/* end rtt_stop */

/*
 * A timeout has occurred.
 * Return -1 if it's time to give up, else return 0.
 */

/* include rtt_timeout */
int
rtt_timeout(struct rtt_info *ptr)
{
	ptr->rtt_rto *= 2;		/* next RTO */

	if (++ptr->rtt_nrexmt > RTT_MAXNREXMT)
		return(-1);			/* time to give up for this packet */
	return(0);
}
/* end rtt_timeout */

/*
 * Print debugging information on stderr, if the "rtt_d_flag" is nonzero.
 */

void
rtt_debug(struct rtt_info *ptr)
{
	if (rtt_d_flag == 0)
		return;

	fprintf(stderr, "rtt = %.3f, srtt = %.3f, rttvar = %.3f, rto = %.3f\n",
			ptr->rtt_rtt, ptr->rtt_srtt, ptr->rtt_rttvar, ptr->rtt_rto);
	fflush(stderr);
}

```



### udpserv03.c

### 

```c++
/* include udpserv1 */
#include	"unpifi.h"

void	mydg_echo(int, SA *, socklen_t, SA *);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	pid_t				pid;
	struct ifi_info		*ifi, *ifihead;
	struct sockaddr_in	*sa, cliaddr, wildaddr;

	for (ifihead = ifi = Get_ifi_info(AF_INET, 1);
		 ifi != NULL; ifi = ifi->ifi_next) {

			/*4bind unicast address */
		sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

		Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		sa = (struct sockaddr_in *) ifi->ifi_addr;
		sa->sin_family = AF_INET;
		sa->sin_port = htons(SERV_PORT);
		Bind(sockfd, (SA *) sa, sizeof(*sa));
		printf("bound %s\n", Sock_ntop((SA *) sa, sizeof(*sa)));

		if ( (pid = Fork()) == 0) {		/* child */
			mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
			exit(0);		/* never executed */
		}
/* end udpserv1 */
/* include udpserv2 */
		if (ifi->ifi_flags & IFF_BROADCAST) {
				/* 4try to bind broadcast address */
			sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
			Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

			sa = (struct sockaddr_in *) ifi->ifi_brdaddr;
			sa->sin_family = AF_INET;
			sa->sin_port = htons(SERV_PORT);
			if (bind(sockfd, (SA *) sa, sizeof(*sa)) < 0) {
				if (errno == EADDRINUSE) {
					printf("EADDRINUSE: %s\n",
						   Sock_ntop((SA *) sa, sizeof(*sa)));
					Close(sockfd);
					continue;
				} else
					err_sys("bind error for %s",
							Sock_ntop((SA *) sa, sizeof(*sa)));
			}
			printf("bound %s\n", Sock_ntop((SA *) sa, sizeof(*sa)));

			if ( (pid = Fork()) == 0) {		/* child */
				mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr),
						  (SA *) sa);
				exit(0);		/* never executed */
			}
		}
	}
/* end udpserv2 */
/* include udpserv3 */
		/* 4bind wildcard address */
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	bzero(&wildaddr, sizeof(wildaddr));
	wildaddr.sin_family = AF_INET;
	wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	wildaddr.sin_port = htons(SERV_PORT);
	Bind(sockfd, (SA *) &wildaddr, sizeof(wildaddr));
	printf("bound %s\n", Sock_ntop((SA *) &wildaddr, sizeof(wildaddr)));

	if ( (pid = Fork()) == 0) {		/* child */
		mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
		exit(0);		/* never executed */
	}
	exit(0);
}
/* end udpserv3 */

/* include mydg_echo */
void
mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, SA *myaddr)
{
	int			n;
	char		mesg[MAXLINE];
	socklen_t	len;

	for ( ; ; ) {
		len = clilen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		printf("child %d, datagram from %s", getpid(),
			   Sock_ntop(pcliaddr, len));
		printf(", to %s\n", Sock_ntop(myaddr, clilen));

		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
/* end mydg_echo */

```



### 并发UDP服务器

1. 第一种UDP服务器比较简单，读入一个客户请求并发送一个应答，与这个客户端就不在相关了。这种情形下，只需要服务器读取客户请求并fork一个子进程来处理这个请求即可。
2. 第二种是UD服务器与多个客户交换数据，对于每一个请求bind一个临时端口，然后使用临时端口处理相应的请求，如TFTP。

