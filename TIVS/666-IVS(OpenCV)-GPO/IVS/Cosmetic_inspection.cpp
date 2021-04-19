#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Cosmetic_inspection.h"
#include "ChildView.h"



extern Camera_struct    camera  [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;
extern circle_type      my_circle[MAXR_CIRCLES];

extern BOOL             datarec_on;

extern wchar_t          PC_name[30]; //Computer name read from system configuration

extern Results_manager* result;
extern Results_manager* result2;
extern BYTE             freezemode_on;
extern BOOL             liquid_mode;


/*
                                         // SIZE - SCORE
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,
                                            {   6 ,   6 } ,  // 6,4
                                            {  10 ,  10 } ,
                                            {  30 ,  40 } }; // 30,50
*/

#define  xmin     area->buffrect.left
#define  xmax     area->buffrect.right
#define  ymin     area->buffrect.top
#define  ymax     area->buffrect.bottom



CCosmetic_inspection::CCosmetic_inspection( UINT ind, CFile_mngmt* file_manager_in/*=NULL*/, int type_in/*=CSMTYPE_SIDEWALL*/ )
{
  ASSERT( ind>=0 && ind<num_tlc );

//  ASSERT( type_in==CSMTYPE_SIDEWALL || type_in==CSMTYPE_SIDECAKE || type_in==CSMTYPE_TOPCAKE );

  file_manager = file_manager_in;

  index = ind;
  type  = type_in;

  nhead = 0;

  area_color = COL_BLUE; // 0xFFRRGGBB assigned color to draw inspection area
  
  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  output = (BYTE*) malloc( dimx * dimy );
  ASSERT( output && AfxIsValidAddress(output, dimx*dimy) );

	ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  if ( type & CSMTYPE_SIDECAKE && my_circle[1].n==0 ) precalc_circles(); // precalculate all circles required for processing


  area = new CArea_adjust( index, file_manager );

#ifdef NEWCOSMETIC_COMBO //if New commetic for combo machines adjust limiting box
  area->limrect.top    -=2;
  area->limrect.bottom +=2;
  area->limrect.left   -=2;
  area->limrect.right  +=2;
#endif

/*  area->buffrect.top    =   dimy/3;
  area->buffrect.bottom = 2*dimy/3;
  area->buffrect.left   =   dimx/3;
  area->buffrect.right  = 2*dimx/3; */

  area->buffrect.top    =    5;
  area->buffrect.bottom =  994;
  area->buffrect.left   =  620;
  area->buffrect.right  = 1200;

  blob_score[0].size    =    5;  // GREEN   -   0 points
  blob_score[0].score   =    1;
  blob_score[1].size    =   20;  // YELLOW  -   1 point
  blob_score[1].score   =    5;
  blob_score[2].size    =   80;  // ORANGE  -   5 points
  blob_score[2].score   =   50;
  blob_score[3].size    =  300;  // RED     -  50 points
  blob_score[3].score   =  300;
                                 // MAGENTA - 300 points


  min_dif   =   25;
  max_dif   =   40;
  min_abs   =    0;
  max_abs   =  255;


  min_dif_upper   =   25;
  max_dif_upper   =   40;
  min_abs_upper   =    0;
  max_abs_upper   =  255;
  max_err_upper   =  250;
  cosm_err_upper  =  0; //Pour added for upper cosmetic

  radavg    =      6;
  radius    = radavg;
  
  max_err   =  250;
  cosm_err  =    0;
  num_blob  =    0;
  
  level_th  =   30;
  level_max =  200;
  level_min =  150;

  avglum    = NULL;

#ifndef NEWCOSMETIC_COMBO
  #ifdef SIDEWALL_PFS

  area->buffrect.top    =    5;
  area->buffrect.bottom =  994;
  area->buffrect.left   = 1098;
  area->buffrect.right  = 1522;

  x1_perc    = 70;    // brightness decrease to detect x1
  x1_band    =  5;    // distance band above/right of x1
  x2_perc    = 70;    // brightness decrease to detect x2
  x2_band    =  5;    // distance band above/right of x2

  min_absl   = 180;   // minimum allowed absolute brightness (area into liquid)
  min_absb   = 180;   // minimum allowed absolute brightness (bottom profile)

  min_width  =  10;  // minimum width
  min_height =  10;  // minimum height
  min_dim    = 100;  // minimum dimension of blob
  min_elong  =   1;  // minimum elongation (width/height - height/width on screen)
  min_avgdf  =  10;  // minimum average brightness

  #endif
#endif


  //Pour added 20181027
  #ifdef REJECTION_SEPARATION
  level_max2 =  230;
  level_min2 =  120;
	#endif

#ifdef UPPER_COSMETIC
	offsets_top    = 100;
	offsets_bottom = 30;
#endif

  avglum    = NULL;

  #if !defined NEWCOSMETIC_V20 && !defined NEWCOSMETIC_V30
  //  avglum = (UINT*) malloc( dimx*sizeof(UINT) );
    avglum = (INT*) malloc( dimx*sizeof(UINT) );
    ASSERT( avglum );
    ZeroMemory( avglum, dimx*sizeof(UINT) );
  #endif 

  extimg   = 0;    // buffer to extended image for processing
  extdim   = 0;    // total dimension of extended image
  extdimx  = 0;    // horizontal dimension of extended image
  extdimy  = 0;    // vertical dimension of extended image


/*  #ifndef NEWCOSMETIC_V20
    Buffer_check();
  #endif */

  circ_ofs =   NULL;
  circ_npt =      0;
  circ_rad = radavg;
  
/*  #ifndef NEWCOSMETIC_V20
    Circle_check();
  #endif */

  minbrgt_sw = 140;
  x2ofs_sw   = -20;
  x1ofs_sw   = 600;

  if ( type & CSMTYPE_SIDECAKE )
  {
    minbrgt_sw = 140;
    x1ofs_sw   =   0;
    x2ofs_sw   = 200;
  }


  // new params v20

  #ifdef NEWCOSMETIC_V20
    x1_perc    =  70; // brightness decrease to detect x1
    x1_band    =  20; // distance band above/right of x1
    x2_perc    =  70; // brightness decrease to detect x2
    x2_band    =  20; // distance band above/right of x2
    x3_perc    =  70; // brightness decrease to detect x3
    x3_band    =  20; // distance band above/right of x3

    min_abs    = 140; // minimum allowed absolute brightness 
    min_absl   = 160; // minimum allowed absolute brightness (lower area, into liquid)

    min_width  =   0; // minimum width
    min_height =   0; // minimum height
    min_dim    = 100; // minimum dimension of blob
    min_elong  =   5; // minimum elongation (width/height - height/width on screen)
    min_avgdf  =   5; // minimum average brightness
  #endif

  #ifdef NEWCOSMETIC_V30
    x1_perc     =  70; // brightness decrease to detect x1
    x1_band     =  20; // distance band above/right of x1
    x2_perc     =  70; // brightness decrease to detect x2
    x2_band     =  20; // distance band above/right of x2

    min_absb    = 140; // minimum allowed absolute brightness (bottom profile) 
    min_absl    = 160; // minimum allowed absolute brightness (lower area, into liquid)

	//Pour added for liquid color density control on20201027
	//max_absl    = 250; //maximum allowed absolute brightness
	//Pour added for liquid color density control on20201027

    bott_maxerr = 999; // max error on bottom profile
    maxbt_thk   =  70; // maximum bottom profile thickness

    min_width  =    0; // minimum width
    min_height =    0; // minimum height
    min_dim    =  100; // minimum dimension of blob
    min_elong  =    5; // minimum elongation (width/height - height/width on screen)
    min_avgdf  =    5; // minimum average brightness

    area->buffrect.top    =    5;
    area->buffrect.bottom =  994;
    area->buffrect.left   = 1000;
    area->buffrect.right  = 1200;

  #endif

	//Pour removed and marked on 20190525
#ifdef NEWCOSMETIC_COMBO
	  if ( type & CSMTYPE_COMBO )
  {

    x1_perc    =  80; // lum perc to detect beginning of bottom
    x2_perc    =  60; // lum perc to detect beginning of meniscus
    x3_perc    =  60; // lum perc to detect top end of sidewall

    x1_band    = 120; // allowance width above bottom to skip false rejects
    x2_band    =  20; // allowance width below meniscus to skip false rejects
    x3_band    =  90; // allowance width above meniscus to skip false rejects
    x4_band    =  20; // allowance width below top end of sidewall to skip false reject

    min_abslq  = 160;   // minimum allowed absolute brightness (liquid)
    min_abssw  = 170;   // minimum allowed absolute brightness (sidewall)

    min_width  =   5; // minimum width
    min_height =   5; // minimum height
    min_dim    =  20; // minimum dimension of blob
    min_elong  =   5; // minimum elongation (width/height - height/width on screen)
    min_avgdf  =   5; // minimum average brightness

    xmin       =  150;
    xmax       = 1500;
    ymin       =    0;
    ymax       =  999;

    min_cakevol =     0;
    max_cakevol = 10000;

  }
#endif

	//Pour removed and marked on 20190525

/*  if ( type & CSMTYPE_SIDECAKE ) // allocate for sidecake
  {
    circ_npt = 2*my_circle[SDCK_AMP].n;
    circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

    int i;
    int j=0;
    for ( i=0 ; i<my_circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  my_circle[SDCK_AMP].x[i]+my_circle[SDCK_AMP].y[i]*dimx,  my_circle[SDCK_AMP].x[i],  my_circle[SDCK_AMP].y[i] );
      circ_ofs[j++] =  my_circle[SDCK_AMP].x[i] + my_circle[SDCK_AMP].y[i]*dimx;
    }
    for ( i=0 ; i<my_circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -my_circle[SDCK_AMP].x[i]-my_circle[SDCK_AMP].y[i]*dimx, -my_circle[SDCK_AMP].x[i], -my_circle[SDCK_AMP].y[i] );
      circ_ofs[j++] = -my_circle[SDCK_AMP].x[i] - my_circle[SDCK_AMP].y[i]*dimx;
    }

    //TRACE("circ_ofs prepared - %d points\n", circ_npt );
    //for ( i=0 ; i<circ_npt ; i++ ) TRACE( "ofs[%2d] = %4d (%3d,%3d)\n", i, circ_ofs[i], circ_ofs[i]%dimx, circ_ofs[i]/dimx );
    //TRACE("FINISH\n");
  } */


}


CCosmetic_inspection::~CCosmetic_inspection(void)
{

//  if ( type & CSMTYPE_SIDECAKE && my_circle[1].n ) precalc_circles(); // precalculate all circles required for processing


  if ( extimg ) 
  {
    free( extimg );
    extimg = NULL;
  }

  if ( circ_ofs )
  {
    delete circ_ofs;
    circ_ofs = NULL;
  }

  if ( area )
  {
    delete area;
    area = NULL;
  }

  if ( avglum )
  {
    free( avglum );
    avglum = NULL;
  }

  if ( output )
  {
    free( output );
    output = NULL;
  }

}


int CCosmetic_inspection::Set_inspection_area( int ind )
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

  TRACE("CCosmetic_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}

//Pour 20190522
#if defined NEWCOSMETIC_COMBO
int CCosmetic_inspection::Add_menu_parameters_combo( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
	
	//if ( type & CSMTYPE_COMBO )
   //{
	#ifdef UPPER_COSMETIC
  pView->parameter[pView->num_params++] = new CParameter( &max_dif_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762c[lng], string_log [lng],   40,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_dif_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764c[lng], string_log [lng],   20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_abs_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776c[lng], string_log [lng],    0,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_abs_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778c[lng], string_log [lng],  255,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080c[lng], string_unit[lng], 1000,    0, 99000, file_manager );
	#endif
   //}

  pView->parameter[pView->num_params++] = new CParameter( &x1_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2520[lng], string_percent[lng],  70,    0,   150, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x2_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2522[lng], string_percent[lng],  70,    0,   150, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x3_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2524[lng], string_percent[lng],  70,    0,   150, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &x1_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2530[lng], string_pix[lng]    ,  20,    0,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x2_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2532[lng], string_pix[lng]    ,  20,    0,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x3_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2534[lng], string_pix[lng]    ,  20,    0,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x4_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2536[lng], string_pix[lng]    ,  20,    0,   300, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &min_abslq,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2394[lng], string_log [lng],    160,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_abssw,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2398[lng], string_log [lng],    160,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &min_cakevol, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2326[lng], string_unit[lng],     0,    0, 99999, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_cakevol, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2328[lng], string_unit[lng],     0,    0, 99999, file_manager );



  pView->parameter[pView->num_params++] = new CParameter( &min_dim,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620 [lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2092[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_height, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2090[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_elong,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2094[lng], string_unit[lng],    5,    0,    99, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_avgdf,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2096[lng], string_log [lng],    5,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

  // bottom thickness/profile
  //pView->parameter[pView->num_params++] = new CParameter( &min_absb,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2396[lng], string_log [lng],  160,    0,   255, file_manager );
  //pView->parameter[pView->num_params++] = new CParameter( &maxbt_thk,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2386[lng], string_pix [lng],   80,    4,   200, file_manager );
  //pView->parameter[pView->num_params++] = new CParameter( &bott_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2388[lng], string_unit[lng], 1000,    0, 99000, file_manager );


/*
  #ifdef NEWCOSMETIC_C40
    RECT area;
    pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
    area.top -= 100;
    pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);
  #else
    // Add inspection area
    pView->num_menu_btn++;
	  pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );
  #endif
  */

  pView->num_menu_btn++;
  pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );

  return 0; // completed successfully
}
#endif
//Pour 20190522


#if defined NEWCOSMETIC_V20 || defined NEWCOSMETIC_C40 || defined NEWCOSMETIC_COMBO
int CCosmetic_inspection::Add_menu_parameters_v20( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
  //ASSERT( 0 ); // VERIFICARE LE STRINGHE
  pView->parameter[pView->num_params++] = new CParameter( &min_abs,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2112[lng], string_log [lng],  255,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_absl,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2110[lng], string_log [lng],  255,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_dim,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620 [lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2092[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_height, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2090[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_elong,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2094[lng], string_unit[lng],    5,    0,    99, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_avgdf,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2096[lng], string_log [lng],    5,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

#if defined NEWCOSMETIC_C40 || defined NEWCOSMETIC_COMBO
  RECT area;
  pView-> menu_btn[pView-> num_menu_btn]->child->GetWindowRect(&area);
  area.top -= 100;
  pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);
#else
  // Add inspection area
  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );
#endif

	return 0; // completed successfully
}
#endif

#ifdef NEWCOSMETIC_V30
int CCosmetic_inspection::Add_menu_parameters_v30( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
  //ASSERT( 0 ); // VERIFICARE LE STRINGHE
  pView->parameter[pView->num_params++] = new CParameter( &x2_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2390[lng], string_percent[lng],  70,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x2_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2391[lng], string_pix[lng]    ,  20,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x1_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2392[lng], string_percent[lng],  70,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x1_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2393[lng], string_pix[lng]    ,  20,    0,   100, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &min_absl,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2394[lng], string_log [lng],  160,    0,   255, file_manager );
  //Pour added for density control on 20201028
  //pView->parameter[pView->num_params++] = new CParameter( &max_absl,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2395[lng], string_log [lng],  250,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &min_dim,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620 [lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2092[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_height, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2090[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_elong,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2094[lng], string_unit[lng],    5,    0,    99, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_avgdf,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2096[lng], string_log [lng],    5,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

  // bottom thickness/profile
  pView->parameter[pView->num_params++] = new CParameter( &min_absb,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2396[lng], string_log [lng],  160,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxbt_thk,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2386[lng], string_pix [lng],   80,    4,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &bott_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2388[lng], string_unit[lng], 1000,    0, 99000, file_manager );


  // Add inspection area
  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );
  return 0; // completed successfully
}
#endif

#ifdef SIDEWALL_PFS
int CCosmetic_inspection::Add_menu_parameters_PFS( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
  //ASSERT( 0 ); // VERIFICARE LE STRINGHE
  pView->parameter[pView->num_params++] = new CParameter( &x2_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2390[lng], string_percent[lng],  70,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x2_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2391[lng], string_pix[lng]    ,  20,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x1_perc,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2392[lng], string_percent[lng],  70,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &x1_band,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2393[lng], string_pix[lng]    ,  20,    0,   100, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &min_absl,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2394[lng], string_log [lng],  160,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_dim,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620 [lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2092[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_height, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2090[lng], string_pix [lng], 1000,    0, 99000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_elong,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2094[lng], string_unit[lng],    5,    0,    99, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_avgdf,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2096[lng], string_log [lng],    5,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

  // bottom thickness/profile
  //pView->parameter[pView->num_params++] = new CParameter( &min_absb,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2396[lng], string_log [lng],  160,    0,   255, file_manager );
  //pView->parameter[pView->num_params++] = new CParameter( &maxbt_thk,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2386[lng], string_pix [lng],   80,    4,   200, file_manager );
  //pView->parameter[pView->num_params++] = new CParameter( &bott_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2388[lng], string_unit[lng], 1000,    0, 99000, file_manager );


  // Add inspection area
  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );
  return 0; // completed successfully
}
#endif

int CCosmetic_inspection::Add_menu_parameters( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
/*  #ifdef TIVS_A40
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  #endif

  #ifdef TIVS_F40
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  #endif  */

  if ( name && AfxIsValidString( name[lng] ) )
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
    file_manager->Add_separator( name[lng], area );
  }
  else
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  }

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  #ifdef NEWCOSMETIC_V20
    return Add_menu_parameters_v20( child, name/*=NULL*/ );
  #endif

	  #ifdef SIDEWALL_PFS
	//Only for C3A,C3B
	if ( index>=1 && index<= 2)//Only for C3A,C3B
	{
    return Add_menu_parameters_PFS( child, name/*=NULL*/ );
	}
  #endif


  #ifdef NEWCOSMETIC_V30
    return Add_menu_parameters_v30( child, name/*=NULL*/ );
  #endif

	#ifdef NEWCOSMETIC_COMBO
	if (type & CSMTYPE_COMBO ) return Add_menu_parameters_combo(child,name /*=NULL*/);
	#endif
	
	#ifdef NEWCOSMETIC_C40
    if ( IS_PC2 && index>=6 && index<=8 ) Add_menu_parameters_v20( child, name/*=NULL*/ );
	#endif

  pView->parameter[pView->num_params++] = new CParameter( &radavg,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],    6,    1,    12, file_manager );
  
   if ( type & CSMTYPE_SIDEWALL || type & CSMTYPE_TOPCAKE )
   {
	#ifdef UPPER_COSMETIC
  pView->parameter[pView->num_params++] = new CParameter( &max_dif_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762c[lng], string_log [lng],   40,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_dif_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764c[lng], string_log [lng],   20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_abs_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776c[lng], string_log [lng],    0,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_abs_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778c[lng], string_log [lng],  255,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_err_upper,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080c[lng], string_unit[lng], 1000,    0, 99000, file_manager );
	#endif
   }

   #ifdef UPPER_COSMETIC
	    if ( type & CSMTYPE_SIDEWALL )
		{
    pView->parameter[pView->num_params++] = new CParameter( &offsets_top,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2352[lng],  string_pix[lng],  100, 0,     300, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &offsets_bottom,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2372[lng],  string_pix[lng],   30, 0,     300, file_manager );
		}
#endif

  pView->parameter[pView->num_params++] = new CParameter( &max_dif,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   40,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_dif,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_abs,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],    0,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_abs,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  255,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

  if ( type & CSMTYPE_CAKEVOLUME )
  {
    pView->parameter[pView->num_params++] = new CParameter( &level_th,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_930[lng],  string_log[lng],  30,    0,   255, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &level_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_932[lng],  string_pix[lng], 200,    0,  1000, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &level_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_934[lng],  string_pix[lng], 150,    0,  1000, file_manager );
#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &level_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_932a[lng],  string_pix[lng], 200,    0,  1000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &level_max2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_932b[lng],  string_pix[lng], 230,    0,  1000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &level_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_934a[lng],  string_pix[lng], 150,    0,  1000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &level_min2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_934b[lng],  string_pix[lng], 120,    0,  1000, file_manager );
#else

	pView->parameter[pView->num_params++] = new CParameter( &level_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_932[lng],  string_pix[lng], 200,    0,  1000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &level_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_934[lng],  string_pix[lng], 150,    0,  1000, file_manager );

#endif
  }
  else 
  {
    level_min  = 0;
    level_max  = 0;
	//Pour added 20181027
#ifdef REJECTION_SEPARATION
	level_min2  = 0;
    level_max2  = 0;
#endif
  }

  #ifdef NEWCAP_V21
  if ( type & CSMTYPE_SIDEWALL )
  {
    pView->parameter[pView->num_params++] = new CParameter( &minbrgt_sw, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2350[lng],  string_log[lng],   140,    0,  255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2352[lng],  string_pix[lng],     0, -300,  300, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2354[lng],  string_pix[lng],   600,  20, 2000, file_manager );
  }
	if ( type & CSMTYPE_SIDECAKE )
  {
    pView->parameter[pView->num_params++] = new CParameter( &minbrgt_sw, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2370[lng],  string_log[lng],   140,    0,  255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2372[lng],  string_pix[lng],     0, -300,  300, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2354[lng],  string_pix[lng],   200,   20, 2000, file_manager ); // previously minimum was set to 100, reduced to 80 on Victor's request, then 20 on 20151125 by Morris
  }
      if ( type & CSMTYPE_TOPCAKE )
  {
    pView->parameter[pView->num_params++] = new CParameter( &minbrgt_sw, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2350[lng],  string_log[lng],   140,    0,  255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2352[lng],  string_pix[lng],     0, -300,  300, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2354[lng],  string_pix[lng],   600,  100, 2000, file_manager );
  }
  #endif

	  	
  // Add inspection area
  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );

	/*
	RECT area;
    pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
    //area.top -= 210;
    area.top -= 210;
    pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);
	*/

  return 0; // completed successfully
}





int CCosmetic_inspection::Process_image( BYTE* img_acq, UINT nimg  )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  cosm_err_upper = 0;//Pour added for upper cosmetic

  cosm_err = 0;
  num_blob = 0;

  xmin = area->buffrect.left;
  xmax = area->buffrect.right;
  ymin = area->buffrect.top;
  ymax = area->buffrect.bottom;

//  SCALE_IMG( index, img_acq );

  txt1 = L"-";
  txt2 = L"-";
  //Pour marked on 20201121
  //txt3 = L"-";
  //txt4 = L"-";

  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;

   //Pour marked on 20201121
  //col3 = TXT_DARKRED;
  //col4 = TXT_DARKRED;


  // draw inspection area first
 /* int x, y;
  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  {
    DRAW_PT( index, x, area->buffrect.top,    0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom, 0x000000FF ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y+=4 )
  {
    DRAW_PT( index, area->buffrect.left,  y, 0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right, y, 0x000000FF ); // 0xFFRRGGBB
  }*/

    #ifdef NEWCOSMETIC_COMBO

//    return 0;

  if ( type & CSMTYPE_COMBO )//Pour marked to be recovered to combo mode 20190522 //Pour recovered for C20 on 20210307
  {
	  //Pour changed only for Kexin  on 20200919
#if SERIAL_N == 202000022 || SERIAL_N == 202000028 || SERIAL_N == 202000099

    if ( freezemode_on )   return Cosmetic_analisys_freeze( img_acq ); //freezemode_on for C40 and C60
    else                 return Cosmetic_analisys_liquid( img_acq );

#else
	  if (liquid_mode) return Cosmetic_analisys_liquid( img_acq );// liquid_mode only for C20
	  else             return Cosmetic_analisys_freeze( img_acq );
#endif
  }

  if ( type & CSMTYPE_SIDECAKE ) return( Sidecake_analysis( img_acq ) ); // sidecake & topcake (F40 PC2)


   // return Cosmetic_analisys_freeze( img_acq );
   // return Cosmetic_analisys_liquid( img_acq );
  #endif



  #ifdef NEWCOSMETIC_V20
    return Cosmetic_v20_analysis( img_acq );
  #endif

	  #ifdef NEWCOSMETIC_C40 //Pour 20190522
    if ( IS_PC2 && index>=6 && index<=8 && freezemode_on ) return Cosmetic_v20_analysis( img_acq );
  #endif

  #ifdef NEWCOSMETIC_V30
    return Cosmetic_v30_analysis( img_acq );
  #endif

//#if defined NEWCOSMETIC_V20 || defined NEWCOSMETIC_C40


/*  #ifdef TIVS_C40 // On C40 use cosmetic v20 ONLY in liquid mode and for cosmetic cameras (PC2 - 6/7/8)
    if ( IS_PC2 && index>=6 && index<=8 ) return Cosmetic_v20_analysis( img_acq );
  #endif */

	if ( type & CSMTYPE_PFS ) return( Sidewall_PFS( img_acq ) ); // sidecake & topcake (F40 PC2)

  if ( type & CSMTYPE_SIDECAKE ) return( Sidecake_analysis( img_acq ) ); // sidecake & topcake (F40 PC2)

  return( Sidewall_analysis( img_acq ) ); // sidewall (F40 PC1) / cosmetic (A40/L40 PC1)
}



#undef BLOCCO_BLOB



#define BLOCCO_BLOB(ox,oy)  if ( OUTP(ox,oy) )               {       \
                              tote     += OUTP(ox,oy);               \
                              dafx[end] = ox;                        \
                              dafy[end] = oy;                        \
                              if ( bbox.left  >ox ) bbox.left  =ox;  \
                              if ( bbox.right <ox ) bbox.right =ox;  \
                              if ( bbox.top   >oy ) bbox.top   =oy;  \
                              if ( bbox.bottom<oy ) bbox.bottom=oy;  \
                              OUTP(dafx[end],dafy[end]) = 0;         \
                              end++;                         }

#undef   OUTP
#define  OUTP(x,y) output[x+(y)*dimx]

#undef   PT
#define  PT(x,y)  img_acq[x+(y)*dimx]
//#define  PT(x,y)  extimg[x+(y)*extdimx]

//#undef   OUTP
//#define  OUTP(x,y) output [x+(y)*dimx]
//#define  OUT(x,y) output[x+(y)*extdimx]





//#ifdef NEWCOSMETIC_V20
#if defined NEWCOSMETIC_V20 || defined NEWCOSMETIC_C40

// Double self-aligned areas above/below filling level
int CCosmetic_inspection::Cosmetic_v20_analysis1( BYTE* img_acq )
{

  UINT col = 0x00FF0000FF;

//  COL_BLUE

  int x, y;

  cosm_err = 0;


/*  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  {
    DRAW_PT( index, x, area->buffrect.top,    0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom, 0x000000FF ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y+=4 )
  {
    DRAW_PT( index, area->buffrect.left,  y, 0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right, y, 0x000000FF ); // 0xFFRRGGBB
  } */


  CTimer timer;

  #define NSTRIPES 10

  int width = dimy / NSTRIPES;

  ASSERT( NSTRIPES * width == dimy );


  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area
/*  for ( y=0 ; y<dimy ; y+=4 )
  {
    DRAW_PT( index, xmin, y, COL_BLUE ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, COL_BLUE ); // 0xFFRRGGBB
  } */

  // cycle for stripes 

  int x1 =     0; // bottom of lower inspection area (into  liquid)
  int x2 =     0; // top    of lower inspection area (into  liquid)
  int x3 =     0; // bottom of upper inspection area (above liquid)
  int x4 =     0; // top    of upper inspection area (above liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe
//  for ( y=width ; y<=dimy-width ; y+=width )
  for ( int i=0 ; i<NSTRIPES ; i++ ) // y=index of stripe
  {


    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    x = xmin;
    UINT lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    UINT lum_thr1 = lum*x1_perc/100; // lower threshold value
    UINT lum_thr2 = lum*x2_perc/100; // upper threshold value
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>5 && lum>lum_thr1 );

    if ( x>5 ) // valid bottom found
    {
      x1 = x + x1_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x1,    y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }

    // search upper limit of lower insp area (x2)
    x = xmin;

    do
    {
      x += 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );

    } while ( x<xmax && lum>lum_thr2 );

    if ( x<xmax ) // valid bottom found
    {
      x2 = x - x2_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x2,    y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x-15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }



    //  SEARCH X3 OF UPPER AREA (ABOVE LIQUID)
    // search lower edge of lower area (x1)
    x  = xmax;
    lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    lum_thr1 = lum*x3_perc/100; // lower threshold value

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>x2+20 && lum>lum_thr1 );

    if ( x>x2+20 ) // valid bottom found
    {
      x3 = x + x3_band;
      x4 = xmax;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x3,    y, COL_MAGENTA ); // 0xFFRRGGBB
        DRAW_PT( index, x4,    y, COL_MAGENTA ); // 0xFFRRGGBB (upper - fixed)
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }

    if ( x1 && x2 && x3 && x4 ) // only if ALL coords have been found proceed with analysis
    {

      for ( y=y1 ; y<y2 ; y++ )
      {

        // analysis of LOWER part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          if ( min_absl-PT(x,y)>0 )
          {
            OUTP(x,y) = min_absl-PT(x,y);
            //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }

        // analysis of UPPER part
        for ( x=x3 ; x<=x4 ; x++ )
        {
          if ( min_abs-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abs-PT(x,y);
            //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }
      }



    }
    else // not found, draw a RED cross
    {
      for ( x=xmin+20 ; x<xmax-20 ; x+=2 )
      {
        DRAW_PT( index, x, y1+(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
      }

      cosm_err += 100;
    }


    // draw stripes
    //for ( x=xmin ; x<=xmax ; x+=2 ) DRAW_PT( index, x, y1, COL_ORANGE ); // 0xFFRRGGBB
 /*   for ( x=x1 ; x<=x2 ; x+=2 )
    {
      DRAW_PT( index, x, y1,   COL_ORANGE ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2-1, COL_ORANGE ); // 0xFFRRGGBB
    }

    for ( x=x3 ; x<=x4 ; x+=2 )
    {
      DRAW_PT( index, x, y1,   COL_MAGENTA ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2-1, COL_MAGENTA ); // 0xFFRRGGBB
    }  */
    

    y1 += width;
    y2 += width;

  }




  // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    //output[x] = 0;
    //output[x+dimx*(dimy-1)] = 0;
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 0;

        if ( width>height ) elong = width/height;

        
        
        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 0 se width<=height altrimenti ?il rapporto (5 alto e stretto in visualizzazione)




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
        if ( end>min_dim && elong>min_elong && width>min_height && height>min_width && tote>end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }


        
/*        // IN QUESTO PUNTO end ?la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else 
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_MAGENTA );  // MAGENTA - SCORE[3]
              }
            }
          } 
       
        }  //BLOG BIG ENOUGH   */
        


        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	if ( index==6)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==7)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==8)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==9)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	#endif
	//Pour added 20180927

    return 0;  // result of inspection GOOD
  }



  return 0;

}

