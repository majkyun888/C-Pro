// Recipe_manager.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "Recipe_manager.h"
#include "ChildView.h"
#include "Input_string.h"
//#include "File_mngmt.h"



// CRecipe_manager dialog


extern UINT                lng;
extern CFile_mngmt         file_manager;
extern CChildView*         pView;
extern CLogger*            logger;

extern int pwd_levels[];
 

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif

extern CBatch_manager*     batch_mngr;
//extern CStatic*            Recipe_st;
//extern CPassword_manager*  password_mngr;

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif

// UINT CRecipe_manager::nID = RECIPES_NID;
//extern char output_folder[MAX_LENGHT_PATH];
//extern char input_folder [MAX_LENGHT_PATH];


BEGIN_MESSAGE_MAP(CRecipe_manager, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_LOAD_RECIPE,   &CRecipe_manager::OnBnClickedLoadRecipe   )
  ON_BN_CLICKED(ID_SAVE_RECIPE,   &CRecipe_manager::OnBnClickedSaveRecipe   )
  ON_BN_CLICKED(ID_SCROLL_UP,     &CRecipe_manager::OnBnClickedScrollUp     )
  ON_BN_CLICKED(ID_SCROLL_DOWN,   &CRecipe_manager::OnBnClickedScrollDown   )
  ON_BN_CLICKED(ID_RENAME_RECIPE, &CRecipe_manager::OnBnClickedRenameRecipe )
  ON_BN_CLICKED(ID_DELETE_RECIPE, &CRecipe_manager::OnBnClickedDeleteRecipe )
  ON_BN_CLICKED(ID_CLOSE_RECIPE,  &CRecipe_manager::OnBnClickedCloseRecipe  )
  ON_WM_UPDATEUISTATE()
END_MESSAGE_MAP()




IMPLEMENT_DYNAMIC(CRecipe_manager, CDialog)


CRecipe_manager::CRecipe_manager( CWnd* pParent, CFile_mngmt* file_manager_in ) : CDialog(CRecipe_manager::IDD, pParent)
{
	//Create( IDD_RECIPE_DIALOG );
//  if ( pParent && pParent->m_hWnd ) pParent->GetClientRect( &area );
  if ( pParent && pParent->m_hWnd ) pParent->GetWindowRect( &par_area );

//  file_manager = file_manager_in;

  filename = (CString**) malloc( MAX_NUM_RECIPES * sizeof(CString*) );

  ZeroMemory( filename, MAX_NUM_RECIPES * sizeof(CString*) );

//  swprintf( recipe_name, 50, L"Unitialized" );

//  if ( file_manager )

/*
  if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) )
  {
    wcsncpy( recipe_name, file_manager.recipe_name, 50 );
    if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );
  }
  */

//	file_mgr = new CFile_mngmt( "recipes.bin" );
//	file_mgr->Add_data( name, sizeof(name) );
//	file_mgr->Load_all();
//  Recipe_select( 0 ); // Aggiorno i testi
}



CRecipe_manager::~CRecipe_manager()
{
  if ( filename )
  {
  	for ( int i=0 ; i<MAX_NUM_RECIPES ; i++ ) if ( filename[i] ) delete( filename[i] );
    free( filename );
  }

//	file_mgr->Save_all();
	for ( int i=0 ; i<NUM_RECIPES ; i++ )
	{
//		if ( text[i]   ) free( text[i] );
		if ( button[i] ) delete( button[i] );
	}
//	if ( file_mgr ) delete file_mgr;
	DestroyWindow();

//  TRACE("CRecipe_manager distrutto\n");
  TRACE( L"CRecipe_manager destroyed\n" );
}


void CRecipe_manager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


