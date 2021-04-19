#include "StdAfx.h"
#include "IVS.h"
#include "Globals.h"
#include "Container_main.h"
#include "Info_box.h"


IMPLEMENT_DYNAMIC( CContainer_main, CDialog )


//extern char PC_name[100]; // Computer name read from system configuration
extern wchar_t PC_name[30]; // Computer name read from system configuration

extern BOOL                debug;

//extern CPassword_manager*  password_mngr;

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif



#ifdef CFR21
  extern char User_str[100]; 
#endif



BEGIN_MESSAGE_MAP(CContainer_main, CContainer_wnd)
	ON_BN_CLICKED ( IDD_LOGO_BTN, &CContainer_main::OnBnClickedLogoBtn )
	//ON_STN_CLICKED( IDD_LOGO_BTN, &CContainer_main::OnBnClickedLogoBtn )
  //ON_WM_COMMAND(
	//ON_BN_CLICKED( IDD_USER_BTN, &CContainer_main::OnBnClickedUserBtn )
	//ON_CONTROL_REFLECT( STN_CLICKED, &CContainer_main::OnBnClickedUserBtn)
END_MESSAGE_MAP()


/*
// CStatic_mod message handlers
BEGIN_MESSAGE_MAP(CContainer_main, CDialog)
	//ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
*/


//CContainer_main::CContainer_main( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER*/ ) : CContainer_wnd( text_in, rett_in, parent_in, templatename/*=IDD_CONTAINER*/ )
CContainer_main::CContainer_main( wchar_t* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER*/ ) : CContainer_wnd( text_in, rett_in, parent_in, templatename/*=IDD_CONTAINER*/ )
{
/*
	// CARICO E VISUALIZZO IL LOGO
	char* img_name = "LOGO_330x64.png" ;
  if (!PathFileExists(img_name))
  {
		msg_err.Format("ERRORE: File <%s> non trovato", img_name);
    Message_box( msg_err, MBOX_ERROR );
  }
	else logo.Load( img_name );
*/

	CRect logo_rect( 6, 6, 10+100+10, 10+42+10 );

	logo_button = new CButton;
	logo_button->Create( L"LOGO BUTTON", WS_CHILD|WS_VISIBLE|BS_BITMAP, logo_rect, this, IDD_LOGO_BTN );
	HBITMAP  hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOGO_SMALL));
	if ( hbmp ) logo_button->SetBitmap(hbmp);

  logo_rect.left   =                      5;
  logo_rect.right  = logo_rect.left   + 140;
  logo_rect.top    = logo_rect.bottom +   4;
	logo_rect.bottom = logo_rect.top    +  16;

//  CFont font;
//	VERIFY( font.CreatePointFont( 120, L"DejaVu Sans" ) );

  font.CreateFont( 15,                            // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                             // Orientation
                   FW_SEMIBOLD,                   // Weight
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );


  date_time = new CStatic;
	VERIFY( date_time->Create( L"...", WS_CHILD|WS_VISIBLE|SS_LEFT, logo_rect, this ) );
  date_time->SetFont( &font );

	logo_rect.top    = logo_rect.bottom +  4;
	logo_rect.bottom = logo_rect.top    + 16;

	PCname_st = new CStatic;
	VERIFY( PCname_st->Create( PC_name, WS_CHILD|WS_VISIBLE|SS_LEFT, logo_rect, this ) );
  PCname_st->SetFont( &font );

  // Add logged-in user field
  logo_rect.top    = logo_rect.bottom +  4;
	logo_rect.bottom = logo_rect.top    + 16;
	User_st = new CStatic;
  VERIFY( User_st->Create( L"no user", SS_NOTIFY|WS_CHILD|WS_VISIBLE|SS_LEFT, logo_rect, this, IDD_USER_BTN ) );
  User_st->SetFont( &font );

  // Add current recipe field
  logo_rect.top    = logo_rect.bottom +  4;
	logo_rect.bottom = logo_rect.top    + 16;
	Recipe_st = new CStatic;
  VERIFY( Recipe_st->Create( L"default", SS_NOTIFY|WS_CHILD|WS_VISIBLE|SS_LEFT, logo_rect, this, IDD_USER_BTN ) );
  Recipe_st->SetFont( &font );


  font.DeleteObject();
}

CContainer_main::~CContainer_main(void)
{
	if ( logo_button ) delete logo_button;
	if ( date_time   ) delete date_time;
	if ( PCname_st   ) delete PCname_st;
  if ( User_st     ) delete User_st;
  if ( Recipe_st   ) delete Recipe_st;
}





void CContainer_main::OnBnClickedLogoBtn()
{
	CInfo_box info_box;
	info_box.DoModal();
}


/*
void CContainer_main::OnBnClickedUserBtn()
{
	TRACE("Logout required\n");
}
*/



BOOL CContainer_main::OnCommand(WPARAM wParam, LPARAM lParam)
{
  // TODO: Add your specialized code here and/or call the base class
  if ( wParam==IDD_USER_BTN && password_mngr ) password_mngr->logout(1);
 
  return CContainer_wnd::OnCommand(wParam, lParam);
}
