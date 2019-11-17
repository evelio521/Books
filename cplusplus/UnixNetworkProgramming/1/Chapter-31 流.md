## Chapter-31 流

----

### 基本知识点

1. XTI(X/Open Transport Interface)是独立于套接字API的另一套网络编程API。
2. 流在进程和驱动程序提供全双工连接。
3. 流头是有一些内核例程构成，应用程序针对流描述符执行系统调用时这些内核例程被激活。
4. 流消息可分为高优先级、优先级带和普通三类。255-[254,1]-0。
5.  ioctl是设备驱动程序中对设备的I/O通道进行管理的函数。
6. TPI(Transport Provider Interface) 传输提供者接口。
7. NPI(NetWork Provider Interface) 网络提供者接口。
8. XTI和套接字均使用TPI实现。



|  函数  | 控制 | :-:  |    :-:    |    :-:    |
| :----: | :--: | :--: | :-------: | :-------: |
| write  |      |  是  |           |  M_DATA   |
| putmsg | 不是 |  是  |     0     |  M_DATA   |
| putmsg |  是  | 无关 |     0     |  M_PROTO  |
| putmsg |  是  | 无关 | MSG_HIPRI | M_PCPROTO |

​                                                            由write和putmsg产生的流消息类型





-------

###TPI实现时间获取客户端的主要函数

```c++
#include <stropts.h>
 
 
int getmsg(int fd, struct strbuf *ctlptr, struct strbuf *dataptr, int *flagsp) ;
 
 
int putmsg(int fd, const struct strbuf *ctlptr, const struct strbuf *dataptr, int flags) ;
 
 
Both return: non-negative value if OK (see text), –1 on error
```



```c++
#include <stropts.h>
 
 
int getpmsg(int fd, struct strbuf *ctlptr, struct strbuf *dataptr, int *bandp, int *flagsp) ;
 
 
int putpmsg(int fd, const struct strbuf *ctlptr, const struct strbuf *dataptr, int band, int flags) ;
 
 
 Both return: non-negative value if OK, –1 on error
```



```c++
void
tpi_bind(int fd, const void *addr, size_t addrlen) {
	struct {
	  struct T_bind_req	msg_hdr;
	  char				addr[128];
	} bind_req;
	struct {
	  struct T_bind_ack	msg_hdr;
	  char				addr[128];
	} bind_ack;
	struct strbuf		ctlbuf;
	struct T_error_ack	*error_ack;
	int					flags;

  // 填充bind_req 
	bind_req.msg_hdr.PRIM_type = T_BIND_REQ;
	bind_req.msg_hdr.ADDR_length = addrlen;
	bind_req.msg_hdr.ADDR_offset = sizeof(struct T_bind_req);
	bind_req.msg_hdr.CONIND_number = 0; /* 客户端设置为0 */
	memcpy(bind_req.addr, addr, addrlen);	/* sockaddr_in{} */
  
  // TPI要求我们把刚构造的结构作为一个M_PROTO消息传给提供者
	ctlbuf.len = sizeof(struct T_bind_req) + addrlen;
	ctlbuf.buf = (char *) &bind_req;
	Putmsg(fd, &ctlbuf, NULL, 0);

  // 对于T_BIND_REQ请求或者是T_BIND_ACK， 或者是T_ERROR_ACK消息
	ctlbuf.maxlen = sizeof(bind_ack);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &bind_ack;
	flags = RS_HIPRI;
	Getmsg(fd, &ctlbuf, NULL, &flags);

	if (ctlbuf.len < (int) sizeof(long)) {
		err_quit("bad length from getmsg");
	}

	switch(bind_ack.msg_hdr.PRIM_type) {
    case T_BIND_ACK:
		  return; /* Success */

    case T_ERROR_ACK:
		  if (ctlbuf.len < (int) sizeof(struct T_error_ack)) {
			  err_quit("bad length for T_ERROR_ACK");
			}
		  error_ack = (struct T_error_ack *) &bind_ack.msg_hdr;
		  err_quit("T_ERROR_ACK from bind (%d, %d)",
				 error_ack->TLI_error, error_ack->UNIX_error);

    default:
		  err_quit("unexpected message type: %d", bind_ack.msg_hdr.PRIM_type);
	}
}

```



