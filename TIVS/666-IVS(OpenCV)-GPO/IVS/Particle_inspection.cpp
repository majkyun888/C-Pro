#pragma once



//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Ampoule_filling_level.h"
//#include "ChildView.h"

//Pour added 20180601
#include"Particle_inspection.h"

//pour added for interface tests on 20190428 foropencv
#include <opencv2\opencv.hpp>
#include "../MiddlewareImprocessDll/middlewareParticleDi.h"
using namespace std;

// 20190506 foropencv
#ifdef _DEBUG
#pragma comment(lib,"../ivs/debug/MiddlewareImprocessDll.lib")
#else
#pragma comment(lib,"../ivs/release/MiddlewareImprocessDll.lib")
#endif

//pour added for interface tests on 20190428

//CArea_adjust* areainsp=NULL; // inspection/search area

extern Camera_struct    camera  [NUM_TLC];
//extern CArea_adjust*    areainsp[NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;

extern BOOL             stn1_part_back;     // In case of selectable TRUE/FALSE BACK/BOTTOM LIGHT particle station 1 
//Pour added on 20190513
//extern INT      imagesavemode;
//Pour added on 20190513
//#define NCRIT_BLOB 4


extern UINT             thread_status;
extern CLogger*         logger;

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif


//blob_score_type blob_score = { 1 , 1 };
/*
struct blob_score_type
{
  int  size;     // min size of blob to get error score (must be bigger/equal than)
  int  score;    // score added if blob bigger than size
};
*/
                                         // SIZE - SCORE
/*
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,
                                            {   6 ,   6 } ,  // 6,4
                                            {  10 ,  10 } ,
                                            {  30 ,  40 } }; // 30,50
*/

/* RESO MEMBRO DI OGNI OGGETTO (NON STATICO) 20141022
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,   // size 1 => 0 points 
                                            {   4 ,   4 } ,   // size 4 => 1 points
                                            {   8 ,  15 } ,
                                            {  25 ,  50 } };  
                                            */


/* HOW SCORES ARE CALCULATED
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,
                                            {   4 ,   4 } ,
                                            {   8 ,  15 } ,
                                            {  25 ,  50 } };  

  0 =>  0
  1 =>  1
  2 =>  1
  3 =>  1
  4 =>  4
  5 =>  4
  6 =>  4
  7 =>  4
  8 => 15
  9 => 15
 16 => 15
 24 => 15
 25 => 50
100 => 50

*/



#define PT(x,y) img_acq[x+(y)*camera[index].width]
#define FLT(x,y) PT(x,y-6)+PT(x,y+6)-3*PT(x,y)

//extern BOOL      learn_reflections;
extern BOOL      learning;



// if mode_in is added to 1/2/3 csv file is loaded accordingly containing new blob weights

CParticle_inspection::CParticle_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in/*=NULL*/ )
{

	// 20190506
	p_middleware_particle_di = NULL;

  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  mode = mode_in; // PARTICLE_MODE_REFLECTION; // default mode

  ASSERT( mode&PARTICLE_MODE_REFLECTION || mode&PARTICLE_MODE_ADSORPTION || mode&PARTICLE_MODE_BOTH );  // Check AT LEAST one mode selected
//  ASSERT( !( (mode&PARTICLE_MODE_REFLECTION) && (mode&PARTICLE_MODE_ADSORPTION) ) ); // Check NO MORE than one mode selected

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;
/*
  //Pour added 20180530
#ifdef COLOR_DETECTION_P1
  if(index == 3 || index == 4)
  {
  dim_c  = dimx * dimy * 3;// dim in bytes (x3) !
  }
#endif
  //Pour added 20180530
  */
  area_color = COL_BLUE; // 0xFFRRGGBB assigned color to draw inspection area
  //area_color = COL_RED;
	ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =   dimy/3;
  area->buffrect.bottom = 2*dimy/3;
  area->buffrect.left   =   dimx/3;
  area->buffrect.right  = 2*dimx/3;

  // init inspection parameters
//  learning = FALSE;
  num_img =   20;    // number of images for inspection
  min_dif =   20;    // minimum brightness difference during analysis
  max_err = 1000;    // maximum allowed particle error 

  baseimg = (BYTE*) _aligned_malloc( dim, 16 ); 
  ASSERT( baseimg && AfxIsValidAddress( baseimg, dim ) );
  ZeroMemory( baseimg, dim );

  reflections = (BYTE*) _aligned_malloc( dim , 16 ); 
  ASSERT( reflections && AfxIsValidAddress( reflections, dim ) );
  ZeroMemory( reflections, dim );

  output = (BYTE*) _aligned_malloc( dim , 16 ); 
  ASSERT( output && AfxIsValidAddress( output, dim ) );
  ZeroMemory( output, dim );

  minmax = (BYTE*) _aligned_malloc( dim , 16 ); 
  ASSERT( minmax && AfxIsValidAddress( minmax, dim ) );
  ZeroMemory( minmax, dim );

  sensitdata = (BYTE*) _aligned_malloc( 16 , 16 ); 
  ASSERT( sensitdata && AfxIsValidAddress( sensitdata, 16 ) );
  ASSERT( (UINT(sensitdata)&15)==0);
  FillMemory( sensitdata, 16, min_dif );
/*  TRACE("Sensitdata: ");
  for ( int i=0 ; i<16 ; i++ ) TRACE("%d-",sensitdata[i]);
  TRACE("\n"); */

	img_gray = (BYTE*) _aligned_malloc( dimx*dimy , 16 ); 
	ASSERT( img_gray && AfxIsValidAddress(img_gray,dimx*dimy) );
	ZeroMemory( img_gray, dimx*dimy );

  ZeroMemory( LUTBLOB, sizeof(LUTBLOB) );

  int i;

  if ( mode & PARTICLE_MODE_REFLECTION ) // 0=>0 - 40=>10 - 150=>40 ca
  {
    for ( i=0 ; i<256 ; i++ )
    {
      //LUTBLOB[i] = int( i * 0.25 );
      //LUTBLOB[i] = int( i * 0.30 );
      LUTBLOB[i] = int( i * 0.10 );
      //TRACE("%3d,%3d\n",i,LUTBLOB[i]);
    }

    ZeroMemory( LUTBLOB, sizeof(LUTBLOB) );
  }

  
  if ( mode & PARTICLE_MODE_ADSORPTION ) // 54=>7 ca / fondo attuale ca 110=>0 - punti pi?scuri ca 30=>10
  {
    int ax = 110;
    int ay =   0;
    int bx =  30;
    int by =  10;

    for ( i=0 ; i<256 ; i++ )
    {

      if ( i>=ax ) LUTBLOB[i] = ay;
      else
      {
        if ( i<=bx ) LUTBLOB[i] = by;
        else LUTBLOB[i] = int( 0.5 + double(by)*(ax-i)/(ax-bx) );
      }

      //TRACE("%3d,%3d\n",i,LUTBLOB[i]);
    }
    //TRACE("LUTBLOB calculated\n");

    ZeroMemory( LUTBLOB, sizeof(LUTBLOB) );
  }

/*
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,   // size 1 => 0 points 
                                            {   4 ,   4 } ,   // size 4 => 1 points
                                            {   8 ,  15 } ,
                                            {  25 ,  50 } };  
                                            */
  // default blob scores   1=>0   2,3,4=>1   5,6,7,8=>4
  blob_score[0].size  =   1;
  blob_score[0].score =   1;
  blob_score[1].size  =   4;
  blob_score[1].score =   4;
  blob_score[2].size  =   8;
  blob_score[2].score =  15;
  blob_score[3].size  =  25;
  blob_score[3].score =  50;

//  #if SERIAL_N==201300501 // "2013-005V" - first ampoule prototype - LIONCO HAINAN HAIKOU
  #if SERIAL_N==201400029 // "2013-005V" - new ampoule prototype - LIONCO HAINAN HAIKOU
    blob_score[0].size =   3;    // from  1 to  3 =>  0 points 
    blob_score[1].size =   6;    // from  4 to  6 =>  1 points 
    blob_score[2].size =   9;    // from  7 to  9 =>  4 points
    blob_score[3].size =  18;    // from 10 to 18 => 15 points - bigger than 18 => 50 points
  #endif

  int stn = mode & 3; // stn extract only station n
  //Pour removed on 20201010
  /*
  if ( stn>=1 && stn<=3 ) 
  {
    //TRACE("Creating particle %d/%d\n", stn, ind );

    CString file_name;
    file_name.Format(L"%s\\blob_p%d.csv",MAIN_DIRECTORY,stn);

    ASSERT( sizeof(blob_score)==NCRIT_BLOB*sizeof(int)*2 );

    CString txt;
    wchar_t txt2[200+1];
    if ( !Read_csv( file_name, 4, 2, (int*)blob_score, txt2 ) ) // 4 rows / 2 columns
    {
      //TRACE(L"BLOB TABLE FOR %d/%d SUCCESSFULLY READ FROM FILE <%s>\n", stn,ind,file_name);
      txt.Format(L"BLOB TABLE FOR %d/%d READ FROM FILE <%s> NEW VALUES: (%d,%d)(%d,%d)(%d,%d)(%d,%d)\n", stn,ind,file_name,
        blob_score[0].size,blob_score[0].score , 
        blob_score[1].size,blob_score[1].score , 
        blob_score[2].size,blob_score[2].score , 
        blob_score[3].size,blob_score[3].score ); 
    }
    else
    {
      txt.Format(L"UNABLE TO READ BLOB TABLE FOR STN%d/CAM[%d] FROM FILE <%s> <%s>\n", stn,ind,file_name,txt2); 
    }

    if ( logger ) logger->Add_string( txt.GetBuffer(0) );
    //TRACE("Completed\n");

  }
  */
   //Pour removed on 20201010

  #ifdef NEW_PARTICLE
    ZeroMemory(images,sizeof(images));
    path_name  = NULL;
    
    for ( i=0 ; i<NIMAGES ; i++ )
    {
      if ( images[i]==0 ) images[i] = (BYTE*) _aligned_malloc( dim, 16 ); 
      ASSERT( images[i] && AfxIsValidAddress( images[i], dim ) );
    }

    min_dif    =    3;    // minimum brightness difference during analysis
    if ( mode & PARTICLE_MODE_ADSORPTION ) min_dif = 10; // higher for backlight

    threshold  = min_dif;
    min_dim    =    0;
    max_dim    =   50;
    min_depth  =    3;
    max_dist   =   30;
    max_dist2  = max_dist*max_dist;

    num_img    =   20;    // number of images for inspection
    max_err    =   20;    // maximum allowed particle error 


    area->buffrect.top    =   51;
    area->buffrect.bottom =  312;
    area->buffrect.left   =  126;
    area->buffrect.right  =  750;


    thr_old1              =    0; 
    thr_old2              =    0;  

    mintotnpt             =  100;
    minnptimg             =   10;
    minnumimg             =    3;

	//Pour added for color detection 20180604
	max_dist_lab          = 10;
	Lab_refl              = 10;
	Lab_refa              = 10;
	Lab_refb              = 10;


    if ( mode & PARTICLE_MODE_ADSORPTION ) // backlight
    {
      sigma               =  1.0;
      threshold1          =   15;
      threshold2          =   15;
    }
    else                                   // bottomlight
    {
      sigma               =  0.5;
      threshold1          =   10;
      threshold2          =   90;
    }

    Create_GF();
    Prepare_thr();

  #endif // NEW_PARTICLE

#ifdef OPENCV_PARTICLE
	insp_savemode = 0;
	nozero_single_thr  = 1;
	nozero_frm_sum_thr = 2;
	ratio = 0.9;
	sensitivity_part = 1;
	cstr_camId = 1;
#endif // OPENCV_PARTICLE

}




CParticle_inspection::~CParticle_inspection(void)
{

  if ( area )
  {
    delete area;
    area = NULL;
  }

  if ( baseimg )
  {
    _aligned_free( baseimg );
    baseimg = NULL;
  }

  if ( reflections )
  {
    _aligned_free( reflections );
    reflections = NULL;
  }

  if ( output )
  {
    _aligned_free( output );
    output = NULL;
  }

  if ( minmax )
  {
    _aligned_free( minmax );
    minmax = NULL;
  }

  if ( sensitdata )
  {
    _aligned_free( sensitdata );
    sensitdata = NULL;
  }

  #ifdef NEW_PARTICLE
    for ( int i=0 ; i<NIMAGES ; i++ )
    {
      if ( images[i] ) _aligned_free( images[i] );
      images[i] = 0;
    }

  #endif // NEW_PARTICLE

	  if(img_gray)
	  {
	  	_aligned_free(img_gray);
		img_gray = NULL;
	  }

}


int CParticle_inspection::Set_inspection_area( int ind )
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

  TRACE("CParticle_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CParticle_inspection::Add_menu_parameters( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2072[lng], child );
  file_manager->Add_separator( string_2072[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

	//pView->parameter[pView->num_params++] = new CParameter( &learning,    CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2052[lng], 0,  0,  0, 1, NULL, &Learn_reflections );

  #ifdef NEW_PARTICLE
#ifndef OPENCV_PARTICLE//Pour marked on 20190514
    pView->parameter[pView->num_params++] = new CParameter( &num_img,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit[lng],   20,    2,  NIMAGES, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &threshold1, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2950[lng], string_unit[lng],   10,    0,      255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &threshold2, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2952[lng], string_unit[lng],   10,    0,      255, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &min_dif,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2070[lng], string_log [lng],   20,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &minnptimg,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2082[lng], string_unit[lng],    5,    0,     5000, file_manager );
#endif // OPENCV_PARTICLE
    pView->parameter[pView->num_params++] = new CParameter( &minnumimg,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2084[lng], string_unit[lng],    3,    0,  NIMAGES, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &mintotnpt,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng],  100,    0,     5000, file_manager );
	//

#ifdef COLOR_DETECTION_P1
	if(index==3|| index==4 || index==5|| index==6)
	{
	pView->parameter[pView->num_params++] = new CParameter( &max_dist_lab,  CTYPE_REAL,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2410[lng], string_unit[lng],   10,    0,  5000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &Lab_refl,  CTYPE_REAL,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2410L[lng], string_unit[lng],   10,    -200,  5000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &Lab_refa,  CTYPE_REAL,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2410a[lng], string_unit[lng],   10,    -200,  5000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &Lab_refb,  CTYPE_REAL,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2410b[lng], string_unit[lng],  10,    -200,   5000, file_manager );
	}
#endif  

#ifdef OPENCV_PARTICLE//Pour marked on 20190514
	{
		pView->parameter[pView->num_params++] = new CParameter(&insp_savemode, CTYPE_INT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_320a[lng], string_unit[lng], 0, 0, 3, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&nozero_single_thr, CTYPE_INT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2152[lng], string_unit[lng], 20, 2, NIMAGES, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&nozero_frm_sum_thr, CTYPE_INT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2150[lng], string_unit[lng], 5, 0, 5000, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&ratio, CTYPE_REAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2154[lng], string_unit[lng], 0.9, 0.1, 1.0, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&sensitivity_part, CTYPE_REAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2156[lng], string_unit[lng], 1.0, 0.5, 2.0, file_manager);
	}
#endif  //OPENCV_PARTICLE

/*    pView->parameter[pView->num_params++] = new CParameter( &num_img,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit[lng],   20,    0,    40, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_dif,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2070[lng], string_log [lng],   20,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &min_dim,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2104[lng], string_pix [lng],    0,    0,  5000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max_dim,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2106[lng], string_pix [lng],   20,    0,  5000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max_dist,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2102[lng], string_pix [lng],   30,    1,   400, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_depth, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2108[lng], string_unit[lng],    3,    1,    10, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &max_err,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager ); */
  #else
    pView->parameter[pView->num_params++] = new CParameter( &num_img, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit[lng],   20,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_dif, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2070[lng], string_log [lng],   20,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max_err, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );
    file_manager->Add_data( reflections, dim );
  #endif // NEW_PARTICLE

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );


  return 0; // completed successfully
}

//#ifdef TIVS_S60
#if defined TIVS_S40 || defined TIVS_S60

