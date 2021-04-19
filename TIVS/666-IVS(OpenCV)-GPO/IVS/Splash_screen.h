#pragma once

#include "Globals.h"

// finestra di dialogo CSplash_screen

class CSplash_screen : public CDialog
{
	DECLARE_DYNAMIC(CSplash_screen)

public:
	CSplash_screen(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CSplash_screen();

  CStatic myStatic;
  HBITMAP bitmap;

// Dati della finestra di dialogo
	enum { IDD = IDD_INFO_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
