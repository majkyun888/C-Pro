#pragma once
#include <iostream>
using namespace std;


template <typename E>
class LinkedListQueue
{
public:
	LinkedListQueue(): size(0), Head(NULL), Tail(NULL) {}

	int getSize() { return size; }

	bool isEmpty() { return size == 0; }

	void enqueue(E e)
	{
		if (NULL == Tail)
		{
			Tail = new Node(e);
			Head = Tail;
		}
		else
		{
			Tail->next = new Node(e);
			Tail = Tail->next;
		}
		size++;
	}

	E deque()
	{
		if (isEmpty())
			throw - 99;
		Node* pDel = Head;
		Head = Head->next;
		if (Head == NULL)
			Tail = Head;
		size--;
		E temp = pDel->e;
		delete pDel;
		pDel = NULL;
		return temp;
	}

	E getFront()
	{
		if (isEmpty())
			throw - 99;
		return Head->e;
	}

	void toPrint()
	{
		Node* cur = Head;
		while (cur != NULL)
		{
			cout << cur->e << "-->";
			cur = cur->next;
		}
		cout << "NULL" << endl;
	}
private:
	struct Node
	{
		E e;
		Node* next;
		Node(E e, Node* next)
		{
			this->e = e;
			this->next = next;
		}
		Node(E e) : Node(e, NULL) {}
		Node() : Node(NULL, NULL) {}
	};
private:
	Node* Head, * Tail;
	int size;
};