// Parameter.cpp : implementation file
//

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "MainFrm.h"
#include "Globals.h"
#include "Parameter.h"

// CParameter

//#ifdef BECKHOFF  
extern AmsAddr		  Addr;
extern CDialog*     info_wnd;
extern CLogger*     logger;
//#endif

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif


 //extern wchar_t* msg_err;

//extern CPassword_manager*  password_mngr;
#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif


//extern wchar_t*      parcat_1;  // main category description for cparameter logging (camera/PLC)
//extern wchar_t*      parcat_2;  // sub  category description for cparameter logging (control)


  //Pour 20200702
 //char isdectect[1][50] ={ ".F_RUN_AUTOMATIC" };
 //unsigned long  H_F_RUN_AUTOMATIC;
  //Pour 20200702


IMPLEMENT_DYNAMIC(CParameter, CWnd)


int    CParameter::nID_prog =      1000; // Creazione e inizializzazione dei membri statici di CParameter
RECT   CParameter::area_tot = {0,0,0,0}; // Area attuale
SIZE   CParameter::dim_ctrl = { 130,26}; // Dimensione attuale largh/alt
int    CParameter::spcx     =        6; // Spaziatura orizzontale

//SIZE   CParameter::dim_ctrl = { 130,12}; // Dimensione attuale largh/alt
//int    CParameter::spcx     =         8; // Spaziatura orizzontale

int    CParameter::amp_desc =       130; // Ampiezza del campo di descrizione
int    CParameter::amp_val  =        80; // Ampiezza del campo del valore
int    CParameter::amp_udm  =        50; // Ampiezza del campo dell'unità di misura


extern CChildView*  pView;
extern UINT           lng;  // Lingua attuale

//extern wchar_t*   msg_err;
//Pour added for Sunho on 20200702
//Pour added 20200702
extern BOOL            f_run_automatic;

DWORD  CParameter::stile    = WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY;

//CFont  CParameter::font;


BEGIN_MESSAGE_MAP(CParameter, CWnd)
  ON_MESSAGE( WM_MODIFIED, &CParameter::On_modified)
//	ON_WM_SHOWWINDOW()
//ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


//#ifdef BECKHOFF


/*
// -------------------------------------------------------------------------
//      Costruttore per parametri di tipo intero su BECKHOFF
// -------------------------------------------------------------------------
CParameter::CParameter( char* var_in, int type_in, CContainer_wnd* parent_in, char* desc, char* udm, int def_in, int min_in, int max_in, CFile_mngmt* file_manager )
{
	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );
	ASSERT( AfxIsValidString(var_in) );
	if ( desc ) ASSERT( AfxIsValidString(desc) );
	if ( udm  ) ASSERT( AfxIsValidString(udm ) );
  
	var_name = var_in;
	parent   = parent_in;

	// Cerchiamo i parametri di accesso per la variabile nel PLC
	int nErr = AdsSyncReadWriteReq( &Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle), &handle, strlen(var_in), var_in );
	if ( nErr )
	{
		msg_err.Format("Impossibile rilevare su PLC la variabile intera <%s>.\r\nErrore n.%d <%s>", var_in, nErr, ADS_error_desc(nErr) );
    Message_box( msg_err );
		handle = 0; // Parametro non agganciato
	}

	// Tipi accettati
	ASSERT( type_in & CTYPE_BECKHOFF  ); 
	ASSERT( type_in & CTYPE_UDINT ); 

	type = type_in | CTYPE_BECKHOFF;
	if ( type & CTYPE_UDINT )
	{
		dim     =      4;
		val     = malloc(dim); // Alloco lo spazio necessario a mantenere il valore della variabile
    val_min = min_in;
	  val_max = max_in;
	}
	Construct_int( desc, udm, def_in, min_in, max_in );
	if ( file_manager ) file_manager->Add_data( this );
}

*/

// ------------------------------------------------------------------------------
//   Costruttore per parametri su BECKHOFF
// ------------------------------------------------------------------------------
// var_in       è la stringa contenente il nome del parametro
// type_in      è una combinazione di CTYPE_BOOL8, CTYPE_UDINT, CTYPE_STRING, CTYPE_BECKHOFF, CTYPE_READONLY, CTYPE_UPDATE
// dim_in       è la dimensione in bytes del dato (per le stringhe non include il terminatore)
// parent_in    è il puntatore al contenitore padre
// desc_in      è il puntatore alla stringa di descrizione (se NULL il campo non viene creato/visualizzato)
// udm_in       è il puntatore alla stringa dell'unit?di misura (se NULL il campo non viene creato/visualizzato)
// def_in       valore di default (per ora non utilizzato)
// min_in       min_in  minimo valore accettato in fase di validazione
// max_in       max_in  minimo valore accettato in fase di validazione
// file_manager puntatore all'oggetto file da/sui cui caricare/salvare il valore del parametro
// funct        puntatore alla funzione da chiamare in caso di modifica/caricamento del parametro