#endif //NEWCOSMETIC_V20  || NEWCOSMETIC_C40

//Pour 20190522
#ifdef NEWCOSMETIC_COMBO

// Single top/bottom self-aligned area
int CCosmetic_inspection::Cosmetic_analisys_liquid( BYTE* img_acq )
{
  UINT col = 0x00FF0000FF;
  int x, y;

  cosm_err = 0;

  //Pour added for filling level checking on cosmetic station on 20200919

#if SERIAL_N == 202000022 ||SERIAL_N == 202000028 || SERIAL_N == 202000099
  cake_vol = 0;
#endif

  //Pour added for filling level checking on cosmetic station on 20200919

  CTimer timer;

  #define NSTRIPES 10

  int width = dimy / NSTRIPES;

  ASSERT( NSTRIPES * width == dimy );

  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area
/*  for ( y=0 ; y<dimy ; y+=4 )
  {
    DRAW_PT( index, xmin, y, COL_BLUE ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, COL_BLUE ); // 0xFFRRGGBB
  } */

  // cycle for stripes 

  int x1 =     0; // lower/left of lower inspection area (into  liquid)
  int x2 =     0; // top/right  of lower inspection area (into  liquid)
  int x3 =     0; // lower/left of upper inspection area (above liquid)
  int x4 =     0; // top/right  of upper inspection area (above liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe
//  for ( y=width ; y<=dimy-width ; y+=width )

 // for ( x1=0 ; x1<dimx*dimy ; x1++ ) output[x1];

 /*
  x1_perc =  80; // lum perc to detect beginning of bottom
  x2_perc =  60; // lum perc to detect beginning of meniscus
  x3_perc =  60; // lum perc to detect top end of sidewall

  x1_band = 120; // allowance width above bottom to skip false rejects
  x2_band =  20; // allowance width below meniscus to skip false rejects
  x3_band =  90; // allowance width above meniscus to skip false rejects
  x4_band =  20; // allowance width below top end of sidewall to skip false reject

  min_abslq = 160;   // minimum allowed absolute brightness (liquid)
  min_abssw = 170;   // minimum allowed absolute brightness (sidewall)

  xmin =  150;
  xmax = 1500;
  ymin =    0;
  ymax =  999;
*/

  for ( int i=0 ; i<NSTRIPES ; i++ ) // i=index of stripe
  {
    ASSERT( 0<=y1 && y1<y2 && y2<=dimy );
    // analisys in y between y1 and y2

    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    x = xmin; // start from lower insp area
    UINT lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    UINT lum_thr1 = lum*x1_perc/100; // threshold value to detect x1 (lower/left beginning of bottom)
    UINT lum_thr2 = lum*x2_perc/100; // threshold value to detect x2 (lower/left beginning of meniscus)
    UINT lum_thr3 = lum*x3_perc/100; // threshold value to detect x2 (lower/left beginning of meniscus)
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );


    x1 = 0;
    x2 = 0;
    x3 = 0;
    x4 = 0;


    // search lower edge of bottom
    do
    {
      x  += 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x<xmax-x1_band-10 && lum>lum_thr1 );

    if ( x<xmax-x1_band-10 ) // valid bottom found
    {
      x1 = x + x1_band; // x1 is lower/left point to start inspection inside liquid area
      for ( y=y1 ; y<y2 ; y+=6 )
      {
        DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
        DRAW_PT( index, x1, y, COL_BLUE    ); // 0xFFRRGGBB
      }

      x = x1 + 10;

      // search lower/left edge of meniscus
      do
      {
        x  += 4;
        lum = 0;
        for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
        //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
      } while ( x<xmax-x3_band-10 && lum>lum_thr2 );

      if ( x<xmax-x3_band-10 ) // valid meniscus found
      {
        x2 = x - x2_band; // x2 is upper/right point to end inspection inside liquid area
        x3 = x + x3_band; // x3 is lower/left point to start inspection above liquid area

		  //Pour added for filling level checking on cosmetic station on 20200919

#if SERIAL_N == 202000022 || SERIAL_N == 202000028 || SERIAL_N == 202000099
  cake_vol += x - x1;
#endif

  //Pour added for filling level checking on cosmetic station on 20200919


        for ( y=y1 ; y<y2 ; y+=6 )
        {
          DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
          DRAW_PT( index, x2, y, COL_BLUE    ); // 0xFFRRGGBB
          DRAW_PT( index, x3, y, COL_BLUE    ); // 0xFFRRGGBB
        }

        x = x3 + 10;

        // search lower/left edge of upper sidewall area
        do
        {
          x  += 4;
          lum = 0;
          for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
          //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
        } while ( x<xmax && lum>lum_thr3 );

        //if ( x<xmax ) // valid upper sidewall found
        {
          x4 = x - x4_band; // x4 is upper/right point to end inspection of sidewall
          for ( y=y1 ; y<y2 ; y+=6 )
          {
            DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
            DRAW_PT( index, x4, y, COL_BLUE    ); // 0xFFRRGGBB
          }

        } // end of if upper sidewall found

      } // end of if meniscus found

    } // end of if bottom found

    if ( x1 && x2 && x3 && x4 ) // if ALL x1/2/3/4 found, analysis 
    {

      // scan stripes
      for ( y=y1 ; y<y2 ; y++ )
      {

        // analysis of liquid part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          if ( min_abslq-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abslq-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }
        }

        // analysis of sidewall part
        for ( x=x3 ; x<=x4 ; x++ )
        {
          if ( min_abssw-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abssw-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }
        }
      }




    }
    else // else draw a RED cross inside
    {
/*      for ( x=xmin+20 ; x<xmax-20 ; x+=6 ) 
      {
        int ofs = (y2-y1)*(x-xmin)/(xmax-xmin);
        DRAW_PT( index, x, y1+ofs, COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-ofs, COL_RED ); // 0xFFRRGGBB
      } */
      for ( y=y1+2 ; y<y2-2 ; y++ ) 
      {
        int ofs = (xmax-xmin)*(y-y1)/(y2-y1);
        DRAW_PT( index, xmin+ofs, y, COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, xmax-ofs, y, COL_RED ); // 0xFFRRGGBB
      }
      cosm_err += 1000; // and add some error
    }
   

    // draw stripes
    for ( x=x1 ; x<=x2 ; x+=6 ) DRAW_PT( index, x, y1, COL_BLUE ); // 0xFFRRGGBB
    for ( x=x3 ; x<=x4 ; x+=6 ) DRAW_PT( index, x, y1, COL_BLUE ); // 0xFFRRGGBB


    y1 += width;
    y2 += width;

  } // end of for i(stripes)










  // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    //output[x] = 0;
    //output[x+dimx*(dimy-1)] = 0;
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 100;

       // if ( width>height ) elong = width/height;
        if ( width>2 && height>2 ) elong = 100*height/width;
     

        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 100 se width O height sono minori di 2, altrimenti ?il rapporto moltitplicato 100 E=10 significa largo e stretto, E=1000 significa stretto e alto




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
        if ( end>=min_dim && elong>=min_elong && width>=min_height && height>=min_width && tote>=end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }


        
