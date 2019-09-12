/*
 * timer.cpp - Implement time calculating.
 *
 * Authors: Jia Wang
 */

#include "common.h"

#ifdef _WIN32

#include <windows.h>

#else // !_WIN32

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#endif // _WIN32

#include "timer.h"

namespace os
{

#ifdef _WIN32

namespace detail
{

class timer2_impl
{
	ULARGE_INTEGER real_, user_, sys_;

public:
	timer2_impl()
	{
		get_current_time();
	}

	void get_current_time()
	{
		GetSystemTimeAsFileTime((LPFILETIME)&real_);

		FILETIME a, b;
		GetProcessTimes(GetCurrentProcess(), &a, &b,
			(LPFILETIME)&sys_, (LPFILETIME)&user_);
	}

	double real(const timer2_impl &t) const
	{
		return (real_.QuadPart-t.real_.QuadPart)/1e7;
	}
	double user(const timer2_impl &t) const
	{
		return (user_.QuadPart-t.user_.QuadPart)/1e7;
	}
	double sys(const timer2_impl &t) const
	{
		return (sys_.QuadPart-t.sys_.QuadPart)/1e7;
	}

}; // timer2_impl

class performance_counter_helper
{
	LARGE_INTEGER freq;

public:
	performance_counter_helper()
	{
		QueryPerformanceFrequency(&freq);
	}

	long long nano_time()
	{
		LARGE_INTEGER cur;
		QueryPerformanceCounter(&cur);

		return cur.QuadPart*1000000000LL/freq.QuadPart;
	}

} performance_counter;

} // namespace detail

unsigned long long current_time_millis()
{
	ULARGE_INTEGER gmt;
	GetSystemTimeAsFileTime((LPFILETIME)&gmt);

	return (gmt.QuadPart-116444736000000000LL)/10000LL;
}

unsigned long long nano_time()
{
	return detail::performance_counter.nano_time();
}

#else // !_WIN32

namespace detail
{

class timer2_impl
{
	double real_, user_, sys_;

public:
	timer2_impl()
	{
		get_current_time();
	}

	void get_current_time()
	{
		timeval tv;
		gettimeofday(&tv, 0);

		rusage ru;
		getrusage(RUSAGE_SELF, &ru);

		real_ = tv.tv_sec+tv.tv_usec/1e6;
		user_ = ru.ru_utime.tv_sec+ru.ru_utime.tv_usec/1e6;
		sys_ = ru.ru_stime.tv_sec+ru.ru_stime.tv_usec/1e6;
	}

	double real(const timer2_impl &t) const
	{
		return real_-t.real_;
	}
	double user(const timer2_impl &t) const
	{
		return user_-t.user_;
	}
	double sys(const timer2_impl &t) const
	{
		return sys_-t.sys_;
	}

}; // timer2_impl

} // namespace detail

unsigned long long current_time_millis()
{
	timeval tv;
	gettimeofday(&tv, 0);

	return (unsigned long long)(tv.tv_sec)*1000LL
		+(unsigned long long)(tv.tv_usec)/1000LL;
}

unsigned long long nano_time()
{
	timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	return (unsigned long long)(tp.tv_sec)*1000000000LL
		+(unsigned long long)tp.tv_nsec;
}

#endif // _WIN32

timer2::timer2()
	: start_(new detail::timer2_impl), stop_(new detail::timer2_impl)
{
}

timer2::~timer2()
{
}

void timer2::start_now()
{
	start_->get_current_time();
}

void timer2::start_last()
{
	*start_ = *stop_;
}

void timer2::stop_now()
{
	stop_->get_current_time();
}

double timer2::real() const
{
	return stop_->real(*start_);
}

double timer2::user() const
{
	return stop_->user(*start_);
}

double timer2::sys() const
{
	return stop_->sys(*start_);
}

timer_vec::timer_vec()
	:total_real_(0), total_user_(0), total_sys_(0)
{
	items_.reserve(3000);
}

void timer_vec::start_now()
{
	timer_.start_now();
}

void timer_vec::start_last()
{
	timer_.start_last();
}

void timer_vec::stop_now()
{
	timer_.stop_now();

	double r = timer_.real();
	double u = timer_.user();
	double s = timer_.sys();

	items_.push_back(r);
	items_.push_back(u);
	items_.push_back(s);

	total_real_ += r;
	total_user_ += u;
	total_sys_ += s;
}

size_t timer_vec::num_items() const
{
	return items_.size()/3;
}

double timer_vec::real(size_t i) const
{
	assert(i < num_items());

	return items_[i*3];
}

double timer_vec::user(size_t i) const
{
	assert(i < num_items());

	return items_[i*3+1];
}

double timer_vec::sys(size_t i) const
{
	assert(i < num_items());

	return items_[i*3+2];
}

double timer_vec::total_real() const
{
	return total_real_;
}

double timer_vec::total_user() const
{
	return total_user_;
}

double timer_vec::total_sys() const
{
	return total_sys_;
}

} // namespace os
