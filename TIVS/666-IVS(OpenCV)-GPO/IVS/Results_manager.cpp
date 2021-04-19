#include "StdAfx.h"
#include "Results_manager.h"
#include "Def_globali.h"
#include "Globals.h"



// struttura di data
//                         categories=3
//                        --------------------
//                    |  val0(0) val0(1) val0(2)
//                    |  val1(0) val1(1) val1(2)
//  max_num_data=5    |  val2(0) val2(1) val2(2)
//                    |  val3(0) val3(1) val3(2)
//                    |  val4(0) val4(1) val4(2)


//extern char output_folder[MAX_LENGHT_PATH];
//extern char input_folder [MAX_LENGHT_PATH];



//Results_manager::Results_manager( wchar_t* desc, UINT max_num_data_in, UINT categories_in )

// If format_in==NULL (default) INT type selected for data, otherwise DOUBLE is assigned
Results_manager::Results_manager( wchar_t* desc, UINT max_num_data_in, UINT categories_in, wchar_t* format_in/*=NULL*/ )
{

  ASSERT( max_num_data_in>0 && max_num_data_in<=50000 );
  ASSERT( categories_in>0 && categories_in<70 );

//  if ( desc && AfxIsValidString(desc) ) description = desc;
//  else description = "No description provided";

//  if ( desc && AfxIsValidString(desc) ) strncpy( description, desc, 50 );
//  else strcpy( description, "No description provided" );

  ZeroMemory( description, sizeof(description) );
  if ( desc && AfxIsValidString(desc) ) wcsncpy( description, desc, 100 );
  else wcscpy( description, L"No description provided" );
//  description[99] = L'\0';

  if ( format_in && AfxIsValidString(format_in) ) // format provided: DOUBLE data type
  {
    intdata = FALSE;
    wcsncpy ( format, format_in, wcslen(format_in)+1 );
    swprintf( format_cr, 19, L"%s\r\n", format );
  }
  else   // format NOT provided: INT data type
  {
    intdata = TRUE;
    wcscpy  ( format, L"%d," );
    swprintf( format_cr, 19, L"%s\r\n", format );
  }


  max_num_data = max_num_data_in;
  categories   = categories_in;

  if ( intdata ) // INT data
  {
    dim = max_num_data * categories * sizeof(int);
    datai = (int*) malloc( dim );
    datad = NULL;
    ASSERT( datai && AfxIsValidAddress(datai,dim));
  }
  else // DOUBLE data
  {
    dim = max_num_data * categories * sizeof(double);
    datad = (double*) malloc( dim );
    datai = NULL;
    ASSERT( datad && AfxIsValidAddress(datad,dim));
  }

  //dim          = max_num_data * categories * sizeof(double);
  ndata = (UINT*) malloc( categories*sizeof(UINT) );

  Clean();
//  TRACE("Oggetto Results_manager <%s> creato - max_num_data=%d - categories=%d\n", description, max_num_data, categories );
  
}



Results_manager::~Results_manager(void)
{
  // clean everything
  if ( datad ) free( datad );
  datad = NULL;
  if ( datai ) free( datai );
  datai = NULL;

  if ( ndata ) free( ndata );
  ndata = NULL;

  dim             = 0;
  max_num_data    = 0;
  categories = 0;
}


// Clean and reset data and memory
int Results_manager::Clean(void)
{
  if ( datad ) ZeroMemory( datad, dim );
  if ( datai ) ZeroMemory( datai, dim );
  ZeroMemory( ndata, categories*sizeof(UINT) );
  return 0;
}


// DOUBLE version
int Results_manager::InsertD( double val, UINT cat)
{
  ASSERT( cat>=0 && cat<categories );

  if ( ndata[cat]>=max_num_data ) return( 99 ); // array already filled completely

//  ASSERT( cat+ndata[cat]*categories>=0 && cat+ndata[cat]*categories<dim/8 );
//  data[ cat + ndata[cat]*categories ] = val;

  ASSERT( ndata[cat]+cat*max_num_data>=0 && ndata[cat]+cat*max_num_data<dim/sizeof(double) );
  datad[ ndata[cat]+cat*max_num_data ] = val;
  ndata[cat]++;

  return 0;
}

