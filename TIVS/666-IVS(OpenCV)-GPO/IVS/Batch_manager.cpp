// Batch_manager.cpp : implementation file
#pragma once
#include "stdafx.h"
#include "IVS.h"
#include "Batch_manager.h"
#include "ChildView.h"
#include "Input_string.h"
//#include "UTF8.h"


jmp_buf env;

extern UINT             lng;
extern CRecipe_manager  recipe_wnd( pView );

//Pour added 20190328
extern CStdioFile       log_file;
extern CStdioFile       alarm_log_file;
extern CBatch_manager*  batch_mngr;

extern                  BYTE          freezemode_on;
//Pour added 20190328

// CBatch_manager dialog

IMPLEMENT_DYNAMIC(CBatch_manager, CDialog)

CBatch_manager::CBatch_manager(CWnd* pParent /*=NULL*/)	: CDialog(CBatch_manager::IDD, pParent)
{
  ZeroMemory( &current, sizeof(current) );
  wcscpy( current.recipe, string_1320[lng] );
  wcscpy( current.oper,   string_1320[lng] );
  wcscpy( current.name,   string_1320[lng] );
  wcscpy( current.code,   string_1320[lng] );
  wcscpy( current.desc,   string_1320[lng] );
  current.status = 0; // 0=not running - 1=batch active
  Load_data();
}

CBatch_manager::~CBatch_manager()
{
  Save_data();
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



void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
//  printf ("ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no);

//  msg_err.Format( "PDF ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no );
  swprintf( msg_err, 200, L"error_handler: error_no=0x%04X detail_no=%d", (unsigned int) error_no, (int) detail_no );
  
  //TRACE("PDF ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no );
  TRACE( L"\n%s\n", msg_err );
  longjmp (env, 1); /* invoke longjmp() on error */
}




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
  /*
    #ifdef CIRON_BATCH   // to use ciron custom batch report
    GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_1326[lng]  ); // Product name
    GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );  //current.name 20180505
    GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1328[lng] );  // Batch number
    GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );  //current.code 20180505
    GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1329[lng] );  // Batch size
    GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );  //
    GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_8025[lng] );  // Batch operator
    GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );  // batch operator
  */

  //Pour removed 20190405
/*     
	#ifdef CIRON_BATCH   // to use ciron custom batch report
    GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_1324[lng]  ); // Product name
    GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );  //current.name 20180505
    GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1326[lng] );  // Batch number
    GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );  //current.code 20180505
    GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1330a[lng] );  // Batch size
    GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );  //
    GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_1330[lng] );  // Batch operator
    GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );  // batch operator
*/
  //Pour removed 20190405
  	#ifdef CIRON_BATCH   // to use ciron custom batch report
    GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_1324[lng]  ); // Product name
    GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );  //current.name 20180505
    GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1326[lng] );  // Batch number
    GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );  //current.code 20180505
    GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1330a[lng] );  // Batch size
    GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );  //
    GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_1330[lng] );  // Batch operator
    GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );  // batch operator

#elif  SERIAL_N == 202000022 || SERIAL_N == 202000028

    GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_8013[lng]  ); // Operator
	GetDlgItem( IDC_BATCHOPER_VAL  )->EnableWindow(true);
    GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );
    GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1326[lng] ); // Product name
    GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );
    GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1328[lng] ); // Batch number
    GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );
    GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_8014[lng] ); // Batch date
    GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );


  #else                // normal version of batch
    GetDlgItem( IDC_BATCHOPER_TXT  )->SetWindowTextW( string_280[lng]  ); // Operator
    GetDlgItem( IDC_BATCHOPER_VAL  )->SetWindowTextW( current.oper     );
    GetDlgItem( IDC_BATCHNAME_TXT  )->SetWindowTextW( string_1326[lng] ); // Product name
    GetDlgItem( IDC_BATCHNAME_VAL  )->SetWindowTextW( current.name     );
    GetDlgItem( IDC_BATCHCODE_TXT  )->SetWindowTextW( string_1328[lng] ); // Batch number
    GetDlgItem( IDC_BATCHCODE_VAL  )->SetWindowTextW( current.code     );
    GetDlgItem( IDC_BATCHDESC_TXT  )->SetWindowTextW( string_1330[lng] ); // Batch date
    GetDlgItem( IDC_BATCHDESC_VAL  )->SetWindowTextW( current.desc     );
  #endif
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

  swprintf( stringpad.buffer , 100 , buffer );

  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( caption );
 // stringpad.GetDlgItem( IDC_CURRENT_VAL  )->SetWindowText( buffer  );

//  if ( stringpad.RunModalLoop() == IDOK )
  if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>2 )
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
  //if ( !read_string( buffer, string_1330a[lng] ) ) //==IDOK )
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

  if ( !read_string( buffer, string_8013[lng] ) )
  //if ( !read_string( buffer, string_1324[lng] ) ) //==IDOK )
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

  //if ( !read_string( buffer, string_1330[lng] ) ) 
	if ( !read_string( buffer, string_8014[lng] ) )
  {
    wcsncpy( current.desc, buffer, MAXLEN_BATCH_TXT );
    //TRACE( L"new batch code <%s>\n", buffer );
  }
  //else TRACE("canceled\n");

  GetDlgItem( IDC_BATCHDESC_VAL )->SetWindowTextW( current.desc );
  TRACE( L"New batch desc=<%s>\n", current.desc );
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//             B A T C H     R E P O R T     P D F      F U N C T I O N S
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



int CBatch_manager::Create_report(void)
{
//  wchar_t name[100];

//  CTime time = CTime::GetCurrentTime(); //Pour removed 20190328

//Pour added 20190328
  #if SERIAL_N==201500013 ||SERIAL_N==201800027 || SERIAL_N==201900198
	CTime time = batch_mngr->current.end_time;
#else

  CTime time = CTime::GetCurrentTime();
#endif
//Pour added 20190328

//  sprintf( file_name, "BATCH_REPORT_%04d%02d%02d_%02d%02d%02d.pdf", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  /*
	if ( batch_mngr->current.status==1 ) // batch active - ask to end
	{
		CString batch_info;
	    batch_info.Format( string_1328a[lng], current.code, current.name );
        Message_box( batch_info, MBOX_INFO );
	    return 0;
	}
	*/
	//else
    //{
		//if ( Message_box( string_1340[lng], MBOX_YESNO ) == IDYES )
	//{
  sprintf( file_name, "BATCHREPORT(ENG)_%s.pdf", time2txt(&time,10) );
  if ( Create_pdf( ENGLISH ) ) Message_box( msg_err, MBOX_ERROR );

  if ( lng==ENGLISH && PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );

  if ( lng==CHINESE )
  {
    sprintf( file_name, "BATCHREPORT(CHN)_%s.pdf", time2txt(&time,10) );
    if ( Create_pdf( CHINESE ) ) Message_box( msg_err, MBOX_ERROR );
    if ( PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );
  }

  if ( lng==SPANISH )
  {
    sprintf( file_name, "BATCHREPORT(ESP)_%s.pdf", time2txt(&time,10) );
    if ( Create_pdf( SPANISH ) ) Message_box( msg_err, MBOX_ERROR );
    if ( PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );
  }

  if ( lng==RUSSIAN )
  {
    //sprintf( file_name, "BATCHREPORT(ENG)_%s.pdf", time2txt(&time,10) );
    //if ( Create_pdf( ENGLISH ) ) Message_box( msg_err, MBOX_ERROR );

    sprintf( file_name, "BATCHREPORT(RUS)_%s.pdf", time2txt(&time,10) );
    if ( Create_pdf( RUSSIAN ) ) Message_box( msg_err, MBOX_ERROR );
    if ( PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );
  }
	//}
		else
			return 0;
	//}
//#endif

  return 0;
}

//Pour added 20190328