int CParticle_inspection::Add_menu_parameters_needle( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2078[lng], child );
  file_manager->Add_separator( string_2072[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

		pView->parameter[pView->num_params++] = new CParameter(&insp_savemode, CTYPE_INT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_320a[lng], string_unit[lng], 0, 0, 3, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&nozero_frm_sum_thr, CTYPE_INT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2150[lng], string_unit[lng], 5, 0, 5000, file_manager);
		pView->parameter[pView->num_params++] = new CParameter(&sensitivity_part, CTYPE_REAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2156[lng], string_unit[lng], 1.0, 0.5, 2.0, file_manager);

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  return 0; // completed successfully
}

#endif

/*
//Pour confirmed 20180604
// yang add for A20 special config 20180601
// if it is color image, we need to convert to RGB8 format 
// here is the converter

int CParticle_inspection::Process_image( CGrabResultPtr ptr_gr, UINT nimg )
{
	image_dsp[index]->scale_grabresult( ptr_gr ); // scale for display and prepare RGB buffer from BAYER data
	return Process_image( image_dsp[index]->ptr_buf, nimg);
}
// yang add end 20180601
*/
//UINT colred = 0;


int CParticle_inspection::Process_image( BYTE* img_acq, UINT nimg )
{
	//// 20190506
	//{
	//	cv::Mat im = cv::Mat::zeros(dimy, dimx, CV_8UC1);
	//	memcpy(im.data, img_acq, dimx*dimy)	;
	//	cv::imshow("opencvTest", im);
	//	cv::waitKey();
	//}
  ASSERT( nimg>=0 && nimg<200 );
  ASSERT( AfxIsValidAddress( img_acq, dim ) );

/*
  for ( i=0 ; i<num_img ; i++ )
  {
    Gaussian_blur( images[i], i );
    //fname.Format( L"%s\\gb_%02d", path_name, i ); 
    //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }
  //return 0;


  //proctime += timer.elapsed_us();

  SCALE_IMG( index, images[0] );

  nimgpt = 0;
  totnpt = 0;

  timer.reset();

  for ( i=1 ; i<num_img ; i++ )
  {
    Subtract_dilate( images[i], images[i-1], output, i );
    //fname.Format( L"%s\\diffb_%02d", path_name, i ); 
    //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

  proctime += timer.elapsed_us();

  if ( nimgpt>=minnumimg && totnpt>=mintotnpt )
  {
    txt1.Format( L"REJECT nimgpt %d/%d - totnpt %d/%d", nimgpt, minnumimg, totnpt, mintotnpt );
    col1 = TXT_DARKRED;
  }
  else
  {
    txt1.Format( L"GOOD nimgpt %d/%d - totnpt %d/%d", nimgpt, minnumimg, totnpt, mintotnpt );
    col1 = TXT_DARKGREEN;
  }



*/

  #ifdef NEW_PARTICLE
    ASSERT( nimg>=0 && nimg<NIMAGES );

    CTimer timer;

	  // yang add for A20 20180601

  	//prepare BW image buffer
#ifdef COLOR_DETECTION_P1

  if ( mode & PARTICLE_MODE_ADSORPTION )
  {
	//(BYTE*) img_gray = (BYTE*) _aligned_malloc( dimx*dimy , 16 ); 
	//ASSERT( img_gray && AfxIsValidAddress(img_gray,dimx*dimy) );
	//ZeroMemory( img_gray, dimx*dimy );

	float sumX = 0,sumY = 0,sumZ = 0;
	dist_lab = 0;
	//Lab_refl = 0, Lab_refa = 0, Lab_refb = 0;
	for(int y = 0; y < dimy; ++y)
	{
		BYTE* pline = img_acq + 3*( y*dimx );
		for ( int x = 0; x < dimx; ++x)
		{
			float R = *(pline + 3*x);
			float G = *(pline + 3*x+1);
			float B = *(pline + 3*x+2);
			
	//img_acq = img_gray; //removed 20180604

			//Pour added improved method 3 RGB2XYZ 20180306
			// 0.433953  * 2^20  = 455032.700928   455033
			// 0.376219  * 2^20  = 394494.214144   394494
			// 2^20-455033 -394494 =199049
			// 524288 = 2^20/2

			//Sum up all pixels value in inspection area of last image 20180604
			//if ( nimg >= num_img-1 && x>xmin && x<=xmax && y>ymin &&y<=ymax )

			//Sum up all pixels value in reduced inspection area of last image 20180604
			if ( nimg >= num_img-1 /*last image*/
				&& x > (xmin+(xmax-xmin)/4) && x <= (xmax-(xmax-xmin)/4) 
				&& y > (ymin+(ymax-ymin)/4) && y <= (ymax-(ymax-ymin)/4))
			{
			int X = 199049*B + 394494*G + 455033*R + 524288;
			int Y = 75675*B + 749900*G + 223002*R + 524288;
			int Z = 915161*B + 114795*G + 18621*R + 524288;
			X = X >> 20;
			Y = Y >> 20;
			Z = Z >> 20;

			sumX=sumX+X;
			sumY=sumY+Y;
			sumZ=sumZ+Z;
			}
			//
			*(img_gray + y* dimx + x) = (BYTE)(R*0.25+G*0.625+B*0.125);
		}
	}
	//--------------------------------------------------------------------------------------------------
	//int  sum_pixels = (ymax-ymin)*(xmax-xmin)/4;//reduced inspection area for average 20180605
	if(nimg >= num_img-1)
	{
	int   sum_pixels = ((xmax-(xmax-xmin)/4)-(xmin+(xmax-xmin)/4))*((ymax-(ymax-ymin)/4)-(ymin+(ymax-ymin)/4));
	double avg_X = sumX/sum_pixels;
	double avg_Y = sumY/sum_pixels;
	double avg_Z = sumZ/sum_pixels;

				//transfer XYZ to L,a,b 20180531
			L = 0.0;
			if ( avg_Y>0.008856 ) L = 116.0*pow(avg_Y,1.0/3.0)-16.0;
			else                  L = 903.3*avg_Y;

			a = 500.0 * ( funz_t(avg_X) - funz_t(avg_Y) );
			b = 200.0 * ( funz_t(avg_Y) - funz_t(avg_Z) );
	/*
	if(Lab_refl == 0 && Lab_refa == 0 && Lab_refb == 0)
	{
	Lab_refl = L;
	Lab_refa = a;
	Lab_refb = b;
	}
	*/
  }
	//dist_lab = (L-Lab_refl)*(L-Lab_refl) + (a-Lab_refa)*(a-Lab_refa) + (b-Lab_refb)*(b-Lab_refb);

	if ((index==3||index==4 ||index==5||index==6) && nimg >= num_img-1)
	{
	//txt.Format( L"%.2f, %.2f, %.2f, %.2f", avg_L, avg_a, avg_b, dist_lab );
	//camera[index].text[4]->SetWindowTextW( txt4 );
	
	if(max_dist_lab > 0)
	{
		dist_lab = (L-Lab_refl)*(L-Lab_refl) + (a-Lab_refa)*(a-Lab_refa) + (b-Lab_refb)*(b-Lab_refb);
		dist_lab =  dist_lab/1000.0;
	  if ( dist_lab>max_dist_lab )
	  {
	  //Draw red rectangle for color detection 20180604
		  area_color = COL_RED;
		  for ( int x = xmin+(xmax-xmin)/4 ; x <= xmax-(xmax-xmin)/4 ; x+=8 )
      {
        DRAW_PT( index, x, ymin+(ymax-ymin)/4, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax-(ymax-ymin)/4, area_color ); // 0xFFRRGGBB
      }
		  for ( int y = ymin+(ymax-ymin)/4 ; y <= ymax-(ymax-ymin)/4 ; y+=8 )
      {
        DRAW_PT( index, xmin+(xmax-xmin)/4, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmax-(xmax-xmin)/4, y, area_color ); // 0xFFRRGGBB
      }
	  //Draw red rectangle for color detection 20180604
		txt4.Format( L"%.2f, %.2f, %.2f, %.2f/%.2f", L, a, b, dist_lab, max_dist_lab );
	    //txt4.Format( L"REJECT %.2f - %.2f", dist_lab, max_dist_lab );
	    col4 = TXT_DARKRED;
		camera[index].text[4]->SetText( txt4 , col4);
	  }
	  else
	  {
	 //Draw green color rectangle for color detection 20180604
		  area_color = COL_GREEN;
		  for ( int x = xmin+(xmax-xmin)/4 ; x <= xmax-(xmax-xmin)/4 ; x+=8 )
      {
        DRAW_PT( index, x, ymin+(ymax-ymin)/4, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax-(ymax-ymin)/4, area_color ); // 0xFFRRGGBB
      }
		  for ( int y = ymin+(ymax-ymin)/4 ; y <= ymax-(ymax-ymin)/4 ; y+=8 )
      {
        DRAW_PT( index, xmin+(xmax-xmin)/4, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmax-(xmax-xmin)/4, y, area_color ); // 0xFFRRGGBB
      }
	 //Draw green color rectangle for color detection 20180604
		txt4.Format( L"%.2f, %.2f, %.2f, %.2f/%.2f", L, a, b, dist_lab, max_dist_lab );
	   //txt4.Format( L"GOOD  %.2f / %.2f", dist_lab, max_dist_lab );
	   col4 = TXT_DARKGREEN;
	   camera[index].text[4]->SetText( txt4,col4 );
	  }
	}
	else //disabled
	{
		txt4.Format(string_2426[lng]);
		col4 = 0;
		camera[index].text[4]->SetWindowTextW( txt4 );
	}

	}
	//------------------------------------------------------




    //col1 = 0;
    //return 0;
  }
  // yang add end 20180601
    //timer.reset();

  if ( mode & PARTICLE_MODE_ADSORPTION ) Gaussian_blur( img_gray, nimg );

#endif //COLOR_DETECTION_P1

  //::Message_box(_T("opencv test"));

  //Pour added for testing for MACTER 20180917   //Pour marked on 20201103

	//if ( mode & PARTICLE_MODE_ADSORPTION ) Gaussian_blur( img_acq, nimg );//Pour removed on 20190513 for OpenCV

	//if ( mode & PARTICLE_MODE_REFLECTION ) Gaussian_blur( img_acq, nimg );//Pour removed on 20190513 for OpenCV

	 // 20190506 foropencv
  #ifdef OPENCV_PARTICLE
	 memcpy(images[nimg] , img_acq, dimx*dimy);//Pour marked on 20210402
	 //memcpy(images[nimg] , img_gray, dimx*dimy); //Pour marked on 20210402
	#endif
    //fname.Format( L"%s\\gb_%02d", IMAGES_DIRECTORY, nimg ); 
    //fname.Format( L"%s\\gb_%02d", path_name, nimg ); 
    //fname.Format( L"%s\\gb_%d_%02d", path_name, int(100.0*sigma), nimg ); 
    //Save_buffer( images[nimg], dimx, dimy, fname.GetBuffer(0) );

    //proctime += timer.elapsed_us();

    if ( nimg==0 ) // first 
    {
      nimgpt = 0;
      totnpt = 0;
      Prepare_thr();

      SCALE_IMG( index, img_acq );
      for ( int x=xmin ; x<=xmax ; x+=8 )
      {
        DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
      }
      for ( int y=ymin ; y<=ymax ; y+=8 )
      {
        DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
      }

      //if ( mode & PARTICLE_MODE_REFLECTION ) modenew = 0; // bottomlight
      //else                                   modenew = 1; // backlight
      //TRACE("First img\n");
    }

    if ( nimg >= num_img-1 ) // last
    {
		      //SCALE_IMG( index, img_acq );
#ifdef COLOR_DETECTION_P1
      for ( int x=xmin ; x<=xmax ; x+=8 )
      {
        DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
      }
      for ( int y=ymin ; y<=ymax ; y+=8 )
      {
        DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
      }
#endif//COLOR_DETECTION_P1

      timer.reset();

	  //Pour added for testing of OPENCV on 20190504
	  // foropencv 20190506
	  //{

#ifdef OPENCV_PARTICLE
		  {
		  area->buffrect.left;
		  cv::Rect rt_roi = cv::Rect(cv::Point(area->buffrect.left, area->buffrect.top), cv::Point(area->buffrect.right, area->buffrect.bottom) );
		  MiddlewareParticleDi::ParaIn para_particleDi;
		  para_particleDi.minnumimg = minnumimg; 
		  para_particleDi.mintotnpt = mintotnpt;
		  para_particleDi.ratio = ratio;
		  para_particleDi.nozero_frm_sum_thr = nozero_frm_sum_thr;
		  para_particleDi.nozero_single_thr = nozero_single_thr;
		  para_particleDi.sensity = sensitivity_part;//Pour marked
		  para_particleDi.roi = rt_roi;
		  para_particleDi.iVimRltSave = false;
		  //para_particleDi.iVimOnlineSave = imagesavemode;//2;//Pour marked on 20190513 : 0:Disabled;1:Save only good images;2:Save only bad images; 3:Save all images
		  para_particleDi.iVimOnlineSave = insp_savemode;//Pour marked on 20190513 : 0:Disabled;1:Save only good images;2:Save only bad images; 3:Save all images
		 /*MiddlewareParticleDi* p_middleware_particle_di ;*/
		  if(NULL == p_middleware_particle_di)
		  p_middleware_particle_di= new MiddlewareParticleDi(para_particleDi);
		  p_middleware_particle_di->para.iVimOnlineSave = para_particleDi.iVimOnlineSave;
		  p_middleware_particle_di->ptrs2cvMats(images, dimx, dimy, num_img);
		  cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
		  cstr_camId = index;
		  p_middleware_particle_di->run(rlt_particleDi, cstr_camId, imcolor);
		 
		  //// log test for partilce start
		  //CString cstrlog;
		  //static int partilce_num(0);
		  //cstrlog.Format(L"particle_num is %d", partilce_num++);
		  //if ( logger ) logger->Add_string( cstrlog.GetBuffer(0) );
		  //// log test for partilce end

		  nimgpt = rlt_particleDi.nimgpt;
		  totnpt = rlt_particleDi.totnpt;
		  p_middleware_particle_di->cvMat2Ptr(image_dsp[index]->ptr_dsp, image_dsp[index]->dim_dsp.cx, image_dsp[index]->dim_dsp.cy,imcolor );
		  /*delete p_middleware_particle_di;*/
		  }
//#endif // OPENCV_PARTICLE
	  //}
	  //Pour added for testing of OPENCV on 20190504

#else
	  {
	  //Pour removed for opencv integration on 20190505
	    for ( UINT i=1 ; i<num_img ; i++ ) //Pour 20200715
	    //UINT i=12;
	  { //Pour 20200715
		  //fname.Format( L"%s\\diffb_%02d", path_name, i ); 
		  //colred = 0xFF000000 + ((200+i)<<16);
		  Subtract_dilate( images[i], images[i-1] );//Pour 20200715
		  //fname.Format( L"%s\\diffb_%02d", path_name, i ); 
		  //fname.Format( L"%s\\diffb_%02d", IMAGES_DIRECTORY, i ); 
		  //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
	  } //Pour 20200715
	  //Pour removed for opencv integration on 20190505
	  }
#endif // OPENCV_PARTICLE


      proctime += timer.elapsed_us();

    }

    return 0; // completed successfully  
  
  
/*    if ( nimg==0 ) // first vecchia versione
    {
      trace_lev  = 0;
      part_err   = 0;
      threshold  = min_dif; 
      max_dist2  = max_dist*max_dist;
      ZeroMemory( output, dim );
      //TRACE("First img\n");
    }
    else
    {
      //Subtract( img_acq, past_img, output, nimg ); prima versione bottoml
      //TRACE("img %d\n", nimg );

      if ( mode & PARTICLE_MODE_REFLECTION ) Subtract( img_acq,  past_img, output, nimg );
      if ( mode & PARTICLE_MODE_ADSORPTION ) Subtract( past_img, img_acq,  output, nimg );

      if ( nimg >= num_img-1 )
      {
        SCALE_IMG(index,img_acq);
        Post_process();
        //TRACE("Post process finito\n" );
      }
    }
    past_img  = img_acq; 

    return 0; // completed successfully */
  #endif // NEW_PARTICLE

  if ( nimg==0 ) // first image
  {
//    if ( particle_mode==PARTICLE_MODE_ADSORPTION ) Dilate2H1V_add_sensit_learn_neg( baseimg, img_acq, reflections, sensitdata, dimx, dimy ); // ca 3500us in debug con 3 thread e 1280x960
//    if ( particle_mode==PARTICLE_MODE_REFLECTION ) Dilate2H1V_add_sensit_learn_pos( baseimg, img_acq, reflections, sensitdata, dimx, dimy ); // ca 3500us in debug con 3 thread e 1280x960

    if ( mode&PARTICLE_MODE_BOTH )   // First  station defined for particle with selectable back/bottom light
    {
      if ( stn1_part_back ) mode=PARTICLE_MODE_ADSORPTION;
      else                  mode=PARTICLE_MODE_REFLECTION;
    }


    if ( learning ) // learning process
    {
      ZeroMemory( sensitdata, 16 );
    }
    else  // normal processing mode
    {
      part_err = 0;
      num_blob = 0;
      SCALE_IMG( index, img_acq );
      FillMemory( sensitdata, 16, min_dif );
    }

    /*
    if ( mode==PARTICLE_MODE_ADSORPTION ) Dilate2H1V_add_sensit_learn_neg( img_acq );
    if ( mode==PARTICLE_MODE_REFLECTION ) Dilate2H1V_add_sensit_learn_pos( img_acq );

    if ( mode==PARTICLE_MODE_ADSORPTION ) Dilate3H1V_add_sensit_learn_neg( img_acq );
    if ( mode==PARTICLE_MODE_REFLECTION ) Dilate3H1V_add_sensit_learn_pos( img_acq );
    */


    if ( mode & PARTICLE_MODE_ADSORPTION )
    {
      if ( mode & PARTICLE_MODE_REFLECTION ) Dilate8H5Vhor_add_sensit_learn_pos( img_acq );
      else                                   Dilate8H5Vhor_add_sensit_learn_neg( img_acq );
    }
    else // original/standard vertical mode
    {
      if ( mode & PARTICLE_MODE_ADSORPTION ) Dilate4H1V_add_sensit_learn_neg( img_acq );

      #ifdef PARTFLT_H4V1
        if ( mode & PARTICLE_MODE_REFLECTION ) Dilate4H1V_add_sensit_learn_pos( img_acq );
      #endif

      #ifdef PARTFLT_H6V3
        if ( mode & PARTICLE_MODE_REFLECTION ) Dilate6H3V_add_sensit_learn_pos( img_acq );
      #endif

      #ifdef PARTFLT_H8V4
        if ( mode & PARTICLE_MODE_REFLECTION ) Dilate8H4V_add_sensit_learn_pos( img_acq );
      #endif
    }

    CopyMemory( minmax, img_acq, dim ); // first image into minmax

    return 0;
  }


  // images in the middle
//  if ( mode==PARTICLE_MODE_ADSORPTION ) Get_diff_neg( img_acq );
//  if ( mode==PARTICLE_MODE_REFLECTION ) Get_diff_pos( img_acq );
  if ( mode & PARTICLE_MODE_REFLECTION ) Get_max( img_acq );
  if ( mode & PARTICLE_MODE_ADSORPTION ) Get_min( img_acq ); 

  
  if ( nimg>=num_img-1 ) // last image of sequence
  {

    if ( mode & PARTICLE_MODE_REFLECTION ) Sub_images( minmax, baseimg ); // img1-img2=>output
    if ( mode & PARTICLE_MODE_ADSORPTION ) Sub_images( baseimg, minmax ); // img1-img2=>output

    if ( learning ) // learning process
    {
      //Save_image( baseimg, dimx, dimy, "baseimg", PNG_FILE );
      //Save_image( minmax,  dimx, dimy, "minmax",  PNG_FILE );
      //Save_image( output,  dimx, dimy, "output",  PNG_FILE );
      Add_learning();
      //Save_image( reflections, dimx, dimy, "reflections", PNG_FILE );
      SCALE_IMG(index, reflections);
      //if ( image_dsp[index]->visible ) image_dsp[index]->Draw_image(TRUE);
    }
    else  // normal processing mode
    {
      //Dilate_max( output ); 
      //Sub_images( output, baseimg );

      /*if ( mode==PARTICLE_MODE_REFLECTION )
      {
//        Sub_images( output, baseimg );  // img1-img2=>output
        Sub_images( minmax, baseimg );  // img1-img2=>output
        Calculate_blob();
//        Calculate_blob( output, baseimg );
      }

      if ( mode==PARTICLE_MODE_ADSORPTION )
      {

        //Save_image( baseimg, dimx, dimy, "baseimg", PNG_FILE );
        //Save_image( minmax,  dimx, dimy, "minmax",  PNG_FILE );

        Sub_images( baseimg, minmax );  // img1-img2=>output
        Save_image( output,  dimx, dimy, "output",  PNG_FILE );

//        CopyMemory( output, baseimg, dim ); // new proc
//        Save_image( output, dimx, dimy, "output2", PNG_FILE );
        Calculate_blob();
//        Calculate_blob( baseimg, output );
      }*/

      Calculate_blob();
      //if ( image_dsp[index]->visible) image_dsp[index]->Draw_image(TRUE); 
    }
    //return 0;
  }

  return 0; // completed successfully
}









//#define DX DIMX_ACQ

//#define TEST_BLOB(x,y)          output[x+(y)*dimx]>minmax[x+(y)*dimx]

//#define TEST_BLOB(x,y)          output[x+(y)*dimx]   // only output
//#define TEST_BLOB(x,y)          img1[x+(y)*dimx]>LUTBLOB[img2[x+(y)*dimx]]
#define TEST_BLOB(x,y)          output[x+(y)*dimx]>LUTBLOB[baseimg[x+(y)*dimx]]



#define BLOCCO_BLOB( ox , oy )  if ( TEST_BLOB(ox,oy) )                { \
                                   dafx[end] = ox;                       \
                                   dafy[end] = oy;                       \
                                   output[dafx[end]+dafy[end]*dimx] = 0;   \
                                   end++;                              }
/*
#define BLOCCO_BLOB( ox , oy )  if ( output[ox+(oy)*dimx] )            { \
                                   dafx[end] = ox;                       \
                                   dafy[end] = oy;                       \
                                   output[dafx[end]+dafy[end]*dimx] = 0; \
                                   end++;                              }
                                   */



