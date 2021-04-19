#pragma once


#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>

using namespace Pylon;


class CImage_storage
{
public:
  CImage_storage( UINT dim, UINT ind );
  ~CImage_storage(void);
  int add_image( CGrabResultPtr grab_ptr, UINT counter );
  int save_sequence( void );

  UINT   i;

  uint64_t* tmstmp;
  uint32_t* frmnmb;

  BYTE** buffs; // array to pointers of buffers to hold all images


  UINT   nbuff; // number of allocated buffers
  UINT   ind;   // index of camera/image_dsp
  UINT   dimb;  // size in bytes of each buffer

};

