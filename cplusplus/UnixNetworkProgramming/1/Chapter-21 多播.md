Chapter-21 多播

----

### 概述

1. IPv4多播地址224.0.0.0 到239.255.255.255为多播地址。
2. 局部多播地址：在224.0.0.0～224.0.0.255之间，这是为路由协议和其他用途保留的地址，路由器并不转发属于此范围的IP包。
3. 预留多播地址：在224.0.1.0～238.255.255.255之间，可用于全球范围（如Internet）或网络协议。
4. 管理权限多播地址：在239.0.0.0～239.255.255.255之间，可供组织内部使用，类似于私有IP地址，不能用于Internet，可限制多播范围。
5. 224.0.0.1所有主机地址。
6. 224.0.0.2 所有路由器。
7. 多播地址到硬件地址不是一对一的。
8. 在广域网上，某个主机加入多播，就会向所有直接连接多播路由器发送IGMP消息，多播路由器使用多播路由协议MRP和相邻的路由器交换这些信息。
9. 源特定多播SSM把应用系统原地址结合到组地址上，解决了多播难以部署的问题。

### 多播套接字选项

1. **IP_ADD_MEMBERSHIP, IPV6_JOIN_GROUP, MCAST_JOIN_GROUP**
2. **IP_DROP_MEMBERSHIP, IPV6_LEAVE_GROUP, MCAST_LEAVE_GROUP**
3. **IP_BLOCK_SOURCE, MCAST_BLOCK_SOURCE**
4. **IP_UNBLOCK_SOURCE, MCAST_UNBLOCK_SOURCE**
5. **IP_ADD_SOURCE_MEMBERSHIP, MCAST_JOIN_SOURCE_GROUP**
6. **IP_DROP_SOURCE_MEMBERSHIP, MCAST_LEAVE_SOURCE_GROUP**
7. **IP_MULTICAST_IF, IPV6_MULTICAST_IF**
8. **IP_MULTICAST_TTL, IPV6_MULTICAST_HOPS**
9. **IP_MULTICAST_LOOP, IPV6_MULTICAST_LOOP**

### mcast_get_if.c

```c++
#include	"unp.h"

int
mcast_get_if(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		/* TODO: similar to mcast_set_if() */
		return(-1);
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int		idx;
		socklen_t	len;

		len = sizeof(idx);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
					   &idx, &len) < 0)
			return(-1);
		return(idx);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

int
Mcast_get_if(int sockfd)
{
	int		rc;

	if ( (rc = mcast_get_if(sockfd)) < 0)
		err_sys("mcast_get_if error");
	return(rc);
}

```



### mcast_get_loop.c

```c++
#include	"unp.h"

int
mcast_get_loop(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

int
Mcast_get_loop(int sockfd)
{
	int		rc;

	if ( (rc = mcast_get_loop(sockfd)) < 0)
		err_sys("mcast_get_loop error");
	return(rc);
}

```



### mcast_get_ttl.c

```c++
#include	"unp.h"

int
mcast_get_ttl(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		ttl;
		socklen_t	len;

		len = sizeof(ttl);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,
					   &ttl, &len) < 0)
			return(-1);
		return(ttl);
	}

#ifdef	IPV6
	case AF_INET6: {
		int			hop;
		socklen_t	len;

		len = sizeof(hop);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
					   &hop, &len) < 0)
			return(-1);
		return(hop);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

int
Mcast_get_ttl(int sockfd)
{
	int		rc;

	if ( (rc = mcast_get_ttl(sockfd)) < 0)
		err_sys("mcast_get_ttl error");
	return(rc);
}

```



### mcast_join.c

