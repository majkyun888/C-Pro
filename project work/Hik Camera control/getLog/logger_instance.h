#pragma once
#include <string>
#include "./common.h"
#include "./auto_file.h"
#include "mthelp.h"
#include "nano_log2.h"
#include "async_writer.h"

typedef nano::default_log_translator::cstr_ptr cstr_ptr;

class logger_instance;
class logger_wrapper
{
public:
	typedef nano::logger<
		nano::single_thread_executor<512, false>
	> logger_type;

	typedef std::unique_ptr<logger_type> logger_ptr_type;

	logger_wrapper(const char *scope_name, int scope_id, bool is_trace, size_t N_log_2 = 20);

	template <class ...Args>
	void info(Args &&...args) {
		logger_->info(std::forward<Args>(args)...);
	}

	template <class ...Args>
	void debug(Args &&...args) {
		logger_->debug(std::forward<Args>(args)...);
	}

	template <class ...Args>
	void warn(Args &&...args) {
		logger_->warn(std::forward<Args>(args)...);
		log_writer_->flush_file();
	}

	template <class ...Args>
	void error(Args &&...args) {
		logger_->error(std::forward<Args>(args)...);
		log_writer_->flush_file();
	}

	template <class ...Args>
	void trace(Args &&...args) {
		logger_->trace(std::forward<Args>(args)...);
	}

private:
	std::unique_ptr<logger_type::executor_type> log_exe_;
	std::unique_ptr<logger_type::writer_type> log_writer_;
	std::unique_ptr<logger_type> logger_;

	logger_wrapper() = delete;
	logger_wrapper(const logger_wrapper &) = delete;
	logger_wrapper &operator=(const logger_wrapper &) = delete;
	logger_wrapper &operator=(logger_wrapper &&) = delete;
};

class logger_instance {
public :
/*
	// MEMO : if execute "void init_instance()" in global, compiler treat it as declaration
	// error C2761: “void logger_instance::init_instance(const char *,int,bool)”: 不允许重新声明成员
	static void init_instance(const char *scope_name, int scope_id, bool is_trace) {
		if (!logger_) {
			logger_ = new logger_instance(scope_name, scope_id, is_trace);
		}
	}
*/
	static bool init_instance(const char *scope_name, int scope_id, bool is_trace, size_t N_log_2 = 20) {
		if (!logger_) {
			logger_ = new logger_wrapper(scope_name, scope_id, is_trace, N_log_2);
			return true;
		}
		return false;
	}

	template <class ...Args>
	static void info(Args &&...args) {
		assert(logger_);
		logger_->info(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void debug(Args &&...args) {
		assert(logger_);
		logger_->debug(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void warn(Args &&...args) {
		assert(logger_);
		logger_->warn(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void error(Args &&...args) {
		assert(logger_);
		logger_->error(std::forward<Args>(args)...);
	}

	template <class ...Args>
	static void trace(Args &&...args) {
		assert(logger_);
		logger_->trace(std::forward<Args>(args)...);
	}
	
private:
	static logger_wrapper* logger_;

	logger_instance() = delete;
	logger_instance(const logger_instance &) = delete;
	logger_instance &operator=(const logger_instance &) = delete;
	logger_instance &operator=(logger_instance &&) = delete;
};