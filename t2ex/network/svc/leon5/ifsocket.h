/*
 *	T2EX SVC parameter packet
 *
 */

#include <basic.h>
#include <t2ex/socket.h>
#include <sys/str_align.h>
#include "fnsocket.h"

typedef struct {
	int sd;	_align64
	struct sockaddr* addr;	_align64
	socklen_t *addrlen;	_align64
} NET_SO_ACCEPT_PARA;

typedef struct {
	int sd;	_align64
	const struct sockaddr* addr;	_align64
	socklen_t addrlen;	_align64
} NET_SO_BIND_PARA;

typedef struct {
	int sd;	_align64
} NET_SO_CLOSE_PARA;

typedef struct {
	int sd;	_align64
	const struct sockaddr* addr;	_align64
	socklen_t addrlen;	_align64
} NET_SO_CONNECT_PARA;

typedef struct {
	int sd;	_align64
	struct sockaddr* addr;	_align64
	socklen_t* addrlen;	_align64
} NET_SO_GETPEERNAME_PARA;

typedef struct {
	int sd;	_align64
	struct sockaddr* addr;	_align64
	socklen_t* addrlen;	_align64
} NET_SO_GETSOCKNAME_PARA;

typedef struct {
	int sd;	_align64
	int level;	_align64
	int optname;	_align64
	void* optval;	_align64
	socklen_t* optlen;	_align64
} NET_SO_GETSOCKOPT_PARA;

typedef struct {
	int sd;	_align64
	int level;	_align64
	int optname;	_align64
	const void* optval;	_align64
	socklen_t optlen;	_align64
} NET_SO_SETSOCKOPT_PARA;

typedef struct {
	int sd;	_align64
	int backlog;	_align64
} NET_SO_LISTEN_PARA;

typedef struct {
	int sd;	_align64
	void* buf;	_align64
	size_t count;	_align64
} NET_SO_READ_PARA;

typedef struct {
	int sd;	_align64
	void* buf;	_align64
	size_t len;	_align64
	int flags;	_align64
} NET_SO_RECV_PARA;

typedef struct {
	int sd;	_align64
	void* buf;	_align64
	size_t len;	_align64
	int flags;	_align64
	struct sockaddr* src_addr;	_align64
	socklen_t* addrlen;	_align64
} NET_SO_RECVFROM_PARA;

typedef struct {
	int sd;	_align64
	struct msghdr* msg;	_align64
	int flags;	_align64
} NET_SO_RECVMSG_PARA;

typedef struct {
	int nfds;	_align64
	fd_set* readfds;	_align64
	fd_set* writefds;	_align64
	fd_set* exceptfds;	_align64
	TMO_U tmout_u;	_align64
} NET_SO_SELECT_US_PARA;

typedef struct {
	int nfds;	_align64
	fd_set* readfds;	_align64
	fd_set* writefds;	_align64
	fd_set* exceptfds;	_align64
	TMO tmout;	_align64
} NET_SO_SELECT_MS_PARA;

typedef struct {
	int nfds;	_align64
	fd_set* readfds;	_align64
	fd_set* writefds;	_align64
	fd_set* exceptfds;	_align64
	struct timeval* tv;	_align64
} NET_SO_SELECT_PARA;

typedef struct {
	int sd;	_align64
	const void* buf;	_align64
	size_t count;	_align64
} NET_SO_WRITE_PARA;

typedef struct {
	int sd;	_align64
	const void* buf;	_align64
	size_t len;	_align64
	int flags;	_align64
} NET_SO_SEND_PARA;

typedef struct {
	int sd;	_align64
	const void* buf;	_align64
	size_t len;	_align64
	int flags;	_align64
	const struct sockaddr* dest_addr;	_align64
	socklen_t addrlen;	_align64
} NET_SO_SENDTO_PARA;

typedef struct {
	int sd;	_align64
	const struct msghdr* msg;	_align64
	int flags;	_align64
} NET_SO_SENDMSG_PARA;

