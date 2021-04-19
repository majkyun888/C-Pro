#include "StdAfx.h"
#include "Output_data.h"
#include "Globals.h"
#include "SG_file_management.h"


extern SG_type field_header[NUM_FIELDS_HEADER];   // codes/fields sent
extern SG_type field_row   [NUM_FIELDS_ROW   ];   // codes/fields sent
extern int cavities[36];

extern BOOL execute_planeite_db;  // measure tilt of top part of bottle
extern BOOL datarec_on;
 
//extern char output_folder[MAX_LENGHT_PATH];
//extern char input_folder [MAX_LENGHT_PATH];



COutput_data::COutput_data( void ) //( int control_number , int bottle_number )
{

  // ASSERT( control_number>0 && control_number<=20 );
  // ASSERT( bottle_number >0 && bottle_number <=16 );
  // control_num = control_number;
  // bottle_num  = bottle_number;

//  control_num = 0;
  vision_nc   = 0;
  gage_nc     = 0;
  bottle_num  = 0;
//  result      = NULL;

  ZeroMemory( vision_data, sizeof(vision_data) );
  TRACE( "<vision_data> ripulito (%d bytes)\n", sizeof(vision_data) );
  ZeroMemory( gage_data,   sizeof(gage_data)   );
  TRACE( "<gage_data> ripulito (%d bytes)\n", sizeof(gage_data) );

  // UINT dim = sizeof(res_type) * control_num * bottle_num;
  //result = (res_type**) malloc( sizeof(res_type*) * bottle_num );
  //for ( UINT nb=0 ; nb<bottle_num ; nb++ ) result[nb] = (res_type*) malloc( sizeof(res_type) * control_num );

//  ZeroMemory( result, dim );

/*  for ( UINT nb=0 ; nb<bottle_num ; nb++ )
  {
    for ( UINT nc=0 ; nc<control_num ; nc++ )
    {
      result[nb][nc].min = nb*10.0 + nc + 0.1;
      result[nb][nc].avg = nb*10.0 + nc + 0.2;
      result[nb][nc].max = nb*10.0 + nc + 0.3;
    }
  } */


}


COutput_data::~COutput_data( void )
{
//  for ( UINT nb=0 ; nb<bottle_num ; nb++ ) if ( result[nb] ) free( result[nb] );
//  if ( result ) free( result );
//  result = NULL;
//  Deallocate();
}

/*
int COutput_data::Allocate(void)
{
  ASSERT( control_num>0 && control_num<=20 );
  ASSERT( bottle_num >0 && bottle_num <=16 );

  Deallocate();

  result = (res_type**) malloc( sizeof(res_type*) * bottle_num );
  for ( UINT nb=0 ; nb<bottle_num ; nb++ )
  {
    result[nb] = (res_type*) malloc( sizeof(res_type) * control_num );
    ZeroMemory( result[nb], sizeof(res_type) * control_num );
  }


  TRACE("COutput_data::Allocate %d bottles - %d controls/bottle\n", bottle_num, control_num );

  return 0;
}


int COutput_data::Deallocate(void)
{
  if ( result )
  {
    for ( UINT nb=0 ; nb<bottle_num ; nb++ )
    {
      if ( result[nb] )
      {
        free( result[nb] );
        result[nb] = NULL;
      }
    }
    free( result );
    result = NULL;
  }
  return 0;
}
*/


int COutput_data::Add_control( char* code, int type, Results_manager* results )
{
  ASSERT( results );
  ASSERT( code );

  if ( type & VISION_TYPE )
  {
    vision_data[vision_nc].results = results;
    strncpy( vision_data[vision_nc].code, code, 4 );
    vision_data[vision_nc].type = type;
    vision_nc++;
  }

  if ( type & GAGE_TYPE )
  {
    gage_data[gage_nc].results = results;
    strncpy( gage_data[gage_nc].code, code, 4 );
    gage_data[gage_nc].type = type;
    gage_nc++;
  }
  
  
  
  //control_num++;

  return 0;
}


#define CMP4CHAR(txt1,txt2) *((UINT*)txt1)==*((UINT*)txt2)

