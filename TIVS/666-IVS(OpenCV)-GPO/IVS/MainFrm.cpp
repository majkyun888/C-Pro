// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "IVS.h"
#include "MainFrm.h"
#include "Globals.h"
#include "Shield_access2OS.h"//Pour added to disable windows access on 20200422

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
//	ON_WM_SETFOCUS()
//	ON_WM_ENTERIDLE()
//ON_WM_SHOWWINDOW()
//ON_WM_WINDOWPOSCHANGED()
ON_WM_KEYDOWN()
ON_WM_CHAR()
END_MESSAGE_MAP()

/*
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
*/
/*
static UINT indicators[] =
{
	IDS_STATUSBAR_0,
	IDS_STATUSBAR_1,
	IDS_STATUSBAR_2,
	IDS_STATUSBAR_3,
	IDS_STATUSBAR_4,
	IDS_STATUSBAR_5,
	IDS_STATUSBAR_6,
	IDS_STATUSBAR_7,
	IDS_STATUSBAR_8,
	IDS_STATUSBAR_9
};
*/


CRect rectFull;

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

/*
  // Calculates span of display area
  HDC hDC = ::GetDC(NULL);
  int XBorder    = GetSystemMetrics(SM_CXFRAME);
  int YBorder    = GetSystemMetrics(SM_CYFRAME);
  ::ReleaseDC(NULL, hDC);

  // Calculates size of window elements
  int YCaption   = GetSystemMetrics(SM_CYCAPTION);
  int YMenu      = GetSystemMetrics(SM_CYMENU);
  int YBorder    = GetSystemMetrics(SM_CYFRAME);
	
	
	int YStatusBar = 15;
	YBorder = 0;

  // Calculates window origin and span for full screen mode
//  CPoint Origin = CPoint(-XBorder, -YBorder - YCaption - YMenu);
	CPoint Origin;
	Origin.x =  -5;
	Origin.y = -23;

//  XSpan += 2 * XBorder, YSpan += 2 * YBorder + YMenu + YCaption + YStatusBar;

//	XSpan = APP_RESX + 9;
//	YSpan = APP_RESY + 48;


  // Calculates full screen window rectangle
  rectFull = CRect(Origin, CSize(XSpan, YSpan));
*/


}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	// create a view to occupy the client area of the frame

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		//TRACE0("Failed to create view window\n");
		//T/RACE0("Failed to create view window\n");
		TRACE( L"Failed to create view window\n" );
		return -1;
	}




/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/

/*
	// IMPOSTAZIONI STATUSBAR
	if ( !m_wndStatusBar.Create(this) )
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
  
	if ( !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)) )
	{
		TRACE0("Unable to set indicators of status bar\n");
		return -1;      // fail to create
	}


	for ( int i=0 ; i<10 ; i++ )
	{
		char text[30];
		sprintf( text, "sb%d", i );
		m_wndStatusBar.SetPaneInfo( i, IDS_STATUSBAR_0+i, i?SBPS_NORMAL:SBPS_STRETCH, 100 );
	  m_wndStatusBar.SetPaneText( i, text );
	}

	statusbar_hwnd = m_wndStatusBar.m_hWnd;
*/


/*	if ( !m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
  // TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar); */

/*
	WINDOWPLACEMENT wpos;
	GetWindowPlacement( &wpos );
	wpos.rcNormalPosition.top    =       -23;
	wpos.rcNormalPosition.left   =        -4;
	wpos.rcNormalPosition.bottom = 1050 + 40;
	wpos.rcNormalPosition.right  = 1680 + 10; 
	SetWindowPlacement( &wpos );
*/

/*
	wpos.flags = SW_SHOW | SW_SHOWMAXIMIZED;
	wpos.flags = SW_SHOWMAXIMIZED;
*/


/*
	rectFull.left    =  -5;
	rectFull.top     = -23;
	rectFull.right   = APP_RESX;
	rectFull.bottom  = APP_RESY + 60;
  MoveWindow( rectFull );
*/

	m_wndView.PostMessage( WM_INIT, 123, 456 );

	//SetWindowPos(&CWnd::wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);


  SetFocus();

	return 0;
}




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
/*
  #define ACE_TSFREQ 125000000 // BASLER ACE, READ FROM VIEWER
  unsigned __int64 time1=0, time2=0;
  UINT    t1, t2;
  time1 = 412000000000;
  time2 = time1 + 123456;
  t1 = time1 & 0xFFFFFFFF;
  t2 = time2 & 0xFFFFFFFF;
  int diff = t2-t1;
  double diff_sec = double(diff)/ACE_TSFREQ;
  TRACE("%d - %f\n", 1000, 1000.1234 );
  TRACE("ACQ_OK: TimeStamp=%d - Elapsed=%d (%6.5f sec)\n", int(time2), diff, diff_sec ); 
  time1 = time2;
*/

	//added by Pour on 20200422 to shield access to windows 
	//===================================
	// mask buttons: ctrl,win, alt,tab 
    DWORD dwVK[] = { VK_CONTROL,VK_LWIN,VK_RWIN,VK_MENU,VK_TAB };

    int nLength = sizeof(dwVK) / sizeof(DWORD);

    StartMaskKey(dwVK, nLength);    
	//  disable mask manager 
	
	DWORD value = 1;
    if ( WriteReg(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), _T("DisableTaskMgr"),value))
		//Message_box( _T("Windows access disabled"), MBOX_ERROR);//Pour removed on 20200502
		 //Message_box( _T("Failed to disable Task manager"), MBOX_ERROR);
	//====================================



