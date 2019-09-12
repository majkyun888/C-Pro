/*
 * misc_str.cpp - Common string things.
 *
 * Authors: Jia Wang
 *
 */

#include "common.h"

#include "misc_str.h"

namespace misc
{

namespace
{

std::string left(cstr_t str, size_t len, int n)
{
	if (n < 0)
		n += (int)len;

	// truncate
	if (n < 0)
		n = 0;
	if (n > (int)len)
		n = (int)len;

	return std::string(str, str+n);
}

std::string right(cstr_t str, size_t len, int n)
{
	if (n < 0)
		n += (int)len;

	// truncate
	if (n < 0)
		n = 0;
	if (n > (int)len)
		n = (int)len;

	return std::string(str+n, str+len);
}

} // namespace

std::string str_left(cstr_t str, int n)
{
	return left(str, strlen(str), n);
}

std::string str_left(const std::string &str, int n)
{
	return left(str.c_str(), str.length(), n);
}

std::string str_right(cstr_t str, int n)
{
	return right(str, strlen(str), n);
}

std::string str_right(const std::string &str, int n)
{
	return right(str.c_str(), str.length(), n);
}

} // namespace misc

namespace os
{

namespace
{

size_t after_last_sep(misc::cstr_t str, size_t len)
{
	for (; len != 0; --len)
		if ((str[len-1] == '\\') || (str[len-1] == '/'))
			break;
	return len;
}

} // namespace

std::string basename(misc::cstr_t str)
{
	size_t len = strlen(str);

	return std::string(str+after_last_sep(str, len), str+len);
}

std::string basename(const std::string &str)
{
	return str.substr(after_last_sep(str.c_str(), str.length()));
}

std::string dirname(misc::cstr_t str)
{
	size_t i = after_last_sep(str, strlen(str));

	if (i == 0)
		return std::string();
	else
		return std::string(str, str+i-1);
}

std::string dirname(const std::string &str)
{
	size_t i = after_last_sep(str.c_str(), str.length());

	if (i < 2)
		return ".";
	else
		return str.substr(0, i-1);
}

} // namespace os