int CParticle_inspection::Calculate_blob( void )
//int CParticle_inspection::Calculate_blob( BYTE* img1, BYTE* img2 )
{
  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

//  int part_err  = 0;
//  int numblob = 0;

/*  int nblob0 = 0;
  int nblob1 = 0;
  int nblob2 = 0;
  int nblob3 = 0;
  int nblob4 = 0;*/

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;
 

  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x++ )
  {
    output[x+area->buffrect.top   *dimx] = 0;
    output[x+area->buffrect.bottom*dimx] = 0;
    DRAW_PT( index, x, area->buffrect.top,    area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom, area_color ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y++ )
  {
    output[area->buffrect.left +y*dimx] = 0;
    output[area->buffrect.right+y*dimx] = 0;
    DRAW_PT( index, area->buffrect.left,  y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right, y, area_color ); // 0xFFRRGGBB
  }



  for ( y = area->buffrect.top ; y <= area->buffrect.bottom ; y++ )
    for ( x = area->buffrect.left ; x <= area->buffrect.right ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( TEST_BLOB(x,y) ) // ( output[x+y*dimx] ) 
      {
        num_blob++;          // new blob found     
        dafx[0] = x;
        dafy[0] = y;
        end     = 1;
        output[x+y*dimx] = 0; // img_in[x+y*DX] = 0;
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finch?ci sono punti in lista da fare          
        {
           //Controllo se il punto che sto esaminando ha vicini da marcare
          /*BLOCCO_BLOB( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] - 1 );*/

	    	  BLOCCO_BLOB( dafx[start] - 1 , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start] + 1 );

          start++; // Ho controllato questo punto, percio' lo tolgo dalla lista

          ASSERT ( (   end >= 0 ) && (   end <= MAX_DIM + 2 ) );
          ASSERT ( ( start >= 0 ) && ( start <= MAX_DIM + 2 ) );

        } // Fine di questo blob

		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/

        // IN QUESTO PUNTO end ?la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            part_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              part_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_LIGHTORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                part_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                part_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[3]
              }
            }
          } /***--***/
       
        }  //BLOG BIG ENOUGH
        
        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

      } // if TEST
      start = 0;
      end   = 0;

    }  // ciclo for
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  return part_err;
}
 












int CParticle_inspection::Image_filter( learning_filter_type* filter )
{
  struct tipo_segm
  {
    int ofs;  // offset of first point of segment
    int lng;  // lenght of segment (number of points into it)
  };

  int dx        = filter->dimx;
  int dy        = filter->dimy;
  int raggio    = filter->radius;    //  6;
  int soglia    = filter->threshold; // 10;
  int perc_high = filter->perc_high; // 70;
  int perc_low  = filter->perc_low;  // 50;

  ASSERT( dx        > 100 && dx        <  2000 );
  ASSERT( dy        > 100 && dy        <  2000 );
  ASSERT( raggio    >   0 && raggio    <    20 );
  ASSERT( soglia    >=  0 && soglia    <   256 );
  ASSERT( perc_high >=  0 && perc_high <=  100 );
  ASSERT( perc_low  >=  0 && perc_low  <=  100 );
    
  CString msg;
  int x, y;
  int count;
  int i, j;
  int ofs;
  int hist[256];
  int npt_low, npt_high; // number of points corresponding with low and high percentage
  int soglia_pt;
  int ltot; // total brightness
  int nsegm;
  int npt;
  UCHAR* imm2 = NULL; // Immagine di backup dei dati




// Preparo un array dinamico per la memorizzazione dei segmenti
  tipo_segm *segm = NULL; // pointer to array to contain all segments
  nsegm = 2 * raggio + 1;
  segm = (tipo_segm*) malloc( nsegm * sizeof( tipo_segm ) ); // allocate reuired space
  if ( !segm ) return( false );

  // Precalcolo dei segmenti costituenti il cerchio richiesto
  npt = 0;
  for ( y = -raggio ; y <= raggio ; y++ )
  {
    segm[y+raggio].lng = 0; // Initialize to zero this segment
    for ( x = -raggio ; x <= raggio ; x++ )
    {
      if ( x*x+y*y <= raggio*raggio )
      {
        if ( segm[y+raggio].lng == 0 ) segm[y+raggio].ofs = x + y*dx; // Set initial offset
        segm[y+raggio].lng++; // Include this point into segment
      }
    }
    npt += segm[y+raggio].lng; // Update total number of points
  }

//  TRACE( "Struct \"segm\" completed: %i segments and %i points into it\n", nsegm, npt );

  // Allocate internal buffer and copy input image into it
  imm2 = (BYTE*) malloc( dx * dy );
  CopyMemory( imm2 , filter->img_in , dx*dy );

//  soglia_pt = int( 0.5 + 0.01 * npt * perc_low );
//  if ( soglia_pt == 0 ) soglia_pt = 1;
//  ASSERT( soglia_pt >= 0 && soglia_pt <= npt );


// Calcolo il numero di punti corrispondenti a perc_low e perc_high
// (in perc del numero di punti che costituiscono il cerchio)

  npt_low  = int( 0.5 + 0.01 * npt * perc_low  ); // number of points corresponding
  if ( npt_low == 0 ) npt_low = 1;

  npt_high = int( 0.5 + 0.01 * npt * perc_high ); // with low and high percentage
  if ( npt_high == 0 ) npt_high = 1;

  // Scansione dell'immagine in <y> da <raggio> a <dy-raggio>
  for ( y = raggio ; y < dy-raggio ; y++ )
  {
    // Preload histogram and ltot
    ltot = 0;
    ZeroMemory( hist , sizeof( hist ) );
    ofs = raggio + y*dx;
    for ( j = 0 ; j < nsegm ; j++ ) // Scan all segments
    {
      for ( i = segm[j].ofs ; i < segm[j].ofs+segm[j].lng ; i++ ) // Scan all offset within this segemnt
      {
        ASSERT( ofs+i >= 0 && ofs+i < dx*dy );
        hist[ imm2[ofs+i] ]++;
        ltot += imm2[ofs+i];
      }
    }

    // Scan the rest of line
    while ( ofs < dx-raggio+y*dx )
    {
      for ( j = 0 ; j < nsegm ; j++ ) // Scan all segments
      {
        ASSERT( ofs+segm[j].ofs >= 0 && ofs+segm[j].ofs < dx*dy );
        hist[ imm2[ofs+segm[j].ofs ] ]--; // remove exiting point

        ASSERT( ofs+segm[j].ofs+segm[j].lng >= 0 && ofs+segm[j].ofs+segm[j].lng < dx*dy );
        hist[ imm2[ofs+segm[j].ofs+segm[j].lng] ]++; // and add entering point

        ltot += imm2[ofs+segm[j].ofs+segm[j].lng] - imm2[ofs+segm[j].ofs]; // update total sum of brightness
        ASSERT( ltot >= 0 && ltot <= 255*npt );
      }

      // Compute average brightness and number of points to count
      if ( ltot > soglia*npt ) soglia_pt = npt_high;
      else                     soglia_pt = npt_low;

      // Search for new value to set
      i = 0;
      count = 0;
      while ( count < soglia_pt && i<255 ) count += hist[i++];

      ASSERT( i >= 0 && i < 256 );
      ASSERT( ofs >= 0 && ofs < dx*dy );
      filter->img_in[ofs+1] = i;

      ofs++;
    }

  }

  if ( imm2 ) free( imm2 );
  if ( segm ) free( segm );

  return( true );

}




int CParticle_inspection::Filter_reflections( int inp )
{

	//for ( int i=0 ; i< NUM_TLC ; i++ ) 
    //FillMemory( learnt[i], DIMX_ACQ*DIMY_ACQ, 50 + 100*i );

  learning_filter_type filter;

//  char name[100];
//  BYTE* backup[NUM_IMG] = { NULL, NULL, NULL, NULL, NULL };


  filter.img_in    = reflections;
  filter.dimx      =        dimx;
  filter.dimy      =        dimy;
  filter.perc_high =          90;
  filter.perc_low  =          30;
  filter.radius    =           5;
  filter.threshold =           0; 
  Image_filter( &filter );



  /* ORIGINALE 
  // PRIMO GIRO 
  filter.img_in    = reflections;
  filter.dimx      =        dimx;
  filter.dimy      =        dimy;

  filter.perc_high =          30; // provo a togliere le bolle
  filter.perc_low  =          30;
  filter.radius    =           2;
  filter.threshold =           0; 
  Image_filter( &filter );

  filter.perc_high =          85; // provo a togliere le bolle
  filter.perc_low  =          40;
  filter.radius    =           5;
  filter.threshold =           6; 
  Image_filter( &filter );
  */
  
  
  //  Elabora_riflessi( learnt[i], &filter );
//  Elabora_riflessi( learnt[i], &filter );

/*
  for ( int i=0 ; i<NUM_IMG ; i++ ) 
  {
    sprintf( name, "images\\raw_learning[%d].png", i );
    Save_image( learnt[i], DIMX_ACQ, DIMY_ACQ, name );


    backup[i] = ( BYTE* ) malloc( DIMX_ACQ*DIMY_ACQ );
    ASSERT( backup[i] );
    CopyMemory( backup[i], learnt[i], DIMX_ACQ*DIMY_ACQ );
/*
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        3;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
*/

  /*
    
    // PRIMO GIRO 
    filter.perc_high =       35; // provo a togliere le bolle
    filter.perc_low  =       30;
    filter.radius    =        2;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
    Elabora_riflessi( learnt[i], &filter );
   
    // SECONDO GIRO ESPANSIONE PURE
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        3;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );

    filter.radius    =        2;
    Elabora_riflessi( learnt[i], &filter );
//    Elabora_riflessi( learnt[i], &filter );

*/

/*
    // PRIMO GIRO 
    filter.perc_high =      100; // provo a togliere le bolle
    filter.perc_low  =       30;
    filter.radius    =        1;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
   
    // SECONDO GIRO ESPANSIONE PURE
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        1;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
*/



//    sprintf( name, "images\\processed_learning[%d].png", i );
//    Save_image( learnt[i], DIMX_ACQ, DIMY_ACQ, name );

//    if ( i==0 ) Save_image( learnt[i], DIMX_ACQ, DIMY_ACQ, "images\\learning_2_proc.png" );
    

/*
    // TERZO GIRO ESPANSIONE PURE
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        1;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
    if ( i==0 ) Save_image( learnt[i], DIMX_ACQ, DIMY_ACQ, "images\\learning_3_proc.png" );


    // QUARTO GIRO ESPANSIONE PURE
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        1;
    filter.threshold =        0; 
    Elabora_riflessi( learnt[i], &filter );
    if ( i==0 ) Save_image( learnt[i], DIMX_ACQ, DIMY_ACQ, "images\\learning_4_proc.png" );
*/

/*    // QUINTO GIRO ESPANSIONE PURE
    filter.perc_high =      100;
    filter.perc_low  =       30;
    filter.radius    =        1;
    filter.threshold =        1; 
    Elabora_riflessi( learnt[i], &filter ); */

    /*
    #ifdef IMAGE_DISPLAY
      SCALE_PT ( i, learnt[i] );
      image_dsp[i]->Draw_image();
    #else
      Scale_image( learnt[i], (int*) img_dsp[i].GetMemStart() );
    #endif


  }

  */
//  pView->Invalidate();

/*
  if ( MessageBox( NULL, L"Keep processed learning?", L"", MB_YESNO ) != IDYES )
  {
    for ( int i=0 ; i< NUM_IMG ; i++ ) 
    {
      CopyMemory( learnt[i], backup[i], DIMX_ACQ*DIMY_ACQ );
      SCALE_PT ( i, learnt[i] );
      image_dsp[i]->Draw_image();
    }
  }

  for ( int i=0 ; i< NUM_IMG ; i++ ) if ( backup[i] ) free( backup[i] );

  */
  
  return 0;
}


#ifdef NEW_PARTICLE

int CParticle_inspection::Prepare_images( int val, wchar_t* name )
{
  num_img = val;
  ASSERT( num_img>0 && num_img<NIMAGES );

  ASSERT( name && AfxIsValidString( name ) );
  path_name = name;

  for ( UINT i=0 ; i<num_img ; i++ )
  {
    //if ( images[i]==0 ) images[i] = (BYTE*) _aligned_malloc( dim, 16 ); 
    //ASSERT( images[i] && AfxIsValidAddress( images[i], dim ) );
    //ZeroMemory( images[i], dim );


    if ( path_name ) // L"C:\\sequences\\Sequence[P2_A]_20160726_183509"
    {
      fname.Format( L"%s\\img_%04d.png", path_name, i ); 
      image_dsp[index]->Load_image( fname.GetBuffer(0) );

      CopyMemory( images[i], image_dsp[index]->ptr_buf, dim );

//      SCALE_IMG( index, images[i] );
//      image_dsp[index]->Draw_image();
    } // if ( fname ) 
 
  } // for ( UINT i=0 ; i<num_img ; i++ )

  return 0;
}



