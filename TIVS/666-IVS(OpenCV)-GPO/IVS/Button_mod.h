#pragma once


// CButton_mod

//class CParameter;

class CButton_mod : public CButton
{
	DECLARE_DYNAMIC(CButton_mod)

public:
	CButton_mod();
	virtual ~CButton_mod();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	CWnd* parent;
//	CParameter* parent;

	COLORREF text_col;
	COLORREF back_col;
//	CBrush   brush;

  BOOL  add_onoff;



protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClicked();
//	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


