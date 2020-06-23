#pragma once
#include "Sys.h"
int width;
int height;

stack<int> bottomPoint;	//瓶底点
int findX(Mat& I, BottomPoint& point)
{
	CV_Assert(I.depth() == CV_32F);
	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols * channels;
	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	float* p = I.ptr<float>(0);
	for (int j = nCols - OFFSET_LEVEL; j > 0; j--)
	{
		if ((int)p[j] >= Liquid_Point)
		{
			for (int i = j; i > 0; i--)
				if ((int)p[i] == 0 && abs(i - j) > Level_distance)
				{
					point.centerThickness = abs(i - j);
					return i;
				}
				else
					continue;
		}
	}
	return 0;
}

int findY(Mat& I)
{
	CV_Assert(I.depth() == CV_32F);

	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols * channels;
	if (I.isContinuous())
	{
		nRows *= nCols;
		nCols = 1;
	}
	int i, j;
	int maxDot = 0;
	float* p;
	int y = 0;
	for (int i = 0; i < nRows; i++)
	{
		p = I.ptr<float>(i);
		if ((int)p[0] >= maxDot)
		{
			maxDot = (int)p[0];
			y = i;
		}
	}
	return y;
}

int findBottom(Mat& I)
{
	CV_Assert(I.depth() == CV_32F);
	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols * channels;
	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	/*imshow("Image sequence", I);
	waitKey(0);*/
	float* p = I.ptr<float>(0);
	for (int j = nCols; j > 0; j--)
	{
		if ((int)p[j] >= BOTTOM_POINT)
		{
			/*if (bottomPoint.empty())
			{
				bottomPoint.push(j);
				return j;
			}
			else if (abs(j - bottomPoint.top()) > BOTTOM_THICKNESS)
			{
				j = bottomPoint.top();
				bottomPoint.push(j);
				return j;
			}*/
			return j;
		}
	}
	return 0;
}

int findBottomUpDown(Mat& I, BottomPoint& point, vector<Point>& BottomSidePoint)
{
	CV_Assert(I.depth() == CV_8UC1);
	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols * channels;
	for (int j = 1; j < SAMPLING_POINT_NUM; j += 3)
	{
		for (int i = 0; i < IMAGE_HEIGHT; i++)
		{
			uchar* p = I.ptr<uchar>(i);
			if (p[point.centerPoint.x + j] >= BRIGHTNESS_SIDE)
			{
				if (i >= (point.centerPoint.y / 2) + SUPPORT_UPPER_BOUND && i <= (point.centerPoint.y / 2)  + SUPPORT_LOWER_BOUND && (point.centerPoint.x + j) <= IMAGE_WIDTH)
					BottomSidePoint.push_back(Point(point.centerPoint.x + j, i));
				break;
			}
		}
	}

	if (BottomSidePoint.size() == 0)
		return ((point.centerPoint.y / 2) + SUPPORT_UPPER_BOUND);
	Vec4f line_para;
	fitLine(BottomSidePoint, line_para, CV_DIST_L2, 0, FITTING_ACCURACY, FITTING_ACCURACY);		//拟合直线
	Point point0;
	point0.x = line_para[2];
	point0.y = line_para[3];
	return point0.y;
}

int findBottomDownUp(Mat& I, BottomPoint& point)
{
	CV_Assert(I.depth() == CV_8UC1);
	int channels = I.channels();
	int nRows = I.rows;
	int nCols = I.cols * channels;
	Point uf;
	for (int j = 1; j < SAMPLING_POINT_NUM; j += 2)
	{
		for (int i = IMAGE_HEIGHT - 1; i > 0; i--)
		{
			uchar* p = I.ptr<uchar>(i);
			if (p[point.centerPoint.x + j] >= BRIGHTNESS_SIDE)
			{
				if (i >= (point.centerPoint.y + (point.centerPoint.y) / 2) - SUPPORT_LOWER_BOUND && i <= (point.centerPoint.y + (point.centerPoint.y) / 2))
					BottomSidePoint2.push_back(Point(point.centerPoint.x + j, i));
				break;
			}
			else if (p[point.centerPoint.x - j] >= BRIGHTNESS_SIDE)
			{
				if (i >= (point.centerPoint.y + (point.centerPoint.y) / 2) - SUPPORT_LOWER_BOUND && i <= (point.centerPoint.y + (point.centerPoint.y) / 2) && i >= 380)
					BottomSidePoint2.push_back(Point(point.centerPoint.x - j, i));
				break;
			}
		}
	}

	if (BottomSidePoint2.size() == 0)
		return 0;
	Vec4f line_para;
	fitLine(BottomSidePoint2, line_para, CV_DIST_L2, 0, FITTING_ACCURACY, FITTING_ACCURACY);		//最小二乘拟合直线
	Point point0;
	point0.x = line_para[2];
	point0.y = line_para[3];
	return point0.y;
}

