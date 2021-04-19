// Container_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Container_wnd.h"
//#include "Parameter.h"
//#include "MainFrm.h"
#include "Globals.h"

// CContainer_wnd

IMPLEMENT_DYNAMIC(CContainer_wnd, CWnd)

UINT                      CContainer_wnd::nID           =   108000;
CContainer_wnd::mode_type CContainer_wnd::mode          = top_left;
BOOL                      CContainer_wnd::use_ext_rect  =    FALSE;
RECT                      CContainer_wnd::ext_rect; //      =    FALSE;




BEGIN_MESSAGE_MAP(CContainer_wnd, CWnd)
//  ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
//	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// PRIMO COSTRUTTORE CHE OSPITA UNA FINESTRA FIGLIA CONTENITORE (type=MB_WINDOW) 
//CContainer_wnd::CContainer_wnd( char* text_in, RECT rett_in, CWnd* parent_in )
//CContainer_wnd::CContainer_wnd( char* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER*/ )
CContainer_wnd::CContainer_wnd( wchar_t* text_in, RECT rett_in, CWnd* parent_in, UINT templatename/*=IDD_CONTAINER*/ )
{

	ASSERT( AfxIsValidString( text_in ) );
	ASSERT( parent_in && parent_in->m_hWnd );
//	ASSERT( parent_in->IsWindow() );

	label  = text_in;
	parent = parent_in;
	rett   = rett_in;
	bordo  = 4;
	posy   = bordo;
  spcy   = 10;

  #ifdef MONITOR_SEC
    rett.left  += 1920;
    rett.right += 1920;
  #endif

	ZeroMemory( children, sizeof(children) );
	n_children = 0;

	ZeroMemory( parameter, sizeof(parameter) );
	n_params = 0;


//  VERIFY( Create( IDD_CONTAINER ) );
  VERIFY( Create( templatename ) );
	

//  ScreenToClient( &rett );
  ClientToScreen( &rett );
  MoveWindow( &rett );
//  SetWindowPos( &CWnd::wndTop, rett.left, rett.top, rett.right-rett.left+1, rett.bottom-rett.top+1, SWP_NOZORDER|SWP_SHOWWINDOW );

/*  if ( !font.m_hObject )
	{
  	CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    VERIFY(font.CreatePointFont(100, L"Arial", &dc));
	}*/


  font.CreateFont( 15,                            // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                             // Orientation
                   FW_SEMIBOLD,                   // Weight
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );


	SetFont( &font );
	//TRACE("Creato CContainer_wnd <%s>\n", label );

//  font.DeleteObject();

}




CContainer_wnd::~CContainer_wnd()
{
  font.DeleteObject();
	DestroyWindow();
	//TRACE("Distrutto CContainer_wnd <%s>\n", label );
}


void CContainer_wnd::Close( void )
{
	for ( int i=0 ; i<n_children ; i++ ) children[i]->Hide();
	ShowWindow( SW_HIDE );
}



void CContainer_wnd::Add_child( CMenu_button* child )
{
	ASSERT( child );
  ASSERT( n_children<20 );

	children[n_children++] = child;
}

/*
void CContainer_wnd::Add_child( CContainer_wnd* child )
{
	ASSERT( child );
  ASSERT( n_children<20 );

//	children[n_children++] = child;
}
*/

int CContainer_wnd::Add_parameter(CParameter* param )
{
	ASSERT( param );
  ASSERT( n_params<30 );

	parameter[n_params++] = param;

	return 0;
}

/*
void CContainer_wnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rett;
	GetClientRect( &rett );
	dc.FillSolidRect( &rett, COLORE_CONTAIN );
}
*/


void CContainer_wnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	int i;
	if ( bShow ) for ( i=0 ; i<n_params ; i++ ) parameter[i]->Update_val_text();
	CWnd::OnShowWindow(bShow, nStatus);
}






// CContainer_wnd message handlers

RECT CContainer_wnd::next_rect( SIZE dim_in )
{
	RECT rett;
	GetClientRect( &rett );

	if ( rett.right-rett.left-2*bordo < dim_in.cx )
	{
		CString msg;
		//msg.Format("Richiesto uno spazio in orizzontale %d al contenitore <%s>\r\nsuperiore a quello disponibile %d", dim_in.cx, label, rett.right-rett.left-2*bordo );
		msg.Format(L"Richiesto uno spazio in orizzontale %d al contenitore <%s>\r\nsuperiore a quello disponibile %d", dim_in.cx, label, rett.right-rett.left-2*bordo );
//		MessageBox( msg.GetBuffer() , "ATTENZIONE:" , MB_OK|MB_ICONWARNING );
		Message_box( msg, MBOX_WARNING );
	}

	if ( use_ext_rect ) return ext_rect;

	rett.left   =              bordo;
	rett.right  = rett.right - bordo;
	rett.top    = posy;
	rett.bottom = rett.top + dim_in.cy; 



/*
	switch ( mode )
	{
    case top_left:
			rett.left   = rett.right + bordo;
			rett.right  = rett.left  + dim_in.cx;
//			rett.top    = posy;
			rett.bottom = rett.top + dim_in.cy;
  		break;

    case top_right:
			rett.left   =              bordo;
			rett.right  = rett.right - bordo;
			rett.top    = posy;
			rett.bottom = rett.top + dim_in.cy;
    	break;
	}
*/

	posy += spcy + dim_in.cy;

	return rett;
}



// MODIFICA LE DIMENSIONI ADATTANDOLE ALLE NUOVE RICHIESTE
// E MANTENENDO L'AGGANCIO IN ALTO A DX
int CContainer_wnd::new_size( int dx, int dy )
{ 
	ASSERT( m_hWnd );
	ASSERT( dx>=100 && dx<1000 );
	ASSERT( dy>=90 && dy<1000 );//Pour changed from 100 to 90

	// Sposto il campo di visualizzazione sotto e lo allargo
	CRect rett;
	GetWindowRect( &rett ); // coordinate schermo

 // ClientToScreen( &rett );




	/*
  if ( mode == topright )
	{
   	rett.left    -= dx - (rett.right-rett.left); // Calcolo il nuovo rett in coordinate schermo
   	rett.bottom  += dy - (rett.bottom-rett.top);
	}
	*/

//	rett.OffsetRect( OFFS_COORDX , OFFS_COORDY ); // necessaria per evitare l'offset della cornice

	switch ( mode )
	{
    case top_left:
			rett.right   += dx - (rett.right-rett.left); // Calcolo il nuovo rett in coordinate schermo
     	rett.bottom  += dy - (rett.bottom-rett.top);
  		break;

    case top_right:
     	rett.left    -= dx - (rett.right-rett.left); // Calcolo il nuovo rett in coordinate schermo
     	rett.bottom  += dy - (rett.bottom-rett.top);
    	break;
	}


  parent->ScreenToClient( &rett );

	rett.OffsetRect( 1 , 1 ); // necessaria per evitare l'offset della cornice
  
  //  parent->ClientToScreen( &rett );
	MoveWindow( &rett ); // rispetto al padre

	return 0;
}



//void CContainer_wnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: Add your message handler code here and/or call default
//	CPaintDC dc(this); // device context for painting
//	RECT rett;
//	GetClientRect( &rett );
//	dc.FillSolidRect( &rett, COLORE_CONTAIN );
//
//	dc.TextOutA( 10, 10, "12345678901234567890", 20 );
//
//	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}