int CParticle_inspection::Process_sequence( int val )
{

  ASSERT( dim == dimx*dimy );

//  ZeroMemory( minmax, dim );
//  ZeroMemory( output, dim );

  // ORIGINAL

/*  trace_lev = 1;

  for ( UINT i=0 ; i<num_img ; i++ )
  {
    if ( i==0 )
    {
      part_err   = 0;
      threshold  = min_dif; 
      max_dist2  = max_dist*max_dist;
    }
    else // after first
    {
      Subtract( images[i], images[i-1], output, i );
    }
  } // for ( UINT i=0 ; i<num_img ; i++ )
  
  //fname.Format( L"%s\\minmax", path_name ); 
  //Save_buffer( minmax, dimx, dimy, fname.GetBuffer(0) );

  fname.Format( L"%s\\output", path_name ); 
  Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );


  Post_process();
  return 0; */
  


  // registering
//  Find_offset( 0, 1 );
//  Find_offset( 1, 2 );


  /* tentativo con allineamento
  for ( UINT i=1 ; i<2 ; i++ ) //i<num_img-1 ; i++ )
  {
    Find_offset( i-1, i );
    Subtract_single_ofs( images[i], images[i-1], output, i );
    fname.Format( L"%s\\ndiff_%02d", path_name, i ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );

    ofsx = 2;
    ofsy = 0;
    Subtract_single_ofs( images[i], images[i-1], output, i );
    fname.Format( L"%s\\ndiff-2-0_%02d", path_name, i ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );

    ofsx = 1;
    ofsy = 1;
    Subtract_single_ofs( images[i], images[i-1], output, i );
    fname.Format( L"%s\\ndiff-1-1_%02d", path_name, i ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );

    ofsx =  1;
    ofsy = -1;
    Subtract_single_ofs( images[i], images[i-1], output, i );
    fname.Format( L"%s\\ndiff-1- -1_%02d", path_name, i ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );

  }
  */
  UINT i;
  CTimer timer;

  CString txt;
  UINT    col;

//  timer.reset();

  //for ( i=0 ; i<num_img ; i++ ) Process_image( images[i], i );
  //for ( i=0 ; i<num_img ; i++ ) Process_image( images[i], i );//Pour marked on 20200715//20201103

  
 // //Pour added for interface tests on 20190428

 //   {
	//  MiddlewareParticleDi::ParaIn para_particleDi;
	//  para_particleDi.minnumimg = minnumimg; 
	//  para_particleDi.mintotnpt = mintotnpt;
	//  MiddlewareParticleDi* p_middleware_particle_di = new MiddlewareParticleDi(para_particleDi);
	//  p_middleware_particle_di->ptrs2cvMats(images, dimx, dimy, num_img);
	//  cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
	//  p_middleware_particle_di->run(rlt_particleDi, imcolor);
	//  nimgpt = rlt_particleDi.nimgpt;
	//  totnpt = rlt_particleDi.totnpt;
	//  p_middleware_particle_di->cvMat2Ptr(image_dsp[index]->ptr_dsp, image_dsp[index]->dim_dsp.cx, image_dsp[index]->dim_dsp.cy,imcolor );
	//  delete p_middleware_particle_di;
 // }
	////Pour added for interface tests on 20190428
	

//  proctime += timer.elapsed_us();

  txt.Format( L"%d us", timer.elapsed_us() );
  camera[index].text[2]->SetText( txt, 0 );

  if ( nimgpt>=minnumimg && totnpt>=mintotnpt )
  {
    txt.Format( L"REJECT err %d/%d (%d/%d)", totnpt, mintotnpt, nimgpt, minnumimg );
    col = TXT_DARKRED;
//    update_view = TRUE;
  }
  else
  {
//    insp_res->val |= bit_particle_ok; // set good result bit high
    txt.Format( L"GOOD err %d/%d (%d/%d)", totnpt, mintotnpt, nimgpt, minnumimg );
    col = TXT_DARKGREEN;
  }

  camera[index].text[1]->SetText( txt, col );
  image_dsp[index]->Draw_image();

  return 0;

  //Pour added transform of images 20180530
  /*
  if ( mode & PARTICLE_MODE_ADSORPTION )
  {
    for ( i=0 ; i<num_img ; i++ )
  {
    Color_gray( images[i], i);
    //fname.Format( L"%s\\gb_%02d", path_name, i ); 
    //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }
  }
  */
  //Pour added transform of images 20180530


  for ( i=0 ; i<num_img ; i++ )
  {
    Gaussian_blur( images[i], i );
    //fname.Format( L"%s\\gb_%02d", path_name, i ); 
    //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }
  //return 0;


  //proctime += timer.elapsed_us();

  SCALE_IMG( index, images[0] );

  nimgpt = 0;
  totnpt = 0;

  timer.reset();

  for ( i=1 ; i<num_img ; i++ )
  {
    Subtract_dilate( images[i], images[i-1], output, i );
    //fname.Format( L"%s\\diffb_%02d", path_name, i ); 
    //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

  proctime += timer.elapsed_us();

  if ( nimgpt>=minnumimg && totnpt>=mintotnpt )
  {
    txt1.Format( L"REJECT nimgpt %d/%d - totnpt %d/%d", nimgpt, minnumimg, totnpt, mintotnpt );
    col1 = TXT_DARKRED;
  }
  else
  {
    txt1.Format( L"GOOD nimgpt %d/%d - totnpt %d/%d", nimgpt, minnumimg, totnpt, mintotnpt );
    col1 = TXT_DARKGREEN;
  }


  return 0;



  // NEW PROCESSING
  for ( UINT i=0 ; i<num_img ; i++ )
  {
    if ( i==0 )
    {
      part_err   = 0;
      threshold  = min_dif; 
      max_dist2  = max_dist*max_dist;
    }
    else // after first
    {
      if ( modenew & 1024 ) // accumulate diffs into minmax
      {
        Subtract_single( images[i], images[i-1], minmax, i );
      }
      else                 // save all difference images
      {
        Subtract_single( images[i], images[i-1], output, i );
        fname.Format( L"%s\\diff_%02d", path_name, i ); 
        Save_buffer( images[i-1], dimx, dimy, fname.GetBuffer(0) );
      }

      //Single_blob( i-1,images[i-1]);  


/*      Subtract_singledil( images[i], images[i-1], output, i );

      fname.Format( L"%s\\ndiff_%02d", path_name, i ); 
      Save_buffer( images[i-1], dimx, dimy, fname.GetBuffer(0) );

      //Single_blob( i-1,images[i-1]);*/

    }
  } // for ( UINT i=0 ; i<num_img ; i++ )






  if ( modenew & 1024 ) // accumulate diffs into minmax
  {
    fname.Format( L"%s\\minmax", path_name ); 
    Save_buffer( minmax, dimx, dimy, fname.GetBuffer(0) );
  }

/*  CopyMemory( image_dsp[index]->ptr_buf, minmax, dim );
  fname.Format( L"%s\\minmax_a", path_name ); 
  image_dsp[index]->Save_buf( fname.GetBuffer(0) ); */


  //fname.Format( L"%s\\minmax", path_name ); 
  //Save_buffer( minmax, dimx, dimy, fname.GetBuffer(0) );

  //fname.Format( L"%s\\output", path_name ); 
  //Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );

  return 0;

  Post_process2();

//  Finalize();

  return 0;
}

#define PX1(x,y) images[i1][x+(y)*dimx]
#define PX2(x,y) images[i2][x+(y)*dimx]




int CParticle_inspection::Create_GF( double sigma_in )
{
  int x, y;
  int i, j; 


  if ( sigma_in!=-1.0 ) sigma=sigma_in;

  int sog = int( 0.5 + 0.01*(1<<16) );

//  for ( y=-int(sigma*3.0) ; y<=int(sigma*3.0) ; y++ )
  gf_num = 0;
  for ( y=-6 ; y<=6 ; y++ )
  {
//    for ( x=-int(sigma*3.0) ; x<=int(sigma*3.0) ; x++ )
    for ( x=-6 ; x<=6 ; x++ )
    {
      double val = exp( -0.5*(x*x+y*y)/(sigma*sigma) )/(2.0*PI*sigma*sigma);

      int vali = int( 0.5 + val*(1<<16) );

      if ( vali>sog && gf_num<MAXGFPT-1 ) // bigger than 10/1024, about 1%
      {
        gf_val[gf_num] = vali;
        gf_ofs[gf_num] = x+y*dimx;
        gf_num++;
      }
      ASSERT( gf_num<MAXGFPT-1 );
//      TRACE("%5d ", vali );
    }
//    TRACE("\n");
  }
  
  j = 0;
  for ( i=0 ; i<gf_num ; i++ )
  {
/*    y = DOUBLETOINT( double(gf_ofs[i])/dimx );
    x = gf_ofs[i]-dimx*y;
    TRACE("%4d - %4d,%4d\n", gf_val[i], x, y ); */
    j += gf_val[i];
  }
//  TRACE("Sigma=%3.2lf - selected %d points - total=%d - Perc=%3.2lf%%\n", sigma, gf_num, j, 100.0*j/(1<<16) ); //1024.0 );

  // secondo giro corretto
  double coef = double(1<<16)/j;

  gf_num = 0;
  for ( y=-6 ; y<=6 ; y++ )
  {
    for ( x=-6 ; x<=6 ; x++ )
    {
      double val = coef*exp( -0.5*(x*x+y*y)/(sigma*sigma) )/(2.0*PI*sigma*sigma);

      int vali = int( 0.5 + val*(1<<16) );

      if ( vali>sog && gf_num<MAXGFPT-1 ) // bigger than 10/1024, about 1%
      {
        gf_val[gf_num] = vali;
        gf_ofs[gf_num] = x+y*dimx;
        gf_num++;
      }
      ASSERT( gf_num<MAXGFPT-1 );
//      TRACE("%5d ", vali );
    }
//    TRACE("\n");
  }

  j = 0;
  for ( i=0 ; i<gf_num ; i++ )
  {
    j += gf_val[i];
  }
  TRACE("DOPO CORREZ - Sigma=%3.2lf - selected %d points - total=%d - Perc=%3.2lf%%\n", sigma, gf_num, j, 100.0*j/(1<<16) ); //1024.0 );

  return 0;
}



int CParticle_inspection::Find_offset( int i1, int i2 )
{
  int x, y;

  // model
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  int score = 0;

  UINT err     =          0;
  UINT err_min = 2000000000;

  int ox;
  int oy;

 

  y  = y1;
  oy = 0;

  for ( oy = -5 ; oy <= 5 ; oy++ )
  {
    for ( ox = -5 ; ox <= 5 ; ox++ )
    {
      err = 0;

      // first hor 
      x1 = 680;
      x2 = 750;
      y  = 210;

      for ( x=x1 ; x<x2 ; x++ )
      {
        err += (PX1(x,y)-PX2(x+ox,y+oy))*(PX1(x,y)-PX2(x+ox,y+oy));
      }

      // second ver
      x  = 704;
      y1 = 365;
      y2 = 390;

      for ( y=y1 ; y<y2 ; y++ )
      {
        err += (PX1(x,y)-PX2(x+ox,y+oy))*(PX1(x,y)-PX2(x+ox,y+oy));
      }

      if ( err < err_min )
      {
        ofsx    = ox;
        ofsy    = oy;
        err_min = err;
      }

      //TRACE("ox=%3d - oy=%3d - err=%5d\n", ox, oy, err );
    }
  }

  TRACE("%2d/%2d - max corr ofsx=%3d - ofsy=%3d - err=%5d\n", i1, i2, ofsx, ofsy, err_min );

  return 0;
}


int CParticle_inspection::Terminate( int val )
{



  return 0;
}



/////////////////////

// img1-img2=>out
// idx is index to be assigned

#define IMG1(x,y)   img1[x+(y)*dimx]
#define IMG2(x,y)   img2[x+(y)*dimx]
#define OUTP(x,y) output[x+(y)*dimx]
#define MINM(x,y) minmax[x+(y)*dimx]


//      if ( mode & PARTICLE_MODE_REFLECTION ) Subtract( img_acq,  past_img, output, nimg );
//      if ( mode & PARTICLE_MODE_ADSORPTION ) Subtract( past_img, img_acq,  output, nimg );






int CParticle_inspection::Subtract_singledil( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );

  for ( int y=0 ; y<dimy ; y++ )
  {
    for ( int x=0 ; x<dimx ; x++ )
    {
      if ( IMG1(x,y)-IMG2(x,y)>threshold ) IMG2(x,y) = 200;
      else                                 IMG2(x,y) =   0; // salva differenze step by step
    }
  }
  return 0;
}



// img[i],img[i-1] - img1 ultima / img2 penultima
int CParticle_inspection::Subtract_single( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );

  //Subtract_SSE(0,0,0,0);


  // accumulate differences

  if ( modenew & 1024 )
  {
    BYTE val=idx*6;
    ASSERT( val>0 && val<256 );

    if ( modenew&1 ) // backlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG2(x,y)-IMG1(x,y)>threshold ) MINM(x,y)=val;
    }
    else             // bottomlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=val;
    }

    return 0;
  }


  // check only differences frame by frame

  if ( modenew&1 ) // backlight
    for ( int y=0 ; y<dimy ; y++ )
      for ( int x=0 ; x<dimx ; x++ )
        if ( IMG2(x,y)-IMG1(x,y)>threshold ) IMG2(x,y) = 200;
        else                                 IMG2(x,y) =   0; // salva differenze step by step
  else             // bottomlight
    for ( int y=0 ; y<dimy ; y++ )
      for ( int x=0 ; x<dimx ; x++ )
        if ( IMG1(x,y)-IMG2(x,y)>threshold ) IMG2(x,y) = 200;
        else                                 IMG2(x,y) =   0; // salva differenze step by step

  return 0;



  for ( int y=0 ; y<dimy ; y++ )
  {
    for ( int x=0 ; x<dimx ; x++ )
    {
      //if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=thr*10;
      //if ( IMG1(x,y)-IMG2(x,y)>threshold ) OUTP(x,y)=val; // 

      if ( IMG1(x,y)-IMG2(x,y)>threshold ) IMG2(x,y) = 200;
      else                                 IMG2(x,y) =   0; // salva differenze step by step

 //     if ( IMG1(x,y)-IMG2(x,y)>threshold ) OUTP(x,y) += 10; // somma differenze in output
 //     if ( IMG1(x,y)-IMG2(x,y)>MINM(x,y) ) MINM(x,y)=IMG1(x,y)-IMG2(x,y); // store max variation into minmax

/*      if ( IMG1(x,y)-IMG2(x,y)>thr )
      {
        MINM(x,y)++;
        //OUTP(x,y) = 255;
        //minmax[x+y*dimx]++;
      } */
/*      else
      {
        OUTP(x,y) =   0;
      }  */
/*      if      ( IMG1(x,y)-IMG2(x,y)>thr ) OUTP(x,y) = 255;
      else if ( IMG2(x,y)-IMG1(x,y)>thr ) OUTP(x,y) = 255;
           else                           OUTP(x,y) =   0;  */
    }
  }
  return 0;
}




// img[i],img[i-1] - img1 ultima / img2 penultima include ofs
int CParticle_inspection::Subtract_single_ofs( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );

  // accumulate differences

  #define BRDR 10 // border

  if ( modenew & 1024 )
  {
    ASSERT(0);

    BYTE val=idx*6;
    ASSERT( val>0 && val<256 );

    if ( modenew&1 ) // backlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG2(x,y)-IMG1(x,y)>threshold ) MINM(x,y)=val;
    }
    else             // bottomlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=val;
    }



    return 0;
  }



  // check only differences frame by frame

  if ( modenew&1 ) // backlight
    for ( int y=BRDR ; y<dimy-BRDR ; y++ )
      for ( int x=BRDR ; x<dimx-BRDR ; x++ )
        if ( IMG2(x-ofsx,y-ofsy)-IMG1(x,y)>threshold ) OUTP(x-ofsx,y-ofsy) = 200;
        else                                           OUTP(x-ofsy,y-ofsy) =   0; // salva differenze step by step
  else             // bottomlight
    for ( int y=BRDR ; y<dimy-BRDR ; y++ )
      for ( int x=BRDR ; x<dimx-BRDR ; x++ )
        if ( IMG1(x,y)-IMG2(x-ofsx,y-ofsy)>threshold ) OUTP(x-ofsx,y-ofsy) = 200;
        else                                           OUTP(x-ofsx,y-ofsy) =   0; // salva differenze step by step

  return 0;




  #undef BRDR
}



int CParticle_inspection::Prepare_thr( void )
{
  int i;

//  static BYTE thr1 = 0;
//  static BYTE thr2 = 0;

  if ( thr_old1==threshold1 && thr_old2==threshold2 ) return 0;


  double coef = ( threshold2 - threshold1 ) / 256.0;

  for ( i=0 ; i<256 ; i++ )
  {
    int val;
    
    if ( mode & PARTICLE_MODE_ADSORPTION ) val = int( 0.5 + i - threshold1 - coef * i ); // backlight
    else                                   val = int( 0.5 + i + threshold1 + coef * i ); // bottomlight

    if      ( val > 255 ) thr[i] = 255;
    else if ( val <   0 ) thr[i] =   0;
         else             thr[i] = val;

    //TRACE("%d,%d\n", i, thr[i] );
  }

  thr_old1 = threshold1;
  thr_old2 = threshold2;
  //TRACE("threshold[%d] created\n", index );

  return 0;
}


// Applica gaussian blur all'immagine (si appoggia su output per calcolo intermedio)
// versione per calcolo offline/test
int CParticle_inspection::Gaussian_blur( BYTE* img ) //, int ind )
{
  ASSERT( AfxIsValidAddress( img, dimx*dimy ) );
//  ASSERT( ind>=0 && ind<NIMAGES );

  #define BRDR 10 // border

/*  UINT tot[256];
   ZeroMemory( tot, sizeof(tot) ); */


/* versione originale
  for ( int y=BRDR ; y<dimy-BRDR ; y++ )
    for ( int x=BRDR ; x<dimx-BRDR ; x++ )
    {
      int v1 = 1<<15;
      for ( int i=0 ; i<gf_num ; i++ ) v1+=img[x+y*dimx+gf_ofs[i]]*gf_val[i];
      v1 = v1 >> 16;
      ASSERT( v1>=0 && v1<256 );
      OUTP(x,y) = v1;
    }
    */

  for ( int y=BRDR*dimx ; y<(dimy-BRDR)*dimx ; y+=dimx )
    for ( int x=BRDR ; x<dimx-BRDR ; x++ )
    {
      int v1 = 1<<15;
      for ( int i=0 ; i<gf_num ; i++ ) v1+=img[x+y+gf_ofs[i]]*gf_val[i];
      //v1 = v1 >> 16;
      //ASSERT( v1>=0 && v1<256 );
      output[x+y] = v1 >> 16;
      //OUTP(x,y) = v1;
    }

    /*
  int acc = 0;
  int npx = 0;
  for ( int y=50 ; y<200 ; y++ )
  {
    acc+=tot[y];
    npx+=y*tot[y];
  }

//  avglum[ind] = BYTE( 0.5 + acc / ((dimy-2*BRDR)*(dimx-2*BRDR)) );
  avglum[ind] = BYTE( 0.5 + double(npx)/acc );

  ASSERT( avglum[ind]>=0 && avglum[ind]<256 );
  TRACE("avglum[%2d]=%3d\n",ind,avglum[ind]);
  */

//  avglum[ind] = BYTE( 0.5 + tot / ((dimy-2*BRDR)*(dimx-2*BRDR)) );
//  ASSERT( avglum[ind]>=0 && avglum[ind]<256 );
//  TRACE("avglum[%2d]=%3d\n",ind,avglum[ind]);

  #undef BRDR

  CopyMemory( img, output, dimx*dimy );
  return 0;
}



// Applica gaussian blur all'immagine 
// versione per macchna reale - img viene gb in images[ind]
int CParticle_inspection::Gaussian_blur( BYTE* img, int ind )
{
  ASSERT( AfxIsValidAddress( img, dimx*dimy ) );
  ASSERT( ind>=0 && ind<NIMAGES );

  #define BRDR 10 // border

  for ( int y=dimx*ymin ; y<=dimx*ymax ; y+=dimx ) // ( int y=BRDR*dimx ; y<(dimy-BRDR)*dimx ; y+=dimx )
  {
    for ( int x=xmin ; x<=xmax ; x++ ) // ( int x=BRDR ; x<dimx-BRDR ; x++ )
    {
      int v1 = 1<<15;
      for ( int i=0 ; i<gf_num ; i++ ) v1+=img[x+y+gf_ofs[i]]*gf_val[i];
      //v1 = v1 >> 16;
      //ASSERT( v1>=0 && v1<256 );
      images[ind][x+y] = v1 >> 16;
      //output[x+y] = v1 >> 16;
      //OUTP(x,y) = v1;
    }
  }

  #undef BRDR

  //CopyMemory( img, output, dimx*dimy );
  return 0;
}


//Pour added 20180530

//Mathematics

double CParticle_inspection::funz_t( double in )
{
  if ( in > 0.008856 ) return( pow( in , 1.0/3.0 ) );
  else return( 7.787 * in + 16-0 / 116.0 );
}
 
// Transfer color to gray 20180531
/*
int CParticle_inspection::Color_gray( BYTE* img, int ind )
{

	ASSERT( AfxIsValidAddress( img, dimx*dimy ) );

	#ifdef COLOR_DETECTION_P1

	//prepare gray image buffer 
	(BYTE*) im_gray_data = (BYTE*) _aligned_malloc( dimx*dimy , 16 ); //Pour added 20180530
	ASSERT( im_gray_data && AfxIsValidAddress(im_gray_data,dimx*dimy) );
	ZeroMemory( im_gray_data, dimx*dimy );

	// transform RGB to XYZ, gray  

	//long  sumx=0, sumy=0, sumz=0;
	int dif_xz1=0, dif_yz1 = 0;
	double dif_xz = 0, dif_yz = 0;
	for(int y = 0; y < dimy; ++y)
	{
		BYTE* pline = img + 3*( y*dimx );
		for ( int x = 0; x < dimx; ++x)
		{
			double dxz =0, dyz =0; //Pour added 20180301
			double R = *(pline + 3*x);
			double G = *(pline + 3*x+1);
			double B = *(pline + 3*x+2);

			//Pour added improved method 3 RGB2XYZ 20180306
			// 0.433953  * 2^20  = 455032.700928   455033
			// 0.376219  * 2^20  = 394494.214144   394494
			// 2^20-455033 -394494 =199049
			// 524288 = 2^20/2
			int X = 199049*B + 394494*G + 455033*R + 524288;
			int Y = 75675*B + 749900*G + 223002*R + 524288;
			int Z = 915161*B + 114795*G + 18621*R + 524288;
			X = X >> 20;
			Y = Y >> 20;
			Z = Z >> 20;

			//transfer XYZ to L,a,b 20180531
			L = 0.0;
			if ( Y>0.008856 ) L = 116.0*pow(Y,1.0/3.0)-16.0;
			else              L = 903.3*Y;

			a = 500.0 * ( funz_t(X) - funz_t(Y) );
			b = 200.0 * ( funz_t(Y) - funz_t(Z) );
			

			// to Gray can select one of 3 ways for transform 
			//(BYTE*)*(im_gray_data + y* dimx + x) = (BYTE)(B+R+G)/3;//(1)
  			//*(im_gray_data + y* dimx + x) = (BYTE)(B+R+G)/3;//(1)
			//(BYTE*)(im_gray_data + y* dimx + x) = (BYTE)R*0.299+G*0.587+B*0.144;//(2)
			//(BYTE*)(im_gray_data + y* dimx + x) = G;//(3)
			*(im_gray_data + y* dimx + x) = (BYTE)(R*0.25+G*0.625+B*0.125);//(4)
		}
	}
	TRACE("Image transform to gray using method 1");

	img = im_gray_data; //image to process turned to be gray image
	//Pour added 20180604
	//images[0] = &(*img);

#endif

	return 0;
}
*/
//Pour added 20180530




// img[i],img[i-1] - img1 ultima / img2 include dilatazione
// versione per prove produzione
int CParticle_inspection::Subtract_dilate( BYTE* img1, BYTE* img2 )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );

  #define BRDR 10 // border
  //#define SAVE_RESOUT // if defined saves result in output

  int nptimg = 0; // points found in this image

  #ifdef SAVE_RESOUT // if defined saves result in output
    ZeroMemory( output, dim );
  #endif

  // check only differences frame by frame
