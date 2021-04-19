#include "StdAfx.h"
#include "File_mngmt.h"
#include "Globals.h"


extern UINT  lng;
extern wchar_t          PC_name[30]; // Computer name read from system configuration

CFile_mngmt::CFile_mngmt( wchar_t* name_in /*= "default.par"*/ )
{
	if ( name_in ) ASSERT( AfxIsValidString(name_in) && wcslen(name_in)>4 ); // C'è qualcosa che non va nel nome!

	ZeroMemory( ptr,       sizeof( ptr       ) );
	ZeroMemory( data,      sizeof( data      ) );
	ZeroMemory( separator, sizeof( separator ) );

  for ( int i=0 ; i<MAX_NUM_DATA ; i++ ) separator[i].index = 0xFFFFFFFF;

  num_separators = 0;
	num_params     = 0;
	num_data       = 0; 
	dim_file       = 0;
	file_name      = name_in; //"default.par";

  swprintf( recipe_name, L"default" );
  Add_data( &recipe_name, sizeof(recipe_name) );

}

CFile_mngmt::~CFile_mngmt(void)
{
  for ( int i=0 ; i<MAX_NUM_DATA ; i++ )
  {
    if ( separator[i].text )
    {
      free( separator[i].text );
      separator[i].text = NULL;
    }
  }
}


// Aggiunta di un parametro di tipo CParameter
int CFile_mngmt::Clear_all( void )
{
	ZeroMemory( ptr,       sizeof( ptr       ) );
	ZeroMemory( data,      sizeof( data      ) );
	ZeroMemory( separator, sizeof( separator ) );

  for ( int i=0 ; i<MAX_NUM_DATA ; i++ ) separator[i].index = 0xFFFFFFFF;

  num_separators = 0;
	num_params     = 0;
	num_data       = 0; 
	dim_file       = 0;

  TRACE("file_manager.Clear_data() completed\n");

  return 0;
}



// Aggiunta di un parametro di tipo CParameter
int CFile_mngmt::Add_data( CParameter* par_in )
{
	ASSERT( par_in );
	ASSERT( AfxIsValidAddress( par_in, sizeof( CParameter ) ) );
	if ( num_params>=MAX_NUM_PARAMS )
	{
		//Message_box( "Raggiunto MAX_NUM_PARAMS, impossibile gestire caricamento&salvataggio del parametro" );
		Message_box( L"Reached MAX_NUM_PARAMS, unable to add load/save of parameter", MBOX_ERROR );
		return( 99 );
	}

	ptr[num_params++] = par_in;
	dim_file += par_in->dim; // accumulo la dimensione del dato
	return 0;
}

// Aggiunta di un dato generico (puntatore e dimensione in bytes)
int CFile_mngmt::Add_data( void* data_in, UINT size_in, UINT type_in /*=0*/ )
{
	ASSERT( data_in && AfxIsValidAddress(data_in, size_in) );
  ASSERT( size_in > 0 && size_in < 5000000 );

	if ( num_data>=MAX_NUM_DATA )
	{
		//Message_box( "Raggiunto MAX_NUM_DATA, impossibile gestire caricamento&salvataggio del parametro" );
		Message_box( L"Reached MAX_NUM_DATA, unable to add load/save of parameter", MBOX_ERROR );
		return( 99 );
	}

	data[num_data].ptr  = data_in;
	data[num_data].size = size_in;
  data[num_data].type = type_in;
	num_data++;

	dim_file += size_in; // accumulo la dimensione del dato
	return 0;
}


