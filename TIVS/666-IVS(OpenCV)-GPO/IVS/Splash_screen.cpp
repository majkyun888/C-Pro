// Splash_screen.cpp : file di implementazione
//

#include "stdafx.h"
#include "Splash_screen.h"
#include "afxdialogex.h"


// finestra di dialogo CSplash_screen

IMPLEMENT_DYNAMIC(CSplash_screen, CDialog)

CSplash_screen::CSplash_screen(CWnd* pParent /*=NULL*/)
	: CDialog(CSplash_screen::IDD, pParent)
{

}

CSplash_screen::~CSplash_screen()
{
}

void CSplash_screen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplash_screen, CDialog)
  ON_WM_CREATE()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// gestori di messaggi CSplash_screen


int CSplash_screen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;


  CRect rect;

  GetClientRect( &rect );

  rect.bottom -= 70;

  // Create a child bitmap static control.
  myStatic.Create(_T("my static"), WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE, rect, this);

  HINSTANCE hinst = AfxGetInstanceHandle();

  bitmap = ::LoadBitmap(hinst, MAKEINTRESOURCE(IDB_SPLASH));

  // Set the bitmap of the static control to be the 
  // system check-mark bitmap.
  myStatic.SetBitmap( bitmap );



  return 0;
}


void CSplash_screen::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);
  GetDlgItem( IDC_TEXT_1 )->SetWindowText( L"Initializing..." );
  GetDlgItem( IDC_TEXT_2 )->SetWindowText( L"" );
}
