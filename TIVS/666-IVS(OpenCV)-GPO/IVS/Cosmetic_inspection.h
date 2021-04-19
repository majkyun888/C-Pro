

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"



#define NCRIT_BLOB          4  // number of blob criteria used to calculate total error 

#define CSMTYPE_SIDEWALL    1  // Basic dark areas inspection (liquid and sidewall)
#define CSMTYPE_CAKEVOLUME  2  // Calculates cake volume by H profile brightness threshold
#define CSMTYPE_SIDECAKE    4
#define CSMTYPE_TOPCAKE     8
#define CSMTYPE_COMBO      128 //NEW cosemtic combining both LIQUID and FREEZE DRIED modes
#define CSMTYPE_PFS       256  // New cosmetic for PFS S40


class CCosmetic_inspection
{

//	DECLARE_DYNAMIC(CCosmetic_inspection)

public:
  CCosmetic_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int type_in=CSMTYPE_SIDEWALL );
  ~CCosmetic_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child, wchar_t** name=NULL );

  int Add_menu_parameters_v20( CContainer_wnd* child, wchar_t** name/*=NULL*/ );
  int Add_menu_parameters_v30( CContainer_wnd* child, wchar_t** name/*=NULL*/ );
  int Add_menu_parameters_combo( CContainer_wnd* child, wchar_t** name/*=NULL*/ );//20190522
  int Add_menu_parameters_PFS  ( CContainer_wnd* child, wchar_t** name/*=NULL*/ );

  int Process_image ( BYTE* img_acq, UINT nimg=0 );
  
  int Cosmetic_v20_analysis1( BYTE* img_acq );
  int Cosmetic_v20_analysis( BYTE* img_acq );
  int Cosmetic_v30_analysis( BYTE* img_acq );

  int Sidewall_analysis( BYTE* img_acq );
  int Sidecake_analysis( BYTE* img_acq );
  int Sidewall_PFS     ( BYTE* img_acq );

  int Cosmetic_analisys_freeze( BYTE* img_acq );//20190522
  int Cosmetic_analisys_liquid( BYTE* img_acq );//20190522
  
  //Pour added for upper cosmetic
  int Calculate_blob_upper( BYTE* img_acq );
  //Pour added for upper cosmetic

  int Calculate_blob( BYTE* img_acq );
  int Calculate_blob_output( void );

  //int Sidecake_analysis_orig( BYTE* img_acq );

  int Buffer_check( void );
  int Circle_check( void );


  static int Set_inspection_area( int ind );

  CFile_mngmt*  file_manager;
  CArea_adjust* area; // inspection/search area

  BYTE*   output;     // output buffer/image for blox analysis
  int     index;      // index of camera/image to be bound to
  int     type;       // type of inspection

  int     nhead;      // used for data collection

  // results area
  wchar_t* txt_desc;    // description text (SideCake or TopCake)
  BOOL    good;       // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;       // first row of result's text
  CString txt2;       // second row of result's text
  UINT    col1;       // color of first row of text
  UINT    col2;       // color of second row of text

  // inspection parameters
  int     min_dif_upper;    // minimum brightness difference during analysis
  int     max_dif_upper;    // maximum brightness difference during analysis
  int     min_abs_upper;    // minimum allowed absolute brightness
  int     max_abs_upper;    // maximum allowed absolute brightness

  int     min_dif;    // minimum brightness difference during analysis
  int     max_dif;    // maximum brightness difference during analysis
  int     min_abs;    // minimum allowed absolute brightness
  int     max_abs;    // maximum allowed absolute brightness
  int     radius;     // radius of circle used for averaging
  int     radavg;     // temporary radius used during inspection

  // new params v20
  #if defined NEWCOSMETIC_V20  || defined NEWCOSMETIC_C40 

  int     min_absl;   // minimum allowed absolute brightness (lower area, into liquid)

  int     x1_perc;    // brightness decrease to detect x1
  int     x1_band;    // distance band above/right of x1
  int     x2_perc;    // brightness decrease to detect x2
  int     x2_band;    // distance band above/right of x2
  int     x3_perc;    // brightness decrease to detect x3
  int     x3_band;    // distance band above/right of x3

  int     min_width;  // minimum width
  int     min_height; // minimum height
  int     min_dim;    // minimum dimension of blob
  int     min_elong;  // minimum elongation (width/height - height/width on screen)
  int     min_avgdf;  // minimum average brightness
  #endif

#ifndef NEWCOSMETIC_COMBO
  #ifdef SIDEWALL_PFS
  int     x1_perc;    // brightness decrease to detect x1
  int     x1_band;    // distance band above/right of x1
  int     x2_perc;    // brightness decrease to detect x2
  int     x2_band;    // distance band above/right of x2
  //int     x3_perc;    // brightness decrease to detect x3
  //int     x3_band;    // distance band above/right of x3

  int     bott_err;     // error on bottom profile
  int     bott_maxerr;  // maximum allowed error on bottom profile

  int     maxbt_thk;  // maximum bottom profile thickness

  int     min_absl;   // minimum allowed absolute brightness (area into liquid)
  int     min_absb;   // minimum allowed absolute brightness (bottom profile)

  int     min_width;  // minimum width
  int     min_height; // minimum height
  int     min_dim;    // minimum dimension of blob
  int     min_elong;  // minimum elongation (width/height - height/width on screen)
  int     min_avgdf;  // minimum average brightness
  #endif