Point findCenter_P(Mat& I, float luminance, BottomPoint& point, vector<Point>& BottomSidePoint)
{
	Mat binaryImg;
	Mat Horizontal;
	Mat Vertical;
	Point CenterPoint;
	threshold(I, binaryImg, luminance, BINARY_POINT, THRESH_BINARY);
	morphologyEx(binaryImg, binaryImg, CV_MOP_CLOSE, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT));
	reduce(binaryImg, Horizontal, AXIS_X, CV_REDUCE_SUM, CV_32FC1); 
	reduce(binaryImg, Vertical, AXIS_Y, CV_REDUCE_SUM, CV_32FC1);
	/*imshow("Image sequence", Horizontal);
	waitKey(0);*/
	point.centerPoint.x = findX(Horizontal, point);
	point.centerPoint.y = findY(Vertical);
	point.bottomCenterPoint.y = point.centerPoint.y;
	point.bottomUpPoint.y = findBottomUpDown(I, point, BottomSidePoint);
	point.bottomDownPoint.y = findBottomDownUp(I, point);
	point.bottomCenterPoint.x = findBottom(Horizontal);
	return CenterPoint;
}

void searchCO()
{
	//Rect rect1(370, 0, 140, 400);
	//Mat ddt = image(rect1);
	//GaussianBlur(ddt, ddt, Size(3, 3), 3, 3);
	///*imshow("test", ddt);
	//waitKey(0);*/
	//Mat gray;
	//Canny(ddt, ddt, 3, 9, 3);
	//threshold(gray, gray, 200, 255, THRESH_BINARY);
	//threshold(ddt, gray, 200, 255, THRESH_BINARY);
	/*imshow("test", ddt);
	waitKey(0);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat dst = Mat::zeros(ddt.rows, ddt.cols, CV_8UC3);
	findContours(gray, contours, hierarchy,CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);*/
	//convexHull(gray)
	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	//int idx = 0;
	//boundingRect();

// 	cout << contours.size() << endl;
//	/*for (; idx >= 0; idx = hierarchy[idx][0])
//	{*/
//		Scalar color(rand() & 255, rand() & 255, rand() & 255);
//		drawContours(dst, contours, -1, Scalar(255,0, 0), 1, 8, hierarchy, 2);
////	}
//		Rect rect2 = boundingRect(contours[0]);
//	rectangle(dst, rect2, Scalar(255, 255, 0), 1, 8, 0);
//	/*for (size_t i = 0; i < contours[0].size(); i++)
	/*{
		line(dst, contours[0][i], contours[0][(i + 1) % contours[0].size()], Scalar(0, 255, 0), 1, 8);
	}
	for (size_t i = 0; i < contours[5].size(); i++)
	{
		line(dst, contours[5][i], contours[5][(i + 1) % contours[5].size()], Scalar(0, 0, 255), 1, 8);
	}

	for (size_t i = 0; i < contours[10].size(); i++)
	{
		line(dst, contours[10][i], contours[10][(i + 1) % contours[10].size()], Scalar(255, 0, 0), 1, 8);
	}
	for (size_t i = 0; i < contours[15].size(); i++)
	{
		line(dst, contours[15][i], contours[15][(i + 1) % contours[15].size()], Scalar(255, 255, 255), 1, 8);
	}*/
	//imshow("test", dst);
	//waitKey(0);*/
	////circle(image, )

}
void DrawLevelRect(Mat& image, BottomPoint& point)
{
	cvtColor(image, image, CV_GRAY2BGR);
	Rect rect(point.centerPoint.x, (point.centerPoint.y) / 2 + 20, point.centerThickness + RECT_OFFSET_X, 140);
	rectangle(image, rect, RECT_COLOR, 1, 1, 0);
	
	return;
}

void CalculationBox(Mat& image, BottomPoint& point)
{
	width = point.bottomDownPoint.y - point.bottomUpPoint.y;
	height = 200;
}

