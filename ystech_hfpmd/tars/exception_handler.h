#pragma once
#include <stdio.h>
#include <atomic>
#include <functional>
#include <mutex>

namespace tars
{
	const int EX_UNHANDLED = -100;
	const int EX_HANDLED = -101;

	const int EX_SOCKET = -1000;
	const int EX_SOCKET_INIT = -1001;

	const int EX_REQUEST = -1100;
	const int EX_REQ_INIT = -1101;
	const int EX_REQ_START = -1102;
	const int EX_REQ_LOGIN = -1103;
	const int EX_REQ_ACCOUNT = -1104;
	const int EX_REQ_POSITION = -1105;
	const int EX_REQ_SUBMIT = -1106;
	const int EX_REQ_CANCEL = -1017;
	const int EX_REQ_SUBSCRIBE = -1008;

	const int EX_CALLBACK = -1200;

	class exception_handler {
	public :
		typedef std::function<void(int last_ex)> callback_type;

		exception_handler(bool is_rethrow_ex)
			: is_rethrow_ex_(is_rethrow_ex)
			, callback_func_(nullptr)
			, last_ex_(0)
		{ }

		// callback_func should be thread safe.
		void init_callback(callback_type callback_func) {
			if (callback_func_ == nullptr) {
				callback_func_ = callback_func;
			}
		}

		int last_ex() {
			return last_ex_;
		}

		// operator() will be called in async
		int operator()(int biz_ex, int func_ex) {
			if (func_ex == 0) {
				return 0;
			}
			else {
				//std::lock_guard<std::mutex> lg(debug_mutex_);

				printf("[exception_handler] catch biz_ex=%d, func_ex=%d\n", biz_ex, func_ex);
				fflush(stdout);

				/*if (last_ex_ == 0) {
					last_ex_ = biz_ex;
				}*/
				int old_last_ex = last_ex_;
				int expect_ex = 0;
				bool is_exchanged = last_ex_.compare_exchange_strong(expect_ex, biz_ex);
				printf("[exception_handler] is_exchanged=%d, old_last_ex=%d, new_last_ex=%d\n", is_exchanged, old_last_ex, last_ex_.load());	
				
				if (!is_rethrow_ex_ && callback_func_) {
					callback_func_(last_ex_);
				}
				return biz_ex;
			}
		}

	private:
		bool is_rethrow_ex_;
		callback_type callback_func_;
		std::atomic<int> last_ex_;
		std::mutex debug_mutex_;
	};
} // namespace tars
