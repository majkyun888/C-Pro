#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace std;

/*
// 文件：V99.h
 inline namespace V99 {
		 void f(int);     // 对V98版本进行改进
		 void f(double);  // 新特性
		 // …
 }
 // 文件：V98.h
 namespace V98 {
	 void f(int);        // V98版本只实现基本功能
	 // …
 }
 // 文件：Mine.h
 namespace Mine {
 #include “V99.h”
 #include “V98.h”
 }

 #include “Mine.h”
using namespace Mine;
// …
V98::f(1);        // 早期版本
V99::f(1);        // 较新版本
f(1);            // 默认版本


这里的f(1)函数调用相当于显式调用Mine::V99::f(1)，使用inline关键字定义的内联名字空间成为默认名字空间。 
（就像内联函数一样，内联的名字空间被嵌入到它的外围名字空间，成为外围名字空间的一部分。 ）
*/