/*        // IN QUESTO PUNTO end ?la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else 
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_MAGENTA );  // MAGENTA - SCORE[3]
              }
            }
          } 
       
        }  //BLOG BIG ENOUGH   */
        


        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );
  
  good = TRUE;

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	if ( index==6)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==7)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==8)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==9)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
#endif
	//Pour added 20180927


	//Pour added for filling level checking on cosmetic station on 20200919

#if SERIAL_N == 202000022 || SERIAL_N == 202000028 || SERIAL_N == 202000099

	  // Check cake volume
  if ( min_cakevol && max_cakevol )
  {
    if ( cake_vol < min_cakevol )
    {
      txt2.Format( string_2312[lng], cake_vol, min_cakevol, max_cakevol );
      col2 = TXT_DARKRED;
      good = FALSE;
      //return 99;  // result of inspection REJECT
    }
    else
    {
      if ( cake_vol > max_cakevol )
      {
        txt2.Format( string_2314[lng], cake_vol, min_cakevol, max_cakevol );
        col2 = TXT_DARKRED;
        good = FALSE;
        //return 99;  // result of inspection REJECT
      }
      else
      {
        txt2.Format( string_2316[lng], cake_vol, min_cakevol, max_cakevol );
        col2 = TXT_DARKGREEN;
        //return 0;  // result of inspection GOOD
      }
    }
  }
  else
  {
    txt2.Format( string_2324[lng] );
    col2 = TXT_BLACK;
  }


#endif

	//Pour added for filling level checking on cosmetic station on 20200919



    //return 0;  // result of inspection GOOD
  }

/*  if ( bott_err > bott_maxerr )
  {
    txt2.Format( string_2380[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt2.Format( string_2382[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }*/


  if ( good ) return 0;

  return 99; // reject

} 