//#ifndef _DEBUG

	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

  #ifdef MONITOR_SEC
	  cs.x  =  1920;
  #else
    cs.x  =     0;
  #endif
	cs.y  =     0;
	cs.cx = APP_RESX;
	cs.cy = APP_RESY;


	cs.style  =  WS_POPUP; 
// Finestra Ok, senza bordo e titolo ma con problemi di mancata visualizzazione delle msgbox



//	cs.style = WS_OVERLAPPED;


//cs.style  =  WS_CHILD; 
//cs.style &= ~WS_BORDER;
//cs.style &= ~WS_CAPTION;
//cs.style &= ~WS_SYSMENU;

//	cs.style |=  WS_CLIPCHILDREN; 

//	cs.style  =  WS_DLGFRAME; 
//	cs.style |=  WS_VISIBLE;
//  cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//  cs.dwExStyle |=  WS_EX_STATICEDGE;

	cs.lpszClass = AfxRegisterWndClass(0);
//#endif

//  CRect rett;
//	m_wndView.GetWindowRect( &rett );
//	rett.OffsetRect( -OFFS_COORDX , -OFFS_COORDY ); // necessaria per evitare l'offset della cornice
//	m_wndView.MoveWindow( &rett );
//	m_wndView.GetWindowRect( &rett ); 

//	GetWindowRect( &rett );
//	rett.OffsetRect( -OFFS_COORDX , -OFFS_COORDY ); // necessaria per evitare l'offset della cornice
//	MoveWindow( &rett );
//	GetWindowRect( &rett ); 

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

//void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
//{
//	// forward focus to the view window
//	m_wndView.SetFocus();
//
//
///*	RedrawWindow();
//	Beep( 1000 , 100 );
//	Sleep( 2000 );
//	Beep( 1000 , 100 ); */
//}


/*
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command

/*	if ( nID!=0x65 && 
		   nID!=0x66 && 
			 nID!=0x67 && 
			 nID!=0x68 && 
			 nID!=0x69 &&
			 nID!=0x81 &&
			 nID!=0x82 &&
			 nID!=0x83 &&
			 nID!=0x84 
			 
			 )
	{
	  TRACE( "Msg %6X - Code %6d - m_wndView.IsVisible %d\n", nID, nCode, m_wndView.IsWindowVisible() );
	}  */

/*
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))	return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
*/

/*
int CMainFrame::Initialize(void)
{
	pView      = &m_wndView;
	pStatusBar = &m_wndStatusBar;
	pToolBar   = &m_wndToolBar;
	return 0;
}
*/
//void CMainFrame::OnEnterIdle(UINT nWhy, CWnd* pWho)
//{
//	CFrameWnd::OnEnterIdle(nWhy, pWho);
//
//		//pView->Initialize();
//
//	Beep( 1000 , 100 );
//	// TODO: Add your message handler code here
//}

//void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CFrameWnd::OnShowWindow(bShow, nStatus);
//
///*
//	static bool first_time = true;
//	if ( first_time && IsWindowVisible() )
//	{
//		first_time = false;
//
//		RedrawWindow();
//
//		counter = 0;
//
//		Beep( 1000 , 100 );
//		Sleep( 1000 );
//
//	}
//	//	Beep( 1000 , 100 );
//*/
//
//
//	// TODO: Add your message handler code here
//}

//void CMainFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
//{
//	CFrameWnd::OnWindowPosChanged(lpwndpos);
//
//	// TODO: Add your message handler code here
//	TRACE("CMainFrame::OnWindowPosChanged %d,%d,%d,%d %d\n", lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags );
//}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("CMainFrame::OnKeyDown: Premuto %d - nRep=%d, nFlags=%d\n", nChar, nRepCnt, nFlags );
	TRACE( L"CMainFrame::OnKeyDown: Pushed=%d - nRep=%d, nFlags=%d\n", nChar, nRepCnt, nFlags );

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("CMainFrame::OnChar: Premuto %d - nRep=%d, nFlags=%d\n", nChar, nRepCnt, nFlags );
	TRACE("CMainFrame::OnChar: Pusched=%d - nRep=%d, nFlags=%d\n", nChar, nRepCnt, nFlags );

	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);
}
