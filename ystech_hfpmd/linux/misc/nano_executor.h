/*
* executor.h - low-latency runnable and executor
*
* Authors: Jia Wang
*/

#ifndef NANO_EXECUTOR_H
#define NANO_EXECUTOR_H

#include "./nano_queue2.h"
#include "./timer.h"

namespace nano
{

namespace detail
{

// allow to run a task w/o knowing its type
struct runnable
{
	virtual void run() = 0;
	virtual ~runnable() {}
}; // struct runnable

template <int N>
struct unpack_tuple_and_call
{
	template <class Tuple, class F, class ...Args>
	static void apply(Tuple &&t, F &&f, Args &&...args)
	{
		unpack_tuple_and_call<N-1>::apply(
			std::forward<Tuple>(t),
			std::forward<F>(f),
			std::get<N-1>(std::forward<Tuple>(t)),
			std::forward<Args>(args)...);
	}
}; // struct unpack_tuple_and_call<N>
template <>
struct unpack_tuple_and_call<0>
{
	template <class Tuple, class F, class ...Args>
	static void apply(Tuple &&t, F &&f, Args &&...args)
	{
		f(std::forward<Args>(args)...);
	}
}; // struct unpack_tuple_and_call<0>

// wrap a closure to be used for runnable
template <class F, class... Args>
class runnable_adaptor: public runnable
{
	// use decayed type to make copies of everything
	typename std::decay<F>::type f_;
	std::tuple<typename std::decay<Args>::type...> args_tuple_;

public:
	runnable_adaptor(F &&f, Args &&...args)
		: f_(std::forward<F>(f)),
		args_tuple_(std::forward<Args>(args)...)
	{
	}

	void run() override
	{
		unpack_tuple_and_call<sizeof...(Args)>
			::apply(args_tuple_, f_);
	}
}; // class runnable_adaptor<F, Args...>

} // namespace detail

// actor support via thread confinement
//
// Notes on ENABLE_STD_FUNCTION:
// 1 Allow to use std::function for closures larger than SIZE.
// 2 May impact performance.
template <
	size_t SIZE = 512,
	bool ENABLE_STD_FUNCTION = true
>
class single_thread_executor
{
	typedef single_thread_executor<SIZE, ENABLE_STD_FUNCTION> this_type;//single_thread_executorÀà±ðÃû

	typedef mpsc_queue<detail::runnable, SIZE> queue;

	queue Q_;

	// confined to thread
	size_t max_batch_; // 0 for infinity
	bool done_;
	std::list<std::function<void()>> on_batch_begin_;
	std::list<std::function<void()>> on_batch_end_;
	std::multimap<long long, std::function<void()>> scheduled_;

	// make sure this is the last to ensure
	// that all member variables are visible
	std::thread thread_;

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

