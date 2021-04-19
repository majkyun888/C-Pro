#pragma once

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Bottom_inspection.h"
//#include "ChildView.h"



extern Camera_struct    camera  [NUM_TLC];
//extern CArea_adjust*    areainsp[NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
//extern BOOL             learn_models;
extern circle_type      my_circle[MAXR_CIRCLES];
extern UINT             thread_status;

extern BOOL             datarec_on;
extern Results_manager* bottomA_result;
extern Results_manager* bottomB_result;


#define  xmin    area->buffrect.left  
#define  xmax    area->buffrect.right 
#define  ymin    area->buffrect.top   
#define  ymax    area->buffrect.bottom




CBottom_inspection::CBottom_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  nhead = 0;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
  area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 ); //&& (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =      2; //  dimy/6;
  area->buffrect.bottom = dimy-2; // 5*dimy/6;
  area->buffrect.left   =      2; // dimx/6;
  area->buffrect.right  = dimx-2; // 5*dimx/6;

//  src_radius = 183; // src_radius used for alignment matching

//  ZeroMemory( model, sizeof(model) );
//  ZeroMemory( vial,  sizeof(vial)  );

//  model.data = NULL;
//  vial.data  = NULL;

//  Create_model();

//  circle.data = NULL;
  //precalc_circle3( 12, 100, &circle );
//  precalc_circles();


  src_radius    =  190;
  anl_radius    =  185;

  avg_max       =  220;
  avg_min       =  160;
  
  pos_dif       =   40;
  neg_dif       =   40;
  max_lum       =  200;
  min_lum       =   20;

  maxerr_pos    = 1000;
  maxerr_neg    = 1000;
  maxerr_max    = 1000;
  maxerr_min    = 1000;


  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required
    min2_radius =  190;   // start radius for second analysis
    max2_radius =  200;   // end   radius for second analysis
    avg2_max    =  220;   // maximum allowed average brightness
    avg2_min    =  100;   // minimum allowed average brightness
    pos2_dif    =   50;   // positive threshold brightness difference (calculated on average circle)
    neg2_dif    =   50;   // negative threshold brightness difference (calculated on average circle)
    max2_lum    =  240;   // maximum absolute brightness 0-255
    min2_lum    =   20;   // minimum absolute brightness 0-255
    maxerr2_pos = 2000;   // maximum allowed error for positive points (relative to average)
    maxerr2_neg = 2000;   // maximum allowed error for negative points (relative to average)
    maxerr2_max = 2000;   // maximum allowed error for absolute maximum brightness
    maxerr2_min = 2000;   // maximum allowed error for absolute minimum brightness
  #endif

  src_radius2 = 100;

    //Pour added 20180925
#ifdef REJECTION_SEPARATION

	avg_max2    =  240;   // maximum allowed average brightness
    avg_min2    =  80;    // minimum allowed average brightness
    maxerr_pos2 = 2000;   // maximum allowed error for positive points (relative to average)
    maxerr_neg2 = 2000;   // maximum allowed error for negative points (relative to average)
    maxerr_max2 = 2000;   // maximum allowed error for absolute maximum brightness
    maxerr_min2 = 2000;   // maximum allowed error for absolute minimum brightness

	//For double bottom ring
	avg2_max2    =  230;   // maximum allowed average brightness
    avg2_min2    =  80;   // minimum allowed average brightness
    maxerr2_pos2 = 2200;   // maximum allowed error for positive points (relative to average)
    maxerr2_neg2 = 2200;   // maximum allowed error for negative points (relative to average)
    maxerr2_max2 = 2200;   // maximum allowed error for absolute maximum brightness
    maxerr2_min2 = 2200;   // maximum allowed error for absolute minimum brightness

#endif//REJECTION_SEPARATION
  //Pour added 20180925


/*  lum_thr  =  50;
  min_npt  = 200;

  htot_max = 700;
  htot_min = 600;
  
  scalefct     = 210.0/11.0; // mm to pixel scale factor
  tip_diameter = 7.0;

  maxerr_symm  =   0;
  maxerr_curv  = 100;
  maxerr_bulge =   0;*/

//  profile = NULL;
//  prosize = 0;

//  mode = mode_in; //TIP_INSPECTION;
//  ASSERT( mode==TIP_INSPECTION || mode==CAP_INSPECTION );

/*  model   = NULL;
  modsize = 0;
  if ( mode==CAP_INSPECTION )
  {
    modsize = CAPMODELSIZE; // numero di elementi nel modello a partire dall'alto
    model = (mod_type*) malloc( sizeof(mod_type)*modsize );
    ZeroMemory( model, sizeof(mod_type)*modsize );
  }*/

}


CBottom_inspection::~CBottom_inspection(void)
{

//  if ( model.data ) free( model.data );
//  if ( vial.data  ) free( vial.data  );

  if ( area )
  {
    delete area;
    area = NULL;
  }
}


/*
int CBottom_inspection::Allocate_profile( void )
{
//  if ( profile ) free( profile );
//  prosize = xmax-xmin+1;
//  profile = (segm_type*) malloc( sizeof(segm_type)*prosize );
  return 0;
}
*/


int CBottom_inspection::Set_inspection_area( int ind )
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