void DrawBottomRect(Mat& image, BottomPoint& point, stack<int>& disturbance)
{
	if (!disturbance.empty() && abs(point.bottomUpPoint.y - disturbance.top()) <= 3)
		point.bottomUpPoint.y = disturbance.top();
	Rect rect1(640, point.bottomUpPoint.y, 100, width + 5);//point.bottomCenterPoint.x - 428 + BOTTOM_THICKNESS, point.bottomDownPoint.y - point.bottomUpPoint.y);
	rectangle(image, rect1, Scalar(0, 255, 0), 2, 1, 0);
	disturbance.push(point.bottomUpPoint.y);
	//line(image, Point(640, 94), Point(640, 315), Scalar(0, 255, 0), 2);
	return;
}

void RectDebug(Mat& image, BottomPoint& bottom)
{
	line(image, Point(0, bottom.centerPoint.y), Point(600, bottom.centerPoint.y), Scalar(0, 200, 255));
	line(image, Point(bottom.centerPoint.x, 0), Point(bottom.centerPoint.x, 400), Scalar(0, 200, 255));
	line(image, Point(bottom.bottomCenterPoint.x + 2, 0), Point(bottom.bottomCenterPoint.x + 2, 400), Scalar(0, 255, 0));
	line(image, Point(0, bottom.bottomUpPoint.y), Point(600, bottom.bottomUpPoint.y), Scalar(255, 255, 0));
	line(image, Point(0, bottom.bottomDownPoint.y), Point(600, bottom.bottomDownPoint.y), Scalar(255, 0, 255));
}

void BottomSideDebug(Mat& image, BottomPoint& bottom)
{
	/*line(image, Point(0, bottom.bottomUpPoint.y), Point(600, bottom.bottomUpPoint.y), Scalar(0, 255, 0));
	line(image, Point(0, (bottom.centerPoint.y) / 2  + SUPPORT_UPPER_BOUND), Point(600, (bottom.centerPoint.y) / 2 + SUPPORT_UPPER_BOUND), Scalar(255, 255, 0));
	line(image, Point(0, (bottom.centerPoint.y) / 2), Point(600, (bottom.centerPoint.y) / 2 ), Scalar(0, 255,255));
	line(image, Point(0, (bottom.centerPoint.y) / 2 + SUPPORT_LOWER_BOUND), Point(600, (bottom.centerPoint.y) / 2 + SUPPORT_LOWER_BOUND), Scalar(255, 255, 0));*/
	Vec4f line_para;
	//fitLine(BottomSidePoint, line_para, CV_DIST_L2, 0, 1e-2, 1e-2);
	Point point0;
	point0.x = line_para[2];
	point0.y = line_para[3];
	double k = 0;
	cv::Point point1, point2;
	point1.x = 0;
	point1.y = k * (0 - point0.x) + point0.y;
	point2.x = 640;
	point2.y = k * (640 - point0.x) + point0.y;
	line(image, Point(400, point1.y), point2, cv::Scalar(0, 255, 0), 2, 8, 0);
	//for (int i = 0; i < BottomSidePoint.size(); i++)		//调试找两侧
	//{
	//	circle(image, BottomSidePoint[i], 3, Scalar(0, 0, 255));
	//}
}

void readSequence(string path)
{
	stack<int> Shake;													//扰动量
	vector<Point> BottomSidePoint;										//拟合点
	while (1)
	{
		bool OnePic = true;
		VideoCapture sequence(path);
		if (!sequence.isOpened())
		{
			cerr << "Failed to open the image sequence!\n" << endl;
			return;
		}
		Mat image;
		namedWindow("Image sequence", 0);
		BottomPoint bottom;
		for (;;)
		{
			sequence >> image;
			if (image.empty())
			{
				cout << "End of Sequence" << endl;
				BottomSidePoint.clear();
				BottomSidePoint2.clear();
				while (!Shake.empty()) Shake.pop();

				width = 0;
				height = 0;
				OnePic = true;
				int key = waitKey(0);
				if (key == 74 || key == 106)
					break;
				else if (key == 107 || key == 75)
					return;
				else
					return;
			}
			Point tt = findCenter_P(image, 200.0, bottom, BottomSidePoint);
			DrawLevelRect(image, bottom);
			if (OnePic)
			{
				//DrawBottomRect(image, bottom);
				CalculationBox(image, bottom);
				OnePic = false;
			}
			DrawBottomRect(image, bottom, Shake);
			imshow("Image sequence", image);
			if (OnePic) OnePic = false;
			waitKey(IMAGE_DELAY);
		}
	}
};

void getFiles(string path, vector<string>& files)
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(p.assign(path).append("\\").append(fileinfo.name).append("\\%03d.png"));
			}
			else
			{
				continue;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
