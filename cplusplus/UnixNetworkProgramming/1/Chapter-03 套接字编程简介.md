##Chapter-03 套接字编程简介

----

### 概述

1. 进程到内核的函数  bind connect sendto
2. 内核到进程的函数 accept recvfrom getsockname getpeername

```c++
// IPv4 POSIX结构
struct in_addr { 
  in_addr_t s_addr;/* 32-bit IPv4 address */
                   /* network byte ordered */
};
struct sockaddr_in {   
  uint8_t sin_len;         /* length of structure (16) */
  sa_family_t sin_family;  /* AF_INET */
  in_port_t sin_port;       /* 16-bit TCP or UDP port number */
                            /* network byte ordered */
  struct in_addr sin_addr; /* 32-bit IPv4 address */ 
                         /* network byte ordered */
  char sin_zero[8]; /* unused */ 
};
```

```c++
// 通用套接字结构
struct sockaddr {
  uint8_t sa_len;
  sa_family_t sa_family; /* address family: AF_xxx value */ 
  char sa_data[14]; /* protocol-specific address */
};
```

```c++
// IPv6套接字结构

struct in6_addr {
  uint8_t s6_addr[16]; /* 128-bit IPv6 address */
                       /* network byte ordered */
};
#define SIN6_LEN /* required for compile-time tests */

struct sockaddr_in6{
  uint8_t sin6_len; //IPv6 为固定的24 字节长度
  sa_family_t sin6_family; //地址簇类型，为AF_INET6
  in_port_t sin6_port; //16 位端口号，网络字节序
  uint32_t sin6_flowinfo; //32 位流标签
  struct in6_addr sin6_addr; //128 位IP 地址
  uint32_t sin6_scope_id; /* set of interfaces for a scope */
}
```

```c++
// 新的通用套接字结构
struct sockaddr_storage {
  uint8_t ss_len; /* length of this struct (implementation dependent) */
  sa_family_t ss_family; /* address family: AF_xxx value */ 
   /* implementation-dependent elements to provide:
    * a) alignment sufficient to fulfill the alignment requirements of 
    *    all socket address types that the system supports.
    * b) enough storage to hold any type of socket address that the
    * system supports.
    */ 
};
```

```c++
//intro/byteorder.c 大小数端的判断
#include	"unp.h"

int
main(int argc, char **argv)
{
	union {
	  short  s;
      char   c[sizeof(short)];
    } un;

	un.s = 0x0102;
	printf("%s: ", CPU_VENDOR_OS);
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	} else
		printf("sizeof(short) = %d\n", sizeof(short));

	exit(0);
}

```

```c++
// 字节操纵函数
#include <strings.h> 
void bzero(void *dest, size_t nbytes); 
void bcopy(const void *src, void *dest, size_t nbytes); 
int bcmp(const void *ptr1, const void *ptr2, size_t nbytes);
// Returns: 0 if equal, nonzero if unequal

#include <string.h>
void *memset(void *dest, int c, size_t len);
void *memcpy(void *dest, const void *src, size_t nbytes);
int memcmp(const void *ptr1, const void *ptr2, size_t nbytes);
//Returns: 0 if equal, <0 or >0 if unequal (see text)
```

```c++
// 地址转换函数
#include <arpa/inet.h>
int inet_aton(const char *strptr, struct in_addr *addrptr);
//Returns: 1 if string was valid, 0 on error
in_addr_t inet_addr(const char *strptr);
//Returns: 32-bit binary network byte ordered IPv4 address; INADDR_NONE if error
char *inet_ntoa(struct in_addr inaddr);
//Returns: pointer to dotted-decimal string

// IPv4 IPv6都适用
#include <arpa/inet.h>
int inet_pton(int family, const char *strptr, void *addrptr);
// Returns: 1 if OK, 0 if input not a valid presentation format, -1 on error
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
// Returns: pointer to result if OK, NULL on error

#define INET_ADDRSTRLEN 16 /* for IPv4 dotted-decimal */ 
#define INET6_ADDRSTRLEN 46 /* for IPv6 hex string */
```

```c++
// 协议无关的地址函数 lib/sock_ntop.c
#include	"unp.h"

#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif

/* include sock_ntop */
char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char		portstr[8];
    static char str[128];		/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
			strcat(str, portstr);
		}
		return(str);
	}
/* end sock_ntop */

#ifdef	IPV6
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		str[0] = '[';
		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
			return(NULL);
		if (ntohs(sin6->sin6_port) != 0) {
			snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
			strcat(str, portstr);
			return(str);
		}
		return (str + 1);
	}
#endif

#ifdef	AF_UNIX
	case AF_UNIX: {
		struct sockaddr_un	*unp = (struct sockaddr_un *) sa;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
			strcpy(str, "(no pathname bound)");
		else
			snprintf(str, sizeof(str), "%s", unp->sun_path);
		return(str);
	}
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
	case AF_LINK: {
		struct sockaddr_dl	*sdl = (struct sockaddr_dl *) sa;

		if (sdl->sdl_nlen > 0)
			snprintf(str, sizeof(str), "%*s (index %d)",
					 sdl->sdl_nlen, &sdl->sdl_data[0], sdl->sdl_index);
		else
			snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
		return(str);
	}
#endif
	default:
		snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
				 sa->sa_family, salen);
		return(str);
	}
    return (NULL);
}

char *
Sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	char	*ptr;

	if ( (ptr = sock_ntop(sa, salen)) == NULL)
		err_sys("sock_ntop error");	/* inet_ntop() sets errno */
	return(ptr);
}

```

```c++
// 通用的地址函数
#include "unp.h"
int sock_bind_wild(int sockfd, int family);
//  Returns: 0 if OK, -1 on error
int sock_cmp_addr(const struct sockaddr *sockaddr1,
                  const struct sockaddr *sockaddr2, socklen_t addrlen);
 // Returns: 0 if addresses are of the same family and ports are equal, else nonzero
 
 
int sock_cmp_port(const struct sockaddr *sockaddr1,
                  const struct sockaddr *sockaddr2, socklen_t addrlen);
//Returns: 0 if addresses are of the same family and ports are equal, else nonzero 
int sock_get_port(const struct sockaddr *sockaddr, socklen_t addrlen);
//Returns: non-negative port number for IPv4 or IPv6 address, else -1
char *sock_ntop_host(const struct sockaddr *sockaddr, socklen_t addrlen);
//Returns: non-null pointer if OK, NULL on error
void sock_set_addr(const struct sockaddr *sockaddr, socklen_t addrlen, void *ptr);
void sock_set_port(const struct sockaddr *sockaddr, socklen_t addrlen, int port);
void sock_set_wild(struct sockaddr *sockaddr, socklen_t addrlen);
```

```c++
// lib/readn.c
/* include readn */
#include	"unp.h"

ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft);		/* return >= 0 */
}
/* end readn */

ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = readn(fd, ptr, nbytes)) < 0)
		err_sys("readn error");
	return(n);
}

```

```c++
// lib/writen.c
/* include writen */
#include	"unp.h"

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}

```

```c++
// lib/readline.c
/* include readline */
#include	"unp.h"

static int	read_cnt;
static char	*read_ptr;
static char	read_buf[MAXLINE];

static ssize_t
my_read(int fd, char *ptr)
{

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

ssize_t
readlinebuf(void **vptrptr)
{
	if (read_cnt)
		*vptrptr = read_ptr;
	return(read_cnt);
}
/* end readline */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}

```

