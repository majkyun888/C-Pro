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
  VERIFY(font.CreatePointFont(240, L"Arial", NULL));
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

  Create( IDD_INPUT_STRING ); //, pParent );
 
  if (mode&CINPSTR_PASSWORD) password = TRUE;
  else                       password = FALSE;

  noclose = FALSE;
}


//CInput_string::CInput_string( CWnd* pParent, wchar_t* initial, UINT lenght, UINT mode )	: CDialog(CInput_string::IDD, pParent)
CInput_string::CInput_string(CWnd* pParent /*=NULL*/)	: CDialog(CInput_string::IDD, pParent)
{
  //VERIFY(font.CreatePointFont(240, "Arial", NULL));
  VERIFY(font.CreatePointFont(240, L"Arial", NULL));
/*	ASSERT( lenght>0 && lenght<50 );
	ASSERT( initial && AfxIsValidString( initial ) );
	//ASSERT( strlen(initial)>=0 && strlen(initial)<=lenght );
	ASSERT( wcslen(initial)>=0 && wcslen(initial)<=lenght );
  */

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
}





CInput_string::~CInput_string()
{
	DestroyWindow();
	if ( buffer ) free( buffer );
  buffer = NULL;
}

void CInput_string::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInput_string, CDialog)
  ON_WM_CREATE()
	ON_WM_SHOWWINDOW()

//  ON_BN_CLICKED(IDC_BACKSPACE,    &CInput_string::OnBnClickedButtonBkspc  )
//  ON_BN_CLICKED(IDC_CLEAR,        &CInput_string::OnBnClickedButtonClear  )
//  ON_BN_CLICKED(IDC_BUTTON_UPLOW, &CInput_string::OnBnClickedButtonUpLow  )
  ON_BN_CLICKED(IDC_BUTTON_SPACE,   &CInput_string::OnBnClickedButtonSpc    )
  ON_BN_CLICKED(IDC_BUTTON_OK,      &CInput_string::OnBnClickedButtonOk     )

//  ON_BN_CLICKED(IDOK, &CInput_string::OnBnClickedOk)
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
	GetDlgItem(IDC_CURRENT_VAL )->SetFont( &font );

  //GetDlgItem(IDC_CURRENT_DESC )->SetFont( &font );


	int nid;

	for ( nid=2000+48 ; nid<=2000+57 ; nid++ ) GetDlgItem( nid )->SetFont( &font ); // cifre
 	for ( nid=2000+65 ; nid<=2000+90 ; nid++ ) GetDlgItem( nid )->SetFont( &font ); // lettere

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

	GetDlgItem( IDC_BUTTON_OK     )->SetWindowText( string_ok       [lng] );
	GetDlgItem( IDC_BUTTON_CANCEL )->SetWindowText( string_cancel   [lng] );
	GetDlgItem( IDC_BACKSPACE     )->SetWindowText( string_backspace[lng] );
	GetDlgItem( IDC_CLEAR         )->SetWindowText( string_clear    [lng] );


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


// 256=WM_KEYDOWN - 257=WM_KEYUP - 
// 258=WM_CHAR 0x0102
/*
#define WM_KEYFIRST                     0x0100   256
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101   257
#define WM_CHAR                         0x0102   258
#define WM_DEADCHAR                     0x0103   259
#define WM_SYSKEYDOWN                   0x0104   260
#define WM_SYSKEYUP                     0x0105   261
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107
*/


BOOL CInput_string::PreTranslateMessage(MSG* pMsg)
{
  // noclose = FALSE;

	// TODO: Add your specialized code here and/or call the base class
  //return CDialog::PreTranslateMessage(pMsg);

  //if ( pMsg->message == 512 ) return CDialog::PreTranslateMessage(pMsg);

  //TRACE("CInput_string::PreTranslateMessage: pMsg=%04X - %08X %08X\n", pMsg->message, pMsg->wParam, pMsg->lParam );

  //return CDialog::PreTranslateMessage(pMsg);


	// 256=WM_KEYDOWN - 257=WM_KEYUP - 258=WM_CHAR
	if ( pMsg->message==WM_CHAR )
	{
		TRACE("CInput_string::PreTranslateMessage: WM_CHAR keycode=%d - extra=%08X\n", pMsg->wParam, pMsg->lParam );

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

      TRACE("CInput_string::PreTranslateMessage: Inserito carattere alfanumerico <%c>\n", pMsg->wParam );
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
      TRACE ("CInput_string::PreTranslateMessage: Inserito spazio\n");
      //noclose = TRUE;
      //return(TRUE);
    }

		if ( pMsg->wParam==VK_RETURN ) // 13
    {
      TRACE("CInput_string::PreTranslateMessage: wParam=13 => Chiusura Ok\n");
      //noclose = FALSE;
      OnOK(); // Enter
      //return(TRUE);
    }

    if ( pMsg->wParam==VK_ESCAPE ) // 27
    {
      TRACE("CInput_string::PreTranslateMessage: wParam=27 => Chiusura ESC\n");
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
	
 /* if ( noclose )
  {
    TRACE("Noclose: skip OnOK\n");
    noclose = FALSE;
    return;
  }*/
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
	PostMessage( WM_CHAR, VK_BACK , 0 );  // 8 - Backspace
}

void CInput_string::OnBnClickedButtonClear()
{
	PostMessage( WM_CHAR, 24, 0 ); // Cancel
}



BOOL CInput_string::OnCommand(WPARAM wParam, LPARAM lParam)
{
 // noclose = FALSE;

	//return CDialog::OnCommand(wParam, lParam);

  TRACE("CInput_string::OnCommand: %04X-%04X %04X-%04X\n",HIWORD(wParam),LOWORD(wParam),HIWORD(lParam),LOWORD(lParam) );

  //return TRUE;

  return CDialog::OnCommand(wParam, lParam);


  if ( HIWORD(wParam)==0 ) // gets commands from touch screen (controls)
	{
 
    /*if ( LOWORD(wParam)>=2048 && LOWORD(wParam)<=2057 )
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
		} */

    /*

		if ( LOWORD(wParam)==2032 )
	  {
			TRACE("CInput_string::OnCommand premuto spazio\n");
    	PostMessage( WM_CHAR, ' ', 0 );
      //noclose=TRUE;
      //return(0);
		}*/
	}


  //TRACE("OnCommand %d,%d %d,%d\n",HIWORD(wParam),LOWORD(wParam),HIWORD(lParam),LOWORD(lParam) );

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

void CInput_string::OnBnClickedButtonSpc()
{
  PostMessage( WM_CHAR, VK_SPACE, 390001 );
}

/*
void CInput_string::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  //CDialog::OnOK();

  PostMessage( WM_CHAR, VK_RETURN, 390001 );

}
*/

void CInput_string::OnBnClickedButtonOk()
{
  // TODO: Add your control notification handler code here
  PostMessage( WM_CHAR, VK_RETURN, 390001 );
}
