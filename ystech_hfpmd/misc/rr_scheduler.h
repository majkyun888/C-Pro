/*
* rr_scheduler.h - round-robin scheduler
*
* Authors: Jia Wang
*/

#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "nano_executor.h"
#include "timer.h"

namespace os
{

// Round-robin scheduler
// 1 Run on the current thread
// 2 Repeatedly execute all jobs on a round-robin basis
// 3 All threads may submit tasks, possibly using a timer
//
// Notes on ENABLE_STD_FUNCTION:
// 1 Allow to use std::function for closures larger than SIZE.
// 2 May impact performance.
template <
	size_t SIZE = 512,
	bool ENABLE_STD_FUNCTION = true
>
class rr_scheduler
{
	typedef rr_scheduler<SIZE, ENABLE_STD_FUNCTION> this_type;

	typedef nano::mpsc_queue<nano::detail::runnable, SIZE> queue;

	queue Q_;

	// confined to current thread

	// spin interval
	long long spin_us_;

	// batch processing on Q
	size_t seq_;
	size_t max_batch_; // 0 for infinity
	std::list<std::function<void()>> on_batch_begin_;
	std::list<std::function<void()>> on_batch_end_;

	// timer
	std::multimap<unsigned long long,
		std::function<void()>> scheduled_;

	// jobs
	std::multimap<int, std::function<bool()>> jobs_;

	bool run_scheduled()
	{
		bool ret = false;

		for (; !scheduled_.empty();)
		{
			auto it = scheduled_.begin();

			// not yet
			if (it->first > os::nano_time())
				return ret;

			// fire
			it->second();
			scheduled_.erase(it);
			ret = true;
		}

		return ret;
	}

	bool run_batch()
	{
		size_t n = Q_.count_available(seq_);

		if (n == 0)
			return false;

		// notify batch begin
		if (!on_batch_begin_.empty())
		{
			for (auto f: on_batch_begin_)
				f();
		}

		size_t batch_n = (max_batch_ == 0)?
			n : std::min(n, max_batch_);
		for (size_t i = 0; i < batch_n; ++i, ++seq_)
		{
			nano::detail::runnable *r = Q_.get_one(seq_);

			// some producer may abort
			if (r != nullptr)
				r->run();
		}
		Q_.free(batch_n);

		// notify batch end
		if (!on_batch_end_.empty())
		{
			for (auto f: on_batch_end_)
				f();
		}

		return true;
	}

	typedef typename queue::producer producer;

	struct tag_F {};
	struct tag_std {};

	template <class F, class ...Args>
	void do_create(tag_F t, producer &p, F &&f, Args &&...args)
	{
		typedef nano::detail::runnable_adaptor<F, Args...> adaptor_F;

		p.create<adaptor_F>(
			std::forward<F>(f), std::forward<Args>(args)...);
	}
	template <class F, class ...Args>
	void do_create(tag_std t, producer &p, F &&f, Args &&...args)
	{
		typedef std::function<void()> std_func;
		typedef nano::detail::runnable_adaptor<std_func> adaptor_std;

		p.create<adaptor_std>(std::bind(f, args...));
	}

	template <class F, class ...Args>
	void create(producer &p, F &&f, Args &&...args)
	{
		typedef nano::detail::runnable_adaptor<F, Args...> adaptor_F;

		constexpr bool FIT_F = (queue::BUF_SIZE >= sizeof(adaptor_F));

		// check fit
		static_assert(
			FIT_F || ENABLE_STD_FUNCTION,
			"size is too small, consider to use std::function");

		// select an implementation
		typedef typename std::conditional<
			FIT_F, tag_F, tag_std>::type tag;

		do_create(tag(), p,
			std::forward<F>(f),
			std::forward<Args>(args)...);
	}

	// confined to thread
	class defer
	{
		this_type &parent_;

		unsigned long long at_ns_;

	public:
		defer(this_type &parent, unsigned long long at_ns)
			: parent_(parent), at_ns_(at_ns)
		{
		}

		template <class F, class ...Args>
		void operator()(F &&f, Args &&...args)
		{
			parent_.scheduled_.emplace(at_ns_, std::bind(
				std::forward<F>(f), std::forward<Args>(args)...));
		}
	}; // class defer

public:
	// Allow to choose:
	// 1 N_log_2: log2(queue size)
	//   more => reduce call-site latency but more memory
	// 2 max_batch: max batch size, 0 for infinity
	//   less => better scheduler delay but less throughput
	rr_scheduler(
		size_t N_log_2,
		size_t max_batch,
		long long spin_us)
		: Q_(size_t(1) << N_log_2),
		spin_us_(spin_us),
		seq_(Q_.get_seq_init()),
		max_batch_(max_batch)
	{
		jobs_.emplace(0, [this] {return run_batch();});
		jobs_.emplace(100, [this] {return run_scheduled();});
	}

	// safe from all threads
	template <class F, class ...Args>
	void submit(F &&f, Args &&...args)
	{
		producer p = std::move(Q_.alloc_one([] {}));
		create(p,
			std::forward<F>(f),
			std::forward<Args>(args)...);
	}

	// safe from all threads
	template <class F, class ...Args>
	void schedule(size_t after_ns, F &&f, Args &&...args)
	{
		unsigned long long at_ns = os::nano_time()+after_ns;

		producer p = std::move(Q_.alloc_one([] {}));
		create(p,
			defer(*this, at_ns),
			std::forward<F>(f),
			std::forward<Args>(args)...);
	}

	// safe from all threads
	template <class F>
	void set_on_batch_begin(F &&f)
	{
		submit([this, f] {
			on_batch_begin_.emplace_back(f);
		});
	}

	// safe from all threads
	template <class F>
	void set_on_batch_end(F &&f)
	{
		submit([this, f] {
			on_batch_end_.emplace_back(f);
		});
	}

	// safe from all threads
	// 1 Jobs with smaller priorities will run first
	// 2 Queued tasks have a priority of 0
	// 3 Timed tasks have a priority of 100
	template <class F>
	void add_job(int priority, F &&f)
	{
		submit([priority, this, f] {
			jobs_.emplace(priority, f);
		});
	}

	// will not return, may throw
	void run()
	{
		for (;;)
		{
			bool any = false;
			for (auto &pri_job: jobs_)
				if (pri_job.second())
					any = true;

			if (!any && spin_us_ != 0)
				std::this_thread::sleep_for(
					std::chrono::microseconds(spin_us_));
		}
	}
}; // class rr_scheduler<SIZE, ENABLE_STD_FUNCTION>

} // namespace os

#endif // !RR_SCHEDULER_H
