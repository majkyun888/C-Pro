

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

//  CArea_adjust* areainsp; // inspection/search area

/*
#define NCRIT_BLOB                   4
#define PARTICLE_MODE_REFLECTION   128
#define PARTICLE_MODE_ADSORPTION   256
#define PARTICLE_MODE_BOTH         512  // selectable 
#define PARTICLE_HORIZONTAL       1024  // camera is horizontal
*/

#define  xmin    area->buffrect.left
#define  xmax    area->buffrect.right
#define  ymin    area->buffrect.top
#define  ymax    area->buffrect.bottom

#define  xmin2   area2->buffrect.left
#define  xmax2   area2->buffrect.right
#define  ymin2   area2->buffrect.top
#define  ymax2   area2->buffrect.bottom

#define  PFS_NEEDLESHIELD  128
#define  PFS_LOWSTOPPER    256

#ifndef  NEW_PARTICLE
#define  NIMAGES 30
#endif

#define  PT(x,y) img_acq[x+(y)*camera[index].width]


class CPFSidewall_inspection
{

//	DECLARE_DYNAMIC(CPFSidewall_inspection)

  #define MAXLNGTDESC 15 // Max lenght of text for description of measures
  
  struct measure_type
  {
    //wchar_t** desc;       // name/description
    wchar_t   desc[MAXLNGTDESC+1];   // name/description
    int       val;        // measurement ID
    int       min;        // minimum allowed value
    int       max;        // maximum allowed value
    int       nsmpl;      // number of samples being used
    int       ex;         // sum of all data
    int       ex2;        // sum of all squared data
    double    avg;        // mean
    double    std;        // std deviation
  };

public:
  CPFSidewall_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in=NULL );
  ~CPFSidewall_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );
  static int Set_inspection_area( int ind );

  int Process_image( BYTE* img_acq, UINT nimg );

  int Draw_stopper_updown( int pos, int res );


  // NEEDLE SHIELD
  int Find_contour_needlesh( int y0=0 );
  int Analyze_contour_needlesh( void );

  // STOPPER
  int Find_contour_stopper( int y0=0 );
  int Analyze_contour_stopper( void );

  int Saving_switch( void ); // Start/stop data saving
  int Learning_switch( void ); // Start/stop learning

//  int Measure_check( int val, wchar_t** desc, measure_type* meas );
  int Measure_check( int val, measure_type* meas );
  int Init_measure( wchar_t** desc, measure_type* meas );
  
  int Init_measures( int ind=999 ); // 999 means init all
  
//  int Calc_average( wchar_t** txt, measure_type* meas );
  int Calc_average( measure_type* meas );

  int Calc_distance( int pt1, int pt2 );

  int Draw_boundingbox( int x1, int y1, int x2, int y2, UINT col );

  CString meas_txt;

  CMenu_button* learn_btn;
  int learning_needle; // 0=processing/analysis - 1=learning of model active

  CMenu_button* savedt_btn; // save model data
  int saving_data;     // 0=non saving data - 1=saving data ON

  CFile datafp; // file pointer for saving data

  CFile_mngmt* file_manager;