//Pour added 20180903
int CBatch_manager::Txt2pdf()
{
  
  CFileException ex;
  
  CString txtfile;
  //Pour marked 20190220
  CString txtfile_alarm;
  //Pour marked 20190220
  CTime time = batch_mngr->current.end_time;
  //txtfile.Format(L"logcopy_%04d%02d%02d_%02d%02d%02d.txt",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

  //Pour added for testing 20190219

  txtfile.Format(L"logcopy_%04d%02d%02d_%02d%02d%02d.txt",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

  //Pour marked 20190219 for the modification of log_file
  if ( !log_file.Open( txtfile ,  CFile::modeRead | CFile::typeText, &ex ) )
  {
	ex.ReportError();
    return 99;
  }
  
  if(lng == CHINESE)
	setlocale( LC_CTYPE, ("chs")); // if language is CHN, must set the LC_TYPE, or it will cause error
 
  Create_Log_pdf(lng,batch_mngr->current.end_time); // different from the old function
  log_file.Close();
  return 0;
}

//Pour added 20190220 for tansfer alarm log

int CBatch_manager::Txt2pdf_alarm()
{
  
  CFileException ex;
  
  //Pour marked 20190220
  CString txtfile_alarm;
  //Pour marked 20190220
  CTime time = batch_mngr->current.end_time;
  
  //Pour added for alarm log 20190220
  txtfile_alarm.Format(L"alarm_log_copy_%04d%02d%02d_%02d%02d%02d.txt",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
    if ( !log_file.Open( txtfile_alarm ,  CFile::modeRead | CFile::typeText, &ex ) )
  {
	ex.ReportError();
    return 99;
  }
  //Pour added for alarm log 20190220
 
  if(lng == CHINESE)
	setlocale( LC_CTYPE, ("chs")); // if language is CHN, must set the LC_TYPE, or it will cause error
 
  Create_AlarmLog_pdf(lng,batch_mngr->current.end_time); // different from the old function
  log_file.Close();
  return 0;
}
//Pour added 20190220

int CBatch_manager::Add_Log_new_page( int npg )
{
  page = HPDF_AddPage(pdf);
  HPDF_Page_SetSize( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

  page_dimx     = HPDF_Page_GetWidth (page);
  page_dimy     = HPDF_Page_GetHeight(page);
  left_margin   = page_dimx/20;
  right_margin  = page_dimx - left_margin;
  top_margin    = page_dimy*25/26;
  bottom_margin = page_dimy - top_margin;
  
  codepage = 437;
  if( pdf_lng == CHINESE) 
	  codepage = 936;

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 11 );

//  sprintf ( text, string_8006[pdf_lng], 1, 1 );

  //wchar_t wtext[200];
 // swprintf ( wtext, string_8006[pdf_lng], 1, 1 );
  char text[200];
  char str[200];
  //char txtpdf[200];
  //int ret = WideCharToMultiByte( 936, 0, wtext, -1, txtpdf, 200, NULL, NULL );
  //codepage = 936  simplified Chinese
  int ret = WideCharToMultiByte( codepage, 0, string_8006a[pdf_lng], -1, str, 200, NULL, NULL );//Pour changed for Sunho on20200629


//  sprintf ( text, string_8006[pdf_lng], npg, num_pages );
  sprintf_s ( text, str, npg );


  HPDF_Page_BeginText(page);
  //HPDF_Page_TextOut  (page, left_margin + 10 , bottom_margin-12, file_name );

  HPDF_Page_TextOut  (page, right_margin- 75, bottom_margin-12, text );//Pour marked on 20200629
  //HPDF_Page_TextOut  (page, right_margin- 60, bottom_margin-12, txtpdf );

  HPDF_Page_EndText  (page);

  posx = left_margin + 20;
  posy = top_margin  - 24; // Start from top of page

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 10 );
  row_height = 19;

  return 0;

}

// create pdf - lang can be ENGLISH(0) or CHINESE(1)
int CBatch_manager::Create_Log_pdf( int lang, int type )
{
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  pdf = HPDF_New( error_handler, NULL );
  if ( !pdf ) 
  {
    swprintf_s( msg_err, 200, L"CBatch_manager::Create_Log_pdf unable to create pdf object" );
    return 1001;
  }

  if ( setjmp(env) )
  {
    HPDF_Free(pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  // English
  fontname = "Courier"; // warning: case sensitive!
  encoding = NULL;
  if ( pdf_lng==CHINESE ) // Chinese simplified
  {
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);
    //fontname = "SimSun";   // warning: case sensitive!
    fontname = "SimHei";   // warning: case sensitive!
    encoding = "GB-EUC-H"; // warning: case sensitive!
  }

 
  // create page
  CString fpath,fname;
  CTime time = CTime::GetCurrentTime(); 
  fpath =  MAIN_DIRECTORY;//Pour removed for Sunho on 20200507
  //fpath =  LOG_DIRECTORY;//Pour added for Sunho on 20200507

  //Pour marked for Sunho on 20200510
  if(type==0)
	fname.Format(  L"\\%s_%04d%02d%02d_%02d%02d%02d.pdf", L"Alarm_LOG", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  else
	fname.Format(  L"\\%s_%04d%02d%02d_%02d%02d%02d.pdf", L"LOG", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

  int  cpl=0, npg = 0, lpp = 1;
	//ULONG fileLeng = hga_file.GetLength();
	ULONG lineNum =0;
  if( log_file.m_hFile == CFile::hFileNull)
	  return -1; // log.txt file not open 
  //if(pdf_lng == ENGLISH)
	  cpl = (float)(595-30*2)/6.5; // characters per line
	  lpp = (float)(842-32*2)/19.0; // lines per page

	  log_file.Seek(0,CFile::begin);
	  CString cs,cs1;
	
	  while( log_file.ReadString( cs ))
	  {
	    wchar_t* line;
		if( lineNum ==0)
		Add_Log_new_page(++npg);
		while ( cs.GetLength() > cpl)
		{
			cs1 = cs.Left(cpl);
		    line = (cs1).GetBuffer(0);
			Add_text( line, TEXT_MODE_LEFT );
			cs.Delete(0,cpl);
			lineNum++;
			if( lineNum % lpp == 0)
				Add_Log_new_page(++npg);
		}
		line = (cs).GetBuffer(0);
		Add_text( line, TEXT_MODE_LEFT );
		lineNum++;
		if( lineNum % lpp == 0)
			Add_Log_new_page(++npg);
	  }
	  //CreateDirectory( BATCH_DIRECTORY, NULL );	 
	  HPDF_SaveToFile( pdf,WtoA((fpath+fname).GetBuffer(0)) );

	  //Pour added for open pdf report automatically 20180912
		ShellExecute( NULL, L"open", AtoW(WtoA((fpath+fname).GetBuffer(0))), NULL, L"", SW_SHOWDEFAULT );
	  //Pour added for open pdf report automatically 20180912

	  HPDF_Free(pdf); 
	  return 0;  
}




int CBatch_manager::Create_Log_pdf( int lang, CTime &save_time )
{
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  // add pages to the batch report, so no need to define a new pdf file 
  //pdf = HPDF_New( error_handler, NULL );
  //if ( !pdf ) 
  //{
  //  swprintf_s( msg_err, 200, L"CBatch_manager::Create_Log_pdf unable to create pdf object" );
  //  return 1001;
  //}

  //if ( setjmp(env) )
  //{
  //  HPDF_Free(pdf);
  //  return 1002;
  //}

  //// set compression mode 
  //HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  //// English
  //fontname = "Courier"; // warning: case sensitive!
  //encoding = NULL;
  //if ( pdf_lng==CHINESE ) // Chinese simplified
  //{
  //  HPDF_UseCNSFonts(pdf);
  //  HPDF_UseCNSEncodings(pdf);
  //  //fontname = "SimSun";   // warning: case sensitive!
  //  fontname = "SimHei";   // warning: case sensitive!
  //  encoding = "GB-EUC-H"; // warning: case sensitive!
  //}

 //Pour changed for testing 20180905
  // create pdf name 
  //CString fpath,fname;
  //CTime time = save_time; 
  //fpath =  BATCH_DIRECTORY;
  //fpath =  MAIN_DIRECTORY;//added for testing 20180906
  //fname.Format(  L"\\%s_%04d%02d%02d_%02d%02d%02d.pdf", L"LOG", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
 

  //wchar_t buff[NEWLOG_MAXFILESZ+1];
  //Pour changed for testing 20180906
  wchar_t buff[200*1024] = {0};
  //Pour changed for testing 20180906
   CStdioFile file;
   CFileException ex;
  //log_file.Read(buff, wcslen(buff)*sizeof(wchar_t));//Pour marked 20180906

   //Pour changed for testing 20180906
  log_file.Read(buff, 200*1024);//Pour marked 20180906

 // "temp.txt" is a temporary file, read "log_file" write to "file", and then ReadString works well, or it will fail
  if(!file.Open(L"\\temp.txt",CFile::modeCreate | CFile::modeReadWrite,&ex))
	{
    ex.ReportError();
    return 99;
	}
  file.WriteString(buff);

  int  cpl=0, npg = 0, lpp = 1;
	ULONG lineNum =0;
  if( file.m_hFile == CFile::hFileNull)
	  return -1; // log.txt file not open 
  //if(pdf_lng == ENGLISH)
	  cpl = (float)(595-30*2)/6.5; // characters per line
	  lpp = (float)(842-32*2)/19.0; // lines per page

	  file.Seek(0,CFile::begin);
	  CString cs,cs1;
	
	  while( file.ReadString( cs ))
	  {
	    wchar_t* line;
		if( lineNum ==0)
		Add_Log_new_page(++npg);

		while ( cs.GetLength() > cpl)
		{
			cs1 = cs.Left(cpl);
		    line = (cs1).GetBuffer(0);
			Add_text( line, TEXT_MODE_LEFT );
			cs.Delete(0,cpl);
			lineNum++;
			if( lineNum % lpp == 0)
				Add_Log_new_page(++npg);
		}
		line = (cs).GetBuffer(0);
		Add_text( line, TEXT_MODE_LEFT );
		lineNum++;
		if( lineNum % lpp == 0)
			Add_Log_new_page(++npg);
	  }
	    //Pour added for testing 20180907
	  if((posy-40) >= 51)

	  {
		  posy-=40;
		  Add_text( string_8040[pdf_lng], TEXT_MODE_LEFT );
	  }
	  else 
	  {
		Add_Log_new_page(++npg);
	  posy-=40;
	  Add_text( string_8040[pdf_lng], TEXT_MODE_LEFT );
	  }

  //Pour added for testing 20180906
	  file.Close();
	  DeleteFile(L"\\temp.txt");

	  return 0;

}
//Pour added 20190222
int CBatch_manager::Create_AlarmLog_pdf( int lang, CTime &save_time )
{
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  //wchar_t buff[NEWLOG_MAXFILESZ+1];
  //Pour changed for testing 20180906
  wchar_t buff_a[180*1024] = {0};
  //Pour changed for testing 20180906
   CStdioFile alarm_file;
   CFileException ex;
  //log_file.Read(buff, wcslen(buff)*sizeof(wchar_t));//Pour marked 20180906

   //Pour changed for testing 20180906
  //log_file.Read(buff_a, 1800*1024);//Pour marked 20180906
   alarm_log_file.Read(buff_a, 180*1024);//Pour marked 20180906

 // "temp.txt" is a temporary file, read "log_file" write to "file", and then ReadString works well, or it will fail
  if(!alarm_file.Open(L"\\temp1.txt",CFile::modeCreate | CFile::modeReadWrite,&ex))
	{
    ex.ReportError();
    return 99;
	}
  alarm_file.WriteString(buff_a);

  int  cpl=0, npg = 0, lpp = 1;
	ULONG lineNum =0;
  if( alarm_file.m_hFile == CFile::hFileNull)
	  return -1; // log.txt file not open 
  //if(pdf_lng == ENGLISH)
	  cpl = (float)(595-30*2)/6.5; // characters per line
	  lpp = (float)(842-32*2)/19.0; // lines per page

	  alarm_file.Seek(0,CFile::begin);
	  CString cs,cs1;
	
	  while( alarm_file.ReadString( cs ))
	  {
	    wchar_t* line;
		if( lineNum ==0)
		Add_Log_new_page(++npg);

		while ( cs.GetLength() > cpl)
		{
			cs1 = cs.Left(cpl);
		    line = (cs1).GetBuffer(0);
			Add_text( line, TEXT_MODE_LEFT );
			cs.Delete(0,cpl);
			lineNum++;
			if( lineNum % lpp == 0)
				Add_Log_new_page(++npg);
		}
		line = (cs).GetBuffer(0);
		Add_text( line, TEXT_MODE_LEFT );
		lineNum++;
		if( lineNum % lpp == 0)
			Add_Log_new_page(++npg);
	  }
	    //Pour added for testing 20180907
	  if((posy-40) >= 51)

	  {
		  posy-=40;
		  Add_text( string_8040[pdf_lng], TEXT_MODE_LEFT );
	  }
	  else 
	  {
		Add_Log_new_page(++npg);
	  posy-=40;
	  Add_text( string_8040[pdf_lng], TEXT_MODE_LEFT );
	  }

  //Pour added for testing 20180906
	  alarm_file.Close();
	  DeleteFile(L"\\temp1.txt");

	  return 0;

}
//Pour added 20190222


#ifdef HGA_DATAREC
int CBatch_manager::Add_HGA_new_page( int npg,int tpg )
{
  page = HPDF_AddPage(pdf);
  HPDF_Page_SetSize( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

  page_dimx     = HPDF_Page_GetWidth (page);
  page_dimy     = HPDF_Page_GetHeight(page);
  left_margin   = page_dimx/20;
  right_margin  = page_dimx - left_margin;
  top_margin    = page_dimy*25/26;
  bottom_margin = page_dimy - top_margin;
  //row_height    = 30;
  codepage = 437;
  //HPDF_Page_SetLineWidth (page, 0.5);
  //HPDF_Page_Rectangle (page, left_margin, top_margin, right_margin-left_margin, bottom_margin-top_margin );
  //HPDF_Page_Rectangle( page, left_margin, top_margin, right_margin-left_margin, -40 );
  //HPDF_Page_Stroke (page);

  //posx = left_margin + 10;
  //posy = top_margin  - 24; // Start from top of page

//  font = HPDF_GetFont( pdf, fontname, encoding );
//  HPDF_Page_SetFontAndSize( page, font, 18 );
////  sprintf ( text, string_8150[pdf_lng] ); 
////  Add_text( text, TEXT_MODE_CENTER );
//  Add_text( string_8160[pdf_lng], TEXT_MODE_CENTER );
//
//  font = HPDF_GetFont( pdf, fontname, encoding );
//  HPDF_Page_SetFontAndSize (page, font, 12 );
//  posy += 10;

//  sprintf( text, "%s - %s", CUSTOMER, MODEL );
//  Add_text( text, TEXT_MODE_CENTER );

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 11 );

//  sprintf ( text, string_8006[pdf_lng], 1, 1 );

  //wchar_t wtext[200];
 // swprintf ( wtext, string_8006[pdf_lng], 1, 1 );
  char text[200];
  char str[200];
  char txtpdf[200];
  //int ret = WideCharToMultiByte( 936, 0, wtext, -1, txtpdf, 200, NULL, NULL );
  //codepage = 936  simplified Chinese
  int ret = WideCharToMultiByte( codepage, 0, string_8006[pdf_lng], -1, str, 200, NULL, NULL );
//  sprintf ( text, string_8006[pdf_lng], npg, num_pages );
  sprintf ( text, str, npg, tpg );


  HPDF_Page_BeginText(page);
  //HPDF_Page_TextOut  (page, left_margin + 10 , bottom_margin-12, file_name );

  HPDF_Page_TextOut  (page, right_margin- 75, bottom_margin-12, text );
//  HPDF_Page_TextOut  (page, right_margin- 60, bottom_margin-12, txtpdf );

  HPDF_Page_EndText  (page);

  posx = left_margin + 20;
  posy = top_margin  - 24; // Start from top of page

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 11 );
  row_height = 20;

  return 0;

}

/*
// create pdf - lang can be ENGLISH(0) or CHINESE(1)
int CBatch_manager::Create_HGA_report( int lang )
{
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  pdf = HPDF_New( error_handler, NULL );
  if ( !pdf ) 
  {
    swprintf( msg_err, 200, L"CBatch_manager::Create_HGA_report unable to create pdf object" );
    return 1001;
  }

  if ( setjmp(env) )
  {
    HPDF_Free(pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  // English
  fontname = "Courier"; // warning: case sensitive!
  encoding = NULL;
  if ( pdf_lng==CHINESE ) // Chinese simplified
  {
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);
    //fontname = "SimSun";   // warning: case sensitive!
    fontname = "SimHei";   // warning: case sensitive!
    encoding = "GB-EUC-H"; // warning: case sensitive!
  }

 
  // create page
  CString fpath,fname;
  
  CTime time = CTime::GetCurrentTime(); 
  fpath = BATCH_DIRECTORY;
  fname.Format(  L"\\%s_%04d%02d%02d_%02d%02d%02d.pdf", L"HGAData", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  
  // --------------------------------------
  // just for test, simulate the csv file that we have got
  //CStdioFile  hga_file;
  //CString name;
  //CFileException ex;
  //name= _T("d:\\beijingHGA\\HGADATA_20180717_162024.csv");
  //bool isOpen = hga_file.Open( name, CStdioFile::modeRead, &ex );//hga_file.Open( name, CFile::modeRead, &ex )
  //---------------------------------------

	int lineLeng=1, tot_pages=0, npg = 0;
	ULONG fileLeng = hga_file.GetLength();
	ULONG lineNum =0;
  if(hgadatarec_on && hga_file.m_hFile != CFile::hFileNull) // 
  {
	  CString csLine;
	  while( hga_file.ReadString( csLine ))
	  {
	    wchar_t* line;
		line = (csLine).GetBuffer(0);
		if( lineNum == 0) 
		{
			lineLeng = csLine.GetLength()+1;
			tot_pages = ceil((float)fileLeng/(float)lineLeng/38.0 );
		}
		if(lineNum%38 == 0) // 38 lines per page
		{
			Add_HGA_new_page(++npg,tot_pages);
			Add_text( line, TEXT_MODE_LEFT );
		}
		else 
			Add_text( line, TEXT_MODE_LEFT );
		lineNum++;
	  }
	  
	  CreateDirectory( BATCH_DIRECTORY, NULL );
	  
	  //sprintf_s( file_name, "%s", fname.GetBuffer(0) );//WtoA(BATCH_DIRECTORY),
	 
	  HPDF_SaveToFile( pdf,WtoA((fpath+fname).GetBuffer(0)) );
	  HPDF_Free(pdf); 
	  return 0;
	
  }
  else  
	  return -1;
  
}
*/
#endif
//Pour added 20180903
//Pour added 20190328




int CBatch_manager::Grippers_statistics(void)
{
  CTime time = CTime::GetCurrentTime();

  sprintf( file_name, "GRIPPERS_STATISTICS(ENG)_%s.pdf", time2txt(&time,10) );
  if ( Create_grippers_report( ENGLISH ) ) Message_box( msg_err, MBOX_ERROR );

  if ( PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );

  return 0;
}





// create pdf - lang can be ENGLISH(0) or CHINESE(1)
int CBatch_manager::Create_grippers_report( int lang )
{
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  pdf = HPDF_New( error_handler, NULL );
  if ( !pdf ) 
  {
    swprintf( msg_err, 200, L"CBatch_manager::Create_grippers_report unable to create pdf object" );
    return 1001;
  }

  if ( setjmp(env) )
  {
    HPDF_Free(pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  // English
  fontname = "Courier"; // warning: case sensitive!
  encoding = NULL;
  if ( pdf_lng==CHINESE ) // Chinese simplified
  {
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);
    //fontname = "SimSun";   // warning: case sensitive!
    fontname = "SimHei";   // warning: case sensitive!
    encoding = "GB-EUC-H"; // warning: case sensitive!
  }

  // create page
  char text[200];
  char varn[200];

  page = HPDF_AddPage(pdf);
  HPDF_Page_SetSize( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

  page_dimx     = HPDF_Page_GetWidth (page);
  page_dimy     = HPDF_Page_GetHeight(page);
  left_margin   = page_dimx/20;
  right_margin  = page_dimx - left_margin;
  top_margin    = page_dimy*25/26;
  bottom_margin = page_dimy - top_margin;
  row_height    = 30;

  HPDF_Page_SetLineWidth (page, 0.5);
  HPDF_Page_Rectangle (page, left_margin, top_margin, right_margin-left_margin, bottom_margin-top_margin );
  HPDF_Page_Rectangle( page, left_margin, top_margin, right_margin-left_margin, -40 );
  HPDF_Page_Stroke (page);

  posx = left_margin + 10;
  posy = top_margin  - 24; // Start from top of page

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 18 );
//  sprintf ( text, string_8150[pdf_lng] ); 
//  Add_text( text, TEXT_MODE_CENTER );
  Add_text( string_8150[pdf_lng], TEXT_MODE_CENTER );

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 12 );
  posy += 10;

//  sprintf( text, "%s - %s", CUSTOMER, MODEL );
//  Add_text( text, TEXT_MODE_CENTER );

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 11 );

//  sprintf ( text, string_8006[pdf_lng], 1, 1 );

  wchar_t wtext[200];
  swprintf ( wtext, string_8006[pdf_lng], 1, 1 );

  char txtpdf[200];
  int ret = WideCharToMultiByte( 936, 0, wtext, -1, txtpdf, 200, NULL, NULL );


  HPDF_Page_BeginText(page);
  HPDF_Page_TextOut  (page, left_margin + 10 , bottom_margin-12, file_name );

//  HPDF_Page_TextOut  (page, right_margin- 60, bottom_margin-12, text );
  HPDF_Page_TextOut  (page, right_margin- 60, bottom_margin-12, txtpdf );

  HPDF_Page_EndText  (page);

  posx = left_margin + 10;
  posy = top_margin  - 24; // Start from top of page


  // create page
  SYSTEMTIME time;
	GetLocalTime(&time);

  //    font = HPDF_GetFont (pdf, "Courier", NULL);
  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 12 );
  row_height = 25;
  posy -= 40;

  /*
  Add_text( string_8012[pdf_lng], MACHINE_ID, TEXT_MODE_LEFTRIGHT );
  Add_text( string_8021[pdf_lng], WtoA(current.recipe), TEXT_MODE_LEFTRIGHT );
  // batch name
  Add_text( string_8023[pdf_lng], WtoA(current.name), TEXT_MODE_LEFTRIGHT );
  // batch operator
  Add_text( string_8025[pdf_lng], WtoA(current.oper), TEXT_MODE_LEFTRIGHT );
  // batch description
  Add_text( string_8027[pdf_lng], WtoA(current.desc), TEXT_MODE_LEFTRIGHT );
  // batch status
  if ( current.status==1 ) Add_text( string_8033[pdf_lng], string_8034[pdf_lng], TEXT_MODE_LEFTRIGHT );
  else                     Add_text( string_8033[pdf_lng], string_8036[pdf_lng], TEXT_MODE_LEFTRIGHT );

  // batch start time
  if ( current.start_time>0 ) Add_text( string_8029[pdf_lng], time2txt(&current.start_time), TEXT_MODE_LEFTRIGHT );
  // batch end time
  if ( current.end_time>current.start_time && current.status==0 ) Add_text( string_8031[pdf_lng], time2txt(&current.end_time), TEXT_MODE_LEFTRIGHT );
  
  posy -= 10;
  sprintf ( text, string_8040[pdf_lng] ); // Batch responsible
  Add_text( text, TEXT_MODE_LEFT );

  posy -= 70;
  sprintf( text, string_8050[pdf_lng] );
  Add_text( text, TEXT_MODE_CENTER );
  posy -= 10;
  */

  UINT tot_isp = 0;
  UINT tot_rej = 0;


  // Read and add PLC statistics
  if ( ADS_status )
  {
    row_height = 18;

    int nr, nc;
    int ind=0;

    int nrows = 30;
    #ifdef TIVS_A60
      nrows      = 45;
      row_height = 14;
    #endif

    #ifdef TIVS_A60
      int modulo = 6;
      char* tcn[6] = { "TcF", "TcE", "TcD", "TcC", "TcB", "TcA" };
    #else
      int modulo = 4;
      char* tcn[4] = { "TcD", "TcC", "TcB", "TcA" };
    #endif


    for ( nc=0 ; nc<2 ; nc++ )
    {
      posy = 750;

      for ( nr=0 ; nr<nrows ; nr++ )
      {
//        sprintf( text, "GRIP[%02d]", ind+1 );
        sprintf( text, "Grp_%02d/%s", ind+1, tcn[ind%modulo] );
        sprintf( varn, "GRIPPERS_MANAGER.GRIPPER[%d]", ind );
        gripper_read( varn, text, &tot_isp, &tot_rej ); 
        posy -= row_height;
        //goto fine;
        ind++;
      }
      posx += 280;
    }
  }
  else Add_text( string_8090[pdf_lng], TEXT_MODE_LEFT );

  posx  = left_margin + 10;

  HPDF_Page_BeginText(page);

  sprintf( text, "Total number of rejects/inspections %d/%d", tot_rej, tot_isp );
  posy -= row_height;
  HPDF_Page_TextOut  (page, posx, posy, text ); 

  sprintf( text, "Average reject rate %3.3lf%%", tot_isp>0?100.0*tot_rej/tot_isp:0.0 );
  posy -= row_height;
  HPDF_Page_TextOut  (page, posx, posy, text ); 

  HPDF_Page_EndText  (page);


 //fine:

  CreateDirectory( BATCH_DIRECTORY, NULL );
   
  sprintf( full_name, "%s\\%s", WtoA(BATCH_DIRECTORY), file_name );
  HPDF_SaveToFile( pdf, full_name );
  HPDF_Free(pdf); 
  return 0;
}


/*
// CREATE BATCH REPORT FOR LIQUID INSPECTION MACHINE
int CBatch_manager::Create_page_0( void )
{
  return 0;
}
*/



int CBatch_manager::gripper_read( char* varname,  char* string, UINT* totisp, UINT* totrej )
{
  int  nisp = -1;
  int  nrej = -1;
  char txt[200];


  sprintf( txt, "%s.NISP", varname );
  if ( ADS_read_parameter( txt, &nisp ) )
  {
    //Add_text( string_8046[pdf_lng], TEXT_MODE_LEFT );
    return 99; // unable to read value
  }

  sprintf( txt, "%s.NREJ", varname );
  if ( ADS_read_parameter( txt, &nrej ) )
  {
    //Add_text( string_8046[pdf_lng], TEXT_MODE_LEFT );
    return 199; // unable to read value
  }

  *totisp += nisp;
  *totrej += nrej;

  sprintf( txt, "%s: %4d/%4d (%5.1lf%%)", string, nrej, nisp, nisp>0?100.0*nrej/nisp:0.0);

/*
  float x, y;
  // left field
  x = posx;
  y = posy;
  */
  HPDF_Page_BeginText(page);
  HPDF_Page_TextOut  (page, posx, posy, txt ); 
//  HPDF_Page_TextOut  (page, x, y, string ); 
  HPDF_Page_EndText  (page);

  //xl = x + HPDF_Page_TextWidth(page,string) + 10;
  //HPDF_Page_Stroke (page);

/*      // right field
      x = right_x - HPDF_Page_TextWidth(page,txt2);
      xr = x - 10;
      HPDF_Page_TextOut  (page, x, y, txt2 ); 
      HPDF_Page_EndText  (page);

      HPDF_Page_MoveTo (page, xl, yl );
      HPDF_Page_LineTo (page, xr, yl );
      HPDF_Page_Stroke (page);*/

  return 0;
}

/*
void error_handler( HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data )
{
  //printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
  TRACE("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
  longjmp(env, 1);
}
*/


// create pdf - lang can be ENGLISH(0) or CHINESE(1)
int CBatch_manager::Create_pdf( int lang )
{
//  ASSERT( lang==ENGLISH || lang==CHINESE || lang==SPANISH );
  ASSERT( lang>=0 && lang<NLANGS ); // || lang==CHINESE || lang==SPANISH );

  pdf_lng = lang;

  pdf = HPDF_New( error_handler, NULL );
  if ( !pdf ) 
  {
    swprintf( msg_err, 200, L"CBatch_manager::Create_pdf: unable to create pdf object" );
    return 1001;
  }

  if ( setjmp(env) )
  {
    HPDF_Free(pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

  // English
  fontname = "Courier"; // warning: case sensitive!
  encoding = NULL;
  codepage =  936;

  if ( pdf_lng==CHINESE ) // Chinese simplified
  {
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);
    fontname = "SimSun";   // warning: case sensitive!
    fontname = "SimHei";   // warning: case sensitive!
//    fontname = "Courier";   // warning: case sensitive!
    encoding = "GB-EUC-H"; // warning: case sensitive!
    //font = HPDF_GetFont( pdf, "SimSun", "GB-EUC-H" ); // ca uguale a CP936 - simplified chinese (no traditional)
  }

  if ( pdf_lng==RUSSIAN ) // Cyrillic
  {
    codepage =  21866;

//    HPDF_GetEncoder( pdf, encod );
//    TRACE("current encoder=%s\n",encod);
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

    encoding = "KOI8-R"; // warning: case sensitive!
    HPDF_STATUS encdr = HPDF_SetCurrentEncoder( pdf, encoding );
    TRACE("encdr=%d\n",encdr);

    HPDF_GetEncoder( pdf, encoding );
//    HPDF_GetEncoder( pdf, encod );
//    TRACE("current encoder=%s\n",encod);

    //encoding = "ISO8859-5"; // warning: case sensitive!
    //font = HPDF_GetFont( pdf, "SimSun", "GB-EUC-H" ); // ca uguale a CP936 - simplified chinese (no traditional)


  }



  num_pages = 1;
  Add_new_page( 1 );



/*  HPDF_Page_BeginText(page);
  unsigned char buf[2];
  buf[1] = 0;

  for ( int j=0xC ; j<=0xF; j++ )
  {
    for ( int i=0 ; i<16 ; i++ )
    {
      buf[0] = j * 16 + i;
      HPDF_Page_TextOut( page, 100 + i*20, 100 - (j-0xC)*15, (char*) buf );
    }
  }

  HPDF_Page_EndText  (page);*/



  Create_page_0();

  //Pour added 20180903
#if SERIAL_N==201500013 ||SERIAL_N==201800027  || SERIAL_N == 201900198/*|| SERIAL_N==201600003*/
  Txt2pdf();
  //Pour added 20190220
  Txt2pdf_alarm();
#endif
  //Pour added 20180903

  CreateDirectory( BATCH_DIRECTORY, NULL );
   
//  HPDF_SaveToFile( pdf, file_name );
//  sprintf( full_name, "%s\\%s", WtoA(BATCH_DIRECTORY), file_name );

    //Pour added for testing 20180905
  sprintf_s( full_name, "%s\\%s", WtoA(BATCH_DIRECTORY), file_name );
  HPDF_SaveToFile( pdf, full_name );
  HPDF_Free (pdf); 
  return 0;
}



// CREATE BATCH REPORT FOR LIQUID INSPECTION MACHINE
int CBatch_manager::Create_page_0( void )
{
  SYSTEMTIME time;
	GetLocalTime(&time);
  char text[200];

  //    font = HPDF_GetFont (pdf, "Courier", NULL);
  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 12 );
  row_height = 25;
  posy -= 70;

  //#define CUSTNM L"山东瑞阳"
  //Add_text( CUSTNM, TEXT_MODE_LEFT );

  #ifdef MACHINE_ID2 // if defined is used/printed in batch report
    //Add_text( string_8012[pdf_lng], MACHINE_ID2, TEXT_MODE_LEFTRIGHT );//Pour changed to machine model
	Add_text( string_8011[pdf_lng], MACHINE_ID2, TEXT_MODE_LEFTRIGHT );//Pour changed to machine model for Xiamen Tebao on20201031
  #else // otherwise use normal/standard id
    Add_text( string_8012[pdf_lng], MACHINE_ID,  TEXT_MODE_LEFTRIGHT );
  #endif

    /*
  Add_text( L"Normal: Hello world! Здравствуй, мир!", L"До свидания!", TEXT_MODE_LEFTRIGHT ); // current recipe

  Add_text( "Normal: Hello world! Здравствуй, мир!", "До свидания!", TEXT_MODE_LEFTRIGHT ); // current recipe

  Add_text( L"Normal: Hello world! Здравствуй, мир!", L"До свидания!", TEXT_MODE_LEFTRIGHT ); // current recipe
  */

  Add_text( string_8021[pdf_lng], WtoA(current.recipe), TEXT_MODE_LEFTRIGHT ); // current recipe


  #ifdef CIRON_BATCH   // to use ciron custom batch report
  //Pour removed 20180505
  /*
    Add_text( string_8023[pdf_lng], current.oper, TEXT_MODE_LEFTRIGHT ); // Product name
    Add_text( string_1328[pdf_lng], current.name, TEXT_MODE_LEFTRIGHT ); // Batch number
    Add_text( string_1329[pdf_lng], current.code, TEXT_MODE_LEFTRIGHT ); // Batch size
    Add_text( string_8024[pdf_lng], current.desc, TEXT_MODE_LEFTRIGHT ); // Batch operator
	*/
  //Pour removed 20180505
  /*
  //Pour added 20180505
      Add_text( string_8023a[pdf_lng], current.name, TEXT_MODE_LEFTRIGHT ); // Batch name
	  Add_text( string_1329a[pdf_lng], current.code, TEXT_MODE_LEFTRIGHT ); // Batch size
      Add_text( string_8024[pdf_lng], current.oper, TEXT_MODE_LEFTRIGHT ); // Product name
      Add_text( string_1330a[pdf_lng], current.desc, TEXT_MODE_LEFTRIGHT ); // Batch operator
	//Pour added 20180505
*/
	    //Pour added 20180505
      Add_text( string_8023a[pdf_lng], current.code, TEXT_MODE_LEFTRIGHT ); // Batch name
	  Add_text( string_1329a[pdf_lng], current.oper, TEXT_MODE_LEFTRIGHT ); // Batch size
      Add_text( string_8024[pdf_lng], current.desc, TEXT_MODE_LEFTRIGHT ); // Product name
      Add_text( string_1330a[pdf_lng], current.name, TEXT_MODE_LEFTRIGHT ); // Batch operator
	//Pour added 20180505
	
/*    Add_text( string_8023[pdf_lng], WtoA(current.oper), TEXT_MODE_LEFTRIGHT ); // Product name
    Add_text( string_1328[pdf_lng], WtoA(current.name), TEXT_MODE_LEFTRIGHT ); // Batch number
    Add_text( string_1329[pdf_lng], WtoA(current.code), TEXT_MODE_LEFTRIGHT ); // Batch size
    Add_text( string_8024[pdf_lng], WtoA(current.desc), TEXT_MODE_LEFTRIGHT ); // Batch operator */

  #else                // normal batch report

   // batch name
    Add_text( string_8023[pdf_lng], current.name, TEXT_MODE_LEFTRIGHT ); // product name

    // batch code
    Add_text( string_1328[pdf_lng], current.code, TEXT_MODE_LEFTRIGHT ); // Batch number

	#if SERIAL_N==202000022 || SERIAL_N==202000028

    // batch operator
    Add_text( string_8013[pdf_lng], current.oper, TEXT_MODE_LEFTRIGHT ); // Batch operator

    // batch description
    Add_text( string_8014[pdf_lng], current.desc, TEXT_MODE_LEFTRIGHT ); // Batch date

	#endif

	#if SERIAL_N !=202000022 
    // batch operator
    Add_text( string_8025[pdf_lng], current.oper, TEXT_MODE_LEFTRIGHT ); // Batch operator

    // batch description
    Add_text( string_8013[pdf_lng], current.desc, TEXT_MODE_LEFTRIGHT ); // Batch date//Pour marked

	#endif
 
 
 /*   // batch name
    Add_text( string_8023[pdf_lng], WtoA(current.name), TEXT_MODE_LEFTRIGHT ); // product name

    // batch code
    Add_text( string_1328[pdf_lng], WtoA(current.code), TEXT_MODE_LEFTRIGHT ); // Batch number

    // batch operator
    Add_text( string_8025[pdf_lng], WtoA(current.oper), TEXT_MODE_LEFTRIGHT ); // Batch operator

    // batch description
    Add_text( string_8027[pdf_lng], WtoA(current.desc), TEXT_MODE_LEFTRIGHT ); // Batch date  */
 
  #endif



  //// batch status
  //if ( current.status==1 )
  //{
  //  Add_text( string_8033[pdf_lng], string_8034[pdf_lng], TEXT_MODE_LEFTRIGHT );
  //}
  //else
  //{
  //  Add_text( string_8033[pdf_lng], string_8036[pdf_lng], TEXT_MODE_LEFTRIGHT );
  //}

  // batch start time
  if ( current.start_time>0 )
  {
    Add_text( string_8029[pdf_lng], time2txt(&current.start_time), TEXT_MODE_LEFTRIGHT );
  }

  // batch end time
  if ( current.end_time>current.start_time && current.status==0 )
  {
    Add_text( string_8031[pdf_lng], time2txt(&current.end_time), TEXT_MODE_LEFTRIGHT );
  }

  posy -= 10;
//  sprintf ( text, string_8040[pdf_lng] ); // Batch responsible
//  Add_text( text, TEXT_MODE_LEFT );
  Add_text( string_8040[pdf_lng], TEXT_MODE_LEFT );

  #ifdef CIRON_BATCH   // to use ciron custom batch report
    posy -= 10;
    Add_text( string_8041[pdf_lng], TEXT_MODE_LEFT );
  #endif



  posy -= 50;
//  sprintf( text, string_8050[pdf_lng] );
//  Add_text( text, TEXT_MODE_CENTER );
  Add_text( string_8050[pdf_lng], TEXT_MODE_CENTER );
  posy -= 10;



  // Read and add PLC statistics
  if ( ADS_status )
  {
    row_height = 20;
	//row_height = 40;

    //#ifdef TIVS_C40 // MODEL TIVS-C40L
    #if defined TIVS_C40 || defined TIVS_C60  // if combo machine
      if ( freezemode_on ) Add_text( string_8094[pdf_lng], TEXT_MODE_LEFT ); // freeze mode ENABLED 
      else                 Add_text( string_8092[pdf_lng], TEXT_MODE_LEFT ); // liquid mode ENABLED
    #endif

    ADS_read_parameter( ".NUM_PROD_IN", &n_prod_in );
    if ( n_prod_in == 0 )
    {
      //sprintf( text, string_8052[pdf_lng] );
      //Add_text( text, TEXT_MODE_LEFT );
      Add_text( string_8052[pdf_lng], TEXT_MODE_LEFT );
    }
    else
    {
      sprintf( text, "%d pcs (%7.3lf%%)", n_prod_in, 100.0 );
      Add_text( string_8055[pdf_lng], text, TEXT_MODE_LEFTRIGHT );

//Pour added for Kexing
#if SERIAL_N==202000022 || SERIAL_N==202000028
	  Add_statistic( ".NUM_SAMPLING",         string_8060 ); 
#endif
//Pour added for Kexing

      Add_statistic( ".NUM_GOODS",         string_8057 ); 

	  #ifdef MANU_CHECK
	  Add_statistic( ".NUM_REJ_GOODS",    string_8058 ); 
		#endif

      Add_statistic( ".NUM_REJECTS",       string_8059 ); 

      #if ( !defined TIVS_A60 && !defined TIVS_C20 &&!defined TIVS_S60 ) || defined DOUBLE_REJET // single deviator 
        Add_statistic( ".NUM_REJECTS1",      string_8061 ); 
        Add_statistic( ".NUM_REJECTS2",      string_8063 ); 
        #if defined THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
          Add_statistic( ".NUM_REJECTS3",    string_8064 ); // HGA
        #endif
      #endif


      #ifdef TIVS_C40 // MODEL TIVS-C40L
        Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap 
        Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color

        if ( freezemode_on )  // freeze mode ENABLED 
        {
          //Add_text( string_8055[pdf_lng], text, TEXT_MODE_LEFTRIGHT );
          Add_statistic( ".NUM_BOTTOMA_REJ",   string_8109 ); // Bottom A
          Add_statistic( ".NUM_BOTTOMB_REJ",   string_8111 ); // Bottom B
          Add_statistic( ".NUM_PARTICLE1_REJ", string_8105 ); // Side Cake

		#ifdef HEEL_INSPECTION
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
        #else
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake
        #endif

          //Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
		  //Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Topcake
        }
        else // Liquid mode ENABLED
        {
          Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); // Particle rejects
          Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); // Particle1 rejects
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); // Particle2 rejects
		  		  
		#ifdef OPACITY_CTRL
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8074 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8073 ); 
        #else
          Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 
        #endif

          //Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
          //Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
          //Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 
        }
		  Add_statistic( ".NUM_BOTTOMA_REJ",   string_8109 ); // Bottom A
          Add_statistic( ".NUM_BOTTOMB_REJ",   string_8111 ); // Bottom B

        Add_statistic( ".NUM_COSMETIC_REJ",  string_8079 ); // Cosmetic

        #ifdef HGA_LEAKTEST      // HGA components installed
          #if defined THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
            //Add_statistic( ".NUM_REJECTS3", string_8116 ); // HGA
          #else
            Add_statistic( ".NUM_HGA_REJ", string_8116 ); // HGA
          #endif
        #endif

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 
      #endif // TIVS_C40


      #ifdef TIVS_A60 // MODEL TIVS-A60 - LIQUID
      
        Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); 
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); 
        Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 

        #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // PARTICLE ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8071 ); 
        #else // COSMETIC ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
        #endif

        //Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
        //Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
        //Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 );
		  
		  #ifdef OPACITY_CTRL
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8074 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8073 ); 
        #else
          Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 
        #endif

        #ifdef FO_COLOR
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap rejects
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color
        #else
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8081 ); // Tip rejects
        #endif

        #ifdef HGA_LEAKTEST      // HGA components installed
          Add_statistic( ".NUM_HGA_REJ", string_8116 ); // HGA
        #endif

		#ifdef HV_LEAKTEST
		  Add_statistic( ".NUM_HV_REJ",       string_hvrej ); 
		#endif

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 

      #endif // TIVS_A60

      #ifdef TIVS_A40 // MODEL TIVS-A40 - LIQUID
      
        Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); 
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); 