int CCosmetic_inspection::Cosmetic_analisys_freeze( BYTE* img_acq )
{
  UINT col = 0x00FF0000FF;
  int x, y;

  cosm_err = 0;

  cake_vol = 0;

  CTimer timer;

  #define NSTRIPES 10

  int width = dimy / NSTRIPES;

  ASSERT( NSTRIPES * width == dimy );

  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area
/*  for ( y=0 ; y<dimy ; y+=4 )
  {
    DRAW_PT( index, xmin, y, COL_BLUE ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, COL_BLUE ); // 0xFFRRGGBB
  } */

  // cycle for stripes 

  int x1 =     0; // lower/left of lower inspection area (into  liquid)
  int x2 =     0; // top/right  of lower inspection area (into  liquid)
  int x3 =     0; // lower/left of upper inspection area (above liquid)
  int x4 =     0; // top/right  of upper inspection area (above liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe
//  for ( y=width ; y<=dimy-width ; y+=width )

 // for ( x1=0 ; x1<dimx*dimy ; x1++ ) output[x1];

 /*
  x1_perc =  80; // lum perc to detect beginning of bottom
  x2_perc =  60; // lum perc to detect beginning of meniscus
  x3_perc =  60; // lum perc to detect top end of sidewall

  x1_band = 120; // allowance width above bottom to skip false rejects
  x2_band =  20; // allowance width below meniscus to skip false rejects
  x3_band =  90; // allowance width above meniscus to skip false rejects
  x4_band =  20; // allowance width below top end of sidewall to skip false reject

  min_abslq = 160;   // minimum allowed absolute brightness (liquid)
  min_abssw = 170;   // minimum allowed absolute brightness (sidewall)

  xmin =   90;
  xmax = 1230;
  ymin =    0;
  ymax =  999;
  */

  for ( int i=0 ; i<NSTRIPES ; i++ ) // i=index of stripe
  {
    ASSERT( 0<=y1 && y1<y2 && y2<=dimy );
    // analisys in y between y1 and y2

    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    x = xmin; // start from lower insp area
    UINT lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    UINT lum_thr1 = lum*x1_perc/100; // threshold value to detect x1 (lower/left beginning of bottom)
    UINT lum_thr2 = lum*x2_perc/100; // threshold value to detect x2 (lower/left beginning of meniscus)
    UINT lum_thr3 = lum*x3_perc/100; // threshold value to detect x2 (lower/left beginning of meniscus)
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );


    x1 = 0;
    x2 = 0;
    x3 = 0;
    x4 = 0;


    // search lower edge of bottom
    do
    {
      x  += 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x<xmax-x1_band-10 && lum>lum_thr1 );

    if ( x<xmax-x1_band-10 ) // valid bottom found
    {
      x1 = x + x1_band; // x1 is lower/left point to start inspection inside liquid area
      for ( y=y1 ; y<y2 ; y+=6 )
      {
        DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x1, y, COL_BLUE    ); // 0xFFRRGGBB
      }

      x = x1 + 10;

      // search upper/right edge of cake
      do
      {
        x  += 4;
        lum = 0;
        for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
        //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
      } while ( x<xmax-x3_band-10 && lum<lum_thr2 );

      if ( x<xmax-x3_band-10 ) // valid meniscus/upper cake found
      {
        x2 = x - x2_band; // x2 is upper/right point to end inspection inside liquid area
        x3 = x + x3_band; // x3 is lower/left point to start inspection above liquid area

        cake_vol += x - x1;

        for ( y=y1 ; y<y2 ; y+=6 )
        {
          DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
          //DRAW_PT( index, x2, y, COL_BLUE    ); // 0xFFRRGGBB
          DRAW_PT( index, x3, y, COL_BLUE    ); // 0xFFRRGGBB

          // Show cake volume area
          //for ( int j=x1 ; j<x ; j+=6 ) DRAW_PT( index, j, y, 0xFFAAAA00 ); // 0xFFRRGGBB 
        }

        x = x3 + 10;

        // search lower/left edge of upper sidewall area
        do
        {
          x  += 4;
          lum = 0;
          for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
          //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
        } while ( x<xmax && lum>lum_thr3 );

        //if ( x<xmax ) // valid upper sidewall found
        {
          x4 = x - x4_band; // x4 is upper/right point to end inspection of sidewall
          for ( y=y1 ; y<y2 ; y+=6 )
          {
            DRAW_PT( index, x,  y, COL_MAGENTA ); // 0xFFRRGGBB
            DRAW_PT( index, x4, y, COL_BLUE    ); // 0xFFRRGGBB
          }

        } // end of if upper sidewall found

      } // end of if meniscus found

    } // end of if bottom found



    if ( x1 && x2 && x3 && x4 ) // if ALL x1/2/3/4 found, analysis 
    {

      // scan stripes
      for ( y=y1 ; y<y2 ; y++ )
      {

      /*  // analysis of liquid part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          if ( min_abslq-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abslq-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }
        }  */

        // analysis of sidewall part
        for ( x=x3 ; x<=x4 ; x++ )
        {
          if ( min_abssw-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abssw-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }
        }
      }




    }
    else // else draw a RED cross inside
    {
/*      for ( x=xmin+20 ; x<xmax-20 ; x+=6 ) 
      {
        int ofs = (y2-y1)*(x-xmin)/(xmax-xmin);
        DRAW_PT( index, x, y1+ofs, COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-ofs, COL_RED ); // 0xFFRRGGBB
      } */
      for ( y=y1+2 ; y<y2-2 ; y++ ) 
      {
        int ofs = (xmax-xmin)*(y-y1)/(y2-y1);
        DRAW_PT( index, xmin+ofs, y, COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, xmax-ofs, y, COL_RED ); // 0xFFRRGGBB
      }
      cosm_err += 1000; // and add some error
    }
   

    // draw vertical stripes
    for ( x=x3 ; x<=x4 ; x+=30 ) DRAW_PT( index, x, y1, COL_BLUE ); // 0xFFRRGGBB

    y1 += width;
    y2 += width;

  } // end of for i(stripes)



  // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    //output[x] = 0;
    //output[x+dimx*(dimy-1)] = 0;
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 100;

       // if ( width>height ) elong = width/height;
        if ( width>2 && height>2 ) elong = 100*height/width;
     

        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 100 se width O height sono minori di 2, altrimenti è il rapporto moltitplicato 100 E=10 significa largo e stretto, E=1000 significa stretto e alto




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
        if ( end>=min_dim && elong>=min_elong && width>=min_height && height>=min_width && tote>=end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end; // add blob's area to error

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }


        
/*        // IN QUESTO PUNTO end è la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else 
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_MAGENTA );  // MAGENTA - SCORE[3]
              }
            }
          } 
       
        }  //BLOG BIG ENOUGH   */
        


        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );
  
  good = TRUE;

  // check cosmetic error
  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }


  // Check cake volume
  if ( min_cakevol && max_cakevol )
  {
    if ( cake_vol < min_cakevol )
    {
      txt2.Format( string_2312[lng], cake_vol, min_cakevol, max_cakevol );
      col2 = TXT_DARKRED;
      good = FALSE;
      //return 99;  // result of inspection REJECT
    }
    else
    {
      if ( cake_vol > max_cakevol )
      {
        txt2.Format( string_2314[lng], cake_vol, min_cakevol, max_cakevol );
        col2 = TXT_DARKRED;
        good = FALSE;
        //return 99;  // result of inspection REJECT
      }
      else
      {
        txt2.Format( string_2316[lng], cake_vol, min_cakevol, max_cakevol );
        col2 = TXT_DARKGREEN;
        //return 0;  // result of inspection GOOD
      }
    }
  }
  else
  {
    txt2.Format( string_2324[lng] );
    col2 = TXT_BLACK;
  }
  /*
  //Pour changed the senquence on 20200919
    // check cosmetic error
  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }
  //Pour changed the senquence on 20200919
  */

/*  if ( bott_err > bott_maxerr )
  {
    txt2.Format( string_2380[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt2.Format( string_2382[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }*/


  if ( good ) return 0;

  return 99; // reject

} 



#endif // NEWCOSMETIC_COMBO


#ifdef NEWCOSMETIC_V30

