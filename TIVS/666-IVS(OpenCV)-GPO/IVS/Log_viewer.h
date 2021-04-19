#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "MainFrm.h"
#include "Globals.h"

// CLog_viewer dialog

#define MAX_LOG_SIZE 1000000


class CLog_viewer : public CDialog
{
	DECLARE_DYNAMIC(CLog_viewer)

public:
	CLog_viewer(CWnd* pParent = NULL);   // standard constructor
//	CLog_viewer(CString &name, CWnd* pParent = NULL );   // constructor
	CLog_viewer( wchar_t* name, CWnd* pParent = NULL );   // constructor
	virtual ~CLog_viewer();
 // int CLog_viewer::Open_file( CString &name );

  int Load_and_show( wchar_t* file );

  CString file_name;
  CEdit*  editptr;

// Dialog Data
//	enum { IDD = IDD_DIALOG3 };
	enum { IDD = IDD_LOG_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnBnClickedScrollup();
  afx_msg void OnBnClickedScrolldn();
  afx_msg void OnBnClickedSaveTxt();
  afx_msg void OnBnClickedLoadLog();
};