//        Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 

        #if STN2==COSMETIC_BACK
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 
        #else
          #if STN2==PARTICLE_BOTTOM || STN2==PARTICLE_BACK // PARTICLE ON SECOND STATION
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 
          #else // COSMETIC ON SECOND STATION
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8079 ); 
          #endif
        #endif

        #if STN2==COSMETIC_BACK
          Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
        #else
          #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // PARTICLE ON THIRD STATION
            Add_statistic( ".NUM_COSMETIC_REJ", string_8071 ); 
          #else // COSMETIC ON THIRD STATION
            Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
          #endif
        #endif


        #ifdef OPACITY_CTRL
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8074 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8073 ); 
        #else
          Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 
        #endif

        #ifdef FO_COLOR
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap rejects
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color
        #else
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8081 ); // Tip rejects
        #endif

        #ifdef LIQUID_COLOR
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8118 ); // Liquid color rejects
        #endif

        #ifdef DOTRING_COLOR
          Add_statistic( ".NUM_DRCOL_REJ",   string_8088 ); // Dot/ring color rejects
        #endif

        #ifdef HGA_LEAKTEST      // HGA components installed
          Add_statistic( ".NUM_HGA_REJ", string_8116 ); // HGA
        #endif

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 

      #endif // TIVS_A40

      #ifdef TIVS_F40 // MODEL TIVS-F40 - FREEZE DRIED
        Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap 
        Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color

        Add_statistic( ".NUM_BOTTOMA_REJ",   string_8109 ); // Bottom A
        Add_statistic( ".NUM_BOTTOMB_REJ",   string_8111 ); // Bottom B

        Add_statistic( ".NUM_COSMETIC_REJ",  string_8103 ); // Sidewall
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8105 ); // Side Cake

        #ifdef HEEL_INSPECTION
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
        #else
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake
        #endif

        #if STN1==CAKESIDE_ARBACK // WAIST LAYER
          Add_statistic( ".NUM_WAISTLAYR_REJ", string_8114 ); // Waist/layered
        #endif

        #ifdef HGA_LEAKTEST      // HGA components installed
          Add_statistic( ".NUM_HGA_REJ", string_8116 ); // HGA
        #endif

