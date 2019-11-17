##Chapter-07 套接字选项

----

### 设置套接字的方法

1. getsockopt和setsockopt
2. fcntl
3. ioctl 

| level          | Optname              | get  | set  | 说明                           | 标志 | 数据类型       |
| -------------- | -------------------- | ---- | ---- | ------------------------------ | ---- | -------------- |
|                |                      |      |      |                                |      |                |
| SOL_SOCKET     | SO_BROADCAST         | y    | y    | 允许发送广播数据报             | y    | int            |
|                | SO_DEBUG             | y    | y    | 使能调试跟踪                   | y    | int            |
|                | SO_DONTROUTE         | y    | y    | 旁路路由表查询                 | y    | int            |
|                | SO_ERROR             | y    |      | 获取待处理错误并消除           |      | int            |
|                | SO_KEEPALIVE         | y    | y    | 周期性测试连接是否存活         | y    | int            |
|                | SO_LINGER            | y    | y    | 若有数据待发送则延迟关闭       |      | linger{}       |
|                | SO_OOBINLINE         | y    | y    | 让接收到的带外数据继续在线存放 | y    | int            |
|                | SO_RCVBUF            | y    | y    | 接收缓冲区大小                 |      | int            |
|                | SO_SNDBUF            | y    | y    | 发送缓冲区大小                 |      | int            |
|                | SO_RCVLOWAT          | y    | y    | 接收缓冲区低潮限度             |      | int            |
|                | SO_SNDLOWAT          | y    | y    | 发送缓冲区低潮限度             |      | int            |
|                | SO_RCVTIMEO          | y    | y    | 接收超时                       |      | timeval{}      |
|                | SO_SNDTIMEO          | y    | y    | 发送超时                       |      | timeval{}      |
|                | SO_REUSEADDR         | y    | y    | 允许重用本地地址               | y    | int            |
|                | SO_REUSEPORT         | y    | y    | 允许重用本地地址               | y    | int            |
|                | SO_TYPE              | y    |      | 取得套接口类型                 |      | int            |
|                | SO_USELOOPBACK       | y    | y    | 路由套接口取得所发送数据的拷贝 | y    | int            |
|                |                      |      |      |                                |      |                |
| IPPROTO_IP     | IP_HDRINCL           | y    | y    | IP头部包括数据                 | y    | int            |
|                | IP_OPTIONS           | y    | y    | IP头部选项                     |      | 见后面说明     |
|                | IP_RECVDSTADDR       | y    | y    | 返回目的IP地址                 | y    | int            |
|                | IP_RECVIF            | y    | y    | 返回接收到的接口索引           | y    | int            |
|                | IP_TOS               | y    | y    | 服务类型和优先权               |      | int            |
|                | IP_TTL               | y    | y    | 存活时间                       |      | int            |
|                | IP_MULTICAST_IF      | y    | y    | 指定外出接口                   |      | in_addr{}      |
|                | IP_MULTICAST_TTL     | y    | y    | 指定外出TTL                    |      | u_char         |
|                | IP_MULTICAST_LOOP    | y    | y    | 指定是否回馈                   |      | u_char         |
|                | IP_ADD_MEMBERSHIP    |      | y    | 加入多播组                     |      | ip_mreq{}      |
|                | IP_DROP_MEMBERSHIP   |      | y    | 离开多播组                     |      | ip_mreq{}      |
|                |                      |      |      |                                |      |                |
| IPPROTO_ICMPV6 | ICMP6_FILTER         | y    | y    | 指定传递的ICMPv6消息类型       |      | icmp6_filter{} |
|                |                      |      |      |                                |      |                |
| IPPROTO_IPV6   | IPV6_ADDRFORM        | y    | y    | 改变套接口的地址结构           |      | int            |
|                | IPV6_CHECKSUM        | y    | y    | 原始套接口的校验和字段偏移     |      | int            |
|                | IPV6_DSTOPTS         | y    | y    | 接收目标选项                   | y    | int            |
|                | IPV6_HOPLIMIT        | y    | y    | 接收单播跳限                   | y    | int            |
|                | IPV6_HOPOPTS         | y    | y    | 接收步跳选项                   | y    | int            |
|                | IPV6_NEXTHOP         | y    | y    | 指定下一跳地址                 | y    | sockaddr{}     |
|                | IPV6_PKTINFO         | y    | y    | 接收分组信息                   | y    | int            |
|                | IPV6_PKTOPTIONS      | y    | y    | 指定分组选项                   |      | 见后面说明     |
|                | IPV6_RTHDR           | y    | y    | 接收原路径                     | y    | int            |
|                | IPV6_UNICAST_HOPS    | y    | y    | 缺省单播跳限                   |      | int            |
|                | IPV6_MULTICAST_IF    | y    | y    | 指定外出接口                   |      | in6_addr{}     |
|                | IPV6_MULTICAST_HOPS  | y    | y    | 指定外出跳限                   |      | u_int          |
|                | IPV6_MULTICAST_LOOP  | y    | y    | 指定是否回馈                   | y    | u_int          |
|                | IPV6_ADD_MEMBERSHIP  |      | y    | 加入多播组                     |      | ipv6_mreq{}    |
|                | IPV6_DROP_MEMBERSHIP |      | y    | 离开多播组                     |      | ipv6_mreq{}    |
|                |                      |      |      |                                |      |                |
| IPPROTO_TCP    | TCP_KEEPALIVE        | y    | y    | 控测对方是否存活前连接闲置秒数 |      | int            |
|                | TCP_MAXRT            | y    | y    | TCP最大重传时间                |      | int            |
|                | TCP_MAXSEG           | y    | y    | TCP最大分节大小                |      | int            |
|                | TCP_NODELAY          | y    | y    | 禁止Nagle算法                  | y    | int            |
|                | TCP_STDURG           | y    | y    | 紧急指针的解释                 |      |                |

