

#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"




//  CArea_adjust* areainsp; // inspection/search area


//#define TIP_INSPECTION    64
//#define CAP_INSPECTION   128
//#define CAPMODELSIZE     250

//#define NCRIT_BLOB 4

//#undef   PT
#define  PT2(x,y) img[x+(y)*camera[index].width]

#define SEPARATEAREAS // separate inner and outer areas of inspection



class COralFO_inspection
{

//	DECLARE_DYNAMIC(COralFO_inspection)

public:
  COralFO_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int mode_in=TIP_INSPECTION );
  ~COralFO_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image    ( BYTE* img_acq, UINT nimg );

  int FO_analysis( void );
  int FO_analysis_circle( void );
  int FO_analysislenght_circle( void );

  int FO_contour( void );

  UINT sectbrgth( int xc, int yc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius

  static int Set_inspection_area( int ind );


  // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text


//  int radius; // radius used for alignment
//  int xcnt;   // calculated x of center
//  int ycnt;   // calculated y of center
  int xcnt;   // calculated x of center
  int ycnt;   // calculated y of center

  // sectbrgth scores in current and other four directions
  int rv[5]; // 0=center - 1=right - 2=bottom - 3=left - 4=top
  int bv[5];
  int lv[5];
  int tv[5];
  double score[5]; // overall rating for each position

  int Search_center( void );
  int Calculate_scores( int x, int y );
//  int Calculate_score( int ind );

  int Search_alignment( void );


  CFile_mngmt*  file_manager;

  CArea_adjust* area;   // inspection/search area

  BYTE*   img;          // pointer to image to process

  int     nhead;        // used for data collection

  int     index;        // index of camera/image to be bound to
//  int mode;           // TIP_INSPECTION for AMPOULES - CAP_INSPECTION for VIALS

  // inspection parameters

  int     avg_max;      // maximum allowed average brightness
  int     avg_min;      // minimum allowed average brightness

  int     pos_dif;      // positive threshold brightness difference (calculated on average circle)
  int     neg_dif;      // negative threshold brightness difference (calculated on average circle)
  int     max_lum;      // maximum absolute brightness 0-255
  int     min_lum;      // minimum absolute brightness 0-255

  int     maxerr_pos;   // maximum allowed error for positive points (relative to average)
  int     maxerr_neg;   // maximum allowed error for negative points (relative to average)
  int     maxerr_max;   // maximum allowed error for absolute maximum brightness
  int     maxerr_min;   // maximum allowed error for absolute minimum brightness

  int     err_pos;      // negative difference accumulated error
  int     err_neg;      // positive difference accumulated error
  int     err_max;      // over maximum total accumulated error
  int     err_min;      // below minimum total accumulated error

  int     src_radius;   // radius used for alignment search (parameter from GUI)
  int     anl_radius;   // radius used for analysis

  int     src_radtmp;   // temporary used during seach process
  int     anl_radtmp;   // temporary used during analysis process


  #ifdef SEPARATEAREAS // separate inner and outer areas of inspection

//  int     iavg_max;      // maximum allowed average brightness
//  int     iavg_min;      // minimum allowed average brightness

  int     ipos_dif;      // positive threshold brightness difference (calculated on average circle)
  int     ineg_dif;      // negative threshold brightness difference (calculated on average circle)
  int     imax_lum;      // maximum absolute brightness 0-255
  int     imin_lum;      // minimum absolute brightness 0-255

  int     imaxerr_pos;   // maximum allowed error for positive points (relative to average)
  int     imaxerr_neg;   // maximum allowed error for negative points (relative to average)
  int     imaxerr_max;   // maximum allowed error for absolute maximum brightness
  int     imaxerr_min;   // maximum allowed error for absolute minimum brightness

  int     ierr_pos;      // negative difference accumulated error
  int     ierr_neg;      // positive difference accumulated error
  int     ierr_max;      // over maximum total accumulated error
  int     ierr_min;      // below minimum total accumulated error

  int     ianl_radius;   // radius used for analysis
  int     ianl_radtmp;   // temporary used during analysis process

  #endif SEPARATEAREAS // separate inner and outer areas of inspection






  // Parameters related to contour search/analysis
  BYTE    contour_thr;  // contour threshold
  int     max_dist;     // minimum sq distance from ideal circle to be considered error
  int     max_errprof;  // maximum allowed profile error



/*
  #ifdef DOUBLE_BOTTOM_RING // If external ring analysis required
    int     min2_radius;   // start radius for second analysis
    int     max2_radius;   // end   radius for second analysis
    int     avg2_max;      // maximum allowed average brightness
    int     avg2_min;      // minimum allowed average brightness
    int     pos2_dif;      // positive threshold brightness difference (calculated on average circle)
    int     neg2_dif;      // negative threshold brightness difference (calculated on average circle)
    int     max2_lum;      // maximum absolute brightness 0-255
    int     min2_lum;      // minimum absolute brightness 0-255
    int     maxerr2_pos;   // maximum allowed error for positive points (relative to average)
    int     maxerr2_neg;   // maximum allowed error for negative points (relative to average)
    int     maxerr2_max;   // maximum allowed error for absolute maximum brightness
    int     maxerr2_min;   // maximum allowed error for absolute minimum brightness
    int     err2_pos;      // negative difference accumulated error
    int     err2_neg;      // positive difference accumulated error
    int     err2_max;      // over maximum total accumulated error
    int     err2_min;      // below minimum total accumulated error

    int     min2_radtmp;   // temporary used during analysis process
    int     max2_radtmp;   // temporary used during analysis process
  #endif
  */

  UINT dim, dimx, dimy;
  UINT area_color;

//  #define    

  int        ptn;            // number of points found into CAP contour 
  int        ptx[NPTCNT];    // CAP's contour x coordinates
  int        pty[NPTCNT];    // CAP's contour y coordinates


};

