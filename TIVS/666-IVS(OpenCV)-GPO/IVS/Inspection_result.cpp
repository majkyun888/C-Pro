#include "StdAfx.h"
#include "Inspection_result.h"


CInspection_result::CInspection_result(void)
{

  objtype = 0x3CD09AB2; // object type - default is 0x3CD09AB2 - 1020304050
  size    = 0;          // size on disk of file/structure - to be calculated later


  /*
  UINT  type;   // type of image
  UINT  dimx;   // horizontal size
  UINT  dimy;   // vertical size
  BYTE* bitmap; // pointer to bitmap 
  */

}


CInspection_result::~CInspection_result(void)
{
}