```c++
// 套接字选项测试
/* include checkopts1 */
/* *INDENT-OFF* */
#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_xxx defines */

union val {
  int				i_val;
  long				l_val;
  struct linger		linger_val;
  struct timeval	timeval_val;
} val;

static char	*sock_str_flag(union val *, int);
static char	*sock_str_int(union val *, int);
static char	*sock_str_linger(union val *, int);
static char	*sock_str_timeval(union val *, int);

struct sock_opts {
  const char	   *opt_str;
  int		opt_level;
  int		opt_name;
  char   *(*opt_val_str)(union val *, int);
} sock_opts[] = {
	{ "SO_BROADCAST",		SOL_SOCKET,	SO_BROADCAST,	sock_str_flag },
	{ "SO_DEBUG",			SOL_SOCKET,	SO_DEBUG,		sock_str_flag },
	{ "SO_DONTROUTE",		SOL_SOCKET,	SO_DONTROUTE,	sock_str_flag },
	{ "SO_ERROR",			SOL_SOCKET,	SO_ERROR,		sock_str_int },
	{ "SO_KEEPALIVE",		SOL_SOCKET,	SO_KEEPALIVE,	sock_str_flag },
	{ "SO_LINGER",			SOL_SOCKET,	SO_LINGER,		sock_str_linger },
	{ "SO_OOBINLINE",		SOL_SOCKET,	SO_OOBINLINE,	sock_str_flag },
	{ "SO_RCVBUF",			SOL_SOCKET,	SO_RCVBUF,		sock_str_int },
	{ "SO_SNDBUF",			SOL_SOCKET,	SO_SNDBUF,		sock_str_int },
	{ "SO_RCVLOWAT",		SOL_SOCKET,	SO_RCVLOWAT,	sock_str_int },
	{ "SO_SNDLOWAT",		SOL_SOCKET,	SO_SNDLOWAT,	sock_str_int },
	{ "SO_RCVTIMEO",		SOL_SOCKET,	SO_RCVTIMEO,	sock_str_timeval },
	{ "SO_SNDTIMEO",		SOL_SOCKET,	SO_SNDTIMEO,	sock_str_timeval },
	{ "SO_REUSEADDR",		SOL_SOCKET,	SO_REUSEADDR,	sock_str_flag },
#ifdef	SO_REUSEPORT
	{ "SO_REUSEPORT",		SOL_SOCKET,	SO_REUSEPORT,	sock_str_flag },
#else
	{ "SO_REUSEPORT",		0,			0,				NULL },
#endif
	{ "SO_TYPE",			SOL_SOCKET,	SO_TYPE,		sock_str_int },
	{ "SO_USELOOPBACK",		SOL_SOCKET,	SO_USELOOPBACK,	sock_str_flag },
	{ "IP_TOS",				IPPROTO_IP,	IP_TOS,			sock_str_int },
	{ "IP_TTL",				IPPROTO_IP,	IP_TTL,			sock_str_int },
#ifdef	IPV6_DONTFRAG
	{ "IPV6_DONTFRAG",		IPPROTO_IPV6,IPV6_DONTFRAG,	sock_str_flag },
#else
	{ "IPV6_DONTFRAG",		0,			0,				NULL },
#endif
#ifdef	IPV6_UNICAST_HOPS
	{ "IPV6_UNICAST_HOPS",	IPPROTO_IPV6,IPV6_UNICAST_HOPS,sock_str_int },
#else
	{ "IPV6_UNICAST_HOPS",	0,			0,				NULL },
#endif
#ifdef	IPV6_V6ONLY
	{ "IPV6_V6ONLY",		IPPROTO_IPV6,IPV6_V6ONLY,	sock_str_flag },
#else
	{ "IPV6_V6ONLY",		0,			0,				NULL },
#endif
	{ "TCP_MAXSEG",			IPPROTO_TCP,TCP_MAXSEG,		sock_str_int },
	{ "TCP_NODELAY",		IPPROTO_TCP,TCP_NODELAY,	sock_str_flag },
#ifdef	SCTP_AUTOCLOSE
	{ "SCTP_AUTOCLOSE",		IPPROTO_SCTP,SCTP_AUTOCLOSE,sock_str_int },
#else
	{ "SCTP_AUTOCLOSE",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXBURST
	{ "SCTP_MAXBURST",		IPPROTO_SCTP,SCTP_MAXBURST,	sock_str_int },
#else
	{ "SCTP_MAXBURST",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXSEG
	{ "SCTP_MAXSEG",		IPPROTO_SCTP,SCTP_MAXSEG,	sock_str_int },
#else
	{ "SCTP_MAXSEG",		0,			0,				NULL },
#endif
#ifdef	SCTP_NODELAY
	{ "SCTP_NODELAY",		IPPROTO_SCTP,SCTP_NODELAY,	sock_str_flag },
#else
	{ "SCTP_NODELAY",		0,			0,				NULL },
#endif
	{ NULL,					0,			0,				NULL }
};
/* *INDENT-ON* */
/* end checkopts1 */

/* include checkopts2 */
int
main(int argc, char **argv)
{
	int					fd;
	socklen_t			len;
	struct sock_opts	*ptr;

	for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
		printf("%s: ", ptr->opt_str);
		if (ptr->opt_val_str == NULL)
			printf("(undefined)\n");
		else {
			switch(ptr->opt_level) {
			case SOL_SOCKET:
			case IPPROTO_IP:
			case IPPROTO_TCP:
				fd = Socket(AF_INET, SOCK_STREAM, 0);
				break;
#ifdef	IPV6
			case IPPROTO_IPV6:
				fd = Socket(AF_INET6, SOCK_STREAM, 0);
				break;
#endif
#ifdef	IPPROTO_SCTP
			case IPPROTO_SCTP:
				fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
				break;
#endif
			default:
				err_quit("Can't create fd for level %d\n", ptr->opt_level);
			}

			len = sizeof(val);
			if (getsockopt(fd, ptr->opt_level, ptr->opt_name,
						   &val, &len) == -1) {
				err_ret("getsockopt error");
			} else {
				printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
			}
			close(fd);
		}
	}
	exit(0);
}
/* end checkopts2 */

/* include checkopts3 */
static char	strres[128];

static char	*
sock_str_flag(union val *ptr, int len)
{
/* *INDENT-OFF* */
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres),
				 "%s", (ptr->i_val == 0) ? "off" : "on");
	return(strres);
/* *INDENT-ON* */
}
/* end checkopts3 */

static char	*
sock_str_int(union val *ptr, int len)
{
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres), "%d", ptr->i_val);
	return(strres);
}

static char	*
sock_str_linger(union val *ptr, int len)
{
	struct linger	*lptr = &ptr->linger_val;

	if (len != sizeof(struct linger))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct linger)", len);
	else
		snprintf(strres, sizeof(strres), "l_onoff = %d, l_linger = %d",
				 lptr->l_onoff, lptr->l_linger);
	return(strres);
}

static char	*
sock_str_timeval(union val *ptr, int len)
{
	struct timeval	*tvptr = &ptr->timeval_val;

	if (len != sizeof(struct timeval))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct timeval)", len);
	else
		snprintf(strres, sizeof(strres), "%d sec, %d usec",
				 tvptr->tv_sec, tvptr->tv_usec);
	return(strres);
}

```



|            操作            |       :----:       |        ioctl         | 路由套接字 |   POSIX    |
| :------------------------: | :----------------: | :------------------: | :--------: | :--------: |
|   设置套接字为非阻塞式IO   | F_SETFL O_NONBLOCK |       FIONBIO        |            |   fcntl    |
|   设置套接字为信号驱动IO   |  F_SETFL O_ASYNC   |       FIOASYNC       |            |   fcntl    |
|       设置套接字属主       |      F_SEROWN      | SIOCSPGRP或FIOSETOWN |            |   fcntl    |
|       获取套接字属主       |      F_GETOWN      | SIOCGPGRP或FIOGETOWN |            |   fcntl    |
|  获取套接字缓冲区的字节数  |                    |       FIOBREAD       |            |            |
| 测试套接字是否处于带外标记 |                    |      SIOCATMARK      |            | sockatmark |
|        获取接口列表        |                    |     SIOCGIFCONF      |   sysctl   |            |
|          接口操作          |                    |    SIOC[GS]IFxxx     |            |            |
|      arp高速缓存操作       |                    |       SIOCxARP       |  RTM_xxx   |            |
|         路由表操作         |                    |      SIOCxxxRT       |  RTM_xxx   |            |



