#include <opencv2\opencv.hpp>
#include <iostream>
#include <stack>

using namespace cv;
using namespace std;

//c++中在一个类中定义另一个只有带参数构造函数的类的对象
class CrackInfo
{
public:
	CrackInfo(Point& position, long length, float width) {};
};

/* 增加对比度 */
void addContrast(Mat& srcImg);
/* 交换两个Mat */
void swapMat(Mat& srcImg, Mat& dstImg);
/* 二值化图像。0->0,非0->255 */
void binaryzation(Mat& srcImg);
/* 检测连通域，并删除不符合条件的连通域 */
void findConnectedDomain(Mat& srcImg, vector<vector<Point>>& connectedDomains, int area, int WHRatio);
/* 提取连通域的骨架 */
void thinImage(Mat& srcImg);
/* 获取图像中白点的数量 */
void getWhitePoints(Mat& srcImg, vector<Point>& domain);
/* 计算宽高信息的放置位置 */
Point calInfoPosition(int imgRows, int imgCols, int padding, const std::vector<cv::Point>& domain);

int main(int argc, char** argv) {
	Mat srcImg = imread("./image/20180803215201452.jpg");
	Mat dstImg, dstImg2;
	//灰度化
	cvtColor(srcImg, dstImg, CV_BGR2GRAY, 1);
	//增加对比度
	addContrast(dstImg);
	//图像交换
	swapMat(srcImg, dstImg);
	//边缘检测
	Canny(srcImg, dstImg, 50, 150);
	//形态学变换
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(dstImg, dstImg, kernel);//膨胀
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 3);
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel);
	//寻找连通域
	vector<vector<Point>> connectedDomains;
	findConnectedDomain(dstImg, connectedDomains, 20, 3);
	kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 5);

	connectedDomains.clear();
	findConnectedDomain(dstImg, connectedDomains, 20, 3);
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	morphologyEx(dstImg, dstImg, CV_MOP_OPEN, kernel);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	erode(dstImg, dstImg, kernel);

	connectedDomains.clear();
	findConnectedDomain(dstImg, connectedDomains, 20, 3);

	cout << "开始测量" << endl;
	cout << "连通域数量：" << connectedDomains.size() << endl;
	Mat lookUpTable(1, 256, CV_8U, Scalar(0));
	vector<CrackInfo> crackInfos;
	for (auto domain_it = connectedDomains.begin(); domain_it != connectedDomains.end(); ++domain_it) {
		LUT(dstImg, lookUpTable, dstImg);
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it) {
			dstImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
		double area = (double)domain_it->size();
		thinImage(dstImg);
		getWhitePoints(dstImg, *domain_it);
		long length = (long)domain_it->size();
		Point position = calInfoPosition(dstImg.rows, dstImg.cols, 50, *domain_it);
		crackInfos.push_back(CrackInfo(position, length, (float)(area / length)));
	}

	cout << "开始绘制信息" << endl;
	cout << "信息数量：" << crackInfos.size() << endl;

	LUT(dstImg, lookUpTable, dstImg);
	for (auto domain_it = connectedDomains.cbegin(); domain_it != connectedDomains.cend(); ++domain_it) {
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it) {
			dstImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
	}

	//ostringstream info;
	//for (auto it = crackInfos.cbegin(); it != crackInfos.cend(); ++it) {
	//	info.str("");
	//	info << *it;
	//	putText(dstImg, info.str(), it->Position, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
	//}

	imwrite("result1.png", dstImg);
	cout << "保存图像完成" << endl;
	return 0;
}

