#pragma once


// CInfo_box dialog

class CInfo_box : public CDialog
{
	DECLARE_DYNAMIC(CInfo_box)

public:
	CInfo_box(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo_box();

// Dialog Data
	enum { IDD = IDD_INFO_BOX };

	CFont font;

	HBITMAP  hbmp_logo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