//  if ( modenew&1 ) // backlight
  if ( mode & PARTICLE_MODE_ADSORPTION ) // backlight
  {

    //int y = 183;
//    for ( int y=ymin ; y<=ymax ; y++ ) // Originale - 217ms di media debug / 112 in release
    for ( int y=ymin+1 ; y<ymax ; y++ ) // Originale - 217ms di media debug / 112 in release
    {
      //int x = 720;
//      for ( int x=xmin ; x<=xmax ; x++ )
      for ( int x=xmin+1 ; x<xmax ; x++ )
      {
        BYTE imgbk = IMG2(x-1,y-1);
        if ( imgbk > IMG2(x  ,y-1) ) imgbk = IMG2(x  ,y-1);
        if ( imgbk > IMG2(x+1,y-1) ) imgbk = IMG2(x+1,y-1);
        if ( imgbk > IMG2(x-1,y  ) ) imgbk = IMG2(x-1,y  );
        if ( imgbk > IMG2(x  ,y  ) ) imgbk = IMG2(x  ,y  );
        if ( imgbk > IMG2(x+1,y  ) ) imgbk = IMG2(x+1,y  );
        if ( imgbk > IMG2(x-1,y+1) ) imgbk = IMG2(x-1,y+1);
        if ( imgbk > IMG2(x  ,y+1) ) imgbk = IMG2(x  ,y+1);
        if ( imgbk > IMG2(x+1,y+1) ) imgbk = IMG2(x+1,y+1); 

/*        BYTE                         imgbk = IMG2(x-1,y-2);
        if ( imgbk > IMG2(x  ,y-1) ) imgbk = IMG2(x  ,y-2);
        if ( imgbk > IMG2(x+1,y-1) ) imgbk = IMG2(x+1,y-2);
        if ( imgbk > IMG2(x-1,y  ) ) imgbk = IMG2(x-1,y-1);
        if ( imgbk > IMG2(x  ,y  ) ) imgbk = IMG2(x  ,y-1);
        if ( imgbk > IMG2(x+1,y  ) ) imgbk = IMG2(x+1,y-1);
        if ( imgbk > IMG2(x-1,y  ) ) imgbk = IMG2(x-1,y  );
        if ( imgbk > IMG2(x  ,y  ) ) imgbk = IMG2(x  ,y  );
        if ( imgbk > IMG2(x+1,y  ) ) imgbk = IMG2(x+1,y  );
        if ( imgbk > IMG2(x-1,y+1) ) imgbk = IMG2(x-1,y+1);
        if ( imgbk > IMG2(x  ,y+1) ) imgbk = IMG2(x  ,y+1);
        if ( imgbk > IMG2(x+1,y+1) ) imgbk = IMG2(x+1,y+1);
        if ( imgbk > IMG2(x-1,y+1) ) imgbk = IMG2(x-1,y+2);
        if ( imgbk > IMG2(x  ,y+1) ) imgbk = IMG2(x  ,y+2);
        if ( imgbk > IMG2(x+1,y+1) ) imgbk = IMG2(x+1,y+2); */

        if ( IMG1(x,y)<thr[imgbk] )
        {
          nptimg++;
          DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          //DRAW_PT( index, x, y, colred ); // 0xFFRRGGBB
        }
      }
    }  


/*    BYTE* pimg2 = img2 + xmin + ymax*dimx;
    for ( int y=ymin ; y<=ymax ; y++ )
    {
      for ( int x=xmin ; x<=xmax ; x++ )
      {
        pimg2 -= dimx+1;

        BYTE imgbk = *pimg2; 
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;

        pimg2+=dimx-2;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;

        pimg2+=dimx-2;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk > *pimg2 ) imgbk = *pimg2;

        pimg2+=dimx-2;

        if ( IMG1(x,y)<thr[imgbk] )
        {
          nptimg++;
          DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        }
      }
    }*/


  }
  else             // bottomlight
  {

//    for ( int y=ymin ; y<=ymax ; y++ )  // ORIGINALE - 27.9 rel
    for ( int y=ymin+1 ; y<ymax ; y++ )  // skip outer
    {
//      for ( int x=xmin ; x<=xmax ; x++ )
      for ( int x=xmin+1 ; x<xmax ; x++ ) // skip outer
      {
        BYTE imgbk = IMG2(x-1,y-1);
        if ( imgbk < IMG2(x  ,y-1) ) imgbk = IMG2(x  ,y-1);
        if ( imgbk < IMG2(x+1,y-1) ) imgbk = IMG2(x+1,y-1);
        if ( imgbk < IMG2(x-1,y  ) ) imgbk = IMG2(x-1,y  );
        if ( imgbk < IMG2(x  ,y  ) ) imgbk = IMG2(x  ,y  );
        if ( imgbk < IMG2(x+1,y  ) ) imgbk = IMG2(x+1,y  );
        if ( imgbk < IMG2(x-1,y+1) ) imgbk = IMG2(x-1,y+1);
        if ( imgbk < IMG2(x  ,y+1) ) imgbk = IMG2(x  ,y+1);
        if ( imgbk < IMG2(x+1,y+1) ) imgbk = IMG2(x+1,y+1);

        if ( IMG1(x,y)>thr[imgbk] )
        {
          nptimg++;
          DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          #ifdef SAVE_RESOUT // if defined saves result in output
            output[x+y*dimx] = 200;
          #endif

        }
      }
    }   

    
/*    for ( int y=ymin ; y<=ymax ; y++ ) //  66ms e qualcosa - 27.5 rel
    {
      BYTE* pimg2 = img2 + xmin + y*dimx;
      for ( int x=xmin ; x<=xmax ; x++ )
      {
        pimg2 -= dimx+1;

        BYTE imgbk = *pimg2; 
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2 += dimx-2;

        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2 += dimx-2;

        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2++;
        if ( imgbk < *pimg2 ) imgbk = *pimg2;
        pimg2 -= dimx;

        if ( IMG1(x,y)>thr[imgbk] )
        {
          nptimg++;
          DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        }
      }
    } */
    

  }

  #ifdef SAVE_RESOUT // if defined saves result in output
    //fname.Format( L"%s\\diffb_%02d", path_name, i ); 
    //fname.Format( L"%s\\diffb_%02d", IMAGES_DIRECTORY, i ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  #endif

  if ( nptimg>minnptimg ) // enough points
  {
    totnpt += nptimg;
    nimgpt++; // increase number of images with enough points
  }

  return 0;
  #undef BRDR
}



// img[i],img[i-1] - img1 ultima / img2 include dilatazione
// versione per prove banco/simulazione
int CParticle_inspection::Subtract_dilate( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );

  #define BRDR 10 // border

  int nptimg = 0; // points found in this image

/*  if ( modenew & 1024 )
  {
    ASSERT(0);

    BYTE val=idx*6;
    ASSERT( val>0 && val<256 );

    if ( modenew&1 ) // backlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG2(x,y)-IMG1(x,y)>threshold ) MINM(x,y)=val;
    }
    else             // bottomlight
    {
      for ( int y=0 ; y<dimy ; y++ )
        for ( int x=0 ; x<dimx ; x++ )
          if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=val;
    }



    return 0;
  } */

//  int i;

  // check only differences frame by frame

  if ( modenew&1 ) // backlight
    for ( int y=BRDR ; y<dimy-BRDR ; y++ )
      for ( int x=BRDR ; x<dimx-BRDR ; x++ )
      {
        //if ( IMG2(x-ofsx,y-ofsy)-IMG1(x,y)>threshold ) OUTP(x-ofsx,y-ofsy) = 200;
        //else                                           OUTP(x-ofsy,y-ofsy) =   0; // salva differenze step by step
      }
  else             // bottomlight
    for ( int y=BRDR ; y<dimy-BRDR ; y++ )
      for ( int x=BRDR ; x<dimx-BRDR ; x++ )
      {

        //x = 687;
        //y = 178;


        BYTE imgbk = IMG2(x-1,y-1);
        if ( imgbk < IMG2(x  ,y-1) ) imgbk = IMG2(x  ,y-1);
        if ( imgbk < IMG2(x+1,y-1) ) imgbk = IMG2(x+1,y-1);
        if ( imgbk < IMG2(x-1,y  ) ) imgbk = IMG2(x-1,y  );
        if ( imgbk < IMG2(   ,y  ) ) imgbk = IMG2(x  ,y  );
        if ( imgbk < IMG2(x+1,y  ) ) imgbk = IMG2(x+1,y  );
        if ( imgbk < IMG2(x-1,y+1) ) imgbk = IMG2(x-1,y+1);
        if ( imgbk < IMG2(   ,y+1) ) imgbk = IMG2(x  ,y+1);
        if ( imgbk < IMG2(x+1,y+1) ) imgbk = IMG2(x+1,y+1);

//        if ( IMG1(x,y)-imgbk>threshold ) OUTP(x,y) = 200;

//        if ( IMG1(x,y)>thr[imgbk] ) OUTP(x,y) = 200;
//        else                        OUTP(x,y) =   0;

        if ( IMG1(x,y)>thr[imgbk] )
        {
          //OUTP(x,y) = 200;
          nptimg++;
          DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        }

//        OUTP(x,y) = v1;

        //if ( IMG1(x,y)-IMG2(x-ofsx,y-ofsy)>threshold ) OUTP(x-ofsx,y-ofsy) = 200;
        //else                                           OUTP(x-ofsx,y-ofsy) =   0; // salva differenze step by step
     /*   if ( IMG1(x,y)-IMG2(x-1,y-1)>threshold &&
             IMG1(x,y)-IMG2(x  ,y-1)>threshold &&
             IMG1(x,y)-IMG2(x+1,y-1)>threshold &&
             IMG1(x,y)-IMG2(x-1,y  )>threshold &&
             IMG1(x,y)-IMG2(x  ,y  )>threshold &&
             IMG1(x,y)-IMG2(x+1,y  )>threshold &&
             IMG1(x,y)-IMG2(x-1,y+1)>threshold &&
             IMG1(x,y)-IMG2(x  ,y+1)>threshold &&
             IMG1(x,y)-IMG2(x+1,y+1)>threshold )       OUTP(x,y) = 200;
        else                                           OUTP(x,y) =   0; // salva differenze step by step  */



      }


      if ( nptimg>minnptimg ) // enough points
      {
        totnpt += nptimg;
        nimgpt++;
      }

  return 0;
  #undef BRDR
}



int CParticle_inspection::Subtract( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );

  //Subtract_SSE(0,0,0,0);

  BYTE val=idx*6;
  ASSERT( val>0 && val<256 );

  for ( int y=0 ; y<dimy ; y++ )
  {
    for ( int x=0 ; x<dimx ; x++ )
    {
      //if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=thr*10;
      if ( IMG1(x,y)-IMG2(x,y)>threshold ) OUTP(x,y)=val; // 

     // if ( IMG1(x,y)-IMG2(x,y)>MINM(x,y) ) MINM(x,y)=IMG1(x,y)-IMG2(x,y); // store max variation into minmax
    }
  }
  return 0;
}


int CParticle_inspection::Subtract_SSE( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx )
{
 /* ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );
  ASSERT( idx*10>0 && idx*10<256 );

  ASSERT( ((UINT)img1&15) == 0 );
	ASSERT( AfxIsValidAddress( img1, dim ) );

  ASSERT( ((UINT)img2&15) == 0 );
	ASSERT( AfxIsValidAddress( img2, dim ) ); */




/*
  BYTE data1[16] = { 100, 150, 200, 210,  50,  10,  0, 255, 100, 150, 200, 210,  50,  10,  0, 255 };
  BYTE data2[16] = { 101, 160, 200, 200,  49,  10,  0, 255, 101, 160, 200, 200,  49,  10,  0, 255 };
  BYTE outpt[16] = {   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0 };
  */

  BYTE* data1 = (BYTE*) _aligned_malloc( 16, 16 ); // { 100, 150, 200, 210,  50,  10,  0, 255, 100, 150, 200, 210,  50,  10,  0, 255 };
  BYTE* data2 = (BYTE*) _aligned_malloc( 16, 16 ); // { 100, 150, 200, 210,  50,  10,  0, 255, 100, 150, 200, 210,  50,  10,  0, 255 };
  BYTE* outpt = (BYTE*) _aligned_malloc( 16, 16 ); // { 100, 150, 200, 210,  50,  10,  0, 255, 100, 150, 200, 210,  50,  10,  0, 255 };


  BYTE* im1  = data1; //img1   +4*dimx;
  BYTE* im2  = data2; //img2   +4*dimx;
  BYTE* outp = outpt; // +4*dimx;
//  UINT  dimr = dimx*(dimy-8);



  ASSERT( ((UINT)im1&15) == 0 );
	ASSERT( AfxIsValidAddress( im1, 16 ) );

  ASSERT( ((UINT)im2&15) == 0 );
	ASSERT( AfxIsValidAddress( im2, 16 ) );

  ASSERT( ((UINT)outp&15) == 0 );
	ASSERT( AfxIsValidAddress( outp, 16 ) );

  data1[ 0] = 100;
  data1[ 1] = 150;
  data1[ 2] = 200;
  data1[ 3] = 210;
  data1[ 4] =  50;
  data1[ 5] =  10;
  data1[ 6] =   0;
  data1[ 7] = 255;
  data1[ 8] = 110;
  data1[ 9] = 151;
  data1[10] = 203;
  data1[11] = 210;
  data1[12] =  50;
  data1[13] =  10;
  data1[14] =  14;
  data1[15] = 255;

  data2[ 0] = 101;
  data2[ 1] = 160;
  data2[ 2] = 200;
  data2[ 3] = 210;
  data2[ 4] =  50;
  data2[ 5] =  10;
  data2[ 6] =   1;
  data2[ 7] = 255;
  data2[ 8] = 100;
  data2[ 9] = 160;
  data2[10] = 200;
  data2[11] = 210;
  data2[12] =  50;
  data2[13] =  10;
  data2[14] =   0;
  data2[15] = 255;

  ZeroMemory( outpt, 16 );

  TRACE("\n\n");

  TRACE("data1 %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",data1[0],data1[1],data1[2],data1[3],data1[4],data1[5],data1[6],data1[7],data1[8],data1[9],data1[10],data1[11],data1[12],data1[13],data1[14],data1[15]);
  TRACE("data2 %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",data2[0],data2[1],data2[2],data2[3],data2[4],data2[5],data2[6],data2[7],data2[8],data2[9],data2[10],data2[11],data2[12],data2[13],data2[14],data2[15]);
  TRACE("outpt %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",outpt[0],outpt[1],outpt[2],outpt[3],outpt[4],outpt[5],outpt[6],outpt[7],outpt[8],outpt[9],outpt[10],outpt[11],outpt[12],outpt[13],outpt[14],outpt[15]);

  TRACE("\n\n");
  TRACE("processing...\n");

  __asm
	{
		mov		   esi, outp       ; Load <output> address into esi
		mov		   edi, im1        ; Load <img1> address into edi  
		mov		   eax, im2        ; Load <img2> address into eax
;		mov		   ebx, dimr       ; Load dimension into ebx
  	xor		   ecx, ecx        ; Clear ecx
;		LoopA:                   ; Loop start

    movdqa   xmm0, [edi+ecx] ; <img1>      => xmm0
    psubusb  xmm0, [eax+ecx] ; <img1-img2> => xmm0
		movdqa 	[esi+ecx], xmm0  ; xmm0        => <output>

;   add  	   ecx, 16         ; update ecx
;		cmp		   ecx, ebx        ; completed?
;		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}



  TRACE("data1 %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",data1[0],data1[1],data1[2],data1[3],data1[4],data1[5],data1[6],data1[7],data1[8],data1[9],data1[10],data1[11],data1[12],data1[13],data1[14],data1[15]);
  TRACE("data2 %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",data2[0],data2[1],data2[2],data2[3],data2[4],data2[5],data2[6],data2[7],data2[8],data2[9],data2[10],data2[11],data2[12],data2[13],data2[14],data2[15]);
  TRACE("outpt %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",outpt[0],outpt[1],outpt[2],outpt[3],outpt[4],outpt[5],outpt[6],outpt[7],outpt[8],outpt[9],outpt[10],outpt[11],outpt[12],outpt[13],outpt[14],outpt[15]);
  TRACE("\n\n");

  _aligned_free(data1);
  _aligned_free(data2);
  _aligned_free(outpt);



  return 0; // completed successfully






/*
  for ( int y=0 ; y<dimy ; y++ )
  {
    for ( int x=0 ; x<dimx ; x++ )
    {
      //if ( IMG1(x,y)-IMG2(x,y)>threshold ) MINM(x,y)=thr*10;
      if ( IMG1(x,y)-IMG2(x,y)>threshold ) OUTP(x,y)=idx*10; // out[x+y*dimx]=idx*10;


    }
  } */
  return 0;
}







//#define TEST_BLOB2(x,y)         output[x+(y)*dimx]


/*
#define BLOCCO_BLOB2(ox,oy)  if ( OUTP(ox,oy) )                      {  \
                                dafx[end]   = ox;                       \
                                dafy[end]   = oy;                       \
                                histo[OUTP(ox,oy)]++;                   \
                                OUTP(ox,oy) =  0;                       \
                                end++;                              }
                                */



#define BLOCCO_BLOB3(ox,oy)  if ( IMG(ox,oy) )                                 {           \
                                IMG(ox,oy) =  0;                                           \
                                dafx[end]  = ox;                                           \
                                dafy[end]  = oy;                                           \
                                totx += ox;                                                \
                                toty += oy;                                                \
                                if ( ox<tracce[ind][ntr].xmn ) tracce[ind][ntr].xmn = ox;  \
                                if ( ox>tracce[ind][ntr].xmx ) tracce[ind][ntr].xmx = ox;  \
                                if ( oy<tracce[ind][ntr].ymn ) tracce[ind][ntr].ymn = oy;  \
                                if ( oy>tracce[ind][ntr].ymx ) tracce[ind][ntr].ymx = oy;  \
                                end++;                                         }



#define IMG(x,y) img[x+(y)*dimx]


