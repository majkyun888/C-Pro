#pragma once

//#pragma pack(16)

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <Math.h>
//#include <Winver.h>
//#include <Windows.h>

#include "shlwapi.h"
#include "strings.h"
#include "Def_globali.h"
#include "ChildView.h"
#include "Timer.h"
#include "Text_box.h"
#include "Area_adjust.h"
//#include "File_mngmt.h"
//-------------------added for new processing function
#include <opencv2/opencv.hpp> //added for new processing algo 
//-------------------/
#include "Recipe_manager.h"
#include "Parameter.h"
#include "Menu_button.h"
#include "Batch_manager.h"
#include "Results_manager.h"
#include "Logger.h"
#include "Calibration_data.h"
#include "Splash_screen.h"
#include "Ampoule_filling_level.h"
#include "Particle_inspection.h"
#include "Cosmetic_inspection.h"
#include "Filling_level.h"
#include "Tip_inspection.h"
#include "Bottom_inspection.h"
#include "FOcolor_inspection.h"
#include "LQcolor_inspection.h"
#include "OralFO_inspection.h"
#include "DRcolor_inspection.h"
#include "Waist_layer.h"
#include "Skinning.h"
#include "Batch_manager.h"
#include "Image_storage.h"
#include "Inspection_result.h"
#include "Log_viewer.h"
#include "Stopper_inspection.h"
#include "PFStopper_inspection.h"
#include "PFSidewall_inspection.h"
#include "Flange_inspection.h"

//added new processing function
#include "Neck_inspection.h"

#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  #include "Password_manager_v2.h"
#else
  #include "Password_manager.h"
#endif




//#ifdef IMAGE_DISPLAY
  #include "Image_display.h"
  #include "Image_layout.h"
//#endif


  #include "TcAdsDef.h"
  #include "TcAdsApi.h"


  #define CTYPE_BOOL8            4L  // Booleano/flag a 8 bit
  #define CTYPE_BYTE             8L  // Intero senza segno 8bit
  #define CTYPE_INT             16L  // Intero con segno 16bit
  #define CTYPE_DINT            32L  // Intero con segno 32bit
  #define CTYPE_REAL            64L  // Virgola singola precisione
  #define CTYPE_LREAL          128L  // Virgola doppia precisione
  #define CTYPE_STRING         256L  // Stringa di lunghezza data da dim + 1 x terminatore
  #define CTYPE_GENERAL        512L  // Dato di tipo generico (da gestire a parte)
  #define CTYPE_SCALED16      1024L  // Dato di tipo scalato (real HMI -  int PLC)
  #define CTYPE_SCALED32      2048L  // Dato di tipo scalato (real HMI - dint PLC)

  #define CTYPE_BECKHOFF      4096L  // Se definito, il parametro ?connesso con una variabile PLC
  #define CTYPE_READONLY     16384L  // Se definito non ?ammessa la modifica
  #define CTYPE_UPDATE       32768L  // Se definito il valore viene aggiornato ciclicamente
  #define CTYPE_UNSIGNED     65536L  // Adding UNSIGNED type by Pour 20170724

	struct ADS_data_type
	{
		char*     var_name; // = NULL;
		ULONG     handle;   // = 0;
		UINT      dim;      // = 0;
		void*     val;      // = NULL;
	};



  //#pragma pack(show)

	#pragma pack(1)
	struct message_type
	{
    bool         on;
    BYTE         format;
		USHORT       index;
		int          param_0;
		int          param_1;
		double       param_2;
		double       param_3;
		char         param_4[20+1];
    char         msg[140+1];
	};
	#pragma pack(8) // restore previous (check: should be 8!)


  int      ADS_init      ( void );
	int      ADS_terminate ( void );
  wchar_t* ADS_state_desc( int val );
	wchar_t* ADS_error_desc( int nErr );
  
  int     ADS_create_updatelist( void );
  int     ADS_create_updatelist_new( void );
  int     ADS_update_list( void );
  int     ADS_terminate_updatelist( void );
  
  int     ADS_init_parameter ( char* var_name, ADS_data_type* param, UINT dim, BOOL silent=FALSE );
  int     ADS_read_parameter ( ADS_data_type* param );
  int     ADS_read_parameter ( char* var_name, int* val );

  int     ADS_read_int ( ADS_data_type* param );
  BYTE    ADS_read_byte( ADS_data_type* param );

  int     ADS_write_byte( ADS_data_type* param, BYTE val );
  int     ADS_write_int ( ADS_data_type* param, int  val );

  int     ADS_write_parameter( ADS_data_type* param );
  int     ADS_write_value    ( char* name_in, double val_in, int type_in );

  int     ADS_clean_parameter(  ADS_data_type* param );
  int     ADS_register_trigger_callback( void );
  int     ADS_deregister_trigger_callback( void );

  int     ADS_read_serialnumber( void );