//  int Save_text( char* txt );
  //int Save_text( wchar_t* txt );
  int Save_text( CString* txt, int addeol=TRUE );
  int Save_text( wchar_t* txt, int addeol=TRUE );

  int index;          // index of camera/image to be bound to
  int mode;          // index of camera/image to be bound to


  // inspection parameters
  UINT      num_img;    // number of images for inspection
  BYTE*     img;          // pointer to image to process

  int       dim, dimx, dimy;
  
  UINT      proctime;

  int       good;         // final result of inspection

  int       draw_ovl;     // draw overlay

  CString   errors_txt1;
  CString   errors_txt2;

  //  NEEDLE SHIELD CONTOUR
  int       ptmid;
  int       ptn;            // number of points found into contour 
  int       ptx[NPTCNT];    // contour x coordinates
  int       pty[NPTCNT];    // contour y coordinates
  int       contour_thr;    // threshold for contour extraction
  int       min_npt;        // minimum number of points to validate found profile
  int       max_npt;        // maximum number of points to validate found profile
  CString   txt1;           // text containing inspection result
  UINT      col1;           // color for text

  CArea_adjust* area; // inspection/search area for needle sheild
  UINT area_color;  // 

  int allowance; // percentage of stdev allowed for min/max 100=+/-stdev
  int err_needle;    // accumulated error for needle shield
  int maxerr_needle; // maximum allowed error for needle shield
  CString desc_needle; // contains description of needle inspection

  // NEEDLE SHIELD POINTS
  #define PA    0   // Upper left/right knee
  #define PB    1   // Lower left/right knee
  #define PC    2
  #define PD    3
  #define PE    4
  #define PF    5

  #define LEFT  0
  #define RIGHT 1

  int    kpts[6][2];    // [ABDCE][LR] keypoints indexes (0..ptn-1)

  // Measures of needle shield
  measure_type ALR_width;    // width  of left-right A
  measure_type ALR_height;   // height of left-right A
  measure_type BLR_width;    // width  of left-right B
  measure_type BLR_height;   // height of left-right B
  //measure_type AB_offsx;     // hor offset/tilt of needlesh

  measure_type CLR_width;    // width PCLR
  measure_type DLR_width;    // width PDLR
  measure_type ELR_width;    // width PELR

  measure_type PAL_PBR;      // diagonal PAL-PBR
  measure_type PAR_PBL;      // diagonal PAR-PBL


  int      maxerr_nedlow;    // maximum error for lower needle corner
  int      err_nedlow;       // measured error for lower needle corner




  //  STOPPER CONTOUR
  //int       ptmid2;
  int       ptn2;            // number of points found into CAP contour 
  int       ptx2[NPTCNT];    // CAP's contour x coordinates
  int       pty2[NPTCNT];    // CAP's contour y coordinates
  int       contour2_thr;    // threshold for contour extraction
  int       min2_npt;        // minimum number of points to validate found profile
  int       max2_npt;        // maximum number of points to validate found profile
  CString   txt2;            // text containing inspection result
  UINT      col2;            // color for text

  // Stopper contour bounding box
  int       stxmin;
  int       stxmax;
  int       stymin;
  int       stymax;


  int       minnptlow;       // minimum num of points low, close to bottom to find orientation


  CArea_adjust* area2; // inspection/search area for stopper
  UINT area2_color;

  int allowance2;   // percentage of stdev allowed for min/max 100=+/-stdev
  int err_stppr;    // accumulated error for stopper
  int maxerr_stppr; // maximum allowed error for stopper

  CString desc_stppr; // contains description of stopper inspection

  // Measures of stopper
  measure_type stppr_width;  // outer width  of stopper 
  measure_type stppr_height; // outer height of stopper 
  measure_type stppr_vpos;   // vertical position of lower part of stopper 

  CString*     mdlresptr;    // pointer to the Cstring to hold model check result


  int Sidewall_PFS( BYTE* img_acq );

  //Pour 20201017
  	xilin_head_para para_crack;
	xilin_head_rlt  result_crack;
	//int detectCrack(const Mat &srcImage, Point pt1, Point pt2);
  //Pour 20201017

  BYTE*   output;     // output buffer/image for blox analysis
  int     cosm_err;   // calculated total error
  UINT    num_blob;
//  UINT    max_err;    // maximum allowed particle error 
  int     maxerr_cosm; // maximum allowed error for sidewall

#ifndef NEWCOSMETIC_COMBO
  #ifdef SIDEWALL_PFS //Pour removed on 20200927
  int     x1_perc;    // brightness decrease to detect x1
  int     x1_band;    // distance band above/right of x1
  int     x2_perc;    // brightness decrease to detect x2
  int     x2_band;    // distance band above/right of x2
  int     min_absl;   // minimum allowed absolute brightness (area into liquid)
  int     min_width;  // minimum width
  int     min_height; // minimum height
  int     min_dim;    // minimum dimension of blob
  int     min_elong;  // minimum elongation (width/height - height/width on screen)
  int     min_avgdf;  // minimum average brightness
  #endif  //Pour removed on 20200927
#endif



};



