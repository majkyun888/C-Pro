#pragma once

namespace tars
{

namespace util
{

using misc::cstr_t;

long long parse_millis(cstr_t hhmmss);

class micro_spin
{
	long long micro_;
public:
	micro_spin(long long micro = 10)
		: micro_(micro)
	{
	}

	void operator()() const
	{
		std::this_thread::sleep_for(
			std::chrono::microseconds(micro_));
	}
}; // class micro_spin

} // namespace util

} // namespace tars
