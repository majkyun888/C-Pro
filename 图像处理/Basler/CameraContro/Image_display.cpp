// Image_display.cpp : Class to Draw Images
#include "StdAfx.h"
#include "Image_display.h"


extern  UINT *lut32;              // Pointer to LUT (either BW or COLOR)
extern  Camera_struct camera[NUM_TLC];

#define DRW_PT(x,y,c) ptr_dsp[(x)+(y)*dim_dsp.cx] = c;





CImage_display::CImage_display( int ind_in )
{

  ASSERT( ind_in>=0 && ind_in<NUM_TLC );

  UINT x, y;

  ind             = ind_in;
  camera[ind].ptr = NULL;
  draw_dash       = NULL;
  draw_pt         = NULL;
  draw_cross      = NULL;
  draw_square     = NULL;
  scale_img       = NULL;
  ptr_buf         = NULL;

  save  = 0; // initially disabled FALSE;

  scale = int( 0.5 + camera[ind].scale * 1024.0 );
  multi = int( 0.5 + 1024.0 / camera[ind].scale );

  mode = camera[ind].modedsp;

  // check and assign functions according to rotation
//  switch ( camera[ind].modedsp & (DISPROT_000|DISPROT_090|DISPROT_180|DISPROT_270) )
  switch ( mode ) // camera[ind].modedsp )
  {
    case DISPROT_000:
      scale_img   = &CImage_display::scale_000;
      draw_pt     = &CImage_display::draw_pt_000; 
      draw_dash   = &CImage_display::draw_dash_000; 
      draw_cross  = &CImage_display::draw_cross_000;
      draw_square = &CImage_display::draw_square_000;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_000;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_000;
      buf2dsp     = &CImage_display::buf2dsp_000;
      buf2ofs     = &CImage_display::buf2ofs_000;
      dsp2buf     = &CImage_display::dsp2buf_000;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].width  * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].height * camera[ind].scale );
	  	  //------
	  outmat2dsp  = &CImage_display::outmat2dsp_000;
      break;

    case DISPROT_090:
      scale_img   = &CImage_display::scale_090;
      draw_pt     = &CImage_display::draw_pt_090;
      draw_dash   = &CImage_display::draw_dash_090; 
      draw_cross  = &CImage_display::draw_cross_090;
      draw_square = &CImage_display::draw_square_090;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_090;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_090;
      buf2dsp     = &CImage_display::buf2dsp_090;
      buf2ofs     = &CImage_display::buf2ofs_090;
      dsp2buf     = &CImage_display::dsp2buf_090;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].height * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].width  * camera[ind].scale );
	  	   //------
	  outmat2dsp  = &CImage_display::outmat2dsp_090;
      break;

    case DISPROT_180:
      scale_img   = &CImage_display::scale_180;
      draw_pt     = &CImage_display::draw_pt_180;
      draw_dash   = &CImage_display::draw_dash_180; 
      draw_cross  = &CImage_display::draw_cross_180;
      draw_square = &CImage_display::draw_square_180;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_180;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_180;
      buf2dsp     = &CImage_display::buf2dsp_180;
      buf2ofs     = &CImage_display::buf2ofs_180;
      dsp2buf     = &CImage_display::dsp2buf_180;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].width  * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].height * camera[ind].scale );
	  	     //------
	  outmat2dsp  = &CImage_display::outmat2dsp_180;
      break;

  case DISPROT_270:
      scale_img   = &CImage_display::scale_270;
      draw_pt     = &CImage_display::draw_pt_270;
      draw_dash   = &CImage_display::draw_dash_270; 
      draw_cross  = &CImage_display::draw_cross_270;
      draw_square = &CImage_display::draw_square_270;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_270;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_270;
      buf2dsp     = &CImage_display::buf2dsp_270;
      buf2ofs     = &CImage_display::buf2ofs_270;
      dsp2buf     = &CImage_display::dsp2buf_270;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].height * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].width  * camera[ind].scale );
	  	  //------
	  outmat2dsp  = &CImage_display::outmat2dsp_270;
      break;


    case DISPLAY_COL:
      scale_img   = &CImage_display::scale_000c; // all color versions
      draw_pt     = &CImage_display::draw_pt_000c; 
      draw_dash   = &CImage_display::draw_dash_000c; 
      draw_cross  = &CImage_display::draw_cross_000c;
      draw_square = &CImage_display::draw_square_000c;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_000c;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_000c;
      buf2dsp     = &CImage_display::buf2dsp_000c;
      buf2ofs     = &CImage_display::buf2ofs_000c;
      dsp2buf     = &CImage_display::dsp2buf_000c;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].width  * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].height * camera[ind].scale );
	  	  //------
	  outmat2dsp  = &CImage_display::outmat2dsp_000;
      break;


    case DISPLAY_COL90:
      scale_img   = &CImage_display::scale_090c; // all color versions
      draw_pt     = &CImage_display::draw_pt_090c; 
      draw_dash   = &CImage_display::draw_dash_090c; 
      draw_cross  = &CImage_display::draw_cross_090c;
      draw_square = &CImage_display::draw_square_090c;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_090c;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_090c;
      buf2dsp     = &CImage_display::buf2dsp_090c;
      buf2ofs     = &CImage_display::buf2ofs_090c;
      dsp2buf     = &CImage_display::dsp2buf_090c;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].height * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].width  * camera[ind].scale );
	  	    //------
	  outmat2dsp  = &CImage_display::outmat2dsp_090;
      break;

    case DISPLAY_COL270:
      scale_img   = &CImage_display::scale_270c; // all color versions
      draw_pt     = &CImage_display::draw_pt_270c; 
      draw_dash   = &CImage_display::draw_dash_270c; 
      draw_cross  = &CImage_display::draw_cross_270c;
      draw_square = &CImage_display::draw_square_270c;
      //buf_to_dsp  = &CImage_display::buf_to_dsp_270c;
      //dsp_to_buf  = &CImage_display::dsp_to_buf_270c;
      buf2dsp     = &CImage_display::buf2dsp_270c;
      buf2ofs     = &CImage_display::buf2ofs_270c;
      dsp2buf     = &CImage_display::dsp2buf_270c;
      dim_buf.cx  = (LONG) camera[ind].width;
      dim_buf.cy  = (LONG) camera[ind].height;
      dim_dsp.cx  = camera[ind].dspdim.cx = int( 0.5 + camera[ind].height * camera[ind].scale );
      dim_dsp.cy  = camera[ind].dspdim.cy = int( 0.5 + camera[ind].width  * camera[ind].scale );
	  	    //------
	  outmat2dsp  = &CImage_display::outmat2dsp_270;
      break;



    default:
      ASSERT(0);
      // SISTEMARE!
  }
  
  ASSERT (draw_pt );
  ASSERT (draw_dash );
  ASSERT (scale_img);

  buf_size = dim_buf.cx*dim_buf.cy;
  dsp_size = dim_dsp.cx*dim_dsp.cy*4;

  if ( camera[ind].modedsp==DISPLAY_COL    || 
       camera[ind].modedsp==DISPLAY_COL90  || 
       camera[ind].modedsp==DISPLAY_COL180 || 
       camera[ind].modedsp==DISPLAY_COL270 ) buf_size = 3*dim_buf.cx*dim_buf.cy;

  if ( camera[ind].dsporg.x+camera[ind].dspdim.cx>APP_RESX || camera[ind].dsporg.y+camera[ind].dspdim.cy>APP_RESY )
  {
    Message_box ( L"ERROR Dimension!",MBOX_ERROR );
    camera[ind].dsporg.x = 0;
    camera[ind].dsporg.y = 0;
  }

  ptr_img = NULL;

  ptr_buf = (BYTE*) _aligned_malloc( buf_size , 16 ); 
  ASSERT( ptr_buf && AfxIsValidAddress( ptr_buf, buf_size ) );

  FillMemory( ptr_buf, buf_size, 0 );
  ASSERT( AfxIsValidAddress( ptr_buf, buf_size ) );


  area_dsp.left   = camera[ind].dsporg.x;
  area_dsp.right  = camera[ind].dsporg.x + camera[ind].dspdim.cx;
  area_dsp.top    = camera[ind].dsporg.y;
  area_dsp.bottom = camera[ind].dsporg.y + camera[ind].dspdim.cy;

  img_dsp.Create( dim_dsp.cx, dim_dsp.cy, 32 );
  ptr_dsp = (UINT*) img_dsp.GetMemStart();