/*      #if SERIAL_N==201400011 // third station changed to heel inspection
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8108 ); // Heel inspection
        #else
          #if SERIAL_N==201400022 // Yaopharma top-cake changed to HEEL cameras
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
          #else  // Normal - Top-cake
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake
          #endif
        #endif
        */

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 
      #endif // TIVS_F40

		      #ifdef TIVS_F20 // MODEL TIVS-F40 - FREEZE DRIED
        Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap 
        Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color

        Add_statistic( ".NUM_BOTTOMA_REJ",   string_8109 ); // Bottom A
        Add_statistic( ".NUM_BOTTOMB_REJ",   string_8111 ); // Bottom B

        Add_statistic( ".NUM_COSMETIC_REJ",  string_8103 ); // Sidewall
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8105 ); // Side Cake

        #ifdef HEEL_INSPECTION
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
        #else
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake
        #endif

        #if STN1==CAKESIDE_ARBACK // WAIST LAYER
          Add_statistic( ".NUM_WAISTLAYR_REJ", string_8114 ); // Waist/layered
        #endif

        #ifdef HGA_LEAKTEST      // HGA components installed
          Add_statistic( ".NUM_HGA_REJ", string_8116 ); // HGA
        #endif

/*      #if SERIAL_N==201400011 // third station changed to heel inspection
          Add_statistic( ".NUM_PARTICLE2_REJ", string_8108 ); // Heel inspection
        #else
          #if SERIAL_N==201400022 // Yaopharma top-cake changed to HEEL cameras
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8112 ); // Heel
          #else  // Normal - Top-cake
            Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake
          #endif
        #endif
        */

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 
      #endif // TIVS_F20


		#ifdef TIVS_S40  // MODEL TIVS-S40
        Add_statistic( ".NUM_REJECTS1",      string_1strej ); 
        Add_statistic( ".NUM_REJECTS2",      string_2ndrej ); 
        Add_statistic( ".NUM_REJECTS3",      string_3rdrej ); // rejects tray 3
        Add_statistic( ".NUM_REJECTS4",      string_4threj ); // rejects tray 4 

        Add_statistic( ".NUM_PARTICLE_REJ",  string_partrej   ); // particle  rejects
        Add_statistic( ".NUM_PARTICLE1_REJ", string_part1rej  ); // particle1 rejects
        Add_statistic( ".NUM_PARTICLE2_REJ", string_part2rej  ); // particle2 rejects
        Add_statistic( ".NUM_COSMETIC_REJ",  string_cosmrej   ); // cosmetic1 rejects
        Add_statistic( ".NUM_COSMETIC3_REJ", string_cosm2rej  ); // cosmetic2 rejects
        Add_statistic( ".NUM_FILLLEV_REJ",   string_fillevrej ); // fillinglv rejects
        Add_statistic( ".NUM_BOTTOMA_REJ",   string_cosm3arej ); // cosmetic3a rejects
        Add_statistic( ".NUM_BOTTOMB_REJ",   string_cosm3brej ); // cosmetic3b rejects
        Add_statistic( ".NUM_SPIN_REJ",      string_8089      ); // spin rejects
      #endif

		#ifdef TIVS_S60 // MODEL TIVS-S60
        Add_statistic( ".NUM_REJECTS1",      string_1strej ); 
        Add_statistic( ".NUM_REJECTS2",      string_2ndrej ); 
        Add_statistic( ".NUM_REJECTS3",      string_3rdrej ); // rejects tray 3
        //Add_statistic( ".NUM_REJECTS4",      string_4threj ); // rejects tray 4 

		Add_statistic( ".NUM_CAPSEAL_REJ",   string_flangerej ); // rejects tray 3
        Add_statistic( ".NUM_PARTICLE_REJ",  string_partrej   ); // particle  rejects
        Add_statistic( ".NUM_PARTICLE1_REJ", string_part1rej  ); // particle1 rejects
        Add_statistic( ".NUM_PARTICLE2_REJ", string_part2rej  ); // particle2 rejects
        Add_statistic( ".NUM_COSMETIC_REJ",  string_cosmrej   ); // cosmetic1 rejects
        Add_statistic( ".NUM_COSMETIC3_REJ", string_cosm2rej  ); // cosmetic2 rejects
        //Add_statistic( ".NUM_FILLLEV_REJ",   string_fillevrej ); // fillinglv rejects
		#ifdef OPACITY_CTRL
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8074 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8073 ); 
        #else
          Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
          Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
          Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 
        #endif

        Add_statistic( ".NUM_BOTTOMA_REJ",   string_cosm3arej ); // cosmetic3a rejects
        Add_statistic( ".NUM_BOTTOMB_REJ",   string_cosm3brej ); // cosmetic3b rejects
        Add_statistic( ".NUM_SPIN_REJ",      string_8089      ); // spin rejects
        #endif




      #ifdef TIVS_C20 // 
        Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap 
        Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color

        Add_statistic( ".NUM_BOTTOMA_REJ",   string_8109 ); // Bottom A
        Add_statistic( ".NUM_BOTTOMB_REJ",   string_8111 ); // Bottom B

        Add_statistic( ".NUM_PARTICLE1_REJ", string_8120 ); // Particle1/Sidewall
        Add_statistic( ".NUM_COSMETIC_REJ",  string_8122 ); // Particle2/Sidecake
        Add_statistic( ".NUM_PARTICLE2_REJ", string_8107 ); // Top Cake

        Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); // Filling level

        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 
      #endif // TIV_C20

      #ifdef TIVS_A20L // MODEL TIVS-A40L - LIQUID+HV
      
        Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); 
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); 
        Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 

        #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // PARTICLE ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8071 ); 
        #else // COSMETIC ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
        #endif

        Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
        //Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
        //Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 

        #ifdef FO_COLOR
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap rejects
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color
        #else
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8081 ); // Tip rejects
        #endif

        #ifdef DOTRING_COLOR
          Add_statistic( ".NUM_DRCOL_REJ",   string_8088 ); // Dot/ring color rejects
        #endif
          
        Add_statistic( ".NUM_HV_REJ",        string_hvrej ); 
        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 

      #endif // TIVS_A20L

