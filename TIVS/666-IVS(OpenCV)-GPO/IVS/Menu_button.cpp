// Menu_button.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Menu_button.h"
#include "Def_globali.h"
//#include "Container_main.h"
#include "Globals.h"
#include "strings.h"


// CMenu_button
IMPLEMENT_DYNAMIC(CMenu_button, CButton)

//CFont CMenu_button::font;
UINT  CMenu_button::nID;

CMenu_button::hook_type CMenu_button::hook_pos  = CMenu_button::bottom_left;
SIZE                    CMenu_button::child_dim = { 320 , 300 };

//#include "Recipe_manager.h"
//extern CRecipe_manager* recipe_wnd;

extern UINT  lng;
//extern CContainer_main* container_main;

extern CChildView*         pView;
//extern CPassword_manager*  password_mngr;

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif

void CMenu_button::Common_init( void )
{

  ASSERT( pView->num_menu_btn < MAX_NUM_MENU_BTN-1 ); // MAX NUMBER REACHED: INCREASE MAX_NUM_MENU_BTN

	type       =   -1; // Non inizializzato
	child_name = NULL;
	label      = NULL;
	parent     = NULL;
	child      = NULL;
	function   = NULL;
	window     = NULL;
	checked    = FALSE;
  enabled    = TRUE;


  int dimch = 20; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 20;

  font.CreateFont( dimch,                         // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                            // Orientation
                   400,                           // Weight 100-900
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );

}


// PRIMO COSTRUTTORE CHE OSPITA UNA FINESTRA FIGLIA CONTENITORE (type=MB_WINDOW) 
//CMenu_button::CMenu_button( char* text_in, RECT rett, CWnd* parent_in )
//CMenu_button::CMenu_button( char* text_in, CContainer_wnd* container_in )
CMenu_button::CMenu_button( wchar_t* text_in, CContainer_wnd* container_in )
//CMenu_button::CMenu_button( wchar_t** text_in, CContainer_wnd* container_in )
{
	Common_init();

	ASSERT( AfxIsValidString( text_in ) );
	ASSERT( container_in && container_in->m_hWnd );

	label  = text_in;
	parent = container_in;

	// Creazione del pulsante
  SIZE dim = { 170, 40 }; 
//  VERIFY( Create( label, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, parent->next_rect(dim), parent, nID++ ) );
  VERIFY( Create( label, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, parent->next_rect(dim), parent, nID++ ) );

/*  if ( !font.m_hObject )
	{
		CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    // VERIFY(font.CreatePointFont(140, L"Arial", &dc));
    //VERIFY(font.CreatePointFont( 140, L"DejaVu Sans Condensed", &dc ) );
    VERIFY( font.CreatePointFont( MAIN_BTN_FONT, &dc ) );
	}*/


	SetFont( &font );

	// QUESTO MANTIENE DIMENSIONI E POSIZIONE DELLA FINESTRA
	CRect rect_wnd;
	parent->GetWindowRect( &rect_wnd );
  //	rect_wnd.OffsetRect( OFFS_COORDX , OFFS_COORDY ); // necessaria per evitare l'offset della cornice

	// IN BASE ALL'IMPOSTAZIONE DI pos_hook la finestra child viene creata così:
	// bottom_left => child sotto e a destra dell'angolo inferiore sinistro del padre
	// top_right   => child a destra dell'angolo superiore destro del padre

	CRect rect_child;
	switch ( hook_pos )
	{
    case top_right:
  	  rect_child.top    = rect_wnd.top    ; //   - 4;
	    rect_child.left   = rect_wnd.right  ; //   - 2;
			rect_child.right  = rect_child.left  + child_dim.cx;
	    rect_child.bottom = rect_child.top   + child_dim.cy;
  		break;

    case bottom_left:
	    rect_child.top    = rect_wnd.bottom ; // -   2;
	    rect_child.left   = rect_wnd.left   ; // -   4;
		  rect_child.right  = rect_child.left  + child_dim.cx;
	    rect_child.bottom = rect_child.top   + child_dim.cy;
    	break;
	}

//	rect_child.OffsetRect( OFFS_COORDX , OFFS_COORDY ); // necessaria per evitare l'offset della cornice


/*
  #ifdef CHILD_ALIGN_LEFT
	  rect_child.top    = rect_wnd.top;
	  rect_child.right  = rect_wnd.left      - 2;
	  rect_child.left   = rect_child.right - 200;
	  rect_child.bottom = rect_child.top   + 300;
  #endif

  #ifdef CHILD_ALIGN_BELOW
	  rect_child.top    = rect_wnd.bottom  -   2;
	  rect_child.left   = rect_wnd.left    -   4;
		rect_child.right  = rect_child.left  + 200;
	  rect_child.bottom = rect_child.top   + 300;
  #endif
*/

//  CString msg;
//	msg.Format("contenitore associato al tasto %s", label);

	//child_name = (char*) malloc( 50 );
	//sprintf( child_name, "figlio di %s", label );
	child_name = (wchar_t*) malloc( 150 );
	swprintf( child_name, L"figlio di %s", label );

	child = new CContainer_wnd( child_name, rect_child, parent->parent, IDD_CONTAINER );
	child->ShowWindow( SW_HIDE );
//	parent->Add_child( child );
	parent->Add_child( this );



  

  // if necessary, increase height of parent window
  CRect rect;
  GetWindowRect( &rect );

  CRect parect;
  parent->GetWindowRect( &parect );

  int downmargin = 10;
  int deltah = rect.bottom-parect.bottom+downmargin;
  if ( deltah > 0 && container_in!=pView->container_main )
  {
    //TRACE("CMenu_button: Increasing height by %d\n", deltah );
    parect.InflateRect( 0, 0, 0, deltah );
    parent->MoveWindow( &parect );

    TRACE( L"CMenu_button: increasing height of %s by %d\n", label, deltah );
  }


  /*
  // if necessary, increase height of parent window
  CRect parect;
  parent->GetWindowRect( &parect );
  //parent->GetClientRect( &parect );

  int downmargin = 20;
  int deltah = (rect_child.bottom-rect_child.top)+downmargin-parect.Height();
  if ( deltah > 0 )
  {
    TRACE("CMenu_button: Increasing height by %d\n", deltah );
    parect.InflateRect( 0, 0, 0, deltah );
    parent->MoveWindow( &parect );
  }
  */


	// Dimensioni iniziali dx=112 - dy=27
//	wnd_rect.left   = wnd_rect.right - 112; // MANTENERE ALMENO 112 ALTRIMENTI SI PERDE L'ALLINEAMENTO (DIMX MINIMA)
//	wnd_rect.bottom = wnd_rect.top   +  27; // MANTENERE ALMENO  27 ALTRIMENTI SI PERDE L'ALLINEAMENTO (DIMY MINIMA)
//	window = new CWnd;
//  VERIFY( window->Create( "STATIC", NULL, WS_CHILD|WS_BORDER|WS_THICKFRAME, wnd_rect, parent, nID++ ) );

	type = MB_WINDOW;

 // font.DeleteObject();


	//TRACE("Creato CMenu_button <%s> di tipo MB_WINDOW\n", label );
}