// INT version
int Results_manager::InsertI( int val, UINT cat)
{
  ASSERT( cat>=0 && cat<categories );

  if ( ndata[cat]>=max_num_data ) return( 99 ); // array already filled completely

//  ASSERT( cat+ndata[cat]*categories>=0 && cat+ndata[cat]*categories<dim/8 );
//  data[ cat + ndata[cat]*categories ] = val;

  ASSERT( ndata[cat]+cat*max_num_data>=0 && ndata[cat]+cat*max_num_data<dim/sizeof(int) );
  datai[ ndata[cat]+cat*max_num_data ] = val;
  ndata[cat]++;

  return 0;
}


// function to set val at ind/cat (DOUBLE version)
int Results_manager::Set( double val, UINT ind, UINT cat )
{
  ASSERT( cat>=0 && cat<categories   );
  ASSERT( ind>=0 && ind<max_num_data );
  ASSERT( ind+cat*max_num_data>=0 && ind+cat*max_num_data<dim/sizeof(double) );
  
  datad[ind+cat*max_num_data]=val;
  return 0;
}


// function to set val at ind/cat (DOUBLE version)
int Results_manager::Set( int val, UINT ind, UINT cat )
{
  ASSERT( cat>=0 && cat<categories   );
  ASSERT( ind>=0 && ind<max_num_data );
  ASSERT( ind+cat*max_num_data>=0 && ind+cat*max_num_data<dim/sizeof(int) );
  
  datai[ind+cat*max_num_data]=val;
  return 0;
}


// function to return stored value at ind/cat (DOUBLE version)
double Results_manager::GetD( UINT ind, UINT cat )
{
  ASSERT( cat>=0 && cat<categories   );
  ASSERT( ind>=0 && ind<max_num_data );

  ASSERT( ind+cat*max_num_data>=0 && ind+cat*max_num_data<dim/8 );
  return datad[ind+cat*max_num_data];
}

// function to return stored value at ind/cat (INT version)
int Results_manager::GetI( UINT ind, UINT cat )
{
  ASSERT( cat>=0 && cat<categories   );
  ASSERT( ind>=0 && ind<max_num_data );

  ASSERT( ind+cat*max_num_data>=0 && ind+cat*max_num_data<dim/8 );
  return datai[ind+cat*max_num_data];
}




//int Results_manager::Create_csv(char* name, BOOL header )
int Results_manager::Create_csv( wchar_t* name, BOOL header /*=FALSE*/ )
{
  CTime time = CTime::GetCurrentTime();   

  ASSERT( file.m_hFile == INVALID_HANDLE_VALUE );

//  sprintf( buff, "D:\\Data\\%s_%04d%02d%02d_%02d%02d%02d.csv", name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
//  sprintf( buff, "%s\\%s_%04d%02d%02d_%02d%02d%02d.csv", MAIN_DIRECTORY, name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  swprintf( buff, 900, L"%s\\%s_%04d%02d%02d_%02d%02d%02d.csv", MAIN_DIRECTORY, name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  file.Open( buff, CFile::modeCreate|CFile::modeWrite );

  if ( header )
  {
    CString dt;
    dt = time.Format("%A %d %B %Y at %H:%M:%S");
    swprintf( buff, 900, L"File created on %s\r\n", dt );    //time.Format("%A %d %B %Y at %H:%M:%S") ); 
    file.Write( buff, wcslen(buff)*sizeof(wchar_t) );
    swprintf( buff, 900, L"Measurement data from <%s>\r\n", description ); 
    file.Write( buff, wcslen(buff)*sizeof(wchar_t) );
  }

  return( 0 );
}


