// Input_figures.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Input_figures.h"
#include "strings.h"
#include "Globals.h"


// CInput_figures dialog

extern UINT             lng;

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern  CPassword_manager_v2*          password_mngr;
#else
  extern  CPassword_manager*             password_mngr;
#endif


IMPLEMENT_DYNAMIC(CInput_figures, CDialog)

CInput_figures::CInput_figures(CWnd* pParent /*=NULL*/)	: CDialog(CInput_figures::IDD, pParent)
//CInput_figures::CInput_figures(CWnd* pParent /*=NULL*/)
{

//	Create( IDD_INPUT_FIGURES );
//	Create( IDD_INPUT_STRING  );

  //VERIFY(font.CreatePointFont(240, "Arial", NULL));
//  VERIFY(font.CreatePointFont(240, L"Arial", NULL)); VERIFICARE


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
                    lng==CHINESE?L"SimSun":L"Arial" );



}

CInput_figures::~CInput_figures()
{
  DeleteObject( font1 );
  DeleteObject( font2 );
	DestroyWindow();
}

void CInput_figures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInput_figures, CDialog)
	ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_BUTTON_0, &CInput_figures::OnBnClickedButton0)
  ON_BN_CLICKED(IDC_BUTTON_1, &CInput_figures::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON_2, &CInput_figures::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON_3, &CInput_figures::OnBnClickedButton3)
  ON_BN_CLICKED(IDC_BUTTON_4, &CInput_figures::OnBnClickedButton4)
  ON_BN_CLICKED(IDC_BUTTON_5, &CInput_figures::OnBnClickedButton5)
  ON_BN_CLICKED(IDC_BUTTON_6, &CInput_figures::OnBnClickedButton6)
  ON_BN_CLICKED(IDC_BUTTON_7, &CInput_figures::OnBnClickedButton7)
  ON_BN_CLICKED(IDC_BUTTON_8, &CInput_figures::OnBnClickedButton8)
  ON_BN_CLICKED(IDC_BUTTON_9, &CInput_figures::OnBnClickedButton9)

  ON_BN_CLICKED(IDC_BACKSPACE,      &CInput_figures::OnBnClickedButtonBkspc  )
  ON_BN_CLICKED(IDC_CLEAR,          &CInput_figures::OnBnClickedButtonClear  )
  ON_BN_CLICKED(IDC_BUTTON_DOT,     &CInput_figures::OnBnClickedButtonDot    )
  ON_BN_CLICKED(IDC_BUTTON_CHNGSGN, &CInput_figures::OnBnClickedButtonChngSgn)
	
//	ON_BN_CLICKED(IDOK, &CInput_figures::OnBnClickedOk)
END_MESSAGE_MAP()



// CInput_figures message handlers

void CInput_figures::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
//	CWnd* cnt = GetDlgItem(IDC_BUTTON_1);
//	cnt->SetFont( &font );
	
	if ( !bShow ) return;

	// Assegna font
	GetDlgItem(IDC_CURRENT_VAL   )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_1      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_2      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_3      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_4      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_5      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_6      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_7      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_8      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_9      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_0      )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_DOT    )->SetFont( &font1 );
	GetDlgItem(IDC_BUTTON_CHNGSGN)->SetFont( &font1 );

	GetDlgItem( IDC_CURRENT_VAL  )->SetFont( &font1 );

  GetDlgItem( IDOK             )->SetFont( &font2 );
  GetDlgItem( IDCANCEL         )->SetFont( &font2 );
  GetDlgItem( IDC_BACKSPACE    )->SetFont( &font2 );
  GetDlgItem( IDC_CLEAR        )->SetFont( &font2 );

	// Inizializzazione del valore
	GetDlgItem(IDC_CURRENT_VAL   )->SetWindowText( buffer );
	buff_len = wcslen( buffer );

	GetDlgItem( IDOK          )->SetWindowText( string_ok       [lng] );
	GetDlgItem( IDCANCEL      )->SetWindowText( string_cancel   [lng] );
	GetDlgItem( IDC_BACKSPACE )->SetWindowText( string_backspace[lng] );
	GetDlgItem( IDC_CLEAR     )->SetWindowText( string_clear    [lng] );


}


