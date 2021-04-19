// Input_string.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "Input_string.h"
#include "strings.h"


// CInput_string dialog

BOOL CInput_string::maiuscolo = FALSE;

extern UINT           lng;  // Lingua attuale


IMPLEMENT_DYNAMIC(CInput_string, CDialog)

//CInput_string::CInput_string(CWnd* pParent /*=NULL*/)
//CInput_string::CInput_string( CWnd* pParent, char* initial, UINT lenght, UINT mode )	: CDialog(CInput_string::IDD, pParent)
CInput_string::CInput_string( CWnd* pParent, wchar_t* initial, UINT lenght, UINT mode )	: CDialog(CInput_string::IDD, pParent)
{
  //VERIFY(font.CreatePointFont(240, "Arial", NULL));
  //VERIFY(font.CreatePointFont(240, L"Arial", NULL));
	ASSERT( lenght>0 && lenght<50 );
	ASSERT( initial && AfxIsValidString( initial ) );
	//ASSERT( strlen(initial)>=0 && strlen(initial)<=lenght );
	ASSERT( wcslen(initial)>=0 && wcslen(initial)<=lenght );

	max_lenght = lenght;
	//buffer = (char*) malloc( max_lenght+1 );
	buffer = (wchar_t*) malloc( max_lenght+1 );
//  description = NULL;
	//strcpy( buffer, initial );
	wcsncpy( buffer, initial, lenght );



  // Bigger font for digits buttons
  font1.CreateFont( 27,                            // Height
                    0,                             // Width
                    0,                             // Escapement
                    10,                            // Orientation
                    400,                           // Weight 100-900
                    FALSE,                         // Italic
                    FALSE,                         // Underline
                    0,                             // StrikeOut
                    ANSI_CHARSET,                  // CharSet
                    OUT_DEFAULT_PRECIS,            // OutPrecision
                    CLIP_DEFAULT_PRECIS,           // ClipPrecision
                    DEFAULT_QUALITY,               // Quality
                    DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                    lng==CHINESE?L"SimSun":L"Arial" );


  // Smaller font for other buttons
  font2.CreateFont( 15,                            // Height
                    0,                             // Width
                    0,                             // Escapement
                    10,                            // Orientation
                    400,                           // Weight 100-900
                    FALSE,                         // Italic
                    FALSE,                         // Underline
                    0,                             // StrikeOut
                    ANSI_CHARSET,                  // CharSet
                    OUT_DEFAULT_PRECIS,            // OutPrecision
                    CLIP_DEFAULT_PRECIS,           // ClipPrecision
                    DEFAULT_QUALITY,               // Quality
                    DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                    L"Arial" );






  Create( IDD_INPUT_STRING ); //, pParent );
 
  if (mode&CINPSTR_PASSWORD) password = TRUE;
  else                       password = FALSE;

  //font.DeleteObject();



//  noclose = FALSE;
}


