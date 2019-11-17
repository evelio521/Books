##Chapter-13 守护进程和inetd超级服务器

----

### 守护进程的多种启动方法

1. 在系统启动阶段，许多守护进程由系统初始化脚本启动的，一般来说这些脚本位于/etc 或/etc/rc。
2. 由inetd超级服务器启动。
3. cron守护进程按照规则定期直行一些程序。
4. at命令用于指定将来某个时刻的程序启动。
5. 守护进程还可以从用户终端或在前台或在后台启动。

### syslogd守护进程

1. 读配置文件/etc/syslog.conf
2. 创建一个unix域套接字，给它捆绑路径名/var/run/log
3. 创建一个udp套接字，捆绑514端口
4. 打开路径/dev/klog
5. 进入无限循环模式。



```c++
// lib/daemon_init.c 把一个普通进程编程守护进程
#include	"unp.h"
#include	<syslog.h>

#define	MAXFD	64

extern int	daemon_proc;	/* defined in error.c */

int
daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */

	if (setsid() < 0)			/* become session leader */
		return (-1);

	Signal(SIGHUP, SIG_IGN);
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* child 1 terminates */

	/* child 2 continues... */

	daemon_proc = 1;			/* for err_XXX() functions */

	chdir("/");				/* change working directory */

	/* close off file descriptors */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return (0);				/* success */
}

```



```c++
// inetd/daytimetcpsrv2.c 作为守护进程运行协议无关时间获取服务器程序
#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t addrlen, len;
	struct sockaddr	*cliaddr;
	char buff[MAXLINE];
	time_t ticks;

	if (argc < 2 || argc > 3)
		err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");

	daemon_init(argv[0], 0);

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);
		err_msg("connection from %s", Sock_ntop(cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}

```



### inetd守护进程的工作流程

1. 启动阶段，读入/etc/inetd.conf文件并给该文件中指定的每个服务创建一个适当的字节类型的套接字
2. 为每个套接字调用bind，指定捆绑相应的服务器的众所周知的端口和通配地址。
3. 对于每个TCP套接字，调用listen接受外来连接请求，数据包套接字不执行本步。
4. 创建完所有套接字之后，调用select等待其中一个套接字变为可读。
5. 当select返回指出某个套接字已可读之后，如果该套接字是一个TCP套接字，而且服务器的wait-flag值为nowait，那么就调用accept接受这个新连接。
6. inetd守护进程调用fork排成进程，并由子进程处理服务请求。
7. 如果第五步中的select返回的是一个字节流套接字，那么父进程必须关闭已连接套接字。父进程再次调用select，等待下一个变为可读的套接字。

```c++
// lib/daemon_inetd.c 守护进程化由inetd运行的进程
#include	"unp.h"
#include	<syslog.h>

extern int	daemon_proc;	/* defined in error.c */

void
daemon_inetd(const char *pname, int facility)
{
	daemon_proc = 1;		/* for our err_XXX() functions */
	openlog(pname, LOG_PID, facility);
}

```



```c++
// inetd/daytimetcpsrv3.c 可由inetd启动的协议无关时间获取服务器程序
#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	socklen_t		len;
	struct sockaddr	*cliaddr;
	char			buff[MAXLINE];
	time_t			ticks;

	daemon_inetd(argv[0], 0);

	cliaddr = Malloc(sizeof(struct sockaddr_storage));
	len = sizeof(struct sockaddr_storage);
	Getpeername(0, cliaddr, &len);
	err_msg("connection from %s", Sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    Write(0, buff, strlen(buff));

	Close(0);	/* close TCP connection */
	exit(0);
}

```