int CBottom_inspection::Add_menu_parameters( CContainer_wnd* child )
{
  wchar_t** name = string_2760; // standard 
  #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
    if ( index==5 ) name = string_2850;
  #endif

	#if defined NEWCAP_ORALCAP && defined TIVS_F20 // for F20 machines
    if ( index==5 ) name = string_2850;
  #endif

  #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
    if ( index==7 ) name = string_2850;
  #endif

  #ifdef DOUBLE_BOTTOM_ANALYSIS
    if ( index==3 ) name = string_2760; //string_2744;
    if ( index==4 ) name = string_2760; //string_2744;
  #endif
  
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
  file_manager->Add_separator( name[lng] );
//  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2760[lng], child );
//  file_manager->Add_separator( string_2760[lng] );

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  #ifdef DOUBLE_BOTTOM_ANALYSIS
    pView->parameter[pView->num_params++] = new CParameter( &src_radius2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2766[lng], string_pix [lng],  105,    0,   255, file_manager );
  #else
    pView->parameter[pView->num_params++] = new CParameter( &src_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2766[lng], string_pix [lng],  105,    0,   255, file_manager );
  #endif

  pView->parameter[pView->num_params++] = new CParameter( &anl_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],  105,    0,   255, file_manager );

  #ifndef DOUBLE_BOTTOM_ANALYSIS
    pView->parameter[pView->num_params++] = new CParameter( &avg_max,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756[lng], string_log [lng],  220,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &avg_min,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758[lng], string_log [lng],  140,    0,   255, file_manager );
  #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter(&avg_max2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2756b[lng], string_log[lng], 220, 0, 255, file_manager);
	pView->parameter[pView->num_params++] = new CParameter(&avg_min2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2758b[lng], string_log[lng], 140, 0, 255, file_manager);
	#endif // REJECTION_SEPARATION

  pView->parameter[pView->num_params++] = new CParameter( &pos_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &neg_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &max_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],  200,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  100,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &maxerr_pos,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772[lng], string_unit[lng],  100,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxerr_neg,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774[lng], string_unit[lng],  100,    0, 99000, file_manager );
  //Pour added 201826
	#ifdef REJECTION_SEPARATION
  pView->parameter[pView->num_params++] = new CParameter(&maxerr_pos2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2772b[lng], string_unit[lng], 100, 0, 99000, file_manager);
  pView->parameter[pView->num_params++] = new CParameter(&maxerr_neg2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2774b[lng], string_unit[lng], 100, 0, 99000, file_manager);
	#endif // REJECTION_SEPARATION

  pView->parameter[pView->num_params++] = new CParameter( &maxerr_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780[lng], string_unit[lng],  100,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxerr_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );
  //Pour added 20180926
	#ifdef REJECTION_SEPARATION
  pView->parameter[pView->num_params++] = new CParameter(&maxerr_max2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2780b[lng], string_unit[lng], 100, 0, 99000, file_manager);
  pView->parameter[pView->num_params++] = new CParameter(&maxerr_min2, CTYPE_DINT, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2782b[lng], string_unit[lng], 100, 0, 99000, file_manager);
	#endif // REJECTION_SEPARATION

  //Pour added 20180927
    if ( index==3 ||index == 4 ) // parameters specific for CAP control
  {
    RECT area;
    pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
//    area.top -= 290;
    area.top     -=150;
	area.bottom  -=150;
    pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);
  }

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required

    name = string_2810;
    //wchar_t** name = string_2760; // standard 
    #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
      if ( index==5 ) name = string_2850;
    #endif

	  #if defined NEWCAP_ORALCAP && defined TIVS_F20 // for F20 machines
      if ( index==5 ) name = string_2850;
    #endif

    #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
      if ( index==7 ) name = string_2850;
    #endif

    /*
    if ( index==5 )
    {
      #ifdef NEWCAP_ORALCAP
        name = string_2852;
      #endif
    }
    */

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
    file_manager->Add_separator( name[lng] );
//    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2810[lng], child );
//    file_manager->Add_separator( string_2810[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    #ifdef DOUBLE_BOTTOM_ANALYSIS
      pView->parameter[pView->num_params++] = new CParameter( &src_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2766[lng], string_pix [lng],  105,    0,   255, file_manager );
    #endif

    pView->parameter[pView->num_params++] = new CParameter( &min2_radius,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2812[lng], string_pix [lng],  190,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max2_radius,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2814[lng], string_pix [lng],  200,    0,   255, file_manager );

    #ifndef DOUBLE_BOTTOM_ANALYSIS
      pView->parameter[pView->num_params++] = new CParameter( &avg2_max,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756[lng], string_log [lng],  220,    0,   255, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &avg2_min,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758[lng], string_log [lng],  140,    0,   255, file_manager );
    #endif

	//Pour 20180926
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &avg2_max2,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756b[lng], string_log [lng],  220,    0,   255, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &avg2_min2,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758b[lng], string_log [lng],  140,    0,   255, file_manager );
	#endif
	//Pour 20180926

    pView->parameter[pView->num_params++] = new CParameter( &pos2_dif,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   20,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &neg2_dif,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );


    pView->parameter[pView->num_params++] = new CParameter( &max2_lum,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],  200,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min2_lum,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  100,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_pos,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_neg,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774[lng], string_unit[lng],  100,    0, 99000, file_manager );

	//Pour 20170826
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &maxerr2_pos2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772b[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_neg2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774b[lng], string_unit[lng],  100,    0, 99000, file_manager );
	#endif

    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );

	//Pour 20170915
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &maxerr2_max2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780b[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_min2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782b[lng], string_unit[lng],  100,    0, 99000, file_manager );
	#endif

    pView->num_menu_btn++;

    //pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  #endif

  
  return 0; // completed successfully
}




int CBottom_inspection::Process_image( BYTE* img_acq, UINT nimg )
{
  int x, y;

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;

  img  = img_acq;

  if ( nimg>=0 && nimg<60 ) nhead = nimg;
  else                      nhead = 0;

  SCALE_IMG( index, img_acq );

  CTimer timer;

  /*
  #ifdef SAVE_FULL_PROC
    image_dsp[index]->scale_fulldsp(img_acq); // contour overlayed
    //for ( UINT i=0 ; i<image_dsp[index]->buf_size ; i++ ) image_dsp[index]->ptr_fulldsp[i] = 0xFF000000;
  #endif
  */

  // draw inspection area first
//  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  for ( x=xmin ; x<=xmax ; x+=4 )
  {
    DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
  }
  for ( y=ymin ; y<=ymax ; y+=4 )
  {
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
  }

  int dimx = image_dsp[index]->dim_buf.cx;
  int dimy = image_dsp[index]->dim_buf.cy;

  ASSERT( dimx==dimy );

 // Allocate_serie( &vial, dimx );

  timer.reset();

  #if defined DOUBLE_BOTTOM_ANALYSIS // for F40 machines when double bottom is defined (for now only on B_A camera)
    if ( index==3 ) return( Bottom_double_analysis() ); // includes also double alignment
    if ( index==4 ) return( Bottom_double_analysis() ); // includes also double alignment
  #endif


  if ( !Search_center() ) // if alignment succeded
  {
    /*#ifdef NEWCAP_ORALCAP
      if ( index==5 ) return( Bottom_analysis_circle() );
    #endif */
    #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
      if ( index==5 ) return( Bottom_analysis_circle() );
    #endif

	  #if defined NEWCAP_ORALCAP && defined TIVS_F20 // for F20 machines
      if ( index==5 ) return( Bottom_analysis_circle() );
    #endif


    #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
      if ( index==7 ) return( Bottom_analysis_circle() );
    #endif

    return( Bottom_analysis() );
  }


//  TRACE("max (%d,%d) completed in %d - average %d/each us\n", xmx, ymx, timer.elapsed_us(), timer.elapsed_us()/((2*amp+1)*(2*amp+1)) );

  return 999; // no inspection mode selected
}




