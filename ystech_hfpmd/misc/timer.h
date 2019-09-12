/*
 * timer.h - Time calculating.
 *
 * Authors: Jia Wang
 */
#pragma once
#ifndef TIMER_H
#define TIMER_H
#include<iostream>
#include<vector>
using namespace std;
namespace os
{

namespace detail {class timer2_impl;}

// low resolution timer
class timer2
{
	timer2(timer2 &) = delete;
	timer2 &operator=(const timer2 &) = delete;

public:
	timer2(); // will set start and stop both to current
	~timer2();

	// count time between start and stop
	void start_now(); // current time
	void start_last(); // last stop
	void stop_now(); // current time

	double real() const;
	double user() const;
	double sys() const;

protected:
	std::unique_ptr<detail::timer2_impl> start_, stop_;
}; // class timer2

class timer_vec
{
public:
	timer_vec();

	void start_now();
	void start_last();
	void stop_now();

	size_t num_items() const;
	double real(size_t i) const;
	double user(size_t i) const;
	double sys(size_t i) const;

	double total_real() const;
	double total_user() const;
	double total_sys() const;

protected:
	std::vector<double> items_;
	
	timer2 timer_;

	double total_real_, total_user_, total_sys_;

}; // class timer_vec

// msec since Epoch
unsigned long long current_time_millis();

// nano-sec timestamp
unsigned long long nano_time();

} // namespace os

#endif
