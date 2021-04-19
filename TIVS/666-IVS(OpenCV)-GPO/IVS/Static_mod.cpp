// Static_mod.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Static_mod.h"
#include "Def_globali.h"


// CStatic_mod

IMPLEMENT_DYNAMIC(CStatic_mod, CStatic)


HBRUSH CStatic_mod::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd; // Passing a Handle to the Brush
	pDC->SetBkColor  ( Back_Color ); // Setting the Color of the Text Background to the one passed by the Dialog
	pDC->SetTextColor( Text_Color ); // Setting the Text Color to the one Passed by the Dialog

//	if (nCtlColor) nCtlColor += 0;  // To get rid of compiler warning

	return hbr;
}

CStatic_mod::CStatic_mod()
{
	ZeroMemory( text_old , sizeof( text_old ) );
//	Back_Color_old = 0;
//	Text_Color_old = 0;

  parent = NULL;

}

CStatic_mod::~CStatic_mod()
{
}



// CStatic_mod message handlers
BEGIN_MESSAGE_MAP(CStatic_mod, CStatic)
	ON_CONTROL_REFLECT(STN_CLICKED, &CStatic_mod::OnStnClicked)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


void CStatic_mod::SetBackColor(COLORREF crColor)
{
	Back_Color = crColor; // Passing the value passed by the dialog to the member varaible for Backgound Color
	m_brBkgnd.DeleteObject(); // Deleting any Previous Brush Colors if any existed.
	m_brBkgnd.CreateSolidBrush(crColor); // Creating the Brush Color For the Static Text Background
	RedrawWindow();
}

void CStatic_mod::SetTextColor(COLORREF crColor)
{
	Text_Color = crColor; // Passing the value passed by the dialog to the member varaible for Text Color
	RedrawWindow();
}



//void CStatic_mod::Update_text( char* text_in, COLORREF text_color, COLORREF back_color )
void CStatic_mod::Update_text( wchar_t* text_in, COLORREF text_color, COLORREF back_color )
{
//	if ( strcmp( text_in, text_old ) )
	if ( wcscmp( text_in, text_old ) )
	{
		SetWindowText( text_in );
		//strcpy( text_old, text_in );
		wcscpy( text_old, text_in );
	}

	if ( ( text_color != Text_Color ) || ( back_color != Back_Color ) )
	{
		Text_Color = text_color; // 
		Back_Color = back_color; // 
  	m_brBkgnd.DeleteObject(); // Deleting any Previous Brush Colors if any existed.
	  m_brBkgnd.CreateSolidBrush(Back_Color); // Creating the Brush Color For the Static Text Background
  	RedrawWindow();
	}
}



void CStatic_mod::OnStnClicked()
{
	// TODO: Add your control notification handler code here
  if ( parent && parent->m_hWnd ) parent->PostMessage( WM_MODIFIED ); // commentato 20130824 - faceva bloccare il prog cliccando su un valore update stat a schermo!
}

