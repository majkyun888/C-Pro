/*
 * options.cpp - Implementation of option handling.
 *
 * Authors: Jia Wang
 *
 */
#include"./common.h"
#include"./auto_file.h"
#include "./token_parser.h"
#include "./options.h"






namespace misc
{

options &options::get()
{
	static options opts;
	return opts;
}

option_reg::option_reg(
	const std::string &key,
	const std::string &value,
	const std::string &info)
{
	options &opt = options::get();

	opt.reg(key, value, info);
}

void options::reg(
	const std::string &key,
	const std::string &value,
	const std::string &info)
{
	if (!safe_)
		return;

	options::opt_map::iterator it = opts_.find(key);

	if (it != opts_.end())
	{
		fprintf(stderr,
			"error: duplicated option %s: old %s, new %s\n",
			key.c_str(),
			it->second.second.c_str(),
			info.c_str());
		throw std::runtime_error("duplicated_option_key");
	}

	opts_.insert(std::make_pair(key, std::make_pair(value, info)));
}

void options::load_options(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i)
		load_option(argv[i]);
}

void options::load_options(const std::string &options_str)
{
	std::vector<std::string> pairs;

	split_tokens(options_str.c_str(), &pairs, ";");

	for (size_t i = 0; i < pairs.size(); ++i)
		load_option(pairs[i].c_str());
}

bool options::load_option(const std::string &option_str)
{
	if (!safe_)
		return false;

	std::vector<std::string> pair;

	split_tokens(option_str.c_str(), &pair, "=");

	if (pair.size() != 2)
		return false;

	opt_map::iterator it = opts_.find(pair[0]);

	if (it == opts_.end())
	{
		fprintf(stderr,
			"warning: unknown option %s\n",
			pair[0].c_str());
		return true;
	}

	it->second.first = pair[1];

	return true;
}

void options::show_options(FILE *fp, cstr_t prefix) const
{
	if (prefix[0] != 0)
		fprintf(fp, "%s %u options:\n", prefix, opts_.size());
	else
		fprintf(fp, "%u options:\n", opts_.size());

	for (opt_map::const_iterator it = opts_.begin();
		it != opts_.end(); ++it)
		if (it->second.second.c_str()[0] != '*')
			fprintf(fp,
				"  %s=%s: %s\n",
				it->first.c_str(),
				it->second.first.c_str(),
				it->second.second.c_str());

	fflush(fp);
}

int options::as_int(const std::string &key) const
{
	return atoi(as_str(key));
}

double options::as_double(const std::string &key) const
{
	return atof(as_str(key));
}

cstr_t options::as_str(const std::string &key) const
{
	if (!safe_)
		return "";

	opt_map::const_iterator it = opts_.find(key);
	if (it == opts_.end())
	{
		fprintf(stderr,
			"error: unknown option %s\n",
			key.c_str());
		throw std::runtime_error("unknown_option_key");
	}

	return it->second.first.c_str();
}

bool options::as_bool(const std::string &key) const
{
	return strcmp(as_str(key), "true") == 0;
}

std::vector<int> options::as_int_vec(const std::string &key) const
{
	std::vector<std::string> strs = as_str_vec(key);

	std::vector<int> ret(strs.size());

	for (size_t i = 0; i < ret.size(); ++i)
		ret[i] = atoi(strs[i].c_str());

	return ret;
}

std::vector<double> options::as_double_vec(const std::string &key) const
{
	std::vector<std::string> strs = as_str_vec(key);

	std::vector<double> ret(strs.size());

	for (size_t i = 0; i < ret.size(); ++i)
		ret[i] = atof(strs[i].c_str());

	return ret;
}

std::vector<std::string> options::as_str_vec(const std::string &key) const
{
	std::vector<std::string> ret;

	split_tokens(as_str(key), &ret, ",");

	return ret;
}

} // namespace misc
