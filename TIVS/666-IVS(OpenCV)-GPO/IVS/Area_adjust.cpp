// Area_adjust.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"

#include "Image_display.h"
 



// CArea_adjust dialog
//SIZE CArea_adjust::dim_wnd = { 300, 185 }; // Set window size
SIZE CArea_adjust::dim_wnd = { 320, 205 }; // Set window size


IMPLEMENT_DYNAMIC(CArea_adjust, CDialog)

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif

extern CLogger*         logger;

extern Camera_struct    camera   [NUM_TLC];
extern CImage_display*  image_dsp[NUM_TLC];

extern CChildView*      pView;
extern UINT             lng;  // Lingua attuale
extern UINT             thread_status;



//CArea_adjust::CArea_adjust(CWnd* pParent /*=NULL*/) : CDialog(CArea_adjust::IDD, pParent)
//CArea_adjust::CArea_adjust( CWnd* pParent, UINT templatename )	: CContainer_wnd( L"AREA ADJUST", CRect(0,0,dim_wnd.cx,dim_wnd.cy), pParent, templatename=IDD_AREA_DIALOG )
//CArea_adjust::CArea_adjust( CWnd* pParent, int index_in, CFile_mngmt* file_manager )	: CContainer_wnd( L"AREA ADJUST", CRect(0,0,dim_wnd.cx,dim_wnd.cy), pParent, IDD_AREA_DIALOG )

CArea_adjust::CArea_adjust( int index_in, CFile_mngmt* file_manager )	: CContainer_wnd( L"AREA ADJUST", CRect(0,0,dim_wnd.cx,dim_wnd.cy), pView, IDD_AREA_DIALOG )
	
//#ifdef TIVS_S40 //Pour marked on 20200921
//  CArea_adjust::CArea_adjust( int index_in, CFile_mngmt* file_manager )	: CContainer_wnd( L"AREA ADJUST", CRect(0,0,dim_wnd.cx,dim_wnd.cy), pView, IDD_AREA_DIALOGSM )
//#else
//  CArea_adjust::CArea_adjust( int index_in, CFile_mngmt* file_manager )	: CContainer_wnd( L"AREA ADJUST", CRect(0,0,dim_wnd.cx,dim_wnd.cy), pView, IDD_AREA_DIALOG )
//#endif

{
  ASSERT( index_in>=0 && index_in<NUM_TLC );
	ASSERT( WM_LBUTTONDOWN == 513 );
	ASSERT( WM_LBUTTONUP   == 514 );

	index  = index_in;
	//parent = NULL;//(CMenu_button*)pParent;

  if ( file_manager ) file_manager->Add_data( &buffrect, sizeof(buffrect), RECT_TYPE );

  color  = COL_RED;

  ASSERT( image_dsp[index] ); // must be already available!
  
  buffrect.left   =     image_dsp[index]->dim_buf.cx / 3;
  buffrect.right  = 2 * image_dsp[index]->dim_buf.cx / 3;
  buffrect.top    =     image_dsp[index]->dim_buf.cy / 3;
  buffrect.bottom = 2 * image_dsp[index]->dim_buf.cy / 3;
  
  limrect.top    =                              2;
  limrect.left   =                              2;
  limrect.bottom = image_dsp[index]->dim_buf.cy-2;
  limrect.right  = image_dsp[index]->dim_buf.cx-2;

  minsize.cx = 40;
  minsize.cy = 40;

  ptr_dsp = NULL;

	// Load and assign bitmaps to buttons
	HBITMAP  hbmp;
	CButton* btn;

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ARROW_UP));
	btn = (CButton*) GetDlgItem( ID_MOVE_UP );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ARROW_RIGHT));
	btn = (CButton*) GetDlgItem( ID_MOVE_RIGHT );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ARROW_DOWN));
	btn = (CButton*) GetDlgItem( ID_MOVE_DOWN );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ARROW_LEFT));
	btn = (CButton*) GetDlgItem( ID_MOVE_LEFT );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_HOR_ENLARGE));
	btn = (CButton*) GetDlgItem( ID_HOR_ENLARGE );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_HOR_REDUCE));
	btn = (CButton*) GetDlgItem( ID_HOR_REDUCE );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_VER_ENLARGE));
	btn = (CButton*) GetDlgItem( ID_VER_ENLARGE );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

	hbmp = LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_VER_REDUCE));
	btn = (CButton*) GetDlgItem( ID_VER_REDUCE );
	if ( btn && hbmp ) btn->SetBitmap(hbmp);

  move_up     = 0;
  move_down   = 0;
  move_left   = 0;
  move_right  = 0;
  hor_enlarge = 0;
  hor_reduce  = 0;
  ver_enlarge = 0;
  ver_reduce  = 0;

  if ( pView->parcat_1 && AfxIsValidString(pView->parcat_1) )
  {
    parcat_1 = (wchar_t*) malloc( (20+1) * sizeof(wchar_t) );
    ZeroMemory( parcat_1, sizeof(parcat_1) );
    swprintf( parcat_1, 20, pView->parcat_1 );
  }
  else parcat_1 = NULL;

  if ( pView->parcat_2 && AfxIsValidString(pView->parcat_2) )
  {
    parcat_2 = (wchar_t*) malloc( (20+1) * sizeof(wchar_t) );
    ZeroMemory( parcat_2, sizeof(parcat_2) );
    swprintf( parcat_2, 20, pView->parcat_2 );
  }
  else parcat_2 = NULL;


}