typedef struct {
	int sd;	_align64
	int how;	_align64
} NET_SO_SHUTDOWN_PARA;

typedef struct {
	int domain;	_align64
	int type;	_align64
	int protocol;	_align64
} NET_SO_SOCKET_PARA;

typedef struct {
	char* name;	_align64
	size_t len;	_align64
} NET_SO_GETHOSTNAME_PARA;

typedef struct {
	const char* name;	_align64
	size_t len;	_align64
} NET_SO_SETHOSTNAME_PARA;

typedef struct {
	const char* node;	_align64
	const char* service;	_align64
	const struct addrinfo* hints;	_align64
	struct addrinfo** res;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
	TMO_U tmout_u;	_align64
} NET_SO_GETADDRINFO_US_PARA;

typedef struct {
	const char* node;	_align64
	const char* service;	_align64
	const struct addrinfo* hints;	_align64
	struct addrinfo** res;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
	TMO tmout;	_align64
} NET_SO_GETADDRINFO_MS_PARA;

typedef struct {
	const char* node;	_align64
	const char* service;	_align64
	const struct addrinfo* hints;	_align64
	struct addrinfo** res;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
	struct timeval* timeout;	_align64
} NET_SO_GETADDRINFO_PARA;

typedef struct {
	const struct sockaddr* sa;	_align64
	socklen_t salen;	_align64
	char* host;	_align64
	size_t hostlen;	_align64
	char* serv;	_align64
	size_t servlen;	_align64
	int flags;	_align64
	TMO_U tmout_u;	_align64
} NET_SO_GETNAMEINFO_US_PARA;

typedef struct {
	const struct sockaddr* sa;	_align64
	socklen_t salen;	_align64
	char* host;	_align64
	size_t hostlen;	_align64
	char* serv;	_align64
	size_t servlen;	_align64
	int flags;	_align64
	TMO tmout;	_align64
} NET_SO_GETNAMEINFO_MS_PARA;

typedef struct {
	const struct sockaddr* sa;	_align64
	socklen_t salen;	_align64
	char* host;	_align64
	size_t hostlen;	_align64
	char* serv;	_align64
	size_t servlen;	_align64
	int flags;	_align64
	struct timeval* timeout;	_align64
} NET_SO_GETNAMEINFO_PARA;

typedef struct {
	int sd;	_align64
} NET_SO_SOCKATMARK_PARA;

typedef struct {
	ID tskid;	_align64
} NET_SO_BREAK_PARA;

typedef struct {
	const char* devnm;	_align64
} NET_SO_IFATTACH_PARA;

typedef struct {
	const char* devnm;	_align64
} NET_SO_IFDETACH_PARA;

typedef struct {
	int af;	_align64
	int cmd;	_align64
	int flags;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
} NET_SO_RTLIST_PARA;

typedef struct {
	int cmd;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
} NET_SO_RESCTL_PARA;

typedef struct {
	struct ifaddrs** ifap;	_align64
	void* buf;	_align64
	size_t bufsz;	_align64
} NET_SO_GETIFADDRS_PARA;

typedef struct {
	unsigned int ifindex;	_align64
	char* ifname;	_align64
} NET_SO_IFINDEXTONAME_PARA;

typedef struct {
	const char* ifname;	_align64
} NET_SO_IFNAMETOINDEX_PARA;

typedef struct {
	const char* path;	_align64
	int oflag;	_align64
} NET_SO_BPFOPEN_PARA;

typedef struct {
	const char* path;	_align64
	int oflag;	_align64
} NET_SO_TUNOPEN_PARA;

typedef struct {
	int sd;	_align64
	int cmd;	_align64
	void* arg;	_align64
} NET__SO_FCNTL_PARA;

typedef struct {
	int sd;	_align64
	int request;	_align64
	void* data;	_align64
} NET__SO_IOCTL_PARA;