int CRecipe_manager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CDialog::OnCreate(lpCreateStruct) == -1)	return -1;

  nID = RECIPES_NID;


	CString capt;

  // parent window area
  CRect area;

  area.left   = par_area.left   + 100;
  area.right  = par_area.right  - 100;
  area.top    = par_area.top    + 200;
  area.bottom = par_area.bottom - 200;

  MoveWindow( &area ); // resize and move recipe window

	GetClientRect( &cli_area );

  if ( !font.m_hObject )
	{
		CClientDC dc(this);
    //VERIFY(font.CreatePointFont(120, "Arial", &dc));
    VERIFY(font.CreatePointFont(120, L"Arial", &dc));
	}

  int i=0;
  int width = ( area.right - area.left ) / NUM_COLUMNS;
  
  for ( int col=0 ; col<NUM_COLUMNS ; col++ )
	{
    int left  = width * col;
    int right = left + width;
    int hbtn  = 40; 

    for ( int row=0 ; row<NUM_ROWS ; row++ )
    {
      area.left   = left         +   10;
      area.right  = right        -   10;
      area.top    = row*(hbtn+5) +   10;
      area.bottom = area.top     + hbtn;

      //capt.Format("button n.%d", i+1 );
      capt.Format( L"button n.%d", i+1 );

      button[i] = new CButton;
      VERIFY( button[i]->Create( capt, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, nID++ ) );
      button[i]->SetFont( &font );
      i++;
    }

  }

  selected     = -1;
  selected_old = -1;
//  ZeroMemory( selected_name, sizeof(selected_name) );

  Get_filenames();

  font.DeleteObject();

	return 0;

}




BOOL CRecipe_manager::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect area;

//  UINT btn[] = { ID_LOAD_RECIPE, ID_SCROLL_UP, ID_SCROLL_DOWN, ID_CLOSE_RECIPE }; 
  UINT btn[] = { ID_LOAD_RECIPE, ID_SAVE_RECIPE, ID_SCROLL_UP, ID_SCROLL_DOWN, ID_RENAME_RECIPE, ID_DELETE_RECIPE, ID_CLOSE_RECIPE }; //Pour changed for Sunho modification on 20200502
//  UINT btn[] = { ID_LOAD_RECIPE, ID_SAVE_RECIPE, ID_SCROLL_UP, ID_SCROLL_DOWN, ID_DELETE_RECIPE, ID_CLOSE_RECIPE }; //Pour removed for Sunho modification on 20200502
  UINT nbtn = sizeof( btn ) / sizeof(UINT);

  int spcx  = 10;
  int width = ( cli_area.Width() - spcx*(nbtn+1) ) / nbtn;

  area.bottom = cli_area.bottom -  10;
  area.top    = area.bottom     -  40;
  
  for ( UINT i=0 ; i<nbtn ; i++ )
  {
    area.left   = spcx + i*(spcx+width);
    area.right  = area.left + width;

    CWnd* ptr = GetDlgItem( btn[i] );
    ptr->MoveWindow(&area);
    ptr->ShowWindow( SW_SHOW );
  }

  if ( GetDlgItem( ID_SCROLL_UP ) && GetDlgItem( ID_SCROLL_UP )->m_hWnd )
    GetDlgItem( ID_SCROLL_UP )->ShowWindow( SW_HIDE );

  if ( GetDlgItem( ID_SCROLL_DOWN ) && GetDlgItem( ID_SCROLL_DOWN )->m_hWnd && num_pages<=1 )
    GetDlgItem( ID_SCROLL_DOWN )->ShowWindow( SW_HIDE );

  /*
  if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->SetWindowTextA( string_420[lng] );
  if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->SetWindowTextA( string_422[lng] );
  if ( GetDlgItem( ID_SCROLL_UP     ) && GetDlgItem( ID_SCROLL_UP     )->m_hWnd ) GetDlgItem( ID_SCROLL_UP     )->SetWindowTextA( string_424[lng] );
  if ( GetDlgItem( ID_SCROLL_DOWN   ) && GetDlgItem( ID_SCROLL_DOWN   )->m_hWnd ) GetDlgItem( ID_SCROLL_DOWN   )->SetWindowTextA( string_426[lng] );
  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->SetWindowTextA( string_428[lng] );
  if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->SetWindowTextA( string_430[lng] );
  if ( GetDlgItem( ID_CLOSE_RECIPE  ) && GetDlgItem( ID_CLOSE_RECIPE  )->m_hWnd ) GetDlgItem( ID_CLOSE_RECIPE  )->SetWindowTextA( string_432[lng] );
  */

  if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->SetWindowText( string_420[lng] );
  if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->SetWindowText( string_422[lng] );
  if ( GetDlgItem( ID_SCROLL_UP     ) && GetDlgItem( ID_SCROLL_UP     )->m_hWnd ) GetDlgItem( ID_SCROLL_UP     )->SetWindowText( string_424[lng] );
  if ( GetDlgItem( ID_SCROLL_DOWN   ) && GetDlgItem( ID_SCROLL_DOWN   )->m_hWnd ) GetDlgItem( ID_SCROLL_DOWN   )->SetWindowText( string_426[lng] );
  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->SetWindowText( string_428[lng] );
  if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->SetWindowText( string_430[lng] );
  if ( GetDlgItem( ID_CLOSE_RECIPE  ) && GetDlgItem( ID_CLOSE_RECIPE  )->m_hWnd ) GetDlgItem( ID_CLOSE_RECIPE  )->SetWindowText( string_432[lng] );
  
  if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
  if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_HIDE );//Pour added on 20200502
  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );//Pour added on 20200502
  if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );

  // Override in case of password manager
  if ( password_mngr ) // if using password manager
  {


/* OLD VERSION
    if ( password_mngr->current_level >= MAINTENANCE_LEVEL ) // maintenance or higher
    {
      if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
      if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_SHOW );
      // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
      if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
    }
    else // no login or just basic operator
    {
      if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
      if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_HIDE );
      // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
      if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
    }
*/
    
    if ( GetDlgItem( ID_LOAD_RECIPE ) && GetDlgItem( ID_LOAD_RECIPE )->m_hWnd )
    {
      if ( password_mngr->current_level >= pwd_levels[8] ) GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_SHOW );
      else                                                 GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_HIDE );
    }

    if ( GetDlgItem( ID_SAVE_RECIPE ) && GetDlgItem( ID_SAVE_RECIPE )->m_hWnd )
    {
      if ( password_mngr->current_level >= pwd_levels[9] ) GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_SHOW );
      else                                                 GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_HIDE );
    }


	    if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd )
    {
      if ( password_mngr->current_level >= pwd_levels[9] ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_SHOW );
      else                                                 GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
    }


    if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd )
    {
      if ( password_mngr->current_level >= pwd_levels[10] ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_SHOW );
      else                                                  GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
    }

  }


  return TRUE;  // return TRUE unless you set the focus to a control
  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}