// Different version of bottom control where average is calculated circle by circle, not on whole inner
int CBottom_inspection::Bottom_analysis_circle( void )
{
  err_pos      = 0;
  err_neg      = 0;
  err_max      = 0;
  err_min      = 0;

  UINT lum_tot = 0;
  UINT npt_tot = 0;

  UINT col_pos = COL_RED;     // 0xFFFF0000; // AARRGGBB - RED      - POSITIVE DIFFERENCE
  UINT col_neg = COL_CYAN;    // 0xFF00FFFF; // AARRGGBB - CYAN     - NEGATIVE DIFFERENCE
  UINT col_max = COL_MAGENTA; // 0xFFFF00FF; // AARRGGBB - MAGENTA  - MAX ABSOLUTE
  UINT col_min = COL_ORANGE;  // 0xFFFF7700; // AARRGGBB - ORANGE   - MIN ABSOLUTE


//  if ( !anl_radius ) return 0; // disabled

  // assign temporary radius for analysys
  anl_radtmp = anl_radius;

  #define INNER_RAD 20 // inner radius for analysis

  int r = 1;
  int i = 0;

  int ltot = 0;

  // INNER CIRCLE (1 to INNER_RAD)
  // First compute average brightness
//  for ( r=1 ; r<INNER_RAD ; r++ )

  /*
  int inr_radius = INNER_RAD;
  if ( anl_radius < INNER_RAD ) inr_radius = anl_radius;

//  for ( r=1 ; r<INNER_RAD ; r++ )
  for ( r=1 ; r<inr_radius ; r++ )
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    }
    npt_tot += 2*my_circle[r].n;
  }

  lum_tot = ltot;
  ltot = int( 0.5 + double(ltot)/npt_tot );
*/

  // Calculate average and error
  for ( r=5 ; r<anl_radius ; r++ )
  {
    // first calculate average
    ltot    = 0;
    npt_tot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    }
    npt_tot += 2*my_circle[r].n;
    ltot = int( 0.5 + double(ltot)/npt_tot );

    // then calculate error
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }


    }
  }


  // Draw INNER circle
/*  r = INNER_RAD;
  for ( i=0 ; i<my_circle[r].n ; i+=2 )
  {
    DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
    DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
  }  */

