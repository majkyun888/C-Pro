// Log_viewer.cpp : implementation file
//

#include "stdafx.h"
#include "Log_viewer.h"
#include "afxdialogex.h"
//Pour added 20190328
#include <locale> 
#include <afxdisp.h>

#include "afx.h"

// CLog_viewer dialog

CStdioFile log_file;
CStdioFile alarm_log_file;

IMPLEMENT_DYNAMIC(CLog_viewer, CDialog)

extern wchar_t key[];

//Pour added 20190328
extern UINT     lng;
//----------------------
extern CBatch_manager*           batch_mngr;
//----------------------


#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif



CLog_viewer::CLog_viewer(wchar_t* name, CWnd* pParent /*=NULL*/) : CDialog(CLog_viewer::IDD, pParent)
{
  file_name = name;
  CEdit* editptr = NULL;
}


CLog_viewer::CLog_viewer(CWnd* pParent /*=NULL*/) : CDialog(CLog_viewer::IDD, pParent)
{
  //file_name = "C:\\IVS\\log.txt";

  #ifdef NEW_LOGGER      // New log 
    file_name = NEWLOG_FILE_NAME;
  #else
    file_name = LOG_FILE_NAME;
  #endif

  CEdit* editptr = NULL;
}

CLog_viewer::~CLog_viewer()
{
}

void CLog_viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLog_viewer, CDialog)
  ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(ID_SCROLLUP, &CLog_viewer::OnBnClickedScrollup)
  ON_BN_CLICKED(ID_SCROLLDN, &CLog_viewer::OnBnClickedScrolldn)
  ON_BN_CLICKED(ID_SAVE_TXT, &CLog_viewer::OnBnClickedSaveTxt)
  ON_BN_CLICKED(ID_LOAD_LOG, &CLog_viewer::OnBnClickedLoadLog)
END_MESSAGE_MAP()


int CLog_viewer::Load_and_show( wchar_t* file ) 
{
  CFile fp; 
  CFileException ex;

  editptr = (CEdit*) GetDlgItem( IDC_EDIT_LV );
  ASSERT( editptr );

  editptr->SetWindowText( string_3920[lng] );//Pour marked 20190524
  if ( !PathFileExists( file ) ) return 60; // file doesn't exist, nothing to do

  if ( !fp.Open( file , CFile::modeRead, &ex ) )
  {
    ex.ReportError();
    return 65;
  }

  if ( fp.GetLength()<5 )
  {
    fp.Close();
    return 70; // too small
  }

  #ifdef NEW_LOGGER // log alarms separately and add button to show them

    UINT flen = UINT(fp.GetLength());
    UINT klen = wcslen(key);

    UINT crcr = 0x7F3A4322; // seed read
    UINT crcc = crcr;       // seed calculated

    if ( flen>4 ) // file exists, read first 4 bytes as crc
    {
      fp.Read(&crcr,4);
    }
    else // file too short> error
    {
      Message_box( L"Log file size too short", MBOX_ERROR );
      return 80;
    }

    //TRACE( "Read crc=%08X (pos=%d)\n",crcr,fp.GetPosition() );

    ASSERT( fp.GetPosition()==4 );

    wchar_t* content2 = ( wchar_t* ) malloc( flen ); 
    ZeroMemory( content2, flen );

    fp.Read( content2 , flen-4 );
    //TRACE( "Read encrypted buff (pos=%d)\n",fp.GetPosition() );

    fp.Close();

    UINT blen = (flen-4)/sizeof(wchar_t);

    // encrypt and update crc
    for ( UINT i=0 ; i<blen ; i++ )
    {
      //UINT p = i + pos;
      ASSERT( (i+2)%klen>=0 && (i+2)%klen<klen );
      //TRACE( L"buff[%02d]=%d - ",i,content2[i]);
      //TRACE( L"key[%02d]=%d\n",i%klen,key[i%klen]);

      content2[i] = content2[i]^key[(i+2)%klen];
      crcc += content2[i]^(i+2);
    }
    //TRACE( "Read crc=%08X - Calculated crc=%08X\n",crcr,crcc );


    if ( crcc==crcr ) // crc match, show content
    {
      //TRACE("CEdit object GetLimitText=%d char\n",editptr->GetLimitText() );
      editptr->SetLimitText( 1000000 );
      //TRACE("CEdit object GetLimitText=%d chadr\n",editptr->GetLimitText() );
      editptr->SetWindowText( content2 );
    }
    else // crc mismatch: file corrupted
    {
      CString txt,newname;
      newname.Format( L"corrupted_%s", file );
      CopyFile(file,newname,TRUE);
      DeleteFile(file);
      txt.Format( string_3930[lng], file, newname );
      editptr->SetWindowText( txt );
      Message_box( txt, MBOX_INFO );
    }

    free( content2 );

    editptr->LineScroll( editptr->GetLineCount() );
    return 90;

  #endif

   //normal / unencrypted version
  TRACE("Opening and copying %d bytes from file <%s>\n",fp.GetLength(),file_name.GetBuffer());

  CString content( '\0', (int) fp.GetLength() + 1 );

  fp.Read( content.GetBuffer(), (int) fp.GetLength() );

  TRACE("CEdit object GetLimitText=%d char\n",editptr->GetLimitText() );
  editptr->SetLimitText( 1000000 );
  TRACE("CEdit object GetLimitText=%d char\n",editptr->GetLimitText() );
  editptr->SetWindowText( content );

//  TRACE("CEdit object filled with %d char\n",editptr->GetScrollLimit

  fp.Close();
  editptr->LineScroll( editptr->GetLineCount() );

  return 0;
}


