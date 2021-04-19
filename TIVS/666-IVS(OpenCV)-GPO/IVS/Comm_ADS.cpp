#pragma once

#include "stdafx.h"
#include "Def_globali.h"
#include "Globals.h"

//#ifdef BECKHOFF  


  extern UINT            lng;  // Lingua attuale
  extern wchar_t         PC_name[30]; // Computer name read from system configuration
  extern CSplash_screen* info_wnd;
  extern UINT            trigger_label[NUMTRIGGER];
  extern CImage_layout*  layout_dsp;

  long					  nErr;
  long					  nPort;
  unsigned short	       nAdsState;
  USHORT					       nDeviceState;
  char              	   pDevName[50];
  AdsVersion     	       Version;
  char*           	     ADS_text;
	CString                ADS_error; 
	int                    ADS_status = 0; // 0=com inattiva / 1=com attiva
  AmsAddr				         Addr;

  extern int             refresh_delay;


  extern CParameter*     f_hvloop_a;
  //Pour added 20180302
  extern CParameter*     f_hvloop_b;
  extern CParameter*     f_hvloop_c;
  extern CParameter*     f_hvloop_d;


  extern CMenu_button*   calibration_menu;
  extern char            serial_number[20+1];

  extern CMenu_button*   hv_menu;

  #ifdef READ_CYCLES
    extern ADS_data_type          ADS_cycles; // ADS variable connected to cycles
    extern unsigned int           cycles;     // cycles read from PLC
    extern CTimer                 globaltimer;
    extern unsigned int           cycles_0;   // initial value
  #endif



extern update_data_type        uplist[MAX_UPLIST_NUM];
extern UINT                    uplist_num;


ADS_data_type           hv_voltin[4];
ADS_data_type           hv_currin[4];

ADS_data_type           hv_ndata [4];
ADS_data_type           hv_vmin  [4];
ADS_data_type           hv_vmax  [4];
ADS_data_type           hv_error [4];
ADS_data_type           hv_errortot;
ADS_data_type           hvblock4_ptr;


//Pour added 20180319
 ADS_data_type           hv_manager_ptr1;
 ADS_data_type           hv_manager_ptr2;
 ADS_data_type           hv_manager_ptr3;
 ADS_data_type           hv_manager_ptr4;
//Pour added 20180319
 ADS_data_type           num_sampling;


//Pour marked 20180303 A20L
CParameter* hvmax_err = NULL;

//Pour added 20180303
CParameter* hv1min_err = NULL;
CParameter* hv1max_err = NULL;
CParameter* hv2min_err = NULL;
CParameter* hv2max_err = NULL;
CParameter* hv3min_err = NULL;
CParameter* hv3max_err = NULL;
CParameter* hv4min_err = NULL;
CParameter* hv4max_err = NULL;

short vmin [4];
short vmax [4];
short error[4];
short error_tot;
ADS_data_type          hvdata_param;
BYTE                   block4_ptr; 
BYTE                   block4_old;

//Pour added 20180322
unsigned short         block4_ptr1;
unsigned short         block4_ptr2;
unsigned short         block4_ptr3;
unsigned short         block4_ptr4;

unsigned int mark = 0; //flag for HV data record
//Pour added 20180322


CFile                  HV_data;
extern BOOL            hvdatarec_on;

//extern Results_manager* hvdata_result;

ADS_data_type           carousel_phase;
ADS_data_type           encoder_height;
ADS_data_type           input_width;
ADS_data_type           input_phase;
ADS_data_type           outgood_phase;
ADS_data_type           outrej1_phase;
ADS_data_type           outrej2_phase;

ADS_data_type           prod_speed;

#ifdef HGA_LEAKTEST
  ADS_data_type           hga_input;
  ADS_data_type           hga_value;
#endif



#ifdef HGA_DATAREC

  // Variabili relative alla registrazione callback
  ULONG                  hNotification_id, hUser_id; 
  AdsNotificationAttrib  adsNotificationAttrib_id;
  //extern Results_manager* input_result;
  BOOL   first_hgadata = TRUE;
  //CFile  hga_file;
  CStdioFile hga_file;
  ADS_data_type          hgadata_param;

	#pragma pack(1)
	struct hgadata_type
	{
    UINT         label;
    SHORT        hgavalue;
	float        percent;
	} hga_data;
	#pragma pack(8) // restore previous (check: should be 8!)



void _stdcall ADS_hgadata_callback(AmsAddr *Addr, AdsNotificationHeader* pNotification, ULONG hUser)
{
  // TRACE( L"Callback called: hUser=%d - hNotification=%d - %d bytes \n", hUser, pNotification->hNotification, pNotification->cbSampleSize );

  if ( first_hgadata )
  {
    first_hgadata = FALSE;
    return;
  } 

  // TRACE("ADS_hgadata_callback chiamata\n");
  // TRACE( "%d / %d\n", pNotification->cbSampleSize, sizeof(hgadata_type) );

  ASSERT( pNotification->cbSampleSize == sizeof(hgadata_type) );
  CopyMemory( &hga_data, pNotification->data, pNotification->cbSampleSize );

  TRACE( "ADS_hgadata_callback chiamata - Label=%d - HGAVAL=%d\n", hga_data.label, hga_data.hgavalue );

  //Pour added on 20201013

  //Pour added on 20201013

  if ( hga_file != CFile::hFileNull )
  {
    char txt[100];
    sprintf( txt, "%d,%d, %2.1f\n", hga_data.label, hga_data.hgavalue, hga_data.percent );
    hga_file.Write( txt, strlen(txt) );
  }

 /* if ( input_result )
  {
    ASSERT( pNotification->cbSampleSize == sizeof(input_data) );

    CopyMemory( &input_data, pNotification->data, pNotification->cbSampleSize );

    input_result->InsertI( input_data.label,                 0 );
    input_result->InsertI( input_data.camcnt,                1 );
    input_result->InsertI( int(0.5+input_data.ldpos*100.0),  2 );
    input_result->InsertI( input_data.tcpos,                 3 );
  } */


}



// Registra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_hgadata_regcallback( void )
{
 // Specify attributes of the notification
  adsNotificationAttrib_id.cbLength   = sizeof(hga_data);
  adsNotificationAttrib_id.nTransMode = ADSTRANS_SERVERONCHA;
  adsNotificationAttrib_id.nMaxDelay  = 500000; // 50ms (unit=0.1us)
  adsNotificationAttrib_id.nCycleTime = 500000; // 50ms (unit=0.1us)

  // 32-bit variable (including pointers) that are passed to the callback function
  hUser_id = 21000; 

  first_hgadata = TRUE; // avoid savig first set of data

  if ( ADS_init_parameter( "TRIGGER_HGA.HGADATA", &hgadata_param, sizeof(hga_data) ) )
	{
		delete hgadata_param.val;
		return 80;
	}

  // Start transmission of the PLC variables 
	nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, hgadata_param.handle, &adsNotificationAttrib_id, ADS_hgadata_callback, hUser_id, &hNotification_id );
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to register callback for input data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}

	ADS_read_parameter( &hgadata_param );
  ZeroMemory( &hga_data, sizeof(hga_data) );

  // Prepare output file

  CString name;
  CTime time = CTime::GetCurrentTime();   
  CFileException ex;

  ASSERT( hga_file.m_hFile == INVALID_HANDLE_VALUE );

  name.Format(  L"%s\\%s_%04d%02d%02d_%02d%02d%02d.csv", MAIN_DIRECTORY, L"HGADATA", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

  if ( !hga_file.Open( name, CFile::modeReadWrite|CFile::modeCreate, &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
		return 99; // Errore!
	}


  TRACE( L"ADS_hgadata_regcallback executed\n" );

  return( 0 );
}




// Deregistra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_hgadata_deregcallback( void )
{
	//if ( hgadata_param.val ) free( hgadata_param.val );
  ADS_clean_parameter( &hgadata_param );

	// Finish transmission of the PLC variables 
  nErr = AdsSyncDelDeviceNotificationReq( &Addr, hNotification_id);
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to de-register callback for hgadata: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}

  if ( hga_file.m_hFile != CFile::hFileNull )
  {
    hga_file.Close();
  }

  ASSERT(  hga_file.m_hFile == CFile::hFileNull );

  TRACE( L"ADS_hgadata_deregcallback executed\n" );

	return( 0 );
}


#endif // HGA_DATAREC





#ifdef SAVE_INPUTDATA

  // Variabili relative alla registrazione callback
  ULONG                  hNotification_id, hUser_id; 
  AdsNotificationAttrib  adsNotificationAttrib_id;
  ADS_data_type          inpdata_param;

	#pragma pack(1)
	struct inputdata_type
	{
    INT          label;
    INT          camcnt;
    double       ldpos;
    BYTE         tcpos;
	} input_data;
	#pragma pack(8) // restore previous (check: should be 8!)


extern Results_manager* input_result;


BOOL   first_inputdata = TRUE;


void _stdcall ADS_inpdata_callback(AmsAddr *Addr, AdsNotificationHeader* pNotification, ULONG hUser)
{
  // TRACE( L"Callback called: hUser=%d - hNotification=%d - %d bytes \n", hUser, pNotification->hNotification, pNotification->cbSampleSize );

  if ( first_inputdata )
  {
    first_inputdata = FALSE;
    return;
  }

  if ( input_result )
  {
    ASSERT( pNotification->cbSampleSize == sizeof(input_data) );

    CopyMemory( &input_data, pNotification->data, pNotification->cbSampleSize );

    input_result->InsertI( input_data.label,                 0 );
    input_result->InsertI( input_data.camcnt,                1 );
    input_result->InsertI( int(0.5+input_data.ldpos*100.0),  2 );
    input_result->InsertI( input_data.tcpos,                 3 );
  }


}



// Registra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_inputdata_regcallback( void )
{
 // Specify attributes of the notification
  adsNotificationAttrib_id.cbLength   = sizeof(input_data);
  adsNotificationAttrib_id.nTransMode = ADSTRANS_SERVERONCHA;
  adsNotificationAttrib_id.nMaxDelay  = 500000; // 50ms (unit=0.1us)
  adsNotificationAttrib_id.nCycleTime = 500000; // 50ms (unit=0.1us)

  // 32-bit variable (including pointers) that are passed to the callback function
  hUser_id = 20000; 

  first_inputdata = TRUE; // avoid savig first set of data

  if ( ADS_init_parameter( "PRODUCT_FLOW.INPDATA", &inpdata_param, sizeof(input_data) ) )
	{
		delete inpdata_param.val;
		return 80;
	}

  // Start transmission of the PLC variables 
	nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, inpdata_param.handle, &adsNotificationAttrib_id, ADS_inpdata_callback, hUser_id, &hNotification_id );
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to register callback for input data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}

	ADS_read_parameter( &inpdata_param );

  ZeroMemory( &input_data, sizeof(input_data) );



/*  TRACE("TRIGGER - INITIAL VALUE: ");
	for ( int i=0 ; i<NUMTRIGGER ; i++ ) TRACE( "%d - ", trigger_label[i] );
  TRACE("\n"); */


//	trigger_label = (UINT*) trigger_param.val;
//	CopyMemory( trigger_label_old, trigger_label, 24 );
	//delete param.val;

  return( 0 );
}




// Deregistra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_inputdata_deregcallback( void )
{
	if ( inpdata_param.val ) free( inpdata_param.val );

	// Finish transmission of the PLC variables 
  nErr = AdsSyncDelDeviceNotificationReq( &Addr, hNotification_id);
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to de-register callback for inputdata: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}

	return( 0 );
}


#endif // SAVE_INPUTDATA
/*
#pragma pack(1)
struct hvdata_type
{
	unsigned int  label;
	short HV1_V_Min;
	short HV1_V_Max;
	short HV2_V_Min;
	short HV2_V_Max;
	short HV3_V_Min;
	short HV3_V_Max;
	short HV4_V_Min;
	short HV4_V_Max;
}hv_data;
#pragma pack(8)//
*/

//Pour changed 20180224
#ifdef HV_DATAREC

//#define HV_MANAGER_PTR4 1024   //Pour marked 20180915 1024 for TIVS-A40L/TIVS-A60L

#define HV_MANAGER_PTR4 128 //Pour marked 20180915 128 for TIVS-A20L

// Variabili relative alla registrazione callback
ULONG                  hNotification_id, hUser_id; 
AdsNotificationAttrib  adsNotificationAttrib_id;

BOOL   first_hvdata = TRUE;


//Pour added 20180412d
#pragma pack(1)
struct hvdata_type
{
	unsigned int  label;
	short HV1_V_Min;
	short HV1_V_Max;
	short HV2_V_Min;
	short HV2_V_Max;
	short HV3_V_Min;
	short HV3_V_Max;
	short HV4_V_Min;
	short HV4_V_Max;
}hv_data;
#pragma pack(8)//


void _stdcall ADS_HVdata_callback(AmsAddr *Addr, AdsNotificationHeader* pNotification, ULONG hUser)
{
  //TRACE( L"Callback called: hUser=%d - hNotification=%d - %d bytes \n", hUser, pNotification->hNotification, pNotification->cbSampleSize );
	int i;
	unsigned int label_old = 0;
	unsigned int index     = 0;
	if ( first_hvdata )
	{
    first_hvdata = FALSE;
    return;
	}

	//Pour added 20180409
  	//Pour added for testing 20180409
		if ( hvdatarec_on && HV_data.m_hFile )
		{
		  hvdata_type hvdata_p[HV_MANAGER_PTR4];
		  hvdata_p[0].label = *(UINT*)pNotification->data;
		  CopyMemory(hvdata_p,pNotification->data, HV_MANAGER_PTR4*sizeof(hvdata_type));

		  label_old = hvdata_p[0].label;

		  for(i=1; i<HV_MANAGER_PTR4-1; i++)
			{
			  if(label_old < hvdata_p[i].label )
			  {
				  if( hvdata_p[i].HV4_V_Min != 0 && hvdata_p[i].HV4_V_Max != 0 )
				  index = i;
				  label_old = hvdata_p[i].label; 
			  }

				//if(index > mark || label_old > (10000+mark))//Pour changed for testing 20180918
				if( label_old > (10000+mark))
				{
				char txt[200];
			  				//sprintf( txt, "PTR -  MIN/MAX    -    MIN/MAX     -    MIN/MAX    -    MIN/MAX   \r\n" );
				sprintf( txt, "%2d - %4.2lf/%4.2lf - %4.2lf/%4.2lf -  %4.2lf/%4.2lf - %4.2lf/%4.2lf\r\n", 
				hvdata_p[index].label, INT2VDSP(hvdata_p[index].HV1_V_Min), INT2VDSP(hvdata_p[index].HV1_V_Max), 
									   INT2VDSP(hvdata_p[index].HV2_V_Min), INT2VDSP(hvdata_p[index].HV2_V_Max), 
								       INT2VDSP(hvdata_p[index].HV3_V_Min), INT2VDSP(hvdata_p[index].HV3_V_Max), 
								       INT2VDSP(hvdata_p[index].HV4_V_Min), INT2VDSP(hvdata_p[index].HV4_V_Max) );
								       HV_data.Write( txt, strlen(txt) );
				mark++;//after printing every row,mark++
				}
			}
		}
	//Pour added for testing 20180409

}