//  #ifdef SAVE_FULL_PROC // _DEBUG
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    ptr_fulldsp = NULL;
    img_fulldsp.Create(  dim_buf.cx, dim_buf.cy, 32 );
    ptr_fulldsp = (UINT*) img_fulldsp.GetMemStart();
    for ( int i=0 ; i<dim_buf.cx*dim_buf.cy ; i++ ) ptr_fulldsp[i] = 0xFF000000; // con AA=00 appaiono trasparenti 
  #endif


  mem_dc = NULL;

  for ( x=0 ; x<dsp_size/4 ; x++ ) ptr_dsp[x] = COL_BLACK; // con AA=00 appaiono trasparenti
//  for ( x=0 ; x<dsp_size/4 ; x++ ) ptr_dsp[x] = 0x00000000;
  
  for ( x=0; x<40; x++) for ( y=0; y<40; y++)
  {
    (this->*draw_pt)(              x,              y, COL_WHITE  ); // Box upper-left  WHITE
    (this->*draw_pt)( dim_buf.cx-1-x,              y, COL_RED    ); // Box upper-right RED
    (this->*draw_pt)(              x, dim_buf.cy-1-y, COL_GREEN  ); // Box lower-left  GREEN
    (this->*draw_pt)( dim_buf.cx-1-x, dim_buf.cy-1-y, COL_BLUE   ); // Box lower-right BLUE
  }

}




CImage_display::~CImage_display(void)
{
  if ( mem_dc )
  {
    delete mem_dc;
    mem_dc = NULL;
  }

  img_dsp.Destroy();
  ptr_dsp = NULL;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      img_fulldsp.Destroy();   // image dsp full resolution
      ptr_fulldsp = NULL;      // pointer to image dsp
    }
  #endif

  if ( ptr_buf ) _aligned_free( ptr_buf ); // free( ptr_buf );
  ptr_buf = NULL;
}



void CImage_display::enable_full_dsp( int ind/*=0*/ )
{
  //#ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
  if ( ptr_fulldsp == NULL ) // create only once!
    {
      img_fulldsp.Create( dim_buf.cx, dim_buf.cy, 32 );
      ptr_fulldsp = (UINT*) img_fulldsp.GetMemStart();
      for ( int i=0 ; i<dim_buf.cx*dim_buf.cy ; i++ ) ptr_fulldsp[i] = COL_BLACK; // con AA=00 appaiono trasparenti 
    }
  #endif
}



void CImage_display::draw_full( int x, int y, UINT col )
{

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!

  if ( ptr_fulldsp==NULL ) return;
  
  int ofs;
  ofs = x-2+(y-2)*dim_buf.cx;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;

  ofs = x-2+(y-1)*dim_buf.cx;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = col;

  ofs = x-2+(y)*dim_buf.cx;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ofs++;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = col;

  ofs = x-2+(y+1)*dim_buf.cx;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = COL_BLACK;
  ptr_fulldsp[ofs++] = col;

  ofs = x-2+(y+2)*dim_buf.cx;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;
  ptr_fulldsp[ofs++] = col;

/*  ptr_fulldsp[x-2+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x-1+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x  +(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x+1+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x-2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x-2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x-2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y  )*dim_buf.cx] = col;
  ptr_fulldsp[x-2+(y+2)*dim_buf.cx] = col;
  ptr_fulldsp[x-1+(y+2)*dim_buf.cx] = col;
  ptr_fulldsp[x  +(y+2*dim_buf.cx] = col;
  ptr_fulldsp[x+1+(y+2*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y+2*dim_buf.cx] = col;
  ptr_fulldsp[x-1+(y  -dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x  +(y  -dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+1+(y  -dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x-1+(y             ] = 0xFF000000;
  ptr_fulldsp[x+1+y             ] = 0xFF000000;
  ptr_fulldsp[x-1+y  +dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x  +y  +dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+1+y  +dim_buf.cx] = 0xFF000000; */


  // draw X
/*  ptr_fulldsp[x  +(y  )*dim_buf.cx] = col; 
  ptr_fulldsp[x+1+(y+1)*dim_buf.cx] = col;
  ptr_fulldsp[x+2+(y+2)*dim_buf.cx] = col; 
  ptr_fulldsp[x-1+(y+1)*dim_buf.cx] = col;
  ptr_fulldsp[x-2+(y+2)*dim_buf.cx] = col;
  ptr_fulldsp[x+1+(y-1)*dim_buf.cx] = col; 
  ptr_fulldsp[x+2+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x-1+(y-1)*dim_buf.cx] = col; 
  ptr_fulldsp[x-2+(y-2)*dim_buf.cx] = col;
  ptr_fulldsp[x+1+(y  )*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+2+(y+1)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+2+(y-1)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x-1+(y  )*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x-2+(y-1)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x  +(y+1)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+1+(y+2)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x-1+(y+2)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x  +(y-1)*dim_buf.cx] = 0xFF000000;
  ptr_fulldsp[x+1+(y-2)*dim_buf.cx] = 0xFF000000; 
  ptr_fulldsp[x-1+(y-2)*dim_buf.cx] = 0xFF000000;  */

  #endif
}




void CImage_display::scale_grabresult( CGrabResultPtr ptr_gr )
{
  //BYTE* img_acq;
  if ( mode==DISPLAY_COL ) // need to be converted to RGB before showing
  {                        // seems to take about 9ms for 560x560 BG_8PACKED images (ACa 780gc)
    //CTimer conv;
    
    CPylonImage targetImage;
    CImageFormatConverter converter;

    targetImage.AttachUserBuffer( ptr_buf, buf_size, PixelType_RGB8packed, dim_buf.cx, dim_buf.cy, 0, ImageOrientation_TopDown );

    converter.ImageHasDestinationFormat( ptr_gr );
    converter.OutputPixelFormat  = PixelType_RGB8packed;  //PixelType_GBR8packed; // PixelType_Mono8;
    converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
    converter.Convert( targetImage, ptr_gr );
//    converter.Convert( ptr_buf, ptr_gr );
    ASSERT( targetImage.GetWidth ()==dim_buf.cx );
    ASSERT( targetImage.GetHeight()==dim_buf.cy );
    ASSERT( targetImage.GetImageSize()==dim_buf.cx*dim_buf.cy*3 );
    
    //CopyMemory( ptr_buf, (BYTE*) targetImage.GetBuffer(), buf_size );

    scale_000c( ptr_buf ); // (BYTE*) targetImage.GetBuffer() );
    //TRACE("conversion time %d us - ", conv.elapsed_us() );
    return;
  }

  if ( mode==DISPLAY_COL90 ) // need to be converted to RGB before showing
  {                        // seems to take about 9ms for 560x560 BG_8PACKED images (ACa 780gc)
    CPylonImage targetImage;
    CImageFormatConverter converter;
    targetImage.AttachUserBuffer( ptr_buf, buf_size, PixelType_RGB8packed, dim_buf.cx, dim_buf.cy, 0, ImageOrientation_TopDown );
    converter.ImageHasDestinationFormat( ptr_gr );
    converter.OutputPixelFormat  = PixelType_RGB8packed;  //PixelType_GBR8packed; // PixelType_Mono8;
    converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
    converter.Convert( targetImage, ptr_gr );
    ASSERT( targetImage.GetWidth ()==dim_buf.cx );
    ASSERT( targetImage.GetHeight()==dim_buf.cy );
    ASSERT( targetImage.GetImageSize()==dim_buf.cx*dim_buf.cy*3 );
    scale_090c( ptr_buf );
    //TRACE("conversion time %d us - ", conv.elapsed_us() );
    return;
  }

  if ( mode==DISPLAY_COL270 ) // need to be converted to RGB before showing
  {                        // seems to take about 9ms for 560x560 BG_8PACKED images (ACa 780gc)
    CPylonImage targetImage;
    CImageFormatConverter converter;
    targetImage.AttachUserBuffer( ptr_buf, buf_size, PixelType_RGB8packed, dim_buf.cx, dim_buf.cy, 0, ImageOrientation_TopDown );
    converter.ImageHasDestinationFormat( ptr_gr );
    converter.OutputPixelFormat  = PixelType_RGB8packed;  //PixelType_GBR8packed; // PixelType_Mono8;
    converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
    converter.Convert( targetImage, ptr_gr );
    ASSERT( targetImage.GetWidth ()==dim_buf.cx );
    ASSERT( targetImage.GetHeight()==dim_buf.cy );
    ASSERT( targetImage.GetImageSize()==dim_buf.cx*dim_buf.cy*3 );
    scale_270c( ptr_buf );
    //TRACE("conversion time %d us - ", conv.elapsed_us() );
    return;
  }

/*  else  ORIGINAL VERSION
  {
    (this->*scale_img)( (BYTE*) ptr_gr->GetBuffer() ); // use standard BW scaling functions
  }  */


  // else use bw
  (this->*scale_img)( (BYTE*) ptr_gr->GetBuffer() ); // use standard BW scaling functions

}

