/*
* nano_queue2.h - low-latency inter-thread communication
*
* Authors: Jia Wang
*/

#ifndef NANO_QUEUE2_H
#define NANO_QUEUE2_H

#include "./common.h"

namespace nano
{

namespace detail
{

// Manage aligned raw memory allocation for array of T,
// possibly with a larger element size.
template <class T, size_t SIZE = sizeof(T)>
class aligned_rawptr
{
	typedef aligned_rawptr<T, SIZE> this_type;
	aligned_rawptr(const this_type &) = delete;
	this_type &operator=(const this_type &) = delete;
	aligned_rawptr(this_type &&) = delete;
	this_type &operator=(this_type &&) = delete;

	char *const unaligned_;
	char *const aligned_;
	const size_t N_;

	static char *adjust(char *unaligned)
	{
		size_t addr = reinterpret_cast<size_t>(unaligned);
		size_t offset = SIZE-addr%SIZE;

		return unaligned+offset;
	}

public:
	aligned_rawptr(size_t N)
		: unaligned_(reinterpret_cast<char *>(
			::operator new(SIZE*(N+1)))),
		aligned_(adjust(unaligned_)), N_(N)
	{
		// check fit
		static_assert(
			SIZE >= sizeof(T),
			"size is too small");

		// check alignment
		static_assert(
			SIZE%alignof(T) == 0,
			"size is not multiples of alignment");
	}

	~aligned_rawptr()
	{
		::operator delete(unaligned_);
	}

	T *get() const
	{
		return reinterpret_cast<T *>(aligned_);
	}

	T &operator[](size_t i) const
	{
		assert(i < N_);
		return *reinterpret_cast<T *>(aligned_+i*SIZE);
	}
}; // class aligned_rawptr<T>

} // namespace detail

// Multi-producer simultaneous-consumer ringbuffer,
// similar to LMAX Disruptor but improved/simplified.
// Overflow may happen for max(size_t) elements.
template <
	class P, // element type, possibly a polymorphic interface
	size_t SIZE // max element size
>
class mpsc_queue
{
	typedef mpsc_queue<P, SIZE> this_type;

	enum {
		SEQ_INIT = 1,
		BUF_ALIGN = 16, // max element alignment
		ATOMIC_ALIGN = 64 // prevent false sharing
	};

	// fixed size slot with possibly polymorphic interface
	//
	// logical slots
	// EMPTY: end <= i
	//  ||
	//  || slot[end].seq = <end; ++end;
	//  \/
	// RESERVED: begin <= i < end && slot[i].seq < i
	//  ||
	//  || slot[i].seq = i;
	//  \/
	// READY or ABORTED: begin <= i < end && slot[i].seq == i
	//  ||
	//  || ++begin;
	//  \/
	// DONE: i < begin
	//
	// physical slots
	// EMPTY: end <= i
	//  ||
	//  || ++end;
	//  \/
	// RESERVED: begin <= i < end && slot[i%N].seq < i
	//  ||
	//  || i-begin <= N: slot[i%N].seq = i;
	//  \/
	// READY or ABORTED: begin <= i < end && slot[i%N].seq == i
	//  ||
	//  || ++begin;
	//  \/
	// DONE: i < begin
	//
	class slot
	{
		slot(const slot &) = delete;
		slot &operator=(const slot &) = delete;
		slot(slot &&) = delete;
		slot &operator=(slot &&) = delete;

		alignas(ATOMIC_ALIGN) std::atomic<size_t> seq_;

		// nullable
		P *p_;

		// need to set a size for VC to be happy
		alignas(BUF_ALIGN) char buf_[BUF_ALIGN];

		friend this_type;

	public:
		slot()
		{
			seq_.store(SEQ_INIT-1, std::memory_order_relaxed);
		}

