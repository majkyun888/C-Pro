#include "StdAfx.h"
#include "BufferFactory_new.h"


CBufferFactory_new::CBufferFactory_new(void)
{
  TRACE("CBufferFactory_new: constructor called\n");

  nbuff = 0;
  ZeroMemory( buffer,  sizeof(buffer ) );
  ZeroMemory( bufsize, sizeof(bufsize) );

}


CBufferFactory_new::~CBufferFactory_new(void)
{
  TRACE("CBufferFactory_new: destructor called\n");
}


void CBufferFactory_new::AllocateBuffer( size_t bufferSize, void** pCreatedBuffer, intptr_t& bufferContext )
{
  //TRACE("CBufferFactory_new: AllocateBuffer called - size=%d - context(in)=%d --- ", bufferSize, bufferContext );

  if ( nbuff>=BF_MAXMUN ) // no more space to allocate!
  {
    *pCreatedBuffer = NULL;
    bufferContext   = 1000;
    ASSERT(0);
  }

  buffer[nbuff] = (BYTE*) _aligned_malloc( bufferSize, 16 ); 
  ASSERT( buffer[nbuff] && AfxIsValidAddress( buffer[nbuff], bufferSize ) );
  ZeroMemory( buffer[nbuff], bufferSize );
  bufsize[nbuff] = bufferSize;

  *pCreatedBuffer = buffer[nbuff];
  bufferContext   = nbuff;

  // check overlapping
  for ( UINT i=0 ; i<nbuff ; i++ )
  {
    if ( UINT(buffer[nbuff]               )<=UINT(buffer[i]+bufsize[i]) && 
         UINT(buffer[nbuff]+bufsize[nbuff])>=UINT(buffer[i]           )  )
    {
      TRACE("Overlapping buffer!!!! %d - %d \n", nbuff, i );
    }

  }

  //TRACE(" buffer=%08X - context(out)=%d\n", buffer[nbuff], nbuff );

  nbuff++;
//  if ( nbuff>=BF_MAXMUN ) nbuff=0; //rollover
}


void CBufferFactory_new::FreeBuffer(void *pCreatedBuffer, intptr_t bufferContext )
{
  //TRACE("CBufferFactory_new: FreeBuffer called - pCreatedBuffer=%08X - bufferContext=%d --- ", pCreatedBuffer, bufferContext );

 // if ( bufferContext>=0 && bufferContext<(int)nbuff )
  {
    if ( (BYTE*)pCreatedBuffer == buffer[bufferContext] )
    {
      _aligned_free( buffer[bufferContext] );
      buffer[bufferContext] = NULL;
      nbuff--;
    }
    else TRACE("\nMismatch pCreatedBuffer/buffer[bufferContext]!\n");
  }
//  else TRACE("\nbufferContext out of range 0/nbuff\n");

  //TRACE("cleared and freed - nbuff=%d\n", nbuff );

}


void CBufferFactory_new::DestroyBufferFactory( void )
{
  TRACE("CBufferFactory_new: DestroyBufferFactory called\n");
}
