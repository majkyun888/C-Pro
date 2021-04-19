#pragma once

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "OralFO_inspection.h"
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




COralFO_inspection::COralFO_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in )
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

  contour_thr   =   50; // contour threshold
  max_dist      = 1000; // minimum pos/neg sq distance from ideal circle to be considered error
  max_errprof   =  200; // maximum allowed profile error


  #ifdef SEPARATEAREAS // separate inner and outer areas of inspection

  ianl_radius    =  100;

//  iavg_max       =  220;
//  iavg_min       =  160;
  
  ipos_dif       =   40;
  ineg_dif       =   40;
  imax_lum       =  200;
  imin_lum       =   20;

  imaxerr_pos    = 1000;
  imaxerr_neg    = 1000;
  imaxerr_max    = 1000;
  imaxerr_min    = 1000;

  #endif SEPARATEAREAS // separate inner and outer areas of inspection




  /*
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
  */




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


COralFO_inspection::~COralFO_inspection(void)
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
int COralFO_inspection::Allocate_profile( void )
{
//  if ( profile ) free( profile );
//  prosize = xmax-xmin+1;
//  profile = (segm_type*) malloc( sizeof(segm_type)*prosize );
  return 0;
}
*/


int COralFO_inspection::Set_inspection_area( int ind )
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

  TRACE("COralFO_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int COralFO_inspection::Add_menu_parameters( CContainer_wnd* child )
{
  wchar_t** name = string_2850; // standard 
/*  #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
    if ( index==5 ) name = string_2850;
  #endif
  #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
    if ( index==7 ) name = string_2850;
  #endif */


  /*
  contour_thr   =   50; // contour threshold
  max_dist      = 1000; // minimum pos/neg sq distance from ideal circle to be considered error
  max_errprof   =  200; // maximum allowed profile error
  */
  
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
  file_manager->Add_separator( name[lng] );
//  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2760[lng], child );
//  file_manager->Add_separator( string_2760[lng] );

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

// OLD pView->parameter[pView->num_params++] = new CParameter( &src_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2766[lng], string_pix [lng],  105,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &contour_thr,   CTYPE_BYTE,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702 [lng], string_log [lng],   50,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_dist,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2550[lng], string_unit[lng], 1500,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_errprof,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2554[lng], string_unit[lng],  150,    0, 99000, file_manager );
  
  pView->parameter[pView->num_params++] = new CParameter( &anl_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],  105,    0,   255, file_manager );

//  pView->parameter[pView->num_params++] = new CParameter( &avg_max,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756[lng], string_log [lng],  220,    0,   255, file_manager );
//  pView->parameter[pView->num_params++] = new CParameter( &avg_min,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758[lng], string_log [lng],  140,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &pos_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &neg_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &max_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],  200,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  100,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &maxerr_pos,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772[lng], string_unit[lng],  100,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxerr_neg,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774[lng], string_unit[lng],  100,    0, 99000, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &maxerr_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780[lng], string_unit[lng],  100,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxerr_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  #ifdef SEPARATEAREAS // separate inner and outer areas of inspection

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2860[lng], child );
    file_manager->Add_separator( name[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &ianl_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],  105,    0,   255, file_manager );

//    pView->parameter[pView->num_params++] = new CParameter( &iavg_max,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756[lng], string_log [lng],  220,    0,   255, file_manager );
//    pView->parameter[pView->num_params++] = new CParameter( &iavg_min,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758[lng], string_log [lng],  140,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &ipos_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   20,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &ineg_dif,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &imax_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],  200,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &imin_lum,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  100,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &imaxerr_pos,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &imaxerr_neg,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774[lng], string_unit[lng],  100,    0, 99000, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &imaxerr_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &imaxerr_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );

    pView->num_menu_btn++;


  #endif SEPARATEAREAS // separate inner and outer areas of inspection






/*  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required

    name = string_2810;
    //wchar_t** name = string_2760; // standard 
    #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
      if ( index==5 ) name = string_2850;
    #endif
    #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
      if ( index==7 ) name = string_2850;
    #endif


    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
    file_manager->Add_separator( name[lng] );
//    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2810[lng], child );
//    file_manager->Add_separator( string_2810[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &min2_radius,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2812[lng], string_pix [lng],  190,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max2_radius,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2814[lng], string_pix [lng],  200,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &avg2_max,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2756[lng], string_log [lng],  220,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &avg2_min,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2758[lng], string_log [lng],  140,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &pos2_dif,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   20,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &neg2_dif,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &max2_lum,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],  200,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min2_lum,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  100,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_pos,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2772[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_neg,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2774[lng], string_unit[lng],  100,    0, 99000, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2780[lng], string_unit[lng],  100,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr2_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );

    pView->num_menu_btn++;

    //pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  #endif*/

  
  return 0; // completed successfully
}




