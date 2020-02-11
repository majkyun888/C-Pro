#pragma once

int binarySearch(int arr[], int target, int size)
{
	int l = 0;
	int r = size - 1;

	while (l <= r)
	{
		int mid = l + (r - l) / 2;

		if (arr[mid] == target)
			return mid;
		
		if (arr[mid] < target)
			l = mid + 1;
		else
			r = mid - 1;
	}

	return -1;
}

int binarySearch2(int arr[], int target, int low, int heigh)
{
	int mid = low + (heigh - low) / 2;
	if (arr[mid] == target)
		return mid;

	return (arr[mid] < target) ? binarySearch2(arr, target, mid + 1, heigh) : binarySearch2(arr, target, low, mid - 1);
}