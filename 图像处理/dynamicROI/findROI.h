#pragma once
#include "Sys.h"

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
				target.center = Point(box.x + (box.width / 2), box.y + (box.height / 2));
				target.label = labelValue;
				target.radius = box.height / 2;
				target.AspectRatio = (area / ((3.1415926) * pow(target.radius, 2)));
				boxList.push_back(target);
			}
		}
	}
	return labelValue;
}

namespace MyDrawRect {
	void selectImgaeROI(Mat&I, Mat& target, unsigned int mode)
	{
		Mat mask, imageROI, roi;
		switch (mode) {
			case BOTTOM_CAP_ROI: {
				imageROI = target(Rect(127, 80, 1, 230));
				roi = I(Rect(127, 80, 1, 230));
				break;
				}
			case BOTTOM_SIDE_ROI: {
				imageROI = target(Rect(640, 0, 120, 400));
				roi = I(Rect(640, 0, 120, 400));
				break;
			}
			case LEVEL_SIDE_ROI: {
				imageROI = target(Rect(200, 0, 300, 400));
				roi = I(Rect(200, 0, 300, 400));
				break;
			}
			case CAPSULE_ROI: {
				imageROI = target(Rect(0, 0, 180, 400));
				roi = I(Rect(0, 0, 180, 400));
				break;
			}
		}
		roi.copyTo(imageROI, mask);
		return;
	}

	int findX(Mat& I, int anchorPoint, int minThickness)
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
		for (int j = nCols; j > 0; j--)
		{
			if ((int)p[j] >= anchorPoint)
				for (int i = j; i > 0; i--)
					if ((int)p[i] == 0 && abs(i - j) > minThickness)
						return i;
					else continue;
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
		int i, j, maxDot = 0, y = 0;
		float* p;
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

	int findBottomUpDown(Mat& I, BottomPoint& point, vector<Point>& BottomSidePoint)
	{
		CV_Assert(I.depth() == CV_8UC1);

		for (int j = 0; j < SAMPLING_POINT_NUM; j += SEARCH_STEP)
		{
			for (int i = 0; i < IMAGE_HEIGHT; i++)
			{
				uchar* p = I.ptr<uchar>(i);
				if (point.centerPoint.x - j >= 0)
				{
					if (p[point.centerPoint.x - j] >= BRIGHTNESS_SIDE)
					{
						if (i >= (point.centerPoint.y / 2) + SUPPORT_UPPER_BOUND && i <= (point.centerPoint.y / 2) + SUPPORT_LOWER_BOUND && (point.centerPoint.x + j) > 0)
							BottomSidePoint.emplace_back(Point(point.centerPoint.x + j, i));
						break;
					}
				}
			}
		}

		if (BottomSidePoint.empty())
			return (point.bottomUpPoint.y = ((point.centerPoint.y / 2) + SUPPORT_UPPER_BOUND));
			
		Vec4f line_para;
		fitLine(BottomSidePoint, line_para, CV_DIST_L2, 0, FITTING_ACCURACY, FITTING_ACCURACY);		//拟合直线
		return (point.bottomUpPoint.y = line_para[3]);
	}

	int findBottomDownUp(Mat& I, BottomPoint& point, vector<Point>& BottomSidePoint)
	{
		CV_Assert(I.depth() == CV_8UC1);

		for (int j = 0; j < SAMPLING_POINT_NUM; j += SEARCH_STEP)
		{
			for (int i = IMAGE_HEIGHT - 1; i > 0; i--)
			{
				uchar* p = I.ptr<uchar>(i);
				if (point.centerPoint.x - j >= 0)
				{
					if (p[point.centerPoint.x - j] >= BRIGHTNESS_SIDE)
					{
						if (i >= (point.centerPoint.y + (point.centerPoint.y) / 2) - SUPPORT_LOWER_BOUND && i <= (point.centerPoint.y + (point.centerPoint.y) / 2 - SUPPORT_UPPER_BOUND))
							BottomSidePoint.push_back(Point(point.centerPoint.x - j, i));
						break;
					}
				}
			}
		}

		if (BottomSidePoint.empty())
			return (point.bottomDownPoint.y = point.centerPoint.y + (point.centerPoint.y) / 2 - SUPPORT_UPPER_BOUND);

		Vec4f line_para;
		fitLine(BottomSidePoint, line_para, CV_DIST_L2, 0, FITTING_ACCURACY, FITTING_ACCURACY);		//最小二乘拟合直线
		return (point.bottomDownPoint.y = line_para[3]);
	}

	Point findCenter_P(Mat& I, float luminance, int anchorPoint, int minThickness)
	{
		Mat binaryImg, Horizontal, Vertical;
		threshold(I, binaryImg, CENTERPOINT_BINARY_THRESHOLD, BINARY_POINT_MIN, THRESH_BINARY);
		morphologyEx(binaryImg, binaryImg, CV_MOP_CLOSE, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT));
		reduce(binaryImg, Horizontal, AXIS_X, CV_REDUCE_SUM, CV_32FC1);
		reduce(binaryImg, Vertical, AXIS_Y, CV_REDUCE_SUM, CV_32FC1);
		return Point(findX(Horizontal, anchorPoint, minThickness), findY(Vertical));
	}

