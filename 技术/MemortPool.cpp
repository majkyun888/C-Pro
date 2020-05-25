#include "pch.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include "StackAlloc.h"
#include "MemoryPool.h"
#define ELEMS 100000
#define REPS 100
using namespace std;
//void print()
//{
//	cout << "empty" << endl;
//}
//template<class T,class ...Args>
//void print(T head,Args ...rest)
//{
//	cout << "parameter " << head << endl;
//	print(rest...);
//}
//template<class ...T>
//void f(T ...args)
//{
//	cout << sizeof...(args) << endl;
//}
//template<typename T>
//void printarg(T t)
//{
//	cout << t << endl;
//}
//template<typename ...Args>
//void expand(Args... args)
//{
//	int arr[] = { (printarg(args),0)... };
//}
//template<typename F,typename ...Args>
//void expand(const F& f,Args&&...args)
//{
//	initializer_list<int>{(f(std::forward< Args>(args)), 0)...};
//}
//template <typename... Args>
//struct Sum;
//template<typename First,typename... Rest>
//struct Sum<First, Rest...>
//{
//	enum {value = Sum<First>::value + Sum<Rest...>::value};
//};
//template<typename Last>
//struct Sum<Last>
//{
//	enum {value = sizeof(Last) };
//};
//template<int...>
//struct IndexSeq {};
//
//template<int N,int... Indexes>
//struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};
//
//template<int... Indexes>
//struct MakeIndexes<0, Indexes...>
//{
//	typedef IndexSeq<Indexes...> type;
//};


int main()
{
	//print(1, 2, 3, 4);
	//expand(1, 2, 3, 4);
	//expand([](int i) {cout << i << endl; },1,2,3);
	//cout<<Sum<int, double, short>::value;
	std::vector<int> stackVector;
	clock_t start = clock();
	for (int j = 0; j < REPS; j++) {
		assert(stackVector.empty());
		for (int i = 0; i < ELEMS; i++)
			stackVector.push_back(i);
		for (int i = 0; i < ELEMS; i++)
			stackVector.pop_back();
	}
	std::cout << "Vector Time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
	
	StackAlloc<int, MemoryPool<int> > stackPool;
	start = clock();
	for (int j = 0; j < REPS; j++) {
		assert(stackPool.empty());
		for (int i = 0; i < ELEMS; i++)
			stackPool.push(i);
		for (int i = 0; i < ELEMS; i++)
			stackPool.pop();
	}
	std::cout << "MemoryPool Allocator Time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
	



	getchar();
	return 0;
}

