/*
* nano_log2.h - low-latency logging 
*
* Authors: Jia Wang
*/

#ifndef NANO_LOG2_H
#define NANO_LOG2_H

#include "nano_executor.h"
#include "auto_file.h"

namespace nano
{
using misc::cstr_t;

// Translate classes with .c_str(), arithmetic types,
// char arrays, string literals, and std::array<char, N>
// for use with printf-like loggers.
struct default_log_translator
{
	template <class T, class Enable = void>
	class wrapper
	{
		typedef typename std::decay<T>::type TT;

		TT t_;

		template <class C>
		static constexpr auto has_c_str(C *t)
			-> decltype(t->c_str(), bool())
		{
			return true;
		}
		static constexpr bool has_c_str(...)
		{
			return false;
		}

	public:
		wrapper(T &&t)
			: t_(std::forward<T>(t))
		{
			static_assert(
				has_c_str((TT *)nullptr),
				"cannot log object w/o member c_str()");
		}

		cstr_t unwrap() const
		{
			return t_.c_str();
		}
	};

	// pointers
	template <class T>
	class wrapper<T,
		typename std::enable_if<
			std::is_pointer<
				typename std::remove_reference<T>::type
			>::value
		>::type
	>
	{
	public:
		wrapper(T &&t)
		{
			static_assert(
				!std::is_pointer<
					typename std::remove_reference<T>::type
				>::value,
				"cannot log pointer");
		}
	};

	// arithmetic types
	template <class T>
	class wrapper<T,
		typename std::enable_if<
			std::is_arithmetic<
				typename std::decay<T>::type
			>::value
		>::type
	>
	{
		typedef typename std::decay<T>::type TT;
		TT t_;
	public:
		wrapper(T &&t)
			: t_(std::forward<T>(t))
		{
		}

		TT unwrap() const
		{
			return t_;
		}
	};

	// literals as char arrays
	template <size_t N>
	class wrapper<const char (&)[N], void>
	{
		std::array<char, N> a_;
	public:
		wrapper(const char (&a)[N])
		{
			std::copy(a, a+N, a_.data());
		}

		cstr_t unwrap() const
		{
			return a_.data();
		}
	};

	// char arrays
	template <size_t N>
	class wrapper<char (&)[N], void>
	{
		std::array<char, N> a_;
	public:
		wrapper(char (&a)[N])
		{
			std::copy(a, a+N, a_.data());
		}

		cstr_t unwrap() const
		{
			return a_.data();
		}
	};

	// std::array<char, N>
	template <size_t N>
	class wrapper<std::array<char, N>, void>
	{
		std::array<char, N> a_;
	public:
		wrapper(std::array<char, N> &&a)
			: a_(std::move(a))
		{
		}

		cstr_t unwrap() const
		{
			return a_.data();
		}
	};

	template <size_t N>
	class wrapper<std::array<char, N> &, void>
	{
		std::array<char, N> a_;
	public:
		wrapper(const std::array<char, N> &a)
			: a_(a)
		{
		}

		cstr_t unwrap() const
		{
			return a_.data();
		}
	};

	// literals as ptr, use with care
	class cstr_ptr
	{
		cstr_t p_;
	public:
		template <size_t N>
		cstr_ptr(const char (&a)[N])
			: p_(a)
		{
		}

		cstr_t c_str() const
		{
			return p_;
		}
	}; // class cstr_ptr
}; // struct default_log_translator

// write to file and stdout with datetime and prefix
class default_log_writer
{
	const std::string prefix_;
	const bool use_stdout_;
	os::auto_file file_;
	char time_str[100];

public:
	default_log_writer(
		const std::string &prefix,
		bool use_stdout)
		: prefix_(prefix), use_stdout_(use_stdout)
	{
		update_time();
	}

	bool open_file(const std::string &name)
	{
		return file_.file_open(name, "a");
	}

	FILE *get_fp()
	{
		return file_.get_fp();
	}

