#include "StdAfx.h"

#include "File_crypt.h"


CFile_crypt::CFile_crypt(void)
{
  data = NULL;
  data = (BYTE*) malloc( DIM_BUFFER );
  dim_data_file = 0;
  TRACE("CFile_crypt: New object created\n");
}

CFile_crypt::~CFile_crypt(void)
{
  if ( data )
  {
    free( data );
    data = NULL;
    TRACE("CFile_crypt: Data buffer deallocated\n");
  }
}


int CFile_crypt::Add_data(void* ptr, UINT dim)
{
  if ( !AfxIsValidAddress( ptr, dim ) ) 
  {
    msg_error = "CFile_crypt::Add_data: Invalid pointer/buffer";
    TRACE( msg_error );
    return( 99 );
  }

  if ( dim_data_file+dim >= DIM_BUFFER )
  {
    msg_error = "CFile_crypt::Add_data: Buffer full!";
    TRACE( msg_error );
    return( 99 );
  }

  CopyMemory( data+dim_data_file, ptr, dim );
  dim_data_file += dim;

  return 0;
}

int CFile_crypt::Get_data( void* ptr, UINT dim )
{
  if ( !AfxIsValidAddress( ptr, dim ) ) 
  {
    msg_error = "CFile_crypt::Get_data: Invalid pointer/buffer";
    TRACE( msg_error );
    return( 99 );
  }

  if ( dim_data_file+dim > dim_tot_data )
  {
    msg_error = "CFile_crypt::Get_data: Buffer overrun!";
    TRACE( msg_error );
    return( 99 );
  }

  CopyMemory( ptr, data +dim_data_file, dim );
  dim_data_file += dim;

  return 0;
}



BOOL CFile_crypt::Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError )
{
  return( CFile::Open( lpszFileName, nOpenFlags, pError ) );

}

int CFile_crypt::Write( void )
{
  if ( m_hFile == CFile::hFileNull ) 
  {
    msg_error = "CFile_crypt::Write: File not open!";
    TRACE( msg_error );
    return (99);
  }

  int pad =  ( 8 - ( ( dim_data_file + 4 + 4 ) %8) ) %8; //quello che manca x formare il byte
  //int pad = ( ( ( dim_data_file + 4 + 4 )>>3 ) +1 ) <<3;
  UINT dim_buffer = dim_data_file + 4 + 4 + pad;
  BYTE* buffer = (BYTE*) malloc( dim_buffer );
  ZeroMemory( buffer, dim_buffer );

  *((UINT*) buffer) = dim_data_file;
  CopyMemory( buffer+4, data, dim_data_file );

  UINT crc;

  crc = GetCrc( buffer, dim_data_file+4 );
  *((UINT*) (buffer+4+dim_data_file)) = crc;
  TRACE("CFile_crypt::Write Computed CRC = %08X (%d bytes)\n", crc, dim_data_file+4 );

  Encrypt ( buffer, dim_buffer );

  ASSERT( AfxIsValidAddress( buffer, dim_buffer ) );
  
  //TRACE ("dimensione e pad: ----> %d %d", dim_buffer, pad );
  CFile::Write( buffer, dim_buffer );

  free( buffer );

  return 0;
}


