#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "Text_box.h"


/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

//CFont* CText_box::font = NULL;
//CFont CText_box::font;

extern CChildView*  pView;
extern UINT         lng;
//extern int Lingua;

// --------------------------------------------------------------------------------------
//
//  CLASSE PER LA CREAZIONE E VISUALIZZAZIONE DI OGGETTI CSTATIC DI COLORE VARIABILE
//
// --------------------------------------------------------------------------------------

// Dopo la costruzione e la visualizzazione si possono cambiare testo, col.testo e col.sfondo
// i colori testo e sfondo vengono assegnati alle variabili membro colore_testo/sfondo
// con la SetWindowText la finestra viene aggiornata con i colori ed il testo impostati

// Si può ridimensionare/spostare l'area con Get/SetWindowPlacement

// Si può modificare il font nel seguente modo
//  VERIFY(font_grande.CreatePointFont(120, "Arial"));
//  riga1->SetFont( &font_grande );
//  font_grande.DeleteObject(); (attenzione a distruggere il font solo alla fine!

// ESEMPIO:
/* RECT area;
   area.left   = 100;
   area.right  = area.left + 290;
   area.top    = 100;
   area.bottom = area.top + 20;
   riga1 = new CText_box( area , this , SS_LEFT   , WS_EX_CLIENTEDGE );  
   riga1->SetFont( &font_grande );
   area.top    = area.bottom + 10;
   area.bottom = area.top + 30;
   riga2 = new CText_box( area , this , SS_CENTER , WS_EX_STATICEDGE );  
   area.top    = area.bottom + 10;
   area.bottom = area.top + 30;
   riga3 = new CText_box( area , this , SS_RIGHT , WS_EX_WINDOWEDGE    );  */

