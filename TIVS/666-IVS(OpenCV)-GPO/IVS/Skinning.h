

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"


/*
struct skinpt_type
{
	int      x1;
	int      y1;
	int      x2;
	int      y2;
};

static int          nptskin = 0;
static skinpt_type* ptskin  = NULL;
static int          semiwdt = 0;    // semiwidth of analysis ellipse
static int*         anlwdt  = NULL; // array to store the width of analysis ellipse accordind to x

*/

class CSkinning
{

//	DECLARE_DYNAMIC(CSkinning)

public:
  CSkinning( UINT ind, CFile_mngmt* file_manager_in=NULL );
  ~CSkinning(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image( BYTE* img_acq, int nimg );
  //int Analysis( BYTE* img );
  //void Draw_level( int x, UINT col );

//  int Search_bottom ( BYTE* img_acq );
//  int Analyize_sides( BYTE* img_acq );
//  int Analyize_layer( BYTE* img_acq );

  int Final_result( void );

  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );
  //int Search_bottom( BYTE* img_acq );

//  int Prepare_ptskin( void );
  int Prepare_alignment( void );
  int Prepare_analysis ( void );

  static int Set_inspection_area( int ind );

  CString       txt; // text to be displayed as inspection result
  UINT          col; // color to be used for text displaying

  CFile_mngmt*  file_manager;

  CArea_adjust* area; // inspection/search area

  int  index;         // index of camera/image to be bound to
//  int  mode;          // FILLINGL_MODE_REFLECTION / FILLINGL_MODE_ADSORPTION

  int  numimg;


  // find bottom of vials
 // int  bott_thr;      // percentage of brightness drop to detect bottom of vials

  UINT num_img;       // number of images for each inspection cycle

  UINT num_aligned;   // number of images properly aligned 

  BOOL draw_ovl;      // true only during first image for drawing overlay
  BOOL good;          // set to true at beginning, can be set to false during processing

  /*
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
  */

/*  UINT min_npt;       // minimum number of points to validate found level
  int  hliv_max;      // maximum acceptable height
  int  hliv_min;      // minimum acceptable height
  int  hliv_prc;      // brightness percentage threshold to find bottom (0=disabled)  */


  int  dim, dimx, dimy;


  struct skinpt_type
  {
	  int      x1;
  	int      y1;
	  int      x2;
  	int      y2;
  };


  int          nptskin; // = 0;
  skinpt_type* ptskin;  // = NULL;
  int          semiwdt; // = 0;    // semiwidth of analysis ellipse
  int*         anlwdt;  // = NULL; // array to store the width of analysis ellipse accordind to x
  int          imgshw;  // = 1;    // image to show during processing  (0=show all)


  // alignment parameters
   
  int align_w;    // width  used for alignment
  int align_h;    // height used for alignment
  int gap_w;      // gap between external / internal
  int num_pt;     // number of points used for aligment
  int score_min;  // min score required for alignment

  // currents for alignment
  int align_wact;    // width  used for alignment
  int align_hact;    // height used for alignment
  int gap_wact;      // gap between external / internal



  int anlys_w;    // width  used for analysis (image coordinates) - W for GUI
  int anlys_h;    // height used for analysis (image coordinates) - H for GUI

  // currents for analysys
  int anlys_wact;
  int anlys_hact;


  int centr_x;    // x coord of computed center
  int centr_y;    // y coord of computed center

  int centr_ofsx; // x coord offset of computed center
  int centr_ofsy; // y coord offset of computed center

  int lum_min;    // minimum allowed brightness inside analysis area
  int err_max;    // maximum allowed error after analysis
  int err_int;    // measured error inside analysis area (single frame)
  int err_tot;    // total error accumulated over all frames


  UINT area_color;

  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text

  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text


};












