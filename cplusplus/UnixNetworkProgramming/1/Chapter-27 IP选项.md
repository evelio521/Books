Chapter-27 IP选项

----

### IPv4选项(设置IPv4选项使用setsockopt和getsockopt。)

1. NOP:no-operation—单字节选项，典型用途是为某个后续选项落在4字节边界上提供填充。
2. EOL:end-of-list—单字节选项，终止选项的护理。既然各个IP选项 总长度必须为4字节的倍数，因此最后一个有效选项后有可能跟0~3个EOL字节。
3. LSRR:loose source and recordroute(Section8.5ofTCPv1)。
4. SSRR:strict source and record route(Section8.5ofTCPv1)。
5. Timestamp(Section7.4ofTCPv1). 
6. Recordroute(Section7.3ofTCPv1). 
7. Basic security(obsolete). 
8. Extended security(obsolete). 
9. Stream identifier(obsolete). 
10. Router alert—包含该选项的IP数据报要求所有转发路口器都查看其内容。 

### IPv4源路径选项

#### getsockopt格式

| IP1  | NOP  | code | len  | ptr  | IP2  | IP3  | IP4  | IP5  | IP6  | IP7  | IP8  | IP9  | 目的IP |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :----: |
|  4   |  1   |  1   |  1   |  1   |  4   |  4   |  4   |  4   |  4   |  4   |  4   |  4   |   4    |



#### setsockopt格式

| NOP  | code | len  | ptr  | IP1  | IP2  | IP3  | IP4  | IP5  | IP6  | IP7  | IP8  | IP9  | 目的IP |
| :--: | :--: | :--: | :--: | :--: | ---- | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :----: |
|  1   |  1   |  1   |  1   |  4   | 4    |  4   |  4   |  4   |  4   |  4   |  4   |  4   |   4    |

````c++
/* include inet_srcrt_init */
#include	"unp.h"
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>

static u_char	*optr;		/* pointer into options being formed */
static u_char	*lenptr;	/* pointer to length byte in SRR option */
static int		ocnt;		/* count of # addresses */

u_char *
inet_srcrt_init(int type)
{
	optr = Malloc(44);		/* NOP, code, len, ptr, up to 10 addresses */
	bzero(optr, 44);		/* guarantees EOLs at end */
	ocnt = 0;
	*optr++ = IPOPT_NOP;	/* NOP for alignment */
	*optr++ = type ? IPOPT_SSRR : IPOPT_LSRR;
	lenptr = optr++;		/* we fill in length later */
	*optr++ = 4;			/* offset to first address */

	return(optr - 4);		/* pointer for setsockopt() */
}
/* end inet_srcrt_init */

/* include inet_srcrt_add */
int
inet_srcrt_add(char *hostptr)
{
	int					len;
	struct addrinfo		*ai;
	struct sockaddr_in	*sin;

	if (ocnt > 9)
		err_quit("too many source routes with: %s", hostptr);

	ai = Host_serv(hostptr, NULL, AF_INET, 0);
	sin = (struct sockaddr_in *) ai->ai_addr;
	memcpy(optr, &sin->sin_addr, sizeof(struct in_addr));
	freeaddrinfo(ai);

	optr += sizeof(struct in_addr);
	ocnt++;
	len = 3 + (ocnt * sizeof(struct in_addr));
	*lenptr = len;
	return(len + 1);	/* size for setsockopt() */
}
/* end inet_srcrt_add */

