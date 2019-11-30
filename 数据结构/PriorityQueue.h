#pragma once
#include <vector>
using namespace std;

template <typename E>
class MaxHeap
{
private:
	vector<E> HeapArray;

	int parent(int index)
	{
		if (0 == index)
			throw - 1;
		return (index - 1) / 2;
	}

	void swap(int i, int j)
	{
		E temp = HeapArray.at(i);
		HeapArray.at(i) = HeapArray.at(j);
		HeapArray.at(j) = temp;
	}

	int leftChild(int index) { return (index * 2 + 1); }

	int rightChild(int index) { return (index * 2 + 2); }

	void siftUp(int k) 
	{
		while (k > 0 && HeapArray.at(k) > HeapArray.at(parent(k)))
		{
			swap(k, parent(k));
			k = parent(k);
		}
	}

	void siftDown(int k)
	{
		while (leftChild(k) <= HeapArray.size() - 1 && HeapArray.size() > 1)
		{
			int j = leftChild(k);
			//cout << j << endl;
			if (j + 1 <= (HeapArray.size() - 1) && HeapArray.at(j) < HeapArray.at(j + 1))
				j++;
			/*if (((j + 1 )< (HeapArray.size() - 1)) && HeapArray.at(j) < HeapArray(j + 1))
				j++;*/

			if (HeapArray.at(k) > HeapArray.at(j))
				break;

			swap(k, j);
			k = j;
		}
	}
public:
	MaxHeap(){}
	MaxHeap(E* arr, int nLength) {
		for (int i = 0; i < nLength; i++)
		{
			HeapArray.push_back(arr[i]);
		}
			
		for (int i = parent(nLength - 1); i >= 0; i--)
			siftDown(i);

	}
	int size() { return HeapArray.size(); }

	bool isEmpty() { return HeapArray.empty(); }

	void add(E e) {
		HeapArray.push_back(e);
		siftUp(size() - 1);
	}

	E findMax() {
		if (0 == size())
			throw - 99;
		return HeapArray.at(0);
	}

	E extractMax() {
		E ret = findMax();

		if (HeapArray.size() > 1)
		{
			swap(0, size() - 1);
			HeapArray.pop_back();
			siftDown(0);
		}
		else 
		{
			HeapArray.pop_back();
		}
		
		return ret;
	}

	E replace(E e) {
		E ret = findMax();
		HeapArray.at(0) = e;
		siftDown(0);

		return ret;
	}
};

template <typename E>
class QueueInterface
{
public:
	virtual int getSize() = 0;
	virtual bool isEmpty() = 0;
	virtual E getFront() = 0;
	virtual void enqueue(E e) = 0;
	virtual E deque() = 0;
};


template <typename E>
class Queue : public QueueInterface<E>
{
private:
	MaxHeap<E> maxHeap;
public:
	int getSize() { return maxHeap.size(); }

	bool isEmpty() { return maxHeap.isEmpty(); }

	E getFront() { return maxHeap.findMax(); }

	void enqueue(E e) { maxHeap.add(e); }

	E deque() { return maxHeap.extractMax(); }
};