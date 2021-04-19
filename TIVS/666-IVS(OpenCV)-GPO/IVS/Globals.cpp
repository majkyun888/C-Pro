#include "stdafx.h"
#include "Globals.h"
#include "Output_data.h"
//#include "Manual_jog.h"


#include "Logger.h"


//#include "Bottom_inspection.h"


#ifdef IMAGE_DISPLAY
  extern  CImage_display* image_dsp[NUM_TLC];
//  #define SCALE_PT(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_img))( img )
//  #define SCALE_IMG(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_img))( img )
//  #define DRAW_PT(nimg,x,y,col) (image_dsp[nimg]->*(image_dsp[nimg]->/draw_pt))(x,y,col) // 0xFFRRGGBB
#endif

#define STRING_LENGTH 50
#define MAX_CONFIG_FILES 50

UINT      lng; // Lingua attuale

BOOL      test_process = FALSE;  // if true TEST PROCESS BUTTON WILL BE VISIBLE/AVAILABLE

wchar_t** groupnames[5] = { string_538, string_undef, string_undef, string_undef, string_undef }; // 0=layout + other cameras groups
//wchar_t** groupnames[6] = { string_538, string_undef, string_undef, string_undef, string_undef, string_undef }; // 0=layout + other cameras groups

wchar_t   msg_err[201];

BOOL  show_rejects    = FALSE; // FALSE=show all images / TRUE=show only rejects

BOOL  nopart1         = FALSE; // Command line to disable particle 1
BOOL  nopart2         = FALSE; // Command line to disable particle 2
BOOL  nopart3         = FALSE; // Command line to disable particle 3

BOOL  tipcap_tip      =  TRUE; // used only in combined tip/cap mode, when true = TIP mode / false = CAP mode
BOOL  tipcap_freerun  = FALSE; // used only in combined tip/cap mode, when true = TIP/CAP free running enabled

BOOL  stn1_part_back  = FALSE; // In case of selectable TRUE/FALSE BACK/BOTTOM LIGHT particle station 1 

BOOL  pseudo_colors   = FALSE; // FALSE=gray levels / TRUE=pseudocolors
BOOL  liquid_mode     =  TRUE; // 0=freezedried mode / 1=liquid mode (COMBO MACHINE)

BOOL f_run_automatic  = TRUE; //Pour 20200702

//int   npar_lqfdmode =     0;

CButton_mod*      lqfdmode_txt = NULL;

UINT* lut32 = NULL;  // Pointer to current palette
UINT  lut32_bw[256]; // BW Palette
UINT  lut32_pc[256]; // Pseudocolor palette

//Pour 20201022
CMenu_button*     XTS1_1_btn    = NULL;
CMenu_button*     XTS1_2_btn    = NULL;

//Pour 20201022

CMenu_button*     inspection_btn    = NULL;
CMenu_button*     exit_btn          = NULL;
CMenu_button*     movie_btn         = NULL;
CMenu_button*     recipe_btn        = NULL;
CMenu_button*     chview_btn        = NULL;

CMenu_button*     inspmode_btn      = NULL;


CMenu_button*     datarec_btn       = NULL;

CMenu_button*     inpdatarec_btn    = NULL;
CMenu_button*     hvdatarec_btn     = NULL;
CMenu_button*     hgadatarec_btn    = NULL;

//Pour added 20171114
CMenu_button*     idpassword_btn    = NULL;
//Pour added 20171114

CMenu_button*     campwr_btn        = NULL;

CMenu_button*     changemode_btn    = NULL;


CMenu_button*     loopinsp_btn      = NULL;
CMenu_button*     saveinsp_btn      = NULL; // when active, saves all inspections screenshots
CMenu_button*     saveseq_btn       = NULL; // when active, saves inspection's sequence of images
CMenu_button*     saveimages_btn    = NULL;



CMenu_button*     batch_btn         = NULL;

CParameter* ptr_enable1_part1 = NULL;
CParameter* ptr_enable1_part2 = NULL;
CParameter* ptr_enable1_cosm  = NULL;
CParameter* ptr_enable2_part1 = NULL;
CParameter* ptr_enable2_part2 = NULL;
CParameter* ptr_enable2_cosm  = NULL;
CParameter* ptr_enable1_cont1 = NULL;
CParameter* ptr_enable1_cont2 = NULL;
CParameter* ptr_enable2_cont1 = NULL;
CParameter* ptr_enable2_cont2 = NULL;


//CMenu_button*     lqfdmode_btn      = NULL;


//BOOL              learn_reflections = FALSE;
//BOOL              learn_models      = FALSE;

BOOL              learning          = FALSE;
BOOL              datarec_on        = FALSE;

BOOL              inpdatarec_on     = FALSE;
BOOL              hgadatarec_on     = FALSE;

BOOL              hvdatarec_on      = FALSE;

BOOL              saveinsp_on       = FALSE; // when active, saves all inspections screenshots
BOOL              saveseq_on        = FALSE; // when active, saves inspection's sequence of images
//Pour 20190513
//extern INT        imagesavemode     =  2;  //0:Disabled; 1:Save only good images; 2:Save only bad images;3:Save all images;
//Pour 20190513


Results_manager*  input_result      = NULL;
Results_manager*  fillev_result     = NULL;
//Pour opened 20180224
//Results_manager*  hvdata_result      = NULL;

BYTE              freezemode_on     = 0; // used for C40/C60


Results_manager*  result            = NULL;
Results_manager*  result2           = NULL; // only used for particle3
Results_manager*  cap_result        = NULL; // only used for CAP data   (FD_DATAREC defined)
Results_manager*  bottomA_result    = NULL; // only used for BOTTOM A   (FD_DATAREC defined)
Results_manager*  bottomB_result    = NULL; // only used for BOTTOM B   (FD_DATAREC defined)

//wchar_t           machine_name[20+1]= L"nomachinename";
char              serial_number[20+1]= "nomachinename";


update_data_type        uplist[MAX_UPLIST_NUM];
UINT                    uplist_num = 0;


#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing
  BYTE*        img_ptr = NULL;
  //CEvent*      processing_start;
#endif


 int pwd_levels[] = { 2 ,   //  0 - automation button
                      2 ,   //  1 - inspection button
                      1 ,   //  2 - recipes 
                      1 ,   //  3 - start/stop inspection
                      1 ,   //  4 - utilities
                      1 ,   //  5 - exit
                      1 ,   //  6 - batches
                      4 ,   //  7 - calibration
                      1 ,   //  8 - load recipe
                      2 ,   //  9 - save recipe   //BIOCAD 008Vc -3, 2 for others,1 for Baiyu
                      2 };  // 10 - delete recipe //BIOCAD 008Vc -3, 2 for others,1 for Baiyu

  /*
#ifdef READ_CYCLES
  extern ADS_data_type          ADS_cycles; // ADS variable connected to cycles
  extern unsigned int           cycles;     // cycles read from PLC
  extern CTimer                 globaltimer;
  extern unsigned int           cycles_0;   // initial value
#endif
  */


extern UINT                      thread_status;
extern UINT                      thread_exit;
extern UINT                      num_tlc;
extern BOOL                      debug;
extern wchar_t                   PC_name[30]; // Computer name read from system configuration
extern ADS_data_type             trigger_param;
extern Camera_struct             camera[NUM_TLC];

//extern CPassword_manager*        password_mngr;

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif


extern CBatch_manager*           batch_mngr;
extern CImage_layout*            layout_dsp;
extern CSplash_screen*           info_wnd;

extern CAmpoule_filling_level*   filling_level[NUM_TLC];
extern CParticle_inspection*     particle_insp[NUM_TLC];
extern CCosmetic_inspection*     cosmetic_insp[NUM_TLC];
extern CFilling_level*           fillingl_insp[NUM_TLC]; // filling level inspection
extern CTip_inspection*          tip_insp     [NUM_TLC]; // ampoule's tip inspection
extern CBottom_inspection*       bottom_insp  [NUM_TLC]; // bottom inspection
extern CFOcolor_inspection*      focolor_insp [NUM_TLC]; // FO color inspection
extern CLQcolor_inspection*      lqcolor_insp [NUM_TLC]; // Liquid color inspection
extern CDRcolor_inspection*      drcolor_insp [NUM_TLC]; // dotring color inspection
extern COralFO_inspection*       oralfo_insp  [NUM_TLC]; // bottom inspection
extern CWaist_layer*             waistlayer_insp[NUM_TLC]; // waist-layer FD inspection
extern CSkinning*                skinning_insp  [NUM_TLC]; // skinning FD inspection
//Pour on 20201130
extern CFlange_inspection*       flange_insp [NUM_TLC]; // PFS flange inspection
//Pour on 20201130
extern CPFStopper_inspection*    pfstopper_insp   [NUM_TLC]; // PFS stopper inspection
extern CPFSidewall_inspection*   pfsidewall_insp  [NUM_TLC]; // PFS sidewall inspection (area sequence)
//extern CPFCosmetic3_inspection*  pfcosmetic3_insp [NUM_TLC]; // PFS cosmetic3 (bottom A&B)



#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*                alarm_logger;
#endif

extern CLogger*                  logger;
extern CBatch_manager*           batch_mngr;

extern CFile_mngmt               file_manager;
extern CText_box*                machine_txt;  // general status of machine/software


///////////////////////////////////////////////////////////////////////////////////////////////


#define MAXAWCONVSIZE 200

wchar_t* AtoW ( const char* a )
{
  ASSERT( a && AfxIsValidString(a) );
  size_t nc;
  static wchar_t w[MAXAWCONVSIZE+1];
  if ( a ) mbstowcs_s( &nc, w, MAXAWCONVSIZE, a, _TRUNCATE); // conversione char=>wchar_t
  return w;
}

char* WtoA( const wchar_t* w )
{
  ASSERT( w && AfxIsValidString(w) );
  size_t nc;
  static char a[MAXAWCONVSIZE+1];
  if ( w ) wcstombs_s( &nc, a, MAXAWCONVSIZE, w, _TRUNCATE); // conversione wchar_t=>char
  return a;
}



#ifdef _DEBUG // Functions version to check index in debug
  int CHK_BIT( UINT var, UINT nbit)
  {
    ASSERT( nbit>=0 && nbit<32 );
    return( (var>>(nbit)) & 1 );
  }

  void SET_BIT( UINT& var, UINT nbit)
  {
    ASSERT( nbit>=0 && nbit<32 );
    var |= (1<<(nbit));
  }

  void CLR_BIT( UINT& var, UINT nbit)
  {
    ASSERT( nbit>=0 && nbit<32 );
    var &= ~(1<<(nbit));
  }

#endif

// MACROs TO SET, CLEAR AND TEST A SINGLE BIT INTO A INTEGER VAR
//#define SET_BIT(var,nbit) var|= (1<<(nbit))
//#define CLR_BIT(var,nbit) var&=~(1<<(nbit))
//#define CHK_BIT(var,nbit) ((var>>(nbit))&1)





int Message_box( CString& msg, int mode )
{
//	if ( info_wnd && info_wnd->m_hWnd ) info_wnd->DestroyWindow();
//	MessageBox( NULL , msg , "ERRORE:" , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
//	MessageBox( NULL , "test" , "ERRORE:" , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );

//		AfxMessageBox( msg , MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
//	AfxMessageBox( "test" , MB_OK );
	return Message_box( msg.GetBuffer(0), mode );
}



//int Message_box( char* msg, int mode )
int Message_box( wchar_t* msg, int mode )
{
	int  ret = IDOK;
	bool info_visible = info_wnd && info_wnd->m_hWnd && info_wnd->IsWindowVisible();

  if ( logger ) logger->Add_string( msg );
	
	if ( info_visible ) info_wnd->ShowWindow( SW_HIDE );

	switch ( mode )
	{
	  case MBOX_INFO:
		//ret = AfxGetMainWnd()->MessageBox( msg, "", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL );
		ret = AfxGetMainWnd()->MessageBox( msg, L"", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL );
//		ret = MessageBox( NULL, msg, "ERRORE:", MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
		break;

		case MBOX_ERROR:
		ret = AfxGetMainWnd()->MessageBox( msg, string_error[lng], MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
//		ret = MessageBox( NULL, msg, "ERRORE:", MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
		break;

	  case MBOX_WARNING:
		ret = AfxGetMainWnd()->MessageBox( msg, string_warning[lng], MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL );
//		ret = MessageBox( NULL, msg, "ATTENZIONE:", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL );
		break;

		case MBOX_YESNO:
		ret = AfxGetMainWnd()->MessageBox( msg, string_question[lng], MB_YESNO|MB_ICONQUESTION|MB_SYSTEMMODAL );
//		ret = MessageBox( NULL, msg, "RICHIESTA:", MB_YESNO|MB_ICONQUESTION|MB_SYSTEMMODAL );
		break;
	}

	if ( info_visible )
  {
    info_wnd->ShowWindow( SW_SHOW ); // se era visibile, la rivisualizzo
    info_wnd->RedrawWindow();
  }

	return ret;
}





/*
int Start_calibration( int inp )
{
  TRACE("START_CALIBRATION: CALIBRATION MODE REQUIRED\n");

  Inspection_startstop( 20 ); // Arresta l'ispezione
  Sleep(200);

  if ( ADS_read_byte( &ADS_calibration_mode ) ) // ALREADY TRUE, CLEAR IT
  {
    TRACE("START_CALIBRATION: calibration mode already ON\n");
  }
  else // FALSE, SET IT TO TRUE
  {
    ADS_write_value( ".START_VISION_CALIBRATION", TRUE, CTYPE_BOOL8 );
    TRACE("START_CALIBRATION: calibration bit set to TRUE\n");
  } 

  Sleep(50);

  if ( hauteur_results      ) hauteur_results->Clean();
  if ( neck_diam_results[0] ) neck_diam_results[0]->Clean();
  if ( body_diam_results[0] ) body_diam_results[0]->Clean();
  if ( body_diam_results[1] ) body_diam_results[1]->Clean();
  if ( body_diam_results[2] ) body_diam_results[2]->Clean();

  calibration_mode   =  TRUE;
  repeatability_mode = FALSE;

  TRACE("START_CALIBRATION: ADS_calibration_mode=%d\n",ADS_read_byte(&ADS_calibration_mode) );

  Sleep(200);
  Inspection_startstop( 10 ); // Riavvia l'ispezione

  return( 0 );
}
*/

/*
int Start_repeatability( int inp )
{
  TRACE("START_REPEATABILITY: REPEATABILITY TEST REQUIRED\n");
  if ( ADS_read_byte( &ADS_calibration_mode ) ) // ALREADY TRUE, CLEAR IT
  {
    TRACE("START_REPEATABILITY: calibration mode already ON\n");
  }
  else // FALSE, SET IT TO TRUE
  {
    ADS_write_value( ".START_VISION_CALIBRATION", TRUE, CTYPE_BOOL8 );
    TRACE("START_REPEATABILITY: calibration bit set to TRUE\n");
  } 

  Sleep(50);

  if ( hauteur_results      ) hauteur_results->Clean();
  if ( neck_diam_results[0] ) neck_diam_results[0]->Clean();
  if ( body_diam_results[0] ) body_diam_results[0]->Clean();
  if ( body_diam_results[1] ) body_diam_results[1]->Clean();
  if ( body_diam_results[2] ) body_diam_results[2]->Clean();

  calibration_mode   = FALSE;
  repeatability_mode =  TRUE;

  TRACE("START_REPEATABILITY: ADS_calibration_mode=%d\n",ADS_read_byte(&ADS_calibration_mode) );
  return( 0 );
}

*/

/*

int Gage_set_height( int inp )
{
  ADS_write_value( ".GAUGE_LIFTING", TRUE, CTYPE_BOOL8 );
  ADS_write_value( ".GAUGE_STATUS",     4, CTYPE_BYTE  );

  return( 0 );
}

int Gage_start_rotate( int inp )
{
  ADS_write_value( ".GAUGE_ROTATION", TRUE, CTYPE_BOOL8 );
  ADS_write_value( ".GAUGE_STATUS",     4, CTYPE_BYTE  );
  return( 0 );
}

*/

/*
extern ADS_data_type  ADS_vision_lift;
extern ADS_data_type  ADS_vision_lift_tar;


int Vision_set_height( int inp )
{
  ADS_write_value( ".VISION_PLATE_LIFTING", TRUE, CTYPE_BOOL8 );
  ADS_write_value( ".VISION_STATUS",           4, CTYPE_BYTE  );

  while ( ADS_read_byte( &ADS_vision_lift ) )
  {
  	MSG msg; PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
    Sleep(1); // ca 1us con sleep(0) - ca 2ms con sleep(1) - ca 3ms con sleep(2)
  }

  ADS_read_parameter( &ADS_vision_lift_tar );
  *plate_height_cur = *plate_height_tar;
  plateh_mm = ps_2_mm( *plate_height_tar );

//  TRACE("Vision_plate_lift terminata: *plate_height_tar=%d/%.1lf - *plate_height_cur=%d/%.1lf\n", *plate_height_tar, ps_2_mm(*plate_height_tar), *plate_height_cur, ps_2_mm(*plate_height_tar) );

  return( 0 );
}

int Vision_start_rotate( int inp )
{
  ADS_write_value( ".VISION_PLATE_ROTATION", TRUE, CTYPE_BOOL8 );
  ADS_write_value( ".VISION_STATUS",            4, CTYPE_BYTE  );
  return( 0 );
}

*/


/*
qs converte da pix-body a mm
      double height_mm = (DIMY_ACQ-1.0-height)*scale_cf[0] - double(*plate_height_cur)*plateh_cf + plateh_ofs[0];


      la inverto:

      height  = DIMY_ACQ-1.0 - ( height_mm + plate_height_cur*plateh_cf - plateh_ofs[0] ) / scale_cf[0] 

      */

/*
// convert absolute height referred to current plate height to y(ind=0) or x(ind=1) image coordinates
// warning: returned 0.0 means that converted value falls outside of vertical range of camera/lens
int mm_2_pix( int ind, double height )
{
  ASSERT( ind==0 || ind==1 );
//  if ( ind ) return DOUBLETOINT(                ( height - plateh_ofs[ind] + plateh_cf*(*plate_height_cur) ) / scale_cf[ind] ); // neck
//  else       return DOUBLETOINT( DIMY_ACQ - 1 - ( height + plateh_cf*(*plate_height_cur) - plateh_ofs[ind] ) / scale_cf[ind] ); // body

// include quota piattello
//  if ( ind ) return DOUBLETOINT(                ( height - plateh_ofs[ind] + plateh_mm ) / scale_cf[ind] ); // neck
//  else       return DOUBLETOINT( DIMY_ACQ - 1 - ( height + plateh_mm - plateh_ofs[ind] ) / scale_cf[ind] ); // body

// NON include quota piattello: E' riferita a zero!
  if ( ind ) return DOUBLETOINT(                ( height - plateh_ofs[ind] ) / scale_cf[ind] ); // neck
  else       return DOUBLETOINT( DIMY_ACQ - 1 - ( height - plateh_ofs[ind] ) / scale_cf[ind] ); // body
}
*/

  /*
  #ifndef TEMP_TLC
  #error SISTEMARE!
  #endif
  */
//  else       return int( 0.5 + ( height - plateh_ofs[ind] + plateh_cf*(*plate_height_cur) ) / scale_cf[ind] ); // body
//  return newh;


/*
// convert y(ind=0) or x(ind=1) image coordinates into absolute height referred to current plate height
// warning: returned 0.0 means that converted value falls outside of vertical range of camera/lens
double pix_2_mm( int ind, double pix )
{
  ASSERT( ind==0 || ind==1 );
  if ( ind ) return (             pix  *scale_cf[ind]+plateh_ofs[ind] ); // neck
  else       return (( DIMY_ACQ-1-pix )*scale_cf[ind]+plateh_ofs[ind] ); // body
}

*/
  //  double newh = scale_cf[ind]*height + plateh_ofs[ind] - plateh_cf*(*plate_height_cur);
// include quota piattello
//  return( scale_cf[ind]*height + plateh_ofs[ind] - plateh_mm );
// NON include quota piattello - riferita a zero!
//  return( scale_cf[ind]*height + plateh_ofs[ind] );
//  if ( ind ) return (             pix  *scale_cf[ind]+plateh_ofs[ind] ); // neck
//  else       return (( DIMY_ACQ-1-pix )*scale_cf[ind]+plateh_ofs[ind] ); // body
//  return newh;
//}




/*  VERSIONI ORIGINALI LINEARI 

int mm_2_ps( double mm )
{
  static double p1_usr  =    0.0;
  static double p2_usr  =  100.0;
  static int    p1_int  =  60000;
  static int    p2_int  = 806666;
  static double usr2plc = double(p2_int-p1_int)/(p2_usr-p1_usr);
  //static double plc2usr = (p2_usr-p1_usr)/double(p2_int-p1_int);
  return( p1_int + DOUBLETOINT( usr2plc*(mm-p1_usr) ) );  //*plate_height_tar = p1_int + DOUBLETOINT( usr2plc*(height-p1_usr) );
}



double ps_2_mm( int ps )
{
  static double p1_usr  =    0.0;
  static double p2_usr  =  100.0;
  static int    p1_int  =  60000;
  static int    p2_int  = 806666;
  //static double usr2plc = double(p2_int-p1_int)/(p2_usr-p1_usr);
  static double plc2usr = (p2_usr-p1_usr)/double(p2_int-p1_int);
  return( p1_usr + (ps-p1_int)*plc2usr );
}

*/


#include <math.h>

/*
double offset_height(double mm)
{
  static double offset  = 0.199;   // ////////////////////////////////////////
  static double shift   = -1.8;    //parametri per la creazione del senso 
  static double width   = 0.09;    //di compensazione (intervallo 30 - 280 mm)
  static double rad     = 0.052;   // ////////////////////////////////////////
  static double r1_coef = 0.00542; //coefficiente angolare retta compensazione (intervallo 0- 30 mm)
  static double r1_off  = 0.0072;  //offset retta compensazione (intervallo 0 -30 mm)
  static double r2_coef = -0.00389;//coefficiente angolare retta compensazione (intervallo 300- 357 mm)
  static double r2_off  = 1.43736;  //offset retta compensazione (intervallo 300 -357 mm)
  if (mm > 0 && mm <= 30)
	  return (r1_coef*mm+r1_off);  //offset primo tratto
  
  if (mm > 30 && mm < 300)
	  return (sin((mm*rad)+shift)*width+offset); //offset tratto centrale

  if (mm >= 300 && mm <= 358)
	  return (r2_coef*mm+r2_off); //offset terzo tratto

  return 0;
}
*/

/*
int mm_2_ps( double mm )
{
  static double p1_usr  =    0.0;
  static double p2_usr  =  100.0;
  static int    p1_int  =  60000;
  static int    p2_int  = 806666;
  //double offset = offset_height(mm);
  static double usr2plc = double(p2_int-p1_int)/(p2_usr-p1_usr);
  //static double plc2usr = (p2_usr-p1_usr)/double(p2_int-p1_int);
  //return( p1_int + DOUBLETOINT( usr2plc*(mm-p1_usr) ) ); //*plate_height_tar = p1_int + DOUBLETOINT( usr2plc*(height-p1_usr)

//  return( p1_int + DOUBLETOINT( usr2plc*( (mm-offset)-p1_usr ) ) );  
  return( p1_int + DOUBLETOINT( usr2plc*( (mm - offset_height(mm) )-p1_usr ) ) );  


}

double ps_2_mm( int ps )
{
  static double p1_usr  =    0.0;
  static double p2_usr  =  100.0;
  static int    p1_int  =  60000;
  static int    p2_int  = 806666;
  double mm = 0.0;
  //static double usr2plc = double(p2_int-p1_int)/(p2_usr-p1_usr);
  static double plc2usr = (p2_usr-p1_usr)/double(p2_int-p1_int);
  //return ( p1_usr + (ps-p1_int)*plc2usr );
  mm = p1_usr + (ps-p1_int)*plc2usr;
  return ( mm + offset_height(mm));
}

*/







// Called whenever a shutter is changed/loaded to update its value in camera
int Set_camera( int ind )
{
  ASSERT( (UINT)ind>=0 && (UINT)ind<num_tlc );
  Basler_parameters( (UINT)ind );
  return( 0 );
}


int Set_area( int ind )
{
  ASSERT( (UINT)ind>=0 && (UINT)ind<num_tlc );
//  if ( areainsp[ind] && areainsp[ind]->m_hWnd ) areainsp[ind]->ShowWindow( SW_SHOW );
//  if ( areainsp[ind] && areainsp[ind]->m_hWnd ) areainsp[ind]->ShowWindow( SW_SHOW );
  TRACE("Set_area(%d)\n", ind);
  return( 0 );
}



/*

int Manual_jog( int inp )
{
/*	pView->Close_all_menus();
  CManual_jog jog_wnd(pView);
  jog_wnd.DoModal(); */
/*  return( 0 );
}*/



int Set_paths( int inp )
{
  if ( password_mngr ) password_mngr->Select_folders();
  return( 0 );
}


int Set_passwords( int inp )
{
  if ( password_mngr ) password_mngr->Change_passwords();
  return( 0 );
}


int HGAdata_record( int inp )
{
  CString txt; //, txt2;


  #ifdef HGA_DATAREC
  if ( hgadatarec_on )  // ENABLED: DISABLE IT
  {

    if ( IS_PC1 )
    {
      ADS_hgadata_deregcallback();
      hgadatarec_btn->SetWindowText( string_1970[lng] );
    }

    if ( logger ) logger->Add_string( L"Recording of HGA data ended" );

    hgadatarec_on = FALSE;
    TRACE( L"HGADATA_REC DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {

    if ( IS_PC1 )
    {
      ADS_hgadata_regcallback(); // enable callback 
      hgadatarec_btn->SetWindowText( string_1972[lng] );
      //txt.Format( L"Input sensor data" ); 
      //input_result = new Results_manager( txt.GetBuffer(), 20000, 4 );
    }

    if ( logger ) logger->Add_string( L"Recording of HGA data started" );

    hgadatarec_on = TRUE;
    TRACE( L"HGADATA_REC ENABLED\n" );
  }
  #endif

  return( 0 );
}


int Inpdata_record( int inp )
{
  CString txt; //, txt2;

  if ( IS_PC1 && input_result )
  {

    txt = L"INPUT_DATA";
    input_result->Create_csv( txt.GetBuffer(0), TRUE ); // create with header

    /*    label:     UDINT       :=     0;
          camcnt:    UDINT       :=     0;
          ldpos:     LREAL       :=     0;
          tcpos:     BYTE        :=     0;  (* 1/2/3/4 *)  */

    txt.Format( L"inspection,label,camcnt,loadpos,tcind\r\n" );
/*    for ( int i=1 ; i<=60 ; i++ )
    {
      txt2.Format(L"%02d,", i);
      txt.Append( txt2 );
    } */
   // txt.Append( L"\r\n" );

    input_result->Addrow_csv( txt.GetBuffer(0) );
    input_result->Save_csv();
    delete input_result;
    input_result = NULL;

  }

  if ( IS_PC2 && fillev_result )
  {
    txt = L"FILLEV_DATA";
    fillev_result->Create_csv( txt.GetBuffer(0), TRUE ); // create with header
    txt.Format( L"inspection,p2_a,p2_b,p2_c,p2_d\r\n" );
    fillev_result->Addrow_csv( txt.GetBuffer(0) );
    fillev_result->Save_csv();
    delete fillev_result;
    fillev_result = NULL;
  }




  #ifdef SAVE_INPUTDATA
  if ( inpdatarec_on )  // ENABLED: DISABLE IT
  {

    if ( IS_PC1 )
    {
      ADS_inputdata_deregcallback();
      inpdatarec_btn->SetWindowText( string_354[lng] );
    }
    else
    {
      inpdatarec_btn->SetWindowText( string_360[lng] );
    }

    inpdatarec_on = FALSE;
    TRACE( L"INPUTDATA_REC DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {

    if ( IS_PC1 )
    {
      ADS_inputdata_regcallback(); // enable callback 
      inpdatarec_btn->SetWindowText( string_356[lng] );
      txt.Format( L"Input sensor data" ); 
      input_result = new Results_manager( txt.GetBuffer(), 20000, 4 );
    }
    else // PC2 - filling level
    {
      inpdatarec_btn->SetWindowText( string_362[lng] );
      txt.Format( L"Filling level data" ); 
      fillev_result = new Results_manager( txt.GetBuffer(), 20000, 4 ); // npt p2_a/b/c/d
    }


    inpdatarec_on = TRUE;
    TRACE( L"INPUTDATA_REC ENABLED\n" );
  }
  #endif

  return( 0 );
}



extern CFile                  HV_data;  



int HVdata_record( int inp )
{
//  CString txt; //, txt2;
	//Pour removed 20180315
/*
  if ( hvdata_result )
  {

    txt = L"INPUT_DATA";
    hvdata_result->Create_csv( txt.GetBuffer(0), TRUE ); // create with header

    txt.Format( L"inspection,label,camcnt,loadpos,tcind\r\n" );
    txt.Append( L"\r\n" );

    hvdata_result->Addrow_csv( txt.GetBuffer(0) );
    hvdata_result->Save_csv();
    delete hvdata_result;
    hvdata_result = NULL;

  } 
  */
  wchar_t text[200];

  //Pour added 20180224
#ifdef HV_DATAREC

  //Pour added 20180224
  if ( hvdatarec_on )  // ENABLED: DISABLE IT
  {
	HV_data.Close(); //Pour checked 20180224
	  //if(IS_PC1)   //Pour added 20180224
	  {
    ADS_HVdata_deregcallback();
	//ADS_HVdata_deregcallback2();
	//ADS_HVdata_deregcallback3();
	//ADS_HVdata_deregcallback4();
    hvdatarec_btn->SetWindowText( string_3560[lng] );
	  }
	  //Pour added 20180224
	  if(logger) logger->Add_string(L"Recording of HV data ended");

    hvdatarec_on = FALSE;
    TRACE( L"HV DATA_REC DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {
	  //if(IS_PC1)
	  {
		  //Pour removed 20180303
		  //Pour context to be checked 20180307
    CTime time = CTime::GetCurrentTime();   
    ASSERT( HV_data.m_hFile == INVALID_HANDLE_VALUE );
	
    swprintf( text, 200, L"%s\\%s_%04d%02d%02d_%02d%02d%02d.csv", MAIN_DIRECTORY, L"HVDATA", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
    HV_data.Open( text, CFile::modeCreate|CFile::modeWrite );
	
	//Pour removed total error 20180303
    char txt[200];
    sprintf( txt, "       HV1           HV2         HV3         HV4       \r\n" );
    HV_data.Write( txt, strlen(txt) );
    sprintf( txt, "PTR -  MIN/MAX  -  MIN/MAX  -  MIN/MAX  -  MIN/MAX     \r\n" );
//  sprintf( txt, "xx  - x.xx x.xx/x.xx - x.xx x.xx/x.xx - x.xx x.xx/x.xx - x.xx x.xx/x.xx - x.xx  \r\n", 
//  sprintf( txt, "%2d  - %4.2lf %4.2lf/%4.2lf - %4.2lf %4.2lf/%4.2lf - %4.2lf %4.2lf/%4.2lf - %4.2lf %4.2lf/%4.2lf - %4.2lf\r\n", 
    HV_data.Write( txt, strlen(txt) );
	
	//Pour removed end 20180303
    ADS_HVdata_regcallback(); // enable callback //Pour opened 20180224

    hvdatarec_btn->SetWindowText( string_3560a[lng] );
    //txt.Format( L"HV data" ); //Pour opened 20180224
    //hvdata_result = new Results_manager( txt.GetBuffer(), 20000, 4 ); //Pour opened 20180224
	  }

	  if(logger) logger->Add_string(L"Recording of HV data started");

    hvdatarec_on = TRUE;
    TRACE( L"HV DATA_REC ENABLED\n" );
  }
#endif //Pour added 20180224
  return( 0 );
}


int Change_mode( int inp )
{
  //  CString txt, txt2;
  // return 0;
  // pView->Close_all_menus();

  int ind_part1 = 7;
  int ind_part2 = 8;

  #if defined THIRD_DEVIATOR // IF THIRD DEVIATOR ENABLED
    ind_part1 = 8;
    ind_part2 = 9;
  #endif


  TRACE("Called Change_mode(%d) - freezemode_on=%d - ", inp, freezemode_on );

  if ( inp>=0 ) freezemode_on = !freezemode_on; // toggle if inp>=0


  //if ( freezemode_on )  // freeze mode ENABLED //for C40 and C60
  if ( freezemode_on || !liquid_mode )  // freeze mode ENABLED //for C40 and C60
  //if ( !liquid_mode )//for C20 only  //Pour marked on 20210309
  {
//    freezemode_on = FALSE;
    if ( changemode_btn && changemode_btn->m_hWnd ) changemode_btn->SetWindowText( string_3310[lng] );
    ADS_write_value( ".F_FREEZEMODE_ON", TRUE, CTYPE_BOOL8 ); // Force flag into PLC low to enable LIQUID mode

/*    pView->Destroy_menu();
    CTimer timer;
    while ( timer.elapsed_sec()<100 )
    {
      MSG msg; 
      PeekMessage( &msg, NULL,  0, 0, PM_REMOVE);
      Sleep(5);
    }  
    pView->Create_menu_TIVS_C40(); */

    // update enable texts
    if ( ptr_enable1_part1 && ptr_enable1_part1->pt_flag ) ptr_enable1_part1->pt_flag->SetWindowText( string_1770[lng] ); // Sidecake to dev1
    if ( ptr_enable1_part2 && ptr_enable1_part2->pt_flag ) ptr_enable1_part2->pt_flag->SetWindowText( string_1796[lng] ); // Heel to dev1
    if ( ptr_enable2_part1 && ptr_enable2_part1->pt_flag ) ptr_enable2_part1->pt_flag->SetWindowText( string_1780[lng] ); // Sidecake to dev2
    if ( ptr_enable2_part2 && ptr_enable2_part2->pt_flag ) ptr_enable2_part2->pt_flag->SetWindowText( string_1798[lng] ); // Heel to dev2
    
    if ( ptr_enable1_cont1 && ptr_enable1_cont1->pt_flag ) ptr_enable1_cont1->pt_flag->SetWindowText( string_1776[lng] ); // BottomA to dev1
    if ( ptr_enable1_cont2 && ptr_enable1_cont2->pt_flag ) ptr_enable1_cont2->pt_flag->SetWindowText( string_1778[lng] ); // BottomB to dev1
    if ( ptr_enable2_cont1 && ptr_enable2_cont1->pt_flag ) ptr_enable2_cont1->pt_flag->SetWindowText( string_1786[lng] ); // BottomA to dev2
    if ( ptr_enable2_cont2 && ptr_enable2_cont2->pt_flag ) ptr_enable2_cont2->pt_flag->SetWindowText( string_1788[lng] ); // BottomB to dev2



    if ( uplist[ind_part1].desc_st ) // update particle1 / sidecake - particle2 / heel
    {
      uplist[ind_part1].desc_st->SetWindowTextW( string_sideckrej[lng] ); // Sidecake rej
      uplist[ind_part1].desc_txt = string_sideckrej[lng]; // Sidecake rej
    }
    if ( uplist[ind_part2].desc_st )
    {
      uplist[ind_part2].desc_st->SetWindowTextW( string_heelrej  [lng] ); // Heel rej
      uplist[ind_part2].desc_txt = string_heelrej[lng]; // Heel rej
    }


    TRACE( L"FREEZE MODE ENABLED\n" );
  }
  else               // liquid mode ENABLED
  {
    if ( changemode_btn && changemode_btn->m_hWnd ) changemode_btn->SetWindowText( string_3312[lng] );

    ADS_write_value( ".F_FREEZEMODE_ON", FALSE, CTYPE_BOOL8 ); // Force flag into PLC high to enable FREEZE/D mode

 /*   pView->Destroy_menu();
    CTimer timer;
    while ( timer.elapsed_sec()<100 )
    {
      MSG msg; 
      PeekMessage( &msg, NULL,  0, 0, PM_REMOVE);
      Sleep(5);
    } 
    pView->Create_menu_TIVS_C40(); */

    // update enable texts
    if ( ptr_enable1_part1 && ptr_enable1_part1->pt_flag ) ptr_enable1_part1->pt_flag->SetWindowText( string_1670[lng] ); // Particle1 to dev1
    if ( ptr_enable1_part2 && ptr_enable1_part2->pt_flag ) ptr_enable1_part2->pt_flag->SetWindowText( string_1672[lng] ); // Particle2 to dev1
    if ( ptr_enable2_part1 && ptr_enable2_part1->pt_flag ) ptr_enable2_part1->pt_flag->SetWindowText( string_1682[lng] ); // Particle1 to dev2
    if ( ptr_enable2_part2 && ptr_enable2_part2->pt_flag ) ptr_enable2_part2->pt_flag->SetWindowText( string_1683[lng] ); // Particle2 to dev2

    if ( ptr_enable1_cont1 && ptr_enable1_cont1->pt_flag ) ptr_enable1_cont1->pt_flag->SetWindowText( string_1674[lng] ); // Filling1 to dev1
    if ( ptr_enable1_cont2 && ptr_enable1_cont2->pt_flag ) ptr_enable1_cont2->pt_flag->SetWindowText( string_1675[lng] ); // Filling2 to dev1
    if ( ptr_enable2_cont1 && ptr_enable2_cont1->pt_flag ) ptr_enable2_cont1->pt_flag->SetWindowText( string_1684[lng] ); // Filling1 to dev2
    if ( ptr_enable2_cont2 && ptr_enable2_cont2->pt_flag ) ptr_enable2_cont2->pt_flag->SetWindowText( string_1685[lng] ); // Filling2 to dev2


    if ( uplist[ind_part1].desc_st ) //uplist[7].desc_st->SetWindowTextW( string_part1rej[lng] ); // Particle1 rej
    {
      uplist[ind_part1].desc_st->SetWindowTextW( string_part1rej[lng] ); // Particle1 rej
      uplist[ind_part1].desc_txt = string_part1rej[lng]; // Particle1 rej
    }
    if ( uplist[ind_part2].desc_st ) //uplist[8].desc_st->SetWindowTextW( string_part2rej[lng] ); // Particle2 rej
    {
      uplist[ind_part2].desc_st->SetWindowTextW( string_part2rej[lng] ); // Particle2 rej
      uplist[ind_part2].desc_txt = string_part2rej[lng]; // Particle2 rej
    }

    TRACE( L"LIQUID MODE ENABLED\n" );
  }

  /*if ( inp>=0 )*/ Message_box( string_3320[lng] );


  return( 0 );
}


int Data_record( int inp )
{
  CString txt, txt2;

  if ( result )
  {
#if defined TIVS_A40 || defined TIVS_A40L || defined TIVS_A60
    //#ifdef TIVS_A40 || TIVS_A60
      if ( IS_PC1 ) txt = L"PARTICLE1_DATA";
      if ( IS_PC2 ) txt = L"PARTICLE2_DATA";
      #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
        if ( IS_PC2 ) txt2 = L"PARTICLE3_DATA";
      #endif 
/*      if ( IS_PC1 ) result->Create_csv( L"PARTICLE1_DATA", TRUE ); // create with header
      if ( IS_PC2 ) result->Create_csv( L"PARTICLE2_DATA", TRUE ); // create with header
      #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
        if ( IS_PC2 ) result2->Create_csv( L"PARTICLE3_DATA", TRUE ); // create with header
      #endif */
    #endif//TIVS_A40,TIVS_A40L,TIVS_A60

    #ifdef TIVS_F40
      if ( IS_PC1 )
      {
        txt  = L"SIDEWALL_DATA";
        txt2 = L"CAKEVOLUME_DATA";
      }
      if ( IS_PC2 )
      {
        txt  = L"SIDECAKE_DATA";
        txt2 = L"TOPCAKE_DATA";
      }
    #endif

    result->Create_csv( txt.GetBuffer(0), TRUE ); // create with header
    if ( result2 ) result2->Create_csv( txt2.GetBuffer(0), TRUE ); // create with header

    txt.Format( L"Inspection num/Gripper num," );
	//Pour added for A60  on 20190723
#ifdef TIVS_A60
	for (int i = 1; i <= 90; i++)
	{
		txt2.Format(L"%02d,", i);
		txt.Append(txt2);
	}
	txt.Append(L"\r\n");

#endif // 

#ifdef TIVS_A40
    for ( int i=1 ; i<=60 ; i++ )
    {
      txt2.Format(L"%02d,", i);
      txt.Append( txt2 );
    }
    txt.Append( L"\r\n" );
#endif // 

    result->Addrow_csv( txt.GetBuffer(0) );
    result->Save_csv();
    delete result;
    result = NULL;

    if ( result2 )
    {
      result2->Addrow_csv( txt.GetBuffer(0) );
      result2->Save_csv();
      delete result2;
      result2 = NULL;
    }
  }

  #ifdef FD_DATAREC
    if ( cap_result && IS_PC1 )
    {
      cap_result->Create_csv( L"CAP_DATA", TRUE ); // create with header
      txt.Format( L"Inspection num,TC_A (TL),TC_B (TL),TC_C (TL),TC_A (BL),TC_B (BL),TC_C (BL),TC_D\r\n" );
      cap_result->Addrow_csv( txt.GetBuffer(0) );
      cap_result->Save_csv();
      delete cap_result;
      cap_result = NULL;
    }

    if ( bottomA_result && IS_PC1 )
    {
      bottomA_result->Create_csv( L"BOTTOM(A)_DATA", TRUE ); // create with header
      txt.Format( L"Inspection num/Gripper num," );
      for ( int i=1 ; i<=60 ; i++ )
      {
        txt2.Format(L"%02d,", i);
        txt.Append( txt2 );
      }
      txt.Append( L"\r\n" );
      bottomA_result->Addrow_csv( txt.GetBuffer(0) );
      bottomA_result->Save_csv();
      delete bottomA_result;
      bottomA_result = NULL;
    }

    if ( bottomB_result && IS_PC1 )
    {
      bottomB_result->Create_csv( L"BOTTOM(B)_DATA", TRUE ); // create with header
      txt.Format( L"Inspection num/Gripper num," );
      for ( int i=1 ; i<=60 ; i++ )
      {
        txt2.Format(L"%02d,", i);
        txt.Append( txt2 );
      }
      txt.Append( L"\r\n" );
      bottomB_result->Addrow_csv( txt.GetBuffer(0) );
      bottomB_result->Save_csv();
      delete bottomB_result;
      bottomB_result = NULL;
    }

  #endif



  if ( datarec_on )  // ENABLED: DISABLE IT
  {
    datarec_on = FALSE;
    datarec_btn->SetWindowText( string_304[lng] );
    TRACE( L"DATA_REC DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {
    datarec_on = TRUE;
    datarec_btn->SetWindowText( string_306[lng] );

    //#ifdef TIVS_A40
#if defined TIVS_A40 || defined TIVS_A40L || defined TIVS_A60
      if ( IS_PC1 ) txt.Format( L"Backlight (adsorption) particle inspection PC1"   ); //,   camera[ind-3].name );
      if ( IS_PC2 ) txt.Format( L"Bottomlight (reflection) particle inspection PC2" ); //- camera %s", camera[ind  ].name );

#ifdef TIVS_A60
		result = new Results_manager( txt.GetBuffer(), 500, 90 );
#else
	   result = new Results_manager( txt.GetBuffer(), 500, 60 );
#endif

      #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
        if ( IS_PC2 )
        {
          txt.Format( L"Bottomlight (reflection) particle inspection PC3" ); //- camera %s", camera[ind  ].name );

		  #ifdef TIVS_A60
		result2 = new Results_manager( txt.GetBuffer(), 500, 90 );
#else
	   result2 = new Results_manager( txt.GetBuffer(), 500, 60 );
#endif

          //result2 = new Results_manager( txt.GetBuffer(), 500, 60 );
        }
      #endif
    #endif



    #ifdef FD_DATAREC
      if ( IS_PC1 )
      {
        txt.Format( L"Sidewall data"   ); 
        result = new Results_manager( txt.GetBuffer(), 500, 60 );

        txt.Format( L"Cake volume data"   ); 
        result2 = new Results_manager( txt.GetBuffer(), 500, 60 );

        txt.Format( L"Cap control data" ); //
        cap_result = new Results_manager( txt.GetBuffer(), 500, 7 );

        txt.Format( L"Bottom A control data" ); //
        bottomA_result = new Results_manager( txt.GetBuffer(), 500, 60 ); //

        txt.Format( L"Bottom B control data" ); //
        bottomB_result = new Results_manager( txt.GetBuffer(), 500, 60 ); //
      }

      if ( IS_PC2 )
      {
        txt.Format( L"Sidecake data"   ); 
        result = new Results_manager( txt.GetBuffer(), 500, 60 );

        txt.Format( L"Topcake data"   ); 
        result2 = new Results_manager( txt.GetBuffer(), 500, 60 );
      }
    #endif


    TRACE( L"DATA_REC ENABLED\n" );
  }
  return( 0 );
}



int Save_inspections( int inp )
{
  if ( saveinsp_on )  // ENABLED: DISABLE IT
  {
    saveinsp_on = FALSE;
    saveinsp_btn->SetWindowText( string_320[lng] );
    TRACE( L"SAVE_INSPECTIONS DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {
    saveinsp_on = TRUE;
    saveinsp_btn->SetWindowText( string_322[lng] );
    TRACE( L"SAVE_INSPECTIONS ENABLED\n" );
    if ( IS_PC1 ) image_dsp[6]->enable_full_dsp(); // important only first time!
    if ( IS_PC2 ) image_dsp[3]->enable_full_dsp(); // important only first time!
  }
  return( 0 );
}

int Save_sequences( int inp )
{
  if ( saveseq_on )  // ENABLED: DISABLE IT
  {
    saveseq_on = FALSE;
    saveseq_btn->SetWindowText( string_324[lng] );
    TRACE( L"SAVE_SEQUENCES DISABLED\n" );
  }
  else               // DISABLED: ENABLE IT
  {
    saveseq_on = TRUE;
    saveseq_btn->SetWindowText( string_326[lng] );
    TRACE( L"SAVE_SEQUENCES ENABLED\n" );
  }
  return( 0 );
}


int Grippers_statistics( int inp )
{
  TRACE( L"Prepare grippers statistics\n" );

  if ( batch_mngr )
  {
    batch_mngr->Grippers_statistics();
  }

  pView->Close_all_menus();

  return( 0 );
}


int Set_user( int inp )
{
   if ( password_mngr ) password_mngr->authenticate_user();

  return( 0 );
}


/*
int Loop_inspection( int inp )
{

  if ( loop_inspection )  // ENABLED: DISABLE IT
  {
    loop_inspection = FALSE;
    //loopinsp_btn->SetWindowTextA( string_310[lng] );
    loopinsp_btn->SetWindowText( string_310[lng] );
    //TRACE("LOOP INSPECTION DISABLED\n");
    TRACE( L"LOOP INSPECTION DISABLED\n" );
  }
  else                   // DISABLED: ENABLE IT
  {
    loop_inspection = TRUE;
    //loopinsp_btn->SetWindowTextA( string_312[lng] );
    loopinsp_btn->SetWindowText( string_312[lng] );
    TRACE( L"LOOP INSPECTION ENABLED\n" );
  } 

  return( 0 );
}

*/



int Change_passwords( int inp )
{
  #ifdef NEW_PASSW_MNGR  // New users/passwords manager
  if ( password_mngr /*&& password_mngr->current_level >= SUPERVISOR_LEVEL*/ )
  {
    password_mngr->Change_passwords();
  }
  #else
  if ( password_mngr && password_mngr->current_level >= SUPERVISOR_LEVEL )
  {
    password_mngr->Change_passwords();
  }
  #endif
  return 0;
}


int Logout( int inp )
{
  if ( password_mngr ) password_mngr->logout(1);
  return 0;
}



int Show_alarmlog( int inp )
{

//  CString logname = "C:\\IVS\\logcopy_20141207.txt";
//  CString logname = "C:\\IVS\\log.txt";
//  CString logname = "C:\\IVS\\newlog.txt";

//  pView->Show_logfile( ALARMLOG_FILE_NAME ); // logname );

  #ifdef NEW_LOGGER      // New log 
    pView->Show_logfile( NEWALARMLOG_FILE_NAME ); // logname );
  #else
    pView->Show_logfile( ALARMLOG_FILE_NAME ); // logname );
  #endif


  return 0;
}


int Show_log( int inp )
{
  // test


/*
  #ifdef NEW_LOGGER // log alarms separately and add button to show them

    CString name = "Test_log.log"; 
    logger->file_name = name.GetBuffer(0);

    CString text;

    for ( int i=1000 ; i<2000 ; i++ )
    {
      text.Format(L"This is new line of log file n.%04d", i );
      logger->Add_string2( &text ); 
    }
    
    //    DeleteFile(name);
    //  logger->Add_string2( L"stringa 1" ); 
    //  logger->Add_string2( L"stringa 2" ); 

    pView->Show_logfile( name.GetBuffer(0) ); 
    return 0;
  #endif
  */

  #ifdef NEW_LOGGER      // New log 
    pView->Show_logfile( NEWLOG_FILE_NAME ); 
  #else
    pView->Show_logfile( LOG_FILE_NAME ); 
  #endif

  return 0;
}


int Save_images( int inp )
{
  #ifdef CFR21
    if ( login_wnd->PreOperation( LEVEL_UTILS, "UTILS/SAVE IMAGE" )) return 0;
  #endif

  pView->Close_all_menus();
    
	for ( UINT i=0 ; i<num_tlc ; i++ )
	{
    if ( image_dsp[i]->visible )
    {
      image_dsp[i]->save = 1; // request to copy and save TRUE;
      //swprintf( name, 200, L"%s\\%s_%04d%02d%02d_%02d%02d%02d", IMAGES_DIRECTORY, camera[i].name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
      //image_dsp[i]->Save_images( name );
    }
  }

  // Save screenshot 
  wchar_t name[200+1];
  CTime time = CTime::GetCurrentTime();   
  swprintf( name, 200, L"%s\\screenshot_%04d%02d%02d_%02d%02d%02d.png", IMAGES_DIRECTORY, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
  Capture_screenshot( name );


/*  wchar_t    name[100];  
  static int index = 100;

  saveimages_btn->ShowWindow( SW_HIDE );

  for ( int i=0 ; i<NUM_TLC ; i++ )
	{
    image_dsp[i]->save = TRUE;
    TRACE( L"Save_images[%d]: request to save\n", i );
  }

	for ( int i=0 ; i<NUM_TLC ; i++ )
	{
    swprintf( name, L"image%d_%03d", i, index );
    image_dsp[i]->Save_image( name );
    TRACE( L"Save_images[%d]: <%s> saved\n", i, name );
  }

  index++;
  saveimages_btn->ShowWindow( SW_SHOW );  */

  return 0;
}


/*
// Funzione che carica l'immagine e prova ad elaborarla
int Load_images( int inp )
{
	// CARICA DA DISCO
 
  //image_dsp[0]->Load_image( "image0_100_buf.png" );
  image_dsp[0]->Load_image( L"image0_100_buf.png" );
  SCALE_IMG ( 0, image_dsp[0]->ptr_buf );
  image_dsp[0]->Draw_image();

  //image_dsp[1]->Load_image( "image1_100_buf.png" );
  image_dsp[1]->Load_image( L"image1_100_buf.png" );
  SCALE_IMG ( 1, image_dsp[1]->ptr_buf );
  image_dsp[1]->Draw_image();


	return 0;
}
*/




// Se chiamata con inp=999 dealloca la memoria utilizzata dalla variabile <mc_lights>
int Machine_lights( int inp ) //, CMenu_button* ptr_btn = NULL )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_MACH_PAR, "MACHINE PARAMETERS/ MACHINE LIGHTS" )) return 0;
  //#endif

  #define ML_UNDEFINED  0
  #define ML_ALWAYS_ON  1
  #define ML_OPEN_DOOR  2
  #define ML_ALWAYS_OFF 3

	static short int status = ML_UNDEFINED;
	static ADS_data_type mc_lights;
//	static enum current { undefined=0, always_on=1, door_open=2, always_off=3 };

	if ( inp==999 && mc_lights.val )
	{
		free( mc_lights.val );
		mc_lights.val = NULL;
		return( 0 );
	}


	if ( ADS_status!=1 ) return 0; // ADS communication not active

  switch( status )
	{
		case ML_UNDEFINED:
      if ( ADS_init_parameter( ".MACHINE_LIGHTS", &mc_lights, 2 ) )
    	{
				//Message_box("Error in getting handle of <.MACHINE_LIGHTS>", MBOX_ERROR );
				Message_box(L"Error in getting handle of <.MACHINE_LIGHTS>", MBOX_ERROR ); //Pour marked on 20201015
		    delete mc_lights.val;
    	}
			else
			{
        if ( ADS_read_parameter( &mc_lights ) )
				{
					//Message_box("Error in reading value of <.MACHINE_LIGHTS>", MBOX_ERROR );
					Message_box(L"Error in reading value of <.MACHINE_LIGHTS>", MBOX_ERROR );
					delete mc_lights.val;
				}
				status = *(short int*)(mc_lights.val);
			}
			break;

		case ML_ALWAYS_ON:
      status = ML_OPEN_DOOR;
			break;

		case ML_OPEN_DOOR:
      status = ML_ALWAYS_OFF;
			break;

		case ML_ALWAYS_OFF:
      status = ML_ALWAYS_ON;
			break;

  }

	if ( ADS_status==1 && mc_lights.handle )
	{

		*(short int*)(mc_lights.val) = status;
		if ( ADS_write_parameter( &mc_lights ) )
		{
			//Message_box("Error in writing value of <.MACHINE_LIGHTS>", MBOX_ERROR );
			Message_box(L"Error in writing value of <.MACHINE_LIGHTS>", MBOX_ERROR );
			delete mc_lights.val;
		}

	}

	if ( pView && pView->menu_btn[10] )
	{
/*    pView->menu_btn[10]->SetWindowText( "unhandled" );
	  if ( status==ML_ALWAYS_ON  ) pView->menu_btn[10]->SetWindowText( "Mc lights always ON"    );
	  if ( status==ML_OPEN_DOOR  ) pView->menu_btn[10]->SetWindowText( "Mc lights on open door" );
	  if ( status==ML_ALWAYS_OFF ) pView->menu_btn[10]->SetWindowText( "Mc lights always OFF"   ); */
    pView->menu_btn[10]->SetWindowText( L"unhandled" );
	  if ( status==ML_ALWAYS_ON  ) pView->menu_btn[10]->SetWindowText( L"Mc lights always ON"    );
	  if ( status==ML_OPEN_DOOR  ) pView->menu_btn[10]->SetWindowText( L"Mc lights on open door" );
	  if ( status==ML_ALWAYS_OFF ) pView->menu_btn[10]->SetWindowText( L"Mc lights always OFF"   );
	}
/*
	switch( st )
	{
		 0:  // Uninialized: just load current status
      if ( ADS_init_parameter( ".MACHINE_LIGHTS", &mc_lights, 4 ) )
    	{
				Message_box("Error in getting handle of <.MACHINE_LIGHTS>", MBOX_ERROR );
		    delete mc_lights.val;
    	}
		   
	}
*/
	return 0;
}











// Manage input belt manual start/stop command
int Inpbelt_manrun( int inp )
{
	static ADS_data_type f_inpbelt_manrun;
  BYTE status = 0;

	if ( ADS_status!=1 ) return 0; // ADS communication not active

  if ( ADS_init_parameter( ".F_INPBELT_MANRUN", &f_inpbelt_manrun, 1 ) )
  {
    Message_box(L"Error in getting handle of <.F_INPBELT_MANRUN>", MBOX_ERROR );
  }
  else
  {
    if ( ADS_read_parameter( &f_inpbelt_manrun ) )
    {
      Message_box(L"Error in reading value of <.F_INPBELT_MANRUN>", MBOX_ERROR );
    }
    status = *(BYTE*)(f_inpbelt_manrun.val);
  }

  // Toggle 
  if ( ADS_status==1 && f_inpbelt_manrun.handle )
  {
    if ( status ) *(BYTE*)(f_inpbelt_manrun.val) = 0;
    else          *(BYTE*)(f_inpbelt_manrun.val) = 1;

		if ( ADS_write_parameter( &f_inpbelt_manrun ) )
		{
			Message_box(L"Error in writing value of <.F_INPBELT_MANRUN>", MBOX_ERROR );
		}
  }
  delete f_inpbelt_manrun.val;
  return 0;
}


// Manage output belt manual start/stop command
int Outbelt_manrun( int inp )
{
	static ADS_data_type f_outbelt_manrun;
  BYTE status = 0;

	if ( ADS_status!=1 ) return 0; // ADS communication not active

  if ( ADS_init_parameter( ".F_OUTBELT_MANRUN", &f_outbelt_manrun, 1 ) )
  {
    Message_box(L"Error in getting handle of <.F_OUTBELT_MANRUN>", MBOX_ERROR );
  }
  else
  {
    if ( ADS_read_parameter( &f_outbelt_manrun ) )
    {
      Message_box(L"Error in reading value of <.F_OUTBELT_MANRUN>", MBOX_ERROR );
    }
    status = *(BYTE*)(f_outbelt_manrun.val);
  }

  // Toggle 
  if ( ADS_status==1 && f_outbelt_manrun.handle )
  {
    if ( status ) *(BYTE*)(f_outbelt_manrun.val) = 0;
    else          *(BYTE*)(f_outbelt_manrun.val) = 1;

		if ( ADS_write_parameter( &f_outbelt_manrun ) )
		{
			Message_box(L"Error in writing value of <.F_OUTBELT_MANRUN>", MBOX_ERROR );
		}
  }
  delete f_outbelt_manrun.val;
  return 0;
}







// Manage tip OR cap mode variables and labels
int Tipcap_freerunning( int inp )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_INSP_PAR, "INSPECTION/SET LUT" )) return 0;
  //#endif

  
	if ( tipcap_freerun ) // TRUE, enable free running
  {
    ADS_write_value( "TRIGGER_DOUBLE_CAP.FREE_RUN",  TRUE, CTYPE_BOOL8 ); // Force flag into PLC high
    ADS_write_value( "TRIGGER_TRIPLE_TIP.FREE_RUN",  TRUE, CTYPE_BOOL8 ); // Force flag into PLC high
    TRACE("tipcap_freerun TRUE => free running enabled\n");
  }
	else // FALSE disable free running
  {
    ADS_write_value( "TRIGGER_DOUBLE_CAP.FREE_RUN", FALSE, CTYPE_BOOL8 ); // Force flag into PLC high
    ADS_write_value( "TRIGGER_TRIPLE_TIP.FREE_RUN", FALSE, CTYPE_BOOL8 ); // Force flag into PLC high
    TRACE("tipcap_freerun FALSE => free running disabled\n");
  }

  return 0;
}


// Manage tip OR cap mode variables and labels
int Tipcap_mode( int inp )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_INSP_PAR, "INSPECTION/SET LUT" )) return 0;
  //#endif

  
	if ( tipcap_tip ) // TRUE, set to TIP mode
  {
    ADS_write_value( ".F_DOUBLE_CAP", FALSE, CTYPE_BOOL8 ); // Force flag into PLC high
    ADS_write_value( ".F_TRIPLE_TIP",  TRUE, CTYPE_BOOL8 ); // Force flag into PLC high
    TRACE("tipcap_tip TRUE => tip mode selected\n");
  }
	else // FALSE, set to CAP mode
  {
    ADS_write_value( ".F_DOUBLE_CAP",  TRUE, CTYPE_BOOL8 ); // Force flag into PLC high
    ADS_write_value( ".F_TRIPLE_TIP", FALSE, CTYPE_BOOL8 ); // Force flag into PLC high
    TRACE("tipcap_tip FALSE => cap mode selected\n");
  }

  return 0;
}



// ind=camera index

int Set_camera_node( UINT ind, char* nodename, char* nodevalue, int value )
{

  ASSERT( nodename  && AfxIsValidString(nodename ) );
  ASSERT( nodevalue && AfxIsValidString(nodevalue) );
  ASSERT( ind>=0 && ind<num_tlc                    );
  ASSERT( camera[ind].camera                       );

  try
  {
    INodeMap* nodemap = camera[ind].device->GetNodeMap();
    CEnumerationPtr pointer( nodemap->GetNode( nodename ) );

    if ( pointer && IsWritable( pointer ) )
    {
      pointer->SetIntValue( value );
      ASSERT( pointer->GetIntValue() == value );
      TRACE("Camera[%d]: node <%s> set to <%s> (%d)\n", ind , nodename, nodevalue, value );
    }
    else
    {
      swprintf( msg_err, 200, L"Camera[%d]: ERROR Cannot access node <%s> for writing\n", ind, AtoW(nodename) );
      TRACE( L"%s\n", msg_err ); 
      Message_box( msg_err, MBOX_ERROR );
      return 88;
    }
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Camera[%d]: Genicam exception thrown <%s>\n", ind, AtoW(e.GetDescription()) );
    TRACE( L"%s\n", msg_err ); 
    Message_box( msg_err, MBOX_ERROR );
    return 99;
  }

  return 0; // successfull
}



// Manage particle station1 mode BACKLIGHT/BOTTOMLIGHT
int Set_stn1_mode( int inp )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_INSP_PAR, "INSPECTION/SET LUT" )) return 0;
  //#endif

  int ind = 4;

  for ( ind=3 ; ind<7 ; ind++ )
  {

	if ( stn1_part_back ) // TRUE, set to BACKLIGHT mode
  {

    Set_camera_node( ind, "LineSelector", "LineSelector_Out1",             LineSelector_Out1           );
    Set_camera_node( ind, "LineMode",     "LineMode_Output",               LineMode_Output             );
    //Set_camera_node( ind, "LineSource",   "LineSource_FrameTriggerWait ",  LineSource_FrameTriggerWait );
    Set_camera_node( ind, "LineSource",   "LineSource_ExposureActive ",    LineSource_ExposureActive   );
    //Set_camera_node( ind, "LineMode",     "LineMode_Input",               LineMode_Output             );


    //Set_camera_node( ind, "LineFormat",   "LineFormat_NoConnect ",      LineFormat_NoConnect      );
    //Set_camera_node( ind, "LineSource",   "LineSource_UserOutput1",     LineSource_UserOutput1    );

    /*
    Set_camera_node( ind, "UserOutputSelector",   "UserOutputSelector_UserOutput1",     UserOutputSelector_UserOutput1    );
        
    INodeMap* nodemap = camera[ind].device->GetNodeMap();
    CBooleanPtr userout_value( nodemap->GetNode( "UserOutputValue" ) );
    if ( userout_value && IsWritable( userout_value ) ) userout_value->SetValue(FALSE);
    */

/*    try
    {
      INodeMap* nodemap = camera[ind].device->GetNodeMap();

      CEnumerationPtr line_selector( nodemap->GetNode( "LineSelector" ) );
      if ( line_selector && IsWritable( line_selector ) )
      {
        line_selector->SetIntValue( LineSelector_Out1 );
        ASSERT( line_selector->GetIntValue() == LineSelector_Out1 );
        TRACE("LineSelector set to LineSelector_Out1\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSelector> node\n", ind );

      CEnumerationPtr line_source( nodemap->GetNode( "LineSource" ) );
      if ( line_source && IsWritable( line_source ) )
      {
        line_source->SetIntValue( LineSource_Off );
        ASSERT( line_source->GetIntValue() == LineSource_Off  );
        TRACE("LineSource set to LineSource_Off\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSource> node\n", ind );

    }
    catch (GenICam::GenericException &e)
    {
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"Basler_parameters: %s\n", msg_err );
      Message_box( msg_err, MBOX_ERROR );
    }  */


    if ( ind==3 )
    {
      //ADS_write_value( ".O_STN1_BOTTOML", FALSE, CTYPE_BOOL8 ); // Turn OFF output for bottomlight
      ADS_write_value( ".O_LIGHT[2]",      TRUE, CTYPE_BOOL8 ); // Turn ON  output for backlight
      TRACE("stn1_part_back=TRUE BACKLIGHT MODE\n");
    }
  }
	else // FALSE, set to BOTTOMLIGHT mode
  {
    /*
    Set_camera_node( ind, "LineSelector", "LineSelector_Out1",             LineSelector_Out1           );
    Set_camera_node( ind, "LineMode",     "LineMode_Output",               LineMode_Output             );
    */

    //Set_camera_node( ind, "LineSelector", "LineSelector_Out1",          LineSelector_Out1         );
    //Set_camera_node( ind, "LineMode",     "LineMode_Output",            LineMode_Output           );
    //Set_camera_node( ind, "LineFormat",   "LineFormat_OptoCoupled  ",   LineFormat_OptoCoupled    );

    
    Set_camera_node( ind, "LineSelector", "LineSelector_Out1",             LineSelector_Out1           );
    Set_camera_node( ind, "LineMode",     "LineMode_Output",               LineMode_Output             );
    //Set_camera_node( ind, "LineSource",   "LineSource_ExposureActive ",    LineSource_ExposureActive   );
    Set_camera_node( ind, "LineSource",   "LineSource_FrameTriggerWait ",  LineSource_FrameTriggerWait );
    

/*
    try
    {
      INodeMap* nodemap = camera[ind].device->GetNodeMap();

      CEnumerationPtr line_selector( nodemap->GetNode( "LineSelector" ) );
      if ( line_selector && IsWritable( line_selector ) )
      {
        line_selector->SetIntValue( LineSelector_Out1 );
        ASSERT( line_selector->GetIntValue() == LineSelector_Out1 );
        TRACE("LineSelector set to LineSelector_Out1\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSelector> node\n", ind );

      CEnumerationPtr line_mode( nodemap->GetNode( "LineMode" ) );
      if ( line_mode && IsWritable( line_mode ) )
      {
        line_mode->SetIntValue( LineMode_Output );
        ASSERT( line_mode->GetIntValue() == LineMode_Output );
        TRACE("LineMode set to LineMode_Output\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineMode> node\n", ind );

      CEnumerationPtr line_source( nodemap->GetNode( "LineSource" ) );
      if ( line_source && IsWritable( line_source ) )
      {
        line_source->SetIntValue( LineSource_ExposureActive );
        ASSERT( line_source->GetIntValue() == LineSource_ExposureActive );
        TRACE("LineSource set to LineSource_ExposureActive\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSource> node\n", ind ); 
    }
    catch (GenICam::GenericException &e)
    {
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"Basler_parameters: %s\n", msg_err );
      Message_box( msg_err, MBOX_ERROR );
    }


    */

    if ( ind==3 )
    {
      //ADS_write_value( ".O_STN1_BOTTOML",  TRUE, CTYPE_BOOL8 ); // Turn ON  output for bottomlight
      ADS_write_value( ".O_LIGHT[2]",     FALSE, CTYPE_BOOL8 ); // Turn OFF output for backlight
      TRACE("stn1_part_back=FALSE BOTTOMLIGHT MODE\n");
    }
  }

  }


  return 0;
}



// Assegna il puntatore alla palette da utilizzare per la visualizzazione
int Set_LUT ( int inp )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_INSP_PAR, "INSPECTION/SET LUT" )) return 0;
  //#endif

  
	if ( pseudo_colors )
  {
    lut32 = lut32_pc;
    TRACE("Pseudocolor palette selected\n");
  }
	else
  {
    lut32 = lut32_bw;
    TRACE("Gray palette selected\n");
  }

  return 0;
}


/*
// several macros to support definition of integral values
// on a 32-bit machine by using the binary notation
// (we'll make sure that the integral literals are octal by prefixing them with a zero)
#define BIN8(b) ((unsigned char)_b8tpl_<0##b>::val)
#define BIN16(b1,b0) ((((unsigned short)BIN8(b1))<<8)|BIN8(b0))
#define BIN32(b3,b2,b1,b0) ((((unsigned int)BIN16(b3,b2))<<16)|BIN16(b1,b0))
#define BIN64(b7,b6,b5,b4,b3,b2,b1,b0) ((((unsigned __int64)BIN32(b7,b6,b5,b4))<<32)|BIN32(b3,b2,b1,b0))
*/


int Set_lqfdmode( int inp )
{
  //#ifdef CFR21
    //if ( login_wnd->PreOperation( LEVEL_INSP_PAR, "INSPECTION/SET LUT" )) return 0;
  //#endif

/*  if ( movie_active || inspection_active )
  {
    Message_box( string_2026[lng], MBOX_INFO );
    return( 0 );
  }  */


  if ( thread_status & (B10+B11+B12+B13+B14+B15+B16+B17+B18+B19) ) // some thread running
  {
    Message_box( string_2026[lng], MBOX_INFO );
    return 9;
  }

  /*
	if ( liquid_mode )
  {
    ADS_write_value(".DISTOFS_GR1", 0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR2", 0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR3", 0, CTYPE_DINT );

    ADS_write_value(".O_LIGHT[2]", FALSE, CTYPE_BOOL8 ); // backlight red sidewall
    ADS_write_value(".O_LIGHT[3]", FALSE, CTYPE_BOOL8 ); // illuminators for sidecake
	//Pour added 20190327
	ADS_write_value(".O_LIGHT[8]", TRUE, CTYPE_BOOL8 ); // illuminators for particle 1

    lqfdmode_txt->SetWindowTextW( string_2140[lng] );
    TRACE("liquid mode selected\n");
  }
	else
  {
    ADS_write_value(".DISTOFS_GR1",  1, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR2",  0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR3", -1, CTYPE_DINT );

    ADS_write_value(".O_LIGHT[2]", TRUE, CTYPE_BOOL8 ); // backlight red sidewall
    ADS_write_value(".O_LIGHT[3]", TRUE, CTYPE_BOOL8 ); // illuminators for sidecake
	//Pour added 20190327
	ADS_write_value(".O_LIGHT[8]", FALSE, CTYPE_BOOL8 ); // illuminators for particle 1

    lqfdmode_txt->SetWindowTextW( string_2142[lng] );
    TRACE("freeze-dried mode elected\n");
  }
  */
		if ( liquid_mode )
  {
    ADS_write_value(".DISTOFS_GR1", 0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR2", 0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR3", 0, CTYPE_DINT );

    //ADS_write_value(".O_LIGHT[2]", FALSE, CTYPE_BOOL8 ); // backlight red sidewall
    ADS_write_value(".O_LIGHT[3]", FALSE, CTYPE_BOOL8 ); // illuminators for sidecake
	//Pour added 20190327
	ADS_write_value(".P1_LIGHT", TRUE, CTYPE_BOOL8 ); // illuminators for particle 1
	ADS_write_value(".F_MODEL", TRUE, CTYPE_BOOL8 ); //to identify different models //Pour on 20191120
    lqfdmode_txt->SetWindowTextW( string_2140[lng] );
    TRACE("liquid mode selected\n");
	uplist[7].desc_st->SetWindowText(string_part1rej[lng] );//particle 1 text on No.8 in sequence
	uplist[8].desc_st->SetWindowText(string_part2rej[lng]);//particle 2 text on No.9 in sequence
  }
	else
  {
    ADS_write_value(".DISTOFS_GR1",  1, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR2",  0, CTYPE_DINT );
    ADS_write_value(".DISTOFS_GR3", -1, CTYPE_DINT );

    ADS_write_value(".O_LIGHT[2]", TRUE, CTYPE_BOOL8 ); // backlight red sidewall
    ADS_write_value(".O_LIGHT[3]", TRUE, CTYPE_BOOL8 ); // illuminators for sidecake
	//Pour added 20190327
	ADS_write_value(".P1_LIGHT", FALSE, CTYPE_BOOL8 ); // illuminators for particle 1
	ADS_write_value(".F_MODEL", FALSE, CTYPE_BOOL8 ); //to identify different models //Pour on 20191120
    lqfdmode_txt->SetWindowTextW( string_2142[lng] );
    TRACE("freeze-dried mode elected\n");

	//Pour added on 20190507 for display of different mode
	uplist[7].desc_st->SetWindowTextW(string_sideckrej[lng]); //sidecake text on No.8 in sequence
	uplist[8].desc_st->SetWindowTextW(string_topckrej[lng]);  //sidecake text on No.9 in sequence
	//Pour added on 20190507 for display of different mode
  }


  if ( IS_PC1 )
  {
    ADS_write_value( "SEND_SPIN_PARAMETERS.RUN", 1, CTYPE_BOOL8 );
  }

  return 0;
}





void draw_point( int x, int y )
{
  DRAW_PT( 1 , x  , y  , COL_MAGENTA );
  DRAW_PT( 1 , x+4, y  , COL_MAGENTA );
  DRAW_PT( 1 , x-4, y  , COL_MAGENTA );
  DRAW_PT( 1 , x  , y-4, COL_MAGENTA );
  DRAW_PT( 1 , x  , y+4, COL_MAGENTA );

  DRAW_PT( 1 , x-4, y-4, COL_BLACK   );
  DRAW_PT( 1 , x+4, y-4, COL_BLACK   );
  DRAW_PT( 1 , x-4, y+4, COL_BLACK   );
  DRAW_PT( 1 , x+4, y+4, COL_BLACK   );

  DRAW_PT( 1 , x-8, y  , COL_BLACK   );
  DRAW_PT( 1 , x+8, y  , COL_BLACK   );
  DRAW_PT( 1 , x  , y-8, COL_BLACK   );
  DRAW_PT( 1 , x  , y+8, COL_BLACK   );
}



// gaussian blur version
/*
int     gb_npt =    0;
double* gb_val = NULL;
int*    gb_ofs = NULL;
void    gb_compute( double sigma );
void    gb_free( void );
void    vertical_edge_gb( BYTE* img, int x );
*/



/*
void gb_compute( double sigma )
{
  gb_free();
  
  #define PI  3.14159265
  #define MAX_FACT_GB 1000.0

  ASSERT( sigma>0.1 && sigma<3.0 );

  double x, y;
  double val [1000];
  int    ofs [1000];
  int    ofsx[1000];
  int    ofsy[1000];
  int    npt = 0;

  double tot = 0.0;
  double lim = 1.0 / (2.0*PI*sigma*sigma*MAX_FACT_GB);

  for ( y = -8.0 ; y <= 8.0 ; y += 1.0 )
    for ( x = -8.0 ; x <= 8.0 ; x += 1.0 )
    {
      val [npt] = exp( -(x*x+y*y)/(2.0*sigma*sigma) ) / (2.0*PI*sigma*sigma);
      ofs [npt] = DOUBLETOINT(x + y*DIMX_ACQ);
      ofsx[npt] = DOUBLETOINT(x)*8;
      ofsy[npt] = DOUBLETOINT(y)*8;
      if ( val[npt]>lim )
      {
        tot += val[npt];
        npt++;
      }
      ASSERT( npt < 1000 );
    }

//  TRACE("sigma=%4.3lf - precalcolati %d punti\n", sigma, npt );

  gb_val  = (double*) malloc( npt*sizeof(double) );
  gb_ofs  = (int*)    malloc( npt*sizeof(int)    );
  gb_ofsx = (int*)    malloc( npt*sizeof(int)    );
  gb_ofsy = (int*)    malloc( npt*sizeof(int)    );
  gb_npt  = npt;
 
  for ( int i=0 ; i<npt ; i++ )
  {
    gb_val [i] = val [i]/tot;
    gb_ofs [i] = ofs [i];
    gb_ofsx[i] = ofsx[i];
    gb_ofsy[i] = ofsy[i];
    //TRACE("Punto n.%3d - ofs=%5d,%5d - val=%6.5lf\n", i, gb_ofsx[i], gb_ofsy[i], gb_val[i] );
  }

  TRACE("gb_compute: tot=%6.5lf - sigma=%4.3lf - precalcolati %d punti\n", tot, sigma, npt );

}

*/

/*
void gb_free( void )
{
  if ( gb_val ) free( gb_val );
  gb_val = NULL;

  if ( gb_ofs ) free( gb_ofs );
  gb_ofs = NULL;

  if ( gb_ofsx ) free( gb_ofsx );
  gb_ofsx = NULL;

  if ( gb_ofsy ) free( gb_ofsy );
  gb_ofsy = NULL;

  gb_npt = 0;
}
*/

#undef  PT

#define PT(x,y)  img[x+(y)*DIMX_ACQ]
#define VERT_CAL (PT(x,y-2)+3*PT(x,y-1)-3*PT(x,y+1)-PT(x,y+2))

//#define VERT_CAL (img[x+(y-1)*DIMX_ACQ]+3*img[x+y*DIMX_ACQ]-3*img[x+(y+1)*DIMX_ACQ]-img[x+(y+2)*DIMX_ACQ])


/*

#define SP_CF          8
#define DIMX (DIMX_ACQ*8)
#define DIMY (DIMY_ACQ*8)


int cf00[SP_CF][SP_CF];
int cf10[SP_CF][SP_CF];
int cf01[SP_CF][SP_CF];
int cf11[SP_CF][SP_CF];

void sp_prepare_cf( void )
{
  for ( int y=0 ; y<SP_CF ; y++ )
  {
//    TRACE("Y=%d |", y);
    for ( int x=0 ; x<SP_CF ; x++ )
    {
      int pch = DOUBLETOINT( 64.0*x/SP_CF);
      int pcv = DOUBLETOINT( 64.0*y/SP_CF);

      cf00[x][y] = (64-pch) * (64-pcv);
      cf10[x][y] =     pch  * (64-pcv);
      cf01[x][y] = (64-pch) *     pcv ;
      cf11[x][y] =     pch  *     pcv ;

      ASSERT( cf00[x][y]+cf10[x][y]+cf01[x][y]+cf11[x][y] == 64*64 );

//      TRACE("%5d-%5d-%5d-%5d | ", cf00[x][y], cf10[x][y], cf01[x][y], cf11[x][y] );

    }
//    TRACE("\n");
  }
  TRACE("sp_prepare_cf completata\n");

}

int sp_brightness( BYTE* img, int x, int y )
{

  // ATTENZIONE: LE DUE ASSERT SEMBRANO ESSERE SBAGLIATE PERCHE SI LAVORA SUGLI INTERPOLATI !!
	ASSERT( x>=0 && x<DIMX-1 );
	ASSERT( y>=0 && y<DIMY-1 );

  x -= 4;
  y -= 4;


  int xi = x>>3;
  int xr = x-(xi<<3);
  int yi = y>>3;
  int yr = y-(yi<<3);

  return img[xi+ yi   *DIMX_ACQ]*cf00[xr][yr] + img[xi+1+ yi   *DIMX_ACQ]*cf10[xr][yr] +
         img[xi+(yi+1)*DIMX_ACQ]*cf01[xr][yr] + img[xi+1+(yi+1)*DIMX_ACQ]*cf11[xr][yr] ;
}


int gb_brightness( BYTE* img, int x, int y )
{

  double val = 0.0;
  for ( int i=0 ; i<gb_npt ; i++ )
  {
    val += gb_val[i]*sp_brightness(img,(x+gb_ofsx[i]),(y+gb_ofsy[i]));
  }

  return int(0.5+val);

}

*/

/*

int sp_brightness_test( BYTE* img, int x, int y )
{
//	ASSERT( x>=0 && x<1920-1 );
//	ASSERT( y>=0 && y<1920-1 );

  x -= 4;
  y -= 4;


  int xi = x>>3;
  int xr = x-(xi<<3);
  int yi = y>>3;
  int yr = y-(yi<<3);

  return img[xi+ yi   *1920]*cf00[xr][yr] + img[xi+1+ yi   *1920]*cf10[xr][yr] +
         img[xi+(yi+1)*1920]*cf01[xr][yr] + img[xi+1+(yi+1)*1920]*cf11[xr][yr] ;
}

*/

/*
int gb_brightness_test( BYTE* img, int x, int y )
{

  double val = 0.0;
  for ( int i=0 ; i<gb_npt ; i++ )
  {
    val += gb_val[i]*sp_brightness_test(img,(x+gb_ofsx[i]),(y+gb_ofsy[i]));
  }

  return int(0.5+val);

}

*/





/*
#ifdef TEMP_TLC
  #undef  DIMX_ACQ
  #undef  DIMY_ACQ
  #define DIMX_ACQ 1920
  #define DIMY_ACQ 1080
#endif
  */
/*
double horizontal_edge_interp( int ind, BYTE* img, int y, int& x1, int& x2, BOOL print_result )
{
  ASSERT( y>=0 && y<DIMY_ACQ );
  ASSERT( ind==0 || ind==1 );

  int bordo_x  =  20;   // fascia di rispetto esterna

  int sogliah  = soglia[ind]<<12;

  int xd, yd;
  int pt1_l, pt2_l, pt3_l;
  int pt1_x, pt2_x, pt3_x;

  x1 = 0;
  x2 = 0;

  xd = bordo_x << 3; // xd iniziale
  yd = y       << 3; // yd iniziale

  pt1_x = pt2_x = xd;
  pt1_l = pt2_l = gb_brightness(img,xd,yd);

//  ASSERT( pt1_l<sogliah );
  if ( pt1_l >= sogliah ) return( 0.0 );

  while ( pt1_l<sogliah && pt1_x<(DIMX_ACQ-bordo_x)*8 )
  {
    DRAW_PT( ind, pt1_x/8, y, 0xFF00FF00 );
    pt2_x  = pt1_x;
    pt2_l  = pt1_l;
    pt1_x += 64; // avanti 8 pixel
    pt1_l  = gb_brightness(img,pt1_x,yd);
  }


  if ( pt1_x>=(DIMX_ACQ-bordo_x)*8 )
  {
    //TRACE("BORDO SINISTRO NON TROVATO PER yd=%d\n", yd );
    return 0.0;
  }

  //TRACE("TROVATO BORDO TRA %d e %d (%6.2lf e %6.2lf)\n", pt2_y, pt1_y, pt2_y/8.0, pt1_y/8.0 );

  while ( pt1_x-pt2_x > 1 )
  {

    pt3_x = (pt1_x+pt2_x)>>1;
    ASSERT( pt2_x<pt3_x && pt3_x<pt1_x );
    pt3_l  = gb_brightness(img,pt3_x,yd);
//    pt3_l  = gb_brightness(img,pt3_x,yd);


    //TRACE("Nuova iter SX: pt1-pt3-pt2 %d-%d-%d - pt3_l=%d\n", pt1_x, pt3_x, pt2_x, pt3_l/4096 );

    if ( pt3_l>sogliah ) 
    {
      pt1_x = pt3_x;
      pt1_l = pt3_l;
    }
    else
    {
      pt2_x = pt3_x;
      pt2_l = pt3_l;
    }
  }

  // TRACE("BORDO TROVATO TRA %d,%d e %d,%d (%6.2lf e %6.2lf)\n", pt2_y, pt2_l>>12, pt1_y, pt1_l>>12, pt2_y/8.0, pt1_y/8.0 );

  // vediamo chi ?pi?vicino
// vecchia versione  if ( pt2_l-sogliah < sogliah-pt1_l ) x1 = pt2_x;
//  else                                 x1 = pt1_x;
  if ( pt1_l-sogliah < sogliah-pt2_l ) x1 = pt1_x;
  else                                 x1 = pt2_x;


  //TRACE("BORDO SINISTRO in %d - pt1 %d,%d - pt2 %d,%d\n", left_edge, pt1_x, pt1_l/4096, pt2_x, pt2_l/4096 );
  
  // ricerca lower_edge
  xd = (DIMX_ACQ-bordo_x)*8; // xd iniziale

  pt1_x = pt2_x = xd;
  pt1_l = pt2_l = gb_brightness(img,xd,yd);
//  pt1_l = pt2_l = gb_brightness(img,xd,yd);


//  ASSERT( pt1_l<sogliah );
  if ( pt1_l >= sogliah ) return( 0.0 );

  while ( pt1_l<sogliah && pt1_x>x1+10*8 ) // spessore minimo ammissibile
  {
    DRAW_PT( ind, pt1_x/8, y, 0xFF00FF00 );
    pt2_x  = pt1_x;
    pt2_l  = pt1_l;
    pt1_x -= 64; // avanti 8 pixel
    pt1_l  = gb_brightness(img,pt1_x,yd);
  }
//    pt1_l  = gb_brightness(img,pt1_x,yd);


  if ( pt1_x<=x1+5*8 )
  {
    //TRACE("BORDO DESTRO NON TROVATO PER yd=%d\n", yd );
    return 0.0;
  }

  //TRACE("TROVATO BORDO TRA %d e %d (%6.2lf e %6.2lf)\n", pt2_y, pt1_y, pt2_y/8.0, pt1_y/8.0 );

  while ( pt2_x-pt1_x > 1 )
  {

    pt3_x = (pt1_x+pt2_x)>>1;
    ASSERT( pt1_x<pt3_x && pt3_x<pt2_x );
//    pt3_l  = gb_brightness(img,pt3_x,yd);
    pt3_l  = gb_brightness(img,pt3_x,yd);

    //TRACE("Nuova iter DX: pt1-pt3-pt2 %d-%d-%d - pt3_l=%d\n", pt1_x, pt3_x, pt2_x, pt3_l/4096 );

    if ( pt3_l>sogliah ) 
    {
      pt1_x = pt3_x;
      pt1_l = pt3_l;
    }
    else
    {
      pt2_x = pt3_x;
      pt2_l = pt3_l;
    }
  }

//  TRACE("BORDO TROVATO TRA %d,%d e %d,%d (%6.2lf e %6.2lf)\n", pt2_x, pt2_l>>12, pt1_x, pt1_l>>12, pt2_x/8.0, pt1_x/8.0 );

  // vediamo chi ?pi?vicino
  if ( pt1_l-sogliah < sogliah-pt2_l ) x2 = pt1_x;
  else                                 x2 = pt2_x;

  //TRACE("BORDO DESTRO in %d - pt1 %d,%d - pt2 %d,%d\n", right_edge, pt1_x, pt1_l/4096, pt2_x, pt2_l/4096 );

  double diam = scale_cf[ind]*(x2-x1)*0.125;

  if ( print_result )
  {
    char msg[20];
    sprintf( msg, "D:%.3lf", diam );

    image_dsp[ind]->Draw_text( msg, 1, y/4+7, 0x0000FF00 );
  }

  image_dsp[ind]->Draw_marker( x1>>3, y );
  image_dsp[ind]->Draw_marker( x2>>3, y );

  return diam;

  /*
  for ( yd = 100 ; yd<200 ; yd++ )
    for ( xd = 100 ; xd<200 ; xd++ )
       img[xd+yd*DIMX_ACQ]=200;

  for ( yd = 120 ; yd<125 ; yd++ )
    for ( xd = 120 ; xd<125 ; xd++ )
       img[xd+yd*DIMX_ACQ]=20;


  xd = 122*8+4;


  for ( yd=110*8 ; yd<130*8 ; yd++ )
  {
//    TRACE("YD=%6.2lf - %6.2lf\n",yd/8.0 ,sp_brightness( img, xd, yd )/4096.0);
//    TRACE("YD=%6.2lf - %6.2lf - %6.2lf\n",yd/8.0 ,sp_brightness( img, xd, yd )/4096.0 ,gb_brightness( img, xd, yd )/4096.0);

    TRACE("%6.2lf,%3d,%6.2lf,%6.2lf\n",yd/8.0,PT(xd/8,yd/8),sp_brightness(img,xd,yd)/4096.0,gb_brightness(img,xd,yd)/4096.0);
//    TRACE("%6.2lf,%3d,%6.2lf\n",yd/8.0 , PT(xd/8,yd/8),sp_brightness( img, xd, yd )/4096.0 );
  }



    image_dsp[1]->Draw_text( msg, 5, (DIMX_ACQ-x)/4, 0x00FF0000 );

//  }
//  else TRACE("Diametro NON accettabile %6.3lf rilevato in %4d\n", diam, x );


  /*

  extern  CImage_display* image_dsp[NUM_TLC];
  #define SCALE_PT(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_pt))( img )
  #define SCALE_IMG(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_pt))( img )
  #define DRAW_PT(nimg,x,y,col) (image_dsp[nimg]->*(image_dsp[nimg]->draw_pt))(x,y,col) // 0xFFRRGGBB

  if ( vmax>=soglia && vmin<=-soglia )
  {

    for ( y = ymax ; y<=ymin ; y+=4 )
    {
      DRAW_PT( 1 , x, y, 0xFFFF00FF );
    }

    DRAW_PT( 1 , x+ 4, ymin+ 4, 0xFF000000 );
    DRAW_PT( 1 , x+ 8, ymin+ 8, 0xFF000000 );
    DRAW_PT( 1 , x+12, ymin+12, 0xFF000000 );
    DRAW_PT( 1 , x+16, ymin+16, 0xFF000000 );

    DRAW_PT( 1 , x- 4, ymin+ 4, 0xFF000000 );
    DRAW_PT( 1 , x- 8, ymin+ 8, 0xFF000000 );
    DRAW_PT( 1 , x-12, ymin+12, 0xFF000000 );
    DRAW_PT( 1 , x-16, ymin+16, 0xFF000000 );

    DRAW_PT( 1 , x+ 4, ymax- 4, 0xFF000000 );
    DRAW_PT( 1 , x+ 8, ymax- 8, 0xFF000000 );
    DRAW_PT( 1 , x+12, ymax-12, 0xFF000000 );
    DRAW_PT( 1 , x+16, ymax-16, 0xFF000000 );

    DRAW_PT( 1 , x- 4, ymax- 4, 0xFF000000 );
    DRAW_PT( 1 , x- 8, ymax- 8, 0xFF000000 );
    DRAW_PT( 1 , x-12, ymax-12, 0xFF000000 );
    DRAW_PT( 1 , x-16, ymax-16, 0xFF000000 );
  }

  */
//}

/*
#ifdef TEMP_TLC
  #undef  DIMX_ACQ
  #undef  DIMY_ACQ
  #define DIMX_ACQ 2456
  #define DIMY_ACQ 2056
#endif
  */


/*
double vertical_edge_interp( int ind, BYTE* img, int x, int& y1, int& y2, BOOL print_result )
{
  ASSERT( x>=0 && x<DIMX_ACQ );
  ASSERT( ind==0 || ind==1 );

  int bordo_y  =  20;   // fascia di rispetto esterna

  int sogliah  = soglia[ind]*4096;

  int xd, yd;
  int pt1_l, pt2_l, pt3_l;
  int pt1_y, pt2_y, pt3_y;

  y1 = 0;
  y2 = 0;

  xd =       x << 3; // xd iniziale
  yd = bordo_y << 3; // yd iniziale

  pt1_y = pt2_y = yd;
  pt1_l = pt2_l = gb_brightness(img,xd,yd);

 // ASSERT( pt1_l<sogliah );

  if ( pt1_l >= sogliah ) return( 0.0 );

  while ( pt1_l<sogliah && pt1_y<(DIMY_ACQ-bordo_y)*8 )
  {
    DRAW_PT( ind, x, pt1_y/8, 0xFF00FF00 );
    pt2_y  = pt1_y;
    pt2_l  = pt1_l;
    pt1_y += 64; // avanti 8 pixel
    pt1_l  = gb_brightness(img,xd,pt1_y);
  }


  if ( pt1_y>=(DIMY_ACQ-bordo_y)*8 )
  {
    //TRACE("[%d] BORDO SUPERIORE NON TROVATO PER xd=%d\n", ind, xd );
    return 0.0;
  }

  //TRACE("TROVATO BORDO TRA %d e %d (%6.2lf e %6.2lf)\n", pt2_y, pt1_y, pt2_y/8.0, pt1_y/8.0 );

  while ( pt1_y-pt2_y > 1 )
  {

    pt3_y = (pt1_y+pt2_y)>>1;
    ASSERT( pt2_y<pt3_y && pt3_y<pt1_y );
    pt3_l  = gb_brightness(img,xd,pt3_y);

    //TRACE("Nuova iter SX: pt1-pt3-pt2 %d-%d-%d - pt3_l=%d\n", pt1_x, pt3_x, pt2_x, pt3_l/4096 );

    if ( pt3_l>sogliah ) 
    {
      pt1_y = pt3_y;
      pt1_l = pt3_l;
    }
    else
    {
      pt2_y = pt3_y;
      pt2_l = pt3_l;
    }
  }

  // TRACE("BORDO TROVATO TRA %d,%d e %d,%d (%6.2lf e %6.2lf)\n", pt2_y, pt2_l>>12, pt1_y, pt1_l>>12, pt2_y/8.0, pt1_y/8.0 );

  // vediamo chi ?pi?vicino
  if ( pt1_l-sogliah < sogliah-pt2_l ) y1 = pt1_y;
  else                                 y1 = pt2_y;

  //TRACE("BORDO SINISTRO in %d - pt1 %d,%d - pt2 %d,%d\n", left_edge, pt1_x, pt1_l/4096, pt2_x, pt2_l/4096 );
  
  // ricerca lower_edge
  yd = (DIMY_ACQ-bordo_y)*8; // yd iniziale

  pt1_y = pt2_y = yd;
  pt1_l = pt2_l = gb_brightness(img,xd,yd);

//  ASSERT( pt1_l<sogliah );
  if ( pt1_l >= sogliah ) return( 0.0 );

  while ( pt1_l<sogliah && pt1_y>y1+10*8 ) // spessore minimo ammissibile
  {
    DRAW_PT( ind, x, pt1_y/8, 0xFF00FF00 );
    pt2_y  = pt1_y;
    pt2_l  = pt1_l;
    pt1_y -= 64; // avanti 8 pixel
    pt1_l  = gb_brightness(img,xd,pt1_y);
  }

  if ( pt1_y<=y1+5*8 )
  {
    //TRACE("[%d] BORDO INFERIORE NON TROVATO PER xd=%d\n", ind, xd );
    return 0.0;
  }

  //TRACE("TROVATO BORDO TRA %d e %d (%6.2lf e %6.2lf)\n", pt2_y, pt1_y, pt2_y/8.0, pt1_y/8.0 );

  while ( pt2_y-pt1_y > 1 )
  {

    pt3_y = (pt1_y+pt2_y)>>1;
    ASSERT( pt1_y<pt3_y && pt3_y<pt2_y );
    pt3_l  = gb_brightness(img,xd,pt3_y);

    //TRACE("Nuova iter DX: pt1-pt3-pt2 %d-%d-%d - pt3_l=%d\n", pt1_x, pt3_x, pt2_x, pt3_l/4096 );

    if ( pt3_l>sogliah ) 
    {
      pt1_y = pt3_y;
      pt1_l = pt3_l;
    }
    else
    {
      pt2_y = pt3_y;
      pt2_l = pt3_l;
    }
  }

//  TRACE("BORDO TROVATO TRA %d,%d e %d,%d (%6.2lf e %6.2lf)\n", pt2_x, pt2_l>>12, pt1_x, pt1_l>>12, pt2_x/8.0, pt1_x/8.0 );

  // vediamo chi ?pi?vicino
  if ( pt1_l-sogliah < sogliah-pt2_l ) y2 = pt1_y;
  else                                 y2 = pt2_y;

  //TRACE("BORDO DESTRO in %d - pt1 %d,%d - pt2 %d,%d\n", right_edge, pt1_x, pt1_l/4096, pt2_x, pt2_l/4096 );

  double diam = scale_cf[ind]*(y2-y1)*0.125;

  if ( print_result )
  {
    char msg[20];
    sprintf( msg, "D:%.3lf", diam );

    image_dsp[ind]->Draw_text( msg, 1, (DIMX_ACQ-x)/4+7, 0x0000FF00 );
  }

  image_dsp[ind]->Draw_marker( x, y1>>3 );
  image_dsp[ind]->Draw_marker( x, y2>>3 );

  return diam;

  /*
  for ( yd = 100 ; yd<200 ; yd++ )
    for ( xd = 100 ; xd<200 ; xd++ )
       img[xd+yd*DIMX_ACQ]=200;

  for ( yd = 120 ; yd<125 ; yd++ )
    for ( xd = 120 ; xd<125 ; xd++ )
       img[xd+yd*DIMX_ACQ]=20;


  xd = 122*8+4;


  for ( yd=110*8 ; yd<130*8 ; yd++ )
  {
//    TRACE("YD=%6.2lf - %6.2lf\n",yd/8.0 ,sp_brightness( img, xd, yd )/4096.0);
//    TRACE("YD=%6.2lf - %6.2lf - %6.2lf\n",yd/8.0 ,sp_brightness( img, xd, yd )/4096.0 ,gb_brightness( img, xd, yd )/4096.0);

    TRACE("%6.2lf,%3d,%6.2lf,%6.2lf\n",yd/8.0,PT(xd/8,yd/8),sp_brightness(img,xd,yd)/4096.0,gb_brightness(img,xd,yd)/4096.0);
//    TRACE("%6.2lf,%3d,%6.2lf\n",yd/8.0 , PT(xd/8,yd/8),sp_brightness( img, xd, yd )/4096.0 );
  }



    image_dsp[1]->Draw_text( msg, 5, (DIMX_ACQ-x)/4, 0x00FF0000 );

//  }
//  else TRACE("Diametro NON accettabile %6.3lf rilevato in %4d\n", diam, x );


  /*

  extern  CImage_display* image_dsp[NUM_TLC];
  #define SCALE_PT(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_pt))( img )
  #define SCALE_IMG(nimg, img) (image_dsp[nimg]->*(image_dsp[nimg]->scale_pt))( img )
  #define DRAW_PT(nimg,x,y,col) (image_dsp[nimg]->*(image_dsp[nimg]->draw_pt))(x,y,col) // 0xFFRRGGBB

  if ( vmax>=soglia && vmin<=-soglia )
  {

    for ( y = ymax ; y<=ymin ; y+=4 )
    {
      DRAW_PT( 1 , x, y, 0xFFFF00FF );
    }

    DRAW_PT( 1 , x+ 4, ymin+ 4, 0xFF000000 );
    DRAW_PT( 1 , x+ 8, ymin+ 8, 0xFF000000 );
    DRAW_PT( 1 , x+12, ymin+12, 0xFF000000 );
    DRAW_PT( 1 , x+16, ymin+16, 0xFF000000 );

    DRAW_PT( 1 , x- 4, ymin+ 4, 0xFF000000 );
    DRAW_PT( 1 , x- 8, ymin+ 8, 0xFF000000 );
    DRAW_PT( 1 , x-12, ymin+12, 0xFF000000 );
    DRAW_PT( 1 , x-16, ymin+16, 0xFF000000 );

    DRAW_PT( 1 , x+ 4, ymax- 4, 0xFF000000 );
    DRAW_PT( 1 , x+ 8, ymax- 8, 0xFF000000 );
    DRAW_PT( 1 , x+12, ymax-12, 0xFF000000 );
    DRAW_PT( 1 , x+16, ymax-16, 0xFF000000 );

    DRAW_PT( 1 , x- 4, ymax- 4, 0xFF000000 );
    DRAW_PT( 1 , x- 8, ymax- 8, 0xFF000000 );
    DRAW_PT( 1 , x-12, ymax-12, 0xFF000000 );
    DRAW_PT( 1 , x-16, ymax-16, 0xFF000000 );
  }

  */
//}



/*
#ifdef TEMP_TLC
  #undef  DIMX_ACQ
  #undef  DIMY_ACQ
  #define DIMX_ACQ 1920
  #define DIMY_ACQ 1080
#endif
  */

/*
// Ricerca il top del master tra x1 e x2
double compute_master_height( BYTE* img, int x1, int x2, int step, int& height_pix )
{

  int bordo_y  =  20;   // fascia di rispetto esterna

  int sogliah  = soglia[1]*4096;

  int x;
  int xd, yd;
  int y1;
  int pt1_l, pt2_l, pt3_l;
  int pt1_y, pt2_y, pt3_y;

  int npt = 0;
  double ytot = 0.0;

  for ( x=x1 ; x<=x2 ; x+=step )
  {

    xd =       x << 3; // xd iniziale
    yd = bordo_y << 3; // yd iniziale

    pt1_y = pt2_y = yd;
//    pt1_l = pt2_l = gb_brightness_test(img,xd,yd);
    pt1_l = pt2_l = gb_brightness(img,xd,yd);

    if ( pt1_l >= sogliah ) return( 0 );

    while ( pt1_l<sogliah && pt1_y<(DIMY_ACQ-bordo_y)*8 )
    {
      DRAW_PT( 0, x, pt1_y/8, 0xFF00FF00 );
      pt2_y  = pt1_y;
      pt2_l  = pt1_l;
      pt1_y += 64; // avanti 8 pixel
//      pt1_l  = gb_brightness_test(img,xd,pt1_y);
      pt1_l  = gb_brightness(img,xd,pt1_y);
    }


    if ( pt1_y>=(DIMY_ACQ-bordo_y)*8 )
    {
      //TRACE("[%d] BORDO SUPERIORE NON TROVATO PER xd=%d\n", ind, xd );
      //return 0.0;
      break;
    }

    //TRACE("TROVATO BORDO TRA %d e %d (%6.2lf e %6.2lf)\n", pt2_y, pt1_y, pt2_y/8.0, pt1_y/8.0 );

    while ( pt1_y-pt2_y > 1 )
    {

      pt3_y = (pt1_y+pt2_y)>>1;
      ASSERT( pt2_y<pt3_y && pt3_y<pt1_y );
//      pt3_l  = gb_brightness_test(img,xd,pt3_y);
      pt3_l  = gb_brightness(img,xd,pt3_y);

      //TRACE("Nuova iter SX: pt1-pt3-pt2 %d-%d-%d - pt3_l=%d\n", pt1_x, pt3_x, pt2_x, pt3_l/4096 );

      if ( pt3_l>sogliah ) 
      {
        pt1_y = pt3_y;
        pt1_l = pt3_l;
      }
      else
      {
        pt2_y = pt3_y;
        pt2_l = pt3_l;
      }
    }

    // TRACE("BORDO TROVATO TRA %d,%d e %d,%d (%6.2lf e %6.2lf)\n", pt2_y, pt2_l>>12, pt1_y, pt1_l>>12, pt2_y/8.0, pt1_y/8.0 );

    // vediamo chi ?pi?vicino
    if ( pt1_l-sogliah < sogliah-pt2_l ) y1 = pt1_y;
    else                                 y1 = pt2_y;


    ytot += y1;

    DRAW_PT( 0, x  , y1/8, 0xFFFF0000 );
    DRAW_PT( 0, x-4, y1/8, 0xFFFF0000 );
    DRAW_PT( 0, x+4, y1/8, 0xFFFF0000 );

    npt++;
  }


  if ( npt > 80*(x2-x1)/(step*100) ) // almeno 80% di punti trovati
  {
    char msg[40];
//    double height = ytot/(8.0*npt);

//    height_pix = int( 0.5 + height );
    height_pix = int( 0.5 + ytot/(8.0*npt) );

    if ( x1+x2>DIMX_ACQ ) // sul lato destro
    {
      for ( int i = DIMX_ACQ-1-200 ; i < DIMX_ACQ-1 ; i += 4 )
      {
        DRAW_PT( 0,            i, height_pix, 0x0000FF00 );  // 0xAARRGGBB
        DRAW_PT( 0, DIMX_ACQ-1-i, height_pix, 0x0000FF00 );  // 0xAARRGGBB
      }

//      double height_mm = (DIMY_ACQ-1.0-height)*scale_cf[0] - plateh_mm + plateh_ofs[0];
      double height_mm = pix_2_mm( 0, height_pix ) - plateh_mm;

      sprintf( msg, "H:%.3lf", height_mm );
      image_dsp[0]->Draw_text( msg, DIMX_ACQ/4-80, height_pix/4+7, 0x0000FF00 );

      return( height_mm );
    }
    else // lato sinistro
    {
      for ( int i = 0 ; i < 200 ; i += 4 )
      {
        DRAW_PT( 0,            i, height_pix, 0x0000FF00 );  // 0xAARRGGBB
        DRAW_PT( 0, DIMX_ACQ-1-i, height_pix, 0x0000FF00 );  // 0xAARRGGBB
      }

//      double height_mm = (DIMY_ACQ-1.0-height)*scale_cf[0] - plateh_mm + plateh_ofs[0];
      double height_mm = pix_2_mm( 0, height_pix ) - plateh_mm;

      char msg[40];
//      sprintf( msg, "H: %.1lf - %.3lf", height, height_mm );
      sprintf( msg, "H:%.3lf", height_mm );
//      image_dsp[0]->Draw_text( msg, 1, int(height)/4+7, 0x0000FF00 );
      image_dsp[0]->Draw_text( msg, 1, height_pix/4+7, 0x0000FF00 );

      return( height_mm );
    }


  }
  return 0.0; // pochi punti
}

*/

/*
#ifdef TEMP_TLC
  #undef  DIMX_ACQ
  #undef  DIMY_ACQ
  #define DIMX_ACQ 2456
  #define DIMY_ACQ 2056
#endif
  */









/***********************************************************************************************   

                       VARIABILI LEGATE ALL CONTROLLO SUL COLLO 

***********************************************************************************************/


/*

int neck_analysis( BYTE* img )
{

  #define FS_STPX   50
  #define FS_STPY   50

  int x, y;
  int nptblk = 0;

//  bottle_height_ms =  0.0; // reset measured total height 
  neck_angle_ms    = 90.0; // reset measured top tilt/angle

  // calcola altezza contorno (3 come base oppure pi?spessa per includere filetto e/o pente-sous-bague e/o puntop
  double hcontour = 3.0; // altezza base contorno
  if ( execute_p_point         && p_point_height+1.5         > hcontour ) hcontour = p_point_height+1.5;
  if ( execute_pente_sb        && pente_sb_height+1.5        > hcontour ) hcontour = pente_sb_height+1.5;
  if ( execute_diam_surfilet   && diam_surfilet_height       > hcontour ) hcontour = diam_surfilet_height;
  if ( execute_diam_collerette && diam_collerette_height+2.5 > hcontour ) hcontour = diam_collerette_height+2.5;

  int contour_height = 20 + int( 0.5 + hcontour / scale_cf[1] ); // ampiezza del bordo superiore per la ricerca del contorno

  // ricerca da dx a sx 
  x = DIMX_ACQ;

  while ( nptblk < 3 && x > FS_STPX ) 
  {
    x -= FS_STPX;
    nptblk = 0;
    for ( y=FS_STPY ; y<= DIMY_ACQ-FS_STPY ; y += FS_STPY ) if ( img[x+y*DIMX_ACQ]>40 ) nptblk++;
  }

  if ( x <= FS_STPX )  // prima ricerca fallita
  {
    bottle_height_miss++; // increase misses counter
    if ( bottle_height_miss > MAX_BOTTLEHEIGHT_MISSES ) bottle_height_ms = 0.0; // reset measured total height

    pView->tlctext_2[1]->SetText( "Bottle not found (x<FS_STPX)", 0 );
    return( 101 );
  }


  // Altrimenti cerchiamo la transizione tra x e x + FS_STPX
  int x1 = x;
  int x2 = x + FS_STPX;
  

  while ( nptblk >= 3 && x <= x2 ) 
  {
    x += 2;
    nptblk = 0;
    for ( y=FS_STPY ; y<= DIMY_ACQ-FS_STPY ; y += FS_STPY ) if ( img[x+y*DIMX_ACQ]>40 ) nptblk++;
  }

  if ( x > x2 )
  {
    // ERRORE
    bottle_height_miss++; // increase misses counter
    if ( bottle_height_miss > MAX_BOTTLEHEIGHT_MISSES ) bottle_height_ms =  0.0; // reset measured total height

    //bottle_height_ms =  0.0; // reset measured total height
    pView->tlctext_2[1]->SetText( "Bottle not found (x>x2)", 0 );
    return( 102 );
  }

  //TRACE("Transizione trovata in x=%d\n", x);

  if ( x-contour_height<10 || x+50>=DIMX_ACQ-10 )
  {
    // troppo vicino ai bordi
    bottle_height_miss++; // increase misses counter
    if ( bottle_height_miss > MAX_BOTTLEHEIGHT_MISSES ) bottle_height_ms =  0.0; // reset measured total height
   
    //bottle_height_ms =  0.0; // reset measured total height
    pView->tlctext_2[1]->SetText( "Bottle too close to image size", 0 );
    return( 103 );
  }

  // Lancia analisi contorno
  neck_contour_interp( 1, img, x-contour_height, 100, contour_height+50, DIMY_ACQ-200 );

  if ( npt < 100 )
  {
    bottle_height_miss++; // increase misses counter
    if ( bottle_height_miss > MAX_BOTTLEHEIGHT_MISSES ) bottle_height_ms =  0.0; // reset measured total height
    //bottle_height_ms =  0.0; // reset measured total height
    return 999; // too few points found
  }

  neck_contour_analysis();

  return( 0 ); // Analisi completata
}
*/

/*

// funzione che calcola il contorno e ritorna l'altezza in mm (0 se non trovato)
int neck_contour_analysis( void )
{
  #define AMP_SLP   20  // ampiezza dx/sx per il calcolo della pendenza media
  //#define AMP_AVG   10  // ampiezza per calcolo quote medie
  //#define AMP_CRV   20  // ampiezza dx/sx per il calcolo della curvatura media

  ASSERT( npt>100 && npt<NPT_MAX );

  CTimer timer;
  timer.reset();

 // TRACE("Inizio analisi di contorno - %d punti\n", npt );

//  npt = 8000;
//  TRACE("punto, x, y, dx, dy, slope\n" );
  char msg[40];
  int i;

  int *slope = (int*) malloc( npt*sizeof(int) );
  ZeroMemory( slope, npt*sizeof(int) );

  int    nptsta =     AMP_SLP+1;
  int    nptend = npt-AMP_SLP-1;


  int    htot   =   0;
  int    hnpt   =   0;
  
  int    xmax = 0; // altezza massima su contorno interp
  int    imax = 0; // indice dell'altezza massima su contorno interp
  int    xmin = 0; // altezza minima su contorno interp
  int    imin = 0; // indice dell'altezza minima su contorno interp


  // Calcolo tutte le pendenze 
  for ( i=nptsta ; i<nptend ; i++ )
  {
    // singola coppia di punti
    //slope[i] = DOUBLETOINT( atan2( double(ptx[i+AMP_SLP]-ptx[i-AMP_SLP]), double(pty[i+AMP_SLP]-pty[i-AMP_SLP]) ) * 5729.5779513082 );

    // coppia di triplette di punti
    slope[i] = DOUBLETOINT( atan2( double(ptx[i+AMP_SLP-1]+ptx[i+AMP_SLP]+ptx[i+AMP_SLP+1]-ptx[i-AMP_SLP-1]-ptx[i-AMP_SLP]-ptx[i-AMP_SLP+1]), double(pty[i+AMP_SLP-1]+pty[i+AMP_SLP]+pty[i+AMP_SLP+1]-pty[i-AMP_SLP-1]-pty[i-AMP_SLP]-pty[i-AMP_SLP+1]) ) * 5729.5779513082 );
    
    if ( xmax < ptx[i-1]+ptx[i]+ptx[i+1] )
    {
      xmax = ptx[i-1]+ptx[i]+ptx[i+1];
      imax = i;
    }


    //TRACE("%d,%d,%d,%.1lf\n", i, ptx[i]/8, pty[i]/8, 0.01*slope[i] );
    

  }


  int xmaxl = 0;
  int imaxl = 0;
  int xmaxr = 0;
  int imaxr = 0;

  // nuova ricerca altezza min/max
  
  int xsrcmin = int( 0.5 + xmax/3.0 ) - 8 * int( 1.0 / scale_cf[1] );
  //TRACE("hmax=%d in %d - xsrcmin=%d\n", xmax, imax, xsrcmin );

  int nptcen = (nptsta+nptend)/2;

  // cerca entrata zona vicina al top
  i = nptsta;
  while ( i<nptcen && ptx[i]<xsrcmin ) i++;

  while ( i<nptcen && slope[i]<300 ) i++;

  // lato sinistro cerca dove la derivata diventa minore di 2 gradi
  while ( i<nptcen && slope[i]>100 )
  {
    if ( !(i&31) ) DRAW_PT( 1, ptx[i]/8, pty[i]/8, 0x0000FF00 );  // 0xAARRGGBB
//    if ( xmaxl < ptx[i-1]+ptx[i]+ptx[i+1] )
    if ( xmaxl < ptx[i-1]+ptx[i]+ptx[i+1] && slope[i]>-200 && slope[i]<200 )
    {
      xmaxl = ptx[i-1]+ptx[i]+ptx[i+1];
      imaxl = i;
    }
    i++;
  }
  image_dsp[1]->Draw_marker( ptx[imaxl]/8, pty[imaxl]/8, 0x0037C8C8 );


  i = nptend;
  while ( i>nptcen && ptx[i]<xsrcmin ) i--;

  while ( i>nptcen && slope[i]>-300 ) i--;

  // lato destro cerca dove la derivata diventa minore di 2 gradi
  while ( i>nptcen && slope[i]<-100 )
  {
    if ( !(i&31) ) DRAW_PT( 1, ptx[i]/8, pty[i]/8, 0x0000FF00 );  // 0xAARRGGBB
//    if ( xmaxr < ptx[i-1]+ptx[i]+ptx[i+1] )
    if ( xmaxr < ptx[i-1]+ptx[i]+ptx[i+1] && slope[i]>-200 && slope[i]<200 )
    {
      xmaxr = ptx[i-1]+ptx[i]+ptx[i+1];
      imaxr = i;
    }
    i--;
  }
  image_dsp[1]->Draw_marker( ptx[imaxr]/8, pty[imaxr]/8, 0x0037C8C8 );

  int y = 0;


  if ( xmaxl && xmaxr && abs(xmaxl-xmaxr)<2400 )
  {
    bottle_height_px = int( 0.5 + (xmaxl>xmaxr?xmaxl:xmaxr) / 24.0 );

    y = (DIMX_ACQ-bottle_height_px)/4-9;

    double bottle_height_left = pix_2_mm( 1, xmaxl/24.0 ) - plateh_mm;
    sprintf( msg, "HtL: %.3lf", bottle_height_left );
    image_dsp[1]->Draw_text( msg, 1, y , 0x0000FF00 );
    if ( hauteur_results ) hauteur_results->Insert( bottle_height_left, 2 ); // insert result into category 0 LEFT

    double bottle_height_right = pix_2_mm( 1, xmaxr/24.0 ) - plateh_mm;
    sprintf( msg, "HtR: %.3lf", bottle_height_right );
    image_dsp[1]->Draw_text( msg, DIMY_ACQ/4-110, y, 0x0000FF00 );
    if ( hauteur_results ) hauteur_results->Insert( bottle_height_right, 3 ); // insert result into category 0 RIGHT

    if ( bottle_height_right>bottle_height_left )
    {
      bottle_height_ms = bottle_height_right;
      hauteur_results->Insert( bottle_height_ms, 0 );
      hauteur_results->Insert( bottle_height_right-bottle_height_left, 1 );
      if ( execute_planeite_db && planeite_db_results ) planeite_db_results->Insert( bottle_height_right - bottle_height_left );
    }
    else
    {
      bottle_height_ms = bottle_height_left;
      hauteur_results->Insert( bottle_height_ms, 0 );
      hauteur_results->Insert( bottle_height_left-bottle_height_right, 1 );
      if ( execute_planeite_db && planeite_db_results ) planeite_db_results->Insert( bottle_height_left - bottle_height_right );
    } 

    bottle_height_miss = 0; // level found! clear misses counter

  } 
  else // altezza NON valida
  {
    bottle_height_miss++; // increase misses counter
    if ( bottle_height_miss > MAX_BOTTLEHEIGHT_MISSES ) bottle_height_ms =  0.0; // reset measured total height
    bottle_height_ms =  0.0; // reset measured total height
    free( slope );
    return 99;
  }


//  double top_slope = ( sumxy*hnpt - sumx*sumy ) / ( sumx2*hnpt - sumx*sumx );
//  double top_inter = ( sumy - top_slope*sumx ) / hnpt;

// vecchia versione con lettura in gradi
//  neck_angle_ms    = atan2( top_slope, 1.0 )*RAD2DEG; // convert into degrees
//  if ( datarec_on && planeite_db_results ) planeite_db_results->Insert( neck_angle_ms, 0 ); // insert result into category 5 (Angle)



   

//         - - - - - - - - - - - - - - - - - - - - - - - - -
//         - - - - - -     P-POINT COMPUTING     - - - - - -
//         - - - - - - - - - - - - - - - - - - - - - - - - -

  #define SRC_AMP 1.5 // ampiezza di ricerca punto P (in mm)
  xmin = ( bottle_height_px - int( (p_point_height+SRC_AMP)/scale_cf[1] ) );
  xmax = ( bottle_height_px - int( (p_point_height-SRC_AMP)/scale_cf[1] ) );
  #undef SRC_AMP

  if ( execute_p_point && 5<xmin && xmin<xmax && xmax<DIMX_ACQ-5 )
  {
    int    ppointleft_a  = 0;
    int    ppointleft_b  = 0;
    int    ppointright_a = 0;
    int    ppointright_b = 0;
//    double ang           = abs( 0.01*(p_point_smax-p_point_smin) );
    double ang           = abs( p_point_smax - p_point_smin );
    double dist_ab       = 0.0;
    double ppoint_radius_left  = 0.0;
    double ppoint_radius_right = 0.0;

    xmin = xmin<<3;
    xmax = xmax<<3;

    int slope_min = int( 0.5 + 100.0 * p_point_smin );
    int slope_max = int( 0.5 + 100.0 * p_point_smax );

    i=nptsta;
    while ( ptx[i]<xmin && i<nptend ) i++;

//    while ( slope[i]<p_point_smax+100 && i<nptend ) i++;
    while ( slope[i]<slope_max+100 && i<nptend ) i++;
    
//    while ( slope[i]>p_point_smax && i<nptend ) i++;
    while ( slope[i]>slope_max && i<nptend ) i++;
    //TRACE("LEFT  P-POINT A %5d, %5d, %5d, %+3d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ppointleft_a = i;

//    while ( slope[i]<p_point_smax && slope[i]>p_point_smin && i<nptend ) i++;
    while ( slope[i]<slope_max && slope[i]>slope_min && i<nptend ) i++;
    //TRACE("LEFT  P-POINT B %5d, %5d, %5d, %+3d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ppointleft_b = i-1;

    while ( ptx[i]<=xmax && i<nptend ) i++;
    while ( ptx[i]> xmax && i<nptend ) i++;
    
//    while ( slope[i]>=-p_point_smin && i<nptend ) i++;
    while ( slope[i]>=-slope_min && i<nptend ) i++;
    //TRACE("RIGHT P-POINT A %5d, %5d, %5d, %+3d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ppointright_a = i;

//    while ( slope[i]<-p_point_smin && slope[i]>-p_point_smax && i<nptend ) i++;
    while ( slope[i]<-slope_min && slope[i]>-slope_max && i<nptend ) i++;
    //TRACE("RIGHT P-POINT B %5d, %5d, %5d, %+3d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ppointright_b = i-1;

    //TRACE("LEFT P-POINT A %d,%d\n", ptx[ppointleft_a]-x0_8, pty[ppointleft_a]-y0_8 );
    //TRACE("LEFT P-POINT B %d,%d\n", ptx[ppointleft_b]-x0_8, pty[ppointleft_b]-y0_8 );
    if ( i<nptend )
    {
      dist_ab = sqrt( double( ptx[ppointleft_a]-ptx[ppointleft_b] ) * ( ptx[ppointleft_a]-ptx[ppointleft_b] ) );
      dist_ab = dist_ab * scale_cf[1] / 8.0; // convert into mm
      ppoint_radius_left = 0.5 * dist_ab / sin( 0.5 * ang * DEG2RAD );
      
      dist_ab = sqrt( double( ptx[ppointright_a]-ptx[ppointright_b] ) * ( ptx[ppointright_a]-ptx[ppointright_b] ) );
      dist_ab = dist_ab * scale_cf[1] / 8.0; // convert into mm
      ppoint_radius_right = 0.5 * dist_ab / sin( 0.5 * ang * DEG2RAD );
    }

    if ( p_point_results )
    {
      p_point_results->Insert( ppoint_radius_left,  0 ); // insert result into category 0 (left)
      p_point_results->Insert( ppoint_radius_right, 1 ); // insert result into category 1 (right)
    }

    y += 30;
    sprintf( msg, "PPr: %.1lf/%.1lf", ppoint_radius_left, ppoint_radius_right );
    image_dsp[1]->Draw_text( msg, DIMY_ACQ/8-82, y , 0x00000000 );

  }

    //if ( ppoint_radius_left < 0.3 || ppoint_radius_left > 0.9 ) save_last_image = TRUE;

    //TRACE("LEFT P-POINT  - dist=%.3lf - ang=%.2lf - radius=%.3lf\n", dist_ab, ang, ppoint_radius_left );

//    y += 22;
//    sprintf( msg, "PPr:%.2lf", ppoint_radius_left );
//    image_dsp[1]->Draw_text( msg, 1, y , 0x0000FF00 );

    //TRACE("RIGHT P-POINT A %d,%d\n", ptx[ppointright_a]-x0_8, pty[ppointright_a]-y0_8 );
    //TRACE("RIGHT P-POINT B %d,%d\n", ptx[ppointright_b]-x0_8, pty[ppointright_b]-y0_8 );
    //TRACE("RIGHT P-POINT - dist=%.3lf - ang=%.2lf - radius=%.3lf\n", dist_ab, ang, ppoint_radius_right );

    //if ( ppoint_radius_right < 0.3 || ppoint_radius_right > 0.9 ) save_last_image = TRUE;



//         - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//         - - - - - -     SUB-FINISH SLOPE COMPUTING    - - - - - -
//         - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  #define SRC_AMP 1.5 // ampiezza di ricerca pendenza sottobaga (in mm)
  xmin = ( bottle_height_px - int( (pente_sb_height+SRC_AMP)/scale_cf[1] ) );
  xmax = ( bottle_height_px - int( (pente_sb_height-SRC_AMP)/scale_cf[1] ) );
  #undef SRC_AMP

  if ( execute_pente_sb && 5<xmin && xmin<xmax && xmax<DIMX_ACQ-5 ) 
  {


    xmin = xmin<<3;
    xmax = xmax<<3;
   


    //int status = 0;
    //int p_point_smin = 3500;
    //int p_point_smax = 3500;

    //int slope_th = int( 100.0 * pente_sb_max + 5.0 );


//    int slope_th1 = 18000 - int( 100.0 * pente_sb_min ); // prima soglia da superare per il rilevamento del punto in basso
//    int slope_th2 = 18000 - int( 100.0 * pente_sb_max ); // seconda soglia da superare per il rilevamento del punto in alto


    int slope_th1 = 18000 - int( 100.0 * ( pente_sb_max + pente_sb_delta1 ) ); // prima soglia da superare per il rilevamento del punto in basso
    int slope_th2 = 18000 - int( 100.0 * ( pente_sb_max + pente_sb_delta2 ) ); // seconda soglia da superare per il rilevamento del punto in alto


    double psb_left  = 0.0;
    double psb_right = 0.0;

    int ista = 0;
    int iend = 0;

//    for ( i=AMP_SLP+1 ; i<npt-AMP_SLP-1 ; i++ )
//    {
//      if ( ptx[i]>xmin && ptx[i]<xmax ) status = 0;
//      else

    i = nptsta;
    while ( ptx[i]<xmin && i<nptend ) i++;
    //TRACE("PSB(L) 10 inizio ricerca PSB sx i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );

//    while ( 18000-slope[i]>slope_th-50 && ptx[i]<xmax && i<nptend ) i++; // inizio psb sx
    while ( slope[i]<slope_th1+50 && ptx[i]<xmax && i<nptend ) i++; // inizio psb sx
    //TRACE("PSB(L) 20 inizio PSB sx in i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ista = i;
 
//    while ( 18000-slope[i]<slope_th+50 && ptx[i]<xmax && i<nptend ) i++; // fine psb sx
    while ( slope[i]>slope_th2-50 && ptx[i]<xmax && i<nptend ) i++; // fine psb sx
    //TRACE("PSB(L) 30 fine PSB sx i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    iend = i;

    if ( pente_sb_perc != 100 )
    {
      int ista2 = ((100+pente_sb_perc)*ista+(100-pente_sb_perc)*iend)/200;
      int iend2 = ((100-pente_sb_perc)*ista+(100+pente_sb_perc)*iend)/200;
      ista = ista2>=nptsta ? ista2 : nptsta;
      iend = iend2<=nptend ? iend2 : nptend;
    }

    // distanza tra i due punti trovati (in pixel originali)
    double dist = 0.125 * sqrt( double(ptx[iend]-ptx[ista])*(ptx[iend]-ptx[ista]) + double(pty[iend]-pty[ista])*(pty[iend]-pty[ista]) );
    if ( dist*scale_cf[1]>0.35 ) psb_left = 180.0 - RAD2DEG * atan2( double(ptx[iend]-ptx[ista]), double(pty[iend]-pty[ista]) );

//    if ( abs(ptx[iend]-ptx[ista])>80.0 && abs(pty[iend]-pty[ista])>80.0 ) // almeno 10 pixel x lato!
//      psb_left = 180.0 - RAD2DEG * atan2( double(ptx[iend]-ptx[ista]), double(pty[iend]-pty[ista]) );


    //TRACE("PSB(left)=%.1lf\n", psb_left );
//    slope[i] = DOUBLETOINT( atan2( double(ptx[i+AMP_SLP-1]+ptx[i+AMP_SLP]+ptx[i+AMP_SLP+1]-ptx[i-AMP_SLP-1]-ptx[i-AMP_SLP]-ptx[i-AMP_SLP+1]), double(pty[i+AMP_SLP-1]+pty[i+AMP_SLP]+pty[i+AMP_SLP+1]-pty[i-AMP_SLP-1]-pty[i-AMP_SLP]-pty[i-AMP_SLP+1]) ) * 5729.5779513082 );


    while ( ptx[i]<=xmax && i<nptend ) i++; // arriva alla fine dell'area di ricerca
   

    // lato destro
    while ( ptx[i]>xmax && i<nptend ) i++;
    //TRACE("PSB(R) 40 inizio ricerca PSB dx i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );


//    while ( 18000+slope[i]>slope_th-50 && ptx[i]>xmin && i<nptend ) i++; // inizio psb dx
    while ( slope[i]>-slope_th2-50 && ptx[i]>xmin && i<nptend ) i++; // inizio psb dx
    //TRACE("PSB(R) 50 inizio PSB dx i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    ista = i;

//    while ( 18000+slope[i]<slope_th+50 && ptx[i]>xmin && i<nptend ) i++; // fine psb dx
    while ( slope[i]<-slope_th1+50 && ptx[i]>xmin && i<nptend ) i++; // fine psb dx
    //TRACE("PSB(R) 60 fine PSB dx i=%d (%d,%d) slope=%d\n", i, ptx[i]/8, pty[i]/8, slope[i] );
    iend = i;

    if ( pente_sb_perc != 100 )
    {
      int ista2 = ((100+pente_sb_perc)*ista+(100-pente_sb_perc)*iend)/200;
      int iend2 = ((100-pente_sb_perc)*ista+(100+pente_sb_perc)*iend)/200;
      ista = ista2>=nptsta ? ista2 : nptsta;
      iend = iend2<=nptend ? iend2 : nptend;
    }

    // distanza tra i due punti trovati (in pixel originali)
    dist = 0.125 * sqrt( double(ptx[iend]-ptx[ista])*(ptx[iend]-ptx[ista]) + double(pty[iend]-pty[ista])*(pty[iend]-pty[ista]) );
    if ( dist*scale_cf[1]>0.35 ) psb_right = 180.0 + RAD2DEG * atan2( double(ptx[iend]-ptx[ista]), double(pty[iend]-pty[ista]) );

 //   if ( abs(ptx[iend]-ptx[ista])>80.0 && abs(pty[iend]-pty[ista])>80.0 ) // almeno 10 pixel x lato!
 //     psb_right = 180.0 + RAD2DEG * atan2( double(ptx[iend]-ptx[ista]), double(pty[iend]-pty[ista]) );

    //TRACE("PSB(right)=%.1lf\n", psb_right );

    if ( pente_sb_results )
    {
      pente_sb_results->Insert( psb_left,  0 ); // insert left result
      pente_sb_results->Insert( psb_right, 1 ); // insert right result
    }
    sprintf( msg, "PSB: %.1lf/%.1lf", psb_left, psb_right );
    image_dsp[1]->Draw_text( msg, DIMY_ACQ/8-70, DIMX_ACQ/4-(xmin+xmax)/64+2, 0x00000000 );

    //TRACE("Fine ricerca PSB - left=%.1lf right=%.1lf\n", psb_left, psb_right );
  }






//         - - - - - - - - - - - - - - - - - - - - - - - - - -
//         - - - - - -     SCREW DIAM COMPUTING    - - - - - -
//         - - - - - - - - - - - - - - - - - - - - - - - - - -

  xmin = ( bottle_height_px - int( diam_surfilet_height/scale_cf[1] ) );
  xmax = ( bottle_height_px - int(                  0.5/scale_cf[1] ) );

  if ( execute_diam_surfilet && 5<xmin && xmin<xmax && xmax<DIMX_ACQ-5 )
  {
    xmin = xmin<<3;
    xmax = xmax<<3;

    int ymin = DIMY_ACQ<<3;
    int ymax = 0;
    int imin = 0;
    int imax = 0;

    for ( i=1 ; i<npt-1 ; i++ )
    {
      if ( ptx[i]>xmin && ptx[i]<xmax )
      {
        if ( pty[i]<ymin ) 
        {
          ymin = pty[i];
          imin = i;
        }
        if ( pty[i]>ymax ) 
        {
          ymax = pty[i];
          imax = i;
        }
      }
    }

    double diam_sur_filet = scale_cf[1]*(ymax-ymin)*0.125;

    ymin =      ymin>>3;
    ymax =      ymax>>3;
    xmin = ptx[imin]>>3;
    xmax = ptx[imax]>>3;

    if ( diam_surfilet_results && diam_sur_filet>0.8*diam_surfilet_min && diam_surfilet_max*1.2  ) diam_surfilet_results->Insert( diam_sur_filet, 0 ); // insert result
    sprintf( msg, "DSF:%.3lf", diam_sur_filet );
    image_dsp[1]->Draw_text( msg, DIMY_ACQ/8-50, DIMX_ACQ/4-(xmin+xmax)/8+12, 0x00000000 );

    for ( i = -40 ; i <= 40 ; i += 4 )
    {
      DRAW_PT( 1, xmin+i, ymin - 4, 0x00FF0000 );  // 0xAARRGGBB
      DRAW_PT( 1, xmin+i, ymin,     0x000000FF );  // 0xAARRGGBB
      DRAW_PT( 1, xmin+i, ymin + 4, 0x00FF0000 );  // 0xAARRGGBB

      DRAW_PT( 1, xmax+i, ymax - 4, 0x00FF0000 );  // 0xAARRGGBB
      DRAW_PT( 1, xmax+i, ymax,     0x000000FF );  // 0xAARRGGBB
      DRAW_PT( 1, xmax+i, ymax + 4, 0x00FF0000 );  // 0xAARRGGBB
    }
    //TRACE("DIAM_FILET: xmin/xmax %d/%d - ymin %d/%d - ymax %d/%d (%dus)\n", xmin/8, xmax/8, ymin/8, imin, ymax/8, imax, timer.elapsed_us() );
  }


//         - - - - - - - - - - - - - - - - - - - - - - - - - -
//         - - - - -    COLLERETTE DIAM COMPUTING    - - - - -
//         - - - - - - - - - - - - - - - - - - - - - - - - - -

//  xmin = ( bottle_height_px - int( diam_collerette_max/scale_cf[1] ) );
//  xmax = ( bottle_height_px - int( diam_collerette_min/scale_cf[1] ) );
  xmin = ( bottle_height_px - int( (diam_collerette_height+2.5)/scale_cf[1] ) );
  xmax = ( bottle_height_px - int( (diam_collerette_height-2.5)/scale_cf[1] ) );

  if ( execute_diam_collerette && 5<xmin && xmin<xmax && xmax<DIMX_ACQ-5 )
  {
    xmin = xmin<<3;
    xmax = xmax<<3;

    int ymin = DIMY_ACQ<<3;
    int ymax = 0;
    int imin = 0;
    int imax = 0;

    for ( i=1 ; i<npt-1 ; i++ )
    {
      if ( ptx[i]>xmin && ptx[i]<xmax )
      {
        if ( pty[i]<ymin ) 
        {
          ymin = pty[i];
          imin = i;
        }
        if ( pty[i]>ymax ) 
        {
          ymax = pty[i];
          imax = i;
        }
      }
    }

    double diam_collerette = scale_cf[1]*(ymax-ymin)*0.125;

    ymin =      ymin>>3;
    ymax =      ymax>>3;
    xmin = ptx[imin]>>3;
    xmax = ptx[imax]>>3;

    if ( diam_collerette_results ) diam_collerette_results->Insert( diam_collerette, 0 ); // insert result
    sprintf( msg, "DC:%.3lf", diam_collerette );
    image_dsp[1]->Draw_text( msg, DIMY_ACQ/8-50, DIMX_ACQ/4-(xmin+xmax)/8+12, 0x00000000 );

    for ( i = -40 ; i <= 40 ; i += 4 )
    {
      DRAW_PT( 1, xmin+i, ymin - 4, 0x00FF0000 );  // 0xAARRGGBB
      DRAW_PT( 1, xmin+i, ymin,     0x000000FF );  // 0xAARRGGBB
      DRAW_PT( 1, xmin+i, ymin + 4, 0x00FF0000 );  // 0xAARRGGBB

      DRAW_PT( 1, xmax+i, ymax - 4, 0x00FF0000 );  // 0xAARRGGBB
      DRAW_PT( 1, xmax+i, ymax,     0x000000FF );  // 0xAARRGGBB
      DRAW_PT( 1, xmax+i, ymax + 4, 0x00FF0000 );  // 0xAARRGGBB
    }
    //TRACE("DIAM_COLLERETTE: xmin/xmax %d/%d - ymin %d/%d - ymax %d/%d (%dus)\n", xmin/8, xmax/8, ymin/8, imin, ymax/8, imax, timer.elapsed_us() );
  }





  free( slope );
  return( 0 );
}

*/


  /*
  // Calcolo la variazione delle pendenze
  for ( int i=11 ; i<npt-11 ; i++ )
  {
//    slope[i] = slp;
    TRACE("%5d,%4.2lf\n", i, (slope[i+11]+slope[i+10]+slope[i+9]) - (slope[i-9]+slope[i-10]+slope[i-11]) );
  }
  */



  /*
  // Calcolo la variazione delle pendenze
  for ( int i=10 ; i<npt-10 ; i += 10 )
  {
//    slope[i] = slp;

    double slope_avg = 0.186*slope[i]+0.169*(slope[i-1]+slope[i+1])+0.125*(slope[i-2]+slope[i+2])+0.076*(slope[i-3]+slope[i+3])+0.038*(slope[i-4]+slope[i+4]);
    //TRACE("%5d,%4.2lf\n", i, slope_avg );
  }
  */


//  TRACE("Fine analisi pendenza - %d punti - %d us\n", npt, timer.elapsed_us() );



  /*
  for ( i=0 ; i<AMP_AVG ; i++ )
  {
    xacc += ptx[i];
    yacc += pty[i];
  }

  for ( i=AMP_AVG ; i<npt ; i++ )
  {
    xacc += ptx[i] - ptx[i-AMP_AVG];
    yacc += pty[i] - pty[i-AMP_AVG];

    if ( xmax<xacc )
    {
      xmax = xacc;
      xind = i;
    }

    if ( ymin>yacc )
    {
      ymin = yacc;
      yin1 = i;
    }

    if ( ymax<yacc )
    {
      ymax = yacc;
      yin2 = i;
    }

  }


 // TRACE("xmax=%d in xind=%d\n", xmax, xind );

  xmax = xmax/AMP_AVG;
  xind = xind - AMP_AVG/2;

  ymin = ymin/AMP_AVG;
  yin1 = yin1 - AMP_AVG/2;

  ymax = ymax/AMP_AVG;
  yin2 = yin2 - AMP_AVG/2;


 // TRACE("(corretti) xmax=%d in xind=%d\n", xmax, xind );
 // TRACE("(corretti) ymin=%d in yin2=%d\n", ymin, yin1 );
 // TRACE("(corretti) ymax=%d in yin1=%d\n", ymax, yin2 );
 // TRACE("Fine analisi di contorno - %d punti - %d us\n", npt, timer.elapsed_us() );

 */


/*
  double diam = scale_cf[ind]*(y2-y1)/8.0;

  char msg[20];
  sprintf( msg, "%7.3lf", diam );

  image_dsp[ind]->Draw_text( msg, 1, (DIMX_ACQ-x)/4+6, 0x0000FF00 );

  image_dsp[ind]->Draw_marker( x, y1>>3 );
  image_dsp[ind]->Draw_marker( x, y2>>3 );
  */




//    TRACE("%5d, %5d, %5d, %2.0lf, %2.0lf, %5.1lf\n", i, ptx[i], pty[i], 0, 0, 0.0 );
//  }


  /*
  for ( int i=0 ; i<AMP_SLP ; i++ )
  {
//    TRACE("%5d, %5d, %5d, %2.0lf, %2.0lf, %5.1lf\n", i, ptx[i], pty[i], 0, 0, 0.0 );
  }

  double tot = 0.0;

  for ( int i=AMP_SLP ; i<npt-AMP_SLP ; i++ )
  {
    double dx  = ptx[i+AMP_SLP]-ptx[i-AMP_SLP];
    double dy  = pty[i+AMP_SLP]-pty[i-AMP_SLP];
    double slp = atan2( dy, dx );

    tot += slp;

//    TRACE("%5d, %5d, %5d, %2.0lf, %2.0lf, %5.1lf\n", i, ptx[i], pty[i], dx, dy, slp*RAD2DEG );
  }

  for ( int i=npt-AMP_SLP ; i<npt ; i++ )
  {
//    TRACE("%5d, %5d, %5d, %2.0lf, %2.0lf, %5.1lf\n", i, ptx[i], pty[i], 0, 0, 0.0 );
  }
  */


//  TRACE("Fine analisi di contorno - %d punti - %d ms - tot=%8.2lf\n", npt, timer.elapsed_ms(), tot );

  /*

  double slope_var[2*AMP_CRV+1];
  double slope_tot = 0.0;
  ZeroMemory( slope_var, sizeof(slope_var) );

  double slp_old = 0.0;

  for ( int i=AMP_SLP ; i<npt-AMP_SLP ; i++ )
  {
    double dx  = ptx[i+AMP_SLP]-ptx[i-AMP_SLP];
    double dy  = pty[i+AMP_SLP]-pty[i-AMP_SLP];
    double slp = atan2( dy, dx );

    int sv = i % (2*AMP_CRV+1);
    ASSERT( sv>=0 && sv<(2*AMP_CRV+1) );
    slope_tot -= slope_var[sv];
    slope_var[sv] = slp - slp_old;
    slope_tot += slope_var[sv];

    TRACE("punto %d, %5.4lf, %5.4lf, %6.4lf\n", i, slp, slp-slp_old, slope_tot );

    slp_old = slp;

    if ( dx )
    {
      //TRACE("punto %d,%d,%d,%d\n", i, ptx[i], pty[i], int(0.5+slope*RAD2DEG) );

    }
  }

  */








/*

int neck_contour_interp( int ind, BYTE* img, int x0, int y0, int dimx, int dimy )
{

  //#define SAVE_CONTOUR_IMAGE

  ASSERT( 5<=x0 && (x0+dimx)<DIMX_ACQ-5 );
  ASSERT( 5<=y0 && (y0+dimy)<DIMY_ACQ-5 );

  int x , y;
  int y1, y2;
  int sogliah = soglia[ind] << 12;

  CTimer timer;

  char msg[200];

  timer.reset();

 // CopyMemory( image_dsp[1]->ptr_buf, img, DIMX_ACQ*DIMY_ACQ );
 // image_dsp[1]->Save_image( "to_process" );

  #ifdef SAVE_CONTOUR_IMAGE
    FCObjImage temp;
    temp.Create( dimx*8, dimy*8, 32 );
    UINT*  temp_ptr = (UINT*) temp.GetMemStart();
    TRACE ("Creating interpolated-contour image..\n" );
    for ( y=0 ; y<dimy*8 ; y++ )
    {
      for ( x=0 ; x<dimx*8 ; x++ )
      {
        BYTE lum = gb_brightness(img,(x0<<3)+x,(y0<<3)+y) >> 12;
        temp_ptr[x+(dimy*8-1-y)*dimx*8] = lum | lum<<8 | lum<<16 | 0xFF000000;
      }
    }
  #endif

//  temp.Save( L"p_point_test.png" );
//  TRACE ("Interpolated image saved\n" );
//  return;


  for ( y=y0 ; y<y0+dimy ; y += 16 )
  {
    DRAW_PT( ind, x0,      y, 0xFF0000FF );    
    DRAW_PT( ind, x0+dimx, y, 0xFF0000FF );    
  }

  for ( x=x0 ; x<x0+dimx ; x += 16 )
  {
    DRAW_PT( ind, x, y0,      0xFF0000FF );    
    DRAW_PT( ind, x, y0+dimy, 0xFF0000FF );    
  }



  if ( vertical_edge_interp( ind, img, x0, y1, y2 ) == 0.0 )
  {
    //TRACE("Punti di partenza e arrivo non trovati\n");
    //pView->tlctext_2[ind]->SetText( "vertical edge fallito", 0 );
    return 199;
  }

  //TRACE("CALIBRO INIZIALE x=%d BORDI %d,%d (%6.2lf,%6.2lf)\n", x0+1, y1, y2, y1/8.0, y2/8.0 );

  int i;
  int iold;
  int imin;
  int vmin;
  int val;

  int xd, yd;

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6     2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3



//  ptx[npt] = (x0+1) << 3;
//  ptx[npt] = (150) << 3;

  x0_8 = x0<<3;
  y0_8 = y0<<3;

  npt = 0;
  ptx[0] = x0_8;
  pty[0] = y1; // punto di partenza!




  //temp_ptr[ptx[npt]-x0*8+(dimy*8-1-pty[npt]+y0*8)*dimx*8] = 0xFFFF00FF;




  //TRACE("Ricerca contorno a partire da %d,%d - BBOX (%d,%d)-(%d,%d)\n", ptx[npt], pty[npt], (x0<<3), (y0<<3), (x0+dimx)<<3, (y0+dimy)<<3 );

  iold = 2; // start search rightward

  while ( npt<NPT_MAX-3 && ptx[npt]>=x0_8 && ptx[npt]<((x0+dimx)<<3) && pty[npt]>y0_8 && pty[npt]<((y0+dimy)<<3) )
  {
    vmin = 1000<<12;

//    for ( int di=-2 ; di<=2 ; di++ )  // cerco il punto pi?vicino alla soglia
    for ( int di=-1 ; di<=1 ; di++ )  // cerco il punto pi?vicino alla soglia
    {
      i   = (iold+di)&7;
      xd  = ptx[npt]+ofsx[i];
      yd  = pty[npt]+ofsy[i];
      val = gb_brightness(img,xd,yd);
      //TRACE("i=%d val=%d\n", i, val[i]>>12 );
      if ( abs(val-sogliah)<vmin )
      {
        vmin = abs(val-sogliah);
        imin = i;
        ptx[npt+1] = xd;
        pty[npt+1] = yd;
        //TRACE("nuovo minimo imin=%d vmin=%d\n", imin, vmin>>12 );
      }
    }
    npt++;
    //TRACE("punto[%2d]=(%d,%d) - imin=%d vmin=%d\n", npt, ptx[npt], pty[npt], imin, vmin>>12 );

    //if ( npt<700 ) TRACE("punto %d,%d,%d,%d\n", npt, ptx[npt], pty[npt], dxy[npt] );

    // ASSERT( npt!=8224 );
    //if ( npt>8210 && npt<8240 ) TRACE("punto %d,%d,%d\n", npt, ptx[npt]-x0_8, pty[npt]-y0_8 );

    #ifdef SAVE_CONTOUR_IMAGE
      temp_ptr[ptx[npt]-x0_8+(dimy*8-1-pty[npt]+y0_8)*dimx*8] = 0xFFFF00FF;
    #endif

    if ( !(npt&31) ) DRAW_PT( 1 , (ptx[npt]>>3), (pty[npt]>>3), 0xFFFF00FF );

//    if ( !(npt&31) ) TRACE("punto %d,%d,%d\n", npt, ptx[npt], pty[npt] );

    iold = imin;
  }



  #ifdef SAVE_CONTOUR_IMAGE
    temp.Save( L"interpolated_neck.png" );
    TRACE ("Interpolated image saved in %4.1lf seconds\n", timer.elapsed() );
  #endif



  if ( ptx[npt]>=((x0+dimx)<<3) )
  {
    sprintf( msg, "Terminato per uscita verso destra/alto - %d punti trovati", npt );
    //TRACE( "%s\n", msg );
  }

  if ( pty[npt]<=y0_8 )
  {
    sprintf( msg, "Terminato per uscita verso il alto/sinistra - %d punti trovati", npt );
    //TRACE( "%s\n", msg );
  }

  if ( pty[npt]>=((y0+dimy)<<3) )
  {
    sprintf( msg, "Terminato per uscita verso il basso/destra - %d punti trovati", npt );
    //TRACE( "%s\n", msg );
  }

  if ( npt>=NPT_MAX-3 )
  {
    sprintf( msg, "Terminato per max numero punti %d trovati", npt );
    //TRACE( "%s\n", msg );
  }

  if ( ptx[npt]<x0_8 )
  {
    sprintf( msg, "Ricerca contorno conclusa - %d punti - da %d,%d a %d,%d", npt, ptx[0], pty[0], ptx[npt-1], pty[npt-1] );
    //TRACE( "%s\n", msg );
  }
  //pView->tlctext_2[ind]->SetText( msg, 0 );



  //sprintf( msg, "Processing time %d ms", timer.elapsed_ms() );
  //pView->tlctext_3[ind]->SetText( msg, 0 );

  return 0;
}

*/

//wchar_t* groupnames[3]; // = { string_542[NLANGS], string_542[NLANGS], string_542[NLANGS] }; // cap - bottom - cosmetic


int Change_view( int inp/*=-1*/ )  // no inp: cycle - 0: first group - 1: second group
{
  static int group = 0; // 0=machine layout - 1=first tc group - 2=second group .....

  int ngroups = 0;


  #ifdef FILLEV
    return 0;
  #endif

  #ifdef STOPPER_CTRL
    return 0;
  #endif


	/*if (IS_PC1)
		ngroups = 1 + sizeof(tlcgroupPC1) / (2 * sizeof(UINT));*/
	if (1)
		ngroups = 1 + sizeof(tlcgroupPC1) / (2 * sizeof(UINT));
  
	if (IS_PC2)
		ngroups = 1 + sizeof(tlcgroupPC2) / (2 * sizeof(UINT));

  #ifdef TIVS_S40
    ngroups = 1 + 5;
    //return 0;
  #endif

	#ifdef TIVS_S60
	if ( IS_PC2 )
	{
    ngroups = 1 + 3;
	}

	if ( IS_PC2 )
	{
    ngroups = 3;
	}

    //return 0;
	#endif


  #ifdef TIVS_C40
    if ( IS_PC2 )
    {
      #if STN1==CAKESIDE_ARBACK  
        ngroups++;
      #endif
      #if STN3==CAKETOP_ARDIRECT  
        ngroups++;
      #endif
      #ifdef NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
        ngroups--;
      #endif
    }
  #endif // TIVS_C40

  #ifdef TIVS_F40
    if ( IS_PC2 )
    {
      #if STN1==CAKESIDE_ARBACK  
        ngroups++;
      #endif
      #if STN3==CAKETOP_ARDIRECT  
        ngroups++;
      #endif
      #ifdef NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
        ngroups--;
      #endif
    }
  #endif // TIVS_F40

  #ifdef TIVS_A20L
    ngroups = 3;
  #endif // TIVS_A20L

//Pour added 20180523

	#ifdef TIVS_A20
    ngroups = 3;
  #endif // TIVS_A20L

//Pour added 20180523

		#ifdef TIVS_F20
    ngroups = 6;
  #endif // TIVS_F20


 /* #if SERIAL_N==201300502 // Ampoule machine for HENG RUI - includes FO color as last camera
    ngroups++;  // one more view for FO color camera alone
  #endif*/

  // increase and cycle mode
  if ( inp>=0 && inp<ngroups ) group = inp;
  else group = (group+1)%ngroups;

  if ( !layout_dsp && group==0 ) group=1;

  pView->Close_all_menus();

  Show_hide( group );
  
  int next = (group+1)%ngroups;
  if ( !layout_dsp && next==0 ) next=1;

  if ( chview_btn ) chview_btn->SetWindowText( groupnames[next][lng] ); // groupnames[(group+1)%ngroups][lng] );
  TRACE( L"Change_view: <%s> activated for showing\n", groupnames[next][lng] );

  return 0;
}


int Movie_startstop( int inp )
{
  static BOOL movie_active = FALSE;

  UINT i;
  CString txt;

	pView->Close_all_menus();
	TRACE( L"Movie_startstop\n" );

  // force start
  if ( inp==10 && movie_active ) return 0; // already active, just return

  // force stop
  if ( inp==20 && !movie_active ) return 0; // already stopped, just return

  if ( movie_active )  // Running, try to stop all
  {
  	//TRACE( L"Movie_startstop: movie_active TRUE, issuing stop command\n" );
    //for ( i=0 ; i<num_tlc ; i++ ) if ( CHK_BIT(thread_status,10+i) ) SET_BIT( thread_exit, 10+i );

    inspection_btn->SetWindowText( string_154[lng] );
    movie_btn->SetWindowText     ( string_154[lng] );

    UINT mask = 0;
    for ( i=0 ; i<num_tlc ; i++ ) if ( CHK_BIT(thread_status,10+i) ) SET_BIT( mask, 10+i );
    thread_exit = mask;
    CTimer timer;
    while ( (thread_status&mask) && timer.elapsed_sec()<10 )
    {
      MSG msg; 
      PeekMessage( &msg, NULL,  0, 0, PM_REMOVE);
      Sleep(5);
    }
    if ( thread_status&mask )
    {
      swprintf( msg_err, 200, L"Movie_startstop: timeout while waiting thread_status (%d)", thread_status&mask );
      Message_box( msg_err );
    }


    inspection_btn->SetWindowText( string_150[lng] );
    //inspection_btn->EnableWindow( TRUE );
    inspection_btn->ShowWindow( SW_SHOW );

    movie_btn->SetWindowText( string_160[lng] );
    //movie_btn->EnableWindow( TRUE );
    movie_btn->ShowWindow( SW_SHOW );

    if ( inspmode_btn ) inspmode_btn->ShowWindow( SW_SHOW );

    movie_active = FALSE;

    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_063[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_062[lng], file_manager.recipe_name );
    #endif
    //txt.Format( string_062[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

  }
  else                    // Stopped, try to start all
  { 

    TRACE( L"Movie_startstop: movie_active FALSE, start movie thread/s\n" );

    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_061[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_060[lng], file_manager.recipe_name );
    #endif
    //txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );


    #ifdef TIVS_C60  // freeze/dried vials machine

    if ( IS_PC1 )
    {
      // cap
      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,            LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,            LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,            LPVOID(2) ); // seal TC-C
      #endif

      AfxBeginThread( movie_3d, LPVOID(5) ); // TC_D color

      if ( freezemode_on ) // freeze mode
      {
        AfxBeginThread( movie_3d,        LPVOID( 3) ); // bottom HA
        AfxBeginThread( movie_3d,        LPVOID( 4) ); // bottom LA
      }
      else                 // nothing in liquid mode
      {
      }

      AfxBeginThread( movie_3d_linear, LPVOID( 6) ); // CM_A
      AfxBeginThread( movie_3d_linear, LPVOID( 7) ); // CM_B
      AfxBeginThread( movie_3d_linear, LPVOID( 8) ); // CM_C
      AfxBeginThread( movie_3d_linear, LPVOID( 9) ); // CM_D

    }


    if ( IS_PC2 )
    {
      if ( freezemode_on )
      {
        AfxBeginThread( movie_3d_linear, LPVOID( 0) ); // SC_A
        AfxBeginThread( movie_3d_linear, LPVOID( 1) ); // SC_B
        AfxBeginThread( movie_3d_linear, LPVOID( 2) ); // SC_C
        AfxBeginThread( movie_3d_linear, LPVOID( 3) ); // SC_D

        AfxBeginThread( movie_3d_linear, LPVOID( 8) ); // HL_A
        AfxBeginThread( movie_3d_linear, LPVOID( 9) ); // HL_B
        AfxBeginThread( movie_3d_linear, LPVOID(10) ); // HL_C
        AfxBeginThread( movie_3d_linear, LPVOID(11) ); // HL_C
      }
      else // liquid mode
      {
        AfxBeginThread( movie_3d, LPVOID( 4) ); // P1_A
        AfxBeginThread( movie_3d, LPVOID( 5) ); // P1_B
        AfxBeginThread( movie_3d, LPVOID( 6) ); // P1_C 
        AfxBeginThread( movie_3d, LPVOID( 7) ); // P1_D

        AfxBeginThread( movie_3d, LPVOID(12) ); // P2_A
        AfxBeginThread( movie_3d, LPVOID(13) ); // P2_B
        AfxBeginThread( movie_3d, LPVOID(14) ); // P2_C 
        AfxBeginThread( movie_3d, LPVOID(15) ); // P2_D
      }

    }

    #endif // C60



    #ifdef TIVS_C40  // freeze/dried vials machine

    if ( IS_PC1 )
    {
      // cap
      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,            LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,            LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,            LPVOID(2) ); // seal TC-C
      #endif

      AfxBeginThread( movie_3d, LPVOID(5) ); // TC_D color

      if ( freezemode_on ) // freeze mode
      {
        AfxBeginThread( movie_3d,        LPVOID( 3) ); // bottom HA
        AfxBeginThread( movie_3d,        LPVOID( 4) ); // bottom LA
        AfxBeginThread( movie_3d_linear, LPVOID( 9) ); // sidecake CMA
        AfxBeginThread( movie_3d_linear, LPVOID(10) ); // sidecake CMB
        AfxBeginThread( movie_3d_linear, LPVOID(11) ); // sidecake CMC
      }
      else                 // liquid mode
      {
        AfxBeginThread( movie_3d, LPVOID( 6) ); // P1A
        AfxBeginThread( movie_3d, LPVOID( 7) ); // P1B
        AfxBeginThread( movie_3d, LPVOID( 8) ); // P1C 
      }
    }


    if ( IS_PC2 )
    {
      if ( freezemode_on )
      {
        AfxBeginThread( movie_3d_linear, LPVOID(3) ); // HL_A
        AfxBeginThread( movie_3d_linear, LPVOID(4) ); // HL_B
        AfxBeginThread( movie_3d_linear, LPVOID(5) ); // HL_C
      }
      else // liquid mode
      {
        AfxBeginThread( movie_3d, LPVOID(0) ); // P2_A
        AfxBeginThread( movie_3d, LPVOID(1) ); // P2_B
        AfxBeginThread( movie_3d, LPVOID(2) ); // P2_C 
      }

      AfxBeginThread( movie_3d_linear, LPVOID(6) ); // CM_A
      AfxBeginThread( movie_3d_linear, LPVOID(7) ); // CM_B
      AfxBeginThread( movie_3d_linear, LPVOID(8) ); // CM_C
    }

    #endif // C40L

    #ifdef TIVS_A20L

      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,        LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,        LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,        LPVOID(2) ); // seal TC-C
      #endif

      AfxBeginThread( movie_3d , LPVOID(3) );   // P1_A
      AfxBeginThread( movie_3d , LPVOID(4) );   // P1_B
      AfxBeginThread( movie_3d , LPVOID(5) );   // P2_A
      AfxBeginThread( movie_3d , LPVOID(6) );   // P2_B
      AfxBeginThread( movie_3d , LPVOID(7) );   // P3_A
      AfxBeginThread( movie_3d , LPVOID(8) );   // P3_B

    #endif // TIVS_A20L

//Pour added 20180523

	  #ifdef TIVS_A20

      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,        LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,        LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,        LPVOID(2) ); // seal TC-C
      #endif

      AfxBeginThread( movie_3d , LPVOID(3) );   // P1_A
      AfxBeginThread( movie_3d , LPVOID(4) );   // P1_B
      AfxBeginThread( movie_3d , LPVOID(5) );   // P2_A
      AfxBeginThread( movie_3d , LPVOID(6) );   // P2_B

	  //Pour marked to be confirmed 20180523

	  #if STN3==COSMETIC_BACK
      AfxBeginThread( movie_3d_linear , LPVOID( 7) ); // CM_A
      AfxBeginThread( movie_3d_linear , LPVOID( 8) ); // CM_B
	  #else
	  AfxBeginThread( movie_3d , LPVOID(7) );   // P3_A
      AfxBeginThread( movie_3d , LPVOID(8) );   // P3_B
	  #endif

	  //Pour marked to be confirmed 20180523

    #endif // TIVS_A20

//Pour added 20180523

    #if defined TIVS_A40 || defined TIVS_A60 || defined TIVS_A40L // liquid ampoules machine    //Pour added 20180124 TIVS-A40L

    if ( IS_PC1 )
    {

      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,        LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,        LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,        LPVOID(2) ); // seal TC-C
      #endif

      AfxBeginThread( movie_3d , LPVOID(3) );   
      AfxBeginThread( movie_3d , LPVOID(4) );
      AfxBeginThread( movie_3d , LPVOID(5) );  
      AfxBeginThread( movie_3d , LPVOID(6) ); 
      #ifdef TIVS_A60 // extra 2 cameras for A60
        AfxBeginThread( movie_3d , LPVOID(7) );  
        AfxBeginThread( movie_3d , LPVOID(8) ); 
      #endif

      //#if SERIAL_N==201300502 // Ampoule machine for HENG RUI - SW updated 20140415 includes FO color!
      #ifdef TIVS_A60
        #ifdef FO_COLOR
          AfxBeginThread( movie_3d , LPVOID(9) );   
        #endif
      #else // TIVS_A40
        #ifdef FO_COLOR
          AfxBeginThread( movie_3d , LPVOID(7) );   
        #endif
        #ifdef LIQUID_COLOR
          AfxBeginThread( movie_3d , LPVOID(7) );   
        #endif
        #ifdef DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
          AfxBeginThread( movie_3d , LPVOID(7) );   // DR_A TCD
          AfxBeginThread( movie_3d , LPVOID(8) );   // DR_B TCE
        #endif
      #endif

    }

    if ( IS_PC2 )
    {


      #ifdef TIVS_A60 // extra 2 cameras for A60

        AfxBeginThread( movie_3d , LPVOID( 0) ); // P2_A
        AfxBeginThread( movie_3d , LPVOID( 1) ); // P2_B
        AfxBeginThread( movie_3d , LPVOID( 2) ); // P2_C
        AfxBeginThread( movie_3d , LPVOID( 3) ); // P2_D 
        AfxBeginThread( movie_3d , LPVOID( 4) ); // P2_E
        AfxBeginThread( movie_3d , LPVOID( 5) ); // P2_F  

        #if STN3==COSMETIC_BACK
          AfxBeginThread( movie_3d_linear , LPVOID( 6) ); // CM_A
          AfxBeginThread( movie_3d_linear , LPVOID( 7) ); // CM_B
          AfxBeginThread( movie_3d_linear , LPVOID( 8) ); // CM_C
          AfxBeginThread( movie_3d_linear , LPVOID( 9) ); // CM_D
          AfxBeginThread( movie_3d_linear , LPVOID(10) ); // CM_E
          AfxBeginThread( movie_3d_linear , LPVOID(11) ); // CM_F
        #else
          AfxBeginThread( movie_3d , LPVOID( 6) ); // P3_A
          AfxBeginThread( movie_3d , LPVOID( 7) ); // P3_B
          AfxBeginThread( movie_3d , LPVOID( 8) ); // P3_C
          AfxBeginThread( movie_3d , LPVOID( 9) ); // P3_D 
          AfxBeginThread( movie_3d , LPVOID(10) ); // P3_E
          AfxBeginThread( movie_3d , LPVOID(11) ); // P3_F
        #endif 

      #else // A40 - L40

        #if STN2==COSMETIC_BACK
          AfxBeginThread( movie_3d_linear , LPVOID( 0) ); // CM_A
          AfxBeginThread( movie_3d_linear , LPVOID( 1) ); // CM_B
          AfxBeginThread( movie_3d_linear , LPVOID( 2) ); // CM_C
          AfxBeginThread( movie_3d_linear , LPVOID( 3) ); // CM_D
        #else
          AfxBeginThread( movie_3d , LPVOID( 0) ); // P2_A
          AfxBeginThread( movie_3d , LPVOID( 1) ); // P2_B
          AfxBeginThread( movie_3d , LPVOID( 2) ); // P2_C
          AfxBeginThread( movie_3d , LPVOID( 3) ); // P2_D 
        #endif 

        #if STN3==COSMETIC_BACK
          AfxBeginThread( movie_3d_linear , LPVOID( 4) ); // CM_A
          AfxBeginThread( movie_3d_linear , LPVOID( 5) ); // CM_B
          AfxBeginThread( movie_3d_linear , LPVOID( 6) ); // CM_C
          AfxBeginThread( movie_3d_linear , LPVOID( 7) ); // CM_D
        #else
          AfxBeginThread( movie_3d , LPVOID( 4) ); // P3_A
          AfxBeginThread( movie_3d , LPVOID( 5) ); // P3_B
          AfxBeginThread( movie_3d , LPVOID( 6) ); // P3_C
          AfxBeginThread( movie_3d , LPVOID( 7) ); // P3_D 
        #endif 

      #endif 



/*      AfxBeginThread( movie_3d , LPVOID(0) ); // P2_A
      AfxBeginThread( movie_3d , LPVOID(1) ); // P2_B
      AfxBeginThread( movie_3d , LPVOID(2) ); // P2_C
      AfxBeginThread( movie_3d , LPVOID(3) ); // P2_D */


/*      #ifdef TIVS_A60 // extra 2 cameras for A60
        AfxBeginThread( movie_3d , LPVOID( 4) ); // P2_E
        AfxBeginThread( movie_3d , LPVOID( 5) ); // P2_F

        #if STN3==COSMETIC_BACK
          AfxBeginThread( movie_3d_linear , LPVOID(4) );
          AfxBeginThread( movie_3d_linear , LPVOID(5) );
          AfxBeginThread( movie_3d_linear , LPVOID(6) ); 
          AfxBeginThread( movie_3d_linear , LPVOID(7) );  
        #else
          AfxBeginThread( movie_3d , LPVOID( 4) ); // P2_A
          AfxBeginThread( movie_3d , LPVOID( 5) );
          AfxBeginThread( movie_3d , LPVOID( 6) );  
          AfxBeginThread( movie_3d , LPVOID( 7) );
          AfxBeginThread( movie_3d , LPVOID( 8) );
          AfxBeginThread( movie_3d , LPVOID( 9) );
          AfxBeginThread( movie_3d , LPVOID(10) ); 
          AfxBeginThread( movie_3d , LPVOID(11) );  
        #endif */


/*        AfxBeginThread( movie_3d , LPVOID( 6) ); 
        AfxBeginThread( movie_3d , LPVOID( 7) );  
        AfxBeginThread( movie_3d , LPVOID( 8) );
        AfxBeginThread( movie_3d , LPVOID( 9) );
        AfxBeginThread( movie_3d , LPVOID(10) ); 
        AfxBeginThread( movie_3d , LPVOID(11) );  */

/*      #else     

        #if STN3==COSMETIC_BACK
          AfxBeginThread( movie_3d_linear , LPVOID(4) );
          AfxBeginThread( movie_3d_linear , LPVOID(5) );
          AfxBeginThread( movie_3d_linear , LPVOID(6) ); 
          AfxBeginThread( movie_3d_linear , LPVOID(7) );  
        #else
          AfxBeginThread( movie_3d , LPVOID(4) );
          AfxBeginThread( movie_3d , LPVOID(5) );
          AfxBeginThread( movie_3d , LPVOID(6) );  
          AfxBeginThread( movie_3d , LPVOID(7) );
        #endif

      #endif  */
    }

    #endif

    #ifdef TIVS_F40  // freeze/dried vials machine

    if ( IS_PC1 )
    {
      // cap - wait
      
      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,        LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,        LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,        LPVOID(2) ); // seal TC-C
      #endif
      
      AfxBeginThread( movie_3d,        LPVOID(3) ); // bottom HA
      AfxBeginThread( movie_3d,        LPVOID(4) ); // bottom LA
      AfxBeginThread( movie_3d,        LPVOID(5) ); // FO color
      AfxBeginThread( movie_3d_linear, LPVOID(6) ); // cosmetic/sidewall CMA
      AfxBeginThread( movie_3d_linear, LPVOID(7) ); // cosmetic/sidewall CMB
      AfxBeginThread( movie_3d_linear, LPVOID(8) ); // cosmetic/sidewall CMC
      AfxBeginThread( movie_3d_linear, LPVOID(9) ); // cosmetic/sidewall CMD
    }

    if ( IS_PC2 )
    {
      AfxBeginThread( movie_3d_linear, LPVOID(0) ); // sidecake A
      AfxBeginThread( movie_3d_linear, LPVOID(1) ); // sidecake B
      AfxBeginThread( movie_3d_linear, LPVOID(2) ); // sidecake C
      AfxBeginThread( movie_3d_linear, LPVOID(3) ); // sidecake D 

      #if STN1==CAKESIDE_ARBACK // Test AUSIA, add 4 area scan cameras side by side with existing line scan cam for sidewall
        AfxBeginThread( movie_3d , LPVOID( 4) ); // T3_A Side cacke backlight AREA SCAN
        AfxBeginThread( movie_3d , LPVOID( 5) ); // T3_B Side cacke backlight AREA SCAN
        AfxBeginThread( movie_3d , LPVOID( 6) ); // T3_C Side cacke backlight AREA SCAN
        AfxBeginThread( movie_3d , LPVOID( 7) ); // T3_D Side cacke backlight AREA SCAN
      #endif 

      #if STN3==CAKETOP_ARDIRECT // Test AUSIA, add 4 area scan cameras for top cake new inspection
        AfxBeginThread( movie_3d , LPVOID( 8) ); // TC_A Top cake AREA SCAN
        AfxBeginThread( movie_3d , LPVOID( 9) ); // TC_B Top cake AREA SCAN
        AfxBeginThread( movie_3d , LPVOID(10) ); // TC_C Top cake AREA SCAN
        AfxBeginThread( movie_3d , LPVOID(11) ); // TC_D Top cake AREA SCAN
      #endif 

      #ifndef NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
        AfxBeginThread( movie_3d_linear, LPVOID(4) ); // topcake  A
        AfxBeginThread( movie_3d_linear, LPVOID(5) ); // topcake  B
        AfxBeginThread( movie_3d_linear, LPVOID(6) ); // topcake  C
        AfxBeginThread( movie_3d_linear, LPVOID(7) ); // topcake  D
      #endif 
    }

    txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

    #endif

	#ifdef TIVS_F20  // freeze/dried vials machine

    if ( IS_PC1 )
    {
      // cap - wait
      
      #ifdef NEWCAP_DOUBLE
        AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
        AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
        AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      #else  // Normal/old CAP movie threads
        AfxBeginThread( movie_3d,        LPVOID(0) ); // seal TC-A
        AfxBeginThread( movie_3d,        LPVOID(1) ); // seal TC-B
        AfxBeginThread( movie_3d,        LPVOID(2) ); // seal TC-C
      #endif
      
      AfxBeginThread( movie_3d,        LPVOID(3) ); // bottom HA
      AfxBeginThread( movie_3d,        LPVOID(4) ); // bottom LA
      AfxBeginThread( movie_3d,        LPVOID(5) ); // FO color
      AfxBeginThread( movie_3d_linear, LPVOID(6) ); // cosmetic/sidewall CMA
      AfxBeginThread( movie_3d_linear, LPVOID(7) ); // cosmetic/sidewall CMB
      AfxBeginThread( movie_3d_linear, LPVOID(8) ); // cosmetic/sidecake A
      AfxBeginThread( movie_3d_linear, LPVOID(9) ); // cosmetic/sidecake B
	  AfxBeginThread( movie_3d_linear, LPVOID(10) ); // topcake  A
      AfxBeginThread( movie_3d_linear, LPVOID(11) ); // topcake  B

    }

    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_061[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_060[lng], file_manager.recipe_name );
    #endif


    //txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

    #endif



    #ifdef TIVS_C20  // combo machine

    if ( IS_PC1 )
    {
      // cap - wait
      AfxBeginThread( movie_3d_doublecap , LPVOID(0) ); // TC-A
      AfxBeginThread( movie_3d_doublecap , LPVOID(1) ); // TC-B
      AfxBeginThread( movie_3d_doublecap , LPVOID(2) ); // TC-C
      AfxBeginThread( movie_3d,            LPVOID(5) ); // FO color

      if ( liquid_mode ) // liquid mode on
      {
        AfxBeginThread( movie_3d , LPVOID(6) ); // P1_A   
        AfxBeginThread( movie_3d , LPVOID(7) ); // P1_B
      }
      else               // freeze-dried mode on
      {
        AfxBeginThread( movie_3d,        LPVOID(3) ); // bottom HA
        AfxBeginThread( movie_3d,        LPVOID(4) ); // bottom LA
        AfxBeginThread( movie_3d_linear, LPVOID(8) ); // cosmetic/sidewall CMA //Pour removed sidewall from PC1 20190319
        AfxBeginThread( movie_3d_linear, LPVOID(9) ); // cosmetic/sidewall CMB //Pour removed sidewall from PC1 20190319
      }
    }

    if ( IS_PC2 )
    {
      if ( liquid_mode ) // liquid mode on
      {
		
		AfxBeginThread( movie_3d_linear, LPVOID(0) ); //sidewall A on PC2 //Pour added cosmetic inspection for liquid on 20190411
        AfxBeginThread( movie_3d_linear, LPVOID(1) ); //sidewall B on PC2 //Pour added cosmetic inspection for liquid on 20190411

        AfxBeginThread( movie_3d , LPVOID(6) ); // P2_A from 4 to 6
        AfxBeginThread( movie_3d , LPVOID(7) ); // P2_B from 5 to 7
      }
      else               // freeze-dried mode on
      {
        AfxBeginThread( movie_3d_linear, LPVOID(0) ); // sidecake A //sidewall A
        AfxBeginThread( movie_3d_linear, LPVOID(1) ); // sidecake B //sidewall B
        AfxBeginThread( movie_3d_linear, LPVOID(2) ); // topcake  A
        AfxBeginThread( movie_3d_linear, LPVOID(3) ); // topcake  B
		AfxBeginThread( movie_3d_linear, LPVOID(4) ); // cosmetic/sidewall CMA //Pour removed sidewall from PC1 20190319
        AfxBeginThread( movie_3d_linear, LPVOID(5) ); // cosmetic/sidewall CMB //Pour removed sidewall from PC1 20190319
      }

    }

	    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_061[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_060[lng], file_manager.recipe_name );
    #endif

    //txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

    #endif // TIVS_C20 combo machine


	    #ifdef TIVS_S40  // PFS machine

      // flange and stopper top/sides
      AfxBeginThread( movie_3d,            LPVOID(0) ); //  T_A
      AfxBeginThread( movie_3d,            LPVOID(1) ); // C3_A
      AfxBeginThread( movie_3d,            LPVOID(2) ); // C3_B

      // particle 1
      AfxBeginThread( movie_3d,            LPVOID(3) ); //  P1_A
      AfxBeginThread( movie_3d,            LPVOID(4) ); //  P1_B
      AfxBeginThread( movie_3d,            LPVOID(5) ); //  P1_C
     
      // particle 2
      AfxBeginThread( movie_3d,            LPVOID(6) ); //  P2_A
      AfxBeginThread( movie_3d,            LPVOID(7) ); //  P2_B
      AfxBeginThread( movie_3d,            LPVOID(8) ); //  P2_C
      
      // cosmetic
      AfxBeginThread( movie_3d_linear,     LPVOID( 9) ); // cosmetic C1_A
      AfxBeginThread( movie_3d_linear,     LPVOID(10) ); // cosmetic C1_B
      AfxBeginThread( movie_3d_linear,     LPVOID(11) ); // cosmetic C1_C

      // needle/cap
      //AfxBeginThread( movie_3d_linear,     LPVOID(12) ); // cosmetic C2_A
      //AfxBeginThread( movie_3d_linear,     LPVOID(13) ); // cosmetic C2_B
      //AfxBeginThread( movie_3d_linear,     LPVOID(14) ); // cosmetic C2_C
      AfxBeginThread( movie_3d,            LPVOID(12) ); //  C2_A
      AfxBeginThread( movie_3d,            LPVOID(13) ); //  C2_B
      AfxBeginThread( movie_3d,            LPVOID(14) ); //  C2_C


    /*
    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_061[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_060[lng], file_manager.recipe_name );
    #endif
//    txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );
    */
    #endif // TIVS_S40 PFS machine


	#ifdef TIVS_S60  // PFS machine

	      if ( IS_PC1 )
    {

      // flange and stopper top/sides
      AfxBeginThread( movie_3d,            LPVOID(0) ); //  T_A
      AfxBeginThread( movie_3d,            LPVOID(1) ); // C3_A
      AfxBeginThread( movie_3d,            LPVOID(2) ); // C3_B

      // particle 1
      AfxBeginThread( movie_3d,            LPVOID(3) ); //  P1_A
      AfxBeginThread( movie_3d,            LPVOID(4) ); //  P1_B
      AfxBeginThread( movie_3d,            LPVOID(5) ); //  P1_C
	  AfxBeginThread( movie_3d,            LPVOID(6) ); //  P1_D
     
      // particle 2
      AfxBeginThread( movie_3d,            LPVOID(7) ); //  P2_A
      AfxBeginThread( movie_3d,            LPVOID(8) ); //  P2_B
      AfxBeginThread( movie_3d,            LPVOID(9) ); //  P2_C
	  AfxBeginThread( movie_3d,            LPVOID(10) ); //  P2_D
		  }

	if ( IS_PC2 )
    {
      // cosmetic
      AfxBeginThread( movie_3d_linear,     LPVOID(0) ); // cosmetic C1_A
      AfxBeginThread( movie_3d_linear,     LPVOID(1) ); // cosmetic C1_B
      AfxBeginThread( movie_3d_linear,     LPVOID(2) ); // cosmetic C1_C
	  AfxBeginThread( movie_3d_linear,     LPVOID(3) ); // cosmetic C1_D


      AfxBeginThread( movie_3d,            LPVOID(4) ); //  C2_A
      AfxBeginThread( movie_3d,            LPVOID(5) ); //  C2_B
      AfxBeginThread( movie_3d,            LPVOID(6) ); //  C2_C
	  AfxBeginThread( movie_3d,            LPVOID(7) ); //  C2_D
	}

    /*
    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_061[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_060[lng], file_manager.recipe_name );
    #endif
//    txt.Format( string_060[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );
    */
    #endif // TIVS_S60 PFS machine



    #ifdef STOPPER_CTRL  // stopper control

      AfxBeginThread( movie_3d , LPVOID(0) );
      AfxBeginThread( movie_3d , LPVOID(1) );
      AfxBeginThread( movie_3d , LPVOID(2) );

    #endif

    if ( !wcsncmp( PC_name, L"PC-FILLLEV", 10 ) ) // FILLING LEVEL
    {
      AfxBeginThread( movie_3d , LPVOID(0) );
    }


 //   for ( i=0 ; i<NUM_TLC ; i++ ) if ( camera[i].camera ) AfxBeginThread( movie_3d , LPVOID(i) );

    inspection_btn->ShowWindow( SW_HIDE );

    if ( inspmode_btn ) inspmode_btn->ShowWindow( SW_HIDE );

    movie_btn->SetWindowText( string_162[lng] );
    //movie_btn->EnableWindow( TRUE );
    movie_active = TRUE;

    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      msg.Format( string_069[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      msg.Format( string_068[lng], file_manager.recipe_name );
    #endif
//    txt.Format( string_068[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

  }

	return 0;
}





int Inspection_startstop( int inp )
{
  static BOOL inspection_active = FALSE;


  UINT i;

	pView->Close_all_menus();
	TRACE( "Inspection_startstop\n" );

  // force start
  if ( inp==10 && inspection_active ) return 0; // already active, just return

  // force stop
  if ( inp==20 && !inspection_active ) return 0; // already stopped, just return

  if ( inspection_active )  // Running, try to stop all
  {
	f_run_automatic = FALSE;//Pour 20200702

    if ( logger ) logger->Add_string( L"Stop inspection" );

  	//TRACE( L"Inspection_startstop: inspection_active TRUE, issuing stop command\n" );
    //for ( i=0 ; i<num_tlc ; i++ ) if ( CHK_BIT(thread_status,10+i) ) SET_BIT( thread_exit, 10+i );

    inspection_btn->SetWindowText( string_154[lng] );
    movie_btn->SetWindowText     ( string_154[lng] );

    UINT mask = 0;
    for ( i=0 ; i<num_tlc ; i++ ) if ( CHK_BIT(thread_status,10+i) ) SET_BIT( mask, 10+i );
    thread_exit = mask;

    CTimer timer;
    while ( (thread_status&mask) && timer.elapsed_sec()<10 )
    {
      MSG msg; 
      PeekMessage( &msg, NULL,  0, 0, PM_REMOVE);
      Sleep(5);
    }
    if ( thread_status&mask )
    {
      swprintf( msg_err, 200, L"Inspection_startstop: timeout while waiting thread_status (%d)", thread_status&mask );
      Message_box( msg_err );
    }

    inspection_btn->SetWindowText( string_150[lng] );
    //inspection_btn->EnableWindow( TRUE );
    inspection_btn->ShowWindow( SW_SHOW );

    movie_btn->SetWindowText( string_160[lng] );
    //movie_btn->EnableWindow( TRUE );
    movie_btn->ShowWindow( SW_SHOW );

    //recipe_btn->SetWindowText( string_160[lng] );
    //movie_btn->EnableWindow( TRUE );
    recipe_btn->ShowWindow( SW_SHOW );

    if ( inspmode_btn ) inspmode_btn->ShowWindow( SW_SHOW );

    inspection_active = FALSE;

    CString txt;
    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      txt.Format( string_063[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      txt.Format( string_062[lng], file_manager.recipe_name );
    #endif
    //txt.Format( string_062[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

  }
  else                    // Stopped, try to start all
  { 
    TRACE( L"Inspection_startstop: inspection_active FALSE, start movie thread/s\n" );

	//f_run_automatic = TRUE;//Pour 20200702

    if ( logger ) logger->Add_string( L"Start inspection" );

    if ( pseudo_colors && !learning )
    {
      pseudo_colors = FALSE;
      lut32         = lut32_bw;
      TRACE("Pesudo-colors disabled for inspection: Gray palette selected\n");
    }


    #ifdef TIVS_C60  // combo freeze/dried vials machine
      if ( IS_PC1 )
      {
        #ifdef NEWCAP_DOUBLE
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #else  // Normal/old CAP processing
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif
     
        AfxBeginThread( inspect_3d_singlecam , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
       
        if ( freezemode_on ) // freeze mode
        {
          AfxBeginThread( inspect_3d_singlecam, LPVOID( 3), THREAD_PRIORITY_HIGHEST ); // TB-A first  bottom - high angle
          AfxBeginThread( inspect_3d_singlecam, LPVOID( 4), THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low  angle
        }

        AfxBeginThread( inspect_3d_cosmetic , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // CM-A cosmetic control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // CM-B cosmetic control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // CM-C cosmetic control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // CM-D cosmetic control

      }

      if ( IS_PC2 )
      {
        if ( freezemode_on )
        {
          // Sidecake
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID( 0), THREAD_PRIORITY_HIGHEST ); // SC-A sidecake control
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID( 1), THREAD_PRIORITY_HIGHEST ); // SC-B sidecake control
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID( 2), THREAD_PRIORITY_HIGHEST ); // SC-C sidecake control
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID( 3), THREAD_PRIORITY_HIGHEST ); // SC-D sidecake control

          // Heel
          AfxBeginThread( inspect_3d_cosmetic , LPVOID( 8) , THREAD_PRIORITY_HIGHEST ); // HL-A heel control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID( 9) , THREAD_PRIORITY_HIGHEST ); // HL-B heel control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(10) , THREAD_PRIORITY_HIGHEST ); // HL-C heel control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(11) , THREAD_PRIORITY_HIGHEST ); // HL-D heel control
        }
        else // Liquid mode
        {
          // Particle1
          AfxBeginThread( inspect_3d_particle , LPVOID( 4) , THREAD_PRIORITY_HIGHEST ); // P1-A
          AfxBeginThread( inspect_3d_particle , LPVOID( 5) , THREAD_PRIORITY_HIGHEST ); // P1-B
          AfxBeginThread( inspect_3d_particle , LPVOID( 6) , THREAD_PRIORITY_HIGHEST ); // P1-C
          AfxBeginThread( inspect_3d_particle , LPVOID( 7) , THREAD_PRIORITY_HIGHEST ); // P1-D

          // Particle2
          AfxBeginThread( inspect_3d_particle , LPVOID(12) , THREAD_PRIORITY_HIGHEST ); // P2-A
          AfxBeginThread( inspect_3d_particle , LPVOID(13) , THREAD_PRIORITY_HIGHEST ); // P2-B
          AfxBeginThread( inspect_3d_particle , LPVOID(14) , THREAD_PRIORITY_HIGHEST ); // P2-C
          AfxBeginThread( inspect_3d_particle , LPVOID(15) , THREAD_PRIORITY_HIGHEST ); // P2-D
        }

      } 
    #endif // C60




    #ifdef TIVS_C40  // freeze/dried vials machine
      if ( IS_PC1 )
      {

        #ifdef NEWCAP_DOUBLE
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #else  // Normal/old CAP processing
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif
     
        AfxBeginThread( inspect_3d_singlecam , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
       
        if ( freezemode_on ) // freeze mode
        {
          AfxBeginThread( inspect_3d_singlecam, LPVOID( 3), THREAD_PRIORITY_HIGHEST ); // TB-A first  bottom - high angle
          AfxBeginThread( inspect_3d_singlecam, LPVOID( 4), THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low  angle
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID( 9), THREAD_PRIORITY_HIGHEST ); // SC-A sidecake control
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID(10), THREAD_PRIORITY_HIGHEST ); // SC-B sidecake control
          AfxBeginThread( inspect_3d_cosmetic,  LPVOID(11), THREAD_PRIORITY_HIGHEST ); // SC-C sidecake control
        }
        else                 // liquid mode
        {
		
		  AfxBeginThread( inspect_3d_singlecam, LPVOID( 3), THREAD_PRIORITY_HIGHEST ); // TB-A first  bottom - high angle //added on 20201028 for Tebao
          AfxBeginThread( inspect_3d_singlecam, LPVOID( 4), THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low  angle //added on 20201028 for Tebao
		   
          AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P1-A
          AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P1-B
          AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P1.C
        }
      }

      if ( IS_PC2 )
      {
        if ( freezemode_on )
        {
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // HL-A heel control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // HL-B heel control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // HL-C heel control
        }
        else // Liquid mode
        {
          AfxBeginThread( inspect_3d_particle , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // P2-A
          AfxBeginThread( inspect_3d_particle , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // P2-B
          AfxBeginThread( inspect_3d_particle , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // P2-C
        }

        AfxBeginThread( inspect_3d_cosmetic , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // CM-A cosmetic control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // CM-B cosmetic control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // CM-C cosmetic control
      }

    #endif // C40L


    #ifdef TIVS_A20L
      #ifdef NEWCAP_DOUBLE
        #ifdef COMBINED_TIPCAP
          if ( tipcap_tip ) // tip mode enabled
          {

            AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
            AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
            AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
          }
          else // cap mode enabled
          {
            AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
            AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
            AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
          }
        #else  // COMBINED_TIPCAP
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif // COMBINED_TIPCAP
      #else  // NEWCAP_DOUBLE - Normal/old CAP processing
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
      #endif // NEWCAP_DOUBLE

      AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P1-A
      AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P1-B
      AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P2-A
      AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P2-B
      AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P3-A
      AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P3-B

    #endif // TIVS_A20L


//Pour added 20180523

	    #ifdef TIVS_A20
      #ifdef NEWCAP_DOUBLE
        #ifdef COMBINED_TIPCAP
          if ( tipcap_tip ) // tip mode enabled
          {
            AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
            AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
            AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
          }
          else // cap mode enabled
          {
            AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
            AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
            AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
          }
        #else  // COMBINED_TIPCAP
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif // COMBINED_TIPCAP
      #else  // NEWCAP_DOUBLE - Normal/old CAP processing
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
        AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
      #endif // NEWCAP_DOUBLE

      AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P1-A
      AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P1-B
      AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P2-A
      AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P2-B

	  //Pour marked to be confirmed 20180523
	  #if STN3==COSMETIC_BACK
      AfxBeginThread( inspect_3d_cosmetic , LPVOID( 7) , THREAD_PRIORITY_HIGHEST ); // CM-A
      AfxBeginThread( inspect_3d_cosmetic , LPVOID( 8) , THREAD_PRIORITY_HIGHEST ); // CM-B
	  #else
	  AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P3-A
      AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P3-B
      #endif 
	  //Pour marked to be confirmed 20180523

    #endif // TIVS_A20

//Pour added 20180523



    #if defined TIVS_A40 || defined TIVS_A60 || defined TIVS_A40L //Pour added TIVS-A40L 20180124
      if ( IS_PC1 )
      {

        #ifdef NEWCAP_DOUBLE

          #ifdef COMBINED_TIPCAP

            if ( tipcap_tip ) // tip mode enabled
            {
              AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
              AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
              AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
            }
            else // cap mode enabled
            {
              AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
              AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
              AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
            }

          #else  // COMBINED_TIPCAP
            AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
            AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
            AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
          #endif // COMBINED_TIPCAP

        #else  // NEWCAP_DOUBLE - Normal/old CAP processing
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif // NEWCAP_DOUBLE

        if ( nopart1==FALSE )
        {
          AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P1-A
          AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P1-B
          AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P1.C
          AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P1-D 
          #ifdef TIVS_A60
            AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P1.E
            AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P1-F 
          #endif
        }

//        #if SERIAL_N==201300502 // Ampoule machine for HENG RUI - SW updated 20140415 includes FO color!
        #ifdef FO_COLOR    // If defined camera is searched and added for flip off color control
          #ifdef TIVS_A60
            AfxBeginThread( inspect_3d_singlecam , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
          #else // TIVS_A40
            AfxBeginThread( inspect_3d_singlecam , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
          #endif
        #endif

        #ifdef LIQUID_COLOR    // If defined camera is searched and added for flip off color control
          #ifdef TIVS_A60
//            AfxBeginThread( inspect_3d_singlecam , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
          #else // TIVS_A40
            AfxBeginThread( inspect_3d_singlecam , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // TD-E Liquid color camera
          #endif
        #endif

        #ifdef DOTRING_COLOR    // If defined camera is searched and added for flip off color control
          AfxBeginThread( inspect_3d_singlecam , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // DR_A TCD
          AfxBeginThread( inspect_3d_singlecam , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // DR_B TCD
        #endif
      }

      if ( IS_PC2 )
      {

        #ifdef TIVS_A60

//          AfxBeginThread( inspect_3d_particle , LPVOID( 5) , THREAD_PRIORITY_HIGHEST ); // P2-F

          if ( nopart2==FALSE )
          {
            AfxBeginThread( inspect_3d_particle , LPVOID( 0) , THREAD_PRIORITY_HIGHEST ); // P2-A
            AfxBeginThread( inspect_3d_particle , LPVOID( 1) , THREAD_PRIORITY_HIGHEST ); // P2-B
            AfxBeginThread( inspect_3d_particle , LPVOID( 2) , THREAD_PRIORITY_HIGHEST ); // P2-C
            AfxBeginThread( inspect_3d_particle , LPVOID( 3) , THREAD_PRIORITY_HIGHEST ); // P2-D
            AfxBeginThread( inspect_3d_particle , LPVOID( 4) , THREAD_PRIORITY_HIGHEST ); // P2-E
            AfxBeginThread( inspect_3d_particle , LPVOID( 5) , THREAD_PRIORITY_HIGHEST ); // P2-F
          }

          if ( nopart3==FALSE )
          {
            #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
              AfxBeginThread( inspect_3d_particle , LPVOID( 6) , THREAD_PRIORITY_HIGHEST ); // P3-A
              AfxBeginThread( inspect_3d_particle , LPVOID( 7) , THREAD_PRIORITY_HIGHEST ); // P3-B
              AfxBeginThread( inspect_3d_particle , LPVOID( 8) , THREAD_PRIORITY_HIGHEST ); // P3-C
              AfxBeginThread( inspect_3d_particle , LPVOID( 9) , THREAD_PRIORITY_HIGHEST ); // P3-D
              AfxBeginThread( inspect_3d_particle , LPVOID(10) , THREAD_PRIORITY_HIGHEST ); // P3-E
              AfxBeginThread( inspect_3d_particle , LPVOID(11) , THREAD_PRIORITY_HIGHEST ); // P3-F  
            #endif

            #if STN3==COSMETIC_BACK
              AfxBeginThread( inspect_3d_cosmetic , LPVOID( 6) , THREAD_PRIORITY_HIGHEST ); // CM-A
              AfxBeginThread( inspect_3d_cosmetic , LPVOID( 7) , THREAD_PRIORITY_HIGHEST ); // CM-B
              AfxBeginThread( inspect_3d_cosmetic , LPVOID( 8) , THREAD_PRIORITY_HIGHEST ); // CM-C  
              AfxBeginThread( inspect_3d_cosmetic , LPVOID( 9) , THREAD_PRIORITY_HIGHEST ); // CM-D 
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(10) , THREAD_PRIORITY_HIGHEST ); // CM-E
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(11) , THREAD_PRIORITY_HIGHEST ); // CM-F 
            #endif  
          }

        #else  //  A40 / L40
 
          if ( nopart2==FALSE )
          {

            #if STN2==COSMETIC_BACK
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // CM-A
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // CM-B
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // CM-C  
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // CM-D 
            #endif

            #if STN2==PARTICLE_BOTTOM || STN2==PARTICLE_BACK
              AfxBeginThread( inspect_3d_particle , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // P2-A
              AfxBeginThread( inspect_3d_particle , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // P2-B
              AfxBeginThread( inspect_3d_particle , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // P2-C
              AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P2-D
            #endif
          }

          if ( nopart3==FALSE )
          {
            #if STN3==COSMETIC_BACK
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // CM-A
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // CM-B
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // CM-C  
              AfxBeginThread( inspect_3d_cosmetic , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // CM-D 
            #endif

            #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
              AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P3-A
              AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P3-B
              AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P3-C
              AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P3-D
            #endif
          }

        #endif
      }
    #endif

    #ifdef TIVS_F40
      if ( IS_PC1 )
      {
        /*
        AfxBeginThread( inspect_3d_tipcap   , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
        image_dsp[0]->enable_full_dsp(0);
        */
        
        #ifdef NEWCAP_DOUBLE
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #else  // Normal/old CAP processing
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif
     
        AfxBeginThread( inspect_3d_singlecam , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // TB-A first bottom - high angle
        AfxBeginThread( inspect_3d_singlecam , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low angle
        AfxBeginThread( inspect_3d_singlecam , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
       
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // CM-A sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // CM-B sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // CM-C sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // CM-D sidewall control
        
      }

      if ( IS_PC2 )
      {
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // SC-A side cake control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // SC-B side cake control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // SC-C side cake control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // SC-D side cake control


        #if STN1==CAKESIDE_ARBACK // Test AUSIA, add 4 area scan cameras side by side with existing line scan cam for sidewall WAIST-LAYERED
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 4) ); // T3_A Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 5) ); // T3_B Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 6) ); // T3_C Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 7) ); // T3_D Side cacke backlight AREA SCAN
        #endif 

        #if STN3==CAKETOP_ARDIRECT // Test AUSIA, add 4 area scan cameras for top cake new inspection for SKINNING
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 8) ); // T3_A Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID( 9) ); // T3_B Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID(10) ); // T3_C Side cacke backlight AREA SCAN
          AfxBeginThread( inspect_3d_cakesidearback , LPVOID(11) ); // T3_D Side cacke backlight AREA SCAN
        #endif 


        #ifndef NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // TC-A top cake control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TC-B top cake control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // TC-C top cake control
          AfxBeginThread( inspect_3d_cosmetic , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // TC-D top cake control
        #endif
      }
    #endif

	  #ifdef TIVS_F20
      if ( IS_PC1 )
      {
        /*
        AfxBeginThread( inspect_3d_tipcap   , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
        image_dsp[0]->enable_full_dsp(0);
        */
        
        #ifdef NEWCAP_DOUBLE
          AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #else  // Normal/old CAP processing
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
          AfxBeginThread( inspect_3d_tipcap    , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
        #endif
     
        AfxBeginThread( inspect_3d_singlecam , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // TB-A first bottom - high angle
        AfxBeginThread( inspect_3d_singlecam , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low angle
        AfxBeginThread( inspect_3d_singlecam , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera
       
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // CM-A sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // CM-B sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // CM-C sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // CM-D sidewall control
		AfxBeginThread( inspect_3d_cosmetic  , LPVOID(10) , THREAD_PRIORITY_HIGHEST ); // CM-C sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(11) , THREAD_PRIORITY_HIGHEST ); // CM-D sidewall control
        
      }

    #endif



    #ifdef TIVS_C20  // combo machine

    if ( IS_PC1 )
    {
       //cap - wait

//Pour removed for neck inspection
      AfxBeginThread( inspect_3d_doublecap , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
      AfxBeginThread( inspect_3d_doublecap , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
      AfxBeginThread( inspect_3d_doublecap , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C

////Pour added for testing on 20190429
//	  AfxBeginThread( inspect_3d_neck , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TC-A
//      AfxBeginThread( inspect_3d_neck , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // TC-B
//      AfxBeginThread( inspect_3d_neck , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-C
////Pour added for testing on 20190429
      AfxBeginThread( inspect_3d_singlecam , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // TD-D FO color camera

      if ( liquid_mode ) // liquid mode on
      {
        AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P1-A
        AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P1-B
      }
      else               // freeze-dried mode on
      {
        AfxBeginThread( inspect_3d_singlecam , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // TB-A first bottom - high angle
        AfxBeginThread( inspect_3d_singlecam , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // TB-B second bottom - low angle
        //AfxBeginThread( inspect_3d_cosmetic  , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // CM-A sidewall control
        //AfxBeginThread( inspect_3d_cosmetic  , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // CM-B sidewall control
      }
    }

    if ( IS_PC2 )
    {
		//Pour added 20190411
		AfxBeginThread( inspect_3d_cosmetic  , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // CM-A sidewall control  //Pour added cosmetic inspection for liquid
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // CM-B sidewall control  //Pour added cosmetic inspection for liuqid

      if ( liquid_mode ) // liquid mode on
      {
		AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P2-A from 4-6 for second machine 20190319
        AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P2-B from 5-7 for second machine 20190319
      }
      else  // freeze-dried mode on
      {
        //AfxBeginThread( inspect_3d_cosmetic , LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // SC-A side cake control
        //AfxBeginThread( inspect_3d_cosmetic , LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // SC-B side cake control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // TC-A top  cake control
        AfxBeginThread( inspect_3d_cosmetic , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // TC-B top  cake control
		//Pour added 20190319
		AfxBeginThread( inspect_3d_cosmetic  , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // CM-A sidewall control
        AfxBeginThread( inspect_3d_cosmetic  , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // CM-B sidewall control
      }

    }

//    txt.Format( string_060[lng], file_manager.recipe_name );
//    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

    #endif // TIVS_C20 combo machine


	  #ifdef TIVS_S40  // PFS machine

      AfxBeginThread( inspect_3d_singlecam, LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TCA - Flange area

      AfxBeginThread( inspect_3d_singlecam, LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // C3A - cosmetic bottom area backlight
      AfxBeginThread( inspect_3d_singlecam, LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // C3B - cosmetic bottom area backlight


    /*
      AfxBeginThread( inspect_3d_particle,  LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // C3A - cosmetic bottom area backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // C3B - cosmetic bottom area backlight
      */

    
      // Particle 1
      AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P1-A - backlight - adsorbtion
      AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P1-B - backlight - adsorbtion
      AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P1-C - backlight - adsorbtion

      // Particle 2
      AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P2-A - backlight - reflection+polarizers
      AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P2-B - backlight - reflection+polarizers
      AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P2-C - backlight - reflection+polarizers
    
      // Cosmetic 1 - line scan - backlight - sidewall - black spots on needle shield
      AfxBeginThread( inspect_3d_cosmetic , LPVOID( 9), THREAD_PRIORITY_HIGHEST ); // C1A cosmetic1 control - linescan backlight + barlight
      AfxBeginThread( inspect_3d_cosmetic , LPVOID(10), THREAD_PRIORITY_HIGHEST ); // C1B cosmetic1 control - linescan backlight + barlight
      AfxBeginThread( inspect_3d_cosmetic , LPVOID(11), THREAD_PRIORITY_HIGHEST ); // C1C cosmetic1 control - linescan backlight + barlight
          
      // Cosmetic 2 - area scan - backlight - needle shield profile
      AfxBeginThread( inspect_3d_particle,  LPVOID(12), THREAD_PRIORITY_HIGHEST ); // C2A cosmetic2 control - areascan backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(13), THREAD_PRIORITY_HIGHEST ); // C2B cosmetic2 control - areascan backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(14), THREAD_PRIORITY_HIGHEST ); // C2C cosmetic2 control - areascan backlight
      

    #endif // TIVS_S40 PFS machine



	  #ifdef TIVS_S60  // PFS machine

	        if ( IS_PC1 )
      {

      //AfxBeginThread( inspect_3d_singlecam, LPVOID(0) , THREAD_PRIORITY_HIGHEST ); // TCA - Flange area

	  //Cosmetic 3
      AfxBeginThread( inspect_3d_singlecam, LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // C3A - cosmetic bottom area backlight
      AfxBeginThread( inspect_3d_singlecam, LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // C3B - cosmetic bottom area backlight


    /*
      AfxBeginThread( inspect_3d_particle,  LPVOID(1) , THREAD_PRIORITY_HIGHEST ); // C3A - cosmetic bottom area backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(2) , THREAD_PRIORITY_HIGHEST ); // C3B - cosmetic bottom area backlight
      */

    
   //   // Particle 1
      AfxBeginThread( inspect_3d_particle , LPVOID(3) , THREAD_PRIORITY_HIGHEST ); // P1-A - backlight - adsorbtion
      AfxBeginThread( inspect_3d_particle , LPVOID(4) , THREAD_PRIORITY_HIGHEST ); // P1-B - backlight - adsorbtion
      AfxBeginThread( inspect_3d_particle , LPVOID(5) , THREAD_PRIORITY_HIGHEST ); // P1-C - backlight - adsorbtion
	  AfxBeginThread( inspect_3d_particle , LPVOID(6) , THREAD_PRIORITY_HIGHEST ); // P1-C - backlight - adsorbtion

   //   // Particle 2
      AfxBeginThread( inspect_3d_particle , LPVOID(7) , THREAD_PRIORITY_HIGHEST ); // P2-A - backlight - reflection+polarizers
      AfxBeginThread( inspect_3d_particle , LPVOID(8) , THREAD_PRIORITY_HIGHEST ); // P2-B - backlight - reflection+polarizers
      AfxBeginThread( inspect_3d_particle , LPVOID(9) , THREAD_PRIORITY_HIGHEST ); // P2-C - backlight - reflection+polarizers
	  AfxBeginThread( inspect_3d_particle , LPVOID(10) , THREAD_PRIORITY_HIGHEST ); // P2-C - backlight - reflection+polarizers
    
		}

	if ( IS_PC2 )
      {

      // Cosmetic 1 - line scan - backlight - sidewall - black spots on needle shield
      AfxBeginThread( inspect_3d_cosmetic , LPVOID(0), THREAD_PRIORITY_HIGHEST ); // C1A cosmetic1 control - linescan backlight + barlight
      AfxBeginThread( inspect_3d_cosmetic , LPVOID(1), THREAD_PRIORITY_HIGHEST ); // C1B cosmetic1 control - linescan backlight + barlight
      AfxBeginThread( inspect_3d_cosmetic , LPVOID(2), THREAD_PRIORITY_HIGHEST ); // C1C cosmetic1 control - linescan backlight + barlight
	  AfxBeginThread( inspect_3d_cosmetic , LPVOID(3), THREAD_PRIORITY_HIGHEST ); // C1C cosmetic1 control - linescan backlight + barlight
          
      // Cosmetic 2 - area scan - backlight - needle shield profile
      AfxBeginThread( inspect_3d_particle,  LPVOID(4), THREAD_PRIORITY_HIGHEST ); // C2A cosmetic2 control - areascan backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(5), THREAD_PRIORITY_HIGHEST ); // C2B cosmetic2 control - areascan backlight
      AfxBeginThread( inspect_3d_particle,  LPVOID(6), THREAD_PRIORITY_HIGHEST ); // C2C cosmetic2 control - areascan backlight
	  AfxBeginThread( inspect_3d_particle,  LPVOID(7), THREAD_PRIORITY_HIGHEST ); // C2C cosmetic2 control - areascan backlight
      
	}
		#endif // TIVS_S60 PFS machine



    if ( !wcsncmp( PC_name, L"PC-FILLLEV", 10 ) ) // FILLING LEVEL
    {
      AfxBeginThread( inspect_3d_filllev , LPVOID(0) );
    }


//    for ( i=0 ; i<NUM_TLC ; i++ ) AfxBeginThread( inspect_3d , LPVOID(i) );
    movie_btn->ShowWindow( SW_HIDE );
    recipe_btn->ShowWindow( SW_HIDE );

    if ( inspmode_btn ) inspmode_btn->ShowWindow( SW_HIDE );

    inspection_btn->SetWindowText( string_152[lng] );
    //inspection_btn->EnableWindow( TRUE );

    inspection_active = TRUE;

    CString txt;
    #ifdef SHOW_BATCHNUMBER  // show current batch number in machine_txt field
      if ( learning ) txt.Format( string_067[lng], file_manager.recipe_name, batch_mngr->current.code );
      else            txt.Format( string_065[lng], file_manager.recipe_name, batch_mngr->current.code );
    #else
      if ( learning ) txt.Format( string_066[lng], file_manager.recipe_name );
      else            txt.Format( string_064[lng], file_manager.recipe_name );
    #endif
    //if ( learning ) txt.Format( string_066[lng], file_manager.recipe_name );
    //else            txt.Format( string_064[lng], file_manager.recipe_name );
    if ( machine_txt ) machine_txt->SetText( txt, 0, TXT_GREEN );

  }

	return 0;
}


/*
int Inspection_startstop( int inp )
{
//	TRACE("Inspection_startstop (%d)\n",inp );
	TRACE( L"Inspection_startstop (%d)\n",inp );

  int i=0;
  static BOOL inspection_active = FALSE;


	pView->Close_all_menus();

  // force start
  if ( inp==10 && inspection_active ) return 0; // already active, just return

  // force stop
  if ( inp==20 && !inspection_active ) return 0; // already stopped, just return

  #ifdef BASLER_SDK
    if ( inspection_active )
	  {
      //TRACE("Attivo: provo a fermare ispezione\n");
      for ( i=0 ; i<NUM_TLC ; i++ ) if (CHK_BIT(thread_status,10+i)) SET_BIT( thread_exit, 10+i );

      /*if (CHK_BIT(thread_status,12)) SET_BIT( thread_exit, 12 ); // B12 - camera_3d(0)
      if (CHK_BIT(thread_status,13)) SET_BIT( thread_exit, 13 ); // B13 - camera_3d(1)
      if (CHK_BIT(thread_status,22)) SET_BIT( thread_exit, 22 ); // B22 - vision_3d(0)
      if (CHK_BIT(thread_status,23)) SET_BIT( thread_exit, 23 ); // B23 - gage_3d(0)*/
      /*
      while ( CHK_BIT(thread_status,20) || CHK_BIT(thread_status,21) || CHK_BIT(thread_status,22) || CHK_BIT(thread_status,23) )
      {
        MSG msg;
        PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE );
        Sleep(10);
      }
      */
/*
      //inspection_btn->SetWindowTextA( string_150[lng] );
      inspection_btn->SetWindowText( string_150[lng] );
	  	inspection_btn->EnableWindow( TRUE );
  		//movie_btn->SetWindowTextA( string_160[lng] );
  		movie_btn->SetWindowText( string_160[lng] );
	  	movie_btn->EnableWindow( TRUE );
      //exit_btn->EnableWindow( TRUE );
	  }
    else // Stopped, try to start all
    { 
      //TRACE("Fermo: avvio ispezione\n");
      TRACE("Stopped: start inspection\n");
      for ( i=0 ; i<NUM_TLC ; i++ ) AfxBeginThread( inspect_3d , LPVOID(i) );

      //AfxBeginThread( camera_3d, LPVOID(0) ); // B12
      //AfxBeginThread( camera_3d, LPVOID(1) ); // B13
//      AfxBeginThread( vision_3d, LPVOID(0) ); // B22
//      AfxBeginThread( gage_3d,   LPVOID(0) ); // B23
  		//inspection_btn->SetWindowTextA( string_152[lng] );
  		inspection_btn->SetWindowText( string_152[lng] );
	  	inspection_btn->EnableWindow( TRUE );
		  //movie_btn->SetWindowTextA( string_162[lng] );
		  movie_btn->SetWindowText( string_162[lng] );
		  movie_btn->EnableWindow( FALSE );
      //exit_btn->EnableWindow( FALSE );
  	}
  #endif
  inspection_active = !inspection_active;
  return 0;
}

*/

/*

int ppoint_postprocess( Results_manager* data )
{
  ASSERT( data );

  int     nd2   = data->ndata[0]+data->ndata[1];
  double* data2 = (double*) malloc( nd2*sizeof(double) );
  ZeroMemory( data2, nd2*sizeof(double) );

  int i;
  nd2 = 0;

  // prima colonna, di sinistra
  double* ptr = data->data;
  int     nd  = data->ndata[0];
  for ( i=0 ; i<nd ; i++ ) if ( ptr[i]>=p_point_vmin && ptr[i]<=p_point_vmax ) data2[nd2++]=ptr[i];

  // seconda colonna, di destra
  ptr = data->data + data->max_num_data;
  nd  = data->ndata[1];
  for ( i=0 ; i<nd ; i++ ) if ( ptr[i]>=p_point_vmin && ptr[i]<=p_point_vmax ) data2[nd2++]=ptr[i];

  //TRACE("Finito preanalisi: nd2=%d\n",nd2);
  
//  #define PTR(i) ((i)<0?ptr[i+nd]:((i)>=nd?ptr[i-nd]:ptr[i]))

  int indsta = 0;
  int indend = nd2;

  //      trimming of HIGHEST data requested      -   trimming of LOWEST data requested
  if ( ( p_point_cmax>0.0 && p_point_cmax<100.0 ) || ( p_point_cmin>0.0 && p_point_cmin<100.0 ) )
  {
    CTimer timer;
    qsort( data2, nd2, sizeof(double), compdd );
    TRACE("Ordinamento vettore dati punto p eseguito in %d us - %d valori complessivi\n", timer.elapsed_us(), nd2 );
    //for ( i=0 ; i<nd2 ; i++ ) TRACE("%3d,%8.3lf\n", i, data2[i] );
    if ( p_point_cmin > 0.0 ) indsta = DOUBLETOINT( 0.01*       p_point_cmin *nd2 );
    if ( p_point_cmax > 0.0 ) indend = DOUBLETOINT( 0.01*(100.0-p_point_cmax)*nd2 );
    TRACE("Intervallo after-trim punto %d-%d\n", indsta, indend );
  }

  #define AVGLEN 5
  #define DATA(i) ((i+indsta)<indend?data2[i+indsta]:data2[i+2*indsta-indend])
  //#define DATA(i) ((i)<nd2?data2[i]:data2[i-nd2])

  double media  = 0.0;
  double mediar = 0.0;

  //TRACE("Media finale\n");

  // precarico
  // for ( i=0 ; i<AVGLEN ; i++ ) media += data2[i+indsta];
  for ( i=0 ; i<AVGLEN ; i++ ) media += DATA(i);
  mediar = 0.05 * int( 0.5 + media * 20 / AVGLEN );
  data->Insert(mediar,2); // prima media
  //TRACE("%6.3lf\n", mediar);


  i = 1;
//  while( i<nd2 )
  while( i<indend-indsta )
  {
//    media += data2[i+8]-data2[i-1];
    media += DATA(i+AVGLEN-1)-DATA(i-1);
    mediar = 0.05 * int( 0.5 + media * 20 / AVGLEN );
    // mediar = media / AVGLEN; senza arrotondamento
    data->Insert(mediar,2); 
    //TRACE("%6.3lf\n", mediar);
    i++;
  }

  #undef DATA
  #undef AVGLEN
  
  free( data2 );

  return 0; // completed successfully
}

*/


/*

int psb_postprocess( Results_manager* data )
{
  ASSERT( data );

  int     nd2   = data->ndata[0]+data->ndata[1];
  double* data2 = (double*) malloc( nd2*sizeof(double) );
  ZeroMemory( data2, nd2*sizeof(double) );

  int i;
  nd2 = 0;

  // prima colonna, di sinistra
  double* ptr = data->data;
  int     nd  = data->ndata[0];
  for ( i=0 ; i<nd ; i++ ) { if ( ptr[i]>0.0 ) data2[nd2++]=ptr[i]; }

  // seconda colonna, di destra
  ptr = data->data + data->max_num_data;
  nd  = data->ndata[1];
  for ( i=0 ; i<nd ; i++ ) { if ( ptr[i]>0.0 ) data2[nd2++]=ptr[i]; }

  //TRACE("Finito preanalisi: nd2=%d\n",nd2);
//  #define PTR(i) ((i)<0?ptr[i+nd]:((i)>=nd?ptr[i-nd]:ptr[i]))

  int indsta = 0;
  int indend = nd2;

  //      trimming of HIGHEST data requested      -   trimming of LOWEST data requested
  if ( ( pente_sb_cmax>0.0 && pente_sb_cmax<100.0 ) || ( pente_sb_cmin>0.0 && pente_sb_cmin<100.0 ) )
  {
    CTimer timer;
    qsort( data2, nd2, sizeof(double), compdd );
    TRACE("Ordinamento vettore dati pentesb eseguito in %d us - %d valori complessivi\n", timer.elapsed_us(), nd2 );
    //for ( i=0 ; i<nd2 ; i++ ) TRACE("%3d,%8.3lf\n", i, data2[i] );
    if ( pente_sb_cmin > 0.0 ) indsta = DOUBLETOINT( 0.01*       pente_sb_cmin *nd2 );
    if ( pente_sb_cmax > 0.0 ) indend = DOUBLETOINT( 0.01*(100.0-pente_sb_cmax)*nd2 );
    TRACE("Intervallo after-trim pentesb %d-%d\n", indsta, indend );
  }




  #define AVGLEN 7
  #define DATA(i) ((i+indsta)<indend?data2[i+indsta]:data2[i+2*indsta-indend])
//  #define DATA(i) ((i)<nd2?data2[i]:data2[i-nd2])

  double media  = 0.0;
  double mediar = 0.0;

  if ( nd2 <= AVGLEN ) return( 99 ); // not enough valid data

  // precarico
  for ( i=0 ; i<AVGLEN ; i++ ) media += DATA(i);
  mediar = 0.05 * int( 0.5 + media * 20 / AVGLEN );
  data->Insert(mediar,2); // prima media
  //TRACE("%6.3lf\n", mediar);

  i = 1;
  while( i<indend-indsta )
  {
    media += DATA(i+AVGLEN-1)-DATA(i-1);
    data->Insert(media/AVGLEN,2); 
    i++;
  }

  #undef DATA
  #undef AVGLEN
  
  free( data2 );
  return 0; // completed successfully
}

*/

/*  OLD - FIRST VERSION


int psb_postprocess( Results_manager* data )
{
  ASSERT( data );

  int     nd2   = data->ndata[0]+data->ndata[1];
  double* data2 = (double*) malloc( nd2*sizeof(double) );
  ZeroMemory( data2, nd2*sizeof(double) );

  int i;
  nd2 = 0;

  // prima colonna, di sinistra
  double* ptr = data->data;
  int     nd  = data->ndata[0];
  for ( i=0 ; i<nd ; i++ ) { if ( ptr[i]>0.0 ) data2[nd2++]=ptr[i]; }

  // seconda colonna, di destra
  ptr = data->data + data->max_num_data;
  nd  = data->ndata[1];
  for ( i=0 ; i<nd ; i++ ) { if ( ptr[i]>0.0 ) data2[nd2++]=ptr[i]; }

  //TRACE("Finito preanalisi: nd2=%d\n",nd2);
//  #define PTR(i) ((i)<0?ptr[i+nd]:((i)>=nd?ptr[i-nd]:ptr[i]))


  #define AVGLEN 7

  double media  = 0.0;
  double mediar = 0.0;

  if ( nd2 <= AVGLEN ) return( 99 ); // not enough valid data

  // precarico
  for ( i=0 ; i<AVGLEN ; i++ ) media += data2[i];
  //mediar = 0.05 * int( 0.5 + media * 20 / AVGLEN );
  data->Insert(media/AVGLEN,2); // prima media

  #define DATA(i) ((i)<nd2?data2[i]:data2[i-nd2])

  i = 1;
  while( i<nd2 )
  {
//    media += data2[i+8]-data2[i-1];
    media += DATA(i+AVGLEN-1)-DATA(i-1);
    //mediar = 0.05 * int( 0.5 + media * 20 / AVGLEN );
    // mediar = media / AVGLEN; senza arrotondamento
    data->Insert(media/AVGLEN,2); 
    i++;
  }

  #undef DATA
  #undef AVGLEN
  
  free( data2 );

  return 0; // completed successfully
}




*/

/*   OLD - FIRST VERSION
int ppoint_postprocess( Results_manager* data )
{
  ASSERT( data );

  int i;
  double* ptr = data->data;
  int     nd  = data->ndata[0];
  double  media = 0.0;

  for ( i=2 ; i<nd-2 ; i++ )
  {
    media = 0.2 * ( ptr[i-2]+ptr[i-1]+ptr[i]+ptr[i+1]+ptr[i+2] );
    media = 0.05 * int( 0.5 + media * 20 );
    if ( media >= p_point_vmin && media <= p_point_vmax ) data->Insert(media,2);
  }

  // seconda colonna, di destra
  ptr = data->data + data->max_num_data;
  nd  = data->ndata[1];

  for ( i=2 ; i<nd-2 ; i++ )
  {
    media = 0.2 * ( ptr[i-2]+ptr[i-1]+ptr[i]+ptr[i+1]+ptr[i+2] );
    media = 0.05 * int( 0.5 + media * 20 );
    if ( media >= p_point_vmin && media <= p_point_vmax ) data->Insert(media,2);
  }

//  data->ndata[0] = data->ndata[2];
//  data->ndata[1] = data->ndata[2];
  
  return 0; // completed successfully
}
*/







/*
int findmax_diametre_sc( Results_manager* data )
{
  ASSERT( data );

  int i,j;
  double* ptr = data->data;
  int     nd  = data->ndata[0];

  if ( nd < 10 ) return 99; // Too few diameters: abort


//  CTimer timer;
//  timer.reset();

  // calcolo raccordo fine-inizio
  #define SRCAMP 5 // ampiezza di ricerca
  int    imin=  -1;
  double emin=10.0;
  for ( i=0 ; i<20 ; i++ ) // offset all'inizio
  {
    double err = 0.0;
    for ( j=0 ; j<SRCAMP ; j++ ) err += (ptr[i+j]-ptr[nd-SRCAMP+j])*(ptr[i+j]-ptr[nd-SRCAMP+j]);
    if ( err<emin )
    {
      imin =   i;
      emin = err;
    }
    //TRACE("err[%2d]=%6.1lf\n", i, err );
  }

  if ( emin<1.0 ) TRACE("errmin=%.5lf per i=%d - dati chiusi da %d a %d\n", emin, imin, imin, nd-SRCAMP );
  else 
  {
    ASSERT( 0 );
    TRACE("ERRORE DI RICONGIUNGIMENTO!\n");

  }

  //for ( j=0 ; j<SRCAMP ; j++ ) TRACE("%i,%.3lf,%i,%.3lf\n",imin+j,ptr[imin+j],nd-SRCAMP+j,ptr[nd-SRCAMP+j]);
  // cos?sembra ok!
//  ptr = data->data + imin + 2;
//  nd = nd-SRCAMP-imin-3;

  // cos?sembra decisamente meglio
  ptr = data->data + imin;
  nd -= SRCAMP + imin;

//  return 0;

  // inizio ricerca massimi
  nummax = 0;
  ZeroMemory( maxima, sizeof(maxima) );

  #define DER1(i) (ptr[i+1]-ptr[i-1])
  #define DER2(i) (DER1(i+1)-DER1(i-1))
  #define PTR(i) ((i)<0?ptr[i+nd]:((i)>=nd?ptr[i-nd]:ptr[i]))

  for ( i=0 ; i<nd ; i++ )
  {
    //TRACE("%d,%.3lf\n",i,ptr[i]);

//    if ( DER1(i)>=0.0 && DER1(i+1)<0.0 && 
//         PTR(i  )>PTR(i-2) && PTR(i  )>PTR(i-3) && PTR(i  )>PTR(i-6) && 
//         PTR(i+1)>PTR(i+3) && PTR(i+1)>PTR(i+4) && PTR(i+1)>PTR(i+7) ) // massimo relativo
    //TRACE("i=%d (%d) - DER=%.3lf\n", i, i+imin+2, DER1(i) );
    if ( DER1(i)>=0.0 && DER1(i+1)<0.0 )
    {
//      if ( PTR(i  )>PTR(i-2) && PTR(i  )>PTR(i-4) && PTR(i  )>PTR(i-6) && 
//           PTR(i+1)>PTR(i+3) && PTR(i+1)>PTR(i+5) && PTR(i+1)>PTR(i+7) ) // massimo relativo
      if ( ( 3*PTR(i  )-PTR(i-2)-PTR(i-4)-PTR(i-6) ) > 0.15 && 
           ( 3*PTR(i+1)-PTR(i+3)-PTR(i+5)-PTR(i+7) ) > 0.15 ) // massimo relativo
      {

//         ptr[i  ]>ptr[i-2] && ptr[i  ]>ptr[i-3] && ptr[i  ]>ptr[i-6] && 
//         ptr[i+1]>ptr[i+3] && ptr[i+1]>ptr[i+4] && ptr[i+1]>ptr[i+7] ) // massimo relativo
//    {
        if ( nummax<MAXNUMMAX )
        {
          for ( j=-3 ; j<=3 ; j++ )
          {
            if ( ptr[i+j]>maxima[nummax].val )
            {
              maxima[nummax].val = ptr[i+j];
              maxima[nummax].ind = i+j;
            }
          }
          if ( data->categories==2 ) data->Insert( maxima[nummax].val, 1 );
          //TRACE("Massimo in i=%4d-%4d - vmax=%7.3lf\n", i+imin+1, i+imin+2, maxima[nummax].val );
          nummax++;
        }
        else TRACE("Raggiunto MAXNUMMAX (%d) numero di massimi!\n", MAXNUMMAX );
        i++;
      }

    }

  } 

  #undef PTR

  return 0; // completed successfully
}


*/


/*

int find_minima_new( Results_manager* data )
{
  ASSERT( data );

  int i,j;
  double* ptr = data->data;
  int     nd  = data->ndata[0];

  if ( nd < 10 ) return 99; // Too few diameters: abort


//  CTimer timer;
//  timer.reset();

  // calcolo raccordo fine-inizio
  #define SRCAMP 5 // ampiezza di ricerca
  int    imin=  -1;
  double emin=10.0;
  for ( i=0 ; i<20 ; i++ ) // offset all'inizio
  {
    double err = 0.0;
    for ( j=0 ; j<5 ; j++ ) err += (ptr[i+j]-ptr[nd-SRCAMP+j])*(ptr[i+j]-ptr[nd-SRCAMP+j]);
    if ( err<emin )
    {
      imin =   i;
      emin = err;
    }
    //TRACE("err[%2d]=%6.1lf\n", i, err );
  }

  if ( emin<1.0 ) TRACE("errmin=%6.1lf per i=%d - dati chiusi da %d a %d (%d us)\n", emin, imin, imin, nd-SRCAMP, timer.elapsed_us() );
  else 
  {
//    ASSERT( 0 );
    TRACE("ERRORE DI RICONGIUNGIMENTO!\n");

  }
//  return 0;

  // cos?sembra ok!
  //ptr = data->data + imin + 2;
  //nd = nd-SRCAMP-imin-3;

  // cos?sembra decisamente meglio
  ptr = data->data + imin;
  nd -= SRCAMP + imin;


  // inizio ricerca minimi/massimi
  nummin = 0;
  nummax = 0;
  body_width     = 0.0;
  body_thickness = 0.0;
  ZeroMemory( minima, sizeof(minima) );
  ZeroMemory( maxima, sizeof(maxima) );


  #define DER1(i) (ptr[i+1]-ptr[i-1])
  #define DER2(i) (DER1(i+1)-DER1(i-1))


  for ( i=0 ; i<nd ; i++ )
  {
    //TRACE("%d,%.3lf\n",i,ptr[i]);

    if ( DER1(i)>=0.0 && DER1(i+1)<0.0 ) // massimo relativo
    {
      if ( nummax<MAXNUMMAX )
      {
        for ( j=-3 ; j<=3 ; j++ )
        {
          if ( ptr[i+j]>maxima[nummax].val )
          {
            maxima[nummax].val = ptr[i+j];
            maxima[nummax].ind = i+j;
          }
        }
        TRACE("Massimo in i=%4d-%4d - DER2=%6.3lf - vmax=%7.3lf\n", i, i+1, DER2(i), maxima[nummax].val );
        nummax++;
      }
      else TRACE("Raggiunto MAXNUMMAX (%d) numero di massimi!\n", MAXNUMMAX );
      i++;
    }

    if ( DER1(i)<0.0 && DER1(i+1)>=0.0 ) // minimo relativo
    {
      if ( DER2(i)>=peak_d2th ) // minimo a cuspide
      {
        double b   = 2.3; // slope
        double sxl = 0.0;
        double syl = 0.0;
        double sxr = 0.0;
        double syr = 0.0;
        for ( j=0 ; j<=5 ; j++ )
        {
          sxl += -j;
          syl += ptr[i-j];
          sxr += j+1;
          syr += ptr[i+j+1];
        }

        if ( nummin<MAXNUMMIN )
        {
          minima[nummin].val = (syl+b*sxl+syr-b*sxr)/12.0;
          minima[nummin].ind = i;
          TRACE("Minimo a cuspide in i=%4d-%4d - DER2=%6.3lf - vmin=%7.3lf\n", i, i+1, DER2(i), minima[nummin].val );
          
          BOOL keep=TRUE;
          for ( j=0 ; j<nummin ; j++ )
          {
            if ( abs(minima[nummin].val-minima[j].val)<0.5 )
            {
              TRACE("Doppio minimo cuspide, tieni il minore dei due e skippa!");
              if ( minima[j].val > minima[nummin].val )
              {
                minima[j].val = minima[nummin].val;
                minima[j].ind = minima[nummin].ind;
              }
              keep = FALSE;
            }
          }
          if ( keep ) nummin++; // se ?rimasto il minimo

        }
        else TRACE("Raggiunto MAXNUMMIN (%d) numero di minimi!\n", MAXNUMMIN );
        i++;

      }
      else // DER2 sotto soglia - minimo morbido
      {
        if ( nummin<MAXNUMMIN )
        {
          minima[nummin].val = 1.0e6;
          for ( j=-3 ; j<=3 ; j++ )
          {
            if ( ptr[i+j]<minima[nummin].val )
            {
              minima[nummin].val = ptr[i+j];
              minima[nummin].ind = i+j;
            }
          }
          TRACE("Minimo morbido in i=%4d-%4d - DER2=%6.3lf - vmin=%7.3lf\n", i, i+1, DER2(i), minima[nummin].val );
          //nummin++;

          BOOL keep=TRUE;
          for ( j=0 ; j<nummin ; j++ )
          {
            if ( abs(minima[nummin].val-minima[j].val)<0.5 )
            {
              TRACE("Doppio minimo morbido, tieni il minore dei due e skippa!\n");
              if ( minima[j].val > minima[nummin].val )
              {
                minima[j].val = minima[nummin].val;
                minima[j].ind = minima[nummin].ind;
              }
              keep = FALSE;
            }
          }
          if ( keep ) nummin++; // se ?rimasto il minimo

        }
        else TRACE("Raggiunto MAXNUMMIN (%d) numero di minimi!\n", MAXNUMMIN );
        i++;
      }
    } 

  }  

  if ( nummin==2 && nummax>=2 ) // caso royer/rettangolare, scarta i due o pi?massimi
  {

    if ( minima[0].val < minima[1].val )
    {
      body_thickness = minima[0].val;
      body_width     = minima[1].val;
    }
    else
    {
      body_thickness = minima[1].val;
      body_width     = minima[0].val;
    }

    return 0;
  }

  if ( nummin==1 && nummax==1 ) // imperial - 1min e 1 max
  {
    body_thickness = minima[0].val;
    body_width     = maxima[0].val;
    return 0;
  }


  if ( nummin>2 ) // se ho pi?di due min, prendo i due minori
  {
    body_thickness = minima[0].val;
    body_width     = minima[0].val;
  
    for ( i=1 ; i<nummin ; i++ )
    {
      if ( body_thickness > minima[i].val )
      {
        body_width     = body_thickness;
        body_thickness = minima[i].val;
      }
      else if ( body_width > minima[i].val ) body_width = minima[i].val;
    }
    return 0;
  }


//  else TRACE("FALLITO: %d/%d min/max trovati!\n", nummin, nummax );

  return 99; // completed successfully
}


*/


/*

int find_minima( Results_manager* data )
{

 // #define MIN_AMP 2
 // #define DELTA 0.03

  #define DER1(i) (ptr[i+1]-ptr[i-1])
  #define DER2(i) (DER1(i+1)-DER1(i-1))

  ASSERT( data );

  int i,j;

  double* ptr = data->data;

  nummin = 0;
  nummax = 0;
  body_width     = 0.0;
  body_thickness = 0.0;
  ZeroMemory( minima, sizeof(minima) );
  ZeroMemory( maxima, sizeof(maxima) );

  if ( data->ndata[0] < 10 ) return 99; // Too few diameters: abort

  for ( i=7 ; i<int(data->ndata[0]-7) ; i++ )
  {

    if ( DER1(i)>=0.0 && DER1(i+1)<0.0 ) // massimo relativo
    {
      if ( nummax<MAXNUMMAX )
      {
        for ( j=-3 ; j<=3 ; j++ )
        {
          if ( ptr[i+j]>maxima[nummax].val )
          {
            maxima[nummax].val = ptr[i+j];
            maxima[nummax].ind = i+j;
          }
        }
        TRACE("Massimo in i=%4d-%4d - DER2=%6.3lf - vmax=%7.3lf\n", i, i+1, DER2(i), maxima[nummax].val );
        nummax++;
      }
      else TRACE("Raggiunto MAXNUMMAX (%d) numero di massimi!\n", MAXNUMMAX );
      i++;
    }

    if ( DER1(i)<0.0 && DER1(i+1)>=0.0 ) // minimo relativo
    {
      if ( DER2(i)>=peak_d2th ) // minimo a cuspide
      {
        double b   = 2.3; // slope
        double sxl = 0.0;
        double syl = 0.0;
        double sxr = 0.0;
        double syr = 0.0;
        for ( j=0 ; j<=5 ; j++ )
        {
          sxl += -j;
          syl += ptr[i-j];
          sxr += j+1;
          syr += ptr[i+j+1];
        }

        if ( nummin<MAXNUMMIN )
        {
          minima[nummin].val = (syl+b*sxl+syr-b*sxr)/12.0;
          minima[nummin].ind = i;
          TRACE("Minimo a cuspide in i=%4d-%4d - DER2=%6.3lf - vmin=%7.3lf\n", i, i+1, DER2(i), minima[nummin].val );
          
          BOOL keep=TRUE;
          for ( j=0 ; j<nummin ; j++ )
          {
            if ( abs(minima[nummin].val-minima[j].val)<0.5 )
            {
              TRACE("Doppio minimo cuspide, tieni il minore dei due e skippa!");
              if ( minima[j].val > minima[nummin].val )
              {
                minima[j].val = minima[nummin].val;
                minima[j].ind = minima[nummin].ind;
              }
              keep = FALSE;
            }
          }
          if ( keep ) nummin++; // se ?rimasto il minimo

        }
        else TRACE("Raggiunto MAXNUMMIN (%d) numero di minimi!\n", MAXNUMMIN );
        i++;

      }
      else // DER2 sotto soglia - minimo morbido
      {
        if ( nummin<MAXNUMMIN )
        {
          minima[nummin].val = 1.0e6;
          for ( j=-3 ; j<=3 ; j++ )
          {
            if ( ptr[i+j]<minima[nummin].val )
            {
              minima[nummin].val = ptr[i+j];
              minima[nummin].ind = i+j;
            }
          }
          TRACE("Minimo morbido in i=%4d-%4d - DER2=%6.3lf - vmin=%7.3lf\n", i, i+1, DER2(i), minima[nummin].val );
          //nummin++;

          BOOL keep=TRUE;
          for ( j=0 ; j<nummin ; j++ )
          {
            if ( abs(minima[nummin].val-minima[j].val)<0.5 )
            {
              TRACE("Doppio minimo morbido, tieni il minore dei due e skippa!\n");
              if ( minima[j].val > minima[nummin].val )
              {
                minima[j].val = minima[nummin].val;
                minima[j].ind = minima[nummin].ind;
              }
              keep = FALSE;
            }
          }
          if ( keep ) nummin++; // se ?rimasto il minimo

        }
        else TRACE("Raggiunto MAXNUMMIN (%d) numero di minimi!\n", MAXNUMMIN );
        i++;
      }
    }
  }  

  if ( nummin==2 && nummax>=2 ) // caso royer/rettangolare, scarta i due o pi?massimi
  {

    if ( minima[0].val < minima[1].val )
    {
      body_thickness = minima[0].val;
      body_width     = minima[1].val;
    }
    else
    {
      body_thickness = minima[1].val;
      body_width     = minima[0].val;
    }

    return 0;
  }

  if ( nummin==1 && nummax==1 ) // imperial - 1min e 1 max
  {
    body_thickness = minima[0].val;
    body_width     = maxima[0].val;
    return 0;
  }


  if ( nummin>2 ) // se ho pi?di due min, prendo i due minori
  {
    body_thickness = minima[0].val;
    body_width     = minima[0].val;
  
    for ( i=1 ; i<nummin ; i++ )
    {
      if ( body_thickness > minima[i].val )
      {
        body_width     = body_thickness;
        body_thickness = minima[i].val;
      }
      else if ( body_width > minima[i].val ) body_width = minima[i].val;
    }
    return 0;
  }


//  else TRACE("FALLITO: %d/%d min/max trovati!\n", nummin, nummax );

  return 99; // completed successfully
}




*/


/* VECCHIA 
int Find_minima( Results_manager* data )
{

 // #define MIN_AMP 2
 // #define DELTA 0.03

  #define DER1(i) (ptr[i+1]-ptr[i-1])
  #define DER2(i) (DER1(i+1)-DER1(i-1))

  ASSERT( data );

  int i,j;

  double* ptr = data->data;

  ZeroMemory( minima, sizeof(minima) );
  ZeroMemory( maxima, sizeof(maxima) );


  for ( i=5 ; i<data->ndata[0]-5 ; i++ )
  {

    if ( DER1(i)>=0.0 && DER1(i+1)<0.0 ) // massimo relativo
    {
      double y0 = ptr[i];
      if ( ptr[i-1]>y0 ) y0=ptr[i-1];
      if ( ptr[i+1]>y0 ) y0=ptr[i+1];
      TRACE("Massimo in i=%4d-%4d - DER2=%6.3lf - Y0=%7.3lf\n", i, i+1, DER2(i), y0 );
      i++;
    }

    if ( DER1(i)<0.0 && DER1(i+1)>=0.0 ) // minimo relativo
    {
      if ( DER2(i)>= peak_d2th ) // minimo a cuspide
      {
        double b   = 2.3; // slope
        double al  = 0.0; // left intercept
        double ar  = 0.0; // right intercept
        double sxl = 0.0;
        double syl = 0.0;
        double sxr = 0.0;
        double syr = 0.0;
        for ( j=0 ; j<=5 ; j++ )
        {
          sxl += -j;
          syl += ptr[i-j];
          sxr += j+1;
          syr += ptr[i+j+1];
        }
        al = (syl+b*sxl)/6;
        ar = (syr-b*sxr)/6;
        double y0 = (al+ar)/2.0;
        TRACE("Minimo a cuspide in i=%4d-%4d - DER2=%6.3lf - Y0=%7.3lf\n", i, i+1, DER2(i), y0 );
      }
      else // DER2 sotto soglia - minimo morbido
      {
        double y0 = ptr[i];
        if ( ptr[i-1]<y0 ) y0=ptr[i-1];
        if ( ptr[i+1]<y0 ) y0=ptr[i+1];
        TRACE("Minimo morbido   in i=%4d-%4d - DER2=%6.3lf - Y0=%7.3lf\n", i, i+1, DER2(i), y0 );
        i++;
      }
    }
  }  

  return 0;
}

*/



/*    if ( data->data[i]-data->data[i-1]<0.0 && data->data[i+1]-data->data[i]>=0.0 )
    {
      TRACE("Cambio di segno neg=>pos in i=%d\n", i );
    }

    if ( data->data[i]-data->data[i-1]>=0.0 && data->data[i+1]-data->data[i]<0.0 )
    {
      TRACE("Cambio di segno pos=>neg in i=%d\n", i );
    } */


/*    // ricerca minimo "morbido"
    if (  1                    )
    {


    }
    else
    {
      

      if ( ptr[i  ]<ptr[i  -MIN_AMP]-DELTA && ptr[i  ]<ptr[i  +MIN_AMP]-DELTA &&
        ptr[i+1]<ptr[i+1-MIN_AMP]-DELTA && ptr[i+1]<ptr[i+1+MIN_AMP]-DELTA ) 
      {
        double media   = 0.5*(ptr[i]+ptr[i+1]);
        //double deltasx = 

        TRACE("Minimo locale in i=%d-%d - media=%.3lf\n", i, i+1, media );
      }

    } */




int Sequence_acquire( int inp )
{
  pView->Close_all_menus();

  AfxBeginThread( sequence_3d , LPVOID(0) );

  return 0;
}




// Import reflections learning from images on disk
int Import_reflections( int inp )
{
  wchar_t  txt[200+1];

  for ( UINT i=0 ; i<num_tlc ; i++ )
  {
    if ( particle_insp[i] )
    {
      swprintf( txt, 200, L"%s\\learning[%s].png", IMAGES_DIRECTORY, camera[i].name );
      Load_buffer( particle_insp[i]->reflections, particle_insp[i]->dimx, particle_insp[i]->dimy, txt, PNG_FILE );
    }
  }

  Show_reflections(0);

  return 0;
}


// Export reflections learning to images on disk
int Export_reflections( int inp )
{
  wchar_t  txt[200+1];

  for ( UINT i=0 ; i<num_tlc ; i++ )
  {
    if ( particle_insp[i] )
    {
      swprintf( txt, 200, L"%s\\learning[%s]", IMAGES_DIRECTORY, camera[i].name );
      Save_buffer( particle_insp[i]->reflections, particle_insp[i]->dimx, particle_insp[i]->dimy, txt, PNG_FILE );
    }
  }

  Show_reflections(0);

  return 0;
}





// Start/stop reflections learning
int Filter_reflections( int inp )
{
  TRACE("PROCESS FILTERING\n");

  pView->Close_all_menus();

  UINT i   = 0;
  UINT sta = 0;
  UINT end = 0;

  BOOL save_images = FALSE;

  //save_images = TRUE; // when enabled, reflections are saved to images BEFORE and AFTER filtering

  BYTE* backup[12]; // = { NULL, NULL, NULL, NULL };
  ZeroMemory( backup, sizeof(backup) );

  if ( IS_PC1 ) // 3-4-5-6
  {
    Change_view( 1 ); // 0=show first group - 1=show second group - no input cycle
    sta = tlcgroupPC1[1][0];
    end = tlcgroupPC1[1][1];

    #ifdef TIVS_C20
      sta = 6;
      end = 7;
      Change_view( 3 ); // 0=show first group - 1=show second group - no input cycle
    #endif
  }

  if ( IS_PC2 ) // 0-1-2-3 (or 0-1-2-3 / 4-5-6-7 in case of 3 particles)
  {
    Change_view( 0 ); // 0=show first group - 1=show second group - no input cycle
    sta = tlcgroupPC2[0][0];
    end = tlcgroupPC2[0][1];

    #if STN3==PARTICLE_BACK || STN3==PARTICLE_BOTTOM
      end = tlcgroupPC2[1][1];
    #endif

    #ifdef TIVS_C20
      sta = 4;
      end = 5;
      Change_view( 2 ); // 0=show first group - 1=show second group - no input cycle
    #endif
  }


  for ( i=0 ; i<=end-sta ; i++ )
  {
    int ind = sta+i;
    camera[ind].text[1]->SetText( string_2044[lng], TXT_DARKRED );
    ASSERT( particle_insp[ind] && particle_insp[ind]->reflections );
    backup[i] = (BYTE*) malloc( particle_insp[ind]->dim );
    CopyMemory( backup[i], particle_insp[ind]->reflections, particle_insp[ind]->dim );

    if ( save_images )
    {
      wchar_t name[100+1];
      swprintf( name, 100, L"%s\\reflection_raw[%d]", IMAGES_DIRECTORY, ind );
      Save_buffer( particle_insp[ind]->reflections, particle_insp[ind]->dimx, particle_insp[ind]->dimy, name, PNG_FILE );
    }

    particle_insp[ind]->Filter_reflections( ind );

    if ( save_images )
    {
      wchar_t name[100+1];
      swprintf( name, 100, L"%s\\reflection_flt[%d]", IMAGES_DIRECTORY, ind );
      Save_buffer( particle_insp[ind]->reflections, particle_insp[ind]->dimx, particle_insp[ind]->dimy, name, PNG_FILE );
    }

  }

  Show_reflections(0);

  if ( Message_box( string_2040[lng], MBOX_YESNO ) != IDYES )
  {
    for ( i=0 ; i<=end-sta ; i++ )
    {
      int ind = sta+i;
      CopyMemory( particle_insp[ind]->reflections, backup[i], particle_insp[ind]->dim );
    }
    Show_reflections(0);
  }

  for ( i=0 ; i<12 ; i++ )
  {
    if ( backup[i] ) free( backup[i] );
    backup[i] = 0;
  }


  return 0;
}





// 
int Cycle_campwr( int inp )
{
  TRACE("CAMERAS CYCLE POWER\n");

  if ( Message_box( string_348[lng], MBOX_YESNO ) != IDYES ) return 99; 

  if ( ADS_write_value( ".F_CYCLE_CAMPWR", TRUE, CTYPE_BOOL8 ) ) // Force flag into PLC
  {
    Message_box( L"Unable to command camera reset to PLC", MBOX_ERROR );
    return 99;
  }
  
  CString txt;
  CTimer timer;

  campwr_btn->enabled = FALSE;
  MSG msg; 

  while ( timer.elapsed_sec()<20 )
  {
    txt.Format( string_344[lng], timer.elapsed_sec() );
    campwr_btn->SetWindowTextW( txt );
    PeekMessage( &msg, NULL, 0, 0, PM_REMOVE);
    Sleep(5);
  }

  campwr_btn->enabled = TRUE;
  campwr_btn->SetWindowTextW( string_342[lng] );

  Message_box( string_346[lng], MBOX_INFO );


  return 0;
}





// Start/stop reflections learning
int Show_reflections( int inp )
{
  TRACE("SHOW LEARNING\n");

  pView->Close_all_menus();

  UINT i;

  UINT sta = tlcgroupPC1[1][0];
  UINT end = tlcgroupPC1[1][1];

  if ( IS_PC1 )
  {
    Change_view( 2 );

    #ifdef TIVS_C20
      sta = 6;
      end = 7;
      Change_view( 3 );
    #endif

    for ( i=sta ; i<=end ; i++ )
    {
      ASSERT( particle_insp[i] && particle_insp[i]->reflections );
      SCALE_IMG(i,particle_insp[i]->reflections);
      image_dsp[i]->Draw_image();
      camera[i].text[1]->SetText( string_2058[lng], 0 );
    }
  }

  if ( IS_PC2 ) 
  {
    Change_view( 1 ); // 0=show first group - 1=show second group - no input=cycle
    sta = 0;
    end = tlcgroupPC2[0][1];

    #if STN3==PARTICLE_BACK || STN3==PARTICLE_BOTTOM
      end = tlcgroupPC2[1][1];
    #endif

    #ifdef TIVS_C20
      sta = tlcgroupPC2[1][0];
      end = tlcgroupPC2[1][1];
      Change_view( 2 );
    #endif

    for ( i=sta ; i<=end ; i++ )
    {
      ASSERT( particle_insp[i] && particle_insp[i]->reflections );
      SCALE_IMG(i,particle_insp[i]->reflections);
      image_dsp[i]->Draw_image();
      camera[i].text[1]->SetText( string_2058[lng], 0 );
    }

  }

  return 0;
}



// Start/stop models learning
int Learn_models( int inp )
{

  #ifdef CFR21
    if ( login_wnd->PreOperation( LEVEL_INSP_PAR, L"INSPECTION/LEARN REFLECTION" )) return 0;
  #endif

//  int i;

	if ( learning ) // Enabled
  {
    TRACE("START MODELS LEARNING\n");

/*    if ( Message_box( string_2056[lng], MBOX_YESNO ) == IDNO )
		{

      if ( !wcsncmp( PC_name, L"PC1", 3 ) ) // PC1
      {
        for ( i=3 ; i<7 ; i++ )
        {
          ASSERT( particle_insp[i] );
          ZeroMemory( particle_insp[i]->reflections, particle_insp[i]->dim );
          camera[i].text[1]->SetText( string_2050[lng], 0 );
        }
      }

      if ( !wcsncmp( PC_name, L"PC2", 3 ) ) // PC2
      {
        for ( i=0 ; i<4 ; i++ )
        {
          ASSERT( particle_insp[i] );
          ZeroMemory( particle_insp[i]->reflections, particle_insp[i]->dim );
          camera[i].text[1]->SetText( string_2050[lng], 0 );
        }
      }
    }  */

  }
  else
  {
    TRACE("STOP MODELS LEARNING\n");

/*    if ( !wcsncmp( PC_name, L"PC1", 3 ) ) // PC1
    {
      for ( i=3 ; i<7 ; i++ )
      {
        camera[i].text[1]->SetText( string_2048[lng], 0 );
      }
    }

    if ( !wcsncmp( PC_name, L"PC2", 3 ) ) // PC2
    {
      for ( i=0 ; i<4 ; i++ )
      {
        camera[i].text[1]->SetText( string_2048[lng], 0 );
      }
    }   */

  }

	return 0;
}


// Start/stop reflections learning
int Learn_needleshield( int inp )
{
  #ifdef TIVS_S40
    if ( inp==12 )
    {
      for ( int ind=12 ; ind<=14 ; ind++ )
      {
        if ( pfsidewall_insp[ind] ) pfsidewall_insp[ind]->Learning_switch();
      }
    }

    if ( inp==1 || inp==2 )
    {
      if ( pfsidewall_insp[inp] ) pfsidewall_insp[inp]->Learning_switch();
    }

  #endif


	  #ifdef TIVS_S60
    if ( inp==15 )
    {
      for ( int ind=15 ; ind<=18 ; ind++ )
      {
        if ( pfsidewall_insp[ind] ) pfsidewall_insp[ind]->Learning_switch();
      }
    }

    if ( inp==1 || inp==2 )
    {
      if ( pfsidewall_insp[inp] ) pfsidewall_insp[inp]->Learning_switch();
    }

  #endif

  return 0;
}


// Start/stop reflections learning
int Datasave_startstop( int inp )
{
  #ifdef TIVS_S40
    if ( inp==12 )
    {
      for ( int ind=12 ; ind<=14 ; ind++ )
      {
        if ( pfsidewall_insp[ind] ) pfsidewall_insp[ind]->Saving_switch();
      }
    }

    if ( inp==1 || inp==2 )
    {
      if ( pfsidewall_insp[inp] ) pfsidewall_insp[inp]->Saving_switch();
    }


  #endif

	#ifdef TIVS_S60
    if ( inp==15 )
    {
      for ( int ind=15 ; ind<=18 ; ind++ )
      {
        if ( pfsidewall_insp[ind] ) pfsidewall_insp[ind]->Saving_switch();
      }
    }

    if ( inp==1 || inp==2 )
    {
      if ( pfsidewall_insp[inp] ) pfsidewall_insp[inp]->Saving_switch();
    }


  #endif

  return 0;
}




// Start/stop reflections learning
int Learn_reflections( int inp )
{

  #ifdef CFR21
    if ( login_wnd->PreOperation( LEVEL_INSP_PAR, L"INSPECTION/LEARN REFLECTION" )) return 0;
  #endif

  UINT i;
  UINT sta = tlcgroupPC1[1][0];
  UINT end = tlcgroupPC1[1][1];

  if ( IS_PC2 ) // PC2
  {
    sta = tlcgroupPC2[0][0];
    end = tlcgroupPC2[0][1];

    #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK
      end = tlcgroupPC2[1][1];;
    #endif
  }

  #ifdef TIVS_C20
    if ( IS_PC1 ) // PC1
    {
      sta = 6;
      end = 7;
    }
    if ( IS_PC2 ) // PC2
    {
      sta = tlcgroupPC2[1][0];
      end = tlcgroupPC2[1][1];
    }
  #endif

	if ( learning ) // Enabled
  {
    TRACE("START LEARNING\n");
    if ( Message_box( string_2056[lng], MBOX_YESNO ) == IDNO )
		{
      for ( i=sta ; i<=end ; i++ )
      {
        ASSERT( particle_insp[i] );
        ZeroMemory( particle_insp[i]->reflections, particle_insp[i]->dim );
        camera[i].text[1]->SetText( string_2050[lng], 0 );
      }
    }
  }
  else
  {
    TRACE("STOP LEARNING\n");
    for ( i=sta ; i<=end ; i++ )
    {
      camera[i].text[1]->SetText( string_2048[lng], 0 );
    }
  }

  Show_reflections(0);
	return 0;
}



int Send_spin_parameters( int inp )
{
  ADS_write_value( "SEND_SPIN_PARAMETERS.RUN", 1, CTYPE_BOOL8 );
  return 0;
}


int Reset_statistics( int inp )
{
  if ( Message_box( string_1312[lng], MBOX_YESNO ) == IDYES )
  {
    ADS_write_value( ".F_RESET_STATISTICS", 1, CTYPE_BOOL8 );
    if ( logger ) logger->Add_string( L"Batch statistics resetted" );
  }
  pView->Close_all_menus();
  return 0;
}



int Batch_report( int inp )
{

  if ( batch_mngr )
  {
    //batch_mngr->Create_report();//Pour removed on 20200707

	  	if ( batch_mngr->current.status==1 ) // batch active - ask to end
	{
		CString batch_info;
	    batch_info.Format( string_1328a[lng], batch_mngr->current.code, batch_mngr->current.name );
        Message_box( batch_info, MBOX_INFO );
	    return 0;
	}
		else
		{
			//no batch active on 20200707
		CString warning_txt;
	    warning_txt.Format( string_1328b[lng]);
        Message_box( warning_txt, MBOX_ERROR );
		return 0;
		}

  }

  pView->Close_all_menus();

  return 0;
}



int Batch_startstop( int inp )
{

  // ask for statistics reset
/*  if ( Message_box( string_1312[lng], MBOX_YESNO ) == IDYES )
  {
    //ADS_write_value( ".F_RESET_STATISTICS", 1, CTYPE_BOOL8 );
    if ( logger ) logger->Add_string( L"Batch statistics resetted" );
  }  */


  if ( batch_mngr->current.status==0 ) // batch NOT active - ask to create
  {

    batch_mngr->Create( IDD_BATCH_DIALOG, AfxGetMainWnd() );
/*
    #ifdef CIRON_BATCH   // to use ciron custom batch report
      batch_mngr->Create( IDD_BATCH_DIALOGV2, AfxGetMainWnd() );
    #else
    #endif 
*/


    if ( batch_mngr->RunModalLoop()==IDOK )
    {
      batch_mngr->current.status = 1;
      batch_mngr->current.start_time = CTime::GetCurrentTime();
      batch_mngr->Save_data();
      if ( batch_btn ) batch_btn->SetWindowTextW( string_1306[lng] );
	  //Pour added 20190328
	  // start new batch, mark it in log file 
	  CString txt;
	  CString txt1;
	  txt.Format( L"====== New batch started ======" );
	  logger->Add_string( &txt );
	  //Pour added 20180907
	  txt1.Format( L"New batch name  %s", batch_mngr->current.name );
	  logger->Add_string( &txt1 );
	  //Pour added 20180907
	  //-------------------------------
	  //Pour added 20190328

      TRACE("New batch started\n");

      if ( logger ) 
      {
        CString msg;
        msg.Format( string_1314[lng], batch_mngr->current.code, batch_mngr->current.desc );
        logger->Add_string( &msg );
      }

    }
    else
    {
      TRACE("New batch NOT started - canceled\n");
    }
    batch_mngr->DestroyWindow();
    return 100;
  }

  if ( batch_mngr->current.status==1 ) // batch active - ask to end
  {
    if ( Message_box( string_1338[lng], MBOX_YESNO ) == IDYES )
    {
      batch_mngr->current.status   = 0;
      batch_mngr->current.end_time = CTime::GetCurrentTime();
      batch_mngr->Save_data();
	  //Pour removed 20190328
      //batch_mngr->Create_report();//original code

	  //Pour added 20190328
	  CString txt;
	  txt.Format( L"====== Current batch ended ======" );
	  logger->Add_string( &txt );

	  // xinchang add the log to batchreporty, so save the log file then make the report
	#if SERIAL_N==201500013 ||SERIAL_N==201800027 || SERIAL_N==201900198
	  logger->Save_txt_archive();
	  //logger->Save_alarm_txt_archive(); //Pour added 20190220
	  batch_mngr->Create_report();
	#else 
	  batch_mngr->Create_report();
	#endif
	  //Pour added 20190328
      if ( batch_btn ) batch_btn->SetWindowTextW( string_1304[lng] );
      TRACE("Current active batch ended\n");
      if ( logger ) 
      {
        CString msg;
        msg.Format( string_1316[lng], batch_mngr->current.code, batch_mngr->current.desc );
        logger->Add_string( &msg );
      }
    }

  }


  pView->Close_all_menus();

  return 0;
}







/*
int Read_version( void )
{

  CString modFilename;
  if(GetModuleFileName(AfxGetInstanceHandle(), modFilename.GetBuffer(MAX_PATH), MAX_PATH) > 0)
  {
    modFilename.ReleaseBuffer(MAX_PATH);
    DWORD dwHandle = 0;
    DWORD dwSize = GetFileVersionInfoSize(modFilename, &dwHandle);
    if(dwSize > 0)
    {
      LPBYTE lpInfo = new BYTE[dwSize];
      ZeroMemory(lpInfo, dwSize);
      if(GetFileVersionInfo(modFilename, 0, dwSize, lpInfo))
      {
        //// Use the version information block to obtain the FILEVERSION.
        //// This will extract language specific part of versio resources. 040904E4 is English(US) locale, 
        //// it should match to your project
        //UINT valLen = MAX_PATH;
        //LPVOID valPtr = NULL;
        //if(::VerQueryValue(lpInfo, 
        //  TEXT("\\StringFileInfo\\040904E4\\FileVersion"),
        //  &valPtr,
        //  &valLen))
        //{
        //  CString valStr((LPCTSTR)valPtr);

        //  AfxMessageBox(valStr);
        //}

        //// This will extract so called FIXED portion of the version info
        UINT valLen = MAX_PATH;
        LPVOID valPtr = NULL;
        if(::VerQueryValue(lpInfo, 
          TEXT("\\"),
          &valPtr,
          &valLen))
        {
          VS_FIXEDFILEINFO* pFinfo = (VS_FIXEDFILEINFO*)valPtr;

          // convert to text
          CString valStr;
          valStr.Format(_T("%d.%d.%d.%d"), 
            (pFinfo->dwFileVersionMS >> 16) & 0xFFFF,
            (pFinfo->dwFileVersionMS) & 0xFFFF,
            (pFinfo->dwFileVersionLS >> 16) & 0xFFFF,
            (pFinfo->dwFileVersionLS) & 0xFFFF
            );

          AfxMessageBox(valStr);
        }
      }
      delete[] lpInfo;
    }
  }

  return 0;
}

*/



int Capture_screenshot(LPCTSTR szFile,int index)
{
  int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
  int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  HWND hDesktopWnd  = GetDesktopWindow();
  HDC hDesktopDC    = GetDC(hDesktopWnd);
  HDC hCaptureDC    = CreateCompatibleDC(hDesktopDC);
  HBITMAP hCaptureBitmap;

  if ( nScreenWidth  > APP_RESX ) nScreenWidth  = APP_RESX;
  if ( nScreenHeight > APP_RESY ) nScreenHeight = APP_RESY;

  if ( index>=0 ) // get only one camera
  {
    RECT area = { 0, 0, nScreenWidth, nScreenHeight };

//    if ( index == 8 ) // PC1 - A60
    if ( index>=0 && index<NUM_TLC && image_dsp[index] /*&& camera[index].device*/ && camera[index].text[3] )
    {
     /* area.left   =            1064;
      area.top    =             378;
      area.right  = area.left + 208;
      area.bottom = area.top  + 494; */
      area.top    = OFFS_COORDY + image_dsp[index]->area_dsp.top     - 2;
      area.bottom = OFFS_COORDY + camera[index].text[3]->area.bottom + 2;
      area.left   = OFFS_COORDX + image_dsp[index]->area_dsp.left    - 2;
      area.right  = OFFS_COORDX + image_dsp[index]->area_dsp.right   + 2;
    }
//    else ASSERT( 0 );

    hCaptureBitmap = CreateCompatibleBitmap( hDesktopDC , area.right-area.left , area.bottom-area.top );
    //hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC,hCaptureBitmap); 
    BitBlt(hCaptureDC,0,0,area.right-area.left,area.bottom-area.top, hDesktopDC,area.left,area.top, SRCCOPY|CAPTUREBLT); 

  }
  else // get fullscreen
  {
    hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC,hCaptureBitmap); 
    BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight, hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); 
  }

  CImage image;
  image.Attach( hCaptureBitmap ); 
  image.Save( szFile );

  ReleaseDC(hDesktopWnd,hDesktopDC);
  DeleteDC(hCaptureDC);
  DeleteObject(hCaptureBitmap);

  return 0;
}



//#include "windows.h" // should be less than and greater than instead of \"

int CaptureBMP(LPCTSTR szFile)
{
  // Source[1]
  HDC hdcScr, hdcMem;
  HBITMAP hbmScr;
  BITMAP bmp;
  int iXRes, iYRes;

  // Create a normal DC and a memory DC for the entire screen. The
  // normal DC provides a "snapshot" of the screen contents. The
  // memory DC keeps a copy of this "snapshot" in the associated
  // bitmap.
  hdcScr = CreateDC(L"Screenshot", NULL, NULL, NULL);
  hdcMem = CreateCompatibleDC(hdcScr);

  iXRes = GetDeviceCaps(hdcScr, HORZRES);
  iYRes = GetDeviceCaps(hdcScr, VERTRES);

  // Create a compatible bitmap for hdcScreen.
  hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);
  if (hbmScr == 0) return 0;

  // Select the bitmaps into the compatible DC.
  if (!SelectObject(hdcMem, hbmScr)) return 0;

  // Copy color data for the entire display into a
  // bitmap that is selected into a compatible DC.
  if (!StretchBlt(hdcMem,
    0, 0, iXRes, iYRes,
    hdcScr,
    0, 0, iXRes, iYRes,
    SRCCOPY))

    return 0;

  // Source[2]
  PBITMAPINFO pbmi;
  WORD cClrBits;

  // Retrieve the bitmap's color format, width, and height.
  if (!GetObject(hbmScr, sizeof(BITMAP), (LPSTR) &bmp)) return 0;

  // Convert the color format to a count of bits.
  cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
  if      (cClrBits ==  1)  cClrBits =  1;
  else if (cClrBits <=  4)  cClrBits =  4;
  else if (cClrBits <=  8)  cClrBits =  8;
  else if (cClrBits <= 16)  cClrBits = 16;
  else if (cClrBits <= 24)  cClrBits = 24;
  else                      cClrBits = 32;

  // Allocate memory for the BITMAPINFO structure. (This structure
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
  // data structures.)
  if (cClrBits != 24)
    pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
    sizeof(BITMAPINFOHEADER) +
    sizeof(RGBQUAD) * (1 << cClrBits));

  // There is no RGBQUAD array for the 24-bit-per-pixel format.
  else
    pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
    sizeof(BITMAPINFOHEADER));

  // Initialize the fields in the BITMAPINFO structure.
  pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pbmi->bmiHeader.biWidth = bmp.bmWidth;
  pbmi->bmiHeader.biHeight = bmp.bmHeight;
  pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
  pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
  if (cClrBits < 24)
    pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

  // If the bitmap is not compressed, set the BI_RGB flag.
  pbmi->bmiHeader.biCompression = BI_RGB;

  // Compute the number of bytes in the array of color
  // indices and store the result in biSizeImage.
  pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) / 8
    * pbmi->bmiHeader.biHeight * cClrBits;

  // Set biClrImportant to 0, indicating that all of the
  // device colors are important.
  pbmi->bmiHeader.biClrImportant = 0;

  HANDLE hf;                  // file handle
  BITMAPFILEHEADER hdr;       // bitmap file-header
  PBITMAPINFOHEADER pbih;     // bitmap info-header
  LPBYTE lpBits;              // memory pointer
  DWORD dwTotal;              // total count of bytes
  DWORD cb;                   // incremental count of bytes
  BYTE *hp;                   // byte pointer
  DWORD dwTmp;

  pbih = (PBITMAPINFOHEADER) pbmi;
  lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

  if (!lpBits) return 0;

  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hdcMem, hbmScr, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) return 0;

  // Create the .BMP file.
  hf = CreateFile(szFile,
    GENERIC_READ | GENERIC_WRITE,
    (DWORD) 0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    (HANDLE) NULL);
  if (hf == INVALID_HANDLE_VALUE) return 0;

  hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"

  // Compute the size of the entire file.
  hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
    pbih->biSize + pbih->biClrUsed *
    sizeof(RGBQUAD) + pbih->biSizeImage);
  hdr.bfReserved1 = 0;
  hdr.bfReserved2 = 0;

  // Compute the offset to the array of color indices.
  hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
    pbih->biSize + pbih->biClrUsed *
    sizeof (RGBQUAD);

  // Copy the BITMAPFILEHEADER into the .BMP file.
  if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL)) return 0;

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
    + pbih->biClrUsed * sizeof (RGBQUAD),
    (LPDWORD) &dwTmp, NULL))
    return 0;

  // Copy the array of color indices into the .BMP file.
  dwTotal = cb = pbih->biSizeImage;
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp, NULL)) return 0;

  // Close the .BMP file.
  if (!CloseHandle(hf)) return 0;

  // Free memory.
  GlobalFree((HGLOBAL)lpBits);
  ReleaseDC(0, hdcScr);
  ReleaseDC(0, hdcMem);

  return 1;
} 




/*
RGB to HSV & HSV to RGB

The Hue/Saturation/Value model was created by A. R. Smith in 1978. 
It is based on such intuitive color characteristics as tint, shade and tone 
(or family, purety and intensity). The coordinate system is cylindrical, 
and the colors are defined inside a hexcone. The hue value H runs from 0 to 360? 
The saturation S is the degree of strength or purity and is from 0 to 1. 
Purity is how much white is added to the color, so S=1 makes the purest color (no white). 
Brightness V also ranges from 0 to 1, where 0 is the black.
https://www.cs.rit.edu/~ncs/color/t_convert.html
*/

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) 
  {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = int( 0.5 + floor( h ) );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) 
  {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}





#undef  PT
#define PT(x,y) img_acq[x+(y)*camera[index].width]

int Test_process( int inp )
{
  TRACE("Test_process..\n");

//  return 0;
  CString savename;
  CString txt;
  CTimer timer;
  pView->Close_all_menus();
  UINT nimg_acq = 0;
//  int x, y;

  BYTE* img_acq = NULL;


    #ifdef TIVS_S40

  int count =  0;
  int index =  9;
  int ang   =  0;

  CString txt2;


  //goto test_cosmetic3;
  goto test_cosmetic2;


  goto test_flange;
  goto test_cosmetic1;
  goto test_cosmetic2;
  goto test_cosmetic3;
  goto test_particle2;



  

  // *********************  TEST PARTICLE2 / POLARIZERS-REFLECTION  *********************

  test_particle2:
  Change_view(3);
  count =  1;
  index =  9;
  //count = 16;
  //count = 5;



  wchar_t* fname[] = { L"C:\\IVS\\sequences\\Sequence[P1_A]_20170101_005609", // 0
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183509", 
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183509" };


  int nseq  = 0;
  int nseq1 = 0;
  int nseq2 = sizeof(fname)/sizeof(wchar_t*);


  particle_insp[index]->Prepare_images(20,fname[nseq]);

  //particle_insp[index]->modenew = 0 + 1024; // bottomlight + accumulate diff
  //if ( nseq>=31 && nseq<=48) particle_insp[index]->modenew |= 1; // backlight
  timer.reset();

  //particle_insp[index]->Process_sequence(0);

  //particle_insp[index]->modenew &= ~1024; // switch to single differences

  //if ( nseq>=31 && nseq<=48 ) particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
  //else                        particle_insp[index]->mode = PARTICLE_MODE_REFLECTION; // bottomlight
  particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
  particle_insp[index]->thr_old1 = 0;
  particle_insp[index]->Prepare_thr();

  particle_insp[index]->Create_GF(0.50);
  particle_insp[index]->Process_sequence(0);

  particle_insp[index]->Create_GF(0.75);
  particle_insp[index]->Process_sequence(0);

  particle_insp[index]->Create_GF(1.00);
  particle_insp[index]->Process_sequence(0);

  //image_dsp[index]->Draw_image();
  //camera[index].text[1]->SetText( particle_insp[index]->txt1, particle_insp[index]->col1 );





  // *********************  TEST COSMETIC 1 / SIDEWALL  *********************

  test_cosmetic1:
  Change_view(4);
  count =  1;
  index =  9;
  //count = 16;
  //count = 5;

  do
  {
    //txt.Format( L"C:\\IVS\\shield%d.png", count ); 
    txt.Format( L"C:\\IVS\\cosmetic%d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"cosmetic_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;



  test_cosmetic2:


  
  // TEST COSMETIC 2 / NEEDLE SHIELD
  Change_view(5);

  count =  1;
  index = 12;
  //ang   =  0;

//  CString txt2;

  //count = 16;
  //count = 5;

  do
  {
    //txt.Format( L"C:\\IVS\\shield%d.png", count ); 
    //txt.Format( L"C:\\IVS\\image[14]_%d.png", count ); 
    txt.Format( L"C:\\IVS\\needle%03d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( pfsidewall_insp[index] ) pfsidewall_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfsidewall_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfsidewall_insp[index]->txt1, pfsidewall_insp[index]->col1 );
      camera[index].text[2]->SetText( pfsidewall_insp[index]->txt2, pfsidewall_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  



  test_cosmetic3:

  // TEST COSMETIC 3 / LOW STOPPER
  Change_view(1);

  count =  1;
  index =  2;

  do
  {
    txt.Format( L"C:\\IVS\\cosm3_%02d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( pfsidewall_insp[index] ) pfsidewall_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfsidewall_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfsidewall_insp[index]->txt1, pfsidewall_insp[index]->col1 );
      camera[index].text[2]->SetText( pfsidewall_insp[index]->txt2, pfsidewall_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  





  test_flange:
  

  // TEST FLANGE TCA
    // test cosmetic CM_B
    //index = 7;
    //if ( !freezemode_on ) Change_mode(1);
    //Change_view(3);
    //index = 1;
  Change_view(1);

  count = 15;
  index = 0;
  ang   = 0;
  //CString txt2;


  //result = new Results_manager( L"flange_data.csv", 20, 360 );

  //txt.Format( L"C:\\IVS\\pfstopper_%02d_%03d", count, ang );
  //result->Create_csv( L"Flange data results", TRUE ); // create with header




  //result->Addrow_csv( L"Flange data results\r\n" );



  //count = 16;
  //count = 5;

  do
  {
  
    //if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
    


    //txt.Format( L"D:\\Progvis\\S40 - PFS inspection\\sample images\\%s.png", fname[count] ); 
    //txt.Format( L"D:\\Progvis\\S40 - PFS inspection\\sample4\\flange_%03d.png", count ); 
    txt.Format( L"C:\\IVS\\Flange%02d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    //if ( pfstopper_insp[index] ) pfstopper_insp[index]->Process_image( image_dsp[index]->ptr_buf, ang );
	////Pour added on 20201130
	//Mat mat_flange;
	//image_dsp[index]->buf2Mat(image_dsp[index]->ptr_buf, mat_flange );
	//if ( flange_insp[index] ) flange_insp[index]->Process_image( mat_flange, ang );
	////Pour added on 20201130
    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfstopper_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfstopper_insp[index]->txt1, pfstopper_insp[index]->col1 );
      camera[index].text[2]->SetText( pfstopper_insp[index]->txt2, pfstopper_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    if ( count>38 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );




  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  

  #endif // TIVS_S40


      #ifdef TIVS_S60

  int count =  0;
  int index =  9;
  int ang   =  0;

  CString txt2;


  //goto test_cosmetic3;
  goto test_cosmetic2;


  goto test_flange;
  goto test_cosmetic1;
  goto test_cosmetic2;
  goto test_cosmetic3;
  goto test_particle2;



  

  // *********************  TEST PARTICLE2 / POLARIZERS-REFLECTION  *********************

  test_particle2:
  Change_view(3);
  count =  1;
  index =  9;
  //count = 16;
  //count = 5;



  wchar_t* fname[] = { L"C:\\IVS\\sequences\\Sequence[P1_A]_20170101_005609", // 0
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183509", 
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183509" };


  int nseq  = 0;
  int nseq1 = 0;
  int nseq2 = sizeof(fname)/sizeof(wchar_t*);


  particle_insp[index]->Prepare_images(20,fname[nseq]);

  //particle_insp[index]->modenew = 0 + 1024; // bottomlight + accumulate diff
  //if ( nseq>=31 && nseq<=48) particle_insp[index]->modenew |= 1; // backlight
  timer.reset();

  //particle_insp[index]->Process_sequence(0);

  //particle_insp[index]->modenew &= ~1024; // switch to single differences

  //if ( nseq>=31 && nseq<=48 ) particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
  //else                        particle_insp[index]->mode = PARTICLE_MODE_REFLECTION; // bottomlight
  particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
  particle_insp[index]->thr_old1 = 0;
  particle_insp[index]->Prepare_thr();

  particle_insp[index]->Create_GF(0.50);
  particle_insp[index]->Process_sequence(0);

  particle_insp[index]->Create_GF(0.75);
  particle_insp[index]->Process_sequence(0);

  particle_insp[index]->Create_GF(1.00);
  particle_insp[index]->Process_sequence(0);

  //image_dsp[index]->Draw_image();
  //camera[index].text[1]->SetText( particle_insp[index]->txt1, particle_insp[index]->col1 );





  // *********************  TEST COSMETIC 1 / SIDEWALL  *********************

  test_cosmetic1:
  Change_view(4);
  count =  1;
  index =  9;
  //count = 16;
  //count = 5;

  do
  {
    //txt.Format( L"C:\\IVS\\shield%d.png", count ); 
    txt.Format( L"C:\\IVS\\cosmetic%d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"cosmetic_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;



  test_cosmetic2:


  
  // TEST COSMETIC 2 / NEEDLE SHIELD
  Change_view(5);

  count =  1;
  index = 12;
  //ang   =  0;

//  CString txt2;

  //count = 16;
  //count = 5;

  do
  {
    //txt.Format( L"C:\\IVS\\shield%d.png", count ); 
    //txt.Format( L"C:\\IVS\\image[14]_%d.png", count ); 
    txt.Format( L"C:\\IVS\\needle%03d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( pfsidewall_insp[index] ) pfsidewall_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfsidewall_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfsidewall_insp[index]->txt1, pfsidewall_insp[index]->col1 );
      camera[index].text[2]->SetText( pfsidewall_insp[index]->txt2, pfsidewall_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  



  test_cosmetic3:

  // TEST COSMETIC 3 / LOW STOPPER
  Change_view(1);

  count =  1;
  index =  2;

  do
  {
    txt.Format( L"C:\\IVS\\cosm3_%02d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( pfsidewall_insp[index] ) pfsidewall_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfsidewall_insp%02d", count );
    image_dsp[index]->Save_images( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfsidewall_insp[index]->txt1, pfsidewall_insp[index]->col1 );
      camera[index].text[2]->SetText( pfsidewall_insp[index]->txt2, pfsidewall_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    //if ( count>8 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  





  test_flange:
  

  // TEST FLANGE TCA
    // test cosmetic CM_B
    //index = 7;
    //if ( !freezemode_on ) Change_mode(1);
    //Change_view(3);
    //index = 1;
  Change_view(1);

  count = 15;
  index = 0;
  ang   = 0;
  //CString txt2;


  //result = new Results_manager( L"flange_data.csv", 20, 360 );

  //txt.Format( L"C:\\IVS\\pfstopper_%02d_%03d", count, ang );
  //result->Create_csv( L"Flange data results", TRUE ); // create with header




  //result->Addrow_csv( L"Flange data results\r\n" );



  //count = 16;
  //count = 5;

  do
  {
  
    //if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
    


    //txt.Format( L"D:\\Progvis\\S40 - PFS inspection\\sample images\\%s.png", fname[count] ); 
    //txt.Format( L"D:\\Progvis\\S40 - PFS inspection\\sample4\\flange_%03d.png", count ); 
    txt.Format( L"C:\\IVS\\Flange%02d.png", count ); 
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
   // SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( pfstopper_insp[index] ) pfstopper_insp[index]->Process_image( image_dsp[index]->ptr_buf, ang );

    TRACE("Image_%3d processing time %d ms\n", count, timer.elapsed_ms() );

    image_dsp[index]->Draw_image();

    txt.Format( L"pfstopper_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( pfstopper_insp[index]->txt1, pfstopper_insp[index]->col1 );
      camera[index].text[2]->SetText( pfstopper_insp[index]->txt2, pfstopper_insp[index]->col2 );
    }
    txt2.Format( L"<%s>\r\nProcessing time %d us", txt, timer.elapsed_us() );
    //camera[index].text[3]->SetText( txt, 0 );

    count++;

    if ( count>38 ) break;

  } while ( Message_box( txt2+L"\r\nContinue?", MBOX_YESNO ) == IDYES );




  if ( result )
  { 
    result->Save_csv();
    delete result;
    result = NULL;
  }

  return 0;
  

  #endif // TIVS_S60



//// Test cosmetic_cosmo
//
//  #if defined NEWCOSMETIC_COMBO
//
//  //int index = 7; // cosm_d
//  index = 4; // cosm_a
//  Change_view( 2 );
//
//
//  #ifdef TIVS_C40
//
//    // test cosmetic CM_B
//    index = 7;
//    if ( !freezemode_on ) Change_mode(1);
//    Change_view(3);
//
//    //index = 1;
//    //Change_view(1);
//
//  #endif
//
//
//  //wchar_t* fname[] = { L"image[6]_20181207_161421_buf", // FD
//  //                     L"image[6]_20181207_141921_buf", // LQ
//  //                     L"image[6]_20181207_141830_buf", // LQ
//  //                     L"image[6]_20181207_141716_buf", // LQ
//  //                     L"image[6]_20181205_094842_buf" /*,
//  //                     L"image[6]_20170321_162611_buf",
//  //                     L"image[7]_20170321_162611_buf",
//  //                     L"image[4]_20170321_144725_buf",
//  //                     L"image[5]_20170321_162610_buf",
//  //                     L"image[6]_20170321_144725_buf",
//  //                     L"image[7]_20170321_144725_buf" */ };
//
//
//
//  //int count = 0;
//
//
//  count = 1;
//  
//  do
//  {
//  
//    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
//    
////    txt.Format( L"C:\\IVS\\Huapont\\%s.png", fname[count] ); 
////    txt.Format( L"D:\\Progvis\\C40\\Biocad images\\6R\\%s.png", fname[count] ); 
////    txt.Format( L"D:\\Progvis\\C40\\images for cosmetic\\%s.png", fname[count] ); 
//    txt.Format( L"C:\\IVS\\%s.png", fname[count] ); 
//
//
//    image_dsp[index]->Load_image( txt.GetBuffer(0) );
//    SCALE_IMG( index, image_dsp[index]->ptr_buf );
//
//    timer.reset();
//
//    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );
//
//    image_dsp[index]->Draw_image();
//
//    txt.Format( L"cosmetic_%02d", count );
//    image_dsp[index]->Save_images ( txt.GetBuffer() );
//
//    if ( image_dsp[index] )
//    {
//      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
//      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
//    }
//    txt.Format( L"Processing time %d us", timer.elapsed_us() );
//    camera[index].text[3]->SetText( txt, 0 );
//
//    count++;
//  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );
//
//  return 0;
//
//
//  #endif // defined NEWCOSMETIC_COMBO
  







  /*
  // Test logger
  if ( logger )
  {
    for ( int i=0 ; i<10 ; i++ )
    {
      txt.Format( L"log addition %d-%d-%d", i, 1000-i, i*i );
      logger->Add_string( &txt );
    }
    //Show_log(0);
    return 0;
  }
  */






 // drcolor_insp[7]->Analyze_area();

  //img_acq = (BYTE*) _aligned_malloc( image_dsp[index]->buf_size , 8 ); 
  //ASSERT( img_acq && AfxIsValidAddress( img_acq, image_dsp[index]->buf_size ) );

//  txt.Format( L"C:\\IVS\\image[0]_20131006_114833_buf.png" );  // test filling level
//  txt.Format( L"C:\\IVS\\cap images\\image[0]_20131028_142806_buf.png" );  // test filling level


  //CString test(L'X',40);
  //camera[0].text[1]->SetWindowTextW( test );
  //return 0;


//  CaptureBMP( L"screenshot" );
//  Capture_screenshot( L"screenshot.png" );

//  Read_version();
//  return 0;


  //#define USEDB


  /*

  Effective Exception Handling in Visual C++
  sembra interessante
  http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus



    int vara = 10;
    int varb = 0;
    int varc = 0;
  //TRY
  try
 // __try
  {
    varc = vara / varb;

/*    int arrayint[10];
    ZeroMemory( arrayint, sizeof(arrayint) );
    arrayint[11] = 11111;
    Message_box(L"Eseguito, arrayint[11]=11111"); */
/*  }
//  CATCH ( CException* e )
//  CATCH ( CException, e )
//  catch( ... )
//  __except
//  __except( puts("in filter"), EXCEPTION_EXECUTE_HANDLER ){
/*  {
     // Handle the exception here.
  
     // "e" contains information about the exception.
    Message_box(L"EXCEPTION");
  
    throw;
    //e->Delete();
  }
 // __finally

  //END_CATCH
*/


    CString text;
    for ( int i=1000 ; i<2000 ; i++ )
    {
      text.Format(L"This is new line of log file number %04d", i );
      logger->Add_string( &text ); 
    }
    return 0;





  #ifdef USEDB
    // #include "afxdb.h"
    CDatabase database;

   // database.OpenEx( NULL );
   //    database.OpenEx( L"DSN=C:\\IVS\\database_001" ); //, CDatabase::noOdbcDialog );

    CString conString;
    CString dbPath = L"c:\\ivs\\database_001.mdb";   
//    conString.Format( L"ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s", dbPath );
    conString.Format( L"DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s", dbPath );
    database.OpenEx( conString , CDatabase::noOdbcDialog );  // Fails to open the connection
    TRACE( L"database name: %s\n", database.GetDatabaseName() );
    TRACE( L"database connect: %s\n", database.GetConnect() );
    TRACE( "database can be updated: %d\n", database.CanUpdate() );
    TRACE( "database is open: %d\n", database.IsOpen() );
    database.Close();
    return 0;
  #endif

//  void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )

    /*
  for ( int ch=0 ; ch<3 ; ch++ )
  {
    if ( ch==0 ) TRACE("RED\n");
    if ( ch==1 ) TRACE("GREEN\n");
    if ( ch==2 ) TRACE("BLUE\n");
    for ( int i=0 ; i<360 ; i++ )
    {
      float r,g,b;
      float s = 1.0;
      float v = 1.0;
      float h = i;

      HSVtoRGB( &r, &g, &b, h, s, v );

      int ri = int( 0.5 + 255.0*r );
      int gi = int( 0.5 + 255.0*g );
      int bi = int( 0.5 + 255.0*b );

      //    TRACE("%d,%4.3f,%4.3f,%4.3f\n",i,r,g,b);
      //    TRACE("%3d,%3d,%3d,%3d\n",i,ri,gi,bi);
      //    TRACE("%d,%d,%d\n",ri,gi,bi);
      if ( ch==0 ) TRACE("%d ",ri);
      if ( ch==1 ) TRACE("%d ",gi);
      if ( ch==2 ) TRACE("%d ",bi);

    }
    TRACE("\n");
  }
  */


  #if defined TIVS_A60 || defined TIVS_A40



    /*
  int index = 7; // cosm_d
  index = 4; // cosm_a
  Change_view( 2 );

  wchar_t* fname[] = { L"image[4]_20170321_162610_buf",  //  L"image[4]_20170321_085237_buf", 
                       L"image[5]_20170321_162610_buf",
                       L"image[6]_20170321_162611_buf",
                       L"image[7]_20170321_162611_buf",
                       L"image[4]_20170321_144725_buf",
                       L"image[5]_20170321_162610_buf",
                       L"image[6]_20170321_144725_buf",
                       L"image[7]_20170321_144725_buf"  };

  int count = 0;

  
  do
  {
  
    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
    //txt.Format( L"C:\\IVS\\Huapont\\bad\\1#\\%s.png", fname[count] ); 
    txt.Format( L"C:\\IVS\\Huapont\\%s.png", fname[count] ); 
    //txt.Format( L"C:\\IVS\\056\\image[7]_20151009_084033_buf.png" ); 


    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();

    txt.Format( L"cosmetic_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  return 0;
  */



  /* new particle - pc2 */

    
  #ifdef NEW_PARTICLE

  //int index = 5; // PC2_F
  Change_view( 1 );
  //Pour removed on 20200927
/*
  if ( IS_PC1 )
  {
    Change_view( 2 );
    index = 8; // PC1_F
  }
  */
//  ASSERT( particle_insp[index] );
  //Pour removed on 20200927
/*
  particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION;
  particle_insp[index]->Prepare_thr();
  particle_insp[index]->mode = 0;
  particle_insp[index]->Prepare_thr();
  return 0;
  */

//  particle_insp[index]->Prepare(0);
    

  //wchar_t* fname[] = { L"C:\\sequences\\Sequence[P2_A]_20160726_183509", // 0
  //                     L"C:\\sequences\\Sequence[P2_B]_20160726_183509", 
  //                     L"C:\\sequences\\Sequence[P2_C]_20160726_183509",
  //                     L"C:\\sequences\\Sequence[P2_D]_20160726_183509",
  //                     L"C:\\sequences\\Sequence[P2_E]_20160726_183509",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160726_183509",

  //                     L"C:\\sequences\\Sequence[P2_A]_20160726_183520", // 6
  //                     L"C:\\sequences\\Sequence[P2_B]_20160726_183520",
  //                     L"C:\\sequences\\Sequence[P2_C]_20160726_183520",
  //                     L"C:\\sequences\\Sequence[P2_D]_20160726_183520",
  //                     L"C:\\sequences\\Sequence[P2_E]_20160726_183520",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160726_183520",

  //                     L"C:\\sequences\\Sequence[P2_A]_20160726_183532", // 12
  //                     L"C:\\sequences\\Sequence[P2_B]_20160726_183532",
  //                     L"C:\\sequences\\Sequence[P2_C]_20160726_183532",
  //                     L"C:\\sequences\\Sequence[P2_D]_20160726_183532",
  //                     L"C:\\sequences\\Sequence[P2_E]_20160726_183532",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160726_183532",

  //                     L"C:\\sequences\\Sequence[P2_A]_20160726_183544", // 18
  //                     L"C:\\sequences\\Sequence[P2_B]_20160726_183544",
  //                     L"C:\\sequences\\Sequence[P2_C]_20160726_183544",
  //                     L"C:\\sequences\\Sequence[P2_D]_20160726_183544",
  //                     L"C:\\sequences\\Sequence[P2_E]_20160726_183544",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160726_183544",

  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171106", // 24
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171116",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171126", // 26
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171137",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171147", // 28
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171157",
  //                     L"C:\\sequences\\Sequence[P2_F]_20160810_171207" }; // 30       

  ////Pour marked on 20200921
  //int nseq  = 0;
  //int nseq1 = 0;
  //int nseq2 = sizeof(fname)/sizeof(wchar_t*);

//  nseq1  =   100;
//  nseq2  =    88; 
//  nseq2  = nseq1;

  //do
  //{


  //} while ( nseq<=nseq2 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  #endif  // new_partic
  //return 0;

/*
    

  wchar_t* fname[] = { L"C:\\sequences\\Sequence[P2_A]_20160726_183509", // 0
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183509", 
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183509",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183509",

                       L"C:\\sequences\\Sequence[P2_A]_20160726_183520", // 6
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183520",
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183520",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183520",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183520",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183520",

                       L"C:\\sequences\\Sequence[P2_A]_20160726_183532", // 12
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183532",
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183532",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183532",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183532",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183532",

                       L"C:\\sequences\\Sequence[P2_A]_20160726_183544", // 18
                       L"C:\\sequences\\Sequence[P2_B]_20160726_183544",
                       L"C:\\sequences\\Sequence[P2_C]_20160726_183544",
                       L"C:\\sequences\\Sequence[P2_D]_20160726_183544",
                       L"C:\\sequences\\Sequence[P2_E]_20160726_183544",
                       L"C:\\sequences\\Sequence[P2_F]_20160726_183544",

                       L"C:\\sequences\\Sequence[P2_F]_20160810_171106", // 24
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171116",
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171126", // 26
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171137",
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171147", // 28
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171157",
                       L"C:\\sequences\\Sequence[P2_F]_20160810_171207", // 30                      

                       /* Backlight */


/*
                       L"C:\\sequences\\Sequence[P1_A]_20160815_182038", // 31
                       L"C:\\sequences\\Sequence[P1_A]_20160815_182049", // 32
                       L"C:\\sequences\\Sequence[P1_A]_20160815_182059", // 33
                       L"C:\\sequences\\Sequence[P1_B]_20160815_182038", // 34 - vetro unito sul fondo
                       L"C:\\sequences\\Sequence[P1_B]_20160815_182049", // 35
                       L"C:\\sequences\\Sequence[P1_B]_20160815_182059", // 36
                       L"C:\\sequences\\Sequence[P1_C]_20160815_182038", // 37
                       L"C:\\sequences\\Sequence[P1_C]_20160815_182049", // 38
                       L"C:\\sequences\\Sequence[P1_C]_20160815_182059", // 39
                       L"C:\\sequences\\Sequence[P1_D]_20160815_182038", // 40
                       L"C:\\sequences\\Sequence[P1_D]_20160815_182049", // 41
                       L"C:\\sequences\\Sequence[P1_D]_20160815_182059", // 42
                       L"C:\\sequences\\Sequence[P1_E]_20160815_182038", // 43
                       L"C:\\sequences\\Sequence[P1_E]_20160815_182049", // 44
                       L"C:\\sequences\\Sequence[P1_E]_20160815_182059", // 45
                       L"C:\\sequences\\Sequence[P1_F]_20160815_182038", // 46
                       L"C:\\sequences\\Sequence[P1_F]_20160815_182049", // 47
                       L"C:\\sequences\\Sequence[P1_F]_20160815_182059", // 48

                       L"C:\\sequences\\Sequence[P2_A]_20160817_084205", // 49
                       L"C:\\sequences\\Sequence[P2_A]_20160818_092750", // 50 visibile solo in poche immagini
                       L"C:\\sequences\\Sequence[P2_D]_20160726_184140", // 51
                       L"C:\\sequences\\sequenza_truking",               // 52 img truking convertite
                       
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\131336_gauss10",                 // 53 la 54 filtrata con gaussiana 1.0
                       // GOOD SEQUENCES
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_A]_20160906_131336", // 54 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_A]_20160906_131553", // 55 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_B]_20160906_131314", // 56 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_C]_20160906_131444", // 57 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_C]_20160906_131509", // 58 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_D]_20160906_131330", // 59 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_D]_20160906_131515", // 60 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_D]_20160906_131538", // 61 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_D]_20160906_131541", // 62 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_F]_20160906_131450", // 63 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_F]_20160906_131503", // 64 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_F]_20160906_132327", // 65 buona no refl ma movimento
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\GOOD\\Sequence[P2_F]_20160906_132335", // 66 buona no refl ma movimento
                       // REJECT SEQUENCES
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_A]_20160906_133305",  // 67 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_B]_20160906_133300",  // 68 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_C]_20160906_133244",  // 69 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_C]_20160906_133315",  // 70 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_D]_20160906_133230",  // 71 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_D]_20160906_133251",  // 72 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_E]_20160906_133207",  // 73 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_E]_20160906_133224",  // 74 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_E]_20160906_133240",  // 75 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_E]_20160906_133257",  // 76 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_E]_20160906_133310",  // 77 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_F]_20160906_133214",  // 78 BAD
                       L"C:\\sequences\\latest double lighting movement   SINGLE WITH ANTI-REFLECTIVE CLOTH\\BAD\\Sequence[P2_F]_20160906_133235",  // 79 BAD
                       // New sequences by Allen - October 2016
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101457",  // 80 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101507",  // 81 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101517",  // 82 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101528",  // 83 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101538",  // 84 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_101548",  // 85 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 20\\Detected by camera but not marked by SW\\Sequence[P3_F]_20161005_101456",  // 86 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 5\\Detected by camera but not marked by SW\\Sequence[P3_E]_20161005_104958",   // 87 BAD
                       L"C:\\sequences\\new sequences oct\\Classified\\Sensitivity 5\\Detected by camera but not marked by SW\\Sequence[P3_F]_20161005_104947",   // 88 BAD

                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_A]_20161018_191413",                                                          // 89 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_A]_20161018_191556",                                                          // 90 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_A]_20161018_191934",                                                          // 91 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_A]_20161018_194501",                                                          // 92 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_B]_20161018_191413",                                                          // 93 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_B]_20161018_191556",                                                          // 94 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_B]_20161018_191934",                                                          // 95 for testing problem during debug insp
                       L"C:\\sequences\\problems with debug\\BIG glass\\Sequence[P2_C]_20161018_191934",                                                          // 96 for testing problem during debug insp

                       L"C:\\sequences\\20161024 5ml\\backlight\\Sequence[P1_B]_20161024_142653",                                                                 // 97 backlight difficult to detect
                       L"C:\\sequences\\backlight\\Sequence[P1_E]_20161025_100344",                                                                               // 98 backlight visible, dirt backgrnd amplified
                       L"C:\\IVS\\Longhai Yunnan\\6 omitted product",                                                                                              // 99 bottomlight from Yunnan Longhai, red line on bottom
                       L"C:\\IVS\\Longhai Yunnan\\6 omitted product (2)"                                                                                          // 100 bottomlight from Yunnan Longhai, red line on bottom
                                                       };
                                                       


//  int count = 0;
  int nseq  = 0;
//  int totseq = sizeof(fname)/sizeof(wchar_t*);

  int nseq1 = 0;
  int nseq2 = sizeof(fname)/sizeof(wchar_t*);

//  totseq = 1;

  nseq1  =   100;
  nseq2  =    88; 
  nseq2  = nseq1;

/*  nseq1  =    98;
  nseq2  =    97; 
  nseq2  =    nseq1; */


/*
  #ifdef NEW_PARTICLE
    //particle_insp[index]->modenew =    0; // bottomlight
    //particle_insp[index]->modenew = 1024; // bottomlight + accumulate dff
//    particle_insp[index]->Create_GF(1.0);
//    particle_insp[index]->Create_GF(1.5);
//    particle_insp[index]->Create_GF( 0.84089642 );
//    particle_insp[index]->Create_GF( 1.5 );

    particle_insp[index]->proctime = 0;
  #endif


  nseq = nseq1;

  do
  {

    timer.reset();


    #ifdef NEW_PARTICLE
      particle_insp[index]->Prepare_images(20,fname[nseq]);

      //particle_insp[index]->modenew = 0 + 1024; // bottomlight + accumulate diff
      //if ( nseq>=31 && nseq<=48) particle_insp[index]->modenew |= 1; // backlight
      timer.reset();

      //particle_insp[index]->Process_sequence(0);
      
      //particle_insp[index]->modenew &= ~1024; // switch to single differences

      //if ( nseq>=31 && nseq<=48 ) particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
      //else                        particle_insp[index]->mode = PARTICLE_MODE_REFLECTION; // bottomlight
      particle_insp[index]->mode = PARTICLE_MODE_ADSORPTION; // backlight
      particle_insp[index]->thr_old1 = 0;
      particle_insp[index]->Prepare_thr();

      particle_insp[index]->Create_GF(0.50);
      particle_insp[index]->Process_sequence(0);

      particle_insp[index]->Create_GF(0.75);
      particle_insp[index]->Process_sequence(0);

      particle_insp[index]->Create_GF(1.00);
      particle_insp[index]->Process_sequence(0);

      //image_dsp[index]->Draw_image();
      //camera[index].text[1]->SetText( particle_insp[index]->txt1, particle_insp[index]->col1 );

    #endif


/*    if ( particle_insp[index]->part_err > particle_insp[index]->max_err ) // PARTICLE REJECT
    {
      txt.Format( string_842[lng], particle_insp[index]->part_err, particle_insp[index]->max_err );
      camera[index].text[1]->SetText( txt, TXT_DARKRED );
    }
    else      // PARTICLE GOOD
    {
      txt.Format( string_840[lng], particle_insp[index]->part_err, particle_insp[index]->max_err );
      camera[index].text[1]->SetText( txt, TXT_DARKGREEN );
    }  */

/*

    txt.Format( L"Seq %d - Proctm %d ms", nseq, timer.elapsed_ms() );
    camera[index].text[3]->SetText( txt, 0 );

    //if ( image_dsp[index]->visible ) image_dsp[index]->Draw_image();
    //image_dsp[index]->Draw_image();
    //Sleep( 10 );
    //pView->Invalidate(TRUE);
    //pView->OnPaint();
    pView->RedrawWindow();

    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
//      txt.Format( L"C:\\sequences\\results\\%s_%03d", fname[nseq]+13, nseq ); 

      // Extract and set newname to the folder name after sequences
      CString pathname = fname[nseq];
      int pos = pathname.ReverseFind(L'\\');

      CString newname;
      newname = pathname.Right( pathname.GetLength() - pos - 1 ); // fullname.GetLength() - pos );

      //CString path;
      //path = fullname.Left( pos ); // fullname.GetLength() - pos );

      //txt.Format( L"C:\\sequences\\results\\%s_%03d", newname, nseq );  // save in results subfolder
      txt.Format( L"%s\\result", pathname );  // save in original subfolder
      // ORIGINAL      txt.Format( L"C:\\sequences\\results\\%s_%03d", fname[nseq]+13, nseq ); 

      if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->Save_fulldsp( txt.GetBuffer(0) );

      txt.Format( L"%s\\result.png", pathname );  // save 
      Capture_screenshot( txt );

      txt.Format( L"%s\\result_index.png", pathname );  // save 
      Capture_screenshot( txt, index );

      txt.Format( L"C:\\sequences\\results\\result_%03d(%s).png", nseq, newname ); 
      Capture_screenshot( txt, index );

    #endif



    nseq++;

  } while ( nseq<=nseq2 );
//  } while ( nseq<=totseq && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );




  #ifdef NEW_PARTICLE
    //txt.Format( L"Proc time %3.2lf ms/frm", 0.001*particle_insp[index]->proctime/(particle_insp[index]->num_img*(nseq2-nseq1+1)) ); 
    txt.Format( L"Avg proc time %3.2lf ms", 0.001*particle_insp[index]->proctime/(nseq2-nseq1+1) ); 
    camera[index].text[2]->SetText( txt, 0 );
  
    particle_insp[index]->Terminate(0);
  #endif

  return 0;

  */


    /*
  int index = 2; // cap
  Change_view( 1 );
  ASSERT( tip_insp[index] );

  wchar_t* fname[] = { L"6#\\image[0]_20151010_163656_buf", 
                       L"6#\\image[1]_20151010_163656_buf", 
                       L"6#\\image[2]_20151010_163656_buf",
                       L"7#\\image[0]_20151010_163752_buf",
                       L"7#\\image[1]_20151010_163752_buf",
                       L"7#\\image[2]_20151010_163752_buf",
                       L"8#\\image[0]_20151010_163932_buf",
                       L"8#\\image[1]_20151010_163932_buf",
                       L"8#\\image[2]_20151010_163932_buf",
                       L"9#\\image[0]_20151010_164127_buf",
                       L"9#\\image[1]_20151010_164127_buf",
                       L"9#\\image[2]_20151010_164127_buf",
                       L"10#\\image[0]_20151010_164456_buf",
                       L"10#\\image[1]_20151010_164456_buf",
                       L"10#\\image[2]_20151010_164456_buf"   };

  int count = 0;

  
  do
  {
  
    txt.Format( L"C:\\IVS\\sanjing images 6-10\\%s.png", fname[count] ); 

    #ifdef NEW_TIP_V30   // new tip control developed for Kelun
      txt.Format( L"C:\\IVS\\Image_tip%03d.png", count+1 ); 
    #endif

    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();

    txt.Format( L"tip_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
      camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;

  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  return 0;
  */

  #endif // defined TIVS_A60 || defined TIVS_A40
  

  #ifdef TIVS_F40

  
  UINT index = 0;
//  ASSERT( tip_insp[index] );
//  Change_view( 1 );

//  index = 3;
//  ASSERT( bottom_insp[index] );
//  Change_view( 2 );

  /*
  int ptr = 0;
  for ( y=0 ; y<image_dsp[index]->dim_buf.cy ; y++ )
  {
    for ( x=0 ; x<image_dsp[index]->dim_buf.cx ; x++ )
    {
      image_dsp[index]->ptr_buf[ptr++] = 170; // R
      image_dsp[index]->ptr_buf[ptr++] = 100; // G
      image_dsp[index]->ptr_buf[ptr++] =  10; // B
    }
  }
  */

  
  /*
  index = 4; // waist layer
  Change_view( 2 );
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_24bpp.png" ); // 24bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_32bpp.png" ); // 32bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_4a.png" ); // 32bpp
  
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\image[5]_20140321_093953_buf.png" ); // 32bpp
  txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145619\\img_0000.png" ); // 32bpp

  int count = 0;


  do
  {
    if ( count < 10 ) txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145619\\img_%04d.png", count    ); // 32bpp
    else              txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145621\\img_%04d.png", count-10 ); // 32bpp

    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( waistlayer_insp[index] ) waistlayer_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();

    txt.Format( L"waist_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( waistlayer_insp[index] )
    {
      waistlayer_insp[index]->Final_result();
      camera[index].text[1]->SetText( waistlayer_insp[index]->txt1, waistlayer_insp[index]->col1 );
      camera[index].text[2]->SetText( waistlayer_insp[index]->txt2, waistlayer_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );
  
  return 0;
  */


  /*
  index = 11; // top cake - skinning
  Change_view( 3 );
  //txt.Format( L"C:\\IVS\\skinning\\image[11]_20160323_103627_buf.png" ); // 32bpp

  int count = 0;
  int count_end = 30;

  wchar_t* fname[] = { L"image[9]_20160323_103627_buf",
                       L"image[10]_20160323_103627_buf", 
                       L"image[11]_20160323_103627_buf",
                       L"image[9]_20160323_103555_buf",
                       L"image[11]_20160323_103555_buf",
                       L"image[10]_20160323_103555_buf",
                       L"image[10]_20160323_103402_buf"    };


  do
  {
    //if ( count < 10 ) txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145619\\img_%04d.png", count    ); // 32bpp
    //else              txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145621\\img_%04d.png", count-10 ); // 32bpp

//    txt.Format( L"C:\\IVS\\skinning\\%s.png", fname[count] );
    txt.Format( L"C:\\IVS\\skinning\\image[%02d].png", count );

    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( skinning_insp[index] ) skinning_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    image_dsp[index]->Draw_image();

    if ( skinning_insp[index] ) skinning_insp[index]->Final_result();

    txt.Format( L"image[%02d]", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( skinning_insp[index] )
    {
      camera[index].text[1]->SetText( skinning_insp[index]->txt1, skinning_insp[index]->col1 );
      camera[index].text[2]->SetText( skinning_insp[index]->txt2, skinning_insp[index]->col2 );
    }

    //txt.Format( L"%s\r\nContinue?",fname[count]);
    txt.Append(L"\r\nContinue?");
    count++;
  } while ( count<count_end && Message_box( txt , MBOX_YESNO ) == IDYES );
//  } while ( count<(sizeof(fname)/sizeof(wchar_t*)) && Message_box( txt, MBOX_YESNO ) == IDYES );

  return 0;
  */


  
  index = 6; // cosm_d
  Change_view( 3 );

/*  wchar_t* fname[] = { L"image[4]_20170321_085237_buf", 
                       L"image[7]_20151009_084033_buf", 
                       L"image[8]_20151009_084032_buf",
                       L"image[9]_20151009_084032_buf" }; */

  wchar_t* fname[] = { L"image[9]_20170416_140449_buf", 
                       L"image[8]_20170416_140449_buf", 
                       L"image[7]_20170416_140449_buf",
                       L"image[6]_20170416_140449_buf" };

  int count = 0;

  
  do
  {
  
    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
    //txt.Format( L"C:\\IVS\\Huapont\\bad\\1#\\%s.png", fname[count] ); 
    //txt.Format( L"C:\\IVS\\056\\image[7]_20151009_084033_buf.png" ); 
    txt.Format( L"C:\\IVS\\2014-017\\%s.png", fname[count] ); 



    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();

    txt.Format( L"cosmetic_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  return 0;

  




  /*
  index = 9; // cosmetic
  Change_view( 3 );

  wchar_t* fname[] = { L"image[6]_20151009_084032_buf", 
                       L"image[7]_20151009_084033_buf", 
                       L"image[8]_20151009_084032_buf",
                       L"image[9]_20151009_084032_buf" };

  int count = 0;

  
  do
  {
    //if ( count < 10 ) txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145619\\img_%04d.png", count    ); // 32bpp
    //else              txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145621\\img_%04d.png", count-10 ); // 32bpp
  
    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;
    txt.Format( L"C:\\IVS\\056\\%s.png", fname[count] ); 
    //txt.Format( L"C:\\IVS\\056\\image[7]_20151009_084033_buf.png" ); 


    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();

    txt.Format( L"cosmetic_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( image_dsp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  return 0;
 */

  /*
  
  index = 2; // new cap bosen
  Change_view( 1 );
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_24bpp.png" ); // 24bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_32bpp.png" ); // 32bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_4a.png" ); // 32bpp
  
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\image[5]_20140321_093953_buf.png" ); // 32bpp
  //txt.Format( L"C:\\IVS\\Xi'an Bosen\\4-N (2).png" ); // 32bpp

/*  wchar_t* fname[] = { L"1-N (2)", L"1-N (4)", L"1-N (6)", L"3-N (2)", 
                       L"3-N (5)", L"3-N (7)", L"4-N (2)", L"4-N (4)", 
                       L"4-N (6)", L"5-N (2)", L"5-N (4)", L"5-N (6)" }; */

/*  wchar_t* fname[] = { L"C:\\IVS\\BOSEN20151014\\15ml\\1-good\\image[0]_20151013_172629_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\1-good\\image[1]_20151013_172629_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\1-good\\image[2]_20151013_172629_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\1-bad\\image[0]_20151013_172918_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\1-bad\\image[1]_20151013_172918_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\1-bad\\image[2]_20151013_172918_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-good\\image[0]_20151013_172758_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-good\\image[1]_20151013_172758_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-good\\image[2]_20151013_172758_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-bad\\image[0]_20151013_173040_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-bad\\image[1]_20151013_173040_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\15ml\\2-bad\\image[2]_20151013_173040_buf.png",
       
       
       }; */



/*  wchar_t* fname[] = { L"C:\\IVS\\BOSEN20151014\\image[0]_20151021_143529_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\image[1]_20151021_143529_buf.png",
                       L"C:\\IVS\\BOSEN20151014\\image[2]_20151021_143529_buf.png" }; */
  
  /*
  
  wchar_t* fname[] = { L"C:\\IVS\\image[0].png",
                       L"C:\\IVS\\image[1].png",
                       L"C:\\IVS\\image[2].png" };

  int count = 0;


  do
  {
    //if ( count < 10 ) txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145619\\img_%04d.png", count    ); // 32bpp
    //else              txt.Format( L"C:\\IVS\\CAKEWAIST\\Sequence[T3_B]_20150918_145621\\img_%04d.png", count-10 ); // 32bpp
  
    //txt.Format( L"C:\\IVS\\Xi'an Bosen\\%s.png", fname[count] ); // 32bpp
    txt.Format( L"%s", fname[count] ); // 32bpp


    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();

    if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, 1  );
   
    image_dsp[index]->Draw_image();

    txt.Format( L"capbpsen_%02d", count );
    image_dsp[index]->Save_images ( txt.GetBuffer() );

    if ( tip_insp[index] ) // image_dsp[index] )
    {
      camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
      camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
    if ( count>=sizeof(fname)/sizeof(wchar_t*) ) break;

  } while ( count<20 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );

  return 0;
  
  */


  /*
  index = 5; // FO color
  Change_view( 2 );
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_24bpp.png" ); // 24bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_32bpp.png" ); // 32bpp
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_color_4a.png" ); // 32bpp
  
  //txt.Format( L"C:\\PROGIVS\\images\\ausia\\image[5]_20140321_093953_buf.png" ); // 32bpp
  txt.Format( L"C:\\PROGIVS\\images\\ausia\\cap_flaw.png" ); // 32bpp
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  SCALE_IMG( index, image_dsp[index]->ptr_buf );
  
  if ( focolor_insp[index] ) focolor_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );
  
  image_dsp[index]->Draw_image();
  image_dsp[index]->Save_images ( L"savecolor" );

  if ( focolor_insp[index] )
  {
    camera[index].text[1]->SetText( focolor_insp[index]->txt1, focolor_insp[index]->col1 );
    camera[index].text[2]->SetText( focolor_insp[index]->txt2, focolor_insp[index]->col2 );
  }
  txt.Format( L"Processing time %d us", timer.elapsed_us() );
  camera[index].text[3]->SetText( txt, 0 );
  return 0;
  */

  /*
  index = 9; // cosmetic
  Change_view( 3 );
  //txt.Format( L"C:\\PROGIVS\\Ausia\\images for sw processing\\122  hair in the cake  side-wall control\\1.png" ); // 32bpp
  txt.Format( L"C:\\PROGIVS\\Ausia\\images for sw processing\\line on the wall\\230\\image[7]_20140305_191101_buf.png" ); // 32bpp
  //txt.Format( L"C:\\IVS\\level_good.png" );
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  
  timer.reset();
  image_dsp[index]->enable_full_dsp();

  SCALE_IMG( index, image_dsp[index]->ptr_buf );
  
  if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf );
  
  image_dsp[index]->Draw_image();
  image_dsp[index]->Save_images ( L"cosmetic" );

  if ( cosmetic_insp[index] )
  {
    camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
    camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
  }
  txt.Format( L"Processing time %d us", timer.elapsed_us() );
  camera[index].text[3]->SetText( txt, 0 );
  return 0;
  */



  /*
  index = 3; // cosmetic sidewall
  Change_view( 1 );
  //txt.Format( L"C:\\PROGIVS\\Ausia\\images for sw processing\\122  hair in the cake  side-wall control\\1.png" ); // 32bpp
  txt.Format( L"C:\\PROGIVS\\Ausia\\Side and top new images\\SC A   194    high  volume.png" ); // 32bpp
  //txt.Format( L"C:\\IVS\\level_good.png" );
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  
  timer.reset();
  image_dsp[index]->enable_full_dsp();

  //FillMemory( image_dsp[index]->ptr_buf, image_dsp[index]->buf_size, 200 );

  SCALE_IMG( index, image_dsp[index]->ptr_buf );
  
  if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf );
  
  image_dsp[index]->Draw_image();
  image_dsp[index]->Save_images ( L"cosmetic" );

  if ( cosmetic_insp[index] )
  {
    camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
    camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
  }
  txt.Format( L"Processing time %d us", timer.elapsed_us() );
  camera[index].text[3]->SetText( txt, 0 );
  return 0;
  */




  /* single
  txt.Format( L"C:\\IVS\\cap_01.png" );  // cap
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
  image_dsp[index]->Draw_image( TRUE );
  */

  /*
  index = 4; // bottom B
  index = 3; // bottom A
  Change_view( 2 );
//  txt.Format( L"C:\\PROGIVS\\images\\ausia\\bottom_good-a.png" );
  txt.Format( L"C:\\IVS\\konrous\\image[4]_20141231_160611_buf.png" );
  txt.Format( L"C:\\IVS\\konrous\\image[4]_20150113_095743_buf.png" );
  txt.Format( L"C:\\IVS\\konrous\\image[3]_20150526_090800_buf.png" );
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if (  bottom_insp[index] )
  {
    bottom_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
//  CString savename;
//  savename.Format( L"C:\\IVS\\cap_%03d", goods[count] );
//  image_dsp[index]->Save_fulldsp( savename.GetBuffer(0) );
//    image_dsp[index]->Save_images ( savename.GetBuffer(0) );
//    #endif
    camera[index].text[1]->SetText( bottom_insp[index]->txt1, bottom_insp[index]->col1 );
    camera[index].text[2]->SetText( bottom_insp[index]->txt2, bottom_insp[index]->col2 );
    image_dsp[index]->Draw_image();
    image_dsp[index]->Save_fulldsp(L"waist");
          //Save_image( baseimg, dimx, dimy, "baseimg", PNG_FILE );

  }
  return 0;
  

  
  // tip inspection
/*
  index = 0;
  Change_view( 1 );
  image_dsp[index]->enable_full_dsp();
  txt.Format( L"C:\\IVS\\image[0]_20140507_192951_buf.png" );
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  //SCALE_IMG( index, image_dsp[index]->ptr_buf );
  if ( tip_insp[index] )
  {
    tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
    camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
    camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
  }
  savename.Format( L"C:\\IVS\\cap_test" );
  image_dsp[index]->Save_fulldsp( savename.GetBuffer(0) );
  image_dsp[index]->Save_images ( savename.GetBuffer(0) );

//    if (  bottom_insp[index] )  bottom_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );

  image_dsp[index]->Draw_image( TRUE );
  return 0;
*/




//    image_dsp[index]->Draw_image();
//    camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );

//  int count = 1;
//  count = 0;
//  int goods[] = { 48, 47, 46, 45, 41, 38, 37, 36, 35, 34 };
//  wchar_t *names[] = { L"sealing_defect_8", L"sealing_defect_7", L"sealing_defect_6", L"sealing_defect_5",
//                       L"sealing_defect_4", L"sealing_defect_3", L"sealing_defect_2", L"sealing_defect_1" };

  index = 0;
  Change_view( 1 );

  //do 
  {
    //txt.Format( L"C:\\IVS\\cap images\\cap_%03d.png", count++ );  // cap
    //txt.Format( L"C:\\IVS\\cap images\\cap_%03d.png", goods[count++] );  // cap
    //txt.Format( L"C:\\PROGIVS\\images\\cap\\cap_%03d.png", goods[count] );  // cap
    //txt.Format( L"C:\\PROGIVS\\images\\ausia\\%s.png", names[count] );  // cap
    //txt.Format( L"C:\\PROGIVS\\images\\ausia\\image[2]_20140213_112427_buf.png" );
    //txt.Format( L"C:\\PROGIVS\\images\\ausia\\bottom_good-a.png" );
    //txt.Format( L"C:\\PROGIVS\\images\\ausia\\bottom_good.tiff" );

    txt.Format( L"C:\\PROGIVS\\new cap\\image[2]_20140801_134254_buf.png" );
    txt.Format( L"C:\\PROGIVS\\new cap\\newcap_02.png" );
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
//    if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
//    if (  bottom_insp[index] )  bottom_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );

    if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
//    image_dsp[index]->Draw_image( TRUE );

//    #ifdef SAVE_FULL_PROC
//    for ( int i=0 ; i<tip_insp[index]->npt ; i++ ) image_dsp[index]->ptr_fulldsp[tip_insp[index]->ptx[i]+tip_insp[index]->pty[i]*tip_insp[index]->dimx] = 0xFFFF00FF;
    
//    CString savename;
    savename.Format( L"C:\\IVS\\cap_%03d", 1 );//goods[count] );
    image_dsp[index]->Save_fulldsp( savename.GetBuffer(0) );
    image_dsp[index]->Save_images ( savename.GetBuffer(0) );
//    #endif

//    camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
//    camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
//    camera[index].text[1]->SetText( bottom_insp[index]->txt1, bottom_insp[index]->col1 );
//    camera[index].text[2]->SetText( bottom_insp[index]->txt2, bottom_insp[index]->col2 );
    image_dsp[index]->Draw_image();

//    image_dsp[index]->Draw_image();
//    camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
//    camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
    count++;
  } //while ( count<sizeof(names)/sizeof(wchar_t*) && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );
//  } while ( count<sizeof(goods)/sizeof(int) && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );
//  } while ( count<sizeof(goods)/sizeof(int) && Message_box( txt.GetBuffer(0)/*L"\r\nContinue?"*/, MBOX_YESNO ) == IDYES );




  /* bottom insp
  UINT index = 3;
  ASSERT( bottom_insp[index] );
  Change_view( 2 );

  txt.Format( L"C:\\IVS\\image[4]_bottom.png" );  // bottom
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if ( bottom_insp[index] ) bottom_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
  image_dsp[index]->Draw_image( TRUE );

/*  Message_box( L"Completed", MBOX_INFO );

  txt.Format( L"C:\\IVS\\image[5]_bottom.png" );  // bottom
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if ( bottom_insp[index] ) bottom_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
  image_dsp[index]->Draw_image( TRUE );
  */
  #endif


  //SCALE_IMG( index, image_dsp[index]->ptr_buf );

  

  #ifdef TIVS_A40_COSMETICO
  
  UINT index = 7; // cosmetic
  Change_view( 2 );
  //txt.Format( L"C:\\PROGIVS\\Ausia\\images for sw processing\\122  hair in the cake  side-wall control\\1.png" ); // 32bpp
  //txt.Format( L"C:\\PROGIVS\\Ausia\\Side and top new images\\SC A   194    high  volume.png" ); // 32bpp
  //txt.Format( L"C:\\IVS\\level_good.png" );

  int count = 1;

  do
  {
    txt.Format( L"C:\\IVS\\cosm%02d.png",count );
    txt.Format( L"C:\\IVS\\newchanji\\cosm%02d.png",count );
    image_dsp[index]->Load_image( txt.GetBuffer(0) );

    image_dsp[index]->enable_full_dsp();

    timer.reset();
    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    timer.reset();
    if ( cosmetic_insp[index] ) cosmetic_insp[index]->Process_image( image_dsp[index]->ptr_buf );

    int elapsed = timer.elapsed_us();

    txt.Format( L"C:\\IVS\\cosm_elab%02d.png",count );
    image_dsp[index]->Save_images ( txt.GetBuffer(0) ); // L"cosmetic" );

    image_dsp[index]->Draw_image();

    if ( cosmetic_insp[index] )
    {
      camera[index].text[1]->SetText( cosmetic_insp[index]->txt1, cosmetic_insp[index]->col1 );
      camera[index].text[2]->SetText( cosmetic_insp[index]->txt2, cosmetic_insp[index]->col2 );
    }

    txt.Format( L"Processing time %d us", elapsed );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<=40 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );


  return 0;
  

  /*
  index = 7; // FO
  Change_view( 3 );
  txt.Format( L"C:\\IVS\\image[7]_20150522_080951_buf.png" ); // 32bpp
  txt.Format( L"C:\\IVS\\image[7]_20150527_083320_buf.png" ); // 32bpp
  txt.Format( L"C:\\IVS\\image[7]_20150523_091552_buf.png" ); // 32bpp

  txt.Format( L"C:\\IVS\\image[7]_20150609_190545_buf.png" ); // 32bpp

  count = 1;

  do 
  {

//    txt.Format( L"C:\\IVS\\lunangood%05d.png", count ); // 32bpp
    image_dsp[index]->Load_image( txt.GetBuffer(0) );

    timer.reset();
    image_dsp[index]->enable_full_dsp();

    SCALE_IMG( index, image_dsp[index]->ptr_buf );

    if ( oralfo_insp[index] ) oralfo_insp[index]->Process_image( image_dsp[index]->ptr_buf, 0 );

    image_dsp[index]->Draw_image();
    txt.Format( L"ProcessFO%05d", count );
//    image_dsp[index]->Save_images ( L"OralFO" );
    image_dsp[index]->Save_images ( txt.GetBuffer(0) );

    if ( oralfo_insp[index] )
    {
      camera[index].text[1]->SetText( oralfo_insp[index]->txt1, oralfo_insp[index]->col1 );
      camera[index].text[2]->SetText( oralfo_insp[index]->txt2, oralfo_insp[index]->col2 );
    }
    txt.Format( L"Processing time %d us", timer.elapsed_us() );
    camera[index].text[3]->SetText( txt, 0 );

    count++;
  } while ( count<=12 && Message_box( txt+L"\r\nContinue?", MBOX_YESNO ) == IDYES );


  return 0;
  */

/*  
  UINT index = 2; // tip
  Change_view( 1 );
  //txt.Format( L"C:\\IVS\\image[1]_20140311_134809_buf.png" ); // 32bpp
  //txt.Format( L"C:\\PROGIVS\\Pude - Shanxi\\test_01.png" ); // 32bpp
  //txt.Format( L"C:\\IVS\\CAP for Lunan Houpu\\backlight\\GOOD\\02\\T_A.png" ); // 32bpp
  txt.Format( L"C:\\IVS\\CAP for Lunan Houpu\\backlight\\GOOD\\02\\T_A.png" ); // 32bpp
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  
  timer.reset();
  image_dsp[index]->enable_full_dsp();

  SCALE_IMG( index, image_dsp[index]->ptr_buf );
  
  if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf );
  
  image_dsp[index]->Draw_image();
  image_dsp[index]->Save_images ( L"tip" );

  if ( tip_insp[index] )
  {
    camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
    camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
  }
  txt.Format( L"Processing time %d us", timer.elapsed_us() );
  camera[index].text[3]->SetText( txt, 0 );
  return 0;
  */






  /*

  UINT index = 0;

  // test particle
  if ( IS_PC1 )
  {
    index = 2; // tip
    Change_view( 1 );
  }

  if ( IS_PC2 )
  {
    index = 3;   // Part2
    Change_view( 1 );
  }

  int i=0;

  img_acq = (BYTE*) _aligned_malloc( image_dsp[index]->buf_size , 16 ); 
  ASSERT( img_acq && AfxIsValidAddress( img_acq, image_dsp[index]->buf_size ) );
  */
//  FillMemory( particle_insp[index]->reflections, image_dsp[index]->buf_size, 0 );

/* TEST ELABORAZIONE PARTICELLARE
  particle_insp[index]->min_dif = 0;
  particle_insp[index]->num_img = 5;
  particle_insp[index]->area->buffrect.top    =     4;
  particle_insp[index]->area->buffrect.bottom = 400-5;
  particle_insp[index]->area->buffrect.left   =     4;
  particle_insp[index]->area->buffrect.right  = 768-5;

  nimg_acq =  0;
  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  PT(104,100) = 100;
  particle_insp[index]->Process_image( img_acq, nimg_acq );

  nimg_acq = 1;
  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );

  PT(110,100) = 99;
  particle_insp[index]->Process_image( img_acq, nimg_acq );

  nimg_acq = 2;
  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  PT(114,100) = 98;
  particle_insp[index]->Process_image( img_acq, nimg_acq );

  nimg_acq = 3;
  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  PT(118,100) = 97;
  particle_insp[index]->Process_image( img_acq, nimg_acq );

  nimg_acq = 4;
  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  PT(122,100) = 96;
  particle_insp[index]->Process_image( img_acq, nimg_acq );


//  TRACE("prima immagine: tempo = %d us\n", timer.elapsed_us() );


*/




  /*
  for ( i=0 ; i<30 ; i++ )
  {
  
    txt.Format( L"C:\\PROGIVS\\Images\\Sequence_20131218_093910\\img_%04d.png", i );  // part2 - bottom light
    txt.Format( L"C:\\PROGIVS\\Images\\Sequence_20131218_172259\\img_%04d.png", i );  // part1 - back light
    image_dsp[index]->Load_image( txt.GetBuffer(0) );
    //if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );

    BYTE* img_acq = image_dsp[index]->ptr_buf;

    particle_insp[index]->Process_image( img_acq, i );


    if ( particle_insp[index]->part_err > particle_insp[index]->max_err ) // PARTICLE REJECT
    {
      txt.Format( string_842[lng], particle_insp[index]->part_err, particle_insp[index]->max_err );
      camera[index].text[1]->SetText( txt, TXT_DARKRED );
    }
    else      // PARTICLE GOOD
    {
      txt.Format( string_840[lng], particle_insp[index]->part_err, particle_insp[index]->max_err );
      camera[index].text[1]->SetText( txt, DARK_GREEN );
    }

 //   SCALE_IMG( index, img_acq );
 //   image_dsp[index]->Draw_image( TRUE );
 //   Sleep(100);
 //   Message_box( L"Completed", MBOX_INFO );
  }
  */

  // test tip
//  UINT index = 0;
//  ASSERT( tip_insp[index] );
//  Change_view( 1 );
 
/*
  txt.Format( L"C:\\IVS\\tip_ok1.png" );  
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
  camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
  camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
  image_dsp[index]->Draw_image( TRUE );
  */

/*  Message_box( L"Completed", MBOX_INFO );
  
  txt.Format( L"C:\\IVS\\sample_tip2.png" ); 
  image_dsp[index]->Load_image( txt.GetBuffer(0) );
  if ( tip_insp[index] ) tip_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );
  camera[index].text[1]->SetText( tip_insp[index]->txt1, tip_insp[index]->col1 );
  camera[index].text[2]->SetText( tip_insp[index]->txt2, tip_insp[index]->col2 );
  image_dsp[index]->Draw_image( TRUE );  */



  #endif // TIVS_A40

//  txt.Format( L"C:\\IVS\\image[0]_20131001_160106_buf.png" );  // test tip
//  txt.Format( L"C:\\IVS\\image[1]_20131001_160106_buf.png" );
//  txt.Format( L"C:\\IVS\\image[2]_20131001_160106_buf.png" );


  



//  ASSERT( fillingl_insp[index] );
//  if ( fillingl_insp[index] ) fillingl_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );


  //image_dsp[index]->Draw_image( TRUE );


//  ASSERT( fillingl_insp[index] );
//  if ( fillingl_insp[index] ) fillingl_insp[index]->Process_image( image_dsp[index]->ptr_buf, nimg_acq );

  

/*
  
  if ( img_acq ) _aligned_free( img_acq );
  return 0;


  // TEST PARTICLE


  ASSERT( particle_insp[index] );

//  BYTE* img_acq = (BYTE*) _aligned_malloc( image_dsp[index]->buf_size , 16 ); 
//  ASSERT( img_acq && AfxIsValidAddress( img_acq, image_dsp[index]->buf_size ) );

 // FillMemory( particle_insp[index]->reflections, image_dsp[index]->buf_size, 13 );

  //particle_insp[index]->min_dif = 24;

  FillMemory( img_acq, image_dsp[index]->buf_size, 20 );
  for ( y=100 ; y<=120 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 100;


  timer.reset();
  nimg_acq = 0;
  particle_insp[index]->Process_image( img_acq, nimg_acq );
  TRACE("prima immagine: tempo = %d us\n", timer.elapsed_us() );

  // Save_image( particle_insp[index]->baseimg, image_dsp[index]->dim_buf.cx, image_dsp[index]->dim_buf.cy, "baseimg", PNG_FILE );

  
  FillMemory( img_acq, image_dsp[index]->buf_size, 20 );
  for ( y=150 ; y<=170 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 100;

  nimg_acq = 1;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  FillMemory( img_acq, image_dsp[index]->buf_size, 20 );
  for ( y=180 ; y<=200 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 111;

  nimg_acq = 2;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  FillMemory( img_acq, image_dsp[index]->buf_size, 20 );
  for ( y=210 ; y<=230 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 122;

  nimg_acq = 3;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );

  
//  Save_image( particle_insp[index]->output, image_dsp[index]->dim_buf.cx, image_dsp[index]->dim_buf.cy, "first_sub", PNG_FILE );
  Save_buffer( particle_insp[index]->output, image_dsp[index]->dim_buf.cx, image_dsp[index]->dim_buf.cy, L"first_sub", PNG_FILE );

  /*

  // last
  nimg_acq = particle_insp[index]->num_img-1;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );

  */

  if ( img_acq ) _aligned_free( img_acq );
  return 0;

//  pView->Close_all_menus();
//  AfxGetMainWnd()->RedrawWindow();
//  pView->Invalidate();
//  return 0;









  /*

  // TEST FILLING LEVEL
  int ind = 0;

//  CTimer timer;
  timer.reset();

  //  TEST 
  char str[200] = "undefined";

  for ( int index=0 ; index<300 ; index++ )
  {
    txt.Format( L"C:\\PROGIVS\\Filling level\\20ml_3rd\\img_%04d.png", index );
  
    image_dsp[ind]->Load_image( txt.GetBuffer(0) );

    timer.reset();

    SCALE_IMG ( ind, image_dsp[ind]->ptr_buf );
    if ( filling_level[ind] ) filling_level[ind]->Process_image( image_dsp[ind]->ptr_buf );
//    image_dsp[ind]->Draw_image( TRUE );
    image_dsp[ind]->Draw_image();

    txt.Format( L"processing time %d", timer.elapsed_us() );
    camera[ind].text[1]->SetWindowTextW( txt );

    txt.Format( L"image %d loaded and shown", index );
    camera[ind].text[2]->SetWindowTextW( txt );


    while ( !F1_PUSHED )
    {
      if ( F2_PUSHED )
      {
        Message_box( L"Interrupted", MBOX_INFO );
        return 0;
      }
      Sleep(100);
    }
  }
  
  Message_box( L"Completed", MBOX_INFO );



/*  // ca 2.5" con sigma=1 - 18 con sigma=2
  FCObjImage   img_tmp;
  img_tmp.Create( DIMX_ACQ, DIMY_ACQ, 8 ); //8bit
  BYTE* ptr = img_tmp.GetMemStart();
  timer.reset();
  for ( int y=10 ; y<DIMY_ACQ-10 ; y++ )
    for ( int x=10 ; x<DIMX_ACQ-10 ; x++ )
    {
      double val = 0.0;
      for ( int i=0 ; i<gb_npt ; i++ ) val += gb_val[i]*image_dsp[1]->ptr_buf[x+y*DIMX_ACQ+gb_ofs[i]];
      ptr[x+y*DIMX_ACQ] = BYTE( val );
    }
  sprintf( str, "%d ms", timer.elapsed_ms() );
  Message_box( str );
  img_tmp.Save( L"gb.png" ); */


//  FillMemory( image_dsp[1]->ptr_buf, DIMX_ACQ*DIMY_ACQ, 100 );



  /*
  FillMemory( image_dsp[1]->ptr_buf, DIMX_ACQ*DIMY_ACQ, 10 );

  int xc = 150;
  int yc = 150;

  double diag   = 0.5*sqrt(2.0);
  double lumh   =  240;
  double luml   =   10;
  double raggio = 14.0; // raggio nominale in pixel


  for ( int y=100-10 ; y<=200+10 ; y++ )
//  for ( int y=147 ; y<=153 ; y++ )
  {
    for ( int x=100-10 ; x<=200+10 ; x++ )
//    for ( int x=147 ; x<=153 ; x++ )
    {
      // scale_cf[1]

      double dist   = sqrt(double(x-xc)*(x-xc)+double(y-yc)*(y-yc));

      if ( dist < raggio-diag )
      {
        image_dsp[1]->ptr_buf[x+y*DIMX_ACQ] = BYTE(lumh);
        //TRACE("(%3d,%3d) tutto interno - lumh\n", x, y);
      }
      else
      {
        if ( dist > raggio+diag )
        {
          image_dsp[1]->ptr_buf[x+y*DIMX_ACQ] = BYTE(luml);
          //TRACE("(%3d,%3d) tutto esterno - luml\n", x, y);
        }
        else // sfumato
        {
          int count = 0;
          for ( double y2=y-0.45 ; y2<=y+0.45 ; y2+=0.1 )
          {
            for ( double x2=x-0.45 ; x2<=x+0.45 ; x2+=0.1 )
            {
              double d2 = sqrt(double(x2-xc)*(x2-xc)+double(y2-yc)*(y2-yc));
              if ( d2<=raggio ) count++;
            }
          }

          //TRACE("(%3d,%3d) sfumato - count=%3d\n", x, y, count);
          image_dsp[1]->ptr_buf[x+y*DIMX_ACQ] = BYTE(luml + 0.01*count*(lumh-luml));

        }
      }


    }
  }

 // return 0;

  contour_interp( 1, image_dsp[1]->ptr_buf, 100, 100, 100, 100 );
  */



  /*
  char str[200];
  static int index = 5;
  //sprintf( str, "d:\\IVS\\anomala_%03d.png", index++ );
//  sprintf( str, "anomala_%03d.png", index++ );
/*  sprintf( str, "background[1].png" );
  image_dsp[1]->Load_image( str );
  SCALE_IMG ( 1, image_dsp[1]->ptr_buf );
  timer.reset();
  neck_analysis( image_dsp[1]->ptr_buf );
  image_dsp[1]->Draw_image( TRUE );*/

  /*
  sprintf( str, "anomala_%03d_buf.png", index );
  image_dsp[1]->Load_image( str );
  SCALE_IMG ( 1, image_dsp[1]->ptr_buf );
  timer.reset();
  neck_analysis( image_dsp[1]->ptr_buf );
  image_dsp[1]->Draw_image( TRUE );
  */


/*
  CTimer timer;
  timer.reset();

  //  TEST PUNTO P - ULTIMO

  char str[200] = "undefined";
  for ( int index=1 ; index<200 ; index++ )
  {
  
    image_dsp[0]->Load_image( "D:\\IVS\\p_point_001(0).png" );
    Get_diff( image_dsp[0]->ptr_buf, background[0], 1920*1080 );
    SCALE_IMG ( 0, image_dsp[0]->ptr_buf );
 //   image_dsp[0]->Draw_image( TRUE );

//    image_dsp[1]->Load_image( "D:\\IVS\\p_point_001(1).png" );
//    Get_diff( image_dsp[1]->ptr_buf, background[1], DIMX_ACQ*DIMY_ACQ ); // con sottrazione background
    image_dsp[1]->Load_image( "D:\\IVS\\to_test_003.png" ); // senza sottrazione background
    SCALE_IMG ( 1, image_dsp[1]->ptr_buf );
    image_dsp[1]->Draw_image( TRUE );

    timer.reset();

    //char recipe_name[] = "2j261353";
    //if ( SG_read_input_recipe( recipe_name ) ) TRACE("Error during reading of recipe <%s>\n", recipe_name );
    //if ( SG_prepare_controls() )               TRACE("Error controls preparation of recipe <%s>\n", recipe_name );


    int plate_height = int( 0.5 + 7466.6698 * hplate_measuring[0] ); // altezza simulata piattello
    plate_height_cur = &plate_height;

    TRACE("Measuring: lifting plate to %.1lf mm for top height measurement\n", hplate_measuring[0] );

    bottle_analysis   = TRUE;
    vision_meas_start = timer.elapsed_ms();

    bottle_neck_analysis( image_dsp[1]->ptr_buf );
    image_dsp[1]->Draw_image( TRUE );
  
    bottle_body_analysis( image_dsp[0]->ptr_buf );
    image_dsp[0]->Draw_image( TRUE );

    bottle_analysis   = FALSE;
    vision_meas_end   = timer.elapsed_ms();

    if ( F1_PUSHED ) break;

    break;
    
    //sprintf( str, "%d ms", timer.elapsed_ms() );
    //Message_box( str );
  }
  
  */

//  vertical_edge_gb( image_dsp[1]->ptr_buf, 1135 );
//  image_dsp[1]->Create_csv( "lumdata.csv", 1942, 730, 100, 74 );
//  contour_interp( image_dsp[1]->ptr_buf, 1943 );
//  contour_interp( image_dsp[1]->ptr_buf, 1800 );
//  contour_interp( image_dsp[1]->ptr_buf, 400 );
//  for ( int x=1130 ; x<1130+400 ; x++ ) vertical_edge_gb( image_dsp[1]->ptr_buf, x );
//  for ( int x=100 ; x<2100 ; x += 100 ) vertical_edge_gb( image_dsp[1]->ptr_buf, x );
//  for ( int x=100 ; x<2100 ; x += 100 ) vertical_edge_interp( image_dsp[1]->ptr_buf, x );


//#endif //Pour marked on 20200921

  return 0;
}


/*  for ( int y=769 ; y<=788 ; y++ )
    for ( int x=1134 ; x<=1155 ; x++ )
      image_dsp[1]->ptr_buf[x+DIMX_ACQ*y] = 100; 
  SCALE_IMG( 1, image_dsp[1]->ptr_buf ); */



int Prepare_palette( void )
{

  UINT i;

  // prepare and pre-assign gray palette
  for ( i=0 ; i<=255 ; i++ ) lut32_bw[i] = i | (i<<8) | (i<<16) | 0xFF000000; // keep alpha at 255
  lut32 = lut32_bw;

  // load LUT pseudocolor palette

  wchar_t msg[200+1];

  CopyMemory( lut32_pc, lut32_bw, sizeof(lut32_pc) ); 

	CFile pal_file;
	if ( !pal_file.Open( PALETTE_FILE, CFile::modeRead ) )
	{
		swprintf( msg, 200, L"Unable to find/open file <%s> to read pseudocolor palette", PALETTE_FILE );
		Message_box( msg, MBOX_ERROR );
		return( 100 ); // can't find required file
	}

	UINT dim = UINT(pal_file.GetLength());
	ASSERT( dim > 0 && dim < 4000 );

	if ( dim!=768 )
	{
		swprintf( msg, 200, L"Wrong file size for <%s> %d bytes instead of %d", PALETTE_FILE, dim, 768 );
		Message_box( msg, MBOX_ERROR );
		return( 101 ); // can't find required file
	}

  BYTE val;
	for ( i=0 ; i<=255 ; i++ ) // read red first
	{
    pal_file.Read( &val, 1 );
    lut32_pc[i] = 0xFF000000 | (val<<16); // set also alpha channel to 255
  }
	for ( i=0 ; i<=255 ; i++ ) // then green
	{
    pal_file.Read( &val, 1 );
    lut32_pc[i] |= val << 8;
  }
	for ( i=0 ; i<=255 ; i++ ) // then blue
	{
    pal_file.Read( &val, 1 );
    lut32_pc[i] |= val;
    //TRACE("LUT[%3d]=%3d,%3d,%3d\n",i,(lut32_pc[i]>>16)&255,(lut32_pc[i]>>8)&255,lut32_pc[i]&255);
  }

	TRACE( L"Prepare_palette() completed successfully\n" );

  /*
	char* data = (char*) malloc( dim );
	pal_file.Read( data, dim );

	int red, green, blue;

	j = 0;
	for ( i=0 ; i<=255 ; i++ )
	{
		int ret = sscanf( data+j, "%d %d %d", &red, &green, &blue );

		if ( ret != 3 )
		{
  		wchar_t msg[200];
  		swprintf( msg, L"Error reading RGB values <%d>", i );
	  	Message_box( msg, MBOX_ERROR );
			free( data );
			return( 200 );
		}
		
		ASSERT( 0 <= red   && red   <=255 );
		ASSERT( 0 <= green && green <=255 );
		ASSERT( 0 <= blue  && blue  <=255 );


  	lut32_pc[i] = blue | (green<<8) | (red<<16);

		while ( data[j]!=13 && j<dim ) j++;
		j++;
	}




  /*
	CFile pal_file;
	if ( !pal_file.Open( PALETTE_FILE, CFile::modeRead ) )
	{
		swprintf( msg, L"Unable to open file <%s> to read pseudocolor palette", PALETTE_FILE );
		Message_box( msg, MBOX_ERROR );
		return( 100 );
	}

	UINT dim = UINT(pal_file.GetLength());
	ASSERT( dim > 0 && dim < 4000 );

	char* data = (char*) malloc( dim );
	pal_file.Read( data, dim );

	int red, green, blue;

	j = 0;
	for ( i=0 ; i<=255 ; i++ )
	{
		int ret = sscanf( data+j, "%d %d %d", &red, &green, &blue );

		if ( ret != 3 )
		{
  		wchar_t msg[200];
  		swprintf( msg, L"Error reading RGB values <%d>", i );
	  	Message_box( msg, MBOX_ERROR );
			free( data );
			return( 200 );
		}
		
		ASSERT( 0 <= red   && red   <=255 );
		ASSERT( 0 <= green && green <=255 );
		ASSERT( 0 <= blue  && blue  <=255 );


  	lut32_pc[i] = blue | (green<<8) | (red<<16);

		while ( data[j]!=13 && j<dim ) j++;
		j++;
	}
  
  */


	return( 0 );

//  #undef PALETTE_FILE
}


/*
wchar_t* thousands( wchar_t* inp )
{


1994 fracture of the right femur 
surgery 
from 1996 full recovery 



  return L"ecco";
}
*/



/*
  UINT val;
  wchar_t* txt = NULL;
  UINT usec = 0;

  val = 1;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 12;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 123;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 1234;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 12345;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 123456;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 1234567;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 12345678;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 123456789;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );

  val = 1234567890;
  timer.reset();
  txt = thousands(val);
  usec = timer.elapsed_us();
  TRACE( L"Conv %d = <%s> (%d us)\n", val, txt, usec );
*/

wchar_t* thousands( UINT inp )
{
  static wchar_t txt[20];

  txt[18]  = L'0';
  txt[19]  =   0;
  if ( inp==0 ) return txt+18;

  UINT pos =  18;
  while ( inp>0 )
  {
    if ( pos==15 || pos==11 || pos==7 || pos==3 )
    {
      txt[pos] = L'.';
      pos--;
    }
    txt[pos] = L'0' + inp % 10;
    inp /= 10;
    pos--;
  }
  return txt+pos+1;
}



int Read_csv( CString& name, int nrow, int ncol, int* data, wchar_t* txt/*=NULL*/ ) // number of row / columns
{

  // 44     = ,
  // 13+10  = CR+LF
  // 48..57 = 0..9

  int dim = nrow * ncol * sizeof(int);
  ASSERT( AfxIsValidAddress( data, dim ) );

  // try to open and load file
 	CFile file;
  if ( !file.Open( name, CFile::modeRead ) )
	{
    if ( txt ) swprintf( txt, 200, L"Read_csv: Cannot find/open file <%s>\n", name );
    TRACE(L"Read_csv: Cannot find/open file <%s>\n", name );
    return( 19 );
 	}

  BOOL debug=FALSE;
  BYTE ch  = 0;
  int  val = 0;
  int  row = 0; // current row
  int  col = 0; // current columns

//  int  dimf = file.GetLength();

  int* temp = (int*) malloc( dim );
  ASSERT( temp && AfxIsValidAddress( temp, dim ) );
  ZeroMemory( temp, dim );

//  while ( file.GetPosition()<dimf )
  while ( file.GetPosition()<file.GetLength() )
  {
    if ( file.Read( &ch, 1 )!=1 )
    {
      if ( txt ) swprintf( txt, 200, L"Read_csv: Unable to read next char (%d/%d)\n", file.GetPosition(), int(file.GetLength()) );
      //if ( txt ) swprintf( txt, 200, L"Read_csv: Unable to read next char (%d/%d)\n", file.GetPosition(), file.GetLength() );
      //TRACE(L"Read_csv: Unable to read next char (%d/%d)\n", file.GetPosition(), file.GetLength() );
      TRACE(txt);
      goto error;
      //return(29);
    }

    if (debug) TRACE("%d,%d,%d\n",ch,int(file.GetPosition()),int(file.GetLength()));

    switch (ch)
    { 
      case 13: // CR
//        if ( file.GetPosition()<file.GetLength() && file.Read(&ch,1) && ch==10 )
        if ( file.GetPosition()<int(file.GetLength()) && file.Read(&ch,1) && ch==10 )
        {
          if ( col<ncol && row<nrow ) temp[col+row*ncol]=val;
          else
          {
            if ( txt ) swprintf( txt, 200, L"Read_csv: Error in number of rows/columns after CR+LF (%d/%d)\n",row,col );
            TRACE( L"Read_csv: Error in number of rows/columns after CR+LF (%d/%d)\n",row,col );
            goto error;
            //return(49);
          }
          if (debug) TRACE("CR+LF found %d read (%d,%d)\n", val, row, col );
          
          if ( file.GetPosition()<int(file.GetLength()) )
          {
            val = 0; // reset val
            col = 0;
          }
          else col++;
          row++;
        }
        else
        {
          if ( txt ) swprintf( txt, 200, L"Read_csv: Error in CR+LF sequence\n" );
          TRACE( L"Read_csv: Error in CR+LF sequence\n" );
          goto error;
          //return(39);
        }
      break;
      case 44: // ,
        if ( col<ncol && row<nrow ) temp[col+row*ncol]=val;
        else
        {
          if ( txt ) swprintf( txt, 200, L"Read_csv: Error in number of rows/columns after comma (%d/%d)\n",row,col );
          TRACE( L"Read_csv: Error in number of rows/columns after comma (%d/%d)\n",row,col );
          goto error;
          //return(59);
        }
        if (debug) TRACE("Comma found %d read (%d,%d)\n", val, row, col );
        val = 0; // reset val
        col++;
      break;
      case 48: val = val*10;     break; // 0
      case 49: val = val*10 + 1; break; // 1
      case 50: val = val*10 + 2; break; // 2
      case 51: val = val*10 + 3; break; // 3
      case 52: val = val*10 + 4; break;
      case 53: val = val*10 + 5; break;
      case 54: val = val*10 + 6; break;
      case 55: val = val*10 + 7; break;
      case 56: val = val*10 + 8; break;
      case 57: val = val*10 + 9; break;
      default:
        if ( txt ) swprintf( txt, 200, L"Read_csv: Unexpected character in file (%d)\n", ch );
        TRACE( L"Read_csv: Unexpected character in file (%d)\n", ch );
        goto error;
        //return(39);
    }
  }

  if ( col!=ncol || row!=nrow )
  {
    if ( txt ) swprintf( txt, 200, L"Unable to read required number of rows/colums, check file <%s> format\n", name );
    //TRACE( L"Read_csv: Unexpected character in file (%d)\n", ch );
    TRACE( txt );
    goto error;
  }

  // everything ok
  file.Close();
  CopyMemory(data,temp,dim);
  free(temp);
  return 0; // completed successfully

error:

  file.Close();
  free(temp);

  Message_box( txt, MBOX_ERROR );

  return 999; // some error

}




// function to load generic image from <file_name> and <type> into buffer <buf> dimx*dimy
int Load_buffer( BYTE* buff_in, int dimx, int dimy, wchar_t* file_name, int type/*=PNG_FILE*/, BOOL vflip/*=TRUE*/ )
{

  ASSERT( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );
  ASSERT( AfxIsValidString( file_name ) ); 
  ASSERT( dimx>0 && dimx<20000 );
  ASSERT( dimy>0 && dimy<20000 );

  if ( !buff_in || !AfxIsValidAddress( buff_in, dimx*dimy ) ) return 99; // skip if ptr_buf NULL

  if ( !PathFileExists( file_name ) )
  { 
    wchar_t msg[100+1];
    swprintf( msg, 100, L"Could not open file <%s>\r\nError n.%d", file_name, GetLastError());
    Message_box( msg, MBOX_ERROR);
    return 99; 
  }


  int x, y;
  FCObjImage   img_tmp;
  img_tmp.Create( dimx, dimy, 8 );
  img_tmp.Load ( file_name );

  TRACE(L"Image <%s> loaded into img_tmp - %dx%dx%d\n", file_name, img_tmp.Width(), img_tmp.Height(), img_tmp.ColorBits() );

  if ( (img_tmp.ColorBits()!=24 && img_tmp.ColorBits()!=32) || (img_tmp.Width()!=dimx) || (img_tmp.Height()!=dimy) )
  { 
    wchar_t msg[100+1];
    swprintf( msg, 100, L"Image <%s> has wrong dimensions/color depth (%d,%d,%d)", file_name, img_tmp.Width(), img_tmp.Height(), img_tmp.ColorBits() );
    Message_box( msg, MBOX_ERROR);
    return 999; 
  }

  if ( img_tmp.ColorBits() == 24 )
  {
    BYTE* loaded = img_tmp.GetMemStart();
    for ( y=0 ; y<dimy ; y++ ) 
      for ( x=0 ; x<dimx ; x++ ) buff_in[x+y*dimx] = loaded[3*x+(dimy-1-y)*3*dimx]&255;
  }

  if ( img_tmp.ColorBits() == 32 )
  {
    UINT* loaded = (UINT*) img_tmp.GetMemStart();
    for ( y=0 ; y<dimy ; y++ ) 
      for ( x=0 ; x<dimx ; x++ ) 
        buff_in[x+y*dimx] = loaded[x+(dimy-1-y)*dimx]&255;
  }

  return 0;
}


// function to save generic image from buffer <buf>, dimx*dimy into <file_name> and <type>
int Save_buffer( BYTE* buff_in, int dimx, int dimy, wchar_t* file_name, int type/*=PNG_FILE*/, BOOL vflip/*=TRUE*/ )
{
  ASSERT( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );
  ASSERT( AfxIsValidString( file_name ) ); 
  ASSERT( dimx>0 && dimx<20000 );
  ASSERT( dimy>0 && dimy<20000 );
  ASSERT( (dimx&3) == 0 ); // Not working properly when horiz. size is not a multiple of 4

  if ( !buff_in || !AfxIsValidAddress( buff_in, dimx*dimy ) ) return 99; // skip if ptr_buf NULL


  FCObjImage temp;
  temp.Create( dimx, dimy, 8 );
  BYTE*  temp_ptr = (BYTE*) temp.GetMemStart();
  ASSERT( AfxIsValidAddress( temp_ptr, dimx*dimy ) );

  if ( vflip )
  {
    for ( int y=(dimy-1)*dimx ; y>=0 ; y-=dimx ) // vertical flipped copy 
    {
      CopyMemory( temp_ptr, buff_in+y, dimx );
      temp_ptr += dimx;
    }
  } 
  else
  {
    CopyMemory( temp_ptr, buff_in, dimx*dimy ); // NON flipped copy
  }

  wchar_t newstr[200+1];
  switch (type) 
  {
    case PNG_FILE:
      swprintf( newstr, 200, L"%s.png", file_name );
      break;
    case JPG_FILE:
      swprintf( newstr, 200, L"%s.jpg", file_name );
      break;
    case BMP_FILE:
      swprintf( newstr, 200, L"%s.bmp", file_name );
      break;  
  }

  ASSERT( AfxIsValidString( newstr ) ); 
  temp.Save( newstr );

//  TRACE ("Buffer image <%s> saved\n", file );
//  TRACE( L"Buffer image <%s> saved\n", newstr );
  
  return 0;
}





// function to save generic image from buffer <buf>, dimx*dimy into <file_name> and <type>
//int Save_buffer( BYTE* buff_in, int dimx, int dimy, char* file_name, int type/*=PNG_FILE*/ )
int Save_color( UINT* buff_in, int dimx, int dimy, wchar_t* file_name, int type/*=PNG_FILE*/, BOOL vflip/*=TRUE*/, BOOL bpp24/*=TRUE*/ )
{
  ASSERT( type==PNG_FILE || type==JPG_FILE || type==BMP_FILE );
  ASSERT( AfxIsValidString( file_name ) ); 
  ASSERT( dimx>0 && dimx<20000 );
  ASSERT( dimy>0 && dimy<20000 );

  if ( !buff_in || !AfxIsValidAddress( buff_in, dimx*dimy*4 ) ) return 99; // skip if ptr_buf NULL

  FCObjImage temp;
  temp.Create( dimx, dimy, 32 );
//  UINT*  temp_ptr = (UINT*) temp.GetMemStart();
  BYTE*  temp_ptr = (BYTE*) temp.GetMemStart();
  ASSERT( AfxIsValidAddress( temp_ptr, dimx*dimy*4 ) );

  BYTE* buff = (BYTE*) buff_in;

  if ( vflip )
  {
    for ( int y=(dimy-1)*dimx ; y>=0 ; y-=dimx ) // vertical flipped copy 
    {
      if ( bpp24 ) // 24bpp mode
      {
        //CopyMemory( temp_ptr, buff+y, dimx*3 );
        for ( int x=0 ; x<dimx ; x++ )
        {
          temp_ptr[4*x  ] = buff[3*x+3*y+2];
          temp_ptr[4*x+1] = buff[3*x+3*y+1];
          temp_ptr[4*x+2] = buff[3*x+3*y  ];
          temp_ptr[4*x+3] = 255;

        }
        temp_ptr += 4*dimx;
      }
      else // 32bpp mode
      {
        CopyMemory( temp_ptr, buff+4*y, dimx*4 );
        temp_ptr += 4*dimx;
      }
    }
  } 
  else // NON flipped copy
  {
    if ( bpp24 ) // 24bpp mode
    {
      CopyMemory( temp_ptr, buff, dimx*dimy*3 ); 
    }
    else // 32bpp mode
    {
      CopyMemory( temp_ptr, buff, dimx*dimy*4 ); 
    }
  }


  wchar_t newstr[200+1];
  switch (type) 
  {
    case PNG_FILE:
      swprintf( newstr, 200, L"%s.png", file_name );
      break;
    case JPG_FILE:
      swprintf( newstr, 200, L"%s.jpg", file_name );
      break;
    case BMP_FILE:
      swprintf( newstr, 200, L"%s.bmp", file_name );
      break;  
  }

  ASSERT( AfxIsValidString( newstr ) ); 
  temp.Save( newstr );

//  TRACE ("Buffer image <%s> saved\n", file );
  TRACE( L"Color image <%s> saved\n", newstr );
  //TRACE( "Color image <%s> saved\n", newstr );
  
  return 0;
}








int Allocate_serie( serie_type_double* serie, UINT dim )
{
  if ( serie->data && serie->dim==dim ) // Memory Ok, just clear it
  {
    ZeroMemory( serie->data, dim*sizeof(double) );
    return(0);
  }

  if ( serie->data ) // already allocated, free first
  {
    free( serie->data );
    serie->data = NULL;
  }

  serie->data = (double*) malloc( dim*sizeof(double) );
  serie->dim  = dim;
  ZeroMemory( serie->data, dim*sizeof(double) );
  return(0);
}

int Allocate_serie( serie_type_int* serie, UINT dim )
{
  if ( serie->data && serie->dim==dim ) // Memory Ok, just clear it
  {
    ZeroMemory( serie->data, dim*sizeof(int) );
    return(0);
  }

  if ( serie->data ) // already allocated, free first
  {
    free( serie->data );
    serie->data = NULL;
  }

  serie->data = (int*) malloc( dim*sizeof(int) );
  serie->dim  = dim;
  ZeroMemory( serie->data, dim*sizeof(int) );
  return(0);
}




double ncc( double* serie1, double* serie2, UINT dim )
{
  UINT   i;
  double ncc = 0.0;

  ASSERT( dim>0 && dim<100000 );

  ASSERT( serie1 && AfxIsValidAddress( serie1, dim*sizeof(double) ) );
  ASSERT( serie2 && AfxIsValidAddress( serie2, dim*sizeof(double) ) );

  double avg1 = 0.0;
  double avg2 = 0.0;

  for ( i=0 ; i<dim ; i++ )
  {
    avg1 += serie1[i];
    avg2 += serie2[i];
  }

  avg1 /= dim;
  avg2 /= dim;

  double num  = 0.0;
  double den1 = 0.0;
  double den2 = 0.0;

  for ( i=0 ; i<dim ; i++ )
  {
    num  += (serie1[i]-avg1)*(serie2[i]-avg2);
    den1 += (serie1[i]-avg1)*(serie1[i]-avg1);
    den2 += (serie2[i]-avg2)*(serie2[i]-avg2);
  }

  if ( ( den1 != 0.0 ) && ( den2 != 0.0 ) ) ncc = num / ( sqrt(den1)*sqrt(den2) );
  else                                      ncc = 0.0;

  return ncc;
}


// gets all points being touched
int precalc_circle( int radius, serie_type_double* serie )
{
  double ang = 0.0;
  double stp = 0.1*atan( 1.0 / radius );

  int x1=0, y1=0;
  int x2=0, y2=0;

  while ( ang < 2.0*PI )
  {
    x1 = int( 0.5 + radius*cos(ang) );
    y1 = int( 0.5 + radius*sin(ang) );
    if ( x1!=x2 || y1!=y2 )
    {
      x2 = x1;
      y2 = y1;
      TRACE("%d,%d,%.4lf\n", x2, y2, ang );
    }
    ang += stp;
  }
  return 0;
}


int precalc_circle2( int radius, int dimx, serie_type_int* serie )
{
  int error = 1 - radius;
  int errorY = 1;
  int errorX = -2 * radius;
  int x = radius, y = 0;
 
 /* setPixel(x0, y0 + radius);
  setPixel(x0, y0 - radius);
  setPixel(x0 + radius, y0);
  setPixel(x0 - radius, y0); */

  int i=1;
  TRACE("%+3d,%+3d,%d\n", radius, 0, i );
//  TRACE("%+3d,%+3d\n",       0, -radius );
//  TRACE("%+3d,%+3d\n",  radius,       0 );
//  TRACE("%+3d,%+3d\n", -radius,       0 );

  while ( y < x )
  {
    if ( error > 0 ) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;    
/*  setPixel(x0 + x, y0 + y);
    setPixel(x0 - x, y0 + y);
    setPixel(x0 + x, y0 - y);
    setPixel(x0 - x, y0 - y);
    setPixel(x0 + y, y0 + x);
    setPixel(x0 - y, y0 + x);
    setPixel(x0 + y, y0 - x);
    setPixel(x0 - y, y0 - x);  */

    TRACE("%+3d,%+3d,%d\n",  x,  y, ++i );
 /*   TRACE("%+3d,%+3d\n", -x,  y );
    TRACE("%+3d,%+3d\n",  x, -y );
    TRACE("%+3d,%+3d\n", -x, -y );
    TRACE("%+3d,%+3d\n",  y,  x );
    TRACE("%+3d,%+3d\n", -y,  x );
    TRACE("%+3d,%+3d\n",  y, -x );
    TRACE("%+3d,%+3d\n", -y, -x ); */

//    i++;
//    TRACE("i=%d\n",i);


  }

  TRACE("fine\n");
  return 0;

  //int x0 = 0;
  //int y0 = 0;
/*  int error = 1 - radius;
  int errorY = 1;
  int errorX = -2 * radius;
  int x = radius, y = 0;
 
  int npt = 1;
  TRACE("%d,%d\n", radius, 0 );

  while( y < x )
  {
    if( error > 0 ) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;    

    TRACE("%d,%d\n", x, y );
    npt++;
  }
  TRACE("First run %d points\n", npt );


  Allocate_serie( serie, npt*8 );

  serie->data[    0] =  radius;
  serie->data[2*npt] =  radius*dimx;
  serie->data[4*npt] = -radius;
  serie->data[6*npt] = -radius*dimx;
  int i = 1;  */


//  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while( x >= y )
  {

    TRACE("%+3d,%+3d\n",  x,  y );
/*    TRACE("%+3d,%+3d\n",  y,  x );
    TRACE("%+3d,%+3d\n", -x,  y );
    TRACE("%+3d,%+3d\n", -y,  x );
    TRACE("%+3d,%+3d\n", -x, -y );
    TRACE("%+3d,%+3d\n", -y, -x );
    TRACE("%+3d,%+3d\n",  x, -y );
    TRACE("%+3d,%+3d\n",  y, -x ); */

/*  DrawPixel( x + x0,  y + y0);
    DrawPixel( y + x0,  x + y0);
    DrawPixel(-x + x0,  y + y0);
    DrawPixel(-y + x0,  x + y0);
    DrawPixel(-x + x0, -y + y0);
    DrawPixel(-y + x0, -x + y0);
    DrawPixel( x + x0, -y + y0);
    DrawPixel( y + x0, -x + y0); */
    TRACE("i=%d\n",i);
    i++;
 
    y++;
    if ( radiusError<0 ) radiusError+=2*y+1;
    else
    {
      x--;
      radiusError += 2*(y-x+1);
    }
  }


/*  while( y < x )
  {
    if( error > 0 ) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;    

    //TRACE("%d,%d\n", x, y );
    serie->data[i      ] =   x + y*dimx;
    serie->data[i+  npt] =   y + x*dimx;
    serie->data[i+2*npt] =   x - y*dimx;
    serie->data[i+3*npt] =   y - x*dimx;
    serie->data[i+4*npt] = - x + y*dimx;
    serie->data[i+5*npt] = - y + x*dimx;
    serie->data[i+6*npt] = - x - y*dimx;
    serie->data[i+7*npt] = - y - x*dimx;
    npt++;


  }
  //TRACE("First run %d points\n", npt );

  for ( i=0 ; i<npt*8 ; i++ )
  {
    TRACE("%d,%d\n",i,serie->data[i]);
  }


/*    setPixel(x0 + x, y0 + y);
    setPixel(x0 - x, y0 + y);
    setPixel(x0 + x, y0 - y);
    setPixel(x0 - x, y0 - y);
    setPixel(x0 + y, y0 + x);
    setPixel(x0 - y, y0 + x);
    setPixel(x0 + y, y0 - x);
    setPixel(x0 - y, y0 - x); */
  return 0;
}


int precalc_circle3( int radius, int dimx, serie_type_int* serie )
{
  int error = 1 - radius;
  int errorY = 1;
  int errorX = -2 * radius;
  int x = radius, y = 0;
 
  int i=1;
  TRACE("%+3d,%+3d,%d\n", radius, 0, i );
//  TRACE("%+3d,%+3d\n",       0, -radius );
//  TRACE("%+3d,%+3d\n",  radius,       0 );
//  TRACE("%+3d,%+3d\n", -radius,       0 );

  while ( y < x )
  {
    if ( error > 0 ) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;    

    TRACE("%+3d,%+3d,%d\n",  x,  y, ++i );
 /*   TRACE("%+3d,%+3d\n", -x,  y );
    TRACE("%+3d,%+3d\n",  x, -y );
    TRACE("%+3d,%+3d\n", -x, -y );
    TRACE("%+3d,%+3d\n",  y,  x );
    TRACE("%+3d,%+3d\n", -y,  x );
    TRACE("%+3d,%+3d\n",  y, -x );
    TRACE("%+3d,%+3d\n", -y, -x ); */
  }

  TRACE("fine\n");
  return 0;
}


////////////////////////////////////////////////////////////////////////////
//                    LAST VERSION OK
// points are generated in 1st and 2nd Q in right order
//
// data are stored into circle in clockwise direction, only 1st and 2nd Qs
// data for 3rd and 4th can be obtained by reversing x / y / ofs
//
////////////////////////////////////////////////////////////////////////////

/*
#define MAXR_CIRCLES 10

struct circle_type // temporary version
{
  int *x; // xs of all points starting from radius,0 clockwise dir 
  int *y; // ys of all points starting from radius,0 clockwise dir
  int  n; // number of points (half circle only)
};

*/


circle_type my_circle[MAXR_CIRCLES];

int precalc_circles( void )
{
  int x, y, i;
  int semiamp = MAXR_CIRCLES;

  CTimer timer;

  struct circlet_type // temporary version
  {
    int x[4*MAXR_CIRCLES];
    int y[4*MAXR_CIRCLES];
    int n;
  };

  
  if ( my_circle && my_circle[1].n==4 ) return 0; // already called //20200320
  
  circlet_type* my_circle[MAXR_CIRCLES];
  ZeroMemory( my_circle, sizeof(my_circle) );

  for ( i=0 ; i<MAXR_CIRCLES ; i++ )
  {
    my_circle[i] = (circlet_type*) malloc( sizeof(circlet_type) );
    ZeroMemory( my_circle[i], sizeof(circlet_type) );
  }

//  for ( y=-semiamp ; y<=semiamp ; y++ )
  for ( y=0 ; y<=semiamp ; y++ )
  {
//    for ( x=-semiamp ; x<=semiamp ; x++ )
//    for ( x=0 ; x<=semiamp ; x++ )
    for ( x=semiamp ; x>0 ; x-- )
    {
      double dist = sqrt(double(x*x+y*y));
      int r = int( 0.5 + dist ); /// round to closest int

      if ( r<MAXR_CIRCLES )
      {
        my_circle[r]->x[my_circle[r]->n] = x;
        my_circle[r]->y[my_circle[r]->n] = y;
        my_circle[r]->n++;
        //TRACE("%+3d, %+3d, %3.2lf\n", x, y, dist );
      }

    }
  }


  int mem = 0;

  for ( x=1 ; x<MAXR_CIRCLES ; x++ ) // scan over r
  {

    ASSERT( 2*my_circle[x]->n>0 && 2*my_circle[x]->n<10000 );

//    TRACE("Radius=%4d - n=%4d - rapp=%4.1lf\n", x, my_circle[x]->n, x?double(my_circle[x]->n)/x:0 );

    ::my_circle[x].n = 2*my_circle[x]->n;
    ::my_circle[x].x = (int*) malloc( ::my_circle[x].n * sizeof(int) );
    ::my_circle[x].y = (int*) malloc( ::my_circle[x].n * sizeof(int) );

    mem += 2 * ( ::my_circle[x].n * sizeof(int) );

    //TRACE("1Q\n");
    for ( i=0 ; i<my_circle[x]->n ; i++ )
    {
      ::my_circle[x].x[i] = my_circle[x]->x[i];
      ::my_circle[x].y[i] = my_circle[x]->y[i];
      // TRACE("%+3d,%+3d\n", my_circle[x]->x[i], my_circle[x]->y[i] );
      // TRACE("%+3d,%+3d,%5d\n", my_circle[x]->x[i], my_circle[x]->y[i], my_circle[x]->x[i]+100*my_circle[x]->y[i] );
    }

    //TRACE("2Q\n");
    for ( i=0 ; i<my_circle[x]->n ; i++ )
    {
      ::my_circle[x].x[my_circle[x]->n+i] = -my_circle[x]->y[i];
      ::my_circle[x].y[my_circle[x]->n+i] =  my_circle[x]->x[i];
      // TRACE("%+3d,%+3d\n", -my_circle[x]->y[i], my_circle[x]->x[i] );
      // TRACE("%+3d,%+3d,%5d\n", -my_circle[x]->y[i], my_circle[x]->x[i], -my_circle[x]->y[i]+100*my_circle[x]->x[i] );
    }

    //TRACE("Radius=%4d allocated and filled - n=%4d - mem=%8d\n", x, 2*my_circle[x]->n, mem );

    /* NO!
    //TRACE("3Q\n");
    for ( i=0 ; i<my_circle[x]->n ; i++ )
    {
      // TRACE("%+3d,%+3d\n", -my_circle[x]->x[i], -my_circle[x]->y[i] );
      // TRACE("%+3d,%+3d,%5d\n", -my_circle[x]->x[i], -my_circle[x]->y[i], -my_circle[x]->x[i]-100*my_circle[x]->y[i] );
    }

    //TRACE("4Q\n");
    for ( i=0 ; i<my_circle[x]->n ; i++ )
    {
      // TRACE("%+3d,%+3d\n", my_circle[x]->y[i], -my_circle[x]->x[i] );
      // TRACE("%+3d,%+3d,%5d\n", my_circle[x]->y[i], -my_circle[x]->x[i], my_circle[x]->y[i]-100*my_circle[x]->x[i] );
    }
    */
  }

  for ( i=0 ; i<MAXR_CIRCLES ; i++ ) if ( my_circle[i] ) free( my_circle[i] );

  TRACE("completato in %d ms\n", timer.elapsed_ms() );
  return 0;
}



int ATNLUT[DIMATN][DIMATN];  // array[dx][dy] (25x25)


/*
#define AMPATN             6 // semilenght for atan calculation
#define LNGATN  (2*AMPATN+1) // max lenght for atan calculation
static int ATNLUT[LNGATN][LNGATN];
*/

/*               dx>0 dy=0
                    270?                            BUFFER
                     ^                               O-------> X
          +-    225  |  315    ++                    |          ^ 180
                   \ | /                             |          |
dx=0 dy<0 180?<-----0-----> 0?dx=0 dy>0            |     90 <-O-> 270
                   / | \                             v          |
          --    135  |  45     -+                    Y          v
                     v                                          0
                    90?
                 dx<0 dy=0

   dx  dy  atn
  -12 -12  135
    0 -12  180
  +12 -12  225
  -12   0   90
  +12   0  270
  -12 +12   45
    0 +12    0
  +12 +12  315

*/

int precalc_ATNLUT( void )
{

  ZeroMemory( ATNLUT, sizeof(ATNLUT) );
  //TRACE("precalc_ATNLUT (%d bytes) cleared\n",sizeof(ATNLUT));

  for ( int dy=-AMPATN*2 ; dy<=AMPATN*2 ; dy++ )
  {
    //TRACE("%d,",dy);
//    for ( int dx=-AMPATN*2 ; dx<=AMPATN*2 ; dx+=2*AMPATN )
    for ( int dx=-AMPATN*2 ; dx<=AMPATN*2 ; dx++ )
    {
      if ( dx )
      {
        ATNLUT[dx+AMPATN*2][dy+AMPATN*2] = DOUBLETOINT(18000.0+18000.0*atan2(double(dx),double(-dy))/PIGRECO);
      }
      else
      {
        if ( dy>=0) ATNLUT[dx+AMPATN*2][dy+AMPATN*2] =     0;
        else        ATNLUT[dx+AMPATN*2][dy+AMPATN*2] = 18000;
      }
      //TRACE("ATNLUT[%+3d][%+3d] = %5d\n", dx, dy, ATNLUT[dx+AMPATN*2][dy+AMPATN*2] );
      //TRACE("%d,",ATNLUT[dx+AMPATN*2][dy+AMPATN*2]);
    }
    //TRACE("\n");
  }

  TRACE("precalc_ATNLUT completed\n");
  return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////