void CImage_display::bayer2rgb( CGrabResultPtr ptr_gr ) // just convert to RGB
{
    CPylonImage targetImage; // seems to take about 9ms for 560x560 BG_8PACKED images (ACa 780gc)
    CImageFormatConverter converter;
    targetImage.AttachUserBuffer( ptr_buf, buf_size, PixelType_RGB8packed, dim_buf.cx, dim_buf.cy, 0, ImageOrientation_TopDown );

    converter.ImageHasDestinationFormat( ptr_gr );
    converter.OutputPixelFormat  = PixelType_RGB8packed;  //PixelType_GBR8packed; // PixelType_Mono8;
    converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
    converter.Convert( targetImage, ptr_gr );
    ASSERT( targetImage.GetWidth ()==dim_buf.cx );
    ASSERT( targetImage.GetHeight()==dim_buf.cy );
    ASSERT( targetImage.GetImageSize()==dim_buf.cx*dim_buf.cy*3 ); 
    return;
}

void CImage_display::buf2mat( CGrabResultPtr ptr_grab, cv::Mat& im_in )
{
  //BYTE* img_acq;
  if ( mode==DISPLAY_COL || mode==DISPLAY_COL90 || mode==DISPLAY_COL180 || mode==DISPLAY_COL270 ) // need to be converted to RGB before showing
  {                        // seems to take about 9ms for 560x560 BG_8PACKED images (ACa 780gc)
        
    CPylonImage targetImage;
    CImageFormatConverter converter;
    targetImage.AttachUserBuffer( ptr_buf, buf_size, PixelType_BGR8packed, dim_buf.cx, dim_buf.cy, 0, ImageOrientation_TopDown );

    converter.ImageHasDestinationFormat( ptr_grab );
    converter.OutputPixelFormat  = PixelType_BGR8packed;  //PixelType_GBR8packed; // PixelType_Mono8;
    converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
    converter.Convert( targetImage, ptr_grab );
//    converter.Convert( ptr_buf, ptr_gr );
    ASSERT( targetImage.GetWidth ()==dim_buf.cx );
    ASSERT( targetImage.GetHeight()==dim_buf.cy );
    ASSERT( targetImage.GetImageSize()==dim_buf.cx*dim_buf.cy*3 );
    
   
	cv::Mat img_in(dim_buf.cy, dim_buf.cx, CV_8UC3, ptr_buf );  
  }
  else 
	cv::Mat img_in(dim_buf.cy, dim_buf.cx, CV_8U, static_cast<unsigned char*>(ptr_grab->GetBuffer()) );  

    return;
  }







// -----------------------------------------------------------------------------
// FUNCTIONS FOR UNROTATED SCALING MODE DISPROT_000 - CAMERA STICKER ON TOP SIDE
// -----------------------------------------------------------------------------

void CImage_display::scale_000( BYTE* im_in )
{
  //if ( im_in==NULL ) im_in = ptr_buf;
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_000() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
    //Save_images();
  }
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[ ((x*multi)>>10) + (dim_buf.cy-1-((y*multi)>>10))*dim_buf.cx ]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[x+y*dim_buf.cx]]; invertito vertic

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}

void CImage_display::draw_pt_000( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}

void CImage_display::draw_dash_000( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy   );
  ptr_dsp[((x*scale)>>10)-1+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c2; //  ptr_dsp[x+y*dim_dsp.cx] = col;
  ptr_dsp[((x*scale)>>10)  +(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c1; 
  ptr_dsp[((x*scale)>>10)+1+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c2; 

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}

// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_000( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = (x*scale)>>10;
  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
}

// draws 5x5 square in color, inside square black
void CImage_display::draw_square_000( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = (x*scale)>>10;
  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;
  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;
  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif
}


void CImage_display::buf2dsp_000( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  x = (x*scale)>>10;
  y = dim_dsp.cy-1-((y*scale)>>10);
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_000( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx >=             0 );
  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
  return( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx );
}


void CImage_display::dsp2buf_000( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
  x = (x*multi)>>10;
  y = dim_buf.cy-1-((y*multi)>>10);
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}

/*
void CImage_display::buf_to_dsp_000( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
  pt->x = int( pt->x * camera[ind].scale );
  pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_000( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
  pt->x = int( pt->x / camera[ind].scale );
  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/








// --------------------------------------------------------------------------------------------
// FUNCTIONS FOR UNROTATED SCALING MODE DISPROT_000 IN COLOR MODE - CAMERA STICKER ON TOP SIDE
// --------------------------------------------------------------------------------------------
void CImage_display::outmat2dsp_000( cv::Mat& im_out )
{
	ASSERT(  im_out.rows == dim_buf.cy );
	ASSERT(  im_out.cols == dim_buf.cx ); 
	UINT multi = int( 0.5 + 1024.0 / camera[ind].scale );

	for (int x=0; x<dim_dsp.cx; x++)
		for (int y=0; y<dim_dsp.cy; y++)  
		{
		  int ptr = 3*((x*multi)>>10) + 3*(dim_buf.cy-1-((y*multi)>>10))*dim_buf.cx; // offset of first byte
		  ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | *(im_out.data+ptr) | *(im_out.data+ptr+1)<<8 | *(im_out.data+ptr+2)<<16;
		}
}

void CImage_display::scale_000c( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, 3*dim_buf.cx*dim_buf.cy ) );

  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_000c() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }

  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
    {
      int ptr = 3*((x*multi)>>10) + 3*(dim_buf.cy-1-((y*multi)>>10))*dim_buf.cx; // offset of first byte
      ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | im_in[ptr]<<16 | im_in[ptr+1]<<8 | im_in[ptr+2];
    }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif
}

void CImage_display::draw_pt_000c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}

void CImage_display::draw_dash_000c( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[((x*scale)>>10)-1+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c2; //  ptr_dsp[x+y*dim_dsp.cx] = col;
  ptr_dsp[((x*scale)>>10)  +(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c1; 
  ptr_dsp[((x*scale)>>10)+1+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = c2; 

  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}


// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_000c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = (x*scale)>>10;
  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
}

// draws 5x5 square in color, inside square black
void CImage_display::draw_square_000c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = (x*scale)>>10;
  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;
  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;
  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif


}



void CImage_display::buf2dsp_000c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  x = (x*scale)>>10;
  y = dim_dsp.cy-1-((y*scale)>>10);
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_000c( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx >=             0 );
  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
  return( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx );
}


void CImage_display::dsp2buf_000c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
  x = (x*multi)>>10;
  y = dim_buf.cy-1-((y*multi)>>10);
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}

/*
void CImage_display::buf_to_dsp_000c( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
  pt->x = int( pt->x * camera[ind].scale );
  pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_000c( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
  pt->x = int( pt->x / camera[ind].scale );
  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/





// --------------------------------------------------------------------------------------------
// FUNCTIONS FOR 90° CW COLOR ROTATED SCALING MODE DISPROT_090C - CAMERA STICKER ON LEFT SIDE
// --------------------------------------------------------------------------------------------
void CImage_display::outmat2dsp_090( cv::Mat& im_out )
{
	ASSERT(  im_out.rows == dim_buf.cy );
	ASSERT(  im_out.cols == dim_buf.cx ); 
	UINT multi = int( 0.5 + 1024.0 / camera[ind].scale );

	for (int x=0; x<dim_dsp.cx; x++)
		for (int y=0; y<dim_dsp.cy; y++)  
		{
		  int ptr = 3*(dim_buf.cx-1-((y*multi)>>10) ) + 3*( dim_buf.cy-1-((x*multi)>>10) )*dim_buf.cx;  // offset of first byte
		  ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | *(im_out.data+ptr) | *(im_out.data+ptr+1)<<8 | *(im_out.data+ptr+2)<<16;
		}
}

void CImage_display::scale_090c( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, 3*dim_buf.cx*dim_buf.cy ) );

  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_090c() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }

  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
    {
      int ptr = 3*( dim_buf.cx-1-((y*multi)>>10) ) + 3*( dim_buf.cy-1-((x*multi)>>10) )*dim_buf.cx; // offset of first byte
      ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | im_in[ptr]<<16 | im_in[ptr+1]<<8 | im_in[ptr+2];
    }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif
}

void CImage_display::draw_pt_090c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;

  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;

  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}

void CImage_display::draw_dash_090c( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );
//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;

  ptr_dsp[dim_dsp.cx-1-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c2;
  ptr_dsp[dim_dsp.cx-1  -((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c1;
  ptr_dsp[dim_dsp.cx-1+1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c2;

  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}


// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_090c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  int xn = (x*scale)>>10;
//  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;

  int xn = dim_dsp.cx-1-((y*scale)>>10);
  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
}

// draws 5x5 square in color, inside square black
void CImage_display::draw_square_090c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  int xn = (x*scale)>>10;
//  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;
  
  int xn = dim_dsp.cx-1-((y*scale)>>10);
  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;
  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;
  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif


}



void CImage_display::buf2dsp_090c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );

/* vecchio, sec me sbagl
  int xt = dim_dsp.cx-1-((y*scale)>>10);
  y = dim_dsp.cy-1-((x*scale)>>10);
  x = xt; */

  int xt = dim_dsp.cx-1-((y*scale)>>10);
  y = dim_dsp.cy-1-((x*scale)>>10);
  x = xt;

  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_090c( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
//  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx >=             0 );
//  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
//  return( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx >=             0 );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
  return( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx );

}


void CImage_display::dsp2buf_090c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );

  int xt = dim_buf.cx-1-((y*multi)>>10);
  y = dim_buf.cy-1-((x*multi)>>10);
  x = xt; 

  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}

