#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"
#include "CAlgo1.h"

#define LOCAL_TEST
#ifdef _DEBUG
#ifdef LOCAL_TEST
#pragma comment(lib,"./Debug/FlangeProc.lib")
#else
#ifdef COMPILE_LOCAL
#pragma comment(lib, "./Debug/FlangeProc.lib")
#else
//#pragma comment(lib,"\\\\PC1\\NetPC1\\FlangeProc.lib")
#pragma comment(lib,".\\FlangeProc.lib")
#endif // COMPILE_LOCAL
#endif
#else // Release
#ifdef LOCAL_TEST
#pragma comment(lib, "./Release/FlangeProc.lib")
#else
#ifdef COMPILE_LOCAL
//#pragma comment(lib, "\\\\PC1\\NetPC1\\FlangeProc.lib")
#pragma comment(lib, "/Release/FlangeProc.lib")
#else
//#pragma comment(lib,"\\\\PC1\\NetPC1\\FlangeProcs.lib")
#pragma comment(lib,".\\FlangeProc.lib")
#endif // COMPILE_LOCAL
#endif
#endif
//#include "../FlangeProc/FlangeProc.h"

class CFlange_inspection
{

//	DECLARE_DYNAMIC(CTip_inspection)

public:

  static int Set_inspection_area( int ind );

  CFlange_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL );
  ~CFlange_inspection(void);

  int        index;          // index of camera/image to be bound to
  int        mode;           // TIP_INSPECTION for AMPOULES - CAP_INSPECTION for VIALS
	CFile_mngmt*  file_manager;
	CArea_adjust* area; // inspection/search area
  BYTE*  img;          // pointer to image to process
  int    dim, dimx, dimy;

  int  Add_menu_parameters( CContainer_wnd* child );
 
   // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  CString txt3;  // third row of result's text (crimping result)
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text
  UINT    col3;  // color of third row of text (crimping result)
  
  int  Process_image( cv::Mat img_in, UINT nimg=0 );  
  

private:
	Para_Flange input;
	Result_Flange output;


};