int CRecipe_manager::Get_filenames(void)
{

  CFileFind finder;

  wchar_t srcstr[100+1];  // prepare search filter mask
//  swprintf( srcstr, 100, L"%s\\*.%s", MAIN_DIRECTORY, L"ips" );
  swprintf( srcstr, 100, L"%s\\*.%s", RECIPE_DIRECTORY, L"ips" );

  num_files = 0;

  BOOL bWorking = finder.FindFile( srcstr );
  while (bWorking)
  {
    bWorking = finder.FindNextFile();

//    if ( !finder.IsDots() && num_files<MAX_NUM_RECIPES )

    // collect and show only files 1) NOT DOTS - 2) RIGHT SIZE - 3) MORE SPACE AVAILABLE
    if ( !finder.IsDots() && finder.GetLength()==file_manager.dim_file && num_files<MAX_NUM_RECIPES ) 
    {
      filename[num_files] = new CString( finder.GetFileTitle() );
      num_files++;
    }

    /*
    if ( !finder.IsDots() && num_files<MAX_NUM_RECIPES-1 )
    {

      if ( num_files>=0 && num_files<NUM_RECIPES )
      {

        filename[num_files] = new CString( finder.GetFileTitle() );
        //button[numfile]->SetWindowTextA( filename[numfile] );
      }

      num_files++;
      //finder.GetLastAccessTime( create_time );
      // finder.GetCreationTime( create_time );
      // TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
    }*/

  }

  //swprintf( srcstr, 100, L"%d recipes found", num_files );
  //Message_box( srcstr );


  num_page  =  0;
  num_pages = num_files / NUM_RECIPES + 1;

//  TRACE( L"Folder scan end <%s> - %d files found - %d pages\n", srcstr, num_files, num_pages );

  //for ( int i=0 ; i<num_files ; i++ ) TRACE("(%3d) - %s\n", i, *filename[i] );

  Update_filenames();

  return 0;
}