/*
void CImage_display::buf_to_dsp_090c( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x * camera[ind].scale );
//  pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_090c( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x / camera[ind].scale );
//  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/








// --------------------------------------------------------------------------------------------
// FUNCTIONS FOR 270° CW COLOR ROTATED SCALING MODE DISPROT_270C - CAMERA STICKER 
// --------------------------------------------------------------------------------------------
void CImage_display::outmat2dsp_270( cv::Mat& im_out )
{
	ASSERT(  im_out.rows == dim_buf.cy );
	ASSERT(  im_out.cols == dim_buf.cx ); 
	UINT multi = int( 0.5 + 1024.0 / camera[ind].scale );

	for (int x=0; x<dim_dsp.cx; x++)
		for (int y=0; y<dim_dsp.cy; y++)  
		{
		  int ptr = 3*( (y*multi)>>10 ) + 3*( (x*multi)>>10 )*dim_buf.cx; // offset of first byte
		  ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | *(im_out.data+ptr) | *(im_out.data+ptr+1)<<8 | *(im_out.data+ptr+2)<<16;
		}
}

void CImage_display::scale_270c( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, 3*dim_buf.cx*dim_buf.cy ) );

  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_090c() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }

  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
    {
//      int ptr = 3*( dim_buf.cx-1-((y*multi)>>10) ) + 3*( dim_buf.cy-1-((x*multi)>>10) )*dim_buf.cx; // offset of first byte
//       ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ ((y*multi)>>10) + ((x*multi)>>10)*dim_buf.cx ]];

      int ptr = 3*( (y*multi)>>10 ) + 3*( (x*multi)>>10 )*dim_buf.cx; // offset of first byte
      ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | im_in[ptr]<<16 | im_in[ptr+1]<<8 | im_in[ptr+2];
    }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif
}

void CImage_display::draw_pt_270c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;
// 000c ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
//  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;

  ptr_dsp[((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx] = col;

  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}


void CImage_display::draw_dash_270c( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[((y*scale)>>10)-1+((x*scale)>>10)*dim_dsp.cx] = c2;
  ptr_dsp[((y*scale)>>10)  +((x*scale)>>10)*dim_dsp.cx] = c1;
  ptr_dsp[((y*scale)>>10)+1+((x*scale)>>10)*dim_dsp.cx] = c2;

  //TRACE("PT(%3d,%3d)=%08X\n", (x*scale)>>10, dim_dsp.cy-1-((y*scale)>>10), col );
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}

// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_270c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  int xn = dim_dsp.cx-1-((y*scale)>>10);
//  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;
//  int xn = dim_dsp.cx-1-((y*scale)>>10);
//  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;

  int xn = ((y*scale)>>10);
  int yn = ((x*scale)>>10)*dim_dsp.cx;


  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//  ptr_dsp[((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;
}

// draws 5x5 square in color, inside square black
void CImage_display::draw_square_270c( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  int xn = (x*scale)>>10;
//  int yn = (dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx;
  
//  int xn = dim_dsp.cx-1-((y*scale)>>10);
//  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;
  int xn = ((y*scale)>>10);
  int yn = ((x*scale)>>10)*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;
  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;
  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif


}



void CImage_display::buf2dsp_270c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );

// 090
//  int xt = dim_dsp.cx-1-((y*scale)>>10);
//  y = dim_dsp.cy-1-((x*scale)>>10);
//  x = xt;

// 000c
//  x = (x*scale)>>10;
//  y = dim_dsp.cy-1-((y*scale)>>10);

//  int xt = dim_dsp.cx-1-((y*scale)>>10);
//  y = dim_dsp.cy-1-((x*scale)>>10);
//  x = xt;

  int xt = (y*scale)>>10; 
  y = (x*scale)>>10;
  x = xt; 


  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_270c( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
//  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx >=             0 );
//  ASSERT( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
//  return( ((x*scale)>>10)+(dim_dsp.cy-1-((y*scale)>>10))*dim_dsp.cx );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx >=             0 );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );

//  return( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx );
//  return( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx );

  return( ((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx );


}


void CImage_display::dsp2buf_270c( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );

  int xt = (y*multi)>>10; 
  y = (x*multi)>>10;
  x = xt; 

  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}

/*
void CImage_display::buf_to_dsp_270c( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x * camera[ind].scale );
//  pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_270c( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x / camera[ind].scale );
//  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/





// -----------------------------------------------------------------------------------
// FUNCTIONS FOR 90° CW ROTATED SCALING MODE DISPROT_090 - CAMERA STICKER ON LEFT SIDE
// -----------------------------------------------------------------------------------

void CImage_display::scale_090( BYTE* im_in )
{
  //if ( im_in==NULL ) im_in = ptr_buf;
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_090() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  } 
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ dim_buf.cx-1-((y*multi)>>10) + (dim_buf.cy-1-((x*multi)>>10))*dim_buf.cx ]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ dim_buf.cx-((y*multi)>>10) + ((x*multi)>>10)*dim_buf.cx ]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[(dim_buf.cx-y*4-1)+x*4*dim_buf.cx]];

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}


void CImage_display::draw_pt_090( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif

}

void CImage_display::draw_dash_090( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[dim_dsp.cx-1-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c2;
  ptr_dsp[dim_dsp.cx-1  -((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c1;
  ptr_dsp[dim_dsp.cx-1+1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = c2;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif

}


// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_090( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = dim_dsp.cx-1-((y*scale)>>10);
  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;
}



void CImage_display::draw_square_090( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = dim_dsp.cx-1-((y*scale)>>10);
  int yn = (dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;

  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;

  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;

  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;

  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif

}




void CImage_display::buf2dsp_090( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );

  /* vecchio, sec me sbagl
  int xt = dim_dsp.cx-1-((y*scale)>>10);
  y = dim_dsp.cy-1-((x*scale)>>10);
  x = xt; */

  int xt = dim_dsp.cx-1-((y*scale)>>10);
  y = dim_dsp.cy-1-((x*scale)>>10);
  x = xt;

  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_090( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx >=             0 );
  ASSERT( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx <  (dsp_size>>2) );
  return( dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx );
}


