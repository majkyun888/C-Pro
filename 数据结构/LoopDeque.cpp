#pragma once
#include <iostream>
#include <string>
using namespace std;

template<class T>
class Deque
{
private:
	int Capacity;
	int front;
	int tail;
	int size;
	T * que;
public:
	Deque(int capacity): Capacity(capacity + 1), que(new T[Capacity + 1])
	{
		front = 0;
		tail = 0;
		size = 0;
		que = new T[Capacity];
	}

	Deque(): Deque(10)
	{}

	void endeque(T item)
	{
		if ((tail + 1) % Capacity == front)
		{
			reset(this->getCapacity() * 2);
		}
		que[tail] = item;
		tail = (tail + 1) % Capacity;
		size++;
	}

	T popdeque()
	{
		T temp;
		if (front != tail)
		{
			temp = que[front];
			front = (front + 1) % Capacity;
			size--;
		}
		
		if (size == (Capacity / 4) && (Capacity / 4) != 0)
			reset(Capacity / 2);
		return temp;
	}

	void PrintDeque()
	{
		for (int i = front;i != tail; i = (i + 1) % Capacity)
		{
			cout << que[i] << endl;
			
		}
	}
	bool isEmpty() { return front == tail; }

	int getCapacity() { return Capacity - 1; }

	int getSize() { return size; }
private:
	void reset(int newLength)
	{
		T * newQue = new T[newLength];
		T * temp = que;
		for (int i = 0; front != tail; i++)
		{
			newQue[i] = que[front];
			front = (front + 1) % Capacity;
		}
		que = newQue;
		delete temp;
		front = 0;
		tail = size;
		Capacity = newLength;
	}
};

