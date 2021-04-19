// Edit_mod.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Edit_mod.h"
#include "Def_globali.h"

// CEdit_mod


IMPLEMENT_DYNAMIC(CEdit_mod, CEdit)

CEdit_mod::CEdit_mod()
{

}

CEdit_mod::~CEdit_mod()
{
}


BEGIN_MESSAGE_MAP(CEdit_mod, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CEdit_mod::OnEnChange)
END_MESSAGE_MAP()



// CEdit_mod message handlers



void CEdit_mod::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//Beep( 1000 , 100 );


	parent->PostMessage( WM_MODIFIED );
	


//  SendMessage( parent, WM_MODIFIED , 0, 0 );

}
