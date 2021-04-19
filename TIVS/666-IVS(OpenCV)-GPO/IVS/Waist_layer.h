

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

/*
#define FILLINGL_MODE_REFLECTION    64
#define FILLINGL_MODE_ADSORPTION   128
#define FILLINGL_MODE_SELECTABLE   256
*/

//#define NCRIT_BLOB 4



class CWaist_layer
{

//	DECLARE_DYNAMIC(CWaist_layer)

public:
  CWaist_layer( UINT ind, CFile_mngmt* file_manager_in=NULL );
  ~CWaist_layer(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image( BYTE* img_acq, int nimg );
  //int Analysis( BYTE* img );
  //void Draw_level( int x, UINT col );

  int Search_bottom ( BYTE* img_acq );
  int Analyize_sides( BYTE* img_acq );
  int Analyize_layer( BYTE* img_acq );

  int Final_result( void );

  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );


  static int Set_inspection_area( int ind );

  CString       txt; // text to be displayed as inspection result
  UINT          col; // color to be used for text displaying

  CFile_mngmt*  file_manager;

  CArea_adjust* area; // inspection/search area

  int  index;         // index of camera/image to be bound to
//  int  mode;          // FILLINGL_MODE_REFLECTION / FILLINGL_MODE_ADSORPTION

  int  numimg;

  int  imgshw;  // = 1;    // image to show during processing (0=show all)

/*  int  hliv;          // computed average filling level (0=not found)
  int  hliv_tot;      // accumulator to calculate average level
  int  nliv_tot;      // number of found levels
  int  ninsp;  // number of inspections
  int  nrej;   // number of rejections  */
//  UINT npt;    // number of points found


  // inspection parameters for waist/layer

  // find bottom of vials
  int  bott_thr;      // percentage of brightness drop to detect bottom of vials

  UINT num_img;       // number of images for each inspection cycle

  UINT num_aligned;   // number of images properly aligned on bottom

  BOOL draw_ovl;      // true only during first image for drawing overlay
  BOOL good;          // set to true at beginning, can be set to false during processing

  // waist analysis parameters
  int  waist_lum;     // threshold brightness for detection of sides of cake
  int  waist_hmax;    // max/high height for waist inspection 
  int  waist_hmin;    // min/low  height for waist inspection 
  int  waist_wmax;    // max width for waist inspection 
  int  waist_wmin;    // min width height for waist inspection 
  int  waist_err;     // waist shrink total error (num of images WRONG)
  int  waist_maxerr;  // maximum allowed waist shrink total error (0 means control disabled)
  int  waist_minhgt;  // minimum continuous height of shrunk waist (continuous width under wmin)
  int  waist_minnimg; // minimum number of bad images/inspections to cause reject (default 1)
//  int  err_waist;     // waist shrink total error
//  int  maxerr_waist;  // maximum allowed waist shrink total error

  // layer analysis parameters
  int  layer_hmax;    // max/high height for layer inspection 
  int  layer_hmin;    // min/low  height for layer inspection 
  int  layer_width;   // width of area for layer inspection (search of sides)
  int  layer_minlum;  // min brightness to consider layer pixels
  int  layer_err;     // layer shrink total error
  int  layer_maxerr;  // maximum allowed layer total error
//  int  err_layer;     // layer shrink total error
//  int  maxerr_layer;  // maximum allowed layer total error

  int  xbase;         // x coord of vial's base calculated
  int  ycent;         // y coord of vial's cake center calculated


/*  UINT min_npt;       // minimum number of points to validate found level
  int  hliv_max;      // maximum acceptable height
  int  hliv_min;      // minimum acceptable height
  int  hliv_prc;      // brightness percentage threshold to find bottom (0=disabled)  */


  int  dim, dimx, dimy;

  UINT area_color;

  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text

  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text


};