/*
  // OUTER CIRCLE ANALYSIS
  for ( r=INNER_RAD ; r<=anl_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }

    }

  }
  */

  err_pos /= 10;
  err_neg /= 10;
  err_max /= 10;
  err_min /= 10;


  /*
//  int avg_lum = (lum_tot+(npt_tot/2))/npt_tot;
  int avg_lum = npt_tot>0?(lum_tot+(npt_tot/2))/npt_tot:0;

  CString txt;
  if ( avg_lum<avg_min )
  {
    txt.Format( string_2784[lng], avg_lum, avg_min, avg_max );
    txt1.Append( txt );
    col1 = TXT_DARKRED;

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -125, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -125, nhead ); // 
      }
    #endif 

    return 129;
  }

  if ( avg_lum>avg_max )
  {
    txt.Format( string_2786[lng], avg_lum, avg_min, avg_max );
    txt1.Append( txt );
    col1 = TXT_DARKRED;

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -135, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -135, nhead ); // 
      }
    #endif 

    return 139;
  }

  txt.Format( string_2788[lng], avg_lum, avg_min, avg_max );
  txt1.Append( txt );
  */

  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( err_pos < maxerr_pos2 && index==3 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_pos >= maxerr_pos2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( err_pos < maxerr_pos2 && index==4 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_pos >= maxerr_pos2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20170914

    txt2.Format( string_2790[lng], err_pos, err_neg, err_max, err_min );
    return 109;
  }

  if ( err_neg > maxerr_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -155, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -155, nhead ); // 
      }
    #endif 

	//Pour added 20170914
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( err_neg < maxerr_neg2 && index==3 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_neg >= maxerr_neg2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( err_neg < maxerr_neg2 && index==4 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_neg >= maxerr_neg2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20170914

    txt2.Format( string_2792[lng], err_pos, err_neg, err_max, err_min );
    return 119;
  }

  if ( err_max > maxerr_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -165, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -165, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	//  Camera A
	if ( err_max < maxerr_max2 && index==3 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_max >= maxerr_max2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( err_max < maxerr_max2 && index==4 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_max >= maxerr_max2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2794[lng], err_pos, err_neg, err_max, err_min );
    return 129;
  }


  if ( err_min > maxerr_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -175, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -175, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( err_min < maxerr_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_min >= maxerr_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( err_min < maxerr_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if (err_min >= maxerr_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    return 139;
  }

  txt2.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  col2 = TXT_DARKGREEN;

  /*

  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required

  err2_pos      = 0;
  err2_neg      = 0;
  err2_max      = 0;
  err2_min      = 0;

  lum_tot       = 0;
  npt_tot       = 0;

  if ( !min2_radius || !max2_radius ) 
  {
    txt2.Append( string_disabled[lng] );
    return 0;
  }

  if ( xcnt-max2_radius<2 || xcnt+max2_radius>int(dimx-2) || ycnt-max2_radius<2 || ycnt+max2_radius>int(dimy-2) ) 
  {
    txt2.Append( string_disabled[lng] );
    return 0;
  }


  // assign temporary radius for analysys
  min2_radtmp = min2_radius;
  max2_radtmp = max2_radius;

    // IF DEFINED - EXTERNAL CIRCLE ANALYSIS (FOR CRACKS)
  for ( r=min2_radtmp ; r<=max2_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }

    }

  }


  for ( i=0 ; i<my_circle[min2_radtmp].n ; i+=4 )
  {
    DRAW_PT(index,xcnt+my_circle[min2_radtmp].x[i],ycnt+my_circle[min2_radtmp].y[i],COL_YELLOW);
    DRAW_PT(index,xcnt-my_circle[min2_radtmp].x[i],ycnt-my_circle[min2_radtmp].y[i],COL_YELLOW);
  }

  for ( i=0 ; i<my_circle[max2_radtmp].n ; i+=4 )
  {
    DRAW_PT(index,xcnt+my_circle[max2_radtmp].x[i],ycnt+my_circle[max2_radtmp].y[i],COL_YELLOW);
    DRAW_PT(index,xcnt-my_circle[max2_radtmp].x[i],ycnt-my_circle[max2_radtmp].y[i],COL_YELLOW);
  }


  err2_pos /= 10;
  err2_neg /= 10;
  err2_max /= 10;
  err2_min /= 10;

//  int avg2_lum = (lum_tot+(npt_tot/2))/npt_tot;
  int avg2_lum = npt_tot>0?(lum_tot+(npt_tot/2))/npt_tot:0;

  //CString txt;
  //txt1 = L"External";
  if ( avg2_lum<avg2_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -225, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -225, nhead ); // 
      }
    #endif 
    txt.Format( string_2784[lng], avg2_lum, avg2_min, avg2_max );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 229;
  }

  if ( avg2_lum>avg2_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -235, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -235, nhead ); // 
      }
    #endif 
    txt.Format( string_2786[lng], avg2_lum, avg2_min, avg2_max );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 239;
  }

  txt.Format( string_2788[lng], avg2_lum, avg2_min, avg2_max );
  txt2.Append( txt );


  if ( err2_pos > maxerr2_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -245, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -245, nhead ); // 
      }
    #endif 
    txt.Format( string_2790[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 209;
  }

  if ( err2_neg > maxerr2_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -255, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -255, nhead ); // 
      }
    #endif 
    txt.Format( string_2792[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 219;
  }

  if ( err2_max > maxerr2_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -265, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -265, nhead ); // 
      }
    #endif 
    txt.Format( string_2794[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 229;
  }

  if ( err2_min > maxerr2_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -275, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -275, nhead ); // 
      }
    #endif 
    txt.Format( string_2796[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 239;
  }

  txt.Format( string_2798[lng], err2_pos, err2_neg, err2_max, err2_min );
  txt2.Append( txt );
  col2 = TXT_DARKGREEN;


  #endif
*/

  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 
  return 0;
}







// New version of bottom inspection with double alignment and double inspection (inner/outer area)
int CBottom_inspection::Bottom_double_analysis( void )
{

  #ifdef DOUBLE_BOTTOM_ANALYSIS // If external ring analysis required

  int r = 1;
  int i = 0;
  int ltot = 0;

  UINT lum_tot = 0;
  UINT npt_tot = 0;

  UINT col_pos = COL_RED;     // 0xFFFF0000; // AARRGGBB - RED      - POSITIVE DIFFERENCE
  UINT col_neg = COL_CYAN;    // 0xFF00FFFF; // AARRGGBB - CYAN     - NEGATIVE DIFFERENCE
  UINT col_max = COL_MAGENTA; // 0xFFFF00FF; // AARRGGBB - MAGENTA  - MAX ABSOLUTE
  UINT col_min = COL_ORANGE;  // 0xFFFF7700; // AARRGGBB - ORANGE   - MIN ABSOLUTE

/*  if ( !anl_radius )
  {
    txt1.Format( string_2740[lng] );
    col1 = 0; // BLACK
    return 0; // disabled
  } */

  CString txt;

  if ( Search_center() ) // if first alignment failed, return
  {
    TRACE("First alignment failed\n");
    return 99; 
  }

//  col1 = TXT_DARKGREEN;


  // If not, proceed with analysis of outer ring (vial heel)

  err2_pos     = 0;
  err2_neg     = 0;
  err2_max     = 0;
  err2_min     = 0;

  lum_tot      = 0;
  npt_tot      = 0;

  if ( xcnt-max2_radius<2 || xcnt+max2_radius>int(dimx-2) || ycnt-max2_radius<2 || ycnt+max2_radius>int(dimy-2) ) 
  {
    txt2 = string_2742[lng];
    return 789;
  }

  if ( !min2_radius || !max2_radius ) 
  {
    //txt2 = string_disabled[lng];
    min2_radtmp = 0;
    max2_radtmp = 0;
    //col2 = 0; // BLACK COLOR IF DISABLED //TXT_DARKGREEN;
    //return 0;
  }
  else
  {
    // assign temporary radius for analysys
    min2_radtmp = min2_radius;
    max2_radtmp = max2_radius;
  }


    // IF DEFINED - EXTERNAL CIRCLE ANALYSIS (FOR CRACKS)
  for ( r=min2_radtmp ; r<=max2_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }

    }

  }

  // DRAW INSPECTION AREAS
  if ( min2_radtmp )
  {
    for ( i=0 ; i<my_circle[min2_radtmp].n ; i+=4 )
    {
      DRAW_PT(index,xcnt+my_circle[min2_radtmp].x[i],ycnt+my_circle[min2_radtmp].y[i],COL_YELLOW);
      DRAW_PT(index,xcnt-my_circle[min2_radtmp].x[i],ycnt-my_circle[min2_radtmp].y[i],COL_YELLOW);
    }
  }

  if ( max2_radtmp )
  {
    for ( i=0 ; i<my_circle[max2_radtmp].n ; i+=4 )
    {
      DRAW_PT(index,xcnt+my_circle[max2_radtmp].x[i],ycnt+my_circle[max2_radtmp].y[i],COL_YELLOW);
      DRAW_PT(index,xcnt-my_circle[max2_radtmp].x[i],ycnt-my_circle[max2_radtmp].y[i],COL_YELLOW);
    }
  }

  // ADJUST ERROR SCORES
  err2_pos /= 10;
  err2_neg /= 10;
  err2_max /= 10;
  err2_min /= 10;

  // CHECK RESULTS
  if ( err2_pos > maxerr2_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -245, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -245, nhead ); // 
      }
    #endif 
    //txt.Format( string_2790[lng], err2_pos, err2_neg, err2_max, err2_min );
    //txt2.Append( txt );
    txt2.Format( string_2790[lng], err2_pos, err2_neg, err2_max, err2_min );
    //col2 = TXT_DARKRED;
    return 209;
  }

  if ( err2_neg > maxerr2_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -255, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -255, nhead ); // 
      }
    #endif 
    //txt.Format( string_2792[lng], err2_pos, err2_neg, err2_max, err2_min );
    //txt2.Append( txt );
    txt2.Format( string_2792[lng], err2_pos, err2_neg, err2_max, err2_min );
    //col2 = TXT_DARKRED;
    return 219;
  }

  if ( err2_max > maxerr2_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -265, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -265, nhead ); // 
      }
    #endif 
    //txt.Format( string_2794[lng], err2_pos, err2_neg, err2_max, err2_min );
    //txt2.Append( txt );
    txt2.Format( string_2794[lng], err2_pos, err2_neg, err2_max, err2_min );
    //col2 = TXT_DARKRED;
    return 229;
  }

  if ( err2_min > maxerr2_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -275, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -275, nhead ); // 
      }
    #endif 
    //txt.Format( string_2796[lng], err2_pos, err2_neg, err2_max, err2_min );
    //txt2.Append( txt );
    txt2.Format( string_2796[lng], err2_pos, err2_neg, err2_max, err2_min );
    //col2 = TXT_DARKRED;
    return 239;
  }


  if ( !min2_radius || !max2_radius ) 
  {
    txt2 = string_2740[lng];
  }
  else
  {
    //txt.Format( string_2798[lng], err2_pos, err2_neg, err2_max, err2_min );
    //txt2.Append( txt );
    txt2.Format( string_2798[lng], err2_pos, err2_neg, err2_max, err2_min );
    // col2 = TXT_DARKGREEN;
  }

  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 



  /////////////////////////////////////////////////////////////////////////////////////

  // IF EVERYTHING'S FINE AFTER OUTER RING ALIGN AND ANALIZE INNER RING (CAKE)
