##Chapter-20 广播

----

### 概述

| 类型 | IPv4 | IPv6 |  TCP   | UDP  | 所标识接口数 | 递送到接口数 |
| :--: | :--: | :--: | :----: | :--: | :----------: | :----------: |
| 单播 | [x]  | [x]  |  [x]   | [x]  |     一个     |     一个     |
| 任播 |  *   | [x]  | 尚没有 | [x]  |     一组     | 一组中的一个 |
| 多播 | 可选 | [x]  |        | [x]  |     一组     | 一组中的全体 |
| 广播 | [x]  |      |        | [x]  |     全体     |     全体     |



### 使用广播的例子

1. ARP
2. DHCP
3. NTP
4. 路由守护进程

### 广播地址

1. 子网定向广播地址 {子网ID， -1}。
2. 受限广播地址  {-1， -1} 或255.255.255.255，路由器不转发该目的地的ip数据报。
3. {子网ID， 主机ID} 表示一个IPv4地址。

### 单播和广播的区别

1. 图20-3 
2. 图20-4

### 使用广播的dg_cli

```c++
#include	"unp.h"
// 使用pselect 阻塞和解阻塞信号
static void	recvfrom_alarm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	sigset_t		sigset_alrm, sigset_empty;
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	FD_ZERO(&rset);

	Sigemptyset(&sigset_empty);
	Sigemptyset(&sigset_alrm);
	Sigaddset(&sigset_alrm, SIGALRM);

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		Sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);
		alarm(5);
		for ( ; ; ) {
			FD_SET(sockfd, &rset);
			n = pselect(sockfd+1, &rset, NULL, NULL, NULL, &sigset_empty);
			if (n < 0) {
				if (errno == EINTR)
					break;
				else
					err_sys("pselect error");
			} else if (n != 1)
				err_sys("pselect error: returned %d", n);

			len = servlen;
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					Sock_ntop_host(preply_addr, len), recvline);
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	return;		/* just interrupt the recvfrom() */
}

```



```c++
#include	"unp.h"
#include	<setjmp.h>
// 从信号处理函数中使用sigsetjmp 和 siglongjmp
static void			recvfrom_alarm(int);
static sigjmp_buf	jmpbuf;

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);
		for ( ; ; ) {
			if (sigsetjmp(jmpbuf, 1) != 0)
				break;
			len = servlen;
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					Sock_ntop_host(preply_addr, len), recvline);
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	siglongjmp(jmpbuf, 1);
}

```

````c++
#include	"unp.h"
// 使用从信号处理函数到主控函数的管道作为IPC
static void	recvfrom_alarm(int);
static int	pipefd[2];

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n, maxfdp1;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	Pipe(pipefd);
	maxfdp1 = max(sockfd, pipefd[0]) + 1;

	FD_ZERO(&rset);

	Signal(SIGALRM, recvfrom_alarm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);
		for ( ; ; ) {
			FD_SET(sockfd, &rset);
			FD_SET(pipefd[0], &rset);
			if ( (n = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
				if (errno == EINTR)
					continue;
				else
					err_sys("select error");
			}

			if (FD_ISSET(sockfd, &rset)) {
				len = servlen;
				n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
				recvline[n] = 0;	/* null terminate */
				printf("from %s: %s",
						Sock_ntop_host(preply_addr, len), recvline);
			}

			if (FD_ISSET(pipefd[0], &rset)) {
				Read(pipefd[0], &n, 1);		/* timer expired */
				break;
			}
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	Write(pipefd[1], "", 1);	/* write one null byte to pipe */
	return;
}

````

**对于目的地是广播地址的IP数据报，如果大小超过外出接口的MTU， 发送他的系统将会返回EMSGSIZE错误。**