// Aggiunta di un dato generico (puntatore e dimensione in bytes)
//int CFile_mngmt::Add_separator( wchar_t* text_in ) 
int CFile_mngmt::Add_separator( wchar_t* text_in, CArea_adjust* area/*=NULL*/ ) 
{
	ASSERT( text_in && AfxIsValidString( text_in) );
  ASSERT( wcslen(text_in) > 0 && wcslen(text_in) < 200 );

  if ( text_in && AfxIsValidString(text_in) ) pView->parcat_2 = text_in; // string_1809[lng];

	if ( num_separators>=MAX_NUM_DATA )
	{
		Message_box( L"Reached MAX_NUM_DATA, unable to add new separator", MBOX_ERROR );
		return( 99 );
	}

  separator[num_separators].text  = (wchar_t*) malloc( (wcslen(text_in)+1)*sizeof(wchar_t) );
  wcscpy( separator[num_separators].text, text_in );
	separator[num_separators].index = num_params;

  if ( area ) ASSERT( AfxIsValidAddress(area,sizeof(CArea_adjust*) ) );
  separator[num_separators].area  = area;
  //TRACE( L"New separator[%d] added: <%s> index=%d\n", num_separators, separator[num_separators].text, separator[num_separators].index );
	num_separators++;
	return 0;
}






