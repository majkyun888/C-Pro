/*----------------------------- CFile_crypt ------------------------------

            Gestione dei file cryptati
 -- Add_data(void* ptr, UINT dim)
    Put ptr element (dimension: dim_data) to existing structure and modify final dimension
 -- Get_data
    Take ptr element (dimension: dim_data) from existing structure and modify final dimension
 
 
 -- CFile_crypt::Read
    take in input a crypted file formatted, decrypt it and check CRC 
        LUNG(4) - DATA(dim_data) - CRC(4) - PAD(0..7)
 -- CFile_crypt::Write
    Format a data block and crypt it

 -- Encrypt e Decrypt
    Encryption and Decryprion with TEA algorithm
 -- GetCrc ( ++ Init_CRC32_Table )
    32 bit CRC calculation from string

 -- AppendString
    Add a nerw string to file_log and calculate new crc

 -- ReadLine
    Read a line of log_file and add it to RichEdit

 -- RollingFile
    If file dimension is bigger than limit, it deletes a % of file from beginning and recopy
    the original file from there to the end

*/

#pragma once
#include "afx.h"

#ifdef CFR21
  #include "CFR21.h"
#endif

#define PG         0x11021	// Polinomio generatore
#define DIM_BUFFER 100000  	// Initial size

class CFile_crypt :  public CFile
{
public:
  CFile_crypt(void);
  ~CFile_crypt(void);

public:

  void Encrypt ( void* buffer, int dim);
  void Decrypt ( void* buffer, int dim);

  BOOL Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );

  int Add_data( void* ptr, UINT dim);
  int Write   ( void );

  int Get_data( void* ptr, UINT dim );
  int Read    ( void );

  int AppendString( char* string );
  int ReadString  ( char * string);

  int ReadLine( CRichEditCtrl* RichEdit );
  void RollingFile( char* nome_file );
  int FindRightPosToDelete ( char* nome_file, int& crc_loc );

  int GetCrc( BYTE* text, int len);

  CString msg_error;

  BYTE*  data;
  
  UINT   dim_data_file;
  UINT   dim_tot_data;

protected:
  ULONG crc32_table[256];  // Lookup table array 
  void Init_CRC32_Table();  // Builds lookup table array 
  ULONG Reflect(ULONG ref, char ch) ;
};