#ifdef HV_DATAREC
  int     ADS_HVdata_deregcallback( void );
  int     ADS_HVdata_regcallback( void ); 

  //Pour added 20180329

  int     ADS_read_hvdata( void ); //Pour added 20180408
  
  //Pour added 20180412
  int     ADS_read_hvdata_array( void );
  //Pour added 20180412
  //Pour added 20180329

#endif

  /*
  #ifdef STATUS_MONITORING
  int     ADS_status_deregcallback( void );
  int     ADS_status_regcallback( void ); 
	#endif
*/

  #ifdef SAVE_INPUTDATA
    int     ADS_inputdata_deregcallback( void );
    int     ADS_inputdata_regcallback( void ); 
  #endif

  #ifdef HGA_DATAREC
    int     ADS_hgadata_deregcallback( void );
    int     ADS_hgadata_regcallback( void ); 
  #endif


  extern 	CString       ADS_error; 
  extern  int           ADS_status; //  0=non inizializzato
                                    //  1=Ok, funzionante
                                    // 99=Errore


struct update_data_type
{
  ADS_data_type*  ADS;        // pointer to ADS object
  UINT*           perc_base;  // pointer to ADS val area to be used to calculate percentage
  int             obj_type;   //
  wchar_t*        desc_txt;   // ptr to description text (eng or chn)
  wchar_t*        format_txt; // display/formatting specifier
  CStatic*        desc_st;    // description cstatic pointer
  CStatic_mod*    text_st;    // text cstatic pointer
  CText_box*      item_rt;    // 
};

#define UPLIST_FONT            110, L"DejaVu Sans Condensed"   // font used for text into text rows 
#define MAX_UPLIST_NUM          20

/*
static update_data_type        uplist[MAX_UPLIST_NUM];
static UINT                    uplist_num;
*/


#ifdef BASLER_SDK
  #include <pylon/PylonIncludes.h>
  #include <pylon/gige/BaslerGigECamera.h>

  //#include "Base/GCBase.h"
  //#include "Base/GCUtilities.h"
  //#include "GenApiVersion.h"

  using namespace Pylon;
  using namespace Basler_GigECameraParams;
  using namespace Basler_GigEStreamParams;
  typedef Pylon::CBaslerGigECamera Camera_t;
  #include "Camera.h"
#endif

class CChildView;

extern CChildView*  pView;
extern wchar_t      msg_err[201];
extern CString      msg;
extern UINT         lng;
extern CText_box*   machine_txt;

int  Prepare_palette     ( void    );

int  Import_reflections  ( int inp );
int  Export_reflections  ( int inp );

int  Filter_reflections  ( int inp );
int  Show_reflections    ( int inp );

int Learn_needleshield   ( int inp );
int Datasave_startstop   ( int inp );

int  Learn_reflections   ( int inp );
int  Learn_models        ( int inp );
int  Set_LUT             ( int inp );
int  Set_lqfdmode        ( int inp );

int  Set_stn1_mode       ( int inp );


int  Tipcap_mode         ( int inp );
int  Tipcap_freerunning  ( int inp );

int  Cycle_campwr        ( int inp );