int CRecipe_manager::Update_filenames(void)
{
//  TRACE("Update filenames - num_page=%d\n", num_page );
  TRACE( L"Update filenames - num_page=%d\n", num_page );

  for ( int i=0 ; i<NUM_RECIPES ; i++ )
  {
    int ind = i + num_page * NUM_RECIPES;
    if ( ind < num_files ) // visibile, aggiorna testo
    {
      //button[i]->SetWindowTextA( *filename[ind] );
      button[i]->SetWindowText( *filename[ind] );
      button[i]->ShowWindow( SW_SHOW );
    }
    else // vuoto, nascondi
    {
      button[i]->ShowWindow( SW_HIDE );
    }

  }

  if ( GetDlgItem( ID_SCROLL_DOWN ) && GetDlgItem( ID_SCROLL_DOWN )->m_hWnd )
  {
    if ( num_page>=num_pages-1 ) GetDlgItem( ID_SCROLL_DOWN )->ShowWindow( SW_HIDE );
    else                         GetDlgItem( ID_SCROLL_DOWN )->ShowWindow( SW_SHOW );
  }

  if ( GetDlgItem( ID_SCROLL_UP ) && GetDlgItem( ID_SCROLL_UP )->m_hWnd )
  {
    if ( num_page<=0 ) GetDlgItem( ID_SCROLL_UP )->ShowWindow( SW_HIDE );
    else               GetDlgItem( ID_SCROLL_UP )->ShowWindow( SW_SHOW );
  }

  if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
  if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );

  return 0;
}


int CRecipe_manager::Recipe_select( void )
{
  if ( selected<0 || selected>=num_files )
  {
    ASSERT(0);
    TRACE( L"Richiesta di selezione ricetta errata: selected=%d\n", selected );
    return( 99 );
  }

  wchar_t txt[MAX_RECIPE_LEN+1];

  int ind = selected % NUM_RECIPES;

  if ( selected != selected_old ) // new selection
  {
    swprintf( txt, 50, L">>> %s <<<", filename[selected]->GetBuffer() );
    button[ind]->SetWindowText( txt );
    //button[ind]->SetCheck( BST_CHECKED );
    //button[ind]->SetState( 1 );

    if ( selected_old >= 0 )
    {
      ind = selected_old % NUM_RECIPES;
      button[ind]->SetWindowText( filename[selected_old]->GetBuffer() );
      //button[ind]->SetCheck( BST_UNCHECKED );
      //button[ind]->SetState( 0 );
    }

    selected_old = selected;
  }
  else  // toggle same selection
  {

    if ( selected_old > 0 )  // already selected, deselect 
    {
      ind = selected_old % NUM_RECIPES;
      button[ind]->SetWindowText( filename[selected_old]->GetBuffer() );
      //button[ind]->SetCheck( BST_UNCHECKED );
      //button[ind]->SetState( 0 );
      selected     = -1;
      selected_old = -1;
    }
    else
    {
      swprintf( txt, 50, L">>> %s <<<", filename[selected]->GetBuffer() );
      button[ind]->SetWindowText( txt );
      //button[ind]->SetCheck( BST_CHECKED );
      //button[ind]->SetState( 1 );
      selected_old = selected;
    }

  }

  //TRACE("selected/old  %d/%d\n", selected, selected_old );

/*
	ON_BN_CLICKED(ID_LOAD_RECIPE,   &CRecipe_manager::OnBnClickedLoadRecipe   )
  ON_BN_CLICKED(ID_SAVE_RECIPE,   &CRecipe_manager::OnBnClickedSaveRecipe   )
  ON_BN_CLICKED(ID_SCROLL_UP,     &CRecipe_manager::OnBnClickedScrollUp     )
  ON_BN_CLICKED(ID_SCROLL_DOWN,   &CRecipe_manager::OnBnClickedScrollDown   )
  ON_BN_CLICKED(ID_RENAME_RECIPE, &CRecipe_manager::OnBnClickedRenameRecipe )
  ON_BN_CLICKED(ID_DELETE_RECIPE, &CRecipe_manager::OnBnClickedDeleteRecipe )
  ON_BN_CLICKED(ID_CLOSE_RECIPE,  &CRecipe_manager::OnBnClickedCloseRecipe  )
*/


  if ( selected<0 || selected>=num_files ) // No recipe selected
  {
    if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
	if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_HIDE );
    if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_SHOW );//Pour changed for Sunho on 20200502
    if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );

    if ( password_mngr ) // if using password manager
    {

      if ( GetDlgItem( ID_LOAD_RECIPE ) && GetDlgItem( ID_LOAD_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[8] ) GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_HIDE );
      }

      if ( GetDlgItem( ID_SAVE_RECIPE ) && GetDlgItem( ID_SAVE_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[9] ) GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_HIDE );
      }

	  //Pour added for Sunho on 20200502
	  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[10] ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
      }
	  //Pour added for Sunho on 20200502


      if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[10] ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_SHOW );
        else                                                  GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
      }


