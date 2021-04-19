#pragma once


// CContainer_wnd
class CParameter;
class CMenu_button;

// *class CContainer_wnd : public CWnd
class CContainer_wnd : public CDialog
{
	DECLARE_DYNAMIC(CContainer_wnd)
	enum mode_type { top_left, top_right };

public:
	//afx_msg void OnPaint();
//	CContainer_wnd( char* text_in, RECT rett_in, CWnd* parent_in );
  //CContainer_wnd( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER */);
  CContainer_wnd( wchar_t* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER */);
//  CContainer_wnd( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename );

	virtual ~CContainer_wnd();

	void   Close(void); // chiudi/nascondi (con eventuali figli)
//  void   Add_child( CContainer_wnd* child ); // aggiunge un figlio all'elenco
  void   Add_child( CMenu_button* child ); // aggiunge un figlio all'elenco

//	CContainer_wnd*  children[20];
	CMenu_button*   children[20];
	int             n_children;

	CParameter*     parameter[30]; // Elenco puntatori agli elementi contenuti da aggiornare alla visualizz
	int             n_params; 

	CWnd*           parent;
	//char*           label;
	wchar_t*        label;
	int             level;      // depth level of container (0=topmost, 1=his child)
	CFont           font;
	RECT            rett;       // coordinate di costruzione dell'oggetto  
	int             bordo;      // bordo che viene mantenuto dalle pareti e tra 1 oggetto e il successivo
  int             spcy;       // Intervallo in verticale tra un oggetto e il successivo
	int             posy;       // altezza progressivamente decrescente di creazione oggetti
	static UINT     nID;
	static BOOL     use_ext_rect;  // Se TRUE next_rect fornisce ext_rect
	static RECT     ext_rect;

	static mode_type mode;

protected:
	DECLARE_MESSAGE_MAP()
public:
	RECT next_rect( SIZE dim_in );
//	mode_type mode; // current coordinates hook type
//	int new_size( int dx, int dy, mode_type mode=topright );
	int new_size( int dx, int dy );
	int Add_parameter(CParameter* param );
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