// Single top/bottom self-aligned area
int CCosmetic_inspection::Cosmetic_v30_analysis( BYTE* img_acq )
{

  UINT col = 0x00FF0000FF;

//  COL_BLUE

  int x, y;

  cosm_err = 0;


/*  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  {
    DRAW_PT( index, x, area->buffrect.top,    0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, x, area->buffrect.bottom, 0x000000FF ); // 0xFFRRGGBB
  }
  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y+=4 )
  {
    DRAW_PT( index, area->buffrect.left,  y, 0x000000FF ); // 0xFFRRGGBB
    DRAW_PT( index, area->buffrect.right, y, 0x000000FF ); // 0xFFRRGGBB
  } */


  CTimer timer;

  #define NSTRIPES 10

  int width = dimy / NSTRIPES;

  ASSERT( NSTRIPES * width == dimy );


  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area
/*  for ( y=0 ; y<dimy ; y+=4 )
  {
    DRAW_PT( index, xmin, y, COL_BLUE ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, COL_BLUE ); // 0xFFRRGGBB
  } */

  // cycle for stripes 

  int x1 =     0; // bottom of lower inspection area (into  liquid)
  int x2 =     0; // top    of lower inspection area (into  liquid)
//  int x3 =     0; // bottom of upper inspection area (above liquid)
//  int x4 =     0; // top    of upper inspection area (above liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe
//  for ( y=width ; y<=dimy-width ; y+=width )

 // for ( x1=0 ; x1<dimx*dimy ; x1++ ) output[x1];

  bott_err = 0;

  for ( int i=0 ; i<NSTRIPES ; i++ ) // y=index of stripe
  {


    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    x = xmin; // start from lower insp area
    UINT lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    UINT lum_thr1 = lum*x1_perc/100; // lower threshold value
    UINT lum_thr2 = lum*x2_perc/100; // upper threshold value
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>5 && lum>lum_thr1 );

    if ( x>5 ) // valid bottom found
    {
      x1 = x + x1_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x1, y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }

    // search upper limit of lower insp area (x2)
    x = xmax;

    do
    {
      x += 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );

    } while ( x<dimx-5 && lum>lum_thr2 );

    if ( x<dimx-5 ) // valid position
    {
      x2 = x - x2_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x2, y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x-15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }

    /*
    //  SEARCH X3 OF UPPER AREA (ABOVE LIQUID)
    // search lower edge of lower area (x1)
    x  = xmax;
    lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    lum_thr1 = lum*x3_perc/100; // lower threshold value

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>x2+20 && lum>lum_thr1 );

    if ( x>x2+20 ) // valid bottom found
    {
      x3 = x + x3_band;
      x4 = xmax;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x3,    y, COL_MAGENTA ); // 0xFFRRGGBB
        DRAW_PT( index, x4,    y, COL_MAGENTA ); // 0xFFRRGGBB (upper - fixed)
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }
    */


    if ( x1 && x2 ) // only if ALL coords have been found proceed with analysis
    {

      int xt = x1 +  20; // right/top   of bottom profile analysis
      if ( xt<5 ) xt = 5;

      int xb = xt - 150; // left/bottom of bottom profile analysis
      if ( xb<5 ) xb = 5;

      for ( y=y1 ; y<y2 ; y++ )
      {

        // analysis of LOWER part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          //DRAW_PT( index, x, y, COL_YELLOW ); // 0xFFRRGGBB

          if ( min_absl-PT(x,y)>0 )
          {
            OUTP(x,y) = min_absl-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }

		  //Pour added for color density control on 20201027
		  /*
		  if ( PT(x,y)-max_absl>0 )
          {
            OUTP(x,y) = PT(x,y)-max_absl;
            DRAW_PT( index, x, y, COL_YELLOW ); // COL_RED ); // 0xFFRRGGBB
          }
		  */
		  //Pour added for color density control on 20201027

        }


        // FIND BOTTOM PROFILE
        DRAW_PT( index, xb, y, COL_BLUE ); // 0xFFRRGGBB
        DRAW_PT( index, xt, y, COL_BLUE ); // 0xFFRRGGBB

        int npb = 0;

        // analysis of LOWER part
        for ( x=xb ; x<=xt ; x++ ) if ( PT(x,y)<min_absb ) npb++;

        if ( npb>maxbt_thk ) // if exceeds maximum bottom thickness
        {
          bott_err += npb-maxbt_thk;
          for ( x=xb ; x<=xt ; x+=2 )
          {
            if ( PT(x,y)<min_absb ) DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }


      }


    }
    else // not found, draw a RED cross
    {
      for ( x=xmin+20 ; x<xmax-20 ; x+=2 )
      {
        DRAW_PT( index, x, y1+(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
      }

      cosm_err += 100;
    }





    // draw stripes
    //for ( x=xmin ; x<=xmax ; x+=2 ) DRAW_PT( index, x, y1, COL_ORANGE ); // 0xFFRRGGBB
 /*   for ( x=x1 ; x<=x2 ; x+=2 )
    {
      DRAW_PT( index, x, y1,   COL_ORANGE ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2-1, COL_ORANGE ); // 0xFFRRGGBB
    }

    for ( x=x3 ; x<=x4 ; x+=2 )
    {
      DRAW_PT( index, x, y1,   COL_MAGENTA ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2-1, COL_MAGENTA ); // 0xFFRRGGBB
    }  */
    

    y1 += width;
    y2 += width;

  }




  // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    //output[x] = 0;
    //output[x+dimx*(dimy-1)] = 0;
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 100;

       // if ( width>height ) elong = width/height;
        if ( width>2 && height>2 ) elong = 100*height/width;
     

        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 100 se width O height sono minori di 2, altrimenti è il rapporto moltitplicato 100 E=10 significa largo e stretto, E=1000 significa stretto e alto




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
        if ( end>=min_dim && elong>=min_elong && width>=min_height && height>=min_width && tote>=end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }


        
/*        // IN QUESTO PUNTO end è la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else 
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_MAGENTA );  // MAGENTA - SCORE[3]
              }
            }
          } 
       
        }  //BLOG BIG ENOUGH   */
        


        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );
  
  good = TRUE;

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }

  if ( bott_err > bott_maxerr )
  {
    txt2.Format( string_2380[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt2.Format( string_2382[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }


  if ( good ) return 0;

  return 99; // reject

}

#endif //NEWCOSMETIC_V30

// Copia????
//#ifdef NEWCOSMETIC_V20
#if defined NEWCOSMETIC_V20 || defined NEWCOSMETIC_C40

// Double self-aligned areas above/below filling level
int CCosmetic_inspection::Cosmetic_v20_analysis( BYTE* img_acq )
{

  UINT col = 0x00FF0000FF;

//  COL_BLUE

  int x, y;

  cosm_err = 0;


  CTimer timer;

  #define NSTRIPES 10

  int width = dimy / NSTRIPES;

  ASSERT( NSTRIPES * width == dimy );


  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area

  // cycle for stripes 

  int x1 =     0; // bottom of lower inspection area (into  liquid)
  int x2 =     0; // top    of lower inspection area (into  liquid)
  int x3 =     0; // bottom of upper inspection area (above liquid)
  int x4 =     0; // top    of upper inspection area (above liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe
//  for ( y=width ; y<=dimy-width ; y+=width )
  for ( int i=0 ; i<NSTRIPES ; i++ ) // y=index of stripe
  {


    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    x = xmin;
    UINT lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    UINT lum_thr1 = lum*x1_perc/100; // lower threshold value
    UINT lum_thr2 = lum*x2_perc/100; // upper threshold value
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>5 && lum>lum_thr1 );

    if ( x>5 ) // valid bottom found
    {
      x1 = x + x1_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x1,    y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }

    // search upper limit of lower insp area (x2)
    x = xmin;

    do
    {
      x += 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );

    } while ( x<xmax && lum>lum_thr2 );

    if ( x<xmax ) // valid bottom found
    {
      x2 = x - x2_band;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x2,    y, COL_MAGENTA ); // 0xFFRRGGBB
        //DRAW_PT( index, x-15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }



    //  SEARCH X3 OF UPPER AREA (ABOVE LIQUID)
    // search lower edge of lower area (x1)
    x  = xmax;
    lum = 0;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);

    lum_thr1 = lum*x3_perc/100; // lower threshold value

    do
    {
      x -= 4;
      lum = 0;
      for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
      //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
    } while ( x>x2+20 && lum>lum_thr1 );

    if ( x>x2+20 ) // valid bottom found
    {
      x3 = x + x3_band;
      x4 = xmax;
      for ( y=y1 ; y<y2 ; y+=2 )
      {
        DRAW_PT( index, x3,    y, COL_MAGENTA ); // 0xFFRRGGBB
        DRAW_PT( index, x4,    y, COL_MAGENTA ); // 0xFFRRGGBB (upper - fixed)
        //DRAW_PT( index, x+15, y, COL_ORANGE ); // 0xFFRRGGBB
      }
    }
    else // NOT FOUND!
    {

    }

    if ( x1 && x2 && x3 && x4 ) // only if ALL coords have been found proceed with analysis
    {

      for ( y=y1 ; y<y2 ; y++ )
      {

        // analysis of LOWER part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          if ( min_absl-PT(x,y)>0 )
          {
            OUTP(x,y) = min_absl-PT(x,y);
            //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }

        // analysis of UPPER part
        for ( x=x3 ; x<=x4 ; x++ )
        {
          if ( min_abs-PT(x,y)>0 )
          {
            OUTP(x,y) = min_abs-PT(x,y);
            //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }
      }



    }
    else // not found, draw a RED cross
    {
      for ( x=xmin+20 ; x<xmax-20 ; x+=2 )
      {
        DRAW_PT( index, x, y1+(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
      }

      cosm_err += 100;
    }


    // draw stripes
    //for ( x=xmin ; x<=xmax ; x+=2 ) DRAW_PT( index, x, y1, COL_ORANGE ); // 0xFFRRGGBB
    

    y1 += width;
    y2 += width;

  }




  // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    //output[x] = 0;
    //output[x+dimx*(dimy-1)] = 0;
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
           //Controllo se il punto che sto esaminando ha vicini da marcare

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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 0;

        if ( width>height ) elong = width/height;

        
        
        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 0 se width<=height altrimenti è il rapporto (5 alto e stretto in visualizzazione)





//        if ( end > 100 && elong > 3 )
        if ( end>min_dim && elong>min_elong && width>min_height && height>min_width && tote>end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }


        
        


        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }



  return 0;

}

#endif // NEWCOSMETIC_V20
 

#undef   PT
//#define  PT(x,y)  img_acq[x+(y)*dimx]
#define  PT(x,y)  extimg[x+(y)*extdimx]

#undef   OUTP
//#define  OUT(x,y) output [x+(y)*dimx]
#define  OUTP(x,y) output[x+(y)*extdimx]









int CCosmetic_inspection::Buffer_check( void )
{
//  if ( extdimx == xmax-xmin+1+2*radius ) return 0; // already Ok, just return
  if ( extdimx == x2_sw-x1_sw+1+2*radius ) return 0; // already Ok, just return

//  extdimx = xmax-xmin+1+2*radius; // horizontal size
  extdimx = x2_sw-x1_sw+1+2*radius; // horizontal size
  extdimy = dimy       +2*radius; // vertical size
  extdim  = extdimx * extdimy;    // total size

  if ( extimg ) 
  {
    free( extimg );
    extimg = NULL;
  }
  extimg  = (BYTE*) malloc( extdim );

  if ( output ) 
  {
    free( output );
    output = NULL;
  }
  output  = (BYTE*) malloc( extdim );

  circ_rad = 0; // force recalculate of offsets to keep new extdimx into account!
  TRACE("CCosmetic_inspection::Buffer_check executed - new extimg dimensions %dx%d\n", extdimx, extdimy );

  return 0;
}





/*

    circ_npt = 2*my_circle[SDCK_AMP].n;
    circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

    int i;
    int j=0;
    for ( i=0 ; i<my_circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  my_circle[SDCK_AMP].x[i]+my_circle[SDCK_AMP].y[i]*dimx,  my_circle[SDCK_AMP].x[i],  my_circle[SDCK_AMP].y[i] );
      circ_ofs[j++] =  my_circle[SDCK_AMP].x[i] + my_circle[SDCK_AMP].y[i]*dimx;
    }
    for ( i=0 ; i<my_circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -my_circle[SDCK_AMP].x[i]-my_circle[SDCK_AMP].y[i]*dimx, -my_circle[SDCK_AMP].x[i], -my_circle[SDCK_AMP].y[i] );
      circ_ofs[j++] = -my_circle[SDCK_AMP].x[i] - my_circle[SDCK_AMP].y[i]*dimx;
    }

    //TRACE("circ_ofs prepared - %d points\n", circ_npt );
    //for ( i=0 ; i<circ_npt ; i++ ) TRACE( "ofs[%2d] = %4d (%3d,%3d)\n", i, circ_ofs[i], circ_ofs[i]%dimx, circ_ofs[i]/dimx );
    //TRACE("FINISH\n");

    */


int CCosmetic_inspection::Circle_check( void )
{
  if ( radius==circ_rad && circ_ofs && circ_npt ) return 0; // already set, just returns

  if ( circ_ofs )
  {
    delete circ_ofs;
    circ_ofs = NULL;
  }

  circ_npt = 2*my_circle[radius].n;
  circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

  int i, j=0;
  for ( i=0 ; i<my_circle[radius].n ; i++ ) 
  {
    //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  my_circle[SDCK_AMP].x[i]+my_circle[SDCK_AMP].y[i]*dimx,  my_circle[SDCK_AMP].x[i],  my_circle[SDCK_AMP].y[i] );
    circ_ofs[j++] =  my_circle[radius].x[i] + my_circle[radius].y[i]*extdimx;
  }
  for ( i=0 ; i<my_circle[radius].n ; i++ ) 
  {
    //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -my_circle[SDCK_AMP].x[i]-my_circle[SDCK_AMP].y[i]*dimx, -my_circle[SDCK_AMP].x[i], -my_circle[SDCK_AMP].y[i] );
    circ_ofs[j++] = -my_circle[radius].x[i] - my_circle[radius].y[i]*extdimx;
  }

  circ_rad = radius;

  TRACE("CCosmetic_inspection::Circle_check executed - new radius=%d (%d points)\n", circ_rad, circ_npt );

  return 0;
}



int CCosmetic_inspection::Sidecake_analysis( BYTE* img_acq )
{
//  #define NPT (SDCK_AMP<<3) // perimeter ?distance*8
  int x, y;
  int i;

  radius = radavg; // set radius (constant during inspection)

//  Buffer_check(); // allocate / reallocate external buffer, if required
//  Circle_check();

  // execute autoalignment, if required
  x1_sw = xmin;
  x2_sw = xmax;

  ASSERT( dimx==2048 ); // if not, change shift below

  #ifdef NEWCAP_V21
  if ( minbrgt_sw>0 )
  {
    x = 0;
    //UINT ltot = 0;
    INT ltot = 0;
    while ( ltot<minbrgt_sw*dimy && x<dimx )
    {
      ltot = 0;
      for ( y=0 ; y<dimy ; y++ ) ltot+=img_acq[x+(y<<11)]; //PT(x,y);
      //TRACE("x=%3d - ltot=%d\n", x, ltot );
      x++;
    }

    if ( x<dimx ) // bottom found, set inspection coordinates
    {
      //TRACE("index=%d - x=%d\n", index, x );
      x1_sw = x     + x1ofs_sw;
      x2_sw = x1_sw + x2ofs_sw;
    }
    else // autoalignment failed, return
    {
      #ifdef FD_DATAREC
        if ( index<4 ) if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        else           if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 

      //TRACE("index=%d - NOT FOUND\n", index );
      txt1.Format( string_2374[lng] );
      return 69;  // result of inspection REJECT
    }

  }
  #endif

  Buffer_check(); // allocate / reallocate external buffer, if required
  Circle_check();

 
  // copy required part of image to extimg
  ZeroMemory( output, extdimx*extdimy );

//  if ( xmin<      radius ) xmin = radius;
//  if ( xmax>=dimx-radius ) xmax = dimx-radius-1;
  if ( x1_sw<      radius ) x1_sw = radius;
  if ( x2_sw>=dimx-radius ) x1_sw = dimx-radius-1;

  //ASSERT( xmin>=SDCK_AMP && xmax<dimx-SDCK_AMP );

  y = 0;
//  x = xmin-radius; //SDCK_AMP;
  x = x1_sw-radius; //SDCK_AMP;

  // top stripe (minus SDCK_AMP)
  for ( i=dimy-radius ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  } 

  // main, straight block
  for ( i=0 ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  // bottom stripe (extra SDCK_AMP)
  for ( i=0 ; i<radius ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  //Save_buffer( extimg, extdimx, extdimy, L"extimg" );
  //return 0;

  double coef = 1.0 / circ_npt;

  // START ANALYSIS
  for ( y=radius ; y<radius+dimy ; y++ )
  {
//    for ( x=radius ; x<=radius+xmax-xmin ; x++ )
    for ( x=radius ; x<=radius+x2_sw-x1_sw ; x++ )
    {
      int lum = 0;

      /*for ( i=0 ; i<circ_npt ; i++ )
      {
        //TRACE("(%3d,%3d)[%2d] = %3d\n", x, y, i, extimg[x+y*extdimx+circ_ofs[i]] );
        lum += extimg[x+y*extdimx+circ_ofs[i]];
      }
      lum = ( lum + (circ_npt>>1) ) / circ_npt; */

      BYTE* ptr = extimg+x+y*extdimx;
      for ( i=0 ; i<circ_npt ; i++ ) lum += ptr[circ_ofs[i]];
      lum = int( 0.5 + coef*lum );

      ASSERT( lum>=0 && lum<256 );

      //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
      if ( lum-min_dif-PT(x,y) > 0 ) OUTP(x,y) = lum-min_dif-PT(x,y);
 /*     {
        OUT(x,y) = lum-min_dif-PT(x,y);
        //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        //TRACE("fine: lum=%d\n", lum );
      } */

      if ( PT(x,y)-lum-max_dif > 0 ) OUTP(x,y) = PT(x,y)-lum-max_dif;
/*      {
        OUT(x,y) = PT(x,y)-lum-max_dif;
        //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        //TRACE("fine: lum=%d\n", lum );
      } */

      if ( PT(x,y)-max_abs > 0 ) OUTP(x,y) = PT(x,y)-max_abs;
      if ( min_abs-PT(x,y) > 0 ) OUTP(x,y) = min_abs-PT(x,y);

//      ASSERT( OUT(x,y)==0 );
      //ASSERT( lum == 123*circ_npt );

      /*
      int ofs0 = x + y*dimx;
      for ( i=0 ; i<circ_npt ; i++ ) lum += img_acq[ofs0+circ_ofs[i]];
      lum = ( lum + (circ_npt>>1) ) / circ_npt;

      //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
      if ( lum-min_dif-PT(x,y) > 0 )
      {
        output[x+y*dimx] = lum-min_dif-PT(x,y);
        //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        //TRACE("fine: lum=%d\n", lum );
      }
      */

    }
  }



/*  for ( y=0 ; y<dimy ; y++ )
  {
    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB

    if ( y>=SDCK_AMP && y<dimy-SDCK_AMP )
    {
      for ( x=xmin+SDCK_AMP ; x<=xmax-SDCK_AMP ; x++ )
      {
        // 
        int lum = 0;
        int ofs0 = x + y*dimx;
        for ( i=0 ; i<circ_npt ; i++ ) lum += img_acq[ofs0+circ_ofs[i]];
        lum = ( lum + (circ_npt>>1) ) / circ_npt;

        //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
        if ( lum-min_dif-PT(x,y) > 0 )
        {
          output[x+y*dimx] = lum-min_dif-PT(x,y);
          //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          //TRACE("fine: lum=%d\n", lum );
        }
      }
    }
  }  */

  //Save_buffer( output, extdimx, extdimy, L"output" );

  Calculate_blob_output();

  #ifdef FD_DATAREC
    if ( index<4 )
    {
      if ( datarec_on && result  ) result ->InsertI( cosm_err, nhead ); // insert cosm_err
    }
    else
    {
      if ( datarec_on && result2 ) result2->InsertI( cosm_err, nhead ); // insert cosm_err
    }
  #endif 


  if ( cosm_err > max_err )
  {
    //txt1.Format( string_2302[lng], cosm_err, max_err );
    txt1.Format( string_2302[lng], txt_desc, cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    //txt1.Format( string_2304[lng], cosm_err, max_err );
    txt1.Format( string_2304[lng], txt_desc, cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }


}


/*
int CCosmetic_inspection::Sidecake_analysis_orig( BYTE* img_acq )
{

  #undef SDCK_AMP
  #define SDCK_AMP       5  // semiamp for inspection
  #define NPT (SDCK_AMP<<3) // perimeter ?distance*8

  int x, y;
  int i;

  ZeroMemory( output, dimx*dimy );

  for ( y=0 ; y<dimy ; y++ )
  {
    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB


    if ( y>=SDCK_AMP && y<dimy-SDCK_AMP )
    {
      for ( x=xmin+SDCK_AMP ; x<=xmax-SDCK_AMP ; x++ )
      {
        // 
        int lum = 0;
        for ( i=0 ; i<2*SDCK_AMP+1 ; i++ )
        {
          lum += PT(x-SDCK_AMP+i,y-SDCK_AMP); // tratto superiore
          lum += PT(x-SDCK_AMP+i,y+SDCK_AMP); // tratto inferiore
          //TRACE("H(%2d) - (%3d,%3d)-(%3d,%3d)\n", i, x-SDCK_AMP+i, y-SDCK_AMP, x-SDCK_AMP+i, y+SDCK_AMP );
        }

        for ( i=1 ; i<2*SDCK_AMP ; i++ )
        {
          lum += PT(x-SDCK_AMP,y-SDCK_AMP+i); // tratto superiore
          lum += PT(x+SDCK_AMP,y-SDCK_AMP+i); // tratto inferiore
          //TRACE("V(%2d) - (%3d,%3d)-(%3d,%3d)\n", i, x-SDCK_AMP, y-SDCK_AMP+i, x+SDCK_AMP, y-SDCK_AMP+i );
        }


        lum = ( lum + (SDCK_AMP<<2) ) / NPT;

        //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
        if ( lum-min_dif-PT(x,y) > 0 )
        {
          output[x+y*dimx] = lum-min_dif-PT(x,y);
          //DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          //TRACE("fine: lum=%d\n", lum );
        }


      }
    }


  }  


  Calculate_blob_output();



  if ( cosm_err > max_err )
  {
    txt1.Format( string_2302[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2304[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }

}
*/



/*
#undef BLOCCO_BLOB



#define BLOCCO_BLOB(ox,oy)  if ( OUT(ox,oy) )               { \
                              dafx[end] = ox;                 \
                              dafy[end] = oy;                 \
                              OUT(dafx[end],dafy[end]) = 0;   \
                              end++;                        }

                              */



#undef BLOCCO_BLOB



#define BLOCCO_BLOB(ox,oy)  if ( OUTP(ox,oy) )               {       \
                              dafx[end] = ox;                        \
                              dafy[end] = oy;                        \
                              OUTP(dafx[end],dafy[end]) = 0;         \
                              end++;                         }




int CCosmetic_inspection::Calculate_blob_output( void )
{
//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

  cosm_err = 0;
 
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  


//  for ( y=1 ; y<dimy-1 ; y++ )
//    for ( x=xmin+1; x<=xmax-1 ; x++ )

  for ( y=radius ; y<radius+dimy ; y++ )
    //for ( x=radius ; x<=radius+xmax-xmin ; x++ )
    for ( x=radius ; x<=radius+x2_sw-x1_sw ; x++ )
    {
      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        num_blob++;   // new blob found     
        dafx[0]  = x;
        dafy[0]  = y;
        end      = 1;
        OUTP(x,y) = 0; // img[x+y*dimx] = 0; 
 
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
          //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_GREEN );  // GREEN - NO ERROR
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_YELLOW );  // YELLOW - SCORE[0]
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_ORANGE );  // LIGHT ORANGE - SCORE[1]
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_ORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_RED );  // ORANGE - SCORE[2]
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_RED );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_MAGENTA );  // RED - SCORE[3]
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_MAGENTA );  // RED - SCORE[3]
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

  return cosm_err;
}
 







// STANDARD SIDEWALL ANALYSIS (COMPARED TO HORIZONTAL AVERAGE)

#undef   PT
#define  PT(x,y)  img_acq[x+(y)*dimx]





int CCosmetic_inspection::Sidewall_analysis( BYTE* img_acq )
{
//  ASSERT( 0<=data->xmin && data->xmin<data->xmax-10 && data->xmax<data->dimx ); // Check upper/lower limit of inspection area
  //ASSERT( data->dimx && data->dimy );                                           // dimx and dimy have been defined
  //ASSERT( AfxIsValidAddress( img_in, data->dimx*data->dimy ) );                 // Check processing buffer 

/*  if (  <=0)
    data->xmin = 1;
  if(data->xmax >= DIMXLIN_ACQ)
    data->xmax = DIMXLIN_ACQ-2; */

//  BYTE* ptr = img_in;
 // int xmin, xmax;
  //TRACE("%d: difflum = %d\n", data->ind, data->difflum );
  // Image pre-scan to compute average brightness of all columns

  /*
  int x, y;

  UINT* lum_avg = (UINT*) malloc( (xmax-xmin+1)*sizeof(UINT) );
  ASSERT( lum_avg );
  //ZeroMemory( lum_avg, (xmax-xmin+1)*sizeof(int) );

  for ( x=xmin ; x<=xmax ; x++ )
  {
    lum_avg[x-xmin] = 0;
//    for ( y=ymin ; y<=ymax ; y++ ) lum_avg[x-xmin] += PT(x,y);
    for ( y=0 ; y<dimy ; y++ ) lum_avg[x-xmin] += PT(x,y);
    lum_avg[x-xmin] = lum_avg[x-xmin] / (ymax-ymin+1);
    ASSERT( lum_avg[x-xmin]>=0 && lum_avg[x-xmin]<=255 );
    if ( lum_avg[x-xmin]<min_dif ) lum_avg[x-xmin]=min_dif;
  }
  for ( x=xmin+1 ; x<=xmax-1 ; x++ )
  {
    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;
    //for ( y=ymin ; y<=ymax ; y++ )
    for ( y=1 ; y<dimy-1 ; y++ )
    {
      if ( PT(x,y)<lum_avg[x-xmin]-min_dif ) PT(x,y) = lum_avg[x-xmin]-min_dif-PT(x,y);
      else                                   PT(x,y) = 0;
    }
  }
  if ( lum_avg ) free( lum_avg );
  */

  int x, y;
  UINT lum = 0;


  #ifdef NEWCAP_V21 // new version 2.1 with auto-alignment

  // first calculate ALL average values of brightness through dimx
  double coeff = 1.0/dimy;
  for ( x=0 ; x<dimx ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    avglum[x] = UINT( 0.5 + lum*coeff );
    ASSERT( avglum[x]>=0 && avglum[x]<=255 );
  }

  // then calculate top and bottom heights (autoalignment)
  if ( minbrgt_sw>0 )
  {
    x2_sw = dimx-1; // start from rightmost
    while ( avglum[x2_sw]<minbrgt_sw && x2_sw>50 ) x2_sw--;

    if ( x2_sw<=50 ) // upper (right) x coordinate NOT found, fail and return
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 
      txt1.Format( string_2360[lng] );
      return 131;
    }

    // ok, change of brightness found. calculate x1 and x2
    x2_sw = x2_sw + x2ofs_sw; // compute right/top analysis coord
    x1_sw = x2_sw - x1ofs_sw; // compute left/bottom analysis coord

    if ( x1_sw<5 || x2_sw>dimx-5 || x2_sw-x1_sw<20 ) // something wrong with autoaligned sidewall area, error and return
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 
      txt1.Format( string_2362[lng] );
      return 132;
    }
  }
  else // autoalignemnt OFF
  {
    x1_sw = xmin+1;
    x2_sw = xmax-1;
  }
  
  // ok, left/bottom and right/top coordinates valid, start analysis

  BYTE min_rel;
  BYTE max_rel;

  for ( x=x1_sw ; x<=x2_sw ; x++ )
  {
    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;

    if ( avglum[x]>min_dif )     min_rel = avglum[x]-min_dif;
    else                         min_rel = 0;

    if ( avglum[x]+max_dif<255 ) max_rel = avglum[x]+max_dif;
    else                         max_rel = 255;

    for ( y=1 ; y<dimy-1 ; y++ )
    {
// ORIGINAL - ONLY THIS ONE
//      if ( PT(x,y)<avglum[x]-min_dif ) PT(x,y) = avglum[x]-min_dif-PT(x,y);
//      else                             PT(x,y) = 0; 

//      PT(x,y) = 0;

      if ( PT(x,y)<min_rel )                PT(x,y) = 10; // avglum[x]-min_dif-PT(x,y);
      else if ( PT(x,y)>max_rel )           PT(x,y) = 20; // PT(x,y)-avglum[x]-max_dif;
           else if ( PT(x,y)<min_abs )      PT(x,y) = 30; // min_abs-PT(x,y);
                else if ( PT(x,y)>max_abs ) PT(x,y) = 40; // PT(x,y)-max_abs;
                     else                   PT(x,y) = 0;
    }
  }

  //Pour added for upper area analysis for powder sticking on the wall on 20200522
	#ifdef UPPER_COSMETIC

    BYTE min_rel_upper;
	BYTE max_rel_upper;
	//x2_sw_bottom = x2_sw + offsets_bottom;
	//x2_sw_top    = x2_sw + offsets_top;


    //for ( x=x2_sw + 30; x<=x2_sw + 100 ; x++ )
	for ( x=x2_sw + offsets_bottom; x<=x2_sw + offsets_top ; x++ )
  {
    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;

    if ( avglum[x]>min_dif_upper )     min_rel_upper = avglum[x]-min_dif_upper;
    else                         min_rel_upper = 0;

    if ( avglum[x]+max_dif_upper<255 ) max_rel_upper = avglum[x]+max_dif_upper;
    else                         max_rel_upper = 255;

    for ( y=1 ; y<dimy-1 ; y++ )
    {
// ORIGINAL - ONLY THIS ONE
//      if ( PT(x,y)<avglum[x]-min_dif ) PT(x,y) = avglum[x]-min_dif-PT(x,y);
//      else                             PT(x,y) = 0; 

//      PT(x,y) = 0;

      if ( PT(x,y)<min_rel_upper )                PT(x,y) = 10; // avglum[x]-min_dif-PT(x,y);
      else if ( PT(x,y)>max_rel_upper )           PT(x,y) = 20; // PT(x,y)-avglum[x]-max_dif;
           else if ( PT(x,y)<min_abs_upper )      PT(x,y) = 30; // min_abs-PT(x,y);
                else if ( PT(x,y)>max_abs_upper ) PT(x,y) = 40; // PT(x,y)-max_abs;
                     else                   PT(x,y) = 0;
    }
  }
	
	//#endif //Pour removed on 20210327 to solve the cosmetic problem
  //Pour added for upper area analysis for powder sticking on the wall on 20200522

  #else  // older version WITHOUT autoalignment

  for ( x=0 ; x<=xmin ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    avglum[x] = (lum+(dimy>>1)) / dimy;
  }
  
  x1_sw = xmin+1;
  x2_sw = xmax-1;

  for ( x=x1_sw ; x<=x2_sw ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    lum = (lum+(dimy>>1)) / dimy;
    ASSERT( lum>=0 && lum<=255 );

    avglum[x] = lum;

    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;

    for ( y=1 ; y<dimy-1 ; y++ )
    {
      if ( PT(x,y)<lum-min_dif ) PT(x,y) = lum-min_dif-PT(x,y);
      else                       PT(x,y) = 0;
    }
  }

  #endif

  #endif

  // SEARCH FOR CAKE VOLUME (IF ENABLED)
  if ( level_min>0 && level_max>level_min )
  {
    level = 0;
    x = xmin; // start from bottom of inspection area
    while ( x>20 && avglum[x]>level_th ) x--;
    if ( x>20 )
    {
      level_high = x;

      x = 10;
      while ( x<level_high-10 && avglum[x]>level_th ) x++;
      if ( x<level_high-10 )
      {
        level_low = x;
        level = level_high - level_low; // calculate filling volume found

        #ifdef FD_DATAREC
          if ( datarec_on && result2 ) result2->InsertI( level, nhead ); // insert level into cake-volume (result2) data
        #endif 

        UINT col = COL_GREEN;
        if ( level < level_min )   // volume TOO LOW
        {

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION

	    //Camera A
		if (index==6 && level > level_min2)
		//if (cosmetic_insp[ind]->level > cosmetic_insp[ind]->level_min2 && ind==5)
		{
		 ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
		}
		if (index==6 && level <= level_min2)
		//if (cosmetic_insp[ind]->level <= cosmetic_insp[ind]->level_min2 && ind==5)
		{
		ADS_write_value(".F_ENABLE_COSMETIC_1", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
		}
		//Camera B
	  	if (index==7 && level > level_min2)
		//if (cosmetic_insp[ind]->level > cosmetic_insp[ind]->level_min2 && ind==6)
		{
		 ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
		}
		if (index==7 && level <= level_min2)
		//if (cosmetic_insp[ind]->level <= cosmetic_insp[ind]->level_min2 && ind==6)
		{
		ADS_write_value(".F_ENABLE_COSMETIC_2", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
		}
		//Camera C
		if (index==8 && level > level_min2)
		//if (cosmetic_insp[ind]->level > cosmetic_insp[ind]->level_min2 && ind==7)
		{
		ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
		}
		if (index==8 && level <= level_min2)
		//if (cosmetic_insp[ind]->level <= cosmetic_insp[ind]->level_min2 && ind==7)
		{
		 ADS_write_value(".F_ENABLE_COSMETIC_3", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
		}
		//Camera D
		if (index==9 && level > level_min2)
		//if (cosmetic_insp[ind]->level > cosmetic_insp[ind]->level_min2 && ind==8)
		{
		ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
		}
		if (index==9 && level <= level_min2)
		//if (cosmetic_insp[ind]->level <= cosmetic_insp[ind]->level_min2 && ind==8)
		{
		 ADS_write_value(".F_ENABLE_COSMETIC_4", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
		}
		//Pour added 20180927

	#endif //REJECTION_SEPARATION

          txt2.Format( string_2312[lng], level, level_min, level_max );

	#ifdef TIVS_S60
		  txt2.Format( string_2312a[lng], level, level_min, level_max );
	#endif
          col = COL_RED;
        }
        else
        {
          if ( level > level_max ) // volume TOO HIGH
          {

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
			if ( index==6 && level < level_max2)
	        //if (cosmetic_insp[ind]->level < cosmetic_insp[ind]->level_max2 && ind==5)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
			}
			if ( index==6 && level >= level_max2)
			//if (cosmetic_insp[ind]->level >= cosmetic_insp[ind]->level_max2 && ind==5)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_1", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
			}
			//Camera B
	  		if (index==7 &&  level < level_max2)
			//if (cosmetic_insp[ind]->level < cosmetic_insp[ind]->level_max2 && ind==6)
			{
			 ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
			}
			if (index==7 && level >= level_max2)
			//if (cosmetic_insp[ind]->level >= cosmetic_insp[ind]->level_max2 && ind==6)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_2", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
			}
			//Camera C
			if (index==8 && level < level_max2)
			//if (cosmetic_insp[ind]->level < cosmetic_insp[ind]->level_max2 && ind==7)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
			}
			if (index==8 && level >= level_max2)
			//if (cosmetic_insp[ind]->level >= cosmetic_insp[ind]->level_max2 && ind==7)
			{
			 ADS_write_value(".F_ENABLE_COSMETIC_3", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
			}
			//Camera D
			 if (index==9 && level < level_max2)
			//if (cosmetic_insp[ind]->level < cosmetic_insp[ind]->level_max2 && ind==8)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
			}
			if (index==9 && level >= level_max2)
			//if (cosmetic_insp[ind]->level >= cosmetic_insp[ind]->level_max2 && ind==8)
			{
			ADS_write_value(".F_ENABLE_COSMETIC_4", TRUE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 TRUE
			}
	#endif
	//Pour added 20180927

            txt2.Format( string_2314[lng], level, level_min, level_max );

	#ifdef TIVS_S60
		  txt2.Format( string_2314a[lng], level, level_min, level_max );
	#endif
            col = COL_RED;
          }
          else                     // Cake volume Ok
          {
            txt2.Format( string_2316[lng], level, level_min, level_max );

	#ifdef TIVS_S60
		  txt2.Format( string_2316a[lng], level, level_min, level_max );
	#endif

            col  = COL_GREEN;
            col2 = TXT_DARKGREEN;
          }
        }

        for ( y=0 ; y<dimy ; y+=8 )
        {
          DRAW_PT( index, level_high, y, col ); // 0xFFRRGGBB
          DRAW_PT( index, level_low,  y, col ); // 0xFFRRGGBB
        }

        if ( col==COL_RED ) // rejected because TOO LOW or TOO HIGH
        {
          #ifdef FD_DATAREC
            if ( datarec_on && result ) result->InsertI( -1, nhead ); // insert cosm_err into sidewall (result) data
          #endif 
          return 89;
        }
      }
      else // x>=level_high-10 => LOWER cake level NOT FOUND
      {
        #ifdef FD_DATAREC
          if ( datarec_on && result  ) result ->InsertI( -1, nhead ); // insert -1 into sidewall (result) data
          if ( datarec_on && result2 ) result2->InsertI( -1, nhead ); // insert -1 into cake-volume (result2) data
        #endif 
        txt2.Format( string_2320[lng] );
        return 92;
      }
    }
    else // x<20 => UPPER cake level NOT FOUND
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( -1, nhead ); // insert -2 into sidewall (result) data
        if ( datarec_on && result2 ) result2->InsertI( -2, nhead ); // insert -2 into cake-volume (result2) data
      #endif 
      txt2.Format( string_2322[lng] );
      return 91;
    }
  }
  else  // cake volume disabled
  {
    #ifdef FD_DATAREC
      if ( datarec_on && result2 ) result2->InsertI( -3, nhead ); // insert -3 into cake-volume (result2) data
    #endif 
    txt2.Format( string_2324[lng] );
    col2 = 0;
  }


  Calculate_blob( img_acq );
  
#ifdef UPPER_COSMETIC
  Calculate_blob_upper( img_acq );
#endif

  #ifdef FD_DATAREC
    if ( datarec_on && result ) result->InsertI( cosm_err, nhead ); // insert cosm_err into sidewall (result) data
  #endif 

  if ( cosm_err > max_err || cosm_err_upper > max_err_upper )
  {

	//Pour added 20171015
	#ifdef REJECTION_SEPARATION
	if ( index==6 && level < level_max2)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==7 && level < level_max2)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==8 && level < level_max2)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==9 && level < level_max2)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	#endif
	//Pour added 20171015

	txt1.Format( string_2330a[lng], cosm_err_upper,cosm_err, max_err );
    //txt1.Format( string_2330[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2330a[lng], cosm_err_upper,cosm_err, max_err );
	//txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
	//Pour added 20171013
	#ifdef REJECTION_SEPARATION
	if ( index==6)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_1", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==7)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_2", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==8)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_3", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	if ( index==9)
	{
	ADS_write_value(".F_ENABLE_COSMETIC_4", FALSE, CTYPE_BOOL8); // Force enable of COSMETIC reject to tray 1 FALSE
	}
	#endif
	//Pour added 20171013

	//for ( y=0 ; y<dimy ; y+=8 )
    //    {
    //      DRAW_PT( index,x2_sw+100 , y, col1 ); // 0xFFRRGGBB
    //    }

    return 0;  // result of inspection GOOD
  }

  /*
    if ( cosm_err_upper > max_err_upper )
  {
	txt1.Format( string_2330a[lng], cosm_err_upper,cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
	txt1.Format( string_2330a[lng], cosm_err_upper,cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }
  */



