// Image_display.h : class to Draw Images

#pragma once
#include "afx.h"
#include "ImageStone.h"
#include "Globals.h"

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>



//#define   INTBUFFER        64 // Allocate internal buffer for saving


#define   DISPROT_000       2 // no rotation acquisition-display
#define   DISPROT_090       4 // acquired image is rotate  90 CW before display
#define   DISPROT_180       8 // acquired image is rotate 180 CW before display
#define   DISPROT_270      16 // acquired image is rotate 270 CW before display

#define   DISPLAY_COL      64 // allocate and use color 24bits display for image - 
#define   DISPLAY_COL90   128 // allocate and use color 24bits display for image also rotated 90°
#define   DISPLAY_COL180  256 // allocate and use color 24bits display for image also rotated 180°
#define   DISPLAY_COL270  512 // allocate and use color 24bits display for image also rotated 270°


//#define   NORMAL          128 // Transform to RGB
//#define   SCALE           256 // scale 4:1
//#define   SCALE_CW        512 // scale 4:1 and rotate clockwise
//#define   SCALE_CCW      1024 // scale 4:1 and rotate counterclockwise

//#define   FLIP_X         2048 // flip horizontal
//#define   FLIP_Y         4096 // flip vertical

#define   PNG_FILE       8192
#define   JPG_FILE      16384
#define   BMP_FILE      32768


#define   MAX_LENGTH_STR   50


using namespace Pylon;



class CImage_display 
{

  POINT origin_dsp;         // Display origin coordinates
  UINT  scale;              // reduction scaling integer factor (/1024)
  UINT  multi;              // amplification scaling integer factor (/1024)

public:

  BOOL           visible;
  int            save;      // request to save images to disk ( 1=copy, 2=save )
  int            mode;
  int            ind;       // index of camera object connected with
  //HDC            hdc;       // view
  UINT*          ptr_dsp;   // pointer to image dsp
  BYTE*          ptr_buf;   // Pointer to buffer area memory (internal)
  //BYTE*          ptr_ext;   // Pointer to buffer area memory (external)
  BYTE*          ptr_img;   // Pointer to last buffer scaled to dsp
  FCObjImage     img_dsp;   // image dsp
  FCImageDrawDC* mem_dc;
//  HDC            img_dc;    // HDC oggetto FC
  RECT           area_dsp;
  SIZE           dim_dsp;
  SIZE           dim_buf;            // Dimensions of buf and dsp areas
  UINT           buf_size, dsp_size; // Size (in bytes) of buffers

  void draw_full( int x, int y, UINT col );

  void enable_full_dsp( int ind=0 );

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    FCObjImage     img_fulldsp;   // image dsp
    UINT*          ptr_fulldsp;   // pointer to image dsp
  #endif

  //CImage_display( POINT origin, SIZE dim, int mode );
  CImage_display( int ind_in );

  ~CImage_display(void);

  void ( CImage_display::*scale_img   )( BYTE* );           //function pointer
  void ( CImage_display::*draw_pt     )( int , int , int ); //function pointer
  void ( CImage_display::*draw_dash   )( int , int , int , int ); //function pointer
  void ( CImage_display::*draw_cross  )( int , int , int ); //function pointer
  void ( CImage_display::*draw_square )( int , int , int ); //function pointer
 // void ( CImage_display::*buf_to_dsp  )( POINT* );          //function pointer
 // void ( CImage_display::*dsp_to_buf  )( POINT* );          //function pointer
  void ( CImage_display::*buf2dsp )( int& , int& );         //function pointer
  void ( CImage_display::*dsp2buf )( int& , int& );         //function pointer
  int  ( CImage_display::*buf2ofs )( int  , int  );         //function pointer

  // ----------------- added for opencv mat dispaly
  void ( CImage_display::*outmat2dsp )( cv::Mat&  );
  //void ( CImage_display::*outmat2dsp )( cv::Mat& im_out );
  void		outmat2dsp_000 ( cv::Mat& im_out );
  void      outmat2dsp_090 ( cv::Mat& im_out );
  void      outmat2dsp_180 ( cv::Mat& im_out );
  void      outmat2dsp_270 ( cv::Mat& im_out );
  
