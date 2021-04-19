// IVS.cpp : Defines the class behaviors for the application.
//

// define this before including any afx files in stdafx.h
//#define _AFX_SECURE_NO_WARNINGS
//#define _ATL_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS


#include "stdafx.h"
#include "IVS.h"
#include "MainFrm.h"
#include "Globals.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIVS

BEGIN_MESSAGE_MAP(CIVS, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CIVS::OnAppAbout)
END_MESSAGE_MAP()


// CIVS construction

CIVS::CIVS()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIVS object

CIVS theApp;


// CIVS initialization

BOOL CIVS::InitInstance()
{
	CWinApp::InitInstance();

	/*
  #ifdef BASLER_GIGE
	  AfxEnableMemoryTracking ( false );
  #endif
*/
	/*
	//Pour added for log file on 20200506
		if ( !SetCurrentDirectory( LOG_DIRECTORY ) )
	{
		if ( !CreateDirectory( LOG_DIRECTORY, NULL ) )
		{
      Message_box( L"Unable to access/create BATCH_DIRECTORY directory", MBOX_ERROR );
			ASSERT( 0 );
		}
    SetCurrentDirectory( LOG_DIRECTORY );
	}
	//Pour added for log file on 20200506
	*/

	if ( !SetCurrentDirectory( BATCH_DIRECTORY ) )
	{
		if ( !CreateDirectory( BATCH_DIRECTORY, NULL ) )
		{
      Message_box( L"Unable to access/create BATCH_DIRECTORY directory", MBOX_ERROR );
			ASSERT( 0 );
		}
    SetCurrentDirectory( BATCH_DIRECTORY );
	}

	if ( !SetCurrentDirectory( IMAGES_DIRECTORY ) )
	{
		if ( !CreateDirectory( IMAGES_DIRECTORY, NULL ) )
		{
      Message_box( L"Unable to access/create IMAGES_DIRECTORY directory", MBOX_ERROR );
			ASSERT( 0 );
		}
    SetCurrentDirectory( IMAGES_DIRECTORY );
	}

	if ( !SetCurrentDirectory( RECIPE_DIRECTORY ) )
	{
		if ( !CreateDirectory( RECIPE_DIRECTORY, NULL ) )
		{
      Message_box( L"Unable to access/create RECIPE_DIRECTORY directory", MBOX_ERROR );
			ASSERT( 0 );
		}
    SetCurrentDirectory( RECIPE_DIRECTORY );
	}

  // KEEP AS LAST ONE!
	if ( !SetCurrentDirectory( MAIN_DIRECTORY ) )
	{
		if ( !CreateDirectory( MAIN_DIRECTORY, NULL ) )
		{
      Message_box( L"Unable to access/create MAIN_DIRECTORY directory", MBOX_ERROR );
			ASSERT( 0 );
		}
    SetCurrentDirectory( MAIN_DIRECTORY );
	}




	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame) return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
//	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

//	pFrame->LoadFrame(IDR_MAINFRAME, WS_POPUP, NULL, NULL);
	//pFrame->Create( "Visiquick", "Visiquick" , /*WS_EX_CLIENTEDGE*/ WS_EX_WINDOWEDGE   );
	pFrame->Create( L"TIVS", L"TIVS" , /*WS_EX_CLIENTEDGE*/ WS_EX_WINDOWEDGE   );


	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();


//	PostMessage( pFrame->m_hWnd, WM_USER+1, 123, 456 );
//	pFrame->Initialize();

	//pView->Initialize();


//	TRACE( "Commandline: <%s>\n", m_lpCmdLine );
	TRACE( L"Commandline: <%s>\n", m_lpCmdLine );


	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CIVS message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CIVS::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CIVS message handlers

