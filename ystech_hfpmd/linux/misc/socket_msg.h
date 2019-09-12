/*
 * socket_msg.h - socket message interface
 *
 * Authors: Jia Wang
 *
 */

#ifndef SOCKET_MSG_H
#define SOCKET_MSG_H

#include "socket.h"

namespace os
{

using misc::cstr_t;
typedef std::list<std::string> l_str;

// HTTP like message communication:
// header 0\r\n
// header 1\r\n
// ...
// header n\r\n
// \r\n
class string_msg: public Socket
{
public:
	string_msg(Socket s) : Socket(s) {}
	
	bool recv_all(void *data, size_t len, long secs);
	bool send_all(const void *data, size_t len, long secs);
	bool recv_line(std::string &line, long secs);
	bool send_str(cstr_t str, long secs);
	bool send_str(const std::string &str, long secs);

	// throw exceptions when failed
	void send_msg(const l_str &message, long secs);
	void recv_msg(l_str &message, long secs);
}; // class string_msg

// Binary format, both end should match,
// throw exceptions when failed, no explicit timeout.
class binary_msg: public Socket
{
public:
	binary_msg(Socket s) : Socket(s) {}

	void recv_raw(void *data, size_t len);
	void send_raw(const void *data, size_t len);

	// buf comes with a size and could be null
	std::pair<void *, size_t> recv_buf(void *data, size_t max_len);
	void send_buf(const void *data, size_t len);

	// strings are special buffers
	char *recv_str(void *data, size_t max_len);
	void send_str(cstr_t str);
	void send_str(const std::string &str);

	// POD only, be careful with 32/64 types.
	template <class T>
	T recv_pod()
	{
		T t;
		recv_raw(&t, sizeof(T));
		return t;
	}
	template <class T>
	void send_pod(const T &t)
	{
		send_raw(&t, sizeof(T));
	}
}; // class binary_msg

} // namespace os

#endif // SOCKET_MSG_H