```c++
void
tpi_connect(int fd, const void *addr, size_t addrlen) {
	struct {
	  struct T_conn_req	msg_hdr;
	  char				addr[128];
	} conn_req;
	struct {
	  struct T_conn_con	msg_hdr;
	  char				addr[128];
	} conn_con;
	struct strbuf		ctlbuf;
	union T_primitives	rcvbuf;
	struct T_error_ack	*error_ack;
	struct T_discon_ind	*discon_ind;
	int					flags;

  // 填充conn_req
	conn_req.msg_hdr.PRIM_type = T_CONN_REQ;
	conn_req.msg_hdr.DEST_length = addrlen;
	conn_req.msg_hdr.DEST_offset = sizeof(struct T_conn_req);
	conn_req.msg_hdr.OPT_length = 0;
	conn_req.msg_hdr.OPT_offset = 0;
	memcpy(conn_req.addr, addr, addrlen);	/* sockaddr_in{} */

  // TPI要求我们把刚构造的结构作为一个M_PROTO消息传给提供者
	ctlbuf.len = sizeof(struct T_conn_req) + addrlen;
	ctlbuf.buf = (char *) &conn_req;
	Putmsg(fd, &ctlbuf, NULL, 0);

  // 接收T_OK_ACK消息 
	ctlbuf.maxlen = sizeof(union T_primitives);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &rcvbuf;
	flags = RS_HIPRI;
	Getmsg(fd, &ctlbuf, NULL, &flags);

	if (ctlbuf.len < (int) sizeof(long))
		err_quit("tpi_connect: bad length from getmsg");

	switch(rcvbuf.type) {
    case T_OK_ACK:
		  break;

    case T_ERROR_ACK:
		  if (ctlbuf.len < (int) sizeof(struct T_error_ack))
			  err_quit("tpi_connect: bad length for T_ERROR_ACK");
		  error_ack = (struct T_error_ack *) &rcvbuf;
		  err_quit("tpi_connect: T_ERROR_ACK from conn (%d, %d)",
				 error_ack->TLI_error, error_ack->UNIX_error);

    default:
		  err_quit("tpi_connect: unexpected message type: %d", rcvbuf.type);
	}

  // 再次接收消息，T_CONN_CON 表示接收成功
	ctlbuf.maxlen = sizeof(conn_con);
	ctlbuf.len = 0;
	ctlbuf.buf = (char *) &conn_con;
	flags = 0;
	Getmsg(fd, &ctlbuf, NULL, &flags);

	if (ctlbuf.len < (int) sizeof(long))
		err_quit("tpi_connect2: bad length from getmsg");

	switch(conn_con.msg_hdr.PRIM_type) {
    case T_CONN_CON:
		  break;

    case T_DISCON_IND:
		  if (ctlbuf.len < (int) sizeof(struct T_discon_ind))
			  err_quit("tpi_connect2: bad length for T_DISCON_IND");
		  discon_ind = (struct T_discon_ind *) &conn_con.msg_hdr;
		  err_quit("tpi_connect2: T_DISCON_IND from conn (%d)",
				 discon_ind->DISCON_reason);

    default:
		err_quit("tpi_connect2: unexpected message type: %d",
				 conn_con.msg_hdr.PRIM_type);
	}
}

```



```c++
ssize_t
tpi_read(int fd, void *buf, size_t len) {
	struct strbuf		ctlbuf;
	struct strbuf		datbuf;
	union T_primitives	rcvbuf;
	int					flags;

  // getmsg同时读入控制信息和数据
 	ctlbuf.maxlen = sizeof(union T_primitives);
	ctlbuf.buf = (char *) &rcvbuf;

	datbuf.maxlen = len;
	datbuf.buf = buf;
	datbuf.len = 0;

	flags = 0;
	Getmsg(fd, &ctlbuf, &datbuf, &flags);

  /*
   * T_DISCON_IND 表示收到一个断连接请求
   */
	if (ctlbuf.len >= (int) sizeof(long)) {
		if (rcvbuf.type == T_DATA_IND)
			return(datbuf.len); /* 数据消息到达*/
		else if (rcvbuf.type == T_ORDREL_IND)
			return(0); /* 所有分节均被消费，下一个是FIN*/
		else
			err_quit("tpi_read: unexpected type %d", rcvbuf.type);
	} else if (ctlbuf.len == -1)
		return(datbuf.len);
	else
		err_quit("tpi_read: bad length from getmsg");
}

```



```c++
void
tpi_close(int fd) {
	struct T_ordrel_req	ordrel_req;
	struct strbuf		ctlbuf;
 
  /* 构造一个T_ordrel_req 结构并作为M_PROTO消息发送出去*/
	ordrel_req.PRIM_type = T_ORDREL_REQ;

	ctlbuf.len = sizeof(struct T_ordrel_req);
	ctlbuf.buf = (char *) &ordrel_req;
	Putmsg(fd, &ctlbuf, NULL, 0);

	Close(fd);
}

```



```c++
#include	"tpi_daytime.h"

int
main(int argc, char **argv)
{
	int					fd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	myaddr, servaddr;

	if (argc != 2)
		err_quit("usage: tpi_daytime <IPaddress>");

	fd = Open(XTI_TCP, O_RDWR, 0);

	/* bind any local address */
	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	tpi_bind(fd, &myaddr, sizeof(struct sockaddr_in));

	/* fill in server's address */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server */
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	tpi_connect(fd, &servaddr, sizeof(struct sockaddr_in));

	for ( ; ; ) {
		if ( (n = tpi_read(fd, recvline, MAXLINE)) <= 0) {
			if (n == 0)
				break;
			else
				err_sys("tpi_read error");
		}
		recvline[n] = 0;		/* null terminate */
		fputs(recvline, stdout);
	}
	tpi_close(fd);
	exit(0);
}

```

