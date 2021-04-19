#include "StdAfx.h"
#include "Logger.h"
#include "Globals.h"


extern wchar_t          PC_name[30];

//Pour added 20180903
extern CBatch_manager*   batch_mngr;
extern CStdioFile        log_file;
extern CStdioFile        alarm_log_file;
//Pour added 20180903

wchar_t key[] = L"dfkhfroo34957^*(Ndjf;;//<{D";


//CLogger::CLogger(void)
CLogger::CLogger( wchar_t* name /*=LOG_FILE_NAME*/ )
{
  CString txt;

  ASSERT( name && AfxIsValidString( name ) );

  file_name = name;

  Check_filesize();


  if ( !wcscmp( file_name, LOG_FILE_NAME ) || !wcscmp( file_name, NEWLOG_FILE_NAME ) ) // only standard logger adds this
  {
	Add_string( L"LOG REPORT" );//Pour added 20190328
    Add_string( L"----------------------------------" );
    txt.Format( L"Starting up program on %s", PC_name );
    Add_string( &txt );
  }
}


CLogger::~CLogger(void)
{
  //Add_string( "Exiting program" );
  if ( !wcscmp( file_name, LOG_FILE_NAME ) || !wcscmp( file_name, NEWLOG_FILE_NAME )  ) // only standard logger adds this
  {
    Add_string( L"Exiting program - Log end" );
    Add_string( L"----------------------------------" );
  }
}


