/*
* create_mask.cpp
*
* Author:
* Siddharth Kherada <siddharthkherada27[at]gmail[dot]com>
*
* This tutorial demonstrates how to make mask image (black and white).
* The program takes as input a source image and ouputs its corresponding
* mask image.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
using namespace std;
using namespace cv;

//Mat img0/*, img1*/;
//int drag = 0;
void mouseHandler(int, int, int, int, void* pcvim);
cv::Rect roiSel;

void mouseHandler(int event, int x, int y, int, void* pcvim)
{
	static int drag = 0;
	cv::Mat img0 = *(cv::Mat*)pcvim;
	static cv::Mat img1;
	if (event == EVENT_LBUTTONDOWN && !drag)
	{
		img1 = img0.clone();
		drag = 1;
		roiSel = cv::Rect(Point(x, y), Point(x, y));
		imshow("Source", img1);
	}
	if (event == EVENT_LBUTTONUP && drag)
	{
		imshow("Source", img1);
		drag = 0;
	}
	if (event == EVENT_MOUSEMOVE && drag) {
		img1 = img0.clone();
		roiSel = cv::Rect(roiSel.tl(), cv::Point(x, y));
		cv::rectangle(img1, roiSel, cv::Scalar(0, 255, 0), 2);
		imshow("Source", img1);
	}
}

int main(int argc, char** argv)
{
	cv::Mat imm;
	Mat src = imread("img_0000.png");
	if (src.channels() == 1)
		cv::cvtColor(src, imm, CV_GRAY2BGR);
	else
		imm = src.clone();
	namedWindow("Source", 1);
	setMouseCallback("Source", mouseHandler, &imm/*NULL*/);
	imshow("Source", imm);
	waitKey(0);
	imm.release();
	/* img1.release();*/
}
