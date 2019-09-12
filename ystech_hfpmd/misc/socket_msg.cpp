/*
 * socket_msg.cpp - socket message implementation
 *
 * Authors: Jia Wang
 *
 */

#include "common.h"

#include "socket_msg.h"

namespace os
{

bool string_msg::recv_all(void *data, size_t len, long secs)
{
	char *buf = (char *)data;

	for (; len > 0;)
	{
		set_fd sf;
		sf.set(_s);
		timeval tv = {secs, 0};
		if (select(sf.maxfd()+1, &sf, 0, 0, &tv) != 1)
			return false;

		int recv_len = recv(buf, len);
		if (recv_len < 1)
			return false;

		buf += recv_len;
		len -= recv_len;
	}
	return true;
}

bool string_msg::send_all(const void *data, size_t len, long secs)
{
	const char *buf = (const char *)data;

	for (; len > 0;)
	{
		set_fd sf;
		sf.set(_s);
		timeval tv = {secs, 0};
		if (select(sf.maxfd()+1, 0, &sf, 0, &tv) != 1)
			return false;

		int send_len = send(buf, len);
		if (send_len < 1)
			return false;

		buf += send_len;
		len -= send_len;
	}
	return true;
}

bool string_msg::recv_line(std::string &line, long secs)
{
	for (;;)
	{
		set_fd sf;
		sf.set(_s);
		timeval tv = {secs, 0};
		char ch;
		if ((select(sf.maxfd()+1, &sf, 0, 0, &tv) != 1)
			|| (recv(&ch, 1) != 1) || (ch == 0))
			return false;

		line.append(1, ch);
		
		if (ch != '\n')
			continue;

		if (line.length() < 2)
			continue;

		if (line[line.length()-2] != '\r')
			continue;

		return true;
	}
}

bool string_msg::send_str(cstr_t str, long secs)
{
	return send_all(str, strlen(str), secs);
}

bool string_msg::send_str(const std::string &str, long secs)
{
	return send_all(str.c_str(), str.length(), secs);
}

void string_msg::send_msg(const l_str &message, long secs)
{
	for (l_str::const_iterator i = message.begin(); i != message.end(); ++i)
		if (!send_str(*i, secs)
			|| !send_all("\r\n", 2, secs))
			throw std::runtime_error("string_msg::send strings");
	if (!send_all("\r\n", 2, secs))
		throw std::runtime_error("string_msg::send termination");
}

void string_msg::recv_msg(l_str &message, long secs)
{
	message.clear();

	for (;;)
	{
		std::string line;
		if (!recv_line(line, secs))
			throw std::runtime_error("string_msg::recv");
		// ending \r\n
		if (line.length() == 2)
			return;
		message.push_back(line.substr(0, line.length()-2));
	}
}

void binary_msg::recv_raw(void *data, size_t len)
{
	char *buf = (char *)data;

	for (; len > 0;)
	{
		int recv_len = recv(buf, len);
		if (recv_len < 1)
			throw std::runtime_error(
				"binary_msg::recv_raw terminated");

		buf += recv_len;
		len -= recv_len;
	}
}

void binary_msg::send_raw(const void *data, size_t len)
{
	const char *buf = (const char *)data;

	for (; len > 0;)
	{
		int send_len = send(buf, len);
		if (send_len < 1)
			throw std::runtime_error(
				"binary_msg::send_raw terminated");

		buf += send_len;
		len -= send_len;
	}
}

std::pair<void *, size_t> binary_msg::recv_buf(void *data, size_t max_len)
{
	size_t len = recv_pod<size_t>();
	bool is_null = recv_pod<bool>();
	if (len > max_len)
	{
		char temp[1000];
		sprintf(temp,
			"binary_msg::recv_buf out of range: %llu > %llu",
			len, max_len);
		throw std::runtime_error(temp);
	}
	if (is_null)
	{
		return std::make_pair(nullptr, len);
	}
	else
	{
		recv_raw(data, len);
		return std::make_pair(data, len);
	}
}

void binary_msg::send_buf(const void *data, size_t len)
{
	send_pod(len);
	if (data == nullptr)
	{
		send_pod(true);
	}
	else
	{
		send_pod(false);
		send_raw(data, len);
	}
}

void binary_msg::send_str(cstr_t str)
{
	// send ending 0 for easy recovery
	size_t len = strlen(str)+1;
	send_pod(len);
	send_raw(str, len);
}

void binary_msg::send_str(const std::string &str)
{
	// send ending 0 for easy recovery
	size_t len = str.size()+1;
	send_pod(len);
	send_raw(str.c_str(), len);
}

char *binary_msg::recv_str(void *data, size_t max_len)
{
	size_t len = recv_pod<size_t>();
	if (len > max_len)
		throw std::runtime_error(
			"binary_msg::recv_str out of range");
	recv_raw(data, len);
	char *str = (char *)data;
	if (str[len-1] != 0)
		throw std::runtime_error(
			"binary_msg::recv_str not a string");
	return str;
}

} // namespace os
