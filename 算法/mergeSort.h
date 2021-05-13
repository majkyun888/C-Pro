#pragma once
#include <iostream>
using namespace std;


void merge_sort_recursive(int* arr, int* reg, int start, int end)
{
	if (start >= end)
		return;

	int start1 = start;
	int mid = start + ((end - start) >> 1);
	int end1 = mid;
	int start2 = end1 + 1;
	int end2 = end;

	// 分
	merge_sort_recursive(arr, reg, start1, end1);
	merge_sort_recursive(arr, reg, start2, end2);

	int k = start;
	//治
	while (start1 <= end1 && start2 <= end2)
		reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];
	while (start1 <= end1)
		reg[k++] = arr[start1++];
	while (start2 <= end2)
		reg[k++] = arr[start2++];

	for (int i = start; i <= end; i++)
		arr[i] = reg[i];
}

void merge_sort(int* arr, int len)
{
	int* reg = new int[len];
	merge_sort_recursive(arr, reg, 0, len - 1);
	return;
}

// 归并排序（C++-迭代版）
template<typename T>
void merge_sort(T arr[], int len) {
	T* a = arr;
	T* b = new T[len];
	for (int seg = 1; seg < len; seg += seg) {
		for (int start = 0; start < len; start += seg + seg) {
			int low = start, mid = min(start + seg, len), high = min(start + seg + seg, len);
			int k = low;
			int start1 = low, end1 = mid;
			int start2 = mid, end2 = high;
			while (start1 < end1 && start2 < end2)
				b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
			while (start1 < end1)
				b[k++] = a[start1++];
			while (start2 < end2)
				b[k++] = a[start2++];
		}
		T* temp = a;
		a = b;
		b = temp;
	}

	if (a != arr) {
		for (int i = 0; i < len; i++)
			b[i] = a[i];
		b = a;
	}

	delete[] b;
}