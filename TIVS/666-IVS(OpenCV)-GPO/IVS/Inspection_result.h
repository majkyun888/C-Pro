#pragma once
class CInspection_result
{
public:

  UINT objtype; // object type - default is 0x3CD09AB2 - 1020304050
  UINT size;    // size on disk of file/structure


  UINT  type;   // type of image
  UINT  dimx;   // horizontal size
  UINT  dimy;   // vertical size
  BYTE* bitmap; // pointer to bitmap 


  CInspection_result(void);
  ~CInspection_result(void);
};