/* include inet_srcrt_print */
void
inet_srcrt_print(u_char *ptr, int len)
{
	u_char			c;
	char			str[INET_ADDRSTRLEN];
	struct in_addr	hop1;

	memcpy(&hop1, ptr, sizeof(struct in_addr));
	ptr += sizeof(struct in_addr);

	while ( (c = *ptr++) == IPOPT_NOP)
		;		/* skip any leading NOPs */

	if (c == IPOPT_LSRR)
		printf("received LSRR: ");
	else if (c == IPOPT_SSRR)
		printf("received SSRR: ");
	else {
		printf("received option type %d\n", c);
		return;
	}
	printf("%s ", Inet_ntop(AF_INET, &hop1, str, sizeof(str)));

	len = *ptr++ - sizeof(struct in_addr);	/* subtract dest IP addr */
	ptr++;		/* skip over pointer */
	while (len > 0) {
		printf("%s ", Inet_ntop(AF_INET, ptr, str, sizeof(str)));
		ptr += sizeof(struct in_addr);
		len -= sizeof(struct in_addr);
	}
	printf("\n");
}
/* end inet_srcrt_print */

````



#### tcpcli01.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	struct addrinfo		*ai;

	if (argc < 2)
		err_quit("usage: tcpcli01 [ -[gG] <hostname> ... ] <hostname>");

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "gG")) != -1) {
		switch (c) {
		case 'g':			/* loose source route */
			if (ptr)
				err_quit("can't use both -g and -G");
			ptr = inet_srcrt_init(0);
			break;

		case 'G':			/* strict source route */
			if (ptr)
				err_quit("can't use both -g and -G");
			ptr = inet_srcrt_init(1);
			break;

		case '?':
			err_quit("unrecognized option: %c", c);
		}
	}

	if (ptr)
		while (optind < argc-1)
			len = inet_srcrt_add(argv[optind++]);
	else
		if (optind < argc-1)
			err_quit("need -g or -G to specify route");

	if (optind != argc-1)
		err_quit("missing <hostname>");

	ai = Host_serv(argv[optind], SERV_PORT_STR, AF_INET, SOCK_STREAM);

	sockfd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

	if (ptr) {
		len = inet_srcrt_add(argv[optind]);	/* dest at end */
		Setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, ptr, len);
		free(ptr);
	}

	Connect(sockfd, ai->ai_addr, ai->ai_addrlen);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

```

#### tcpserv01.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	u_char				*opts;
	pid_t				childpid;
	socklen_t			clilen, len;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);

	opts = Malloc(44);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				err_sys("accept error");
		}

		len = 44;
		Getsockopt(connfd, IPPROTO_IP, IP_OPTIONS, opts, &len);
		if (len > 0) {
			printf("received IP options, len = %d\n", len);
			inet_srcrt_print(opts, len);
		}

		if ( (childpid = Fork()) == 0) {	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
```



### IPv6不跳选项和目的地选项

| 类型 | 长度 |    选项值    |
| :--: | :--: | :----------: |
|  1   |  1   | 长度字段值 6 |

```c++
#include <netinet/in.h>
 
 
int inet6_opt_init(void *extbuf, socklen_t extlen) ;
//Returns: number of bytes required to hold empty extension header, -1 on error
 
 
int inet6_opt_append(void *extbuf, socklen_t extlen, int offset, uint8_t type, socklen_t len, uint_t align, void **databufp) ;
//Returns: updated length of overall extension header after adding option, -1 on error
 
 
int inet6_opt_finish(void *extbuf, socklen_t extlen, int offset) ;
//Returns: updated length of finished extension header, –1 on error
 
 
int inet6_opt_set_val(void *databuf, int offset, const void *val, socklen_t vallen) ;
//Returns: new offset inside databuf

#include <netinet/in.h>
int inet6_opt_next(const void *extbuf, socklen_t extlen, int offset, uint8_t *typep, socklen_t *lenp, void **databufp) ;
//Returns: offset of next option, -1 on end of options or error
int inet6_opt_find(const void *extbuf, socklen_t extlen, int offset, uint8_t type, socklen_t *lenp, void **databufp) ;
//Returns: offset of next option, -1 on end of options or error 
int inet6_opt_get_val(const void *databuf, int offset, void *val, socklen_t vallen) ;
//Returns: new offset inside databuf
```

### IPv6路由首部

