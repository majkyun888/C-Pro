/*
 * misc_str.h - Common string things.
 *
 * Authors: Jia Wang
 *
 */

#ifndef MISC_STR_H
#define MISC_STR_H

namespace misc
{

typedef const char *cstr_t;

struct cstr_less : public std::binary_function<cstr_t, cstr_t, bool>
{
	bool operator() (cstr_t a, cstr_t b) const
	{
		return strcmp(a, b) < 0;
	}
}; // struct cstr_less

struct cstr_equal_to : public std::binary_function<cstr_t, cstr_t, bool>
{
	bool operator() (cstr_t a, cstr_t b) const
	{
		return strcmp(a, b) == 0;
	}
}; // struct cstr_equal_to

struct cstr_hash : public std::unary_function<cstr_t, size_t>
{
	size_t operator()(cstr_t s) const
	{
		size_t ret = 5381;

		for(; *s != 0; ++s)
			ret = ret*33+*s;

		return ret;
	};
}; // struct cstr_hash

// python-like slicing: str[:n] and str[n:]
std::string str_left(cstr_t str, int n);
std::string str_left(const std::string &str, int n);
std::string str_right(cstr_t str, int n);
std::string str_right(const std::string &str, int n);

// Protected strings before lambda capture.
// - string literals and equivalents: no protection
// - char[N]: std::array<char, N> or std::array<char, M>
// - char */const char *: std::array<char, M>
template <size_t N>
std::array<char, N> wrap_str(char(&a)[N])
{
	std::array<char, N> ret;
	std::copy(a, a+N, ret.data());
	return ret;
}
template <size_t M>
std::array<char, M> cut_str(cstr_t str)
{
	std::array<char, M> ret;
	strncpy(ret.data(), str, M-1);
	ret[M-1] = 0;
	return ret;
}
template <size_t M>
std::array<char, M> cut_str(const std::string &str)
{
	return cut_str<M>(str.c_str());
}

} // namespace misc

// I don't know where to put them now ...
namespace os
{

std::string basename(misc::cstr_t str);
std::string basename(const std::string &str);
std::string dirname(misc::cstr_t str);
std::string dirname(const std::string &str);

} // namespace os

#endif // MISC_STR_H