void CImage_display::dsp2buf_090( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );

  int xt = dim_buf.cx-1-((y*multi)>>10);
  y = dim_buf.cy-1-((x*multi)>>10);
  x = xt; 

  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}


/*
void CImage_display::buf_to_dsp_090( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
  pt->x = int( (dim_buf.cx-pt->y) * camera[ind].scale );
  pt->y = int( pt->x * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_090( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( dimpt->x / camera[ind].scale );
//  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/



// --------------------------------------------------------------------------------
// FUNCTIONS FOR UNROTATED SCALING MODE DISPROT_180 - CAMERA STICKER ON BOTTOM SIDE
// --------------------------------------------------------------------------------

void CImage_display::outmat2dsp_180( cv::Mat& im_out )
{
	ASSERT(  im_out.rows == dim_buf.cy );
	ASSERT(  im_out.cols == dim_buf.cx ); 
	UINT multi = int( 0.5 + 1024.0 / camera[ind].scale );

	for (int x=0; x<dim_dsp.cx; x++)
		for (int y=0; y<dim_dsp.cy; y++)  
		{
		  int ptr = 3*(dim_buf.cx-1- ((x*multi)>>10)) + 3*((y*multi)>>10)*dim_buf.cx; // offset of first byte
		  ptr_dsp[x+y*dim_dsp.cx] = 0xFF000000 | *(im_out.data+ptr) | *(im_out.data+ptr+1)<<8 | *(im_out.data+ptr+2)<<16;
		}
}

void CImage_display::scale_180( BYTE* im_in )
{
  //if ( im_in==NULL ) im_in = ptr_buf;
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_180() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[ dim_buf.cx-1- ((x*multi)>>10) + ((y*multi)>>10)*dim_buf.cx ]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[x+y*dim_buf.cx]]; invertito vertic

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}

void CImage_display::draw_pt_180( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[dim_dsp.cx-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}

void CImage_display::draw_dash_180( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[dim_dsp.cx-1-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx] = c2; //  ptr_dsp[x+y*dim_dsp.cx] = col;
  ptr_dsp[dim_dsp.cx-1  -((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx] = c1; 
  ptr_dsp[dim_dsp.cx-1+1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx] = c2; 

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}











// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_180( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = dim_dsp.cx-1-((x*scale)>>10);
  int yn = ((y*scale)>>10)*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//   ptr_dsp[dim_dsp.cx-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx] = col; // //  ptr_dsp[x+y*dim_dsp.cx] = col;

}



void CImage_display::draw_square_180( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = dim_dsp.cx-1-((x*scale)>>10);
  int yn = ((y*scale)>>10)*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;

  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;

  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;

  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;

  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif

}


void CImage_display::buf2dsp_180( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );

  x = dim_dsp.cx-1-((x*scale)>>10);
  y = (y*scale)>>10; 

  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_180( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  ASSERT( dim_dsp.cx-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx >=             0 );
  ASSERT( dim_dsp.cx-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx <  (dsp_size>>2) );
  return( dim_dsp.cx-1-((x*scale)>>10)+((y*scale)>>10)*dim_dsp.cx );
}


void CImage_display::dsp2buf_180( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );

  x = dim_buf.cx-1-((x*multi)>>10);
  y = (y*multi)>>10; 

  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}


/*
void CImage_display::buf_to_dsp_180( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x * camera[ind].scale );
//  pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_180( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
//  pt->x = int( pt->x / camera[ind].scale );
//  pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/


// -------------------------------------------------------------------------------------
// FUNCTIONS FOR 270° CW ROTATED SCALING MODE DISPROT_270 - CAMERA STICKER ON RIGHT SIDE
// -------------------------------------------------------------------------------------

void CImage_display::scale_270( BYTE* im_in )
{
  //if ( im_in==NULL ) im_in = ptr_buf;
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  //ptr_img = im_in;
  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_270() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ ((y*multi)>>10) + ((x*multi)>>10)*dim_buf.cx ]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[ im_in[ dim_buf.cx-1-((y*multi)>>10) + (dim_buf.cy-1-((x*multi)>>10))*dim_buf.cx ]];

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}


void CImage_display::draw_pt_270( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx] = col;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
  #endif
}

void CImage_display::draw_dash_270( int x , int y , int c1, int c2 )
{
  ASSERT ( x>=1 && x<dim_buf.cx-1 );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[((y*scale)>>10)-1+((x*scale)>>10)*dim_dsp.cx] = c2;
  ptr_dsp[((y*scale)>>10)  +((x*scale)>>10)*dim_dsp.cx] = c1;
  ptr_dsp[((y*scale)>>10)+1+((x*scale)>>10)*dim_dsp.cx] = c2;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( ptr_fulldsp )
    {
      ptr_fulldsp[x-1+y*dim_buf.cx] = c2;
      ptr_fulldsp[x  +y*dim_buf.cx] = c1;
      ptr_fulldsp[x+1+y*dim_buf.cx] = c2;
    }
  #endif
}






// draws central point with col, surrounding 4 pts in black
void CImage_display::draw_cross_270( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = ((y*scale)>>10);
  int yn = ((x*scale)>>10)*dim_dsp.cx;

  ptr_dsp[xn  +yn           ] = col;
  ptr_dsp[xn+1+yn           ] = col;
  ptr_dsp[xn-1+yn           ] = col;
  ptr_dsp[xn  +yn-dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+dim_dsp.cx] = col;

//   ptr_dsp[((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx] = col;


}


void CImage_display::draw_square_270( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  int xn = ((y*scale)>>10);
  int yn = ((x*scale)>>10)*dim_dsp.cx;

  ptr_dsp[xn-2+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn-2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn-2*dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn  -dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  -dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn             ] = col;
  ptr_dsp[xn+2+yn             ] = col;

  ptr_dsp[xn-2+yn  +dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn  +dim_dsp.cx] = col;

  ptr_dsp[xn-2+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn-1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn  +yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+1+yn+2*dim_dsp.cx] = col;
  ptr_dsp[xn+2+yn+2*dim_dsp.cx] = col;

  ptr_dsp[xn-1+yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  -dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  -dim_dsp.cx] = COL_BLACK;

  ptr_dsp[xn-1+yn             ] = COL_BLACK;
  ptr_dsp[xn+1+yn             ] = COL_BLACK;

  ptr_dsp[xn-1+yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn  +yn  +dim_dsp.cx] = COL_BLACK;
  ptr_dsp[xn+1+yn  +dim_dsp.cx] = COL_BLACK;

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    draw_full(x,y,col);
  #endif

}

void CImage_display::buf2dsp_270( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );

  int xt = (y*scale)>>10;
  y = (x*scale)>>10;
  x = xt;

  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );
}


int CImage_display::buf2ofs_270( int x, int y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
  ASSERT( ((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx >=             0 );
  ASSERT( ((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx <  (dsp_size>>2) );
  return( ((y*scale)>>10)+((x*scale)>>10)*dim_dsp.cx );
}


void CImage_display::dsp2buf_270( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_dsp.cx );
  ASSERT( 0<=y && y<dim_dsp.cy );

  int xt = (y*multi)>>10; 
  y = (x*multi)>>10;
  x = xt; 

  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
}


/*
void CImage_display::buf_to_dsp_270( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
 // pt->x = int( pt->x * camera[ind].scale );
 // pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_270( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
 // pt->x = int( pt->x / camera[ind].scale );
 // pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}
*/

void CImage_display::scale_000vf( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  if ( save==1 )
  {
    TRACE("CImage_display[%d]->scale_000vf() calling CopyMemory()...",ind);
    CopyMemory( ptr_buf, im_in, buf_size );
    save=2;
    TRACE("DONE\n");
  }
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[((x*multi)>>10)+((y*multi)>>10)*dim_buf.cx]];
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[x+y*dim_buf.cx]];

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}



