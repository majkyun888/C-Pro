#pragma once

namespace tars
{

// Todo: apply LRU to speed-up order lookup
template <class K, class V>
class order_cache
{
public:
	order_cache()
	{

	}

	V *put(const K &k, const V &v = V())
	{
		typename table_type::iterator it = orders_.insert(
			std::make_pair(k, v)).first;
		return &it->second;
	}

	V *get(const K &k)
	{
		typename table_type::iterator it = orders_.find(k);
		return (it == orders_.end())? nullptr: &it->second;
	}

private:
	typedef std::map<K, V> table_type;
	table_type orders_;
}; // class order_cache

} // namespace tars
