#include "pch.h"
#include <iostream>

using namespace std;


template<class T>
class RCPtr
{
public:
	RCPtr(T * realPtr = 0):pointee(realPtr)
	{
		init();
	}
	RCPtr(const RCPtr& rhs):pointee(rhs.pointee)
	{
		init();
	}
	~RCPtr()
	{
		if (pointee)
			pointee->removeReference();
	}
	RCPtr& operator= (const RCPtr& rhs) 
	{
		if (pointee != rhs.pointee)
		{
			if (pointee)
				pointee->removeReference();
			pointee = rhs.pointee;
			init();
		}
		return *this;
	}
	T * operator->() const
	{
		return pointee;
	}
	T& operator*() const
	{
		return *pointee;
	}
private:
	T * pointee;
	void init()
	{
		if (pointee == 0) return;
		if (pointee->isShareable() == false)
		{
			pointee = new T(*pointee);
		}
		pointee->addReference();
	}
};

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

class My_String
{
public:
	My_String(const char * data): value(new StrValue(data))
	{}
	const char& operator[] (int index) const
	{
		return value->sdata[index];
	}
	char& operator[] (int index)
	{
		if (value->isShared())
		{
			value = new StrValue(value->sdata);
		}
		value->markUnshareable();
		return value->sdata[index];
	}
	~My_String()
	{
	/*	if (--value->refValue == 0)
			delete value;
		value = NULL;*/
	}
private:
	struct StrValue:public RCObject
	{
		char * sdata;
		void init(const char * initValue)
		{
			sdata = new char[strlen(initValue) + 1];
			strcpy(sdata, initValue);
		}
		StrValue(const char * InitValue) 
		{
			if (InitValue != NULL)
			{
				init(InitValue);
			}
		}
		StrValue(const StrValue& rhs)
		{
			init(rhs.sdata);
		}
		~StrValue()
		{
			delete[] sdata;
		}
	};
	RCPtr<StrValue> value;
};

int main()
{
	My_String a("abcd");
	
	system("pause");
	return 0;
}