//CParameter::CParameter( char* var_in, int type_in, int dim_in, CContainer_wnd* parent_in, char* desc_in, char* udm_in, double def_in, double min_in, double max_in, CFile_mngmt* file_manager )
//CParameter::CParameter( LPVOID var_in, int type_in, int dim_in, CContainer_wnd* parent_in, char* desc_in, char* udm_in, double def_in, double min_in, double max_in, CFile_mngmt* file_manager, int (*funct_in)(int) )
CParameter::CParameter( LPVOID var_in, int type_in, int dim_in, CContainer_wnd* parent_in, wchar_t* desc_in, wchar_t* udm_in, double def_in, double min_in, double max_in, CFile_mngmt* file_manager, int (*funct_in)(int), int index_in )
{

  format_spc = L"%.1lf";
//  format_spc = "%.1lf";

  ASSERT( index_in>=0 && index_in<NUM_TLC );
  index = index_in;

  if (pView->num_params>=MAX_NUM_PARAMS)
	{
		Message_box( L"ERROR: MAX_NUM_PARAMS REACHED!", MBOX_ERROR );
	}

	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );

	if ( desc_in ) ASSERT( AfxIsValidString(desc_in) );
	if ( udm_in  ) ASSERT( AfxIsValidString(udm_in ) );

	if ( !(type_in&CTYPE_BOOL8) ) ASSERT( min_in <= def_in && def_in <= max_in );

	if ( type_in&CTYPE_BECKHOFF ) // Variabile PLC
	{
  	ASSERT( AfxIsValidString((char*)var_in) );
	  var_name = (char*) var_in;
	}
	else // Variabile PC
	{
		var_name = NULL; // Niente
	}

	desc     = desc_in;
	udm      = udm_in;
	val_def  = def_in;
	val_min  = min_in;
	val_max  = max_in;
	parent   = parent_in;
	handle   = 0;

  enabled  = TRUE;


  if ( pView->parcat_1 && AfxIsValidString(pView->parcat_1) )
  {
    parcat_1 = (wchar_t*) malloc( (20+1) * sizeof(wchar_t) );
    ZeroMemory( parcat_1, sizeof(parcat_1) );
    swprintf( parcat_1, 20, pView->parcat_1 );
  }
  else parcat_1 = NULL;

  if ( pView->parcat_2 && AfxIsValidString(pView->parcat_2) )
  {
    parcat_2 = (wchar_t*) malloc( (20+1) * sizeof(wchar_t) );
    ZeroMemory( parcat_2, sizeof(parcat_2) );
    swprintf( parcat_2, 20, pView->parcat_2 );
  }
  else parcat_2 = NULL;


//  ZeroMemory( parcat_2, sizeof(parcat_2) );
//  if ( pView->parcat_2 && AfxIsValidString(pView->parcat_2) ) swprintf( parcat_2, 20, pView->parcat_2 );


//	sprintf( format_spc , "%%.1lf" );
//	strcpy( format_spc , "%.1lf" );


	// Cerco i parametri di accesso per la variabile nel PLC
	if ( type_in&CTYPE_BECKHOFF && ADS_status )
	{
  	int nErr = AdsSyncReadWriteReq( &Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle), &handle, strlen(var_name), var_name );
	  if ( nErr )
	  {
//		  msg_err.Format( string_186[lng], var_in, nErr, ADS_error_desc(nErr) );
      //size_t  num=0;
      //wchar_t txt[100];
      //mbstowcs_s( &num, txt, 100, var_name, _TRUNCATE); // conversione char=>wchar_t
      //swprintf( msg_err, string_186[lng], txt, nErr, ADS_error_desc(nErr) );
      swprintf( msg_err, 200, string_186[lng], AtoW(var_name), nErr, ADS_error_desc(nErr) );
      Message_box( msg_err, MBOX_ERROR );
		  handle = 0; // Parametro non agganciato
	  }
	}

	// Tipi accettati
	type = type_in;

	if ( type & CTYPE_BOOL8    ) dim = 1;
	if ( type & CTYPE_BYTE     ) dim = 1;
	if ( type & CTYPE_INT      ) dim = 2;
	if ( type & CTYPE_SCALED16 ) dim = 2;        // Dato di tipo scalato (HMI=>DOUBLE - PLC=>INT)
	if ( type & CTYPE_DINT     ) dim = 4;
	if ( type & CTYPE_REAL     ) dim = 4;
	if ( type & CTYPE_SCALED32 ) dim = 4;        // Dato di tipo scalato (HMI=>DOUBLE - PLC=>DINT)
	if ( type & CTYPE_LREAL    ) dim = 8;
	if ( type & CTYPE_STRING   ) dim = dim_in+1; // Aggiungo il terminatore
	if ( type & CTYPE_GENERAL  ) dim = dim_in;   // Dato di dimensione generica

	if ( type_in&CTYPE_BECKHOFF ) // Variabile di tipo PLC, alloco lo spazio locale
	{
  	val = malloc(dim); // Alloco lo spazio necessario a mantenere il valore della variabile
	  ZeroMemory( val, dim );
	}
	else // Variabile PC, verifico che disponga già dello spazio necessario
	{
		ASSERT( AfxIsValidAddress( var_in, dim ) );
		val = var_in; // Associo al puntatore in ingresso
	}

	if ( type & CTYPE_UPDATE )
	{
		ASSERT( pView );
		ASSERT( pView->num_toupdate < MAX_NUM_PARAMS );
		pView->toupdate[pView->num_toupdate++] = this;
	}

	if ( type & CTYPE_BOOL8 ) Construct_bool();
	else                      Construct_int();

	if ( file_manager ) file_manager->Add_data( this );

	if ( funct_in ) function = funct_in;
	else            function = NULL;
  
}