int CFile_crypt::Read( void )
{

  if ( m_hFile == CFile::hFileNull ) 
  {
    msg_error = "CFile_crypt::Read: File not open!";
    TRACE( msg_error );
    return (99);
  }

  if ( GetLength()<1 || GetLength()>DIM_BUFFER ) 
  {
    msg_error.Format( "CFile_crypt::Read: Wrong file size (%d bytes)", GetLength() );
    TRACE( msg_error );
    return (99);
  }

  // READ WHOLE FILE AND COPY ALL DATA INTO <buffer>
  //                          DIM CRC PAD
  UINT dim_buffer = (UINT) GetLength(); //dim_data_file + 4 + 4 + 3;
  BYTE* buffer = (BYTE*) malloc( dim_buffer );
  ZeroMemory( buffer, dim_buffer );

  CFile::Read( buffer, dim_buffer );

  Decrypt ( buffer, dim_buffer);

  dim_tot_data = ((int*) buffer)[0];
  //dim_tot_data = dim_buffer;

  ASSERT( dim_tot_data>=0 && dim_tot_data<DIM_BUFFER );

  dim_data_file = 0;

  int crc = *((UINT*) (buffer+dim_tot_data+4));
  TRACE("CFile_crypt::Read Read CRC = %08X\n", crc );

  int crc1 = GetCrc( buffer, dim_tot_data+4 ); //Ricalcolo il crc e li confronto

  if ( crc != crc1 ) 
  {
    msg_error.Format( "CFile_crypt::Read: Read/computed CRC mismatch (%08X/%08X)", crc, crc1 );
    TRACE( msg_error );
    //Message_box("CRC mismatch",MBOX_ERROR);
    return (99);
  }

  TRACE ("Confronto CRC Ok: (%08X/%08X)\n", crc, crc1);
  CopyMemory ( data, buffer+4, dim_tot_data ); //questi sono i dati ok
 
  free (buffer);

return 0;
}


int CFile_crypt::AppendString( char* string )
{
  if ( m_hFile == CFile::hFileNull ) 
  {
    msg_error = "CFile_crypt::AppendString: File not open!";
    TRACE( msg_error );
    return (99);
  }
  
  UINT old_crc = 0;
  UINT dim_file = (UINT) GetLength();

  BYTE* file = (BYTE*) malloc( dim_file );
  int pos = 0;

  if (dim_file>4)
  {
    CFile::Read( file, dim_file );

    old_crc = *((UINT*)( file + dim_file - 4));
    pos = dim_file -4 -1; //crc e terminatore
  }

  SYSTEMTIME time;
	GetSystemTime( &time );
  char *str = (char*) malloc ( 100 );
  int len = sprintf(str, "%02d/%02d/%04d %02d:%02d:%02d %s", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, string);

  int new_crc; //calcola il crc partendo dal vecchio valore

  BYTE* buffer = (BYTE*) malloc ( len + 8 + 8 +1);
  ZeroMemory ( buffer, len + 8 + 8 );
  *((UINT*) buffer) = (UINT)len; //scrivo la lunghezza della stringa
  CopyMemory ( buffer+4, str, len );

  new_crc = GetCrc( buffer, len +4 ); //Ricalcolo il crc sommando il vecchio
  new_crc += old_crc;
  
  //int pad = ( ( (len +4 )>>3 ) +1 ) <<3;
  int pad = ( 8 - ( ( len +4 ) %8) ) %8;
  Encrypt( buffer, len + pad); //il CRC resta in chiaro
  *((UINT*) ( buffer + len + pad +4 )) = new_crc;
  buffer[ len+pad +4 +4]=0; //lungh +crc

  Seek( pos, begin ); 
  CFile::Write( buffer, len + 4 + pad +4 +1); //terminatore
 
  free( buffer );
  free( file );
  free( str );

  return 0;
}

