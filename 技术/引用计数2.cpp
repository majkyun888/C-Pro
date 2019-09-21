#include "pch.h"
#include <iostream>


class RCObject
{
public:
	void addReference() { ++refCount; };
	void removeReference() { if (--refCount == 0) delete this; };
	void markUnshareable() { shareable = false; };
	bool isShareable() const
	{
		return shareable;
	};
	bool isShared() const
	{
		return refCount > 1;
	};
protected:
	RCObject() :refCount(0), shareable(true)
	{}
	RCObject(const RCObject& rhs) :refCount(0), shareable(true)
	{}
	RCObject& operator= (const RCObject& rhs)
	{
		return *this;
	}
	virtual ~RCObject() {};
private:
	int refCount;
	bool shareable;
};

template<class T>
class RCIPtr
{
private:
	struct CountHolder :public RCObject
	{
		~CountHolder() { delete pointee; }
		T *pointee;
	};
	CountHolder *counter;
public:
	RCIPtr(T * realPtr = 0) :counter(new CounterHolder)
	{
		counter->pointee = realPtr;
		init();
	}
	RCIPtr(const RCIPtr& rhs):counter(rhs.counter)
	{
		init();
	}
	~RCIPtr()
	{
		counter->removeReference();
	}
	RCIPtr& operator= (const RCIPtr& rhs)
	{
		if (counter != rhs.counter)
		{
			counter->removeReference();
			counter = rhs.counter;
			init();
		}
		return *this;
	}
	const T* operator->() const
	{
		return counter->pointee;
	}
	T * operator->()
	{
		makeCopy();
		return counter->pointee;
	}
	const T& operator* () const
	{
		return *(counter->pointee);
	}
	T& operator* ()
	{
		makeCopy();
		return *(counter->pointee);
	}
private:
	void init()
	{
		if (counter->isShareable() == false)
		{
			T * oldValue = counter->pointee;
			counter = new CounterHolder;
			counter->pointee = new T(*oldValue);
		}
		counter->addReference();
	}
	void makeCopy()
	{
		if (counter->isShared())
		{
			T * oldValue = counter->pointee;
			counter->removeReference();
			counter = new CounterHolder;
			counter->pointee = new T(*oldValue);
			cpunter->addReference();
		}
	}
};

class Widget
{
public:
	Widget(int size);
	Widget(const Widget& rhs);
	~Widget();
	Widget& operator= (const Widget& rhs);
	void doThis();
	int showThat() const;
};

class RCWidget
{
public:
	RCWidget(int size):value(new Widget(size)){}
	void doThis() { value->doThis(); }
	int showThat() const
	{
		return value->showThat();
	}
private:
	RCIPtr<Widget> value;
};





int main()
{






	getchar();
	return 0;
}