/*
CParameter::CParameter( LPVOID var_in, int type_in, int dim_in, CContainer_wnd* parent_in, char* desc_in, char* udm_in, double def_in, double min_in, double max_in, CFile_mngmt* file_manager )
{
  if (pView->num_params>=MAX_NUM_PARAMS)
	{
		Message_box( "ATTENZIONE: MAX_NUM_PARAMS RAGGIUNTO!" );
	}

	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );
	ASSERT( AfxIsValidString((char*)var_in) );

	if ( desc_in ) ASSERT( AfxIsValidString(desc_in) );
	if ( udm_in  ) ASSERT( AfxIsValidString(udm_in ) );

	if ( !(type_in&CTYPE_BOOL8) ) ASSERT( min_in <= def_in && def_in <= max_in );

	var_name = (char*) var_in;
	desc     = desc_in;
	udm      = udm_in;
	val_def  = def_in;
	val_min  = min_in;
	val_max  = max_in;
	parent   = parent_in;
	handle   = 0;


	// Cerco i parametri di accesso per la variabile nel PLC
	if ( type_in & CTYPE_BECKHOFF && ADS_status )
	{
  	int nErr = AdsSyncReadWriteReq( &Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle), &handle, strlen(var_name), var_name );
	  if ( nErr )
	  {
		  msg_err.Format("Impossibile rilevare su PLC la variabile <%s>\r\nErrore n.%d <%s>\r\n\r\nNon sarà possibile visualizzare/modificare questa variabile", var_in, nErr, ADS_error_desc(nErr) );
      Message_box( msg_err );
		  handle = 0; // Parametro non agganciato
	  }
	}

	// Tipi accettati
//	ASSERT( type_in & CTYPE_BECKHOFF  ); 
//	ASSERT( type_in & CTYPE_UDINT ); 
//	ASSERT( type_in & CTYPE_READONLY  ); 
		

	type = type_in;

	if ( type & CTYPE_BOOL8   ) dim = 1;
	if ( type & CTYPE_INT     ) dim = 2;
	if ( type & CTYPE_DINT    ) dim = 4;
	if ( type & CTYPE_LREAL   ) dim = 8;
	if ( type & CTYPE_STRING  ) dim = dim_in+1; // Aggiungo il terminatore
	if ( type & CTYPE_GENERAL ) dim = dim_in;          

	val = malloc(dim); // Alloco lo spazio necessario a mantenere il valore della variabile
	ZeroMemory( val, dim );

	if ( type & CTYPE_UPDATE )
	{
		ASSERT( pView );
		ASSERT( pView->num_toupdate < MAX_NUM_PARAMS );
		pView->toupdate[pView->num_toupdate++] = this;
	}

	if ( type & CTYPE_BOOL8 ) Construct_bool();
	else                      Construct_int();

	if ( file_manager ) file_manager->Add_data( this );
  
}


*/
/*

// -------------------------------------------------------------------------
//          Costruttore per parametri di tipo bool su BECKHOFF
// -------------------------------------------------------------------------
CParameter::CParameter( char* var_in, int type_in, CContainer_wnd* parent_in, char* desc, bool def_in, CFile_mngmt* file_manager )
{
	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );
	ASSERT( AfxIsValidString(var_in) );
	if ( desc ) ASSERT( AfxIsValidString(desc) );

  var_name = var_in;
	parent   = parent_in;

	// Cerchiamo i parametri di accesso per la variabile nel PLC
	int nErr = AdsSyncReadWriteReq( &Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle), &handle, strlen(var_in), var_in );
	if ( nErr )
	{
		msg_err.Format("Impossibile rilevare su PLC la variabile booleana <%s>.\r\nErrore n.%d <%s>", var_in, nErr, ADS_error_desc(nErr) );
    Message_box( msg_err );
		handle = 0; // Parametro non agganciato
	}

	// Tipi accettati
	ASSERT( type_in==CTYPE_BOOL8 );

	type = type_in | CTYPE_BECKHOFF;
	if ( type & CTYPE_BOOL8 )
	{
		dim = 1;
		val = malloc(dim); // Alloco lo spazio necessario a mantenere il valore della variabile
	}
	Construct_bool( desc, def_in );

	if ( file_manager ) file_manager->Add_data( this );

}

*/

//#endif


/*

// -------------------------------------------------------------------------
//          Costruttore per parametri di tipo intero locali
// -------------------------------------------------------------------------
CParameter::CParameter( int* val_in, CContainer_wnd* parent_in, char* desc, char* udm, int def_in, int min_in, int max_in, CFile_mngmt* file_manager )
{
	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );
	ASSERT( AfxIsValidAddress(val_in,sizeof(int) ) );
	if ( desc ) ASSERT( AfxIsValidString(desc) );
	if ( udm  ) ASSERT( AfxIsValidString(udm ) );

	parent  = parent_in;
	type    = CTYPE_UDINT;
	val     = val_in;
	dim     =      4;
  val_min = min_in;
  val_max = max_in;

//	Construct_int( desc, udm, def_in, min_in, max_in );
	Construct_int();

  if ( file_manager ) file_manager->Add_data( this );

}


// -------------------------------------------------------------------------
//          Costruttore per parametri di tipo bool locali
// -------------------------------------------------------------------------
CParameter::CParameter( bool* val_in, CContainer_wnd* parent_in, char* desc, bool def_in, CFile_mngmt* file_manager )
{
	ASSERT( parent_in );
	ASSERT( parent_in->m_hWnd );
	ASSERT( AfxIsValidAddress(val_in,sizeof(bool) ) );
	if ( desc ) ASSERT( AfxIsValidString(desc) );

	parent = parent_in;
	type   = CTYPE_BOOL8;
	val    = val_in;
	dim    =      1;

	Construct_bool( desc, def_in );

  if ( file_manager ) file_manager->Add_data( this );

}

*/



