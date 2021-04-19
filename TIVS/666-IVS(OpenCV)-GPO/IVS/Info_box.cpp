// Info_box.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Info_box.h"
#include "Def_globali.h"
#include "strings.h"
#include "Globals.h"


// CInfo_box dialog

extern UINT             lng;

IMPLEMENT_DYNAMIC(CInfo_box, CDialog)

CInfo_box::CInfo_box(CWnd* pParent /*=NULL*/) : CDialog(CInfo_box::IDD, pParent)
{
	// CARICO E VISUALIZZO IL LOGO
/*	char* img_name = "LOGO_330x64.png" ;
  if (!PathFileExists(img_name))
  {
		msg_err.Format("ERRORE: File <%s> non trovato", img_name);
    Message_box( msg_err, MBOX_ERROR );
  }
	else logo.Load( img_name ); */


//	logo_button = new CButton;
//	logo_button->Create( "LOGO BUTTON", WS_CHILD|WS_VISIBLE|BS_BITMAP, logo_rect, this, IDD_LOGO_BTN );
	hbmp_logo = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOGO_BIG));

 

}

CInfo_box::~CInfo_box()
{
}

void CInfo_box::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfo_box, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInfo_box message handlers

void CInfo_box::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rect;
	GetClientRect( &rect );

  CBrush brush;   // Must initialize!
  brush.CreateSolidBrush(RGB(255, 255, 255)); 


  CWnd* close_btn = GetDlgItem( IDOK );
  if ( close_btn && close_btn->m_hWnd ) close_btn->SetWindowTextW( string_432[lng] );


  dc.FillRect( rect, &brush );

  CBitmap bmp;
	VERIFY( bmp.LoadBitmap( IDB_LOGO_BIG ) );

  BITMAP bmpInfo;
  bmp.GetBitmap(&bmpInfo);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);


	CBitmap* oldbmp = dcMemory.SelectObject(&bmp);

//	dc.BitBlt( 10, 10, bmp.GetBitmapDimension().cx, bmp.GetBitmapDimension().cy, &dcMemory, 0, 0, SRCCOPY );
	dc.BitBlt( 10, 10, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY );

//	dcMemory.SelectObject(oldbmp);

  dcMemory.SelectObject( oldbmp );

  bmp.DeleteObject();

	//VERIFY( bmp.LoadBitmap( IDB_VISIQUICK_LOGO ) );
	//oldbmp = dcMemory.SelectObject(&bmp);
  //dc.BitBlt( 400, 20, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY );
  //dc.SetBkColor( COLORE_CONTAIN );
  //dc.SetTextAlign( TA_CENTER );
	dc.SetTextAlign( TA_LEFT );

	int xpos =  20;
	int ypos = 120;
	int dy   =   8;






	/*font.CreatePointFont( 160, "Verdana", &dc );
	dc.SelectObject( &font );
	dc.TextOut( xpos, ypos, "VISIQUICK" );
	font.DeleteObject(); */

	//font.CreatePointFont( 130, "Arial", &dc );
//	font.CreatePointFont( 130, L"Arial", &dc );

//  font.DeleteObject();

  font.CreateFont( 23,                            // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                            // Orientation
                   500,                           // Weight
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );


//	dc.SelectObject( &font );
	CFont* old_font = dc.SelectObject( &font );


	//CSize dim_txt = dc.GetTextExtent("QWERTYUIOP");
	CSize dim_txt = dc.GetTextExtent(L"QWERTYUIOP");

  int nrows = sizeof(info_text_eng)/sizeof(wchar_t*);
  ASSERT( sizeof(info_text_chn)/sizeof(wchar_t*)==nrows ); // check same size
  TRACE("CInfo_box: nrows=%d\n", nrows );

  CString txt;

  txt.Format( info_text_eng[0], MODEL_NAME, CString(MACHINE_ID) );

  if ( lng==CHINESE ) txt.Format( info_text_chn[0], MODEL_NAME, CString(MACHINE_ID) );
  if ( lng==SPANISH ) txt.Format( info_text_esp[0], MODEL_NAME, CString(MACHINE_ID) );

  // Prepare string with machine model and number
  //if ( lng==CHINESE ) txt.Format( info_text_chn[0], MODEL_NAME, CString(MACHINE_ID) );
  //else                txt.Format( info_text_eng[0], MODEL_NAME, CString(MACHINE_ID) );
  dc.TextOut( xpos, ypos, txt );
  ypos += dim_txt.cy + dy;

  txt.Format( info_text_eng[1], VERSION_PC );
  if ( lng==CHINESE ) txt.Format( info_text_chn[1], VERSION_PC );
  if ( lng==SPANISH ) txt.Format( info_text_esp[1], VERSION_PC );

  // Read and prepare string with inspection version ( macro VERSION_PC )
  //if ( lng==CHINESE ) txt.Format( info_text_chn[1], VERSION_PC );
  //else                txt.Format( info_text_eng[1], VERSION_PC );
  dc.TextOut( xpos, ypos, txt );
  ypos += dim_txt.cy + dy;

  // Read from PLC and prepare string with PLC ( string .SW_DESCRIPTION )
  ADS_data_type PLC_version;
  if ( !ADS_init_parameter( ".SW_VERSION", &PLC_version, 10 ) )
  {
    txt.Format( info_text_eng[2], AtoW((char*)PLC_version.val) );
    if ( lng==CHINESE ) txt.Format( info_text_chn[2], AtoW((char*)PLC_version.val) );
    if ( lng==SPANISH ) txt.Format( info_text_esp[2], AtoW((char*)PLC_version.val) );

    //if ( lng==CHINESE ) txt.Format( info_text_chn[2], AtoW((char*)PLC_version.val) );
    //else                txt.Format( info_text_eng[2], AtoW((char*)PLC_version.val) );
  }
  else txt.Format( info_text_eng[2], L"unable to read from PLC" );
  dc.TextOut( xpos, ypos, txt );
  ypos += dim_txt.cy + dy;
  ADS_clean_parameter( &PLC_version );

  // Next items only text
  for ( int i=3 ; i<nrows ; i++ ) 
  {
    dc.TextOut( xpos, ypos, info_text_eng[i] );
    if ( lng==CHINESE ) dc.TextOut( xpos, ypos, info_text_chn[i] );
    if ( lng==SPANISH ) dc.TextOut( xpos, ypos, info_text_esp[i] );

  	//if ( lng==CHINESE ) dc.TextOut( xpos, ypos, info_text_chn[i] );
    //else                dc.TextOut( xpos, ypos, info_text_eng[i] );
	  ypos += dim_txt.cy + dy;
  }
  
 
  dc.SelectObject( old_font );

	font.DeleteObject();

}