CArea_adjust::~CArea_adjust()
{

  if ( parcat_1 ) 
  {
    delete parcat_1;
    parcat_1 = NULL;
  }

  if ( parcat_2 ) 
  {
    delete parcat_2;
    parcat_2 = NULL;
  }

  if ( ptr_dsp ) 
  {
    free( ptr_dsp );
    ptr_dsp = NULL;
  }

}

void CArea_adjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//  DDX_Control(pDX, IDC_LEVEL, m_level);
}


BEGIN_MESSAGE_MAP(CArea_adjust, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_CLOSE, &CArea_adjust::OnBnClickedClose)
//	ON_BN_CLICKED(ID_INDEX, &CArea_adjust::OnBnClickedIndex)
//  ON_BN_CLICKED(ID_X10,   &CArea_adjust::OnBnClickedX10)
//  ON_BN_CLICKED(IDC_LEVEL, &CArea_adjust::OnBnClickedLevel)
  ON_WM_TIMER()
//  ON_WM_PAINT()
END_MESSAGE_MAP()




// CArea_adjust message handlers


void CArea_adjust::OnShowWindow(BOOL bShow, UINT nStatus)
{

 // CRect pos;

  /*
  if ( CHK_BIT( thread_status, 10+index ) )
  {
//    this->ShowWindow( SW_HIDE );
    Message_box( string_2022[lng], MBOX_INFO );
    //::ShowWindow( this->m_hWnd, SW_HIDE );

    OnBnClickedClose();
    return;
  }
  */
  


  if ( !bShow )
  {
    /* if ( ptr_dsp ) 
    {
      free( ptr_dsp );
      ptr_dsp = NULL;
    } */
    return;
  } 


  CString txt;

  if ( parcat_1 && AfxIsValidString(parcat_1) && parcat_2 && AfxIsValidString(parcat_2) )
  {
    txt.Format( L"Changing camera <%s> <%s> inspection area", parcat_1, parcat_2 );
    if ( logger ) logger->Add_string( &txt );
  }


//  SCALE_IMG( index, NULL ); // rescale last image!
//  image_dsp[index]->Draw_image(TRUE);

  // first backup current dsp image
  if ( ptr_dsp==NULL ) ptr_dsp = (int*) malloc( image_dsp[index]->dsp_size );
  CopyMemory( ptr_dsp, image_dsp[index]->ptr_dsp, image_dsp[index]->dsp_size );

  pView->Close_all_menus();

  if ( camera[index].modedsp==DISPROT_000 || camera[index].modedsp==DISPLAY_COL )
  {
    move_up     = GetDlgItem( ID_MOVE_UP     )->m_hWnd;
    move_down   = GetDlgItem( ID_MOVE_DOWN   )->m_hWnd;
    move_left   = GetDlgItem( ID_MOVE_LEFT   )->m_hWnd;
    move_right  = GetDlgItem( ID_MOVE_RIGHT  )->m_hWnd;
    hor_enlarge = GetDlgItem( ID_HOR_ENLARGE )->m_hWnd;
    hor_reduce  = GetDlgItem( ID_HOR_REDUCE  )->m_hWnd;
    ver_enlarge = GetDlgItem( ID_VER_ENLARGE )->m_hWnd;
    ver_reduce  = GetDlgItem( ID_VER_REDUCE  )->m_hWnd;
  }

  if ( camera[index].modedsp == DISPROT_090 || camera[index].modedsp==DISPLAY_COL90 )
  {
    move_up     = GetDlgItem( ID_MOVE_RIGHT  )->m_hWnd;
    move_down   = GetDlgItem( ID_MOVE_LEFT   )->m_hWnd;
    move_left   = GetDlgItem( ID_MOVE_UP     )->m_hWnd;
    move_right  = GetDlgItem( ID_MOVE_DOWN   )->m_hWnd;
    hor_enlarge = GetDlgItem( ID_VER_ENLARGE )->m_hWnd;
    hor_reduce  = GetDlgItem( ID_VER_REDUCE  )->m_hWnd;
    ver_enlarge = GetDlgItem( ID_HOR_ENLARGE )->m_hWnd;
    ver_reduce  = GetDlgItem( ID_HOR_REDUCE  )->m_hWnd;
  }

  if ( camera[index].modedsp == DISPROT_180 || camera[index].modedsp==DISPLAY_COL180 )
  {
    move_up     = GetDlgItem( ID_MOVE_DOWN   )->m_hWnd;
    move_down   = GetDlgItem( ID_MOVE_UP     )->m_hWnd;
    move_left   = GetDlgItem( ID_MOVE_RIGHT  )->m_hWnd;
    move_right  = GetDlgItem( ID_MOVE_LEFT   )->m_hWnd;
    hor_enlarge = GetDlgItem( ID_HOR_ENLARGE )->m_hWnd;
    hor_reduce  = GetDlgItem( ID_HOR_REDUCE  )->m_hWnd;
    ver_enlarge = GetDlgItem( ID_VER_ENLARGE )->m_hWnd;
    ver_reduce  = GetDlgItem( ID_VER_REDUCE  )->m_hWnd;
  }

  if ( camera[index].modedsp == DISPROT_270 || camera[index].modedsp==DISPLAY_COL270 )
  {
    move_up     = GetDlgItem( ID_MOVE_LEFT   )->m_hWnd;
    move_down   = GetDlgItem( ID_MOVE_RIGHT  )->m_hWnd;
    move_left   = GetDlgItem( ID_MOVE_DOWN   )->m_hWnd;
    move_right  = GetDlgItem( ID_MOVE_UP     )->m_hWnd;
    hor_enlarge = GetDlgItem( ID_VER_ENLARGE )->m_hWnd;
    hor_reduce  = GetDlgItem( ID_VER_REDUCE  )->m_hWnd;
    ver_enlarge = GetDlgItem( ID_HOR_ENLARGE )->m_hWnd;
    ver_reduce  = GetDlgItem( ID_HOR_REDUCE  )->m_hWnd;
  }



  ASSERT( move_up     );
  ASSERT( move_down   );
  ASSERT( move_left   );
  ASSERT( move_right  );
  ASSERT( hor_enlarge );
  ASSERT( hor_reduce  );
  ASSERT( ver_enlarge );
  ASSERT( ver_reduce  );





  TRACE("Showing area[%d]\n", index );

  Draw_area();


/*  int i=0;

	CContainer_wnd::OnShowWindow(bShow, nStatus);
	parent->Hide();

  
	// TODO: Add your message handler code here
	if ( bShow ) // If being showed: 
	{

    for ( i=0 ; i<NUM_TLC ; i++ )
    {
      if ( buffdsp[i]==NULL ) buffdsp[i] = (BYTE*) malloc( image_dsp[i]->dsp_size );
      ASSERT( buffdsp[i] );
      CopyMemory( buffdsp[i], image_dsp[i]->ptr_dsp, image_dsp[i]->dsp_size );
    }


		pView->Close_all_menus(); // close all menus

		int ip = 0;
  	
	  MoveWindow( 700, 476, dim_wnd.cx, dim_wnd.cy );
    
		Draw_area( index, TRUE, FALSE );
		//pView->Invalidate();

	}
  else // If being hidden
  {
    for ( i=0 ; i<NUM_TLC ; i++ )
    {
      ASSERT( buffdsp[i] );
      CopyMemory( image_dsp[i]->ptr_dsp, buffdsp[i], image_dsp[i]->dsp_size );
      image_dsp[i]->Draw_image();
    }
  }  */
}