//CInput_string::CInput_string( CWnd* pParent, wchar_t* initial, UINT lenght, UINT mode )	: CDialog(CInput_string::IDD, pParent)
CInput_string::CInput_string(CWnd* pParent /*=NULL*/)	: CDialog(CInput_string::IDD, pParent)
{
  //VERIFY(font.CreatePointFont(240, "Arial", NULL));
//  VERIFY(font.CreatePointFont(240, L"Arial", NULL));
/*	ASSERT( lenght>0 && lenght<50 );
	ASSERT( initial && AfxIsValidString( initial ) );
	//ASSERT( strlen(initial)>=0 && strlen(initial)<=lenght );
	ASSERT( wcslen(initial)>=0 && wcslen(initial)<=lenght );
  */

  // Bigger font for digits buttons
  font1.CreateFont( 25,                            // Height
                    0,                             // Width
                    0,                             // Escapement
                    10,                            // Orientation
                    400,                           // Weight 100-900
                    FALSE,                         // Italic
                    FALSE,                         // Underline
                    0,                             // StrikeOut
                    ANSI_CHARSET,                  // CharSet
                    OUT_DEFAULT_PRECIS,            // OutPrecision
                    CLIP_DEFAULT_PRECIS,           // ClipPrecision
                    DEFAULT_QUALITY,               // Quality
                    DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                    lng==CHINESE?L"SimSun":L"Arial" );


  // Smaller font for other buttons
  font2.CreateFont( 15,                            // Height
                    0,                             // Width
                    0,                             // Escapement
                    10,                            // Orientation
                    400,                           // Weight 100-900
                    FALSE,                         // Italic
                    FALSE,                         // Underline
                    0,                             // StrikeOut
                    ANSI_CHARSET,                  // CharSet
                    OUT_DEFAULT_PRECIS,            // OutPrecision
                    CLIP_DEFAULT_PRECIS,           // ClipPrecision
                    DEFAULT_QUALITY,               // Quality
                    DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                    lng==CHINESE?L"SimSun":L"Arial" );



	max_lenght = 100; // lenght;
	//buffer = (char*) malloc( max_lenght+1 );
	buffer = (wchar_t*) malloc( max_lenght*sizeof(wchar_t)+1 );
//  description = NULL;
	//strcpy( buffer, initial );
	//wcsncpy( buffer, initial, lenght ); 

  wcscpy( buffer, L"" );

 // Create( IDD_INPUT_STRING ); //, pParent );  */
 
  int mode = 0;

  if ( mode & CINPSTR_PASSWORD ) password = TRUE;
  else password = FALSE;

//  font.DeleteObject();

}





CInput_string::~CInput_string()
{
  DeleteObject( font1 );
  DeleteObject( font2 );
	DestroyWindow();
	if ( buffer ) free( buffer );
  buffer = NULL;
}

void CInput_string::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInput_string, CDialog)
	ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_BACKSPACE,    &CInput_string::OnBnClickedButtonBkspc  )
  ON_BN_CLICKED(IDC_CLEAR,        &CInput_string::OnBnClickedButtonClear  )
  ON_BN_CLICKED(IDC_BUTTON_UPLOW, &CInput_string::OnBnClickedButtonUpLow  )
  ON_WM_CREATE()
END_MESSAGE_MAP()



// CInput_string message handlers

void CInput_string::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
  //	CWnd* cnt = GetDlgItem(IDC_BUTTON_1);
  //	cnt->SetFont( &font );
	
	if ( !bShow ) return;

	// Assegna font
	GetDlgItem(IDC_CURRENT_VAL )->SetFont( &font1 );

  //GetDlgItem(IDC_CURRENT_DESC )->SetFont( &font );


	int nid;

	for ( nid=2000+48 ; nid<=2000+57 ; nid++ ) GetDlgItem( nid )->SetFont( &font1 ); // cifre
 	for ( nid=2000+65 ; nid<=2000+90 ; nid++ ) GetDlgItem( nid )->SetFont( &font1 ); // lettere

	/*
	char ch[2] = "0";
	if ( maiuscolo )
	{
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( "MAIUSCOLO" );
		for ( nid=65 ; nid<=90 ; nid++ )
		{
			ch[0] = nid;
			GetDlgItem(2000+nid)->SetWindowText(ch); // lettere
		}
	}
	else
	{
		for ( nid=65 ; nid<=90 ; nid++ )
		{
			ch[0] = nid;
			GetDlgItem(2000+nid)->SetWindowText(ch); // lettere
		}
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( "minuscolo" );
	}

	// Inizializzazione del valore
	GetDlgItem(IDC_CURRENT_VAL )->SetWindowText( buffer );
	buff_lenght = strlen( buffer );
	*/


  GetDlgItem( IDOK             )->SetFont( &font2 );
  GetDlgItem( IDCANCEL         )->SetFont( &font2 );
  GetDlgItem( IDC_BACKSPACE    )->SetFont( &font2 );
  GetDlgItem( IDC_CLEAR        )->SetFont( &font2 );
  GetDlgItem( IDC_BUTTON_UPLOW )->SetFont( &font2 );





	GetDlgItem( IDOK          )->SetWindowText( string_ok       [lng] );
	GetDlgItem( IDCANCEL      )->SetWindowText( string_cancel   [lng] );
	GetDlgItem( IDC_BACKSPACE )->SetWindowText( string_backspace[lng] );
	GetDlgItem( IDC_CLEAR     )->SetWindowText( string_clear    [lng] );


	Update_wnd();

}



