/*
 * socket.h - berkeley socket interface
 *
 * Authors: Jia Wang
 *
 */

#ifndef SOCKET_H
#define SOCKET_H

// system headers
#ifdef _WIN32
// Win32 system
#define NOMINMAX
#include <winsock2.h>
#include <process.h>
typedef int socklen_t;
typedef unsigned long in_addr_t;
typedef unsigned short in_port_t;

#define GET_LAST_SOCK_ERROR() h_errno
#ifdef EWOULDBLOCK
#undef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif // EWOULDBLOCK

#else // !defined(_WIN32)
// Unix like systems
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#define SD_BOTH SHUT_RDWR

#define GET_LAST_SOCK_ERROR() errno
#define ioctlsocket ioctl

#endif // _WIN32

namespace os
{

using misc::cstr_t;

// internet IPv4 wrapper
class Socket
{
protected:
	SOCKET _s;

public:
	Socket() : _s(INVALID_SOCKET) {}
	Socket(SOCKET s) : _s(s) {}
	SOCKET operator = (SOCKET s) {_s = s; return s;}
	operator SOCKET () {return _s;}
	void close(bool bshutdown = true);

	bool UDP();
	bool TCP(bool no_delay);
	bool settcp_noblock();
	void shutdown();

	bool bind(in_port_t port = 0, in_addr_t nip = 0);
	bool listen(int backlog = SOMAXCONN);

	bool connect(cstr_t ipstr, in_port_t port);
	bool connect(in_addr_t nip, in_port_t port);
	bool connect(in_addr ip, in_port_t port);

	SOCKET accept(in_addr_t *pnip, in_port_t *pport);

	int recv(void *data, size_t len);
	int send(const void *data, size_t len);

	int recv_noblock(void *data, size_t len);
	int send_noblock(void *data, size_t len);

	int recvfrom(void *data, size_t len, in_addr_t *pnip, in_port_t *pport);
	int sendto(const void *data, size_t len, in_addr_t nip, in_port_t port);

	bool getsockname(in_addr_t *pnip, in_port_t *pport);
	bool getpeername(in_addr_t *pnip, in_port_t *pport);

}; // class Socket

class set_fd : public fd_set
{
	int _maxfd;

public:
	set_fd();

	void set(Socket s);
	int isset(Socket s);

	int maxfd() const {return _maxfd;}

}; // class set_fd

std::string gethostname();

} // namespace os

inline const char *inet_ltoa(in_addr_t nip)
{
	return inet_ntoa(*(in_addr *)&nip);
}

#endif // SOCKET_H
