#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace std;

/*
// �ļ���V99.h
 inline namespace V99 {
		 void f(int);     // ��V98�汾���иĽ�
		 void f(double);  // ������
		 // ��
 }
 // �ļ���V98.h
 namespace V98 {
	 void f(int);        // V98�汾ֻʵ�ֻ�������
	 // ��
 }
 // �ļ���Mine.h
 namespace Mine {
 #include ��V99.h��
 #include ��V98.h��
 }

 #include ��Mine.h��
using namespace Mine;
// ��
V98::f(1);        // ���ڰ汾
V99::f(1);        // ���°汾
f(1);            // Ĭ�ϰ汾


�����f(1)���������൱����ʽ����Mine::V99::f(1)��ʹ��inline�ؼ��ֶ�����������ֿռ��ΪĬ�����ֿռ䡣 
��������������һ�������������ֿռ䱻Ƕ�뵽������Χ���ֿռ䣬��Ϊ��Χ���ֿռ��һ���֡� ��
*/