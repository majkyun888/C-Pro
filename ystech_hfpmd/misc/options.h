/*
 * options.h - Handling options.
 *
 * Authors: Jia Wang
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include "./common.h"

namespace misc
{

class option_reg;
class tracer;

// opt_name=opt_value ...
class options
{
	friend class option_reg;
	friend class tracer;

	bool safe_; // pretended to be empty when not safe

	// key, value, info
	typedef std::pair<std::string, std::string> str_pair;
	typedef std::map<std::string, str_pair> opt_map;
	opt_map opts_;

public:
	// for non-singleton use
	options() : safe_(true) {}
	~options() {safe_ = false;}
	void reg(
		const std::string &key,
		const std::string &value,
		const std::string &info);

	// for singleton use
	static options &get();

	// will automaticly load option files via key options
	// will replace existing values
	void load_options(int argc, char *argv[]);
	void load_options(const std::string &options_str);
	bool load_option(const std::string &option_str);

	// show options
	// will skip those secrets whose info start w/ *
	void show_options(FILE *fp, cstr_t prefix = "") const;

	// find an option and convert to single value
	int as_int(const std::string &key) const;
	double as_double(const std::string &key) const;
	cstr_t as_str(const std::string &key) const;
	bool as_bool(const std::string &key) const;

	// find an option and convert to multiple values separated by commas
	std::vector<int> as_int_vec(const std::string &key) const;
	std::vector<double> as_double_vec(const std::string &key) const;
	std::vector<std::string> as_str_vec(const std::string &key) const;

}; // class options

   // for singleton use
class option_reg
{
public:
	option_reg(
		const std::string &key,
		const std::string &value,
		const std::string &info);
}; // class option_reg

} // namespace misc

#define RegisterOption(key, value, info) \
	namespace {misc::option_reg register_option_##key(#key, value, info);}

#endif // OPTIONS_H
