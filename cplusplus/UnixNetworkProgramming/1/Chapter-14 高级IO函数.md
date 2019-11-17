##Chapter-14 高级IO函数

----

### 设置套接字超时的三种方法

1. 调用alarm，它在指定超时期满时产生SIGALARM信号。
2. 在select中阻塞等待IO，以此代替直接阻塞在read或write上。
3. 使用较新的SO_RCVTIMEO和SO_SNDTIMEO套接字选项。



```c++
// lib/connect_timeo.c 使用SIGALRM设置超时的connect
/* include connect_timeo */
#include	"unp.h"

static void	connect_alarm(int);

int
connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc	*sigfunc;
	int		n;

	sigfunc = Signal(SIGALRM, connect_alarm);
	if (alarm(nsec) != 0)
		err_msg("connect_timeo: alarm was already set");

	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		close(sockfd);
		if (errno == EINTR)
			errno = ETIMEDOUT;
	}
	alarm(0);					/* turn off the alarm */
	Signal(SIGALRM, sigfunc);	/* restore previous signal handler */

	return(n);
}

static void
connect_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}
/* end connect_timeo */

void
Connect_timeo(int fd, const SA *sa, socklen_t salen, int sec)
{
	if (connect_timeo(fd, sa, salen, sec) < 0)
		err_sys("connect_timeo error");
}

```



```c++
// advio/dgclitimeo3.c 使用SIGALRM设置超时的recvfrom
#include	"unp.h"

static void	sig_alrm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	Signal(SIGALRM, sig_alrm);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);
		if ( (n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
			if (errno == EINTR)
				fprintf(stderr, "socket timeout\n");
			else
				err_sys("recvfrom error");
		} else {
			alarm(0);
			recvline[n] = 0;	/* null terminate */
			Fputs(recvline, stdout);
		}
	}
}

static void
sig_alrm(int signo)
{
	return;			/* just interrupt the recvfrom() */
}

```



```c++
// lib/readable_timeo.c 使用select为recvfrom设置超时
/* include readable_timeo */
#include	"unp.h"

int
readable_timeo(int fd, int sec)
{
	fd_set			rset;
	struct timeval	tv;

	FD_ZERO(&rset);
	FD_SET(fd, &rset);

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	return(select(fd+1, &rset, NULL, NULL, &tv));
		/* 4> 0 if descriptor is readable */
}
/* end readable_timeo */

int
Readable_timeo(int fd, int sec)
{
	int		n;

	if ( (n = readable_timeo(fd, sec)) < 0)
		err_sys("readable_timeo error");
	return(n);
}

```



```c++
// advio/dgclitimeo1.c 调用readable_timeo设置超时的dg_cli
#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		if (Readable_timeo(sockfd, 5) == 0) {
			fprintf(stderr, "socket timeout\n");
		} else {
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;	/* null terminate */
			Fputs(recvline, stdout);
		}
	}
}

```

```c++

// advoi/dgclitimeo2.c 使用SO_RCVTIMEO套接字为recvfrom设置超时
#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	struct timeval	tv;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
		if (n < 0) {
			if (errno == EWOULDBLOCK) {
				fprintf(stderr, "socket timeout\n");
				continue;
			} else
				err_sys("recvfrom error");
		}

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}

```



````c++
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buff, size_t nbytes, int flags);
ssize_t send(int sockfd, const void *buff, size_t nbytes, int flags);
//Both return: number of bytes read or written if OK, –1 on error

#include <sys/uio.h>
ssize_t readv(int filedes, const struct iovec *iov, int iovcnt); 
ssize_t writev(int filedes, const struct iovec *iov, int iovcnt);
//Both return: number of bytes read or written, –1 on error

#include <sys/socket.h>
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags); 
//Both return: number of bytes read or written if OK, –1 on error
````

### 辅助数据

1. 辅助数据可用过调用sendmsg 和recvmsg两个函数，使用msghdr结构中的msg_control 和msg_controllen两个成员发送和接受。
2. 辅助数据也叫控制信息。

