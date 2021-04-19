// Button_mod.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Button_mod.h"
#include "Def_globali.h"

#include "Parameter.h"


// CButton_mod

extern UINT             lng;

IMPLEMENT_DYNAMIC(CButton_mod, CButton)

CButton_mod::CButton_mod()
{
  text_col = 0x00AA0000;
  back_col = 0x00AA0000;
//  brush.CreateSolidBrush( back_col );

  add_onoff = TRUE; // default
}

CButton_mod::~CButton_mod()
{
}


BEGIN_MESSAGE_MAP(CButton_mod, CButton)
//	ON_CONTROL_REFLECT(BN_CLICKED, &CButton_mod::OnBnClicked)
//	ON_WM_DRAWITEM()
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/*
// 	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR_REFLECT()

// CButton_mod message handlers
HBRUSH CButton_mod::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
  pDC->SetTextColor( text_col );   // text
  pDC->SetBkColor  ( back_col );   // text bkgnd
  return brush;                    // ctl bkgnd

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	//return NULL;
}
*/


//void CButton_mod::OnBnClicked()
//{
//	// TODO: Add your control notification handler code here
//
//	TRACE("Pulsante <%08X> cliccato\n", this );
//
//	parent->PostMessage( WM_MODIFIED, WPARAM(this) );
//
//	TRACE("CButton_mod: fuoco corrente su %X\n",	GetFocus() );
//}


void CButton_mod::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
//	TRACE("Pulsante <%08X> LBdown\n", this );
	parent->PostMessage( WM_MODIFIED, WPARAM(this) );
	CButton::OnLButtonDown(nFlags, point);
}



/*
BOOL CButton_mod::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	RECT rett;
	GetClientRect( &rett );
	pDC->FillSolidRect( &rett, back_col );


//CDC:: FillSolidRect()


	return CButton::OnEraseBkgnd(pDC);
}
*/



//void CButton_mod::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//void CButton_mod::DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: Add your message handler code here and/or call default
//   UINT uStyle = DFCS_BUTTONPUSH;
//
//   // This code only works with buttons.
//   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
//
//   // If drawing selected, add the pushed style to DrawFrameControl.
//   if (lpDrawItemStruct->itemState & ODS_SELECTED) uStyle |= DFCS_PUSHED;
//
//
// Draw the button frame.
//   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);
//
//   // Get the button's text.
//   CString strText;
//   GetWindowText(strText);
//
//   // Draw the button text using the text color red.
//   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255,0,0));
//   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
//   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
//
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}


void CButton_mod::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	 COLORREF col;
	 //char*    text_add;
	 wchar_t*    text_add;

	 //TRACE(L"CButton_mod(%X): DrawItem\n", this);

   // If drawing selected, add the pushed style to DrawFrameControl.
//   if (lpDrawItemStruct->itemState & ODS_SELECTED)


   CParameter* par = (CParameter*)parent;
   if ( *(bool*)    ((CParameter*)par->val) )
   {
     uStyle |= DFCS_PUSHED;
     col = RGB(200,0,0);
     text_add = string_on[lng]; //L" (ON)";
   }
   else                  
   {
     uStyle  &= ~DFCS_PUSHED;
     col      = RGB(130,130,130);
     text_add = string_off[lng]; //L" (OFF)";
   }

   if ( !add_onoff ) // no changes in color/add ON/OFF
   {
     uStyle  &= ~DFCS_PUSHED;
     col = RGB(0,0,100);
     text_add = L"";
   }

   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);

   // Get the button's text.
   CString strText;
   GetWindowText(strText);
	 strText.Append( text_add );

   // Draw the button text using the text color red.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, col);
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);


//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