/*
  int filter[DIMXLIN_ACQ];

  ZeroMemory( filter, sizeof(filter) );
  x = 4;
  filter[x] = lum_avg[x+1]+lum_avg[x+2]+lum_avg[x+3]+lum_avg[x+4] - lum_avg[x-4]-lum_avg[x-3]-lum_avg[x-2]-lum_avg[x-1];

  while ( x < dimx-5 )
  {
    x++;
    filter[x] += lum_avg[x-5]+lum_avg[x-1]-lum_avg[x]-lum_avg[x+4];
    //TRACE( "%d,%d\n", x, filter[x] );
  };
  */

  //  ATTENZIONE: RICERCA AUTOMATICA SOLO SE XMIN O XMAX SONO MESSI A ZERO !

/*  if ( xmin )
  {
    xmin = xmin; // Fisso, impostato da operatore
  }
  else // calcolato automaticamente
  {
    // Search left border (negative value for filter)
    int left_threshold = -30;
    x = 500;
    while ( x > 10 && filter[x]>left_threshold ) x--;
    if ( x > 10 ) xmin = x + 10; // Left border found!
    else          xmin =    170; // Left border NOT found - fix value
  } */


/*  if ( data->xmax )
  {
    xmax = data->xmax; // Fisso, impostato da operatore
  }
  else // calcolato automaticamente
  {
    // Search right border (positive value for filter)
    int right_threshold = 30;
    x = 500;
    while ( x < DIMXLIN_ACQ && filter[x]<right_threshold ) x++;
    if ( x < DIMXLIN_ACQ ) 
      xmax = x - 10; // Right border found!
    else            
      xmax =    860; // Right border NOT found - fix value
  } */


  /*
  if ( tot_lum < 80 ) // Whole line is too dark! Mark error!
  {
     // TO DO !!!
  }

  BYTE* blob_img = (BYTE*) _aligned_malloc( dim , 16 );

  ZeroMemory( blob_img, DIMXLIN_ACQ*DIMYLIN_ACQ );

  // SECOND PASS WITH INSPECTION
  for ( x=xmin ; x<=xmax ; x++ )
  {

    BYTE reference;
    if ( lum_avg[x] > min_dif )   reference = BYTE( lum_avg[x]-min_dif );
    else                          reference = 0;

    ptr = img_in+x;
    BYTE* blob_ptr = blob_img+x;

    for ( y=0 ; y<DIMXLIN_ACQ*DIMYLIN_ACQ ; y+=DIMXLIN_ACQ )
    {
      BYTE pippo = ptr[y];
      if ( reference-ptr[y] > 0  )
      {
        blob_ptr[y] = reference-ptr[y];
      }
    }
  }
    
    */
