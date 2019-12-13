#pragma once
#include <iostream>
#include <algorithm>
using namespace std;

template <typename T>
void insertSort(T arr[], int l, int r)
{
	for (int i = l + 1; i <= r; i++)
	{
		T temp = arr[i];
		int j;
		for (j = i; j > l&& arr[j - 1] > temp; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}

template <typename T>
void merge(T arr[], int l, int mid, int r)
{
	T* aux = new T[r - l + 1];

	for (int i = l; i <= r; i++)
		aux[i - l] = arr[i];

	int i = l;
	int j = mid + 1;
	for (int k = l; k <= r; k++)
	{
		if (i > mid)
			arr[k] = aux[j++ - l];
		else if (j > r)
			arr[k] = aux[i++ - l];
		else if (aux[i - l] < aux[j - l])
			arr[k] = aux[i++ - l];
		else
			arr[k] = aux[j++ - l];
	}
	delete[] aux;
	aux = NULL;
}

template <typename T>
void merSortBU(T arr[], int n)
{

	for (int i = 0; i < n; i += 16)
		insertSort(arr, i, min(i + 15, n - 1));



	for (int sz = 16; sz < n; sz += sz)
		for (int i = 0; i + sz < n; i += sz + sz)
			if (arr[i + sz - 1] > arr[i + sz])
				merge(arr, i, i + sz - 1, min(i + sz + sz - 1, n - 1));

}