void CParameter::Scaling_parameters( double p1_usr, int p1_int, double p2_usr, int p2_int )
{
  ASSERT( p1_usr != p2_usr );
  ASSERT( p1_int != p2_int );

  usrp[0] = p1_usr;
  usrp[1] = p2_usr;

  plcp[0] = p1_int;
  plcp[1] = p2_int;

  usr2plc = double(p2_int-p1_int)/(p2_usr-p1_usr);
  plc2usr = (p2_usr-p1_usr)/double(p2_int-p1_int);

  Update_val_text(); 
  
}


////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
//                       PARTE DI COSTRUZIONE COMUNE A TUTTI GLI OGGETTI                      //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////


// Codice di costruzione/inizializzazione generico per booleani
//void CParameter::Construct_bool( char* desc, bool def_in )
void CParameter::Construct_bool( void )
{

	// Creazione di this (CWnd)
//	nID = nID_prog++;
  VERIFY( Create(NULL,NULL,WS_CHILD,CRect(10,10,20,20),parent,nID_prog++) ); // Creazione di this
	//TRACE( L"CParameter n.%d created\n", nID_prog-1 );


  /*
	if ( !font.m_hObject )
	{
  	CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    VERIFY(font.CreatePointFont(100, L"Arial", &dc));
//		dim_ctrl     = dc.GetTextExtent("ABCDEFG");
//		dim_ctrl.cy += 2 * 3; // + 10; // Aggiungo il bordo superiore e inferiore
	}
  */

  int dimch = 20; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 20;

  font.CreateFont( dimch,                         // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                            // Orientation
                   400,                           // Weight 100-900
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );


	pt_desc = NULL;
	pt_val  = NULL;
	pt_udm  = NULL;
	pt_flag = NULL;


  SIZE dim_bool = { amp_desc, 30 };
  area_tot = parent->next_rect( dim_bool );

// codice originale
//  dim_ctrl.cx = amp_desc;
//	area_tot = parent->next_rect( dim_ctrl );

	pt_flag = new CButton_mod;
//	VERIFY( pt_flag->Create( desc, WS_CHILD|WS_VISIBLE|BS_CHECKBOX|BS_LEFTTEXT, area_tot, parent, nID_prog++ ) );
//	VERIFY( pt_flag->Create( desc, WS_CHILD|WS_VISIBLE|BS_PUSHLIKE, area_tot, parent, nID_prog++ ) );
	VERIFY( pt_flag->Create( desc, WS_CHILD|WS_VISIBLE|BS_PUSHLIKE|BS_OWNERDRAW, area_tot, parent, nID_prog++ ) );
	pt_flag->parent = this;
	pt_flag->SetFont( &font );


  // if necessary, increase height of parent window
  CRect parect;
  parent->GetWindowRect( &parect );

  int downmargin = 10;
  if ( parect.Height() < area_tot.bottom + downmargin )
  {
    parect.InflateRect( 0, 0, 0, area_tot.bottom+downmargin-parect.Height() );
    parent->MoveWindow( &parect );
  }


//	if ( type&CTYPE_BECKHOFF && !handle ) pt_flag->SetState(FALSE);

//	pt_flag->SubclassDlgItem(nID_prog-1,pView); // subclassing


//	TRACE("CParameter <%s> created (%d,%d,%d,%d)\n", desc, wpos.rcNormalPosition.top, wpos.rcNormalPosition.left, wpos.rcNormalPosition.bottom, wpos.rcNormalPosition.right );

  parent->Add_parameter( this );
	Update_val_text();

 // font.DeleteObject();

}