void CArea_adjust::OnBnClickedClose()
{
	ShowWindow( SW_HIDE );
  //TRACE( "areainsp[%d] closed by user command\n", index );
  //SCALE_IMG( index, NULL ); // rescale last image!
  
  CString txt;
  txt.Format( L"new area coordinates (%d,%d)-(%d,%d)", buffrect.left, buffrect.top, buffrect.right, buffrect.bottom );
  if ( logger ) logger->Add_string( &txt );


  
  if ( image_dsp[index]->ptr_dsp && ptr_dsp )
  {
    CopyMemory( image_dsp[index]->ptr_dsp, ptr_dsp, image_dsp[index]->dsp_size );
  }

//  image_dsp[index]->Draw_image(TRUE);
  image_dsp[index]->Draw_image();

  if ( ptr_dsp ) 
  {
    free( ptr_dsp );
    ptr_dsp = NULL;
  }
}



BOOL CArea_adjust::PreTranslateMessage(MSG* pMsg)
{
  //TRACE("CArea_adjust::PreTranslateMessage hwnd=%d - message=%d - lParam=%d - wParam=%d\n", pMsg->hwnd, pMsg->message, pMsg->lParam, pMsg->wParam );

  if ( pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONUP ) // 513 LBUTTONDOWN / 514 LBUTTONUP
  {
    //TRACE("CArea_adjust::PreTranslateMessage hwnd=%d - message=%d - lParam=%d - wParam=%d\n", pMsg->hwnd, pMsg->message, pMsg->lParam, pMsg->wParam );
    //if ( pMsg->message==WM_LBUTTONDOWN ) TRACE("CArea_adjust: LBUTTONDOWN Tasto UP\n");
    //else                                 TRACE("CArea_adjust: LBUTTONUP   Tasto UP\n");

    //int dir = GetDlgItem( ID_MOVE_UP )->m_hWnd;

/*    if ( camera[index].modedsp == DISPROT_000 )
    {
      if ( pMsg->hwnd == GetDlgItem( ID_MOVE_UP     )->m_hWnd ) Manage_command( pMsg->message, MOVE_UP     );
      if ( pMsg->hwnd == GetDlgItem( ID_MOVE_DOWN   )->m_hWnd ) Manage_command( pMsg->message, MOVE_DOWN   );
      if ( pMsg->hwnd == GetDlgItem( ID_MOVE_LEFT   )->m_hWnd ) Manage_command( pMsg->message, MOVE_LEFT   );
      if ( pMsg->hwnd == GetDlgItem( ID_MOVE_RIGHT  )->m_hWnd ) Manage_command( pMsg->message, MOVE_RIGHT  );
      if ( pMsg->hwnd == GetDlgItem( ID_HOR_ENLARGE )->m_hWnd )	Manage_command( pMsg->message, HOR_ENLARGE );
      if ( pMsg->hwnd == GetDlgItem( ID_HOR_REDUCE  )->m_hWnd )	Manage_command( pMsg->message, HOR_REDUCE  );
      if ( pMsg->hwnd == GetDlgItem( ID_VER_ENLARGE )->m_hWnd )	Manage_command( pMsg->message, VER_ENLARGE );
      if ( pMsg->hwnd == GetDlgItem( ID_VER_REDUCE  )->m_hWnd )	Manage_command( pMsg->message, VER_REDUCE  );
    } */



      if ( pMsg->hwnd == move_up     ) Manage_command( pMsg->message, MOVE_UP     );
      if ( pMsg->hwnd == move_down   ) Manage_command( pMsg->message, MOVE_DOWN   );
      if ( pMsg->hwnd == move_left   ) Manage_command( pMsg->message, MOVE_LEFT   );
      if ( pMsg->hwnd == move_right  ) Manage_command( pMsg->message, MOVE_RIGHT  );
      if ( pMsg->hwnd == hor_enlarge ) Manage_command( pMsg->message, HOR_ENLARGE );
      if ( pMsg->hwnd == hor_reduce  ) Manage_command( pMsg->message, HOR_REDUCE  );
      if ( pMsg->hwnd == ver_enlarge ) Manage_command( pMsg->message, VER_ENLARGE );
      if ( pMsg->hwnd == ver_reduce  ) Manage_command( pMsg->message, VER_REDUCE  ); 





  //  Sleep(5);

  }

  return CContainer_wnd::PreTranslateMessage(pMsg);
}


