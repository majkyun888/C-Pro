#pragma once

#include "resource.h"       // main symbols

#define MAX_LENGHT 10 // massima lunghezza accettata
#define MAX_NOTE_LENGTH  80 

// CInput_figures dialog

class CInput_figures : public CDialog
{
	DECLARE_DYNAMIC(CInput_figures)

public:
	CInput_figures(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInput_figures();

	CFont font1; // bigger for digits
	CFont font2; // smaller for buttons

	int curr_val; // valore attuale
//	char buffer[MAX_LENGHT+1];
	wchar_t buffer[MAX_LENGHT+1];
	int  buff_len;

	int read_string( wchar_t* buffer, wchar_t* caption );
	wchar_t note[MAX_NOTE_LENGTH+1];

// Dialog Data
	enum { IDD = IDD_INPUT_FIGURES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonBkspc();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonDot();
	afx_msg void OnBnClickedButtonChngSgn();
//	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
};
