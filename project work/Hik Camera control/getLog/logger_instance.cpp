#include "logger_instance.h"
#include <tars/util.h>

logger_wrapper* logger_instance::logger_ = nullptr;

logger_wrapper::logger_wrapper(const char *scope_name, int scope_id, bool is_trace, size_t N_log_2) {
	assert(log_exe_.get() == nullptr);
	assert(log_writer_.get() == nullptr);
	assert(logger_.get() == nullptr);

	std::string log_name = std::string(scope_name);
	std::string log_file = log_name + ".log";
	log_writer_.reset(
		new logger_type::writer_type(log_name, true));
	if (!log_writer_->open_file(log_file))
	{
		printf("%s: error: cannot open %s for log\n",
			scope_name, log_file.c_str());
		fflush(stdout);
		throw std::runtime_error("init failed");
	}

	// create executor
	log_exe_.reset(new logger_type::executor_type(
		N_log_2/*20*/, 0, tars::util::micro_spin()));

	// create logger
	int log_level = is_trace ? nano::LOG_TRACE : nano::LOG_DEBUG;
	logger_.reset(new logger_type(
		log_level, *log_exe_, *log_writer_));
}