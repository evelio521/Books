##Chapter-18 路由套接字

----

### 概述

1. 原始套接字支持路由套接字。
2. 进程可以通过写出到路由套接字往内核发送消息。 Root
3. 进程可以通过从路由套接字读入自内核接收消息。Root
4. 进程可以使用sysctl函数倾泻出路由表或列出所有已配置的接口。 Any

```c++
/* include getrt1 */
#include	"unproute.h"
// 通过路由套接字发出RTM_GET命令
#define	BUFLEN	(sizeof(struct rt_msghdr) + 512)
					/* sizeof(struct sockaddr_in6) * 8 = 192 */
#define	SEQ		9999

int
main(int argc, char **argv)
{
	int					sockfd;
	char				*buf;
	pid_t				pid;
	ssize_t				n;
	struct rt_msghdr	*rtm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_in	*sin;

	if (argc != 2)
		err_quit("usage: getrt <IPaddress>");

	sockfd = Socket(AF_ROUTE, SOCK_RAW, 0);	/* need superuser privileges */

	buf = Calloc(1, BUFLEN);	/* and initialized to 0 */

	rtm = (struct rt_msghdr *) buf;
	rtm->rtm_msglen = sizeof(struct rt_msghdr) + sizeof(struct sockaddr_in);
	rtm->rtm_version = RTM_VERSION;
	rtm->rtm_type = RTM_GET;
	rtm->rtm_addrs = RTA_DST;
	rtm->rtm_pid = pid = getpid();
	rtm->rtm_seq = SEQ;

	sin = (struct sockaddr_in *) (rtm + 1);
	sin->sin_len = sizeof(struct sockaddr_in);
	sin->sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &sin->sin_addr);

	Write(sockfd, rtm, rtm->rtm_msglen);

	do {
		n = Read(sockfd, rtm, BUFLEN);
	} while (rtm->rtm_type != RTM_GET || rtm->rtm_seq != SEQ ||
			 rtm->rtm_pid != pid);
/* end getrt1 */

/* include getrt2 */
	rtm = (struct rt_msghdr *) buf;
	sa = (struct sockaddr *) (rtm + 1);
	get_rtaddrs(rtm->rtm_addrs, sa, rti_info);
	if ( (sa = rti_info[RTAX_DST]) != NULL)
		printf("dest: %s\n", Sock_ntop_host(sa, sa->sa_len));

	if ( (sa = rti_info[RTAX_GATEWAY]) != NULL)
		printf("gateway: %s\n", Sock_ntop_host(sa, sa->sa_len));

	if ( (sa = rti_info[RTAX_NETMASK]) != NULL)
		printf("netmask: %s\n", Sock_masktop(sa, sa->sa_len));

	if ( (sa = rti_info[RTAX_GENMASK]) != NULL)
		printf("genmask: %s\n", Sock_masktop(sa, sa->sa_len));

	exit(0);
}
/* end getrt2 */

```

```c++
#include	"unproute.h"

/*
 * Round up 'a' to next multiple of 'size', which must be a power of 2
 */
#define ROUNDUP(a, size) (((a) & ((size)-1)) ? (1 + ((a) | ((size)-1))) : (a))

/*
 * Step to next socket address structure;
 * if sa_len is 0, assume it is sizeof(u_long).
 */
#define NEXT_SA(ap)	ap = (SA *) \
	((caddr_t) ap + (ap->sa_len ? ROUNDUP(ap->sa_len, sizeof (u_long)) : \
									sizeof(u_long)))

void
get_rtaddrs(int addrs, SA *sa, SA **rti_info)
{
	int		i;

	for (i = 0; i < RTAX_MAX; i++) {
		if (addrs & (1 << i)) {
			rti_info[i] = sa;
			NEXT_SA(sa);
		} else
			rti_info[i] = NULL;
	}
}

```

````c++
#include	"unproute.h"

