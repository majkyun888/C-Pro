// Image_layout.cpp : Class to Draw Images
#include "StdAfx.h"
#include "Image_layout.h"


extern  UINT         *lut32;              // Pointer to LUT (either BW or COLOR)
extern  UINT          lng;
extern  Camera_struct camera[NUM_TLC];

#define DRW_PT(x,y,c) ptr_dsp[(x)+(y)*dim_dsp.cx] = c;





CImage_layout::CImage_layout( wchar_t* file_name )
{

  ASSERT( file_name && AfxIsValidString(file_name) );


  wchar_t msg[100+1];
  if ( PathFileExists( file_name ) )
  { 
    if ( !img_dsp.Load( file_name ) )
    {
      swprintf( msg, 100, L"Could not open layout file <%s>\r\nError n.%d", file_name, GetLastError());
      Message_box( msg, MBOX_ERROR);
      return;
    }
  }


  // COORDINATES FOR LQ LAYOUT
  area_dsp.top    = 185;

    #ifdef TIVS_S40
    area_dsp.top    = 165;
  #endif

	#ifdef TIVS_S60
    area_dsp.top    = 205;
  #endif

  area_dsp.bottom = area_dsp.top  + img_dsp.Height();
  area_dsp.left   = (APP_RESX-10-img_dsp.Width())/2;//+ 180; // - 100;
  area_dsp.right  = area_dsp.left + img_dsp.Width();

  // COORDINATES FOR FD LAYOUT
  if ( !wcscmp( file_name, L"layout_fd.png" ) )
  {
    area_dsp.left   = (APP_RESX-10-img_dsp.Width())/2 + 40;
    area_dsp.right  = area_dsp.left + img_dsp.Width();
  }

  dsp_size = dim_dsp.cx*dim_dsp.cy*4;

  // Create update items
/*
  RECT area;

  area.left   =              10;
  area.right  = area.left + 200;
  area.top    =              30;
  area.bottom = area.top  + 200;

  uplist[0].item_rt = new CRiga_testo( TEXT_ROWS_FONT, area, pView, SS_CENTER|SS_CENTERIMAGE );
  uplist[0].item_rt->SetText( L"primo elemento", TXT_RED );
  */
}




CImage_layout::~CImage_layout(void)
{
/*
  for ( UINT i=0 ; i<uplist_num ; i++ )
  {
    if ( uplist[0].item_rt )
    {
      delete uplist[0].item_rt;
      uplist[0].item_rt = NULL;
    }

  }
  */

/*  if ( mem_dc )
  {
    //mem_dc->reset();
    delete mem_dc;
    mem_dc = NULL;
  }  */


//  delete img_dsp;
//  img_dsp.Destroy();
//  ptr_dsp = NULL;

//  if ( ptr_buf ) free( ptr_buf );
//  ptr_buf = NULL;
}




int CImage_layout::Load_image ( wchar_t* file_name, BOOL silent  )
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
  //img_tmp.Load ( newstr );
  img_tmp.Load ( file_name );

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

  return 0; // caricamento ok
}




void CImage_layout::Draw_image( BOOL reset_alpha /*=FALSE*/ )
{
//  ASSERT( hdc );
  hdc = GetDC( pView->m_hWnd );
  ASSERT( hdc );

  img_dsp.Draw( hdc, area_dsp ); // Update image displaying

  ReleaseDC( pView->m_hWnd, hdc );
}