int COralFO_inspection::Process_image( BYTE* img_acq, UINT nimg )
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


  if ( !FO_contour() )
  {
    return( FO_analysis_circle() );
  }


  /*
  // ORIGINAL CODE FROM BOTTOM 

  if ( !Search_center() ) // if alignment succeded
  {
    FO_contour();
    return( FO_analysis_circle() );
    return( FO_analysis() );
  }

*/
  /*
  if ( !Search_alignment() ) // if alignment succeded
  {
    return( FO_analysis() );
  }
  */

//  TRACE("max (%d,%d) completed in %d - average %d/each us\n", xmx, ymx, timer.elapsed_us(), timer.elapsed_us()/((2*amp+1)*(2*amp+1)) );

  return 999; // contour failed, return REJECT
}


int COralFO_inspection::Search_alignment( void )
{
  #define FLTMAX(i) 2*profx[i]-profx[i-3]-profx[i+3]
  int x, y;
  int soglia = 4000; // detect maximum only when above 

  int  dimx  = xmax - xmin + 1;
  int* profx = (int*) malloc( dimx*sizeof(int) );
  ZeroMemory( profx, dimx*sizeof(int) );

  for ( x=xmin ; x<=xmax ; x++ )
  {
    for ( y=ymin ; y<=ymax ; y++ ) profx[x-xmin]+=PT2(x,y);
  }

  TRACE("PROF X\n");

  for ( x=3 ; x<dimx-3 ; x++ )
  {
    TRACE( "%d,%d\n", x+xmin, profx[x] ); 
    //if ( FLTMAX(x) > soglia ) TRACE( "%d,%d\n", x+xmin, FLTMAX(x) );
  }

  free( profx );


  int  dimy  = ymax - ymin + 1;
  int* profy = (int*) malloc( dimy*sizeof(int) );
  ZeroMemory( profy, dimy*sizeof(int) );

  for ( y=ymin ; y<=ymax ; y++ )
  {
    for ( x=xmin ; x<=xmax ; x++ ) profy[y-ymin]+=PT2(x,y);
  }

  TRACE("PROF Y\n");

  for ( y=3 ; y<dimy-3 ; y++ )
  {
    TRACE( "%d,%d\n", y+ymin, profy[y] ); 
    //if ( FLTMAX(y) > soglia ) TRACE( "%d,%d\n", y+ymin, FLTMAX(y) );
  }

  free( profy );





  return 0; // successful
}





// Different version of bottom control where average is calculated circle by circle, not on whole inner
int COralFO_inspection::FO_analysislenght_circle( void )
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

  // assign temporary radius for analysys
  anl_radtmp = anl_radius;

  #define INNER_RAD 20 // inner radius for analysis
  #define ANLNG     10 // fore/back dist