int CFile_crypt::ReadLine( CRichEditCtrl* RichEdit)
{
  if ( m_hFile == CFile::hFileNull ) 
  {
    msg_error = "CFile_crypt::Read: File not open!";
    TRACE( msg_error );
    return (99);
  }

  int dim_buffer =  (int)GetLength(); 

  if (dim_buffer==0) 
  {
    msg_error = "CFile_crypt::Read: File empty!";
    return (99);
  }

  BYTE* buffer = (BYTE*) malloc( dim_buffer +4 );
  //BYTE* text   = (BYTE*) malloc( dim_buffer );
  ZeroMemory( buffer, dim_buffer );
  //ZeroMemory( text,   dim_buffer );

  CFile::Read( buffer, dim_buffer );
  Decrypt ( buffer, dim_buffer );

  int text_pos = 0, buffer_pos = 0;
  int line_length = *((int*)(buffer+buffer_pos)); 
 /* while ( (buffer_pos + line_length < dim_buffer-4) && (buffer_pos <dim_buffer -4) ) //&& text_pos<dim_buffer )
  { //preparo la stringa
    buffer_pos += 4;

    CopyMemory ( text+text_pos, buffer+buffer_pos, line_length );

    buffer_pos += line_length;
    text_pos   += line_length;

    text[text_pos++] = '\r';
    text[text_pos++] = '\n';
      
    while ( buffer[buffer_pos]==0 ) buffer_pos++;

    line_length = *((int*)(buffer+buffer_pos));
  }
*/
  char *text  = (char*) malloc ( dim_buffer );
  ZeroMemory( text,   dim_buffer );

  while ( (buffer_pos + line_length < dim_buffer-4) && (buffer_pos <dim_buffer -4) ) //&& text_pos<dim_buffer )
  { //preparo la stringa
    buffer_pos += 4;

    strncat ( text, (char*)(buffer+buffer_pos), line_length );

    buffer_pos += line_length;

    text_pos   = strlen (text);
    text[text_pos++]='\r';
    text[text_pos++]='\n';
      
    while ( buffer[buffer_pos]==0 )
        buffer_pos++;

    line_length = *((int*)(buffer+buffer_pos));
    if ( line_length>100 || line_length<0 )
      break;
  }

  RichEdit->SetWindowTextA( (char*)text );
  
  free (text);
  free (buffer);

return 0;
}



void CFile_crypt::RollingFile( char* nome_file )
{

  int dim_buffer = (int)GetLength()-5;
  
  BYTE* buffer  = (BYTE*) malloc ( dim_buffer );
  CFile::Read ( buffer, dim_buffer );
  //CFile_crypt::Get_data ( buffer, dim_buffer );
  Decrypt ( buffer, dim_buffer );
  int crc = *((UINT*)( buffer + dim_buffer - 5));
  CFile::Close ();

  int dim_del = FindRightPosToDelete ( nome_file, crc );

  
  CFile_crypt::Open ( nome_file , CFile::modeWrite|CFile::modeCreate );
 
  BYTE* buffer_dest = (BYTE*) malloc ( dim_buffer-dim_del+1 + 4);
  if (dim_buffer-dim_del >0 )
  {
    CopyMemory ( buffer_dest, buffer+dim_del, dim_buffer-dim_del);
    Encrypt ( buffer_dest, dim_buffer-dim_del );

  *((UINT*) ( buffer_dest + dim_buffer-dim_del)) = crc;
  buffer[dim_buffer-dim_del +4]=0;
  CFile::Write( buffer_dest, dim_buffer-dim_del +1 +4 );
  } //altrim il file è vuoto

  CFile_crypt::Close ();  

  free ( buffer_dest );
  free ( buffer );

  //riapro in normale lett
  CFile::Open ( nome_file, CFile::modeReadWrite|CFile::modeNoTruncate );

}


int CFile_crypt::FindRightPosToDelete ( char* nome_file, int& crc_loc )
{
  
  CFile_crypt::Open ( nome_file , CFile::modeRead );
  int dim_file = (int) GetLength();
  BYTE* buffer = (BYTE*) malloc ( dim_file +4 );

  CFile::Read( buffer, dim_file );
  Decrypt ( buffer, dim_file );
  
  int dim_del = (int)(0.2 * dim_file );

  int text_pos = 0, buffer_pos = 0;
  while ( buffer_pos < dim_del && buffer_pos < dim_file)  //sono al punto giusto
  { 
    int line_length = *((int*)(buffer+buffer_pos));
    
    BYTE* buff_loc = (BYTE*) malloc ( 4+line_length );
    CopyMemory( buff_loc, buffer+buffer_pos, 4+line_length ); 

    buffer_pos += 4;
    buffer_pos += line_length;

    crc_loc -= GetCrc ( buff_loc, line_length+4 );
    
    while ( buffer[buffer_pos]==0 ) buffer_pos++; 

   free ( buff_loc );  
  }
  CFile_crypt::Close();
  
  free ( buffer );

  return buffer_pos;
}

/*----------------------------- Criptaggio e Decriptaggio -------------------------------*/

//http://it.wikipedia.org/wiki/Tiny_Encryption_Algorithm