/*利用查找表(Look-up table)增加图像对比度*/
void addContrast(Mat& srcImg) {
	Mat lookUpTable(1, 256, CV_8U);
	double temp = pow(1.1, 5);
	uchar* p = lookUpTable.data;
	for (int i = 0; i < 256; ++i)
		p[i] = saturate_cast<uchar>(i * temp);
	LUT(srcImg, lookUpTable, srcImg);
}
/*图像交换*/
void swapMat(Mat& srcImg, Mat& dstImg) {
	Mat tempImg = srcImg;
	srcImg = dstImg;
	dstImg = tempImg;
}
/* 检测连通域，并删除不符合条件的连通域 */
void findConnectedDomain(Mat& srcImg, vector<vector<Point>>& connectedDomains, int area, int WHRatio) {
	Mat_<uchar> tempImg = (Mat_<uchar>&)srcImg;

	for (int i = 0; i < tempImg.rows; ++i) {
		uchar* row = tempImg.ptr(i);    ////调取存储图像内存的第i行的指针
		for (int j = 0; j < tempImg.cols; ++j) {
			if (row[j] == 255) {
				stack<Point> connectedPoints;
				vector<Point> domain;
				connectedPoints.push(Point(j, i));
				while (!connectedPoints.empty()) {
					Point currentPoint = connectedPoints.top();
					domain.push_back(currentPoint);

					int colNum = currentPoint.x;
					int rowNum = currentPoint.y;

					tempImg.ptr(rowNum)[colNum] = 0;
					connectedPoints.pop();

					if (rowNum - 1 >= 0 && colNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum - 1] == 255) {
						tempImg.ptr(rowNum - 1)[colNum - 1] = 0;
						connectedPoints.push(Point(colNum - 1, rowNum - 1));
					}
					if (rowNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum] == 255) {
						tempImg.ptr(rowNum - 1)[colNum] = 0;
						connectedPoints.push(Point(colNum, rowNum - 1));
					}
					if (rowNum - 1 >= 0 && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum - 1)[colNum + 1] == 255) {
						tempImg.ptr(rowNum - 1)[colNum + 1] = 0;
						connectedPoints.push(Point(colNum + 1, rowNum - 1));
					}
					if (colNum - 1 >= 0 && tempImg.ptr(rowNum)[colNum - 1] == 255) {
						tempImg.ptr(rowNum)[colNum - 1] = 0;
						connectedPoints.push(Point(colNum - 1, rowNum));
					}
					if (colNum + 1 < tempImg.cols && tempImg.ptr(rowNum)[colNum + 1] == 255) {
						tempImg.ptr(rowNum)[colNum + 1] = 0;
						connectedPoints.push(Point(colNum + 1, rowNum));
					}
					if (rowNum + 1 < tempImg.rows && colNum - 1 > 0 && tempImg.ptr(rowNum + 1)[colNum - 1] == 255) {
						tempImg.ptr(rowNum + 1)[colNum - 1] = 0;
						connectedPoints.push(Point(colNum - 1, rowNum + 1));
					}
					if (rowNum + 1 < tempImg.rows && tempImg.ptr(rowNum + 1)[colNum] == 255) {
						tempImg.ptr(rowNum + 1)[colNum] = 0;
						connectedPoints.push(Point(colNum, rowNum + 1));
					}
					if (rowNum + 1 < tempImg.rows && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum + 1)[colNum + 1] == 255) {
						tempImg.ptr(rowNum + 1)[colNum + 1] = 0;
						connectedPoints.push(Point(colNum + 1, rowNum + 1));
					}
				}
				if (domain.size() > area) {
					RotatedRect rect = minAreaRect(domain);
					float width = rect.size.width;
					float height = rect.size.height;
					if (width < height) {
						float temp = width;
						width = height;
						height = temp;
					}
					if (width > height * WHRatio && width > 50) {
						for (auto cit = domain.begin(); cit != domain.end(); ++cit) {
							tempImg.ptr(cit->y)[cit->x] = 250;
						}
						connectedDomains.push_back(domain);
					}
				}
			}
		}
	}

	binaryzation(srcImg);
}
/* 二值化图像。0->0,非0->255 */
void binaryzation(Mat& srcImg) {
	Mat lookUpTable(1, 256, CV_8U, Scalar(255));
	lookUpTable.data[0] = 0;
	LUT(srcImg, lookUpTable, srcImg);
}
/* 提取连通域的骨架 */
void thinImage(Mat& srcImg) {
	vector<Point> deleteList;
	int neighbourhood[9];
	int nl = srcImg.rows;
	int nc = srcImg.cols;
	bool inOddIterations = true;
	while (true) {
		for (int j = 1; j < (nl - 1); j++) {
			uchar* data_last = srcImg.ptr<uchar>(j - 1);
			uchar* data = srcImg.ptr<uchar>(j);
			uchar* data_next = srcImg.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++) {
				if (data[i] == 255) {
					int whitePointCount = 0;
					neighbourhood[0] = 1;
					if (data_last[i] == 255) neighbourhood[1] = 1;
					else  neighbourhood[1] = 0;
					if (data_last[i + 1] == 255) neighbourhood[2] = 1;
					else  neighbourhood[2] = 0;
					if (data[i + 1] == 255) neighbourhood[3] = 1;
					else  neighbourhood[3] = 0;
					if (data_next[i + 1] == 255) neighbourhood[4] = 1;
					else  neighbourhood[4] = 0;
					if (data_next[i] == 255) neighbourhood[5] = 1;
					else  neighbourhood[5] = 0;
					if (data_next[i - 1] == 255) neighbourhood[6] = 1;
					else  neighbourhood[6] = 0;
					if (data[i - 1] == 255) neighbourhood[7] = 1;
					else  neighbourhood[7] = 0;
					if (data_last[i - 1] == 255) neighbourhood[8] = 1;
					else  neighbourhood[8] = 0;
					for (int k = 1; k < 9; k++) {
						whitePointCount = whitePointCount + neighbourhood[k];
					}
					if ((whitePointCount >= 2) && (whitePointCount <= 6)) {
						int ap = 0;
						if ((neighbourhood[1] == 0) && (neighbourhood[2] == 1)) ap++;
						if ((neighbourhood[2] == 0) && (neighbourhood[3] == 1)) ap++;
						if ((neighbourhood[3] == 0) && (neighbourhood[4] == 1)) ap++;
						if ((neighbourhood[4] == 0) && (neighbourhood[5] == 1)) ap++;
						if ((neighbourhood[5] == 0) && (neighbourhood[6] == 1)) ap++;
						if ((neighbourhood[6] == 0) && (neighbourhood[7] == 1)) ap++;
						if ((neighbourhood[7] == 0) && (neighbourhood[8] == 1)) ap++;
						if ((neighbourhood[8] == 0) && (neighbourhood[1] == 1)) ap++;
						if (ap == 1) {
							if (inOddIterations && (neighbourhood[3] * neighbourhood[5] * neighbourhood[7] == 0)
								&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[5] == 0)) {
								deleteList.push_back(Point(i, j));
							}
							else if (!inOddIterations && (neighbourhood[1] * neighbourhood[5] * neighbourhood[7] == 0)
								&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[7] == 0)) {
								deleteList.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deleteList.size() == 0)
			break;
		for (size_t i = 0; i < deleteList.size(); i++) {
			Point tem;
			tem = deleteList[i];
			uchar* data = srcImg.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deleteList.clear();

		inOddIterations = !inOddIterations;
	}
}
/* 获取图像中白点的数量 */
void getWhitePoints(Mat& srcImg, vector<Point>& domain) {
	domain.clear();
	Mat_<uchar> tempImg = (Mat_<uchar>&)srcImg;
	for (int i = 0; i < tempImg.rows; i++) {
		uchar* row = tempImg.ptr<uchar>(i);
		for (int j = 0; j < tempImg.cols; ++j) {
			if (row[j] != 0)
				domain.push_back(Point(j, i));
		}
	}
}
/* 计算宽高信息的放置位置 */
Point calInfoPosition(int imgRows, int imgCols, int padding, const std::vector<cv::Point>& domain) {
	long xSum = 0;
	long ySum = 0;
	for (auto it = domain.cbegin(); it != domain.cend(); ++it) {
		xSum += it->x;
		ySum += it->y;
	}
	int x = 0;
	int y = 0;
	x = (int)(xSum / domain.size());
	y = (int)(ySum / domain.size());
	if (x < padding)
		x = padding;
	if (x > imgCols - padding)
		x = imgCols - padding;
	if (y < padding)
		y = padding;
	if (y > imgRows - padding)
		y = imgRows - padding;

	return cv::Point(x, y);
}