	void findLevelCenterPoint(Mat& I, float luminance, int anchorPoint, int minThickness, BottomPoint& point)
	{
		Mat carrier = Mat::zeros(I.rows, I.cols, CV_8UC1);
		selectImgaeROI(I, carrier, LEVEL_SIDE_ROI);
		point.centerPoint = findCenter_P(carrier, luminance, anchorPoint, minThickness);
		return;
	}

	void findBottomCenterPoint(Mat& I, float luminance, int anchorPoint, int minThickness, BottomPoint& point)
	{
		Mat carrier = Mat::zeros(I.rows, I.cols, CV_8UC1);
		selectImgaeROI(I, carrier, BOTTOM_SIDE_ROI);
		point.bottomCenterPoint = findCenter_P(carrier, luminance, anchorPoint, minThickness);
		return;
	}

	void boxFilterFunc(vector<targetBox>& boxList)
	{
		if (boxList.size() <= 1)
			return;

		for (vector<targetBox>::iterator ite = boxList.begin(); ite != boxList.end(); )
		{
			if (boxList.size() > 1)
			{
				if (ite->boundryBox.width > ite->boundryBox.height)	ite = boxList.erase(ite);							//竖直型
				else if (ite->center.y > MAXIMUM_CENTER_POINT)	ite = boxList.erase(ite);								//中心偏离最大点
				else if (ite->center.y < MINIMUM_CENTER_POINT)	ite = boxList.erase(ite);								//中心偏离最小点
				else if (ite->AspectRatio > ASPECT_RATIO)	ite = boxList.erase(ite);									//圆宽比
				else ite++;
			}
			else
				break;
		}

		while (boxList.size() > 1)
		{
			vector<targetBox>::iterator box1 = boxList.begin(), box2 = box1 + 1;
			if (abs(box1->center.y - 200) < abs(box2->center.y - 200))
				if (box1->boundryBox.height > box2->boundryBox.height)	boxList.erase(box2);
				else boxList.erase(box1);
			else
				if (box1->boundryBox.height > box2->boundryBox.height) boxList.erase(box2);
				else boxList.erase(box1);
		}
		return;
	}

	int randomSeed(Mat& I, Rect& rec, int seedNum)
	{
		vector<int> seed;
		srand((int)time(NULL));
		int pos;
		for (int i = 0; i < seedNum; i++)
		{
			pos = I.at<uchar>(Point((rec.x + 1 + rand() % rec.width - 1), (rec.y + rand() % rec.height)));
			if (pos > 15 && pos < 70)	seed.push_back(pos);
		}
		sort(seed.begin(), seed.end());
		return seed[seed.size() * 0.5];
	}

	bool SearchStrategy(Mat& I, targetBox rec)
	{
		/*int u1 = 0, u2 = 0;
		float std1 = 0.0, std2 = 0.0;
		for (int i = rec.center.y; i > rec.boundryBox.y; i--)	u1 += I.at<uchar>(i, rec.center.x);
		u1 /= (rec.center.y - rec.boundryBox.y);

		for (int i = rec.center.y; i > rec.boundryBox.y; i--)	std1 += pow((I.at<uchar>(i, rec.center.x) - u1), 2);
		std1 = pow(std1, 0.5);

		for (int i = rec.center.y; i < rec.boundryBox.y + rec.boundryBox.height; i++)	u2 += I.at<uchar>(i, rec.center.x);
		u2 = u2 / ((rec.boundryBox.y + rec.boundryBox.height) - rec.center.y);

		for (int i = rec.center.y; i < rec.boundryBox.y + rec.boundryBox.height; i++)	std2 += pow((I.at<uchar>(i, rec.center.x) - u2), 2);
		std2 = pow(std2, 0.5);
		return ((std1 <= std2) || (abs(u1 - u2) > 10 && u1 < u2 && u1 > 20));*/
		uchar ppt;
		Point p1;
		Point p2;
		for (int i = rec.center.y; i > 0; i--)
		{
			ppt = I.at<uchar>(i, rec.center.x);
			if (ppt == 255)
			{
				p1.y = i;
				break;
			}
		}
		for (int i = rec.center.y; i < 400; i++)
		{
			ppt = I.at<uchar>(i, rec.center.x);
			if (ppt == 255)
			{
				p2.y = i;
				break;
			}
		}
		if (abs(p2.y - (rec.boundryBox.y + rec.boundryBox.height)) >= 6 && abs(p1.y - rec.boundryBox.y) < abs(p2.y - (rec.boundryBox.y + rec.boundryBox.height)))
		{
			return true;
		}
		else if (abs(p1.y - rec.boundryBox.y) >= 6 && abs(p2.y - (rec.boundryBox.y + rec.boundryBox.height)) < abs(p1.y - rec.boundryBox.y))
		{
			return false;
		}
		return true;
	}

