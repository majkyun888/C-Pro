// Zeus.h : main header file for the Zeus application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


class CIVS : public CWinApp
{
public:
	CIVS();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIVS theApp;