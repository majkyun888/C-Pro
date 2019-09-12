/*
 * socket.cpp - berkeley socket implementation
 *
 * Authors: Jia Wang
 *
 */

#include "common.h"
#include "socket.h"

namespace os
{

void Socket::close(bool bshutdown)
{
	if (_s != INVALID_SOCKET)
	{
		if (bshutdown)
			shutdown();
		::closesocket(_s);
	}
	_s = INVALID_SOCKET;
}

bool Socket::TCP(bool no_delay)
{
	_s = socket(AF_INET, SOCK_STREAM, 0);
	if (_s == INVALID_SOCKET)
		return false;

    if (no_delay) {
#ifndef _WIN32
        int yes = 1;
		setsockopt(_s, SOL_TCP, TCP_NODELAY, &yes, sizeof(yes));
#endif		
    }
	return true;
}

bool Socket::settcp_noblock() {
	for (;;)
	{
#ifdef _WIN32
		unsigned long on_windows = 1;
		if (ioctlsocket(_s, FIONBIO, &on_windows) < 0)
#else
		int on = 1;
		if (ioctlsocket(_s, FIONBIO, (char *)&on) < 0)
#endif
		{
			int err = GET_LAST_SOCK_ERROR();
			if (GET_LAST_SOCK_ERROR() != EINTR) {
				char errmsg[256];
				sprintf(errmsg, "settcp_noblock failed: errno=%d, errmsg=%s", err, strerror(err));
				throw std::runtime_error(errmsg);
			}
		}
		else
			break;
	} // for
	return true;
}

bool Socket::UDP()
{
	_s = socket(AF_INET, SOCK_DGRAM, 0);

	return _s != INVALID_SOCKET;
}

void Socket::shutdown()
{
	::shutdown(_s, SD_BOTH);
}

bool Socket::bind(in_port_t port, in_addr_t nip)
{
	sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = nip;
	addr.sin_port = htons(port);

#ifndef _WIN32
	if (port != 0)
	{
		int opt = 1;
		setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}
#endif // !_WIN32
	
	return ::bind(_s, (sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool Socket::connect(in_addr_t nip, in_port_t port)
{
	sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = nip;
	addr.sin_port = htons(port);

	return ::connect(_s, (sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool Socket::connect(cstr_t ipstr, in_port_t port)
{
	return connect(inet_addr(ipstr), port);
}

bool Socket::connect(in_addr ip, in_port_t port)
{
	return connect(ip.s_addr, port);
}

bool Socket::listen(int backlog)
{
	return ::listen(_s, backlog)!=SOCKET_ERROR;
}

SOCKET Socket::accept(in_addr_t *pnip, in_port_t *pport)
{
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	SOCKET ret = ::accept(_s, (sockaddr *)&addr, &addrlen);

	if (pnip != 0) *pnip = addr.sin_addr.s_addr;
	if (pport != 0) *pport = ntohs(addr.sin_port);

	return ret;
}

int Socket::recv(void *data, size_t len)
{
	// Winsock 2 need char *
	return ::recv(_s, (char *)data, len, 0);
}

int Socket::send(const void *data, size_t len)
{
#ifdef _WIN32
	int flag = 0;
#else
	int flag = MSG_NOSIGNAL; // don't signal me
#endif // !_WIN32

	// Winsock 2 need const char *
	return ::send(_s, (const char *)data, len, flag);
}

int Socket::recv_noblock(void *data, size_t len)
{
	int recv_len = recv(data, len);
	if (recv_len > 0)
		return recv_len;
	else if (recv_len == 0)
		throw std::runtime_error("recv_raw_noblock failed: connection has been gracefully closed");
	else {
		int nErrno = GET_LAST_SOCK_ERROR();
		if (nErrno == EWOULDBLOCK) {
			return 0;
		}
		else {
			char err_msg[256];
			sprintf(err_msg, "recv_noblock failed:error=%d", nErrno);
			throw std::runtime_error(err_msg);
		} 
	} 
}

int Socket::send_noblock(void *data, size_t len)
{
	int sent_len = send(data, len);
	if (sent_len >= 0) {
		return sent_len;
	}
	else /* if (send_len == SOCKET_ERROR) */ {
		int nErrno = GET_LAST_SOCK_ERROR();
		if (nErrno == EWOULDBLOCK) {
			return 0;
		}
		else {
			char err_msg[256];
			sprintf(err_msg, "send_noblock failed, error=%d", nErrno);
			throw std::runtime_error(err_msg);
		} 
	}
}

int Socket::recvfrom(void *data, size_t len, in_addr_t *pnip, in_port_t *pport)
{
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	// Winsock 2 need char *
	int ret=::recvfrom(_s, (char *)data, len, 0, (sockaddr *)&addr, &addrlen);
	
	if(pnip != 0) *pnip = addr.sin_addr.s_addr;
	if(pport != 0) *pport = ntohs(addr.sin_port);

	return ret;
}

int Socket::sendto(const void *data, size_t len, in_addr_t nip, in_port_t port)
{
	sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = nip;
	addr.sin_port = htons(port);

	// Winsock 2 need const char *
	return ::sendto(_s, (const char *)data, len, 0, (sockaddr*)&addr, sizeof(addr));
}

bool Socket::getsockname(in_addr_t *pnip, in_port_t *pport)
{
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	if (::getsockname(_s, (sockaddr *)&addr, &addrlen) == SOCKET_ERROR)
		return false;

	if(pnip != 0) *pnip = addr.sin_addr.s_addr;
	if(pport != 0) *pport = ntohs(addr.sin_port);

	return true;
}

bool Socket::getpeername(in_addr_t *pnip, in_port_t *pport)
{
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	if (::getpeername(_s, (sockaddr *)&addr, &addrlen) == SOCKET_ERROR)
		return false;

	if(pnip != 0) *pnip = addr.sin_addr.s_addr;
	if(pport != 0) *pport = ntohs(addr.sin_port);

	return true;
}

set_fd::set_fd()
{
	FD_ZERO(this);
	_maxfd = 0;
}

void set_fd::set(Socket s)
{
	FD_SET(s, this);

// WinSock2 does not need maxfd in select
#ifndef _WIN32
	if (s > _maxfd)
		_maxfd = s;
#endif // _WIN32
}

int set_fd::isset(Socket s)
{
	return FD_ISSET(s, this);
}

std::string gethostname()
{
	char buf[1000];
	size_t len = sizeof(buf)-1;
	::gethostname(buf, len);
	buf[len] = 0;
	return buf;
}

}; // namespace os

// Winsock2 Initialize
#ifdef _WIN32
  #ifdef _MSC_VER
  #pragma comment(lib, "ws2_32.lib")
  #endif // _MSC_VER
namespace
{
	struct WSAInit
	{
		WSAInit()
		{
			WSADATA wsaData;	
			WSAStartup(MAKEWORD(2,2),&wsaData);
		};
		~WSAInit()
		{
			WSACleanup();
		};
	}wsainit;
}
#endif // _WIN32