// Save all data and close file 
//int Results_manager::Addrow_csv(char* text)
int Results_manager::Addrow_csv( wchar_t* text)
{
  ASSERT( file.m_hFile ); // file must be open
  if ( !file.m_hFile ) return( 99 ); // create file first
  //file.Write( text, strlen(text) );
  file.Write( text, wcslen(text)*sizeof(wchar_t) );
  return( 0 );
}


// Save all data and close file 
int Results_manager::Save_csv( BOOL clean_after )
{
  ASSERT( file.m_hFile ); // file must be open
  if ( !file.m_hFile ) return( 99 ); // create file first

  UINT nrow = ndata[0]; // stored data if only single category

  if ( categories>1 ) // find maximum value for ndata[x]
    for ( UINT nc=1 ; nc<categories ; nc++ )
    {
      if ( ndata[nc]>0 && ndata[nc]>nrow ) nrow=ndata[nc];
      //TRACE("%d,%d,%d\n", nc,ndata[nc],nrow);
    }
  //TRACE("Save_csv: inizio for\n");

  for ( UINT nd=0 ; nd<nrow ; nd++ )  // scan data index / csv row
  {
    swprintf( buff, 900, L"%d,", nd+1 ); // data index
    file.Write( buff, wcslen(buff)*sizeof(wchar_t) );
    for ( UINT nc=0 ; nc<categories ; nc++ ) // scan category / column
    {
//      if ( nc<categories-1 ) swprintf( buff, L"%.3lf,",    data[nd+nc*max_num_data] );
//      else                   swprintf( buff, L"%.3lf\r\n", data[nd+nc*max_num_data] );

      if ( intdata ) // INT version
      {
        if ( nc<categories-1 ) swprintf( buff, 900, format,    datai[nd+nc*max_num_data] );
        else                   swprintf( buff, 900, format_cr, datai[nd+nc*max_num_data] );
      }
      else  // DOUBLE version
      {
        if ( nc<categories-1 ) swprintf( buff, 900, format,    datad[nd+nc*max_num_data] );
        else                   swprintf( buff, 900, format_cr, datad[nd+nc*max_num_data] );
      }
      file.Write( buff, wcslen(buff)*sizeof(wchar_t) );
    }
  }

  //TRACE("Save_csv: inizio close\n");

  file.Close();

  //TRACE("Save_csv: inizio clean\n");

  if ( clean_after ) Clean(); // after saving, clean everything for next bottle

  return 0;
}