//  int r = 1;
  int r = 1;
  int i = 0;
  int ltot = 0;

  int npt = my_circle[anl_radius].n*2 + ANLNG*2 + 100;

  BYTE* buffer = (BYTE*) malloc( npt );

  #define FLT(i) 2*buffer[i+ANLNG]-buffer[i]-buffer[i+2*ANLNG]
  #define MINRAD 5

  // Calculate average and error
  for ( r=MINRAD ; r<anl_radius ; r++ )
  {

    int j=0;
    for ( i=my_circle[r].n-ANLNG ; i<my_circle[r].n ; i++ ) buffer[j++]=PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    for ( i=                0 ; i<my_circle[r].n ; i++ ) buffer[j++]=PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
    for ( i=                0 ; i<my_circle[r].n ; i++ ) buffer[j++]=PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    for ( i=                0 ; i<ANLNG       ; i++ ) buffer[j++]=PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
   
    ASSERT( j<npt );
    /*    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      TRACE("%d,%d,%d,%d\n", i, my_circle[r].x[i], my_circle[r].y[i], PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) );
    }

    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      TRACE("%d,%d,%d,%d\n", i, my_circle[r].x[i], my_circle[r].y[i], PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) );
    }  */

    /* for ( i=0 ; i<2*my_circle[r].n ; i++ )
    {
      TRACE("%d,%d\n", i, FLT(i) );
    }*/

    // first calculate average
/*    ltot    = 0;
    npt_tot = 0;
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ltot += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
      ltot += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
    }
    npt_tot += 2*my_circle[r].n;
    ltot = int( 0.5 + double(ltot)/npt_tot );  */

    //ASSERT( my_circle[r].n>2*ANLNG+1 );

    // then calculate error
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {

      // negative RELATIVE difference 
      if ( ltot-neg_dif-FLT(i) > 0  )
      {
        err_neg += ltot-neg_dif-FLT(i);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference 
      if ( FLT(i)-ltot-pos_dif > 0  )
      {
        err_pos += FLT(i)-ltot-pos_dif;
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
    }


    // then calculate error
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      j = i + my_circle[r].n;

      // negative RELATIVE difference 
      if ( ltot-neg_dif-FLT(j) > 0  )
      {
        err_neg += ltot-neg_dif-FLT(j);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference 
      if ( FLT(j)-ltot-pos_dif > 0  )
      {
        err_pos += FLT(j)-ltot-pos_dif;
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

 /*     // ******************  BOTTOM HALF CIRCLE *******************

      // negative RELATIVE difference 
      if ( ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference 
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

      // ******************  TOP HALF CIRCLE *******************


      // negative difference 
      if ( ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        err_neg += ltot-neg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference 
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
      }   */

 //   }
  }

  if ( buffer ) 
  {
    free( buffer );
    buffer = NULL;
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
    txt2.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    return 139;
  }

  txt2.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  col2 = TXT_DARKGREEN;


  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 
  return 0;
}



int COralFO_inspection::FO_contour( void )
{
 // BYTE contour_thr = 50;
 
  UINT x,y;
  UINT xscanmax;

  /*
  // make sure we have some clearance around src_radius
  ASSERT( xcnt>src_radius+20 && xcnt<dimx-src_radius-20 );
  ASSERT( ycnt>src_radius+20 && ycnt<dimy-src_radius-20 );

  // search first point (starting from aligment)
  y = ycnt;
  x = xcnt-src_radius-10;
  xscanmax = xcnt-src_radius+10;
  */

/*  while ( x<xcnt-src_radius+10 )
  {
    TRACE("%x,%d\n", x, y );
    x++;
  } */

  if ( contour_thr==0 )
  {
    col1 = 0;
    txt1 = string_2572[lng];
    return 0;
  }



  // starting from search area
  x = xmin;
  y = (ymin+ymax)/2;
  xscanmax = (xmin+xmax)/2;


  while ( ( PT2(x,y)>contour_thr || PT2(x+1,y)<=contour_thr ) && x<xscanmax ) x++;
  
  if ( x>=xscanmax )
  {
    //TRACE("Cannot find first point for contouring\n");
    //txt1.Format();
    txt1 = string_2560[lng];
    return 99;
  }

  //TRACE("First point %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptn    = 0;
  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

  #define PTD(i) PT2(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
//  #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
  UINT xtot = 0;
  UINT ytot = 0;
  int xcmin = xmax;
  int xcmax = xmin;
  int ycmin = ymax;
  int ycmax = ymin;

  int radius = 0;

  int i = 6; // start moving upward - CW search
//  while ( ptn<400 && ptx[ptn]>xmin+10 && ptx[ptn]<xmax-10 && pty[ptn]>ymin+10 && pty[ptn]<ymax-10 )
  while ( ptn<NPTCNT-1 && ptx[ptn]>xmin && ptx[ptn]<xmax && pty[ptn]>ymin && pty[ptn]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i2;
    while ( i>=i1 && !( PTD(i+1)>contour_thr && PTD(i)<=contour_thr ) ) i--;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];

   // TRACE("%d,%d,%d\n", ptn, ptx[ptn+1], pty[ptn+1] );
    ptn++;

    xtot += ptx[ptn];
    ytot += pty[ptn];

    if ( ptx[ptn]>xcmax ) xcmax = ptx[ptn];
    if ( ptx[ptn]<xcmin ) xcmin = ptx[ptn];

    if ( pty[ptn]>ycmax ) ycmax = pty[ptn];
    if ( pty[ptn]<ycmin ) ycmin = pty[ptn];

   // int dst = (ptx[ptn]-xcnt)*(ptx[ptn]-xcnt)+(pty[ptn]-ycnt)*(pty[ptn]-ycnt)-src_radius*src_radius;
   // TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn], pty[ptn], dst );

    DRAW_PT(index,ptx[ptn],pty[ptn],0xFFFF7700); // 0xAARRGGBB

    if ( ptx[ptn]==ptx[0] && pty[ptn]==pty[0] ) // check closing
    {
      xcnt   = int( 0.5 + xtot/ptn );
      ycnt   = int( 0.5 + ytot/ptn );
      radius = int( 0.5 + 0.25*(xcmax-xcmin+ycmax-ycmin) ); // - 1;

      if ( xcnt-radius<20 || xcnt+radius>int(dimx-20) || ycnt-radius<20 || ycnt+radius>int(dimy-20) )
      {
        txt2 = string_2800[lng];
        return 109;
      }

      for ( i=0 ; i<my_circle[radius].n ; i+=4 )  // Draw found circle
      {
        DRAW_PT(index,xcnt+my_circle[radius].x[i],ycnt+my_circle[radius].y[i],0xFF9999FF);
        DRAW_PT(index,xcnt-my_circle[radius].x[i],ycnt-my_circle[radius].y[i],0xFF9999FF);
      } 

      int errprof = 0;
      for ( i=0 ; i<ptn ; i++ )  // Check distance from ideal circle
      {
        int dst = (ptx[i]-xcnt)*(ptx[i]-xcnt)+(pty[i]-ycnt)*(pty[i]-ycnt)-radius*radius;
//        dstot += dst;

        if ( dst>max_dist )
        {
          DRAW_PT(index,ptx[i],pty[i],0xFFFF0000); // 0xAARRGGBB
          errprof += dst-max_dist;
        }
        if ( dst<-max_dist )
        {
          DRAW_PT(index,ptx[i],pty[i],0xFFFF00FF); // 0xAARRGGBB
          errprof += -dst-max_dist;
        }

        //TRACE("%d,%d,%d,%d\n", i, ptx[i], pty[i], dst );
      }

      errprof = int( sqrt( double( errprof ) ) );

      if ( errprof>max_errprof )
      {
        txt1.Format( string_2564[lng], errprof, max_errprof );
        return( 199 );
      }

      col1 = TXT_DARKGREEN;
      txt1.Format( string_2562[lng], errprof, max_errprof );

      //TRACE("%d,%d,%d\n", ptn, dstot, dstot/ptn );

      //TRACE("CHIUSURA - %d punti - %d,%d %d,%d - %dx%d\n", ptn, xcmin, xcmax, ycmin, ycmax, xcmax-xcmin, ycmax-ycmin );
      //TRACE("CHIUSURA - %d punti - %d,%d %d,%d\n", ptn, ptx[0], pty[0], ptx[ptn-1], pty[ptn-1] );
      //break;
      return 0; // Ok, move on
    }
//    DRAW_PT(index,ptx[ptn],pty[ptn],0xFF0000FF); // 0xAARRGGBB

  }
/*
  for ( i=0 ; i<my_circle[src_radius].n ; i+=2 )  // Draw found circle
  {
    DRAW_PT(index,xcnt+my_circle[src_radius].x[i],ycnt+my_circle[src_radius].y[i],COL_BLUE);
    DRAW_PT(index,xcnt-my_circle[src_radius].x[i],ycnt-my_circle[src_radius].y[i],COL_BLUE);
  }
  */

  txt1.Format( string_2568[lng], ptn );
  return 99;
}




// Different version of bottom control where average is calculated circle by circle, not on whole inner
int COralFO_inspection::FO_analysis_circle( void )
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


  if ( contour_thr==0 )
  {
    col2 = 0;
    txt2 = L"";
    return 0;
  }



//  #define INNER_RAD 20 // inner radius for analysis

  int r = 1;
  int i = 0;
  int ltot = 0;

  if ( xcnt-anl_radius<20 || xcnt+anl_radius>int(dimx-20) || ycnt-anl_radius<20 || ycnt+anl_radius>int(dimy-20) )
  {
    txt2 = string_2800[lng];
    return 109;
  }




  /*
  //  ********************** NEW SYSTEM - MOVING AVERAGE ADJUSTABLE LENGHT  ************************

  #define MAXDIM 3000
//  #define LNGAVG   9  // define averaging length before/after pixel
  #define LNGAVG   10  // define averaging length before/after pixel

  BYTE* buffer = (BYTE*) malloc( MAXDIM );

  ZeroMemory( buffer, MAXDIM );

  // Calculate average and error
  for ( r=15 ; r<anl_radius ; r++ )
//  r = anl_radius;
  {
    int npt = 2*LNGAVG+2*my_circle[r].n;
    ASSERT( npt < MAXDIM );
    ASSERT( my_circle[r].n > LNGAVG );

    // prepare buffer
    int j = my_circle[r].n-LNGAVG;
    for ( i=0 ; i<LNGAVG ; i++ )
    {
      buffer[i] = PT2(xcnt-my_circle[r].x[j],ycnt-my_circle[r].y[j]);
      j++;
    }

    j=0;
    for ( i=LNGAVG ; i<LNGAVG+my_circle[r].n ; i++ )
    {
      buffer[i] = PT2(xcnt+my_circle[r].x[j],ycnt+my_circle[r].y[j]);
      j++;
    }

    j=0;
    for ( i=LNGAVG+my_circle[r].n ; i<LNGAVG+2*my_circle[r].n ; i++ )
    {
      buffer[i] = PT2(xcnt-my_circle[r].x[j],ycnt-my_circle[r].y[j]);
      j++;
    }

    j=0;
    for ( i=LNGAVG+2*my_circle[r].n ; i<2*LNGAVG+2*my_circle[r].n ; i++ )
    {
      buffer[i] = PT2(xcnt+my_circle[r].x[j],ycnt+my_circle[r].y[j]);
      j++;
    }

    //for ( i=0 ; i<npt ; i++ ) TRACE("%d,%d\n", i, buffer[i] );

    int ltot = 0;

    // preload
    for ( i=0 ; i<2*LNGAVG+1 ; i++ ) ltot += buffer[i];

    // start analysis
    for ( i=0 ; i<2*my_circle[r].n ; i++ )
    {
      //TRACE("%d,%d\n", i, int(0.5+double(ltot)/(2*LNGAVG+1) ) );
      BYTE avg = BYTE( 0.5 + double(ltot)/(2.0*LNGAVG+1) );

//      if ( buffer[i+LNGAVG]-int(0.5+double(ltot)/(2*LNGAVG+1))>pos_dif )
      if ( buffer[i+LNGAVG]-avg>pos_dif )
      {
        err_pos += buffer[i+LNGAVG]-avg-pos_dif;
        if ( i<my_circle[r].n ) DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
        else                 DRAW_PT(index,xcnt-my_circle[r].x[i-my_circle[r].n],ycnt-my_circle[r].y[i-my_circle[r].n],col_pos);
      }

//      if ( int(0.5+double(ltot)/(2*LNGAVG+1))-buffer[i+LNGAVG]>neg_dif )
      if ( avg-buffer[i+LNGAVG]>neg_dif )
      {
        err_neg += avg-buffer[i+LNGAVG]-neg_dif;
        if ( i<my_circle[r].n ) DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
        else                 DRAW_PT(index,xcnt-my_circle[r].x[i-my_circle[r].n],ycnt-my_circle[r].y[i-my_circle[r].n],col_neg);
      }

      ltot += buffer[i+2*LNGAVG+1]-buffer[i];
    }

    //for ( i=0 ; i<npt ; i++ ) TRACE("%d,%d\n", i, buffer[i] );
    //for ( i=0 ; i<my_circle[r].n ; i++ ) TRACE("%d,%d,%d,%d\n", i, xcnt-my_circle[r].x[i], ycnt-my_circle[r].y[i], PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) );
  }


  if ( buffer ) free( buffer );
  buffer = NULL;

  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif 
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
    txt2.Format( string_2792[lng], err_pos, err_neg, err_max, err_min );
    return 119;
  }

  /*
  if ( err_max > maxerr_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -165, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -165, nhead ); // 
      }
    #endif 
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
    txt2.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    return 139;
  }
  */

/*

  txt2.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  col2 = TXT_DARKGREEN;


  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 

  
  return 0;

  */

  //  ********************** OLD SYSTEM - AVERAGE AND CHECK ON WHOLE CIRCLES  ************************



  #ifdef SEPARATEAREAS // separate inner and outer areas of inspection


  // INNER AREA 

  // assign temporary radius for analysys
  ianl_radtmp = ianl_radius;

  for ( i=0 ; i<my_circle[ianl_radtmp].n ; i+=4 )  // Draw found circle
  {
    DRAW_PT(index,xcnt+my_circle[ianl_radtmp].x[i],ycnt+my_circle[ianl_radtmp].y[i],0xFFFF7700); // 0xAARRGGBB
    DRAW_PT(index,xcnt-my_circle[ianl_radtmp].x[i],ycnt-my_circle[ianl_radtmp].y[i],0xFFFF7700); // 0xAARRGGBB
  } 



  // Calculate average and error
  for ( r=15 ; r<ianl_radtmp ; r++ )
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

    //TRACE("radius=%d npt=%d average=%d\n", r, my_circle[r].n, ltot );

    // then calculate error
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
//      TRACE("%d,%d,%d,%d\n", i, xcnt+my_circle[r].x[i], ycnt+my_circle[r].y[i],  );

      // negative RELATIVE difference (bottom half-circle)
      if ( ltot-ineg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > 0  )
      {
        ierr_neg += ltot-ineg_dif-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_neg);
      }

      // positive RELATIVE difference (bottom half-circle)
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-ipos_dif > 0  )
      {
        ierr_pos += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-ltot-ipos_dif;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( imin_lum > PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) )
      {
        ierr_min += imin_lum-PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]);
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i]) > imax_lum )
      {
        ierr_max += PT2(xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i])-imax_lum;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],col_max);
      }


      // negative difference (top half-circle)
      if ( ltot-ineg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > 0  )
      {
        ierr_neg += ltot-ineg_dif-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_neg);
      }

      // positive difference (top half-circle)
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-ipos_dif > 0  )
      {
        ierr_pos += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-ltot-ipos_dif;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_pos);
      }

      // negative ABSOLUTE difference
      if ( imin_lum > PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) )
      {
        ierr_min += imin_lum-PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]);
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_min);
      }

      // positive ABSOLUTE difference
      if ( PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i]) > imax_lum )
      {
        ierr_max += PT2(xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i])-imax_lum;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],col_max);
      }
    }
  }

  ierr_pos /= 10;
  ierr_neg /= 10;
  ierr_max /= 10;
  ierr_min /= 10;

  CString inn, out;

  if ( ierr_pos > imaxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif 
    inn.Format( string_2790[lng], ierr_pos, ierr_neg, ierr_max, ierr_min );
    txt2.Format( string_2870[lng], inn );
    return 109;
  }

  if ( ierr_neg > imaxerr_neg )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -155, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -155, nhead ); // 
      }
    #endif 
    inn.Format( string_2792[lng], ierr_pos, ierr_neg, ierr_max, ierr_min );
    txt2.Format( string_2870[lng], inn );
    return 119;
  }
  
  if ( ierr_max > imaxerr_max )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -165, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -165, nhead ); // 
      }
    #endif 
    inn.Format( string_2794[lng], ierr_pos, ierr_neg, ierr_max, ierr_min );
    txt2.Format( string_2870[lng], inn );
    return 129;
  }


  if ( ierr_min > imaxerr_min )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -175, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -175, nhead ); // 
      }
    #endif 
    inn.Format( string_2796[lng], ierr_pos, ierr_neg, ierr_max, ierr_min );
    txt2.Format( string_2870[lng], inn );
    return 139;
  }
  
  txt2.Format( string_2798[lng], ierr_pos, ierr_neg, ierr_max, ierr_min );
  inn.Format( string_2870[lng], txt2 );
  #endif SEPARATEAREAS // separate inner and outer areas of inspection



  // assign temporary radius for analysys
  anl_radtmp = anl_radius;

  // Calculate average and error
  for ( r=ianl_radtmp ; r<anl_radtmp ; r++ )
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

    //TRACE("radius=%d npt=%d average=%d\n", r, my_circle[r].n, ltot );

    // then calculate error
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
//      TRACE("%d,%d,%d,%d\n", i, xcnt+my_circle[r].x[i], ycnt+my_circle[r].y[i],  );

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

  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif 
    out.Format( string_2790[lng], err_pos, err_neg, err_max, err_min );
    txt2.Format( string_2872[lng], inn, out );
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
    out.Format( string_2792[lng], err_pos, err_neg, err_max, err_min );
    txt2.Format( string_2872[lng], inn, out );
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
    out.Format( string_2794[lng], err_pos, err_neg, err_max, err_min );
    txt2.Format( string_2872[lng], inn, out );
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
    out.Format( string_2796[lng], err_pos, err_neg, err_max, err_min );
    txt2.Format( string_2872[lng], inn, out );
    return 139;
  }
  

  out.Format( string_2798[lng], err_pos, err_neg, err_max, err_min );
  txt2.Format( string_2872[lng], inn, out );
  col2 = TXT_DARKGREEN;


  #ifdef FD_DATAREC
    if ( datarec_on )
    {
      if ( index==3 && bottomA_result ) bottomA_result->InsertI( 0, nhead ); // 0 = GOOD
      if ( index==4 && bottomB_result ) bottomB_result->InsertI( 0, nhead ); // 0 = GOOD 
    }
  #endif 

  
  return 0;
}





