#pragma once


// CStatic_mod

class CStatic_mod : public CStatic
{
	DECLARE_DYNAMIC(CStatic_mod)

public:

	CWnd* parent;

	CStatic_mod();
	virtual ~CStatic_mod();
	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	void SetBackColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text.

//	void Update_text( char* text_in, COLORREF text_color, COLORREF back_color );
	void Update_text( wchar_t* text_in, COLORREF text_color, COLORREF back_color );



protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnStnClicked();


//	char text_old[200]; 
	wchar_t text_old[200]; 

	CBrush m_brBkgnd;        // Holds Brush Color for the Static Text

	COLORREF Back_Color;       // Holds the Background Color for the Text
//	COLORREF Bk_Color_old;   // Holds the Background Color for the Text

	COLORREF Text_Color;     // Holds the Color for the Text
//	COLORREF Text_Color_old; // Holds the Color for the Text


	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};