		// producer: create the item and publish as READY
		template <class T, class ...Args>
		void create(size_t seq, Args &&...args)
		{
			// check alignment
			static_assert(
				BUF_ALIGN%alignof(T) == 0,
				"buffer alignment is too small");

			// approximated, use relaxed for efficiency
			assert(seq > seq_.load(std::memory_order_relaxed));

			p_ = new(buf_) T(std::forward<Args>(args)...);

			// store-store S: T() -> slot.seq
			seq_.store(seq, std::memory_order_release);
		}

		// producer: publish as ABORTED if cannot create
		void abort(size_t seq)
		{
			// approximated, use relaxed for efficiency
			assert(seq > seq_.load(std::memory_order_relaxed));

			p_ = nullptr;

			// store-store S: T() -> slot.seq
			seq_.store(seq, std::memory_order_release);
		}

		// consumer: see if the slot is available
		// should be called before get() and destroy()
		bool is_available(size_t seq) const
		{
			// load-load S: slot.seq -> T.*
			return seq_.load(std::memory_order_acquire) == seq;
		}

		// consumer: access the item
		// should be called after is_available()
		P *get(size_t seq)
		{
			// after is_available(), relaxed is suffice
			assert(seq == seq_.load(std::memory_order_relaxed));

			return p_;
		}

		// consumer: destroy the item
		// should be called after is_available()
		void destroy(size_t seq)
		{
			P *p = get(seq);
			if (p != nullptr)
				p->~P();
		}
	}; // class slot

	struct slot_range
	{
		alignas(ATOMIC_ALIGN) std::atomic<size_t> begin;
		alignas(ATOMIC_ALIGN) std::atomic<size_t> end;

		slot_range()
		{
			begin.store(SEQ_INIT, std::memory_order_relaxed);
			end.store(SEQ_INIT, std::memory_order_relaxed);
		}
	}; // struct slot_range

	const size_t N_;
	const size_t N_mask_; // for % N

	// slot and slot_range need special alignment
	// to prevent false sharing. Allocate them on
	// heap so the queue itself can use default
	// object alignment.
	const detail::aligned_rawptr<slot, SIZE> slots_;
	const detail::aligned_rawptr<slot_range> r_;
	
public:
	// Expose actual size of buffer so that
	// users may choose different concrete type.
	enum {
		BUF_SIZE = SIZE-offsetof(slot, buf_)
	};

private:
	// forward all requests to physical slots
	template <class T, class ...Args>
	void create(size_t seq, Args &&...args)
	{
		// check fit
		static_assert(
			BUF_SIZE >= sizeof(T),
			"size is too small");

		slots_[seq & N_mask_].create<T>(
			seq, std::forward<Args>(args)...);
	}
	void abort(size_t seq)
	{
		slots_[seq & N_mask_].abort(seq);
	}
	bool is_available(size_t seq)
	{
		return slots_[seq & N_mask_].is_available(seq);
	}
	P *get(size_t seq)
	{
		return slots_[seq & N_mask_].get(seq);
	}
	void destroy(size_t seq)
	{
		slots_[seq & N_mask_].destroy(seq);
	}

public:
	// You'll need to publish the this pointer to
	// all producer/consumer threads after ctor,
	// e.g. via a memory barrier in thread creation
	// or os synchronizations.
	mpsc_queue(size_t N)
		: N_(N), N_mask_(N-1), slots_(N), r_(1)
	{
		// power check
		assert((N_ & N_mask_) == 0);

		for (size_t i = 0; i < N_; ++i)
			new(&slots_[i]) slot();

		new(r_.get()) slot_range();
	}

	// Although the queue should be empty before destroyed
	// to avoid any memory leakage, we don't care as mostly
	// the dtor will never be actually called.
	~mpsc_queue()
	{
		for (size_t i = 0; i < N_; ++i)
			slots_[i].~slot();
		r_.get()->~slot_range();
	}

	// producer side
	//
	// First alloc(), then create() or abort() for each slot
	// in order. By default remaining slots will be aborted.
	//
	// Producer ownership may be transfered in the same thread.
	//
	// Choose your own AllocSpin to wait for empty slots.