/*
// Registra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_HVdata_regcallback( void )
{
 // Specify attributes of the notification
  //adsNotificationAttrib_id.cbLength   = sizeof(hv_manager_ptr1);
  adsNotificationAttrib_id.cbLength   = sizeof(hv_manager_ptr4);
  adsNotificationAttrib_id.nTransMode = ADSTRANS_SERVERONCHA;
  adsNotificationAttrib_id.nMaxDelay  = 500000; // 50ms (unit=0.1us)
  adsNotificationAttrib_id.nCycleTime = 500000; // 50ms (unit=0.1us)

  // 32-bit variable (including pointers) that are passed to the callback function
  hUser_id = 20200; 
  //hUser_id = 20020; 

  first_hvdata = TRUE; // avoid savig first set of data

  // Start transmission of the PLC variables

	nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, hvdata_param.handle, &adsNotificationAttrib_id, ADS_HVdata_callback, hUser_id, &hNotification_id );
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to register callback for HV data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}
	//Pour added 20180412
	ADS_read_hvdata_array( );
	//Pour added 20180412

	//ADS_read_parameter(&hvdata_param);
	//ZeroMemory(&hv_data,sizeof(hv_data));

  TRACE("ADS_HVdata_regcallback completed succesfully\n");

  return( 0 );
}
*/

//int ADS_read_hvdata_array( void )
int ADS_HVdata_regcallback( void )
{
unsigned long    lHdlVar2;
int              nIndex;
struct           hvdata_type hvdata[HV_MANAGER_PTR4];
char			 szVar2[]={"PRODUCT_FLOW.HVLD_DATA"};

	//Specify attributes of the notification
	adsNotificationAttrib_id.cbLength   = sizeof(hvdata);
	adsNotificationAttrib_id.nTransMode = ADSTRANS_SERVERONCHA;
	adsNotificationAttrib_id.nMaxDelay  = 500000; // 50ms (unit=0.1us)
	adsNotificationAttrib_id.nCycleTime = 500000; // 50ms (unit=0.1us)

	// 32-bit variable (including pointers) that are passed to the callback function
	hUser_id = 20200; 
	//hUser_id = 20020; 

  if ( ADS_init_parameter( "PRODUCT_FLOW.HVLD_DATA", &hvdata_param, sizeof(hvdata) ) )
	{
		delete hvdata_param.val;
		return 80;
	}

	nErr = AdsSyncReadWriteReq(&Addr, ADSIGRP_SYM_HNDBYNAME,0x0,sizeof(lHdlVar2),&lHdlVar2,sizeof(szVar2),szVar2);
	if( nErr )
	{
		swprintf( msg_err, 200, L"Unable to get handle of HV data array: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id);
		return( nErr );
	}
	nErr = AdsSyncReadReq(&Addr, ADSIGRP_SYM_VALBYHND, lHdlVar2, sizeof(hvdata), &hvdata[0]);
	if( nErr )
	{
		swprintf( msg_err, 200, L"Unable to get data of HV data array: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id);
		return( nErr );
	}

		nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, lHdlVar2, &adsNotificationAttrib_id, ADS_HVdata_callback, hUser_id, &hNotification_id );
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to register callback for HV data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}

	ZeroMemory( &hv_data, sizeof(hv_data) );

	return( 0 );
}
//Pour added 20180412



// Deregistra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_HVdata_deregcallback( void )
{
	// Finish transmission of the PLC variables 
  nErr = AdsSyncDelDeviceNotificationReq( &Addr, hNotification_id);
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to de-register callback for HV data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
		return( nErr );
	}
	
	return( 0 );
}

#endif//HV_DATAREC
//Pour changed 20180224

////Pour changed 20201021
//#ifdef STATUS_MONITORING
//
//// Variabili relative alla registrazione callback
//ULONG                  hNotification_id, hUser_id; 
//AdsNotificationAttrib  adsNotificationAttrib_id;
//
//
//void _stdcall ADS_status_callback(AmsAddr *Addr, AdsNotificationHeader* pNotification, ULONG hUser)
//{
//  //TRACE( L"Callback called: hUser=%d - hNotification=%d - %d bytes \n", hUser, pNotification->hNotification, pNotification->cbSampleSize );
//	int i;
//	unsigned int label_old = 0;
//	unsigned int index     = 0;
//	if ( first_hvdata )
//	{
//    first_hvdata = FALSE;
//    return;
//	}
//
//		if ( hvdatarec_on && HV_data.m_hFile )
//		{
//
//		}
//
//}
//
//
//int ADS_status_regcallback( void )
//{
//unsigned long    lHdlVar2;
//int              nIndex;
//struct           hvdata_type hvdata[HV_MANAGER_PTR4];
//char			 szVar2[]={"PRODUCT_FLOW.XXXXX"};
//
//	//Specify attributes of the notification
//	adsNotificationAttrib_id.cbLength   = sizeof(hvdata);
//	adsNotificationAttrib_id.nTransMode = ADSTRANS_SERVERONCHA;
//	adsNotificationAttrib_id.nMaxDelay  = 500000; // 50ms (unit=0.1us)
//	adsNotificationAttrib_id.nCycleTime = 500000; // 50ms (unit=0.1us)
//
//	// 32-bit variable (including pointers) that are passed to the callback function
//	hUser_id = 20200; 
//	//hUser_id = 20020; 
//
//  if ( ADS_init_parameter( "PRODUCT_FLOW.XXXXX", &hvdata_param, sizeof(hvdata) ) )
//	{
//		delete hvdata_param.val;
//		return 80;
//	}
//
//	nErr = AdsSyncReadWriteReq(&Addr, ADSIGRP_SYM_HNDBYNAME,0x0,sizeof(lHdlVar2),&lHdlVar2,sizeof(szVar2),szVar2);
//	if( nErr )
//	{
//		swprintf( msg_err, 200, L"Unable to get handle of Status: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id);
//		return( nErr );
//	}
//	nErr = AdsSyncReadReq(&Addr, ADSIGRP_SYM_VALBYHND, lHdlVar2, sizeof(hvdata), &hvdata[0]);
//	if( nErr )
//	{
//		swprintf( msg_err, 200, L"Unable to get data of Status: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id);
//		return( nErr );
//	}
//
//		nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, lHdlVar2, &adsNotificationAttrib_id, ADS_HVdata_callback, hUser_id, &hNotification_id );
//	if ( nErr )
//	{
//		swprintf( msg_err, 200, L"Unable to register callback for Status: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
//		return( nErr );
//	}
//
//	ZeroMemory( &hv_data, sizeof(hv_data) );
//
//	return( 0 );
//}
////Pour added 20180412
//
//
//
//// Deregistra la chiamata della callback su qualunqur variazione dei trigger
//// se tutto Ok ritorna 0
//int ADS_status_deregcallback( void )
//{
//	// Finish transmission of the PLC variables 
//  nErr = AdsSyncDelDeviceNotificationReq( &Addr, hNotification_id);
//	if ( nErr )
//	{
//		swprintf( msg_err, 200, L"Unable to de-register callback for HV data: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification_id );
//		return( nErr );
//	}
//	
//	return( 0 );
//}
//
//#endif//HV_DATAREC
////Pour changed 20201021

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variabili relative alla registrazione callback
ULONG                  hNotification, hUser; 
AdsNotificationAttrib  adsNotificationAttrib;
ADS_data_type          trigger_param;



void _stdcall ADS_callback(AmsAddr *Addr, AdsNotificationHeader* pNotification, ULONG hUser)
{
  // TRACE( L"Callback called: hUser=%d - hNotification=%d - %d bytes \n", hUser, pNotification->hNotification, pNotification->cbSampleSize );
  CopyMemory( trigger_label, pNotification->data, pNotification->cbSampleSize );

  /*
  TRACE("TRIGGER: ");
	for ( int i=0 ; i<NUMTRIGGER ; i++ ) TRACE( "%d - ", trigger_label[i] );
  TRACE("\n");
  */


/*  int                     nIndex; 
  static ULONG            nCount = 0; 
  SYSTEMTIME              SystemTime, LocalTime; 
  FILETIME                FileTime; 
  LARGE_INTEGER           LargeInteger; 
  TIME_ZONE_INFORMATION   TimeZoneInformation; 

  cout << ++nCount << ". Call:\n"; 

  // Output value of the variable
  cout << "Value: " << *(ULONG *)pNotification->data << '\n'; 
  cout << "Notification: " << pNotification->hNotification << '\n';

  // Convert time stamp to SYSTEMTIME
  LargeInteger.QuadPart = pNotification->nTimeStamp;
  FileTime.dwLowDateTime = (DWORD)LargeInteger.LowPart;
  FileTime.dwHighDateTime = (DWORD)LargeInteger.HighPart;
  FileTimeToSystemTime(&FileTime, &SystemTime);

  // Calculate time in local representation
  GetTimeZoneInformation(&TimeZoneInformation);
  SystemTimeToTzSpecificLocalTime(&TimeZoneInformation, &SystemTime, &LocalTime);

  // Output time stamp
  cout << LocalTime.wHour << ":" << LocalTime.wMinute << ":" << LocalTime.wSecond << '.' << LocalTime.wMilliseconds << 
       " den: " << LocalTime.wDay << '.' << LocalTime.wMonth << '.' << LocalTime.wYear << '\n';

  // Size of buffer in bytes
  cout << "SampleSize: " << pNotification->cbSampleSize << '\n';
 
  // 32-bit variable (including pointers) that are set by AddNotification // (See main)
  cout << "hUser: " << hUser << '\n';

  // Output ADS address of the sender
  cout << "ServerNetId: ";
  for (nIndex = 0; nIndex < 6; nIndex++)
    cout << (int)pAddr->netId.b[nIndex] << ".";
  cout << "\nPort: " << pAddr->port << "\n\n";
  cout.flush(); */
}



