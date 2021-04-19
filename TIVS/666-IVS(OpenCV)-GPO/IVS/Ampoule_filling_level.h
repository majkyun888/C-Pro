

#pragma once

#include "stdafx.h"
#include "Area_adjust.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
//#include "File_mngmt.h"

//  CArea_adjust* areainsp; // inspection/search area


class CArea_adjust;

class CAmpoule_filling_level
{

//	DECLARE_DYNAMIC(CAmpoule_filling_level)

public:
  CAmpoule_filling_level( UINT ind, CFile_mngmt* file_manager_in=NULL );
  ~CAmpoule_filling_level(void);

  int Add_menu_parameters( CContainer_wnd* child );
  int Process_image( BYTE* img_acq );
  static int Set_inspection_area( int ind );


  CFile_mngmt*   file_manager;

  CArea_adjust*  area; // inspection/search area

//  CArea_adjust* areainsp; // inspection/search area

//  CArea_adjust* areainsp; // inspection/search area
  int index;          // index of camera/image to be bound to

  // Inspection parameters
  int    necksearch_x;
  int    necksearch_th;
  int    lum_min;
  int    min_npt;
  double diam;

  double mm2pix;  // mm to pixel conversion coefficient
  double pix2mm;  // pixel to mm conversion coefficient



//#include "Area_adjust.h"




};