	Point findBenchmarkPoint(Mat& I, float luminance, int anchorPoint, targetBox rec, bool& flag)							//寻找基准点
	{
		targetBox tar;
		vector<RotatedRect> rectSet;
		Mat carrier = Mat::zeros(I.rows, I.cols, CV_8UC1);
		selectImgaeROI(I, carrier, CAPSULE_ROI);
		float door = mean(carrier)[0];
		if (door > 2.700)
			carrier = carrier * 0.300;
		threshold(carrier, carrier, 12, 255, THRESH_BINARY_INV);
		morphologyEx(carrier, carrier, CV_MOP_CLOSE, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT), Point(-1, -1), 3);
		uchar seedP;
		Point record, p1, p2;
		if (flag)
		{
			for (int i = rec.center.y; i > 0; i--)
			{
				seedP = carrier.at<uchar>(i, rec.center.x);
				if (seedP == 255 && (i - 2) > 0 && carrier.at<uchar> (i - 1, rec.center.x) == 255)
				{
					record.y = i;
					break;
				}
			}
		}
		else
		{
			for (int i = rec.center.y; i < IMAGE_HEIGHT; i++)
			{
				seedP = carrier.at<uchar>(i, rec.center.x);
				if (seedP == 255)
				{
					record.y = rec.boundryBox.y + (i - (rec.boundryBox.y + rec.boundryBox.height));
					break;
				}
			}
		}
		return record;
	}

	void adjustCenterPoint(Mat& I, targetBox& box)
	{
		int oldP = 0, newP = 0;
		oldP = I.at<uchar>(box.center);
		if (box.boundryBox.width > 30)
		{
			newP = I.at<uchar>(box.center.y, box.boundryBox.x + ((box.center.x - box.boundryBox.x) / 2));
			if ((newP < oldP && (newP > 20 && newP < 100)) || ((box.center.y, box.boundryBox.x + ((box.center.x - box.boundryBox.x) / 2)) < box.center.y && box.center.y, box.boundryBox.x + ((box.center.x - box.boundryBox.x) / 2) >= 100))
				box.center = Point(box.boundryBox.x + ((box.center.x - box.boundryBox.x) / 2), box.center.y);
		}
		return;
	}

	targetBox convergenceRect(Mat& I, targetBox rect, int lodiff = 15)
	{
		targetBox alternativeBox = rect;
		vector<Point> randomStart;
		uchar seedP = 0;
		if (rect.center.x >= 0 && rect.center.x < I.cols && rect.center.y >= 0 && rect.center.y < I.rows)
			seedP = I.at<uchar>(rect.center);
		if (seedP < MINIMUM_SEED_VALUE || seedP > MAXIMUM_SEED_VALUE)
			seedP = randomSeed(I, rect.boundryBox, SAMPLING_POINT_NUM);

		//up------------------------------->down
		Mat mask1 = Mat::zeros(I.rows, I.cols, CV_8UC1);
		for (int j = alternativeBox.boundryBox.x; j < alternativeBox.boundryBox.x + alternativeBox.boundryBox.width; j++)
			for (int i = alternativeBox.boundryBox.y; i < alternativeBox.boundryBox.y + alternativeBox.boundryBox.height; i++)
				if (abs(I.at<uchar>(i, j) - seedP) >= lodiff)
					mask1.at<uchar>(i, j) = 1;

		reduce(mask1, mask1, AXIS_X, CV_REDUCE_SUM, CV_32FC1);
		int pos = 0;
		for (int i = 0; i < mask1.cols; i++)
			if (mask1.at<float>(0, i) != 0 && mask1.at<float>(0, i) <= (alternativeBox.boundryBox.height * 0.5))
			{
				pos = i;
				break;
			}
		if (pos != 0 && pos < alternativeBox.center.x)	alternativeBox.boundryBox.x = pos;

		pos = 0;
		//down---------------------------->up
		Mat mask2 = Mat::zeros(I.rows, I.cols, CV_8UC1);
		for (int j = alternativeBox.boundryBox.x + alternativeBox.boundryBox.width; j > alternativeBox.boundryBox.x; j--)//控制列
			for (int i = alternativeBox.boundryBox.y; i < alternativeBox.boundryBox.y + alternativeBox.boundryBox.height; i++)//控制行
				if (abs(I.at<uchar>(i, j) - seedP) >= lodiff)
					mask2.at<uchar>(i, j) = 1;
		reduce(mask2, mask2, AXIS_X, CV_REDUCE_SUM, CV_32FC1);
		for (int i = mask2.cols - 1; i > 0; i--)
			if (mask2.at<float>(0, i) != 0 && mask2.at<float>(0, i) <= (alternativeBox.boundryBox.height * 0.7))
			{
				pos = i;
				break;
			}
		if (pos != 0 && pos > alternativeBox.boundryBox.x)	alternativeBox.boundryBox.width = pos - alternativeBox.boundryBox.x;

		pos = 0;
		//left------------------------->right
		Mat mask3 = Mat::zeros(I.rows, I.cols, CV_8UC1);
		for (int i = alternativeBox.boundryBox.y; i < alternativeBox.center.y; i++)//行
			for (int j = alternativeBox.boundryBox.x; j < alternativeBox.boundryBox.x + alternativeBox.boundryBox.width; j++)//列
				if (abs(I.at<uchar>(i, j) - seedP) >= lodiff)
					mask3.at<uchar>(i, j) = 1;
		reduce(mask3, mask3, AXIS_Y, CV_REDUCE_SUM, CV_32FC1);
		for (int i = 0; i < I.rows; i++)
			if (mask3.at<float>(i, 0) != 0 && mask3.at<float>(i, 0) <= (alternativeBox.boundryBox.width * 0.8))
			{
				pos = i;
				break;
			}
		if (pos != 0 && pos > alternativeBox.boundryBox.y)	alternativeBox.boundryBox.y = pos;


		if ((alternativeBox.boundryBox.y + alternativeBox.boundryBox.height) >= I.rows)
		{
			alternativeBox = rect;
			return alternativeBox;
		}

		pos = 0;
		//right--------------------------------------->left
		Mat mask4 = Mat::zeros(I.rows, I.cols, CV_8UC1);
		for (int i = alternativeBox.boundryBox.y + alternativeBox.boundryBox.height; i > alternativeBox.center.y; i--)
			for (int j = alternativeBox.boundryBox.x; j < alternativeBox.boundryBox.x + alternativeBox.boundryBox.width; j++)
				if (abs(I.at<uchar>(i, j) - seedP) >= lodiff)
					mask4.at<uchar>(i, j) = 1;
		reduce(mask4, mask4, AXIS_Y, CV_REDUCE_SUM, CV_32FC1);
		for (int i = I.rows - 1; i > 0; i--)
			if (mask4.at<float>(i, 0) != 0 && mask4.at<float>(i, 0) <= (alternativeBox.boundryBox.width * 0.5))
			{
				pos = i;
				break;
			}
		if (pos != 0)	alternativeBox.boundryBox.height = pos - alternativeBox.boundryBox.y;
		alternativeBox.center.x = alternativeBox.boundryBox.x + (alternativeBox.boundryBox.width) / 2;
		alternativeBox.center.y = alternativeBox.boundryBox.y + (alternativeBox.boundryBox.height) / 2;
		seedP = I.at<uchar>(alternativeBox.center);
		if (alternativeBox.boundryBox.y < rect.boundryBox.y || alternativeBox.boundryBox.height > rect.boundryBox.height || alternativeBox.boundryBox.width > rect.boundryBox.width || alternativeBox.boundryBox.width > alternativeBox.boundryBox.height || (alternativeBox.boundryBox.height * alternativeBox.boundryBox.width) < 150 /*|| abs(beiXuan.center.y - 200) > abs(rect.center.y - 200)*/ || alternativeBox.boundryBox.width < 10 /*|| abs(beiXuan.boundryBox.height - rect.boundryBox.height) > 50*/)
			alternativeBox = rect;
		return alternativeBox;
	}

	targetBox searchCakeRect(Mat& I)
	{
		//step1------------------分离背景与前景-------------------------------------------------------
		Mat binaryImage, A;
		float door = mean(I)[0];
		if (door > 2.700)
		{
			I = I * 0.200;
			threshold(I, binaryImage, 0, 255, THRESH_OTSU);
			morphologyEx(binaryImage, binaryImage, CV_MOP_OPEN, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT), Point(-1, -1), 3);
		}
		else
		{
			threshold(I, A, 190, 255, THRESH_BINARY);
			threshold(I, binaryImage, 10, 255, THRESH_BINARY_INV);
			morphologyEx(binaryImage, binaryImage, CV_MOP_CLOSE, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT), Point(-1, -1), 3);
			bitwise_not(binaryImage, binaryImage);
			absdiff(A, binaryImage, binaryImage);
			morphologyEx(binaryImage, binaryImage, CV_MOP_OPEN, getStructuringElement(MORPH_RECT, KERNEL_SIZE, ANCHOR_POINT), Point(-1, -1), 3);
		}

		//step2-----------------------划分区域---------------------------------------------------------
		Mat target;
		vector<targetBox> boxList;
		findBoundaryBox(binaryImage, target, boxList);
		boxFilterFunc(boxList);
		boxList[0] = convergenceRect(I, boxList[0]);
		return boxList[0];
	}

	void divideReactROI(Mat& I, targetBox& box)																								//划分备选区域
	{
		Mat carrier = Mat::zeros(I.rows, I.cols, CV_8UC1);
		selectImgaeROI(I, carrier, CAPSULE_ROI);
		GaussianBlur(carrier, carrier, GAUSSIAN_KERNEL_SIZE, 2);
		box = searchCakeRect(carrier);
		return;
	}

	Point findBenchmarkPoint(Mat& I, float luminance, int anchorPoint, targetBox& boundryBox)												//寻找基准点
	{
		divideReactROI(I, boundryBox);
		adjustCenterPoint(I, boundryBox);
		return Point(boundryBox.boundryBox.y + boundryBox.boundryBox.height, boundryBox.boundryBox.y);										//x=左上
	}

	void DrawLevelRect(Mat& image, BottomPoint& point)
	{
		Rect rect(point.centerPoint.x, (point.centerPoint.y) / 2 + 20, 40 + RECT_OFFSET_X, 140);
		cv::rectangle(image, rect, RECT_COLOR, 1, 1, 0);
		return;
	}

	void CalculationBox(Mat& image, BottomPoint& point)
	{
		point.BottomRectWidth = point.bottomDownPoint.y - point.bottomUpPoint.y;
		point.BottomRectHeight = point.bottomCenterPoint.x - 640;
	}

	void DrawBottomRect(Mat& image, BottomPoint& point, float luminance, int anchorPoint, int minThickness, stack<int>& disturbance, findBottomPoint& bottom, Point& biasP, stack<int>& verticalBiasPoint, bool& flag, targetBox& a)
	{
		if (!disturbance.empty() && abs(biasP.y - disturbance.top()) >= DISTURBANCEOFFSET)
		{
			if (!disturbance.empty() && abs(biasP.y - disturbance.top()) >= DISTURBANCEOFFSET)
			{
				if (abs(biasP.y - disturbance.top()) <= DISTURBANCEOFFSET + 2)
				{
					int test = (disturbance.top() + biasP.y) / 2;																//平均
					biasP.y = test;
				}
				else
					biasP.y = disturbance.top();
			}
		}

		if (!verticalBiasPoint.empty() && abs(bottom.DatumVerticalPoint.x - point.bottomCenterPoint.x) >= VERTICAL_RESISTANCE)
			point.bottomCenterPoint.x = verticalBiasPoint.top();

		Rect rect1;
		if (biasP.x > 200)
		{
			//再次判断
			rect1 = Rect(IMAGE_WIDTH - (IMAGE_WIDTH * 0.3), point.bottomUpPoint.y - 3, point.BottomRectHeight + 120, point.BottomRectWidth + 5);
		}
		else if (abs(biasP.y - bottom.BenchmarkNeckPoint.y) > 20)
		{
			rect1 = Rect(IMAGE_WIDTH - (IMAGE_WIDTH * 0.3) , bottom.DatumPointY.y - 3, point.BottomRectHeight + 120, point.BottomRectWidth + 5);
			biasP.y = bottom.DatumPointY.y - 4;
		}
		else
		{
			if (biasP.y - bottom.BenchmarkNeckPoint.y > 4)
			{
				if (abs(biasP.y - bottom.BenchmarkNeckPoint.y) < 4)
					biasP.y += (biasP.y - bottom.BenchmarkNeckPoint.y) / 2;
				else
				{
					if (!disturbance.empty())
						biasP.y = disturbance.top();
					else
						biasP.y = bottom.BenchmarkNeckPoint.y;
				}
			}
			////基准点+晃动点
			rect1 = Rect(IMAGE_WIDTH - (IMAGE_WIDTH * 0.3), bottom.DatumPointY.y + (biasP.y - bottom.BenchmarkNeckPoint.y), point.BottomRectHeight + 120, point.BottomRectWidth + 5);
		}
		cv::rectangle(image, rect1, COLOR(0, 255, 0), 2, 1, 0);
		disturbance.push(biasP.y);
		verticalBiasPoint.push(point.bottomCenterPoint.x);
		return;
	}
}