//  TRACE("A: Second alignment radius = %d\n", src_radius2 );

  if ( !src_radius2 ) 
  {
    txt1.Append( L" - " );
    txt1.Append( string_2746[lng] );

    xcnt2 = xcnt;
    ycnt2 = ycnt;

    //col1 = TXT_DARKGREEN;
    //return 0;
  }
  else
  {
    // assign temporary radius for analysys
    src_radtmp2 = src_radius2;

    if ( Search_center2() ) // if second alignment failed, return
    {
      col1 = TXT_DARKRED;
      TRACE("Second alignment failed\n");
      return 199; 
    }
  }



  //TRACE("B: Second alignment radius = %d\n", src_radius2 );

  if ( !anl_radius ) 
  {
    txt2.Append( L" - " );
    txt2.Append( string_2740[lng] );
    col2 = TXT_DARKGREEN;
    return 0;
  }

  anl_radtmp  = anl_radius;

  if ( xcnt2-anl_radius<2 || xcnt2+anl_radius>int(dimx-2) || ycnt2-anl_radius<2 || ycnt2+anl_radius>int(dimy-2) ) 
  {
    txt1.Append( L" - " );
    txt1.Append( string_2742[lng] );
    col1 = TXT_DARKRED;
    return 789;
  }


  col2 = TXT_DARKRED;

  // If not, proceed with analysis of INNER (cake) circle

  err_pos      = 0;
  err_neg      = 0;
  err_max      = 0;
  err_min      = 0;

  #define INNER_RAD 20 // inner radius for analysis

  // INNER CIRCLE (1 to INNER_RAD)
  // First compute average brightness
  lum_tot      = 0;
  npt_tot      = 0;

  int inr_radius = INNER_RAD;
  if ( anl_radius < INNER_RAD ) inr_radius = anl_radius;

  ltot = 0;
  for ( r=1 ; r<inr_radius ; r++ )
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
      ltot += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
    }
    npt_tot += 2*my_circle[r].n;
  }

  lum_tot = ltot;
  ltot = int( 0.5 + double(ltot)/npt_tot );

  // Second: check error
  for ( r=1 ; r<inr_radius ; r++ )
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_max);
      }

      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_max);
      }
    }
  }




  // OUTER CIRCLE ANALYSIS
  for ( r=inr_radius ; r<=anl_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
      ltot += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]);
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt2+my_circle[r].x[i],ycnt2+my_circle[r].y[i],col_max);
      }

      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]);
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt2-my_circle[r].x[i],ycnt2-my_circle[r].y[i],col_max);
      }

    }

  }

  err_pos /= 10;
  err_neg /= 10;
  err_max /= 10;
  err_min /= 10;

  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif 
    txt.Format( string_2790[lng], err_pos, err_neg, err_max, err_min );
    txt2.Append( L" - " );
    txt2.Append( txt );
    return 109;
  }

  if ( err_neg > maxerr_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -155, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -155, nhead ); // 
      }
    #endif 
    txt.Format( string_2792[lng], err_pos, err_neg, err_max, err_min );
    txt2.Append( L" - " );
    txt2.Append( txt );
    return 119;
  }

  if ( err_max > maxerr_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -165, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -165, nhead ); // 
      }
    #endif 
    txt.Format( string_2794[lng], err_pos, err_neg, err_max, err_min );
    txt2.Append( L" - " );
    txt2.Append( txt );
    return 129;
  }

  if ( err_min > maxerr_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -175, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -175, nhead ); // 
      }
    #endif 
    txt.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    txt2.Append( L" - " );
    txt2.Append( txt );
    return 139;
  }

  txt.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  txt2.Append( L" - " );
  txt2.Append( txt );
  col2 = TXT_DARKGREEN;

  #endif // DOUBLE_BOTTOM_ANALYSIS // If external ring analysis required


  return 0;
}




// Standard version of bottom inspection
int CBottom_inspection::Bottom_analysis( void )
{
  err_pos      = 0;
  err_neg      = 0;
  err_max      = 0;
  err_min      = 0;

  UINT lum_tot = 0;
  UINT npt_tot = 0;

  UINT col_pos = COL_RED;     // 0xFFFF0000; // AARRGGBB - RED      - POSITIVE DIFFERENCE
  UINT col_neg = COL_CYAN;    // 0xFF00FFFF; // AARRGGBB - CYAN     - NEGATIVE DIFFERENCE
  UINT col_max = COL_MAGENTA; // 0xFFFF00FF; // AARRGGBB - MAGENTA  - MAX ABSOLUTE
  UINT col_min = COL_ORANGE;  // 0xFFFF7700; // AARRGGBB - ORANGE   - MIN ABSOLUTE


//  if ( !anl_radius ) return 0; // disabled

  // assign temporary radius for analysys
  anl_radtmp = anl_radius;

  #define INNER_RAD 20 // inner radius for analysis

  int r = 1;
  int i = 0;

  int ltot = 0;

  // INNER CIRCLE (1 to INNER_RAD)
  // First compute average brightness
//  for ( r=1 ; r<INNER_RAD ; r++ )

  int inr_radius = INNER_RAD;
  if ( anl_radius < INNER_RAD ) inr_radius = anl_radius;

//  for ( r=1 ; r<INNER_RAD ; r++ )
  for ( r=1 ; r<inr_radius ; r++ )
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    }
    npt_tot += 2*my_circle[r].n;
  }

  lum_tot = ltot;
  ltot = int( 0.5 + double(ltot)/npt_tot );

  // Second: check error
//  for ( r=1 ; r<INNER_RAD ; r++ )
  for ( r=1 ; r<anl_radius ; r++ )
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }


    }
  }


  // Draw INNER circle
/*  r = INNER_RAD;
  for ( i=0 ; i<my_circle[r].n ; i+=2 )
  {
    DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
    DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
  }  */


  // OUTER CIRCLE ANALYSIS
  for ( r=INNER_RAD ; r<=anl_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif > 0  )
      {
        err_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err_min += min_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max_lum )
      {
        err_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }

    }

  }

  err_pos /= 10;
  err_neg /= 10;
  err_max /= 10;
  err_min /= 10;