// Registra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_register_trigger_callback( void )
{
 // Specify attributes of the notification
  adsNotificationAttrib.cbLength   = sizeof(trigger_label);
  adsNotificationAttrib.nTransMode = ADSTRANS_SERVERONCHA;
//  adsNotificationAttrib.nMaxDelay  = 100000; // 10ms (unit=0.1us)
//  adsNotificationAttrib.nCycleTime = 100000; // 10ms (unit=0.1us)
  adsNotificationAttrib.nMaxDelay  = 10000; // 1ms (unit=0.1us)
  adsNotificationAttrib.nCycleTime = 10000; // 1ms (unit=0.1us)

  // 32-bit variable (including pointers) that are passed to the callback function
  hUser = 123456; 

  if ( ADS_init_parameter( "PRODUCT_FLOW.TRIGGER_LABEL", &trigger_param, sizeof(trigger_label) ) )
	{
  	//Message_box("Error in getting handle of <trigger_PCx_label>", MBOX_ERROR );
		delete trigger_param.val;
		return 80;
	}

  // Start transmission of the PLC variables 
	nErr = AdsSyncAddDeviceNotificationReq( &Addr, ADSIGRP_SYM_VALBYHND, trigger_param.handle, &adsNotificationAttrib, ADS_callback, hUser, &hNotification );
	if ( nErr )
	{
		swprintf( msg_err, 200, L"Unable to register callback for trigger: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification );
		return( nErr );
	}

	ADS_read_parameter( &trigger_param );

  ZeroMemory( trigger_label, sizeof(trigger_label) );

  TRACE("TRIGGER - INITIAL VALUE: ");
	for ( int i=0 ; i<NUMTRIGGER ; i++ ) TRACE( "%d - ", trigger_label[i] );
  TRACE("\n");


//	trigger_label = (UINT*) trigger_param.val;
//	CopyMemory( trigger_label_old, trigger_label, 24 );
	//delete param.val;

  return( 0 );
}




// Deregistra la chiamata della callback su qualunqur variazione dei trigger
// se tutto Ok ritorna 0
int ADS_deregister_trigger_callback( void )
{
	//delete trigger_param.val;

	if ( trigger_param.val ) free( trigger_param.val );

	// Finish transmission of the PLC variables 
  nErr = AdsSyncDelDeviceNotificationReq( &Addr, hNotification);
	if ( nErr )
	{
		//msg_err.Format("Unable to de-register callback for triggers: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification );
//		msg_err.Format( L"Unable to de-register callback for triggers: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification );
		swprintf( msg_err, 200, L"Unable to de-register callback for triggers: error n.%d <%s>\r\nNotification = %d", nErr, ADS_error_desc(nErr), hNotification );
		return( nErr );
	}

	return( 0 );
}

// NON SI RIESCE AD USARE LA MESSAGEBOX, QUINDI USO IL VALORE DI RITORNO
//  0 => OK
// 99 => IMPOSSIBILE STABILIRE COMM CON PLC

int ADS_init( void )
{

//	Message_box( "ADS_init" );
//	AfxMessageBox( "Ads_init" , MB_OK );
//	return( 100 );


	// Lettura versione DLL
	long nTemp              = AdsGetDllVersion();
	AdsVersion* pDLLVersion = (AdsVersion *) &nTemp;
	//msg.Format( "ADS DLL: Version=%d - Revision=%d - Build=%d\n", int(pDLLVersion->version), int(pDLLVersion->revision), int(pDLLVersion->build) );
	msg.Format( L"ADS DLL: Version=%d - Revision=%d - Build=%d\n", int(pDLLVersion->version), int(pDLLVersion->revision), int(pDLLVersion->build) );
	TRACE( msg );


	// Open communication port for local PLC (Run-time system 1)
	nPort = AdsPortOpen();
	//if ( nPort ) msg.Format( "Apertura della porta.. Ok. Porta n°%d", nPort );
	if ( nPort ) msg.Format( L"Port opening n°%d OK", nPort );
	else
	{
//		msg_err.Format( "Impossibile aprire porta. Porta n°%d", nPort );
//		Message_box( msg_err );
		//ADS_error.Format( "Impossibile aprire porta. Porta n°%d", nPort );
		ADS_error.Format( L"Unable to open port n°%d", nPort );
		return 99;
	}
	//if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
	if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowText( msg );

	/*  LETTURA INDIRIZZO LOCALE
	nErr = AdsGetLocalAddress( &Addr );
	if ( nErr )
	{
		msg.Format( "ERRORE n°%d: Impossibile ottenere indirizzo locale", nErr );
		Message_box( msg_err );
		return 10010;
	}
	msg.Format( "Rilevato indirizzo locale %d.%d.%d.%d.%d.%d porta %d",
	Addr.netId.b[0], Addr.netId.b[1], Addr.netId.b[2], Addr.netId.b[3], Addr.netId.b[4], Addr.netId.b[5], Addr.port );
	if ( info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
	*/

 	// ASSEGNAZIONE INDIRIZZO E PORTA DEL SISTEMA REMOTO
	Addr.netId.b[0] =   5;
	Addr.netId.b[1] =  19;
	Addr.netId.b[2] =  40;
	Addr.netId.b[3] =  80;
	Addr.netId.b[4] =   1;
	Addr.netId.b[5] =   1;

  ASSERT( PLC_ADDRESS && AfxIsValidAddress(PLC_ADDRESS,6) );
  CopyMemory( Addr.netId.b, PLC_ADDRESS, 6 );

  /*
	char str[100];
	if ( GetPrivateProfileSection( "PLC ADDRESS", str, sizeof(str), INI_FILE ) )
	{
		if ( !strncmp( "AMS Net Id=", str, 11 ) )
		{
			int val[6];
			int ival = 0;
			int temp = 0;
			UINT ind = 11; // Indice da cui iniziare la scansione
			while ( ind<strlen(str) && ival<6 )
			{
				if ( ( str[ind]<'0' || str[ind]>'9' ) && str[ind]!='.' ) break;

				if ( str[ind]>='0' && str[ind]<='9' )
				{
					temp = temp*10 + str[ind]-'0';
					ind++;
				}
				if ( str[ind]=='.' || ( ival==5 && ( str[ind]=='\0' || str[ind]==' ' ) ) )
				{
					if ( temp>=0 && temp<256 ) val[ival] = temp;
					else break;
					temp      = 0;
					ind++;
					ival++;
				}
			}

			if ( ival==6 )
			{
      	// ASSEGNAZIONE INDIRIZZO E PORTA DEL SISTEMA REMOTO
	      Addr.netId.b[0] = val[0];
       	Addr.netId.b[1] = val[1];
	      Addr.netId.b[2] = val[2];
       	Addr.netId.b[3] = val[3];
	      Addr.netId.b[4] = val[4];
       	Addr.netId.b[5] = val[5];

				TRACE("Indirizzo AMS Net Id %d.%d.%d.%d.%d.%d letto e assegnato per la comunicazione\n", val[0], val[1], val[2], val[3], val[4], val[5] );
			}
			else
			{
//  		  msg_err.Format( "Impossibile leggere correttamente l'indirizzo dalla stringa \"AMS Net Id\" nel file ini" );
//	  	  Message_box( msg_err );
  		  ADS_error.Format( "Impossibile leggere correttamente l'indirizzo dalla stringa \"AMS Net Id\" nel file ini" );
				return( 99 );
			}
		}
		else
		{
//		  msg_err.Format( "Errore di formattazione della stringa \"AMS Net Id\" nel file ini" );
//		  Message_box( msg_err );
 		  ADS_error.Format( "Errore di formattazione della stringa \"AMS Net Id\" nel file ini" );
			return( 99 );
		}
	}
	else
	{
//		msg_err.Format( "Impossibile leggere valore dal registro: errore n.%d", GetLastError() );
//		Message_box( msg_err );
	  ADS_error.Format( "Impossibile leggere valore dal registro: errore n.%d", GetLastError() );
		return( 99 );
	}


//	exit(0);
  */



	Addr.port = AMSPORT_R0_PLC_RTS1;

	//msg.Format( "Nuovo indirizzo assegnato: %d.%d.%d.%d.%d.%d porta %d",
	msg.Format( L"New assigned address: %d.%d.%d.%d.%d.%d port n.%d",
	Addr.netId.b[0], Addr.netId.b[1], Addr.netId.b[2], Addr.netId.b[3], Addr.netId.b[4], Addr.netId.b[5], Addr.port );
	//if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
	if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowText( msg );

  AdsSyncSetTimeout( 500 );

/*
	int i;
	GetKeyboardState( keyb );
	TRACE("Stato tastiera: ");
	for ( i=0 ; i<255 ; i++ ) TRACE("%02X=%02X ", i, keyb[i] );
	TRACE("\n");
*/

	// LETTURA DELLO STATO ATTUALE
  SHORT   esc   = 0;
//  int     wait  = 0;
//  char progress[] = { '\\' , '|' , '/' , '-' };
	CTimer timer_loc;

  do
  {
		nErr = AdsSyncReadStateReq( &Addr, &nAdsState, &nDeviceState);
		if ( nErr )
    {
//      Error_code_desc( nErr );
//			msg.Format( "ERROR n°%d: Unable to read ADS status\r\n<%s>", nErr, err_desc );
			//msg.Format( "Unable to read ADS status\r\nError %d", nErr );
			//msg.Format( L"Unable to read ADS status\r\nError %d", nErr );

//			esc = GetAsyncKeyState(VK_F1);
//			msg.Format("Waiting to establish communication with PLC\r\nPress F1 to abort.. (%d s)", timer_loc.elapsed_ms()/1000  /* timer_loc.elapsed()*/ );
			//msg.Format("Waiting to establish communication with PLC\r\nPress F1 to abort.. (%1.0lf seconds)", timer_loc.elapsed() );
			//msg.Format( L"Waiting to establish communication with PLC - F1 to abort.. %.0lf\"", timer_loc.elapsed() );
//    	if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
    	
      msg.Format( string_178[lng], timer_loc.elapsed() );
      if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowText( msg );
//			TRACE( "%s\n", msg );
      Sleep( 1000 );
    }
  } while ( nErr && !F1_PUSHED && timer_loc.elapsed()<60.0 );



/*	BYTE keyb[256];
	GetKeyboardState( keyb );
	TRACE("Stato key[27]=%02X\n", keyb[VK_ESCAPE]);
  keyb[VK_ESCAPE] = 0;
	SetKeyboardState( keyb );

	GetKeyboardState( keyb );
	TRACE("Stato key[27]=%02X\n", keyb[VK_ESCAPE]); */

/*
	TRACE("Stato tastiera: ");
	for ( i=0 ; i<255 ; i++ ) TRACE("%02X=%02X ", i, keyb[i] );
	TRACE("\n");
	keyb[27]=0;
	SetKeyboardState( keyb );
*/


  if ( nErr || timer_loc.elapsed()>=60.0 || esc )
  {
		//TRACE("Ricerca connessione con PLC impossibile/interrotta\n");
		TRACE( L"Connection to PLC impossible/interrupted\n");
//    MessageBox(NULL, "UNABLE TO DETECT PLC RUNNING" , "ERROR:", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
//    msg_err.Format( "UNABLE TO DETECT/CONNECT WITH PLC RUNNING" );
//		Message_box( msg_err );
    //ADS_error.Format( "UNABLE TO DETECT/CONNECT WITH PLC RUNNING" );
    ADS_error.Format( L"UNABLE TO DETECT/CONNECT WITH PLC RUNNING" );
		return 99;
  }

//  Ads_state_desc( nAdsState );
  //msg.Format( "ADS status = %d - Device status = %d", nAdsState, nDeviceState );
  msg.Format( L"ADS status = %d - Device status = %d", nAdsState, nDeviceState );
	//if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
	if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowText( msg );

  if ( nAdsState == 11001 )
  {
//    msg_err.Format( "Unable to find PLC/ADS system" );
//   	if ( info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
//		Message_box( CString("UNABLE TO DETECT PLC RUNNING") );
//    MessageBox( NULL , msg.c_str() , "ERROR:" , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
    //ADS_error.Format( "Unable to find PLC/ADS system" );
    ADS_error.Format( L"Unable to find PLC/ADS system" );
		return 99;
  }


  if ( nAdsState != ADSSTATE_RUN	) //&& nDeviceState != ADSSTATE_STOP )
  {
//		if ( MessageBox( NULL, "PLC PROGRAM NOT IN RUN STATUS\r\nWOULD YOU LIKE TO START IT?" , "WARNING:" , MB_YESNO|MB_ICONWARNING|MB_SYSTEMMODAL ) == IDYES )
		//if ( Message_box( "PLC PROGRAM NOT IN RUN STATUS\r\nWOULD YOU LIKE TO START IT?", MBOX_YESNO ) == IDYES )
		if ( Message_box( L"PLC PROGRAM NOT IN RUN STATUS\r\nWOULD YOU LIKE TO START IT?", MBOX_YESNO ) == IDYES )
    {
      nErr = AdsSyncWriteControlReq ( &Addr, ADSSTATE_RUN, nDeviceState, 0, NULL );
      if ( nErr )
      {
//        Error_code_desc( nErr );
//				msg_err.Format( "Unable to start run of PLC program\r\nError n.%d", nErr );
//  			Message_box( msg_err );
//        MessageBox( NULL , msg.c_str() , "ERROR:" , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
				//ADS_error.Format( "Unable to start run of PLC program\r\nError n.%d", nErr );
				ADS_error.Format( L"Unable to start run of PLC program\r\nError n.%d", nErr );
				return 99;
      }
    }
	}

	// LETTURA INFORMAZIONI
	nErr = AdsSyncReadDeviceInfoReq( &Addr, pDevName, &Version);
	if ( nErr )
  {
//    Error_code_desc( nErr );
//    msg_err.Format( "ERRORE n°%d: Impossibile leggere informazioni ADS\r\n<%s>", nErr );
//  	Message_box( msg_err );
//    MessageBox( NULL, msg.c_str(), "ERROR:", MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );

		//ADS_error.Format( "ERRORE n°%d: Impossibile leggere informazioni ADS\r\n<%s>", nErr );
		ADS_error.Format( L"ERRORE n°%d: Impossibile leggere informazioni ADS\r\n<%s>", nErr );
		return( 99 );
  }
	else 
	{
		//msg.Format( "<%s> - Versione %d.%d - Build %d", pDevName, Version.version, Version.revision, Version.build );
		//msg.Format( L"<%s> - Versione %d.%d - Build %d", pDevName, Version.version, Version.revision, Version.build );
		msg.Format( L"Version %d.%d - Build %d", Version.version, Version.revision, Version.build );
  	//if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowTextA( msg );
  	if ( info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible() ) info_wnd->GetDlgItem( IDC_TEXT_2 )->SetWindowText( msg );
	}

//  status = 1; // Initialization completed, read all parameters

  // Upon showing, try to read all PLC parameters
/*  if ( Read_PLC_parameters() )
  {
    MessageBox( NULL , msg.c_str() , "ERROR:" , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
  }
  else status = 9999; // */






//	TRACE("Inizializzazione comunicazione ADS completata correttamente\n");
	TRACE( L"ADS communication completed successfully\n");
  return( 0 );
}





int ADS_terminate( void )
{

  #ifdef READ_CYCLES
    ADS_clean_parameter( &ADS_cycles );
  #endif

  ADS_deregister_trigger_callback();


  int nErr = AdsPortClose();
  if ( nErr ) TRACE( L"Error in AdsPortClose() n.%d\n", nErr );
	else        TRACE( L"AdsPortClose() completed successfully\n" );
//  if ( nErr ) TRACE("Errore in chiusura AdsPortClose() n.%d\n", nErr );
//	else        TRACE("AdsPortClose() eseguito correttamente\n" );
	return( 0 );
}



// Funzione che cerca l'handle di una variabile dal nome
// se ritorna 0 tutto Ok e l'handle viene aggiornato,
// altrimenti ritorna 99 e visualizza una messagebox con l'errore
int ADS_init_parameter( char* var_name, ADS_data_type* param, UINT dim, BOOL silent/*=FALSE*/ )
{
 
  ZeroMemory( param, sizeof(ADS_data_type ) );
//  param->handle   = 0; // Parametro non agganciato

	if ( ADS_status!=1 ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

	ASSERT( param && var_name );
	ASSERT( dim>0 && dim<1024*1024 ); // per ora limito a 1MB la max dimensione del dato
	ASSERT( AfxIsValidString ( var_name ) );
	ASSERT( AfxIsValidAddress( param , sizeof(param) ) );

	param->dim      = dim;
	param->val      = malloc( dim );
  param->var_name = var_name;
	ASSERT( AfxIsValidAddress( param->val , dim ) );

  int nErr = AdsSyncReadWriteReq( &Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(param->handle), &param->handle, strlen(var_name), var_name );
  if ( nErr )
  {
    if ( !silent )
    {
      swprintf( msg_err, 200, string_186[lng], AtoW(var_name), nErr, ADS_error_desc(nErr) );
      Message_box( msg_err );
    }
    return( 99 );
  }

  ADS_read_parameter( param ); // leggi valore iniziale
	return 0; // Tutto Ok
}




// Function to link and read DINT parameter from PLC
// returns 0 if Ok, >0 in case of error
int ADS_read_parameter( char* var_name, int* val )
{
  ASSERT( val && AfxIsValidAddress(val,4) );
	if ( ADS_status!=1 ) return( 19 ); // no communication

	ASSERT( var_name );
	ASSERT( AfxIsValidString ( var_name ) );

  ADS_data_type data;

  if ( ADS_init_parameter( var_name, &data, 4 ) ) return( 29 );

  *val = *(int*)data.val;

  if ( data.val ) free( data.val );

  return 0; // Tutto Ok
}




// Read from PLC serialnumber 
int ADS_read_serialnumber( void )
{
  // Read from PLC and prepare string with machine name (
  ADS_data_type sernum;

  if ( !ADS_init_parameter( ".SERIAL_NUMBER", &sernum, 20 ) )
  {
    _snprintf( serial_number, 20, (char*)sernum.val );
  }
  else _snprintf( serial_number, 20, "cant read sernum" );

  ADS_clean_parameter( &sernum );
  return 0; // Tutto Ok
}



/*
int ADS_read_serialnumber( void )
{
  // Read from PLC and prepare string with machine name (
  ADS_data_type sernum;

  if ( !ADS_init_parameter( ".SERIAL_NUMBER", &sernum, 20 ) )
  {
    _snprintf( machine_name, 20, (char*)sernum.val );
  }
  else _snprintf( machine_name, 20, "cantreadmcnum" );

  ADS_clean_parameter( &sernum );
  return 0; // Tutto Ok
}
*/

// Funzione per la deallocazione della memoria associata al parametro
int ADS_clean_parameter(  ADS_data_type* param )
{
  ASSERT( param );
  if ( param->handle && param->val )
  {
    free( param->val );
    param->val = NULL;
  }
  return 0;
}





// Funzione che scrive il valore attuale del parametro nel PLC
int ADS_write_parameter( ADS_data_type* param )
{
	if ( ADS_status!=1 || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );

	int nErr = AdsSyncWriteReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format( string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

	return 0; // Tutto Ok
}



// Funzione che scrive il valore BYTE del parametro nel PLC
int ADS_write_byte( ADS_data_type* param, BYTE val )
{
	if ( ADS_status!=1 || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

  ASSERT( param->dim == 1 );
	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );

  *((BYTE*)param->val) = val;
	int nErr = AdsSyncWriteReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format( string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

	return 0; // Tutto Ok
}

// Funzione che scrive il valore INT del parametro nel PLC
int ADS_write_int( ADS_data_type* param, int val )
{
	if ( ADS_status!=1 || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

  ASSERT( param->dim == 4 );
	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );

  *((int*)param->val) = val;
	int nErr = AdsSyncWriteReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format( string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_182[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

	return 0; // Tutto Ok
}





// Funzione che legge il valore attuale del parametro dal PLC (tipo BYTE/BOOL8) 
BYTE ADS_read_byte( ADS_data_type* param )
{
	if ( ADS_status!=1 || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

  ASSERT( param->dim == 1 ); // verifica tipo/dimensione
	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );

	int nErr = AdsSyncReadReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format( string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

	return *((BYTE*)param->val); // Ritorno direttamente il valore letto
}


// Function to link and read DINT(int) parameter directly from PLC
int ADS_read_int( ADS_data_type* param )
{
	if ( ADS_status!=1 || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

  ASSERT( param->dim == 4 ); // verifica tipo/dimensione
	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );

	int nErr = AdsSyncReadReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	if ( nErr )
	{
		ADS_status = 0;
//		msg_err.Format(string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		swprintf( msg_err, 200, string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
		return( nErr );
	}

	return *((int*)param->val); // Ritorno direttamente il valore letto
}







// Function to read PLC parameter value
int ADS_read_parameter( ADS_data_type* param )
{
	if ( ADS_status!=1 || !param || !param->handle ) return( -1 ); // comunicazione inattiva oppure variabile non agganciata

	ASSERT( param && param->handle );
	ASSERT( AfxIsValidAddress( param, sizeof(param) ) );
	ASSERT( AfxIsValidAddress( param->val, param->dim ) );
  ULONG readnb = 0;

//	int	nErr = AdsSyncReadReqEx2( nPort, &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val, &readnb ); 
//	int nErr = AdsSyncReadReq( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val ); 
	int nErr = AdsSyncReadReqEx( &Addr, ADSIGRP_SYM_VALBYHND, param->handle, param->dim, param->val, &readnb ); 
	if ( nErr )
	{
		//ADS_status = 0;
//		msg_err.Format( string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
		//swprintf( msg_err, string_184[lng], param->var_name, nErr, ADS_error_desc(nErr) );
    //if ( Message_box( msg_err, 1 ) != IDYES ) ADS_status=1;
    TRACE("ADS_read_parameter: ERRORE %d reading <%s> - read %d bytes\n", nErr, param->var_name, readnb );
		return( nErr );
	}

	return 0; // Tutto Ok
}



// Funzione che aggancia la variabile col nome in ingresso e la aggiorna al valore in ingresso 
int ADS_write_value( char* name_in, double val_in, int type_in )
{
	int dim;
	ADS_data_type param;

	if ( ADS_status!=1 ) return( -1 ); // comunicazione inattiva

	switch ( type_in )
	{
    case CTYPE_BOOL8:
    case CTYPE_BYTE:
			dim = 1;
			break;

    case CTYPE_INT:
			dim = 2;
			break;

    case CTYPE_DINT:
    case CTYPE_REAL:
			dim = 4;
			break;

		case CTYPE_LREAL:
			dim = 8;
			break;

		default: // Unhandled case
      //msg_err.Format("Unhandled type of data passed to ADS_write_value (%d)", type_in);
//      msg_err.Format(L"Unhandled type of data passed to ADS_write_value (%d)", type_in);
      swprintf( msg_err, 200, L"Unhandled type of data passed to ADS_write_value (%d)", type_in);
			return 999;
	}


  if ( ADS_init_parameter( name_in, &param, dim ) )
	{
    //char msg[100];
    //sprintf( msg, "Error in getting handle of <%s>", name_in );
    wchar_t msg[100+1];
    swprintf( msg, 100, L"Error in getting handle of <%s>", name_in );
  	Message_box( msg, MBOX_ERROR );
		delete param.val;
		return 80;
	}

	switch ( type_in )
	{
    case CTYPE_BOOL8:
     	*((bool*)param.val) = val_in!=0; //bool(val_in);
			break;

    case CTYPE_BYTE:
     	*((BYTE*)param.val) = BYTE(val_in); 
			break;

    case CTYPE_INT:
     	*((short int*)param.val) = short int(val_in);
			break;

    case CTYPE_DINT:
     	*((int*)param.val) = int(val_in);
			break;

		case CTYPE_REAL:
     	*((float*)param.val) = float(val_in);
			break;

    case CTYPE_LREAL:
     	*((double*)param.val) = val_in;
			break;
	}

  if ( ADS_write_parameter( &param ) )
	{
		//Message_box("Error writing new value for <SPIN_PARAMS.INDEX>", MBOX_ERROR );
		Message_box( L"Error writing new value for <SPIN_PARAMS.INDEX>", MBOX_ERROR );
		delete param.val;
		return 90;
	}

  delete param.val;
	return 0; // Successfully
}




/*
struct update_data_type
{
  ADS_data_type*  ADS;        // pointer to ADS object
  UINT*           perc_base;  // pointer to ADS val area to be used to calculate percentage
  int             obj_type;   //
  wchar_t*        format_txt; // display/formatting specifier
  CStatic*        desc_st;    // description cstatic pointer
  CStatic_mod*    text_st;    // text cstatic pointer
  RECT            desc_a1;
  RECT            desc_a2;
  RECT            text_a1;
  RECT            text_a2;
};

#define UPLIST_FONT     110, L"DejaVu Sans Condensed"   // font used for text into text rows 
#define MAX_UPLIST_NUM   20

*/


/*
ADS_data_type           input_good_err; // distance error between input and output good ps
ADS_data_type           input_rej1_err; // distance error between input and reject1 output ps
ADS_data_type           input_rej2_err; // distance error between input and reject2 output ps
*/
CFont                   font_stats;








int ADS_create_updatelist_new( void )
{

  TRACE("ADS_create_updatelist_new.....");

//  ZeroMemory( uplist, sizeof(uplist) );
//  uplist_num = 0;


  // define matrix, max n° of elements
  UINT nrows    = 4;
  UINT ncolumns = 3;

  UINT nId = 10000;

  /*
  #ifdef TIVS_A40

    //#if SERIAL_N==201300502 // HENG RUI - includes also FO color
    #ifdef FO_COLOR
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #else
      #ifdef DOTRING_COLOR
        char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ", ".NUM_DRCOL_REJ" };
        int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT,       CTYPE_DINT      };
      #else
        char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
        int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
      #endif
    #endif
  #endif

  #ifdef TIVS_F40  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

  #ifdef TIVS_A60
    #ifdef FO_COLOR
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #else
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #endif
  #endif

  #ifdef TIVS_C20  //                                                                                                                                      SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_FILLLEV_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT        , CTYPE_DINT      };
  #endif

  #ifdef FILLEV
    nrows = 0;
    char* ADS_objs[] = { ".NUM_PROD_IN" };
    int   obj_type[] = { CTYPE_DINT     };
  #endif

  #ifdef STOPPER_CTRL
    nrows = 0;
    char* ADS_objs[] = { ".NUM_PROD_IN" };
    int   obj_type[] = { CTYPE_DINT     };
  #endif
  */

  struct update_item
  {
    char*     ADS_name;
    int       type;
    wchar_t** string;
  };

  update_item   st_prodin    = { ".NUM_PROD_IN",        CTYPE_DINT, string_entered   };
  update_item   st_good      = { ".NUM_GOODS",          CTYPE_DINT, string_goods   };

  #ifdef MANU_CHECK
  update_item   st_totgood = { ".NUM_TOTAL_GOODS",     CTYPE_DINT, string_totgoods  };
  //update_item   st_totgood = { ".NUM_GOODS",     CTYPE_DINT, string_totgoods  };
  update_item   st_goodmanu = { ".NUM_REJ_GOODS",     CTYPE_DINT, string_manugoods  };
	#endif

  update_item   st_rejects   = { ".NUM_REJECTS",        CTYPE_DINT, string_totrej    };
  update_item   st_rejects1  = { ".NUM_REJECTS1",       CTYPE_DINT, string_1strej    };
  update_item   st_rejects2  = { ".NUM_REJECTS2",       CTYPE_DINT, string_2ndrej    };
  update_item   st_rejects3  = { ".NUM_REJECTS3",       CTYPE_DINT, string_3rdrejhga };
  update_item   st_rejects4  = { ".NUM_REJECTS4",       CTYPE_DINT, string_4threj    };

  update_item   st_caprej    = { ".NUM_CAPSEAL_REJ",    CTYPE_DINT, string_caprej    };
  update_item   st_tiprej    = { ".NUM_CAPSEAL_REJ",    CTYPE_DINT, string_tiprej    };
#ifdef TIVS_S60
  update_item   st_flangerej    = { ".NUM_CAPSEAL_REJ",    CTYPE_DINT, string_flangerej    };
#endif

/*
  #ifdef FO_COLOR
    update_item   st_focolrej  = { ".NUM_FPOFFCOL_REJ",   CTYPE_DINT, string_focolrej  };
  #endif

  #ifdef LIQUID_COLOR
    update_item   st_focolrej  = { ".NUM_FPOFFCOL_REJ",   CTYPE_DINT, string_lqcolrej  };
  #endif
  */

  #ifdef LIQUID_COLOR
    update_item   st_focolrej  = { ".NUM_FPOFFCOL_REJ",   CTYPE_DINT, string_lqcolrej  };
  #else
    update_item   st_focolrej  = { ".NUM_FPOFFCOL_REJ",   CTYPE_DINT, string_focolrej  };
  #endif


  update_item   st_bottarej  = { ".NUM_BOTTOMA_REJ",    CTYPE_DINT, string_bottarej  };
  update_item   st_bottbrej  = { ".NUM_BOTTOMB_REJ",    CTYPE_DINT, string_bottbrej  };

  update_item   st_sidewlrej = { ".NUM_COSMETIC_REJ",   CTYPE_DINT, string_sidewlrej };
  update_item   st_sideckrej = { ".NUM_PARTICLE1_REJ",  CTYPE_DINT, string_sideckrej };
  update_item   st_topckrej  = { ".NUM_PARTICLE2_REJ",  CTYPE_DINT, string_topckrej  };
  update_item   st_heelrej   = { ".NUM_PARTICLE2_REJ",  CTYPE_DINT, string_heelrej   };

  update_item   st_partrej   = { ".NUM_PARTICLE_REJ",   CTYPE_DINT, string_partrej   };
  update_item   st_part1rej  = { ".NUM_PARTICLE1_REJ",  CTYPE_DINT, string_part1rej  };
  update_item   st_part2rej  = { ".NUM_PARTICLE2_REJ",  CTYPE_DINT, string_part2rej  };
  update_item   st_part3rej  = { ".NUM_COSMETIC_REJ",   CTYPE_DINT, string_part3rej  };
  update_item   st_cosmrej   = { ".NUM_COSMETIC_REJ",   CTYPE_DINT, string_cosmrej   };
  update_item   st_cosm3rej  = { ".NUM_COSMETIC3_REJ",  CTYPE_DINT, string_cosm2rej  }; // S40 rejects1 from C2ABC

 // update_item   st_wstlyrrej = { ".NUM_WAISTLAYR_REJ",  CTYPE_DINT, string_wstlyrrej };

  update_item   st_fillevrej = { ".NUM_FILLLEV_REJ",    CTYPE_DINT, string_fillevrej };

  #ifdef OPACITY_CTRL
    update_item   st_densitrj1 = { ".NUM_FILLLEV1_REJ",   CTYPE_DINT, string_densityrej };
    update_item   st_fillevrj2 = { ".NUM_FILLLEV2_REJ",   CTYPE_DINT, string_fillevrej  };
  #endif

	update_item   st_sampling   = { ".NUM_SAMPLING",       CTYPE_DINT, string_num_sampling  };

  update_item   st_spinrej   = { ".NUM_SPIN_REJ",       CTYPE_DINT, string_spinrej   };

  update_item   st_hvrej     = { ".NUM_HV_REJ",         CTYPE_DINT, string_hvrej     };

  #ifdef DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    update_item   st_drrej     = { ".NUM_DRCOL_REJ",     CTYPE_DINT, string_drrej    };
  #endif

  #ifdef HGA_LEAKTEST            // HGA components installed
    update_item   st_hgarej   = { ".NUM_HGA_REJ",       CTYPE_DINT, string_hgarej   };
  #endif


  #ifdef TIVS_F40
    nrows  =  5;
    //   hspace =  6;

  //  #if SERIAL_N==201400054 // AUSIA TEST - ADDED WAIST LAYER
    #if STN1==CAKESIDE_ARBACK // WAIST LAYER
      update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_wstlyrrej, st_spinrej };
    #else
      #ifdef HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection
//        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_spinrej };

        #ifdef HGA_LEAKTEST      // HGA components installed
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_hgarej, st_spinrej };
        #else  // BASIC - STANDARD 
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_spinrej };
        #endif

      #else 

        #ifdef HGA_LEAKTEST      // HGA components installed

	#ifdef MANU_CHECK
	update_item update_list[] = { st_prodin, st_totgood, st_good, st_goodmanu,st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_hgarej, st_spinrej };

	#else
    update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_hgarej, st_spinrej };
	#endif

        #else  // BASIC - STANDARD 
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_spinrej };
        #endif

      #endif
    #endif

  #endif 


	#ifdef TIVS_F20
    nrows  =  5;
    //   hspace =  6;

  //  #if SERIAL_N==201400054 // AUSIA TEST - ADDED WAIST LAYER
    #if STN1==CAKESIDE_ARBACK // WAIST LAYER
      update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_wstlyrrej, st_spinrej };
    #else
      #ifdef HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection
//        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_spinrej };

        #ifdef HGA_LEAKTEST      // HGA components installed
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_hgarej, st_spinrej };
        #else  // BASIC - STANDARD 
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_spinrej };
        #endif

      #else 

        #ifdef HGA_LEAKTEST      // HGA components installed
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_hgarej, st_spinrej };
        #else  // BASIC - STANDARD 
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_topckrej, st_spinrej };
        #endif

      #endif
    #endif

  #endif 


  #ifdef TIVS_A20L

    // nrows  =  5;
    // hspace =  6;

    #if defined NEWCAP_DOUBLE || defined NEWCAP_V21  || defined FO_COLOR    // L40 (VIALS / CAP CONTROL )
    //#if MODEL_NAME  == L"TIVS-L40"    // L40 (VIALS / CAP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
      #endif

    #else                             // A40 (AMPOULES / TIP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_hvrej, st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_hvrej, st_spinrej };
      #endif

    #endif

  #endif 