//Pour added 20180523
      #ifdef TIVS_A20 // MODEL TIVS-A40L - LIQUID+HV
      
        Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); 
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); 
        Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 

        #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // PARTICLE ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8071 ); 
        #else // COSMETIC ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
        #endif

        Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
        //Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
        //Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 

        #ifdef FO_COLOR
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap rejects
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color
        #else
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8081 ); // Tip rejects
        #endif

        #ifdef DOTRING_COLOR
          Add_statistic( ".NUM_DRCOL_REJ",   string_8088 ); // Dot/ring color rejects
        #endif
          
        //Add_statistic( ".NUM_HV_REJ",        string_hvrej ); 
        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 

      #endif // TIVS_A20

//Pour added 20180523

//Pour added 20180124
		      #ifdef TIVS_A40L // MODEL TIVS-A40L - LIQUID+HV
      
        Add_statistic( ".NUM_PARTICLE_REJ",  string_8065 ); 
        Add_statistic( ".NUM_PARTICLE1_REJ", string_8067 ); 
        Add_statistic( ".NUM_PARTICLE2_REJ", string_8069 ); 

        #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // PARTICLE ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8071 ); 
        #else // COSMETIC ON THIRD STATION
          Add_statistic( ".NUM_COSMETIC_REJ", string_8079 ); 
        #endif

        Add_statistic( ".NUM_FILLLEV_REJ",   string_8073 ); 
        //Add_statistic( ".NUM_FILLLEV1_REJ",  string_8075 ); 
        //Add_statistic( ".NUM_FILLLEV2_REJ",  string_8077 ); 

        #ifdef FO_COLOR
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8083 ); // Cap rejects
          Add_statistic( ".NUM_FPOFFCOL_REJ",  string_8087 ); // FO color
        #else
          Add_statistic( ".NUM_CAPSEAL_REJ",   string_8081 ); // Tip rejects
        #endif

        #ifdef DOTRING_COLOR
          Add_statistic( ".NUM_DRCOL_REJ",   string_8088 ); // Dot/ring color rejects
        #endif
          
        Add_statistic( ".NUM_HV_REJ",        string_hvrej ); 
        Add_statistic( ".NUM_SPIN_REJ",      string_8089 ); 

      #endif // TIVS_A40L