/*      if ( password_mngr->current_level >= MAINTENANCE_LEVEL ) // maintenance or higher
      {
        if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_SHOW );
     // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
      }
      else // no login or just basic operator
      {
        if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_HIDE );
     // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
      }*/

    }


  }
  else // Some recipe is selected
  {
    if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_SHOW );
	if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_SHOW );
    //if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );//Pour changed for Sunho on 20200502
	if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_SHOW );//Pour changed for Sunho on 20200502
    if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_SHOW );


    if ( password_mngr ) // if using password manager
    {


      if ( GetDlgItem( ID_LOAD_RECIPE ) && GetDlgItem( ID_LOAD_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[8] ) GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_LOAD_RECIPE )->ShowWindow( SW_HIDE );
      }

      if ( GetDlgItem( ID_SAVE_RECIPE ) && GetDlgItem( ID_SAVE_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[9] ) GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_SAVE_RECIPE )->ShowWindow( SW_HIDE );
      }

	  //Pour added for Sunho on 20200502
	  if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[10] ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_SHOW );
        else                                                 GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
      }
	  //Pour added for Sunho on 20200502

      if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd )
      {
        if ( password_mngr->current_level >= pwd_levels[10] ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_SHOW );
        else                                                  GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );
      }

/*      if ( password_mngr->current_level >= MAINTENANCE_LEVEL ) // maintenance or higher
      {
        if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_SHOW );
        if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_SHOW );
     // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_SHOW );
      }
      else // no login or just basic operator
      {
        if ( GetDlgItem( ID_LOAD_RECIPE   ) && GetDlgItem( ID_LOAD_RECIPE   )->m_hWnd ) GetDlgItem( ID_LOAD_RECIPE   )->ShowWindow( SW_SHOW );
        if ( GetDlgItem( ID_SAVE_RECIPE   ) && GetDlgItem( ID_SAVE_RECIPE   )->m_hWnd ) GetDlgItem( ID_SAVE_RECIPE   )->ShowWindow( SW_HIDE );
     // if ( GetDlgItem( ID_RENAME_RECIPE ) && GetDlgItem( ID_RENAME_RECIPE )->m_hWnd ) GetDlgItem( ID_RENAME_RECIPE )->ShowWindow( SW_HIDE );
        if ( GetDlgItem( ID_DELETE_RECIPE ) && GetDlgItem( ID_DELETE_RECIPE )->m_hWnd ) GetDlgItem( ID_DELETE_RECIPE )->ShowWindow( SW_HIDE );

      } */

    }



  }


	return 0;
}