int COutput_data::Vision_process_data( int bottle_id, int bottle_cav )
{

  double max_height = -999.0;
  double min_height = -888.0;

  if ( bottle_id<0 ) bottle_id=0;


  for ( UINT nc=0 ; nc<vision_nc ; nc++ )
  {
    UINT col = vision_data[nc].type & 15;           // parte bassa per selezionare la colonna da cui leggere i dati
    UINT ndt = vision_data[nc].results->ndata[col]; // numero di dati da processare

    if ( ndt > 0 || ( vision_data[nc].type & PLANEITE_TYPE ) )
    {
      double min = 1.0e6;
      double max = 0.0;
      double avg = 0.0;

      for ( UINT i=0 ; i<ndt ; i++ )
      {
        double val = vision_data[nc].results->Get( i, col );
        if ( val < min ) min = val;
        if ( val > max ) max = val;
        avg += val;
      }

      // versione "standard" valida per tutti i diametri collo/corpo
      vision_data[nc].min[bottle_id] = min;
      vision_data[nc].max[bottle_id] = max;
      vision_data[nc].avg[bottle_id] = avg/ndt;


/*      if ( ( vision_data[nc].type & VERTICALITY_TYPE ) && !(vision_data[nc].type & PLANEITE_TYPE ) )
      {
        vision_data[nc].min[bottle_id] = min;
        vision_data[nc].max[bottle_id] = max;
        vision_data[nc].avg[bottle_id] = avg/ndt;
      }   */

      // Larghezza
      if ( vision_data[nc].type & WIDTH_TYPE ) 
      {
        vision_data[nc].min[bottle_id] = vision_data[nc].results->Get( 0, 0 );
        vision_data[nc].max[bottle_id] = vision_data[nc].results->Get( 0, 0 );
        vision_data[nc].avg[bottle_id] = vision_data[nc].results->Get( 0, 0 );
      }

      // Spessore
      if ( vision_data[nc].type & THICKNESS_TYPE ) 
      {
        vision_data[nc].min[bottle_id] = vision_data[nc].results->Get( 0, 1 );
        vision_data[nc].max[bottle_id] = vision_data[nc].results->Get( 0, 1 );
        vision_data[nc].avg[bottle_id] = vision_data[nc].results->Get( 0, 1 );
      }


      // Verticalite
      if ( vision_data[nc].type & VERTICALITY_TYPE ) 
      {
        avg = 0.5 * ( max - min );
        vision_data[nc].min[bottle_id] = avg;
        vision_data[nc].max[bottle_id] = avg;
        vision_data[nc].avg[bottle_id] = avg;
      }

      if ( vision_data[nc].type & HEIGHT_TYPE ) // altezza - max assoluto - min assoluto - media dei due
      {
        for ( UINT i=0 ; i<vision_data[nc].results->ndata[col+1] ; i++ )
        {
          double val = vision_data[nc].results->Get( i, col+1 );
          if ( val < min ) min = val;
          if ( val > max ) max = val;
          avg += val;
        }
        avg = 0.5 * ( max + min );
        vision_data[nc].min[bottle_id] = min;
        vision_data[nc].max[bottle_id] = max;
        vision_data[nc].avg[bottle_id] = avg;
        max_height = max;
        min_height = min;
        TRACE("Fine process altezza - max_h=%.3lf - min_h=%.3lf\n", max_height, min_height );
      }


      // se c'è anche la planeite la cerco e inserisco i valori
//      if ( execute_planeite_db )
      if ( vision_data[nc].type & PLANEITE_TYPE ) // planeitè = max height assoluta - min assoluta
      {

        vision_data[nc].min[bottle_id] = max_height-min_height;
        vision_data[nc].max[bottle_id] = max_height-min_height;
        vision_data[nc].avg[bottle_id] = max_height-min_height;

        TRACE("Fine process planeite - max_h=%.3lf - min_h=%.3lf\n", max_height, min_height );

        /*for ( UINT nc2=0 ; nc2<vision_nc ; nc2++ )
        {
          if ( CMP4CHAR(vision_data[nc2].code, "000M") )
          {
            vision_data[nc2].min[bottle_id] = max-min;
            vision_data[nc2].max[bottle_id] = max-min;
            vision_data[nc2].avg[bottle_id] = max-min;
          }
        } */
      }


    }
    else // not enough data - set all to zero
    {
      vision_data[nc].min[bottle_id] = -9.9;
      vision_data[nc].max[bottle_id] = -9.9;
      vision_data[nc].avg[bottle_id] = -9.9;
    }


    if ( !datarec_on ) vision_data[nc].results->Clean();
    
  }

  return 0;
}


int COutput_data::Gage_process_data( int bottle_id, int bottle_cav )
{

  for ( UINT nc=0 ; nc<gage_nc ; nc++ )
  {

    double min = 1.0e6;
    double max = 0.0;
    double avg = 0.0;

    // dati nella prima colonna
    for ( UINT i=0 ; i<gage_data[nc].results->ndata[0] ; i++ )
    {
      double val = gage_data[nc].results->Get( i, 0 );
      if ( val < min ) min = val;
      if ( val > max ) max = val;
      avg += val;
    }

    gage_data[nc].min[bottle_id] = min;
    gage_data[nc].max[bottle_id] = max;
    gage_data[nc].avg[bottle_id] = avg / gage_data[nc].results->ndata[0];


    if ( !datarec_on ) gage_data[nc].results->Clean();

  }

  return 0;
}





