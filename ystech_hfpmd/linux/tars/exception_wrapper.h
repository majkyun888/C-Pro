#pragma once
#include  "exception_handler.h"

namespace tars
{

// allow to wrap a function with exception handlers
class exception_wrapper
{
	misc::cstr_t context_;
	bool rethrow_;
public:
	exception_wrapper(misc::cstr_t context, bool rethrow = true)
		: context_(context), rethrow_(rethrow)
	{
		printf("[exception_wrapper.debug]rethrow=%d\n", rethrow_);
	}

	template <class F, class ...Args>
	int operator()(misc::cstr_t name, F &&f, Args &&...args)
	{
		try
		{
			f(std::forward<Args>(args)...);
			return 0;
		}
		catch (std::exception &e)
		{
			printf("[exception_wrapper.debug]%s::%s error: %s\n",
				context_, name, e.what());
			fflush(stdout);
			if (rethrow_)
				throw;
			else
				return EX_HANDLED;
		}
		catch (...)
		{
			printf("[exception_wrapper.debug]%s::%s error: unknown exception\n",
				context_, name);
			fflush(stdout);
			if (rethrow_)
				throw;
			else
				return EX_UNHANDLED;
		}
		return 0;
	}
}; // class exception_wrapper

} // namespace tars
