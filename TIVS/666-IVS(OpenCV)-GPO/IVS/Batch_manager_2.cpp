// Batch_manager.cpp : implementation file
#pragma once
#include "stdafx.h"
#include "IVS.h"
#include "Batch_manager.h"
#include "ChildView.h"
#include "Input_string.h"
//#include "File_mngmt.h"

//#include "LoadPdfFile.h"

//extern LoadPdfFile*       pdf_wnd;
//#define MAX_LOG_STRING_LENGTH 100


jmp_buf env;



extern UINT             lng;

// CBatch_manager dialog

IMPLEMENT_DYNAMIC(CBatch_manager, CDialog)

CBatch_manager::CBatch_manager(CWnd* pParent /*=NULL*/)	: CDialog(CBatch_manager::IDD, pParent)
{
  ZeroMemory( &current, sizeof(current) );
  wcscpy( current.oper, string_1320[lng] );
  wcscpy( current.name, string_1320[lng] );
  wcscpy( current.code, string_1320[lng] );
  wcscpy( current.desc, string_1320[lng] );
  current.status = 0; // 0=not running - 1=batch active
  Load_data();
}

CBatch_manager::~CBatch_manager()
{
}

void CBatch_manager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBatch_manager, CDialog)
 // ON_EN_CHANGE(IDC_BATCHNAME_VAL, &CBatch_manager::OnEnChangeBatchnameVal)
//  ON_EN_CHANGE(IDC_BATCHOPERATOR_VAL, &CBatch_manager::OnEnChangeBatchoperatorVal)
//  ON_EN_CHANGE(IDC_BATCHCODE_VAL, &CBatch_manager::OnEnChangeBatchcodeVal)
  ON_WM_SHOWWINDOW()
/*  ON_EN_SETFOCUS(IDC_BATCHCODE_VAL, &CBatch_manager::OnEnSetfocusBatchcodeVal )
  ON_EN_SETFOCUS(IDC_BATCHNAME_VAL, &CBatch_manager::OnEnSetfocusBatchnameVal )
  ON_EN_SETFOCUS(IDC_BATCHDESC_VAL, &CBatch_manager::OnEnSetfocusBatchdescVal )
  ON_EN_SETFOCUS(IDC_BATCHOPER_VAL, &CBatch_manager::OnEnSetfocusBatchoperVal ) */

  ON_STN_CLICKED(IDC_BATCHCODE_VAL, &CBatch_manager::OnStnClickedBatchcodeVal)
  ON_STN_CLICKED(IDC_BATCHNAME_VAL, &CBatch_manager::OnStnClickedBatchnameVal)
  ON_STN_CLICKED(IDC_BATCHDESC_VAL, &CBatch_manager::OnStnClickedBatchdescVal)
  ON_STN_CLICKED(IDC_BATCHOPER_VAL, &CBatch_manager::OnStnClickedBatchoperVal)
END_MESSAGE_MAP()







int CBatch_manager::Load_data(void)
{
  // provo a caricare i dati dell'ultima calibrazione
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( BATCHDATA_FILE , CFile::modeRead, &ex ) )
  {
    TRACE("CBatch_manager::Load_data unable to open file. Skipping\n");
    // ex.ReportError();
    return 99;
  }

  if ( fp.GetLength() != sizeof(batch_data) ) // wrong file size
  {
    TRACE("CBatch_manager::Load_data wrong file size %d/%d. Skipping\n",fp.GetLength(), sizeof(batch_data) );
    return 109;
  }

  fp.Read( &current, sizeof(batch_data) );
  fp.Close();
  TRACE("CBatch_manager::Load_data data properly loaded from disk\n");

  return 0;
}

int CBatch_manager::Save_data(void)
{
  // provo a caricare i dati dell'ultima calibrazione
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( BATCHDATA_FILE , CFile::modeWrite|CFile::modeCreate, &ex ) )
  {
    TRACE("CBatch_manager::Save_data data properly loaded from disk\n");
    ex.ReportError();
    return 199;
  }

  //TRACE( "%d bytes\n", CALDATADIM );
  //TRACE( L"%d bytes\n", CALDATADIM );

  fp.Write( &current, sizeof(batch_data) );
  fp.Close();
  TRACE("CBatch_manager::Save_data data properly saved to disk\n");
  return 0;
}




