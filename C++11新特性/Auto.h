#pragma once
using namespace std;

//��ʾ����������ֵ,��ֻ��Ӧ���ں�������,�����������з���ֵ����ô��������Ч, ���һ��������
[[noreturn]]  void f()
{
	throw "error";
}

struct foo{};


//��ʾ�����ͷ�ģʽ���������ں������⴫��,�������������һЩ����Ҫ��դ��ָ��
struct foo* ff [[carries_dependency]] (int i);//�����Ż�ѡ��
int* g(int* x, int* y [[carries_dependency]] );