// pMsg->message is passed to mode to select between lbuttondown (513) and lbuttonup (514)
// dir

void CArea_adjust::Manage_command( int msg, int cmd )
{
	if ( msg==WM_LBUTTONDOWN ) // WM_LBUTTONDOWN
	{
		command = cmd;
		Move_area( 0 );
		first   =   1;
		SetTimer( ID_TIMER, 1000, NULL );
//		TRACE("Prima pressione: avviato timer a 1200ms\n");
	}

	if ( msg==WM_LBUTTONUP ) // WM_LBUTTONUP
	{
		first   = 0;
		command = 0;
		KillTimer( ID_TIMER );
//		TRACE("Rilasciato: fermato timer\n");
	}


}


void CArea_adjust::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  Move_area(0);

	if ( first==1 && command>0 ) 
	{
		first = 0;
		KillTimer( ID_TIMER );
		SetTimer( ID_TIMER, 30, NULL );
		//TRACE("Pressione prolungata: avviato timer a 60ms\n");
	}

	CContainer_wnd::OnTimer(nIDEvent);
}



void CArea_adjust::Move_area( int dir )
{

  int step = 2;


  switch ( command )
  {
    case MOVE_UP:
      if ( buffrect.top-step >= limrect.top )
      {
        buffrect.top    -= step;
        buffrect.bottom -= step;
      }
      break;

    case MOVE_DOWN:
      if ( buffrect.bottom+step <= limrect.bottom )
      {
        buffrect.top    += step;
        buffrect.bottom += step;
      }
      break;

    case MOVE_LEFT:
      if ( buffrect.left-step >= limrect.left )
      {
        buffrect.left   -= step;
        buffrect.right  -= step;
      }
      break;

    case MOVE_RIGHT:
      if ( buffrect.right+step <= limrect.right )
      {
        buffrect.left   += step;
        buffrect.right  += step;
      }
      break;

    case HOR_ENLARGE:
      if ( buffrect.left-step >= limrect.left && buffrect.right+step <= limrect.right )
      {
        buffrect.left   -= step;
        buffrect.right  += step;
      }
      break;

    case HOR_REDUCE:
      if ( buffrect.right-buffrect.left-2*step+1>minsize.cx )
      {
        buffrect.left   += step;
        buffrect.right  -= step;
      }
      break;

    case VER_ENLARGE:
      if ( buffrect.top-step >= limrect.top && buffrect.bottom+step <= limrect.bottom )
      {
        buffrect.top    -= step;
        buffrect.bottom += step;
      }
      break;

    case VER_REDUCE:
      if ( buffrect.bottom-buffrect.top-2*step+1>minsize.cy )
      {
        buffrect.top    += step;
        buffrect.bottom -= step;
      }
      break;

  }

  ASSERT( limrect.left    <= buffrect.left   );
  ASSERT( buffrect.right  <= limrect.right   );
  ASSERT( limrect.top     <= buffrect.top    );
  ASSERT( buffrect.bottom <= limrect.bottom  );
  ASSERT( buffrect.right -buffrect.left >= minsize.cx );
  ASSERT( buffrect.bottom-buffrect.top  >= minsize.cy );

  Draw_area();
  
}





