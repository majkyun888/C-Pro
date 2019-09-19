#include<iostream>
using namespace std;
//class Singleton	//单例模式实现方法1
//{
//public:
//	static Singleton * GetInstance()
//	{
//		if (mInstance == NULL)
//			mInstance = new Singleton;
//		return mInstance;
//	}
//	~Singleton() { cout << "destory Object" << endl; }
//private:
//	Singleton() { cout << "Object struce" << endl; }
//	Singleton(const Singleton &) = delete;
//	Singleton& operator=(const Singleton &) = delete;
//private:
//	static Singleton * mInstance;
//};
//Singleton * Singleton::mInstance = NULL;//需要手动进行内存回收


//class Singleton //单例模式实现方法2
//{
//private:
//	Singleton() { cout << "Object Struct" << endl; }
//	~Singleton() { cout << "Destory Object" << endl; }
//	Singleton(const Singleton &) = delete;
//	Singleton & operator=(const Singleton &) = delete;
//public:
//	static Singleton & GetInstance()
//	{
//		static Singleton oj;
//		return oj;
//	}
//};

//class Singleton	//单例模式实现方法3
//{
//private:
//	Singleton() { cout << "Construct Object" << endl; }
//	Singleton(const Singleton &) = delete;
//	Singleton & operator= (const Singleton &) = delete;
//	static Singleton m_Instance;
//public:
//	static Singleton &GetInstance()
//	{
//		return m_Instance;
//	}
//
//};
//Singleton Singleton::m_Instance;




//class Singleton //单例模式实现方法4
//{
//private:
//	Singleton() { cout << "Object Construct" << endl; }
//	~Singleton() { cout << "Destory Object" << endl; }
//	static Singleton * const m_Instance;
//public:
//	static Singleton *  GetInstance()
//	{
//		return m_Instance;
//	}
//	static void DestoryObject()
//	{
//		if (m_Instance != NULL)
//		{
//			delete m_Instance;
//			//const_cast<Singleton *>(m_Instance) = NULL;
//		}
//	}
//};
//Singleton * const Singleton::m_Instance = new Singleton;对象直接创建好了,但是销毁还需要手动

class Singleton
{
public:
	static Singleton * getInstance()
	{
		return Instance;
	}
	static void Destory()
	{
		if (Instance)
		{
			delete Instance;
			Instance = NULL;
		}
	}
private:
	static Singleton * Instance;
	Singleton() { cout << "Create Object.\n" << endl; }
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton&) = delete;
	class GC
	{
	public:
		~GC()
		{
			Destory();
		}
	};
	static GC gc;
};
Singleton * Singleton::Instance = new Singleton;
Singleton::GC Singleton::gc;














//class Singleton
//{
//
//public:
//	static Singleton * GetInstance()
//	{
//		return m_Instance;
//	}
//	static void Destory()
//	{
//		if (m_Instance != NULL)
//		{
//			delete m_Instance;
//		}
//	}
//private:
//	static Singleton  * const m_Instance;
//	Singleton() { cout << "Object Construct" << endl; }
//	class GC
//	{
//		public:
//			~GC()
//			{
//				Destory();
//			}
//	};
//	Singleton(const Singleton &) = delete;
//	Singleton & operator= (const Singleton &) = delete;
//	static GC gc;
//};
//Singleton * const Singleton::m_Instance = new Singleton;
//Singleton::GC Singleton::gc;

int main()
{
	/*{
		Singleton * aa = Singleton::GetInstance();
		delete aa;
	}*/
	/*{
		Singleton &aa = Singleton::GetInstance();
	}*/
	//{
	//	Singleton &fa = Singleton::GetInstance();
	//	
	//}
	/*{
		Singleton * pt = Singleton::GetInstance();
	}*/
	//{
	//	Singleton * pt = Singleton::GetInstance();
	//}
	
	system("pause");
	return 0;
}