const char *
sock_masktop(SA *sa, socklen_t salen)
{
	static char		str[INET6_ADDRSTRLEN];
	unsigned char	*ptr = &sa->sa_data[2];

	if (sa->sa_len == 0)
		return("0.0.0.0");
	else if (sa->sa_len == 5)
		snprintf(str, sizeof(str), "%d.0.0.0", *ptr);
	else if (sa->sa_len == 6)
		snprintf(str, sizeof(str), "%d.%d.0.0", *ptr, *(ptr+1));
	else if (sa->sa_len == 7)
		snprintf(str, sizeof(str), "%d.%d.%d.0", *ptr, *(ptr+1), *(ptr+2));
	else if (sa->sa_len == 8)
		snprintf(str, sizeof(str), "%d.%d.%d.%d",
				 *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
	else
		snprintf(str, sizeof(str), "(unknown mask, len = %d, family = %d)",
				 sa->sa_len, sa->sa_family);
	return(str);
}

````



### sysctl

```c++
#include	"unproute.h"
#include	<netinet/udp.h>
#include	<netinet/ip_var.h>
#include	<netinet/udp_var.h>		/* for UDPCTL_xxx constants */
// 检查UDP校验和是否开始
int
main(int argc, char **argv)
{
	int		mib[4], val;
	size_t	len;

	mib[0] = CTL_NET;
	mib[1] = AF_INET;
	mib[2] = IPPROTO_UDP;
	mib[3] = UDPCTL_CHECKSUM;

	len = sizeof(val);
	Sysctl(mib, 4, &val, &len, NULL, 0);
	printf("udp checksum flag: %d\n", val);

	exit(0);
}

```



```c++
/* include net_rt_iflist */
#include	"unproute.h"
// 调用sysctl返回制定地址族接口列表
char *
net_rt_iflist(int family, int flags, size_t *lenp)
{
	int		mib[6];
	char	*buf;

	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = family;		/* only addresses of this family */
	mib[4] = NET_RT_IFLIST;
	mib[5] = flags;			/* interface index or 0 */
	if (sysctl(mib, 6, NULL, lenp, NULL, 0) < 0)
		return(NULL);

	if ( (buf = malloc(*lenp)) == NULL)
		return(NULL);
	if (sysctl(mib, 6, buf, lenp, NULL, 0) < 0) {
		free(buf);
		return(NULL);
	}

	return(buf);
}
/* end net_rt_iflist */

char *
Net_rt_iflist(int family, int flags, size_t *lenp)
{
	char	*ptr;

	if ( (ptr = net_rt_iflist(family, flags, lenp)) == NULL)
		err_sys("net_rt_iflist error");
	return(ptr);
}

```



```c++
#include	"unpifi.h"
#include	"unproute.h"
// 返回所有在工的接口
/* include get_ifi_info1 */
struct ifi_info *
get_ifi_info(int family, int doaliases)
{
	int 				flags;
	char				*buf, *next, *lim;
	size_t				len;
	struct if_msghdr	*ifm;
	struct ifa_msghdr	*ifam;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;
	struct ifi_info		*ifi, *ifisave, *ifihead, **ifipnext;

	buf = Net_rt_iflist(family, 0, &len);

	ifihead = NULL;
	ifipnext = &ifihead;

	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			if ( ((flags = ifm->ifm_flags) & IFF_UP) == 0)
				continue;	/* ignore if interface not up */

			sa = (struct sockaddr *) (ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				ifi = Calloc(1, sizeof(struct ifi_info));
				*ifipnext = ifi;			/* prev points to this new one */
				ifipnext = &ifi->ifi_next;	/* ptr to next one goes here */

				ifi->ifi_flags = flags;
				if (sa->sa_family == AF_LINK) {
					sdl = (struct sockaddr_dl *) sa;
					ifi->ifi_index = sdl->sdl_index;
					if (sdl->sdl_nlen > 0)
						snprintf(ifi->ifi_name, IFI_NAME, "%*s",
								 sdl->sdl_nlen, &sdl->sdl_data[0]);
					else
						snprintf(ifi->ifi_name, IFI_NAME, "index %d",
								 sdl->sdl_index);

					if ( (ifi->ifi_hlen = sdl->sdl_alen) > 0)
						memcpy(ifi->ifi_haddr, LLADDR(sdl),
							   min(IFI_HADDR, sdl->sdl_alen));
				}
			}
/* end get_ifi_info1 */

/* include get_ifi_info3 */
		} else if (ifm->ifm_type == RTM_NEWADDR) {
			if (ifi->ifi_addr) {	/* already have an IP addr for i/f */
				if (doaliases == 0)
					continue;

					/* 4we have a new IP addr for existing interface */
				ifisave = ifi;
				ifi = Calloc(1, sizeof(struct ifi_info));
				*ifipnext = ifi;			/* prev points to this new one */
				ifipnext = &ifi->ifi_next;	/* ptr to next one goes here */
				ifi->ifi_flags = ifisave->ifi_flags;
				ifi->ifi_index = ifisave->ifi_index;
				ifi->ifi_hlen = ifisave->ifi_hlen;
				memcpy(ifi->ifi_name, ifisave->ifi_name, IFI_NAME);
				memcpy(ifi->ifi_haddr, ifisave->ifi_haddr, IFI_HADDR);
			}

			ifam = (struct ifa_msghdr *) next;
			sa = (struct sockaddr *) (ifam + 1);
			get_rtaddrs(ifam->ifam_addrs, sa, rti_info);

			if ( (sa = rti_info[RTAX_IFA]) != NULL) {
				ifi->ifi_addr = Calloc(1, sa->sa_len);
				memcpy(ifi->ifi_addr, sa, sa->sa_len);
			}

			if ((flags & IFF_BROADCAST) &&
				(sa = rti_info[RTAX_BRD]) != NULL) {
				ifi->ifi_brdaddr = Calloc(1, sa->sa_len);
				memcpy(ifi->ifi_brdaddr, sa, sa->sa_len);
			}

			if ((flags & IFF_POINTOPOINT) &&
				(sa = rti_info[RTAX_BRD]) != NULL) {
				ifi->ifi_dstaddr = Calloc(1, sa->sa_len);
				memcpy(ifi->ifi_dstaddr, sa, sa->sa_len);
			}

		} else
			err_quit("unexpected message type %d", ifm->ifm_type);
	}
	/* "ifihead" points to the first structure in the linked list */
	return(ifihead);	/* ptr to first structure in linked list */
}
/* end get_ifi_info3 */

void
free_ifi_info(struct ifi_info *ifihead)
{
	struct ifi_info	*ifi, *ifinext;

	for (ifi = ifihead; ifi != NULL; ifi = ifinext) {
		if (ifi->ifi_addr != NULL)
			free(ifi->ifi_addr);
		if (ifi->ifi_brdaddr != NULL)
			free(ifi->ifi_brdaddr);
		if (ifi->ifi_dstaddr != NULL)
			free(ifi->ifi_dstaddr);
		ifinext = ifi->ifi_next;		/* can't fetch ifi_next after free() */
		free(ifi);					/* the ifi_info{} itself */
	}
}

struct ifi_info *
Get_ifi_info(int family, int doaliases)
{
	struct ifi_info	*ifi;

	if ( (ifi = get_ifi_info(family, doaliases)) == NULL)
		err_quit("get_ifi_info error");
	return(ifi);
}

```



### 接口名字和索引函数

```c++
/* include if_nameindex */
#include	"unpifi.h"
#include	"unproute.h"
// 返回指向if_nameindex结构数组的指针
struct if_nameindex *
if_nameindex(void)
{
	char				*buf, *next, *lim;
	size_t				len;
	struct if_msghdr	*ifm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;
	struct if_nameindex	*result, *ifptr;
	char				*namptr;

	if ( (buf = net_rt_iflist(0, 0, &len)) == NULL)
		return(NULL);

	if ( (result = malloc(len)) == NULL)	/* overestimate */
		return(NULL);
	ifptr = result;
	namptr = (char *) result + len;	/* names start at end of buffer */

	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			sa = (struct sockaddr *) (ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				if (sa->sa_family == AF_LINK) {
					sdl = (struct sockaddr_dl *) sa;
					namptr -= sdl->sdl_nlen + 1;
					strncpy(namptr, &sdl->sdl_data[0], sdl->sdl_nlen);
					namptr[sdl->sdl_nlen] = 0;	/* null terminate */
					ifptr->if_name = namptr;
					ifptr->if_index = sdl->sdl_index;
					ifptr++;
				}
			}

		}
	}
	ifptr->if_name = NULL;	/* mark end of array of structs */
	ifptr->if_index = 0;
	free(buf);
	return(result);			/* caller must free() this when done */
}
/* end if_nameindex */

/* include if_freenameindex */
void
if_freenameindex(struct if_nameindex *ptr)
{
	free(ptr);
}
/* end if_freenameindex */

struct if_nameindex *
If_nameindex(void)
{
	struct if_nameindex	*ifptr;

	if ( (ifptr = if_nameindex()) == NULL)
		err_quit("if_nameindex error");
	return(ifptr);
}

```



```c++
/* include if_indextoname */
#include	"unpifi.h"
#include	"unproute.h"
// 指向接口名字的指针
char *
if_indextoname(unsigned int idx, char *name)
{
	char				*buf, *next, *lim;
	size_t				len;
	struct if_msghdr	*ifm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;

	if ( (buf = net_rt_iflist(0, idx, &len)) == NULL)
		return(NULL);

	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			sa = (struct sockaddr *) (ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				if (sa->sa_family == AF_LINK) {
					sdl = (struct sockaddr_dl *) sa;
					if (sdl->sdl_index == idx) {
						int slen = min(IFNAMSIZ - 1, sdl->sdl_nlen);
						strncpy(name, sdl->sdl_data, slen);
						name[slen] = 0;	/* null terminate */
						free(buf);
						return(name);
					}
				}
			}

		}
	}
	free(buf);
	return(NULL);		/* no match for index */
}
/* end if_indextoname */

char *
If_indextoname(unsigned int idx, char *name)
{
	char	*ptr;

	if ( (ptr = if_indextoname(idx, name)) == NULL)
		err_quit("if_indextoname error for %d", idx);
	return(ptr);
}

```



```c++
/* include if_nametoindex */
#include	"unpifi.h"
#include	"unproute.h"
// 返回接口索引
unsigned int
if_nametoindex(const char *name)
{
	unsigned int		idx, namelen;
	char				*buf, *next, *lim;
	size_t				len;
	struct if_msghdr	*ifm;
	struct sockaddr		*sa, *rti_info[RTAX_MAX];
	struct sockaddr_dl	*sdl;

	if ( (buf = net_rt_iflist(0, 0, &len)) == NULL)
		return(0);

	namelen = strlen(name);
	lim = buf + len;
	for (next = buf; next < lim; next += ifm->ifm_msglen) {
		ifm = (struct if_msghdr *) next;
		if (ifm->ifm_type == RTM_IFINFO) {
			sa = (struct sockaddr *) (ifm + 1);
			get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
			if ( (sa = rti_info[RTAX_IFP]) != NULL) {
				if (sa->sa_family == AF_LINK) {
					sdl = (struct sockaddr_dl *) sa;
					if (sdl->sdl_nlen == namelen && strncmp(&sdl->sdl_data[0], name, sdl->sdl_nlen) == 0) {
						idx = sdl->sdl_index;	/* save before free() */
						free(buf);
						return(idx);
					}
				}
			}

		}
	}
	free(buf);
	return(0);		/* no match for name */
}
/* end if_nametoindex */

unsigned int
If_nametoindex(const char *name)
{
	int		idx;

	if ( (idx = if_nametoindex(name)) == 0)
		err_quit("if_nametoindex error for %s", name);
	return(idx);
}

```



```c++

```

