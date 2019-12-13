#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
void selectionSort(T arr[], int n) {
	int minj;

	for (int i = 0; i < n; i++) {
		minj = i;
		for (int j = i + 1; j < n; j++)
			if (arr[j] < arr[minj])
				minj = j;
		swap(arr[i], arr[minj]);
	}
}

template <typename T>
void insertSort(T arr[], int n)
{
	for (int i = 1; i < n; i++)
		for (int j = i; j > 0 && arr[j] < arr[j - 1]; j--)
			swap(arr[j], arr[j - 1]);
}

template <typename T>
void insertSort2(T arr[], int n)
{
	for (int i = 1; i < n; i++) {
		T temp = arr[i];
		int j;
		for (j = i; j > 0 && arr[j - 1] > temp; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}

template <typename T>
void insertSort3(T arr[], int l, int r)
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
void bubbleSort(T arr[], int n)
{
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (arr[i] > arr[j])
				swap(arr[i], arr[j]);
}

template <typename T>
void bubbleSort2(T arr[], int n)
{
	int nnew ;

	do {
		nnew = 0;
		for (int i = 1; i < n; i++)
			if (arr[i - 1] > arr[i])
			{
				swap(arr[i - 1], arr[i]);
				nnew = i;
			}
		n = nnew;
	} while (nnew > 0);
}

template <typename T>
void _merge(T arr[], int l, int mid, int r)
{
	T* aux = new T[r - l + 1];

	for (int i = l; i <= r; i++)
	{
		aux[i - l] = arr[i];
	}

	int i = l;
	int j = mid + 1;
	for (int k = l; k <= r; k++)
	{
		if (j > r)
		{
			arr[k] = aux[i - l];
			i++;
		}
		else if (i > mid)
		{
			arr[k] = aux[j++ - l];
			//j++;
		}
		else if (aux[i - l] < aux[j - l])
		{
			arr[k] = aux[i - l];
			i++;
		}
		else
		{
			arr[k] = aux[j - l];
			j++;
		}
	}
	delete[] aux;
	aux = NULL;
}

template <typename T>
void _mergeSort(T arr[], int l, int r)
{
	if (r - l <= 15)
	{
		insertSort3(arr, l, r);
		return;
	}

	int mid = (l + r) / 2;
	_mergeSort(arr, l, mid);
	_mergeSort(arr, mid + 1, r);

	if (arr[mid] > arr[mid + 1])
		_merge(arr, l, mid, r);
}

template <typename T>
void mergerSort(T arr[], int n)
{
	_mergeSort(arr, 0, n - 1);
}

template <typename T>
void shellSort(T arr[], int n)
{
	int h = 1;
	while (h < n / 3)
		h = 3 * h + 1;

	while (h >= 1)
	{
		for (int i = h; i < n; i++)
		{
			T temp = arr[i];
			int j;
			for (j = i; j >= h && temp < arr[j - h]; j -= h)
				arr[j] = arr[j - h];
			arr[j] = temp;
		}

		h /= 3;
	}
}