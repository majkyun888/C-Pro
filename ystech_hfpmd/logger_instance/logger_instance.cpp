//#include "logger_instance.h"
//#include <util.h>
//
//logger_instance* logger_instance::instance_ = nullptr;
//
//logger_instance::logger_instance(const char *scope_name, int scope_id, bool is_trace) {
//	assert(st_log_exe.get() == nullptr);
//	assert(st_log_writer.get() == nullptr);
//	assert(st_logger.get() == nullptr);
//	assert(st_md_writer.get() == nullptr);
//
//	std::string log_name = std::string(scope_name);
//	std::string log_file = log_name + ".log";
//	st_log_writer.reset(
//		new logger_type::writer_type(log_name, true));
//	if (!st_log_writer->open_file(log_file))
//	{
//		printf("%s: error: cannot open %s for log\n",
//			scope_name, log_file.c_str());
//		fflush(stdout);
//		throw std::runtime_error("init failed");
//	}
//
//	// create executor
//	st_log_exe.reset(new logger_type::executor_type(
//		20, 0, tars::util::micro_spin()));
//
//	/*
//	// create ticks file
//	std::string ticks_file = std::string(scope_name) + ".mt_ticks";
//	st_md_writer.reset(new writer_type(*st_log_exe));
//	if (!st_md_writer->open_file(ticks_file, "a"))
//	{
//		printf("%s: error: cannot open %s for ticks\n",
//			scope_name, ticks_file.c_str());
//		fflush(stdout);
//		throw std::runtime_error("init failed");
//	}
//	st_md_writer->write_one([](FILE *fp, long long) {
//		fprintf(fp, "%s,OrigMillis\n", "mini_tars::md_csv_headers()");
//	});
//	*/
//
//	// create logger
//	int log_level = is_trace ? nano::LOG_TRACE : nano::LOG_DEBUG;
//	st_logger.reset(new logger_type(
//		log_level, *st_log_exe, *st_log_writer));
//}