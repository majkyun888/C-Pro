/*
 * auto_file.h - Automatic file/pipe.
 *
 * Authors: Jia Wang
 *
 */

#ifndef AUTO_FILE_H
#define AUTO_FILE_H

#include "common.h"

namespace os
{

class auto_file
{
	auto_file(const auto_file &) = delete;
	auto_file &operator = (const auto_file &) = delete;

	FILE *fp_;
	bool pipe_;

public:
	auto_file();
	~auto_file();

	void clear();

	// will clear the old one automatically
	// make sure you get the right text/binary mode
	bool file_open(const std::string &name, misc::cstr_t mode);
	bool pipe_open(const std::string &cmd, misc::cstr_t mode);

	FILE *get_fp() const;

	// some commonly used commands
	bool gz_read(const std::string &name, bool binary = false);
	bool gz_write(const std::string &name, bool binary = false);

}; // class auto_file

void mkdir(misc::cstr_t path);

} // namespace os

#endif // AUTO_FILE_H