//Pour added 20180124
    }
  }
  else Add_text( string_8090[pdf_lng], TEXT_MODE_LEFT );
  return 0;
}





int CBatch_manager::Add_statistic( char* varname,  wchar_t** string )
{
  int     value = -1;
  wchar_t text[200];

  if ( ADS_read_parameter( varname, &value ) )
  {
    Add_text( string_8046[pdf_lng], TEXT_MODE_LEFT );
    return 99; // unable to read value
  }

  swprintf( text, L"%d pcs (%7.3lf%%)", value, 100.0*value/n_prod_in );
  Add_text( string[pdf_lng], text, TEXT_MODE_LEFTRIGHT );

  return 0;
}




int CBatch_manager::Add_statistic( char* varname,  char** string )
{
  int  value = -1;
  char text[200];

  if ( ADS_read_parameter( varname, &value ) )
  {
    Add_text( string_8046[pdf_lng], TEXT_MODE_LEFT );
    return 99; // unable to read value
  }

  sprintf( text, "%d pcs (%7.3lf%%)", value, 100.0*value/n_prod_in );
  Add_text( string[pdf_lng], text, TEXT_MODE_LEFTRIGHT );

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


int CBatch_manager::Add_text( const wchar_t* txt1, const char* txt2, int mode ) 
{
  char txtpdf[200];
//  int ret = WideCharToMultiByte( 936, 0, txt1, -1, txtpdf, 200, NULL, NULL );
  


/*  UINT codepage = 936;
  if ( lng==RUSSIAN ) codepage = 21866; */
  int ret = WideCharToMultiByte( codepage, 0, txt1, -1, txtpdf, 200, NULL, NULL );


/*  if ( lng==RUSSIAN )
  {
    int ret = WideCharToMultiByte( 21866, 0, txt1, -1, txtpdf, 200, NULL, NULL );
  }
  else
  {
    int ret = WideCharToMultiByte( 936, 0, txt1, -1, txtpdf, 200, NULL, NULL );
  } */
  
  return Add_text( txtpdf, txt2, mode );
}


int CBatch_manager::Add_text( const wchar_t* txt1, const wchar_t* txt2, int mode ) 
{
/*  UINT codepage = 936;
  if ( lng==RUSSIAN ) codepage = 21866;*/

  char txtpdf1[200];
  int ret = WideCharToMultiByte( codepage, 0, txt1, -1, txtpdf1, 200, NULL, NULL );

  char txtpdf2[200];
  ret = WideCharToMultiByte( codepage, 0, txt2, -1, txtpdf2, 200, NULL, NULL );

  return Add_text( txtpdf1, txtpdf2, mode );
}




int CBatch_manager::Add_text( const char* txt1, const char* txt2, int mode ) 
{
  float x, y;
  float xl, xr; // left/right coords for line
  float yl;

  float right_x = right_margin-10; // 500;

  switch ( mode )
  {
    case TEXT_MODE_LEFTRIGHT:

      // left field
      x = posx;
      y = posy;

      yl = y;
      HPDF_Page_BeginText(page);
      
      // left field
      if ( txt1 )
      {
        HPDF_Page_TextOut  (page, x, y, txt1 ); 
        xl = x + HPDF_Page_TextWidth(page,txt1) + 10;
      }

      // right field
      if ( txt2 )
      {
        x = right_x - HPDF_Page_TextWidth(page,txt2);
        xr = x - 10;
        HPDF_Page_TextOut  (page, x, y, txt2 ); 
      }
     
      HPDF_Page_EndText(page);
      HPDF_Page_MoveTo (page, xl, yl );
      HPDF_Page_LineTo (page, xr, yl );
      HPDF_Page_Stroke (page);


      break;

 /*   case TEXT_MODE_CENTER:
      x = (left_margin+right_margin)/2 - HPDF_Page_TextWidth(page,txt)/2;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_RIGHT:
      x = right_margin - HPDF_Page_TextWidth( page, txt ) - (posx-left_margin);
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break; */

    default:
      TRACE("CBatch_management::Add_text called with unknown mode\n");

  }

  posy -= row_height;
  return 0;
}






int CBatch_manager::Add_text( const wchar_t* txt, int mode ) 
{
//  float x, y;

  char txtpdf[200];

 /* UINT codepage = 936;
  if ( lng==RUSSIAN ) codepage = 21866; */
  int ret = WideCharToMultiByte( codepage, 0, txt, -1, txtpdf, 200, NULL, NULL );

//  int ret = WideCharToMultiByte( 936, 0, txt, -1, txtpdf, 200, NULL, NULL );

  return Add_text( txtpdf, mode );

/*  switch ( mode )
  {
    case TEXT_MODE_LEFT:
      x = posx;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txtpdf );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_CENTER:
//      x = left_margin+(page_dimx-right_margin-HPDF_Page_TextWidth(page,txt))/2;
      x = (left_margin+right_margin)/2 - HPDF_Page_TextWidth(page,txtpdf)/2;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txtpdf );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_RIGHT:
//      x = page_dimx - right_margin - HPDF_Page_TextWidth( page, txt );
      x = right_margin - HPDF_Page_TextWidth( page, txtpdf ) - (posx-left_margin);
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txtpdf );
      HPDF_Page_EndText  (page);
      break;

    default:
      TRACE("CBatch_management::Add_text called with unknown mode\n"); 

  }

  //  posy -= 25; //HPDF_Page_TextHeight( page, txt );
  posy -= row_height; //HPDF_Page_TextHeight( page, txt );
  return 0;*/
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


// convert input CTime into output string according to format 0=default - 10=YYYYMMDD_HHMMSS
char *CBatch_manager::time2txt( CTime* time, int format )
{
  static char txt[100];

  switch ( format )
  {
    case  0:  // default format YYYY/MM/DD HH:MM:SS
      sprintf( txt, "%04d/%02d/%02d %02d:%02d:%02d", time->GetYear(), time->GetMonth(), time->GetDay(), time->GetHour(), time->GetMinute(), time->GetSecond() );
      break;

    case 10: // file name format YYYYMMDD_HHMMSS
      sprintf( txt, "%04d%02d%02d_%02d%02d%02d", time->GetYear(), time->GetMonth(), time->GetDay(), time->GetHour(), time->GetMinute(), time->GetSecond() );
      break;
  }

  return txt;
}


int CBatch_manager::Add_new_page( int npg )
{
  char text[200];
  char str[200];

  page = HPDF_AddPage(pdf);
  HPDF_Page_SetSize( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

  page_dimx     = HPDF_Page_GetWidth (page);
  page_dimy     = HPDF_Page_GetHeight(page);
  left_margin   = page_dimx/20;
  right_margin  = page_dimx - left_margin;
  top_margin    = page_dimy*25/26;
  bottom_margin = page_dimy - top_margin;
  row_height    = 30;

  //TRACE ("<Page dimension: %f x %f >\n", page_dimx, page_dimy );
  //TRACE ("<Page margins: %f,%f %f,%f >\n", left_margin, top_margin, right_margin, bottom_margin );

  HPDF_Page_SetLineWidth (page, 0.5);
  HPDF_Page_Rectangle (page, left_margin, top_margin, right_margin-left_margin, bottom_margin-top_margin );
  HPDF_Page_Rectangle( page, left_margin, top_margin, right_margin-left_margin, -60 );
  //  HPDF_Page_Rectangle( page, left_margin, (page_dimy-top_margin-20), page_dimx-right_margin-left_margin, page_dimy );
  HPDF_Page_Stroke (page);

  posx = left_margin + 10;
  posy = top_margin  - 24; // Start from top of page

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 18 );

//  sprintf ( text, string_8000[pdf_lng] ); 
//  Add_text( text, TEXT_MODE_CENTER );
  Add_text( string_8000[pdf_lng], TEXT_MODE_CENTER );

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize (page, font, 12 );
  posy += 10;

//  sprintf( text, "%s - %s", CUSTOMER, MODEL );
//  Add_text( text, TEXT_MODE_CENTER );

  wchar_t wtext[200];
  //if ( pdf_lng==CHINESE ) swprintf( wtext, L"%s - %s", CUSTOMER_NAME,  MODEL_NAME ); // use chinese customer name
  if ( pdf_lng==CHINESE ) swprintf( wtext, L"%s", CUSTOMER_NAME ); // use chinese customer name
  else                    swprintf( wtext, L"%s - %s", CUSTOMER_NAMEE, MODEL_NAME ); // use english customer name
  Add_text( wtext, TEXT_MODE_CENTER );

  font = HPDF_GetFont( pdf, fontname, encoding );
  HPDF_Page_SetFontAndSize( page, font, 11 );

  int ret = WideCharToMultiByte( codepage /*936*/, 0, string_8006[pdf_lng], -1, str, 200, NULL, NULL );


//  sprintf ( text, string_8006[pdf_lng], npg, num_pages );
  sprintf ( text, str, npg, num_pages );

  HPDF_Page_BeginText(page);

  HPDF_Page_TextOut  (page, left_margin + 10 , bottom_margin-12, file_name );
  //Pour marked on 20201031 to print batch code
  //sprintf( current_code, "%s.pdf", current.code );
  //HPDF_Page_TextOut  (page, left_margin + 10 , bottom_margin-12, current_code );

  HPDF_Page_TextOut  (page, right_margin- 60, bottom_margin-12, text );

  HPDF_Page_EndText  (page);

  posx = left_margin + 10;
  posy = top_margin  - 24; // Start from top of page

  return 0;
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


/*

int CBatch_manager::Add_text( const wchar_t* txt, int mode ) 
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

*/
