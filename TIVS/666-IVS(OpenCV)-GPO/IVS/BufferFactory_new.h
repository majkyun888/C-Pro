#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Camera.h"
//#include "c:\program files\basler\pylon 3.2\pylon\include\pylon\bufferfactory.h"

#define  BF_MAXMUN 350 // max num of buffer the class can manage


class CBufferFactory_new : public IBufferFactory
{
public:
  CBufferFactory_new(void);
  ~CBufferFactory_new(void);

  void AllocateBuffer( size_t  bufferSize,  void **  pCreatedBuffer,  intptr_t &  bufferContext );

//  void FreeBuffer(void *pCreatedBuffer, intptr_t bufferContext);
  void FreeBuffer( void*, int );
 
  void DestroyBufferFactory( void );


  BYTE* buffer[BF_MAXMUN];   // array of pointers to allocated buffers
  UINT  bufsize[BF_MAXMUN];  // array containing sizes of allocated buffers
  UINT  nbuff;               // number of allocated buffers
  UINT  ind;                 // array scan index


};

/*

virtual  ~IBufferFactory ()=0 
  Ensure proper destruction by using a virtual destructor. 
 
virtual void  AllocateBuffer (size_t bufferSize, void **pCreatedBuffer, intptr_t &bufferContext)=0 
  Allocates a buffer and provides additional context information. 
 
virtual void  FreeBuffer (void *pCreatedBuffer, intptr_t bufferContext)=0 
  Frees a previously allocated buffer. 
 
virtual void  DestroyBufferFactory ()=0 
  Destroys the buffer factory. 
 
*/