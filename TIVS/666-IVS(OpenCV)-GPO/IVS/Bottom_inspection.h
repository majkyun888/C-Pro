

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


class CBottom_inspection
{

//	DECLARE_DYNAMIC(CBottom_inspection)

public:
  CBottom_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int mode_in=TIP_INSPECTION );
  ~CBottom_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image    ( BYTE* img_acq, UINT nimg );

  int Bottom_analysis( void );
  int Bottom_analysis_circle( void );
  int Bottom_double_analysis( void );

  UINT sectbrgth( int xc, int yc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius
  UINT sectbrgth( int xc, int yc, int rc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius

  static int Set_inspection_area( int ind );


  // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text

//  int radius; // radius used for alignment
  int xcnt;   // calculated x of center
  int ycnt;   // calculated y of center

  // parameters/functions involved into search of second (inner cake) center
  int Search_center2( void );
  int xcnt2;   // calculated x of second (inner cake) center
  int ycnt2;   // calculated y of second (inner cake) center
  int src_radius2;   // radius used for alignment search (parameter from GUI)
  int src_radtmp2;   // temporary used during seach process


  // sectbrgth scores in current and other four directions
  int rv[5]; // 0=center - 1=right - 2=bottom - 3=left - 4=top
  int bv[5];
  int lv[5];
  int tv[5];
  double score[5]; // overall rating for each position

  int Search_center( void );
  int Calculate_scores( int x, int y );
  int Calculate_scores( int x, int y, int r );

  
  //  int Calculate_score( int ind );



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

    //Pour added 20180925

#ifdef REJECTION_SEPARATION

  int     avg_max2;      // maximum allowed average brightness
  int     avg_min2;      // minimum allowed average brightness

  int     pos_dif2;      // positive threshold brightness difference (calculated on average circle)
  int     neg_dif2;      // negative threshold brightness difference (calculated on average circle)
  int     max_lum2;      // maximum absolute brightness 0-255
  int     min_lum2;      // minimum absolute brightness 0-255

  int     maxerr_pos2;   // maximum allowed error for positive points (relative to average)
  int     maxerr_neg2;   // maximum allowed error for negative points (relative to average)
  int     maxerr_max2;   // maximum allowed error for absolute maximum brightness
  int     maxerr_min2;   // maximum allowed error for absolute minimum brightness

  int     err_pos2;      // negative difference accumulated error
  int     err_neg2;      // positive difference accumulated error
  int     err_max2;      // over maximum total accumulated error
  int     err_min2;      // below minimum total accumulated error
  
//Pour 20180925
  int     avg2_max2;      // maximum allowed average brightness
  int     avg2_min2;      // minimum allowed average brightness

  int     pos2_dif2;      // positive threshold brightness difference (calculated on average circle)
  int     neg2_dif2;      // negative threshold brightness difference (calculated on average circle)
  int     max2_lum2;      // maximum absolute brightness 0-255
  int     min2_lum2;      // minimum absolute brightness 0-255

  int     maxerr2_pos2;   // maximum allowed error for positive points (relative to average)
  int     maxerr2_neg2;   // maximum allowed error for negative points (relative to average)
  int     maxerr2_max2;   // maximum allowed error for absolute maximum brightness
  int     maxerr2_min2;   // maximum allowed error for absolute minimum brightness

  int     err2_pos2;      // negative difference accumulated error
  int     err2_neg2;      // positive difference accumulated error
  int     err2_max2;      // over maximum total accumulated error
  int     err2_min2;      // below minimum total accumulated error

#endif // REJECTION_SEPARATION

//Pour added 20180925

  #if defined DOUBLE_BOTTOM_RING || defined DOUBLE_BOTTOM_ANALYSIS // If external ring analysis required
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





  
  
//  int min_npt;        // minimum number of points to validate found profile
//  int htot_max;       // maximum acceptable height
//  int htot_min;       // minimum acceptable height
//  int maxerr_symm;   // maximum allowed error for symmetry      ( 0=disabled )
//  int maxerr_curv;   // maximum allowed error for top curvature ( 0=disabled )
//  double scalefct;
//  double tip_diameter;  // nominal diameter of tip top radius
//  int  curv_err;
//  int  bulge_err;

//  int  dim, dimx, dimy;
  UINT dim, dimx, dimy;

  UINT area_color;

};