int  Test_process        ( int inp );
int  Sequence_acquire    ( int inp );
int  Send_spin_parameters( int inp );
int  Reset_statistics    ( int inp );
int  Batch_startstop     ( int inp );
int  Batch_report        ( int inp );

int  CaptureBMP          ( LPCTSTR szFile );
int  Capture_screenshot  ( LPCTSTR szFile, int index=-1 );
int  Read_version        ( void );


wchar_t* thousands( UINT     inp );
wchar_t* thousands( wchar_t* inp );


//int  Save_image          ( BYTE* buf, int dimx, int dimy, char* file_name, int type==PNG_FILE );
int  Load_buffer( BYTE* buff_in, int dimx, int dimy, wchar_t* file_name, int type=PNG_FILE, BOOL vflip=TRUE );
int  Save_buffer( BYTE* buff_in, int dimx, int dimy, wchar_t* file_name, int type=PNG_FILE, BOOL vflip=TRUE );
int  Save_color ( UINT* buff_in, int dimx, int dimy, wchar_t* file_name, int type=PNG_FILE, BOOL vflip=TRUE, BOOL bpp24=TRUE );


int  Show_alarmlog       ( int inp );
int  Show_log            ( int inp );


int  Change_passwords    ( int inp );
int  Logout              ( int inp );
int  Save_images         ( int inp );
int  Load_images         ( int inp );
int  Machine_lights      ( int inp ); //, CMenu_button* ptr = NULL );
int  Inspection_startstop( int inp = 0 ); // 0=toggle - 10=force start - 20=force stop
int  Movie_startstop     ( int inp = 0 ); // 0=toggle - 10=force start - 20=force stop

int  Change_view         ( int inp=-1 );  // no input=cycle - 0: first group - 1: second group - etc
//void Show_hide           ( int sta, int end );
void Show_hide           ( UINT group );

int  Set_user            ( int inp );
int  Data_record         ( int inp );
int  Inpdata_record      ( int inp );
int  HVdata_record       ( int inp );
int  HGAdata_record      ( int inp );

int  Save_inspections    ( int inp );
int  Save_sequences      ( int inp );
int  Grippers_statistics ( int inp );

int  Loop_inspection     ( int inp );
int  Manual_jog          ( int inp );
int  Set_paths           ( int inp );
int  Set_passwords       ( int inp );

int  Change_mode         ( int inp );

int  Read_csv( CString& name, int nr, int nc, int* data, wchar_t* txt=NULL ); 

//Pour added for Sunho on 20200702
//Pour added 20200702
//extern BOOL          f_run_automatic;

void Scale_image( BYTE* img_in , int* img_out );
//void Add_2_img_and_delta( BYTE* dest, BYTE* base, BYTE* source, BYTE* delta, UINT dim );
//v/oid Add_2_img_and_delta_inv( BYTE* dest, BYTE* source, BYTE* sensiv, UINT dim );
void Get_diff( BYTE* dest, BYTE* source, UINT dim );


#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing
  UINT __cdecl processing_thread  ( LPVOID pParam );
  //BYTE*        img_ptr; // = NULL;
  //CEvent*      processing_start;
#endif


UINT __cdecl sequence_3d        ( LPVOID pParam );

UINT __cdecl movie_3d           ( LPVOID pParam );
UINT __cdecl movie_3d_sg        ( LPVOID pParam );
UINT __cdecl movie_3d_linear    ( LPVOID pParam );
UINT __cdecl movie_3d_doublecap ( LPVOID pParam );

//UINT __cdecl inspect_3d         ( LPVOID pParam );
UINT __cdecl inspect_3d_cosmetic( LPVOID pParam );

//Pour added on 20190429
UINT __cdecl inspect_3d_neck( LPVOID pParam );
//Pour added on 20190429

UINT __cdecl inspect_3d_doublecap( LPVOID pParam );
UINT __cdecl inspect_3d_tipcap   ( LPVOID pParam );
UINT __cdecl inspect_3d_singlecam( LPVOID pParam );