// Load data from csv and stores them in the structure
//int Results_manager::Load_csv(char* name, UINT nc )
int Results_manager::Load_csv( wchar_t* name, UINT nc )
{
  CFile file;
  CString msg_err;
  CFileException ex;
  //char err[1024];
  //char *buffer = NULL;
  wchar_t err[1024];
  wchar_t *buffer = NULL;
  int  dim = 0;

  ASSERT( categories >= nc );

  if ( !file.Open( name, CFile::modeRead , &ex ) )
	{
		ex.GetErrorMessage(err,1024);
    //msg_err.Format("Unable to open file <%s>:\r\n<%s>", name, ex.GetErrorMessage(err,1024) );
    msg_err.Format(L"Unable to open file <%s>:\r\n<%s>", name, ex.GetErrorMessage(err,1024) );
    return 99;
	}

  dim = UINT( file.GetLength() );
  ASSERT( dim>0 && dim<100*1024 ); // 100kB max!
  //buffer = (char*) malloc( dim );
  buffer = (wchar_t*) malloc( dim );

  if ( file.Read( buffer, dim ) != dim ) // Unable to read all data into buffer
  {
    //msg_err.Format("Unable to read data from file <%s> (%d bytes)", name, dim );
    msg_err.Format(L"Unable to read data from file <%s> (%d bytes)", name, dim );
		Message_box( msg_err, MBOX_ERROR );
  }
//  TRACE("File <%s> letto interamente - %d bytes", name, dim );
  TRACE( L"File <%s> read entirely - %d bytes", name, dim );
  file.Close();

  Clean();

  int j    =  0;
  int i    =  0;
  int sta  =  0;
  int end  =  0;
  int nr   = -1;

  while ( i<dim )
  {
    while ( buffer[i-1]!='\r' && buffer[i]!='\n' && i<dim ) i++; // search end of this row
    end = i-2;
    
    // riga compresa tra sta ed end
/*    TRACE("Riga %d: <", nr );
    for ( j=sta ; j<=end ; j++ ) TRACE("%c",buffer[j]);
    TRACE(">\n"); */

    if ( nr <= -1 )
    {
      //if ( !strncmp( buffer+sta, "index,", 6 ) )  nr = 0;
      if ( !wcsncmp( buffer+sta, L"index,", 6 ) )  nr = 0;
    }
    else
    {
      int    index = 0;
      double val1 = 0.0;
      double val2 = 0.0;

      if ( nc==1 )
      {
        //if ( sscanf( buffer+sta, "%d,%lf", &index, &val1 ) == 2 ) Insert( val1 );
        //else TRACE("\nERRORE: Impossibile leggere dati dalla riga %d (index=%d - val=%.3lf)\n", nr, index, val1 );
        if ( swscanf( buffer+sta, L"%d,%lf", &index, &val1 ) == 2 ) InsertD( val1 );
        else TRACE( L"\nERROR: Unable to read data from row %d (index=%d - val=%.3lf)\n", nr, index, val1 );
//        else TRACE("\nERRORE: Impossibile leggere dati dalla riga %d (index=%d - val=%.3lf)\n", nr, index, val1 );
      }

      if ( nc==2 )
      {
        //if ( sscanf( buffer+sta, "%d,%lf,%lf", &index, &val1, &val2 ) == 3 )
        if ( swscanf( buffer+sta, L"%d,%lf,%lf", &index, &val1, &val2 ) == 3 )
        {
          InsertD( val1, 0 );
          InsertD( val2, 1 );
        }
        else TRACE("\nERROR: Unable to read data from row %d (index=%d - val1=%.3lf - val2=%.3lf)\n", nr, index, val1, val2 );
//        else TRACE("\nERRORE: Impossibile leggere dati dalla riga %d (index=%d - val1=%.3lf - val2=%.3lf)\n", nr, index, val1, val2 );
      }



//      TRACE("Riga %4d: <", nr ); // riga compresa tra sta ed end
//      for ( j=sta ; j<=end ; j++ ) TRACE("%c",buffer[j]);
//      TRACE(">\n");

      nr++; // indice riga
    }

    i++;
    sta = i;
  }

  free( buffer );

//  TRACE(" - %d righe di dati\n", nr );
  TRACE( L" - %d rows of data\n", nr );

  return 0;
}





// funzione per sorting (DOUBLE)
inline int compdd( const void *v1, const void *v2 )
{
  if ( *((double*)v1) >= *((double*)v2) ) return(  1 );
  else                                    return( -1 );
}


// funzione per sorting (INT)
inline int compdi( const void *v1, const void *v2 )
{
  if ( *((int*)v1) >= *((int*)v2) ) return(  1 );
  else                              return( -1 );
}





int Results_manager::Sort(int col)
{
//  double data[] = { 1.0, 1.1, 1.5, 2.0, 10.0, 15.0 };
//  double data[] = { 12.0, 1.1, 0.5, 2.0, 10.0, 15.0, -4.4, 0.00001, -8.4, -100.123 };
//  int ndata = 10;

  if ( col ) return 99; // wrong/invalid col value - only zero allowed

  if ( intdata )
  {
    qsort( datai, ndata[0], sizeof(int),    compdi /*compare*/ );
  }
  else
  {
    qsort( datad, ndata[0], sizeof(double), compdd /*compare*/ );
  }

//  qsort( data, ndata, sizeof(double), compare );

  /*
  TRACE("----------------------------\n");
  for ( UINT i=0 ; i<ndata[0] ; i++ )
  {
    TRACE( "data[%3d] = %8.3lf\n", i, data[i] );
  }
  TRACE("----------------------------\n");
  */


  return 0;
}