	// Getting time is expensive. Only update internal
	// time when necessary, e.g. before a batch begins.
	void update_time()
	{
		time_t now_t = time(nullptr);
		tm *now = localtime(&now_t);

		sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
			now->tm_year+1900, now->tm_mon+1, now->tm_mday,
			now->tm_hour, now->tm_min, now->tm_sec);
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
		if (use_stdout_)
		{
			fflush(stdout);
		}
	}

	template <class ...Args>
	void write(cstr_t level_str, Args &&...args)
	{
		FILE *fp = file_.get_fp();
		if (fp != nullptr)
		{
			fprintf(fp, "%s %s %s ",
				time_str, prefix_.c_str(), level_str);
			fprintf(fp, args...);
			fprintf(fp, "\n");
		}
		if (use_stdout_)
		{
			printf("%s %s %s ",
				time_str, prefix_.c_str(), level_str);
			printf(args...);
			printf("\n");
		}
	}
}; // class default_log_writer

// log levels
enum
{
	LOG_ERROR = 0,
	LOG_WARN = 1,
	LOG_INFO = 2,
	LOG_DEBUG = 3,
	LOG_TRACE = 4
};

// The objective of logger is to reduce the latency
// in the logging threads but the overall throughput
// is limited by the writer thread -- so be careful
// if you are logging from multiple threads.
template <
	class Executor,
	class Writer = default_log_writer,
	class Translator = default_log_translator
>
class logger
{
	typedef logger<Executor, Writer, Translator> this_type;

	const int level_;

	Executor &exe_;

	Writer &writer_;

	class writer_wrapper
	{
		Writer &writer_;

	public:
		writer_wrapper(Writer &writer)
			: writer_(writer)
		{
		}

		template <class ...Args>
		void operator()(cstr_t level_str, Args &&...args)
		{
			writer_.write(
				level_str, args.unwrap()...);
		}
	}; // writer_wrapper

	template <class ...Args>
	void submit(cstr_t level_str, Args &&...args)
	{
		exe_.alloc().submit(
			writer_wrapper(writer_),
			level_str,
			(typename Translator::template wrapper<Args>)(
				std::forward<Args>(args))...);
	}

public:
	logger(int level, Executor &exe, Writer &writer)
		: level_(level), exe_(exe), writer_(writer)
	{
		// allow writer to collect time at the
		// beginning of the batch
		exe_.set_on_batch_begin([this] {
			writer_.update_time();
		});
		// allow writer to flush file at the end
		// of the batch
		exe_.set_on_batch_end([this] {
			writer_.flush_file();
		});
	}

	typedef Writer writer_type;
	typedef Executor executor_type;
	typedef Translator translator_type;

	Writer &get_writer()
	{
		return writer_;
	}

	Executor &get_executor()
	{
		return exe_;
	}

	// call them to save time passing params
	bool is_trace_enabled() const
	{
		return LOG_TRACE <= level_;
	}
	bool is_debug_enabled() const
	{
		return LOG_DEBUG <= level_;
	}

	template <class ...Args>
	void trace(Args &&...args)
	{
		if (LOG_TRACE <= level_)
			submit("TRACE", std::forward<Args>(args)...);
	}

	template <class ...Args>
	void debug(Args &&...args)
	{
		if (LOG_DEBUG <= level_)
			submit("DEBUG", std::forward<Args>(args)...);
	}

	template <class ...Args>
	void info(Args &&...args)
	{
		if (LOG_INFO <= level_)
			submit("INFO", std::forward<Args>(args)...);
	}

	template <class ...Args>
	void warn(Args &&...args)
	{
		if (LOG_WARN <= level_)
			submit("WARN", std::forward<Args>(args)...);
	}
	template <class ...Args>
	void error(Args &&...args)
	{
		if (LOG_ERROR <= level_)
			submit("ERROR", std::forward<Args>(args)...);
	}
}; // class logger<Executor, Writer, Translator>

} // namespace nano

#endif // !NANO_LOG2_H
