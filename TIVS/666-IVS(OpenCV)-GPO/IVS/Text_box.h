#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////


class CText_box : public CStatic
{
// Construction
public:

  CText_box( RECT& area_in, CWnd* parent, DWORD stili_add = 0 , DWORD stili_ex_add = 0 );
  CText_box( int dim, LPCTSTR name, RECT& area_in, CWnd* par, DWORD stili_add = 0 , DWORD stili_ex_add = 0 );
	
	
//  CText_box( RECT& area_in , CWnd* parent , CFont* , DWORD stili_add = 0 , DWORD stili_ex_add = 0 );
//  CText_box( Dimensioni_immagine& area_in , CWnd* parent , CFont* , DWORD stili_add = 0 , DWORD stili_ex_add = 0 );

  CWnd*     parent;
  CString   testo;
  RECT      area;          // Area di visualizzazione
  COLORREF  colore_sfondo; // Colori del testo
  COLORREF  colore_testo;  // e dello sfondo
  CBrush    penn_sfondo;
  //static CFont* font;
  //static    CFont font;
  CFont     font;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CText_box)
	//}}AFX_VIRTUAL

// Implementation
public:

//  void SetText( char**   stringa );
//	void SetText( char*    stringa );
	void SetText( wchar_t*    stringa );
	void SetText( CString& stringa );
//  void SetText( char**   stringa , COLORREF col_testo );
//	void SetText( char*    stringa , COLORREF col_testo );
	void SetText( wchar_t* stringa , COLORREF col_testo );
	void SetText( CString& stringa , COLORREF col_testo );

//  void SetText( char*    stringa, COLORREF col_testo, COLORREF col_sfondo );
  void SetText( wchar_t* stringa, COLORREF col_testo, COLORREF col_sfondo );
  void SetText( CString& stringa, COLORREF col_testo, COLORREF col_sfondo );

	virtual ~CText_box();
	//virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );


/*
  void SetText( char**   stringa );
	void SetText( char*    stringa );
	void SetText( CString& stringa );
  void SetText( char**   stringa , COLORREF col_testo );
	void SetText( char*    stringa , COLORREF col_testo );
	void SetText( CString& stringa , COLORREF col_testo );
	virtual ~CText_box();
*/
	// Generated message map functions
protected:
	//{{AFX_MSG(CText_box)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	CBrush m_brBkgnd;

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnPaint();
};