// SECONDO COSTRUTTORE ASSOCIATO DIRETTAMENTE AD UNA FUNZIONE (type=MB_FUNCTION) 
CMenu_button::CMenu_button( wchar_t* text_in, CContainer_wnd* container_in, int (*funct_in)(int), int index_in )
{

	Common_init();

	ASSERT( AfxIsValidString( text_in ) );
	//ASSERT( parent_in && parent_in->m_hWnd );
	ASSERT( container_in && container_in->m_hWnd );


//  ASSERT( index_in>=0 && index_in<NUM_TLC );
  index = index_in;

	label  = text_in;
//	parent = parent_in;
	parent = container_in;

  SIZE dim = { 170, 40 }; 

  Create( label, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, parent->next_rect(dim), parent, nID++ );

/*  if ( !font.m_hObject )
	{
  	CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    //VERIFY(font.CreatePointFont( 100, L"Arial", &dc));
    VERIFY( font.CreatePointFont( MAIN_BTN_FONT, &dc ) );
	}
	if ( font.m_hObject ) SetFont( &font );*/

  SetFont( &font );

	ASSERT( funct_in );
	function = funct_in;

	type = MB_FUNCTION;


  // if necessary, increase height of parent window
  CRect rect;
  GetWindowRect( &rect );

  CRect parect;
  parent->GetWindowRect( &parect );

  int downmargin = 10;
  int deltah = rect.bottom-parect.bottom+downmargin;
  if ( deltah > 0 )
  {
    //TRACE("CMenu_button: Increasing height by %d\n", deltah );
    parect.InflateRect( 0, 0, 0, deltah );
    parent->MoveWindow( &parect );
  }

  //font.DeleteObject();

	//TRACE("Creato CMenu_button <%s> di tipo MB_FUNCTION\n", label );
}