```c++
/* include mcast_join1 */
#include	"unp.h"
#include	<net/if.h>

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;
	if (ifindex > 0) {
		req.gr_interface = ifindex;
	} else if (ifname != NULL) {
		if ( (req.gr_interface = if_nametoindex(ifname)) == 0) {
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} else
		req.gr_interface = 0;
	if (grplen > sizeof(req.gr_group)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_JOIN_GROUP, &req, sizeof(req)));
#else
/* end mcast_join1 */

/* include mcast_join2 */
	switch (grp->sa_family) {
	case AF_INET: {
		struct ip_mreq		mreq;
		struct ifreq		ifreq;

		memcpy(&mreq.imr_multiaddr,
			   &((const struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&mreq.imr_interface,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
/* end mcast_join2 */

/* include mcast_join3 */
#ifdef	IPV6
#ifndef	IPV6_JOIN_GROUP		/* APIv0 compatibility */
#define	IPV6_JOIN_GROUP		IPV6_ADD_MEMBERSHIP
#endif
	case AF_INET6: {
		struct ipv6_mreq	mreq6;

		memcpy(&mreq6.ipv6mr_multiaddr,
			   &((const struct sockaddr_in6 *) grp)->sin6_addr,
			   sizeof(struct in6_addr));

		if (ifindex > 0) {
			mreq6.ipv6mr_interface = ifindex;
		} else if (ifname != NULL) {
			if ( (mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0) {
				errno = ENXIO;	/* i/f name not found */
				return(-1);
			}
		} else
			mreq6.ipv6mr_interface = 0;

		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
						  &mreq6, sizeof(mreq6)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}
/* end mcast_join3 */

void
Mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex)
{
	if (mcast_join(sockfd, grp, grplen, ifname, ifindex) < 0)
		err_sys("mcast_join error");
}

int
mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen,
						const char *ifname, u_int ifindex)
{
#ifdef MCAST_JOIN_SOURCE_GROUP
	struct group_source_req req;
	if (ifindex > 0) {
		req.gsr_interface = ifindex;
	} else if (ifname != NULL) {
		if ( (req.gsr_interface = if_nametoindex(ifname)) == 0) {
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} else
		req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_JOIN_SOURCE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_ADD_SOURCE_MEMBERSHIP
	case AF_INET: {
		struct ip_mreq_source	mreq;
		struct ifreq			ifreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&mreq.imr_interface,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_JOIN_SOURCE_GROUP */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen,
						const char *ifname, u_int ifindex)
{
	if (mcast_join_source_group(sockfd, src, srclen, grp, grplen,
								ifname, ifindex) < 0)
		err_sys("mcast_join_source_group error");
}

int
mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_BLOCK_SOURCE
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_BLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_BLOCK_SOURCE
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_BLOCK_SOURCE,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_BLOCK_SOURCE */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_block_source(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_block_source error");
}

int
mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_UNBLOCK_SOURCE
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_UNBLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_UNBLOCK_SOURCE
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_UNBLOCK_SOURCE,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_UNBLOCK_SOURCE */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_unblock_source(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_unblock_source error");
}

```



### mcast_leave.c

```c++
#include	"unp.h"

int
mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;
	req.gr_interface = 0;
	if (grplen > sizeof(req.gr_group)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_LEAVE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
	case AF_INET: {
		struct ip_mreq		mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((const struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}

#ifdef	IPV6
#ifndef	IPV6_LEAVE_GROUP	/* APIv0 compatibility */
#define	IPV6_LEAVE_GROUP	IPV6_DROP_MEMBERSHIP
#endif
	case AF_INET6: {
		struct ipv6_mreq	mreq6;

		memcpy(&mreq6.ipv6mr_multiaddr,
			   &((const struct sockaddr_in6 *) grp)->sin6_addr,
			   sizeof(struct in6_addr));
		mreq6.ipv6mr_interface = 0;
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_LEAVE_GROUP,
						  &mreq6, sizeof(mreq6)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
	if (mcast_leave(sockfd, grp, grplen) < 0)
		err_sys("mcast_leave error");
}

int
mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
#ifdef MCAST_LEAVE_SOURCE_GROUP
	struct group_source_req req;
	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) {
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, family_to_level(grp->sa_family),
			MCAST_LEAVE_SOURCE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) {
#ifdef IP_DROP_SOURCE_MEMBERSHIP
	case AF_INET: {
		struct ip_mreq_source	mreq;

		memcpy(&mreq.imr_multiaddr,
			   &((struct sockaddr_in *) grp)->sin_addr,
			   sizeof(struct in_addr));
		memcpy(&mreq.imr_sourceaddr,
			   &((struct sockaddr_in *) src)->sin_addr,
			   sizeof(struct in_addr));
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP,
						  &mreq, sizeof(mreq)));
	}
#endif

#ifdef	IPV6
	case AF_INET6: /* IPv6 source-specific API is MCAST_LEAVE_SOURCE_GROUP */
#endif
	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

void
Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
						const SA *grp, socklen_t grplen)
{
	if (mcast_leave_source_group(sockfd, src, srclen, grp, grplen) < 0)
		err_sys("mcast_leave_source_group error");
}

```