//Pour added 20180903
int CLogger::Save_txt_archive( )
{
//  if ( !PathFileExists( LOG_FILE_NAME ) ) return 0; // file doesn't exist, nothing to do
  if ( !PathFileExists( file_name ) ) return 0; // file doesn't exist, nothing to do

  //UINT dim;
  CFile file;
  CFileException ex;

  
  //CTime time = CTime::GetCurrentTime();   
  CTime time = batch_mngr->current.end_time; //save_time 
  CString newname;
//  newname.Format( L"logcopy_%04d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay() );

  #ifdef NEW_LOGGER      // New log 
    newname.Format( L"logcopy_%04d%02d%02d_%02d%02d%02d.lgf", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	//newname.Format( L"logcopy_%04d%02d%02d_%02d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
  #else
    newname.Format( L"logcopy_%04d%02d%02d_%02d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
  #endif

  try
  {
//    CFile::Rename( LOG_FILE_NAME, newname );
    CFile::Rename( file_name, newname );
  }
  catch( CFileException* pEx )
  {
//  TRACE(_T("File %20s not found, cause = %d\n"), LOG_FILE_NAME, pEx->m_cause );
    TRACE(_T("File %20s not found, cause = %d\n"), file_name, pEx->m_cause );
    pEx->Delete();
  }
  SetFileAttributes( newname, FILE_ATTRIBUTE_READONLY);
  return 0;
}
//Pour added 20180903

//Pour added 20190220
int CLogger::Save_alarm_txt_archive( )
{
  if ( !PathFileExists( ALARMLOG_FILE_NAME ) ) return 0; // file doesn't exist, nothing to do
//  if ( !PathFileExists( file_name ) ) return 0; // file doesn't exist, nothing to do

  //UINT dim;
  CFile file;
  CFileException ex;
  CTime time = batch_mngr->current.end_time; //save_time 
  CString newname;
  newname.Format( L"alarm_log_copy_%04d%02d%02d_%02d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

  try
  {
    CFile::Rename( ALARMLOG_FILE_NAME, newname );
//    CFile::Rename( file_name, newname );
  }
  catch( CFileException* pEx )
  {
    TRACE(_T("File %20s not found, cause = %d\n"), ALARMLOG_FILE_NAME, pEx->m_cause );
//    TRACE(_T("File %20s not found, cause = %d\n"), file_name, pEx->m_cause );
    pEx->Delete();
  }
  SetFileAttributes( newname, FILE_ATTRIBUTE_READONLY);
  return 0;
}
//Pour added 20190220


int CLogger::Check_filesize( void )
{
//  if ( !PathFileExists( LOG_FILE_NAME ) ) return 0; // file doesn't exist, nothing to do
  if ( !PathFileExists( file_name ) ) return 0; // file doesn't exist, nothing to do

  UINT dim;
  CFile file;
  CFileException ex;

//  if ( !file.Open( LOG_FILE_NAME, CFile::modeRead , &ex ) )
  if ( !file.Open( file_name, CFile::modeRead , &ex ) )
	{
    ex.ReportError();
    return 99;
	}

  dim = UINT(file.GetLength());
  file.Close();

  if ( dim<NEWLOG_MAXFILESZ ) return 0;   // if log file smaller than 200kB nothing to do return
//  if ( dim<1*1024 ) return 0;   // if log file smaller than 200kB nothing to do return

  CTime time = CTime::GetCurrentTime();   

  CString newname;
  //newname.Format( L"logcopy_%04d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay() );

  //Pour removed for testing 20190330
  
  #ifdef NEW_LOGGER      // New log 
    newname.Format( L"logcopy_%04d%02d%02d.lgf", time.GetYear(), time.GetMonth(), time.GetDay() );
  #else
    newname.Format( L"logcopy_%04d%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay() );
  #endif
  
//Pour removed for testing 20190330

  try
  {
//    CFile::Rename( LOG_FILE_NAME, newname );
    CFile::Rename( file_name, newname );
  }
  catch( CFileException* pEx )
  {
//    TRACE(_T("File %20s not found, cause = %d\n"), LOG_FILE_NAME, pEx->m_cause );
    TRACE(_T("File %20s not found, cause = %d\n"), file_name, pEx->m_cause );
    pEx->Delete();
  }

  return 0;
}


#ifndef NEW_LOGGER // log alarms separately and add button to show them


int CLogger::Add_string( CString* string_in )
{
  return Add_string( string_in->GetBuffer() );
}

int CLogger::Add_string( wchar_t* string_in )
{
  CFile fp; 
  CFileException ex;
  wchar_t buff[LOG_BUFF_SIZE+1];

  ASSERT( AfxIsValidString( string_in ) );

//  if ( !fp.Open( LOG_FILE_NAME , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::osWriteThrough, &ex ) )
  if ( !fp.Open( file_name , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::osWriteThrough, &ex ) )
	{
    ex.ReportError();
    return 99;
	}

  CTime time = CTime::GetCurrentTime();   


  swprintf_s( buff, LOG_BUFF_SIZE, L"%02d/%02d/%04d %02d:%02d:%02d - %s\r\n", time.GetDay(), time.GetMonth(), time.GetYear(), time.GetHour(), time.GetMinute(), time.GetSecond(), string_in );

  fp.SeekToEnd();
  fp.Write( buff, wcslen(buff)*sizeof(wchar_t) );

  fp.Close();

  return 0;
}

#endif // NEW_LOGGER // log alarms separately and add button to show them


// --------------------------------------------------------------------------

// NEW VERSIONS WITH ENCRYPTION

#ifdef NEW_LOGGER // log alarms separately and add button to show them

int CLogger::Add_string( CString* string_in )
{
  return Add_string( string_in->GetBuffer() );
}

int CLogger::Add_string( wchar_t* string_in )
{
  CFile fp; 
  CFileException ex;
  wchar_t buff[LOG_BUFF_SIZE+1];

  ASSERT( AfxIsValidString( string_in ) );

//  if ( !fp.Open( file_name , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::osWriteThrough, &ex ) )
//  if ( !fp.Open( file_name , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::osWriteThrough, &ex ) )
  if ( !fp.Open( file_name , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite, &ex ) )
	{
    ex.ReportError();
    return 99;
	}

  UINT flen = UINT(fp.GetLength());
  UINT klen = wcslen(key);
  UINT crc  = 0x7F3A4322; // seed
  UINT pos  = 2;

  if ( flen>4 ) // file exists, read first 4 bytes as crc
  {
    fp.Read(&crc,4);
    ASSERT( fp.GetPosition()==4 );
    pos  = flen/sizeof(wchar_t);
  }
  else ASSERT( fp.GetPosition()==0 );


  // prepare string to be written
  CTime time = CTime::GetCurrentTime();   
  swprintf_s( buff, LOG_BUFF_SIZE, L"%02d/%02d/%04d %02d:%02d:%02d - %s\r\n", time.GetDay(), time.GetMonth(), time.GetYear(), time.GetHour(), time.GetMinute(), time.GetSecond(), string_in );
  UINT blen = wcslen(buff);

  //TRACE( "Initial crc=%08X (pos=%d)\n",crc,fp.GetPosition() );


  // encrypt and update crc
  for ( UINT i=0 ; i<blen ; i++ )
  {
    UINT p = i + pos;
    crc += buff[i]^p;
    //ASSERT( p%klen>=0 && p%klen<klen );
    buff[i] = buff[i]^key[p%klen];
  }

/*  for ( UINT i=0 ; i<blen ; i++ )
  {
    crc += buff[i]*i;
    ASSERT( i%klen>=0 && i%klen<klen );
    buff[i] = buff[i]+key[i%klen];
  } */

  // write new crc at the beginning
  fp.SeekToBegin();
  fp.Write( &crc, 4 );
  
  //TRACE( "Written crc=%08X (pos=%d)\n",crc,fp.GetPosition() );

  // append encrypted string at the end
  fp.SeekToEnd();
  fp.Write( buff, blen*sizeof(wchar_t) );
  
  //TRACE( "Written buff (pos=%d)\n",fp.GetPosition() );

  fp.Close();

  return 0;
}

#endif // NEW_LOGGER 

// --------------------------------------------------------------------------------

int CLogger::Trim_numlines(void)
{
  CFile file;
  CFileException ex;
  
  char *buffer = NULL;
  UINT  dim = 0;

//  if ( !PathFileExists( LOG_FILE_NAME ) )
  if ( !PathFileExists( file_name ) )
  {
    //TRACE("Errorlog file <%s> doesn't exist - Skipping Trim_numlines\n", LOG_FILE_NAME );
    TRACE( L"Errorlog file <%s> doesn't exist - Skipping Trim_numlines()\n", file_name );
    return 9;
  }


//  if ( !file.Open( LOG_FILE_NAME, CFile::modeRead , &ex ) )
  if ( !file.Open( file_name, CFile::modeRead , &ex ) )
	{
		//ex.GetErrorMessage(err,1024);
    //msg_err.Format("Unable to open file <%s>:\r\n<%s>", name, ex.GetErrorMessage(err,1024) );
    ex.ReportError();
    return 99;
	}

  dim = UINT(file.GetLength());
  buffer = (char*) malloc( dim );

  if ( !buffer )
  {
    file.Close();
    return 999; // failed to allocate required memory
  }

  file.Read( buffer, dim );
  file.Close();

  int pos   = dim-1;
  int nrows = 1;

  while ( pos>=2 ) // ciclo esterno di conteggio righe
  {
    //TRACE("%4d - %3d - %c\n", pos, buffer[pos], buffer[pos]>32?buffer[pos]:'#' );
    if ( buffer[pos-1]==10 && buffer[pos-2]==13 )
    {
      nrows++;
      //TRACE("NUOVA RIGA\n");
      if ( nrows>LOG_NUM_LINES ) // cut here
      {
        //TRACE("CUT HERE\n");

//        if ( !file.Open( "D:\\IVS\\new.txt", CFile::modeCreate|CFile::modeWrite, &ex ) )
//        if ( !file.Open( LOG_FILE_NAME, CFile::modeCreate|CFile::modeWrite, &ex ) )
        if ( !file.Open( file_name, CFile::modeCreate|CFile::modeWrite, &ex ) )
        {
          ex.ReportError();
          free( buffer );
          return 199;
        }

        file.Write( buffer+pos, dim-pos );
        file.Close();
        //TRACE("New file created\n");
        TRACE( L"New file created\n" );
        break;
      }
    }
    pos--;
  }

  //TRACE("Tot %d rows\n", nrows);

  free( buffer );

  return 0;
}