// TERZO COSTRUTTORE CHE OSPITA UNA FINESTRA FIGLIA COSTRUITA ESTERNAMENTE (type=MB_WINDOW) 
//CMenu_button::CMenu_button( char* text_in, RECT rett, CWnd* parent_in )
//CMenu_button::CMenu_button( char* text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in )
CMenu_button::CMenu_button( wchar_t* text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in )
//CMenu_button::CMenu_button( wchar_t** text_in, CContainer_wnd* container_in, /*CWnd**/ CDialog* wnd_in )
{
	Common_init();

	ASSERT( AfxIsValidString( text_in ) );
	ASSERT( container_in && container_in->m_hWnd );
	ASSERT( wnd_in && wnd_in->m_hWnd );

	label  = text_in;
	parent = container_in;
	window = wnd_in;

	// Creazione del pulsante
  SIZE dim = { 170, 40 }; 
  VERIFY( Create( label, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, parent->next_rect(dim), parent, nID++ ) );
	window->ShowWindow( SW_HIDE );

	parent->Add_child( this );
	
/*  if ( !font.m_hObject )
	{
		CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    //VERIFY(font.CreatePointFont(100, L"Arial", &dc));
    VERIFY( font.CreatePointFont( MAIN_BTN_FONT, &dc ) );
	}*/



	SetFont( &font );

	// QUESTO MANTIENE DIMENSIONI E POSIZIONE DELLA FINESTRA
	CRect rect_wnd;
	parent->GetWindowRect( &rect_wnd );
//	rect_wnd.OffsetRect( OFFS_COORDX , OFFS_COORDY ); // necessaria per evitare l'offset della cornice

	CRect rect_child;
	window->GetWindowRect( &rect_child );


	// IN BASE ALL'IMPOSTAZIONE DI pos_hook la finestra child viene creata così:
	// bottom_left => child sotto e a destra dell'angolo inferiore sinistro del padre
	// top_right   => child a destra dell'angolo superiore destro del padre


	switch ( hook_pos )
	{
    case top_right:
			rect_child.OffsetRect( rect_wnd.right-rect_child.left+2, rect_wnd.top-rect_child.top );
/*  	  rect_child.top    = rect_wnd.top       - 4;//  -   2;
	    rect_child.left   = rect_wnd.right     - 2; //    -   4;
			rect_child.right  = rect_child.left  + child_dim.cx;
	    rect_child.bottom = rect_child.top   + child_dim.cy; */
  		break;

    case bottom_left:
	    rect_child.top    = rect_wnd.bottom  -   2;
	    rect_child.left   = rect_wnd.left    -   4;
		  rect_child.right  = rect_child.left  + child_dim.cx;
	    rect_child.bottom = rect_child.top   + child_dim.cy;
    	break;
	}

//	rect_child.OffsetRect( OFFS_COORDX , OFFS_COORDY ); // necessaria per evitare l'offset della cornice

	window->MoveWindow( &rect_child );

/*
  #ifdef CHILD_ALIGN_LEFT
	  rect_child.top    = rect_wnd.top;
	  rect_child.right  = rect_wnd.left      - 2;
	  rect_child.left   = rect_child.right - 200;
	  rect_child.bottom = rect_child.top   + 300;
  #endif

  #ifdef CHILD_ALIGN_BELOW
	  rect_child.top    = rect_wnd.bottom  -   2;
	  rect_child.left   = rect_wnd.left    -   4;
		rect_child.right  = rect_child.left  + 200;
	  rect_child.bottom = rect_child.top   + 300;
  #endif
*/

//  CString msg;
//	msg.Format("contenitore associato al tasto %s", label);

//	child_name = (char*) malloc( 50 );
//	sprintf( child_name, "figlio di %s", label );

//	child = new CContainer_wnd( child_name, rect_child, parent->parent );
//	child->ShowWindow( SW_HIDE );
//	parent->Add_child( child );
//	parent->Add_child( this );

	// Dimensioni iniziali dx=112 - dy=27
//	wnd_rect.left   = wnd_rect.right - 112; // MANTENERE ALMENO 112 ALTRIMENTI SI PERDE L'ALLINEAMENTO (DIMX MINIMA)
//	wnd_rect.bottom = wnd_rect.top   +  27; // MANTENERE ALMENO  27 ALTRIMENTI SI PERDE L'ALLINEAMENTO (DIMY MINIMA)
//	window = new CWnd;
//  VERIFY( window->Create( "STATIC", NULL, WS_CHILD|WS_BORDER|WS_THICKFRAME, wnd_rect, parent, nID++ ) );

	type = MB_EXT_WND;

//  font.DeleteObject();

	//TRACE("Creato CMenu_button <%s> di tipo MB_EXT_WND\n", text_in );
}









