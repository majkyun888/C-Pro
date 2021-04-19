#pragma once

#include "resource.h"       // main symbols

//#define MAX_LENGHT 20 // massima lunghezza accettata

#define CINPSTR_PASSWORD 100
// CInput_string dialog

class CInput_string : public CDialog
{
	DECLARE_DYNAMIC(CInput_string)

public:
//	CInput_string(CWnd* pParent = NULL);   // standard constructor
//	CInput_string( CWnd* pParent, char* initial, UINT lenght, UINT mode=0 );   // standard constructor
	CInput_string( CWnd* pParent, wchar_t* initial, UINT lenght, UINT mode=0 );   // standard constructor
	CInput_string(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInput_string();

  CFont font1;
  CFont font2;

//	int curr_val; // valore attuale
//	char buffer[MAX_LENGHT+1];
	
  //char* buffer;
	wchar_t* buffer;

  //  char* description;
	UINT  buff_lenght; // lunghezza attuale
	UINT  max_lenght;  // massima lunghezza ammessa
  BOOL  password;

	static BOOL maiuscolo;


  //BOOL noclose;

	void Update_wnd( void );

// Dialog Data
	enum { IDD = IDD_INPUT_STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonBkspc();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonUpLow();
protected:
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
  //afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnBnClickedButtonSpc();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButtonOk();
};
