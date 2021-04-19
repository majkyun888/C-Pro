

#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"
//#include <opencv2/highgui.h>
//#include <opencv2/opencv.hpp>
//#include <opencv/highgui/highgui.hpp>
//#include <opencv/opencv.hpp>
//#include <opencv/cv.hpp>


#define    NECK_ONLY          2  // vial neck 
#define    CRIMPING_ONLY      4  // crimping area 
#define    NECK_CRIMPING    8  // neck and crimping check


//#define CAPMODELSIZE     250
	typedef	struct 
	{
		// params about key points postitioning
  		int roi_left; //搜索区左端
		int roi_right; //搜索区右端
		int head_border; //瓶头搜索门槛
		int radius_offset; //瓶颈半径缩进
		int min_points; //搜索到的最少点数
		// params about bloc configure
		int neck_width; //瓶脖分块宽度
		int bloc_height;//瓶脖分块高度
		int crimp_width; //翻边检测宽度
		// params about absolute level in neck area
		int low_limit;// 绝对灰度低限
		int high_limit;//绝对灰度高限
		int abs_num_th;//绝对值数量门槛
		// params about relative difference in neck area
		int dist;//相对误差距离
		int diff_th;//相对灰度门槛
		int rlt_num_th;//相对误差数量门槛
		// params about relative difference in crimp area 
		int dist_crimp;
		int diff_th_crimp;
		int rlt_num_th_crimp;

	}Para_Neck ;

	typedef	struct 
	{
		int abs_high_err;
		int abs_low_err;
		int rlt_err;
		int rlt_err_crimp;
		//int num;
	}Rlt_Neck;


class CNeck_inspection
{

//	DECLARE_DYNAMIC(CTip_inspection)

public:

  static int Set_inspection_area( int ind );

  CNeck_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int mode_in=NECK_ONLY);
  ~CNeck_inspection(void);

  int        index;          // index of camera/image to be bound to
  int        mode;           // TIP_INSPECTION for AMPOULES - CAP_INSPECTION for VIALS
	CFile_mngmt*  file_manager;
	CArea_adjust* area; // inspection/search area
  BYTE*  img;          // pointer to image to process
  int    dim, dimx, dimy;

  int  Add_menu_parameters( CContainer_wnd* child );
 

   // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  CString txt3;  // third row of result's text (crimping result)
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text
  UINT    col3;  // color of third row of text (crimping result)
  int  Process_image( cv::Mat img_acq, UINT ind );  
  //Para_Neck input;
  //Rlt_Neck output;


private:


	int neck(const cv::Mat &im_src, cv::Mat &im_result);
    Para_Neck input;
	Rlt_Neck output;
	int detect_diff_dist( const cv::Mat &srcImage, cv::Mat &colorImage,  int direction, int dist, int diff_th );
	//int vec2Mat(vector<vector<int> > src,cv::Mat & dst);  
	int vector2Mat(vector< vector<int> > src, cv::Mat &dst);
	int tintPixel(cv::Mat& des, cv::Mat& mask, cv::Scalar &color, const vector<cv::Point> origin);
	//vector<int> partition(cv::Mat &im_src, Rect bloc);
	vector<int> partition(cv::Mat &im_src, cv::Rect bloc);
	void impainting(const cv::Mat& im_src, cv::Mat& mask, cv::Mat &im_dst);
};