// Codice di costruzione/inizializzazione generico per interi
//void CParameter::Construct_int( char* desc, char* udm, int def_in, int min_in, int max_in )
void CParameter::Construct_int( void )
{

	// Creazione di this (CWnd)
//	nID = nID_prog++;
  VERIFY( Create(NULL,NULL,WS_CHILD,CRect(10,10,20,20),parent,nID_prog++) ); // Creazione di this
	//TRACE("CParameter n.%d creato\n", nID_prog-1);

/*	if ( !font.m_hObject )
	{
  	CClientDC dc(this);
    //VERIFY(font.CreatePointFont(100, "Arial", &dc));
    VERIFY(font.CreatePointFont(100, L"Arial", &dc));
//		dim_ctrl     = dc.GetTextExtent("ABCDEFG");
//		dim_ctrl.cy += 2 * 3; // Aggiungo il bordo superiore e inferiore
	} */


  int dimch = 16; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 16;

  font.CreateFont( dimch,                         // Height
                   0,                             // Width
                   0,                             // Escapement
                   10,                            // Orientation
                   400,                           // Weight 100-900
                   FALSE,                         // Italic
                   FALSE,                         // Underline
                   0,                             // StrikeOut
                   ANSI_CHARSET,                  // CharSet
                   OUT_DEFAULT_PRECIS,            // OutPrecision
                   CLIP_DEFAULT_PRECIS,           // ClipPrecision
                   DEFAULT_QUALITY,               // Quality
                   DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                   lng==CHINESE?L"SimSun":L"Arial" );


	pt_desc = NULL;
	pt_val  = NULL;
	pt_udm  = NULL;
	pt_flag = NULL;

	// Calcolo la dimensione necessaria alla visualizzazione
	dim_ctrl.cx = amp_val;

	// Se sono abilitati, aggiungo dim e spazio per "desc" e "udm"
	if ( desc ) dim_ctrl.cx += amp_desc + spcx;
	if ( udm  ) dim_ctrl.cx += amp_udm  + spcx;

	// Mi faccio dire dal contenitore padre le coordinate di costruzione
	area_tot = parent->next_rect( dim_ctrl );
	RECT area = area_tot;

	// Creazione del campo di descrizione
  if ( desc ) // Costruisco SOLO se <desc> è diverso da NULL
	{
		area.left  = area_tot.left;
  	area.right = area_tot.right-(amp_val+amp_udm+2*spcx);
	  pt_desc    = new CStatic;
	  //VERIFY( pt_desc->CreateEx( WS_EX_LEFT, "Static", desc, stile|SS_LEFT, area , parent, nID_prog++ ) );
	  VERIFY( pt_desc->CreateEx( WS_EX_LEFT, L"Static", desc, stile|SS_LEFT, area , parent, nID_prog++ ) );
	  pt_desc->SetFont( &font );
	}

	// Creazione del campo dell'unità di misura
  if ( udm ) // Costruisco SOLO se <udm> è diverso da NULL
	{
		area.right  = area_tot.right; // Regolazione delle x
		area.left   = area_tot.right-amp_udm;
	  pt_udm      = new CStatic;
	  //VERIFY( pt_udm->CreateEx( WS_EX_LEFT, "Static", udm, stile|SS_CENTER, area , parent, nID_prog++ ) );
	  VERIFY( pt_udm->CreateEx( WS_EX_LEFT, L"Static", udm, stile|SS_CENTER, area , parent, nID_prog++ ) );
	  pt_udm->SetFont( &font );
	}

	// Creazione del campo di visualizzazione/modifica del valore
	if ( desc )	area.left  = area_tot.right-(amp_val+amp_udm+spcx); // Regolazione delle x
	else        area.left  = area_tot.left;

	if ( udm )  area.right = area_tot.right-(amp_udm+spcx);
	else        area.right = area_tot.right;

//DWORD  CParameter::stile    = WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY;

	// SS_CENTERIMAGE permette il multiriga ma impedisce l'andata a capo
	pt_val      = new CStatic_mod;

	//if (type&CTYPE_READONLY) VERIFY( pt_val->CreateEx( WS_EX_STATICEDGE, "Static", "###", WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_CENTER|SS_CENTERIMAGE, area , parent, nID_prog++ ) );		
	//else                     VERIFY( pt_val->CreateEx( WS_EX_TOPMOST|WS_EX_CLIENTEDGE, "Static", "###", WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_CENTER|SS_CENTERIMAGE, area , parent, nID_prog++ ) );
	if (type&CTYPE_READONLY) VERIFY( pt_val->CreateEx( WS_EX_STATICEDGE,               L"Static", L"###", WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_CENTER|SS_CENTERIMAGE, area , parent, nID_prog++ ) );		
	else                     VERIFY( pt_val->CreateEx( WS_EX_TOPMOST|WS_EX_CLIENTEDGE, L"Static", L"###", WS_CHILD|WS_VISIBLE|SS_NOTIFY|SS_CENTER|SS_CENTERIMAGE, area , parent, nID_prog++ ) );

	pt_val->parent = this;
	pt_val->SetFont( &font );

  // if necessary, increase height of parent window
  CRect parect;
  parent->GetWindowRect( &parect );

  int downmargin = 10;
  if ( parect.Height() < area.bottom + downmargin )
  {
    parect.InflateRect( 0, 0, 0, area.bottom+downmargin-parect.Height() );
    parent->MoveWindow( &parect );
  }


//	pt_val->SubclassDlgItem(nID_prog-1,pView); // subclassing
  parent->Add_parameter( this );
	
  if ( !(type & (CTYPE_SCALED16|CTYPE_SCALED32) ) ) Update_val_text();

  //font.DeleteObject();

}


CParameter::~CParameter()
{

  if ( parcat_1 ) 
  {
    delete parcat_1;
    parcat_1 = NULL;
  }

  if ( parcat_2 ) 
  {
    delete parcat_2;
    parcat_2 = NULL;
  }

  font.DeleteObject();

	if ( pt_flag )
	{
		pt_flag->DestroyWindow();
		delete pt_flag;
	}

	if ( pt_val  )
	{
		pt_val->DestroyWindow();
		delete pt_val;
	}

	if ( pt_desc )
	{
		pt_desc->DestroyWindow();
		delete pt_desc;
	}

	if ( pt_udm  )
	{
		pt_udm->DestroyWindow();
		delete pt_udm;
	}

	if ( type&CTYPE_BECKHOFF && val ) // Dealloco solo nel caso di parametri PLC
	{
		free( val );
		val = NULL;
	}

	DestroyWindow();

	//TRACE("CParameter destroyed\n" );
}



