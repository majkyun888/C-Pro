#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace std;

template<typename X>
void f(vector<X>& v)
{
	struct Less {
		bool operator() (const X& a, const X& b)
		{
			return a < b;
		}
	};
	sort(v.begin(), v.end(), Less());
}
template <typename X>
void f1(vector<X>& v)
{
	sort(v.begin(), v.end(),
		[](const X& a, const X& b) {return a < b; });
}

template <typename T> void foo(T const& t) {}
enum X { x };
enum { y };