```c++
#include <sys/socket.h>
#include <sys/param.h> /* for ALIGN macro on many implementations */ 
struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *mhdrptr) ;
// Returns: pointer to first cmsghdr structure or NULL if no ancillary data 
struct cmsghdr *CMSG_NXTHDR(struct msghdr *mhdrptr, struct cmsghdr*cmsgptr) ;
// Returns: pointer to next cmsghdr structure or NULL if no more ancillary data objects 
unsigned char *CMSG_DATA(struct cmsghdr *cmsgptr) ;
//Returns: pointer to first byte of data associated with cmsghdr structure 
unsigned int CMSG_LEN(unsigned int length) ;
//Returns: value to store in cmsg_len given the amount of data 
unsigned int CMSG_SPACE(unsigned int length) ;
//Returns: total size of an ancillary data object given the amount of data
```



```c++
//advio/str_echo_stdio02.c 使用标准IO的str_cli
#include	"unp.h"

void
str_echo(int sockfd)
{
	char		line[MAXLINE];
	FILE		*fpin, *fpout;

	fpin = Fdopen(sockfd, "r");
	fpout = Fdopen(sockfd, "w");

	while (Fgets(line, MAXLINE, fpin) != NULL)
		Fputs(line, fpout);
}

```



### 高级轮询技术

1. /dev/poll
2. Kqueue接口

```c++
//advio/str_cli_poll03.c 使用/dev/poll 的str_cli函数
#include	"unp.h"
#include	<sys/devpoll.h>

void
str_cli(FILE *fp, int sockfd)
{
	int		stdineof;
	char		buf[MAXLINE];
	int		n;
	int		wfd;
	struct pollfd	pollfd[2];
	struct dvpoll	dopoll;
	int		i;
	int		result;

	wfd = Open("/dev/poll", O_RDWR, 0);

	pollfd[0].fd = fileno(fp);
	pollfd[0].events = POLLIN;
	pollfd[0].revents = 0;

	pollfd[1].fd = sockfd;
	pollfd[1].events = POLLIN;
	pollfd[1].revents = 0;

	Write(wfd, pollfd, sizeof(struct pollfd) * 2);

	stdineof = 0;
	for ( ; ; ) {
		/* block until /dev/poll says something is ready */
		dopoll.dp_timeout = -1;
		dopoll.dp_nfds = 2;
		dopoll.dp_fds = pollfd;
		result = Ioctl(wfd, DP_POLL, &dopoll);

		/* loop through ready file descriptors */
		for (i = 0; i < result; i++) {
			if (dopoll.dp_fds[i].fd == sockfd) {
				/* socket is readable */
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					if (stdineof == 1)
						return;		/* normal termination */
					else
						err_quit("str_cli: server terminated prematurely");
				}

				Write(fileno(stdout), buf, n);
			} else {
				/* input is readable */
				if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
					stdineof = 1;
					Shutdown(sockfd, SHUT_WR);	/* send FIN */
					continue;
				}

				Writen(sockfd, buf, n);
			}
		}
	}
}

```



```c++
// advio/str_cli_kqueue04.c 使用kqueue的str_cli函数
#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int				kq, i, n, nev, stdineof = 0, isfile;
	char			buf[MAXLINE];
	struct kevent	kev[2];
	struct timespec	ts;
	struct stat		st;

	isfile = ((fstat(fileno(fp), &st) == 0) &&
			  (st.st_mode & S_IFMT) == S_IFREG);

	EV_SET(&kev[0], fileno(fp), EVFILT_READ, EV_ADD, 0, 0, NULL);
	EV_SET(&kev[1], sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	kq = Kqueue();
	ts.tv_sec = ts.tv_nsec = 0;
	Kevent(kq, kev, 2, NULL, 0, &ts);

	for ( ; ; ) {
		nev = Kevent(kq, NULL, 0, kev, 2, NULL);

		for (i = 0; i < nev; i++) {
			if (kev[i].ident == sockfd) {	/* socket is readable */
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					if (stdineof == 1)
						return;		/* normal termination */
					else
						err_quit("str_cli: server terminated prematurely");
				}

				Write(fileno(stdout), buf, n);
			}

			if (kev[i].ident == fileno(fp)) {  /* input is readable */
				n = Read(fileno(fp), buf, MAXLINE);
				if (n > 0)
					Writen(sockfd, buf, n);

				if (n == 0 || (isfile && n == kev[i].data)) {
					stdineof = 1;
					Shutdown(sockfd, SHUT_WR);	/* send FIN */
					kev[i].flags = EV_DELETE;
					Kevent(kq, &kev[i], 1, NULL, 0, &ts);	/* remove kevent */
					continue;
				}
			}
		}
	}
}

```


