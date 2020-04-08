
struct B {
	void f(double)
	{
		cout << "I am B f" << endl;
	}
};

struct D : B
{
	using B::f;
	void f(int)
	{
		cout << "I am D f" << endl;
	}
	void f(double)
	{
		cout << "I am D f double" << endl;
	}
};

struct B1 {
	B1(int) {}
};

struct D1 : B1
{
	using B1::B1;
	int x;
};