/*
 * json_writer.h - Append-only writer for json.
 *
 * Authors: Jia Wang
 *
 */

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include "common.h"

namespace misc
{

template <bool pretty = false> // one line or pretty
class json_writer
{
	// when appending, we store w/o the ending '\0'
	// the '\0' is added after build
	std::vector<char> buf_;
	size_t len_;

	// more space for n char+\0
	char *more(size_t n)
	{
		buf_.reserve(len_+n+1);
		buf_.resize(buf_.capacity());
		return &buf_[len_];
	}

	// append string
	void append(cstr_t str)
	{
		size_t n = strlen(str);
		memcpy(more(n), str, n);
		len_ += n;
	}
	void append(const std::string &str)
	{
		size_t n = str.size();
		memcpy(more(n), str.c_str(), n);
		len_ += n;
	}
	template <size_t N>
	void append(const std::array<char, N> &str)
	{
		append(str.data());
	}

	// pretty by two spaces per level
	template <size_t L>
	void make_pretty()
	{
		if (!pretty)
			return;

		append("\n");
		for (size_t i = 0; i < L; ++i)
			append("  ");
	}

	// key
	template <class K>
	void append_key(bool first, K &&k)
	{
		append("\"");
		append(std::forward<K>(k));
		append("\":");
	}

	// value, default to non-number
	template <class V, class = typename std::enable_if<
		!std::is_arithmetic<V>::value>::type>
	void append_value(V &&v)
	{
		append("\"");
		append(std::forward<V>(v));
		append("\"");
	}
	void append_value(long long i)
	{
		len_ += sprintf(more(100), "%lld", i);
	}
	void append_value(unsigned long long i)
	{
		len_ += sprintf(more(100), "%llu", i);
	}
	void append_value(int i)
	{
		len_ += sprintf(more(100), "%d", i);
	}
	void append_value(unsigned int i)
	{
		len_ += sprintf(more(100), "%u", i);
	}
	void append_value(long i)
	{
		len_ += sprintf(more(100), "%ld", i);
	}
	void append_value(unsigned long i)
	{
		len_ += sprintf(more(100), "%lu", i);
	}
	void append_value(double f)
	{
		len_ += sprintf(more(100), "%f", f);
	}

public:
	json_writer() : len_(0)
	{
	}

	typedef json_writer<pretty> root_type;

	template <class P, size_t L> class json_array;
	template <class P, size_t L>
	class json_object
	{
		root_type &r_;
		bool first_;
	public:
		typedef json_object<P, L> this_type;
		typedef json_object<this_type, L+1> next_object_type;
		typedef json_array<this_type, L+1> next_array_type;

		json_object(root_type &r, bool first) :
			r_(r), first_(first)
		{
			if (first_)
			{
				if (L == 1)
					r_.len_ = 0;
				r_.append("{");
			}
		}

		P close() const
		{
			r_.template make_pretty<L-1>();
			r_.append("}");
			if (L == 1)
				r_.buf_[r_.len_] = 0;
			return P(r_);
		}

		template <class T>
		this_type append(const T &t)
		{
			return json_append(*this, t);
		}

		template <class T, class F>
		this_type append(const T &t, F f)
		{
			return f(*this, t);
		}

		template <class K>
		next_object_type object(K &&k) const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			r_.append_key(first_, std::forward<K>(k));
			return next_object_type(r_, true);
		}

		template <class K>
		next_array_type array(K &&k) const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			r_.append_key(first_, std::forward<K>(k));
			return next_array_type(r_, true);
		}

		template <class K, class V>
		this_type put(K &&k, V &&v) const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			r_.append_key(first_, std::forward<K>(k));
			r_.append_value(std::forward<V>(v));
			return this_type(r_, false);
		}
	}; // class json_object<P, L>

	template <class P, size_t L>
	class json_array
	{
		root_type &r_;
		bool first_;
	public:
		typedef json_array<P, L> this_type;
		typedef json_object<this_type, L+1> next_object_type;
		typedef json_array<this_type, L+1> next_array_type;

		json_array(root_type &r, bool first) :
			r_(r), first_(first)
		{
			if (first_)
			{
				if (L == 1)
					r_.len_ = 0;
				r_.append("[");
			}
		}

		P close() const
		{
			r_.template make_pretty<L-1>();
			r_.append("]");
			if (L == 1)
				r_.buf_[r_.len_] = 0;
			return P(r_);
		}

		template <class T>
		this_type append(const T &t)
		{
			return json_append(*this, t);
		}

		template <class T, class F>
		this_type append(const T &t, F f)
		{
			return f(*this, t);
		}

		template <class K>
		next_object_type object() const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			return next_object_type(r_, true);
		}

		template <class K>
		next_array_type array() const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			return next_array_type(r_, true);
		}

		template <class V>
		this_type put(V &&v) const
		{
			if (!first_)
				r_.append(",");
			r_.template make_pretty<L>();
			r_.append_value(std::forward<V>(v));
			return this_type(r_, true);
		}
	}; // class json_array<P, L>

	// place holder, do nothing
	class json_0
	{
		root_type &r_;
	public:
		json_0(root_type &r): r_(r) {}
		cstr_t get() const {return &r_.buf_[0];}
	}; // class json_0

	typedef json_object<json_0, 1> json_object_1;
	typedef json_array<json_0, 1> json_array_1;
	json_object_1 object() {return json_object_1(*this, true);}
	json_array_1 array() {return json_array_1(*this, true);}
	cstr_t get() const {return &buf_[0];}
	size_t len() const {return len_;}

	// shortcuts

	template <class T>
	cstr_t to_str(const T &t)
	{
		return object().append(t).close().get();
	}


}; // class json_writer<pretty>

} // namespace misc

#endif // JSON_WRITER_H