// Chiamata dal CStatic_mod *pt_val quando viene cliccato dall'utente  
LRESULT CParameter::On_modified(WPARAM wParam, LPARAM lParam)
{
	//TRACE( L"Parameter <%s> clicked WP=%d - LP=%d\n", desc, wParam, lParam );
  CString cat = L"Parameter ";
  CString txt;
  if ( parcat_1 )
  {
    txt.Format( L"<%s> ", parcat_1 );
    cat.Append( txt );
  }
  if ( parcat_2 )
  {
    txt.Format( L"<%s> ", parcat_2 );
    cat.Append( txt );
  }
  txt.Format( L"<%s> ", desc );
  cat.Append( txt );

	if ( type & CTYPE_READONLY ) return( 0 ); // Se è readonly, skippa e ritorna


  if ( !enabled ) // return;  // ask authentication
  {
    if ( password_mngr )
    {
      password_mngr->authenticate_user();
      if ( !enabled ) return(0); //password_mngr->current_level
    }

  }

	if ( type & CTYPE_BOOL8 )
	{	
    if ( *((bool*)val) ) // previously TRUE
    {
      *((bool*)val) = 0; // Flag=TRUE,  lo metto a false
      cat.Append( L"changed from TRUE to FALSE" );
    }
		else // previously FALSE
    {
      *((bool*)val) = 1; // Flag=FALSE, lo metto a true
      cat.Append( L"changed from FALSE to TRUE" );
    }

		#ifdef BECKHOFF
      if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
    #endif

    if ( function ) (*function)(index);
			
		//return( 0 );
	}



	if ( type & CTYPE_BYTE )
	{
		//Pour added on 20200702

		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702
  	CInput_figures numpad;
		swprintf( numpad.buffer, 10, L"%d" , *(BYTE*)val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			int val_ins;
			if ( swscanf( numpad.buffer , L"%d" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          BYTE old_val = *(BYTE*)val;
					if ( type & CTYPE_BYTE ) *(BYTE*)val = val_ins;
					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);

          txt.Format( L"changed from %d to %d", old_val, val_ins );
          cat.Append( txt );


			/*
		  //add the note of operation in log file for VEM
		  if (password_mngr->annotate_on)
		  {
			  numpad.read_string( numpad.note,string_1043[lng]);
		  }
		  txt.Format(L" - %s",numpad.note);
		  cat.Append(txt);
		  // modif end
		  */
					//TRACE( L"Control modified: new value %d!\n", val_ins );
				}
				else
				{
					CString msg;
					//msg.Format("Input value <%d> is outside allowed range <%d-%d>", val_ins, int(val_min), int(val_max) );
					msg.Format( string_500[lng] , val_ins, int(val_min), int(val_max) );
					Message_box( msg );
				}
			}
			else Message_box( string_502[lng], MBOX_ERROR);
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}



	if ( type & CTYPE_INT )
	{
		//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702

  	CInput_figures numpad;
		//Pour added 20170724
		if (type & CTYPE_UNSIGNED)swprintf( numpad.buffer,10,L"%d",*(short unsigned int*)val);
		else                      swprintf(numpad.buffer, 10, L"%d" , *(short int*)val );
		//swprintf( numpad.buffer, 10, L"%d" , *(short int*)val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			int val_ins;
			if ( swscanf( numpad.buffer , L"%d" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          short int old_val = *(short int*)val;
					if ( type & CTYPE_INT ) *(short int*)val = val_ins;
					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);
					//TRACE("Controllo modificato: inserito %d!\n", val_ins );

          txt.Format( L"changed from %d to %d", old_val, val_ins );
          cat.Append( txt );
		  /*
		  					// add the note of operation in log file for VEM 
					if( password_mngr->annotate_on)
					{
						numpad.read_string( numpad.note, string_1043[lng]);
					}
					txt.Format( L" - %s",numpad.note);
					cat.Append(txt);
					// modif end 
		*/
          //TRACE( L"Control modified: new value %d!\n", val_ins );
				}
				else
				{
					CString msg;
					msg.Format( string_500[lng], val_ins, int(val_min), int(val_max) );
					Message_box( msg );
				}
			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}


	if ( type & CTYPE_DINT )
	{
		//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702

  	CInput_figures numpad;
		swprintf( numpad.buffer, 10, L"%d" , *(int*)val );
		numpad.Create( IDD_INPUT_FIGURES, this );

		if ( numpad.RunModalLoop() == IDOK )
		{
			int val_ins;
			if ( swscanf( numpad.buffer , L"%d" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          int old_val = *(int*)val;
					if ( type & CTYPE_DINT ) *(int*)val = val_ins;
					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);

          txt.Format( L"changed from %d to %d", old_val, val_ins );
          cat.Append( txt );
		  /*
		  					// add the note of operation in log file for VEM 
					if( password_mngr->annotate_on)
					{
						//Pour marked for testing on 20200701
						numpad.read_string( numpad.note, string_1043[lng]);

					}
					txt.Format( L" - %s",numpad.note);
					cat.Append(txt);
					// modif end 
		*/
					//TRACE( L"Control modified: new value %d!\n", val_ins );
				}
				else
				{
					CString msg;
					msg.Format( string_500[lng], val_ins, int(val_min), int(val_max) );
					Message_box( msg );
				}
			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}

	if ( type & CTYPE_STRING )
  {
	  	//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702

    //ASSERT( val>=0 && valore<NUM_RECIPES );
    CInput_string stringpad( this, (wchar_t*)val, MAX_RECIPE_LEN ); // parent, stringa iniziale, massima lunghezza

    //	if ( stringpad.RunModalLoop() == IDOK )
    if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>0 )
    {
      ASSERT( AfxIsValidString(stringpad.buffer) );
      ASSERT( wcslen(stringpad.buffer)<=MAX_RECIPE_LEN );
      //TRACE( L"interted value: %s\n", stringpad.buffer );
      wcsncpy( (wchar_t*)val, stringpad.buffer, wcslen(stringpad.buffer) );

      txt.Format( L"set to <%s>", stringpad.buffer );
      cat.Append( txt );

    }
  }

	if ( type & CTYPE_LREAL )
	{
		//Pour added on 20200702
		//if ( f_run_automatic )
		//{
		//Message_box( string_122[lng], MBOX_ERROR );
		//return( 0 );
		//}
		//Pour added on 20200702
  	CInput_figures numpad;
		swprintf( numpad.buffer, 10, format_spc , *(double*)val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			double val_ins;
			if ( swscanf( numpad.buffer , L"%lf" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          double old_val = *(double*)val;
					if ( type & CTYPE_LREAL ) *(double*)val = val_ins;
					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);
					//TRACE( L"Control modified: new value %6.1lf!\n", val_ins );

          txt.Format( L"changed from %.3lf to %.3lf", old_val, val_ins );
          cat.Append( txt );

			/*
		  					// add the note of operation in log file for VEM 
					if( password_mngr->annotate_on)
					{
						//Pour marked for testing on 20200701
						numpad.read_string( numpad.note, string_1043[lng]);

					}
					txt.Format( L" - %s",numpad.note);
					cat.Append(txt);
					// modif end 
			*/
				}
				else
				{
					CString msg;
					msg.Format( string_501[lng], val_ins, val_min, val_max );
					Message_box( msg );
				}

			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}

	if ( type & CTYPE_REAL )
	{
		//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702
  	CInput_figures numpad;
		swprintf( numpad.buffer, 10, format_spc , *(float*)val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			float val_ins;
			if ( swscanf( numpad.buffer , L"%f" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          float old_val = *(float*)val;
					if ( type & CTYPE_REAL ) *(float*)val = (float) val_ins;
					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);
					//TRACE( L"Control modified: new value %6.1f!\n", val_ins );

          txt.Format( L"changed from %.3f to %.3f", old_val, val_ins );
          cat.Append( txt );
		  /*
		  					// add the note of operation in log file for VEM 
					if( password_mngr->annotate_on)
					{
						//Pour marked for testing on 20200701
						numpad.read_string( numpad.note, string_1043[lng]);

					}
					txt.Format( L" - %s",numpad.note);
					cat.Append(txt);
					// modif end 
			*/
				}
				else
				{
					CString msg;
					msg.Format( string_501[lng], val_ins, val_min, val_max );
					Message_box( msg );
				}

			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}


	if ( type & CTYPE_SCALED16 )
	{
		//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702
  	CInput_figures numpad;

    ASSERT( plc2usr );
    double usr_val = usrp[0] + double( (*(short*)val) - plcp[0] ) * plc2usr;
		swprintf( numpad.buffer, 10, format_spc , usr_val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			double val_ins;
			if ( swscanf( numpad.buffer , L"%lf" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          ASSERT( usr2plc );
          *((short*)val) = plcp[0] + DOUBLETOSHORT(usr2plc*(val_ins-usrp[0]));

					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);
					//TRACE( L"Control modified: new value %6.1lf!\n", val_ins );

          txt.Format( L"set to %.3lf", val_ins );
          cat.Append( txt );

				}
				else
				{
					CString msg;
					msg.Format( string_501[lng], val_ins, val_min, val_max );
					Message_box( msg );
				}

			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}



	if ( type & CTYPE_SCALED32 )
	{
		//Pour added on 20200702
		if ( f_run_automatic )
		{
		Message_box( string_122[lng], MBOX_ERROR );
		return( 0 );
		}
		//Pour added on 20200702
  	CInput_figures numpad;

    ASSERT( plc2usr );
    double usr_val = usrp[0] + double( (*(int*)val) - plcp[0] ) * plc2usr;
		swprintf( numpad.buffer, 10, format_spc , usr_val );
		numpad.Create( IDD_INPUT_FIGURES, this );
		if ( numpad.RunModalLoop() == IDOK )
		{
			double val_ins;
			if ( swscanf( numpad.buffer , L"%lf" , &val_ins ) == 1 )
			{
				if ( val_ins>=val_min && val_ins<=val_max ) // Validazione del valore inserito
				{
          ASSERT( usr2plc );
          *((int*)val) = plcp[0] + DOUBLETOINT(usr2plc*(val_ins-usrp[0]));

					#ifdef BECKHOFF
  					if ( type & CTYPE_BECKHOFF ) Write_PLC_val(); // Parametro di tipo Beckhoff
          #endif
					Update_val_text();
          if ( function ) (*function)(index);
					//TRACE( L"Control modified: new value %6.1lf!\n", val_ins );

          txt.Format( L"set to %.3lf", val_ins );
          cat.Append( txt );

				}
				else
				{
					CString msg;
					msg.Format( string_501[lng], val_ins, val_min, val_max );
					Message_box( msg );
				}

			}
			else Message_box( string_502[lng], MBOX_ERROR );
		}
		else TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}

	// Signal to parent that parameter has (possibily) been changed
	if ( parent && parent->m_hWnd ) parent->PostMessage( WM_MODIFIED );

  if ( logger ) logger->Add_string( &cat );

  return( 0 );
}




void CParameter::Update_val_text( void )
{
	wchar_t text[20+1];
	#ifdef BECKHOFF
    if ( type & CTYPE_BECKHOFF )
		{
			if ( !handle || ADS_status!=1 )
			{
				if ( pt_val  ) pt_val->SetWindowText( L"###" );
				if ( pt_flag ) pt_flag->EnableWindow( FALSE );
				return; // Skippo se non c'è connessione/aggancio
			}
			Read_PLC_val();
		}
  #endif

//	if ( !IsWindowVisible() ) return;
//	if ( !(pt_val->IsWindowVisible()) ) return;

	// Aggiornamento della visualizzazione in caso di campo stringa
	if ( type & CTYPE_STRING )
	{
//		swprintf( text , L"%s" , (wchar_t*)val );
		swprintf( text , 20 , L"%s" , (wchar_t*)val );
		ASSERT( AfxIsValidString( text ) );
	  pt_val->SetWindowText( text );
	}


	// Aggiornamento della visualizzazione in caso di campo numerico intero 32bit
	if ( type & CTYPE_DINT )
	{
//		swprintf( text , L"%d" , *((int*)val) );
		swprintf( text , 20 , L"%d" , *((int*)val) );
	  pt_val->SetWindowText( text );
	}

	if ( type & CTYPE_INT )
	{
//		swprintf( text , L"%d" , *((short int*)val) );
		if ( type & CTYPE_UNSIGNED ) swprintf ( text, 20, L"%d", *((unsigned short int*)val));
		else                         swprintf ( text, 20, L"%d", *((short int*)val));
		//swprintf( text , 20 , L"%d" , *((short int*)val) );
	  pt_val->SetWindowText( text );
	}

	if ( type & CTYPE_BYTE )
	{
//		swprintf( text , L"%d" , *((BYTE*)val) );
		swprintf( text , 20 , L"%d" , *((BYTE*)val) );
	  pt_val->SetWindowText( text );
	}

  // Aggiornamento della visualizzazione in caso di campo numerico LREAL (double)
	if ( type & CTYPE_LREAL )
	{
//		swprintf( text , format_spc , *((double*)val) );
		swprintf( text , 20 , format_spc , *((double*)val) );
	  pt_val->SetWindowText( text );
	}

  // Aggiornamento della visualizzazione in caso di campo numerico LREAL (double)
	if ( type & CTYPE_REAL )
	{
//		swprintf( text , format_spc , *((float*)val) );
		swprintf( text , 20 , format_spc , *((float*)val) );
	  pt_val->SetWindowText( text );
	}

	if ( type & CTYPE_SCALED16 )
	{
    ASSERT( plc2usr );
    double usr_val = usrp[0] + double( (*(short*)val) - plcp[0] ) * plc2usr;
//    swprintf( text , format_spc , usr_val );
    swprintf( text , 20 , format_spc , usr_val );
	  pt_val->SetWindowText( text );
	}


	if ( type & CTYPE_SCALED32 )
	{
    ASSERT( plc2usr );
    double usr_val = usrp[0] + double( (*(int*)val) - plcp[0] ) * plc2usr;
//    swprintf( text , format_spc , usr_val );
    swprintf( text , 20 , format_spc , usr_val );
	  pt_val->SetWindowText( text );
	}



}




//#ifdef BECKHOFF

// Funzione che legge il valore attuale da PLC in val
int CParameter::Read_PLC_val( void )
{
	if ( ADS_status!=1 || !handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata
	int nErr = AdsSyncReadReq( &Addr, ADSIGRP_SYM_VALBYHND, handle, dim, val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format( string_184[lng], var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_184[lng], var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
//		handle = 0; // Parametro non agganciato
		return( nErr );
	}

	return( 0 );
}


// Funzione che scrive il valore attuale di val nel PLC
int CParameter::Write_PLC_val( void )
{
	if ( ADS_status!=1 || !handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata
	int nErr = AdsSyncWriteReq( &Addr, ADSIGRP_SYM_VALBYHND, handle, dim, val ); 
	if ( nErr )
	{
//		msg_err.Format("Impossibile scrivere il valore di <%s> nel PLC\r\nErrore n.%d <%s>", var_name, nErr, ADS_error_desc(nErr) );
//		Message_box( msg_err );
//		handle = 0; // Parametro non agganciato

		ADS_status = 0;
//		msg_err.Format( string_182[lng], var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_182[lng], var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

//	TRACE("Scritto PLC param\n");

	return( 0 );
}

/*
//Pour added on 20200702
void CParameter::Read_PLC_bool( ULONG handle, bool *input )
{
		nErr = AdsSyncReadReq(pAddr, ADSIGRP_SYM_VALBYHND, handle, 0x1, input);
	if (nErr) cerr << "Error: AdsSyncReadBOOLReq: " << nErr << '\n';
}
*/