int CFile_mngmt::Save_text( wchar_t* name, int lang )
{

  wchar_t txt[200+1];
	wcsncpy( txt, name, wcslen(name)-4 );
	txt[wcslen(name)-4] = 0;
	wcscat( txt, L".txt" );

  CFile file;
  CFileException ex;
  UINT i;

	if ( !file.Open( txt, CFile::modeCreate|CFile::modeWrite , &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
		return 99; // Errore!
	}

  UINT sepind = 0;

  // Write CParameter objects
	for ( i=0 ; i<num_params ; i++ )
	{

    while ( separator[sepind].index==i && sepind<num_separators )
    {
      swprintf( txt, 200, L"< < < < <     %s     > > > > >\r\n", separator[sepind].text );
      ASSERT( AfxIsValidString( txt ) );
      file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
       
      if ( separator[sepind].area )
      {
        swprintf( txt, 200, L"Inspection area.........................(%d,%d)-(%d,%d)\r\n", separator[sepind].area->buffrect.left, separator[sepind].area->buffrect.top, separator[sepind].area->buffrect.right, separator[sepind].area->buffrect.bottom  );
        ASSERT( AfxIsValidString( txt ) );
        file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
      }

      sepind++;
    }
    
    wchar_t desc[200+1];
    UINT maxl = 40;

    if ( 0 ) // ( ptr[i]->type & CTYPE_BECKHOFF ) removed: use always text shown on screen
    {
      ASSERT( ptr[i]->var_name && AfxIsValidString(ptr[i]->var_name) );
      swprintf( desc, 200, L"%S", ptr[i]->var_name );
    }
    else
    {
      ASSERT( ptr[i]->desc && AfxIsValidString(ptr[i]->desc) );
      swprintf( desc, 200, L"%s", ptr[i]->desc );
    }

    UINT j;
    for ( j=wcslen(desc) ; j<maxl ; j++ ) desc[j]=L'.';
    desc[j] = 0;

    //TRACE("maxl=%d\n", maxl);

    if ( ptr[i]->type & CTYPE_STRING   ) swprintf( txt, 200, L"%s %s\r\n",    desc,   (char*)     ptr[i]->val  );
    //if ( ptr[i]->type & CTYPE_INT      ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((short int*)ptr[i]->val) );
	if ( ptr[i]->type & CTYPE_INT      ) 
	{ if(ptr[i]->type & CTYPE_UNSIGNED) swprintf(txt,200,L"%s %d\r\n",     desc,   *((short unsigned int*)ptr[i]->val) );
		else                            swprintf( txt, 200, L"%s %d\r\n",    desc, *((short int*)ptr[i]->val) );
	}
    if ( ptr[i]->type & CTYPE_DINT     ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((int*)      ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_LREAL    ) swprintf( txt, 200, L"%s %.3lf\r\n", desc, *((double*)   ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_REAL     ) swprintf( txt, 200, L"%s %.3f\r\n",  desc, *((float*)    ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_SCALED16 ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((short*)    ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_SCALED32 ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((int*)      ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_BOOL8 )
    {
      if ( *((bool*)(ptr[i]->val)) )   swprintf( txt, 200, L"%s %s\r\n", desc, string_enabled [lang] );
      else                             swprintf( txt, 200, L"%s %s\r\n", desc, string_disabled[lang] );
    }
    
    ASSERT( AfxIsValidString( txt ) );
		file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
	}

	file.Flush();
	file.Close();
	return 0;
}




// Il nome passato dev'essere lungo almeno 5 caratteri ed includere l'estensione di 3 car.
//int CFile_mngmt::Save_all( char* name /* =NULL */ )
int CFile_mngmt::Save_all( wchar_t* name /* =NULL */ )
{
	//if ( name ) ASSERT( AfxIsValidString(name) && strlen(name)>4 ); // C'è qualcosa che non va nel nome!
	if ( name ) ASSERT( AfxIsValidString(name) && wcslen(name)>4 ); // C'è qualcosa che non va nel nome!
	else name = file_name;

	//ASSERT( AfxIsValidString(name) && strlen(name)>4 ); // O dalla chiamata o dalla inizializzazione dev'essere a posto
	ASSERT( AfxIsValidString(name) && wcslen(name)>4 ); // O dalla chiamata o dalla inizializzazione dev'essere a posto

	CFile file;
	CFileException ex;

	// Verifica apertura del file in scrittura
	if ( !file.Open( name, CFile::modeCreate|CFile::modeWrite , &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
		return 99; // Errore!
	}

	UINT i;

  // Salvo i valori dei parametri
	for ( i=0 ; i<num_params ; i++ )
	{
		file.Write( ptr[i]->val, ptr[i]->dim );
	}

  // Salvo i valori dei dati
	for ( i=0 ; i<num_data ; i++ )
	{
		file.Write( data[i].ptr, data[i].size );
	}

	file.Flush();
	file.Close();

	if ( num_params==0 ) return 0;

	// Finito di salvare il binario, salvo anche la versione txt
	
	// Verifica apertura del file in scrittura
/*  char txt[200];
	strncpy( txt, name, strlen(name)-4 );
	txt[strlen(name)-4] = 0;
	strcat( txt, ".txt" ); */


  Save_text( name, lng );


/* COMMENTO E PROVO VERSIONE SEPARATA
  wchar_t txt[200+1];
	wcsncpy( txt, name, wcslen(name)-4 );
	txt[wcslen(name)-4] = 0;
	wcscat( txt, L".txt" );

	if ( !file.Open( txt, CFile::modeCreate|CFile::modeWrite , &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
		return 99; // Errore!
	}

  UINT sepind = 0;

  // Write CParameter objects
	for ( i=0 ; i<num_params ; i++ )
	{

    while ( separator[sepind].index==i && sepind<num_separators )
    {
      swprintf( txt, 200, L"< < < < <     %s     > > > > >\r\n", separator[sepind].text );
      ASSERT( AfxIsValidString( txt ) );
      file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
       
      if ( separator[sepind].area )
      {
        swprintf( txt, 200, L"Inspection area.........................(%d,%d)-(%d,%d)\r\n", separator[sepind].area->buffrect.left, separator[sepind].area->buffrect.top, separator[sepind].area->buffrect.right, separator[sepind].area->buffrect.bottom  );
        ASSERT( AfxIsValidString( txt ) );
        file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
      }

      sepind++;
    }
    
    wchar_t desc[200+1];
    UINT maxl = 40;

    if ( 0 ) // ( ptr[i]->type & CTYPE_BECKHOFF ) removed: use always text shown on screen
    {
      ASSERT( ptr[i]->var_name && AfxIsValidString(ptr[i]->var_name) );
      swprintf( desc, 200, L"%S", ptr[i]->var_name );
    }
    else
    {
      ASSERT( ptr[i]->desc && AfxIsValidString(ptr[i]->desc) );
      swprintf( desc, 200, L"%s", ptr[i]->desc );
    }

    UINT j;
    for ( j=wcslen(desc) ; j<maxl ; j++ ) desc[j]=L'.';
    desc[j] = 0;

    //TRACE("maxl=%d\n", maxl);

    if ( ptr[i]->type & CTYPE_STRING ) swprintf( txt, 200, L"%s %s\r\n",    desc,   (char*)     ptr[i]->val  );
    if ( ptr[i]->type & CTYPE_INT    ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((short int*)ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_DINT   ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((int*)      ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_LREAL  ) swprintf( txt, 200, L"%s %.3lf\r\n", desc, *((double*)   ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_REAL   ) swprintf( txt, 200, L"%s %.3f\r\n",  desc, *((float*)    ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_SCALED ) swprintf( txt, 200, L"%s %d\r\n",    desc, *((int*)      ptr[i]->val) );
    if ( ptr[i]->type & CTYPE_BOOL8 )
    {
      if ( *((bool*)(ptr[i]->val)) )   swprintf( txt, 200, L"%s %s\r\n", desc, string_enabled [lng] );
      else                             swprintf( txt, 200, L"%s %s\r\n", desc, string_disabled[lng] );
    }
    
    ASSERT( AfxIsValidString( txt ) );
		file.Write( txt, wcslen(txt)*sizeof(wchar_t) );
	}

	file.Flush();
	file.Close();*/

	return 0; // Eseguito correttamente
}


// Ritorna 0 se tutto Ok
//int CFile_mngmt::Load_all( char* name /* =NULL */  )
int CFile_mngmt::Load_all( wchar_t* name /* =NULL */  )
{
	//if ( name ) ASSERT( AfxIsValidString(name) && strlen(name)>4 ); // C'è qualcosa che non va nel nome!
	if ( name ) ASSERT( AfxIsValidString(name) && wcslen(name)>4 ); // C'è qualcosa che non va nel nome!
	else name = file_name;

  ASSERT( AfxIsValidString(name) && wcslen(name)>4 ); // O dalla chiamata o dalla inizializzazione dev'essere a posto

	CFile file;
	CFileException ex;

	// Verifica apertura del file in scrittura
	if ( !file.Open( name, CFile::modeRead|CFile::shareDenyWrite, &ex ) )
	{
    //char txt[400];
    //TCHAR desc[255];
		//ex.GetErrorMessage( desc , 255 );
    //wcscpy( msg_err, desc );
    ex.GetErrorMessage( msg_err , 255 );
		return 99; // Errore: Impossibile aprire il file
	}

	if ( file.GetLength() != dim_file )
	{
//		swprintf( msg_err, 254, L"Expected file size (%d bytes) to load <%s>\r\ndoesn't match with actual file (%d bytes)", dim_file, name, file.GetLength() );
		swprintf( msg_err, 254, L"Expected file size (%d bytes) to load <%s>\r\ndoesn't match with actual file (%d bytes)", dim_file, name, file.GetLength() );
		return 99; // Errore!
	}

	UINT i;


  
  // Leggo i valori
 	for ( i=0 ; i<num_params ; i++ )
	{
    ASSERT( ptr[i] && ptr[i]->val );
    ASSERT( AfxIsValidAddress( ptr[i]->val, ptr[i]->dim ) );
		file.Read( ptr[i]->val, ptr[i]->dim ); // Leggo da file il valore
		#ifdef BECKHOFF
		  if ( ptr[i]->type & CTYPE_BECKHOFF ) ptr[i]->Write_PLC_val();   // Prima di aggiornare il video, scrivo nel PLC il valore caricato
    #endif
		ptr[i]->Update_val_text(); // Aggiorno la visualizzazione

		if ( ptr[i]->function )
		{
      int (*function)(int) = ptr[i]->function; // wrapper pointer
//		  function(0);
      function( ptr[i]->index );
		}
	}

  
  // Leggo i valori dei dati
	for ( i=0 ; i<num_data ; i++ )
	{
    ASSERT( AfxIsValidAddress( data[i].ptr, data[i].size ) );
		file.Read( data[i].ptr, data[i].size );
	}
  
  TRACE("End of reading - pos=%d\n", file.GetPosition() );

	file.Close();

//  #ifdef TIVS_C40 // MODEL TIVS-C40 - Update freezemode_on after loading recipe
  #if defined TIVS_C40 || defined TIVS_C60  // if combo machine Update freezemode_on after loading recipe
    if ( IS_PC1 ) Change_mode( -1 ); // Only update according to current value
  #endif


  if ( pView && pView->container_main && pView->container_main->Recipe_st && pView->container_main->Recipe_st->m_hWnd ) pView->container_main->Recipe_st->SetWindowTextW( recipe_name );


	return 0; // Eseguito correttamente
}

