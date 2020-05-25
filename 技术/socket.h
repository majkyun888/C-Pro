#pragma once
#ifndef SOCKET_H
#define SOCKET_H
#endif
#include<iostream>
#include<WinSock2.h>
#include<process.h>
using namespace std;
typedef int socklen_t;
typedef unsigned long in_addr_t;
typedef unsigned short in_port_t;


class Socket
{
protected:
	SOCKET _s;

public:
	Socket() :_s(INVALID_SOCKET) {};
	Socket(SOCKET s) :_s(s) {};
	Socket operator = (SOCKET s) { _s = s; return s; };
	operator SOCKET () { return _s; };
	void close(bool bshutdown = true) {};

	bool UDP()
	{
		_s = socket(AF_INET, SOCK_DGRAM, 0);	//����UDP,ʹ��IPV4�����ݱ�
		return _s != INVALID_SOCKET;
	}
	bool TCP()	//��TCP����
	{
		_s = socket(AF_INET, SOCK_STREAM, 0);
		if (_s == INVALID_SOCKET)
			return false;

		return true;
	}
	bool settcp_noblock()	//��socket����Ϊ������ģʽ
	{
		for (;;)
		{
			unsigned long on_windows = 1;
			if (ioctlsocket(_s, FIONBIO, &on_windows) < 0)
			{
				int err = WSAGetLastError();
				if (err != EINTR)
				{
					char errmsg[256];
					sprintf(errmsg, "settcp_noblock failed: errno=%d, errmsg=%s", err, strerror(err));
					throw std::runtime_error(errmsg);
				}
			}
			else
				break;
		}
		return true;
	}
	bool bind(in_port_t port, in_addr_t nip)	//�󶨶˿ں�ip
	{
		sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = nip;
		addr.sin_port = htons(port);
		return ::bind(_s, (sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR;
	}
	bool listen(int backlog)	//�������еĴ�С
	{
		return ::listen(_s, backlog) != SOCKET_ERROR;
	}
	bool connect(in_addr_t nip, in_port_t port)	//TCP�ͻ���
	{
		sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = nip;
		addr.sin_port = htons(port);
		return ::connect(_s, (sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR;
	}
	/*bool connect(const char * ipstr, in_port_t port)
	{
		return connect(inet_addr(ipstr), port);
	}*/
	bool connect(in_addr ip, in_port_t port)	//����TCP
	{
		return connect(ip.s_addr, port);
	}
	
	/*SOCKET accept(in_addr_t *pnip, in_port_t *pport)
	{

	}*/
	int recvfrom(void *data,size_t len,in_addr_t *pnip,in_port_t *pport) //UDP������Ϣ,����1���ܻ�����������2��������С������3���ܶ�ip������4���ܶ˿�
	{
		sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);

		int ret = ::recvfrom(_s, (char *)data, len, 0, (sockaddr *)&addr, &addrlen);
		if (pnip != 0)
			*pnip = addr.sin_addr.s_addr;
		if (pport != 0)
			*pport = ntohs(addr.sin_port);
		return ret;
	}
	int sendto(const void *data, size_t len, in_addr_t nip, in_port_t port)//����UDPͨ��
	{
		sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = nip;
		addr.sin_port = htons(port);

		return ::sendto(_s, (const char *)data, len, 0, (sockaddr *)&addr, sizeof(addr));
	}
	//bool getsockname(in_addr_t)
	SOCKET accept(in_addr_t *pnip, in_port_t *pport)	//	����TCP
	{
		sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);

		SOCKET ret = ::accept(_s, (sockaddr *)&addr, &addrlen);

		if (pnip != 0)
			*pnip = addr.sin_addr.s_addr;
		if (pport != 0)
			*pport = ntohs(addr.sin_port);
		return ret;
	}
	int recv(void *data,size_t len)	//����ģʽ
	{
		return ::recv(_s, (char *)data, len, 0);
	}
	int recv_noblock(void *data,size_t len)	//������ģʽ
	{
		int recv_len = recv(data, len);
		if (recv_len > 0)
			return recv_len;
		else if (recv_len == 0)	//����ʱ����
			throw std::runtime_error("recv_raw_noblock failed: connection has been gracefully closed");
		else//С��0�������ʹ���
		{
			int nErrno = WSAGetLastError();
			if (nErrno == EWOULDBLOCK)
			{
				return 0;
			}
			else
			{
				char err_msg[256];
				sprintf(err_msg,"recv_noblock failed:error=%d",nErrno);
				throw std::runtime_error(err_msg);
			}
		}
	}
	int send(const void *data, size_t len)	//����ģʽ
	{
		int flag = 0;
		return ::send(_s, (const char *)data, len, flag);
	}
	int send_noblock(void *data,size_t len)	//������ģʽ
	{
		int sent_len = send(data, len);
		if (sent_len >= 0)
		{
			return sent_len;
		}
		else
		{
			int nErrno = WSAGetLastError();
			if (nErrno == EWOULDBLOCK)
				return 0;
			else
			{
				char err_msg[256];
				sprintf(err_msg,"send_noblock failed, error=%d",nErrno);
				throw std::runtime_error(err_msg);
			}
		}

	}
	void closeHandle()
	{
		closesocket(_s);
	}

};