/*  #if NUM_TLC>0
	POINT pt1_acq, pt2_acq; //puntatore estremo alto-sinistro e basso-destro per acq
	POINT pt1_dsp, pt2_dsp; //puntatore estremo alto-sinistro e basso-destro per dsp

  

  if(!level_flag[index])
  {
	  Old_pt1.x = pt1_acq.x = area_insp[index].left;
	  Old_pt1.y = pt1_acq.y = area_insp[index].top;
    Old_pt2.x = pt2_acq.x = area_insp[index].right;
	  Old_pt2.y = pt2_acq.y = area_insp[index].bottom;
  }
  else
  {
	  Old_pt1.x = pt1_acq.x = fill[index].area.left;
	  Old_pt1.y = pt1_acq.y = fill[index].area.top;
    Old_pt2.x = pt2_acq.x = fill[index].area.right;
	  Old_pt2.y = pt2_acq.y = fill[index].area.bottom;
  }

  if(PC_num == 1)
  {
    if(index < NUM_TLC_CAP)
    {
      acq2dsp( &pt1_acq, &pt1_dsp );
      acq2dsp( &pt2_acq, &pt2_dsp );
    }
    else
    {
      acq2dsplin( &pt1_acq, &pt1_dsp );
      acq2dsplin( &pt2_acq, &pt2_dsp );
    }
  }
  else
  {
    acq2dsp( &pt1_acq, &pt1_dsp );
    acq2dsp( &pt2_acq, &pt2_dsp );
  }

 
  
  

  #ifdef ROT90CCW_SCALE21 // In questo caso devo scambiare le y di pt1_dsp e pt2_dsp
    int interm = pt1_dsp.y;
	  pt1_dsp.y  = pt2_dsp.y;
	  pt2_dsp.y  = interm;
  
  
  #endif
 

  int nMove = 1;
  if ( ToggleOn ) nMove *= 10;

  
    switch ( command )
    {
    case MOVE_DOWN:
 
      if ( pt1_dsp.y -nMove >0       ) pt1_dsp.y -= nMove;
      if ( pt2_dsp.y>pt1_dsp.y+nMove ) pt2_dsp.y -= nMove;
      
      break;

    case MOVE_UP:
    
      if ( pt1_dsp.y<pt2_dsp.y-nMove ) pt1_dsp.y += nMove;
      if ( pt2_dsp.y<DIMY_DSP-nMove  ) pt2_dsp.y += nMove;
      
      break;

    case MOVE_LEFT:
      
      if ( pt1_dsp.x -nMove>0        ) pt1_dsp.x -= nMove;
      if ( pt2_dsp.x>pt1_dsp.x+nMove ) pt2_dsp.x -= nMove;
      
      break;

    case MOVE_RIGHT:
     
      if ( pt1_dsp.x<pt2_dsp.x-nMove ) pt1_dsp.x += nMove;
      if ( pt2_dsp.x<DIMX_DSP-nMove  ) pt2_dsp.x += nMove;
      
      break;

    case HOR_ENLARGE:
      if ( pt1_dsp.x -nMove >0       ) pt1_dsp.x -= nMove;
      if ( pt2_dsp.x<DIMX_DSP-nMove  ) pt2_dsp.x += nMove;
      break;

    case HOR_REDUCE:
      if ( pt1_dsp.x<pt2_dsp.x-nMove ) pt1_dsp.x += nMove;
      if ( pt2_dsp.x>pt1_dsp.x+nMove ) pt2_dsp.x -= nMove;
      break;

    case VER_ENLARGE:
      if ( pt1_dsp.y - nMove>0      ) pt1_dsp.y -= nMove;
      if ( pt2_dsp.y<DIMY_DSP-nMove ) pt2_dsp.y += nMove;
      break;

    case VER_REDUCE:
      if ( pt1_dsp.y<pt2_dsp.y-nMove ) pt1_dsp.y += nMove;
      if ( pt2_dsp.y>pt1_dsp.y+nMove ) pt2_dsp.y -= nMove;
      break;

    }
  
  
  
  #ifdef ROT90CCW_SCALE21 // In questo caso devo scambiare le y di pt1_dsp e pt2_dsp
    interm = pt1_dsp.y;
	  pt1_dsp.y  = pt2_dsp.y;
	  pt2_dsp.y  = interm;
   
  #endif



  if(PC_num == 1)
  { 
    if(index < NUM_TLC_CAP)
    {
	    dsp2acq( &pt1_dsp, &pt1_acq );
	    dsp2acq( &pt2_dsp, &pt2_acq );
    }
    else
    {
      dsp2acqlin( &pt1_dsp, &pt1_acq );
	    dsp2acqlin( &pt2_dsp, &pt2_acq );
    }
  }
  else
  {
    dsp2acq( &pt1_dsp, &pt1_acq );
	  dsp2acq( &pt2_dsp, &pt2_acq );
  }

 

  if(!level_flag[index])
  {
    New_pt1.x = area_insp[index].left   = pt1_acq.x;
	  New_pt1.y = area_insp[index].top    = pt1_acq.y;
	  New_pt2.x = area_insp[index].right  = pt2_acq.x;
	  New_pt2.y = area_insp[index].bottom = pt2_acq.y;
  }
  else
    {
  	New_pt1.x = fill[index].area.left   = pt1_acq.x;
	  New_pt1.y = fill[index].area.top    = pt1_acq.y;
	  New_pt2.x = fill[index].area.right  = pt2_acq.x;
	  New_pt2.y = fill[index].area.bottom = pt2_acq.y;
  }

  
  
	//Draw_area( index, TRUE, FALSE );
  Draw_area( index, TRUE, TRUE );
 // Invalidate();
#endif*/