BOOL CRecipe_manager::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ( HIWORD(wParam)==0 )
	{
		for ( int i=0 ; i<NUM_RECIPES ; i++ )
		{
			if ( LOWORD(wParam)==RECIPES_NID+i )
			{
        //TRACE("Premuto pulsante ricetta n.%d\n", i ); // codice/ident.contr - handle contr
        selected = i+num_page*NUM_RECIPES;
//        if ( selected != selected_old ) Recipe_select();
        Recipe_select();
//        TRACE("New selection n.%2d/ %2d/%2d  %X/%X\n", i, selected, selected_old, wParam, lParam ); // codice/ident.contr - handle contr
        TRACE("New selection n.%2d/ %2d/%2d  %X/%X\n", i, selected, selected_old, wParam, lParam ); // codice/ident.contr - handle contr
				return 0;
			}
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}


void CRecipe_manager::OnBnClickedLoadRecipe()
{

//  TRACE( "CRecipe_manager: OnBnClickedLoadRecipe\n" );
  EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!

  if ( selected>=0 && selected<num_files )
  {
    swprintf( recipe_name,      MAX_RECIPE_LEN, L"%s.ips", filename[selected]->GetBuffer() ); 
    swprintf( full_recipe_name, MAX_RECIPE_LEN, L"%s\\%s.ips", RECIPE_DIRECTORY, filename[selected]->GetBuffer() ); 

  	if ( file_manager.Load_all( full_recipe_name ) )
	  {
  		msg.Format( string_030[lng], recipe_name, file_manager.msg_err );
      Message_box( msg, MBOX_ERROR );
	  } 
    else
	  {
  		msg.Format( string_034[lng], recipe_name );
      if ( logger ) logger->Add_string( &msg );
      if ( batch_mngr )
      {
        //ZeroMemory( batch_mngr->current.recipe, MAX_RECIPE_LEN+1 );
        //TRACE("len=%d\n", wcslen(recipe_name) );
        wcsncpy( batch_mngr->current.recipe, recipe_name, wcslen(recipe_name)-4 );
        batch_mngr->current.recipe[wcslen(recipe_name)-4] = 0;
      }
      recipe_name[wcslen(recipe_name)-4] = 0; // remove extension

      if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) )
      {
        wcsncpy( file_manager.recipe_name, recipe_name, wcslen(recipe_name)-4 );
        file_manager.recipe_name[wcslen(recipe_name)] = '\0';
      }

      if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );
      Message_box( msg, MBOX_INFO );
	  } 

    TRACE( L"CRecipe_manager: LOAD RECIPE - Filename <%s>\n", recipe_name );
    //EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!
  }
  else
  {
    TRACE( L"CRecipe_manager: LOAD RECIPE - No selection\n" );
    //EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!
  }

  return;
}

void CRecipe_manager::OnBnClickedSaveRecipe()
{
//  TRACE( "CRecipe_manager: OnBnClickedSaveRecipe\n" );
  EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!

 // ShowWindow( SW_HIDE );

//    CInput_string stringpad( pView, L"Insert name", 40, 0 ); // parent, stringa iniziale, massima lunghezza


  if ( selected>=0 && selected<num_files ) // file selected: overwrite
  {
    swprintf( recipe_name, MAX_RECIPE_LEN, L"%s.ips", filename[selected]->GetBuffer() ); 
    TRACE( L"CRecipe_manager: SAVE RECIPE selected - Overwrite recipe <%s>\n", recipe_name );

    msg.Format( string_038[lng], recipe_name );

    if ( Message_box( msg, MBOX_YESNO )==IDYES )
    {
      if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) )
      {
        wcsncpy( file_manager.recipe_name, recipe_name, wcslen(recipe_name)-4 );
        file_manager.recipe_name[wcslen(recipe_name)-4] = '\0';
      }

      swprintf( full_recipe_name, MAX_RECIPE_LEN, L"%s\\%s.ips", RECIPE_DIRECTORY, filename[selected]->GetBuffer() ); 
      if ( file_manager.Save_all( full_recipe_name ) )
      {
        msg.Format( string_032[lng], recipe_name, file_manager.msg_err );
        Message_box( msg, MBOX_ERROR );
      } 
      else
      {
        msg.Format( string_036[lng], recipe_name );
        //if ( logger ) logger->Add_string( &msg );
        if ( batch_mngr )
        {
          //ZeroMemory( batch_mngr->current.recipe, MAX_RECIPE_LEN+1 );
          wcsncpy( batch_mngr->current.recipe, recipe_name, wcslen(recipe_name)-4 );
          batch_mngr->current.recipe[wcslen(recipe_name)-4] = 0;
        }
        recipe_name[wcslen(recipe_name)-4] = 0; // remove extension
        //if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) ) wcsncpy( file_manager.recipe_name, recipe_name, 50 );
        if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );
        Message_box( msg, MBOX_INFO );
      } 

    }

    //EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!
  }

  //Pour removed for Sunho to make two seperate buttons
  /*
  else // NO file selected: create a new file
  {
    // CInput_string stringpad( pView, L"Insert name", 40, 0 ); // parent, stringa iniziale, massima lunghezza
 
    // stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_040[lng] );
 
  	CInput_string stringpad;
		//sprintf( numpad.buffer , "%d" , *(BYTE*)val );
		//swprintf( numpad.buffer , L"%d" , *(BYTE*)val );
		stringpad.Create( IDD_INPUT_STRING, this );
    stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_040[lng] );

    if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>3 )
	  {
		  ASSERT( AfxIsValidString(stringpad.buffer) );
 		  ASSERT( wcslen(stringpad.buffer)<=50 );

      for ( int i=0 ; i<num_files ; i++ )
      {
//        if ( CFile(filename[i])==recipe_name )
        if ( !wcscmp( filename[i]->GetBuffer(0) , stringpad.buffer ) )
        {
          msg.Format( string_042[lng], stringpad.buffer );
          Message_box( msg, MBOX_ERROR );
          return;
        }
      }

      swprintf( recipe_name,      MAX_RECIPE_LEN, L"%s.ips", stringpad.buffer ); 
      swprintf( full_recipe_name, MAX_RECIPE_LEN, L"%s\\%s.ips", RECIPE_DIRECTORY, stringpad.buffer ); 

      if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) )
      {
        wcsncpy( file_manager.recipe_name, recipe_name, wcslen(recipe_name)-4 );
        file_manager.recipe_name[wcslen(recipe_name)-4] = '\0';
      }

      if ( file_manager.Save_all( full_recipe_name ) )
      {
        msg.Format( string_032[lng], recipe_name, file_manager.msg_err );
        Message_box( msg, MBOX_ERROR );
      } 
      else
      {
        msg.Format( string_037[lng], recipe_name );
        //if ( logger ) logger->Add_string( &msg );
        if ( batch_mngr )
        {
          //ZeroMemory( batch_mngr->current.recipe, MAX_RECIPE_LEN+1 );
          wcsncpy( batch_mngr->current.recipe, recipe_name, wcslen(recipe_name)-4 );
          batch_mngr->current.recipe[wcslen(recipe_name)-4] = 0;
        }

        recipe_name[wcslen(recipe_name)-4] = 0; // remove extension
        if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );

        Message_box( msg, MBOX_INFO );
      } 
      
      TRACE( L"CRecipe_manager: SAVE RECIPE selected - NEW recipe <%s>\n", recipe_name );

  	}

  }
  */
  //Pour removed for Sunho to make two seperate buttons

 // EndDialog(IDCANCEL); //IDOK); att.ne IDOK non funziona!