void CLog_viewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{


  // Update texts
  CHNGCTRLNAME( IDOK,        string_3904[lng] );
  CHNGCTRLNAME( ID_SCROLLUP, string_424 [lng] );
  CHNGCTRLNAME( ID_SCROLLDN, string_426 [lng] );

  if ( password_mngr && password_mngr->current_level>MAINTENANCE_LEVEL )
  {
//    if ( GetDlgItem(ID_LOAD_LOG) && GetDlgItem(ID_LOAD_LOG)->m_hWnd ) GetDlgItem(ID_LOAD_LOG)-ShowWindow( SW_SHOW );
    if ( GetDlgItem(ID_LOAD_LOG) && GetDlgItem(ID_LOAD_LOG)->m_hWnd )
    {
      GetDlgItem(ID_LOAD_LOG)-ShowWindow( SW_HIDE );
      GetDlgItem(ID_LOAD_LOG)->EnableWindow(TRUE);//FALSE Pour markded on 20200427
      GetDlgItem(ID_LOAD_LOG)->SetWindowText(string_3912[lng]);
      GetDlgItem(ID_LOAD_LOG)->RedrawWindow();
    }
    //CHNGCTRLNAME( ID_LOAD_LOG, string_3912[lng] );
  }
  else
  {
    if ( GetDlgItem(ID_LOAD_LOG) && GetDlgItem(ID_LOAD_LOG)->m_hWnd )
    {
      GetDlgItem(ID_LOAD_LOG)-ShowWindow( SW_HIDE );
      GetDlgItem(ID_LOAD_LOG)->EnableWindow(FALSE);
     GetDlgItem(ID_LOAD_LOG)->RedrawWindow();
     }
  }
  
  CHNGCTRLNAME( ID_SAVE_TXT, string_3914[lng] );//Pour changed from string_3910 to string_3914 on 20190603

  CDialog::OnShowWindow(bShow, nStatus);

  Load_and_show( file_name.GetBuffer(0) );



  // TODO: Add your message handler code here
}


void CLog_viewer::OnBnClickedScrollup()
{
  ASSERT( editptr );

  editptr->LineScroll(-10);

  TRACE("OnBnClickedScrollup\n");
  // TODO: Add your control notification handler code here
}


void CLog_viewer::OnBnClickedScrolldn()
{

  editptr->LineScroll(10);

  TRACE("OnBnClickedScrolldn\n");
  // TODO: Add your control notification handler code here
}


// Save current shown log into text or PDF file
void CLog_viewer::OnBnClickedSaveTxt()
{
  editptr = (CEdit*) GetDlgItem( IDC_EDIT_LV );
  ASSERT( editptr );
  //CStdioFile fp; 
  CFileException ex;
  //CString file_name;
  CString fname;
  //file_name.Format( L"exported_logfile.txt" );//Pour removed on 20200507
  fname = _T("\\exported_logfile.txt");       //Pour removed on 20200507
  fname = MAIN_DIRECTORY + fname;//Pour removed for testing on 20200507
  //fname = LOG_DIRECTORY + fname;//Pour added on 20200507

    //if ( !log_file.Open( file_name , CFile::modeCreate | CFile::modeReadWrite | CFile::typeText, &ex ) )//Pour removed on 20191024
	if ( !log_file.Open( fname , CFile::modeCreate | CFile::modeReadWrite | CFile::typeText, &ex ) )//Pour added on 20191024 to replace the above
	{
    return;
	}
	if(lng == CHINESE)
	setlocale( LC_CTYPE, ("chs")); // if language is CHN, must set the LC_TYPE, or it will cause error

  CString content;
 //int lenght = editptr->GetWindowText( content.GetBuffer(0), MAX_LOG_SIZE );
  editptr->GetWindowText(content);
  if (content.GetLength()>1)
	  log_file.WriteString(content);
  else
  {
	  log_file.Write( L"Failed to read content of log",50 );
	  CString txt;
	  txt.Format( string_3920[lng],fname );
	  Message_box( txt, MBOX_INFO);
	  return;
  }

  //make pdf file, print the data displayed in dialog: Type=0 log, type=1,alarmlog
  int type=0;
  //if ( !wcscmp(file_name, ALARMLOG_FILE_NAME ) /*|| !wcscmp( file_name, NEWLOG_FILE_NAME )*/  )
	  if ( !wcscmp(file_name, LOG_FILE_NAME ) || !wcscmp( file_name, NEWLOG_FILE_NAME )  )
	 type = 1;
  
  
  if (!(batch_mngr->Create_Log_pdf(lng,type)))
  {	  
	  //Pour removed on 20190601
	  
	  //CString txt;
	  //txt.Format( string_3932[lng],fname );
	  //Message_box( txt, MBOX_INFO);
	  
  }

  log_file.Close();//Pour removed on 20200710
  //Pour removed on 20190601
  /*
  CString txt;
  txt.Format( string_3932[lng], file_name );
  Message_box( txt, MBOX_INFO );
  */
  TRACE("OnBnClickedSaveTxt\n");
}


// Load another older file
void CLog_viewer::OnBnClickedLoadLog()
{

  wchar_t szFilters[]= L"Encrypted log files (*.lgf)|*.lgf||";
  CFileDialog selfile(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,szFilters,pView);
  selfile.m_ofn.lpstrInitialDir = MAIN_DIRECTORY; // SG_INPUT_PATH;//Pour removed for testing on 20200507
  //selfile.m_ofn.lpstrInitialDir = LOG_DIRECTORY;//Pour added on 20200507

  if ( selfile.DoModal() == IDOK )
  {
    TRACE( L"Choosen <%s>\n", selfile.GetFileName() );
    Load_and_show( selfile.GetFileName().GetBuffer(0) );
  }



  // TODO: Add your control notification handler code here
  TRACE("OnBnClickedLoadLog\n");
}
