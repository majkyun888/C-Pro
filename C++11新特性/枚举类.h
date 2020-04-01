#pragma once
#include <vector>
using namespace std;
//����������δ֪�򲻱���дʱ��ʹ��auto���ñ������Զ��������Գ�ʼ�������ı��ʽ�������Ƶ��������͡�
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