//  ShowWindow( SW_HIDE );

}

void CRecipe_manager::OnBnClickedScrollUp()
{
//  TRACE("CRecipe_manager: SCROLL UP\n");
  TRACE( L"CRecipe_manager: SCROLL UP\n" );

  if ( num_page > 0 ) num_page--;
  selected     = -1;
  selected_old = -1;
  Update_filenames();
//  if ( num_page =

	// Scroll verso l'alto
}

void CRecipe_manager::OnBnClickedScrollDown()
{
//  TRACE("CRecipe_manager: SCROLL DOWN\n");
  TRACE( L"CRecipe_manager: SCROLL DOWN\n" );

  if ( num_page < num_pages-1 ) num_page++;
  selected     = -1;
  selected_old = -1;
  Update_filenames();

  // Scroll verso l'alto
}

void CRecipe_manager::OnBnClickedRenameRecipe()
{
//  TRACE("CRecipe_manager: RENAME RECIPE\n");
  TRACE( L"CRecipe_manager: RENAME RECIPE\n" );

	//ASSERT( selected>=0 && selected<NUM_RECIPES );

	//Pour added for Sunho on 20200502
	if ( selected<0 || selected>=num_files ) // file selected: overwrite // NO file selected: create a new file
  {
    // CInput_string stringpad( pView, L"Insert name", 40, 0 ); // parent, stringa iniziale, massima lunghezza
 
    // stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_040[lng] );
 
  	CInput_string stringpad;
		//sprintf( numpad.buffer , "%d" , *(BYTE*)val );
		//swprintf( numpad.buffer , L"%d" , *(BYTE*)val );
		stringpad.Create( IDD_INPUT_STRING, this );
    stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_040[lng] );

    if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>3 )
	  {
		  ASSERT( AfxIsValidString(stringpad.buffer) );
 		  ASSERT( wcslen(stringpad.buffer)<=50 );

      for ( int i=0 ; i<num_files ; i++ )
      {
//        if ( CFile(filename[i])==recipe_name )
        if ( !wcscmp( filename[i]->GetBuffer(0) , stringpad.buffer ) )
        {
          msg.Format( string_042[lng], stringpad.buffer );
          Message_box( msg, MBOX_ERROR );
          return;
        }
      }

      swprintf( recipe_name,      MAX_RECIPE_LEN, L"%s.ips", stringpad.buffer ); 
      swprintf( full_recipe_name, MAX_RECIPE_LEN, L"%s\\%s.ips", RECIPE_DIRECTORY, stringpad.buffer ); 

      if ( file_manager.recipe_name && AfxIsValidString(file_manager.recipe_name) )
      {
        wcsncpy( file_manager.recipe_name, recipe_name, wcslen(recipe_name)-4 );
        file_manager.recipe_name[wcslen(recipe_name)-4] = '\0';
      }

      if ( file_manager.Save_all( full_recipe_name ) )
      {
        msg.Format( string_032[lng], recipe_name, file_manager.msg_err );
        Message_box( msg, MBOX_ERROR );
      } 
      else
      {
        msg.Format( string_037[lng], recipe_name );
        //if ( logger ) logger->Add_string( &msg );
        if ( batch_mngr )
        {
          //ZeroMemory( batch_mngr->current.recipe, MAX_RECIPE_LEN+1 );
          wcsncpy( batch_mngr->current.recipe, recipe_name, wcslen(recipe_name)-4 );
          batch_mngr->current.recipe[wcslen(recipe_name)-4] = 0;
        }

        recipe_name[wcslen(recipe_name)-4] = 0; // remove extension
        if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );

        Message_box( msg, MBOX_INFO );
      } 
      
      TRACE( L"CRecipe_manager: SAVE RECIPE selected - NEW recipe <%s>\n", recipe_name );

  	}

  }
	//Pour added for Sunho on 20200502

  /*
  CInput_string stringpad( this, name[selected], MAX_NAME_LENGHT ); // parent, stringa iniziale, massima lunghezza

//	ASSERT( selected>=0 && selected<NUM_RECIPES );
//	strncpy( stringpad.buffer, name[selected], MAX_NAME_LENGHT );
//	stringpad.Create( IDD_INPUT_STRING, this );
	if ( stringpad.RunModalLoop() == IDOK )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( strlen(stringpad.buffer)<=MAX_NAME_LENGHT );
		strncpy( name[selected], stringpad.buffer, MAX_NAME_LENGHT );
		Recipe_select(selected);
	}
  */
}