```c++
// 构造路由首部
#include <netinet/in.h>
 
socklen_t inet6_rth_space(int type, int segments) ;
Returns: positive number of bytes if OK, 0 on error
 
void *inet6_rth_init(void *rthbuf, socklen_t rthlen, int type, int segments);
Returns: non-null pointer if OK, NULL on error
 
int inet6_rth_add(void *rthbuf, const struct in6_addr *addr);
Returns: 0 if OK, -1 on error

// 处理接收的路由首部
#include <netinet/in.h>

int inet6_rth_reverse(const void *in, void *out);
Returns: 0 if OK, -1 on error
 
int inet6_rth_segments(const void *rthbuf);
Returns: number of segments in routing header if OK, -1 on error
 
struct in6_addr *inet6_rth_getaddr(const void *rthbuf, int index);
Returns: non-null pointer if OK, NULL on err
```



#### udpcli01.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	void				*rth;
	struct addrinfo		*ai;

	if (argc < 2)
		err_quit("usage: udpcli01 [ <hostname> ... ] <hostname>");

	if (argc > 2) {
		int i;

		len = Inet6_rth_space(IPV6_RTHDR_TYPE_0, argc-2);
		ptr = Malloc(len);
		Inet6_rth_init(ptr, len, IPV6_RTHDR_TYPE_0, argc-2);
		for (i = 1; i < argc-1; i++) {
			ai = Host_serv(argv[i], NULL, AF_INET6, 0);
			Inet6_rth_add(ptr, &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr);
		}
	}

	ai = Host_serv(argv[argc-1], SERV_PORT_STR, AF_INET6, SOCK_DGRAM);

	sockfd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

	if (ptr) {
		Setsockopt(sockfd, IPPROTO_IPV6, IPV6_RTHDR, ptr, len);
		free(ptr);
	}

	dg_cli(stdin, sockfd, ai->ai_addr, ai->ai_addrlen);	/* do it all */

	exit(0);
}

```



#### udpserv01.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in6	servaddr, cliaddr;

	sockfd = Socket(AF_INET6, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family     = AF_INET6;
	servaddr.sin6_addr       = in6addr_any;
	servaddr.sin6_port       = htons(SERV_PORT);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}

#include	"unp.h"

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int				n;
	char			mesg[MAXLINE];
	int				on;
	char			control[MAXLINE];
	struct msghdr	msg;
	struct cmsghdr	*cmsg;
	struct iovec	iov[1];

	on = 1;
	Setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVRTHDR, &on, sizeof(on));

	bzero(&msg, sizeof(msg));
	iov[0].iov_base = mesg;
	msg.msg_name = pcliaddr;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	for ( ; ; ) {
		msg.msg_namelen = clilen;
		msg.msg_controllen = sizeof(control);
		iov[0].iov_len = MAXLINE;
		n = Recvmsg(sockfd, &msg, 0);

		for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;
			 cmsg = CMSG_NXTHDR(&msg, cmsg)) {
			if (cmsg->cmsg_level == IPPROTO_IPV6 &&
				cmsg->cmsg_type == IPV6_RTHDR) {
				inet6_srcrt_print(CMSG_DATA(cmsg));
				Inet6_rth_reverse(CMSG_DATA(cmsg), CMSG_DATA(cmsg));
			}
		}

		iov[0].iov_len = n;
		Sendmsg(sockfd, &msg, 0);
	}
}

```



#### sourceroute6.c

```c++
#include	"unp.h"

void
inet6_srcrt_print(void *ptr)
{
	int			i, segments;
	char			str[INET6_ADDRSTRLEN];

	segments = Inet6_rth_segments(ptr);
	printf("received source route: ");
	for (i = 0; i < segments; i++)
		printf("%s ", Inet_ntop(AF_INET6, Inet6_rth_getaddr(ptr, i),
								str, sizeof(str)));
	printf("\n");
}

```