// Standard version of bottom inspection
int COralFO_inspection::FO_analysis( void )
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

//  int r = 1;
  int r = 1;
  int i = 0;

  int ltot = 0;

  // INNER CIRCLE (1 to INNER_RAD)
  // First compute average brightness
//  for ( r=1 ; r<INNER_RAD ; r++ )

//  int inr_radius = INNER_RAD;
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


  if ( err_pos > maxerr_pos )
  {
    #ifdef FD_DATAREC
      if ( datarec_on )
      {
        if ( index==3 && bottomA_result ) bottomA_result->InsertI( -145, nhead ); // 
        if ( index==4 && bottomB_result ) bottomB_result->InsertI( -145, nhead ); // 
      }
    #endif 
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


int COralFO_inspection::Search_center( void )
{

  // initial search coords
  xcnt   = (xmin+xmax)/2; // 263;
  ycnt   = (ymin+ymax)/2; // 254;

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
    Calculate_scores( x, y );

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

/*
    for ( i=4 ; i<my_circle[src_radtmp].n ; i+=8 )  // Draw found circle
    {
      DRAW_CROSS(index,xcnt+my_circle[src_radtmp].x[i],ycnt+my_circle[src_radtmp].y[i],COL_BLUE);
      DRAW_CROSS(index,xcnt-my_circle[src_radtmp].x[i],ycnt-my_circle[src_radtmp].y[i],COL_BLUE);
    }
    */

    return 0; // search completed successfully
  }
  else
  {

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




// 0=center - 1=right - 2=bottom - 3=left - 4=top
int COralFO_inspection::Calculate_scores( int x, int y )
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


/*
int COralFO_inspection::Calculate_score( int ind )
{
  ASSERT( ind>=0 && ind<5 );
//  score[ind] = rv[ind]*bv[ind]*lv[ind]*tv[ind];
//  score[ind] = 

  return 0; // search completed successfully
}
*/

UINT COralFO_inspection::sectbrgth( int xc, int yc, int side ) // 0=right 1=bottom 2=left 3=top 
{
  UINT lum = 0;
  int  i;

  //i = my_circle[src_radtmp].x[0];
  
  ASSERT( img ); // must have been already assigned to img_acq
  ASSERT( my_circle[src_radtmp].n && my_circle[src_radtmp].x && my_circle[src_radtmp].y );

  if ( side==0 ) // right sector
  {
    for ( i=                   0 ; i<my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
    for ( i=3*my_circle[src_radtmp].n/4 ; i<my_circle[src_radtmp].n   ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  if ( side==1 ) // bottom sector
  {
    for ( i=my_circle[src_radtmp].n/4 ; i<3*my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
  }

  if ( side==2 ) // left sector
  {
    for ( i=3*my_circle[src_radtmp].n/4 ; i<my_circle[src_radtmp].n   ; i++ ) lum += PT2( xc+my_circle[src_radtmp].x[i] , yc+my_circle[src_radtmp].y[i] );
    for ( i=                   0 ; i<my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  if ( side==3 ) // top sector
  {
    for ( i=my_circle[src_radtmp].n/4 ; i<3*my_circle[src_radtmp].n/4 ; i++ ) lum += PT2( xc-my_circle[src_radtmp].x[i] , yc-my_circle[src_radtmp].y[i] );
  }

  return( 2*lum/my_circle[src_radtmp].n );
}