void CBatch_manager::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);

  //GetDlgItem( IDC_CAPTION_TXT    )->SetWindowTextW( string_1322[lng] );
  GetDlgItem( IDC_CAPTION_TXT    )->SetWindowTextW( string_1336[lng] );

  GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_1324[lng] );
  GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );
  GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1326[lng] );
  GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );
  GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1328[lng] );
  GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );
  GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_1330[lng] );
  GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );
  GetDlgItem( IDOK               )->SetWindowTextW( string_1332[lng] );
  GetDlgItem( IDCANCEL           )->SetWindowTextW( string_1334[lng] );
}




int CBatch_manager::read_string( wchar_t* buffer, wchar_t* caption )
{
  ASSERT( buffer  && AfxIsValidAddress(buffer, MAXLEN_BATCH_TXT+1) );
  ASSERT( caption && AfxIsValidAddress(caption,100) );

// wcscpy( buffer ,L"" );

  CInput_string stringpad;
  stringpad.password = FALSE;

  swprintf( stringpad.buffer , buffer );

  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( caption );
 // stringpad.GetDlgItem( IDC_CURRENT_VAL  )->SetWindowText( buffer  );

  if ( stringpad.RunModalLoop() == IDOK )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( wcslen(stringpad.buffer)<=MAXLEN_BATCH_TXT );
		wcsncpy( buffer, stringpad.buffer, MAXLEN_BATCH_TXT );
    return 0;
	}

  return 99;
}



void CBatch_manager::OnOK()
{
  // TODO: Add your specialized code here and/or call the base class
  //TRACE("CBatch_manager::OnOK\n");
  // Save_data();

  CDialog::OnOK();
}


void CBatch_manager::OnCancel()
{
  // TODO: Add your specialized code here and/or call the base class

  CDialog::OnCancel();
}




//void CBatch_manager::OnEnSetfocusBatchcodeVal()
void CBatch_manager::OnStnClickedBatchcodeVal()
{
  //TRACE("OnEnSetfocusBatchcodeVal\n");
  wchar_t buffer[MAXLEN_BATCH_TXT+1];
  wcscpy( buffer, current.code );

  if ( !read_string( buffer, string_1328[lng] ) ) //==IDOK )
  {
    wcsncpy( current.code, buffer, MAXLEN_BATCH_TXT );
    //TRACE( L"new batch code <%s>\n", buffer );
  }
  //else TRACE("canceled\n");

  GetDlgItem( IDC_BATCHCODE_VAL )->SetWindowTextW( current.code );
  TRACE( L"New batch code= <%s>\n", current.code );
  //CTime time = CTime::GetCurrentTime()
}


//void CBatch_manager::OnEnSetfocusBatchoperVal()
void CBatch_manager::OnStnClickedBatchoperVal()
{
  //TRACE("OnEnSetfocusBatchoperVal\n");
  wchar_t buffer[MAXLEN_BATCH_TXT+1];
  wcscpy( buffer, current.oper );

  if ( !read_string( buffer, string_1324[lng] ) ) //==IDOK )
  {
    wcsncpy( current.oper, buffer, MAXLEN_BATCH_TXT );
    //TRACE( L"new batch code <%s>\n", buffer );
  }
  //else TRACE("canceled\n");

  GetDlgItem( IDC_BATCHOPER_VAL )->SetWindowTextW( current.oper );
  TRACE( L"New batch oper= <%s>\n", current.oper );
}


//void CBatch_manager::OnEnSetfocusBatchnameVal()
void CBatch_manager::OnStnClickedBatchnameVal()
{
  //TRACE("OnEnSetfocusBatchoperVal\n");
  wchar_t buffer[MAXLEN_BATCH_TXT+1];
  wcscpy( buffer, current.name );

  if ( !read_string( buffer, string_1326[lng] ) )
  {
    wcsncpy( current.name, buffer, MAXLEN_BATCH_TXT );
    //TRACE( L"new batch code <%s>\n", buffer );
  }
  //else TRACE("canceled\n");

  GetDlgItem( IDC_BATCHNAME_VAL )->SetWindowTextW( current.name );
  TRACE( L"New batch name= <%s>\n", current.name );
}


//void CBatch_manager::OnEnSetfocusBatchdescVal()
void CBatch_manager::OnStnClickedBatchdescVal()
{
  //TRACE("OnEnSetfocusBatchdescVal\n");
  wchar_t buffer[MAXLEN_BATCH_TXT+1];
  wcscpy( buffer, current.desc );

  if ( !read_string( buffer, string_1330[lng] ) ) 
  {
    wcsncpy( current.desc, buffer, MAXLEN_BATCH_TXT );
    //TRACE( L"new batch code <%s>\n", buffer );
  }
  //else TRACE("canceled\n");

  GetDlgItem( IDC_BATCHDESC_VAL )->SetWindowTextW( current.desc );
  TRACE( L"New batch desc=<%s>\n", current.desc );
}