//  int avg_lum = (lum_tot+(npt_tot/2))/npt_tot;
  int avg_lum = npt_tot>0?(lum_tot+(npt_tot/2))/npt_tot:0;

  CString txt;
  if ( avg_lum<avg_min )
  {
    txt.Format( string_2784[lng], avg_lum, avg_min, avg_max );
    txt1.Append( txt );
    col1 = TXT_DARKRED;

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( avg_lum>avg_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg_lum<=avg_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( avg_lum>avg_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg_lum<=avg_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -125, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -125, nhead ); // 
      }
    #endif 

    return 129;
  }

  if ( avg_lum>avg_max )
  {
    txt.Format( string_2786[lng], avg_lum, avg_min, avg_max );
    txt1.Append( txt );
    col1 = TXT_DARKRED;

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( avg_lum<avg_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg_lum>=avg_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	 // Camera B
	if ( avg_lum<avg_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg_lum>=avg_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -135, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -135, nhead ); // 
      }
    #endif 

    return 139;
  }

  txt.Format( string_2788[lng], avg_lum, avg_min, avg_max );
  txt1.Append( txt );


  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( err_pos < maxerr_pos2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err_pos >= maxerr_pos2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}

	// Camera B
	if ( err_pos < maxerr_pos2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err_pos >= maxerr_pos2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2790[lng], err_pos, err_neg, err_max, err_min );
    return 109;
  }

  if ( err_neg > maxerr_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -155, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -155, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( err_neg < maxerr_neg2 && index==3)
	{
	ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err_neg >= maxerr_neg2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( err_neg < maxerr_neg2 && index==4)
	{
	ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err_neg >= maxerr_neg2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2792[lng], err_pos, err_neg, err_max, err_min );
    return 119;
  }

  if ( err_max > maxerr_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -165, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -165, nhead ); // 
      }
    #endif 

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camara A
	if ( err_max < maxerr_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_max >= maxerr_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camara B
	if ( err_max < maxerr_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_max >= maxerr_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2794[lng], err_pos, err_neg, err_max, err_min );
    return 129;
  }

  if ( err_min > maxerr_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -175, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -175, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	  if ( err_min <maxerr_min2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_min >= maxerr_min2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	  if ( err_min <maxerr_min2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( err_min >= maxerr_min2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt2.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    return 139;
  }

  txt2.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  col2 = TXT_DARKGREEN;



  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required

  err2_pos      = 0;
  err2_neg      = 0;
  err2_max      = 0;
  err2_min      = 0;

  lum_tot       = 0;
  npt_tot       = 0;

  if ( !min2_radius || !max2_radius ) 
  {
    txt2.Append( string_disabled[lng] );
    return 0;
  }

  if ( xcnt-max2_radius<2 || xcnt+max2_radius>int(dimx-2) || ycnt-max2_radius<2 || ycnt+max2_radius>int(dimy-2) ) 
  {
    txt2.Append( string_disabled[lng] );
    return 0;
  }


  // assign temporary radius for analysys
  min2_radtmp = min2_radius;
  max2_radtmp = max2_radius;

    // IF DEFINED - EXTERNAL CIRCLE ANALYSIS (FOR CRACKS)
  for ( r=min2_radtmp ; r<=max2_radtmp ; r++ )
  {
    ltot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
      //TRACE("%d,%d\n", i, PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    lum_tot += ltot;
    ltot = int( 0.5 + 0.5*ltot/my_circle[r].n );
    npt_tot += 2*my_circle[r].n;

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err2_neg += ltot-neg2_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif > 0  )
      {
        err2_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-pos2_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( min2_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        err2_min += min2_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > max2_lum )
      {
        err2_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-max2_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }

    }

  }


  for ( i=0 ; i<my_circle[min2_radtmp].n ; i+=4 )
  {
    DRAW_PT(index,xcnt+my_circle[min2_radtmp].x[i],ycnt+my_circle[min2_radtmp].y[i],COL_YELLOW);
    DRAW_PT(index,xcnt-my_circle[min2_radtmp].x[i],ycnt-my_circle[min2_radtmp].y[i],COL_YELLOW);
  }

  for ( i=0 ; i<my_circle[max2_radtmp].n ; i+=4 )
  {
    DRAW_PT(index,xcnt+my_circle[max2_radtmp].x[i],ycnt+my_circle[max2_radtmp].y[i],COL_YELLOW);
    DRAW_PT(index,xcnt-my_circle[max2_radtmp].x[i],ycnt-my_circle[max2_radtmp].y[i],COL_YELLOW);
  }


  err2_pos /= 10;
  err2_neg /= 10;
  err2_max /= 10;
  err2_min /= 10;

//  int avg2_lum = (lum_tot+(npt_tot/2))/npt_tot;
  int avg2_lum = npt_tot>0?(lum_tot+(npt_tot/2))/npt_tot:0;

  //CString txt;
  //txt1 = L"External";
  if ( avg2_lum<avg2_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -225, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -225, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( avg2_lum>avg2_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg2_lum<=avg2_min2 && index==3)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( avg2_lum>avg2_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg2_lum<=avg2_min2 && index==4)
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2784[lng], avg2_lum, avg2_min, avg2_max );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 229;
  }

  if ( avg2_lum>avg2_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -235, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -235, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( avg2_lum<avg2_max2 && index==3 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg2_lum>=avg2_max2 && index==3 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( avg2_lum<avg2_max2 && index==4 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if ( avg2_lum>=avg2_max2 && index==4 )
	{
	  ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2786[lng], avg2_lum, avg2_min, avg2_max );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 239;
  }

  txt.Format( string_2788[lng], avg2_lum, avg2_min, avg2_max );
  txt2.Append( txt );


  if ( err2_pos > maxerr2_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -245, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -245, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if ( err_pos < maxerr2_pos2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_pos >= maxerr2_pos2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if ( err_pos < maxerr2_pos2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_pos >= maxerr2_pos2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2790[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 209;
  }

  if ( err2_neg > maxerr2_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -255, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -255, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( err2_neg < maxerr2_neg2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_neg >= maxerr2_neg2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( err2_neg < maxerr2_neg2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_neg >= maxerr2_neg2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2792[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 219;
  }

  if ( err2_max > maxerr2_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -265, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -265, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	//Camera A
	if (err2_max < maxerr2_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_max >= maxerr2_max2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if (err2_max < maxerr2_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_max >= maxerr2_max2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2794[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 229;
  }

  if ( err2_min > maxerr2_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -275, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -275, nhead ); // 
      }
    #endif

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	// Camera A
	if (err2_min < maxerr2_min2 && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_min >= maxerr2_min2  && index==3)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	// Camera B
	if (err2_min < maxerr2_min2 && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	if ( err2_min >= maxerr2_min2  && index==4)
	{
	 ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926

    txt.Format( string_2796[lng], err2_pos, err2_neg, err2_max, err2_min );
    txt2.Append( txt );
    col2 = TXT_DARKRED;
    return 239;
  }

  txt.Format( string_2798[lng], err2_pos, err2_neg, err2_max, err2_min );
  txt2.Append( txt );
  col2 = TXT_DARKGREEN;


  #endif

  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 
  return 0;
}


int CBottom_inspection::Search_center( void )
{

  // initial search coords
  xcnt   = (xmin+xmax)/2; // 263;
  ycnt   = (ymin+ymax)/2; // 254;


  if ( !src_radius )
  {
    txt1 = string_2746[lng];
    return 0;
  }

  int    i;
  int    x      = xcnt;
  int    y      = ycnt;
  int    maxdir = 0;
  int    olddir = 0;
  int    niter  = 0; // number of iterations to decide deadlock
  double maxval = 0.0;

  src_radtmp = src_radius;

  do
  {
//    Calculate_scores( x, y );
    Calculate_scores( x, y, src_radtmp ); // test!

    maxdir = 0;
    maxval = score[0];
    for ( i=1 ; i<5 ; i++ )
    {
      if ( score[i]>maxval )
      {
        maxval=score[i];
        maxdir=i;
      }
    }

    if ( maxdir==olddir )
    {
      //TRACE("UGUALI\n");
      maxdir = 0;
    }

    switch ( maxdir )
    {
      case 1: // max to the right
        x++;
        olddir = 3;
        //TRACE("moving RIGHT to (%d,%d)\n",x,y);
        break;
      case 2: // max to bottom
        y++;
        olddir = 4;
        //TRACE("moving DOWN to (%d,%d)\n",x,y);
        break;
      case 3: // max to the left
        x--;
        olddir = 1;
        //TRACE("moving LEFT to (%d,%d)\n",x,y);
        break;
      case 4: // max to top
        y--;
        olddir = 2;
        //TRACE("moving UP   to (%d,%d)\n",x,y);
        break;
    } 
    
    niter++;

  //    if ( x==xo && y==yo ) TRACE("UGUALI\n");
  } while( maxdir>0 && x>xmin+src_radtmp && x<xmax-src_radtmp && y>ymin+src_radtmp && y<ymax-src_radtmp && niter<500 );

  //TRACE("niter=%d\n",niter);


  int lummin = 10;
//  lummin = 10;

  if ( rv[0]>lummin && bv[0]>lummin && lv[0]>lummin && tv[0]>lummin )
  {

    xcnt = x;
    ycnt = y;

    txt1.Format( string_2752[lng],xcnt,ycnt);
    col1 = TXT_DARKGREEN;

    for ( i=4 ; i<my_circle[src_radtmp].n ; i+=8 )  // Draw found circle
    {
      DRAW_CROSS(index,xcnt+my_circle[src_radtmp].x[i],ycnt+my_circle[src_radtmp].y[i],COL_BLUE);
      DRAW_CROSS(index,xcnt-my_circle[src_radtmp].x[i],ycnt-my_circle[src_radtmp].y[i],COL_BLUE);
    }

    return 0; // search completed successfully
  }
  else
  {

	//Pour added 20180926
	#ifdef REJECTION_SEPARATION
	if(index==3)
	{
	ADS_write_value(".F_ENABLE_BOTTOMA", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	if(index==4)
	{
	ADS_write_value(".F_ENABLE_BOTTOMB", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180926 

    txt1.Format( string_2754[lng], rv[0], bv[0], lv[0], tv[0] ); // Center NOT found

    double cf = double(ymax-ymin)/(xmax-xmin);
    for ( x=xmin+50 ; x<=xmax-50 ; x+=4 )
    {
      DRAW_PT( index, x, ymin+int(cf*(x-xmin)), COL_RED );
      DRAW_PT( index, x, ymax-int(cf*(x-xmin)), COL_RED );
    }

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -105, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -105, nhead ); // 
      }
    #endif 


    return 99; // too low: not validated
  }

//  TRACE("Max corrispondenza in %d,%d (%d us)\n", x, y, timer.elapsed_us() );
//  return 0; // search completed successfully
}

#undef  xmin2
#undef  xmax2
#undef  ymin2
#undef  ymax2


int CBottom_inspection::Search_center2( void )
{

  // initial search coords from outer
  xcnt2 = xcnt;
  ycnt2 = ycnt;

  src_radtmp2 = src_radius2;

  int    i;
  int    x      = xcnt2;
  int    y      = ycnt2;
  int    maxdir = 0;
  int    olddir = 0;
  int    niter  = 0; // number of iterations to decide deadlock
  double maxval = 0.0;


  int    xmin2 = xcnt - src_radtmp + src_radtmp2 - 10;
  int    xmax2 = xcnt + src_radtmp - src_radtmp2 + 10;
  int    ymin2 = ycnt - src_radtmp + src_radtmp2 - 10;
  int    ymax2 = ycnt + src_radtmp - src_radtmp2 + 10;

  do
  {
    Calculate_scores( x, y, src_radtmp2 );

    maxdir = 0;
    maxval = score[0];
    for ( i=1 ; i<5 ; i++ )
    {
      if ( score[i]>maxval )
      {
        maxval=score[i];
        maxdir=i;
      }
    }

    if ( maxdir==olddir )
    {
      //TRACE("UGUALI\n");
      maxdir = 0;
    }

    switch ( maxdir )
    {
      case 1: // max to the right
        x++;
        olddir = 3;
        //TRACE("moving RIGHT to (%d,%d)\n",x,y);
        break;
      case 2: // max to bottom
        y++;
        olddir = 4;
        //TRACE("moving DOWN to (%d,%d)\n",x,y);
        break;
      case 3: // max to the left
        x--;
        olddir = 1;
        //TRACE("moving LEFT to (%d,%d)\n",x,y);
        break;
      case 4: // max to top
        y--;
        olddir = 2;
        //TRACE("moving UP   to (%d,%d)\n",x,y);
        break;
    } 
    
    niter++;

  //    if ( x==xo && y==yo ) TRACE("UGUALI\n");
  } while( maxdir>0 && x>xmin2 && x<xmax2 && y>ymin2 && y<ymax2 && niter<500 );
//  } while( maxdir>0 && x>xmin2+src_radtmp2 && x<xmax2-src_radtmp2 && y>ymin2+src_radtmp2 && y<ymax2-src_radtmp2 && niter<500 );

  //TRACE("niter=%d\n",niter);


  int lummin = 10;

  CString txtb;

  if ( rv[0]>lummin && bv[0]>lummin && lv[0]>lummin && tv[0]>lummin )
  {

    xcnt2 = x;
    ycnt2 = y;

    //txt1.Format( string_2752[lng],xcnt,ycnt);
    txtb.Format( string_2753[lng],xcnt2,ycnt2);
    txt1.Append( txtb );
    col1 = TXT_DARKGREEN;

    for ( i=4 ; i<my_circle[src_radtmp2].n ; i+=8 )  // Draw found circle
    {
      DRAW_CROSS(index,xcnt2+my_circle[src_radtmp2].x[i],ycnt2+my_circle[src_radtmp2].y[i],COL_BLUE);
      DRAW_CROSS(index,xcnt2-my_circle[src_radtmp2].x[i],ycnt2-my_circle[src_radtmp2].y[i],COL_BLUE);
    }

    return 0; // search completed successfully
  }
  else
  {

    //txt1.Format( string_2754[lng], rv[0], bv[0], lv[0], tv[0] ); // Center NOT found
    txtb.Format( string_2755[lng], rv[0], bv[0], lv[0], tv[0] ); // Center2 NOT found
    txt1.Append( txtb );
    col1 = TXT_DARKRED;

    double cf = double(ymax-ymin)/(xmax-xmin);
    for ( x=xmin+50 ; x<=xmax-50 ; x+=4 )
    {
      DRAW_PT( index, x, ymin+int(cf*(x-xmin)), COL_RED );
      DRAW_PT( index, x, ymax-int(cf*(x-xmin)), COL_RED );
    }

    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -105, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -105, nhead ); // 
      }
    #endif 


    return 99; // too low: not validated
  }

//  TRACE("Max corrispondenza in %d,%d (%d us)\n", x, y, timer.elapsed_us() );
//  return 0; // search completed successfully
}





// 0=center - 1=right - 2=bottom - 3=left - 4=top
int CBottom_inspection::Calculate_scores( int x, int y )
{
  // central position's score
  rv[0] = sectbrgth(x,y,0); // 0=right 1=bottom 2=left 3=top 
  bv[0] = sectbrgth(x,y,1); // 0=right 1=bottom 2=left 3=top 
  lv[0] = sectbrgth(x,y,2); // 0=right 1=bottom 2=left 3=top 
  tv[0] = sectbrgth(x,y,3); // 0=right 1=bottom 2=left 3=top 
  //Calculate_score(0);
  score[0] = 0.0;
  score[1] = lv[0]?double(rv[0])/lv[0]:rv[0];  // toward right
  score[2] = tv[0]?double(bv[0])/tv[0]:bv[0];  // toward bottom
  score[3] = rv[0]?double(lv[0])/rv[0]:lv[0];  // toward left
  score[4] = bv[0]?double(tv[0])/bv[0]:tv[0];  // toward top

  return 0;
}


// 0=center - 1=right - 2=bottom - 3=left - 4=top
int CBottom_inspection::Calculate_scores( int x, int y, int r )
{
  // central position's score
  rv[0] = sectbrgth(x,y,r,0); // 0=right 1=bottom 2=left 3=top 
  bv[0] = sectbrgth(x,y,r,1); // 0=right 1=bottom 2=left 3=top 
  lv[0] = sectbrgth(x,y,r,2); // 0=right 1=bottom 2=left 3=top 
  tv[0] = sectbrgth(x,y,r,3); // 0=right 1=bottom 2=left 3=top 

  //Calculate_score(0);
  score[0] = 0.0;
  score[1] = lv[0]?double(rv[0])/lv[0]:rv[0];  // toward right
  score[2] = tv[0]?double(bv[0])/tv[0]:bv[0];  // toward bottom
  score[3] = rv[0]?double(lv[0])/rv[0]:lv[0];  // toward left
  score[4] = bv[0]?double(tv[0])/bv[0]:tv[0];  // toward top

  return 0;
}






/*
int CBottom_inspection::Calculate_score( int ind )
{
  ASSERT( ind>=0 && ind<5 );
//  score[ind] = rv[ind]*bv[ind]*lv[ind]*tv[ind];
//  score[ind] = 

  return 0; // search completed successfully
}
*/

UINT CBottom_inspection::sectbrgth( int xc, int yc, int side ) // 0=right 1=bottom 2=left 3=top 
{
  UINT lum = 0;
  int  i;

  ASSERT( img ); // must have been already assigned to img_acq
  ASSERT( my_circle[src_radtmp].n && my_circle[src_radtmp].x && my_circle[src_radtmp].y );

  if ( side==0 ) // right sector
  {
    for ( i=                       0 ; i<my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
    for ( i=3*my_circle[src_radtmp].n/4 ; i<my_circle[src_radtmp].n   ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  if ( side==1 ) // bottom sector
  {
    for ( i=my_circle[src_radtmp].n/4 ; i<3*my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
  }

  if ( side==2 ) // left sector
  {
    for ( i=3*my_circle[src_radtmp].n/4 ; i<my_circle[src_radtmp].n   ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
    for ( i=                       0 ; i<my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  if ( side==3 ) // top sector
  {
    for ( i=my_circle[src_radtmp].n/4 ; i<3*my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  return( 2*lum/my_circle[src_radtmp].n );
}


UINT CBottom_inspection::sectbrgth( int xc, int yc, int rc, int side ) // 0=right 1=bottom 2=left 3=top 
{
  UINT lum = 0;
  int  i;

  ASSERT( img ); // must have been already assigned to img_acq
  ASSERT( my_circle[rc].n && my_circle[rc].x && my_circle[rc].y );

  if ( side==0 ) // right sector
  {
    for ( i=               0 ; i<my_circle[rc].n/4 ; i++ ) lum += PT2( xc+my_circle[rc].x[i] , yc+my_circle[rc].y[i] );
    for ( i=3*my_circle[rc].n/4 ; i<my_circle[rc].n   ; i++ ) lum += PT2( xc-my_circle[rc].x[i] , yc-my_circle[rc].y[i] );
  }

  if ( side==1 ) // bottom sector
  {
    for ( i=my_circle[rc].n/4 ; i<3*my_circle[rc].n/4 ; i++ ) lum += PT2( xc+my_circle[rc].x[i] , yc+my_circle[rc].y[i] );
  }

  if ( side==2 ) // left sector
  {
    for ( i=3*my_circle[rc].n/4 ; i<my_circle[rc].n   ; i++ ) lum += PT2( xc+my_circle[rc].x[i] , yc+my_circle[rc].y[i] );
    for ( i=               0 ; i<my_circle[rc].n/4 ; i++ ) lum += PT2( xc-my_circle[rc].x[i] , yc-my_circle[rc].y[i] );
  }

  if ( side==3 ) // top sector
  {
    for ( i=my_circle[rc].n/4 ; i<3*my_circle[rc].n/4 ; i++ ) lum += PT2( xc-my_circle[rc].x[i] , yc-my_circle[rc].y[i] );
  }

  return( 2*lum/my_circle[rc].n );
}


