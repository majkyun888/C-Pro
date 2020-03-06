#pragma once
#include <iostream>
#include <algorithm>

using namespace std;


template <typename T>
void insertSort4(T arr[], int l, int r)
{
	for (int i = l + 1; i <= r; i++)
	{
		int temp = arr[i];
		int j;
		for (j = i; j > l&& arr[j - 1] > temp; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}


template <typename T>
int partition(T arr[], int l, int r)
{
	swap(arr[l], arr[l + rand() % (r - l + 1)]);
	T temp = arr[l];

	int j = l;
	for (int i = l + 1; i <= r; i++)
	{
		if (arr[i] < temp)
			swap(arr[++j], arr[i]);
	}

	swap(arr[l], arr[j]);

	return j;
}

template <typename T>
void _quickSort(T arr[], int l, int r)
{
	if (r - l <= 15)
	{
		insertSort4(arr, l, r);
		return;
	}

	int p = partition(arr, l, r);
	_quickSort(arr, l, p - 1);
	_quickSort(arr, p + 1, r);
}

template <typename T>
void quickSort(T arr[], int n)
{
	srand(time(NULL));
	_quickSort(arr, 0, n - 1);
}

template <typename T>
int _partition2(T arr[], int l, int r)
{
	swap(arr[l], arr[l + (rand() % (r - l + 1))]);
	T temp = arr[l];

	int i = l + 1;
	int j = r;

	while (true)
	{
		while (i <= r && arr[i] < temp) i++;
		while (j > l + 1 && arr[j] > temp) j--;
		if (i > j) break;
		swap(arr[i++], arr[j--]);
	}

	swap(arr[l], arr[j]);

	return j;
}

template <typename T>
void _quickSort2(T arr[], int l, int r)
{
	if (r - l <= 15)
	{
		insertSort4(arr, l, r);
		return;
	}

	int p = _partition2(arr, l, r);
	_quickSort2(arr, l, p - 1);
	_quickSort2(arr, p + 1, r);
}

template <typename T>
void quickSort2(T arr[], int n)
{
	srand(time(NULL));
	_quickSort2(arr, 0, n - 1);
}


template <typename T>
void _quickSort3(T arr[], int l, int r)
{
	if ((r - l) <= 15)
	{
		insertSort4(arr, l, r);
		return;
	}

	swap(arr[l], arr[l + rand() % (r - l + 1)]);
	//cout << arr[l] << endl;
	T v = arr[l];

	int lt = l;
	int gt = r + 1;
	int i = l + 1;
	while (i < gt)
	{
		if (arr[i] < v)
		{
			/*swap(arr[i++], arr[++lt]);
			lt++;*/
			swap(arr[i], arr[lt + 1]);
			i++;
			lt++;
		}
		else if (arr[i] > v)
		{
			swap(arr[i], arr[gt - 1]);
			gt--;
			/*swap(arr[i], arr[gt - 1]);
			gt--;*/
		}
		else i++;
	}
	//cout << "lt: " << lt << " " << "gt: " << gt << endl;
	swap(arr[l], arr[lt]);

	_quickSort3(arr, l, lt - 1);
	_quickSort3(arr, gt, r);
}


template <typename T>
void quickSort3(T arr[], int n)
{
	srand(time(NULL));
	_quickSort3(arr, 0, n - 1);
}