/*

void CImage_display::draw_pt_normal( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[x+y*dim_dsp.cx] = col;
}


void CImage_display::draw_pt_scale( int x , int y , int col )
{
  //dimezza
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  
  ptr_dsp[(x>>2)+(dim_dsp.cy-1-(y>>2))*dim_dsp.cx]=col;
}

void CImage_display::draw_pt_scale_cw( int x , int y , int col )
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

  ptr_dsp[(y>>2)+(dim_dsp.cy-1-(x>>2))*dim_dsp.cx]=col;
 
}

void CImage_display::draw_pt_scale_ccw( int x , int y , int col )
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );

//  ptr_dsp[(dim_buf.cy/4-1-(y>>2))+(x>>2)*dim_dsp.cx]=col;
  ptr_dsp[(y>>2)+(x>>2)*dim_dsp.cx]=col;
}

*/


/*

void CImage_display::scale_scale( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
//      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[x*4+y*4*dim_buf.cx]];
      ptr_dsp[x+(dim_dsp.cy-1-y)*dim_dsp.cx] = lut32[im_in[x*4+y*4*dim_buf.cx]];

}

void CImage_display::scale_scale_ccw( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[(y*4)+(x*4)*dim_buf.cx]];

}

*/


/*
void CImage_display::scale_normal( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[x+y*dim_buf.cx]];
}
*/


/*
void CImage_display::scale_scale_cw( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)  
      ptr_dsp[x+y*dim_dsp.cx] = lut32[im_in[(dim_buf.cx-y*4-1)+x*4*dim_buf.cx]];


}

void CImage_display::scale_flip_y( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)
      ptr_dsp[x+y*dim_dsp.cx] = im_in[x+(dim_buf.cy-y-1)*dim_buf.cx];

}

void CImage_display::scale_flip_x( BYTE* im_in )
{
  ASSERT( AfxIsValidAddress( im_in, dim_buf.cx*dim_buf.cy ) );
  for (int x=0; x<dim_dsp.cx; x++)
    for (int y=0; y<dim_dsp.cy; y++)
      ptr_dsp[x+y*dim_dsp.cx] = im_in[(dim_buf.cx-x-1)+y*dim_buf.cx];  
}
*/



