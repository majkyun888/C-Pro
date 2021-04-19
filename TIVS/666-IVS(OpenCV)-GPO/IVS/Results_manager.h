#pragma once
class Results_manager
{


public:
  
//  Results_manager( char* desc, UINT max_num_data_in, UINT measures_number_in );
  Results_manager( wchar_t* desc, UINT max_num_data_in, UINT measures_number_in, wchar_t* format=NULL );
  ~Results_manager(void);

  int      Clean  ( void );

  int      InsertD( double val, UINT cat=0) ;
  int      InsertI( int    val, UINT cat=0) ;

  double   GetD   ( UINT ind, UINT cat=0 );
  int      GetI   ( UINT ind, UINT cat=0 );

  int      Set    ( double val, UINT ind, UINT cat=0 );
  int      Set    ( int    val, UINT ind, UINT cat=0 );

  int      Sort   ( int col);

  BOOL     intdata; // if TRUE all data are INT, DOUBLE otherwise

//  int      Create_csv(char* name, BOOL header=FALSE );
//  int      Addrow_csv(char* text);

  int      Create_csv( wchar_t* name, BOOL header=FALSE );
  int      Addrow_csv( wchar_t* text);

  int      Save_csv  (BOOL clean_after = TRUE );
//  int      Load_csv  ( char* name, UINT nc=1 );
  int      Load_csv  ( wchar_t* name, UINT nc=1 );

  wchar_t  format[20];
  wchar_t  format_cr[20];

  wchar_t  description[100];   // text pointer for description of result data
  UINT     max_num_data;    // max number of measurement to be recorded (size of arrays)
  UINT     categories;      // number of measuremes to be stored (categories)
  UINT*    ndata;      // array to store measurements for each cat
  UINT     dim;        // total array dimension in bytes
//  char     buff[1000];     // used for preparing text to write
  wchar_t  buff[1000];     // used for preparing text to write

  double*  datad;        // 2D array to store all data [data][category] (DOUBLE)
  int*     datai;        // 2D array to store all data [data][category] (INT)

  CFile    file;

};

