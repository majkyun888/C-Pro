#pragma once
#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
//#include <math.h>
#include <vector>
//#include <algorithm>
#include "Flange_inspection.h"

using namespace cv;

extern Camera_struct    camera   [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;

CFlange_inspection::CFlange_inspection( UINT ind, CFile_mngmt* file_manager_in )
{
	ASSERT( ind>=0 && ind<num_tlc );

	//mode = mode_in; //TIP_INSPECTION or CAP_INSPECTION
	//ASSERT( mode==NECK_ONLY || mode==CRIMPING_ONLY  );

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
  	input.roi_left   = area->buffrect.left;//125; //ËÑË÷Çø×ó¶Ë
	input.roi_right  = area->buffrect.right;//225; //ËÑË÷ÇøÓÒ¶Ë
	input.roi_top    = area->buffrect.top;
	input.roi_bottom = area->buffrect.bottom; 
	input.th_dist  = 30; //Æ¿Í·ËÑË÷ÃÅ¼÷
	input.th_conv_depth  = 2000;
	input.th_sym         = 30;
	input.learning_flange = false;
	output.dist = 0;

}

CFlange_inspection::~CFlange_inspection(void)
{
 /* if ( profile ) free( profile );
  profile = NULL;*/

  if ( area )
  {
    delete area;
    area = NULL;
  }
}

int CFlange_inspection::Set_inspection_area( int ind )
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

  TRACE("CFlange_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}

int CFlange_inspection::Add_menu_parameters( CContainer_wnd* child )
{
	
     pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_4100[lng], child );
    file_manager->Add_separator( string_4100[lng], area );

	pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


	pView->parameter[pView->num_params++] = new CParameter( &input.th_dist,         CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4104a[lng], string_unit[lng],   30,    0,  600, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &input.th_conv_depth ,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4104b[lng], string_unit[lng],   2000,    0,  10000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &input.th_sym ,         CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4104c[lng], string_unit[lng],   30,    0,  100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &input.learning_flange,   CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_4102a[lng], 0, 0, 0, 1, NULL );


 
  return( 0 );
}

int CFlange_inspection::Process_image( Mat img_in, UINT nimg)
{
	txt1 = L"-";
	txt2 = L"";
	good = FALSE;
	col1 = TXT_DARKRED;
	col2 = TXT_DARKRED;
			 
	//cv::Mat img_result;
	cv::Mat img_result(img_in.rows, img_in.cols, CV_8UC3) ;
	
	input.roi_left = area->buffrect.left;//125; //ËÑË÷Çø×ó¶Ë
	input.roi_right = area->buffrect.right;//225; //ËÑË÷ÇøÓÒ¶Ë
	input.roi_top    = area->buffrect.top;
	input.roi_bottom = area->buffrect.bottom; 
	int rtnValue;	
	CAlgo* pAlgo = GetFlange(&input, &output);
	if( pAlgo)
		rtnValue = pAlgo->process(img_in, img_result);
	delete(pAlgo);
	pAlgo = nullptr;
	//========
	if(input.learning_flange)
	{
		if ( rtnValue )
		{	
		
			if ( rtnValue == 1 ) txt1.Format(string_4110[lng]);
			if ( rtnValue == 2 ) txt1.Format(string_4112a[lng]);
			//if ( rtnValue == 8 ) txt1.Format(string_4113[lng]);
			if (rtnValue == 8)
			{
				txt1.Format(string_4113[lng]);
				txt2.Format(string_4109[lng], output.sym_perc1, output.sym_perc2, input.th_sym, output.num_conv_defect);
			}

			good = FALSE;
		}
		else 
		{  
			txt1.Format(string_4106a[lng]);
			col1 = TXT_DARKGREEN;
			good = TRUE;
		}
	  }
	  else
	  {
		if ( rtnValue )
		{
		  if ( rtnValue == 3 ) txt1.Format(string_4114a[lng]);
		  if ( rtnValue == 4 ) txt1.Format(string_4116a[lng]);
		  if ( rtnValue == 5 ) txt1.Format(string_4118a[lng]);
		  if ( rtnValue == 7 ) txt1.Format(string_4118a[lng]);
		  if ( rtnValue == 6 ) 
		  {
			txt1.Format(string_4108a[lng],output.dist, input.th_dist);
		    txt2.Format(string_4109[lng],output.sym_perc1,output.sym_perc2,input.th_sym, output.num_conv_defect );
		  }
		  good = FALSE;
		}
		else 
		{
		  	txt1.Format(string_4108a[lng],output.dist, input.th_dist );
		    txt2.Format(string_4109[lng],output.sym_perc1,output.sym_perc2,input.th_sym, output.num_conv_defect);
		 
			col1 = TXT_DARKGREEN;//
			col2 = TXT_DARKGREEN;
			good = TRUE;
		}
	  }
	
	//OUTMAT2DSP( index,img_result); 
	(image_dsp[index]->*(image_dsp[index]->outmat2dsp))(img_result);//Pour marked to be confirmed

	if( rtnValue ) 
		return 99;
	return 0;
} 