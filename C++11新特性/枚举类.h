#pragma once
#include <vector>
using namespace std;
//当数据类型未知或不便书写时，使用auto可让编译器自动根据用以初始化变量的表达式类型来推导变量类型。
template<class T>
void printall(const vector<T>& v)
{
	for (auto p = v.begin(); p != v.end(); ++p)
	{
		cout << *p << "n";
	}
}

template<class T>
void printall2(const vector<T>& v)
{
	for (typename vector<T>::const_iterator p = v.begin();
		p != v.end(); ++p)
		cout << *p << "n";

}

template<class T, class U>
void multiply(const vector<T>& vt, const vector<U>& vu)
{
	typename B a = vt[0] * vu[0];
	cout << a << endl;
}