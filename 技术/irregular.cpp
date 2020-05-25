#include "pch.h"
#include <iostream>
#include <string>
#include<vector>
using namespace std;
//class CException	//异常处理类
//{
//public:
//	string msg;
//	CException(string s) :msg(s)
//	{}
//};
//double Devide(double x, double y)
//{
//	if (0 == y)
//		throw CException("devided by zero");
//	cout << "In Devide" << endl;
//	return x / y;
//}
//int CountTax(int salary)
//{
//	try
//	{
//		if (salary < 0)
//			throw - 1;
//		cout << "counting tax" << endl;
//	}
//	catch (int)
//	{
//		cout << "salary < 0" << endl;
//	}
//	cout << "tax counted" << endl;
//	return salary * 0.15;
//}
//int CountTax(int salary)
//{
//	try {
//		if (salary < 0)
//			throw string("zero salary");
//		cout << "counting tax" << endl;
//	}
//	catch (string s)
//	{
//		cout << "CountTax error :"<< s.c_str() << endl;
//		throw; //继续抛出捕获的异常
//	}
//	cout << "tax counted" << endl;
//	return salary * 0.15;
//}
class Base
{
	virtual void func() {}
};
class Derived : public Base
{
public:
	void Print() {}
};
void PrintObj(Base & b)
{
	try
	{
		Derived & rd = dynamic_cast <Derived &>(b);
		//此转换若不安全，会拋出 bad_cast 异常
		rd.Print();
	}
	catch (bad_cast &e)	//exception的派生类
	{
		cerr << e.what() << endl;
	}
}
class NLComponent
{
public:
	virtual ostream& print(ostream& s)const = 0;
};
class TextBlock : public NLComponent
{
public:
	virtual ostream& print(ostream& s) const
	{
		s << "TextBlock";
		return s;
	}
};
class Graphic : public NLComponent
{
	virtual ostream& print(ostream& s) const
	{
		s << "Graphic";
		return s;
	}
};
inline ostream& operator<<(ostream &s, const NLComponent &c)
{
	return c.print(s);
}
int main()
{
	//double m, n;
	//cin >> m >> n;
	//try 
	//{
	//	cout << "before dividing." << endl;
	//	if (n == 0)
	//		throw - 1;
	//	else if (m == 0)
	//		throw - 1.0;
	//	else
	//		cout << m / n << endl;
	//	cout << "after dividing." << endl;
	//}
	//catch (double d)	//捕获浮点异常
	//{
	//	cout << "catch(double) " << d << endl;
	//}
	//catch (char e)	//捕获字符异常
	//{
	//	cout << "catch(int) " << e << endl;
	//}
	//catch (...)	//捕获所有异常
	//{
	//	cout << "catch(...)" << endl;
	//}
	//cout << "finished" << endl;
	//double f = 1.2;
	////CountTax(-1);
	//try
	//{
	//	CountTax(-1);
	//	f = Devide(3, 0);
	//	cout << "end of try block" << endl;
	//}
	//catch(CException e)
	//{
	//	cout << e.msg.c_str() << endl;
	//}
	//cout << "f=" << f << endl;
	//cout << "finished" << endl;
	/*double f = 1.2;
	try 
	{
		CountTax(-1);
		cout << "end of try block" << endl;
	}
	catch (string s)
	{
		cout << s << endl;
	}
	cout << "finished" << endl;*/
	/*{
		Base b;
		PrintObj(b);
	}*/
	/*{
		try {
			char * p = new char[0x7fffffff];
		}
		catch (bad_alloc & e)
		{
			cerr << e.what() << endl;
		}
	}*/
	/*vector<int> v(10);
	try 
	{
		v.at(100) = 100;
	}
	catch (out_of_range &e)
	{
		cerr << e.what() << endl;
	}
	string s = "hello";
	try
	{
		char c = s.at(100);
	}
	catch (out_of_range &e)
	{
		cerr << e.what() << endl;
	}*/
	TextBlock tx;
	Graphic gc;
	cout << tx << endl;
	cout << gc << endl;







	getchar();
	return 0;
}

