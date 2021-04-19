#pragma once

#include "Container_wnd.h"


// CMenu_button
#define MB_WINDOW    2 // Tipo finestra che ospita ulteriori pulsanti
#define MB_FUNCTION  4 // Tipo pulsante che richiama una funzione
#define MB_EXT_WND   8 // Tipo pulsante che richiama una finestra esterna


//class CContainer_wnd;

class CMenu_button : public CButton
{
	DECLARE_DYNAMIC(CMenu_button)

	enum hook_type { bottom_left, top_right };

public:

  void Common_init( void ); // Parte di inizializzazione comune

	static hook_type hook_pos;
	static SIZE      child_dim;

	//CMenu_button( char* text_in, CContainer_wnd* container_in );
  //CMenu_button( char* text_in, CContainer_wnd* container_in, int (*funct_in)(int) );
  //CMenu_button( char* text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in );
	
  CMenu_button( wchar_t* text_in, CContainer_wnd* container_in );
  CMenu_button( wchar_t* text_in, CContainer_wnd* container_in, int (*funct_in)(int), int index_in=0 );
  CMenu_button( wchar_t* text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in );

//  CMenu_button( wchar_t** text_in, CContainer_wnd* container_in );
//  CMenu_button( wchar_t** text_in, CContainer_wnd* container_in, int (*funct_in)(int) );
//  CMenu_button( wchar_t** text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in );


  void   Show( void );
  void   Hide( void );


	//char* label;       // testo/etichetta (puntatore)
	//char* child_name;  // testo/etichetta del contenitore figlio (se esiste)
	wchar_t*  label;       // testo/etichetta (puntatore)
	wchar_t*  child_name;  // testo/etichetta del contenitore figlio (se esiste)

  CFont font;

	int   type;        // MB_WINDOW o MB_FUNCTION
  int   index;       // to be passed to function on calling
	BOOL  checked;
	BOOL  enabled;

//	CWnd* parent; // puntatore alla finestra madre
	CContainer_wnd* parent; // puntatore al contenitore padre
	CContainer_wnd* child;  // puntatore al contenitore figlio
	CWnd*           window; // puntatore alla finestra figlia (se type=MB_EXT_WND)
//	RECT  wnd_rect; // area e dimensioni della finestra figlia

	virtual ~CMenu_button();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

//	static CFont font;
	static UINT  nID;  // nID progressivo

protected:
	DECLARE_MESSAGE_MAP()

	int (*function)(int); // function to call in case of button push

public:
	afx_msg void OnBnClicked();
};


