#pragma once

#include "../misc/nano_log2.h"

namespace tars
{

typedef nano::default_log_translator::cstr_ptr cstr_ptr;

template <class Logger>
class error_checker2
{
	Logger &logger_;

	cstr_ptr loc_;

public:
	error_checker2(Logger &logger, cstr_ptr loc)
		: logger_(logger), loc_(loc)
	{
	}

	template <class Rsp>
	bool isErrorRspInfo(Rsp *pRspInfo)
	{
		// 如果ErrorID != 0, 说明收到了错误的响应
		if ((pRspInfo != nullptr) && (pRspInfo->ErrorID != 0))
		{
			logger_.error(cstr_ptr("%s: error_id %d, error_msg %s"),
				loc_, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	bool isNullParam(T *p, cstr_ptr name)
	{
		if (p == nullptr)
		{
			logger_.error(cstr_ptr("%s: null %s"), loc_, name);
			return true;
		}
		else
		{
			return false;
		}
	}
}; // class error_checker<Logger>

template <class Logger>
error_checker2<Logger> make_checker2(
	Logger &logger, cstr_ptr loc)
{
	return error_checker2<Logger>(logger, loc);
}

} // namespace tars
