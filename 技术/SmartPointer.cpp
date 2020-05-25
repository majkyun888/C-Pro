#include <iostream>
#include<time.h>
using namespace std;
template<class T>
class SmartPtr
{
public:
	typedef T* pointer;
public:
	SmartPtr(pointer realPtr = 0):pointee(realPtr)
	{};
	SmartPtr(const  SmartPtr<T> & rhs) 
	{
		pointee = rhs.pointee;
		rhs.pointee = 0;
	};
	SmartPtr<T>& operator=(const SmartPtr<T> &rhs)
	{
		if (this == rhs)
			return *this;
		delete pointee;
		pointee = rhs.pointee;
		rhs.pointee = 0;
		return *this;
	};
	~SmartPtr() 
	{
		if (pointee != NULL)
			delete pointee;
	};
	
	template<class newType>
	inline	operator SmartPtr<newType>()
		{
			return SmartPtr<newType>(pointee);
		}
	T * operator-> () 
	{
		return pointee;
	};
	T & operator* () const
	{
		return *pointee;
	};
private:
	pointer pointee;
};

class MusicProduct
{
public:
	MusicProduct() {};
	MusicProduct(const string & title) {};
	virtual void play() const = 0;
	virtual void displayTitle() const = 0;
};

class Cassette : public MusicProduct
{
public:
	Cassette(const string & title) 
	{
		tittle = title;
	}
	void displayTitle() const
	{
		cout << tittle.c_str() << endl;
	}
	void play() const
	{
		cout << "lala" << endl;
	}
private:
	string tittle;
};
class CD : public MusicProduct
{
public:
	CD(const string & title)
	{
		tittle = title;
	}
	void displayTitle() const
	{
		cout << tittle.c_str() << endl;
	}
	void play() const
	{
		cout << "lala" << endl;
	}
private:
	string tittle;
};
void displayAndPlay(const SmartPtr<MusicProduct> &pmp,int numTimes)
{
	for (int i = 1; i <= numTimes; i++)
	{
		/*pmp->displayTitle();
		pmp->play();*/
	}
}

int main()
{
	/*SmartPtr<int> pt = new int(10);
	SmartPtr<A> dk = new A;
	dk->print();
	cout << *pt << endl;*/
	SmartPtr<Cassette> funMusic (new Cassette("Alapalooza"));
	SmartPtr<CD> nightmareMusic (new CD("Disco Hits of the 70s"));
	displayAndPlay(funMusic, 10);





	getchar();
	return 0;
}