### mcast_set_if.c

```c++
#include	"unp.h"
#include	<net/if.h>

int
mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		struct in_addr		inaddr;
		struct ifreq		ifreq;

		if (ifindex > 0) {
			if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
				errno = ENXIO;	/* i/f index not found */
				return(-1);
			}
			goto doioctl;
		} else if (ifname != NULL) {
			strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
			if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				return(-1);
			memcpy(&inaddr,
				   &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
				   sizeof(struct in_addr));
		} else
			inaddr.s_addr = htonl(INADDR_ANY);	/* remove prev. set default */

		return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF,
						  &inaddr, sizeof(struct in_addr)));
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int	idx;

		if ( (idx = ifindex) == 0) {
			if (ifname == NULL) {
				errno = EINVAL;	/* must supply either index or name */
				return(-1);
			}
			if ( (idx = if_nametoindex(ifname)) == 0) {
				errno = ENXIO;	/* i/f name not found */
				return(-1);
			}
		}
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
						  &idx, sizeof(idx)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

void
Mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	if (mcast_set_if(sockfd, ifname, ifindex) < 0)
		err_sys("mcast_set_if error");
}

```



### mcast_set_loop.c

```c++
/* include mcast_set_loop */
#include	"unp.h"

int
mcast_set_loop(int sockfd, int onoff)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		flag;

		flag = onoff;
		return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,
						  &flag, sizeof(flag)));
	}

#ifdef	IPV6
	case AF_INET6: {
		u_int		flag;

		flag = onoff;
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
						  &flag, sizeof(flag)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}
/* end mcast_set_loop */

void
Mcast_set_loop(int sockfd, int onoff)
{
	if (mcast_set_loop(sockfd, onoff) < 0)
		err_sys("mcast_set_loop error");
}

```



###mcast_set_ttl.c

```c++
#include	"unp.h"

int
mcast_set_ttl(int sockfd, int val)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		ttl;

		ttl = val;
		return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,
						  &ttl, sizeof(ttl)));
	}

#ifdef	IPV6
	case AF_INET6: {
		int		hop;

		hop = val;
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
						  &hop, sizeof(hop)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

void
Mcast_set_ttl(int sockfd, int val)
{
	if (mcast_set_ttl(sockfd, val) < 0)
		err_sys("mcast_set_ttl error");
}

```



### 接收IP多播基础设置会话声明

### main.c

```c++
#include	"unp.h"

#define	SAP_NAME	"sap.mcast.net"	/* default group name and port */
#define	SAP_PORT	"9875"

void	loop(int, socklen_t);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sa;

	if (argc == 1)
		sockfd = Udp_client(SAP_NAME, SAP_PORT, (void **) &sa, &salen);
	else if (argc == 4)
		sockfd = Udp_client(argv[1], argv[2], (void **) &sa, &salen);
	else
		err_quit("usage: mysdr <mcast-addr> <port#> <interface-name>");

	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(sockfd, sa, salen);

	Mcast_join(sockfd, sa, salen, (argc == 4) ? argv[3] : NULL, 0);

	loop(sockfd, salen);	/* receive and print */

	exit(0);
}

```



