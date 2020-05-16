/*
 * async_writer.h - async file writer to use with executors
 *
 * Authors: Jia Wang
 *
 */

#ifndef ASYNC_WRITER_H
#define ASYNC_WRITER_H

#include "auto_file.h"

namespace os
{

template <class Executor>
class async_writer
{
	Executor &exe_;
	os::auto_file file_;
	long long millis_;

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

public:
	async_writer(Executor &exe) :
		exe_(exe)
	{
		exe_.set_on_batch_begin([this] {
			update_time();
		});
		exe_.set_on_batch_end([this] {
			flush_file();
		});

		update_time();
	}

	bool open_file(
		const std::string &name,
		misc::cstr_t mode)
	{
		return file_.file_open(name, mode);
	}

	template <class Func>
	void write_one(Func &&f)
	{
		exe_.submit([f, this]{
			FILE *fp = file_.get_fp();
			if (fp != nullptr)
			{
				f(fp, millis_);
			}
		});
	}
}; // class async_writer

} // namespace os

#endif // ASYNC_WRITER_H
