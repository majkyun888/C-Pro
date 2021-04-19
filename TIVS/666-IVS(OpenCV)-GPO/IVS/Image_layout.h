// Image_layout.h : class to Draw Images

#pragma once
#include "afx.h"
#include "ImageStone.h"
#include "Globals.h"


//#define   INTBUFFER        64 // Allocate internal buffer for saving

/*
#define   NORMAL          128 // Transform to RGB
#define   SCALE           256 // scale 4:1
#define   SCALE_CW        512 // scale 4:1 and rotate clockwise
#define   SCALE_CCW      1024 // scale 4:1 and rotate counterclockwise
*/

#define   DISPROT_000       2 // no rotation acquisition-display
#define   DISPROT_090       4 // acquired image is rotate  90 CW before display
#define   DISPROT_180       8 // acquired image is rotate 180 CW before display
#define   DISPROT_270      16 // acquired image is rotate 270 CW before display

//#define   FLIP_X         2048 // flip horizontal
//#define   FLIP_Y         4096 // flip vertical

#define   PNG_FILE       8192
#define   JPG_FILE      16384
#define   BMP_FILE      32768


#define   MAX_LENGTH_STR   50


class CImage_layout 
{

  //POINT origin_dsp;         // Display origin coordinates
  //UINT  scale;              // reduction scaling integer factor (/1024)
  //UINT  multi;              // amplification scaling integer factor (/1024)

public:

  BOOL           visible;
  //BOOL           save;      // request to save images to disk
  //int            mode;
  //int            ind;       // index of camera object connected with
  HDC            hdc;       // view
  //int*           ptr_dsp;   // pointer to image dsp
  BYTE*          ptr_buf;   // Pointer to buffer area memory (internal)
  //BYTE*          ptr_ext;   // Pointer to buffer area memory (external)
  //BYTE*          ptr_img;   // Pointer to last buffer scaled to dsp
  FCObjImage     img_dsp;   // image dsp
  FCImageDrawDC* mem_dc;
  HDC            img_dc;    // HDC oggetto FC
  RECT           area_dsp;
  SIZE           dim_dsp;
  UINT           dsp_size; // Size (in bytes) of buffers
  SIZE           dim_buf;            // Dimensions of buf and dsp areas
  //UINT           buf_size, dsp_size; // Size (in bytes) of buffers

  //CImage_layout( POINT origin, SIZE dim, int mode );
  CImage_layout( wchar_t* file_name );  //( int ind_in );

  ~CImage_layout(void);

  int       Load_image( wchar_t* file_name, BOOL silent=FALSE );

  void      Draw_image( BOOL reset_alpha=FALSE );
 // void      Draw_marker( int x, int y, COLORREF color=COL_RED );

  void      Draw_text( wchar_t* text, int x, int y, COLORREF color );
  void      Draw_info( HDC hdc );

  //Pour added for status displaying on 20201020
  //void      Draw_status(CDC* pDC);
  //void      Draw_image_status( BOOL reset_alpha=FALSE );
  //Pour added for status displaying on 20201020

protected:




};