  void      buf2Mat( BYTE* ptr_buf, cv::Mat img, int type =0 );// define a Mat from the buf data 

  //void      bayer2rgb( CGrabResultPtr ptr_gr );//color raw Bayer data to RGB data, discarded  
// ------------------added end by yang  2019/04/15   



  void      scale_fulldsp   ( BYTE* im_in );
  void      scale_grabresult( CGrabResultPtr ptr_gr );


  // Copy buffer into dsp
  void      scale_000  ( BYTE* im_in );
  void      scale_090  ( BYTE* im_in );
  void      scale_180  ( BYTE* im_in );
  void      scale_270  ( BYTE* im_in );
  void      scale_000c ( BYTE* im_in );
  void      scale_090c ( BYTE* im_in );
  void      scale_180c ( BYTE* im_in );
  void      scale_270c ( BYTE* im_in );
  void      scale_000vf( BYTE* im_in );  // no rotation vertical flip

  void      draw_pt_000 ( int x , int y , int col );
  void      draw_pt_090 ( int x , int y , int col );  
  void      draw_pt_180 ( int x , int y , int col );  
  void      draw_pt_270 ( int x , int y , int col );  
  void      draw_pt_000c( int x , int y , int col );
  void      draw_pt_090c( int x , int y , int col );
  void      draw_pt_180c( int x , int y , int col );
  void      draw_pt_270c( int x , int y , int col );

  void      draw_dash_000 ( int x , int y , int c1, int c2 );
  void      draw_dash_090 ( int x , int y , int c1, int c2 );
  void      draw_dash_180 ( int x , int y , int c1, int c2 );
  void      draw_dash_270 ( int x , int y , int c1, int c2 );  
  void      draw_dash_000c( int x , int y , int c1, int c2 );
  void      draw_dash_090c( int x , int y , int c1, int c2 );
  void      draw_dash_180c( int x , int y , int c1, int c2 );
  void      draw_dash_270c( int x , int y , int c1, int c2 );

  void      draw_cross_000 ( int x , int y , int col);
  void      draw_cross_090 ( int x , int y , int col);
  void      draw_cross_180 ( int x , int y , int col);
  void      draw_cross_270 ( int x , int y , int col);
  void      draw_cross_000c( int x , int y , int col);
  void      draw_cross_090c( int x , int y , int col);
  void      draw_cross_180c( int x , int y , int col);
  void      draw_cross_270c( int x , int y , int col);

  void      draw_square_000 ( int x , int y , int col);
  void      draw_square_090 ( int x , int y , int col);
  void      draw_square_180 ( int x , int y , int col);
  void      draw_square_270 ( int x , int y , int col);
  void      draw_square_000c( int x , int y , int col);
  void      draw_square_090c( int x , int y , int col);
  void      draw_square_180c( int x , int y , int col);
  void      draw_square_270c( int x , int y , int col);

/*  void      buf_to_dsp_000 ( POINT* pt );
  void      buf_to_dsp_090 ( POINT* pt );
  void      buf_to_dsp_180 ( POINT* pt );
  void      buf_to_dsp_270 ( POINT* pt );
  void      buf_to_dsp_000c( POINT* pt );
  void      buf_to_dsp_090c( POINT* pt );
  void      buf_to_dsp_180c( POINT* pt );
  void      buf_to_dsp_270c( POINT* pt );

  void      dsp_to_buf_000 ( POINT* pt );
  void      dsp_to_buf_090 ( POINT* pt );
  void      dsp_to_buf_180 ( POINT* pt );
  void      dsp_to_buf_270 ( POINT* pt );
  void      dsp_to_buf_000c( POINT* pt );
  void      dsp_to_buf_090c( POINT* pt );
  void      dsp_to_buf_180c( POINT* pt );
  void      dsp_to_buf_270c( POINT* pt );*/

  void      buf2dsp_000 ( int& , int& );
  void      buf2dsp_090 ( int& , int& );
  void      buf2dsp_180 ( int& , int& );
  void      buf2dsp_270 ( int& , int& );
  void      buf2dsp_000c( int& , int& );
  void      buf2dsp_090c( int& , int& );
  void      buf2dsp_180c( int& , int& );
  void      buf2dsp_270c( int& , int& );

