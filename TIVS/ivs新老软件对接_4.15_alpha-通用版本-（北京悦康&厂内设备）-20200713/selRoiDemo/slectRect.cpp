#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace cv;


char imname[500];
cv::Rect roi_sel_cur;
vector<cv::Rect> roi_sel_vec;
cv::Mat img1;


void mouseHandler(int event, int x, int y, int, void* pcvim)
{
	static int drag = 0;
	cv::Mat img0 = *(cv::Mat*)pcvim;

	if (event == cv::EVENT_LBUTTONDOWN && !drag)
	{
		img1 = img0.clone();
		drag = 1;
		roi_sel_cur = cv::Rect(cv::Point(x, y), cv::Point(x, y));
		for (vector<cv::Rect>::iterator it = roi_sel_vec.begin(); it != roi_sel_vec.end(); it++)
			cv::rectangle(img1, *it, cv::Scalar(255, 0, 0), 2);
		imshow(imname, img1);
	}
	if (event == cv::EVENT_LBUTTONUP && drag)
	{
		imshow(imname, img1);
		roi_sel_vec.push_back(roi_sel_cur);
		drag = 0;
	}
	if (event == cv::EVENT_MOUSEMOVE && drag) {
		img1 = img0.clone();
		roi_sel_cur = cv::Rect(roi_sel_cur.tl(), cv::Point(x, y));
		cv::rectangle(img1, roi_sel_cur, cv::Scalar(0, 255, 0), 2);
		for (vector<cv::Rect>::iterator it = roi_sel_vec.begin(); it != roi_sel_vec.end(); it++)
			cv::rectangle(img1, *it, cv::Scalar(255, 0, 0), 2);
		imshow(imname, img1);
	}
}

void main() {
	bool iret(false);
	int cnt(0);
	do {
		roi_sel_vec.clear();
		sprintf(imname, "./pic/%03d.png", cnt++);
		cv::Mat im = cv::imread(imname, CV_LOAD_IMAGE_ANYCOLOR);
		if (im.empty())
			break;
		cv::namedWindow(imname, CV_WINDOW_NORMAL);
		cv::resizeWindow(imname, 800, 600);
		cv::imshow(imname, im);
		cv::setMouseCallback(imname, mouseHandler, &im);
		bool ret(true);
		while (ret)
		{
			cv::imshow(imname, im);
			char ch = cv::waitKey(0);
			switch (ch)
			{
			case 'c':  // 删除上一个rect
				;
				break;
			case 'n': // 跳转到下一张图像输入
				;
				ret = false;
				break;
			case 'j':  // 选择缺陷1；
				cv::putText(img1, "", cv::Point(roi_sel_cur.tl().x, roi_sel_cur.tl().y /2 + roi_sel_cur.br().y /2 ), 1, 1, cv::Scalar(0, 255, 0),2 );
				cv::imshow(imname, img1);
				cv::waitKey();
				break;
			default:
				break;
			}
		}
		cv::destroyWindow(imname);

	} while (true);
}