int COutput_data::File_write(void)
{

  #define ROWLEN  32
  #define FILLER  32 //tilde=126 - space=32

  char  msg[200];
  char  file_name[250];
  char  row[ROWLEN];
  CFile file;
  CFileException ex;


  CTime time = CTime::GetCurrentTime();   

  // generazione nome file
  int year   = time.GetYear();
  int month  = time.GetMonth();
  int day    = time.GetDay();
  int hour   = time.GetHour();
  int minute = time.GetMinute();
  char ychar = '0' + year%10;
  char mchar =  96 + month;

//  sprintf( file_name, "%c%c%02d%02d%02d.txt", ychar, mchar, day, hour, minute );
  sprintf( file_name, "%s\\%c%c%02d%02d%02d.txt", MAIN_DIRECTORY, ychar, mchar, day, hour, minute );

  // Create/open file 
//  if ( !file.Open( "results.txt", CFile::modeCreate|CFile::modeWrite, &ex ) )
  if ( !file.Open( file_name, CFile::modeCreate|CFile::modeWrite, &ex ) )
	{
    char cause[1024];
    ex.GetErrorMessage(cause,1024);
    sprintf(msg, "Unable to create results output file <%s>\r\nReason: <%s>", file_name, cause );
		Message_box(msg, MBOX_ERROR);
    return 99;
	}  



//  sprintf( buff, "D:\\Data\\%s_%04d%02d%02d_%02d%02d%02d.csv", name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
//  file.Open( buff, CFile::modeCreate|CFile::modeWrite );


  // Write header first
  FillMemory(row,ROWLEN-1,FILLER);
  row[0] = 'A';
  strncpy( row+1, SEM_LIG,  2 );
  strncpy( row+3, SEM_ART, 10 );
  sprintf( row+13, "%s", time.Format("%y%m%d%H%M") );
  strncpy( row+23, SEM_USR, 3 );
  row[ROWLEN-1] = 0x0A;
  file.Write( row, ROWLEN );


  
  // Loop on VISION controls
  for ( UINT nc=0 ; nc<vision_nc ; nc++ ) //  for ( UINT nc=0 ; nc<control_num ; nc++ )
  {

    // First row for each control
    FillMemory(row,ROWLEN-1,FILLER);
    row[0] = 'B';
    strncpy(row+1, vision_data[nc].code, 4 );
//    row[strlen(row)]=FILLER;
    row[ROWLEN-1] = 0x0A;
    file.Write( row, ROWLEN );
    
    for ( UINT nb=0 ; nb<bottle_num ; nb++ )
    {
      FillMemory(row,ROWLEN-1,FILLER);
      row[0] = 'C';
      sprintf( row+ 1, "%02d",   cavities[nb] );
      sprintf( row+ 3, "%8.2lf", vision_data[nc].min[nb] );
      sprintf( row+11, "%8.2lf", vision_data[nc].max[nb] );
      sprintf( row+19, "%8.2lf", vision_data[nc].avg[nb] );
      row[strlen(row)]=FILLER;
      row[ROWLEN-1] = 0x0A;
      file.Write( row, ROWLEN );
    } 
  }


  // Loop on GAGE controls
  for ( UINT nc=0 ; nc<gage_nc ; nc++ ) //  for ( UINT nc=0 ; nc<control_num ; nc++ )
  {

    // First row for each control
    FillMemory(row,ROWLEN-1,FILLER);
    row[0] = 'B';
    strncpy(row+1, gage_data[nc].code, 4 );
//    row[strlen(row)]=FILLER;
    row[ROWLEN-1] = 0x0A;
    file.Write( row, ROWLEN );
    
    for ( UINT nb=0 ; nb<bottle_num ; nb++ )
    {
      FillMemory(row,ROWLEN-1,FILLER);
      row[0] = 'C';
      sprintf( row+ 1, "%02d",   cavities[nb] );
      sprintf( row+ 3, "%8.2lf", gage_data[nc].min[nb] );
      sprintf( row+11, "%8.2lf", gage_data[nc].max[nb] );
      sprintf( row+19, "%8.2lf", gage_data[nc].avg[nb] );
      row[strlen(row)]=FILLER;
      row[ROWLEN-1] = 0x0A;
      file.Write( row, ROWLEN );
    } 
  }

  file.Close();
  return 0;
}
