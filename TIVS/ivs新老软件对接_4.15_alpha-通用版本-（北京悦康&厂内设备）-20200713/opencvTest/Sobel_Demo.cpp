/**
 * @file Sobel_Demo.cpp
 * @brief Sample code using Sobel and/orScharr OpenCV functions to make a simple Edge Detector
 * @author OpenCV team
 */

//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/**
 * @function main
 */
int main( int, char** argv )
{

  Mat src, src_gray;
  Mat grad;
  const char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  cv::namedWindow(window_name, CV_WINDOW_NORMAL);
  /// Load an image
  //src = imread( argv[1] );
  for (int idx = 0; idx < 125; idx++) {

	  char ch[100];
	  sprintf(ch, "./pic/%03d.png", idx);
	  src = cv::imread(ch,CV_LOAD_IMAGE_GRAYSCALE);
	  cv::Mat src_show;
	  cv::cvtColor(src, src_show, CV_GRAY2BGR);

	  if (src.empty())
	  {
		  return -1;
	  }


	  cv::Mat mclose = cv::Mat::zeros(src.size(), CV_8UC1);
	  GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	  cv::Mat grad_y, abs_grad_y;
	  cv::Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	  cv::convertScaleAbs(grad_y, abs_grad_y);
	  cv::Mat grad;
	  grad = abs_grad_y;
	  cv::threshold(grad, grad, 35, 255, CV_THRESH_BINARY);
	  cv::Mat binary;
	  cv::threshold(src, binary, 110, 255, CV_THRESH_BINARY_INV);
	  cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	  cv::dilate(binary, binary, kn3x3);
	  cv::bitwise_and(grad, binary, binary);
	  cv::Mat kn7x1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 1));
	  cv::dilate(binary, binary, kn7x1);
	  vector< vector<cv::Point> > contours;
	  cv::findContours(binary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	  /*cv::cvtColor(binary, binary, CV_GRAY2BGR);*/
	  for (int ic = 0; ic < contours.size(); ic++) {
		  cv::Rect rect = cv::boundingRect(contours[ic]);
		  //cv::RNG rng(12345);
		  //cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		  cv::rectangle(mclose, rect, cv::Scalar::all(255) , -1);
	  }
	  contours.clear();
	  cv::findContours(mclose.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	  for (int ic = 0; ic < contours.size(); ic++) {
		  cv::Rect rect = cv::boundingRect(contours[ic]);
		  if(rect.width > 45)
			  cv::rectangle(src, rect, cv::Scalar::all(255), -1);
		  //cv::RNG rng(12345);
		  //cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		  
	  }


	  /////// Convert it to gray
	  ////cvtColor(src, src_gray, COLOR_RGB2GRAY);
	  //src_gray = src.clone();

	  /////// Create window
	  ////namedWindow(window_name, WINDOW_AUTOSIZE);

	  ///// Generate grad_x and grad_y
	  //Mat /*grad_x, */grad_y;
	  //Mat /*abs_grad_x,*/ abs_grad_y;

	  /////// Gradient X
	  //////Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	  ////Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	  ////convertScaleAbs(grad_x, abs_grad_x);

	  ///// Gradient Y
	  ////Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	  //Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	  //convertScaleAbs(grad_y, abs_grad_y);

	  /////// Total Gradient (approximate)
	  ////addWeighted(abs_grad_x, 0.0, abs_grad_y, 1., 0, grad);
	  //grad = abs_grad_y;

	  //cv::cvtColor(grad, grad, CV_GRAY2BGR);
	  //imshow(window_name, grad);
	  //cv::waitKey();

	  //cv::threshold(grad, grad, 35, 255, CV_THRESH_BINARY);
	  //imshow(window_name, grad);
	  //waitKey(0);

	  //cv::Mat binary;
	  //cv::threshold(src, binary, 110, 255, CV_THRESH_BINARY_INV);
	  //cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	  //cv::dilate(binary, binary, kn3x3);
	  //cv::cvtColor(grad, grad, CV_BGR2GRAY);
	  //cv::bitwise_and(grad, binary, binary);
	  //imshow(window_name, binary);
	  //waitKey(0);

	  //binary.colRange(0, 300).setTo(0);
	  //binary.colRange(600, binary.cols).setTo(0);
	  //cv::Mat kn5x1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 1));
	  //cv::dilate(binary, binary, kn5x1);
	  //vector< vector<cv::Point> > contours;
	  //cv::findContours(binary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	  //cv::cvtColor(binary, binary, CV_GRAY2BGR);
	  //for (int ic = 0; ic < contours.size(); ic++) {
		 // cv::Rect rect = cv::boundingRect(contours[ic]);
		 // cv::RNG rng(12345);
		 // cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		 // cv::rectangle(src_show, rect, clr);
	  //}
	  //cv::imshow(window_name, binary);
	  //waitKey(0);

	  //cv::imshow(window_name, src_show);
	  //waitKey(0);
  }


  return 0;
}