UINT __cdecl inspect_3d_filllev ( LPVOID pParam );
UINT __cdecl inspect_3d_particle( LPVOID pParam );

UINT __cdecl inspect_3d_cakesidearback( LPVOID pParam );

UINT __cdecl alarm_warning_3d   ( LPVOID pParam );



wchar_t* AtoW( const char*    a );
char*    WtoA( const wchar_t* w );

#ifdef _DEBUG
  int  CHK_BIT( UINT  var, UINT nbit);
  void SET_BIT( UINT& var, UINT nbit);
  void CLR_BIT( UINT& var, UINT nbit);
#else
  #define CHK_BIT(var,nbit) ((var>>(nbit))&1)
  #define SET_BIT(var,nbit) var|= (1<<(nbit))
  #define CLR_BIT(var,nbit) var&=~(1<<(nbit))
#endif


#define MAXR_CIRCLES 260

struct circle_type // temporary version
{
  int *x; // xs of all points starting from radius,0 clockwise dir
  int *y; // ys of all points starting from radius,0 clockwise dir
  int  n; // number of points (half circle only)
};

//circle_type my_circle[MAXR_CIRCLES];



int Allocate_serie( serie_type_double* serie, UINT dim );
int Allocate_serie( serie_type_int*    serie, UINT dim );

double ncc( double* serie1, double* serie2, UINT dim );
int precalc_circle ( int radius, serie_type_double* serie );
int precalc_circle2( int radius, int dimx, serie_type_int* serie );
int precalc_circle3( int radius, int dimx, serie_type_int* serie );
int precalc_circles( void );



//#define AMPATN             6 // semilenght for atan calculation
//#define LNGATN  (2*AMPATN+1) // max lenght for atan calculation
int precalc_ATNLUT( void );


#define AMPATN             6 // semilenght for atan calculation
#define LNGATN  (2*AMPATN+1) // max lenght for atan calculation
#define DIMATN  (4*AMPATN+1) // 1D dimension of array

#ifdef NEWCAP_DOUBLE
  #undef  AMPATN
  #define AMPATN           4 // reduced
#endif


#define PI  3.141592653589


#define SCALE_IMG(nimg, img)      (image_dsp[nimg]->*(image_dsp[nimg]->scale_img))( img )

#define DRAW_DASH(nimg,x,y,c1,c2) (image_dsp[nimg]->*(image_dsp[nimg]->draw_dash))(x,y,c1,c2)
#define DRAW_PT(nimg,x,y,col)     (image_dsp[nimg]->*(image_dsp[nimg]->draw_pt))(x,y,col)
#define DRAW_CROSS(nimg,x,y,col)  (image_dsp[nimg]->*(image_dsp[nimg]->draw_cross))(x,y,col)
#define DRAW_SQUARE(nimg,x,y,col) (image_dsp[nimg]->*(image_dsp[nimg]->draw_square))(x,y,col)

// convert buffer to display coordinates
#define BUF2DSP(nimg,x,y)     (image_dsp[nimg]->*(image_dsp[nimg]->buf2dsp))(x,y)
#define BUF2OFS(nimg,x,y)     (image_dsp[nimg]->*(image_dsp[nimg]->buf2ofs))(x,y)

// convert display to buffer coordinates
#define DSP2BUF(nimg,x,y)     (image_dsp[nimg]->*(image_dsp[nimg]->dsp2buf))(x,y)

//#define SETPIXELXY(nimg,x,y,col)  image_dsp[nimg]->ptr_dsp[x+y*image_dsp[nimg]->dim_dsp.cx]=col
#define SETPIXELOFS(nimg,ofs,col) image_dsp[nimg]->ptr_dsp[ofs]=col
#define DSPDX(ind)                image_dsp[ind]->dim_dsp.cx

//Pour marked to be confirmed on 20201130
#define OUTMAT2DSP (nimg, im_out)   (image_dsp[nimg]->*(image_dsp[nimg]->outmat2dsp))(im_out)


//#define MAX_NUM_DIAM  20  // max total number of diameters each camera can execute 


