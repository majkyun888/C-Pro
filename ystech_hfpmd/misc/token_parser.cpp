/*
 * token_parser.cpp - Implementation of token parser and helper functions.
 *
 * Authors: Jia Wang
 *
 */

#include"./common.h"
#include "./token_parser.h"


namespace misc
{

template <class T>
T skip_space_tmpl(T buf, cstr_t delimit)
{
	for (; *buf != 0; ++buf)
	{
		size_t i;
		for (i = 0; delimit[i] != 0; ++i)
			if (*buf == delimit[i])
				break;
		if (delimit[i] == 0)
			return buf;
	}
	return buf;
}

template <class T>
T goto_space_tmpl(T buf, cstr_t delimit)
{
	for (; *buf != 0; ++buf)
		for (size_t i = 0; delimit[i] != 0; ++i)
			if (*buf == delimit[i])
				return buf;
	return buf;
}

delimit_bmp create_delimit_bmp(cstr_t delimit)
{
	delimit_bmp ret;

	for (size_t i = 0; i < 8; ++i)
		ret.bmp[i] = 0;

	for (; *delimit != 0; ++delimit)
	{
		size_t i = (unsigned char)(*delimit)/32;
		size_t bit = (unsigned char)(*delimit)%32;

		ret.bmp[i] |= (1 << bit);
	}
	return ret;
}

char *skip_space(char *buf, const delimit_bmp &bmp)
{
	for (; *buf != 0; ++buf)
	{
		size_t i = (unsigned char)(*buf)/32;
		size_t bit = (unsigned char)(*buf)%32;

		if ((bmp.bmp[i] & (1 << bit)) == 0)
			return buf;
	}
	return buf;
}

char *goto_space(char *buf, const delimit_bmp &bmp)
{
	for (; *buf != 0; ++buf)
	{
		size_t i = (unsigned char)(*buf)/32;
		size_t bit = (unsigned char)(*buf)%32;

		if ((bmp.bmp[i] & (1 << bit)) != 0)
			return buf;
	}
	return buf;
}

char *skip_space(char *buf, cstr_t delimit)
{
	return skip_space_tmpl(buf, delimit);
}

char *goto_space(char *buf, cstr_t delimit)
{
	return goto_space_tmpl(buf, delimit);
}

const char *skip_space(const char *buf, cstr_t delimit)
{
	return skip_space_tmpl(buf, delimit);
}

const char *goto_space(const char *buf, cstr_t delimit)
{
	return goto_space_tmpl(buf, delimit);
}

void split_tokens(char *buf, std::vector<cstr_t> *ptokens, cstr_t delimit)
{
	for (char *begin = buf; *begin != 0;)
	{
		char *end = goto_space(begin, delimit);
		ptokens->push_back(begin);
		if (*end == 0)
			return;
		*end = 0;
		begin = skip_space(end+1, delimit);
	}
}

// split to string vector
void split_tokens(const char *buf, std::vector<std::string> *ptokens, cstr_t delimit)
{
	for (const char *begin = buf; *begin != 0;)
	{
		const char *end = goto_space(begin, delimit);
		ptokens->push_back(std::string(begin, end));
		if (*end == 0)
			return;
		begin = skip_space(end+1, delimit);
	}
}

void split_tokens(char *buf, std::vector<cstr_t> *ptokens, const delimit_bmp &bmp)
{
	for (char *begin = buf; *begin != 0;)
	{
		char *end = goto_space(begin, bmp);
		ptokens->push_back(begin);
		if (*end == 0)
			return;
		*end = 0;
		begin = skip_space(end+1, bmp);
	}
}

} // namespace misc
