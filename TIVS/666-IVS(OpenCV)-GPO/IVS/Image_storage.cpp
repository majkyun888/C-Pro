#include "StdAfx.h"
#include "Image_storage.h"



extern UINT             num_tlc;

extern CImage_display*  image_dsp[NUM_TLC];
extern Camera_struct    camera  [NUM_TLC];

extern BOOL             saveinsp_on;
extern BOOL             saveseq_on;


CImage_storage::CImage_storage( UINT nimg_in, UINT ind_in )
{
  ASSERT( nimg_in>1 && nimg_in<200 );
  ASSERT( ind_in>=0 && ind_in < 21 ); // && ind_in<num_tlc );

  buffs = NULL;
  nbuff = 0;

  ind   = ind_in;
  nbuff = nimg_in;
  dimb  = image_dsp[ind]->buf_size;

  buffs = (BYTE**) _aligned_malloc( sizeof(BYTE*)*nbuff, 16 );
  ASSERT( buffs && AfxIsValidAddress(buffs,sizeof(BYTE*)*nbuff) );

  //TRACE("CImage_storage: allocated buffs (%d images / %d bytes)\n", nbuff, sizeof(BYTE*)*nbuff );
  for ( i=0 ; i<nbuff ; i++ )
  {
    buffs[i] = (BYTE*) _aligned_malloc( dimb , 16 ); 
    ASSERT( buffs[i] && AfxIsValidAddress(buffs[i],dimb) );
    //TRACE("CImage_storage: allocated buffs[%d] (%d bytes)\n", i, dimb );
  }

  tmstmp = (uint64_t*) malloc(nbuff*sizeof(uint64_t));
  ASSERT( tmstmp && AfxIsValidAddress(tmstmp,nbuff*sizeof(uint64_t)) );

  frmnmb = (uint32_t*) malloc(nbuff*sizeof(uint32_t));
  ASSERT( frmnmb && AfxIsValidAddress(frmnmb,nbuff*sizeof(uint32_t)) );
}


CImage_storage::~CImage_storage(void)
{
  if ( nbuff>0 && buffs )
  {
    for ( i=0 ; i<nbuff ; i++ )
    {
      _aligned_free( buffs[i] );
      //TRACE("CImage_storage: deallocated buffs[%d]\n", i );
    }

    _aligned_free( buffs );
    //TRACE("CImage_storage: deallocated buffs\n" );
  }

  if ( tmstmp ) 
  {
    free( tmstmp );
    tmstmp = NULL;
  }

  if ( frmnmb ) 
  {
    free( frmnmb );
    frmnmb = NULL;
  }

}



int CImage_storage::add_image( CGrabResultPtr grab_ptr, UINT ind )
{
  if ( ind<0 || ind>=nbuff ) // wrong index!
  {
    ASSERT( 0 );
    return 99; 
  }

  //i = sizeof(CGrabResultPtr); // 8 bytes
  
  CopyMemory( buffs[ind], grab_ptr->GetBuffer(), dimb );
/*  frames[nimg_acq].buffer    = img_acq;
    frames[nimg_acq].timestamp = ptrGrabResult->GetTimeStamp();
    frames[nimg_acq].framenum  = ptrGrabResult->GetFrameNumber();  */

  tmstmp[ind] = grab_ptr->GetTimeStamp();
  frmnmb[ind] = grab_ptr->GetFrameNumber();

  //TRACE("part[%d] - framen=%5d - timest=%I64u\n",ind,ptrGrabResult->GetFrameNumber(),ptrGrabResult->GetTimeStamp());


  //TRACE("Adding new frame %d\n", ind );


  return 0; // completed successfully
}


int CImage_storage::save_sequence( void )
{
  wchar_t name[200+1];
  wchar_t dir [200+1];
  CString original;

  CFileException ex;
  CTime time = CTime::GetCurrentTime();   


  swprintf( dir, 200, L"%s\\Sequence[%s]_%04d%02d%02d_%02d%02d%02d", IMAGES_DIRECTORY, camera[ind].name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  CreateDirectory( dir, NULL );

  swprintf( name, 200, L"%s\\screenshot.png", dir );
//  Capture_screenshot( name );
  Capture_screenshot( name, ind );

  camera[ind].text[3]->GetWindowTextW( original );
  camera[ind].text[3]->SetWindowTextW( L"Saving images.." );


  if ( saveseq_on )
  {
    TRACE( L"save_sequence: directory <%s> created\n", dir );
    for ( i=0 ; i<nbuff ; i++ )
    {
      swprintf( name, 200, L"%s\\img_%04d", dir, i );
      Save_buffer( buffs[i], image_dsp[ind]->dim_buf.cx, image_dsp[ind]->dim_buf.cy, name, PNG_FILE );

      //TRACE( L"save_sequence: buffer[%2d] saved as <%s>\n", i, name );
      //txt.Format( L"Saving image n %d", i );
      //camera[ind].text[2]->SetWindowTextW( txt );
    }

    // Write CSV file
    CFile fp;
    swprintf( name, 200, L"%s\\sequence_data.csv", dir );
    fp.Open( name, CFile::modeCreate|CFile::modeWrite );

    swprintf( name, 200, L"Index,Framenumber,Timestamp\r\n" );
    fp.Write( name, wcslen(name)*sizeof(wchar_t) );
    for ( i=0 ; i<nbuff ; i++ )
    {
      swprintf( name, 200, L"%d,%d,%I64u,%.1lf\r\n", i, frmnmb[i], tmstmp[i], ACE_CFMS*(tmstmp[i]-tmstmp[0]) );
      fp.Write( name, wcslen(name)*sizeof(wchar_t) );
    }

    fp.Close();
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    swprintf( name, 200, L"%s\\processed", dir );
    image_dsp[ind]->Save_fulldsp( name );
  #endif


  //SetCurrentDirectory( MAIN_DIRECTORY );
  TRACE( L"save_sequence: completed\n", i, name );
  camera[ind].text[3]->SetWindowTextW( original );

  return 0;
}


