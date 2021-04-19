

#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>

using namespace Pylon;

//#undef   PT
#define  PT2(x,y) img[x+(y)*camera[index].width]


class CLQcolor_inspection
{

//	DECLARE_DYNAMIC(CLQcolor_inspection)

public:
  CLQcolor_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int mode_in=TIP_INSPECTION );
  ~CLQcolor_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image( BYTE* img_acq, UINT nimg );
  int Process_image( CGrabResultPtr ptr_gr, UINT nimg );
  //int Analysis( BYTE* img );

  //int Calculate_blob( void );
  int Calculate_average( void );

  double funz_t( double in );

  static int Set_inspection_area( int ind );

//  UINT sectbrgth( int xc, int yc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius
//  int  Search_center( void );
//  int  Analyze_center( void );

  //int  Calculate_scores( int x, int y );

//  BOOL   learning_fo;  // TRUE to learn color (separated from particle learning)

/*
  // sectbrgth scores in current and other four directions
  int    rv[3]; // 0=center - 1=right - 2=bottom - 3=left - 4=top
  int    bv[3];
  int    lv[3];
  int    tv[3];
//  double score[3]; // overall rating for each position
  int    srcrad; // temporary radius used during inspection
*/

  // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text
  int     index; // index of camera/image to be bound to
  int     dim, dimx, dimy;
  UINT    area_color;

  CFile_mngmt*  file_manager;
  CArea_adjust* area;   // inspection/search area

//  int radius; // radius used for alignment
//  int     xcnt;   // calculated x of center
//  int     ycnt;   // calculated y of center

//  BYTE    ref_col[3]; // reference color values R G B 
//  double  Lab_ref[3]; // reference color (Lab coordinates)
//  double  dist_lab;   // Lab distance between reference and current color
  double  L, a, b;

  int redness;
  int max_redness;



//  BYTE*   path;  // buffer used for stopping search first,
  BYTE*   img;          // pointer to image to process

  // accumulators for calculating avergae colors
  UINT    npt;
  UINT    totr;
  UINT    totg;
  UINT    totb;

/*
  int     err_col;       

  // inspection parameters
  int     col_diff;       // max allowed color difference
  int     src_radius;     // radius used for alignment search (parameter from GUI)
  int     src_error;      // maximum allowed alignment error

  int     anl_radius;     // radius used for analysis
  int     src_radtmp;     // temporary used during seach process
  int     maxerr_lab;     // maximum allowed average color allowed error
  int     maxerr_col;     // maximum allowed color error (spots/flaws)
*/
//  int     err_dark;       // dark spots accumulated error
//  int     err_bright;     // bright spots accumulated error

};