### loop.c

```c++
#include	"mysdr.h"

void
loop(int sockfd, socklen_t salen)
{
	socklen_t		len;
	ssize_t			n;
	char			*p;
	struct sockaddr	*sa;
	struct sap_packet {
	  uint32_t	sap_header;
	  uint32_t	sap_src;
	  char		sap_data[BUFFSIZE];
	} buf;

	sa = Malloc(salen);

	for ( ; ; ) {
		len = salen;
		n = Recvfrom(sockfd, &buf, sizeof(buf) - 1, 0, sa, &len);
		((char *)&buf)[n] = 0;			/* null terminate */
		buf.sap_header = ntohl(buf.sap_header);

		printf("From %s hash 0x%04x\n", Sock_ntop(sa, len),
				buf.sap_header & SAP_HASH_MASK);
		if (((buf.sap_header & SAP_VERSION_MASK) >> SAP_VERSION_SHIFT) > 1) {
			err_msg("... version field not 1 (0x%08x)", buf.sap_header);
			continue;
		}
		if (buf.sap_header & SAP_IPV6) {
			err_msg("... IPv6");
			continue;
		}
		if (buf.sap_header & (SAP_DELETE|SAP_ENCRYPTED|SAP_COMPRESSED)) {
			err_msg("... can't parse this packet type (0x%08x)", buf.sap_header);
			continue;
		}
		p = buf.sap_data + ((buf.sap_header & SAP_AUTHLEN_MASK)
							>> SAP_AUTHLEN_SHIFT);
		if (strcmp(p, "application/sdp") == 0)
			p += 16;
		printf("%s\n", p);
	}
}

```



### 发送和接收多播的程序

### main.c

```c++
#include	"unp.h"

void	recv_all(int, socklen_t);
void	send_all(int, SA *, socklen_t);

int
main(int argc, char **argv)
{
	int					sendfd, recvfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sasend, *sarecv;

	if (argc != 3)
		err_quit("usage: sendrecv <IP-multicast-address> <port#>");

	sendfd = Udp_client(argv[1], argv[2], (void **) &sasend, &salen);

	recvfd = Socket(sasend->sa_family, SOCK_DGRAM, 0);

	Setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	sarecv = Malloc(salen);
	memcpy(sarecv, sasend, salen);
	Bind(recvfd, sarecv, salen);

	Mcast_join(recvfd, sasend, salen, NULL, 0);
	Mcast_set_loop(sendfd, 0);

	if (Fork() == 0)
		recv_all(recvfd, salen);		/* child -> receives */

	send_all(sendfd, sasend, salen);	/* parent -> sends */
}

```

### send.c

```c++
#include	"unp.h"
#include	<sys/utsname.h>

#define	SENDRATE	5		/* send one datagram every five seconds */

void
send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char		line[MAXLINE];		/* hostname and process ID */
	struct utsname	myname;

	if (uname(&myname) < 0)
		err_sys("uname error");;
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

	for ( ; ; ) {
		Sendto(sendfd, line, strlen(line), 0, sadest, salen);

		sleep(SENDRATE);
	}
}

```



### recv.c

```c++
#include	"unp.h"

void
recv_all(int recvfd, socklen_t salen)
{
	int					n;
	char				line[MAXLINE+1];
	socklen_t			len;
	struct sockaddr		*safrom;

	safrom = Malloc(salen);

	for ( ; ; ) {
		len = salen;
		n = Recvfrom(recvfd, line, MAXLINE, 0, safrom, &len);

		line[n] = 0;	/* null terminate */
		printf("from %s: %s", Sock_ntop(safrom, len), line);
	}
}

```