	// return #elements processed
	size_t run_batch(size_t seq, size_t n)
	{
		// notify batch begin
		if (!on_batch_begin_.empty())
		{
			for (auto f: on_batch_begin_)
				f();
		}

		size_t batch_n = (max_batch_ == 0)?
			n : std::min(n, max_batch_);
		for (size_t i = 0; i < batch_n; ++i, ++seq)
		{
			detail::runnable *r = Q_.get_one(seq);

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

		// see if we could run any scheduled task
		run_scheduled();

		return batch_n;
	}

	template <class WaitSpin>
	void run(WaitSpin spin)
	{
		for (size_t seq = Q_.get_seq_init();;)
		{
			size_t n = Q_.count_available(seq);

			if (n == 0)
			{
				// see if we could run any scheduled task
				if (!run_scheduled())
					spin();
				continue;
			}

			// run tasks in batch
			for (; n > 0;)
			{
				size_t processed = run_batch(seq, n);

				seq += processed;
				n -= processed;
			}

			// shutdown?
			if (done_)
				return;
		}
	}

	typedef typename queue::producer producer;

	struct tag_F {};
	struct tag_std {};

	template <class F, class ...Args>
	void do_create(tag_F t, producer &p, F &&f, Args &&...args)
	{
		typedef detail::runnable_adaptor<F, Args...> adaptor_F;

		p.create<adaptor_F>(
			std::forward<F>(f), std::forward<Args>(args)...);
	}
	template <class F, class ...Args>
	void do_create(tag_std t, producer &p, F &&f, Args &&...args)
	{
		typedef std::function<void()> std_func;
		typedef detail::runnable_adaptor<std_func> adaptor_std;

		p.create<adaptor_std>(std::bind(f, args...));
	}

	template <class F, class ...Args>
	void create(producer &p, F &&f, Args &&...args)
	{
		typedef detail::runnable_adaptor<F, Args...> adaptor_F;

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

		long long at_ns_;

	public:
		defer(this_type &parent, long long at_ns)
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
	// 3 spin: wait spin policy
	//   less => reduce end-to-end latency but more CPU usage
	template <class WaitSpin>
	single_thread_executor(size_t N_log_2,
		size_t max_batch, WaitSpin spin)
		: Q_(size_t(1) << N_log_2), max_batch_(max_batch),
		done_(false), thread_([this, spin] {run(spin);})
	{
	}

	single_thread_executor(size_t N_log_2, size_t max_batch = 0)
		: single_thread_executor(N_log_2, max_batch, [] {})
	{
	}

	// Submit a task by alloc().submit() or submit().
	// Schedule a task by alloc().schedule_at() or schedule().
	//
	// The memory barrier in alloc() may prevent
	// compiler optimization via reordering object
	// creation. We therefore expose this step in
	// case you need such optimization.

	class producer_wrapper
	{
		this_type &parent_;

		producer p_;

		friend this_type;

		producer_wrapper(this_type &parent, producer &&p)
			: parent_(parent), p_(std::move(p))
		{
		}

	public:
		producer_wrapper(producer_wrapper &&rhs)
			: parent_(rhs.parent_), p_(std::move(rhs.p_))
		{
		}

		template <class F, class ...Args>
		void submit(F &&f, Args &&...args)
		{
			parent_.create(p_,
				std::forward<F>(f),
				std::forward<Args>(args)...);
		}

		template <class F, class ...Args>
		void schedule_at(long long at_ns, F &&f, Args &&...args)
		{
			parent_.create(p_,
				defer(parent_, at_ns),
				std::forward<F>(f),
				std::forward<Args>(args)...);
		}
	}; // class producer_wrapper

	template <class AllocSpin>
	producer_wrapper alloc(AllocSpin spin)
	{
		return producer_wrapper(*this, Q_.alloc_one(spin));
	}
	producer_wrapper alloc()
	{
		return producer_wrapper(*this, Q_.alloc_one([] {}));
	}

	template <class F, class ...Args>
	void submit(F &&f, Args &&...args)
	{
		alloc().submit(
			std::forward<F>(f),
			std::forward<Args>(args)...);
	}

	template <class F, class ...Args>
	void schedule(size_t after_ns, F &&f, Args &&...args)
	{
		// special case
		if (after_ns == 0)
		{
			alloc().submit(
				std::forward<F>(f),
				std::forward<Args>(args)...);
			return;
		}

		long long at_ns = os::nano_time()+after_ns;

		alloc().schedule_at(at_ns,
			std::forward<F>(f),
			std::forward<Args>(args)...);
	}

	template <class F>
	void set_on_batch_begin(F &&f)
	{
		alloc().submit([this, f] {
			on_batch_begin_.emplace_back(f);
		});
	}

	template <class F>
	void set_on_batch_end(F &&f)
	{
		alloc().submit([this, f] {
			on_batch_end_.emplace_back(f);
		});
	}

	void shutdown()
	{
		alloc().submit([this] {
			done_ = true;
		});
		thread_.join();
	}
}; // class single_thread_executor<SIZE, ENABLE_STD_FUNCTION>

} // namespace nano

#endif // !NANO_EXECUTOR_H