BOOL CInput_figures::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
//	TRACE("3 pMsg=%d\n", pMsg->message );

	// 256=WM_KEYDOWN - 257=WM_KEYUP - 258=WM_CHAR
	if ( pMsg->message==WM_CHAR )
	{
		//TRACE("WM_CHAR keycode=%d - extra=%08X\n", pMsg->wParam, pMsg->lParam );

		switch( pMsg->wParam )
		{
			// INSERIMENTO DI UNA CIFRA 0..9
		  case '0' :
		  case '1' :
		  case '2' :
		  case '3' :
		  case '4' :
		  case '5' :
		  case '6' :
		  case '7' :
		  case '8' :
		  case '9' :
        if ( buff_len < MAX_LENGHT ) // Solo se non siamo già alla massima lunghezza
				{
  				buffer[buff_len++] = pMsg->wParam;
	  			buffer[buff_len  ] = '\0';
				}
				break;

			// Enter 
			//case 13:
      case 32:
				OnOK();
				break;

			// ESC
			case 27:
				OnCancel();
				break;

			// Punto decimale
		  case '.' :
				if ( buff_len < MAX_LENGHT ) // Solo se non siamo già alla massima lunghezza
				{
  				buffer[buff_len++] = pMsg->wParam;
	  			buffer[buff_len  ] = '\0';
				}
				break;

			// CANCELLAZIONE DELL'ULTIMA CIFRA (BACKSPACE o PULSANTE "Canc")
		  case  8 :
				if ( buff_len > 0 ) // Solo se la lunghezza è magdiore di 0
				{
				  buff_len--;
				  buffer[buff_len] = '\0';
				}
				break;

			// CANCELLAZIONE COMPLETA (TASTO 'c' O PULSANTE "Clear")
		  case  'c' : 
		  case  'C' : 
  		  buff_len = 0;
	 		  buffer[buff_len] = '\0';
				break;

			// CAMBIO DI SEGNO (TASTI '+' o '-' O PULSANTE +/-)
		  case  '+' : 
		  case  '-' : 
				//char temp[MAX_LENGHT+1];
				wchar_t temp[MAX_LENGHT+1];
        if ( buff_len>0 && buff_len<MAX_LENGHT )
				{
          if ( buffer[0]=='-' ) // C'è già il segno meno, lo tolgo
				  {
					  //strcpy( temp, buffer );
					  //strcpy( buffer, temp + 1 );
					  wcscpy( temp, buffer );
					  wcscpy( buffer, temp + 1 );
						buff_len--;
				  }
				  else // Non c'è il segno meno, lo metto				
				  {
					  //strcpy( temp, buffer );
					  wcscpy( temp, buffer );
						buffer[0] = '-';
					  //strcpy( buffer+1, temp );
					  wcscpy( buffer+1, temp );
						buff_len++;
				  }
				}
				break;

		}
    GetDlgItem(IDC_CURRENT_VAL   )->SetWindowText( buffer );



	}


	return CDialog::PreTranslateMessage(pMsg);
}



void CInput_figures::OnBnClickedButton0()
{
	PostMessage( WM_CHAR, '0', 0 );
}

void CInput_figures::OnBnClickedButton1()
{
	PostMessage( WM_CHAR, '1', 0 );
}

void CInput_figures::OnBnClickedButton2()
{
	PostMessage( WM_CHAR, '2', 0 );
}

void CInput_figures::OnBnClickedButton3()
{
	PostMessage( WM_CHAR, '3', 0 );
}

void CInput_figures::OnBnClickedButton4()
{
	PostMessage( WM_CHAR, '4', 0 );
}

void CInput_figures::OnBnClickedButton5()
{
	PostMessage( WM_CHAR, '5', 0 );
}

void CInput_figures::OnBnClickedButton6()
{
	PostMessage( WM_CHAR, '6', 0 );
}

void CInput_figures::OnBnClickedButton7()
{
	PostMessage( WM_CHAR, '7', 0 );
}

void CInput_figures::OnBnClickedButton8()
{
	PostMessage( WM_CHAR, '8', 0 );
}

void CInput_figures::OnBnClickedButton9()
{
	PostMessage( WM_CHAR, '9', 0 );
}

void CInput_figures::OnBnClickedButtonBkspc()
{
	PostMessage( WM_CHAR, 8, 0 );
}

void CInput_figures::OnBnClickedButtonClear()
{
	PostMessage( WM_CHAR, 'c', 0 );
}

void CInput_figures::OnBnClickedButtonDot()
{
	PostMessage( WM_CHAR, '.', 0 );
}

void CInput_figures::OnBnClickedButtonChngSgn()
{
	PostMessage( WM_CHAR, '+', 0 );
}

void CInput_figures::OnOK()
{
	// Lettura e validazione del valore corrente
	CDialog::OnOK();
}

int CInput_figures::read_string(wchar_t* buffer, wchar_t* caption )
{
  ASSERT( buffer  && AfxIsValidAddress(buffer, MAX_NOTE_LENGTH+1) );
  ASSERT( caption && AfxIsValidAddress(caption, 100) );

  wcsncpy( buffer ,L"",MAX_NOTE_LENGTH+1 );

  CInput_string stringpad;
  stringpad.password = FALSE;

  swprintf( stringpad.buffer ,MAX_NOTE_LENGTH+1 , buffer );

  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( caption );
 // stringpad.GetDlgItem( IDC_CURRENT_VAL  )->SetWindowText( buffer  );

//  if ( stringpad.RunModalLoop() == IDOK )
  if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>2 )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( wcslen(stringpad.buffer)<=MAX_NOTE_LENGTH );
		wcsncpy( buffer, stringpad.buffer, MAX_NOTE_LENGTH+1 );
    return 0;
	}

  return 99;
}