CMenu_button::~CMenu_button()
{
  font.DeleteObject();

	if ( child )
	{
		if ( child->m_hWnd ) child->DestroyWindow();
		delete child;
	}

	if ( child_name )
	{
	  free( child_name );
		child_name = NULL;
	}

	//TRACE("Distrutto oggetto di tipo CMenu_button <%s>\n", label );
}


BEGIN_MESSAGE_MAP(CMenu_button, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CMenu_button::OnBnClicked)
END_MESSAGE_MAP()



// CMenu_button message handlers


void CMenu_button::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	// TODO: Add your message handler code here and/or call default
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	 COLORREF col;
//	 char*    text_add;

   // If drawing selected, add the pushed style to DrawFrameControl.
   if ( enabled )
   {
     //uStyle &= ~DFCS_INACTIVE;
     if (checked) //(lpDrawItemStruct->itemState & ODS_SELECTED)
     {
       uStyle |= DFCS_PUSHED;
       col = RGB(200,0,0);
     }
     else                  
     {
       uStyle  &= ~DFCS_PUSHED;
       col      = RGB(0,0,0);
     }
   }
   else col = RGB(150,150,150);


/*   else
   {
     uStyle |= DFCS_INACTIVE;
     uStyle &= ~DFCS_PUSHED;
   } */

/*   if ( enabled )
   {
     uStyle &= ~DFCS_INACTIVE;
   }
   else
   {
		 uStyle |= DFCS_INACTIVE;
   } */


//   if ( enabled ) uStyle = DFCS_INACTIVE;
   

// Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);

   // Get the button's text.
   CString strText;
   GetWindowText(strText);
//	 strText.Append( text_add );

   // Draw the button text using the text color red.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, col);
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);


//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}



void CMenu_button::Show()
{
	if ( child  && child->m_hWnd  ) child->ShowWindow( SW_SHOW );
 	if ( window && window->m_hWnd ) window->ShowWindow( SW_SHOW );
	checked = TRUE;
	Invalidate();
}


void CMenu_button::Hide()
{
	if ( child && child->IsWindowVisible() ) child->Close();
 	if ( window && window->m_hWnd ) window->ShowWindow( SW_HIDE );
	checked = FALSE;
	Invalidate();
}




void CMenu_button::OnBnClicked()
{

  ASSERT( AfxIsValidString( label ) );
	TRACE( L"CMenu <%s> cliccato\n", label );

  if ( !enabled ) // return;  // ask authentication
  {
    if ( password_mngr )
    {
      password_mngr->authenticate_user();
      if ( !enabled ) return; //password_mngr->current_level
    }
  }





	// Se esiste ed è visibile la finestra delle ricette annullo e restituisco il fuoco
/*  if ( recipe_wnd && recipe_wnd->IsWindowVisible() )
	{
		recipe_wnd->SetFocus();
		return;
	} */

	// TODO: Add your control notification handler code here
	if ( type==MB_WINDOW || type==MB_EXT_WND ) 
	{
		if ( checked ) // Visibile: nascondo lui e tutti i suoi figli
		{
			Hide();			
		}
		else // Nascosto, nascondo fratelli e relativi figli e visualizzo
		{
			for ( int i=0 ; i<parent->n_children ; i++ ) parent->children[i]->Hide();
			Show();
		}
	}

	if ( type==MB_FUNCTION ) 
	{
		if ( function )
		{
			(*function)(index);
		}
	}
/*
	if ( type==MB_EXT_WND ) 
	{
		if ( checked ) // Visibile: nascondo lui e tutti i suoi figli
		{
			if ( window && window->m_hWnd ) window->ShowWindow( SW_HIDE );
    	checked = FALSE;
	    Invalidate();
      //	if ( child && child->IsWindowVisible() ) child->Close();
		}
		else // Nascosto, nascondo fratelli e relativi figli e visualizzo
		{
			for ( int i=0 ; i<parent->n_children ; i++ ) parent->children[i]->Hide();
			//Show();
     	if ( window && window->m_hWnd ) window->ShowWindow( SW_SHOW );
//	child->BringWindowToTop();
    	checked = TRUE;
	    Invalidate();
		}
	}
*/

//  CButton::OnBnClicked();
}