#endif


  #ifdef NEWCOSMETIC_V30

  int     x1_perc;    // brightness decrease to detect x1
  int     x1_band;    // distance band above/right of x1
  int     x2_perc;    // brightness decrease to detect x2
  int     x2_band;    // distance band above/right of x2
  //int     x3_perc;    // brightness decrease to detect x3
  //int     x3_band;    // distance band above/right of x3

  int     bott_err;     // error on bottom profile
  int     bott_maxerr;  // maximum allowed error on bottom profile

  int     maxbt_thk;  // maximum bottom profile thickness

  int     min_absl;   // minimum allowed absolute brightness (area into liquid)

  //int     max_absl;   // minimum allowed absolute brightness (area into liquid)

  int     min_absb;   // minimum allowed absolute brightness (bottom profile)

  int     min_width;  // minimum width
  int     min_height; // minimum height
  int     min_dim;    // minimum dimension of blob
  int     min_elong;  // minimum elongation (width/height - height/width on screen)
  int     min_avgdf;  // minimum average brightness
  #endif

  #ifdef NEWCOSMETIC_COMBO

    int     x1_perc;    // lum perc to detect beginning of bottom
    int     x2_perc;    // lum perc to detect beginning of meniscus
    int     x3_perc;    // lum perc to detect top end of sidewall

    int     x1_band;    // allowance width above bottom to skip false rejects
    int     x2_band;    // allowance width below meniscus to skip false rejects
    int     x3_band;    // allowance width above meniscus to skip false rejects
    int     x4_band;    // allowance width below top end of sidewall to skip false reject

  //  int     bott_err;     // error on bottom profile
  //  int     bott_maxerr;  // maximum allowed error on bottom profile
  //  int     maxbt_thk;  // maximum bottom profile thickness

    int     min_absl;   // minimum allowed absolute brightness (lower area, into liquid)


    int     min_abslq;   // minimum allowed absolute brightness (liquid)
    int     min_abssw;   // minimum allowed absolute brightness (sidewall)

    int     min_width;  // minimum width
    int     min_height; // minimum height
    int     min_dim;    // minimum dimension of blob
    int     min_elong;  // minimum elongation (width/height - height/width on screen)
    int     max_elong;  // maximum elongation (width/height - height/width on screen)
    int     min_avgdf;  // minimum average brightness

    int     min_cakevol; // min allowed cake volume
    int     max_cakevol; // max allowed cake volume

  #endif // NEWCOSMETIC_COMBO


  int     cake_vol;   // calculated cake volume 


  UINT    cosm_err_upper; //Pour added for upper cosmetic
  UINT    max_err_upper;    // maximum allowed particle error

  UINT    max_err;    // maximum allowed particle error 
  UINT    cosm_err;   // calculated total error
  UINT    num_blob;

  int     dim, dimx, dimy;

  //UINT    level_th;
  int     level_th;
  int     level_high;
  int     level_low;

  UINT    level;      // level found
  UINT    level_min;  // minimum allowed thickness
  UINT    level_max;  // maximum allowed thickness

  //Pour added 20180927

#ifdef REJECTION_SEPARATION
  UINT    level_min2;  // minimum allowed thickness
  UINT    level_max2;  // maximum allowed thickness
#endif

  //#ifdef UPPER_COSMETIC
  UINT    offsets_top;     // offset distance to top line
  UINT    offsets_bottom;  // offset distance to bottom line
//#endif

  UINT    area_color;

//  UINT*   avglum;     // pointer to array of average brightness
  int*    avglum;     // pointer to array of average brightness

  // parameters related to NEWCAP_V21 (autoalignment)
//  UINT    minbrgt_sw;   // minimum average brightness required to find TOP (right) x of inspection area for sidewall
  int     minbrgt_sw;   // minimum average brightness required to find TOP (right) x of inspection area for sidewall
  int     x2ofs_sw;     // offset for sidewall analysis area (>0 move to top/right, <0 move to bottom/left)
  int     x1ofs_sw;     // offset/height of whole sidewall inspection area 
  int     x1_sw, x2_sw; // calculated x coords for sidewall analysys

  //int     x2_sw_top, x2_sw_bottom; // calculated x coords for sidewall analysys

  struct blob_score_type
  {
    int  size;     // min size of blob to get error score (must be bigger/equal than)
    int  score;    // score added if blob bigger than size
  };

//  #define SDCK_AMP  10  // radius of circle used for side cake inspection
 // #define SDCK_AMP  radius  // radius of circle used for side cake inspection
  int*    circ_ofs;    // array of offsets to use during side cake analysis
  int     circ_npt;    // number of points of circle to use during side cake analysis
  int     circ_rad;    // current radius of circle structure

  BYTE*   extimg;      // buffer to extended image for processing
  int     extdim;      // total dimension of extended image
  int     extdimx;     // horizontal dimension of extended image
  int     extdimy;     // vertical dimension of extended image

                                         // SIZE - SCORE
  blob_score_type blob_score[NCRIT_BLOB];//  = { {   1 ,   1 } ,
                                            //   {   6 ,   6 } ,  // 6,4
                                            //   {  10 ,  10 } ,
                                            //   {  30 ,  40 } }; // 30,50



};

