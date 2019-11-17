##Chapter-24 带外数据

----

### 概述

1. 带外数据是被认为比普通数据优先级更高的数据，带外数据并不要求在客户端和服务器之间使用一条连接，而是映射到已有的连接中。
2. UDP没有实现带外数据。
3. TCP实现了带外数据。
4. SO_OOBINLINE如果开启，那么TCP紧急指针指向的实际数据字节将被留在套接字接收缓冲区。

### tcpsend01.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpsend01 <host> <port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	Write(sockfd, "123", 3);
	printf("wrote 3 bytes of normal data\n");
	sleep(1);

	Send(sockfd, "4", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	Write(sockfd, "56", 2);
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	Send(sockfd, "7", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	Write(sockfd, "89", 2);
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	exit(0);
}

```



### tcperecv01.c

```c++
#include	"unp.h"

int		listenfd, connfd;

void	sig_urg(int);

int
main(int argc, char **argv)
{
	int		n;
	char	buff[100];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv01 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	Signal(SIGURG, sig_urg);
	Fcntl(connfd, F_SETOWN, getpid());

	for ( ; ; ) {
		if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[100];

	printf("SIGURG received\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}

```

### tcprecv03.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, justreadoob = 0;
	char	buff[100];
	fd_set	rset, xset;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv03 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);

	FD_ZERO(&rset);
	FD_ZERO(&xset);
	for ( ; ; ) {
		FD_SET(connfd, &rset);
		if (justreadoob == 0)
			FD_SET(connfd, &xset);

		Select(connfd + 1, &rset, NULL, &xset, NULL);

		if (FD_ISSET(connfd, &xset)) {
			n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
			justreadoob = 1;
			FD_CLR(connfd, &xset);
		}

		if (FD_ISSET(connfd, &rset)) {
			if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
				printf("received EOF\n");
				exit(0);
			}
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
			justreadoob = 0;
		}
	}
}

```



### sockatmark

```c++
#include <sys/socket.h>

int sockatmark(int sockfd) ;
//Returns: 1 if at out-of-band mark, 0 if not at mark, –1 on error

#include "unp.h"
int
sockatmark(int fd) {
  int flag;
  if (ioctl(fd, SIOCATMARK, &flag) < 0)
    return (-1);
  return (flag != 0);
}
```



### tcpsend04.c

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpsend04 <host> <port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	Write(sockfd, "123", 3);
	printf("wrote 3 bytes of normal data\n");

	Send(sockfd, "4", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");

	Write(sockfd, "5", 1);
	printf("wrote 1 byte of normal data\n");

	exit(0);
}

```



### tcprecv04.c

### 

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, on=1;
	char	buff[100];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv04 [ <host> ] <port#>");

	Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));

	connfd = Accept(listenfd, NULL, NULL);
	sleep(5);

	for ( ; ; ) {
		if (Sockatmark(connfd))
			printf("at OOB mark\n");

		if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
}

```



### tcpsend05.c

### 

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd, size;
	char	buff[16384];

	if (argc != 3)
		err_quit("usage: tcpsend05 <host> <port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	size = 32768;
	Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

	Write(sockfd, buff, 16384);
	printf("wrote 16384 bytes of normal data\n");
	sleep(5);

	Send(sockfd, "a", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");

	Write(sockfd, buff, 1024);
	printf("wrote 1024 bytes of normal data\n");

	exit(0);
}

```



### tcprecv05.c

### 

```c++
#include	"unp.h"

int		listenfd, connfd;

void	sig_urg(int);

int
main(int argc, char **argv)
{
	int		size;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv05 [ <host> ] <port#>");

	size = 4096;
	Setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	connfd = Accept(listenfd, NULL, NULL);

	Signal(SIGURG, sig_urg);
	Fcntl(connfd, F_SETOWN, getpid());

	for ( ; ; )
		pause();
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[2048];

	printf("SIGURG received\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte\n", n);
}

```



### tcpsend06.c

### 

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpsend06 <host> <port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	Write(sockfd, "123", 3);
	printf("wrote 3 bytes of normal data\n");

	Send(sockfd, "4", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");

	Write(sockfd, "5", 1);
	printf("wrote 1 byte of normal data\n");

	Send(sockfd, "6", 1, MSG_OOB);
	printf("wrote 1 byte of OOB data\n");

	Write(sockfd, "7", 1);
	printf("wrote 1 byte of normal data\n");

	exit(0);
}

```



### tcprecv06.c

### 

```c++
#include	"unp.h"

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, on=1;
	char	buff[100];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv06 [ <host> ] <port#>");

	Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));

	connfd = Accept(listenfd, NULL, NULL);
	sleep(5);

	for ( ; ; ) {
		if (Sockatmark(connfd))
			printf("at OOB mark\n");

		if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
}

```



### 客户端和服务器的心搏函数

#### heartbeatcli.c

```c++
#include	"unp.h"

static int		servfd;
static int		nsec;			/* #seconds betweeen each alarm */
static int		maxnprobes;		/* #probes w/no response before quit */
static int		nprobes;		/* #probes since last server response */
static void	sig_urg(int), sig_alrm(int);

void
heartbeat_cli(int servfd_arg, int nsec_arg, int maxnprobes_arg)
{
	servfd = servfd_arg;		/* set globals for signal handlers */
	if ( (nsec = nsec_arg) < 1)
		nsec = 1;
	if ( (maxnprobes = maxnprobes_arg) < nsec)
		maxnprobes = nsec;
	nprobes = 0;

	Signal(SIGURG, sig_urg);
	Fcntl(servfd, F_SETOWN, getpid());

	Signal(SIGALRM, sig_alrm);
	alarm(nsec);
}

static void
sig_urg(int signo)
{
	int		n;
	char	c;
 
	if ( (n = recv(servfd, &c, 1, MSG_OOB)) < 0) {
		if (errno != EWOULDBLOCK)
			err_sys("recv error");
	}
	nprobes = 0;			/* reset counter */
	return;					/* may interrupt client code */
}

static void
sig_alrm(int signo)
{
	if (++nprobes > maxnprobes) {
		fprintf(stderr, "server is unreachable\n");
		exit(0);
	}
	Send(servfd, "1", 1, MSG_OOB);
	alarm(nsec);
	return;					/* may interrupt client code */
}

```



#### heartbeatserv.c

```c++
#include	"unp.h"

static int	servfd;
static int	nsec;			/* #seconds between each alarm */
static int	maxnalarms;		/* #alarms w/no client probe before quit */
static int	nprobes;		/* #alarms since last client probe */
static void	sig_urg(int), sig_alrm(int);

void
heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg)
{
	servfd = servfd_arg;		/* set globals for signal handlers */
	if ( (nsec = nsec_arg) < 1)
		nsec = 1;
	if ( (maxnalarms = maxnalarms_arg) < nsec)
		maxnalarms = nsec;

	Signal(SIGURG, sig_urg);
	Fcntl(servfd, F_SETOWN, getpid());

	Signal(SIGALRM, sig_alrm);
	alarm(nsec);
}

static void
sig_urg(int signo)
{
	int		n;
	char	c;
 
	if ( (n = recv(servfd, &c, 1, MSG_OOB)) < 0) {
		if (errno != EWOULDBLOCK)
			err_sys("recv error");
	}
	Send(servfd, &c, 1, MSG_OOB);	/* echo back out-of-band byte */

	nprobes = 0;			/* reset counter */
	return;					/* may interrupt server code */
}

static void
sig_alrm(int signo)
{
	if (++nprobes > maxnalarms) {
		printf("no probes from client\n");
		exit(0);
	}
	alarm(nsec);
	return;					/* may interrupt server code */
}

```