void CImage_display::Save_images( void )
{
  //static int counter = 100;
  wchar_t name[100+1];
  CTime time = CTime::GetCurrentTime();   
//    dir.Format( L"Sequence_%04d%02d%02d_%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

  swprintf( name, 100, L"%s\\image[%d]_%04d%02d%02d_%02d%02d%02d", IMAGES_DIRECTORY, ind, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  Save_images( name );
  save = 0; // FALSE;
}





void CImage_display::Save_fulldsp( wchar_t* file_name, int type )
{
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!

    if ( ptr_fulldsp==NULL ) return;

    wchar_t name[200+1];
    swprintf( name, 200, L"%s_full", file_name );
    Save_color( ptr_fulldsp, dim_buf.cx, dim_buf.cy, name, PNG_FILE, TRUE, FALSE ); // vflip - 32bpp
  #endif
}


// Function to save BUF (bw) version of image
void CImage_display::Save_buf( wchar_t* file_name, int type/*=PNG_FILE*/ )
{
  ASSERT ( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );
  wchar_t name[200+1];
  swprintf( name, 200, L"%s_buf", file_name );
  if ( mode==DISPLAY_COL || mode==DISPLAY_COL90 || mode==DISPLAY_COL180 || mode==DISPLAY_COL270 )
  {
    Save_color( (UINT*)ptr_buf, dim_buf.cx, dim_buf.cy, name, type, TRUE, TRUE ); // vflip - 24bpp
  }
  else // B&W images
  {
    Save_buffer( ptr_buf, dim_buf.cx, dim_buf.cy, name, type );
  }
}


// Function to save DSP (color) version of image
void CImage_display::Save_dsp( wchar_t* file_name, int type/*=PNG_FILE*/ )
{
  ASSERT ( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );
  wchar_t name[200+1];
  swprintf( name, 200, L"%s_dsp", file_name );
  Save_color( ptr_dsp, dim_dsp.cx, dim_dsp.cy, name, type, FALSE, FALSE ); // noflip - 32bpp
}




// Function to save both DSP (color) and BUF (bw) version of image
void CImage_display::Save_images( wchar_t* file_name, int type/*=PNG_FILE*/ )
{
  ASSERT ( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );

  Save_buf( file_name );
  Save_dsp( file_name );

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    Save_fulldsp( file_name );
  #endif



/*  wchar_t name[200];
  // save buffer first
  swprintf( name, L"%s_buf", file_name );
  Save_buffer( ptr_buf, dim_buf.cx, dim_buf.cy, name, type );

  // then color 
  swprintf( name, L"%s_dsp", file_name );
  Save_color( ptr_dsp, dim_buf.cx, dim_buf.cy, name, type );
 */
}



/*
void CImage_display::Save_image( wchar_t* file_name, int type )
{
  ASSERT ( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );

  //char file[200];
  wchar_t file[200];

  
  //CopyMemory( ptr_buf, ptr_ext, buf_size );
 
  
  // First prepare and save DSP (colour) version
  //strncpy( file, file_name, strlen(file_name) );
	//file[strlen(file_name)] = 0;
  wcsncpy( file, file_name, wcslen(file_name) );
	file[wcslen(file_name)] = 0;
  switch (type) 
  {
    case PNG_FILE:
      //strcat (file, "_dsp.png");
      wcscat ( file, L"_dsp.png" );
      break;
    case JPG_FILE:
      //strcat (file, "_dsp.jpg");
      wcscat ( file, L"_dsp.jpg" );
      break;
    case BMP_FILE:
      //strcat (file, "_dsp.bmp");
      wcscat ( file, L"_dsp.bmp" );
      break;
  }
  
  // WARNING: MAKE SURE HIGHER BYTE IS SET TO 255 BEFORE SAVING (ALPHA CHANNEL!)
//  for ( int i=0 ; i<dim_dsp.cx*dim_dsp.cy ; i++ ) ptr_dsp[i] |= 255<<24;
  img_dsp.Save( file );

  TRACE( L"Display image <%s> saved\n", file );




  if ( ptr_buf == NULL ) return; // skip if ptr_buf NULL

   // Then, prepare and save BUF (bw) version
  //strncpy( file, file_name, strlen(file_name) );
	//file[strlen(file_name)] = 0;
  wcsncpy( file, file_name, wcslen(file_name) );
	file[wcslen(file_name)] = 0;

  switch (type) 
  {
    case PNG_FILE:
      //strcat (file, "_buf.png");
      wcscat ( file, L"_buf.png" );
      break;
    case JPG_FILE:
      //strcat (file, "_buf.jpg");
      wcscat ( file, L"_buf.jpg" );
      break;
    case BMP_FILE:
      //strcat (file, "_buf.bmp");
      wcscat ( file, L"_buf.bmp" );
      break;
  }

  FCObjImage temp;
  temp.Create( dim_buf.cx, dim_buf.cy, 8 );
  BYTE*  temp_ptr = (BYTE*) temp.GetMemStart();
  ASSERT( AfxIsValidAddress( temp_ptr, buf_size ) );
//  CopyMemory( temp_ptr, ptr_buf, buf_size );

  //BYTE* ptr = ptr_buf;
  //if ( ptr_ext ) ptr = ptr_ext;
  //ASSERT( AfxIsValidAddress( ptr, buf_size ) );

  if ( !ptr_img || !AfxIsValidAddress( ptr_img, buf_size ) ) return;

  BYTE* ptr = ptr_img;

  // vertical flip
  for ( int y=(dim_buf.cy-1)*dim_buf.cx ; y>=0 ; y-=dim_buf.cx )
  {
    CopyMemory( temp_ptr, ptr+y, dim_buf.cx );
    //CopyMemory( temp_ptr, ptr_buf+y, dim_buf.cx );
    temp_ptr += dim_buf.cx;
  }

  temp.Save( file );

//  TRACE ("Buffer image <%s> saved\n", file );
  TRACE( L"Buffer image <%s> saved\n", file );
 
}
*/


// Funziona anche con TIFF ma c'è qualche problema, da verificare
// molto meglio PNG !!!
int CImage_display::Load_image ( wchar_t* file_name, BOOL silent  )
{
  ASSERT( AfxIsValidString( file_name ) );

  if ( !PathFileExists( file_name ) )
  { 
    wchar_t msg[100+1];
    swprintf( msg, 100, L"Could not open file <%s>\r\nError n.%d", file_name, GetLastError());
    if ( !silent ) Message_box( msg, MBOX_ERROR);
    return 99; 
  }

  int x, y;
  FCObjImage   img_tmp;
  img_tmp.Create( dim_buf.cx, dim_buf.cy, 8 ); //8bit
  img_tmp.Load ( file_name );

  TRACE(L"Image <%s> loaded into img_tmp - %dx%dx%d\n", file_name, img_tmp.Width(), img_tmp.Height(), img_tmp.ColorBits() );

//  ASSERT( img_tmp.ColorBits() == 24 || img_tmp.ColorBits() == 32 );
//  ASSERT( img_tmp.Width()     == DIMX_ACQ );
//  ASSERT( img_tmp.Height()    == DIMY_ACQ );

  if ( (img_tmp.ColorBits()!=24 && img_tmp.ColorBits()!=32) || (img_tmp.Width()!=dim_buf.cx) || (img_tmp.Height()!=dim_buf.cy) )
  { 
    wchar_t msg[100+1];
    swprintf( msg, 100, L"Image <%s> has wrong dimensions/color depth (%d,%d,%d)", file_name, img_tmp.Width(), img_tmp.Height(), img_tmp.ColorBits() );
    if ( !silent ) Message_box( msg, MBOX_ERROR);
    return 999; 
  }

  if ( mode == DISPLAY_COL ) // load color image (24 or 32) into buffer
  {

    if ( ptr_buf && AfxIsValidAddress( ptr_buf, 3*dim_buf.cx*dim_buf.cy ) )
    {
      if ( img_tmp.ColorBits() == 24 )
      {
        //BYTE* loaded = img_tmp.GetMemStart();
        //CopyMemory( ptr_buf, loaded, 3*dim_buf.cx*dim_buf.cy );
        //for ( y=0 ; y<dim_buf.cy ; y++ ) 
        //  for ( x=0 ; x<dim_buf.cx ; x++ ) ptr_buf[x+y*dim_buf.cx] = loaded[3*x+(dim_buf.cy-1-y)*3*dim_buf.cx]&255;

        BYTE* loaded = (BYTE*) img_tmp.GetMemStart();
        for ( y=0 ; y<dim_buf.cy ; y++ ) 
          for ( x=0 ; x<dim_buf.cx ; x++ ) 
          {
            ptr_buf[3*x+3*y*dim_buf.cx  ] = loaded[3*x+3*(dim_buf.cy-1-y)*dim_buf.cx+2];
            ptr_buf[3*x+3*y*dim_buf.cx+1] = loaded[3*x+3*(dim_buf.cy-1-y)*dim_buf.cx+1];
            ptr_buf[3*x+3*y*dim_buf.cx+2] = loaded[3*x+3*(dim_buf.cy-1-y)*dim_buf.cx  ];
          }


      }

      if ( img_tmp.ColorBits() == 32 )
      {
        UINT* loaded = (UINT*) img_tmp.GetMemStart();
        for ( y=0 ; y<dim_buf.cy ; y++ ) 
          for ( x=0 ; x<dim_buf.cx ; x++ ) 
          {
            ptr_buf[3*x+3*y*dim_buf.cx  ] = (loaded[x+(dim_buf.cy-1-y)*dim_buf.cx]>>16)&255;
            ptr_buf[3*x+3*y*dim_buf.cx+1] = (loaded[x+(dim_buf.cy-1-y)*dim_buf.cx]>> 8)&255;
            ptr_buf[3*x+3*y*dim_buf.cx+2] =  loaded[x+(dim_buf.cy-1-y)*dim_buf.cx]     &255;
          }
      }
    }


    if ( mode==DISPLAY_COL90 || mode==DISPLAY_COL180 || mode==DISPLAY_COL270 ) // load color image (24 or 32) into buffer
    {
      ASSERT(0); // CHECK AND FIX
    }

  }
  else  // load bw image into buffer
  {

    if ( ptr_buf && AfxIsValidAddress( ptr_buf, dim_buf.cx*dim_buf.cy ) )
    {
      if ( img_tmp.ColorBits() == 24 )
      {
        BYTE* loaded = img_tmp.GetMemStart();
        for ( y=0 ; y<dim_buf.cy ; y++ ) 
          for ( x=0 ; x<dim_buf.cx ; x++ ) ptr_buf[x+y*dim_buf.cx] = loaded[3*x+(dim_buf.cy-1-y)*3*dim_buf.cx]&255;
      }

      if ( img_tmp.ColorBits() == 32 )
      {
        UINT* loaded = (UINT*) img_tmp.GetMemStart();
        for ( y=0 ; y<dim_buf.cy ; y++ ) 
          for ( x=0 ; x<dim_buf.cx ; x++ ) 
            ptr_buf[x+y*dim_buf.cx] = loaded[x+(dim_buf.cy-1-y)*dim_buf.cx]&255;
      }
    }

  }

  (this->*scale_img)( ptr_buf );

  return 0; // caricamento ok
}



//int CImage_display::Create_csv( char* file_name, int x0, int y0, int dx, int dy )
int CImage_display::Create_csv( wchar_t* file_name, int x0, int y0, int dx, int dy )
{
  // Scrivo i dati raccolti su file
  int x, y;
  CFile file_out;
  char str[500];

  ASSERT( x0>=0 && x0+dx<dim_buf.cx );
  ASSERT( y0>=0 && y0+dy<dim_buf.cy );

  file_out.Open( file_name, CFile::modeCreate|CFile::modeReadWrite );

  ZeroMemory( str, sizeof(str) );
  strcat( str, "Y\\X," );
  file_out.Write( str, strlen(str) );
  for ( x=x0 ; x<=x0+dx ; x++ ) // prima riga - coordinate x
  {
    sprintf( str, "%d,", x );
    file_out.Write( str, strlen(str) );
  }
  sprintf( str, "\r\n" );
  file_out.Write( str, strlen(str) );

  for ( y=y0 ; y<=y0+dy ; y++ ) // riga n 
  {
    sprintf( str, "%d,", y );
    file_out.Write( str, strlen(str) );

    for ( x=x0 ; x<=x0+dx ; x++ ) // coordinate x
    {
      sprintf( str, "%d,", ptr_buf[x+y*dim_buf.cx] );
      file_out.Write( str, strlen(str) );
    }
    sprintf( str, "\r\n" );
    file_out.Write( str, strlen(str) );
  }

  file_out.Close();
//  TRACE("FILE <%s> SALVATO\n", file_name );
  TRACE( L"FILE <%s> SAVED\n", file_name );
  return 0;
}





/*
void CImage_display::buf_to_dsp_normal ( POINT* pt )
{

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );

    pt->x = int( pt->x * camera[ind].scale );
	  pt->y = pt->y;

  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

}
void CImage_display::buf_to_dsp_scale ( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );


//  ptr_dsp[(x>>2)+(dim_dsp.cy-1-(y>>2))*dim_dsp.cx]=col;

  pt->x = pt->x>>2;
//  pt->y = pt->y>>2;
  pt->y = dim_dsp.cy-1-(pt->y>>2);
    
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::buf_to_dsp_scale_cw ( POINT* pt )
{
  int tmp;

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );

  tmp = pt->x;
  pt->x = pt->y>>2;
  pt->y = (dim_buf.cx>>2)-(tmp>>2)-1; 

  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

}

void CImage_display::buf_to_dsp_scale_ccw( POINT* pt )
{
  int tmp;

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );

  tmp = pt->x;
//  pt->x = (dim_buf.cy>>2)-(pt->y>>2)-1;
//  pt->y = tmp>>2; 
  pt->x = pt->y>>2;
//  pt->y = dim_dsp.cx-(tmp>>2)-1; 
  pt->y = tmp>>2; 
    
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );


//  ptr_dsp[(y>>2)+(x>>2)*dim_dsp.cx]=col;

}


void CImage_display::dsp_to_buf_normal( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

  pt->x = pt->x;
  pt->y = pt->y; 

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
}

void CImage_display::dsp_to_buf_scale( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

  pt->x = pt->x<<2;
  pt->y = pt->y<<2; 

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
}

void CImage_display::dsp_to_buf_scale_cw( POINT* pt )
{
  int tmp;

  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

  tmp = pt->x*4;
  pt->x = pt->y*4;
  pt->y = tmp; 

  //ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  //ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
}

void CImage_display::dsp_to_buf_scale_ccw( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );

  pt->x = pt->y<<2;
  pt->y = pt->x<<2; 

  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
}

*/

void CImage_display::Draw_image( BOOL reset_alpha /*=FALSE*/ )
{

/*  if ( save )
  {
    CopyMemory( ptr_buf, im_in, buf_size );
    Save_images();
  }*/


  HDC hdc = GetDC( pView->m_hWnd );

  ASSERT( hdc );
//  FCWin32::DrawImage( img_dsp, hdc, area_dsp ); // Update image displaying
  if ( reset_alpha ) for ( UINT i=0 ; i<dsp_size/4 ; i++ ) ptr_dsp[i] |= 0xFF000000;
  img_dsp.Draw( hdc, area_dsp ); // Update image displaying
//  img_dsp.Draw( HDC(mem_dc) , area_dsp ); // Update image displaying

  ReleaseDC( pView->m_hWnd, hdc );

  if ( save==2 )
  {
    //CopyMemory( ptr_buf, im_in, buf_size );
    TRACE("CImage_display[%d] calling Save_images()...",ind);
    Save_images();
    TRACE("DONE\n");
  }


}


void CImage_display::scale_fulldsp( BYTE* im_in )
{
  //#ifdef _DEBUG
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    //if ( ptr_fulldsp==NULL ) return;

    if ( mode==DISPLAY_COL ||mode==DISPLAY_COL90) return; // NOT WORKING DURING TESTS FOR DOUBLE CAP - DISABLE FOR NOW!

    if ( im_in==NULL ) im_in = ptr_buf;
    ASSERT( AfxIsValidAddress( im_in,       dim_buf.cx*dim_buf.cy ) );
    ASSERT( AfxIsValidAddress( im_in,       buf_size              ) );
    ASSERT( AfxIsValidAddress( ptr_fulldsp, buf_size*sizeof(UINT) ) );

//    for ( UINT x=0; x<buf_size; x++) ptr_fulldsp[x] = lut32[im_in[x]];
    for ( UINT x=0; x<buf_size; x++) ptr_fulldsp[x] = lut32[im_in[x]];
  #endif
}


//  ptr_dsp[(x>>2)+(dim_dsp.cy-1-(y>>2))*dim_dsp.cx]=col;




// Draw marker on the screen (input in buf coords)
void CImage_display::Draw_marker( int xb, int yb, COLORREF color ) // 0xAARRGGBB
{
  //#define MARKER_COL 0xFFFF0000

  ASSERT( xb>=6 && xb<dim_buf.cx-6 );
  ASSERT( yb>=6 && yb<dim_buf.cy-6 );


  (this->*buf2dsp)( xb, yb );  // da verificare !!!


  DRW_PT( xb-2, yb  , color );
  DRW_PT( xb-1, yb  , color );
  DRW_PT( xb  , yb  , color );
  DRW_PT( xb+1, yb  , color );
  DRW_PT( xb+2, yb  , color );
  DRW_PT( xb  , yb-2, color );
  DRW_PT( xb  , yb-1, color );
  DRW_PT( xb  , yb+1, color );
  DRW_PT( xb  , yb+2, color );

  DRW_PT( xb-1, yb+1, COL_BLACK );
  DRW_PT( xb+1, yb+1, COL_BLACK );
  DRW_PT( xb-1, yb-1, COL_BLACK );
  DRW_PT( xb+1, yb-1, COL_BLACK );



/*
  POINT pt = { xb, yb };
  (this->*buf_to_dsp)( &pt );

  DRW_PT( pt.x-2, pt.y  , color );
  DRW_PT( pt.x-1, pt.y  , color );
  DRW_PT( pt.x  , pt.y  , color );
  DRW_PT( pt.x+1, pt.y  , color );
  DRW_PT( pt.x+2, pt.y  , color );
  DRW_PT( pt.x  , pt.y-2, color );
  DRW_PT( pt.x  , pt.y-1, color );
  DRW_PT( pt.x  , pt.y+1, color );
  DRW_PT( pt.x  , pt.y+2, color );

  DRW_PT( pt.x-1, pt.y+1, COL_BLACK );
  DRW_PT( pt.x+1, pt.y+1, COL_BLACK );
  DRW_PT( pt.x-1, pt.y-1, COL_BLACK );
  DRW_PT( pt.x+1, pt.y-1, COL_BLACK );
  */

}


// Altezza font predefinito = 16
// color: 0x00BBGGRR (lasciare alpha=00)

//void CImage_display::Draw_text( char* text, int x, int y, COLORREF color )
void CImage_display::Draw_text( wchar_t* text, int x, int y, COLORREF color )
{
 //ASSERT( hdc );
  #define FONT_H 14

//  mem_dc = new FCImageDrawDC(img_dsp);
//  ASSERT( mem_dc );

  std::auto_ptr<FCImageDrawDC>   mem_dc(new FCImageDrawDC(img_dsp)) ;

  HDC dc = *mem_dc;


/*  HFONT hfnt, hOldFont; 
  hfnt = (HFONT) GetStockObject(ANSI_FIXED_FONT); 
  SelectObject( dc, hfnt ); FUNZIONA */

  SelectObject( dc, (HFONT) GetStockObject(ANSI_FIXED_FONT) );
//  SelectObject( dc, (HFONT) GetStockObject(SYSTEM_FIXED_FONT) ); // brutto


/*  if ( old_font = SelectObject( dc, hfnt ) ) 
  { 
        TextOut(hdc, 10, 50, "Sample ANSI_VAR_FONT text.", 26); 
        SelectObject(hdc, hOldFont); 
    }  */

  SetTextAlign( dc, TA_LEFT );
//  SetTextColor( dc, RGB(0,0,255) );
  SetTextColor( dc, color   ) ;

//  SetBkColor( dc, RGB(230,230,230) );
//  SetBkColor( dc, 0xFF000000 );
//  SetBkMode( dc, OPAQUE );

//  SIZE dim;
//  GetTextExtentPoint32( dc, text, strlen(text), &dim );
//  Rectangle( dc, x, y-FONT_H/2, x+dim.cx, y+dim.cy );

  //TextOut( dc, x, y-FONT_H/2, text, strlen(text) );
  TextOut( dc, x, y-FONT_H/2, text, wcslen(text) );
}








/*
// FUNZIONA
  mem_dc = new FCImageDrawDC(img_dsp);
 // ASSERT( mem_dc );

  HDC dc = *mem_dc;
  SetTextColor( dc, RGB(0,0,255)) ;
  TextOut( dc, 20, 20, "TESTO DI PROVA", 14 );

  SetTextColor( dc, RGB(255,0,0)) ;
  TextOut( dc, 20, 40, "TESTO DI PROVA 2", 16 );

  delete mem_dc;
  mem_dc = NULL;
*/

//  ASSERT( img_dc );
//  SetTextColor( img_dc, RGB(0,0,255)) ;
//  TextOut( img_dc, 20, 20, "TESTO DI PROVA", 14 );

//  for ( int i=0 ; i<1000 ; i++ ) (this->*draw_pt)(i,i,0xFF00FF00); //AARRGGBB
//  img_dsp.Draw( hdc, area_dsp ); // Update image displaying

/*
  HDC hdcn = GetDC(NULL);
  SetTextColor( hdcn, RGB(0,0,255)) ;
  TextOut( hdcn, 950, 650, _T("TESTO DI PROVA"), 14 );
  RECT rett = { 1000, 800, 1200, 1000 };
  FillRect( hdcn, &rett, (HBRUSH) (COLOR_WINDOW+1) );
  for ( int i=0 ; i<1000 ; i++ ) (this->*draw_pt)(i,i,0xFF00FF00);
  img_dsp.Draw( hdc, area_dsp ); // Update image displaying
*/