void CImage_layout::Draw_info( HDC hdc )
{

  //CDC dc( CPaintDC );

  hdc = GetDC( pView->m_hWnd ); 

//  CDC dc; //( pView );
//  dc.GetSafeHdc();
//  CPaintDC dc( pView->m_hWnd );
//  CPaintDC dc( CWnd::FromHandle(pView->m_hWnd) );  //   (pView);
//  GetSafeDC()
//  ASSERT( hdc );
//  CFont font;
//  VERIFY( font.CreatePointFont(120, L"Arial", &dc ) );
/*  dc.SelectObject( &font );
  dc.SetBkColor( BKGRND_COL );
  dc.SetTextAlign( TA_LEFT );
  dc.SetTextColor( 0 );*/

  HFONT hFont;
  //hdc = BeginPaint(hWnd, &ps);
  
              
  //Logical units are device dependent pixels, so this will create a handle to a logical font that is 48 pixels in height.
  //The width, when set to 0, will cause the font mapper to choose the closest matching value.
  //The font face name will be Impact.
  hFont = CreateFont( 35,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, FIXED_PITCH,TEXT("Arial"));
  
  //SelectObject(hdc, hFont);
  HGDIOBJ old_font = SelectObject(hdc, hFont);
              
//  SelectObject( hdc, (HFONT) GetStockObject(ANSI_FIXED_FONT) );
//  SetBkColor( hdc, BKGRND_COL ); // RGB(200,200,200)
  SetBkColor( hdc, ::GetSysColor( COLOR_APPWORKSPACE ) ); // COLOR_WINDOW ) ); // RGB(200,200,200)
  SetTextColor( hdc, 0   );
  
  int y0 =   700;
  int xl =   810;
  int xr =  1200;

  #ifdef TIVS_F40
    xl =       20;
    xr = xl + 390;
  #endif

	#ifdef TIVS_F20
    xl =       20;
    xr = xl + 390;
  #endif


  wchar_t text[100+1];

//  static int counter = 10000;
  int value = 0;

  if ( ADS_status==1 )
  {
    int y = y0;
    SetTextAlign( hdc, TA_LEFT );
  
    swprintf( text, 100, string_250[lng] );
    TextOut( hdc, xl, y, text, wcslen(text) );
    y += 40;

    swprintf( text, 100, string_200[lng] );
    TextOut( hdc, xl, y, text, wcslen(text) );
    y += 40;

    swprintf( text, 100, string_210[lng] );
    TextOut( hdc, xl, y, text, wcslen(text) );
    y += 40;

    y = y0;
    SetTextAlign( hdc, TA_RIGHT );

    ADS_read_parameter( ".PRODSPEED" , &value );
    swprintf( text, 100, L"%d", value&0xFFFF );
    TextOut( hdc, xr, y, text, wcslen(text) );
    y += 40;

    ADS_read_parameter( ".NUM_PROD_IN" , &value );
    swprintf( text, 100, L"%s", thousands(value) );
    TextOut( hdc, xr, y, text, wcslen(text) );
    y += 40;

    ADS_read_parameter( ".NUM_REJECTS" , &value );
    swprintf( text, 100, L"%s", thousands(value) );
    TextOut( hdc, xr, y, text, wcslen(text) );
    y += 40;
  }

  SelectObject( hdc, old_font );

  ReleaseDC( pView->m_hWnd, hdc ); 

}



// Altezza font predefinito = 16
// color: 0x00BBGGRR (lasciare alpha=00)

void CImage_layout::Draw_text( wchar_t* text, int x, int y, COLORREF color )
{
  ASSERT( hdc );
  #define FONT_H 14
  std::auto_ptr<FCImageDrawDC>   mem_dc(new FCImageDrawDC(img_dsp)) ;

  HDC dc = *mem_dc;

//  SelectObject( dc, (HFONT) GetStockObject(ANSI_FIXED_FONT) );
  HGDIOBJ old_font = SelectObject( dc, (HFONT) GetStockObject(ANSI_FIXED_FONT) );
  SetTextAlign( dc, TA_LEFT );
  SetTextColor( dc, color   ) ;

  TextOut( dc, x, y-FONT_H/2, text, wcslen(text) );

  SelectObject( dc, old_font );
//  ReleaseDC( img_dsp, mem_dc );
}


//  //Pour added for status displaying on 20201020
//
//void CImage_layout::Draw_image_status(BOOL reset_alpha /*=FALSE*/)
//{
//	  hdc = GetDC( pView->m_hWnd ); 
//
//}
//
//  //Pour added for status displaying on 20201020






