#pragma once
#include <string>
#include "../misc/common.h"
#include "../misc/auto_file.h"
#include "../misc/mthelp.h"
#include "../misc/nano_log2.h"
#include "../misc/async_writer.h"
#include"../tars/util.h"
using namespace nano;
typedef default_log_translator::cstr_ptr cstr_ptr;

class logger_instance {
public :
	typedef logger<
		single_thread_executor<512, false>
	> logger_type;

	typedef os::async_writer<
		logger_type::executor_type
	> writer_type;

	typedef std::unique_ptr<logger_type> logger_ptr_type;
	typedef std::unique_ptr<writer_type> md_writer_ptr_type;

	static void init_instance(const char *scope_name, int scope_id, bool is_trace) {
		if (!instance_) {
			instance_ = new logger_instance(scope_name, scope_id, is_trace);
		}
	}

	template <class ...Args>
	static void info(Args &&...args) {
		assert(instance_);
		instance_->st_logger->info(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void debug(Args &&...args) {
		assert(instance_);
		instance_->st_logger->debug(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void warn(Args &&...args) {
		assert(instance_);
		instance_->st_logger->warn(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void error(Args &&...args) {
		assert(instance_);
		instance_->st_logger->error(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void trace(Args &&...args) {
		assert(instance_);
		instance_->st_logger->trace(std::forward<Args>(args)...);
	}

	static md_writer_ptr_type& md_writer() {
		assert(instance_);
		return instance_->st_md_writer;
	}
	
private:
	static logger_instance* instance_;

	std::unique_ptr<logger_type::executor_type> st_log_exe;
	std::unique_ptr<logger_type::writer_type> st_log_writer;
	std::unique_ptr<logger_type> st_logger;
	std::unique_ptr<writer_type> st_md_writer;

	logger_instance() = delete;
	logger_instance(const logger_instance &) = delete;
	logger_instance &operator=(const logger_instance &) = delete;
	logger_instance &operator=(logger_instance &&) = delete;

	logger_instance(const char *scope_name, int scope_id, bool is_trace)
	{
		assert(st_log_exe.get() == nullptr);
		assert(st_log_writer.get() == nullptr);
		assert(st_logger.get() == nullptr);
		assert(st_md_writer.get() == nullptr);

		std::string log_name = std::string(scope_name);
		std::string log_file = log_name + ".log";
		st_log_writer.reset(
			new logger_type::writer_type(log_name, true));
		if (!st_log_writer->open_file(log_file))
		{
			printf("%s: error: cannot open %s for log\n",
				scope_name, log_file.c_str());
			fflush(stdout);
			throw std::runtime_error("init failed");
		}

		// create executor
		st_log_exe.reset(new logger_type::executor_type(
			20, 0, tars::util::micro_spin()));

		/*
		// create ticks file
		std::string ticks_file = std::string(scope_name) + ".mt_ticks";
		st_md_writer.reset(new writer_type(*st_log_exe));
		if (!st_md_writer->open_file(ticks_file, "a"))
		{
			printf("%s: error: cannot open %s for ticks\n",
				scope_name, ticks_file.c_str());
			fflush(stdout);
			throw std::runtime_error("init failed");
		}
		st_md_writer->write_one([](FILE *fp, long long) {
			fprintf(fp, "%s,OrigMillis\n", "mini_tars::md_csv_headers()");
		});
		*/

		// create logger
		int log_level = is_trace ?LOG_TRACE : LOG_DEBUG;
		st_logger.reset(new logger_type(
			log_level, *st_log_exe, *st_log_writer));
	};
};
logger_instance* logger_instance::instance_ = nullptr;