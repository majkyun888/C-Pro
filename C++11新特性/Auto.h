#pragma once
using namespace std;

//表示函数不返回值,并只能应用在函数上面,如果函数真的有返回值，那么该属性无效, 并且会给出警告
[[noreturn]]  void f()
{
	throw "error";
}

struct foo{};


//表示消耗释放模式的依赖链在函数内外传播,允许编译器跳过一些不必要的栅栏指令
struct foo* ff [[carries_dependency]] (int i);//编译优化选项
int* g(int* x, int* y [[carries_dependency]] );