// Tempo per l'aggiornamento di una finestre da circa 100-200 x 20 pixel circa 100us su Acer732
// Durante la costruzione si possono passare (opzionalmente) stili e stili estesi.
// stili_add può essere ad esempio SS_LEFT, SS_CENTER o SS_RIGHT per la giustificazione del testo
// stili_ex_add può essere ad esempio WS_EX_CLIENTEDGE per una finestra "incassata"
// WS_EX_STATICEDGE   , WS_EX_WINDOWEDGE   
CText_box::CText_box( RECT& area_in, CWnd* par, DWORD stili_add , DWORD stili_ex_add )
{
//	font          = NULL;
  parent        = par;
  area          = area_in;
  colore_sfondo = ::GetSysColor( COLOR_INACTIVEBORDER ); //COLORE_SFONDO;
  colore_testo  = RGB(0,0,0);
  penn_sfondo.CreateSolidBrush( RGB(100,100,100) );
//  VERIFY( CreateEx( stili_ex_add , _T("STATIC") , "" , WS_CHILD | WS_TABSTOP | WS_VISIBLE | stili_add , area , parent , 10000 ) );
//  VERIFY( CreateEx( WS_EX_STATICEDGE|stili_ex_add , _T("STATIC") , "" , /*SS_OWNERDRAW|*/WS_CHILD|WS_VISIBLE|stili_add , area , parent , 10000 ) );
//	VERIFY( Create( "###" , /*SS_OWNERDRAW|*/WS_CHILD|WS_VISIBLE|stili_add , area , parent , 10000 ) );

  //CFont     font;

  VERIFY( CreateEx( WS_EX_STATICEDGE|stili_ex_add , _T("STATIC") , L"" , WS_CHILD|WS_VISIBLE|stili_add , area , parent , 10000 ) );

//	if ( !font ) // !font.m_hObject )
/*  if ( !font.m_hObject ) // !font.m_hObject )
	{
  	CClientDC dc(this);
		//font = new CFont;
    //VERIFY(font->CreatePointFont(110, "Tunga", &dc));
    //VERIFY(font->CreatePointFont(110, L"Tunga", &dc));
    //VERIFY(font->CreatePointFont( TEXT_ROWS_FONT, &dc));

//    VERIFY( font.CreatePointFont( TEXT_ROWS_FONT, &dc) );
    VERIFY( font.CreatePointFont( TEXT_ROWS_FONT, NULL) );

    //		dim_ctrl     = dc.GetTextExtent("ABCDEFG");
    //		dim_ctrl.cy += 2 * 3; // + 10; // Aggiungo il bordo superiore e inferiore
	}
	//if ( font && font->m_hObject ) SetFont( font );
	if ( font.m_hObject ) SetFont( &font );*/


  int dimch = 22; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 22;

  font.CreateFont( dimch,                         // Height
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

  SetFont( &font );




//	ValidateRgn(NULL);

//	EnableWindow(FALSE);
//  VERIFY( SetWindowPos(&CWnd::wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE) );

//	LockWindowUpdate();

  #ifdef _DEBUG 
    //SetWindowText( "Empty" );
    SetWindowText( L"Empty" );
  #endif

  //font.DeleteObject();

}



CText_box::CText_box( int dim, LPCTSTR name, RECT& area_in, CWnd* par, DWORD stili_add , DWORD stili_ex_add )
{
  parent        = par;
  area          = area_in;
  colore_sfondo = ::GetSysColor( COLOR_INACTIVEBORDER ); //COLORE_SFONDO;
  colore_testo  = RGB(0,0,0);
  penn_sfondo.CreateSolidBrush( RGB(100,100,100) );

  ASSERT( par && par->m_hWnd ); // check parent window has been created already

  VERIFY( CreateEx( WS_EX_STATICEDGE|stili_ex_add , _T("STATIC") , L"" , WS_CHILD|stili_add , area , parent , 10000 ) );

  int dimch = dim; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = dim;

  font.CreateFont( dimch,                         // Height
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
  
  SetFont( &font );
//  CFont* old_font = dc.SelectObject( &font );
//  VERIFY( CreateEx( WS_EX_STATICEDGE|stili_ex_add , _T("STATIC") , L"" , WS_CHILD|stili_add , area , parent , 10000 ) );

  #ifdef _DEBUG 
    SetWindowText( L"Empty" );
  #endif

 //  dc.SelectObject( old_font );
 // font.DeleteObject();
}





/*
CText_box::CText_box( Dimensioni_immagine& area_in , CWnd* par , CFont* font , DWORD stili_add , DWORD stili_ex_add )
{
  parent        = par;
  area.left     = area_in.x0;
  area.top      = area_in.y0;
  area.right    = area.left + area_in.dx - 1;
  area.bottom   = area.top  + area_in.dy - 1;
  colore_sfondo = ::GetSysColor( COLOR_INACTIVEBORDER ); //COLORE_SFONDO;
  colore_testo  = COLORE_NERO;
  penn_sfondo.CreateSolidBrush(COLORE_SFONDO );
  VERIFY( CreateEx( stili_ex_add , _T("STATIC") , "" , WS_CHILD | WS_TABSTOP | WS_VISIBLE | stili_add , area , parent , 10000 ) );
  if ( font != NULL ) SetFont( font );
  #ifdef _DEBUG 
    SetWindowText( "Empty" );
  #endif
}
*/


CText_box::~CText_box()
{

  font.DeleteObject();

	DestroyWindow();

/*	if ( font )
	{
		font->DeleteObject();
		delete font;
		font = NULL;
	} */

/*  if ( font.m_hObject )
	{
		font.DeleteObject();
	}*/

}


BEGIN_MESSAGE_MAP(CText_box, CStatic)
	//{{AFX_MSG_MAP(CText_box)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()



HBRUSH CText_box::CtlColor(CDC* pDC, UINT nCtlColor) 
{
//  SetFont( &font );
  pDC->SetTextColor( colore_testo  ); // xxBBGGRR
  pDC->SetBkColor  ( colore_sfondo );
  m_brBkgnd.DeleteObject();
  m_brBkgnd.CreateSolidBrush(colore_sfondo);
  return (HBRUSH)m_brBkgnd; //penn_sfondo;
} 


// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
//void CText_box::SetText( char* stringa )
void CText_box::SetText( wchar_t* stringa )
{
  ASSERT( AfxIsValidString( stringa ) );
  SetWindowText( stringa );
}


/*
// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
// questa seconda funzione passa automaticamente alla sottostringa relativa alla lingua attuale
void CText_box::SetText( char** stringa )
{
  ASSERT( AfxIsValidString( stringa[Lingua] ) );
  SetWindowText( stringa[Lingua] );
}
*/

// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
void CText_box::SetText( CString& stringa )
{
  ASSERT( AfxIsValidString( stringa ) );
  SetWindowText( stringa );
}

/////////////////////////////////////////////////////////////////////////
//    SECONDA VERSIONE CHE INCLUDE NEI PARAMETRI IL COLORE DEL TESTO   //
/////////////////////////////////////////////////////////////////////////

// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
//void CText_box::SetText( char* stringa , COLORREF col_testo )
void CText_box::SetText( wchar_t* stringa , COLORREF col_testo )
{
  ASSERT( AfxIsValidString( stringa ) );
  colore_testo = col_testo;
  SetWindowText( stringa );
}


/*
// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
// questa seconda funzione passa automaticamente alla sottostringa relativa alla lingua attuale
void CText_box::SetText( char** stringa , COLORREF col_testo )
{
  ASSERT( AfxIsValidString( stringa[Lingua] ) );
  colore_testo = col_testo;
  SetWindowText( stringa[Lingua] );
}
*/


// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
void CText_box::SetText( CString& stringa , COLORREF col_testo )
{
  ASSERT( AfxIsValidString( stringa ) );
  colore_testo = col_testo;
  SetWindowText( stringa );
}


// Passo di qui per chiamare SetWindowText dopo aver controllato la validita' della stringa
void CText_box::SetText( CString& stringa, COLORREF col_testo, COLORREF col_sfondo )
{
  ASSERT( AfxIsValidString( stringa ) );
  colore_testo  = col_testo;
  colore_sfondo = col_sfondo;
  SetWindowText( stringa );
}


//void CText_box::SetText( char* stringa, COLORREF col_testo, COLORREF col_sfondo )
void CText_box::SetText( wchar_t* stringa, COLORREF col_testo, COLORREF col_sfondo )
{
  ASSERT( AfxIsValidString( stringa ) );
  colore_testo  = col_testo;
  colore_sfondo = col_sfondo;
  SetWindowText( stringa );
}


/*
void CText_box::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
//   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	 COLORREF col;
//	 char*    text_add;



   /*


   // If drawing selected, add the pushed style to DrawFrameControl.
   if ( pView && ( pView->menu_btn[0]->child->IsWindowVisible() ) ) //(lpDrawItemStruct->itemState & ODS_SELECTED)
	 {
		 //TRACE("NON DISEGNARE!\n");
		 TRACE( L"DON'T DRAW!\n");
		 return;
//		 uStyle |= DFCS_PUSHED;
//		 col = RGB(200,0,0);
//		 TRACE("Pulsante selezionato\n");
//		 text_add = " (ON)";

	 }
	 else                  
	 {
		 //TRACE("DISEGNA!\n");
		 TRACE( L"DRAW!\n");
//		 CDC dc;
//		 CStatic::OnPaint PaintWindowlessControls(&dc);
//		 CStatic::PaintWindowlessControls(&dc);
			 //Static::OnPaint();
		 return;
//		 CStatic::DrawItem(lpDrawItemStruct);

//		 uStyle  &= ~DFCS_PUSHED;
//		 col      = RGB(0,0,0);
//		 TRACE("Pulsante non selezionato\n");
//		 text_add = " (OFF)";
	 }

   */

/*

// Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);
//ODA_DRAWENTIRE

   // Get the button's text.
   CString strText;
   GetWindowText(strText);
//	 strText.Append( text_add );

   // Draw the button text using the text color red.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, col);
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);


//	CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
*/


/*
void CText_box::OnPaint()
{
	//CPaintDC dc(this); // device context for painting

/*	if ( pView && ( pView->menu_btn[0]->child->IsWindowVisible() ) ) 	ValidateRgn(NULL);
	else
	{
		CRgn rgn;
		RECT rett;
		GetClientRect(&rett);
    rgn.CreateRectRgnIndirect(&rett);

		ValidateRgn(&rgn);
	}  */

//	TRACE("CText_box OnPaint()\n");

//	if ( pView->ncont_visible==0 ) CStatic::OnPaint();
//	else                          CStatic::OnPaint();

	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
//}