void CInput_string::Update_wnd( void )
{
	int nid;
	//char ch[2] = "0";
	wchar_t ch[2] = L"0";
	
	if ( maiuscolo )
	{
		//GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( "MAIUSCOLO" );
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( string_uppercase[lng] );
		for ( nid=65 ; nid<=90 ; nid++ )
		{
			ch[0] = nid;
			GetDlgItem(2000+nid)->SetWindowText(ch); // lettere
		}
	}
	else
	{
		for ( nid=65 ; nid<=90 ; nid++ )
		{
			ch[0] = nid + 32;
			GetDlgItem(2000+nid)->SetWindowText(ch); // lettere
		}
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( string_lowercase[lng] );
	}

  //if ( description ) GetDlgItem(IDC_CURRENT_DESC )->SetWindowTextA( description );
  //else               GetDlgItem(IDC_CURRENT_DESC )->SetWindowTextA( "" );

	// Inizializzazione del valore
	GetDlgItem(IDC_CURRENT_VAL)->SetWindowText( buffer );
	//buff_lenght = strlen( buffer );
	buff_lenght = wcslen( buffer );
}




BOOL CInput_string::PreTranslateMessage(MSG* pMsg)
{
  // noclose = FALSE;

	// TODO: Add your specialized code here and/or call the base class
//	TRACE("3 pMsg=%d\n", pMsg->message );

	// 256=WM_KEYDOWN - 257=WM_KEYUP - 258=WM_CHAR
	if ( pMsg->message==WM_CHAR )
	{
		TRACE("WM_CHAR keycode=%d - extra=%08X\n", pMsg->wParam, pMsg->lParam );

		if ( ( pMsg->wParam>='0' && pMsg->wParam<='9' ) || // Cifra
         ( pMsg->wParam>='A' && pMsg->wParam<='Z' ) || // Lettera maiuscola
         ( pMsg->wParam>='a' && pMsg->wParam<='z' ) )//|| // Lettera minuscola
				// ( pMsg->wParam==' ' )                    )    // Spazio
		{

    /*  if ( pMsg->lParam==0 )
      {
        TRACE("Ignorato\n");
        return( 1 );
      } */

      TRACE("Inserito carattere alfanumerico <%c>\n", pMsg->wParam );
			if ( buff_lenght < max_lenght ) // Solo se non siamo già alla massima lunghezza
			{
  			buffer[buff_lenght++] = pMsg->wParam;
  			buffer[buff_lenght  ] = '\0';
			}
      //return(TRUE);
		}

    if ( ( pMsg->wParam==' ' ) || ( pMsg->wParam==32) )     // Spazio
    {
      buffer[buff_lenght++] = ' ';
      buffer[buff_lenght  ] = '\0';
      TRACE ("Inserito spazio\n");
      //noclose = TRUE;
      //return(TRUE);
    }

		if ( pMsg->wParam==13 )
    {
      TRACE("wParam=13 => Chiusura Ok\n");
      //noclose = FALSE;
      OnOK(); // Enter
      //return(TRUE);
    }

    if ( pMsg->wParam==27 )
    {
      TRACE("wParam=27 => Chiusura ESC\n");
      OnCancel(); // ESC
      //return(TRUE);
    }
			
		if ( pMsg->wParam==8 ) // CANCELLAZIONE DELL'ULTIMA CIFRA (BACKSPACE o PULSANTE "Canc")
		{
			if ( buff_lenght > 0 ) // Solo se la lunghezza è magdiore di 0
			{
			  buff_lenght--;
			  buffer[buff_lenght] = '\0';
			}
      //return(TRUE);
		}

		if ( pMsg->wParam==24 ) // CANCELLAZIONE COMPLETA (TASTO 'c' O PULSANTE "Clear")
		{
 		  buff_lenght = 0;
 		  buffer[buff_lenght] = '\0';
      //return(TRUE);
		}

		//TRACE("Nuova stringa: <%s> (%d char)\n", buffer, strlen(buffer) );
    if (password) 
    {
      //CString txt( '*', strlen(buffer) );
      CString txt( '*', wcslen(buffer) );
      GetDlgItem(IDC_CURRENT_VAL )->SetWindowText( txt );
      //return(TRUE);
    }
    else
    {
      GetDlgItem(IDC_CURRENT_VAL )->SetWindowText( buffer );
      //return(0);
    }
	}


	return CDialog::PreTranslateMessage(pMsg);
}




