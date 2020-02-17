#pragma once
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
void _shiftDown(T arr[], int n, int k)
{
	while (2 * k + 1 < n)
	{
		int j = 2 * k + 1;
		if (j + 1 < n && arr[j] < arr[j + 1])
			j++;
		if (arr[k] >= arr[j]) break;
		swap(arr[k], arr[j]);
		k = j;
	}
}

template <typename T>
void heapsort(T arr[], int n)
{
	for (int i = (n - -1 - 1) / 2; i >= 0; i--)
	{
		_shiftDown(arr, n, i);
	}
	for (int i = n - 1; i > 0; i--)
	{
		swap(arr[0], arr[i]);
		_shiftDown(arr, i, 0);
	}
}