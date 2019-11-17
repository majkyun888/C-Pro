#pragma once
#include <string>
#include <iostream>
using namespace std;
template <typename E>
class LinkedList
{

public:
	LinkedList() : dummyHead(new Node), size(0) {}
	~LinkedList()
	{
		while (NULL != dummyHead)
		{
			Node* pDel = dummyHead;
			dummyHead = dummyHead->next;
			delete pDel;
			pDel = NULL;
		}
	}

	int getSize() const { return size; }
	bool isEmpty() const { return size == 0; }
	void addFirst(E e){ add(0, e); }

	void add(int index, E e)
	{
		if (index < 0 || index > size)
			throw - 1;
		Node* prev = dummyHead;
		for (int i = 0; i < index; i++)
			prev = prev->next;
		prev->next = new Node(e, prev->next);
		size++;
	}

	void addLast(E e) { add(size, e); }

	E get(int index)
	{
		if (index < 0 || index >= size)
			throw - 1;
		Node* current = dummyHead->next;
		for (int i = 0; i < index; i++)
			current = current->next;
		return current->e;
	}

	E getFirst() { return get(0); }

	E getLast() { return get(size - 1); }

	void set(int index, E e)
	{
		if (index < 0 || index >= size)
			throw - 1;
		Node* cur = dummyHead->next;
		for (int i = 0; i < index; i++)
			cur = cur->next;
		cur->e = e;
	}

	bool contains(E e)
	{
		Node* cur = dummyHead->next;
		while (cur != NULL)
		{
			if (cur->e == e)
				return true;
			cur = cur->next;
		}
		return false;
	}

	void remove(int index)
	{
		if (index < 0 || index >= size)
			throw - 1;

		Node* prev = dummyHead;
		for (int i = 0; i < index; i++)
			prev = prev->next;

		Node * retNode = prev->next;
		prev->next = retNode->next;
		retNode->next = NULL;
		delete retNode;
		retNode = NULL;
		size--;
	}

	void removeFirst() { remove(0); }

	void removeLast() { remove(size - 1); }

	E removeElement(E e)
	{
		Node* prev = dummyHead;
		while (prev->next != NULL)
		{
			if (prev - next->e == e)
				break;
			prev = prev->next;
		}

		if (prev->next != NULL)
		{
			Node* pDel = prev->next;
			prev->next = pDel->next;
			pDel->next = NULL;
			E temp = pDel->e;
			delete pDel;
			pDel = NULL;
			size--;
			return temp;
		}
	}

	void Print() const
	{
		Node* cur = dummyHead->next;
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
		Node(E e): Node(e, NULL) {}
		Node(): Node(NULL, NULL) {}
	};
private:
	Node* dummyHead;
	int size;
};


template <typename E>
class LinkedListStack
{
private:
	LinkedList<E> list;
public:
	LinkedListStack(){}

	int getSize() { return list.getSize; }
	bool isEmpty() { return list.isEmpty(); }
	void push(E e) { list.addFirst(e); }
	E pop() { return list.removeFirst(); }
	E peek() { return list.getFirst(); }
	void Print() { list.Print(); }
};