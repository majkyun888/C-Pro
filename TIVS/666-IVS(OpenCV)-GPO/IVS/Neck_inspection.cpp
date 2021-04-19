#pragma once
#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include "Neck_inspection.h"

using namespace cv;
#define YELLOW Scalar(0,255,255)
#define BLACK Scalar(0,0,0)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define RED Scalar(0,0,255)
#define PURPLE Scalar(240,32,160)
#define ORANGE Scalar(0,97,255)
#define BROWN  Scalar(42,42,128)
//CArea_adjust* areainsp=NULL; // inspection/search area

extern Camera_struct    camera   [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;

extern BOOL             learning;
extern BOOL             tipcap_tip;      // used only in combined tip/cap mode, when true = TIP mode / false = CAP mode

#define  xmin    area->buffrect.left  
#define  xmax    area->buffrect.right 
#define  ymin    area->buffrect.top   
#define  ymax    area->buffrect.bottom

extern int ATNLUT[DIMATN][DIMATN];  // array[dx][dy]



// #define AMPATN             6 // semilenght for atan calculation
#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DY pty[i+AMPATN]-pty[i-AMPATN]
#define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]


//  #undef PTD
#define PTD(i)  PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

//#define PTD2(i) PT(ptx2[ptn2]+ofsx[(i)&7],pty2[ptn2]+ofsy[(i)&7])


//#define ANGMARK1(a) (a< 9000?a+36000:a)
//#define ANGMARK2(a) (a>27000?a-36000:a)
//#define DIFANG(a1,a2) (a1-a2>18000?

/*
  #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],col_cross); // inner left end of sealing PAU
  #endif
  */

extern BOOL             datarec_on;
extern Results_manager* cap_result;


#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing
  //UINT __cdecl processing_thread  ( LPVOID pParam );
  extern CEvent* processing_start[2];
  extern CEvent* processing_done[2];
  extern BYTE*   img_ptr;
#endif

CNeck_inspection::CNeck_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in/*=TIP_INSPECTION*/ )
{
	ASSERT( ind>=0 && ind<num_tlc );

	mode = mode_in; //TIP_INSPECTION or CAP_INSPECTION
	ASSERT( mode==NECK_ONLY || mode==CRIMPING_ONLY  );

	file_manager = file_manager_in;

	index = ind;

	dimx = image_dsp[index]->dim_buf.cx;
	dimy = image_dsp[index]->dim_buf.cy;
	dim  = dimx * dimy;

  //area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
  //area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 );
	ASSERT( dimy>0 );

	area = new CArea_adjust( index, file_manager );
	area->buffrect.top    =   10; //   dimy/6;
	area->buffrect.bottom = 570; // 5*dimy/6;
	area->buffrect.left   = 125;//276; //   dimx/6;
	area->buffrect.right  = 225;//644; // 5*dimx/6;

  
	// params about key points postitioning
  	input.roi_left = area->buffrect.left;//125; //搜索区左端
	input.roi_right = area->buffrect.right;//225; //搜索区右端
	input.head_border = 15; //瓶头搜索门槛
	input.radius_offset = 50; //瓶颈半径缩进
	input.min_points = 18; //搜索到的最少点数
	// params about bloc configure
	input.neck_width = 54; //瓶脖分块宽度
	input.bloc_height = 3;//瓶脖分块高度
	input.crimp_width = 18; //翻边检测宽度
	// params about absolute level in neck area
	input.low_limit = 20;// 绝对灰度低限
	input.high_limit = 240;//绝对灰度高限
	input.abs_num_th = 10;//绝对值数量门槛
	// params about relative difference in neck area
	input.dist = 3;//相对误差距离
	input.diff_th = 25;//相对灰度门槛
    input.rlt_num_th = 80;//相对误差数量门槛
	// params about relative difference in crimp area 
	input.dist_crimp = 3;
	input.diff_th_crimp = 45;
	input.rlt_num_th_crimp = 50;

}


