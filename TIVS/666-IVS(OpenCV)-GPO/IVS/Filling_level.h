

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

//  CArea_adjust* areainsp; // inspection/search area


#define FILLINGL_MODE_REFLECTION    64
#define FILLINGL_MODE_ADSORPTION   128
#define FILLINGL_MODE_SELECTABLE   256
#define FILLINGL_MODE_HORIZONTAL   512
#define HORIZONTAL                1024
//#define NCRIT_BLOB 4



class CFilling_level
{

//	DECLARE_DYNAMIC(CFilling_level)

public:
  CFilling_level( UINT ind, CFile_mngmt* file_manager_in=NULL );
  ~CFilling_level(void);

  int Add_menu_parameters( CContainer_wnd* child );
  int Add_menu_parameters_f( CContainer_wnd* child );
  int Process_image( BYTE* img_acq, int nimg );
  //Pour added for color detection 20180605
  //int Process_image( CGrabResultPtr ptr_gr, UINT nimg );
  int Analysis( BYTE* img );
  void Draw_level( int x, UINT col );

  static int Set_inspection_area( int ind );


  //int Calculate_blob( BYTE* img_acq );


  CString       txt; // text to be displayed as inspection result
  UINT          col; // color to be used for text displaying

  CFile_mngmt*  file_manager;

  CArea_adjust* area; // inspection/search area

  int  index;         // index of camera/image to be bound to
  int  mode;          // FILLINGL_MODE_REFLECTION / FILLINGL_MODE_ADSORPTION

  int  numimg;
  int  hliv;          // computed average filling level (0=not found)
  int  hliv_tot;      // accumulator to calculate average level
  int  nliv_tot;      // number of found levels

  int  ninsp;  // number of inspections
  int  nrej;   // number of rejections

  UINT npt;    // number of points found


  // inspection parameters
  int  lum_thr;       // threshold brightness
  UINT min_npt;       // minimum number of points to validate found level
  int  hliv_max;      // maximum acceptable height
  int  hliv_min;      // minimum acceptable height

  int  hliv_prc;      // brightness percentage threshold to find bottom (0=disabled)


    #ifdef OPACITY_CTRL

//    UINT histo_inn[256];
//    UINT histo_out[256];

    UINT npt_inn;
    UINT lum_inn;
    UINT npt_out;
    UINT lum_out;

    int opc_step; // step for x/y cycles

    int opc_vofs; // distance of outer area compared to lower
    int opc_vdim; // vertical size of opacity area
 //   int opc_hdim; // horizontal size of opacity area

    int xmino, ymino, xmaxo, ymaxo; // outside window coords

    int opc_mindif; // minimum allowed difference
    int opc_maxdif; // maximum allowed difference


  #endif // OPACITY_CTRL

  BOOL good;

  int  dim, dimx, dimy;

  UINT area_color;
  BYTE* img_gray;

};