  void      dsp2buf_000 ( int& , int& );
  void      dsp2buf_090 ( int& , int& );
  void      dsp2buf_180 ( int& , int& );
  void      dsp2buf_270 ( int& , int& );
  void      dsp2buf_000c( int& , int& );
  void      dsp2buf_090c( int& , int& );
  void      dsp2buf_180c( int& , int& );
  void      dsp2buf_270c( int& , int& );

  int       buf2ofs_000 ( int  , int  );
  int       buf2ofs_090 ( int  , int  );
  int       buf2ofs_180 ( int  , int  );
  int       buf2ofs_270 ( int  , int  );
  int       buf2ofs_000c( int  , int  );
  int       buf2ofs_090c( int  , int  );
  int       buf2ofs_180c( int  , int  );
  int       buf2ofs_270c( int  , int  );

  void      Save_images ( void );                                   // Save both BUF and DSP images to file with date/time name
  void      Save_images ( wchar_t* file_name, int type=PNG_FILE );  // Save both BUF and DSP images to file "file_name"
  void      Save_buf    ( wchar_t* file_name, int type=PNG_FILE );  // Save BUFFER bw image to file "file_name"
  void      Save_dsp    ( wchar_t* file_name, int type=PNG_FILE );  // Save DISPLAYED color image to file "file_name"
  void      Save_fulldsp( wchar_t* file_name, int type=PNG_FILE );  // Save fulldsp color image to file "file_name"

  //void      Save_image ( BYTE* buf, char* file_name, int type=PNG_FILE );    //Save buf to file "file_name"
//  BOOL      Create_csv ( int* ptr, int n_data, int pitch, char FileName[30]=NULL);
  
  int       Create_csv( wchar_t* file_name, int x0, int y0, int dx, int dy );
  int       Load_image( wchar_t* file_name, BOOL silent=FALSE );

  void      Draw_image( BOOL reset_alpha=FALSE );
  void      Draw_marker( int x, int y, COLORREF color=COL_RED );
  void      Draw_text ( wchar_t* text, int x, int y, COLORREF color );

protected:
  UINT          lut32_bw[256]; // BW Palette
  UINT          lut32_pc[256]; // Pseudocolor palette
  
//  void          dsp_to_buff ( POINT* pt );
//  void          buff_to_dsp ( POINT* pt );

  void          Palette( void );



};




/*



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

  #ifdef SAVE_FULL_PROC
    if (ptr_fulldsp) scale_fulldsp(im_in);
  #endif

}


void CImage_display::draw_pt_090( int x , int y , int col)
{
  ASSERT ( x>=0 && x<dim_buf.cx );
  ASSERT ( y>=0 && y<dim_buf.cy );
  ptr_dsp[dim_dsp.cx-1-((y*scale)>>10)+(dim_dsp.cy-1-((x*scale)>>10))*dim_dsp.cx] = col;

  #ifdef SAVE_FULL_PROC
    if ( ptr_fulldsp ) ptr_fulldsp[x+y*dim_buf.cx] = col;
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

  #ifdef SAVE_FULL_PROC
    draw_full(x,y,col);
  #endif

}




void CImage_display::buf2dsp_090( int& x, int& y )
{
  ASSERT( 0<=x && x<dim_buf.cx );
  ASSERT( 0<=y && y<dim_buf.cy );
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



void CImage_display::buf_to_dsp_090( POINT* pt )
{
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
 // pt->x = int( pt->x * camera[ind].scale );
 // pt->y = int( pt->y * camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}

void CImage_display::dsp_to_buf_090( POINT* pt )
{  
  ASSERT( 0<=pt->x && pt->x<dim_buf.cx );
  ASSERT( 0<=pt->y && pt->y<dim_buf.cy );
 // pt->x = int( pt->x / camera[ind].scale );
 // pt->y = int( pt->y / camera[ind].scale );
  ASSERT( 0<=pt->x && pt->x<dim_dsp.cx );
  ASSERT( 0<=pt->y && pt->y<dim_dsp.cy );
}





*/