	// RAII guarentees allocated slots to be published
	class producer
	{
		producer(const producer &) = delete;
		producer &operator=(const producer &) = delete;
		producer &operator=(producer &&) = delete;

		this_type &parent_;

		size_t begin_;
		size_t cur_;
		size_t end_;

		friend this_type;

		producer(this_type &parent,
			size_t begin, size_t end)
			: parent_(parent), begin_(begin),
			cur_(begin), end_(end)
		{
		}

	public:
		producer(producer &&rhs)
			:parent_(rhs.parent_), begin_(rhs.begin_),
			cur_(rhs.cur_), end_(rhs.end_)
		{
			// reset rhs
			rhs.begin_ = rhs.cur_ = rhs.end_;
		}

		~producer()
		{
			for (; cur_ != end_; ++cur_)
				parent_.abort(cur_);
		}

		size_t size() const
		{
			return end_-begin_;
		}

		size_t remaining() const
		{
			return cur_-begin_;
		}

		// create item and publish as READY
		template <class T, class ...Args>
		void create(Args &&...args)
		{
			assert(cur_ < end_);
			parent_.create<T>(cur_++, std::forward<Args>(args)...);
		}

		// publish as ABORTED
		void abort()
		{
			assert(cur_ < end_);
			parent_.abort(cur_++);
		}
	}; // class producer

	// allocate n (<=N) slots
	template <class AllocSpin>
	producer alloc(size_t n, AllocSpin spin)
	{
		assert(n > 0);
		assert(n <= N_);

		// compete with other producers
		size_t seq = r_.get()->end.fetch_add(n,
			std::memory_order_relaxed);

		// spin, which should not happen if N is big enough
		for (;;)
		{
			// load-store B: begin -> T() 
			size_t begin_local = r_.get()->begin.load(
				std::memory_order_acquire);
			if (seq+n-begin_local <= N_)
				return producer(*this, seq, seq+n);
			spin();
		}
	}
	producer alloc(size_t n)
	{
		return alloc(n, [] {});
	}

	// allocate 1 slot
	template <class AllocSpin>
	producer alloc_one(AllocSpin spin)
	{
		return alloc(1, spin);
	}
	producer alloc_one()
	{
		return alloc(1);
	}
	
	// consumer side
	//
	// Each consumer is responsible to track its own seq.
	// First call count_available() to determine how many
	// items are available, then access each by get()
	// (could be done out-of-order). Spin if necessary.
	//
	// The last consumer should call free() in-order.
	// 
	// You need to implement your own mechanism to support
	// synchronization between multiple consumers.

	size_t get_seq_init() const
	{
		return SEQ_INIT;
	}

	// count # of available items from seq
	size_t count_available(size_t seq)
	{
		// approximated, use relaxed for efficiency
		assert(seq >= r_.get()->begin.load(std::memory_order_relaxed));

		// check each, availability can be seen from slots
		for (size_t c = 0;; ++c)
			if (!is_available(seq+c))
				return c;
	}

	// return the item, nullptr for ABORTED
	P *get_one(size_t seq)
	{
		// approximated, use relaxed for efficiency
		assert(seq >= r_.get()->begin.load(std::memory_order_relaxed));

		return get(seq);
	}

	// last consumer: free n slots
	//
	// For now, let's assume the last consumer always stays
	// in the same thread for simplicity.
	void free(size_t n)
	{
		// all updates to begin happen in this thread
		size_t begin_local = r_.get()->begin.load(
			std::memory_order_relaxed);

		// destroy each item
		for (size_t c = 0; c < n; ++c)
			destroy(begin_local+c);

		// announce
		// load-store B: ~T() -> begin
		r_.get()->begin.store(begin_local+n,
			std::memory_order_release);
	}
}; // class mpsc_queue<P, SIZE>

} // namespace misc

#endif // !NANO_QUEUE2_H