#define TEA_DELTA  0x9e3779b9
#define TEA_KEY_A  0x35435898
#define TEA_KEY_B  0x23847238


void CFile_crypt::Encrypt ( void* buffer, int dim) 
{
  int sum;
  int v0,v1;
  int delta=TEA_DELTA;         // a key schedule constant 
  int k0 = (TEA_KEY_A&0xFFFF0000)>>16;
  int k1 = (TEA_KEY_A&0x0000FFFF);
  int k2 = (TEA_KEY_B&0xFFFF0000)>>16;
  int k3 = (TEA_KEY_B&0x0000FFFF);
    
  if ( !AfxIsValidAddress( buffer, dim ) ) 
  {
    msg_error = "CFile_crypt::Encrypt: Invalid pointer/buffer";
    TRACE( msg_error );
    return;
  }

  int* ptr = (int*) buffer;

//  int j=0;
  for (int j=0; j<dim/4; j+=2)
  {
    sum = 0;                    //set up
    v0  = ptr[j  ];
    v1  = ptr[j+1];

    for (int i=0; i < 32; i++) 
    {                           // basic cycle start 
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);  
    }  

    ptr[j  ] = v0;
    ptr[j+1] = v1;
  } //for j

}

void CFile_crypt::Decrypt ( void* buffer, int dim) 
{ 
  int v0,v1;
  int sum=0;                    //set up
  int delta=TEA_DELTA;         // a key schedule constant 

  int k0 = (TEA_KEY_A&0xFFFF0000)>>16;
  int k1 = (TEA_KEY_A&0x0000FFFF);
  int k2 = (TEA_KEY_B&0xFFFF0000)>>16;
  int k3 = (TEA_KEY_B&0x0000FFFF);
 
  if ( !AfxIsValidAddress( buffer, dim ) ) 
  {
    msg_error = "CFile_crypt::Encrypt: Invalid pointer/buffer";
    TRACE( msg_error );
    return;
  }
  
  int* ptr = (int*) buffer;

//  int j=0;
  for (int j=0; j<dim/4; j+=2)
  {
    sum = 0xC6EF3720;
    v0  = ptr[j  ];
    v1  = ptr[j+1];

    for (int i=0; i<32; i++) 
    {                           // basic cycle start 
      v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
      v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
      sum -= delta;                                   
    }                                           
    ptr[j  ] = v0;
    ptr[j+1] = v1;
  } //for j

}
/*--------------------------- end Criptaggio e Decriptaggio -----------------------------*/




/*------------------------------------ Calcolo del CRC ----------------------------------*/
int CFile_crypt::GetCrc( BYTE* text, int len)
{     
  ULONG  ulCRC(0xffffffff); 
  unsigned char* buffer;

  buffer = (unsigned char*)text; 
      // Perform the algorithm on each character in the string, using the lookup table values. 
  while(len--) 
    ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *buffer++]; 
      // Exclusive OR the result with the beginning value. 
  return ulCRC ^ 0xffffffff; 
}

void CFile_crypt::Init_CRC32_Table() 
{// Call this function only once to initialize the CRC table.

      // This is the official polynomial used by CRC-32 writein PKZip, WinZip and Ethernet. 
      ULONG ulPolynomial = 0x04c11db7;

      // 256 values representing ASCII character codes. 
      for(int i = 0; i <= 0xFF; i++) 
      { 
         crc32_table[i]=Reflect(i, 8) << 24; 
         for (int j = 0; j < 8; j++) 
           crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0); 
         crc32_table[i] = Reflect(crc32_table[i], 32); 
      } 
}

ULONG CFile_crypt::Reflect(ULONG ref, char ch) 
{
   ULONG value(0);
   // Swap bit 0 for bit 7 bit 1 for bit 6, etc. 
   for(int i = 1; i < (ch + 1); i++)
   { 
     if(ref & 1) 
       value |= 1 << (ch - i); 
       ref >>= 1; 
   } 
 return value; 
}
/*--------------------------------- end - Calcolo del CRC -------------------------------*/