//Pour added for TIVS-A20 20180523
	  #ifdef TIVS_A20

    // nrows  =  5;
    // hspace =  6;

    #if defined NEWCAP_DOUBLE || defined NEWCAP_V21  || defined FO_COLOR    // L40 (VIALS / CAP CONTROL )
    //#if MODEL_NAME  == L"TIVS-L40"    // L40 (VIALS / CAP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
      #endif

    #else                             // A40 (AMPOULES / TIP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, /*st_rejects2,*/ st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, /*st_hvrej,*/ st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_hvrej, st_spinrej };
      #endif

    #endif

  #endif 

//Pour added for TIVS-A20 20180523

//Pour added for TIVS-C20 20180318
	  #ifdef TIVS_C20

    // nrows  =  5;
    // hspace =  6;

    #if defined NEWCAP_DOUBLE || defined NEWCAP_V21  || defined FO_COLOR    // L40 (VIALS / CAP CONTROL )
    //#if MODEL_NAME  == L"TIVS-L40"    // L40 (VIALS / CAP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
      #else  // particle 3
        //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
		//                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

      #endif

    #else                             // A40 (AMPOULES / TIP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, /*st_rejects2,*/ st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, /*st_hvrej,*/ st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_hvrej, st_spinrej };
      #endif

    #endif

  #endif 