//void CArea_adjust::Draw_area( int ind, BOOL draw_rect, BOOL delete_flag )
void CArea_adjust::Draw_area( void )
{
  int x, y;
  
  //SCALE_IMG( index, NULL ); // rescale last image!

  CopyMemory( image_dsp[index]->ptr_dsp, ptr_dsp, image_dsp[index]->dsp_size );


  for ( x=buffrect.left ; x<buffrect.right ; x++ )
  {
    DRAW_PT(index,x,buffrect.top   ,color );
    DRAW_PT(index,x,buffrect.bottom,color );
  }

  for ( y=buffrect.top ; y<buffrect.bottom ; y++ )
  {
    DRAW_PT(index,buffrect.left,  y, color );
    DRAW_PT(index,buffrect.right, y, color );
  }

//  image_dsp[index]->Draw_image(TRUE);
  image_dsp[index]->Draw_image();

  CString txt;
//  txt.Format( L"Coords:\r\n(%d,%d)\r\n(%d,%d)\r\nDims:\r\n%dx%d", buffrect.left, buffrect.top, buffrect.right, buffrect.bottom, buffrect.right-buffrect.left+1, buffrect.bottom-buffrect.top+1 );
  txt.Format( string_coords[lng], buffrect.left, buffrect.top, buffrect.right, buffrect.bottom, buffrect.right-buffrect.left+1, buffrect.bottom-buffrect.top+1 );
  GetDlgItem( IDC_CURRENT )->SetWindowText( txt );

}

  /*register int x, y;
#if NUM_TLC>0
  ASSERT( ind>=0 && ind<NUM_TLC );

//	CClientDC dc(pView);


 //   for ( i=0 ; i<NUM_TLC ; i++ )
 //   {
  if ( buffdsp[ind] )
  {
    ASSERT( buffdsp[ind] );
    CopyMemory( image_dsp[ind]->ptr_dsp, buffdsp[ind], image_dsp[ind]->dsp_size );
//    image_dsp[ind]->Draw_image();
  }
//    }


 // image_dsp[ind]->Draw_image();

//  SCALE_IMG ( ind, img_acq );


 
//+++	FCWin32::DrawImage( img_dsp[ind], dc.m_hDC, img_area[ind] );

//  img_dsp[ind].Draw( dc.m_hDC, img_area[ind] );

	
	//TRACE("CArea_adjust::Draw_area xxx ind=%d - draw_rect=%d\n", ind, draw_rect );



	if ( draw_rect )
	{
		

  //  if(delete_flag)  DeleteRect(New_pt1.x, New_pt1.y, New_pt2.x, New_pt2.y, ind);

    if(!level_flag[ind])
    {

      New_pt1.x = area_insp[ind].left;
      New_pt1.y = area_insp[ind].top;
      New_pt2.x = area_insp[ind].right;
      New_pt2.y = area_insp[ind].bottom;


      for( y = area_insp[ind].top; y < area_insp[ind].bottom; y++)
      {
        DRAW_PT( ind, area_insp[ind].left,  y, COLORE_GIALLO);
        DRAW_PT( ind, area_insp[ind].right, y, COLORE_GIALLO);
      }

      for( x = area_insp[ind].left; x < area_insp[ind].right; x++)
      {
        DRAW_PT( ind, x, area_insp[ind].top,    COLORE_GIALLO);
        DRAW_PT( ind, x, area_insp[ind].bottom, COLORE_GIALLO);
      }
    }
    else
    {
      New_pt1.x = fill[ind].area.left;
      New_pt1.y = fill[ind].area.top;
      New_pt2.x = fill[ind].area.right;
      New_pt2.y = fill[ind].area.bottom;

      for( y = fill[ind].area.top; y < fill[ind].area.bottom; y++)
      {
        DRAW_PT( ind, fill[ind].area.left,  y, COLORE_ARANCIO);
        DRAW_PT( ind, fill[ind].area.right, y, COLORE_ARANCIO);
      }

      for( x = fill[ind].area.left; x < fill[ind].area.right; x++)
      {
        DRAW_PT( ind, x, fill[ind].area.top,    COLORE_ARANCIO);
        DRAW_PT( ind, x, fill[ind].area.bottom, COLORE_ARANCIO);
      }
    }
    //Invalidate();
    
    
//    image_dsp[ind]->Draw_image(dc.m_hDC);
    image_dsp[ind]->Draw_image();
    }

	TCHAR text[100];
  if(!level_flag[index])
  {
  _stprintf( text, L"(%d,%d  %d,%d) - %dx%d", 
		area_insp[index].left, area_insp[index].top, area_insp[index].right, area_insp[index].bottom, 
		area_insp[index].right-area_insp[index].left+1, area_insp[index].bottom-area_insp[index].top+1 );
  }
  else
  {
    _stprintf( text, L"(%d,%d  %d,%d) - %dx%d", 
		fill[index].area.left, fill[index].area.top, fill[index].area.right, fill[index].area.bottom, 
		fill[index].area.right-fill[index].area.left+1, fill[index].area.bottom-fill[index].area.top+1 );
  

  }
	GetDlgItem( IDC_CURRENT )->SetWindowText( text );

#endif
  /*
  register int x, y;
#if NUM_TLC>0
  ASSERT( ind>=0 && ind<NUM_TLC );

	CClientDC dc(pView);
	FCWin32::DrawImage( img_dsp[ind], dc.m_hDC, img_area[ind] );

  int i = 0;
  int j = 0;

  

	TRACE("CArea_adjust::Draw_area ind=%d - draw_rect=%d\n", ind, draw_rect );



	if ( draw_rect )
	{
		
   

    if(delete_flag)
     DeleteRect(New_pt1.x, New_pt1.y, New_pt2.x, New_pt2.y, ind);

    if(!level_flag[ind])
    {
     
      New_pt1.x = area_insp[ind].left;
      New_pt1.y = area_insp[ind].top;
      New_pt2.x = area_insp[ind].right;
      New_pt2.y = area_insp[ind].bottom;


    }
    else
    {
      New_pt1.x = fill[ind].area.left;
      New_pt1.y = fill[ind].area.top;
      New_pt2.x = fill[ind].area.right;
      New_pt2.y = fill[ind].area.bottom;

      for( y = fill[ind].area.top; y < fill[ind].area.bottom; y++)
      {
        DRAW_PT( ind, fill[ind].area.left,  y, COLORE_ARANCIO);
        DRAW_PT( ind, fill[ind].area.right, y, COLORE_ARANCIO);
      }

      for( x = fill[ind].area.left; x < fill[ind].area.right; x++)
      {
        DRAW_PT( ind, x, fill[ind].area.top,    COLORE_ARANCIO);
        DRAW_PT( ind, x, fill[ind].area.bottom, COLORE_ARANCIO);
      }
    }
 
    image_dsp[ind]->Draw_image(dc.m_hDC);
   
   	}

	TCHAR text[100];
  if(!level_flag[index])
  {
  _stprintf( text, L"(%d,%d  %d,%d) - %dx%d", 
		area_insp[index].left, area_insp[index].top, area_insp[index].right, area_insp[index].bottom, 
		area_insp[index].right-area_insp[index].left+1, area_insp[index].bottom-area_insp[index].top+1 );
  }
  else
  {
    _stprintf( text, L"(%d,%d  %d,%d) - %dx%d", 
		fill[index].area.left, fill[index].area.top, fill[index].area.right, fill[index].area.bottom, 
		fill[index].area.right-fill[index].area.left+1, fill[index].area.bottom-fill[index].area.top+1 );
  

  }
	GetDlgItem( IDC_CURRENT )->SetWindowText( text );

#endif*/


