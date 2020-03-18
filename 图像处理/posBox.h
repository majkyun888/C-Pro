#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
using namespace cv;

typedef struct _targetBox
{
	int label;
	int area;
	Rect boundryBox;
	bool operator>= (const _targetBox& res)
	{
		return area >= res.area;
	}

	bool operator< (const _targetBox& res)
	{
		return area < res.area;
	}
} targetBox;

vector<pair<int, int>> bias = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };	//4¡⁄”Ú
vector<pair<int, int>> bias8 = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };//8¡⁄”Ú

int findBoundaryBox(Mat& src, Mat& dst, vector<targetBox>& boxList)
{
	boxList.clear();
	dst.release();
	dst = src.clone();
	

	int labelValue = 0;
	Point cur, next;
	int rows = src.rows;
	int cols = src.cols;
	int area = 0;
	int leftBox = 0, rightBox = 0, topBox = 0, bottomBox = 0;
	stack<Point> pointStk;

	Rect box;
	targetBox target;
	for (int i = 0; i < rows; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			if (p[j] == 255)
			{
				area = 0;
				labelValue++;
				cur = Point(j, i);
				dst.at<uchar>(cur) = labelValue;
				pointStk.push(cur);
				area++;
				leftBox = cur.x;
				rightBox = cur.x;
				topBox = cur.y;
				bottomBox = cur.y;

				while (!pointStk.empty())
				{
					next = Point(cur.x + bias[0].first, cur.y + bias[0].second);
					if ((cur.x != (cols - 1)) && (dst.at<uchar>(next) == 255))
					{
						dst.at<uchar>(next) = labelValue;
						pointStk.emplace(next);
						area++;
						if (rightBox < next.x)
							rightBox = next.x;
					}

					next = Point(cur.x + bias[1].first, cur.y + bias[1].second);
					if ((cur.x != 0) && (dst.at<uchar>(next) == 255))
					{
						dst.at<uchar>(next) = labelValue;
						pointStk.emplace(next);
						area++;
						if (leftBox > next.x)
							leftBox = next.x;
					}

					next = Point(cur.x + bias[2].first, cur.y + bias[2].second);
					if ((cur.y != (rows - 1)) && (dst.at<uchar>(next) == 255))
					{
						dst.at<uchar>(next) = labelValue;
						pointStk.emplace(next);
						area++;
						if (bottomBox < next.y)
							bottomBox = next.y;
					}

					next = Point(cur.x + bias[3].first, cur.y + bias[3].second);
					if ((cur.y != 0) && (dst.at<uchar>(next) == 255))
					{
						dst.at<uchar>(next) = labelValue;
						pointStk.emplace(next);
						area++;
						if (topBox > next.y)
							topBox = next.y;
					}
					cur = pointStk.top();
					pointStk.pop();
				}
				box = Rect(leftBox, topBox, rightBox - leftBox, bottomBox - topBox);
				target.area = area;
				target.boundryBox = box;
				target.label = labelValue;
				boxList.push_back(target);
			}
		}
	}
	return labelValue;
}