int CParticle_inspection::Single_blob( UINT ind, BYTE* img )
{
  ASSERT( AfxIsValidAddress( img, dimx*dimy ) );

  ASSERT( ind>=0 && ind<20 );

  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  //CString msg;
  int x, y;

  UINT i=0;
  UINT j=0;

  UINT dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  //UINT start, end;

  trace_lev = 0;
  // trace_lev |= 1024; // se c'?anche B10 (1024) salva anche immagini parziali

  // Reset blob
  UINT start    = 0;
  UINT end      = 0;
  //num_blob      = 0;

  //UINT histo[256];
  UINT totx;
  UINT toty;
  //BYTE cmax, cmin;

  //ZeroMemory( tracce, sizeof(blob) );
  //ntracce[ind] = 0;
  UINT ntr = 0;

  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    //ZeroMemory( image_dsp[index]->ptr_fulldsp, image_dsp[index]->dim_buf.cx*image_dsp[index]->dim_buf.cy*4 );
    if ( trace_lev&1024 ) for ( i=0 ; i<UINT(image_dsp[index]->dim_buf.cx*image_dsp[index]->dim_buf.cy) ; i++ ) image_dsp[index]->ptr_fulldsp[i] = 0xFF000000; // con AA=00 appaiono trasparenti 
  #endif

  area_color = COL_MAGENTA;
  for ( x=area->buffrect.left-1 ; x<=area->buffrect.right+1 ; x++ )
  {
    OUTP(x,area->buffrect.top   -1)=0;
    OUTP(x,area->buffrect.bottom+1)=0;
    //DRAW_PT( index, x, area->buffrect.top   -1, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, x, area->buffrect.bottom+1, area_color ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top-1 ; y<=area->buffrect.bottom+1 ; y++ )
  {
    OUTP(area->buffrect.left -1,y)=0;
    OUTP(area->buffrect.right+1,y)=0;
    //DRAW_PT( index, area->buffrect.left -1, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, area->buffrect.right+1, y, area_color ); // 0xFFRRGGBB
  }

  if ( trace_lev&1024 )
  {
    if ( path_name==NULL ) path_name = IMAGES_DIRECTORY;
    fname.Format( L"%s\\output", path_name ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

  y = ymin;
  while ( y<=ymax && ntr<99 )
  {

    x = xmin;
    while ( x<=xmax && ntr<99 )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( IMG(x,y) )
      {
        totx = dafx[0] = x;
        toty = dafy[0] = y;

        tracce[ind][ntr].xmn = tracce[ind][ntr].xmx = x;
        tracce[ind][ntr].ymn = tracce[ind][ntr].ymx = y;
  
        IMG(x,y)= 0;
        end     = 1;
 
        while ( ( end>start ) && (end<MAX_DIM-10) ) // finch?ci sono punti in lista da fare          
        {
	    	  BLOCCO_BLOB3( dafx[start] - 1 , dafy[start] - 1 );
          BLOCCO_BLOB3( dafx[start]     , dafy[start] - 1 );
          BLOCCO_BLOB3( dafx[start] + 1 , dafy[start] - 1 );
          BLOCCO_BLOB3( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB3( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB3( dafx[start] - 1 , dafy[start] + 1 );
          BLOCCO_BLOB3( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB3( dafx[start] + 1 , dafy[start] + 1 );

          start++; // Ho controllato questo punto, percio' lo tolgo dalla lista

          ASSERT ( (   end >= 0 ) && (   end <= MAX_DIM + 2 ) );
          ASSERT ( ( start >= 0 ) && ( start <= MAX_DIM + 2 ) );

        } // Fine di questo blob


//        tracce[ind][ntr].cx  = int( 0.5 + totx/end );
//        tracce[ind][ntr].cy  = int( 0.5 + toty/end );
        tracce[ind][ntr].cx  = (totx+(end>>1))/end;
        tracce[ind][ntr].cy  = (toty+(end>>1))/end;
        tracce[ind][ntr].dim = end;
        tracce[ind][ntr].seq = ind;

        ntr++;

        start = 0;
        end   = 0;

      } // if TEST

      x++;
    }  // ciclo for x

    y++;
  } // for y

  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  if ( trace_lev&1024 )
  {
    fname.Format( L"%s\\final", path_name ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

//  trace_lev= 1; // stampa sequenza dei blob con dim, coords e seq
//  trace_lev= 2; // stampa tabella dist/perc in csv
//  trace_lev= 3; // stampa tabella dist/perc in csv
//  trace_lev=10; // stampa tutti i risultati incrociati

  //trace_lev=1;

  if ( trace_lev==1 )
  {
    if ( ind==0 ) TRACE("Blob#,dim,cx,cy,seq\n" ); // x csv
    TRACE(",,,SINGLE_BLOB(%d) - TROVATI %d BLOBS\n", ind, ntr );
      
    for ( i = 0 ; i < ntr ; i++ )
    {
//      TRACE("%d,%d,%d,%d,%d\n", i, tracce[ind][i].dim, tracce[ind][i].cx, tracce[ind][i].cy, tracce[ind][i].seq ); // x csv
      UINT dx = 1+tracce[ind][i].xmx-tracce[ind][i].xmn;
      UINT dy = 1+tracce[ind][i].ymx-tracce[ind][i].ymn;

      TRACE("%d,%d,%d,%d,%d,%d,%d\n", i, tracce[ind][i].dim, tracce[ind][i].cx, tracce[ind][i].cy, dx, dy, tracce[ind][i].seq ); // x csv
    }
  }

  ntracce[ind] = ntr;


  return 0;
}





int CParticle_inspection::Post_process2( void )
{

//  trace_lev= 1; // stampa sequenza dei blob con dim, coords e seq
//  trace_lev= 2; // stampa tabella dist/perc in csv
//  trace_lev= 3; // stampa tabella dist/perc in csv
//  trace_lev=10; // stampa tutti i risultati incrociati

  int  x, y;
  UINT i, j;

/*
  Find_next2( (0<<10) + 9 );
  Find_next2( (1<<10) + 5 );

  Find_next2( (0<<10) + 7 );
  Find_next2( (1<<10) + 4 );
  */

  ZeroMemory(sequence,sizeof(sequence));
  sql = 0;

/*  sequence[0].i1 = 9; // starting index
  sequence[0].s1 = 6; // starting plane
  while ( Find_next2() ); */

  area_color = COL_MAGENTA;
  for ( x=area->buffrect.left-1 ; x<=area->buffrect.right+1 ; x++ )
  {
    DRAW_PT( index, x, area->buffrect.top   -1, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom+1, area_color ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top-1 ; y<=area->buffrect.bottom+1 ; y++ )
  {
    DRAW_PT( index, area->buffrect.left -1, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right+1, y, area_color ); // 0xFFRRGGBB
  }


  UINT s;
//  UINT sequence[30];
//  UINT depth = 0;

  min_dim   =   5;
  max_dim   = 100;
  min_depth =   2;

  for ( s=0 ; s<num_img-min_depth ; s++ ) // sequence index scan
  {
    for ( i=0 ; i<ntracce[s] ; i++ ) // blob index scan
    {

      if ( tracce[s][i].dim>=min_dim && tracce[s][i].dim<=max_dim )
      {

        sql = 0;
        sequence[0].i1 = i; // starting index
        sequence[0].s1 = s; // starting plane
        while ( Find_next2() ); 

        if ( sql>=min_depth ) // clear sequence's blobs to avoid finding again
        {

          TRACE("Sequenza depth=%2d\n", sql );

          TRACE("%2d,%2d - %3d (%3d,%3d)\n", sequence[0].i1, sequence[0].s1, tracce[sequence[0].s1][sequence[0].i1].dim, tracce[sequence[0].s1][sequence[0].i1].cx, tracce[sequence[0].s1][sequence[0].i1].cy );
          for ( j=0 ; j<sql ; j++ )
          {
            TRACE("%2d,%2d - %3d (%3d,%3d) %3d,%3d\n", 
              sequence[j].i2, sequence[j].s2, 
              tracce[sequence[j].s2][sequence[j].i2].dim, tracce[sequence[j].s2][sequence[j].i2].cx, tracce[sequence[j].s2][sequence[j].i2].cy,
              sequence[j].dx, sequence[j].dy        );
          }

          TRACE("---------------------\n");
/*          TRACE("Sequenza depth=%2d - (%d,%d)", sql, sequence[0].s1, sequence[0].i1 );
          for ( j=0 ; j<sql ; j++ ) TRACE(",(%d,%d)", sequence[j].s2, sequence[j].i2 );
          TRACE("\n"); */


          

/*          for ( j=0 ; j<depth ; j++ )
          {
            Draw_blobbox2( sequence[j], COL_RED );

            part_err += tracce[sequence[j]>>10][sequence[j]&1023].dim;
            tracce[sequence[j]>>10][sequence[j]&1023].dim = 0;
            //blob[sequence[j]].seq = 0;
            //part_err += blob[sequence[j]].dim;
          } */

        }  


/*        sequence[0] = (s<<10) + i;
        depth       = 0; 

        do 
        {
          depth++;
          sequence[depth] = Find_next2(sequence[depth-1]);
        } while ( sequence[depth] );  */

/*        TRACE("Sequenza d=%2d", depth );
//          for ( j=0 ; j<depth ; j++ ) TRACE(" %d", sequence[j] );
        for ( j=0 ; j<depth ; j++ )
        {
          UINT seq = (sequence[j]>>10)&1023;
          UINT ind = sequence[j]&1023;
          TRACE(" - %d,%d (%d,%d)", seq, ind, tracce[seq][ind].cx, tracce[seq][ind].cy );
        }
        TRACE("\n"); */


 /*       if ( depth>min_depth ) // clear sequence's blobs to avoid finding again
        {
          
          TRACE("Sequenza profond d=%2d -", depth );
//          for ( j=0 ; j<depth ; j++ ) TRACE(" %d", sequence[j] );
          for ( j=0 ; j<depth ; j++ ) TRACE("(%d,%d),", (sequence[j]>>10), sequence[j]&1023 );
          TRACE("\n");
          

          for ( j=0 ; j<depth ; j++ )
          {
            Draw_blobbox2( sequence[j], COL_RED );

            part_err += tracce[sequence[j]>>10][sequence[j]&1023].dim;
            tracce[sequence[j]>>10][sequence[j]&1023].dim = 0;
            //blob[sequence[j]].seq = 0;
            //part_err += blob[sequence[j]].dim;
          }

        }  */


      }

      tracce[s][i].dim = 0;


/*      if ( blob[i].seq==s ) // check if this blob has sequence
      {

        ZeroMemory( sequence, sizeof(sequence) );
        sequence[0] = i;
        depth       = 0; 

        do 
        {
          depth++;
          sequence[depth] = Find_next(sequence[depth-1]);
        } while ( sequence[depth] );

        if ( depth>min_depth ) // clear sequence's blobs to avoid finding again
        {
          /*
          TRACE("Sequenza d=%2d -", depth );
//          for ( j=0 ; j<depth ; j++ ) TRACE(" %d", sequence[j] );
          for ( j=0 ; j<depth ; j++ ) TRACE(" %d (%d,%d),", sequence[j], blob[j].cx, blob[j].cy );
          TRACE("\n");
          */
/*          for ( j=0 ; j<depth ; j++ )
          {
            Draw_blobbox( sequence[j], COL_RED );
            blob[sequence[j]].seq = 0;
            part_err += blob[sequence[j]].dim;

          }
        }

      } */

    } // for i

  } // for s

  return 0;
}



// Search if there is any blob matching <ind> (s,i) into (s+1,j)
//int CParticle_inspection::Find_next2( UINT s, UINT i )
int CParticle_inspection::Find_next2( void )
{
  UINT i = sequence[sql].i1;
  UINT s = sequence[sql].s1;

  ASSERT( s>=0 && s<num_img-1  );
  ASSERT( i>=0 && i<ntracce[s] );

  UINT t = s+1; // plane to search within
  if ( t >= num_img-1 ) return 0; // cannot proceed

  ASSERT( t>=0 && t<num_img-1  );

  int dx;
  int dy;

  int  score_min = 850;
  int  score_max =   0;
  UINT jmax      =   0;

  for ( UINT j=0 ; j<ntracce[t] ; j++ ) // blob scan cycle
  {

    // compute score
    // dy min 5 - max 100
    // dx min 0 - max dy/3
    // dim ratio hor/ver
    int score = 0;

    if ( tracce[t][j].dim>0 )
    {
      // dimensions H/V
      #define DIMX1 (1+tracce[s][i].xmx-tracce[s][i].xmn) // >0
      #define DIMX2 (1+tracce[t][j].xmx-tracce[t][j].xmn) // >0
      #define DIMY1 (1+tracce[s][i].ymx-tracce[s][i].ymn) // >0
      #define DIMY2 (1+tracce[t][j].ymx-tracce[t][j].ymn) // >0
      // distances
      //    #define DSTX  (tracce[s][i].cx-tracce[t][j].cx)  // </>/=0
      //    #define DSTY  (tracce[s][i].cy-tracce[t][j].cy)  // </>/=0

//      UINT DSTX = abs( int(tracce[s][i].cx)-int(tracce[t][j].cx));
//      UINT DSTY = abs( int(tracce[s][i].cy)-int(tracce[t][j].cy));
      dx = tracce[s][i].cx-tracce[t][j].cx;
      dy = tracce[s][i].cy-tracce[t][j].cy;

//      ASSERT( DSTX>=0 );
//      ASSERT( DSTY>=0 );

      score = 1000;

      if ( DIMX1>DIMX2 ) score -= (DIMX1<<4)/DIMX2;
      else               score -= (DIMX2<<4)/DIMX1;

      if ( DIMY1>DIMY2 ) score -= (DIMY1<<4)/DIMY2;
      else               score -= (DIMY2<<4)/DIMY1;

/*    if ( DSTX>0 )      score -= DSTX;
    else               score += DSTX;

    if ( DSTY>0 )      score -= DSTY;
    else               score += DSTY;

    score -= (DSTX<<2)/DSTY;  // favorisci movimenti orizzontali*/
    //TRACE("%d\n", 3*DSTX + DSTY ); //int( sqrt( 4.0*DSTX*DSTX + DSTY*DSTY ) ) );

    //score -= int( sqrt( 4.0*DSTX*DSTX + DSTY*DSTY ) );

    //TRACE("%d\n", 4*DSTX + DSTY );
    //  score -= 4*DSTX + DSTY;
      score -= 4*abs(dx) + abs(dy);

      //TRACE("%d,%d\n",j,score);

      if ( score>score_max )
      {
        score_max = score;
        jmax = j;
      }

    }
 /*   if ( tracce[s+1][j].dim>=min_dim && tracce[s+1][j].dim<=max_dim ) // next in sequence and not too big
    {

      if ( DSTX2*DSTX2+DSTY2*DSTY2 <= max_dist2 ) return( ((s+1)<<10)+j );
    }*/

    //TRACE("%d,%d\n",j,score);

  }

  if ( score_max>=score_min )
  {

    sequence[sql].dx = tracce[t][jmax].cx - tracce[s][i].cx;
    sequence[sql].dy = tracce[t][jmax].cy - tracce[s][i].cy;

    sequence[sql].i1 = i;
    sequence[sql].s1 = s;

    sequence[sql].i2 = jmax;
    sequence[sql].s2 = t;

    sequence[sql].sc = score_max;

    tracce[s][i].dim = 0; // remove

    sql++;

    sequence[sql].i1 = jmax;
    sequence[sql].s1 = t;

    return( 1 ); // succeeded

    //TRACE("VALID MATCH (%d,%d)=>(%d,%d) score=%d\n", i, s, j_max, t, score_max );
  }
//  else                        TRACE("MATCH NOT FOUND score_max=%d\n", score_max );


  ZeroMemory( &(sequence[sql]), sizeof(sequence_node) );

  return 0; // NOT FOUND
}












#define BLOCCO_BLOB2(ox,oy)  if ( OUTP(ox,oy) )                      {           \
                                dafx[end]   = ox;                                \
                                dafy[end]   = oy;                                \
                                totx += ox;                                      \
                                toty += oy;                                      \
                                if ( ox<blob[nblob].xmn ) blob[nblob].xmn = ox;  \
                                if ( ox>blob[nblob].xmx ) blob[nblob].xmx = ox;  \
                                if ( oy<blob[nblob].ymn ) blob[nblob].ymn = oy;  \
                                if ( oy>blob[nblob].ymx ) blob[nblob].ymx = oy;  \
                                histo[OUTP(ox,oy)]++;                            \
                                if ( OUTP(ox,oy)<cmin ) cmin=OUTP(ox,oy);        \
                                if ( OUTP(ox,oy)>cmax ) cmax=OUTP(ox,oy);        \
                                OUTP(ox,oy) =  0;                                \
                                end++;                                }


int CParticle_inspection::Post_process( void ) // BYTE* img1, BYTE* img2, BYTE* out, BYTE thr )
{
/*  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );
  ASSERT( thr>=0 && thr<=255 );*/

/*
  #define ESPX 5
  #define ESPY 5
  
  ZeroMemory( output, dim );
  
  for ( int y=-ESPY ; y<=ESPY ; y++ )
  {
    for ( int x=-ESPX ; x<=ESPX ; x++ )
    {
      double dist = sqrt(double(x*x+y*y));

//      if ( x*x+y*y<=ESPX*ESPX ) OUTP(x+100,y+100)=200;
      if ( dist<=ESPX ) OUTP(x+100,y+100)=200;

    }
  }
  */


/*  area->buffrect.left   =      2;
  area->buffrect.right  = dimx-3;
  area->buffrect.top    =      2;
  area->buffrect.bottom = dimy-3;*/


  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y;

  UINT i=0;
  UINT j=0;

  UINT dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  //UINT start, end;

  //trace_lev = 0;
  // trace_lev |= 1024; // se c'?anche B10 (1024) salva anche immagini parziali

  // Reset blob
  UINT start    = 0;
  UINT end      = 0;
  num_blob      = 0;

  UINT histo[256];
  UINT totx;
  UINT toty;
  BYTE cmax, cmin;

  ZeroMemory( blob, sizeof(blob) );
  nblob = 0;

  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    //ZeroMemory( image_dsp[index]->ptr_fulldsp, image_dsp[index]->dim_buf.cx*image_dsp[index]->dim_buf.cy*4 );
    if ( trace_lev&1024 ) for ( i=0 ; i<UINT(image_dsp[index]->dim_buf.cx*image_dsp[index]->dim_buf.cy) ; i++ ) image_dsp[index]->ptr_fulldsp[i] = 0xFF000000; // con AA=00 appaiono trasparenti 
  #endif

  area_color = COL_MAGENTA;
  for ( x=area->buffrect.left-1 ; x<=area->buffrect.right+1 ; x++ )
  {
    OUTP(x,area->buffrect.top   -1)=0;
    OUTP(x,area->buffrect.bottom+1)=0;
    DRAW_PT( index, x, area->buffrect.top   -1, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom+1, area_color ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top-1 ; y<=area->buffrect.bottom+1 ; y++ )
  {
    OUTP(area->buffrect.left -1,y)=0;
    OUTP(area->buffrect.right+1,y)=0;
    DRAW_PT( index, area->buffrect.left -1, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right+1, y, area_color ); // 0xFFRRGGBB
  }

  if ( trace_lev&1024 )
  {
    if ( path_name==NULL ) path_name = IMAGES_DIRECTORY;
    fname.Format( L"%s\\output", path_name ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

  y = ymin;

//  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y++ )
//  {
//    for ( x=area->buffrect.left ; x<=area->buffrect.right ; x++ )
  while ( y<=ymax && nblob<MAXNBLOB-1 )
  {

    x = xmin;
    while ( x<=xmax && nblob<MAXNBLOB-1 )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) ) // ( output[x+y*dimx] ) 
      {
        //TRACE("New blob start at %d,%d\n",x,y);

//        ZeroMemory( histo, sizeof(histo) );
//        num_blob++;          // new blob found     

        totx = dafx[0] = x;
        toty = dafy[0] = y;

        blob[nblob].xmn = blob[nblob].xmx = x;
        blob[nblob].ymn = blob[nblob].ymx = y;
  
        cmax = cmin = OUTP(x,y);

//        histo[OUTP(x,y)]++;

        OUTP(x,y)= 0;
        end      = 1;
        //output[x+y*dimx] = 0; // img_in[x+y*DX] = 0;
 
        while ( ( end>start ) && (end<MAX_DIM-10) ) // finch?ci sono punti in lista da fare          
        {
	    	  BLOCCO_BLOB2( dafx[start] - 1 , dafy[start] - 1 );
          BLOCCO_BLOB2( dafx[start]     , dafy[start] - 1 );
          BLOCCO_BLOB2( dafx[start] + 1 , dafy[start] - 1 );
          BLOCCO_BLOB2( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB2( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB2( dafx[start] - 1 , dafy[start] + 1 );
          BLOCCO_BLOB2( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB2( dafx[start] + 1 , dafy[start] + 1 );

          start++; // Ho controllato questo punto, percio' lo tolgo dalla lista

          ASSERT ( (   end >= 0 ) && (   end <= MAX_DIM + 2 ) );
          ASSERT ( ( start >= 0 ) && ( start <= MAX_DIM + 2 ) );

        } // Fine di questo blob


        blob[nblob].cx  = int( 0.5 + totx/end );
        blob[nblob].cy  = int( 0.5 + toty/end );
        blob[nblob].dim = end;
        if ( cmin==cmax ) // pure 
        {
          blob[nblob].seq = cmax/6;
        }
        else 
        {
          blob[nblob].seq = 250;
        }


        // if difference > 10 neglect
        if ( cmax-cmin>10 ) 
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_BLUE );  // BLUE - REFLEX
        }
        else
        {
          nblob++;
          // draw in ORANGE
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // BLUE - REFLEX
        }

        /* first version
        if ( end>2 ) // check minimum size first
        {
          // calcola n.col
          int ncol=0;
          int mcol=0;

          for ( i=0 ; i<256 ; i++ )
          {
            if (histo[i])
            {
              UINT soglia = 800*end>>10;
              UINT sogfr = int( end*800.0/1024.0 );
              if (histo[i]>(800*end>>10)) mcol=i; // set main color if > 80%
              ncol++;
            }
          }

          //TRACE("Blob[%d] dim=%d ncol=%d -", num_blob-1, end, ncol );
          //for ( i=0 ; i<end ; i++ ) TRACE(" %d,%d", dafx[i], dafy[i] );
          //TRACE("\n");

          if ( ncol>=2 ) // considera riflesso
          {
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_BLUE );  // BLUE - REFLEX
          }
          else // ONLY HERE WE SAVE BLOB FOR FURTHER ANALYSIS
          {
            UINT totx = 0;
            UINT toty = 0;
            for ( i=0 ; i<end ; i++ ) 
            {
              totx += dafx[i];
              toty += dafy[i];
            }


//            TRACE("Blob[%3d] dim=%3d ncol=%2d mcol=%3d -", num_blob-1, end, ncol, mcol );
//            for ( i=0 ; i<end ; i++ ) TRACE(" %d,%d", dafx[i], dafy[i] );
//            TRACE("\n");

            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - PARTICLE

            blob[nblob].cx  = int( 0.5 + totx/end );
            blob[nblob].cy  = int( 0.5 + toty/end );
            blob[nblob].dim = end;
            blob[nblob].seq = mcol/10;
            nblob++;

            if ( nblob>=MAXNBLOB ) 
            {
              return 99; // too many blobs, returns and rejects
            }
          }


        }
        else // too small, just draw in green
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }   first version */

        start = 0;
        end   = 0;

      } // if TEST

      x++;
    }  // ciclo for x

    y++;
  } // for y

  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  if ( trace_lev&1024 )
  {
    fname.Format( L"%s\\final", path_name ); 
    Save_buffer( output, dimx, dimy, fname.GetBuffer(0) );
  }

//  trace_lev= 1; // stampa sequenza dei blob con dim, coords e seq
//  trace_lev= 2; // stampa tabella dist/perc in csv
//  trace_lev= 3; // stampa tabella dist/perc in csv
//  trace_lev=10; // stampa tutti i risultati incrociati

//  trace_lev = 1;

  if ( trace_lev==1 )
  {
    TRACE("Blob#,dim,cx,cy,seq\n" ); // x csv
    for ( i = 0 ; i < nblob ; i++ )
    {
      //TRACE("Blob[%2d] dim=%3d - %3d,%3d - seq=%2d\n", i, blob[i].dim, blob[i].cx, blob[i].cy, blob[i].seq ); // pi?leggibile
      TRACE("%d,%d,%d,%d,%d\n", i, blob[i].dim, blob[i].cx, blob[i].cy, blob[i].seq ); // x csv
      //            for ( i=0 ; i<end ; i++ ) TRACE(" %d,%d", dafx[i], dafy[i] );
      //            TRACE("\n");
    }
  }

  UINT s;
  UINT sequence[30];
  UINT depth = 0;

  for ( s=1 ; s<num_img ; s++ )
  {

    for ( i=0 ; i<nblob ; i++ )
    {

      if ( blob[i].seq==s ) // check if this blob has sequence
      {

        ZeroMemory( sequence, sizeof(sequence) );
        sequence[0] = i;
        depth       = 0; 

        do 
        {
          depth++;
          sequence[depth] = Find_next(sequence[depth-1]);
        } while ( sequence[depth] );

        if ( depth>min_depth ) // clear sequence's blobs to avoid finding again
        {
          /*
          TRACE("Sequenza d=%2d -", depth );
//          for ( j=0 ; j<depth ; j++ ) TRACE(" %d", sequence[j] );
          for ( j=0 ; j<depth ; j++ ) TRACE(" %d (%d,%d),", sequence[j], blob[j].cx, blob[j].cy );
          TRACE("\n");
          */
          for ( j=0 ; j<depth ; j++ )
          {
            Draw_blobbox( sequence[j], COL_RED );
            blob[sequence[j]].seq = 0;
            part_err += blob[sequence[j]].dim;

          }
        }

      }

    }

  }



  /*

  for ( i=0 ; i<nblob ; i++ )
  {
    for ( j=0 ; j<i ; j++ )
    {

      if ( blob[j].seq==blob[i].seq+1 || blob[j].seq==blob[i].seq-1 )
      {
        int dx = blob[i].cx-blob[j].cx;
        int dy = blob[i].cy-blob[j].cy;
        int dist = int( 0.5 + sqrt( double(dx*dx+dy*dy) ) );

        //int perc = int( 0.5 + 100.0 * blob[j].dim / blob[i].dim );
        //int dseq = abs( int(blob[i].seq)-int(blob[j].seq) );

          TRACE("%3d - %3d - %4d - %2d\n", j, dist, perc, dseq );

      }

    }
  }

  */


/*    for ( j=0 ; j<nblob ; j++ )
    {




/*  if ( trace_lev==2 )
  {
    TRACE("#," );
    for ( i=0 ; i<nblob ; i++ ) TRACE("%d,", i );
    TRACE("\n" ); 
  }


  for ( i=0 ; i<nblob ; i++ )
  {

    if ( trace_lev==10 )
    {
      TRACE("----------------------------------- BLOB %d -----------------------------------\n",i);

      for ( UINT j=0 ; j<nblob ; j++ )
      {
        if ( i!=j )
        {
          int dx = blob[i].cx-blob[j].cx;
          int dy = blob[i].cy-blob[j].cy;
          int dist = int( 0.5 + sqrt( double(dx*dx+dy*dy) ) );
          int perc = int( 0.5 + 100.0 * blob[j].dim / blob[i].dim );
          int dseq = abs( int(blob[i].seq)-int(blob[j].seq) );

          TRACE("%3d - %3d - %4d - %2d\n", j, dist, perc, dseq );

        }
      }
      TRACE("-------------------------------------------------------------------------------\n");  
    }

    if ( trace_lev==2 ) TRACE("%d,", i );  */



/*    for ( j=0 ; j<nblob ; j++ )
    {
      if ( j<i )
      {
      
        int dx = blob[i].cx-blob[j].cx;
        int dy = blob[i].cy-blob[j].cy;
        int dist = int( 0.5 + sqrt( double(dx*dx+dy*dy) ) );
        int perc = int( 0.5 + 100.0 * blob[j].dim / blob[i].dim );
        int dseq = abs( int(blob[i].seq)-int(blob[j].seq) );

        //TRACE("%d,", dist );

//        if ( trace_lev==2 ) TRACE("%d,", dist );
//        if ( trace_lev==2 ) TRACE("%d,", perc );
        if ( trace_lev==2 ) TRACE("%d,", dseq );


        if ( blob[j].seq==blob[i].seq-1 || blob[j].seq==blob[i].seq+1 )
        {
//          if ( dist<140 && blob[i].dim>8 && blob[j].dim>8 )
          if ( dist<140 && blob[i].dim>4 && blob[j].dim>4 )
          {
            //blob[i].score++;
            //blob[j].score++;
            if ( trace_lev==3 ) TRACE( "Coppia %2d-%2d - Distanza %3d - Seq %2d-%2d - Dim %2d-%2d\n", i, j, dist, blob[i].seq, blob[j].seq, blob[i].dim, blob[j].dim );
            Draw_blobbox( i, COL_RED );
            Draw_blobbox( j, COL_RED );

            part_err += blob[i].dim + blob[j].dim;

          }
        }

/*        if ( dist<120 && perc>33 && perc<300 && dseq==1 )
        {
          blob[i].score++;
          blob[j].score++;
//          if ( trace_lev==2 ) TRACE("%d,", dist );
//          if ( trace_lev==2 ) TRACE("%d,", perc );
//          if ( trace_lev==2 ) TRACE("%d,", dseq );
        }
        else
        {
//          if ( trace_lev==2 ) TRACE(",");
        } */

    /*


      }
      else
      {
        if ( trace_lev==2 ) TRACE(",");
      }
    }

    if ( trace_lev==2 ) TRACE("\n" );
    */

//  } // for i=0 ; i<nblob  



/*  area_color = COL_RED;

  if ( trace_lev==3 ) TRACE("BLOB SCORES:\n");
  for ( j=0 ; j<nblob ; j++ )
  {
    if ( blob[j].score>=2 )
    {

      int xmn = blob[j].xmn-1;
      if ( xmn<0 ) xmn=0;

      int xmx = blob[j].xmx+1;
      if ( xmx>dimx-1 ) xmn=dimx-1;

      int ymn = blob[j].ymn-1;
      if ( ymn<0 ) ymn=0;

      int ymx = blob[j].ymx+1;
      if ( ymx>dimy-1 ) ymn=dimy-1;



//      for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED!!
      ASSERT( 0<=xmn && xmn<xmx && xmx<dimx );
      ASSERT( 0<=ymn && ymn<ymx && ymx<dimy );

      for ( x=xmn ; x<=xmx ; x++ )
      {
        DRAW_PT( index, x, ymn, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymx, area_color ); // 0xFFRRGGBB
      }

      for ( y=ymn ; y<=ymx ; y++ )
      {
        DRAW_PT( index, xmn, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmx, y, area_color ); // 0xFFRRGGBB
      } 


    } 
 //   if ( trace_lev==3 ) TRACE("%2d,%2d\n",j,blob[j].score);
  } */






  /*
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    fname.Format( L"%s\\fulldsp", path_name ); 
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->Save_fulldsp( fname.GetBuffer(0) );
    fname.Format( L"C:\\sequences\\results\\fulldsp", path_name ); 
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->Save_fulldsp( fname.GetBuffer(0) );
  #endif
  */





  return 0;
}




// Search if there is any blob matching <ind>
int CParticle_inspection::Find_next( UINT i )
{
  ASSERT( i>=0 && i<nblob );

  for ( UINT j=0 ; j<nblob ; j++ )
  {
    if ( j!=i && blob[j].seq==blob[i].seq+1 && blob[j].dim<max_dim ) // next in sequence and not too big
    {
//      int dx = blob[i].cx-blob[j].cx;
//      int dy = blob[i].cy-blob[j].cy;
//      int dist = int( 0.5 + sqrt( double(dx*dx+dy*dy) ) );
//      if ( dist<50 ) return( j );

      if ( ( blob[j].dim*6 >= blob[i].dim ) && ( blob[j].dim <= blob[i].dim*6 ) )
      {
        #undef DSTX
        #undef DSTY
        #define DSTX (blob[i].cx-blob[j].cx)
        #define DSTY (blob[i].cy-blob[j].cy)
        if ( DSTX*DSTX+DSTY*DSTY <= max_dist2 ) return( j );
      }
    }
  }
  return 0; // NOT FOUND
}


int CParticle_inspection::Draw_blobbox2( UINT ind, UINT col )
{

  UINT i = ind&1023;
  UINT s = (ind>>10);

  ASSERT( s>=0 && s<num_img-1 );
  ASSERT( i>=0 && i<ntracce[s] );
//  if ( ind<0 || ind>nblob ) return 99;



  int xmn = tracce[s][i].xmn-2;
  if ( xmn<     0 ) xmn=     0;
  if ( xmn>dimx-1 ) xmn=dimx-1;

  int xmx = tracce[s][i].xmx+2;
  if ( xmx<     0 ) xmx=     0;
  if ( xmx>dimx-1 ) xmx=dimx-1;

  int ymn = tracce[s][i].ymn-2;
  if ( ymn<     0 ) ymn=     0;
  if ( ymn>dimy-1 ) ymn=dimy-1;

  int ymx = tracce[s][i].ymx+2;
  if ( ymx<     0 ) ymx=     0;
  if ( ymx>dimy-1 ) ymx=dimy-1;

  
  //      for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED!!
  ASSERT( 0<=xmn && xmn<xmx && xmx<dimx );
  ASSERT( 0<=ymn && ymn<ymx && ymx<dimy );


  for ( int x=xmn ; x<=xmx ; x++ )
  {
    DRAW_PT( index, x, ymn, col ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymx, col ); // 0xFFRRGGBB
  }

  for ( int y=ymn ; y<=ymx ; y++ )
  {
    DRAW_PT( index, xmn, y, col ); // 0xFFRRGGBB
    DRAW_PT( index, xmx, y, col ); // 0xFFRRGGBB
  } 

  return 0;
}



int CParticle_inspection::Draw_blobbox( UINT ind, UINT col )
{

  if ( ind<0 || ind>nblob ) return 99;


  int xmn = blob[ind].xmn-2;
  if ( xmn<     0 ) xmn=     0;
  if ( xmn>dimx-1 ) xmn=dimx-1;

  int xmx = blob[ind].xmx+2;
  if ( xmx<     0 ) xmx=     0;
  if ( xmx>dimx-1 ) xmx=dimx-1;

  int ymn = blob[ind].ymn-2;
  if ( ymn<     0 ) ymn=     0;
  if ( ymn>dimy-1 ) ymn=dimy-1;

  int ymx = blob[ind].ymx+2;
  if ( ymx<     0 ) ymx=     0;
  if ( ymx>dimy-1 ) ymx=dimy-1;

  
  //      for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED!!
  ASSERT( 0<=xmn && xmn<xmx && xmx<dimx );
  ASSERT( 0<=ymn && ymn<ymx && ymx<dimy );


  for ( int x=xmn ; x<=xmx ; x++ )
  {
    DRAW_PT( index, x, ymn, col ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymx, col ); // 0xFFRRGGBB
  }

  for ( int y=ymn ; y<=ymx ; y++ )
  {
    DRAW_PT( index, xmn, y, col ); // 0xFFRRGGBB
    DRAW_PT( index, xmx, y, col ); // 0xFFRRGGBB
  } 

  return 0;
}




int CParticle_inspection::Finalize( void ) // BYTE* img1, BYTE* img2, BYTE* out, BYTE thr )
{
/*  ASSERT( AfxIsValidAddress( img1, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( img2, dimx*dimy ) );
  ASSERT( AfxIsValidAddress( out,  dimx*dimy ) );
  ASSERT( thr>=0 && thr<=255 );*/

  // prima espansione minmax=>output
/*  for ( int y=1 ; y<dimy-1 ; y++ )
  ZeroMemory( output, dim );
  {
    for ( int x=1 ; x<dimx-1 ; x++ )
    {
      if ( MINM(x,y)>1 )
      {
        OUTP(x-1,y-1)=1; //MINM(x,y);
        OUTP(x  ,y-1)=1; //MINM(x,y);
        OUTP(x+1,y-1)=1; //MINM(x,y);

        OUTP(x-1,y  )=1; //MINM(x,y);
        OUTP(x  ,y  )=1; //MINM(x,y);
        OUTP(x+1,y  )=1; //MINM(x,y);

        OUTP(x-1,y+1)=1; //MINM(x,y);
        OUTP(x  ,y+1)=1; //MINM(x,y);
        OUTP(x+1,y+1)=1; //MINM(x,y);

      }
    }
  } */

/*  ZeroMemory( output, dim );
  for ( int y=2 ; y<dimy-2 ; y++ )
  {
    for ( int x=2 ; x<dimx-2 ; x++ )
    {
      if ( MINM(x,y)>1 )
      {
        OUTP(x-2,y-2)=255; //MINM(x,y);
        OUTP(x-1,y-2)=255; //MINM(x,y);
        OUTP(x  ,y-2)=255; //MINM(x,y);
        OUTP(x+1,y-2)=255; //MINM(x,y);
        OUTP(x+2,y-2)=255; //MINM(x,y);

        OUTP(x-2,y-1)=255; //MINM(x,y);
        OUTP(x-1,y-1)=255; //MINM(x,y);
        OUTP(x  ,y-1)=255; //MINM(x,y);
        OUTP(x+1,y-1)=255; //MINM(x,y);
        OUTP(x+2,y-1)=255; //MINM(x,y);

        OUTP(x-2,y  )=255; //MINM(x,y);
        OUTP(x-1,y  )=255; //MINM(x,y);
        OUTP(x  ,y  )=255; //MINM(x,y);
        OUTP(x+1,y  )=255; //MINM(x,y);
        OUTP(x+2,y  )=255; //MINM(x,y);

        OUTP(x-2,y+1)=255; //MINM(x,y);
        OUTP(x-1,y+1)=255; //MINM(x,y);
        OUTP(x  ,y+1)=255; //MINM(x,y);
        OUTP(x+1,y+1)=255; //MINM(x,y);
        OUTP(x+2,y+1)=255; //MINM(x,y);

        OUTP(x-2,y+2)=255; //MINM(x,y);
        OUTP(x-1,y+2)=255; //MINM(x,y);
        OUTP(x  ,y+2)=255; //MINM(x,y);
        OUTP(x+1,y+2)=255; //MINM(x,y);
        OUTP(x+2,y+2)=255; //MINM(x,y);
      }
    }
  } */

  #define ESPX 5
  #define ESPY 5
  ZeroMemory( output, dim );
  for ( int y=ESPY ; y<dimy-ESPY ; y++ )
  {
    for ( int x=ESPX ; x<dimx-ESPX ; x++ )
    {
      if ( MINM(x,y)>1 )
      {
        for ( int ye=-ESPY ; ye<=ESPY ; ye++ )
        {
          for ( int xe=-ESPX ; xe<=ESPX ; xe++ )
          {
            OUTP(x+xe,y+ye)=255;
          }
        }
     }
    }
  }

  // estrazione finale solo ==1 senza espansione

  for ( int y=1 ; y<dimy-1 ; y++ )
  {
    for ( int x=1 ; x<dimx-1 ; x++ )
    {
      if ( ( MINM(x,y)==1 ) && ( OUTP(x,y)==0 ) ) MINM(x,y)=250;
      else                                        MINM(x,y)=  0;
    }
  }
   

  return 0;
}



#endif // NEW_PARTICLE




/*

// BACKLIGHT - SUBTRACTION MODE
// Copy <img_acq> dark-dilated - <learnt> - <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Dilate2H1V_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* lnr  = reflections+2*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * ( dimy - 4 );

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx-2*PARTICDIMX]  ; 
    pminub   mm1, [eax+ecx-1-PARTICDIMX]  ; 
		pminub   mm1, [eax+ecx  -PARTICDIMX]  ; 
		pminub   mm1, [eax+ecx+1-PARTICDIMX]  ; 
 		pminub   mm1, [eax+ecx-1           ]  ; 
    pminub   mm1, [eax+ecx             ]  ; 
 		pminub   mm1, [eax+ecx+1           ]  ; 
    pminub   mm1, [eax+ecx-1+PARTICDIMX]  ; 
    pminub   mm1, [eax+ecx  +PARTICDIMX]  ; 
		pminub   mm1, [eax+ecx+1+PARTICDIMX]  ; 
    pminub   mm1, [eax+ecx+2*PARTICDIMX]  ; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               2*dimx );
  ZeroMemory( baseimg+dimx*(dimy-2), 2*dimx );

  return 0; // completed successfully
}

*/


// BACKLIGHT - SUBTRACTION MODE
// Copy <img_acq> dark-dilated - <learnt> - <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 

int CParticle_inspection::Dilate2H1V_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * (dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -2*PARTICDIMX]; // !
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX];
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}


int CParticle_inspection::Dilate3H1V_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * (dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -3*PARTICDIMX]; // !
    pminub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-2*PARTICDIMX];
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX];
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+2*PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}


int CParticle_inspection::Dilate4H1V_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * (dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -4*PARTICDIMX]; // !
    pminub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-3*PARTICDIMX];
    pminub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-2*PARTICDIMX];
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX];
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+2*PARTICDIMX];
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+3*PARTICDIMX];
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +4*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}