void CInput_string::OnOK()
{
	
/*  if ( noclose )
  {
    TRACE("Noclose: skip OnOK\n");
    noclose = FALSE;
    return;
  } */
   // Lettura e validazione del valore corrente
  TRACE("OnOK chiamato\n");



	CDialog::OnOK();
}



void CInput_string::OnBnClickedButtonUpLow()
{
	maiuscolo = !maiuscolo;
	Update_wnd();

/*	if ( maiuscolo )
	{
		maiuscolo = 0;
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( "minuscolo" );
	}
	else
	{
		maiuscolo = 1;
		GetDlgItem( IDC_BUTTON_UPLOW )->SetWindowText( "MAIUSCOLO" );
	} */
}


void CInput_string::OnBnClickedButtonBkspc()
{
	PostMessage( WM_CHAR, 8, 0 );  // Backspace
}

void CInput_string::OnBnClickedButtonClear()
{
	PostMessage( WM_CHAR, 24, 0 ); // Cancel
}



BOOL CInput_string::OnCommand(WPARAM wParam, LPARAM lParam)
{
 // noclose = FALSE;

  if ( HIWORD(wParam)==0 ) // gets commands from touch screen (controls)
	{
  	if ( LOWORD(wParam)>=2048 && LOWORD(wParam)<=2057 )
	  {
			//TRACE("Premuta cifra <%d>\n", LOWORD(wParam)-2048 );
    	PostMessage( WM_CHAR, '0'+LOWORD(wParam)-2048, 0 );
		}

		if ( LOWORD(wParam)>=2065 && LOWORD(wParam)<=2090 )
	  {
			//TRACE("Premuta lettera <%c>\n", 'A'+LOWORD(wParam)-2065 );
    	if ( maiuscolo ) PostMessage( WM_CHAR, 'A'+LOWORD(wParam)-2065, 0 );
			else             PostMessage( WM_CHAR, 'a'+LOWORD(wParam)-2065, 0 );

      //PostMessage( WM_CHAR, 2042, 0 );
		}

		if ( LOWORD(wParam)==2032 )
	  {
			TRACE("Premuto spazio OnCommand\n");
    	PostMessage( WM_CHAR, ' ', 0 );
      //noclose=TRUE;
      //return(0);
		}
	}


  TRACE("OnCommand %d,%d %d,%d\n",HIWORD(wParam),LOWORD(wParam),HIWORD(lParam),LOWORD(lParam) );

	return CDialog::OnCommand(wParam, lParam);
}

/*
int CInput_string::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//  if (CDialog::OnCreate(lpCreateStruct) == -1)
//    return -1;

  // TODO:  Add your specialized creation code here
  Create( IDD_INPUT_STRING );

  return 0;
}
*/