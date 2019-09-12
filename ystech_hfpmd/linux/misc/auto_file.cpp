/*
 * auto_file.cpp - Implementation of automatic file/pipe.
 *
 * Authors: Jia Wang
 *
 */

// system headers
#ifdef _WIN32
#include <direct.h>
#endif

#include "./auto_file.h"

namespace os
{

using misc::str_left;
using misc::str_right;

auto_file::auto_file()
	: fp_(0), pipe_(false)
{
}

auto_file::~auto_file()
{
	clear();
}

void auto_file::clear()
{
	if (fp_ == 0)
		return;

	if (pipe_)
		_pclose(fp_);
	else
		fclose(fp_);

	fp_ = 0;
	pipe_ = false;
}

bool auto_file::file_open(const std::string &name, misc::cstr_t mode)
{
	clear();

//	printf("fopen %s\n", name.c_str());
	fp_ = fopen(name.c_str(), mode);

	return fp_ != 0;
}

bool auto_file::pipe_open(const std::string &cmd, misc::cstr_t mode)
{
	clear(); pipe_ = true;

//	printf("popen %s\n", cmd.c_str());
	fp_ = _popen(cmd.c_str(), mode);

	if (fp_ == 0)
	{
		pipe_ = false;
		return false;
	}

	return true;
}	

FILE *auto_file::get_fp() const
{
	return fp_;
}

bool auto_file::gz_read(const std::string &name, bool binary)
{
	std::string cmd = "gzip -cd "+name;

	// What if the file is not there?
	// stat won't work since system/cmd may have different
	// views of the files ...
	// Have to read once to make sure it works
	// and then reopen it

	if (!pipe_open(cmd, binary? "rb": "r"))
		return false;

	char ch;
	if (fread(&ch, 1, 1, fp_) == 0)
		return false;

	return pipe_open(cmd, binary? "rb": "r");
}

bool auto_file::gz_write(const std::string &name, bool binary)
{
	std::string cmd = "gzip >"+name;

	return pipe_open(cmd, binary? "wb": "w");
	// The gzip in MSYS does generate corrupted gzip files.
	// Get a correct one from gzip's website.
}

#ifdef _WIN32
// Win32 System
void mkdir(misc::cstr_t path)
{
	_mkdir(path);
}
#else // !defined(_WIN32)
// Unix like systems
void mkdir(misc::cstr_t path)
{
	::mkdir(path, 0755);
}
#endif // _WIN32

} // namespace os
