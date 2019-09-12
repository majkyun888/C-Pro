#pragma once

#include "../misc/auto_file.h"
#include "../misc/timer.h"

namespace tars
{

class ticks_writer
{
	os::auto_file file_;
	long long millis_;

public:
	ticks_writer()
	{
		update_time();
	}

	bool open_file(
		const std::string &name,
		const std::string &header)
	{
		if (file_.file_open(name, "a"))
		{
			FILE *fp = file_.get_fp();
			fprintf(fp, "%s", header.c_str());
			fprintf(fp, ",OrigMillis\n");
			fflush(file_.get_fp());
			return true;
		}
		return false;
	}

	// Getting time is expensive. Only update internal
	// time when necessary, e.g. before a batch begins.
	void update_time()
	{
		millis_ = os::current_time_millis();
	}

	// Flush file is expensive. Only flush when necessary,
	// e.g. after a batch ends.
	// Consider to call fsync in your own writer if needed.
	void flush_file()
	{
		FILE *fp = file_.get_fp();
		if (fp != nullptr)
		{
			fflush(fp);
		}
	}

	template <class ...Args>
	void write(misc::cstr_t, Args &&...args)
	{
		FILE *fp = file_.get_fp();
		if (fp != nullptr)
		{
			fprintf(fp, args...);
			fprintf(fp, ",%lld\n", millis_);
		}
	}
}; // class ticks_writer

} // namespace tars