/*

// BOTTOMLIGHT - ADDITION MODE
// Copy <img_acq> bright-dilated + <learnt> + <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Dilate2H1V_add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* lnr  = reflections+2*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * ( dimy - 4 );

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx-2*PARTICDIMX]  ; 
    pmaxub   mm1, [eax+ecx-1-PARTICDIMX]  ; 
		pmaxub   mm1, [eax+ecx  -PARTICDIMX]  ; 
		pmaxub   mm1, [eax+ecx+1-PARTICDIMX]  ; 
 		pmaxub   mm1, [eax+ecx-1           ]  ; 
    pmaxub   mm1, [eax+ecx             ]  ; 
 		pmaxub   mm1, [eax+ecx+1           ]  ; 
    pmaxub   mm1, [eax+ecx-1+PARTICDIMX]  ; 
    pmaxub   mm1, [eax+ecx  +PARTICDIMX]  ; 
		pmaxub   mm1, [eax+ecx+1+PARTICDIMX]  ; 
    pmaxub   mm1, [eax+ecx+2*PARTICDIMX]  ; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               2*dimx );
  ZeroMemory( baseimg+dimx*(dimy-2), 2*dimx );

  return 0; // completed successfully
}

*/



/*

// BOTTOMLIGHT - ADDITION MODE
// Copy <img_acq> bright-dilated + <learnt> + <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* lnr  = reflections+2*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * ( dimy - 4 );

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx]  ; 
		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add img_acq to xmm1
		paddusb	 mm1,      mm5   ; xmm1+xmm5 => xmm1 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm1 => base_img
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               2*dimx );
  ZeroMemory( baseimg+dimx*(dimy-2), 2*dimx );

  return 0; // completed successfully
}

*/