//Pour added for TIVS-C20 20190318

//Pour added for TIVS-A40L 20180124
  #ifdef TIVS_A40L

    // nrows  =  5;
    // hspace =  6;

    #if defined NEWCAP_DOUBLE || defined NEWCAP_V21  || defined FO_COLOR    // L40 (VIALS / CAP CONTROL )
    //#if MODEL_NAME  == L"TIVS-L40"    // L40 (VIALS / CAP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
      #endif

    #else                             // A40 (AMPOULES / TIP CONTROL )

      #if STN3==COSMETIC_BACK
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_hvrej, st_spinrej };
      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_hvrej, st_spinrej };
      #endif

    #endif

  #endif 
//Pour added for TIVS-A40L 20180124


  #ifdef TIVS_A40

    // nrows  =  5;
    // hspace =  6;

    #if defined NEWCAP_DOUBLE || defined NEWCAP_V21  || defined FO_COLOR || defined LIQUID_COLOR    // L40 (VIALS / CAP CONTROL )
    //#if MODEL_NAME  == L"TIVS-L40"    // L40 (VIALS / CAP CONTROL )

      #if STN3==COSMETIC_BACK
//        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };

        #ifdef HGA_LEAKTEST      // HGA components installed
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_hgarej, st_spinrej };
        #else  // BASIC - STANDARD 

		            #ifdef MANU_CHECK
				//update_item update_list[] = { st_prodin, st_totgood, st_good, st_goodmanu,st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_bottarej, st_bottbrej, st_sidewlrej, st_sideckrej, st_heelrej, st_spinrej };//Pour removed on 20200222
				update_item update_list[] = { st_prodin, st_totgood, st_good, st_goodmanu,st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej,st_spinrej };
			#else
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
	#endif
        #endif

      #else  // particle 3
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej }; //Pour marked 20190410 to be recovered
		//Pour changed for INTAS old machine 20190410
		//update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_fillevrej, st_part1rej, st_part2rej, st_part3rej, st_spinrej };
		//Pour changed for INTAS old machine 20190410

      #endif

    #else                             // A40 (AMPOULES / TIP CONTROL )

      #ifdef DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
   // update_item   st_drrej     = { ".NUM_DRCOL_REJ",     CTYPE_DINT, string_drrej    };
        #if STN3==COSMETIC_BACK
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_drrej, st_fillevrej, st_spinrej };
        #else  // particle 3
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_drrej, st_fillevrej, st_spinrej };
        #endif
      #else // NO dotring
        #if STN3==COSMETIC_BACK
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
        #else  // particle 3
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_tiprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
        #endif
      #endif

    #endif

  #endif 


  #ifdef TIVS_A60

    // nrows  =  5;
    // hspace =  6;

    //#error DA VERIFICARE
/*
	//Pour added 20181008
	#ifdef HV_LEAKTEST
		  update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_hvrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
	#endif
	//Pour added 20181008
	*/
	//#else
    #if STN3==COSMETIC_BACK
      //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
	  	
	#ifdef OPACITY_CTRL
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej, st_sampling }; //sampling only for KeXing
		  //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_hvrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej, st_sampling }; //sampling only for KeXing
	//update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej };        
	#endif


	  //Pour added 20181008
	#if defined HV_LEAKTEST
		  //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_hvrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_spinrej };
	//Pour added 20181008
		  #ifdef OPACITY_CTRL
          //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_hvrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej, st_sampling }; //sampling only for KeXing
	update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_hvrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej };        
	#else
		  update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_hvrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_spinrej };
	//#else
	//#elif
		  //#ifdef OPACITY_CTRL
          //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej, st_sampling };
          //#else

	  //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_fillevrej, st_spinrej };
	#endif


	#endif

    #else  // particle 3
		  	//Pour added 20181008
	#if defined HV_LEAKTEST
		  update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_hvrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
	#else

		  #ifdef OPACITY_CTRL
          update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_densitrj1, st_cosmrej,  st_part2rej, st_fillevrj2, st_spinrej };
        #else

	//Pour added 20181008
      update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };
    #endif

	#endif

	  //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_hvrej, st_part1rej, st_part2rej, st_part3rej, st_fillevrej, st_spinrej };

  #endif

#endif


  #ifdef TIVS_C40
    #ifdef HGA_LEAKTEST

      #if defined OPACITY_CTRL //THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
        //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14   
        //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

		//                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14          15   
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_densitrj1,  st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

	#elif defined THIRD_DEVIATOR //OPACITY_CTRL  //THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
	    //      //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14          15   
        //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_densitrj1,  st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
        
	  //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14   
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

	#else
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_hgarej, st_spinrej };
      #endif

    #else

	        #if defined OPACITY_CTRL //THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
        //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14   
        //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

		//                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14   
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_densitrj1,  st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

	#elif defined THIRD_DEVIATOR //OPACITY_CTRL  //THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
	    //      //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14          15   
        //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej,  st_densitrj1,  st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
        
	  //                                 0        1         2          3             4             5             6         7             8           9           10           11             12           13         14   
        update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };

	#else


      update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
    #endif
  #endif 

	#endif
 
  #ifdef TIVS_C60
    update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_caprej, st_focolrej, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
  #endif 


	  #ifdef TIVS_S40
    nrows  =  5;
    //hspace =  6;

    //st_rejects3.ADS_name = ".NUM_REJECTS3";
    //st_rejects3.type     =  CTYPE_DINT;
    st_rejects3.string   = string_3rdrej;
    //st_rejects4.ADS_name = ".NUM_REJECTS4";
    //st_rejects4.type     =  CTYPE_DINT;
    st_rejects4.string   = string_4threj;
/*    st_bottarej.string   = 
      st_bottbrej.string =
      st_spinrej.string = */


//    update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_rejects4 }; //, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
    update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_rejects4, st_partrej, st_part1rej, st_part2rej, st_cosmrej, st_cosm3rej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
  #endif


		  #ifdef TIVS_S60
    nrows  =  5;
    //hspace =  6;

    //st_rejects3.ADS_name = ".NUM_REJECTS3";
    //st_rejects3.type     =  CTYPE_DINT;
    st_rejects3.string   = string_3rdrej;
    //st_rejects4.ADS_name = ".NUM_REJECTS4";
    //st_rejects4.type     =  CTYPE_DINT;
    st_rejects4.string   = string_4threj;


    st_bottarej.string   = string_cosm3arej;
    st_bottbrej.string   = string_cosm3brej;
      //st_spinrej.string = 


//    update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_rejects4 }; //, st_part1rej, st_part2rej, st_cosmrej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
    //update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_rejects4, st_partrej, st_part1rej, st_part2rej, st_cosmrej, st_cosm3rej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };
	//update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_partrej, st_part1rej, st_part2rej, st_cosmrej, st_cosm3rej, st_fillevrej, st_bottarej, st_bottbrej, st_spinrej };//Displaying on the main screen
	update_item update_list[] = { st_prodin, st_good, st_rejects, st_rejects1, st_rejects2, st_rejects3, st_flangerej, st_partrej, st_part1rej, st_part2rej, st_cosmrej, st_cosm3rej, st_densitrj1, st_bottarej, st_bottbrej, st_spinrej };//Displaying on the main screen
  #endif



  UINT nitems = sizeof(update_list)/sizeof(update_item);


  if ( nitems>nrows*ncolumns ) 
  {
    nrows++;
  }


  ASSERT( nitems <= nrows * ncolumns );


  //update_item* update_array = (update_item*) malloc( nrows * ncolumns * sizeof(update_item) );
  //  ASSERT( update_array );



    