CNeck_inspection::~CNeck_inspection(void)
{
 /* if ( profile ) free( profile );
  profile = NULL;*/

  if ( area )
  {
    delete area;
    area = NULL;
  }
}

int CNeck_inspection::Set_inspection_area( int ind )
{
//  ASSERT( (UINT)ind>=0 && (UINT)ind<num_tlc );
//  if ( area && area->m_hWnd ) area->ShowWindow( SW_SHOW );

  if ( CHK_BIT( thread_status, 10+((CArea_adjust*)ind)->index ) )
  {
    Message_box( string_2022[lng], MBOX_INFO );
    return( 0 );
  }

  CArea_adjust* area = (CArea_adjust*) ind; // inspection/search area

  ASSERT( area->index>=0 && area->index<num_tlc ); 
  if ( area && area->m_hWnd && image_dsp[area->index]->visible ) area->ShowWindow( SW_SHOW );

  TRACE("CBottom_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CNeck_inspection::Process_image( Mat img_acq, UINT ind)
{
	txt1 = L"-";
	txt2 = L"-";
	good = FALSE;
	col1 = TXT_DARKRED;
	col2 = TXT_DARKRED;
			 
	//Rlt_Neck  output;
	cv::Mat ImgResult;
	if ( input.roi_left<2 || input.roi_right > img_acq.rows-2) //xcnt2+anl_radius>int(dimx-2) || ycnt2-anl_radius<2 || ycnt2+anl_radius>int(dimy-2) ) 
    {
    //txt1.Append( L" - " );
    txt1.Append( string_2742[lng] );
    col1 = TXT_DARKRED;
    return 789;
    }
	input.roi_left = area->buffrect.left;//125; //搜索区左端
	input.roi_right = area->buffrect.right;//225; //搜索区右端
			
	//NeckProcessor* NeckProcessor1 = new NeckProcessor(&input,&output);

	int Good = neck(img_acq,ImgResult);
	if ( Good )
	{	
		txt1.Format( string_3940[lng] );
		col1 = TXT_DARKRED;
		good = FALSE;
		txt2.Format(string_3962[lng], output.abs_high_err, output.abs_low_err,input.abs_num_th);
		col2 = TXT_DARKRED;//_thrv[0], bv[0], lv[0], tv[0]);
		txt3.Format(string_3964[lng], output.rlt_err, input.rlt_num_th);
		col2 = TXT_DARKRED;//_thrv[0], bv[0], lv[0], tv[0]);
	}
	else
	{
		txt1.Format( string_3940[lng] );
		col1 = TXT_DARKGREEN;
		good = TRUE;
		txt2.Format(string_3962[lng], output.abs_high_err, output.abs_low_err,input.abs_num_th);
		col2 = TXT_DARKGREEN;//_thrv[0], bv[0], lv[0], tv[0]);
		txt3.Format(string_3964[lng], output.rlt_err, input.rlt_num_th);
		col2 = TXT_DARKGREEN;//_thrv[0], bv[0], lv[0], tv[0]);

	}
	//for( int i = 0; i< dimx; i++)
	//{
	//	for( int j = 0; j< dimy; j++)
	//	{
	//		 
	//		*(image_dsp[ind]->ptr_dsp+j*dimx+i) = 0xFF000000+ (*(ImgResult.data + ImgResult.step[0]* i + ImgResult.step[1]*j)) + 
	//		+(*(ImgResult.data + ImgResult.step[0]* i + ImgResult.step[1]*j+1)<<8) +(*(ImgResult.data + ImgResult.step[0]* i + ImgResult.step[1]*j+2)<<16);
	//	}
	//}
	UINT multi = int( 0.5 + 1024.0 / camera[ind].scale );
	for (int x=0; x<image_dsp[ind]->dim_dsp.cx; x++)
		for (int y=0; y<image_dsp[ind]->dim_dsp.cy; y++)  
		{
	//      int ptr = 3*( dim_buf.cx-1-((y*multi)>>10) ) + 3*( dim_buf.cy-1-((x*multi)>>10) )*dim_buf.cx; // offset of first byte
	//       ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ ((y*multi)>>10) + ((x*multi)>>10)*dim_buf.cx ]];

		  int ptr = 3*( (y*multi)>>10 ) + 3*( (x*multi)>>10 )*image_dsp[ind]->dim_buf.cx; // offset of first byte
		  image_dsp[ind]->ptr_dsp[x+y*image_dsp[ind]->dim_dsp.cx] = 0xFF000000 | *(ImgResult.data+ptr) | *(ImgResult.data+ptr+1)<<8 | *(ImgResult.data+ptr+2)<<16;
		}
	/*BYTE* img_out = ImgResult.data;
	SCALE_IMG( ind, img_out );*/
	
	if( Good ) 
		return 99;
	return 0;
} 



int CNeck_inspection::vector2Mat(vector< vector<int> > src, Mat &dst)
{
    Mat temp(src.size(),src.at(0).size(),CV_8U);
    for(int i=0; i<temp.rows; ++i)
         for(int j=0; j<temp.cols; ++j)
		 {
			 if( src.at(i).at(j) > 255 || src.at(i).at(j) < 0)
				 return -1;
             temp.at<uchar>(i, j) = src.at(i).at(j);
		 }
    temp.copyTo(dst);
	return 0;
}


int CNeck_inspection::detect_diff_dist( const Mat &srcImage, Mat &dstImage,  int direction, int dist, int diff_th ) 
{
	Mat img; 	
	int num=0;
	img = srcImage;
	Rect roi(0,0,srcImage.cols,srcImage.rows);
	//Sobel( img, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	if( direction == 1) // y direction
	{
		for ( int j = dist; j< img.rows-dist; j++)
		{
			for ( int i = 0; i< img.cols; i++)
			{
				//由于是在y方向，不是在一行中寻址，因此用line寻址方式容易出错，所以废弃不用了		
				// 用三点平均值代替单点可以避免波动影响, 输入已经平滑过了，就不再三个值平均了
				float avr = img.at<uchar>(j,i);//(img.at<uchar>(j,i)+ img.at<uchar>(j-1,i)+ img.at<uchar>(j+1,i))/3.0;
				float avr1 = img.at<uchar>(j-dist,i);//(img.at<uchar>(j-length,i)+ img.at<uchar>(j-length-1,i)+ img.at<uchar>(j-length+1,i))/3.0;
				float avr2 = img.at<uchar>(j+dist,i);//(img.at<uchar>(j+length,i)+ img.at<uchar>(j+length-1,i)+ img.at<uchar>(j+length+1,i))/3.0;
				if(abs( avr - avr1) > diff_th || abs(avr - avr2)> diff_th)//img.at<uchar>(j,i)- img.at<uchar>(j+length,i)
				{	num++;
					int x = i + roi.x;
					int y = j + roi.y;
					*(dstImage.data + y* dstImage.step + x) = 255;
				}
			}
		}
	}
	else if ( direction == 0) // x direction
	{
		for ( int j = dist; j< img.cols-dist; j++)
		{
			for ( int i = 0; i< img.rows; i++)
			{
				
				// 用三点平均值代替单点可以避免波动影响
				float avr = img.at<uchar>(i,j);//(img.at<uchar>(j,i)+ img.at<uchar>(j-1,i)+ img.at<uchar>(j+1,i))/3.0;
				float avr1 = img.at<uchar>(i,j-dist);//(img.at<uchar>(j-length,i)+ img.at<uchar>(j-length-1,i)+ img.at<uchar>(j-length+1,i))/3.0;
				float avr2 = img.at<uchar>(i,j+dist);//(img.at<uchar>(j+length,i)+ img.at<uchar>(j+length-1,i)+ img.at<uchar>(j+length+1,i))/3.0;
				if(abs( avr - avr1) > diff_th || abs(avr - avr2)> diff_th)//img.at<uchar>(j,i)- img.at<uchar>(j+length,i)
				{	num++;
					int x = j + roi.x;
					int y = i + roi.y;
					*(dstImage.data + y* dstImage.step + x) = 255;
				}
			}
		}
	}
	return num;
}

vector<int> CNeck_inspection::partition(cv::Mat &im_src, cv::Rect bloc)
{
	int w, h;
	w = bloc.width;
	h = bloc.height;
	cv::Mat avg;
	reduce(im_src(bloc), avg, 0, CV_REDUCE_AVG);
	vector<int> line;
	line.clear();
	for (int i = 0; i < w / h; i++)
	{
		int sum = 0;

		for (int j = 0; j < h; j++)
		{
			sum = sum + *(avg.data + i*h + j);
		}
		int t = sum / h;
		//cout<< t<<", ";
		line.push_back(t);
	}
	return line;
}

void CNeck_inspection::impainting(const cv::Mat& im_src, cv::Mat& mask, cv::Mat &im_dst)
{
	
	threshold(im_src, mask, 220, 255, THRESH_BINARY);

	im_src.copyTo(im_dst);
	for (int row = 0; row < im_src.rows; row++)
	{

		uchar* pline = im_dst.data + im_dst.step[0] * row;
		//int border_flag =0;
		for (int col = 0; col < im_src.cols - 1; col++)
		{
			int i = 0, sum = 0;
			//if( *(border_known.data + border_known.step * row + col) && (!*(border_known.data + border_known.step * row + col+1)) ) 
			//	border_flag=1;
			//if( (!*(border_known.data + border_known.step * row + col)) && *(border_known.data + border_known.step * row + col+1) ) 
			//	border_flag=0;
			if (*(mask.data + mask.step * row + col))
			{
				//for( int j = -2; j< 3; j++)
				//{	uchar* p1= im_dst.data + im_dst.step[0]* row + im_dst.step[0]*j;
				for (i = col - 1; i< col; i++)
					sum = sum + pline[i];
				//}
				pline[i] = (sum) / 1;
			}
		}
	}
	

}

int CNeck_inspection::neck( const Mat &im_src, Mat &im_result)
{
		
	cvtColor(im_src,im_result,CV_GRAY2RGB);//灰度转换
	
	Rect roi; // region of interest
	Mat temp, im_src1; // temp Mat

	// preprocessing
	threshold(im_src, im_src1, 220, 255,  THRESH_TRUNC); // truncate the saturation area
	roi = Rect(input.roi_left,0, input.roi_right- input.roi_left, im_src.rows); //125,100
	Mat im_roi= im_src1(roi).clone();
	
	// process the saturation area by impainting 
	//Mat im_mask = Mat(im_roi0.cols, im_roi0.rows, CV_8UC1);
	//Mat im_roi  = Mat(im_roi0.cols, im_roi0.rows, CV_8UC1);
	//impainting(im_roi0, im_mask, im_roi);

	// grad x search	
    Mat grad_x,abs_grad_x;
	Sobel(im_roi, grad_x, CV_16S, 1, 0, 3, 1, 1,BORDER_DEFAULT);
	cv::threshold(grad_x,temp,0,255,THRESH_TOZERO_INV );// eliminate the points whose brightness left < right
    convertScaleAbs(temp, abs_grad_x); // absolute value
	Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3)); 
	morphologyEx(abs_grad_x,abs_grad_x,CV_MOP_OPEN,kernel);// remove the isolated points
	
    int h_roi = im_roi.rows, y;
	int w_roi = im_roi.cols, x;
	
	Mat mat,mat1,mat2,mat3;
	// direct x search 
	reduce(abs_grad_x, mat, 1,CV_REDUCE_MAX); // find max value for each line
	reduce(im_roi, mat1, 1, CV_REDUCE_MAX);
	reduce(im_roi, mat2, 1, CV_REDUCE_MIN);
	mat3 = mat1 - mat2;
	
	// search the borders (y coordinate) of head 
	int up = 0, down = im_roi.rows-1,center = h_roi/2;
	for( int i = 0; i< h_roi/2; i++) 
	{
		if( *(mat3.data+ i*mat3.step)> input.head_border && *(mat3.data+ i*mat3.step+1)>input.head_border 
			&& *(mat3.data+ i*mat3.step+2)>input.head_border)// 
		{	up = i+input.radius_offset; //45;
			break;
		}
	}
	for( int j = im_roi.rows-1; j > h_roi/2; j--)
	{
		if( *(mat3.data+ j*mat3.step) >input.head_border && *(mat3.data+ j*mat3.step-1)>input.head_border 
			&& *(mat3.data+ j*mat3.step-2)>input.head_border )//
		{down = j-input.radius_offset; //45;
		 break;
		}
	}
	center = (up+down)/2;
	line(im_result, Point(input.roi_left,up),Point(input.roi_right,up),Scalar(0,255,0),1,8,0);
	line(im_result, Point(input.roi_left,down),Point(input.roi_right,down),Scalar(0,255,0),1,8,0);

	// search the edge points of Alu and glass  
	int step = 10;
	int lastX = 0;
	vector<Point> up_half, dn_half; 
	// search from center to bottom, assure x is min at center and increases 
	for(  y = center+step; y < down; y= y+step)
	{
		int nthres;
		if(*(mat.data+y) > 128) 
			nthres = *(mat.data + y) /3;// big change gives low threshold  
		else if(*(mat.data + y) > 78)
		    nthres = *(mat.data + y) /2;// 
		else
			nthres = *(mat.data + y)*2/3;// low change gives high threshold
		for ( x = w_roi-1; x > 10; x--)
		{		
			if( *(x+ y* abs_grad_x.step + abs_grad_x.data)>= nthres && x >= lastX  ) 
			{	dn_half.push_back(Point(x,y));
				lastX = x;
				break;
			}
		}
		
	    if( abs( x - (w_roi-1)) < 10) break;
	}
	lastX = 0;
	// search from center to top, assure x is min at center and increases 
	for(  y = center; y > up; y= y-step)
	{	
		//if (y == 131)
		//	y = 131;
		int nthres;
		if (*(mat.data + y) > 128)
			nthres = *(mat.data + y) / 3;// big change gives low threshold to 
		else if (*(mat.data + y) > 78)
			nthres = *(mat.data + y) / 2;// 
		else
			nthres = *(mat.data + y) * 2 / 3;// 
		for ( x = w_roi-1; x > 10; x--)
		{		
			if( *(x+ y* abs_grad_x.step + abs_grad_x.data)>= nthres && x >= lastX )
			{	up_half.push_back(Point(x,y));
				lastX = x;
				break;}
		}
		
	    if( abs( x - (w_roi-1)) < 10) break;
	}
	reverse(up_half.begin(),up_half.end());
	up_half.insert(up_half.end(), dn_half.begin(), dn_half.end());
	for( int i =0; i< up_half.size(); i++)
		circle(im_result,Point(up_half[i].x+roi.x,up_half[i].y+roi.y),2,BLUE,-1);
	
	// reject the inormal vector
	if(up_half.size() < input.min_points) 
		return 1;  
	for( int k =0; k<= up_half.size()-2;k++)
	{
	   /*if( up_half[k+1].y - up_half[k].y > 3* step) 
	   {
		   cout<< " point  "<<k<< " y bad"<<endl;
		   return 2;
	   }*/
	   if( abs(up_half[k+1].x - up_half[k].x) > 15)
	   {
		   //cout<< " point "<<k<< " x bad"<<endl;
		   return 3;
	   }
	}

	vector<Point> keyPts,blocPts;
	vector<vector<int> > neck_avg, crimp_avg;
	vector<int> line;
	int h,w;//h- height of block,w- width of block
	 w = input.neck_width; //55 ; 
	 h = input.bloc_height; //5;
	int lastDX =1000;

	// keyPts ---- eliminate the inormal pts, eg. 
	for( int i = 0; i < up_half.size()-1; ++i)
	{
	    int x0,y0,x1,y1;
		x0 = up_half[i].x;
		y0 = up_half[i].y; 
		x1 = up_half[i+1].x; 
		y1 = up_half[i+1].y;
		if(abs((x0-x1)- lastDX) < 3 || (x0-x1)<=lastDX  ) 
		{	
			keyPts.push_back(Point(x0,y0));
		    lastDX = x0-x1;
		}
		else 
		{
			x0 = up_half[i-1].x;
			y0 = up_half[i-1].y;
			lastDX = x0-x1;		
		}

		if( i == up_half.size()-2) 
			keyPts.push_back(Point(x1,y1));
	}
	
	// keyPts -> blocPts, interpolate the pts to get the bloc origins
	for( int k = 0; k< keyPts.size()-1; k++)
	{
		int y1= keyPts[k+1].y;
		int y0= keyPts[k].y;
		int x1= keyPts[k+1].x;
		int x0= keyPts[k].x;
	   	if( y1-y0 > h) 
		{
			for ( int j = 0; j< (y1-y0)/h; j++)
			{
				int tx = (x1-x0)* j* h /(y1-y0) + x0;
				int ty =  y0 + j* h;
				blocPts.push_back(Point(tx,ty));
			}
		}
		else 
			blocPts.push_back(Point(x0,y0));
		if( k == keyPts.size()-2) 
			blocPts.push_back(Point(x1,y1));
	}

	// get the avg values of each bloc
	vector<Point> neck_table, crimp_table;
	for( int k = 0; k< blocPts.size()-1; k++)
	{
		Point ulPt; // up-left point of block
		if ( k == 55) 
			k =55;
		if( blocPts[k].x >= blocPts[k+1].x) 
		{
			ulPt.x = blocPts[k].x;
			ulPt.y = blocPts[k].y;
		}
		else  
		{	
			ulPt.x = blocPts[k+1].x;
			ulPt.y = blocPts[k].y;
		}
		
		// get the neck data
		neck_table.push_back(ulPt);
		Rect bloc = Rect(ulPt.x + roi.x, ulPt.y + roi.y, input.neck_width, input.bloc_height);
		if(k== blocPts.size() - 2)
			rectangle(im_result, Point(bloc.x, bloc.y), Point(bloc.x + w, bloc.y + h), YELLOW);
		line.clear();
		line = partition(im_src1, bloc);
		neck_avg.push_back(line);
		
		// get the crimp data
		crimp_table.push_back(Point(ulPt.x - 2 - input.crimp_width, ulPt.y));
		Rect grid = Rect(ulPt.x + roi.x-2-input.crimp_width, ulPt.y + roi.y, input.crimp_width, input.bloc_height);
		if (k==0)
		rectangle(im_result, Point(grid.x, grid.y), Point(grid.x + input.crimp_width, grid.y + input.bloc_height),GREEN);
		line.clear();
		line = partition(im_src1, grid);
		crimp_avg.push_back(line);		
	}

	// 2D vector -> Mat
	Mat im_neck,im_crimp;
	Mat brt_abs, dark_abs,brt_diff, dark_diff;
	if ( vector2Mat( neck_avg, im_neck)) 
		return -1;
	if (vector2Mat(crimp_avg, im_crimp))
		return -1;
	char imname1[60],imname2[60];
	//sprintf( imname1,"d:\\IMAGES\\yuxi_pingbo\\w100-b\\im_neck_%d.png",num);
	//imwrite(imname1,im_neck);
	//return 0;
	
	//-----------------------------
	// rejections 
    // absolute values reject
	cv::threshold(im_neck, brt_abs,input.high_limit ,255,THRESH_BINARY); //240
    output.abs_high_err  = countNonZero(brt_abs);
	tintPixel(im_result, brt_abs, PURPLE, neck_table);
	cv::threshold(im_neck, dark_abs,input.low_limit  ,255,THRESH_BINARY_INV);//60
	output.abs_low_err = countNonZero(dark_abs);// 
	tintPixel(im_result, dark_abs, PURPLE, neck_table);
	if (output.abs_high_err + output.abs_low_err > input.abs_num_th) 
		return 4; 

	// relatave difference with distance reject 
	Mat im_err_neck = Mat::zeros(im_neck.rows, im_neck.cols, CV_8UC1);
	Mat im_err_crimp = Mat::zeros(im_crimp.rows, im_crimp.cols, CV_8UC1);

	output.rlt_err = detect_diff_dist(im_neck, im_err_neck, 1, input.dist, input.diff_th);
	
		tintPixel(im_result, im_err_neck, RED, neck_table);
	
	
	if (output.rlt_err > input.rlt_num_th)
		return 5 ;
	

	output.rlt_err_crimp = detect_diff_dist(im_crimp, im_err_crimp, 1, input.dist_crimp, input.diff_th_crimp);
	tintPixel(im_result, im_err_crimp, BROWN, crimp_table);
	if (output.rlt_err_crimp > input.rlt_num_th_crimp)
		return 6;
	return 0;
}
 
