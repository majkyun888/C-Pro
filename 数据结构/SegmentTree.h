#pragma once
#include <iostream>
using namespace std;


template <typename E>
class segmentTree
{
private:
	E* data;
	E* tree;
	int leftChild(int index) { return 2 * index + 1; }
	int rightChild(int index) { return 2 * index + 2; }
	void buildSegmentTree(int treeIndex, int l, int r)
	{
		if (l == r) {
			tree[treeIndex] = data[l];
			return;
		}

		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);

		int mid = l + (r - l) / 2;
		buildSegmentTree(leftTreeIndex, l, mid);
		buildSegmentTree(rightTreeIndex, mid + 1, r);
		
		tree[treeIndex] = tree[leftTreeIndex] + tree[rightTreeIndex];
	}

	E query(int treeIndex, int l, int r, int queryL, int queryR)
	{
		if (l == queryL && r == queryR) return tree[treeIndex];

		int mid = l + (r - l) / 2;

		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);

		if (queryL >= mid + 1)
			return query(rightTreeIndex, mid + 1, r, queryL, queryR);
		else if (queryR <= mid)
			return query(leftTreeIndex, l, mid, queryL, queryR);

		E leftResult = query(leftTreeIndex, l, mid, queryL, mid);

		E rightResult = query(rightTreeIndex, mid + 1, r, mid + 1, queryR);

		return leftResult + rightResult;
	}

public:
	segmentTree(E arr[], int size)
	{
		data = new E[size * sizeof(E)];
		for (int i = 0; i < size; i++)
			data[i] = arr[i];
		tree = new E[size * 4 * sizeof(E)];
		buildSegmentTree(0, 0, size - 1);
	}

	~segmentTree()
	{
		delete [] data;
		delete[] tree;
		data = NULL;
		tree = NULL;
	}

	E get(int index) const
	{
		return data[index];
	}
	
	void getter(int index)
	{
		cout << tree[index] << endl;
	}

	E query(int queryL, int queryR) { return query(0, 0, 3, queryL, queryR); }
};