void putTextForSequence(Mat& image, string text, Point p = Point(295, 45), int font_face = cv::FONT_HERSHEY_COMPLEX, double font_scale = 2.0, int thicness = 2, Scalar color = COLOR(255, 0, 0))
{
	cv::putText(image, text, p, font_face, font_scale, color, thicness, 8, 0);
	return;
}

void readSequence(string path)
{
	stack<int> Shake;																											//扰动量
	stack<int> verticalBiasPoint;																								//垂直晃动
	vector<Point> BottomSidePoint;																								//拟合点
	vector<Point> BottomDownSidePoint;																							//下拟合点
	Point biasPoint;																											//水平晃动
	Point bottomBenck;
	targetBox locationBox;
	int picNum = 0;
	bool stategy;
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
			image *= ENHANCEMENT_FACTOR;
			if (image.empty())
			{
				cout << "End of Sequence" << endl;
				BottomSidePoint.clear();
				BottomDownSidePoint.clear();
				while (!Shake.empty()) Shake.pop();
				picNum = 0;
				OnePic = true;
				char key = cv::waitKey(0);
				if (key == 'j' || key == 'J')
					break;
				else if (key == 'k' || key == 'K')
					return;
				else
					return;
			}
			
			picNum++;
			MyDrawRect::findLevelCenterPoint(image, 200.0, 10, 14, bottom);
			MyDrawRect::findBottomCenterPoint(image, 100.0, 3, 0, bottom);
			
			MyDrawRect::findBottomUpDown(image, bottom, BottomSidePoint);
			MyDrawRect::findBottomDownUp(image, bottom, BottomDownSidePoint);
			if (OnePic)
			{
				MyDrawRect::CalculationBox(image, bottom);
				bottom.BenchmarkNeckPoint = MyDrawRect::findBenchmarkPoint(image, 200.0, 10, locationBox);
				stategy = MyDrawRect::SearchStrategy(image, locationBox);
				OnePic = false;
				bottom.DatumPointY = bottom.bottomUpPoint;
				bottom.DatumVerticalPoint = bottom.bottomCenterPoint;
			}
			Point biasPoint = MyDrawRect::findBenchmarkPoint(image, 200.0, 10, locationBox, stategy);
			cv::cvtColor(image, image, CV_GRAY2BGR);
			MyDrawRect::DrawBottomRect(image, bottom, 100.0, 3, 0, Shake, bottom, biasPoint, verticalBiasPoint, stategy, locationBox);
			MyDrawRect::DrawLevelRect(image, bottom);
			putTextForSequence(image, to_string(picNum));
			cv::line(image, Point(locationBox.boundryBox.x, locationBox.boundryBox.y), Point(locationBox.boundryBox.x + locationBox.boundryBox.width, locationBox.boundryBox.y), Scalar(0, 255, 0));
			cv::line(image, Point(locationBox.boundryBox.x, locationBox.boundryBox.y + locationBox.boundryBox.height), Point(locationBox.boundryBox.x + locationBox.boundryBox.width, locationBox.boundryBox.y + locationBox.boundryBox.height), Scalar(0, 255, 0));
			cv::circle(image, locationBox.center, 3, Scalar(0, 0, 255));
			
			cv::imshow("Image sequence", image * 5 );
			if (OnePic) OnePic = false;
			cv::waitKey(IMAGE_DELAY);
		}
	}
};