void CRecipe_manager::OnBnClickedDeleteRecipe()
{

//  TRACE("CRecipe_manager: DELETE RECIPE\n");
  TRACE( L"CRecipe_manager: DELETE RECIPE\n" );


  EndDialog(IDCANCEL);

  if ( selected>=0 && selected<num_files ) // file selected: ask for confirmation
  {
    swprintf( recipe_name, MAX_RECIPE_LEN, L"%s.ips", filename[selected]->GetBuffer() ); 
    swprintf( full_recipe_name, MAX_RECIPE_LEN, L"%s\\%s.ips", RECIPE_DIRECTORY, filename[selected]->GetBuffer() ); 

    msg.Format( string_044[lng], recipe_name );

    if ( Message_box( msg, MBOX_YESNO )==IDYES )
    {

      if ( DeleteFile( full_recipe_name ) )
      {
        msg.Format( string_046[lng], recipe_name );
        Message_box( msg, MBOX_INFO );
      } 
      else
      {
        msg.Format( string_048[lng], recipe_name, GetLastError() );
        Message_box( msg, MBOX_ERROR );
      } 

    }

  }

}

void CRecipe_manager::OnBnClickedCloseRecipe()
{
  TRACE( "CRecipe_manager: CLOSE RECIPE\n" );
  EndDialog(IDCANCEL);

 /* if ( selected>=0 && selected<num_files ) // file selected: ask for confirmation
  {
    swprintf( recipe_name, L"%s.ips", filename[selected]->GetBuffer() ); 

    msg.Format( string_044[lng], recipe_name );

    if ( Message_box( msg, MBOX_YESNO )==IDYES )
    {

      if ( DeleteFile( recipe_name ) )
      {
        msg.Format( string_046[lng], recipe_name );
        Message_box( msg, MBOX_INFO );
      } 
      else
      {
        msg.Format( string_048[lng], recipe_name, GetLastError() );
        Message_box( msg, MBOX_ERROR );
      } 

    }

  } */

}