// BOTTOMLIGHT - ADDITION MODE
// Copy <img_acq> bright-dilated + <learnt> + <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Dilate2H1V_add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}




int CParticle_inspection::Dilate3H1V_add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}



int CParticle_inspection::Dilate4H1V_add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}




int CParticle_inspection::Dilate6H3V_add_sensit_learn_pos( BYTE* img_acq ) 
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +6*dimx;
  BYTE* img  = img_acq    +6*dimx;
  BYTE* lnr  = reflections+6*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-12);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -6*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-1-5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-5*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-1-4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-4*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-3*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-3-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-3             ]; 
    pmaxub   mm1, [eax+ecx-2             ]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
 		pmaxub   mm1, [eax+ecx+2             ]; 
 		pmaxub   mm1, [eax+ecx+3             ]; 

    pmaxub   mm1, [eax+ecx-3+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+3*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-1+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+4*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-1+5*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+5*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx  +6*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               6*dimx );
  ZeroMemory( baseimg+dimx*(dimy-6), 6*dimx );

  return 0; // completed successfully
}



int CParticle_inspection::Dilate8H4V_add_sensit_learn_pos( BYTE* img_acq ) 
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +8*dimx;
  BYTE* img  = img_acq    +8*dimx;
  BYTE* lnr  = reflections+8*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-16);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -8*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-8*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-8*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2-7*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-7*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -7*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-7*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2-7*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2-6*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-6*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -6*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-6*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2-6*PARTICDIMX]; 

		pmaxub   mm1, [eax+ecx-2-5*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-5*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2-4*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-3-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-3*PARTICDIMX]; 

		pmaxub   mm1, [eax+ecx-3-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-4-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4-  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-4             ]; 
    pmaxub   mm1, [eax+ecx-3             ]; 
    pmaxub   mm1, [eax+ecx-2             ]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
 		pmaxub   mm1, [eax+ecx+2             ]; 
 		pmaxub   mm1, [eax+ecx+3             ]; 
    pmaxub   mm1, [eax+ecx+4             ]; 

    pmaxub   mm1, [eax+ecx-4+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4+  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-3+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-3+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+3*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+4*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+5*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+5*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +5*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+5*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2+5*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+6*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+6*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +6*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+6*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2+6*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-2+7*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+7*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +7*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+7*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2+7*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-1+8*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +8*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+8*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               8*dimx );
  ZeroMemory( baseimg+dimx*(dimy-8), 8*dimx );

  return 0; // completed successfully
}







int CParticle_inspection::Dilate8H5Vhor_add_sensit_learn_pos( BYTE* img_acq ) 
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +8*dimx;
  BYTE* img  = img_acq    +8*dimx;
  BYTE* lnr  = reflections+8*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-16);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx-5-4*PARTICDIMX];  // first
    pmaxub   mm1, [eax+ecx-4-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+2-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+3-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+4-4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx+5-4*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-6-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-5-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6-3*PARTICDIMX]; 

		pmaxub   mm1, [eax+ecx-7-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx-6-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx-5-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx-4-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx-3-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+7-2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-8-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-7-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-6-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-5-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2-  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+7-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+8-  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-8             ]; 
    pmaxub   mm1, [eax+ecx-7             ]; 
    pmaxub   mm1, [eax+ecx-6             ]; 
    pmaxub   mm1, [eax+ecx-5             ]; 
    pmaxub   mm1, [eax+ecx-4             ]; 
    pmaxub   mm1, [eax+ecx-3             ]; 
    pmaxub   mm1, [eax+ecx-2             ]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
 		pmaxub   mm1, [eax+ecx+2             ]; 
 		pmaxub   mm1, [eax+ecx+3             ]; 
    pmaxub   mm1, [eax+ecx+4             ]; 
    pmaxub   mm1, [eax+ecx+5             ]; 
    pmaxub   mm1, [eax+ecx+6             ]; 
    pmaxub   mm1, [eax+ecx+7             ]; 
    pmaxub   mm1, [eax+ecx+8             ]; 

    pmaxub   mm1, [eax+ecx-8+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-7+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-6+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-5+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+7+  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+8+  PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-7+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-6+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-5+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+7+2*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-6+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-5+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+6+3*PARTICDIMX]; 

    pmaxub   mm1, [eax+ecx-5+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-4+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-3+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-2+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+2+4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+3+4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+4+4*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+5+4*PARTICDIMX]; 


		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               8*dimx );
  ZeroMemory( baseimg+dimx*(dimy-8), 8*dimx );

  return 0; // completed successfully
}



int CParticle_inspection::Dilate8H5Vhor_add_sensit_learn_neg( BYTE* img_acq ) 
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +8*dimx;
  BYTE* img  = img_acq    +8*dimx;
  BYTE* lnr  = reflections+8*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-16);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx-5-4*PARTICDIMX];  // first
    pminub   mm1, [eax+ecx-4-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1-4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+2-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+3-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+4-4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+5-4*PARTICDIMX]; 

    pminub   mm1, [eax+ecx-6-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-5-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6-3*PARTICDIMX]; 

		pminub   mm1, [eax+ecx-7-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx-6-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx-5-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx-4-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx-3-2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2-2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+7-2*PARTICDIMX]; 

    pminub   mm1, [eax+ecx-8-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-7-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-6-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-5-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2-  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+7-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+8-  PARTICDIMX]; 

    pminub   mm1, [eax+ecx-8             ]; 
    pminub   mm1, [eax+ecx-7             ]; 
    pminub   mm1, [eax+ecx-6             ]; 
    pminub   mm1, [eax+ecx-5             ]; 
    pminub   mm1, [eax+ecx-4             ]; 
    pminub   mm1, [eax+ecx-3             ]; 
    pminub   mm1, [eax+ecx-2             ]; 
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
 		pminub   mm1, [eax+ecx+2             ]; 
 		pminub   mm1, [eax+ecx+3             ]; 
    pminub   mm1, [eax+ecx+4             ]; 
    pminub   mm1, [eax+ecx+5             ]; 
    pminub   mm1, [eax+ecx+6             ]; 
    pminub   mm1, [eax+ecx+7             ]; 
    pminub   mm1, [eax+ecx+8             ]; 

    pminub   mm1, [eax+ecx-8+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-7+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-6+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-5+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+7+  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+8+  PARTICDIMX]; 

    pminub   mm1, [eax+ecx-7+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-6+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-5+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6+2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+7+2*PARTICDIMX]; 

    pminub   mm1, [eax+ecx-6+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-5+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2+3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3+3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4+3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5+3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+6+3*PARTICDIMX]; 

    pminub   mm1, [eax+ecx-5+4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-4+4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-3+4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-2+4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+4*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+2+4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+3+4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+4+4*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+5+4*PARTICDIMX]; 


		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               8*dimx );
  ZeroMemory( baseimg+dimx*(dimy-8), 8*dimx );

  return 0; // completed successfully
}













// add <output> data to <learnt> to accumulate learning
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Add_learning( void )
{

//  ASSERT( ((UINT)img_acq&15) == 0 );
//	ASSERT( AfxIsValidAddress( img_acq, dim ) );

  BYTE* res  = output     +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  UINT  dimr = dimx * ( dimy - 8 );

	__asm
	{
		mov		   esi, res        ; Load <output>   address+dimx into esi  
		mov		   edi, lnr        ; Load <learnt>   address+dimx into edi  
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [edi+ecx] ; Load next 16 bytes of <learnt>  into xmm0
    paddusb  xmm0, [esi+ecx] ; Add and accumulate <output> to <learnt>
		movdqa 	[edi+ecx], xmm0  ; And save to output
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}





/*


// BACKLIGHT - SUBTRACTION MODE
// Copy <img_acq> dark-dilated - <learnt> - <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 

int CParticle_inspection::Dilate_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * (dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
/*    movq     mm1, [eax+ecx  -4*PARTICDIMX]; // negative ofs => dilation toward higher Y (left)
    pminub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  -3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  -2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx+1-2*PARTICDIMX]; */

/*movq     mm1, [eax+ecx  -2*PARTICDIMX]; // !
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX];
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX];  */

/*    pminub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +4*PARTICDIMX]; */

/*    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX];  */

/*    pminub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +4*PARTICDIMX];  */

/*		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}
*/




/*
// BOTTOMLIGHT - ADDITION MODE
// Copy <img_acq> bright-dilated + <learnt> + <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Dilate_add_sensit_learn_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 ); 

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
/*    movq     mm1, [eax+ecx  -4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX];  */

 /*   movq     mm1, [eax+ecx  -2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 

  /*  pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX];  */

	/*	movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		paddusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		paddusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}


*/






// BACKLIGHT - SUBTRACTION MODE
// Subtract <img_acq> from <baseimg> accumulates results into <output>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Get_diff_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{

  ASSERT( ((UINT)img_acq&15) == 0 );
	ASSERT( AfxIsValidAddress( img_acq, dim ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* res  = output     +2*dimx;
  UINT  dimr = dimx * ( dimy - 4 );

	__asm
	{
		mov		   esi, res        ; Load <output>   address+dimx into esi  
		mov		   edi, bas        ; Load <baseimg>  address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [esi+ecx] ; Load next 16 bytes of <output>  into xmm0
    movdqa   xmm1, [edi+ecx] ; Load next 16 bytes of <baseimg> into xmm1
    psubusb  xmm1, [eax+ecx] ; Load and subtract <img_acq> from xmm1 (baseimg)
    paddusb  xmm0,     xmm1  ; Accumulate result to <output>
		movdqa 	[esi+ecx], xmm0  ; And save to output
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}


// BOTTOMLIGHT - ADDITION MODE
// Subtract <baseimg> from <img_acq> accumulates results into <output>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Get_diff_pos( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{

  ASSERT( ((UINT)img_acq&15) == 0 );
	ASSERT( AfxIsValidAddress( img_acq, dim ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* res  = output     +2*dimx;
  UINT  dimr = dimx * ( dimy - 4 );

	__asm
	{
		mov		   esi, res        ; Load <output>   address+dimx into esi  
		mov		   edi, bas        ; Load <baseimg>  address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [esi+ecx] ; Load next 16 bytes of <output>  into xmm0
    movdqa   xmm1, [eax+ecx] ; Load next 16 bytes of <img_acq> into xmm1
    psubusb  xmm1, [edi+ecx] ; Load and subtract <baseimg> from xmm1 (img_acq)
    paddusb  xmm0,     xmm1  ; Accumulate result to <output>
		movdqa 	[esi+ecx], xmm0  ; And save to output
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}



// BOTTOMLIGHT - GetMax
// Accumulates max of all img_acq into output
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Get_max( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{

  ASSERT( ((UINT)img_acq&15) == 0 );
	ASSERT( AfxIsValidAddress( img_acq, dim ) );

//  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* res  = minmax     +4*dimx;
  UINT  dimr = dimx * ( dimy - 8 );

	__asm
	{
		mov		   esi, res        ; Load <minmax>   address+dimx into esi  
//		mov		   edi, bas        ; Load <baseimg>  address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [esi+ecx] ; <minmax>  => xmm0
    movdqa   xmm1, [eax+ecx] ; <img_acq> => xmm1
    psubusb  xmm1,     xmm0  ; xmm1-xmm0 => xmm1
    paddusb  xmm0,     xmm1  ; xmm1+xmmo => xmm0
		movdqa 	[esi+ecx], xmm0  ; And save to minmax
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}




// BACKLIGHT - GetMin
// Accumulates min of all img_acq into minmax
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Get_min( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{

  ASSERT( ((UINT)img_acq&15) == 0 );
	ASSERT( AfxIsValidAddress( img_acq, dim ) );

//  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* res  = minmax     +4*dimx;
  UINT  dimr = dimx * ( dimy - 8 );

	__asm
	{
		mov		   esi, res        ; Load <minmax>   address+dimx into esi  
//		mov		   edi, bas        ; Load <baseimg>  address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [esi+ecx] ; <minmax>  => xmm0
    movdqa   xmm2,     xmm0  ; <minmax>  => xmm2
    movdqa   xmm1, [eax+ecx] ; <img_acq> => xmm1
    psubusb  xmm0,     xmm1  ; xmm0-xmm1 => xmm0
    psubusb  xmm2,     xmm0  ; xmm0-xmm1 => xmm0
		movdqa 	[esi+ecx], xmm2  ; And save to minmax
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}




// BOTTOMLIGHT - Sub_images
// Subtracts img2 from img1 and store result into output
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Sub_images( BYTE* img1, BYTE* img2 ) // img1-img2=>output
{

  ASSERT( ((UINT)img1&15) == 0 );
	ASSERT( AfxIsValidAddress( img1, dim ) );

  ASSERT( ((UINT)img2&15) == 0 );
	ASSERT( AfxIsValidAddress( img2, dim ) );

  BYTE* im1  = img1   +4*dimx;
  BYTE* im2  = img2   +4*dimx;
  BYTE* outp = output +4*dimx;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov		   esi, outp       ; Load <output> address into esi
		mov		   edi, im1        ; Load <img1> address into edi  
		mov		   eax, im2        ; Load <img2> address into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start
    movdqa   xmm0, [edi+ecx] ; <img1>      => xmm0
    psubusb  xmm0, [eax+ecx] ; <img1-img2> => xmm0
		movdqa 	[esi+ecx], xmm0  ; xmm0        => <output>
		add  	   ecx, 16         ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  return 0; // completed successfully
}



/*

// BOTTOMLIGHT - Dilate_max
// Copy dilated <img> into minmax
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
int CParticle_inspection::Dilate_max( BYTE* img_inp ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)img_inp &15) == 0 );
	ASSERT( ((UINT)minmax  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( img_inp, dim ) );
	ASSERT( AfxIsValidAddress( minmax,  dim ) );

  BYTE* max  = minmax     +4*dimx;
  BYTE* img  = img_inp    +4*dimx;
  UINT  dimr = dimx*(dimy-8);

	__asm
	{
		mov		   edi, max        ; Load <minmax>  address+dimx into edi  
		mov		   eax, img        ; Load <img_inp> address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -4*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1-  PARTICDIMX]; 
 		pmaxub   mm1, [eax+ecx-1             ]; 
    pmaxub   mm1, [eax+ecx               ]; 
 		pmaxub   mm1, [eax+ecx+1             ]; 
    pmaxub   mm1, [eax+ecx-1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx-1+3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pmaxub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pmaxub   mm1, [eax+ecx  +4*PARTICDIMX]; 
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( minmax,               4*dimx );
  ZeroMemory( minmax+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}



*/














/* non dilata correttamente gli spigoli
		movq     mm1, [eax+ecx-2*PARTICDIMX]  ; Load 8 bytes of img_acq into xmm1
		pminub   mm1, [eax+ecx-  PARTICDIMX]  ; Max with next line into xmm1
 		pminub   mm1, [eax+ecx-1           ]  ; Max with next line into xmm1
		pminub   mm1, [eax+ecx             ]  ; Max with next line into xmm1
 		pminub   mm1, [eax+ecx+1           ]  ; Max with next line into xmm1
		pminub   mm1, [eax+ecx+  PARTICDIMX]  ; Max with next line into xmm1
		pminub   mm1, [eax+ecx+2*PARTICDIMX]  ; Max with next line into xmm1
*/


/*  PARTE CHE DILATA LA ZONA CHIARA ED ERODE QUELLA SCURA
		pmaxub   mm1, [eax+ecx-  PARTICDIMX]  ; Max with next line into xmm1
 		pmaxub   mm1, [eax+ecx-1           ]  ; Max with next line into xmm1
		pmaxub   mm1, [eax+ecx             ]  ; Max with next line into xmm1
 		pmaxub   mm1, [eax+ecx+1           ]  ; Max with next line into xmm1
		pmaxub   mm1, [eax+ecx+  PARTICDIMX]  ; Max with next line into xmm1
		pmaxub   mm1, [eax+ecx+2*PARTICDIMX]  ; Max with next line into xmm1
*/



/*



// BACKLIGHT - SUBTRACTION MODE
// Copy <img_acq> dark-dilated - <learnt> - <sensit> to <base_img>
// pointers and dimensions must be 16 aligned
// UNSIGNED - SATURATED - PACKED - BYTES
//   base    => dest
//   img_acq => base
//   learnt  => source
//   sensit  => delta
//   dim 
//int CParticle_inspection::Dilate2H1V_add_sensit_learn_neg( BYTE* base_img, BYTE* img_acq, BYTE* learnt, BYTE* sensit, UINT dimx, UINT dimy )
int CParticle_inspection::Dilate2H1V_add_sensit_learn_neg( BYTE* img_acq )
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );
//  ASSERT( dimx==DIMX_ACQ ); // ATTENZIONE!

//  UINT dim=dimx*dimy;
  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +2*dimx;
  BYTE* img  = img_acq    +2*dimx;
  BYTE* lnr  = reflections+2*dimx;
//  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * ( dimy - 4 );

//  BYTE sns[16] = { 10, 10, 10, 10,  10, 10, 10, 10,  10, 10, 10, 10,  10, 10, 10, 10 };
  
  BYTE* sns = (BYTE*) _aligned_malloc( 16 , 16 ); 

  FillMemory( sns, 16, 123 );

  TRACE(" %d - %d \n", sizeof(sns), sizeof(BYTE) );

	__asm
	{
;		mov      esi, sensitdata  ; Put address <sensit> into <esi>
		mov      esi, sns         ; Put address <sensit> into <esi>
    movdqa   xmm5, [esi]      ; load <sensit>   data    into xmm5

; 		mov      esi, sns         ; Put address <sensit> into <esi>
;    movdqa   xmm5, sns        ; Load <sensit>   data    into xmm5
		mov		   esi, lnr         ; Load <learnt>   address+dimx into esi
		mov		   edi, bas         ; Load <base_img> address+dimx into edi  
		mov		   eax, img         ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr        ; Load dimension into ebx
		xor		   ecx, ecx         ; Clear ecx
		LoopA:                    ; Loop start

		movdqa   xmm1, [eax+ecx-2*PARTICDIMX]  ; Load 8 bytes of img_acq into xmm1
		pmaxub   xmm1, [eax+ecx-  PARTICDIMX]  ; Max with next line into xmm1
;		pmaxub   xmm1, [eax+ecx-1           ]  ; Max with next line into xmm1
		pmaxub   xmm1, [eax+ecx             ]  ; Max with next line into xmm1
;		pmaxub   xmm1, [eax+ecx+1           ]  ; Max with next line into xmm1
		pmaxub   xmm1, [eax+ecx+  PARTICDIMX]  ; Max with next line into xmm1
		pmaxub   xmm1, [eax+ecx+2*PARTICDIMX]  ; Max with next line into xmm1

		movdqa   xmm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 xmm1,     xmm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 xmm1,     xmm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movdqa 	[edi+ecx], xmm1   ; xmm0 => dest
		add  	   ecx, 16          ; update ecx
		cmp		   ecx, ebx         ; completed?
		jne    	 LoopA            ; if not, run again
		emms                      ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               2*dimx );
  ZeroMemory( baseimg+dimx*(dimy-2), 2*dimx );

  return 0; // completed successfully
}





*/

