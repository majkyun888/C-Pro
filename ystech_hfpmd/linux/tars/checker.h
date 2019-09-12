#pragma once

namespace tars
{

template <class Logger>
class error_checker
{
	Logger &logger_;
	misc::cstr_t loc_;

public:
	error_checker(Logger &logger, misc::cstr_t loc)
		: logger_(logger), loc_(loc)
	{
	}

	template <class Rsp>
	bool isErrorRspInfo(Rsp *pRspInfo)
	{
		// 如果ErrorID != 0, 说明收到了错误的响应
		if ((pRspInfo != nullptr) && (pRspInfo->ErrorID != 0))
		{
			logger_.error("%s: error_id %d, error_msg %s",
				loc_, pRspInfo->ErrorID, misc::wrap_str(pRspInfo->ErrorMsg));
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	bool isNullParam(T *p, misc::cstr_t name)
	{
		if (p == nullptr)
		{
			logger_.error("%s: null %s", loc_, name);
			return true;
		}
		else
		{
			return false;
		}
	}
}; // class error_checker<Logger>

template <class Logger>
error_checker<Logger> make_checker(Logger &logger, misc::cstr_t loc)
{
	return error_checker<Logger>(logger, loc);
}

} // namespace tars
