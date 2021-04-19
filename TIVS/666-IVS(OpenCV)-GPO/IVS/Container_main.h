#pragma once
#include "Container_wnd.h"
//#include "container_main.h"

#define IDD_LOGO_BTN 1455
#define IDD_USER_BTN 1456


class CContainer_main :	public CContainer_wnd
{
public:
	DECLARE_DYNAMIC(CContainer_main)

//	CContainer_main(void);
  //CContainer_main( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename=160 );
  CContainer_main( wchar_t* text_in, RECT rett_in, CWnd* parent_in, UINT templatename=160 );
//  CContainer_main( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename );
	~CContainer_main(void);

  CFont font;

//	FCObjImage   logo;
	CStatic*     date_time;
	CStatic*     PCname_st;
  CStatic*     User_st;
  CStatic*     Recipe_st;
	CButton*     logo_button;

	DECLARE_MESSAGE_MAP()
	//afx_msg void OnPaint();
	afx_msg void OnBnClickedLogoBtn();
	//afx_msg void OnBnClickedUserBtn();
	//afx_msg void OnStnClicked();

  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