int CBatch_manager::Create_report(void)
{
//  wchar_t name[100];
  char file_name[100];

  CTime time = CTime::GetCurrentTime();

  sprintf( file_name, "BATCH_REPORT_%04d%02d%02d_%02d%02d%02d.pdf", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

  if ( Create_pdf( file_name ) )
  {
    Message_box( msg_err, MBOX_ERROR );
  }

  return 0;
}


void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
//  printf ("ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no);
  TRACE("ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no);
  longjmp (env, 1); /* invoke longjmp() on error */
}



void CBatch_manager::DrawPageNumber( HPDF_Page page, float x, float y, const char* label )
{
  HPDF_Page_SetLineWidth (page, 0.5);
  HPDF_Page_SetFontAndSize (page, font, 6);

  HPDF_Page_BeginText (page);
  HPDF_Page_MoveTextPos (page, (HPDF_Page_GetWidth(page)/2), y - 10);
  HPDF_Page_ShowText (page, label);
  HPDF_Page_EndText (page);

  HPDF_Page_MoveTo (page, x, y);
  HPDF_Page_LineTo (page, x + 400, y );
  HPDF_Page_Stroke (page);

}




int CBatch_manager::Add_text( const char* txt, int mode ) 
{
  float x, y;

  switch ( mode )
  {
    case TEXT_MODE_LEFT:
      x = posx;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_CENTER:
//      x = left_margin+(page_dimx-right_margin-HPDF_Page_TextWidth(page,txt))/2;
      x = (left_margin+right_margin)/2 - HPDF_Page_TextWidth(page,txt)/2;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_RIGHT:
//      x = page_dimx - right_margin - HPDF_Page_TextWidth( page, txt );
      x = right_margin - HPDF_Page_TextWidth( page, txt ) - (posx-left_margin);
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    default:
      TRACE("CBatch_management::Add_text called with unknown mode\n");

  }
//  posy -= 25; //HPDF_Page_TextHeight( page, txt );

  posy -= row_height; //HPDF_Page_TextHeight( page, txt );

  return 0;

}





int CBatch_manager::Create_pdf( const char* fname )
{

  SYSTEMTIME time;
	GetLocalTime(&time);
  char text[200];
  char temp[200];

  int n_page = 0;

  HPDF_Font font;

//  CFileFind findFile;

  pdf = HPDF_New( error_handler, NULL );
  if ( !pdf ) 
  {
    msg_err.Format( L"CBatch_manager::Create_pdf: unable to create pdf object" );
    //TRACE ("error: cannot create PdfDoc object\n");
    return 1001;
  }

  if ( setjmp(env) )
  {
    msg_err.Format( L"CBatch_manager::Create_pdf: unable to setjmp(env)" );
    HPDF_Free (pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  UINT n_prod_in      =   100000;
  UINT n_goods        =    90000;

  UINT n_rejects      =    10000;
  UINT n_rejects1     =     6000;
  UINT n_rejects2     =     6000;

  UINT n_1stpart_rej  =     1111;
  UINT n_2ndpart_rej  =     2222;
  UINT n_filllev1_rej =     3333;
  UINT n_filllev2_rej =     4444;
  UINT n_cosmetic_rej =     5555;
  UINT n_cap_rej      =     6666;
  UINT n_capcolor_rej =     7777;
//  UINT n_spin_rej     =     8888;

  UINT n_camera_rej[12];
  ZeroMemory( n_camera_rej, sizeof( n_camera_rej ) );

  ADS_data_type  camera_rejects, rejects1, rejects2;


  if ( ADS_status )
  {
    n_prod_in      =  1001; // *((UINT*)num_prod_in->val); 
    n_goods        =  1002; //*((UINT*)num_goods->val);
    n_rejects      =  1003; //*((UINT*)num_rejects->val);
    n_1stpart_rej  =  1004; //*((UINT*)num_1stpart_rej->val);
    n_2ndpart_rej  =  1005; //*((UINT*)num_2ndpart_rej->val);
    n_filllev1_rej =  1006; //*((UINT*)num_filllev_rej->val);
    n_filllev2_rej =  1007; //*((UINT*)num_filllev2_rej->val);
    n_cosmetic_rej =  1008; //*((UINT*)num_cosmetic_rej->val);
    n_cap_rej      =  1009; //*((UINT*)num_cap_rej->val);
    n_capcolor_rej =  1010; //*((UINT*)num_capcolor_rej->val);
//    n_spin_rej     =  *((UINT*)num_capcolor_rej->val);

    if ( !ADS_init_parameter( ".num_rejects_cam", &camera_rejects, 12*4 ) && !ADS_read_parameter( &camera_rejects ) )
      CopyMemory( n_camera_rej, camera_rejects.val, camera_rejects.dim );

    if ( !ADS_init_parameter( ".num_rejects1", &rejects1, 4 ) && !ADS_read_parameter( &rejects1 ) )
      n_rejects1 =  *((UINT*)rejects1.val);

    if ( !ADS_init_parameter( ".num_rejects2", &rejects2, 4 ) && !ADS_read_parameter( &rejects2 ) )
      n_rejects2 =  *((UINT*)rejects2.val);
  }

  char dateandtime[100];
  sprintf( dateandtime, "%02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute );
   
  for ( int npage=0 ; npage<2 ; npage++ )
  {

//  const char *page_title =  "Batch Report";

    page = HPDF_AddPage (pdf);
    HPDF_Page_SetSize ( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

    page_dimx = HPDF_Page_GetWidth (page);
    page_dimy = HPDF_Page_GetHeight(page);

    left_margin   = page_dimx/20;
    right_margin  = page_dimx - left_margin;
    top_margin    = page_dimy*25/26;
    bottom_margin = page_dimy - top_margin;
    row_height    = 30;


    TRACE ("<Page dimension: %f x %f >\n", page_dimx, page_dimy );
    TRACE ("<Page margins: %f,%f %f,%f >\n", left_margin, top_margin, right_margin, bottom_margin );

    HPDF_Page_SetLineWidth (page, 0.5);
    HPDF_Page_Rectangle (page, left_margin, top_margin, right_margin-left_margin, bottom_margin-top_margin );
    HPDF_Page_Rectangle( page, left_margin, top_margin, right_margin-left_margin, -60 );
//  HPDF_Page_Rectangle( page, left_margin, (page_dimy-top_margin-20), page_dimx-right_margin-left_margin, page_dimy );
    HPDF_Page_Stroke (page);

    posx = left_margin + 10;
    posy = top_margin - 24; // Start from top of page

//    font = HPDF_GetFont (pdf, "Helvetica", NULL);
    font = HPDF_GetFont (pdf, PDF_REPORT_FONT, NULL);
    HPDF_Page_SetFontAndSize (page, font, 18 );

    sprintf ( text, "BATCH REPORT" ); 
    Add_text( text, TEXT_MODE_CENTER );

//    font = HPDF_GetFont (pdf, "Helvetica", NULL);
    font = HPDF_GetFont (pdf, PDF_REPORT_FONT, NULL);
    HPDF_Page_SetFontAndSize (page, font, 12 );
    posy += 10;

//    sprintf ( text, "NIRMA HEALTHCARE - AUTOMATIC INSPECTION MACHINE ARGO 60-39-9" ); 
    sprintf( text, "%s - %s", CUSTOMER_NAME, MACHINE_NAME );
    Add_text( text, TEXT_MODE_CENTER );

//    font = HPDF_GetFont (pdf, "Courier", NULL);
    font = HPDF_GetFont (pdf, PDF_REPORT_FONT, NULL);
    HPDF_Page_SetFontAndSize (page, font, 12 );
    row_height = 20;
    posy -= 50;

/*    WideCharToMultiByte( CP_ACP, 0, pView->recipe_name, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Recipe              : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT ); */

    sprintf ( text, "Machine I.D. No.    : %s", MACHINE_ID ); 
    Add_text( text, TEXT_MODE_LEFT );

/*    WideCharToMultiByte( CP_ACP, 0, pView->batch_name, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Product name        : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );

    WideCharToMultiByte( CP_ACP, 0, pView->batch_description, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Batch No.           : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT ); */

    sprintf ( text, "No. of containers   : %d", n_prod_in ); 
    Add_text( text, TEXT_MODE_LEFT );

/*    GetDataAndTimeFromFile( temp );
    sprintf ( text, "Started On          : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );*/

    sprintf ( text, "Ended On            : %02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute); 
    Add_text( text, TEXT_MODE_LEFT );

/*    sprintf ( text, "User logged in      : %s", User_str );
    Add_text( text, TEXT_MODE_LEFT ); */

    posy -= 10;
    sprintf ( text, "Pkg. chemist, Name  : _____________________ Sign ____________________" );
    Add_text( text, TEXT_MODE_LEFT );

    posy -= 10;
    sprintf ( text, "Q.A. chemist, Name  : _____________________ Sign ____________________" );
    Add_text( text, TEXT_MODE_LEFT );

    posy -= 70;

    if ( npage==0 )
    {

      sprintf( text, "OVERALL PRODUCTION STATISTICS" );
      Add_text( text, TEXT_MODE_LEFT );
      posy -= 10;

      if ( n_prod_in == 0 )
      {
        sprintf( text, "NO PROCESSED PRODUCT" );
        Add_text( text, TEXT_MODE_LEFT );
      }
      else
      {
        sprintf( text, "Processed products ________________________ %8d pcs   (%8.3lf%%)", n_prod_in,      100.0 );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Accepted products  ________________________ %8d pcs   (%8.3lf%%)", n_goods,        100.0*n_goods/n_prod_in        );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Rejected products  ________________________ %8d pcs   (%8.3lf%%)", n_rejects,      100.0*n_rejects/n_prod_in      );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Products rejected on tray 1 _______________ %8d pcs   (%8.3lf%%)", n_rejects1,     100.0*n_rejects1/n_prod_in     );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Products rejected on tray 2 _______________ %8d pcs   (%8.3lf%%)", n_rejects2,     100.0*n_rejects2/n_prod_in     );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Particle 1 rejects ________________________ %8d pcs   (%8.3lf%%)", n_1stpart_rej,  100.0*n_1stpart_rej/n_prod_in  );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Filling level 1 rejects ___________________ %8d pcs   (%8.3lf%%)", n_filllev1_rej, 100.0*n_filllev2_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Particle 2 rejects ________________________ %8d pcs   (%8.3lf%%)", n_2ndpart_rej,  100.0*n_2ndpart_rej/n_prod_in  );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Filling 2 level rejects ___________________ %8d pcs   (%8.3lf%%)", n_filllev2_rej, 100.0*n_filllev2_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Cosmetic rejects __________________________ %8d pcs   (%8.3lf%%)", n_cosmetic_rej, 100.0*n_cosmetic_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Tip/cap rejects ___________________________ %8d pcs   (%8.3lf%%)", n_cap_rej,      100.0*n_cap_rej/n_prod_in      );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Flip-off color rejects ____________________ %8d pcs   (%8.3lf%%)", n_capcolor_rej, 100.0*n_capcolor_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

      }

    }

    if ( npage==1 )
    {
      for ( int i=0 ; i<12 ; i++ )
      {

        if ( i==0 )
        {
          posy -= 10;
          Add_text( "PC1 - TIP&CAP INSPECTION STATION", TEXT_MODE_LEFT );
        }
        if ( i==3 )

        {
          posy -= 10;
          Add_text( "PC1 - COSMETIC INSPECTION STATION", TEXT_MODE_LEFT );
        }

        if ( i==6 )
        {
          posy -= 10;
          Add_text( "PC2 - FIRST PARTICLE&FILLING LEVEL INSPECTION STATION", TEXT_MODE_LEFT );
        }

        if ( i==9 )
        {
          posy -= 10;
          Add_text( "PC2 - SECOND PARTICLE&FILLING LEVEL INSPECTION STATION", TEXT_MODE_LEFT );
        }


        sprintf ( text, "Camera no.%2d rejects ____________________________ %6d pcs", i+1, n_camera_rej[i] );
        Add_text( text, TEXT_MODE_LEFT );
      }
    }



//    font = HPDF_GetFont (pdf, "Courier", NULL);
    font = HPDF_GetFont (pdf, PDF_REPORT_FONT, NULL);
    HPDF_Page_SetFontAndSize (page, font, 10 );
    posy = bottom_margin-12;
    sprintf ( text, "Page %d/%d", npage+1, 2 );
    Add_text( text, TEXT_MODE_LEFT );
    posy += row_height;
    sprintf ( text, "Date&time %s", dateandtime );
    Add_text( text, TEXT_MODE_RIGHT );


  }





  // save the document to a file 
  HPDF_SaveToFile( pdf, fname );
  
  HPDF_Free (pdf); 
    
  return 0;

}



/*
void CBatch_manager::GetDataAndTimeFromFile( char* deststring )
{
  FILE* fp;
  char temp[100];
  
  fp = fopen( "lotto.txt", "r");
  if(fp == NULL)
    strcpy(deststring, "");
  else
  {
    fgets(temp, 100, fp);
    fgets(temp, 100, fp);
    fgets(temp, 100, fp);
    strcpy(deststring, temp);
    fgets(temp, 100, fp);
    strcat(deststring, " ");
    strcat(deststring, temp);
    fclose( fp );
  }

  return;
}

*/