/*  UINT nobj = sizeof(ADS_objs)/sizeof(char*);

  // check same number of objects in all arrays
  ASSERT( sizeof(obj_type)    /sizeof(int     ) == nobj );
  ASSERT( sizeof(obj_desc_eng)/sizeof(wchar_t*) == nobj );
  ASSERT( sizeof(obj_desc_chn)/sizeof(wchar_t*) == nobj ); */

  RECT area_txt;
  POINT upleft = { 20, 165 };

  UINT hitem  =  30;
  UINT hspace =  10;
  UINT wdesc  = 200; // width of description cstatic
  UINT wtext  = 150; // width of text/val cstatic
  UINT wspace =  50; // horizontal space between different fields

  RECT area;
  area.left   =              10;
  area.right  = area.left + 350;//Pour marked to be adjusted on 20201021
  area.top    =       200 + 100;
  area.bottom = area.top  +  35;


  #ifdef TIVS_F40
    hspace =  2;
  #endif 

  int dimch = 20; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 20;

  VERIFY( font_stats.CreateFont( dimch,                         // Height
                                 0,                             // Width
                                 0,                             // Escapement
                                 10,                            // Orientation
                                 500,                           // Weight 100-900
                                 FALSE,                         // Italic
                                 FALSE,                         // Underline
                                 0,                             // StrikeOut
                                 ANSI_CHARSET,                  // CharSet
                                 OUT_OUTLINE_PRECIS,            // OUT_DEFAULT_PRECIS,            // OutPrecision
                                 CLIP_DEFAULT_PRECIS,           // ClipPrecision
                                 PROOF_QUALITY,                 // DEFAULT_QUALITY,               // Quality
                                 DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                                 lng==CHINESE?L"SimSun":L"Arial" ) );


  for ( UINT nr = 0 ; nr < nrows ; nr++ )
  {
    for ( UINT nc = 0 ; nc < ncolumns ; nc++ )
    {

      if ( uplist_num<nitems ) // nobj )
      {
        area_txt.top    = upleft.y + (hitem+hspace)*nr;
        area_txt.bottom = area_txt.top + hitem;
        area_txt.left   = upleft.x + (wdesc+wtext+wspace)*nc;
        area_txt.right  = area_txt.left + wdesc;

        uplist[uplist_num].desc_st = new CStatic;
        VERIFY( uplist[uplist_num].desc_st->CreateEx( WS_EX_WINDOWEDGE, L"Static", L"A", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY|SS_LEFT, area_txt, pView->container_main, nId++ ) );
        
        uplist[uplist_num].desc_txt = update_list[uplist_num].string[lng];


     /*   uplist[uplist_num].desc_txt = obj_desc_eng[uplist_num]; // default
        if ( lng==CHINESE ) uplist[uplist_num].desc_txt = obj_desc_chn[uplist_num];
        if ( lng==SPANISH ) uplist[uplist_num].desc_txt = obj_desc_esp[uplist_num]; */
        
        uplist[uplist_num].desc_st->SetFont( &font_stats );
        uplist[uplist_num].desc_st->SetWindowTextW( uplist[uplist_num].desc_txt );

        area_txt.left   = area_txt.right + 5;
        area_txt.right  = area_txt.left  + wtext;

        uplist[uplist_num].text_st = new CStatic_mod;
        VERIFY( uplist[uplist_num].text_st->CreateEx( WS_EX_STATICEDGE, L"Static", L"B", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY|SS_CENTER, area_txt, pView->container_main, nId++ ) );
        uplist[uplist_num].desc_st->SetFont( &font_stats );
        uplist[uplist_num].text_st->SetWindowTextW( L"-" );

        uplist[uplist_num].obj_type = update_list[uplist_num].type;

        //uplist[uplist_num].obj_type = obj_type[uplist_num];

        // try to connect with ADS
        uplist[uplist_num].ADS = new ADS_data_type;
        ASSERT( uplist[uplist_num].obj_type == CTYPE_DINT );


        ADS_init_parameter( update_list[uplist_num].ADS_name, uplist[uplist_num].ADS, 4 );
        //ADS_init_parameter( ADS_objs[uplist_num], uplist[uplist_num].ADS, 4 );

        //ASSERT( uplist[uplist_num].ADS && uplist[uplist_num].ADS->handle );

        // ------------  Create update items  ----------------

        #ifdef TIVS_F40
          if ( uplist_num==5 )
          {
            area.top    += 170;
            area.bottom += 170;
          }
        #endif

        uplist[uplist_num].item_rt = new CText_box( TEXT_LAYOUT_FONT, area, pView, WS_VISIBLE|SS_CENTER/*SS_LEFT*/|SS_CENTERIMAGE );
        uplist[uplist_num].item_rt->SetFont( &font_stats );
        uplist[uplist_num].item_rt->SetText( uplist[uplist_num].desc_txt, TXT_BLACK, RGB(179,217,255) );
        area.top    = area.bottom +  5;
        area.bottom = area.top    + 30; // 35;

        uplist_num++;
        ASSERT( uplist_num < MAX_UPLIST_NUM );
      }
    }
  }

  ASSERT( uplist_num==nitems ); // nobj );

  for ( UINT i=1 ; i<uplist_num ; i++ ) // assign base count pointer to calculate percentages
  {
    uplist[i].perc_base = (UINT*) uplist[0].ADS->val;
  }

  ZeroMemory( &carousel_phase, sizeof(carousel_phase) );
  ZeroMemory( &encoder_height, sizeof(encoder_height) );

  if ( IS_PC1 )
  {
    /*
    // add one more static to show phases and readings of machine
    area_txt.left   = upleft.x;
    area_txt.right  = area_txt.left + 1200;
    area_txt.top    = upleft.y + (hitem+hspace)*nrows;
    area_txt.bottom = area_txt.top + hitem;

    uplist[uplist_num+1].desc_st = new CStatic;
    VERIFY( uplist[uplist_num+1].desc_st->CreateEx( WS_EX_WINDOWEDGE, L"Static", L"A", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY, area_txt, pView->container_main, nId++ ) );
    uplist[uplist_num+1].desc_st->SetWindowTextW( L"readings from PLC" );
    */

	#ifndef TIVS_S40
	#ifndef TIVS_S60
    ADS_init_parameter( ".I_CAROUSEL_HEIGHT",            &encoder_height, 4 );
	#endif
	#endif
    ADS_init_parameter( "CAROUSEL_AXIS.NC2PLC.FPOSSOLL", &carousel_phase, 8 );
    ADS_init_parameter( "PRODUCT_FLOW.INPUT_WIDTH",      &input_width,    8 );
    ADS_init_parameter( "PRODUCT_FLOW.INPUT_POSITION",   &input_phase,    8 );
    ADS_init_parameter( "PRODUCT_FLOW.GOOD_POSITION",    &outgood_phase,  8 );
    ADS_init_parameter( "PRODUCT_FLOW.REJECT1_POSITION", &outrej1_phase,  8 );
    ADS_init_parameter( "PRODUCT_FLOW.REJECT2_POSITION", &outrej2_phase,  8 );

	//Pour added 20180915

	ADS_init_parameter( ".PRODSPEED", &prod_speed,  4 );

    #ifdef HGA_LEAKTEST // hga_input
      ADS_init_parameter( ".I_HGA_INPUT",                &hga_input, 2 );
	  ADS_init_parameter( ".HGA_VALUE",                &hga_value, 4 );
    #endif


    #ifdef TIVS_A20L // HV readings to display
      //ADS_init_parameter( ".HVBLOCK_4.PTR",                 &hvblock4_ptr, 1 );//Pour removed 20180914
	  ADS_init_parameter( "HV_MANAGER.PTR4",                 &hvblock4_ptr, 1 );
	  /*
      ADS_init_parameter( ".HVBLOCK_1.I_VOLTAGE_RD",        &hv_voltin[0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.I_CURRENT_RD",        &hv_currin[0], 2 );
      ADS_init_parameter( ".HVBLOCK_2.I_VOLTAGE_RD",        &hv_voltin[1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.I_CURRENT_RD",        &hv_currin[1], 2 );
      ADS_init_parameter( ".HVBLOCK_3.I_VOLTAGE_RD",        &hv_voltin[2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.I_CURRENT_RD",        &hv_currin[2], 2 );
      ADS_init_parameter( ".HVBLOCK_4.I_VOLTAGE_RD",        &hv_voltin[3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.I_CURRENT_RD",        &hv_currin[3], 2 );

      ADS_init_parameter( ".HVBLOCK_1.NDATA",               &hv_ndata[0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.VMIN",                &hv_vmin [0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.VMAX",                &hv_vmax [0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.ERROR",               &hv_error[0], 2 );

      ADS_init_parameter( ".HVBLOCK_2.NDATA",               &hv_ndata[1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.VMIN",                &hv_vmin [1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.VMAX",                &hv_vmax [1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.ERROR",               &hv_error[1], 2 );

      ADS_init_parameter( ".HVBLOCK_3.NDATA",               &hv_ndata[2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.VMIN",                &hv_vmin [2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.VMAX",                &hv_vmax [2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.ERROR",               &hv_error[2], 2 );

      ADS_init_parameter( ".HVBLOCK_4.NDATA",               &hv_ndata[3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.VMIN",                &hv_vmin [3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.VMAX",                &hv_vmax [3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.ERROR",               &hv_error[3], 2 );

      ADS_init_parameter( "HV_MANAGER.ERROR_TOT",           &hv_errortot, 2 );
	  */
	  //Pour added 20180915
	  ADS_init_parameter( "HV_MANAGER.PTR1",           &hv_manager_ptr1, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR2",           &hv_manager_ptr2, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR3",           &hv_manager_ptr3, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR4",           &hv_manager_ptr4, 2 );


    #endif

//Pour added 20180124
	      //#ifdef TIVS_A40L // HV readings to display
	  #if defined TIVS_A40L || SERIAL_N == 201900005|| SERIAL_N == 201900006||SERIAL_N == 201900007 ||SERIAL_N == 201900008 ||SERIAL_N == 201900009 ||SERIAL_N == 201900010 ||SERIAL_N == 201900011 ||SERIAL_N == 201900013 ||SERIAL_N == 201900014 ||SERIAL_N == 201900020 || SERIAL_N == 202000011 || SERIAL_N == 202000021/*|| defined TIVS_A60*/ //removed for TAIJI ON 20180830
      //ADS_init_parameter( ".HVBLOCK_4.PTR",                 &hvblock4_ptr, 1 );
	  ADS_init_parameter( "HV_MANAGER.PTR4",                 &hvblock4_ptr, 1 );

      ADS_init_parameter( ".HVBLOCK_1.I_VOLTAGE_RD",        &hv_voltin[0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.I_CURRENT_RD",        &hv_currin[0], 2 );
      ADS_init_parameter( ".HVBLOCK_2.I_VOLTAGE_RD",        &hv_voltin[1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.I_CURRENT_RD",        &hv_currin[1], 2 );
      ADS_init_parameter( ".HVBLOCK_3.I_VOLTAGE_RD",        &hv_voltin[2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.I_CURRENT_RD",        &hv_currin[2], 2 );
      ADS_init_parameter( ".HVBLOCK_4.I_VOLTAGE_RD",        &hv_voltin[3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.I_CURRENT_RD",        &hv_currin[3], 2 );

      ADS_init_parameter( ".HVBLOCK_1.NDATA",               &hv_ndata[0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.VMIN",                &hv_vmin [0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.VMAX",                &hv_vmax [0], 2 );
      ADS_init_parameter( ".HVBLOCK_1.ERROR",               &hv_error[0], 2 );

      ADS_init_parameter( ".HVBLOCK_2.NDATA",               &hv_ndata[1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.VMIN",                &hv_vmin [1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.VMAX",                &hv_vmax [1], 2 );
      ADS_init_parameter( ".HVBLOCK_2.ERROR",               &hv_error[1], 2 );

      ADS_init_parameter( ".HVBLOCK_3.NDATA",               &hv_ndata[2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.VMIN",                &hv_vmin [2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.VMAX",                &hv_vmax [2], 2 );
      ADS_init_parameter( ".HVBLOCK_3.ERROR",               &hv_error[2], 2 );

      ADS_init_parameter( ".HVBLOCK_4.NDATA",               &hv_ndata[3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.VMIN",                &hv_vmin [3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.VMAX",                &hv_vmax [3], 2 );
      ADS_init_parameter( ".HVBLOCK_4.ERROR",               &hv_error[3], 2 );

      ADS_init_parameter( "HV_MANAGER.ERROR_TOT",           &hv_errortot, 2 );
	  
	  //Pour added 20180319
	  ADS_init_parameter( "HV_MANAGER.PTR1",           &hv_manager_ptr1, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR2",           &hv_manager_ptr2, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR3",           &hv_manager_ptr3, 2 );
	  ADS_init_parameter( "HV_MANAGER.PTR4",           &hv_manager_ptr4, 2 );
	  //Pour added 20180319

	  #if SERIAL_N == 202000020
	  ADS_init_parameter( ".NUM_SAMPLING",           &num_sampling, 2 );
	#endif

	  
    #endif
//Pour added 20180124

  }

  ADS_update_list();

  TRACE("completed - %d objects created\n", uplist_num );

	return 0; // Successfully
}






int ADS_create_updatelist( void )
{

  TRACE("ADS_create_updatelist.....");

  ZeroMemory( uplist, sizeof(uplist) );
  uplist_num = 0;


  // define matrix, max n° of elements
  UINT nrows    = 4;
  UINT ncolumns = 3;

  UINT nId = 10000;

  #ifdef TIVS_A40

    //#if SERIAL_N==201300502 // HENG RUI - includes also FO color
    #ifdef FO_COLOR
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #else
      #ifdef DOTRING_COLOR
        char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ", ".NUM_DRCOL_REJ" };
        int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT,       CTYPE_DINT      };
      #else
        char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
        int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
      #endif
    #endif
  #endif

  #ifdef TIVS_F40  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

	#ifdef TIVS_F20  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif


  #ifdef TIVS_A60
    #ifdef FO_COLOR
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #else
      char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FILLLEV_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_COSMETIC_REJ", ".NUM_SPIN_REJ" };
      int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,         CTYPE_DINT,           CTYPE_DINT,           CTYPE_DINT,          CTYPE_DINT      };
    #endif
  #endif

  #ifdef TIVS_C20  //                                                                                                                                      SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_FILLLEV_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT        , CTYPE_DINT      };
  #endif

  #ifdef TIVS_A20L  //                                                                                                                                      SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_FILLLEV_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT        , CTYPE_DINT      };
  #endif

//Pour added 20180523

	#ifdef TIVS_A20  //                                                                                                                                      SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_FILLLEV_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT        , CTYPE_DINT      };
	#endif

//Pour added 20180523

//Pour added 20180124
	#ifdef TIVS_A40L  //                                                                                                                                      SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_FILLLEV_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT        , CTYPE_DINT      };
  #endif
//Pour added 20180124


  #ifdef TIVS_C40  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

  #ifdef TIVS_S40  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

	#ifdef TIVS_S60  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

  #ifdef TIVS_C60  //                                                                                                                                                                     SIDEWALL              SIDECAKE              TOPCAKE    
    nrows = 5;
    char* ADS_objs[] = { ".NUM_PROD_IN", ".NUM_GOODS", ".NUM_REJECTS", ".NUM_REJECTS1", ".NUM_REJECTS2", ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ" };
    int   obj_type[] = { CTYPE_DINT,     CTYPE_DINT,   CTYPE_DINT,     CTYPE_DINT,      CTYPE_DINT,      CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT,         CTYPE_DINT,          CTYPE_DINT         , CTYPE_DINT          , CTYPE_DINT          , CTYPE_DINT      };
  #endif

  #ifdef FILLEV
    nrows = 0;
    char* ADS_objs[] = { ".NUM_PROD_IN" };
    int   obj_type[] = { CTYPE_DINT     };
  #endif

  #ifdef STOPPER_CTRL
    nrows = 0;
    char* ADS_objs[] = { ".NUM_PROD_IN" };
    int   obj_type[] = { CTYPE_DINT     };
  #endif

    
    
  UINT nobj = sizeof(ADS_objs)/sizeof(char*);

  // check same number of objects in all arrays
  ASSERT( sizeof(obj_type)    /sizeof(int     ) == nobj );
  ASSERT( sizeof(obj_desc_eng)/sizeof(wchar_t*) == nobj );
  ASSERT( sizeof(obj_desc_chn)/sizeof(wchar_t*) == nobj );

  RECT area_txt;
  POINT upleft = { 20, 165 };

  UINT hitem  =  30;
  UINT hspace =  10;
  UINT wdesc  = 300; // width of description cstatic
  UINT wtext  = 50; // width of text/val cstatic
  UINT wspace =  50; // horizontal space between different fields

  RECT area;
  area.left   =              10;
  area.right  = area.left + 350;
  area.top    =             200;
  area.bottom = area.top  +  35;


  #ifdef TIVS_F40
    hspace =  6;
  #endif

  int dimch = 20; // base dimension for ENGLISH characters
  if ( lng==CHINESE ) dimch = 20;

  VERIFY( font_stats.CreateFont( dimch,                         // Height
                         0,                             // Width
                         0,                             // Escapement
                         10,                            // Orientation
                         500,                           // Weight 100-900
                         FALSE,                         // Italic
                         FALSE,                         // Underline
                         0,                             // StrikeOut
                         ANSI_CHARSET,                  // CharSet
                         OUT_OUTLINE_PRECIS, //OUT_DEFAULT_PRECIS,            // OutPrecision
                         CLIP_DEFAULT_PRECIS,           // ClipPrecision
                         PROOF_QUALITY, //DEFAULT_QUALITY,               // Quality
                         DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                         lng==CHINESE?L"SimSun":L"Arial" ) );
//                         L"Arial" ) );


  for ( UINT nr = 0 ; nr < nrows ; nr++ )
  {
    for ( UINT nc = 0 ; nc < ncolumns ; nc++ )
    {

      if ( uplist_num<nobj )
      {
        area_txt.top    = upleft.y + (hitem+hspace)*nr;
        area_txt.bottom = area_txt.top + hitem;
        area_txt.left   = upleft.x + (wdesc+wtext+wspace)*nc;
        area_txt.right  = area_txt.left + wdesc;

        uplist[uplist_num].desc_st = new CStatic;
        VERIFY( uplist[uplist_num].desc_st->CreateEx( /*WS_EX_CENTER*/WS_EX_WINDOWEDGE, L"Static", L"A", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY|SS_LEFT, area_txt, pView->container_main, nId++ ) );
        //if ( lng==ENGLISH ) uplist[uplist_num].desc_st->SetWindowTextW( obj_desc_eng[uplist_num] );
        //if ( lng==CHINESE ) uplist[uplist_num].desc_st->SetWindowTextW( obj_desc_chn[uplist_num] );
        
        uplist[uplist_num].desc_txt = obj_desc_eng[uplist_num]; // default
        if ( lng==CHINESE ) uplist[uplist_num].desc_txt = obj_desc_chn[uplist_num];
        if ( lng==SPANISH ) uplist[uplist_num].desc_txt = obj_desc_esp[uplist_num];
        
        uplist[uplist_num].desc_st->SetFont( &font_stats );
        uplist[uplist_num].desc_st->SetWindowTextW( uplist[uplist_num].desc_txt );

        area_txt.left   = area_txt.right + 5;
        area_txt.right  = area_txt.left  + wtext;

        uplist[uplist_num].text_st = new CStatic_mod;
        VERIFY( uplist[uplist_num].text_st->CreateEx( /*WS_EX_CENTER|*/WS_EX_STATICEDGE, L"Static", L"B", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY|SS_CENTER/*|SS_RIGHT*/, area_txt, pView->container_main, nId++ ) );
        uplist[uplist_num].desc_st->SetFont( &font_stats );
        uplist[uplist_num].text_st->SetWindowTextW( L"-" );

        uplist[uplist_num].obj_type = obj_type[uplist_num];

        // try to connect with ADS
        uplist[uplist_num].ADS = new ADS_data_type;
        ASSERT( uplist[uplist_num].obj_type == CTYPE_DINT );
        ADS_init_parameter( ADS_objs[uplist_num], uplist[uplist_num].ADS, 4 );
        //ASSERT( uplist[uplist_num].ADS && uplist[uplist_num].ADS->handle );

        // ------------  Create update items  ----------------

        #ifdef TIVS_F40
          if ( uplist_num==5 )
          {
            area.top    += 170;
            area.bottom += 170;
          }
        #endif

        uplist[uplist_num].item_rt = new CText_box( TEXT_LAYOUT_FONT, area, pView, WS_VISIBLE|SS_CENTER/*SS_LEFT*/|SS_CENTERIMAGE );
        uplist[uplist_num].item_rt->SetFont( &font_stats );
        uplist[uplist_num].item_rt->SetText( uplist[uplist_num].desc_txt, TXT_BLACK, RGB(179,217,255) );
        area.top    = area.bottom +  5;
        area.bottom = area.top    + 35;

        uplist_num++;
        ASSERT( uplist_num < MAX_UPLIST_NUM );
      }
    }
  }

  ASSERT( uplist_num==nobj );

  for ( UINT i=1 ; i<uplist_num ; i++ ) // assign base count pointer to calculate percentages
  {
    uplist[i].perc_base = (UINT*) uplist[0].ADS->val;
  }

  ZeroMemory( &carousel_phase, sizeof(carousel_phase) );
  ZeroMemory( &encoder_height, sizeof(encoder_height) );

  if ( IS_PC1 )
  {
    /*
    // add one more static to show phases and readings of machine
    area_txt.left   = upleft.x;
    area_txt.right  = area_txt.left + 1200;
    area_txt.top    = upleft.y + (hitem+hspace)*nrows;
    area_txt.bottom = area_txt.top + hitem;

    uplist[uplist_num+1].desc_st = new CStatic;
    VERIFY( uplist[uplist_num+1].desc_st->CreateEx( WS_EX_WINDOWEDGE, L"Static", L"A", WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_NOTIFY, area_txt, pView->container_main, nId++ ) );
    uplist[uplist_num+1].desc_st->SetWindowTextW( L"readings from PLC" );
    */

    ADS_init_parameter( ".I_CAROUSEL_HEIGHT",            &encoder_height, 4 );
    ADS_init_parameter( "CAROUSEL_AXIS.NC2PLC.FPOSSOLL", &carousel_phase, 8 );
    ADS_init_parameter( "PRODUCT_FLOW.INPUT_WIDTH",      &input_width,    8 );
    ADS_init_parameter( "PRODUCT_FLOW.INPUT_POSITION",   &input_phase,    8 );
    ADS_init_parameter( "PRODUCT_FLOW.GOOD_POSITION",    &outgood_phase,  8 );
    ADS_init_parameter( "PRODUCT_FLOW.REJECT1_POSITION", &outrej1_phase,  8 );
    ADS_init_parameter( "PRODUCT_FLOW.REJECT2_POSITION", &outrej2_phase,  8 );

    #ifdef HGA_LEAKTEST   // i_hga_input
      ADS_init_parameter( ".I_HGA_INPUT", &hga_input,  2 );
	  ADS_init_parameter( ".HGA_VALUE", &hga_value,  4 );
    #endif


  }

  ADS_update_list();

  TRACE("completed - %d objects created\n", uplist_num );

	return 0; // Successfully
}




int ADS_update_PLCreadings( wchar_t* text )
{
  int len = 0;
  double carph = 0.0;
  double inpph = 0.0;

  if ( carousel_phase.handle )
  {
    ADS_read_parameter( &carousel_phase );
    carph = *((double*)carousel_phase.val);
    swprintf( text, string_1870[lng], carph );
  }

  len = wcslen( text );
  if ( input_phase.handle )
  {
    ADS_read_parameter( &input_phase );
    inpph = *((double*)input_phase.val);

    #ifdef TIVS_A20L
      swprintf( text+len, string_1871[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,2.0) );
    #endif

//Pour added 20180523
	#ifdef TIVS_A20
      swprintf( text+len, string_1871[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,2.0) );
    #endif
//Pour added 20180523

//Pour added 20180124
	#ifdef TIVS_A40L
      swprintf( text+len, string_1872[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0) );
    #endif
//Pour added 20180124

    #ifdef TIVS_C20
      swprintf( text+len, string_1871[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,2.0) );
    #endif

    //#ifdef TIVS_S40
    //  swprintf( text+len, string_1871[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,2.0), fmod(inpph,2.0) );
    //#endif

	#ifdef TIVS_S40 // L" Inp ph (%d) @3=%.2lf @2=%.2lf"
      swprintf( text+len, string_1871[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,2.0) );
    #endif

	//#ifdef TIVS_S60 // L" Inp ph (%d) @3=%.2lf @2=%.2lf"
 //     swprintf( text+len, string_1872[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0) );
 //   #endif

	  	#ifdef TIVS_S60 // L" Inp ph (%d) @3=%.2lf @2=%.2lf"
      swprintf( text+len, string_1872a[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0), fmod(inpph,18.0) );
    #endif

    #ifdef TIVS_A60
	  //Pour added for A60L 20180830
	#if SERIAL_N==201600007  ||SERIAL_N== 201900020
      swprintf( text+len, string_1873[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,6.0) );
	//#endif
	//Pour marked for testing 20180913
	#else
	  swprintf( text+len, string_1873[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,6.0) );
    #endif

	#endif
    //Pour marked/removed for testing 20180913

    #if defined TIVS_A40 || defined TIVS_F40  || defined TIVS_F20
      swprintf( text+len, string_1872[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0) );
    #endif

    #ifdef TIVS_C60
      swprintf( text+len, string_1872[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0) );
    #endif

    #ifdef TIVS_C40
      //swprintf( text+len, string_1872[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0), fmod(inpph,4.0) );
      swprintf( text+len, string_1875[lng], int(0.5+1000.0*fmod(inpph,1.0)), fmod(inpph,3.0) );
    #endif

  }

  len = wcslen( text );
  if ( input_width.handle )
  {
    ADS_read_parameter( &input_width );
    swprintf( text+len, string_1874[lng], *((double*)input_width.val) );
  }

  len = wcslen( text );
  swprintf( text+len, string_1876[lng], carph-inpph );

  double dist = 0.0;
  if ( outrej1_phase.handle )
  {
    ADS_read_parameter( &outrej1_phase );
    dist = *((double*)outrej1_phase.val) - inpph;
    if ( dist > 0.0 && dist < 100.0 )
    {
      len = wcslen( text );
      swprintf( text+len, string_1878[lng], dist );
    }
  }

  len = wcslen( text );
  if ( outrej2_phase.handle )
  {
    ADS_read_parameter( &outrej2_phase );
    dist = *((double*)outrej2_phase.val) - inpph;
    if ( dist > 0.0 && dist < 100.0 )
    {
      len = wcslen( text );
      swprintf( text+len, string_1880[lng], dist );
    }
  }

  len = wcslen( text );
  if ( outgood_phase.handle )
  {
    ADS_read_parameter( &outgood_phase );
    dist = *((double*)outgood_phase.val) - inpph;
    if ( dist > 0.0 && dist < 100.0 )
    {
      len = wcslen( text );
      swprintf( text+len, string_1882[lng], dist );
    }

//    swprintf( text+len, L" - Inp-Good= %.2lf", *((double*)outgood_phase.val) - inpph );
  }

  len = wcslen( text );
  if ( encoder_height.handle )
  {
    ADS_read_parameter( &encoder_height );
    swprintf( text+len, string_1884[lng], *((UINT*)encoder_height.val) );
  }


  #ifdef HGA_LEAKTEST   // i_hga_input
    len = wcslen( text );
    if ( hga_input.handle )
    {
      ADS_read_parameter( &hga_input );
      swprintf( text+len, string_1962[lng], *((short int*)hga_input.val) );
    }

		len = wcslen( text );
    if ( hga_value.handle )
    {
      ADS_read_parameter( &hga_value );
      swprintf( text+len, string_1963[lng], *((float*)hga_value.val) );
    }

  #endif


/*  len = wcslen( text );
  if ( input_good_err.handle )
  {
    ADS_read_parameter( &input_good_err );
    swprintf( text+len, string_1886[lng], *((double*)input_good_err.val) );
  }

  len = wcslen( text );
  if ( input_rej1_err.handle )
  {
    ADS_read_parameter( &input_rej1_err );
    swprintf( text+len, string_1888[lng], *((double*)input_rej1_err.val) );
  }

  len = wcslen( text );
  if ( input_rej2_err.handle )
  {
    ADS_read_parameter( &input_rej2_err );
    swprintf( text+len, string_1890[lng], *((double*)input_rej2_err.val) );
  }*/


  return 0;
}

//Pour moved 20180322
int ADS_read_hvdata( void )
{
 //Pour removed 20180319
/*
      //ADS_init_parameter( ".HVBLOCK_4.PTR",                 &hvblock4_ptr, 1 );
	ADS_init_parameter( "HV_MANAGER.PTR4",                 &hvblock4_ptr, 1 );
  if ( hvblock4_ptr.handle ) 
  {
    ADS_read_parameter( &hvblock4_ptr );
    block4_ptr = *((BYTE*)hvblock4_ptr.val);
  }
  */
/*
  for ( UINT i=0 ; i<4 ; i++ )
  {
	 
    if ( hv_ndata[i].handle ) 
    {
      ADS_read_parameter( &hv_ndata[i] );
      ndata[i] = *((short*)hv_ndata[i].val);
    }

    if ( hv_vmin[i].handle ) 
    {
      ADS_read_parameter( &hv_vmin[i] );
      vmin[i] = *((short*)hv_vmin[i].val);
    }

    if ( hv_vmax[i].handle ) 
    {
      ADS_read_parameter( &hv_vmax[i] );
      vmax[i] = *((short*)hv_vmax[i].val);
    }

  }
  */

	/*
	int i = 0;
	int label_old = 0;
	int index = 0;
			  for(i=1; i<HV_MANAGER_PTR4-1; i++)
			  {
				if(label_old < hvdata_p[i].label )
				 {
				  if( hvdata_p[i].HV4_V_Min != 0 && hvdata_p[i].HV4_V_Max != 0 )
				  index = i;
				  label_old = hvdata_p[i].label;
				 }
			  }
	*/

  return 0;
  

  
}
//Pour moved 20180322


int ADS_update_HVreadings( wchar_t* text, UINT* color )
{
  int len = 0;
  int i;

  if ( *((bool*)f_hvloop_a->val) ||*((bool*)f_hvloop_b->val) ||*((bool*)f_hvloop_c->val) ||*((bool*)f_hvloop_d->val) ) // loop mode active //Pour added BCD 20180302
  {

  short volt_in[4];
  short curr_in[4];

  for ( i=0 ; i<4 ; i++ )
  {
    if ( hv_voltin[i].handle ) 
    {
      ADS_read_parameter( &hv_voltin[i] );
      volt_in[i] = *((short*)hv_voltin[i].val);
    }

    if ( hv_currin[i].handle ) 
    {
      ADS_read_parameter( &hv_currin[i] );
      curr_in[i] = *((short*)hv_currin[i].val);
    }
  }

  swprintf( text, L"LOOP HV1:%4.2lfkV/%4.2lfmA HV2:%4.2lfkV/%4.2lfmA HV3:%4.2lfkV/%4.2lfmA HV4:%4.2lfkV/%4.2lfmA", 
    INT2VDSP(volt_in[0]), INT2VDSP(curr_in[0]),
    INT2VDSP(volt_in[1]), INT2VDSP(curr_in[1]),
    INT2VDSP(volt_in[2]), INT2VDSP(curr_in[2]),
    INT2VDSP(volt_in[3]), INT2VDSP(curr_in[3]) );
  }
  else // production mode
  {
	  
    //ADS_read_hvdata();  //Pour removed for testing 20180317

	/*
	//Pour added 20180302 
	  swprintf( text, L"PROD HV1:%4.2lf/%4.2lf HV2:%4.2lf/%4.2lf HV3:%4.2lf/%4.2lf HV4:%4.2lf/%4.2lf ", 
      INT2VDSP(vmin[0]), INT2VDSP(vmax[0]), 
      INT2VDSP(vmin[1]), INT2VDSP(vmax[1]), 
      INT2VDSP(vmin[2]), INT2VDSP(vmax[2]), 
      INT2VDSP(vmin[3]), INT2VDSP(vmax[3]) );
	//Pour added 20180303
	*/
/*
	//Pour added 20180412
	//ADS_read_hvdata_array();
	  swprintf( text, L"PROD HV1:%4.2lf/%4.2lf HV2:%4.2lf/%4.2lf HV3:%4.2lf/%4.2lf HV4:%4.2lf/%4.2lf ", 
      INT2VDSP(hv1_vmin), INT2VDSP(hv1_vmax), 
      INT2VDSP(hv2_vmin), INT2VDSP(hv2_vmax), 
      INT2VDSP(hv3_vmin), INT2VDSP(hv3_vmax), 
      INT2VDSP(hv4_vmin), INT2VDSP(hv4_vmax) );
	//Pour added 20180412
*/

	if (hv1min_err || hv1max_err || hv2min_err || hv2max_err|| hv3min_err || hv3max_err || hv4min_err || hv4max_err)
	{
		if ( vmin[0]<*((short*)hv1min_err->val) || vmax[0]>*((short*)hv1max_err->val) ||
			 vmin[1]<*((short*)hv2min_err->val) || vmax[1]>*((short*)hv2max_err->val) ||
			 vmin[2]<*((short*)hv3min_err->val) || vmax[2]>*((short*)hv3max_err->val) ||
			 vmin[3]<*((short*)hv4min_err->val) || vmax[3]>*((short*)hv4max_err->val)) *color = TXT_DARKRED;
	}
	//Pour added 20180303
	
  }

  #ifdef HGA_LEAKTEST   // i_hga_input
    len = wcslen( text );
    if ( hga_input.handle )
    {
      ADS_read_parameter( &hga_input );
      swprintf( text+len, string_1962[lng], *((short int*)hga_input.val) );
    }

	len = wcslen( text );
    if ( hga_value.handle )
    {
      ADS_read_parameter( &hga_value );
      swprintf( text+len, string_1963[lng], *((float*)hga_value.val) );
    }

  #endif
  return 0;
}


extern BOOL              show_rejects;


// refresh all elements in the list after reading from PLC
int ADS_update_list( void )
{
  if ( ADS_status != 1 ) return 99; // communication NON active

  wchar_t  text[300+1];

  // Start cycle to read and show ALL parameters about statistics
  for ( UINT i=0 ; i<uplist_num ; i++ )
  {
    if ( ADS_read_parameter( uplist[i].ADS ) )
    {
      uplist[i].text_st->SetWindowTextW( L"ERROR RD" );
    }
    else
    {

      if ( layout_dsp && layout_dsp->visible ) // LAYOUT SHOWN
      {

        if ( uplist[i].perc_base && *((UINT*)uplist[i].perc_base) && *((UINT*)uplist[i].ADS->val) )
        {
          swprintf( text, 300, L"%s: %s (%.3lf%%)", uplist[i].desc_txt, thousands(*((UINT*)uplist[i].ADS->val)), 100.0*(*(UINT*)uplist[i].ADS->val)/(*(UINT*)uplist[i].perc_base) );
        }
        else
        {
          swprintf( text, 300, L"%s: %s", uplist[i].desc_txt, thousands(*((UINT*)uplist[i].ADS->val)) );
        }
        if ( uplist[i].item_rt ) uplist[i].item_rt->SetText( text );

      }
      else // CAMERA GROUP SHOWN
      {

        if ( uplist[i].perc_base && *((UINT*)uplist[i].perc_base) && *((UINT*)uplist[i].ADS->val) )
        {
          swprintf( text, 300, L"%s (%.3lf%%)", thousands(*((UINT*)uplist[i].ADS->val)), 100.0*(*(UINT*)uplist[i].ADS->val)/(*(UINT*)uplist[i].perc_base) );
        }
        else
        {
          swprintf( text, 300, L"%s", thousands(*((UINT*)uplist[i].ADS->val)) );
        }
        uplist[i].text_st->SetWindowTextW( text );

      }
    }
    Sleep(0);
  }
  
  // If enabled display CALIBRATION values on PC1
//  if ( uplist[uplist_num+1].desc_st )
//  if ( IS_PC1 )
//  {

 // CFont small_font;
 // CFont big_font;


 // static BOOL checked_old = FALSE;

  refresh_delay = 1000;


  if ( calibration_menu )
  {
    static wchar_t old[300+1] = L"";
    if ( calibration_menu->checked )
    {
      refresh_delay = 100;
      if ( machine_txt && machine_txt->m_hWnd )
      {
        if ( old[0]==L'\0' ) machine_txt->GetWindowTextW( old, 300 );
        ADS_update_PLCreadings( text );
        machine_txt->SetText( text, 0 ); 
      }
      //uplist[uplist_num+1].desc_st->SetWindowTextW( text );
    }
    else
    {
      if ( old[0]!=L'\0' )
      {
        if ( machine_txt ) machine_txt->SetText( old, 0 );
        old[0]=L'\0';
      }
    }
  }

  if ( hv_menu )
  {
    UINT txt_color = 0;

    static wchar_t old[300+1] = L"";
    if ( hv_menu->checked )
    {
      refresh_delay = 100;
      if ( machine_txt && machine_txt->m_hWnd )
      {
        if ( old[0]==L'\0' ) machine_txt->GetWindowTextW( old, 300 );
        ADS_update_HVreadings( text, &txt_color );
        machine_txt->SetText( text, txt_color ); 
      }
      //uplist[uplist_num+1].desc_st->SetWindowTextW( text );
    }
    else
    {
      if ( old[0]!=L'\0' )
      {
        if ( machine_txt && ( !show_rejects || ( txt_color==TXT_DARKRED ) ) ) machine_txt->SetText( old, txt_color );
        old[0]=L'\0';
      }
    }
  }

    //else uplist[uplist_num+1].desc_st->SetWindowTextW( L"" );
  Sleep(0);
//  }

	return 0; // Successfully completed
}



int ADS_terminate_updatelist( void )
{
  TRACE("ADS_terminate_updatelist.....");

  for ( UINT i=0 ; i<MAX_UPLIST_NUM ; i++ )
  {
    if ( uplist[i].desc_st )
    {
      delete uplist[i].desc_st;
      uplist[i].desc_st = NULL;
    }

    if ( uplist[i].text_st )
    {
      delete uplist[i].text_st;
      uplist[i].text_st = NULL;
    }

    if ( uplist[i].ADS )
    {
      if ( uplist[i].ADS->val )
      {
        free( uplist[i].ADS->val );
        uplist[i].ADS->val = NULL;
      }
      delete uplist[i].ADS;
      uplist[i].ADS = NULL;
    }

    if ( uplist[i].item_rt )
    {
      delete uplist[i].item_rt;
      uplist[i].item_rt = NULL;
    }

  }

  if ( carousel_phase.val  ) free( carousel_phase.val );
  if ( encoder_height.val  ) free( encoder_height.val );
  if ( input_width.val     ) free( input_width.val    );
  if ( input_phase.val     ) free( input_phase.val    );
  if ( outgood_phase.val   ) free( outgood_phase.val  );
  if ( outrej1_phase.val   ) free( outrej1_phase.val  );
  if ( outrej2_phase.val   ) free( outrej2_phase.val  );

  #ifdef HGA_LEAKTEST
    if ( hga_input.val   ) free( hga_input.val  );
  #endif

  #ifdef TIVS_A20L // HV readings to display
    for ( UINT i=0 ; i<4 ; i++ )
    {
      if ( hv_voltin[i].val ) free( hv_voltin[i].val );
      if ( hv_currin[i].val ) free( hv_currin[i].val );

      if ( hv_ndata[i].val ) free( hv_ndata[i].val );
      if ( hv_vmin [i].val ) free( hv_vmin [i].val );
      if ( hv_vmax [i].val ) free( hv_vmax [i].val );
      if ( hv_error[i].val ) free( hv_error[i].val );
    }

    if ( hv_errortot.val  ) free( hv_errortot.val );
    if ( hvblock4_ptr.val ) free( hvblock4_ptr.val );
  #endif

//Pour added 20180124
	  #ifdef TIVS_A40L // HV readings to display
    for ( UINT i=0 ; i<4 ; i++ )
    {
      if ( hv_voltin[i].val ) free( hv_voltin[i].val );
      if ( hv_currin[i].val ) free( hv_currin[i].val );

      if ( hv_ndata[i].val ) free( hv_ndata[i].val );
      if ( hv_vmin [i].val ) free( hv_vmin [i].val );
      if ( hv_vmax [i].val ) free( hv_vmax [i].val );
      if ( hv_error[i].val ) free( hv_error[i].val );
    }

    if ( hv_errortot.val  ) free( hv_errortot.val );
    if ( hvblock4_ptr.val ) free( hvblock4_ptr.val );
	/*
	//Pour added 20180329
	if ( hv_manager_ptr1.val ) free( hv_manager_ptr1.val );
	if ( hv_manager_ptr2.val ) free( hv_manager_ptr2.val );
	if ( hv_manager_ptr3.val ) free( hv_manager_ptr3.val );
	if ( hv_manager_ptr4.val ) free( hv_manager_ptr4.val );
	//Pour added 20180329
	*/
  #endif
//Pour added 20180124

  DeleteObject( font_stats );

  TRACE("completed\n");

	return 0; // Successfully
}







// Function to convert ADS state from number <val> to text_out string
// Returns 0 if no error, 10 if input code is unknown
//char* Ads_state_desc( int val )
wchar_t* Ads_state_desc( int val )
{
  switch ( val )
  {
    case 0:
      return( L"ADSSTATE_INVALID" );
    case 1:
      return( L"ADSSTATE_IDLE");
    case 2:
      return( L"ADSSTATE_RESET");
    case 3:
      return( L"ADSSTATE_INIT");
    case 4:
      return( L"ADSSTATE_START");
    case 5:
      return( L"ADSSTATE_RUN");
    case 6:
      return( L"ADSSTATE_STOP");
    case 7:
      return( L"ADSSTATE_SAVECFG");
    case 8:
      return( L"ADSSTATE_LOADCFG");
    case 9:
      return( L"ADSSTATE_POWERFAILURE");
    case 10:
      return( L"ADSSTATE_POWERGOOD");
    case 11:
      return( L"ADSSTATE_ERROR");

    default:
      return( L"unknown ADS status!");
  }
//  return( 10 );
}

// Function to convert ADS error code from number <val> to <ADS_text> string
// Returns 0 if no error, 10 if input code is unknown
//char* ADS_error_desc( int nErr )
//int Error_code_desc( int nErr )
wchar_t* ADS_error_desc( int nErr )
{
  switch ( nErr )
  {
    case 1:
      return( L"Internal error");
    case 2:
      return( L"No Rtime");
    case 3:
      return( L"Allocation locked memory error");
    case 4:
      return( L"Insert mailbox error");
    case 5:
      return( L"Wrong receive HMSG");
    case 6:
      return( L"target port not found");
    case 7:
      return( L"target machine not found");
    case 8:
      return( L"Unknown command ID");
    case 9:
      return( L"Bad task ID");
    case 10:
      return( L"No IO");
    case 11:
      return( L"Unknown AMS command");
    case 12:
      return( L"Win 32 error");
    case 13:
      return( L"Port not connected");
    case 14:
      return( L"Invalid AMS length");
    case 15:
      return( L"Invalid AMS Net ID");
    case 16:
      return( L"Low Installation level");
    case 17:
      return( L"No debug available");
    case 18:
      return( L"Port disabled");
    case 19:
      return( L"Port already connected");
    case 20:
      return( L"AMS Sync Win32 error");
    case 21:
      return( L"AMS Sync Timeout");
    case 22:
      return( L"AMS Sync AMS error");
    case 23:
      return( L"AMS Sync no index map");
    case 24:
      return( L"Invalid AMS port");
    case 25:
      return( L"No memory");
    case 26:
      return( L"TCP send error");
    case 27:
      return( L"Host unreachable");
    case 1280:
      return( L"Router: no locked memory");
    case 1282:
      return( L"Router: mailbox full");
    case 1792:
      return( L"error class <device error>");
    case 1793:
      return( L"Service is not supported by server");
    case 1794:
      return( L"invalid index group");
    case 1795:
      return( L"invalid index offset");
    case 1796:
      return( L"reading/writing not permitted");
    case 1797:
      return( L"parameter size not correct");
    case 1798:
      return( L"invalid parameter value(s)");
    case 1799:
      return( L"device is not in a ready state");
    case 1800:
      return( L"device is busy");
    case 1801:
      return( L"invalid context (must be in Windows)");
    case 1802:
      return( L"out of memory");
    case 1803:
      return( L"invalid parameter value(s)");
    case 1804:
      return( L"not found (files, ...)");
    case 1805:
      return( L"syntax error in command or file");
    case 1806:
      return( L"objects do not match");
    case 1807:
      return( L"object already exists");
    case 1808:
      return( L"symbol not found");
    case 1809:
      return( L"symbol version invalid");
    case 1810:
      return( L"server is in invalid state");
    case 1811:
      return( L"AdsTransMode not supported");
    case 1812:
      return( L"Notification handle is invalid");
    case 1813:
      return( L"Notification client not registered");
    case 1814:
      return( L"no more notification handles");
    case 1815:
      return( L"size for watch too big");
    case 1816:
      return( L"device not initialized");
    case 1817:
      return( L"device has a timeout");
    case 1818:
      return( L"query interface failed");
    case 1819:
      return( L"wrong interface required");
    case 1820:
      return( L"class ID is invalid");
    case 1821:
      return( L"object ID is invalid");
    case 1822:
      return( L"request is pending");
    case 1823:
      return( L"request is aborted");
    case 1824:
      return( L"signal warning");
    case 1825:
      return( L"invalid array index");
    case 1826:
      return( L"symbol not active -> release handle and try again");
    case 1827:
      return( L"access denied");
    case 1856:
      return( L"Error class <client error>");
    case 1857:
      return( L"invalid parameter at service");
    case 1858:
      return( L"polling list is empty");
    case 1859:
      return( L"var connection already in use");
    case 1860:
      return( L"invoke ID in use");
    case 1861:
      return( L"timeout elapsed");
    case 1862:
      return( L"error in win32 subsystem");
    case 1864:
      return( L"ads-port not opened");
    case 1872:
      return( L"internal error in ads sync");
    case 1873:
      return( L"hash table overflow");
    case 1874:
      return( L"key not found in hash");
    case 1875:
      return( L"no more symbols in cache");
    case 11001:
      return( L"Unable to find PLC/ADS system");

    default:
      return( L"unknown error code!");
  }
//  return( 10 );
}