int CNeck_inspection::Add_menu_parameters( CContainer_wnd* child )
{

	//wchar_t** name = string_3940; // standard 
	pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3940[lng], child );
	file_manager->Add_separator( string_3940[lng],area );

	pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


	 //pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2910[lng], child );
  //file_manager->Add_separator( string_2910[lng], area );
  //pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  // Contour & inspection area
  /*pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3940[lng], child );
  file_manager->Add_separator( string_3940[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );*/

  pView->parameter[pView->num_params++] = new CParameter( &input.head_border,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3942[lng], string_log [lng],   15,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.radius_offset,CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3944[lng], string_pix[lng],   40,    0,  200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.crimp_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3966[lng], string_pix [lng],  18,    2,   35, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.neck_width,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3946[lng], string_pix[lng],  54,    5,  150, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.bloc_height,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3948[lng], string_pix [lng],  3,    2,   20, file_manager );
  //pView->parameter[pView->num_params++] = new CParameter( &input.bloc_height,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3968[lng], string_pix [lng],  18,    5,   150, file_manager );
  pView->num_menu_btn++;
  pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index ); 
  
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3970[lng], child );
  file_manager->Add_separator( string_3970[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &input.low_limit,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3950[lng], string_log [lng],  20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.high_limit,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child,string_3952[lng], string_log [lng],  240,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.abs_num_th,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child,string_3954[lng], string_pix [lng],  10,    0,   1000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.rlt_num_th,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3956[lng], string_pix [lng],  8,    0,   1000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.diff_th,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3958[lng], string_log [lng],  25,    0,   255, file_manager ); 
  pView->parameter[pView->num_params++] = new CParameter( &input.dist,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3960[lng], string_pix [lng],  3,    0,   10, file_manager );
  
  pView->num_menu_btn++;
   pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3972[lng], child );
  file_manager->Add_separator( string_3972[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );
  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &input.rlt_num_th_crimp,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3956[lng], string_pix [lng],  8,    0,   1000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.diff_th_crimp,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3958[lng], string_log [lng],  25,    0,   255, file_manager ); 
  pView->parameter[pView->num_params++] = new CParameter( &input.dist_crimp,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3960[lng], string_pix [lng],  3,    0,   10, file_manager );


  //pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );  
  
  TRACE("CNeck_inspection::Add_menu_parameters(%d)\n", index);
  return( 0 );
}

int CNeck_inspection::tintPixel(Mat& des, Mat& mask, Scalar &color, const vector<Point> origin)
{
	int cnt = 0;
	for (int i = 0; i< mask.cols; i++)
	{
		for (int j = 0; j < mask.rows; j++)
		{
			int x = i * input.bloc_height + origin[j].x+ input.roi_left;
			int y = origin[j].y;

			int t = *(mask.data + j*mask.step + i);
			if (t >= 200)
			{
				rectangle(des, Rect(x, y, input.bloc_height, input.bloc_height), color, -1, 8, 0);
				cnt++;
			}
		}
	}
	return cnt;
}