//  return( TRUE );
}




#undef  BLOCCO_BLOB

#define BLOCCO_BLOB(ox,oy)  if ( PT(ox,oy) )                { \
                              dafx[end] = ox;                 \
                              dafy[end] = oy;                 \
                              PT(dafx[end],dafy[end]) = 0;    \
                              end++;                        }


/*
#define BLOCCO_BLOB( ox , oy )  if ( img[ox+(oy)*dimx] )               { \
                                   dafx[end] = ox;                       \
                                   dafy[end] = oy;                       \
                                   img[dafx[end]+dafy[end]*dimx] = 0;    \
                                   end++;                              }
                                   */
                                   // TRACE("pt[%3d]=%4d,%4d\n",end,dafx[end],dafy[end]);  \


int CCosmetic_inspection::Calculate_blob( BYTE* img_acq )
{
//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );


  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;
 

/*  for ( x=xmin ; x<=xmax ; x++ )
  {
    img[x+ymin*dimx] = 0;
    img[x+ymax*dimx] = 0;
    DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
  }  
  for ( y=ymin ; y<=ymax ; y++ )
  {
    img[xmin+y*dimx] = 0;
    img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
  }  */


/*  for ( x=xmin ; x<=xmax ; x++ )  // img[x] = 0;
  {
    img[x+       0*dimx] = 0;
    img[x+(dimy-1)*dimx] = 0;
  }   */


  for ( y=0 ; y<dimy ; y++ )
  {
/*    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB*/
    PT(x1_sw,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(x2_sw,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  

//  img[xmin+1] = 0;
  

/*
  for ( y = ymin ; y <= ymax ; y++ )
  {
    DRAW_PT( ind, xmin, y,  COLORE_AREEISP );
    //img_in[xmin+y*(image_dsp[0]->dim_dsp.cx)]=0;
    img_in[xmin+y*DX]=0;
    DRAW_PT( ind, xmax, y,  COLORE_AREEISP );
    //img_in[xmax+y*(image_dsp[0]->dim_dsp.cx)]=0;
    }
    else
    {
    DRAW_PTSHOW( ind, xmin, y,  COLORE_AREEISP );
    //img_in[xmin+y*(image_dsp[0]->dim_dsp.cx)]=0;
    img_in[xmin+y*DX]=0;
    DRAW_PTSHOW( ind, xmax, y,  COLORE_AREEISP );
    }
    img_in[xmax+y*DX]=0;
  } 

  for ( x = xmin ; x <= xmax ; x++ )
  {
    // DISEGNA TRATTI VERTICALI AREA DI ANALISI
    if(!pView->windowsopen[ind])
    {
    DRAW_PT( ind, x, ymin,  COLORE_AREEISP );
    img_in[x+ymax*DX]=0;
    DRAW_PT( ind, x, ymax,  COLORE_AREEISP );
    img_in[x+ymin*DX]=0;
    }
    else
    {
    DRAW_PTSHOW( ind, x, ymin,  COLORE_AREEISP );
    img_in[x+ymax*DX]=0;
    DRAW_PTSHOW( ind, x, ymax,  COLORE_AREEISP );
    img_in[x+ymin*DX]=0;
    }
  }
  */

/*  #ifdef NEWCAP_V21
    int xsta = xmin+1;
    int xend = xmax-1;
  #else
    int xsta = xmin+1;
    int xend = xmax-1;
  #endif */


//  for ( y = ymin ; y <= ymax ; y++ )
  for ( y=1 ; y<dimy-1 ; y++ )
      //    for ( x=xmin+1; x<=xmax-1 ; x++ )
    for ( x=x1_sw+1; x<x2_sw ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( PT(x,y) ) // img[x+y*dimx] ) // ( TEST(x+y*DX) )  
      {
        num_blob++;   // new blob found     
        dafx[0] = x;
        dafy[0] = y;
        end     = 1;
        PT(x,y) = 0; // img[x+y*dimx] = 0; 
 
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
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_LIGHTORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
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

  return cosm_err;
}
 

//Pour added for upper cosmetic on 20200523
int CCosmetic_inspection::Calculate_blob_upper( BYTE* img_acq )
{
//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );


  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

  //for ( y=0 ; y<dimy ; y++ )
  //{
  //  PT(x2_sw+30,y) = 0;   //img[xmin+y*dimx] = 0;
  //  PT(x2_sw+100,y) = 0;   //img[xmax+y*dimx] = 0;
  //  DRAW_PT( index, x2_sw+30, y, area_color ); // 0xFFRRGGBB
  //  DRAW_PT( index, x2_sw+100, y, area_color ); // 0xFFRRGGBB
  //}  

    for ( y=0 ; y<dimy ; y++ )
  {
    PT(x2_sw + offsets_bottom,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(x2_sw + offsets_top,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, x2_sw + offsets_bottom, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw + offsets_top, y, area_color ); // 0xFFRRGGBB
  }  


//  for ( y = ymin ; y <= ymax ; y++ )
  for ( y=1 ; y<dimy-1 ; y++ )
      //    for ( x=xmin+1; x<=xmax-1 ; x++ )
    for ( x=x2_sw + offsets_bottom; x<x2_sw + offsets_top ; x++ )
    {

      // Se il pixel ?maggiore di 0 ho un nuovo blob
      if ( PT(x,y) ) // img[x+y*dimx] ) // ( TEST(x+y*DX) )  
      {
        num_blob++;   // new blob found     
        dafx[0] = x;
        dafy[0] = y;
        end     = 1;
        PT(x,y) = 0; // img[x+y*dimx] = 0; 
 
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
            cosm_err_upper += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err_upper += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_LIGHTORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err_upper += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err_upper += blob_score[3].score;// + end;
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

  return cosm_err_upper;
}

//Pour added for upper cosmetic on 20200523


#undef   PT
#define  PT(x,y)  img_acq[x+(y)*dimx]

#undef   OUTP
#define  OUTP(x,y) output[x+(y)*dimx]

#undef   BLOCCO_BLOB
#define  BLOCCO_BLOB(ox,oy) if ( OUTP(ox,oy) )               {       \
                              tote     += OUTP(ox,oy);               \
                              dafx[end] = ox;                        \
                              dafy[end] = oy;                        \
                              if ( bbox.left  >ox ) bbox.left  =ox;  \
                              if ( bbox.right <ox ) bbox.right =ox;  \
                              if ( bbox.top   >oy ) bbox.top   =oy;  \
                              if ( bbox.bottom<oy ) bbox.bottom=oy;  \
                              OUTP(dafx[end],dafy[end]) = 0;         \
                              end++;                         }


int CCosmetic_inspection::Sidewall_PFS( BYTE* img_acq )
{
//  #define NPT (SDCK_AMP<<3) // perimeter é distance*8
  int x, y;
//  int i;


  ASSERT( AfxIsValidAddress( img_acq, dimx*dimy ) );


  // starting point xmin/xmax
/*  for ( y=0 ; y<dimy ; y++ )
  {
    DRAW_PT( index, xmin, y, 0xFF0000FF ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, 0xFF0000FF ); // 0xFFRRGGBB
  } */

  // find left/xmin position


  #define NSTRIPES 10

  int width = dimy / NSTRIPES;
  ASSERT( NSTRIPES * width == dimy );

//  ASSERT( AfxIsValidAddress( output, dimx*dimy ) );
//  ZeroMemory( output, dimx*dimy );

  // horizontal lines from initial inspection area
/*  for ( y=0 ; y<dimy ; y+=4 )
  {
    DRAW_PT( index, xmin, y, COL_BLUE ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, COL_BLUE ); // 0xFFRRGGBB
  } */

  // cycle for stripes 

  int x1 =     0; // bottom of lower inspection area (into  liquid)
  int x2 =     0; // top    of lower inspection area (into  liquid)

  int y1 =     0; // start y of this stripe
  int y2 = width; // end   y of this stripe

 // for ( x1=0 ; x1<dimx*dimy ; x1++ ) output[x1];

//  bott_err = 0;

  ASSERT( AfxIsValidAddress( output, dim ) );
  ZeroMemory( output, dim );


  /*
  int     x1_perc = 70;    // brightness decrease to detect x1
  int     x1_band =  5;    // distance band above/right of x1
  int     x2_perc = 70;    // brightness decrease to detect x2
  int     x2_band =  5;    // distance band above/right of x2
  //int     x3_perc;    // brightness decrease to detect x3
  //int     x3_band;    // distance band above/right of x3

  int     min_absl = 180;   // minimum allowed absolute brightness (area into liquid)
  int     min_absb = 180;   // minimum allowed absolute brightness (bottom profile)


  int     min_width  =  10;  // minimum width
  int     min_height =  10;  // minimum height
  int     min_dim    = 100;  // minimum dimension of blob
  int     min_elong  =   1;  // minimum elongation (width/height - height/width on screen)
  int     min_avgdf  =  10;  // minimum average brightness
  */

//  int x1_perc = 50;
#ifdef SIDEWALL_PFS

  	if ( index>=1 && index<= 2)//Only for C3A,C3B
	{
  for ( int i=0 ; i<NSTRIPES ; i++ ) // y=index of stripe
  {
    //  SEARCH X1/X2 OF LOWER AREA (LIQUID)
    // search lower edge of lower area (x1)
    //TRACE("STR[%d] x=%d lum_iniz=%d - lum_thr=%d\n", i, x, lum, lum_thr );

    UINT lum = 0;
    x = (xmin+xmax)/2;
    for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
    UINT lum_thr1 = lum*x1_perc/100; // lower threshold value
    UINT lum_thr2 = lum*x2_perc/100; // upper threshold value

    x1 = 0;
    x2 = 0;

    if ( x1_perc==0 ) // NO SEARCH/ALIGNMENT ON LEFT/BOTTOM AREA
    {
      x1 = xmin;
      for ( y=y1 ; y<y2 ; y+=2 ) DRAW_PT( index, x1, y, COL_MAGENTA ); // 0xFFRRGGBB
    }
    else
    {
      x = xmin; // start from lower insp area
      do
      {
        x -= 2;
        lum = 0;
        for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
        //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );
      } while ( x>5 && lum>lum_thr1 );

      if ( x>5 ) // valid bottom found
      {
        x1 = x + x1_band;
        for ( y=y1 ; y<y2 ; y+=2 ) DRAW_PT( index, x1, y, COL_MAGENTA ); // 0xFFRRGGBB
      }
    }

    // search upper limit of lower insp area (x2)

    if ( x2_perc==0 ) // NO SEARCH/ALIGNMENT ON RIGHT/TOP AREA
    {
      x2 = xmax;
      for ( y=y1 ; y<y2 ; y+=2 ) DRAW_PT( index, x2, y, COL_MAGENTA ); // 0xFFRRGGBB
    }
    else
    {
      x = xmax;
      do
      {
        x += 2;
        lum = 0;
        for ( y=y1 ; y<y2 ; y+=4 ) lum += PT(x,y);
        //TRACE("STR[%d] x=%d lum=%d\n", i, x, lum );

      } while ( x<dimx-5 && lum>lum_thr2 );

      if ( x<dimx-5 ) // valid position
      {
        x2 = x - x2_band;
        for ( y=y1 ; y<y2 ; y+=2 ) DRAW_PT( index, x2, y, COL_MAGENTA ); // 0xFFRRGGBB
      }
    }


    // ANALYSIS OF THE STRIPE
    if ( x1 && x2 ) // only if ALL coords have been found proceed with analysis
    {

 /*     int xt = x1 +  20; // right/top   of bottom profile analysis
      if ( xt<5 ) xt = 5;

      int xb = xt - 150; // left/bottom of bottom profile analysis
      if ( xb<5 ) xb = 5;*/

      for ( y=y1 ; y<y2 ; y++ )
      {

        // analysis of LOWER part
        for ( x=x1 ; x<=x2 ; x++ )
        {
          //DRAW_PT( index, x, y, COL_YELLOW ); // 0xFFRRGGBB

          if ( min_absl-PT(x,y)>0 )
          {
            OUTP(x,y) = min_absl-PT(x,y);
            DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
          }
        }

        /*
        // FIND BOTTOM PROFILE
        DRAW_PT( index, xb, y, COL_BLUE ); // 0xFFRRGGBB
        DRAW_PT( index, xt, y, COL_BLUE ); // 0xFFRRGGBB

        int npb = 0;

        // analysis of LOWER part
        for ( x=xb ; x<=xt ; x++ ) if ( PT(x,y)<min_absb ) npb++;

        if ( npb>maxbt_thk ) // if exceeds maximum bottom thickness
        {
          bott_err += npb-maxbt_thk;
          for ( x=xb ; x<=xt ; x+=2 )
          {
            if ( PT(x,y)<min_absb ) DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
          }
        }
        */

      }


    }
    else // not found, draw a RED cross
    {
      for ( x=xmin+20 ; x<xmax-20 ; x+=2 )
      {
        DRAW_PT( index, x, y1+(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, y2-(y2-y1)*(x-xmin)/(xmax-xmin), COL_RED ); // 0xFFRRGGBB
      }

      cosm_err += 100;
    }


    // Move to next stripe
    y1 += width;
    y2 += width;


  } // for ( int i=0 ; i<NSTRIPES ; i++ ) // y=index of stripe
}//Only for C3A,C3B
#endif



    // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob

  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
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


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 100;

       // if ( width>height ) elong = width/height;
        if ( width>2 && height>2 ) elong = 100*height/width;
     

        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 100 se width O height sono minori di 2, altrimenti è il rapporto moltitplicato 100 E=10 significa largo e stretto, E=1000 significa stretto e alto




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
#ifdef SIDEWALL_PFS

		if ( index>=1 && index<= 2)//Only for C3A,C3B
		{

        if ( end>=min_dim && elong>=min_elong && width>=min_height && height>=min_width && tote>=end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }

		} //Only for C3A,C3B
#endif
        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );
  
  good = TRUE;

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }

/*  if ( bott_err > bott_maxerr )
  {
    txt2.Format( string_2380[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt2.Format( string_2382[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }*/


  if ( good ) return 0;

  return 99; // reject











  /*

  radius = radavg; // set radius (constant during inspection)

//  Buffer_check(); // allocate / reallocate external buffer, if required
//  Circle_check();

  // execute autoalignment, if required
  x1_sw = xmin;
  x2_sw = xmax;

  ASSERT( dimx==2048 ); // if not, change shift below

  Buffer_check(); // allocate / reallocate external buffer, if required
  Circle_check();

 
  // copy required part of image to extimg
  ZeroMemory( output, extdimx*extdimy );

//  if ( xmin<      radius ) xmin = radius;
//  if ( xmax>=dimx-radius ) xmax = dimx-radius-1;
  if ( x1_sw<      radius ) x1_sw = radius;
  if ( x2_sw>=dimx-radius ) x1_sw = dimx-radius-1;

  //ASSERT( xmin>=SDCK_AMP && xmax<dimx-SDCK_AMP );

  y = 0;
//  x = xmin-radius; //SDCK_AMP;
  x = x1_sw-radius; //SDCK_AMP;

  // top stripe (minus SDCK_AMP)
  for ( i=dimy-radius ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  } 

  // main, straight block
  for ( i=0 ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  // bottom stripe (extra SDCK_AMP)
  for ( i=0 ; i<radius ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  //Save_buffer( extimg, extdimx, extdimy, L"extimg" );
  //return 0;

  double coef = 1.0 / circ_npt;

  // START ANALYSIS
  for ( y=radius ; y<radius+dimy ; y++ )
  {
//    for ( x=radius ; x<=radius+xmax-xmin ; x++ )
    for ( x=radius ; x<=radius+x2_sw-x1_sw ; x++ )
    {
      int lum = 0;


      BYTE* ptr = extimg+x+y*extdimx;
      for ( i=0 ; i<circ_npt ; i++ ) lum += ptr[circ_ofs[i]];
      lum = int( 0.5 + coef*lum );

      ASSERT( lum>=0 && lum<256 );

      if ( PT(x,y)-max_abs > 0 ) OUTP(x,y) = PT(x,y)-max_abs;
      if ( min_abs-PT(x,y) > 0 ) OUTP(x,y) = min_abs-PT(x,y);

//      ASSERT( OUT(x,y)==0 );
      //ASSERT( lum == 123*circ_npt );


    }
  }

*/


  //Save_buffer( output, extdimx, extdimy, L"output" );

  //Calculate_blob_output();

  cosm_err = 0;

  if ( cosm_err > max_err )
  {
    //txt1.Format( string_2302[lng], cosm_err, max_err );
    txt1.Format( string_2302[lng], txt_desc, cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    //txt1.Format( string_2304[lng], cosm_err, max_err );
    txt1.Format( string_2304[lng], txt_desc, cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }


}





#undef   PT
#undef   OUTP
#undef BLOCCO_BLOB