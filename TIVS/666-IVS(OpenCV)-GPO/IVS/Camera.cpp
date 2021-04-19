
#include "stdafx.h"
#include "IVS.h"
#include "Camera.h"
#include "Globals.h"
#include "Output_data.h"
#include "Image_display.h"
#include "BufferFactory_new.h"

////Pour added for interface tests on 20190429
//#include"Particle_inspection.h"
//#include <opencv2\opencv.hpp>
////#include "./middlewareParticleDi.h"
//#include "../MiddlewareImproess/middlewareParticleDi.h"
//using namespace std;
//#ifdef _DEBUG
//#pragma comment(lib,"../IVS/Debug/MiddlewareImproess.lib")
//#else
//#pragma comment(lib,"../IVS/Release/MiddlewareImproess.lib")
//#endif
////Pour added for interface tests on 20190429

/*
#ifdef AUTORECONNECT_CAMERA
//  #include "../include/ConfigurationEventPrinter.h"
//  #include "ConfigurationEventPrinter.h"


class CSampleConfigurationEventHandler : public Pylon::CConfigurationEventHandler 
{
public:
    // This method is called from a different thread when the camera device removal has been detected.
    void OnCameraDeviceRemoved( CInstantCamera&) 
    {
        // Print two new lines, just for improving printed output.
        cout << endl << endl;
        cout << "CSampleConfigurationEventHandler::OnCameraDeviceRemoved called." << std::endl;
    }
};


#endif // AUTORECONNECT_CAMERA
*/



#define ACE_TSFREQ      125000000 // BASLER ACE, TIMESTAMP FREQUENCY READ FROM VIEWER


UINT                    num_tlc = 0;

UINT                    particle_processing = 0;
UINT                    tipcap_processing   = 0;

BOOL                    show_back_light = FALSE; // When true TOPRING light images are shown / otherwise show BACKLIGHT

UINT                    crcam = 0x43A567E8; // seed

BOOL                    alarmemptypc1 = FALSE; // when true, alarm is triggered upon empty container detected

int                     refresh_delay = 1000;


Camera_struct           camera       [NUM_TLC];
CAmpoule_filling_level* filling_level[NUM_TLC]; // filling level for ampoules objects
CParticle_inspection*   particle_insp[NUM_TLC]; // particle inspection
CCosmetic_inspection*   cosmetic_insp[NUM_TLC]; // cosmetic inspection
CFilling_level*         fillingl_insp[NUM_TLC]; // filling level inspection
CTip_inspection*        tip_insp     [NUM_TLC]; // ampoule's tip / vial's cap inspection
CBottom_inspection*     bottom_insp  [NUM_TLC]; // ampoule's tip inspection
CFOcolor_inspection*    focolor_insp [NUM_TLC]; // flip off color inspection (top cap color camera)
CLQcolor_inspection*    lqcolor_insp [NUM_TLC]; // Liquid color inspection

//CStopper_inspection*    stopper_insp  [NUM_TLC]; // PFS stopper inspection

//Pour on 220201130
CFlange_inspection*      flange_insp[NUM_TLC]; // PFS stopper inspection
//Pour on 20201130

CPFStopper_inspection*   pfstopper_insp[NUM_TLC]; // PFS stopper inspection
CPFSidewall_inspection*  pfsidewall_insp[NUM_TLC]; // PFS sidewall inspection (area sequence)
//CPFCosmetic3_inspection* pfcosmetic3_insp [NUM_TLC]; // PFS cosmetic3 (bottom A&B)




//=====================neck inspection added by yang 20181015
CNeck_inspection*       neck_insp    [NUM_TLC]; // vial's neck inspection 
//CHeel_inspection*       heel_insp    [NUM_TLC]; // cilin's heel inspection
//=====================
CDRcolor_inspection*    drcolor_insp [NUM_TLC]; // dotring color inspection 
//int                     drb_label = insp_res->num; // update global for syncrhonization


//CFilling_level*         fillingl_insp[NUM_TLC]; // filling level inspection
CWaist_layer*           waistlayer_insp[NUM_TLC]; // waist-layer FD inspection
CSkinning*              skinning_insp  [NUM_TLC]; // skinning FD inspection


BOOL                    stop_cameraerror = TRUE; // if TRUE enable stopping of machine when camera error happens

#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing
  //UINT __cdecl processing_thread  ( LPVOID pParam );
  CEvent*      processing_start[2] = { NULL, NULL };
  CEvent*      processing_done [2] = { NULL, NULL };
#endif

#ifdef READ_CYCLES
  extern ADS_data_type          ADS_cycles; // ADS variable connected to cycles
  extern unsigned int           cycles;     // cycles read from PLC
  extern CTimer                 globaltimer;
  extern unsigned int           cycles_0;   // initial value
#endif


COralFO_inspection*     oralfo_insp  [NUM_TLC]; // oral flip off inspection


CBufferFactory_new*     buffact = NULL;
HANDLE                  mutex; 


#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif

extern CLogger*         logger;
extern CLogger*         blalarm_logger;


extern BOOL             debug;
extern wchar_t          PC_name[30]; // Computer name read from system configuration
extern CImage_display*  image_dsp[NUM_TLC];
extern CImage_layout*   layout_dsp;
extern UINT             thread_status;
extern UINT             thread_exit;
extern UINT             trigger_label[NUMTRIGGER];
extern UINT             lng;
extern BOOL             show_rejects; // FALSE=show all images / TRUE=show only rejects

extern BOOL             inpdatarec_on;
extern BOOL             hgadatarec_on;

extern BOOL             datarec_on;
extern BOOL             saveinsp_on;
extern BOOL             saveseq_on;


extern Results_manager* input_result;
extern Results_manager* fillev_result;


//extern Results_manager* hvdata_result;

extern Results_manager* result;
extern Results_manager* result2;
extern Results_manager* cap_result;    // TC A/B/C/D
extern Results_manager* bottomA_result; // bottom A
extern Results_manager* bottomB_result; // bottom B


//extern BOOL             learn_reflections;
//extern BOOL             learn_models;
extern BOOL             learning; // TRUE / FALSE for all controls
extern CMenu_button*    exit_btn;
extern                  ADS_data_type alarm_ads;
extern                  ADS_data_type warning_ads;
extern                  message_type* alarm_data;
extern                  message_type* warning_data;
extern                  CText_box*    alarm_txt;
extern                  CText_box*    warning_txt;

extern CParameter*      f_closed_loop;


extern                  ADS_data_type freezemode_ads;
extern                  BYTE          freezemode_on;



#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
#else
  extern CPassword_manager*  password_mngr;
#endif



extern update_data_type    uplist[MAX_UPLIST_NUM];
extern UINT                uplist_num;

//Pour added 20180605
#ifdef COLOR_DETECTION_P1
void Process_image( CGrabResultPtr ptr_gr, UINT nimg,UINT ind )
{
	image_dsp[ind]->scale_grabresult( ptr_gr ); // scale for display and prepare RGB buffer from BAYER data
	//return Process_image( image_dsp[ind]->ptr_buf, nimg);
}
#endif
//Pour added 20180326
//================================================
class CHearbeatHelper
{
    public:
        explicit CHearbeatHelper(CInstantCamera& camera)
                : m_pHeartbeatTimeout(NULL)
        {
            // m_pHeartbeatTimeout may be NULL
            m_pHeartbeatTimeout = camera.GetTLNodeMap().GetNode("HeartbeatTimeout");
        }

        bool SetValue(int64_t NewValue)
        {
            // Do nothing if no heartbeat feature is available.
            if (!m_pHeartbeatTimeout.IsValid())
                return false;

            // Apply the increment and cut off invalid values if neccessary.
            int64_t correctedValue = NewValue - (NewValue % m_pHeartbeatTimeout->GetInc());

            m_pHeartbeatTimeout->SetValue(correctedValue);
            return true;
        }

        bool SetMax()
        {
            // Do nothing if no heartbeat feature is available.
            if (!m_pHeartbeatTimeout.IsValid())
                return false;

            int64_t maxVal = m_pHeartbeatTimeout->GetMax();
            return SetValue(maxVal);
        }

    protected:
        GenApi::CIntegerPtr m_pHeartbeatTimeout; // Pointer to the node, will be NULL if no node exists.
};
//=================================================
//Pour added 20180326

/*
struct frame_type
{
  uint32_t  framenum;
  uint64_t  timestamp;
  BYTE*     buffer;
};
*/

/*

int Basler_init_old( int ind )
{
//  int ind = 0;

  ASSERT( ind>=0 && ind<NUM_TLC );

  wchar_t  txt[200];
  size_t convertedChars = 0;

  Pylon::PylonAutoInitTerm autoInitTerm;

  // Get all attached cameras and exit the application if no camera is found
  DeviceInfoList_t devices;
  TRACE( L"%d BASLER devices found\n", CTlFactory::GetInstance().EnumerateDevices(devices));

  try
  {
    // Create an instant camera object with the camera device found first.
    CInstantCamera icamera( CTlFactory::GetInstance().CreateFirstDevice() );

    // Print the model name of the camera.
//    mbstowcs_s(&convertedChars, txt, 200, icamera.GetDeviceInfo().GetModelName(), _TRUNCATE); // conversione char=>wchar_t
//    TRACE( L"Created camera <%s>\n", txt );
    mbstowcs_s(&convertedChars, txt, 200, icamera.GetDeviceInfo().GetModelName(), _TRUNCATE); // conversione char=>wchar_t
    TRACE( "Created camera <%s>\n", icamera.GetDeviceInfo().GetModelName() );

    // Print the serial number of the camera.
//    mbstowcs_s(&convertedChars, camera[ind].sn, 9, icamera.GetDeviceInfo().GetSerialNumber(), _TRUNCATE); // conversione char=>wchar_t
//    TRACE( L"Serial number <%s>\n", camera[ind].sn );
    TRACE( "Serial number <%s>\n", icamera.GetDeviceInfo().GetSerialNumber() );


    GenApi::CIntegerPtr pHeartbeat = icamera.GetTLNodeMap().GetNode("HeartbeatTimeout");
    if (pHeartbeat != NULL )
    {
      TRACE( L"Initial Heartbeat value: %.1lf sec\n", 0.001*pHeartbeat->GetValue() );
      pHeartbeat->SetValue(60*1000);
      TRACE( L"Current Heartbeat value: %.1lf sec\n", 0.001*pHeartbeat->GetValue() );
    }

    icamera.Open();
    TRACE( L"Camera[%d] open..\n", ind );

//    camera[ind].camera     = &icamera;
    camera[ind].offsetx    =        0;
    camera[ind].width      = DIMX_ACQ;
    camera[ind].offsety    =        0;
    camera[ind].height     = DIMY_ACQ;


    Parametrize_camera( camera[ind] );

  }
  catch (GenICam::GenericException &e)
  {
    // Error handling
    mbstowcs_s(&convertedChars, txt, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    TRACE( L"Genicam exception thrown <%s>\n", e.GetDescription() );
    return 99;
  }
       

  return( 0 );
}


*/

//CTlFactory* TlFactory = NULL;


int Basler_initialize( void )
{
  Pylon::PylonAutoInitTerm autoInitTerm;
  DeviceInfoList_t devices;
  TRACE( L"%d BASLER cameras found\n", CTlFactory::GetInstance().EnumerateDevices(devices) );

/*  if ( buffer_factory == NULL )
  {
//    buffer_factory = new CBufferFactory_new();
  
  }*/


  buffact = new CBufferFactory_new;

  mutex = CreateMutex( NULL, FALSE, NULL );  // default security attributes - initially not owned - unnamed mutex
  ASSERT( mutex ); // check 


  return 0;
}


int Basler_initialize( UINT ind )
{
//  int ind = 0;

  ASSERT( ind>=0 && ind<num_tlc );
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<NUMTRIGGER );//Pour added +4 on 20201011

 // wchar_t  txt[200];
//  size_t convertedChars = 0;


  try
  {
    CDeviceInfo di;
    di.SetSerialNumber( camera[ind].sn );
    di.SetDeviceClass( BaslerGigEDeviceClass);

    UINT camn = 0;
    sscanf( camera[ind].sn, "%d", &camn );
    crcam += camn ^ ind;

    static CTlFactory& TlFactory = CTlFactory::GetInstance();
    TRACE("Basler_initialize[%d] - CTlFactory::GetInstance() executed\n", ind );
    ITransportLayer* pTl = TlFactory.CreateTl( Camera_t::DeviceClass() );

    camera[ind].device = pTl->CreateDevice(di);
    camera[ind].camera = new CInstantCamera( camera[ind].device );

 /*   #ifdef AUTORECONNECT_CAMERA

      // For demonstration purposes only, register another configuration event handler that handles device removal.
      camera[ind].camera->RegisterConfiguration( new CSampleConfigurationEventHandler, RegistrationMode_Append, Cleanup_Delete);

    #endif // AUTORECONNECT_CAMERA*/


    /*
    mbstowcs_s(&convertedChars, txt, 200, camera[ind].camera->GetDeviceInfo().GetModelName(), _TRUNCATE); // conversione char=>wchar_t
    TRACE( L"Created camera[%d] <%s> - ", ind, txt ); // camera[ind].camera->GetDeviceInfo().GetModelName() );
    mbstowcs_s(&convertedChars, txt, 200, camera[ind].camera->GetDeviceInfo().GetSerialNumber(), _TRUNCATE); // conversione char=>wchar_t
    TRACE( L"SN <%s>\n", txt ); // camera[ind].camera->GetDeviceInfo().GetModelName() );
    */

    TRACE( L"Created camera[%d] <%s> - ", ind, AtoW( camera[ind].camera->GetDeviceInfo().GetModelName() ) );
    TRACE( L"SN <%s>\n", AtoW( camera[ind].camera->GetDeviceInfo().GetSerialNumber() ) );


    CIntegerPtr pHeartbeat = camera[ind].device->GetTLNodeMap()->GetNode("HeartbeatTimeout");
    if ( pHeartbeat != NULL )
    {
      TRACE( L"Initial Heartbeat value=%.1lf sec - ", 0.001*pHeartbeat->GetValue() );
      pHeartbeat->SetValue(60*1000);
      TRACE( L"new value=%.1lf sec\n", 0.001*pHeartbeat->GetValue() );
    }

    /*
    // assign custom bufferfactory
    camera[ind].camera->SetBufferFactory( buffact );
    TRACE("Basler_initialize[%d]: Camera assigned to custom BufferFactory\n", ind );

    camera[ind].camera->MaxNumBuffer = 5;
    // TRACE("Basler_initialize[%d]: Camera buffer number set to %d\n", ind, camera[ind].camera->MaxNumBuffer );
    */

    // assign custom bufferfactory to area scan cameras
    if ( camera[ind].frame_rate<1000 )
    {
      camera[ind].camera->SetBufferFactory( buffact );
      TRACE("Basler_initialize[%d]: Camera assigned to custom BufferFactory\n", ind );
      //camera[ind].camera->MaxNumBuffer = 5;
      
//      camera[ind].camera->MaxNumBuffer = 5;
//      camera[ind].camera->MaxNumBuffer = 10;
      camera[ind].camera->MaxNumBuffer = 20;

/*      #ifdef SAVE_IMAGES
        if ( IS_PC1 && ind>=3 && ind<=6 ) camera[ind].camera->MaxNumBuffer = 30;
        if ( IS_PC2 && ind>=0 && ind<=3 ) camera[ind].camera->MaxNumBuffer = 30;
      #endif */

      // TRACE("Basler_initialize[%d]: Camera buffer number set to %d\n", ind, camera[ind].camera->MaxNumBuffer );
    }
    else
    {
      TRACE("Saltato %d per lineare!\n", ind );
    }

    camera[ind].camera->Open();
    TRACE( L"Camera[%d] open..\n", ind );

    ASSERT( camera[ind].camera           );
    ASSERT( camera[ind].camera->IsOpen() );

    CIntegerPtr pPacketSize = camera[ind].device->GetNodeMap()->GetNode( "GevSCPSPacketSize" );
    if ( pPacketSize != NULL )
    {
      TRACE( L"Initial GevSCPSPacketSize value=%d bytes - ", pPacketSize->GetValue() );
      pPacketSize->SetValue(8192);
      TRACE( L"new value=%d bytes\n", pPacketSize->GetValue() );
    }  


    CIntegerPtr GainRaw = camera[ind].device->GetNodeMap()->GetNode("GainRaw");
    ASSERT( GainRaw );
    GainRaw->SetValue( GainRaw->GetMin() );
    TRACE("GainRaw set to minimum value %d\n", GainRaw->GetMin() );


    // Enable chunks in general.
    CBooleanPtr ChunkEnable = camera[ind].device->GetNodeMap()->GetNode("ChunkModeActive");
    if ( ChunkEnable && IsWritable( ChunkEnable ) )
    {
      ChunkEnable->SetValue( true );
    }
    else
    {
      throw RUNTIME_EXCEPTION( "The camera doesn't support chunk features");
    }

    // Enable timestamps (warning: NO LINEAR!)

    if ( camera[ind].frame_rate<1000 )
    {
      CEnumerationPtr chunksel = camera[ind].device->GetNodeMap()->GetNode("ChunkSelector");
      if ( chunksel && IsWritable( chunksel ) /*&&camera[ind].*/ )
      {
        chunksel->SetIntValue( ChunkSelector_Framecounter );
        chunksel->SetIntValue( ChunkSelector_Timestamp    );
      }
    }
    
  }
  catch (GenICam::GenericException &e)
  {
    // Error handling
    //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"Genicam exception thrown <%s>\n", msg_err );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"Basler_initialize: %s\n", msg_err );
    return 99;
  }



  return( 0 );
}


int Basler_display( UINT ind )
{
  CString txt;

  ASSERT( ind>=0 && ind<num_tlc );

  ASSERT( camera[ind].textnum <= MAXNUMTEXTROWS );

  image_dsp[ind] = new CImage_display( ind );

 	CClientDC dc(pView);

  CRect area_txt;

  int hrow   = 20;
  int hspace =  5;


  area_txt.top    = camera[ind].dsporg.y + camera[ind].dspdim.cy + hspace;
  area_txt.bottom = area_txt.top + hrow;
  area_txt.left   = camera[ind].dsporg.x;
  area_txt.right  = area_txt.left + camera[ind].dspdim.cx;

    //Pour added for TIVS-A20 for alignment 20180523
#ifdef TIVS_A20
  if(ind==3||ind==4||ind==5||ind==6)//P1 and P2
  {
  area_txt.top    = camera[ind].dsporg.y + camera[ind].dspdim.cy + hspace + 1;
  area_txt.bottom = area_txt.top + hrow;
  }
#endif
  //Pour added for TIVS-A20 for alignment 20180523

  if ( camera[ind].textwidth ) area_txt.InflateRect( (camera[ind].textwidth-area_txt.Width())/2, 0 );
//  TRACE( "req=%d - curr=%d\n",  camera[ind].textwidth, area_txt.Width() );

  for ( int i=0 ; i<camera[ind].textnum ; i++ )
  {
    // if already existing, delete it
    if ( camera[ind].text[i] )
    {
      delete camera[ind].text[i];
      camera[ind].text[i] = NULL;
    }

    if ( lng==CHINESE ) camera[ind].text[i] = new CText_box( TEXT_ROWS_FONT_CHN, area_txt, pView, SS_CENTER|SS_CENTERIMAGE );
    else                camera[ind].text[i] = new CText_box( TEXT_ROWS_FONT_ENG, area_txt, pView, SS_CENTER|SS_CENTERIMAGE );
    camera[ind].text[i]->colore_sfondo = ::GetSysColor( COLOR_INACTIVEBORDER ); //COLORE_CONTAIN;
    camera[ind].text[i]->ShowWindow( SW_HIDE );

    #ifdef FILLEV
      camera[ind].text[i]->ShowWindow( SW_SHOW );
    #endif

    #ifdef STOPPER_CTRL
      camera[ind].text[i]->ShowWindow( SW_SHOW );
    #endif

    #ifdef TIVS_A60
      if ( ( IS_PC1 && ind>=tlcgroupPC1[1][0] ) || ( IS_PC2 ) )
      {
        if ( i==3 ) // only lowest row
        {
          camera[ind].text[i]->font.DeleteObject();

          camera[ind].text[i]->font.CreateFont(  lng==CHINESE?10:12,      // Height
                                                  0,      // Width
                                                  0,      // Escapement
                                                 10,      // Orientation
                                                400,      // Weight 100-900
                                              FALSE,      // Italic
                                              FALSE,      // Underline
                                                  0,      // StrikeOut
                                       ANSI_CHARSET,      // CharSet
                                 OUT_DEFAULT_PRECIS,      // OutPrecision
                                CLIP_DEFAULT_PRECIS,      // ClipPrecision
                                    DEFAULT_QUALITY,      // Quality
                           DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                     lng==CHINESE?L"SimSun":L"Arial" );
  
          camera[ind].text[i]->SetFont( &camera[ind].text[i]->font );
        }
      }
    #endif

//    if ( i ) camera[ind].text[i]->SetText( L"ABCDEFGHIJKLMNOPQ" ); // L"-" );
    if ( i ) camera[ind].text[i]->SetText( L"-" );
    else
    {
      txt.Format( string_800[lng], ind, camera[ind].name );
      camera[ind].text[i]->SetText( txt );
    }

    area_txt.top    = area_txt.bottom + hspace;
    area_txt.bottom = area_txt.top    + hrow;
  }

  return 0;
}


int Basler_parameters( UINT ind )
{
//  wchar_t  txt[200];
//  size_t convertedChars = 0;

  if ( !camera[ind].camera ) return 99; // camera not initialized!

  TRACE("Basler_parameters[%d]...\n", ind);

  ASSERT( ind>=0 && ind<num_tlc        );

  if ( !camera[ind].camera || !camera[ind].camera->IsOpen() )
  {
    swprintf( msg_err, 200, L"Camera not open!" );
    TRACE("%s\n",msg_err);
    return 99;
  }

  ASSERT( camera[ind].camera           );
  ASSERT( camera[ind].camera->IsOpen() );

  try
  {
    INodeMap* nodemap = camera[ind].device->GetNodeMap();

    //CIntegerPtr widthmax  ( nodemap.GetNode( "WidthMax"  ) );
    //CIntegerPtr heightmax ( nodemap.GetNode( "HeightMax" ) );
    //TRACE("Min/max width/height %I64d-%I64d\n",camera[ind].width_max, camera[ind].height_max );
//    TRACE("Min/max width/height %d-%d %d-%d\n", width->GetMin(), , height->GetMin(), height->GetMax() );

    // FIRST ADJUST X/Y OFFSETS TO ZERO!
    CIntegerPtr offsetX( nodemap->GetNode( "OffsetX" ) );
    if ( offsetX && IsWritable( offsetX ) )
    {
      offsetX->SetValue( 0 );
      ASSERT( offsetX->GetValue()==0 );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <OffsetX> node\n", ind );

    CIntegerPtr offsetY( nodemap->GetNode( "OffsetY" ) );
    if ( offsetY && IsWritable( offsetY ) )
    {
      offsetY->SetValue( 0 );
      ASSERT( offsetY->GetValue()==0 );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <OffsetY> node\n", ind );

    // THEN ADJUST WIDTH/HEIGHT AS REQUIRED
    CIntegerPtr width( nodemap->GetNode( "Width" ) );
    if ( width && IsWritable( width ) )
    {
      camera[ind].width_max  = width->GetMax();
      width->SetValue( camera[ind].width );
      ASSERT( width->GetValue()==camera[ind].width );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <Width> node\n", ind );

    CIntegerPtr height( nodemap->GetNode( "Height" ) );
    if ( height && IsWritable( height ) )
    {
      camera[ind].height_max = height->GetMax();
      height->SetValue( camera[ind].height );
      ASSERT( height->GetValue()==camera[ind].height );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <Height> node\n", ind );

    // THEN ADJUST X/Y OFFSETS AS REQUIRED!
//    CIntegerPtr offsetX( nodemap->GetNode( "OffsetX" ) );
    if ( offsetX && IsWritable( offsetX ) )
    {
      offsetX->SetValue( camera[ind].offsetx );
      ASSERT( offsetX->GetValue()==camera[ind].offsetx );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <OffsetX> node\n", ind );

//    CIntegerPtr offsetY( nodemap->GetNode( "OffsetY" ) );
    if ( offsetY && IsWritable( offsetY ) )
    {
      offsetY->SetValue( camera[ind].offsety );
      ASSERT( offsetY->GetValue()==camera[ind].offsety );
    }
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <OffsetY> node\n", ind );



    ASSERT( 0<=camera[ind].offsetx && camera[ind].offsetx+camera[ind].width <=camera[ind].width_max  );
    ASSERT( 0<=camera[ind].offsety && camera[ind].offsety+camera[ind].height<=camera[ind].height_max );

    CFloatPtr shutter( nodemap->GetNode( "ExposureTimeAbs" ) );
    if ( shutter && IsWritable( shutter ) )
    {
      shutter->SetValue( camera[ind].shutter );
      ASSERT( shutter->GetValue()==camera[ind].shutter );
      TRACE("Shutter set to %.0f us\n", shutter->GetValue());
    }  
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <ExposureTimeAbs> node\n", ind );

    //INodeMap* nodemap = camera[ind].device->GetNodeMap();
    CEnumerationPtr trigger_mode( nodemap->GetNode( "TriggerMode" ) );
    if ( trigger_mode && IsWritable( trigger_mode ) )
    {
      if ( camera[ind].trigger )
      {
        trigger_mode->SetIntValue( TriggerMode_On );
        ASSERT( trigger_mode->GetIntValue() == TriggerMode_On  );
        TRACE("TriggerMode set to <ON>\n" );
      }
      else
      {
        trigger_mode->SetIntValue( TriggerMode_Off );
        ASSERT( trigger_mode->GetIntValue() == TriggerMode_Off  );
        TRACE("TriggerMode set to <OFF>\n" );
      }

      if ( camera[ind].light_out && AfxIsValidString(camera[ind].light_out) )
      {
        ADS_data_type light_out;
        if ( !ADS_init_parameter( camera[ind].light_out, &light_out, 1 ) )
        {
          ADS_write_byte( &light_out, camera[ind].trigger?0:1 );
          ADS_clean_parameter( &light_out );
        }
      }

    }  
    else TRACE("Basler_parameters[%d] ERROR: Unable to access <TriggerMode> node\n", ind );



    CEnumerationPtr scan_type( nodemap->GetNode( "DeviceScanType" ) );
    if ( scan_type )
    {

      // * * * * * * * * * *      AREA SCAN CAMERA     * * * * * * * * * * 
      if ( scan_type->GetIntValue() == DeviceScanType_Areascan ) 
      {
        TRACE("Area scan camera\n");

        CBooleanPtr framerateenable( nodemap->GetNode( "AcquisitionFrameRateEnable" ) );
        if ( framerateenable && IsWritable( framerateenable ) )
        {
          if ( camera[ind].frame_rate > 0.0 )
          {
            framerateenable->SetValue( TRUE );
            ASSERT( framerateenable->GetValue() );

            CFloatPtr framerate( nodemap->GetNode( "AcquisitionFrameRateAbs" ) );
            if ( framerate && IsWritable( framerate ) )
            {
              framerate->SetValue( camera[ind].frame_rate );
              TRACE("Frame rate requested: %.3f - read %.3f - ratio=%.3f\n", camera[ind].frame_rate, framerate->GetValue(), framerate->GetValue()/camera[ind].frame_rate-1.0 );
              ASSERT( abs(framerate->GetValue()/camera[ind].frame_rate-1.0)<0.01 );
            }
            else TRACE("Basler_parameters[%d] ERROR: Unable to access <AcquisitionFrameRateAbs> node\n", ind );
          }
          else
          {
            framerateenable->SetValue( FALSE );
            ASSERT( !framerateenable->GetValue() );
            TRACE("FrameEnable=%d\n", framerateenable->GetValue() );
          }
        }  
        else TRACE("Basler_parameters[%d] ERROR: Unable to access <AcquisitionFrameRateEnable> node\n", ind );

        if ( camera[ind].strobe )
        {
          ASSERT( !camera[ind].output1 ); // can't be together TRUE in ACE
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
        }


        if ( camera[ind].output1 )
        {
          ASSERT( !camera[ind].strobe ); // can't be together TRUE in ACE
          CEnumerationPtr userout_selector( nodemap->GetNode( "UserOutputSelector" ) );
          if ( userout_selector && IsWritable( userout_selector ) )
          {
            userout_selector->SetIntValue( UserOutputSelector_UserOutput1 );
            ASSERT( userout_selector->GetIntValue() == UserOutputSelector_UserOutput1 );
            TRACE("UserOutputSelector set to UserOutputSelector_UserOutput1\n" );
          }
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <UserOutputSelector> node\n", ind );

          
          CBooleanPtr userout_value( nodemap->GetNode( "UserOutputValue" ) );
          if ( userout_value && IsWritable( userout_value ) ) camera[ind].userout_ptr = userout_value;

          /*
          CBooleanPtr userout_value( nodemap->GetNode( "UserOutputValue" ) );
          if ( userout_value && IsWritable( userout_value ) )
          {
            userout_value->SetValue( TRUE );
            ASSERT( userout_value->GetValue() == TRUE );
            TRACE("UserOutputValue set to TRUE\n" );
          }
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <UserOutputValue> node\n", ind );
          */


       /*   CEnumerationPtr line_source( nodemap->GetNode( "LineSource" ) );
          if ( line_source && IsWritable( line_source ) )
          {
            line_source->SetIntValue( LineSource_UserOutput1 );
            ASSERT( line_source->GetIntValue() == LineSource_UserOutput1 );
            TRACE("LineSource set to LineSource_UserOutput1\n" );
          }
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSource> node\n", ind ); */

        }

        if ( camera[ind].sequence )
        {
          CEnumerationPtr trigger_selector( nodemap->GetNode( "TriggerSelector" ) );
          if ( trigger_selector && IsWritable( trigger_selector ) )
          {
            trigger_selector->SetIntValue( TriggerSelector_AcquisitionStart );
            ASSERT( trigger_selector->GetIntValue() == TriggerSelector_AcquisitionStart );
            TRACE("TriggerSelector set to <TriggerSelector_AcquisitionStart>\n" );
          }  
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <TriggerSelector> node\n", ind );

          if ( trigger_mode && IsWritable( trigger_mode ) )
          {
            trigger_mode->SetIntValue( TriggerMode_On );
            ASSERT( trigger_mode->GetIntValue() == TriggerMode_On  );
            TRACE("TriggerMode set to <ON>\n" );
          }
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <TriggerMode> node\n", ind );

          CEnumerationPtr trigger_source( nodemap->GetNode( "TriggerSource" ) );
          if ( trigger_source && IsWritable( trigger_source ) )
          {
            trigger_source->SetIntValue( TriggerSource_Software );
            ASSERT( trigger_source->GetIntValue() == TriggerSource_Software  );
            TRACE("TriggerSource set to <TriggerSource_Software>\n" );
          }
          else TRACE("Basler_parameters[%d] ERROR: Unable to access <TriggerSource> node\n", ind );
        }

      } // AREA SCAN CAMERA


      // * * * * * * * * * *    LINE SCAN CAMERA    * * * * * * * * * * 
      if ( scan_type->GetIntValue() == DeviceScanType_Linescan ) 
      {
        TRACE("Line scan camera\n");

        CEnumerationPtr acq_mode( nodemap->GetNode( "AcquisitionMode" ) );
        if ( acq_mode && IsWritable( acq_mode ) )
        {
          acq_mode->SetIntValue( AcquisitionMode_SingleFrame );
          ASSERT( acq_mode->GetIntValue() == AcquisitionMode_SingleFrame );
          TRACE("AcquisitionMode set to AcquisitionMode_SingleFrame\n" );
        }
        else TRACE("Basler_parameters[%d] ERROR: Unable to access <AcquisitionMode> node\n", ind );
        
        CFloatPtr linerate( nodemap->GetNode( "AcquisitionLineRateAbs" ) );
        if ( linerate && IsWritable( linerate ) )
        {
          linerate->SetValue( camera[ind].frame_rate );
          TRACE("Line rate requested: %.3f - read %.3f - ratio=%.3f\n", camera[ind].frame_rate, linerate->GetValue(), linerate->GetValue()/camera[ind].frame_rate-1.0 );
          ASSERT( abs(linerate->GetValue()/camera[ind].frame_rate-1.0)<0.01 );
        }
        else TRACE("Basler_parameters[%d] ERROR: Unable to access <AcquisitionLineRateAbs> node\n", ind );
      } // LINE SCAN CAMERA

    }

   /*   CEnumerationPtr line_source( nodemap->GetNode( "LineSource" ) );
      if ( line_source && IsWritable( line_source ) )
      {
        line_source->SetIntValue( LineSource_ExposureActive );
        ASSERT( line_source->GetIntValue() == LineSource_ExposureActive );
        TRACE("LineSource set to LineSource_ExposureActive\n" );
      }
      else TRACE("Basler_parameters[%d] ERROR: Unable to access <LineSource> node\n", ind ); */

    /*
      // Continuous mode, no external trigger used
      Camera[ind]->TriggerSelector.SetValue( TriggerSelector_AcquisitionStart );
      Camera[ind]->TriggerMode.SetValue( TriggerMode_Off );
      Camera[ind]->AcquisitionMode.SetValue(AcquisitionMode_Continuous);

      // Parameterize the stream grabber
      const int bufferSize = (int) Camera[ind]->PayloadSize();
      const int numBuffers = 10;
      StreamGrabber[ind]->MaxBufferSize = bufferSize;
      StreamGrabber[ind]->MaxNumBuffer  = numBuffers;
      //TRACE("StreamGrabber[%d] impostati MaxBufferSize=%d e MaxNumBuffer=%d\n", ind, bufferSize, numBuffers );    
      TRACE( L"StreamGrabber[%d] impostati MaxBufferSize=%d e MaxNumBuffer=%d\n", ind, bufferSize, numBuffers );    

      tlc_status[ind] = TS_TLC_OK; // Telecamera inizializzata correttamente */
  }
  catch (GenICam::GenericException &e)
  {
    // Error handling
//    mbstowcs_s(&convertedChars, txt, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
//    TRACE( L"Genicam exception thrown <%s>\n", e.GetDescription() );
    //TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"Basler_parameters: %s\n", msg_err );

    return 99;
  }


  return 0;
}



int set_camera_output( UINT ind, bool val )
{
//  size_t  convertedChars = 0;
//  wchar_t txt[200];

  ASSERT( ind>=0 && ind<num_tlc );
  
  if ( !camera[ind].camera )
  {
    return 99;  // camera not ready/initialized
  }

  try
  {
 //   INodeMap* nodemap = camera[ind].device->GetNodeMap();
 //   CBooleanPtr userout_value( nodemap->GetNode( "UserOutputValue" ) );
 //   if ( userout_value && IsWritable( userout_value ) )
    if ( camera[ind].userout_ptr && IsWritable( camera[ind].userout_ptr ) )
    {
      camera[ind].userout_ptr->SetValue( val );  //  userout_value->SetValue( val );
      ASSERT( camera[ind].userout_ptr->GetValue() == val ); // ASSERT( userout_value->GetValue() == val );
      //TRACE("UserOutputValue set to %d\n", val );
    }
    else
    {
      TRACE("Basler_parameters[%d] ERROR: Unable to access <UserOutputValue> node\n", ind );
      return( 199 );
    }
  }
  catch (GenICam::GenericException &e)
  {
    // Error handling
    //mbstowcs_s(&convertedChars, txt, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"Genicam exception thrown <%s>\n", e.GetDescription() );
    //TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"set_camera_outputs: %s\n", msg_err );
    return 99;
  }


  return 0;
}


int Stop_ifcamerror( UINT ind )
{
  if ( !stop_cameraerror ) return 1; // skip if is disabled - if TRUE enable stopping of machine when camera error happens
  

  if ( !ADS_status || !alarm_ads.handle ) return 99; // Communication NOT active or variable NOT initialized 

  alarm_data->on     = TRUE;

  alarm_data->format =   10; // single %d parameter
  alarm_data->param_0 = ind; // camera number

//  alarm_data->format =                30; // single parameter
//  alarm_data->param_4 = camera[ind].name; // camera number

  alarm_data->index  =   57; // error message index

  ADS_write_parameter( &alarm_ads );

/*   

    case 0:  // only string
    case 10:  // string + %d integer (param_0)
    case 11:  // string + %d integer (param_0) + %d integer (param_1)
    case 20:  // string + %.3lf double (param_2)
    case 21:  // string + %.3lf double (param_2) + %.3lf double (param_3)
    case 30:  // string + %s string[20] (param_4)

if ( !ADS_read_parameter( &alarm_ads ) )
      {
        if ( alarm_data->on ) // Allarme attivo!
        {
          txt = prepare_alarm_string(alarm_data);
          alarm_txt->SetText( txt, 0, TXT_RED );
        }
        else
        {
          txt = alarm_msg[lng];  // no pending alarm
          alarm_txt->SetText( txt, 0, TXT_GREEN );
        } */

//        swprintf( msg_old, 200, L"inspect_3d_particle[%s] - genicam exception thrown <%s>\n", camera[ind].name, AtoW(e.GetDescription()) );
  return 0;
}


void Add_camerror_log ( UINT ind, const char* text )
{
  if ( logger )
    {
      swprintf( msg_err, 200, L"Camera[%d] genicam exception thrown <%s>", ind, AtoW(text)) ;
      logger->Add_string( msg_err );
    }
  return;
}



/*
void Basler_init_old( )
{ 
  Pylon::PylonInitialize();
  //TRACE("Pylon::PylonInitialize() executed\n");
  TRACE( L"Pylon::PylonInitialize() executed\n");

//  Message_box( "BASLER 1" );

  // Get the transport layer factory
  CTlFactory& TlFactory = CTlFactory::GetInstance();
  //TRACE("CTlFactory::GetInstance() executed\n");
  TRACE( L"CTlFactory::GetInstance() executed\n");

  // Create the transport layer object needed to enumerate or
  ITransportLayer *pTl = NULL;

  // create a camera object of type Camera_t::DeviceClass()
  pTl = TlFactory.CreateTl(Camera_t::DeviceClass());

  // Exit the application if the specific transport layer is not available
  if ( !pTl ) ASSERT("Failed to create transport layer!");




  // Get all attached cameras and exit the application if no camera is found
  DeviceInfoList_t devices;
  if ( pTl->EnumerateDevices(devices) == 0 )
//  if ( TlFactory.EnumerateDevices(devices) == 0 )
  {
    //if ( strcmp( PC_name, "M70 (debug)" )) Message_box ( "Basler camera not found!", MBOX_ERROR );
    if ( wcscmp( PC_name, L"M70 (debug)" )) Message_box ( L"Basler camera not found!", MBOX_ERROR );
    //Camera_error = TRUE;
    return ;
  }
  //TRACE("%d BASLER devices found\n", TlFactory.EnumerateDevices(devices));
  TRACE( L"%d BASLER devices found\n", TlFactory.EnumerateDevices(devices));


//  for ( int ind=0 ; ind<NUM_TLC ; ind++ ) Camera[ind] = NULL;


//  String_t tlc_sn[2];

//  for ( int ind=0 ; ind<pTl->EnumerateDevices(devices) ; ind++ )
//  for ( int ind=0 ; ind<TlFactory.EnumerateDevices(devices) ; ind++ )




  // TLC 21244497 SU CAMERA[0] (BODY)
//  for ( int ind=0 ; ind<TlFactory.EnumerateDevices(devices) ; ind++ )


  for ( int ind=0 ; ind<TlFactory.EnumerateDevices(devices) ; ind++ )
  {
    CDeviceInfo info;

    if ( ind==0 ) info.SetSerialNumber( "21293367" ); // [0] - body (telecamera originale/nuova!)
    //else          info.SetSerialNumber( "21244497" ); // [1] - neck

//    Camera[ind] = new Camera_t;
    //TRACE("Camera[%d] created..\n", ind );
    TRACE( L"Camera[%d] created..\n", ind );
    
    try
    {
//      Camera[ind]->Attach( pTl->CreateDevice( info ) );
      //TRACE("Camera[%d] attached..\n", ind );
      TRACE( L"Camera[%d] attached..\n", ind );
    }
    catch (GenICam::GenericException &e) // Error handling
    {
//      delete Camera[ind];
//      Camera[ind] = NULL;
      wchar_t  txt[200];
      size_t convertedChars = 0;
      mbstowcs_s(&convertedChars, txt, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE("Sizeoftxt=%d\n", sizeof(txt) );
      Message_box( txt, MBOX_ERROR );
    }


    
//    if ( Camera[ind] && Camera[ind]->IsAttached() && !Camera[ind]->IsOpen() )
    {
      try
      {
        //Camera[ind]->Open();
        //TRACE("Camera[%d] open..\n", ind );
        TRACE( L"Camera[%d] open..\n", ind );

        //GenApi::CIntegerPtr pHeartbeat = Camera[ind]->GetTLNodeMap()->GetNode("HeartbeatTimeout");
      }
      catch (GenICam::GenericException &e)
      {
        // Error handling
        //TRACE( "Error %s\n", e.GetDescription() );
        TRACE( L"Error %s\n", e.GetDescription() );
        //AfxMessageBox( e.GetDescription(), MB_OK);
      }
    }
    

    tlc_status[ind] = 0;

   // }
  }

 // if (pTl) TlFactory.ReleaseTl(pTl);
  //TRACE("Rilasciato <TlFactory>\n" );    
//  TRACE( L"Rilasciato <TlFactory>\n" );    


} // Basler_init

*/

//////////////////////////////////////////////////////////////////////////////

void Basler_terminate()
{ 
  
  //if ( !Camera_error )

  CloseHandle(mutex);
  mutex = NULL;


  for ( int ind=0 ; ind<NUM_TLC ; ind++ )
  {

    if ( camera[ind].camera )
    {
      delete camera[ind].camera;
    }

    // Close stream grabber
/*    if ( StreamGrabber[ind] )
    {
      StreamGrabber[ind]->Close();
      delete StreamGrabber[ind];
      StreamGrabber[ind] = NULL;
      //TRACE ( "StreamGrabber[%d] deleted\n",ind );
      TRACE ( L"StreamGrabber[%d] deleted\n", ind );
    }

    if ( Camera[ind] ) 
    {
      Camera[ind]->Close();
      delete Camera[ind];
      Camera[ind] = NULL;
      //TRACE ( "Camera[%d] deleted\n",ind );
      TRACE ( L"Camera[%d] deleted\n", ind );
    } */


/*    if ( background[ind] ) 
    {
      _aligned_free( background[ind] );
      background[ind] = NULL;
      TRACE("background[%d] deallocated\n", ind );
    }*/

  }
   
  PylonTerminate();


  if ( buffact ) 
  {
    delete buffact;
    buffact = 0;
  }

  //TRACE("PylonTerminate() executed\n");
  TRACE( L"PylonTerminate() executed\n" );

}





void Show_hide( int sta, int end )
{
  ASSERT( 0<=sta && sta<=end && end<NUM_TLC );

  for ( int i=0 ; i<NUM_TLC ; i++ )
  {
    if ( i>=sta && i<=end ) // in between => show
    {
      if ( image_dsp[i] ) image_dsp[i]->visible = TRUE;
      for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
    }
    else
    {
      if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
      for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
    }
  }
  pView->Invalidate(TRUE);
}





void Show_hide( UINT group )
{
  ASSERT( 0<=group && group<6 );//Pour marked on 20200812
  static CRect area_tot;
  UINT   i=0;

  if ( group==0 ) // SHOW GROUP 0 (LAYOUT)
  {
    if ( layout_dsp ) layout_dsp->visible = TRUE;
    for ( i=0 ; i<NUM_TLC ; i++ )
    {
      if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
      for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
    }

    for ( i=0 ; i<uplist_num ; i++ )
    {
      if ( uplist[i].item_rt ) uplist[i].item_rt->ShowWindow( SW_SHOW );
    }

    // resize container
    if ( area_tot.Width()==0 )
    {
      pView->container_main->GetWindowRect( &area_tot ); // first time get and keep original
    }

    CRect area = area_tot;

	 #if APP_RESY==1024 // standard display 1280x1024
      area.DeflateRect( 0 , 0 , 0 , 200 );
    #endif

    #if APP_RESY==1080 // FullHD for S40
//      area.InflateRect( 0 , 0 , 0 , 200 );
    #endif

    //area.DeflateRect( 0 , 0 , 0 , 200 );

    pView->container_main->MoveWindow( area );
  }
  else // some other group of camera images
  {
    if ( layout_dsp )
    {
      layout_dsp->visible = FALSE;
      pView->container_main->MoveWindow( area_tot );
    }

    for ( i=0 ; i<uplist_num ; i++ )
    {
      if ( uplist[i].item_rt ) uplist[i].item_rt->ShowWindow( SW_HIDE );
    }


    UINT sta=tlcgroupPC1[group-1][0];
    UINT end=tlcgroupPC1[group-1][1];

    if ( IS_PC2 ) 
    {
      sta=tlcgroupPC2[group-1][0];
      end=tlcgroupPC2[group-1][1];

      #ifdef TIVS_F40
        #if STN1==CAKESIDE_ARBACK  
        #endif
        #if STN3==CAKETOP_ARDIRECT  
           if ( group>2 )
           {
             sta =  8;
             end = 11;
           }
        #endif
        #ifdef NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
        #endif
      #endif // TIVS_F40
    }

    #ifdef TIVS_A20L
      if ( group==1 )
      {
        sta =  0;
        end =  2;
      }
      if ( group==2 )
      {
        sta =  3;
        end =  8;
      }
    #endif // TIVS_A20L

//Pour added 20180523

	  #ifdef TIVS_A20
      if ( group==1 )
      {
        sta =  0;
        end =  2;
      }
      if ( group==2 )
      {
        sta =  3;
        end =  8;
      }
    #endif // TIVS_A20

//Pour added 20180523

	  //Pour added 20180523

	  #ifdef TIVS_F20
      if ( group==1 )
      {
        sta =  0;
        end =  2;
      }
      if ( group==2 )
      {
        sta =  3;
        end =  5;
      }

	  if ( group==3 )
      {
        sta =  6;
        end =  7;
      }

	  if ( group==4 )
      {
        sta =  8;
        end =  9;
      }
	  
	  if ( group==5 )
      {
        sta =  10;
        end =  11;
      }

    #endif // TIVS_F20

//Pour added 20180523


/*
//Pour added 20180124
    #ifdef TIVS_A40L
      if ( group==1 )
      {
        sta =  0;
        end =  2;
      }
      if ( group==2 )
      {
        sta =  3;
        end =  8;
      }
    #endif // TIVS_A20L

//Pour added 20180124
*/

    //TRACE("GROUP=%d - sta=%d - end=%d\n", group, sta, end );

    // FINALLY SHOW/HIDE SUITABLE CAMERAS/IMAGES/TEXTS
    for ( UINT i=0 ; i<NUM_TLC ; i++ )
    {

      //if ( i>=tlcgroupPC1[group-1][0] && i<=tlcgroupPC1[group-1][1] ) // in between => show
      if ( i>=sta && i<=end ) // in between => show
      {
        if ( image_dsp[i] ) image_dsp[i]->visible = TRUE;
        for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
      }
      else
      {
        if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
        for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
      }
    }
  }

  pView->Invalidate(TRUE);
}



/*
void Hide_tip( int ind )
{
  for ( int i=0 ; i<3 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
  }
  pView->Invalidate(TRUE);
}

void Show_tip( int ind )
{
  for ( int i=0 ; i<3 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
  }
  pView->Invalidate(TRUE);
}

void Hide_part1( int ind )
{
  for ( int i=3 ; i<7 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
  }
  pView->Invalidate(TRUE);
}

void Show_part1( int ind )
{
  for ( int i=3 ; i<7 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
  }
  pView->Invalidate(TRUE);
}

void Hide_part2( int ind )
{
  for ( int i=0 ; i<4 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
  }
  pView->Invalidate(TRUE);
}

void Show_part2( int ind )
{
  for ( int i=0 ; i<4 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
  }
  pView->Invalidate(TRUE);
}

void Hide_cosm( int ind )
{
  for ( int i=4 ; i<8 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_HIDE );
  }
  pView->Invalidate(TRUE);
}

void Show_cosm( int ind )
{
  for ( int i=4 ; i<8 ; i++ )
  {
    if ( image_dsp[i] ) image_dsp[i]->visible = FALSE;
    for ( int j=0 ; j<MAXNUMTEXTROWS ; j++ ) if ( camera[i].text[j] ) camera[i].text[j]->ShowWindow( SW_SHOW );
  }
  pView->Invalidate(TRUE);
}

*/







void Set_rawgain( int ind, int val )
{
/*  if ( ind>=0 && ind<NUM_TLC && val>=0 && val<=500 )
  {
    if ( Camera[ind] )
    {
      Camera[ind]->GainRaw.SetValue( val );
      //TRACE("GainRaw of camera[%d] set to: %d units\n", ind, val );
      TRACE( L"GainRaw of camera[%d] set to: %d units\n", ind, val );
    }
  } */
}

void Set_averaging( int ind, int val )
{
/*  if ( ind>=0 && ind<NUM_TLC && val>=1 && val<=16 )
  {
    if ( Camera[ind] )
    {
      Camera[ind]->AveragingNumberOfFrames.SetValue( val );
      //TRACE("AveragingNumberOfFrames of camera[%d] set to: %d units\n", ind, val );
      TRACE( L"AveragingNumberOfFrames of camera[%d] set to: %d units\n", ind, val );
    }
  } */
}


// Esegue la funzione MIN(SOURCE,DEST)=>DEST
// puntatori e dimensione devono essere multipli di/allineati a 16 byte
// UNSIGNED - SATURATED - PACKED - BYTES
void Get_min( BYTE* dest, BYTE* source, UINT dim )
{
	ASSERT( (      dim    &15) == 0 );
	ASSERT( ((UINT)source &15) == 0 );
	ASSERT( ((UINT)dest   &15) == 0 );
	ASSERT( AfxIsValidAddress( dest,   dim ) );
	ASSERT( AfxIsValidAddress( source, dim ) );

	__asm
	{
		mov		   esi, source      ; Load source address into esi
		mov		   edi, dest        ; Load destination address into edi
		mov		   ebx, dim         ; Load dimension into ebx
		xor		   ecx, ecx         ; Clear ecx
		Loop1:                    ; Loop start
		movdqa   xmm0, [esi+ecx]  ; Load next 16 bytes of source into xmm0
		pminub	 xmm0, [edi+ecx]  ; min(xmm0,dest) => xmm0
		movdqa	[edi+ecx], xmm0   ; xmm0 => dest
		add  	   ecx, 16          ; update ecx
		cmp		   ecx, ebx         ; completed?
		jne    	 Loop1            ; if not, run again
		emms                      ; end and terminate MMX mode 
	}
}


// Esegue la funzione (SOURCE-DEST)=>DEST
// puntatori e dimensione devono essere multipli di/allineati a 16 byte
// UNSIGNED - SATURATED - PACKED - BYTES
void Get_diff( BYTE* dest, BYTE* source, UINT dim )
{
	ASSERT( (      dim    &15) == 0 );
	ASSERT( ((UINT)source &15) == 0 );
	ASSERT( ((UINT)dest   &15) == 0 );
	ASSERT( AfxIsValidAddress( dest,   dim ) );
	ASSERT( AfxIsValidAddress( source, dim ) );

	__asm
	{
		mov		   esi, source      ; Load source address into esi
		mov		   edi, dest        ; Load destination address into edi
		mov		   ebx, dim         ; Load dimension into ebx
		xor		   ecx, ecx         ; Clear ecx
		Loop2:                    ; Loop start
		movdqa   xmm0, [esi+ecx]  ; Load next 16 bytes of source into xmm0
		psubusb	 xmm0, [edi+ecx]  ; xmm0-dest => xmm0
		movdqa	[edi+ecx], xmm0   ; xmm0 => dest
		add  	   ecx, 16          ; update ecx
		cmp		   ecx, ebx         ; completed?
		jne    	 Loop2            ; if not, run again
		emms                      ; end and terminate MMX mode 
	}
}



/*
int Get_background( int ind_in )
{
  int ind = ind_in&15;

  ASSERT( ind>=0 && ind<NUM_TLC );

  char msg[100];

  if ( background[ind]==NULL ) // allocazione
  {
    background[ind] = (BYTE*) _aligned_malloc( DIMX_ACQ*DIMY_ACQ, 16 );
    TRACE("Allocation of background[%d]\n", ind );
  }

/*  #ifdef TEMP_TLC
  if ( ind_in&256 ) // try to load from disk
  {
    sprintf( msg, "background[%d].png", ind );
    if ( !image_dsp[ind]->Load_image( msg, TRUE ) ) // load silent!
    {
      CopyMemory( background[ind], image_dsp[ind]->ptr_buf, ind==0?1920*1080:DIMX_ACQ*DIMY_ACQ );
      SCALE_IMG ( ind, background[ind] );
      image_dsp[ind]->Draw_image();
      sprintf( msg, "background loaded from disk image <background[%d].png>", ind );
      pView->tlctext_2[ind]->SetText( msg, 0 );
      return 0;
    }
  }
  #else
  if ( ind_in&256 ) // try to load from disk
  {
    sprintf( msg, "background[%d].png", ind );
    if ( !image_dsp[ind]->Load_image( msg, TRUE ) ) // load silent!
    {
      CopyMemory( background[ind], image_dsp[ind]->ptr_buf, DIMX_ACQ*DIMY_ACQ );
      SCALE_IMG ( ind, background[ind] );
      image_dsp[ind]->Draw_image();
      sprintf( msg, "background loaded from disk image <background[%d].png>", ind );
      pView->tlctext_2[ind]->SetText( msg, 0 );
      return 0;
    }
  }
  #endif  */

/*
  if ( ind_in&256 ) // try to load from disk
  {
    sprintf( msg, "%s\\background[%d].png", MAIN_DIRECTORY, ind );
    if ( !image_dsp[ind]->Load_image( msg, TRUE ) ) // load silent!
    {
      CopyMemory( background[ind], image_dsp[ind]->ptr_buf, DIMX_ACQ*DIMY_ACQ );
      SCALE_IMG ( ind, background[ind] );
      image_dsp[ind]->Draw_image();
      sprintf( msg, "background loaded from disk image <background[%d].png>", ind );
      pView->tlctext_2[ind]->SetText( msg, 0 );
      return 0;
    }
  }


  if ( !Camera[ind] ) return 99; // Camera NON inizializzata

  Camera[ind]->AveragingNumberOfFrames.SetValue( 10 ); // fondo acquisito con averaging

  // Acquisizione immagine
  StreamGrabber[ind]->PrepareGrab();

  // Allocate buffer and buffer handle
  UINT dimbuffer = DIMX_ACQ*DIMY_ACQ;

  BYTE* buffer = (uint8_t*) _aligned_malloc( dimbuffer, 16 );
  ASSERT( buffer );
  StreamBufferHandle buffer_handle = StreamGrabber[ind]->RegisterBuffer( buffer, dimbuffer );
  StreamGrabber[ind]->QueueBuffer( buffer_handle, NULL);

  Camera[ind]->AcquisitionStart.Execute();

  GrabResult Result;

  if ( StreamGrabber[ind]->GetWaitObject().Wait(3000) ) // Wait for the grabbed image with timeout of 3 seconds
  {
    if ( !StreamGrabber[ind]->RetrieveResult(Result) ) // Get the grab result from the grabber's result queue
    {
      Message_box("Unable to retrieve grab result");
      return 199;
    }

    if ( Result.Succeeded() ) // Result.Status() == Grabbed )
    {  
      BYTE* img_acq = (BYTE*) Result.Buffer();

      CopyMemory( background[ind], img_acq, dimbuffer );

      SCALE_IMG ( ind, background[ind] );
      image_dsp[ind]->Draw_image();
      sprintf( msg, "background acquired" );
      pView->tlctext_2[ind]->SetText( msg, 0 );
    }
    else // Timeout!
    {  
      TRACE ("Timeout occurred!\n");// << endl;
      StreamGrabber[ind]->CancelGrab(); // Get the pending buffer back (You are not allowed to deregister buffers when they are still queued)
      for (GrabResult r; StreamGrabber[ind]->RetrieveResult(r);); // Get all buffers back
    }
  } 
  else TRACE("Get_background[%d]: timeout on GetWaitObject\n", ind );


  Camera[ind]->AcquisitionStop.Execute();
  StreamGrabber[ind]->CancelGrab();

  TRACE("Get_background[%d] executed\n", ind );


  // Consume all items from the output queue
  while ( StreamGrabber[ind]->GetWaitObject().Wait(0) ) 
  {
    StreamGrabber[ind]->RetrieveResult( Result );
    if ( Result.Status() == Canceled ) TRACE("Got canceled buffer\n");
  }
//  TRACE("%d buffers canceled from output queue\n", i );

  if ( buffer_handle ) StreamGrabber[ind]->DeregisterBuffer( buffer_handle );
  if ( buffer        ) _aligned_free( buffer );

  StreamGrabber[ind]->FinishGrab();

  Set_averaging( ind, tlc_averaging[ind] );


  if ( !(ind_in&256) ) // ask for saving
  {

    if ( Message_box( string_302[lng], MBOX_YESNO ) == IDYES )
    {
      sprintf( msg, "%s\\background[%d]", MAIN_DIRECTORY, ind );


      Save_image( background[ind], DIMX_ACQ, DIMY_ACQ, msg, PNG_FILE );

    }
  }


  /*
  int lumth_1 = 120;
  int lumth_2 = 255;

  if ( ind==0 ) // corpo
  {
    int  x, y;
    int  semiamp = 5;
    int  delta[256];
//    int  delta2;
    UINT histo[256];
    UINT accum   = 0;
    ASSERT( semiamp>=0 && semiamp<=20 );

    ZeroMemory( histo, sizeof(histo) );
    ZeroMemory( delta, sizeof(delta) );

    for ( x=0 ; x<DIMX_ACQ*DIMY_ACQ ; x++ ) histo[background[ind][x]]++;

    TRACE("histo[%d]\n", ind );
    for ( x=0 ; x<256 ; x++ )
    {
      accum = 0;
      if ( x-semiamp>=0 && x+semiamp<=255 ) for ( y=-semiamp ; y<=semiamp ; y++ ) accum += histo[x+y];
      accum /= 1+2*semiamp;

      //if ( x>0 ) delta[x] = histo[x]-histo[x-1];

      //if ( x>=3 && x<=252 ) delta2 =  3*(histo[x+1]-histo[x  ]) + 2*(histo[x+2]-histo[x  ]) + (histo[x+3]-histo[x  ]) - 
      //                                3*(histo[x  ]-histo[x-1]) - 2*(histo[x  ]-histo[x-2]) - (histo[x  ]-histo[x-3]);
      //else                  delta2 = 0;
      //TRACE("%d,%d,%d\n", x, histo[x], delta2 );

      /*if ( x>50 )
      {
        if ( lumth_1==0 ) 
        {
          if ( accum>50000 ) lumth_1 = x;
        }
        else
        {
          if ( accum<50000 ) lumth_2 = x;
        }
      }

      TRACE("%d,%d,%d\n", x, histo[x], accum );*/
 /*   }
    
    TRACE("lumth_1 = %3d - lumth_2 = %3d\n", lumth_1, lumth_2 ); 

    ASSERT( 50<=lumth_1 && lumth_1 < lumth_2-10 && lumth_2 < 256 );

    UINT npt_bckg = 0;
    for ( y = lumth_1 ; y <= lumth_2 ; y++ ) npt_bckg += histo[y]; 
    
    TRACE("npt_bckg=%d pari al %3.1lf del totale\n", npt_bckg, 100.0*npt_bckg/(DIMX_ACQ*DIMY_ACQ) ); 

    sprintf( msg, "lumth_1= %d - lumth_2= %d - perc= %3.1lf", lumth_1, lumth_2, 100.0*npt_bckg/(DIMX_ACQ*DIMY_ACQ) );
    pView->tlctext_3[ind]->SetText( msg, 0 );

    */
    /*
    for ( y=0 ; y<DIMY_ACQ ; y++ )
      for ( x=0 ; x<DIMX_ACQ ; x++ )
      {
        if ( background[ind][x+y*DIMX_ACQ]>=lumth_1 && background[ind][x+y*DIMX_ACQ]<=lumth_2 )
        {
          DRAW_PT(ind, x, y, 0xFFFF00FF );
        }

      }

    image_dsp[ind]->Draw_image();
    */  
//  }




//  pView->tlctext_3[ind]->SetText( "", 0 );
//  pView->tlctext_4[ind]->SetText( "", 0 );

/*
  return 0;

}

*/








// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +             IMAGE SEQUENCE THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl sequence_3d( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

 /* struct frame_type
  {
    uint32_t  framenum;
    uint64_t  timestamp;
    BYTE*     buffer;
  };*/

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  UINT num_images = 200; // default 


 	CInput_figures numpad;
	swprintf( numpad.buffer, 10 , L"%d" , num_images );
  numpad.Create( IDD_INPUT_FIGURES );
	if ( numpad.RunModalLoop() == IDOK )
  {
    if ( swscanf( numpad.buffer , L"%d" , &num_images ) != 1 ) TRACE( L"Control NOT modified!\n" ); //TRACE("Controllo NON modificato!\n" );
		numpad.DestroyWindow();
	}
  

  TRACE("sequence_3d[%d] thread started\n", ind );

//  frame_type* frames = (frame_type*) malloc( sizeof(frame_type)*num_images );
//  ZeroMemory( frames,  sizeof(frame_type)*num_images );

  SET_BIT( thread_status, 10+ind );

  TRACE("sequence_3d[%d] frames allocated - Initial MaxNumBuffer=%d\n", ind, camera[ind].camera->MaxNumBuffer.GetValue() );


  if ( camera[ind].camera->MaxNumBuffer.GetValue() != num_images )
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->MaxNumBuffer = num_images;
    TRACE("sequence_3d[%d] MaxNumBuffer set to %d (%d)\n", ind, num_images, camera[ind].camera->MaxNumBuffer.GetValue() );
  }
  else TRACE("Number of buffer already set to %d\n", num_images );

//  txt.Format( string_812[lng], ind+1 );
  txt.Format( string_812[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByUser );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  txt.Format(L"Ready to start capture of %d images\r\nPush ENTER or OK to start", num_images );
  Message_box( txt );

  TRACE("sequence_3d[%d] grab started - entering main loop\n", ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) && nimg_acq<num_images ) // bit high requests to exit
  { 

   // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
    camera[ind].camera->RetrieveResult( 500, ptrGrabResult, TimeoutHandling_ThrowException );

    // Image grabbed successfully?
    if ( ptrGrabResult->GrabSucceeded() )
    {

/*      frames[nimg_acq].buffer    = img_acq = (BYTE*) ptrGrabResult->GetBuffer();
      frames[nimg_acq].timestamp = tmstamp = ptrGrabResult->GetTimeStamp();
      frames[nimg_acq].framenum  = frmnum  = ptrGrabResult->GetFrameNumber(); */

      //TRACE( "Buffer %08X - Frame %3d - tmstmp %I64u\n", img_acq, frmnum, tmstamp );

      if ( tmstamp - tmstamp_old > 0.050*ACE_FREQUENCY )
      {
        SCALE_IMG( ind, img_acq );
        image_dsp[ind]->Draw_image();

        txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
        camera[ind].text[1]->SetWindowTextW( txt );

        tmstamp_old = tmstamp;
        frmnum_old  = frmnum;

        txt.Format( string_822[lng], nimg_acq, nimg_lost );
        camera[ind].text[2]->SetWindowTextW( txt );
      }

      nimg_acq++;
    }
    else
    {
      nimg_lost++;
      TRACE("Unable to grab image error %s\n", ptrGrabResult->GetErrorDescription() );
    }

  }

//  image_dsp[ind]->ptr_ext = NULL;
  CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );
  

  if ( Message_box( L"Would you like to save acquired images to disk?", MBOX_YESNO ) == IDYES )
  {
    char buff[100+1];
    CFile fp; 
    CFileException ex;
    CTime time = CTime::GetCurrentTime();   
    CString dir;

    dir.Format( L"Sequence_%04d%02d%02d_%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
    CreateDirectory( dir, NULL );
    SetCurrentDirectory( dir );

    txt.Format( L"Sequence_data.csv" ); //, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
    if ( !fp.Open( txt , CFile::modeCreate|CFile::modeWrite, &ex ) )
    {
      ex.ReportError();
      //return 99;
    }


    for ( UINT i=0 ; i<nimg_acq ; i++ )
    {
//      sprintf( buff, "%d, %I64u\r\n", frames[i].framenum, frames[i].timestamp );
      fp.Write( buff, strlen(buff) );
      //TRACE( "%s - <%d>\r\n", buff,  strlen(buff) );

      wchar_t wbuff[100];
      swprintf( wbuff, 100, L"img_%04d", i );
//      Save_buffer( frames[i].buffer, image_dsp[ind]->dim_buf.cx, image_dsp[ind]->dim_buf.cy, wbuff, PNG_FILE );

//      sprintf( buff, "img_%04d", i );
//      Save_image( frames[i].buffer, image_dsp[ind]->dim_buf.cx, image_dsp[ind]->dim_buf.cy, buff, PNG_FILE );
//      Save_image( frames[i].buffer, DIMX_ACQ, DIMY_ACQ, buff, PNG_FILE );

      txt.Format( L"Saving image n %d", i );
      camera[ind].text[2]->SetWindowTextW( txt );
    }

    fp.Close();

    SetCurrentDirectory( MAIN_DIRECTORY );

  }

  camera[ind].camera->StopGrabbing();

//  if ( frames ) free( frames );

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("sequence_3d[%d] exiting main loop\n", ind );

  return 0;
}











// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +        LINEAR CAMERA INSPECTION THREAD         +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_cosmetic( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_rej  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt, 0 );

  ASSERT( cosmetic_insp[ind] );

  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<=21 ); // 17 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( txt, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;

  //Pour added 20180326
    // Before testing the callbacks, we manually set the heartbeat timeout to a short value when using GigE cameras.
    // Since for debug versions the heartbeat timeout has been set to 5 minutes, it would take up to 5 minutes 
    // until detection of the device removal.
    CHearbeatHelper heartbeatHelper(*(camera[ind].camera));
    heartbeatHelper.SetValue(2000);  // 2000 ms timeout
  //Pour added 20180326


  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    //camera[ind].camera->SetBufferFactory
    //camera[ind].camera->MaxNumBuffer = 5;
  }
  catch (GenICam::GenericException &e)
  {
    //size_t convertedChars = 0;
    //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"Genicam exception thrown <%s>\n", msg_err );
    //TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );

    //swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
	swprintf( msg_err, 200, L"Exception: <%s>\n", AtoW(e.GetDescription()) );

    TRACE( L"inspect_3d_cosmetic: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    return 99;
  }


  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  TRACE("inspect_3d_cosmetic[%d] started - entering main loop\n", ind );

  int last_wait = 0;

  UINT triggerold = trigger_label[camera[ind].triglab];

  UINT bit_ok = BIT_COSMETIC;  // good for LQ machine, only one station and FD sidewall
  #ifdef TIVS_F40
    if ( IS_PC2 )
    {
      if ( ind<4 ) bit_ok = BIT_PARTICLE1; // first group - sidecake control
      else         bit_ok = BIT_PARTICLE2; // second group - topcake control
    }
  #endif
  //#ifdef TIVS_C20
  //  if ( IS_PC1 )
  //  {
  //    bit_ok = BIT_PARTICLE1; // first group - sidewall control
  //  }
  //  if ( IS_PC2 )
  //  {

	 // if (ind<2) bit_ok = BIT_PARTICLE1; // first group - sidewall control
  //    if (ind<2) bit_ok = BIT_PARTICLE2; // second group - sidecake control
  //    else       bit_ok = BIT_COSMETIC;  // third group  - topcake
  //  }
  //#endif

		 //Pour changed on 20190430 for second C20 machine
  #ifdef TIVS_C20

    if ( IS_PC2 )
    {
      if (ind<2)                      bit_ok = BIT_COSMETIC;  // first group - sidewall control
	  else if(ind == 2 || ind == 3)   bit_ok = BIT_PARTICLE1; //second group - sidecake //Pour marked on 20190430
      else                            bit_ok = BIT_PARTICLE2; // third group - topcake
    }
  #endif
 	//Pour marked on 20190430 for second C20 machine.


  #ifdef TIVS_C40  // freeze/dried vials machine
    if ( ind>=9 ) bit_ok = BIT_PARTICLE1; // ind>=9 => Sidecake PC1
    if ( ind<=5 ) bit_ok = BIT_PARTICLE2; // ind<=5 => Heel PC2
  #endif

  #ifdef TIVS_C60  // freeze/dried vials machine SISTEMARE
    if ( IS_PC1 ) // Only cosmetic
    {
      bit_ok = BIT_COSMETIC;  // third group - cosmetic/sidewall PC1&PC2
    }
    else // PC2
    {
      if ( ind<=3 ) bit_ok = BIT_PARTICLE1; // ind<=3 => Sidecake PC2
      if ( ind>=8 ) bit_ok = BIT_PARTICLE2; // ind>=8 => Heel     PC2
    }
  #endif

	  #ifdef TIVS_S40  // PFS machine
    bit_ok = BIT_COSMETIC; // 
  #endif

	#ifdef TIVS_S60  // PFS machine
    bit_ok = BIT_COSMETIC; // 
	#endif


  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 
    // wait sequence start trigger
    timer.reset();
    last_wait = 60;
//    while ( triggerold >= trigger_label[camera[ind].triglab] )
    while ( triggerold==trigger_label[camera[ind].triglab] )
    {
      Sleep( 1 );
      if ( timer.elapsed_sec()>last_wait )
      {
        last_wait = timer.elapsed_sec();
        txt.Format( string_804[lng], last_wait );
        camera[ind].text[1]->SetText( txt, 0 );
      }
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto inspect_3d_cosmetic;
    }

    triggerold = trigger_label[camera[ind].triglab];
    timer.reset();

    try
    {

      if ( camera[ind].camera->GrabOne( 1000, ptrGrabResult ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          int time_acq = timer.elapsed_ms();

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          insp_res->num = triggerold;
          insp_res->val = 0;   


          BOOL update_view = !show_rejects;

          SCALE_IMG( ind, img_acq );
          //if ( cosmetic_insp[ind] ) cosmetic_insp[ind]->Process_image( img_acq );

          if ( cosmetic_insp[ind] )
          {
            if ( datarec_on && triggerold>=0 && triggerold<60 ) cosmetic_insp[ind]->nhead=triggerold;
            cosmetic_insp[ind]->Process_image( img_acq );
            if ( cosmetic_insp[ind]->good ) insp_res->val |= bit_ok; // set good result bit high
            else
            {
              nimg_rej++;
              update_view = TRUE;  // cosmetic reject 
            }
          }


 /*         if (  cosmetic_insp[ind]->cosm_err > cosmetic_insp[ind]->max_err ) // COSMETIC REJECT
          {
            txt.Format( string_852[lng], cosmetic_insp[ind]->cosm_err, cosmetic_insp[ind]->max_err );
            camera[ind].text[1]->SetText( txt, TXT_DARKRED );
          }
          else      // COSMETIC GOOD
          {
            insp_res->val |= BIT_COSMETIC; // set good result bit high
            txt.Format( string_850[lng], cosmetic_insp[ind]->cosm_err, cosmetic_insp[ind]->max_err );
            camera[ind].text[1]->SetText( txt, TXT_DARKGREEN );
          }  */

          ADS_write_parameter( &ADS_insp_res );
          nimg_acq++;

          if ( update_view && image_dsp[ind]->visible )
          {
            image_dsp[ind]->Draw_image();
            if ( cosmetic_insp[ind] )
            {
              camera[ind].text[1]->SetText( cosmetic_insp[ind]->txt1, cosmetic_insp[ind]->col1 );
              camera[ind].text[2]->SetText( cosmetic_insp[ind]->txt2, cosmetic_insp[ind]->col2 );
            }
          }

//          txt.Format( string_836[lng], nimg_acq, time_acq, timer.elapsed_ms()-time_acq );
          txt.Format( string_836[lng], nimg_rej, nimg_acq, time_acq, timer.elapsed_ms()-time_acq, timer.elapsed_ms() );
          camera[ind].text[3]->SetText( txt, 0 );

        }
        else
        {
          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
          if ( CHK_BIT( thread_exit , 10+ind ) ) goto inspect_3d_cosmetic;
        }
      }
      else // Timeout while waiting for trigger
      {

        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetWindowTextW( txt );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto inspect_3d_cosmetic;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }
    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"inspect_3d_cosmetic[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "inspect_3d_cosmetic[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );

      //Stop_ifcamerror( ind );

      swprintf( msg_err, 200, L"Exception thrown <%s>\n", AtoW(e.GetDescription()) );

	  Stop_ifcamerror( ind );


      TRACE( L"inspect_3d_cosmetic: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log ( ind, e.GetDescription());
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto inspect_3d_cosmetic;
    }
  }
  inspect_3d_cosmetic:


  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_cosmetic[%d] exiting main loop\n", ind );
  
  return 0;
}



#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing

UINT __cdecl processing_thread( LPVOID pParam )
{
  UINT ind = (UINT) pParam;

  ASSERT( ind==0 || ind==1 );

  TRACE("processing_thread[%d] started\n", ind);

  processing_start[ind] = new CEvent(FALSE, FALSE);
  processing_done [ind] = new CEvent(FALSE, FALSE);

  TRACE("processing_start created (%08X) - processing_done created (%08X)\n", processing_start[ind], processing_done[ind] );

  if ( ind==0 ) 
  {


  }


  while ( 1 )
  {

    ::WaitForSingleObject(processing_start[ind]->m_hObject, INFINITE);
    //TRACE("event signalled - start processing\n");


    if ( ind==0 ) tip_insp[2]->Find_contour_cw();
    else          tip_insp[2]->Find_contour_ccw();


    processing_done[ind]->SetEvent();

    if ( CHK_BIT(thread_exit, 1) ) break;

  }


  if ( processing_start[ind] ) // delete Cevent
  {
    delete processing_start[ind];
    processing_start[ind] = NULL;
  }

  if ( processing_done[ind] ) // delete Cevent
  {
    delete processing_done[ind];
    processing_done[ind] = NULL;
  }

  TRACE("processing_thread[%d] ended\n", ind);
  return 0;
}

#endif // WORKER_THREAD - if defined uses worker threads to speed-up processing



// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +             LINEAR CAMERA MOVIE THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl movie_3d_linear( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  ASSERT( camera[ind].camera->IsOpen() );


  SET_BIT( thread_status, 10+ind );

  txt.Format( string_814[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    
    //camera[ind].camera->SetBufferFactory
    //camera[ind].camera->MaxNumBuffer = 10;
  }
  catch (GenICam::GenericException &e)
  {
//    size_t convertedChars = 0;
//    mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
//    TRACE( L"Genicam exception thrown <%s>\n", msg_err );
//    TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"movie_3d_linear: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    return 99;
  }


  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  TRACE("movie_3d_linear[%d] started - entering main loop - trigger_ind=%d\n", ind, camera[ind].triglab );


  UINT triggerold = trigger_label[camera[ind].triglab];

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    while ( triggerold == trigger_label[camera[ind].triglab] && !CHK_BIT( thread_exit , 10+ind ) ) Sleep( 2 );
    triggerold = trigger_label[camera[ind].triglab];

    //TRACE("movie_3d_linear[%d] triggered(%d)\n", ind, triggerold );

    // camera[ind].camera->GrabOne( 1000, ptrGrabResult );
    // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
    // camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException );
    // if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )

    try
    {

      if ( camera[ind].camera->GrabOne( 3000, ptrGrabResult ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          TRACE("movie_3d_linear[%d] GrabSucceeded\n", ind );

          if ( image_dsp[ind]->visible && tmstamp-tmstamp_old>0.100*ACE_FREQUENCY )
          {
            SCALE_IMG( ind, img_acq );
            image_dsp[ind]->Draw_image();

            //        txt.Format( string_2132[lng], 128.0e6/timer.elapsed_us() );
            txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
            camera[ind].text[1]->SetWindowTextW( txt );

            tmstamp_old = tmstamp;
            frmnum_old  = frmnum;

            txt.Format( string_822[lng], nimg_acq, nimg_lost );
            camera[ind].text[2]->SetWindowTextW( txt );

            TRACE("movie_3d_linear[%d] scaled and drawn\n", ind );
          }

          nimg_acq++;
        }
        else // ptrGrabResult->GrabSucceeded()
        {
          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }
        timer.reset();
      }
      else // Timeout while waiting for trigger
      {
        CString msg( ptrGrabResult->GetErrorDescription() );
        TRACE( L"movie_3d_linear[%d] GrabOne FAILED (%s)\n", ind, msg );

        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetWindowTextW( txt );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d_linear;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }
    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"movie_3d_linear[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "movie_3d_linear[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
	  //Pour added 20180326
	  Stop_ifcamerror(ind);
	  //Pour added 20180326
      TRACE( L"movie_3d_linear: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d_linear;
    }
  }

  exit_movie_3d_linear:

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("movie_3d_linear[%d] exiting main loop\n", ind );
  
  return 0;
}








// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +               CAMERA MOVIE THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl movie_3d_sg( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_814[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  try
  {
    //camera[ind].camera->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
    //camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->SetBufferFactory( buffact );
    camera[ind].camera->MaxNumBuffer = 5;
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
  }
  catch (GenICam::GenericException &e)
  {
    //size_t convertedChars = 0;
    //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"Genicam exception thrown <%s>\n", msg_err );
    //TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );
   swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
   TRACE( L"movie_3d_sg: %s\n", msg_err );
   if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
   return 99;
  }


  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  TRACE("movie_3d[%d] started - entering main loop\n", ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    try // Genicam exception handling
    {
      camera[ind].camera->ExecuteSoftwareTrigger();



      // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          if ( image_dsp[ind]->visible && tmstamp-tmstamp_old>0.030*ACE_FREQUENCY )
          {
            SCALE_IMG( ind, img_acq );
            image_dsp[ind]->Draw_image();

            //        txt.Format( string_2132[lng], 128.0e6/timer.elapsed_us() );
            txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
            camera[ind].text[1]->SetWindowTextW( txt );

            tmstamp_old = tmstamp;
            frmnum_old  = frmnum;

            txt.Format( string_822[lng], nimg_acq, nimg_lost );
            camera[ind].text[2]->SetWindowTextW( txt );
          }


       /*   if ( nimg_acq&1 ) set_camera_output( ind, 1 );
            else              set_camera_output( ind, 0 );  */

          nimg_acq++;
        }
        else
        {

          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        
      }
      else // Timeout while waiting for trigger
      {
        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetWindowTextW( txt );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d_sg;
//        txt.Format( string_804[lng], timer.elapsed_sec() );
//        camera[ind].text[1]->SetWindowTextW( txt );
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"movie_3d[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "movie_3d[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"movie_3d: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d_sg;
    }

  }

  exit_movie_3d_sg:

//  image_dsp[ind]->ptr_ext = NULL;
  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  camera[ind].camera->StopGrabbing();

//  camera[ind].camera->SetBufferFactory( NULL );


  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("movie_3d[%d] exiting main loop\n", ind );
  
  return 0;
}





// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +               CAMERA MOVIE THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl movie_3d( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  int  last_wait = 20;


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_814[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );


  //TRACE("movie_3d[%d]: STARTGRABBING start mutex waiting...\n", ind);
  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  if ( mutex_res == WAIT_ABANDONED ) TRACE("Mutex[%d]=WAIT_ABANDONED\n",ind);
  if ( mutex_res == WAIT_OBJECT_0  ) TRACE("Mutex[%d]=WAIT_OBJECT_0\n",ind);
  if ( mutex_res == WAIT_TIMEOUT   ) TRACE("Mutex[%d]=WAIT_TIMEOUT\n",ind);
  if ( mutex_res == WAIT_FAILED    ) TRACE("Mutex[%d]=WAIT_FAILED\n",ind);

  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STARTGRABBING mutex allowed to proceed\n", ind);

  try
  {

    if ( camera[ind].sequence )
    {
      INodeMap* nodemap = camera[ind].device->GetNodeMap();
      CIntegerPtr acqframecount( nodemap->GetNode( "AcquisitionFrameCount" ) );
      if ( acqframecount && IsWritable( acqframecount ) )
      {
        acqframecount->SetValue( 20 );
        ASSERT( acqframecount->GetValue()==20 );
        TRACE("movie_3d[%d]: AcquisitionFrameCount set to %d images\n", ind, 20 );
      }
      else TRACE("movie_3d[%d] ERROR: Unable to access <AcquisitionFrameCount> node\n", ind );
    }

    //camera[ind].camera->StaticChunkNodeMapPoolSize/
    //camera[ind].camera->MaxNumBuffer = 16;

    #ifdef TIVS_A60
      Sleep(100*ind);
    #endif

    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->SetBufferFactory( buffact );
    //camera[ind].camera->MaxNumBuffer = 5;
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );

   if ( camera[ind].sequence )
   {
     //Sleep(100);
     camera[ind].camera->ExecuteSoftwareTrigger();
     //Sleep(100);
   }
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"movie_3d: %s\n", msg_err );
   if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
   Add_camerror_log(ind, e.GetDescription());
    return 99;
  }


  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_particle[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  //TRACE("movie_3d[%d]: STARTGRABBING completed - mutex released\n", ind);

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  TRACE("movie_3d[%d] started - entering main loop\n", ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 8000 ms is used.
      if ( camera[ind].camera->RetrieveResult( 8000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();

          //img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();



          //ASSERT( (UINT(img_acq)&15) == 0 );
          //TRACE("Frame grabbed: %dx%d %d\n", ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(), ptrGrabResult->GetImageSize() );


          if ( image_dsp[ind]->visible && tmstamp-tmstamp_old>0.030*ACE_FREQUENCY )
          {
            //SCALE_IMG( ind, img_acq );
            image_dsp[ind]->scale_grabresult( ptrGrabResult );

//					    //Pour added for interface tests on 20190429
//#ifdef NEW_PARTICLE
//	{
//		img_acq = (BYTE*) ptrGrabResult->GetBuffer();
//	  MiddlewareParticleDi::ParaIn para_particleDi;
//	  //para_particleDi.minnumimg = minnumimg; 
//	  //para_particleDi.mintotnpt = mintotnpt;
//	  MiddlewareParticleDi* p_middleware_particle_di = new MiddlewareParticleDi(para_particleDi);
//	  p_middleware_particle_di->ptrs2cvMats(images, dimx, dimy, num_img);
//	  cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
//	  p_middleware_particle_di->run(rlt_particleDi, imcolor);
//	  //nimgpt = rlt_particleDi.nimgpt;
//	  //totnpt = rlt_particleDi.totnpt;
//	  p_middleware_particle_di->cvMat2Ptr(image_dsp[ind]->ptr_dsp, image_dsp[ind]->dim_dsp.cx, image_dsp[ind]->dim_dsp.cy,imcolor );
//	  delete p_middleware_particle_di;
//	}
//#endif
//	//Pour added for interface tests on 20190429

            image_dsp[ind]->Draw_image();

            //        txt.Format( string_2132[lng], 128.0e6/timer.elapsed_us() );
            txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
            camera[ind].text[1]->SetText( txt, 0 );

            tmstamp_old = tmstamp;
            frmnum_old  = frmnum;

            txt.Format( string_822[lng], ++nimg_acq, nimg_lost );
            camera[ind].text[2]->SetText( txt, 0 );
			
			//Pour added for color display 20180531
/*
	#ifdef COLOR_DETECTION_P1

			if( ind==3 || ind==4 )
			{

			txt.Format( string_822[lng], ++nimg_acq, nimg_lost );
			camera[ind].text[3]->SetText(txt,0);

			}
	#endif
	*/
			//Pour added for color display 20180531
          }


       /*   if ( nimg_acq&1 ) set_camera_output( ind, 1 );
            else              set_camera_output( ind, 0 );  */

          //nimg_acq++;
        }
        else
        {

          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        last_wait = 60;
        
      }
      else // Timeout while waiting for trigger
      {

        // wait sequence start trigger
        //timer.reset();
        if ( timer.elapsed_sec()>last_wait )
        {
          last_wait = timer.elapsed_sec();
          txt.Format( string_804[lng], last_wait );
          camera[ind].text[1]->SetText( txt, 0 );
        }

        //if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();

        //txt.Format( string_804[lng], timer.elapsed_sec() );
        //camera[ind].text[1]->SetText( txt, 0 );
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"movie_3d[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "movie_3d[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"movie_3d: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d;
    }

  }

  exit_movie_3d:

//  image_dsp[ind]->ptr_ext = NULL;
//  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

/*  while ( camera[ind].camera->IsGrabbing() )
  {
    // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
    if ( camera[ind].camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException)) camera[ind].camera->ExecuteSoftwareTrigger();
    // Retrieve grab results and notify the camera event and image event handlers.
    camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
  }

  try // Genicam exception handling
  {
    camera[ind].camera->StopGrabbing();
  }
  catch (GenICam::GenericException &e)
  {
    size_t convertedChars = 0;
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"movie_3d: %s\n", msg_err );
  } */

/*  TRACE( L"movie_3d[%d]: waiting acquisition completed before StopGrabbing ", ind );
  while ( camera[ind].camera->IsGrabbing() )
  {
    Sleep(100);
    TRACE(".");
  }
  TRACE(" completed\n"); */


  TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  //TRACE("movie_3d[%d]: STOP GRABBING completed - mutex released\n", ind);

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt,  0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("movie_3d[%d] exiting main loop\n", ind );
  
  return 0;
}






// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +      DOUBLE LIGHT CAP MOVIE THREAD             +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl movie_3d_doublecap( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  int  last_wait = 20;


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_814[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );


  //TRACE("movie_3d[%d]: STARTGRABBING start mutex waiting...\n", ind);
  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STARTGRABBING mutex allowed to proceed\n", ind);

  try
  {

    if ( camera[ind].sequence )
    {
      INodeMap* nodemap = camera[ind].device->GetNodeMap();
      CIntegerPtr acqframecount( nodemap->GetNode( "AcquisitionFrameCount" ) );
      if ( acqframecount && IsWritable( acqframecount ) )
      {
        acqframecount->SetValue( 20 );
        ASSERT( acqframecount->GetValue()==20 );
        TRACE("movie_3d[%d]: AcquisitionFrameCount set to %d images\n", ind, 20 );
      }
      else TRACE("movie_3d[%d] ERROR: Unable to access <AcquisitionFrameCount> node\n", ind );
    }

    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->SetBufferFactory( buffact );
    //camera[ind].camera->MaxNumBuffer = 5;
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );

    if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"movie_3d: %s\n", msg_err );
   if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
   Add_camerror_log(ind, e.GetDescription());
    return 99;
  }


  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_particle[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  //TRACE("movie_3d[%d]: STARTGRABBING completed - mutex released\n", ind);

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  BOOL back_light = FALSE; // false=BACKLIGHT/true=TOPRING image


  
  INodeMap* nodemap = camera[ind].device->GetNodeMap();
  CFloatPtr shutter( nodemap->GetNode( "ExposureTimeAbs" ) );
  shutter->SetValue( camera[ind].shutter ); // set for first image
  ASSERT( shutter->GetValue()==camera[ind].shutter );

  TRACE("movie_3d[%d] started - entering main loop\n", ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    try // Genicam exception handling
    {

   /*       double deltat = 0.000008*(tmstamp-tmstamp_old); // 1000.0*(tmstamp-tmstamp_old)/ACE_FREQUENCY;
          if ( deltat>30.0)
          {
            back_light =  TRUE;
            shutter->SetValue( camera[ind].shutter2 );
            //ASSERT( shutter->GetValue()==camera[ind].shutter );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter );
          }
          else
          {
            back_light = FALSE;
            shutter->SetValue( camera[ind].shutter );
            //ASSERT( shutter->GetValue()==camera[ind].shutter2 );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter2 );
          } */


      // Wait for an image and then retrieve it. A timeout of 500 ms is used.
      if ( camera[ind].camera->RetrieveResult( 500, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();

          //img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          //ASSERT( (UINT(img_acq)&15) == 0 );
          //TRACE("Frame grabbed: %dx%d %d\n", ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(), ptrGrabResult->GetImageSize() );

          //INodeMap* nodemap = camera[ind].device->GetNodeMap();
          //CFloatPtr shutter( nodemap->GetNode( "ExposureTimeAbs" ) );

          double deltat = 0.000008*(tmstamp-tmstamp_old); // 1000.0*(tmstamp-tmstamp_old)/ACE_FREQUENCY;
          if ( deltat>30.0)
          {
            back_light =  TRUE;
            //shutter->SetValue( camera[ind].shutter2 );
            //ASSERT( shutter->GetValue()==camera[ind].shutter );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter );
          }
          else
          {
            back_light = FALSE;
            //shutter->SetValue( camera[ind].shutter );
            //ASSERT( shutter->GetValue()==camera[ind].shutter2 );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter2 );
          }  

          //TRACE("frame=%5d - deltat=%6.3lf\n", frmnum, deltat );

          if ( image_dsp[ind]->visible ) // && tmstamp-tmstamp_old>0.030*ACE_FREQUENCY )
          {

            if ( back_light==show_back_light )
            {

              //SCALE_IMG( ind, img_acq );
              image_dsp[ind]->scale_grabresult( ptrGrabResult );
              image_dsp[ind]->Draw_image();

              //        txt.Format( string_2132[lng], 128.0e6/timer.elapsed_us() );
              txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
              camera[ind].text[1]->SetText( txt, 0 );

              tmstamp_old = tmstamp;
              frmnum_old  = frmnum;

              txt.Format( string_822[lng], ++nimg_acq, nimg_lost );
              camera[ind].text[2]->SetText( txt, 0 );

            }
          }

          tmstamp_old = tmstamp;

  /*        double deltat = 0.000008*(tmstamp-tmstamp_old); // 1000.0*(tmstamp-tmstamp_old)/ACE_FREQUENCY;
          if ( deltat>30.0)
          {
            back_light =  TRUE;
            shutter->SetValue( camera[ind].shutter2 );
            //ASSERT( shutter->GetValue()==camera[ind].shutter );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter );
          }
          else
          {
            back_light = FALSE;
            shutter->SetValue( camera[ind].shutter );
            //ASSERT( shutter->GetValue()==camera[ind].shutter2 );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter2 );
          } */

        }
        else
        {

          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        last_wait = 60;
        
      }
      else // Timeout while waiting for trigger
      {

        // wait sequence start trigger
        //timer.reset();
        if ( timer.elapsed_sec()>last_wait )
        {
          last_wait = timer.elapsed_sec();
          txt.Format( string_804[lng], last_wait );
          camera[ind].text[1]->SetText( txt, 0 );
        }

        //txt.Format( string_804[lng], timer.elapsed_sec() );
        //camera[ind].text[1]->SetText( txt, 0 );
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"movie_3d[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "movie_3d[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"movie_3d: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_movie_3d;
    }

  }

  exit_movie_3d:

//  image_dsp[ind]->ptr_ext = NULL;
//  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

/*  while ( camera[ind].camera->IsGrabbing() )
  {
    // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
    if ( camera[ind].camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException)) camera[ind].camera->ExecuteSoftwareTrigger();
    // Retrieve grab results and notify the camera event and image event handlers.
    camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
  }

  try // Genicam exception handling
  {
    camera[ind].camera->StopGrabbing();
  }
  catch (GenICam::GenericException &e)
  {
    size_t convertedChars = 0;
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"movie_3d: %s\n", msg_err );
  } */

/*  TRACE( L"movie_3d[%d]: waiting acquisition completed before StopGrabbing ", ind );
  while ( camera[ind].camera->IsGrabbing() )
  {
    Sleep(100);
    TRACE(".");
  }
  TRACE(" completed\n"); */


  TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  //TRACE("movie_3d[%d]: STOP GRABBING completed - mutex released\n", ind);

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt,  0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("movie_3d[%d] exiting main loop\n", ind );
  
  return 0;
}




// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +         BOTTOM CAMERA INSPECTION THREAD             +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_singlecam( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  // at least one must exist
  ASSERT( bottom_insp[ind] || focolor_insp[ind] || oralfo_insp[ind] || drcolor_insp[ind] || lqcolor_insp[ind] || pfsidewall_insp[ind] || pfstopper_insp[ind]||flange_insp[ind] );

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt, 0 );


  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<=16 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //Pour added 20180326
  // Before testing the callbacks, we manually set the heartbeat timeout to a short value when using GigE cameras.
  // Since for debug versions the heartbeat timeout has been set to 5 minutes, it would take up to 5 minutes 
  // until detection of the device removal.
     CHearbeatHelper heartbeatHelper(*(camera[ind].camera));
     heartbeatHelper.SetValue(2000);  // 2000 ms timeout
  //Pour added 20180326

  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_singlecam: %s\n", msg_err );
     if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	 Add_camerror_log(ind, e.GetDescription());
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_singlecam[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  #ifdef TIVS_F40
    ASSERT( ind>=3 && ind<=5 );
  #endif

  UINT          bit_ok = BIT_BOTTOMA;   // ind==3 => Bottom A
  if ( ind==4 ) bit_ok = BIT_BOTTOMB;   // ind==4 => Bottom B
  if ( ind==5 ) bit_ok = BIT_FOCOLOR;   // ind==5 => FO Color

 // #if SERIAL_N==201300502 // HENG RUI - FO color reject
  #ifdef FO_COLOR
    bit_ok = BIT_FOCOLOR;
  #endif

  #ifdef LIQUID_COLOR
    bit_ok = BIT_FOCOLOR; // same as FO_COLOR
  #endif

  #ifdef DOTRING_COLOR
    bit_ok = BIT_DRCOLOR;

    ASSERT( drcolor_insp[7] && drcolor_insp[8] );
    drcolor_insp[7]->npt_cam = -1; // still to be calculated
    drcolor_insp[8]->npt_cam = -1; // still to be calculated

    drcolor_insp[7]->insp_label = -1; // still undefined
    drcolor_insp[8]->insp_label = -1; // still undefined

    if ( ind==8 ) ADS_insp_res.handle = 0; // DR_B DOESN'T HAVE TO WRITE res !!

  #endif

  #ifdef TIVS_C40  // freeze/dried vials machine
    if ( ind==3 ) bit_ok = BIT_BOTTOMA;   // ind==3 => Bottom A
    if ( ind==4 ) bit_ok = BIT_BOTTOMB;   // ind==4 => Bottom B
    if ( ind==5 ) bit_ok = BIT_FOCOLOR;   // ind==5 => FO Color / LQ Color
  #endif

  #ifdef TIVS_C60  // freeze/dried vials machine SISTEMARE
    if ( ind==3 ) bit_ok = BIT_BOTTOMA;   // ind==3 => Bottom A
    if ( ind==4 ) bit_ok = BIT_BOTTOMB;   // ind==4 => Bottom B
    if ( ind==5 ) bit_ok = BIT_FOCOLOR;   // ind==5 => FO Color / LQ Color
  #endif

	#ifdef TIVS_S40  // PFS machine
    if ( ind==0 )  bit_ok = BIT_CAPSEAL1;   // ind==0 => Flange
    if ( ind==1 )  bit_ok = BIT_BOTTOMA;    // C3A
    if ( ind==2 )  bit_ok = BIT_BOTTOMB;    // C3B
    if ( ind>11 )  bit_ok = BIT_COSMETIC3;  // 
  #endif

	#ifdef TIVS_S60  // PFS machine //Pour marked to be checked on 20201023
    if ( ind==0 )  bit_ok = BIT_CAPSEAL1;   // ind==0 => Flange
    if ( ind==1 )  bit_ok = BIT_BOTTOMA;    // C3A
    if ( ind==2 )  bit_ok = BIT_BOTTOMB;    // C3B
    //if ( ind>14 )  bit_ok = BIT_COSMETIC3;  // 
	#endif


  TRACE("inspect_3d_singlecam[%d] started - entering main loop\n", ind );
  //Pour added 20180326
  unsigned int loopCount = 0;
  //Pour
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 
	  	   // Pour added for alarming on camera disconnection 2018/03/19
//#ifdef SERIAL_N 201700013
	  if ( ++loopCount % 10 == 0 )
	  {
		  if( camera[ind].camera->IsCameraDeviceRemoved()) //IsReadable())
			  
		  {
			  Stop_ifcamerror(ind);
			  //alarm_data->on   = TRUE;
			  //alarm_data->format = 10;
		   //   alarm_data->index  = 57;
		   //   alarm_data->param_0 = ind;
		   //   ADS_write_parameter(&alarm_ads);
		  }

	  }
//#endif
	// Pour added end 2018/03/19

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 500 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          timer.reset();

          //if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
          ADS_read_parameter( &ADS_insp_res );

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          BOOL update_view = !show_rejects;

		            // Flange for S40
          if ( pfstopper_insp[ind] )
          {
            if ( pfstopper_insp[ind]->Process_image( img_acq, trigger_label[camera[ind].triglab] ) ) // !=0  reject / head number in closed loop for data collection
            {
              insp_res->val = 0;         // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }

#if defined TIVS_S40 || defined TIVS_S60
		  //Pour marked on 20201130
		  // Flange for S40
          if ( flange_insp[ind] )
          {
			  //Mat mat_flange;
			  //image_dsp[ind]->buf2Mat(img_acq, mat_flange );
			  cv::Mat img_in(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8U, static_cast<unsigned char*>(ptrGrabResult->GetBuffer()), ptrGrabResult->GetWidth());
	        if ( flange_insp[ind]->Process_image( img_in, trigger_label[camera[ind].triglab] ))

            //if ( flange_insp[ind]->Process_image( img_acq,  ) // !=0  reject / head number in closed loop for data collection
            {
              insp_res->val = 0;         // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }
		  //Pour marked on 20201130
#endif

          // Cosmetic3 A&B for S40
          if ( pfsidewall_insp[ind] )
          {
//            if ( pfsidewall_insp[ind]->Process_image( img_acq, trigger_label[camera[ind].triglab] ) ) // !=0  reject / head number in closed loop for data collection
            pfsidewall_insp[ind]->Process_image( img_acq, 0 );
            if ( pfsidewall_insp[ind]->good ) // inspection result
            {
              insp_res->val = bit_ok; // set good result bit high
            }
            else 
            {
              insp_res->val = 0;         // nothing: reject
              update_view = TRUE;
            }
          }


          if ( bottom_insp[ind] )
          {
            if ( bottom_insp[ind]->Process_image( img_acq, trigger_label[camera[ind].triglab] ) ) // !=0  reject / head number in closed loop for data collection
            {
              insp_res->val = 0;          // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }

          if ( focolor_insp[ind] )
          {
            if ( focolor_insp[ind]->Process_image( ptrGrabResult, 0 ) ) // !=0  reject
            {
              insp_res->val = 0;         // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }

          if ( lqcolor_insp[ind] )
          {
            if ( lqcolor_insp[ind]->Process_image( ptrGrabResult, 0 ) ) // !=0  reject
            {
              insp_res->val = 0;         // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }

          if ( oralfo_insp[ind] )
          {
            if ( oralfo_insp[ind]->Process_image( img_acq, trigger_label[camera[ind].triglab] ) ) // !=0  reject / head number in closed loop for data collection
            {
              insp_res->val = 0;          // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high
          }

          if ( drcolor_insp[ind] && drcolor_insp[ind]->npt_min ) // 
          {

            drcolor_insp[ind]->insp_label = insp_res->num;
          /*  if ( drcolor_insp[ind]->Process_image( ptrGrabResult, 0 ) ) // !=0  reject
            {
              insp_res->val = 0;          // nothing: reject
              update_view = TRUE;
            }
            else insp_res->val = bit_ok; // set good result bit high */

            drcolor_insp[ind]->Process_image( ptrGrabResult, 0 );

            if ( ind==7 ) // final check on cam[7]/DR_A only
            {
              ASSERT( drcolor_insp[7] && drcolor_insp[8] );

              // wait both thread finish inspection
              //while ( drcolor_insp[7]->npt_cam==-1 && drcolor_insp[8]->npt_cam==-1 ) Sleep(0);

              int tot_npt = 0;
              if ( drcolor_insp[7]->insp_label == drcolor_insp[8]->insp_label ) // if cameras synchronized
              {
                while ( drcolor_insp[8]->npt_cam==-1 && !CHK_BIT( thread_exit , 10+ind ) ) Sleep(0);
                tot_npt = drcolor_insp[7]->npt_cam + drcolor_insp[8]->npt_cam;
              }

              if ( tot_npt<drcolor_insp[7]->npt_min ) // total number not enough: reject
              {
                insp_res->val = 0;          // nothing: reject
                update_view = TRUE;
                drcolor_insp[7]->col2 = TXT_DARKRED;
                drcolor_insp[7]->txt2.Format( L"Final result REJECT: %d/%d points found", tot_npt, drcolor_insp[7]->npt_min );
              }
              else
              {
                insp_res->val = bit_ok; // set good result bit high 
                drcolor_insp[7]->col2 = TXT_DARKGREEN;
                drcolor_insp[7]->txt2.Format( L"Final result GOOD: %d/%d points found", tot_npt, drcolor_insp[7]->npt_min );
              }

              //TRACE("DRA: proc finished %d-%d, writing %d to %d\n", drcolor_insp[7]->npt_cam, drcolor_insp[8]->npt_cam, insp_res->val, trigger_label[camera[ind].triglab] );

              drcolor_insp[7]->npt_cam = -1; // still to be calculated
              drcolor_insp[8]->npt_cam = -1; // still to be calculated
            }
            else // final DR_B only
            {
              //drb_label = insp_res->num; // update global for syncrhonization
              //TRACE("DRB: proc finished %d-%d\n", drcolor_insp[7]->npt_cam, drcolor_insp[8]->npt_cam );
            }

          }
          else // disabled (or drcolor_insp=NULL)
          {
            if ( drcolor_insp[ind] ) // if npt_min=0 completely disabled
            {
              drcolor_insp[ind]->col1 = TXT_BLACK;
              drcolor_insp[ind]->col2 = TXT_BLACK;
              drcolor_insp[ind]->txt1 = L"DISABLED";
              drcolor_insp[ind]->txt2 = L"-";
              insp_res->val = bit_ok; // set good result bit high */
            }

          }


          insp_res->num = trigger_label[camera[ind].triglab];
          if ( ADS_insp_res.handle ) ADS_write_parameter( &ADS_insp_res );

          //TRACE("Scritto insp_res->num=%d insp_res->val=%d\n", insp_res->num, insp_res->val );

          nimg_acq++;

          if ( image_dsp[ind]->visible && update_view ) // final display
          {
			// Flange S40
            if ( pfstopper_insp[ind] )
            {
              camera[ind].text[1]->SetText( pfstopper_insp[ind]->txt1, pfstopper_insp[ind]->col1 );
              camera[ind].text[2]->SetText( pfstopper_insp[ind]->txt2, pfstopper_insp[ind]->col2 );
            }

#if defined TIVS_S40 || defined TIVS_S60
			// Flange S40
            if ( flange_insp[ind] )
            {
              camera[ind].text[1]->SetText( flange_insp[ind]->txt1, flange_insp[ind]->col1 );
              camera[ind].text[2]->SetText( flange_insp[ind]->txt2, flange_insp[ind]->col2 );
            }
#endif

            // Cosmetic3 A&B S40
            if ( pfsidewall_insp[ind] )
            {
              camera[ind].text[1]->SetText( pfsidewall_insp[ind]->txt1, pfsidewall_insp[ind]->col1 );
              camera[ind].text[2]->SetText( pfsidewall_insp[ind]->txt2, pfsidewall_insp[ind]->col2 );
            }

            if ( bottom_insp[ind] )
            {
              camera[ind].text[1]->SetText( bottom_insp[ind]->txt1, bottom_insp[ind]->col1 );
              camera[ind].text[2]->SetText( bottom_insp[ind]->txt2, bottom_insp[ind]->col2 );
            }
            if ( focolor_insp[ind] )
            {
              camera[ind].text[1]->SetText( focolor_insp[ind]->txt1, focolor_insp[ind]->col1 );
              camera[ind].text[2]->SetText( focolor_insp[ind]->txt2, focolor_insp[ind]->col2 );
            }
            //if ( lqcolor_insp[ind] )
            //{
            //  camera[ind].text[1]->SetText( lqcolor_insp[ind]->txt1, lqcolor_insp[ind]->col1 );
            //  camera[ind].text[2]->SetText( lqcolor_insp[ind]->txt2, lqcolor_insp[ind]->col2 );
            //}
            if ( oralfo_insp[ind] )
            {
              camera[ind].text[1]->SetText( oralfo_insp[ind]->txt1, oralfo_insp[ind]->col1 );
              camera[ind].text[2]->SetText( oralfo_insp[ind]->txt2, oralfo_insp[ind]->col2 );
            }
            if ( drcolor_insp[ind] )
            {
              camera[ind].text[1]->SetText( drcolor_insp[ind]->txt1, drcolor_insp[ind]->col1 );
              camera[ind].text[2]->SetText( drcolor_insp[ind]->txt2, drcolor_insp[ind]->col2 );
            }
            image_dsp[ind]->Draw_image();
          }

//          txt.Format( string_826[lng], nimg_acq, nimg_lost );
          txt.Format( string_824[lng], timer.elapsed_us(), nimg_acq, nimg_lost );
          camera[ind].text[3]->SetText( txt, 0 );


          //#ifdef SAVE_FULL_PROC
          #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
            if ( image_dsp[ind]->ptr_fulldsp ) image_dsp[ind]->Save_fulldsp( L"processed" );
          #endif
        }
        else
        {

          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        
      }
      else // Timeout while waiting for trigger
      {
        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          camera[ind].text[2]->SetText( L"", 0 );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_singlecam;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"inspect_3d_bottom[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "inspect_3d_bottom[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );

      //Stop_ifcamerror( ind );//Pour 20201015

      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_singlecam: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
		  /*
	  //Pour added for KAIMAO 20180320
	  //#if SERIAL_N==201700013 // shanghai KAIMAO
	  if(nimg_lost>=1)
	  {
		  alarm_data->on     = TRUE;
		  alarm_data->format = 10;
		  alarm_data->index  = 21;
		  alarm_data->param_0 = ind;
		  ADS_write_parameter(&alarm_ads);
		  logger->Add_string(L"Camera communication error");
	  }
		#endif 
	  //Pour added for KAIMAO 20180320
	  */
    }

  }
  exit_inspect_3d_singlecam:


  if ( drcolor_insp[ind] ) drcolor_insp[ind]->npt_cam = 0; // better to clear on exit to avoid deadlocks


  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_singlecam[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );


 // camera[ind].camera->StopGrabbing();

  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt,  0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_singlecam[%d] exiting main loop\n", ind );
  
  return 0;
}




// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +   NEW DOUBLE IMAGES/LIGHT CAP INSPECTION THREAD       +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_doublecap( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE*  img_acq   = NULL;
  UINT   nimg_acq  = 0;
  UINT   nimg_lost = 0;
  CTimer timer;
  CTime  time = CTime::GetCurrentTime();   


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  ASSERT( tip_insp[ind] );

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt, 0 );


  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<15 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //Pour added 20180326
    // Before testing the callbacks, we manually set the heartbeat timeout to a short value when using GigE cameras.
    // Since for debug versions the heartbeat timeout has been set to 5 minutes, it would take up to 5 minutes 
    // until detection of the device removal.
        CHearbeatHelper heartbeatHelper(*(camera[ind].camera));
        heartbeatHelper.SetValue(2000);  // 1000 ms timeout
  //Pour added 20180326
  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  BOOL back_light = FALSE; // false=BACKLIGHT/true=TOPRING image

  UINT          bit_tip_ok = BIT_CAPSEAL1;   // TC1
  if ( ind==1 ) bit_tip_ok = BIT_CAPSEAL2;   // TC2
  if ( ind==2 ) bit_tip_ok = BIT_CAPSEAL3;   // TC3

  #ifdef REINSPECT_TRAY2 // If defined 2nd deviator sends uncertain rejects to be manually reinspected
    UINT          bit_tip_rm = BIT_CAPSEAL1_RM;   // TC1
    if ( ind==1 ) bit_tip_rm = BIT_CAPSEAL2_RM;   // TC2
    if ( ind==2 ) bit_tip_rm = BIT_CAPSEAL3_RM;   // TC3
  #endif

  //INodeMap* nodemap = camera[ind].device->GetNodeMap();
  //CFloatPtr shutter( nodemap->GetNode( "ExposureTimeAbs" ) );
  //shutter->SetValue( camera[ind].shutter ); // set for first image
  //ASSERT( shutter->GetValue()==camera[ind].shutter );

  int top_insp  = 0;
  int top_rej   = 0;
  int top_time  = 0;
  int back_insp = 0;
  int back_rej  = 0;
  int back_time = 0;

  BOOL toplight_reject  = FALSE;
  BOOL backlight_reject = FALSE;

  #ifdef REINSPECT_TRAY2 // If defined 2nd deviator sends uncertain rejects to be manually reinspected
    BOOL toplight_reject_rm  = FALSE;
    BOOL backlight_reject_rm = FALSE;
  #endif

  tip_insp[ind]->bl_drawovl = TRUE;
  tip_insp[ind]->tl_drawovl = TRUE;


  TRACE("inspect_3d_tipcap[%d] started - entering main loop\n", ind );
  	  //Pour added 20180326
	  unsigned int loopCount;
	  //Pour added 20180326

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 
	  // Pour added for alarming on camera disconnection 20180326
#ifdef SERIAL_N 201700013

		if ( ++loopCount % 10 == 0 )
	    { 
		  if( camera[ind].camera->IsCameraDeviceRemoved())  
		  {
			  alarm_data->on   = TRUE;
			  alarm_data->format = 10;
		      alarm_data->index  = 57;
		      alarm_data->param_0 = ind;
		      ADS_write_parameter(&alarm_ads);
		  }
	  }
#endif
	// Pour added end 20180326

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 500 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          timer.reset();
          tipcap_processing++;

          //if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
          ADS_read_parameter( &ADS_insp_res );

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          // txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
          // camera[ind].text[1]->SetText( txt, 0 );

//          double deltat = 0.000008*(tmstamp-tmstamp_old); // 1000.0*(tmstamp-tmstamp_old)/ACE_FREQUENCY;
//          if ( deltat>20.0) back_light =  TRUE;
//          else              back_light = FALSE;

          double deltat = 0.000008*(tmstamp-tmstamp_old); // 1000.0*(tmstamp-tmstamp_old)/ACE_FREQUENCY;
          if ( deltat>30.0)
          {
            toplight_reject  = FALSE;
            backlight_reject = FALSE;

            back_light =  TRUE;
            //shutter->SetValue( camera[ind].shutter2 );
            //ASSERT( shutter->GetValue()==camera[ind].shutter );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter );
          }
          else
          {
            back_light = FALSE;  // FIRST BACKLIGHT THEN TOP-LIGHT
            //shutter->SetValue( camera[ind].shutter );
            //ASSERT( shutter->GetValue()==camera[ind].shutter2 );
            //TRACE("Shutter=%.0f - %d\n", shutter->GetValue(), camera[ind].shutter2 );
          }  



          //TRACE("frame=%5d - deltat=%6.3lf\n", frmnum, deltat );

          tmstamp_old = tmstamp;

          BOOL update_view = !show_rejects;

          // FIRST BACKLIGHT THEN TOP-LIGHT

          if ( back_light==FALSE ) // TOPLIGHT INSPECTION (SECOND)
          {
            timer.reset();
            if ( image_dsp[ind]->visible && !show_back_light ) SCALE_IMG( ind, img_acq ); // BACKLIGHT to be displayed

            if ( tip_insp[ind] )
            {
              top_insp++;
              tip_insp[ind]->tl_drawovl = image_dsp[ind]->visible && !show_back_light;
              if ( tip_insp[ind]->Process_image( img_acq, 1 ) ) // !=0  reject - NIMG=1 => TOPLIGHT INSPECTION
              {
                toplight_reject = TRUE;
                top_rej++;
              }

              #ifdef REINSPECT_TRAY2 // If defined 2nd deviator sends uncertain rejects to be manually reinspected
                toplight_reject_rm = tip_insp[ind]->toplight_reject_rm;
              #endif // Normal, single reject

              if ( image_dsp[ind]->visible && !show_back_light ) // TOPLIGHT to be displayed
              {
                camera[ind].text[1]->SetText( tip_insp[ind]->txt1, tip_insp[ind]->col1 );
                camera[ind].text[2]->SetText( tip_insp[ind]->txt2, tip_insp[ind]->col2 );
                #ifdef NEWCAP_V40
                  camera[ind].text[4]->SetText( tip_insp[ind]->txt3, tip_insp[ind]->col3 );
                #endif

				#ifdef TEST_QILU
					camera[ind].text[4]->SetText( tip_insp[ind]->txt3, tip_insp[ind]->col3 );
				#endif

                image_dsp[ind]->Draw_image();
              }

              top_time = timer.elapsed_ms();
            }

            // Final update
            txt.Format( string_1480[lng], backlight_reject?string_reject[lng]:string_good[lng], back_rej, back_insp, back_time, toplight_reject?string_reject[lng]:string_good[lng], top_rej, top_insp, top_time );

            #ifdef REINSPECT_TRAY2 // If defined 2nd deviator sends uncertain rejects to be manually reinspected
              if ( toplight_reject_rm || backlight_reject_rm )
              {
                insp_res->val = 0;          // nothing: reject
                update_view = TRUE;
                camera[ind].text[3]->SetText( txt, TXT_DARKRED );  
              }
              else
              {
                insp_res->val = bit_tip_rm; // set good result bit high
                camera[ind].text[3]->SetText( txt, TXT_DARKGREEN );  
              }

              if ( toplight_reject || backlight_reject )
              {
                insp_res->val = 0;          // nothing: reject
                update_view = TRUE;
                camera[ind].text[3]->SetText( txt, TXT_DARKRED );  
              }
              else
              {
                insp_res->val = bit_tip_ok; // set good result bit high
                camera[ind].text[3]->SetText( txt, TXT_DARKGREEN );  
              }

            #else // Normal, single reject

              if ( toplight_reject || backlight_reject )
              {
                insp_res->val = 0;          // nothing: reject
                update_view = TRUE;
                camera[ind].text[3]->SetText( txt, TXT_DARKRED );  
              }
              else
              {
                insp_res->val = bit_tip_ok; // set good result bit high
                camera[ind].text[3]->SetText( txt, TXT_DARKGREEN );  
              }

            #endif



            insp_res->num = trigger_label[camera[ind].triglab];
            ADS_write_parameter( &ADS_insp_res );
            nimg_acq++;

          }
          else // BACKLIGHT INSPECTION (FIRST)
          {
            timer.reset();
            if ( image_dsp[ind]->visible && show_back_light ) SCALE_IMG( ind, img_acq ); // BACKLIGHT to be displayed

            if ( tip_insp[ind] )
            {
              back_insp++;
              //tip_insp[ind]->tl_drawovl = FALSE;
              tip_insp[ind]->bl_drawovl = image_dsp[ind]->visible && show_back_light;
              if ( tip_insp[ind]->Process_image( img_acq, 0 ) ) // !=0  reject - NIMG=0 => BACKLIGHT INSPECTION
              {
                backlight_reject = TRUE;
                back_rej++;
              }
              
              #ifdef REINSPECT_TRAY2 // If defined 2nd deviator sends uncertain rejects to be manually reinspected
                backlight_reject_rm = tip_insp[ind]->backlight_reject_rm;
              #endif // Normal, single reject


              if ( image_dsp[ind]->visible && show_back_light ) // BACKLIGHT to be displayed
              {
                camera[ind].text[1]->SetText( tip_insp[ind]->txt1, tip_insp[ind]->col1 );
                camera[ind].text[2]->SetText( tip_insp[ind]->txt2, tip_insp[ind]->col2 );
                image_dsp[ind]->Draw_image();
              }

              back_time = timer.elapsed_ms();
            }
          }


//          #ifdef SAVE_FULL_PROC
          #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
            if ( image_dsp[ind]->ptr_fulldsp ) image_dsp[ind]->Save_fulldsp( L"processed" );
          #endif

          tipcap_processing--; // increase counter of particle in processing
          //ASSERT( tipcap_processing>=0 && tipcap_processing<=2 );

          #ifdef SAVE_IMAGES // required to save all images
            if ( ind==2 && saveinsp_on && image_dsp[ind]->visible )
            {
              static UINT counter = 0;
              wchar_t name[200+1];
              swprintf( name, 200, L"%s\\tipcap_%04d%02d%02d_%02d%02d%02d_%04d.png", IMAGES_DIRECTORY, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
              CTimer timerw;
              while ( tipcap_processing>0 && timerw.elapsed_ms()<400 ) Sleep(5);
              Capture_screenshot( name );
              if ( saveseq_on ) // if also buffer required
              {
                for ( int i=0 ; i<3 ; i++ )
                {
                  swprintf( name, 200, L"%s\\tipcap[%s]_%04d%02d%02d_%02d%02d%02d_%04d", IMAGES_DIRECTORY, camera[i].name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
                  Save_buffer( tip_insp[i]->img, tip_insp[i]->dimx, tip_insp[i]->dimy, name );
                }
              }
              counter++;
            }
          #endif

        }
        else
        {
          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        
      }
      else // Timeout while waiting for trigger
      {
        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          camera[ind].text[2]->SetText( L"", 0 );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_tipcap;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;

      Stop_ifcamerror( ind );

      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_tipcap;
    }

  }
  exit_inspect_3d_tipcap:

//  image_dsp[ind]->ptr_ext = NULL;
 // DA SISTEMARE! if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);


  TRACE("inspect_3d_tipcap[%d]: exit_inspect_3d_tipcap\n", ind);

// Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.

/*  try
  {
    while ( camera[ind].camera->IsGrabbing() )
    {
      // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
      if ( camera[ind].camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException)) camera[ind].camera->ExecuteSoftwareTrigger();
      // Retrieve grab results and notify the camera event and image event handlers.
      camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
      // Nothing to do here with the grab result, the grab results are handled by the registered event handler.
    }
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
  }  */


  TRACE("inspect_3d_tipcap[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  TRACE("inspect_3d_tipcap[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_tipcap[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );


  TRACE("inspect_3d_tipcap[%d]: STOP GRABBING completed\n", ind);

  // camera[ind].camera->StopGrabbing();
  //while ( camera[ind].camera->IsGrabbing() ) camera[ind].camera->StopGrabbing();

  //ReleaseMutex( mutex );
  //TRACE("inspect_3d_tipcap[%d]: mutex released\n", ind);

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_tipcap[%d] exiting main loop\n", ind );
  
  return 0;
}









// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +     TIP/CAP CAMERA INSPECTION THREAD           +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_tipcap( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE*  img_acq   = NULL;
  UINT   nimg_acq  = 0;
  UINT   nimg_lost = 0;
  CTimer timer;
  CTime  time = CTime::GetCurrentTime();   


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  ASSERT( tip_insp[ind] );

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt, 0 );


  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<15 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  //Pour 20201013
  	// Before testing the callbacks, we manually set the heartbeat timeout to a short value when using GigE cameras.
	// Since for debug versions the heartbeat timeout has been set to 5 minutes, it would take up to 5 minutes 
	// until detection of the device removal.
	CHearbeatHelper heartbeatHelper(*(camera[ind].camera));
	heartbeatHelper.SetValue(1000);  // 1000 ms timeout
	//Pour 20201013

  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );

    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  UINT          bit_tip_ok = BIT_CAPSEAL1;   // TC1
  if ( ind==1 ) bit_tip_ok = BIT_CAPSEAL2;   // TC2
  if ( ind==2 ) bit_tip_ok = BIT_CAPSEAL3;   // TC2


  TRACE("inspect_3d_tipcap[%d] started - entering main loop\n", ind );


  #ifdef NEW_TIP_V30
    if ( ind==2 && tip_insp[ind] )
    {
      tip_insp[ind]->tot_count = 0;
      tip_insp[ind]->tot_time  = 0;
    }
  #endif


	unsigned int loopCount=0;
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

	  	if ( ++loopCount % 10 == 0 )
	{ 
		if( camera[ind].camera->IsCameraDeviceRemoved())  
		{
			Stop_ifcamerror(ind);
		}
	}// detect camera disconnection

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 500 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          timer.reset();
          tipcap_processing++;

          //if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
          ADS_read_parameter( &ADS_insp_res );

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          BOOL update_view = !show_rejects;

          SCALE_IMG( ind, img_acq );


          if ( tip_insp[ind] )
          {
            if ( tip_insp[ind]->Process_image( img_acq, 0 ) ) // !=0  reject
            {
              insp_res->val = 0;          // nothing: reject
              update_view = TRUE;
            }
            else
            {
              insp_res->val = bit_tip_ok; // set good result bit high
            }
          }

          insp_res->num = trigger_label[camera[ind].triglab];
          ADS_write_parameter( &ADS_insp_res );
          nimg_acq++;

          if ( image_dsp[ind]->visible && update_view ) // final display
          {
            camera[ind].text[1]->SetText( tip_insp[ind]->txt1, tip_insp[ind]->col1 );
            camera[ind].text[2]->SetText( tip_insp[ind]->txt2, tip_insp[ind]->col2 );
            image_dsp[ind]->Draw_image();
          }

          txt.Format( string_824[lng], timer.elapsed_us(), nimg_acq, nimg_lost );
          camera[ind].text[3]->SetText( txt, 0 );

//          #ifdef SAVE_FULL_PROC
          #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
            if ( image_dsp[ind]->ptr_fulldsp ) image_dsp[ind]->Save_fulldsp( L"processed" );
          #endif

          #ifndef NEWCAP_V30
            tipcap_processing--; // increase counter of particle in processing
            //ASSERT( tipcap_processing>=0 && tipcap_processing<=2 );
          #endif

          #ifdef SAVE_IMAGES // required to save all images
            if ( ind==2 && saveinsp_on && image_dsp[ind]->visible )
            {
              static UINT counter = 0;
              wchar_t name[200+1];
              swprintf( name, 200, L"%s\\tipcap_%04d%02d%02d_%02d%02d%02d_%04d.png", IMAGES_DIRECTORY, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
              CTimer timerw;
              while ( tipcap_processing>0 && timerw.elapsed_ms()<400 ) Sleep(5);
              Capture_screenshot( name );
              if ( saveseq_on ) // if also buffer required
              {
                for ( int i=0 ; i<3 ; i++ )
                {
                  swprintf( name, 200, L"%s\\tipcap[%s]_%04d%02d%02d_%02d%02d%02d_%04d", IMAGES_DIRECTORY, camera[i].name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
                  Save_buffer( tip_insp[i]->img, tip_insp[i]->dimx, tip_insp[i]->dimy, name );
                }
              }
              counter++;
            }
          #endif


        }
        else
        {
          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        
      }
      else // Timeout while waiting for trigger
      {
        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          camera[ind].text[2]->SetText( L"", 0 );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_tipcap;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_tipcap;
	  Add_camerror_log(ind, e.GetDescription());
    }

  }
  exit_inspect_3d_tipcap:

//  image_dsp[ind]->ptr_ext = NULL;
 // DA SISTEMARE! if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);


  TRACE("inspect_3d_tipcap[%d]: exit_inspect_3d_tipcap\n", ind);

// Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.

/*  try
  {
    while ( camera[ind].camera->IsGrabbing() )
    {
      // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
      if ( camera[ind].camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException)) camera[ind].camera->ExecuteSoftwareTrigger();
      // Retrieve grab results and notify the camera event and image event handlers.
      camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
      // Nothing to do here with the grab result, the grab results are handled by the registered event handler.
    }
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
  }  */


  TRACE("inspect_3d_tipcap[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  TRACE("inspect_3d_tipcap[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_tipcap[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );


  TRACE("inspect_3d_tipcap[%d]: STOP GRABBING completed\n", ind);

  // camera[ind].camera->StopGrabbing();
  //while ( camera[ind].camera->IsGrabbing() ) camera[ind].camera->StopGrabbing();

  //ReleaseMutex( mutex );
  //TRACE("inspect_3d_tipcap[%d]: mutex released\n", ind);

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_tipcap[%d] exiting main loop\n", ind );
  
  return 0;
}


//Pour added 20190428
// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +     NECK CAMERA INSPECTION THREAD           +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_neck( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE*  img_acq   = NULL;
  UINT   nimg_acq  = 0;
  UINT   nimg_lost = 0;
  CTimer timer;
  CTime  time = CTime::GetCurrentTime();   


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetText( txt, 0 );
    return 0;
  }

  ASSERT( neck_insp[ind] );

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt, 0 );


  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<15 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 


	// Before testing the callbacks, we manually set the heartbeat timeout to a short value when using GigE cameras.
	// Since for debug versions the heartbeat timeout has been set to 5 minutes, it would take up to 5 minutes 
	// until detection of the device removal.
	CHearbeatHelper heartbeatHelper(*(camera[ind].camera));
	heartbeatHelper.SetValue(2000);  // 1000 ms timeout

  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
    if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();

  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;

  //UINT bit_tip_ok;   if ( ind== 6)  
  //UINT	  bit_neck_ok = BIT_NECK1;   // TC1
  //if ( ind== 2 ) bit_neck_ok = BIT_NECK2;   // TC2
  //if ( ind== 3 ) bit_neck_ok = BIT_NECK3;   // TC2



  TRACE("inspect_3d_tipcap[%d] started - entering main loop\n", ind );


  #ifdef NEW_TIP_V30
    if ( ind==2 && tip_insp[ind] )
    {
      tip_insp[ind]->tot_count = 0;
      tip_insp[ind]->tot_time  = 0;
    }
  #endif


  unsigned int loopCount=0;
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 
	if ( ++loopCount % 10 == 0 )
	{ 
		if( camera[ind].camera->IsCameraDeviceRemoved())  
		{
			Stop_ifcamerror(ind);
		}
	}// detect camera disconnection

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 500 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {
          timer.reset();
          tipcap_processing++;

          //if ( camera[ind].sequence ) camera[ind].camera->ExecuteSoftwareTrigger();
          ADS_read_parameter( &ADS_insp_res );

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

          BOOL update_view = !show_rejects;

          //SCALE_IMG( ind, img_acq );


          if ( neck_insp[ind] )
          {			            			
			cv::Mat img_in(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8U, static_cast<unsigned char*>(ptrGrabResult->GetBuffer()), ptrGrabResult->GetWidth());
			char name[50];
			sprintf(name,"d:\img%d.jpg",ind);
			cv::imwrite(name,img_in);
			if(	neck_insp[ind]->Process_image( img_in,ind )) // !=0  reject
			{
              insp_res->val = 0;          // nothing: reject
              update_view = TRUE;
            }
            else
            {
              //insp_res->val = bit_neck_ok; // set good result bit high
            }
          }

          insp_res->num = trigger_label[camera[ind].triglab];
          ADS_write_parameter( &ADS_insp_res );
          nimg_acq++;

          if ( image_dsp[ind]->visible && update_view ) // final display
          {
            camera[ind].text[1]->SetText( neck_insp[ind]->txt1, neck_insp[ind]->col1 );
            camera[ind].text[2]->SetText( neck_insp[ind]->txt2, neck_insp[ind]->col2 );
            image_dsp[ind]->Draw_image();
          }

          txt.Format( string_824[lng], timer.elapsed_us(), nimg_acq, nimg_lost );
          camera[ind].text[3]->SetText( txt, 0 );

//          #ifdef SAVE_FULL_PROC
          #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
            if ( image_dsp[ind]->ptr_fulldsp ) image_dsp[ind]->Save_fulldsp( L"processed" );
          #endif

          #ifndef NEWCAP_V30
            tipcap_processing--; // increase counter of particle in processing
            //ASSERT( tipcap_processing>=0 && tipcap_processing<=2 );
          #endif

          #ifdef SAVE_IMAGES // required to save all images
            if ( ind==2 && saveinsp_on && image_dsp[ind]->visible )
            {
              static UINT counter = 0;
              wchar_t name[200+1];
              swprintf( name, 200, L"%s\\tipcap_%04d%02d%02d_%02d%02d%02d_%04d.png", IMAGES_DIRECTORY, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
              CTimer timerw;
              while ( tipcap_processing>0 && timerw.elapsed_ms()<400 ) Sleep(5);
              Capture_screenshot( name );
              if ( saveseq_on ) // if also buffer required
              {
                for ( int i=0 ; i<3 ; i++ )
                {
                  swprintf( name, 200, L"%s\\tipcap[%s]_%04d%02d%02d_%02d%02d%02d_%04d", IMAGES_DIRECTORY, camera[i].name, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), counter );
                  Save_buffer( tip_insp[i]->img, tip_insp[i]->dimx, tip_insp[i]->dimy, name );
                }
              }
              counter++;
            }
          #endif


        }
        else
        {
          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        timer.reset();
        
      }
      else // Timeout while waiting for trigger
      {
        if ( timer.elapsed_sec()>60 )
        {
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          camera[ind].text[2]->SetText( L"", 0 );
        }
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_neck;
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_neck;
    }

  }
  exit_inspect_3d_neck:

//  image_dsp[ind]->ptr_ext = NULL;
 // DA SISTEMARE! if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);


  TRACE("inspect_3d_neck[%d]: exit_inspect_3d_neck\n", ind);

// Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.

/*  try
  {
    while ( camera[ind].camera->IsGrabbing() )
    {
      // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
      if ( camera[ind].camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException)) camera[ind].camera->ExecuteSoftwareTrigger();
      // Retrieve grab results and notify the camera event and image event handlers.
      camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
      // Nothing to do here with the grab result, the grab results are handled by the registered event handler.
    }
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_tipcap: %s\n", msg_err );
  }  */


  TRACE("inspect_3d_neck[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  TRACE("inspect_3d_neck[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_tipcap[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );


  TRACE("inspect_3d_neck[%d]: STOP GRABBING completed\n", ind);

  // camera[ind].camera->StopGrabbing();
  //while ( camera[ind].camera->IsGrabbing() ) camera[ind].camera->StopGrabbing();

  //ReleaseMutex( mutex );
  //TRACE("inspect_3d_tipcap[%d]: mutex released\n", ind);

  //txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_neck[%d] exiting main loop\n", ind );
  
  return 0;
}
//Pour added 20190428





// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +            CAMERA INSPECTION PARTICLE THREAD            +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_particle( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE*   img_acq   = NULL;
  UINT    nimg_acq  = 0;
  //Pour
  UINT nimg_rej  = 0;
  //Pour
  UINT    nimg_lost = 0;
  
  BOOL skip_lost = FALSE;

  ASSERT( particle_insp[ind] || pfsidewall_insp[ind]  );

  CTimer timer;

    UINT numimg;
  
  if ( particle_insp  [ind] ) numimg = particle_insp  [ind]->num_img; // keep it fixed
  if ( pfsidewall_insp[ind] ) numimg = pfsidewall_insp[ind]->num_img; // keep it fixed

  //UINT numimg = particle_insp[ind]->num_img; // keep it fixed

  /* ORIGINAL: WAS OK 
  #ifdef SAVE_IMAGES
    CImage_storage  img_save( numimg, ind );
  #endif */


  #ifdef SAVE_IMAGES
    CImage_storage*  img_save_seq = NULL;
//    if ( saveseq_on && ind==(IS_PC1?6:3) )
//    if ( saveinsp_on && ind==(IS_PC1?3:0) )  // index for PC1/PC2
    if ( saveinsp_on && image_dsp[ind]->visible ) // && ind==0 )
    {
      img_save_seq = new CImage_storage( numimg, ind );
      ASSERT( img_save_seq );
      TRACE("inspect_3d_particle[%d] created img_save_seq object\n",ind);
    }
  #endif



  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<=20 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  //if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	if ( ADS_status==1 && ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  UINT num_isp =  0; // number of inspection cycles completed

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  if ( particle_insp[ind] )
  {
    if ( particle_insp[ind]->mode & PARTICLE_MODE_REFLECTION ) txt.Format( string_810[lng], ind, camera[ind].name );
    else                                                       txt.Format( string_811[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
  }


  //TRACE("inspect_3d_particle[%d]: STARTGRABBING start mutex waiting...\n", ind);
  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  if ( mutex_res == WAIT_ABANDONED ) TRACE("Mutex[%d]=WAIT_ABANDONED\n",ind);
  if ( mutex_res == WAIT_OBJECT_0  ) TRACE("Mutex[%d]=WAIT_OBJECT_0\n",ind);
  if ( mutex_res == WAIT_TIMEOUT   ) TRACE("Mutex[%d]=WAIT_TIMEOUT\n",ind);
  if ( mutex_res == WAIT_FAILED    ) TRACE("Mutex[%d]=WAIT_FAILED\n",ind);

  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //ASSERT( mutex_res==WAIT_OBJECT_0 || mutex_res==WAIT_ABANDONED ); 
  //TRACE("inspect_3d_particle[%d]: STARTGRABBING mutex allowed to proceed\n", ind);

  try
  {
    //camera[ind].camera->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete );
    INodeMap* nodemap = camera[ind].device->GetNodeMap();
    CIntegerPtr acqframecount( nodemap->GetNode( "AcquisitionFrameCount" ) );
    if ( acqframecount && IsWritable( acqframecount ) )
    {
      acqframecount->SetValue( numimg );
      ASSERT( acqframecount->GetValue()==numimg );
      //TRACE("inspect_3d_particle[%d]: AcquisitionFrameCount set to %d images\n", ind, numimg );
    }
    else TRACE("inspect_3d_particle[%d] ERROR: Unable to access <AcquisitionFrameCount> node\n", ind );

    //camera[ind].camera->MaxNumBuffer = 16;

    #if defined TIVS_A60 && !defined SYNCH_PARTIC
      Sleep(100*ind);
    #endif

    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();

    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_particle: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    CLR_BIT( thread_status , 10+ind ); // confirm thread exit
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( mutex_res == WAIT_ABANDONED ) TRACE("Mutex[%d]=WAIT_ABANDONED\n",ind);
  if ( mutex_res == WAIT_OBJECT_0  ) TRACE("Mutex[%d]=WAIT_OBJECT_0\n",ind);
  if ( mutex_res == WAIT_TIMEOUT   ) TRACE("Mutex[%d]=WAIT_TIMEOUT\n",ind);
  if ( mutex_res == WAIT_FAILED    ) TRACE("Mutex[%d]=WAIT_FAILED\n",ind);
  if ( !mutex_res ) TRACE("inspect_3d_particle[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );
  //TRACE("inspect_3d_particle[%d]: STARTGRABBING completed - mutex released\n", ind);

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp1 = 0;
  uint32_t frmnum1  = 0;
  uint64_t tmstamp2 = 0;
  uint32_t frmnum2  = 0;

  UINT bit_particle_ok = 0;
  UINT bit_fillingl_ok = 0;

  if ( fillingl_insp[ind] )
  {
    fillingl_insp[ind]->ninsp = 0;
    fillingl_insp[ind]->nrej  = 0;
  }

  // DEFAULT MACHINES
  if ( IS_PC1 )
  {
    bit_particle_ok = BIT_PARTICLE1;   // PC1
    bit_fillingl_ok = BIT_FILLINGL1;   // PC1
  }
  else // PC2
  {
    #if STN2==COSMETIC_BACK
      bit_particle_ok = BIT_PARTICLE2;   // PC2 - part2
      bit_fillingl_ok = BIT_FILLINGL2;   // PC2 - part2
    #else

      if ( ind>=tlcgroupPC2[1][0] ) // ind>3 ) 
      {
        bit_particle_ok = BIT_COSMETIC;    // PC2 - part3/cosm
      }
      else
      {
        bit_particle_ok = BIT_PARTICLE2;   // PC2 - part2
        bit_fillingl_ok = BIT_FILLINGL2;   // PC2 - part2
      }

    #endif

    //#ifdef TIVS_C20
    //  bit_particle_ok = BIT_COSMETIC;    // PC2 - part3/cosm
    //  bit_fillingl_ok = BIT_FILLINGL2;   // PC2 - part2
    //#endif

	  //Pour changed on 20210306 for testing 
	#ifdef TIVS_C20
      bit_particle_ok = BIT_PARTICLE2;    // PC2 - part3/cosm
      //bit_fillingl_ok = BIT_FILLINGL2;   // PC2 - part2
    #endif


  }

  //  #ifdef TIVS_C40  // freeze/dried vials machine SISTEMARE
  //  if ( ind<=7 ) // PC2 / Particle1
  //  {
  //    bit_particle_ok = BIT_PARTICLE1;
  //    bit_fillingl_ok = BIT_FILLINGL1;
  //  }
  //  if ( ind>=12 ) // PC2 - Particle2
  //  {
  //    bit_particle_ok = BIT_PARTICLE2;
  //    bit_fillingl_ok = BIT_FILLINGL2;
  //  }
  //#endif


  #ifdef TIVS_C60  // freeze/dried vials machine SISTEMARE
    if ( ind<=7 ) // PC2 / Particle1
    {
      bit_particle_ok = BIT_PARTICLE1;
      bit_fillingl_ok = BIT_FILLINGL1;
    }
    if ( ind>=12 ) // PC2 - Particle2
    {
      bit_particle_ok = BIT_PARTICLE2;
      bit_fillingl_ok = BIT_FILLINGL2;
    }
  #endif



  //Pour added for TIVS_A20 rejection
      #ifdef TIVS_A20 // MODEL TIVS-A20 
  if ( IS_PC1 )
  {
    if ( ind>=3 && ind<=4 )
    {
      bit_particle_ok = BIT_PARTICLE1;   // Part1
      bit_fillingl_ok = BIT_FILLINGL1;   // Fill1
    }
    if ( ind>=5 && ind<=6 )
    {
      bit_particle_ok = BIT_PARTICLE2;   // Part1
      bit_fillingl_ok = BIT_FILLINGL2;   // Fill1
    }
    if ( ind>=7 && ind<=8 )
    {
      bit_particle_ok = BIT_COSMETIC;    // Part3/Cosm
    }
  }
  #endif 
  //Pour ddded for TIVS_A20 rejection

  //Pour added for TIVS_A20L 20180917
    #ifdef TIVS_A20L // MODEL TIVS-A20L 
  if ( IS_PC1 )//Pour removed 20180916
  {
    if ( ind>=3 && ind<=4 )
    {
      bit_particle_ok = BIT_PARTICLE1;   // Part1
      bit_fillingl_ok = BIT_FILLINGL1;   // Fill1
    }
    if ( ind>=5 && ind<=6 )
    {
      bit_particle_ok = BIT_PARTICLE2;   // Part1
      bit_fillingl_ok = BIT_FILLINGL2;   // Fill1
    }
    if ( ind>=7 && ind<=8 )
    {
      bit_particle_ok = BIT_COSMETIC;    // Part3/Cosm
    }
  }
  #endif 

    #ifdef TIVS_S40 // MODEL TIVS-S40

    if ( ind==1 ) // C3A
    {
      bit_particle_ok = BIT_BOTTOMA;     // Cosmetic 3A
    }
    if ( ind==2 ) // C3B
    {
      bit_particle_ok = BIT_BOTTOMB;     // Cosmetic 3B
    }

    if ( ind>=3 && ind<=5 )
    {
      bit_particle_ok = BIT_PARTICLE1;   // Part1
      bit_fillingl_ok = BIT_FILLINGL1;   // Fill1
    }
    if ( ind>=6 && ind<=8 )
    {
      bit_particle_ok = BIT_PARTICLE2;   // Part2
      bit_fillingl_ok = BIT_FILLINGL2;   // Fill2
    }

    if ( ind>=12 && ind<=14 ) // C2A/B/C
    {
      bit_particle_ok = BIT_COSMETIC3;   // Cosmetic3 rej1
      bit_fillingl_ok = BIT_COSMETIC4;   // Cosmetic3 rej2
    }
  #endif 


	#ifdef TIVS_S60 // MODEL TIVS-S60

	  if ( IS_PC1 )//Pour removed 20180916
	{

    if ( ind==1 ) // C3A
    {
      bit_particle_ok = BIT_BOTTOMA;     // Cosmetic 3A
    }
    if ( ind==2 ) // C3B
    {
      bit_particle_ok = BIT_BOTTOMB;     // Cosmetic 3B
    }

    if ( ind>=3 && ind<=6 )
    {
      bit_particle_ok = BIT_PARTICLE1;   // Part1
      //bit_fillingl_ok = BIT_FILLINGL1;   // Fill1
    }
    if ( ind>=7 && ind<=10 )
    {
      bit_particle_ok = BIT_PARTICLE2;   // Part2
      //bit_fillingl_ok = BIT_FILLINGL2;   // Fill2
    }
	}

	  if ( IS_PC2 )//Pour removed 20180916
	{

    //if ( ind>=11 && ind<=14 ) // C2A/B/C
    //{
    //  bit_particle_ok = BIT_COSMETIC3;   // Cosmetic3 rej1
    //  //bit_fillingl_ok = BIT_COSMETIC4;   // Cosmetic3 rej2
    //}

	if ( ind>=4 && ind<=7 ) // C2A/B/C/D
    {
      bit_particle_ok = BIT_COSMETIC3;   // Cosmetic3 rej1
      //bit_fillingl_ok = BIT_COSMETIC4;   // Cosmetic3 rej2
	  bit_fillingl_ok = BIT_FILLINGL1;   //Density control on NEEDLE SHIELD
    }
	  }

	#endif 



//  UINT bit_ok = BIT_COSMETIC;  // good for LQ machine, only one station and FD sidewall


  TRACE("inspect_3d_particle[%d] started - entering main loop\n", ind );

  UINT triggerold = trigger_label[camera[ind].triglab];

  timer.reset();
  int    last_wait  = 0;
  unsigned int loopCount = 0;
  // endless loop
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 
	  	  
	  if ( ++loopCount % 10 == 0 )
	  {
		  if( camera[ind].camera->IsCameraDeviceRemoved()) //IsReadable())
			  
		  {
			 Stop_ifcamerror(ind);
		  }

	  }

    // wait sequence start trigger
    timer.reset();
    last_wait = 60;
    //while ( triggerold >= trigger_label[camera[ind].triglab] )

	#ifdef TIVS_S40
      #define Fx_PUSHED(i)  ( GetAsyncKeyState( VK_F1+i-12 ) & 1 ) 
    #else
      #define Fx_PUSHED(i)  ( GetAsyncKeyState( VK_F1+i ) & 1 ) 
    #endif

	#ifdef TIVS_S60
      #define Fx_PUSHED(i)  ( GetAsyncKeyState( VK_F1+i-12 ) & 1 ) 
    #else
      #define Fx_PUSHED(i)  ( GetAsyncKeyState( VK_F1+i ) & 1 ) 
    #endif


    //while ( triggerold==trigger_label[camera[ind].triglab] && !F9_PUSHED )
	while ( triggerold==trigger_label[camera[ind].triglab] && !Fx_PUSHED(ind) )
    {
      Sleep( 1 );
      if ( timer.elapsed_sec()>last_wait )
      {
        last_wait = timer.elapsed_sec();
        txt.Format( string_804[lng], last_wait );
        camera[ind].text[1]->SetText( txt, 0 );
      }
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_particle;
    }  

    #ifdef READ_CYCLES
      UINT cycles = ADS_read_int(&ADS_cycles);
      UINT timesta = globaltimer.elapsed_ms();
      if ( ind==3 ) TRACE("trigger=%2d - cycles=%d - timer=%d - diff=%3d - ", 
        trigger_label[camera[ind].triglab], cycles, timesta, timesta-cycles+cycles_0 );
    #endif

    //particle_processing++; // increase counter of particle in processing

    triggerold = trigger_label[camera[ind].triglab];
    nimg_acq  = 0;
    skip_lost = FALSE;
    timer.reset();

    try // Genicam exception handling
    {
      //camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
      camera[ind].camera->ExecuteSoftwareTrigger();
      #ifdef READ_CYCLES
        if ( ind==3 ) TRACE(" swtrg(%d) ", globaltimer.elapsed_ms() );
      #endif

      while ( nimg_acq < numimg && !skip_lost )
      {
        // Wait for an image and then retrieve it. A timeout of 8000 ms is used.
        if ( camera[ind].camera->RetrieveResult( 8000, ptrGrabResult, TimeoutHandling_Return  ) )
        {

          // Image grabbed successfully?
          if ( ptrGrabResult->GrabSucceeded() )
          {

            //if ( nimg_acq < particle_insp[ind]->num_img-1 ) camera[ind].camera->ExecuteSoftwareTrigger();

            img_acq = (BYTE*) ptrGrabResult->GetBuffer();

            if ( nimg_acq==0 ) // store first image's timestamp and frmnum
            {
              tmstamp1 = ptrGrabResult->GetTimeStamp();
              frmnum1  = ptrGrabResult->GetFrameNumber();
            }

            #ifdef SAVE_IMAGES
              if ( img_save_seq && saveseq_on && image_dsp[ind]->visible ) img_save_seq->add_image( ptrGrabResult, nimg_acq ); // CGrabResultPtr    
            #endif

			//if ( fillingl_insp[ind] && !learning ) fillingl_insp[ind]->Process_image( img_acq, nimg_acq );
            //if ( particle_insp[ind] )              particle_insp[ind]->Process_image( img_acq, nimg_acq );

            //if ( pfsidewall_insp[ind] )            pfsidewall_insp[ind]->Process_image( img_acq, nimg_acq );


            //if ( fillingl_insp[ind] && !learning ) fillingl_insp[ind]->Process_image( img_acq, nimg_acq );
            //if ( particle_insp[ind] )              particle_insp[ind]->Process_image( img_acq, nimg_acq );     //Pour removed 20180601  //Pour 20201103

			//Pour added 20180605
	#ifdef COLOR_DETECTION_P1
			  if(ind==3 ||ind==4 || ind==5 ||ind==6)
			  {
				Process_image(ptrGrabResult, nimg_acq,ind);
				if ( fillingl_insp[ind] && !learning   )  fillingl_insp[ind]->Process_image( image_dsp[ind]->ptr_buf, nimg_acq );
				if ( particle_insp[ind]  )                particle_insp[ind]->Process_image( image_dsp[ind]->ptr_buf, nimg_acq );
			  }
				else
				{
				if ( particle_insp[ind] )                  particle_insp[ind]->Process_image( img_acq, nimg_acq ); 
				//if ( fillingl_insp[ind] && !learning )     fillingl_insp[ind]->Process_image( img_acq, nimg_acq );
				}
	#else
			if ( particle_insp[ind] )                                       particle_insp[ind]->Process_image( img_acq, nimg_acq ); 
			if ( fillingl_insp[ind] && !learning )                          fillingl_insp[ind]->Process_image( img_acq, nimg_acq );

			if ( pfsidewall_insp[ind] )            pfsidewall_insp[ind]->Process_image( img_acq, nimg_acq );
	#endif
			//Pour added 20180605
           
            //TRACE("part[%d] - framen=%5d - timest=%I64u\n",ind,ptrGrabResult->GetFrameNumber(),ptrGrabResult->GetTimeStamp());
            nimg_acq++;
          }
          else
          {
            nimg_lost++;
            skip_lost = TRUE;
            CString msg( ptrGrabResult->GetErrorDescription() );
            TRACE(L"inspect_3d_particle[%d]: GrabSucceeded failed <%s>\n", ind, msg );
            if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_particle;
          }

        }
        else // Timeout while waiting for trigger
        {
          //camera[ind].camera->ExecuteSoftwareTrigger();
          TRACE(L"inspect_3d_particle[%d]: RetrieveResult failed <%s>\n", ind, msg );
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          nimg_acq = numimg;
          if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_particle;
        }


        //camera[ind].camera->ExecuteSoftwareTrigger();

      } // while ( nimg_acq < num_img )


      // SEQUENCE COMPLETED - FINAL UPDATE
      //TRACE("Dfrm=%I32d - Dtmst=%.3lf (%.3lf)\n", deltaframe, deltatime, 1000.0*(deltaframe-1)/deltatime );
      insp_res->num = triggerold;
      insp_res->val = 0;  


	        //if ( particle_insp[ind] )
      //{

      if ( learning )
      {
        if ( image_dsp[ind]->visible )
        {
          txt.Format( string_2046[lng] );
          camera[ind].text[1]->SetText( txt, TXT_DARKBLUE );
//          image_dsp[ind]->Draw_image(TRUE);
          image_dsp[ind]->Draw_image();
        }
      }
      else // normal processing
      {
        BOOL update_view = !show_rejects;
        UINT txt1_col = 0;

        if ( fillingl_insp[ind] )
        {
          if ( fillingl_insp[ind]->good )
          {
            insp_res->val |= bit_fillingl_ok; // set good result bit high
          }
          else // filling level reject
          {
            update_view = TRUE;

            #if SERIAL_N==201400033 // New standard liquid machine Lunan Houpu O40
              if ( alarmemptypc1 && ind>=3 && ind<=6 && fillingl_insp[ind]->hliv==0 )
              {
                alarm_data->on     = TRUE;
                alarm_data->format =    0;
                alarm_data->index  =   45;
                ADS_write_parameter( &alarm_ads );
                //logger->Add_string(L"Empty container on PC1 - Setting Alarm");
              }
            #endif
          }
        }


        #ifdef NEW_PARTICLE
          if ( particle_insp[ind]->nimgpt>=particle_insp[ind]->minnumimg && particle_insp[ind]->totnpt>=particle_insp[ind]->mintotnpt )//Pour removed 20180605

          {
            //txt.Format( L"REJECT err %d/%d (%d/%d)", particle_insp[ind]->totnpt, particle_insp[ind]->mintotnpt, particle_insp[ind]->nimgpt, particle_insp[ind]->minnumimg );
			txt.Format( L"REJECT err %d/%d (%d/%d)", particle_insp[ind]->totnpt, particle_insp[ind]->mintotnpt, particle_insp[ind]->nimgpt, particle_insp[ind]->minnumimg );
            txt1_col = TXT_DARKRED;
			nimg_rej++; //Pour added on 20201012
            update_view = TRUE;
          }
          else if(particle_insp[ind]->dist_lab > particle_insp[ind]->max_dist_lab && particle_insp[ind]->max_dist_lab!= 0) //Pour added color detection judgement 20180605
			   {
				update_view = TRUE;
			   }
		  else
          {
            insp_res->val |= bit_particle_ok; // set good result bit high
            txt.Format( L"GOOD err %d/%d (%d/%d)", particle_insp[ind]->totnpt, particle_insp[ind]->mintotnpt, particle_insp[ind]->nimgpt, particle_insp[ind]->minnumimg );
            txt1_col = TXT_DARKGREEN;
          }
        #else
          if ( particle_insp[ind]->part_err > particle_insp[ind]->max_err ) // PARTICLE REJECT
          {
            txt.Format( string_842[lng], particle_insp[ind]->part_err, particle_insp[ind]->max_err );
            txt1_col    = TXT_DARKRED;
            update_view = TRUE;
          }
          else      // PARTICLE GOOD
          {
            insp_res->val |= bit_particle_ok; // set good result bit high
            txt.Format( string_840[lng], particle_insp[ind]->part_err, particle_insp[ind]->max_err );
            txt1_col    = TXT_DARKGREEN;
          }
        #endif
	  //}

        if ( update_view /*&& image_dsp[ind]->visible*/ )
        {
//          image_dsp[ind]->Draw_image(TRUE);
          if ( image_dsp[ind]->visible ) image_dsp[ind]->Draw_image();
          camera[ind].text[1]->SetText( txt, txt1_col );
          if ( fillingl_insp[ind] ) camera[ind].text[2]->SetText( fillingl_insp[ind]->txt, fillingl_insp[ind]->col );
        }


        #ifdef SAVE_INPUTDATA
          if ( inpdatarec_on && fillev_result )
          {
            fillev_result->InsertI( fillingl_insp[ind]->npt, ind ); // 
          }
        #endif
		  /*
//Pour marked to be confirmed on 20190723
        if ( datarec_on && result )
        {
          //txt.Format(L"inspect_3d_particle[%d]",ind);
          ASSERT( triggerold>=0 && triggerold<60 );

          if ( result2 )
          {
            if ( ind<4 ) result ->InsertI( particle_insp[ind]->part_err, triggerold ); // particle2 into result
            else         result2->InsertI( particle_insp[ind]->part_err, triggerold ); // particle3 into result2
          }
          else result->InsertI( particle_insp[ind]->part_err, triggerold ); // only particle2 

        }
//Pour marked to be confirmed on 20190723
*/

//Pour marked to be confirmed on 20190723
	#ifdef NEW_PARTICLE
		if (datarec_on && result)
		{
			//txt.Format(L"inspect_3d_particle[%d]",ind);
			ASSERT(triggerold >= 0 && triggerold<90);

			if (result2)
			{
				if (ind<6) result->InsertI(particle_insp[ind]->totnpt, triggerold); // particle2 into result      //Pour marked on 20201012 changed from part_err to totnpt
				else         result2->InsertI(particle_insp[ind]->totnpt, triggerold); // particle3 into result2  //Pour marked on 20201012 changed from part_err to totnpt
			}
			else result->InsertI(particle_insp[ind]->totnpt, triggerold); // only particle2   //Pour marked on 20201012 changed from part_err to totnpt

		}
	#endif
		//Pour marked to be confirmed on 20190723

      }

	        if ( pfsidewall_insp[ind] )
      {
        if ( pfsidewall_insp[ind]->good )
        {
          insp_res->val |= bit_particle_ok; // set good result bit high
          //txt.Format( L"REJECT err %d/%d (%d/%d)", particle_insp[ind]->totnpt, particle_insp[ind]->mintotnpt, particle_insp[ind]->nimgpt, particle_insp[ind]->minnumimg );
          //txt.Format( L"REJECT" );
          //txt1_col = TXT_DARKRED;
        }
        else
        {
          //txt.Format( L"GOOD err %d/%d (%d/%d)", particle_insp[ind]->totnpt, particle_insp[ind]->mintotnpt, particle_insp[ind]->nimgpt, particle_insp[ind]->minnumimg );
          //txt.Format( L"GOOD" );
          //txt1_col = TXT_DARKGREEN;
          //update_view = FALSE;      //Pour marked on 20200921
        }

        if ( image_dsp[ind]->visible ) 
        {
          image_dsp[ind]->Draw_image();
          camera[ind].text[1]->SetText( pfsidewall_insp[ind]->txt1, pfsidewall_insp[ind]->col1 );
          camera[ind].text[2]->SetText( pfsidewall_insp[ind]->txt2, pfsidewall_insp[ind]->col2 );
        }

        //TRACE("inspect_3d_particle[%d] - final update\n", ind );

      } // if ( pfsidewall_insp[ind] )


      ADS_write_parameter( &ADS_insp_res );

      num_isp++;//Pour marked on 20201012
      int deltaframe = ptrGrabResult->GetFrameNumber() - frmnum1 + 1;
      int deltatime  = int( ACE_CFMS*( ptrGrabResult->GetTimeStamp() - tmstamp1 ) );
//      txt.Format( string_832[lng], num_isp, nimg_acq, nimg_lost, timer.elapsed_ms() );
      txt.Format( string_3514[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
	        
		#ifdef TIVS_S40
        txt.Format( string_3514[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #else
        txt.Format( string_3514[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #endif

		#ifdef TIVS_S60
        txt.Format( string_3514[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #else
        txt.Format( string_3514[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #endif

		#ifdef TIVS_A60
        txt.Format( string_3514a[lng], nimg_rej, num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #else
        txt.Format( string_3514a[lng], nimg_rej, num_isp,  nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      #endif


      camera[ind].text[3]->SetWindowTextW( txt );
      nimg_acq = 0;


      #ifdef READ_CYCLES
        cycles = ADS_read_int(&ADS_cycles);
        UINT timeend = globaltimer.elapsed_ms();
        if ( ind==3 ) TRACE(" cycles=%d - timer=%d - diff=%d\n", 
           cycles, timeend, timeend-timesta ); // cycles+cycles_0 );
      #endif

//      particle_processing--; // increase counter of particle in processing
//      ASSERT( particle_processing>=0 && particle_processing<=3 );

      #ifdef SAVE_IMAGES // required to save all images
        //ASSERT( 0 ); // tutto da sistemare
        if ( saveinsp_on && img_save_seq && image_dsp[ind]->visible ) img_save_seq->save_sequence();
/*        {
          //txt.Format( string_816[lng], ind, camera[ind].name, triggerold+1 );
          //camera[ind].text[0]->SetWindowTextW( txt );
          if ( img_save_seq )
          {
           // CTimer maxwait;
  //          while( particle_processing>0 && maxwait.elapsed_ms()<300 ) Sleep(5);
            img_save_seq->save_sequence();
          } 
        } */
      #endif

    }
    catch (GenICam::GenericException &e)
    {
      //static GenICam::GenericException e_old;

      static wchar_t msg_old[201] = L"";

      nimg_lost++;
      skip_lost = TRUE;
//      while ( nimg_acq < numimg && !skip_lost )
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_particle: %s\n", msg_err );


      if ( wcscmp( msg_err, msg_old ) && logger )
      {
        //txt.Format( L"Camera <%s> GenicamException in inspect_3d_particle <%s>", camera[ind].name, msg_err );
        swprintf( msg_old, 200, L"inspect_3d_particle[%s] - genicam exception thrown <%s>\n", camera[ind].name, AtoW(e.GetDescription()) );
        logger->Add_string( msg_old );
        wcscpy( msg_old, msg_err );
      }
      
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
      Add_camerror_log(ind, e.GetDescription());

      if ( CHK_BIT( thread_exit , 10+ind ) )
      {
        //CLR_BIT( thread_status , 10+ind ); // confirm thread exit
        //CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
        goto exit_inspect_3d_particle;
      }
    }

  }
  exit_inspect_3d_particle:
  //camera[ind].camera->ExecuteSoftwareTrigger();

  #ifdef SAVE_IMAGES
//    if ( frames ) free( frames );
  #endif

//  image_dsp[ind]->ptr_ext = NULL;
//  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("inspect_3d_particle[%d]: STOPGRABBING start mutex waiting...\n", ind);
  //mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  //ASSERT( mutex_res==WAIT_OBJECT_0 ); 
  //ASSERT( mutex_res==WAIT_OBJECT_0 || mutex_res==WAIT_ABANDONED ); 
  //TRACE("inspect_3d_particle[%d]: STOPGRABBING mutex allowed to proceed\n", ind);
  // WAIT_ABANDONED - 0x00000080L The specified object is a mutex object that was not released by the thread that owned the mutex object before the owning thread terminated. Ownership of the mutex object is granted to the calling thread and the mutex state is set to nonsignaled.
  // WAIT_OBJECT_0  - 0x00000000L The state of the specified object is signaled.
  // WAIT_TIMEOUT   - 0x00000102L The time-out interval elapsed, and the object's state is nonsignaled.
  // WAIT_FAILED    - 0xFFFFFFFF  The function has failed. To get extended error information, call GetLastError. 

  /*
  try // Genicam exception handling
  {
    camera[ind].camera->StopGrabbing();
  }
  catch (GenICam::GenericException &e)
  {
    //nimg_lost++;
    //size_t convertedChars = 0;
    //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"inspect_3d_particle[%d]: genicam exception thrown on StopGrabbing <%s>\n", ind, msg_err );
    //TRACE( "inspect_3d_particle[%d]: genicam exception thrown on StopGrabbing <%s>\n", ind, e.GetDescription() );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_particle: %s\n", msg_err );
  }  

  TRACE( L"inspect_3d_particle[%d]: waiting acquisition completed before StopGrabbing ", ind );
  while ( camera[ind].camera->IsGrabbing() )
  {
    Sleep(100);
    TRACE(".");
  }
  TRACE(" completed\n");


  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_particle[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );
  //TRACE("inspect_3d_particle[%d]: STOP GRABBING completed - mutex released\n", ind);
  */



  #ifdef SAVE_IMAGES
    if ( img_save_seq )
    {
      delete img_save_seq;
    }
  #endif


  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_particle[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );




  /*
  if ( datarec_on && result )
  {
    //TRACE( "inspect_3d_particle[%d]: start Create_csv()\n", ind );
    txt.Format( L"Particle_inspection[%d]", ind );
    result->Create_csv( txt.GetBuffer(), TRUE ); // create with header
    //TRACE( "inspect_3d_particle[%d]: start Save_csv()\n", ind );

//    txt.Format( d );
    result->Addrow_csv( L"Insp. number, Part. error\r\n" );

    result->Save_csv();
    //TRACE( "inspect_3d_particle[%d]: completed Save_csv()\n", ind );
    delete result;
    result = NULL;
  }
  */

  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  //TRACE("inspect_3d_particle[%d] exiting main loop thread_exit=%d\n", ind, thread_exit );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_particle[%d] exiting main loop\n", ind );
  
  return 0;
}









// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +         CAMERA INSPECTION AREA BACKLIGHT THREAD         +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_cakesidearback( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE*   img_acq   =  NULL;
  UINT    nimg_acq  =     0;
  UINT    nimg_lost =     0;
  UINT    num_isp   =     0; // number of inspection cycles completed

  BOOL skip_lost    = FALSE;

  ASSERT( waistlayer_insp[ind] || skinning_insp[ind] );

  if ( !waistlayer_insp[ind] && !skinning_insp[ind] )
  {
    txt.Format( L"Error: inspection object <waistlayer_insp/skinning_insp[%d]> not initialized\r\nExiting thread", ind );
    Message_box( txt, MBOX_ERROR );
    CLR_BIT( thread_status , 10+ind ); // confirm thread exit
    CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
    TRACE("inspect_3d_cakesidearback[%d] forced exit\n", ind );
    return 0;
  }

  CTimer timer;

  UINT numimg = 0;
  
  if ( waistlayer_insp[ind] ) numimg = waistlayer_insp[ind]->num_img; // keep it fixed
  if ( skinning_insp  [ind] ) numimg = skinning_insp  [ind]->num_img; // keep it fixed

  ASSERT( numimg>0 && numimg<50 );

  #ifdef SAVE_IMAGES
    CImage_storage*  img_save_seq = NULL;
//    if ( saveseq_on && ind==(IS_PC1?6:3) )
//    if ( saveinsp_on && ind==(IS_PC1?3:3) )
    if ( saveinsp_on )
    {
      img_save_seq = new CImage_storage( numimg, ind );
      ASSERT( img_save_seq );
      TRACE("inspect_3d_cakesidearback[%d] created img_save_seq object\n");
    }
  #endif



  // connect insp_res object for results output
  ADS_data_type ADS_insp_res;
  char name[50];
  ASSERT( camera[ind].triglab>=0 && camera[ind].triglab<=21 );
  sprintf( name, "PRODUCT_FLOW.INSP_RES[%d]", camera[ind].triglab );
  if ( ADS_init_parameter( name, &ADS_insp_res, sizeof(insp_res_type) ) )
	{
    txt.Format( L"Error in getting handle of <%s>", name );
  	Message_box( msg, MBOX_ERROR );
	}
  insp_res_type* insp_res = (insp_res_type*) ADS_insp_res.val;


  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( L"Camera[%d] %s cakebklgt", ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  try
  {
    //camera[ind].camera->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete );
    INodeMap* nodemap = camera[ind].device->GetNodeMap();
    CIntegerPtr acqframecount( nodemap->GetNode( "AcquisitionFrameCount" ) );
    if ( acqframecount && IsWritable( acqframecount ) )
    {
      acqframecount->SetValue( numimg );
      ASSERT( acqframecount->GetValue()==numimg );
      //TRACE("inspect_3d_particle[%d]: AcquisitionFrameCount set to %d images\n", ind, numimg );
    }
    else TRACE("inspect_3d_cakesidearback[%d] ERROR: Unable to access <AcquisitionFrameCount> node\n", ind );

    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
  }
  catch (GenICam::GenericException &e)
  {
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_particle: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_cakesidearback[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );
  //TRACE("inspect_3d_particle[%d]: STARTGRABBING completed - mutex released\n", ind);

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp1 = 0;
  uint32_t frmnum1  = 0;
  uint64_t tmstamp2 = 0;
  uint32_t frmnum2  = 0;

  UINT bit_ok = BIT_WAIST;
  if ( skinning_insp[ind] ) bit_ok = BIT_PARTICLE2; // top-cake/skinning

  TRACE("inspect_3d_cakesidearback[%d] started - entering main loop\n", ind );

  UINT triggerold = trigger_label[camera[ind].triglab];

  timer.reset();
  int last_wait  = 0;

  CInspection_result* inspection_result = new CInspection_result();

  if ( inspection_result )
  {
    delete inspection_result;
    inspection_result = 0;
  }

  ASSERT( ind>=0 && ind<=11 ); // IF NOT CANNOT USE Fxx_PUSHED BELOW!
  // FOR AUSIA TESTS:
  // F5 ind=4 T3_A
  // F6 ind=5 T3_B
  // F7 ind=6 T3_C
  // F8 ind=7 T3_D

  // endless loop
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    // wait sequence start trigger
    timer.reset();
    last_wait = 60;
//    while ( triggerold==trigger_label[camera[ind].triglab] && !F9_PUSHED )
    while ( triggerold==trigger_label[camera[ind].triglab] && !Fxx_PUSHED(ind) )
    {
      Sleep( 1 );
      if ( timer.elapsed_sec()>last_wait )
      {
        last_wait = timer.elapsed_sec();
        txt.Format( string_804[lng], last_wait );
        camera[ind].text[1]->SetText( txt, 0 );
      }
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_cakesidearback;
    }

    triggerold = trigger_label[camera[ind].triglab];
    nimg_acq  = 0;
    skip_lost = FALSE;
    timer.reset();

    try // Genicam exception handling
    {
      camera[ind].camera->ExecuteSoftwareTrigger();

      while ( nimg_acq < numimg && !skip_lost )
      {
        // Wait for an image and then retrieve it. A timeout of 8000 ms is used.
        if ( camera[ind].camera->RetrieveResult( 8000, ptrGrabResult, TimeoutHandling_Return  ) )
        {

          // Image grabbed successfully?
          if ( ptrGrabResult->GrabSucceeded() )
          {
            img_acq = (BYTE*) ptrGrabResult->GetBuffer();

            if ( nimg_acq==0 ) // store first image's timestamp and frmnum
            {
              tmstamp1 = ptrGrabResult->GetTimeStamp();
              frmnum1  = ptrGrabResult->GetFrameNumber();
            //  SCALE_IMG( ind, img_acq ); // scale first
            }

            #ifdef SAVE_IMAGES
              if ( img_save_seq && saveseq_on ) img_save_seq->add_image( ptrGrabResult, nimg_acq ); // CGrabResultPtr    
            #endif

            if ( waistlayer_insp[ind] )
            {
              waistlayer_insp[ind]->Process_image( img_acq, nimg_acq );
              if ( !waistlayer_insp[ind]->imgshw && image_dsp[ind]->visible ) image_dsp[ind]->Draw_image();
            }
            if ( skinning_insp[ind] )
            {
              skinning_insp[ind]->Process_image( img_acq, nimg_acq );
              if ( !skinning_insp[ind]->imgshw && image_dsp[ind]->visible ) image_dsp[ind]->Draw_image();
            }
       
    //        if ( image_dsp[ind]->visible ) image_dsp[ind]->Draw_image();


            //TRACE("part[%d] - framen=%5d - timest=%I64u\n",ind,ptrGrabResult->GetFrameNumber(),ptrGrabResult->GetTimeStamp());
            nimg_acq++;
          }
          else
          {
            nimg_lost++;
            skip_lost = TRUE;
            CString msg( ptrGrabResult->GetErrorDescription() );
            TRACE(L"inspect_3d_particle[%d]: GrabSucceeded failed <%s>\n", ind, msg );
            if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_cakesidearback;
          }

        }
        else // Timeout while waiting for trigger
        {
          TRACE(L"inspect_3d_cakesidearback[%d]: RetrieveResult failed <%s>\n", ind, msg );
          txt.Format( string_804[lng], timer.elapsed_sec() );
          camera[ind].text[1]->SetText( txt, 0 );
          nimg_acq = numimg;
          if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_cakesidearback;
        }

      } // while ( nimg_acq < num_img )


      // SEQUENCE COMPLETED - FINAL UPDATE
      //TRACE("Dfrm=%I32d - Dtmst=%.3lf (%.3lf)\n", deltaframe, deltatime, 1000.0*(deltaframe-1)/deltatime );
      insp_res->num = triggerold;
      insp_res->val = 0;   

      BOOL update_view = !show_rejects;
      
      if ( waistlayer_insp[ind] ) 
      {
        waistlayer_insp[ind]->Final_result();
        if ( waistlayer_insp[ind]->good ) // GOOD
        {
          insp_res->val |= bit_ok; // set good result bit high
        }
        else update_view = TRUE;   // REJECT
      }

      if ( skinning_insp[ind] ) 
      {
        skinning_insp[ind]->Final_result();
        if ( skinning_insp[ind]->good ) // GOOD
        {
          insp_res->val |= bit_ok; // set good result bit high
        }
        else update_view = TRUE;   // REJECT
      }

      if ( update_view  )
      {
        if ( image_dsp[ind]->visible ) image_dsp[ind]->Draw_image();
        if ( waistlayer_insp[ind] ) 
        {
          camera[ind].text[1]->SetText( waistlayer_insp[ind]->txt1, waistlayer_insp[ind]->col1 );
          camera[ind].text[2]->SetText( waistlayer_insp[ind]->txt2, waistlayer_insp[ind]->col2 );
        }
        if ( skinning_insp[ind] ) 
        {
          camera[ind].text[1]->SetText( skinning_insp[ind]->txt1, skinning_insp[ind]->col1 );
          camera[ind].text[2]->SetText( skinning_insp[ind]->txt2, skinning_insp[ind]->col2 );
        }
      }


      ADS_write_parameter( &ADS_insp_res );

      num_isp++;
      int deltaframe = ptrGrabResult->GetFrameNumber() - frmnum1 + 1;
      int deltatime  = int( ACE_CFMS*( ptrGrabResult->GetTimeStamp() - tmstamp1 ) );
//      txt.Format( string_832[lng], num_isp, nimg_acq, nimg_lost, timer.elapsed_ms() );
      txt.Format( string_834[lng], num_isp, nimg_acq, deltaframe, nimg_lost, deltatime, timer.elapsed_ms(), 1000.0*(deltaframe-1)/deltatime );
      camera[ind].text[3]->SetWindowTextW( txt );
      nimg_acq = 0;

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;

      Stop_ifcamerror( ind );

      skip_lost = TRUE;
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_cakesidearback: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
      if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_cakesidearback;
    }

  }

  exit_inspect_3d_cakesidearback:

  #ifdef SAVE_IMAGES
    if ( img_save_seq ) delete img_save_seq;
//#error 23345
  #endif

  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_cakesidearback[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );


  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetText( txt , 0 );
  camera[ind].text[1]->SetText( L"-", 0 );
  camera[ind].text[2]->SetText( L"-", 0 );
  camera[ind].text[3]->SetText( L"-", 0 );

  if ( ADS_insp_res.val ) free( ADS_insp_res.val );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_cakesidearback[%d] exiting main loop\n", ind );
  
  return 0;
}





// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +            CAMERA INSPECTION THREAD            +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_filllev( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;
  double freq_acq = 0.0;

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_808[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  try
  {
    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    //camera[ind].camera->SetBufferFactory( buffact );
    //camera[ind].camera->MaxNumBuffer = 5;
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
  }
  catch (GenICam::GenericException &e)
  {
    //size_t convertedChars = 0;
    //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    //TRACE( L"Genicam exception thrown <%s>\n", msg_err );
    //TRACE( "Genicam exception thrown <%s>\n", e.GetDescription() );
    swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
    TRACE( L"inspect_3d_filllev: %s\n", msg_err );
    if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	Add_camerror_log(ind, e.GetDescription());
    return 99;
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("movie_3d[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0, tmstamp_ist = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;


  int  ret     = 0;
  bool out     = true;
  bool out_old = false;
  CTimer timer_output;
  set_camera_output( ind, false );

  TRACE("inspect_3d_filllev[%d] started - entering main loop\n", ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    try // Genicam exception handling
    {

      // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
      if ( camera[ind].camera->RetrieveResult( 1000, ptrGrabResult, TimeoutHandling_Return  ) )
      {

        // Image grabbed successfully?
        if ( ptrGrabResult->GrabSucceeded() )
        {

          img_acq = (BYTE*) ptrGrabResult->GetBuffer();
          tmstamp = ptrGrabResult->GetTimeStamp();
          frmnum  = ptrGrabResult->GetFrameNumber();

      /*    if ( image_dsp[ind]->visible && tmstamp-tmstamp_old>0.030*ACE_FREQUENCY )
          {
            SCALE_IMG( ind, img_acq );
            image_dsp[ind]->Draw_image();

            txt.Format( string_820[lng], double(ACE_FREQUENCY)*(frmnum-frmnum_old)/(tmstamp-tmstamp_old) );
            camera[ind].text[1]->SetWindowTextW( txt );

            tmstamp_old = tmstamp;
            frmnum_old  = frmnum;

            txt.Format( string_822[lng], nimg_acq, nimg_lost );
            camera[ind].text[2]->SetWindowTextW( txt );
          }  */

          timer.reset();



          if ( image_dsp[ind]->visible && tmstamp-tmstamp_old>0.050*ACE_FREQUENCY )
          {
            SCALE_IMG ( ind, img_acq );
            if ( filling_level[ind] ) ret = filling_level[ind]->Process_image( img_acq );
//            image_dsp[ind]->Draw_image( TRUE );
            image_dsp[ind]->Draw_image();

            double freq_acq = double(frmnum-frmnum_old)*ACE_FREQUENCY/(tmstamp-tmstamp_old);
            txt.Format( string_828[lng], nimg_acq, nimg_lost, timer.elapsed_ms(), freq_acq );
            camera[ind].text[2]->SetWindowTextW( txt );

            tmstamp_old = tmstamp;
            frmnum_old  = frmnum;
          }
          else
          {
            if ( filling_level[ind] ) ret = filling_level[ind]->Process_image( img_acq );
          }

          if ( ret==900 ) out = false; // REJECT: AMPOULE NECK FOUND BUT FILLING LEVEL IS NOT GOOD/ACCEPTABLE
          else            out = true;

          if ( out!=out_old && timer_output.elapsed_ms()>100 )
          {
            set_camera_output( ind, out );
            TRACE("changing output signal %d=>%d\n", out_old, out );
            out_old = out;
            timer.reset();
          }

          nimg_acq++;
        }
        else
        {

          nimg_lost++;
          CString msg( ptrGrabResult->GetErrorDescription() );
          TRACE(L"GrabSucceeded[%d] failed: <%s>\n", ind, msg );
        }

        
      }
      else // Timeout while waiting for trigger
      {
        txt.Format( string_804[lng], timer.elapsed_sec() );
        camera[ind].text[1]->SetWindowTextW( txt );
        //TRACE("RetrieveResult[%d] failed\n", ind );
      }

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;

      Stop_ifcamerror( ind );

      //size_t convertedChars = 0;
      //mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      //TRACE( L"inspect_3d_filllev[%d]: genicam exception thrown <%s>\n", ind, msg_err );
      //TRACE( "inspect_3d_filllev[%d]: genicam exception thrown <%s>\n", ind, e.GetDescription() );
      swprintf( msg_err, 200, L"Genicam exception thrown <%s>\n", AtoW(e.GetDescription()) );
      TRACE( L"inspect_3d_filllev: %s\n", msg_err );
      if ( camera[ind].text[3] ) camera[ind].text[3]->SetText( msg_err, TXT_DARKRED );
	  Add_camerror_log(ind, e.GetDescription());
    }

  }

//  image_dsp[ind]->ptr_ext = NULL;
//  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

//  camera[ind].camera->StopGrabbing();

  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 

  while ( camera[ind].camera->IsGrabbing() )
  {
    camera[ind].camera->StopGrabbing();
    Sleep(100);
  }

  mutex_res = ReleaseMutex( mutex );
  if ( !mutex_res ) TRACE("inspect_3d_filllev[%d]: ERROR %d WHILE RELEASING MUTEX\n", GetLastError() );



  set_camera_output( ind, false );


  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_filllev[%d] exiting main loop\n", ind );
  
  return 0;
}






/* FIRST VERSION 
UINT __cdecl inspect_3d( LPVOID pParam )
{
  int ind = (int) pParam;
  ASSERT( ind>=0 && ind<NUM_TLC );

  CString txt;
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  CTimer timer;

  SET_BIT( thread_status, 10+ind );

  camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();

  txt.Format( string_810[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );

  camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp = 0, tmstamp_old = 0;
  uint32_t frmnum  = 0, frmnum_old  = 0;;


  TRACE("inspect_3d[%d] started - entering main loop\n", ind );
  SET_BIT( thread_status, 10+ind );

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

   // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
    camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException );

    // Image grabbed successfully?
    if ( ptrGrabResult->GrabSucceeded() )
    {

      img_acq = (BYTE*) ptrGrabResult->GetBuffer();
      tmstamp = ptrGrabResult->GetTimeStamp();
      frmnum  = ptrGrabResult->GetFrameNumber();

//      if ( tmstamp - tmstamp_old > 0.030*ACE_FREQUENCY )
//      {
  
      SCALE_IMG( ind, img_acq );


      if ( filling_level[ind] ) filling_level[ind]->Process_image( img_acq );

      image_dsp[ind]->Draw_image();


//      }

      nimg_acq++;
    }
    else
    {
      nimg_lost++;
      TRACE("Unable to grab image error %s\n", ptrGrabResult->GetErrorDescription() );
    }
  }

  image_dsp[ind]->ptr_ext = NULL;
  CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size );

  camera[ind].camera->StopGrabbing();

//  txt.Format( string_812[lng], ind+1, camera[ind].name );
  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d[%d] exiting main loop\n", ind );

  return 0;
}
*/





/*

// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +               CAMERA MOVIE THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl movie_3d( LPVOID pParam )
{
	int ind = int(pParam);
  ASSERT( ind>=0 && ind<NUM_TLC );

  if ( !camera[ind] ) // || !StreamGrabber[ind] ) 
  {
    //TRACE ("<Camera[%d] not ready - exiting movie_3d[%d]>\n", ind, ind );
    TRACE ( L"<Camera[%d] not ready - exiting movie_3d[%d]>\n", ind, ind );
    return 0;
  }
  ASSERT( camera[ind] ); //&& StreamGrabber[ind] );
  SET_BIT( thread_status, 10+ind );

/*  if ( (tlc_status[ind]&TS_TLC_OK)==0 )
  {
    TRACE("<movie_3d[%d]: camera not initialized - unable to start thread\n", ind);
    pView->tlctext_1[ind]->SetText( "NO CAMERA DETECTED", 0 );
    pView->tlctext_2[ind]->SetText( "", 0 );
    pView->tlctext_3[ind]->SetText( "", 0 );
    Sleep( 500 );
    exit_btn->EnableWindow( TRUE );
    return( 0 );
  }*/

/*
//  TRACE("<movie_3d[%d]: thread started>\n", ind);
  TRACE( L"<movie_3d[%d]: thread started>\n", ind );

  BYTE *img_acq = NULL;

  //hdc = GetDC( pView->m_hWnd );
  //ASSERT(hdc);

  //int n=0, round=0;
  //char msg[120];
  wchar_t msg[120];
  double freq = 0.0;
  UINT buffer_lost = 0;
  UINT nimg_acq    = 0;
  int wait_sec = 0;
  CTimer timer;
  GrabResult Result;
  StreamBufferHandle buffer_handle[NBUFFER_BASLER];
  uint8_t*           buffer       [NBUFFER_BASLER];

  ZeroMemory( buffer_handle, sizeof(buffer_handle) );
  ZeroMemory( buffer       , sizeof(buffer       ) );

  //sprintf( msg, "Camera[%d]: Movie mode running", ind );
  swprintf( msg, L"Camera[%d]: Movie mode running", ind );
  pView->tlctext_1[ind]->SetText( msg, 0 );
  pView->tlctext_2[ind]->SetText( L"", 0 );
  pView->tlctext_3[ind]->SetText( L"", 0 );
//  pView->tlctext_2[ind]->SetText( "", 0 );
//  pView->tlctext_3[ind]->SetText( "", 0 );

 // camera[ind]->AcquisitionMode.SetValue( AcquisitionMode_Continuous );

  // Allocate all resources for grabbing. Critical parameters like image
  // size now must not be changed until FinishGrab() is called.
  //StreamGrabber[ind]->PrepareGrab();

  // Allocate buffer and buffer handle
/*  #ifdef TEMP_TLC

  for ( int i=0 ; i<NBUFFER_BASLER ; i++ )
  {    
    buffer[i] = (uint8_t*) _aligned_malloc( ind==0?1920*1080:DIMX_ACQ*DIMY_ACQ, 16 );
    ASSERT( buffer[i] );
    buffer_handle[i] = StreamGrabber[ind]->RegisterBuffer( buffer[i], ind==0?1920*1080:DIMX_ACQ*DIMY_ACQ );
    StreamGrabber[ind]->QueueBuffer( buffer_handle[i], NULL);
  }

  #else

  for ( int i=0 ; i<NBUFFER_BASLER ; i++ )
  {    
    buffer[i] = (uint8_t*) _aligned_malloc( DIMX_ACQ*DIMY_ACQ, 16 );
    ASSERT( buffer[i] );
    buffer_handle[i] = StreamGrabber[ind]->RegisterBuffer( buffer[i], DIMX_ACQ*DIMY_ACQ );
    StreamGrabber[ind]->QueueBuffer( buffer_handle[i], NULL);
  }

  #endif  */

/*
  for ( int i=0 ; i<NBUFFER_BASLER ; i++ )
  {    
    buffer[i] = (uint8_t*) _aligned_malloc( DIMX_ACQ*DIMY_ACQ, 16 );
    ASSERT( buffer[i] );
//    buffer_handle[i] = StreamGrabber[ind]->RegisterBuffer( buffer[i], DIMX_ACQ*DIMY_ACQ );
//    StreamGrabber[ind]->QueueBuffer( buffer_handle[i], NULL);
  }

  //Camera[ind]->AcquisitionStart.Execute();


  timer.reset();

  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    //if ( StreamGrabber[ind]->GetWaitObject().Wait(3000) ) // Wait for the grabbed image with timeout of 3 seconds
    if ( 1 ) 
    {
     // if ( !StreamGrabber[ind]->RetrieveResult(Result) ) // Get the grab result from the grabber's result queue
      if ( 0 )
      {
        buffer_lost++;
        //Message_box("Unable to retrieve grab result");
        Message_box( L"Unable to retrieve grab result" );
        break;
      }

      if ( Result.Succeeded() ) // Result.Status() == Grabbed )
      {  
        img_acq = (BYTE*) Result.Buffer();

//        StreamGrabber[ind]->QueueBuffer(Result.Handle(), NULL);

        SCALE_IMG ( ind, img_acq );
        image_dsp[ind]->Draw_image();

        if ( (nimg_acq&127)==127 ) 
        {
          freq = 128.0e6/timer.elapsed_us();
//          sprintf( msg, "frequenza di acquisizione %4.2lf", 100.0e6/timer.elapsed_us() );
//          pView->tlctext_3[ind]->SetText( msg, 0 );
          timer.reset();
//          n = 0;
        }


        //sprintf( msg, "%d acquired frames - %d lost buffers - %.1lf fps", nimg_acq++, buffer_lost, freq );
        swprintf( msg, L"%d acquired frames - %d lost buffers - %.1lf fps", nimg_acq++, buffer_lost, freq );
        pView->tlctext_2[ind]->SetText( msg, 0 );


/*        if ( ind==1 && 0 ) // salvataggio immagini filmato
        {
          wchar_t newstr[200];
          char    file  [200];
          FCObjImage temp;
          temp.Create( DIMX_ACQ, DIMY_ACQ, 8 );
          BYTE*  temp_ptr = (BYTE*) temp.GetMemStart();
          ASSERT( AfxIsValidAddress( temp_ptr, DIMX_ACQ*DIMY_ACQ ) );
          ASSERT( AfxIsValidAddress( img_acq,  DIMX_ACQ*DIMY_ACQ ) );
          CopyMemory( temp_ptr, img_acq, DIMX_ACQ*DIMY_ACQ );
          sprintf( file, "p-point_%03d.png", nimg_acq );
          mbstowcs( newstr, file, strlen(file) );
          newstr[strlen(file)] = 0;
          temp.Save( newstr );
        }   */

/*
      }
      else // Timeout!
      {  
        //TRACE ("Timeout occurred!\n");// << endl;
        TRACE( L"Timeout occurred!\n" );// << endl;
//        StreamGrabber[ind]->CancelGrab(); // Get the pending buffer back (You are not allowed to deregister buffers when they are still queued)
//        for (GrabResult r; StreamGrabber[ind]->RetrieveResult(r);); // Get all buffers back
        buffer_lost++;

        //break;
      }
    } //for
    else
    {
      if ( timer_acq.elapsed_sec() != wait_sec && timer_acq.elapsed_sec() > 15 )
      {
        wait_sec = timer_acq.elapsed_sec();
        //sprintf( msg, "Waiting trigger (%d sec)", wait_sec );
        swprintf( msg, L"Waiting trigger (%d sec)", wait_sec );
        pView->tlctext_2[0]->SetText(msg,0);
      }
      Sleep(0);
    }

  }//while

//  Camera[ind]->AcquisitionStop.Execute();
//  StreamGrabber[ind]->CancelGrab();

  if ( image_dsp[ind]->ptr_buf ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size ) ;


//  TRACE("Movie loop ended: total number of lost frames = %d\n", buffer_lost );
  TRACE( L"Movie loop ended: total number of lost frames = %d\n", buffer_lost );


  // Consume all items from the output queue
//  while ( StreamGrabber[ind]->GetWaitObject().Wait(0) ) 
  {
    //StreamGrabber[ind]->RetrieveResult( Result );
    if ( Result.Status() == Canceled ) TRACE( L"Got canceled buffer\n" ); // TRACE("Got canceled buffer\n");
  }
//  TRACE("%d buffers canceled from output queue\n", i );

  for ( int i=0 ; i<NBUFFER_BASLER ; i++ )
  {
//    if ( buffer_handle[i] ) StreamGrabber[ind]->DeregisterBuffer( buffer_handle[i] );
    if ( buffer[i] ) _aligned_free( buffer[i] );
  }

  //StreamGrabber[ind]->FinishGrab();
//  StreamGrabber->Close();

/*  sprintf( msg, "Camera[%d] idle", ind );
  pView->tlctext_1[ind]->SetText( msg, 0 );
  pView->tlctext_2[ind]->SetText( "",  0 );
  pView->tlctext_3[ind]->SetText( "",  0 ); */


/*
  swprintf( msg, L"Camera[%d] idle", ind );
  pView->tlctext_1[ind]->SetText( msg, 0 );
  pView->tlctext_2[ind]->SetText( L"",  0 );
  pView->tlctext_3[ind]->SetText( L"",  0 );

//  tlc_status[ind] = TS_TLC_OK;

//  TRACE ("<Exiting movie_3d[%d]>\n", ind);
  TRACE ( L"<Exiting movie_3d[%d]>\n", ind );

  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit

  return 0;
}

*/





wchar_t* prepare_warning_string( message_type* msg )
{
  size_t num=0;
  static USHORT  index_old = 0;
  static wchar_t str[MAX_TEXT_LEN+1];

  if ( msg->index==index_old && msg->format==0 ) return str;

  if ( msg->index>=WARNING_TXT_NUM )
  {
    //swprintf( str, L"WARNING INDEX %d IS OUT OF ALLOWED RANGE", msg->index );
    //return str;
    mbstowcs_s( &num, str, MAX_TEXT_LEN, msg->msg, _TRUNCATE); // conversione char=>wchar_t
    return str;
  }

  switch ( msg->format )
  {
    case 0:  // only string
      swprintf( str, MAX_TEXT_LEN, warning_msg[msg->index*NLANGS+lng] );
      break;

    case 10:  // string + %d integer (param_0)
      swprintf( str, MAX_TEXT_LEN, warning_msg[msg->index*NLANGS+lng], msg->param_0 );
      break;

    case 11:  // string + %d integer (param_0) + %d integer (param_1)
      swprintf( str, MAX_TEXT_LEN, warning_msg[msg->index*NLANGS+lng], msg->param_0, msg->param_1 );
      break;

    case 20:  // string + %.3lf double (param_2)
      swprintf( str, MAX_TEXT_LEN, warning_msg[msg->index*NLANGS+lng], msg->param_2 );
      break;

    case 21:  // string + %.3lf double (param_2) + %.3lf double (param_3)
      swprintf( str, MAX_TEXT_LEN, warning_msg[msg->index*NLANGS+lng], msg->param_2, msg->param_3 );
      break; 

    case 30:  // string + %s string[20] (param_4)
      wchar_t txt[20];
      mbstowcs_s( &num, txt, 20, msg->param_4, _TRUNCATE); // conversione char=>wchar_t
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], txt );
      break;

    default:
      swprintf( str, MAX_TEXT_LEN, L"UNHANDLED WARNING FORMAT PARAMETER %d", msg->format );

  }

  index_old = msg->index;
  return str;
}


/*
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
  */


wchar_t* prepare_alarm_string( message_type* msg )
{
  size_t num=0;
  static USHORT  index_old = 0;
  static wchar_t str[MAX_TEXT_LEN+1];

  if ( msg->index==index_old && msg->format==0 )
  {
    if ( wcslen(str)>2 ) return str;

    swprintf( str, MAX_TEXT_LEN, L"index=%d msg=%s frmt=%d p0=%d p1=%d p2=%3.3lf p3=%3.3lf p4=%s", msg->index, msg->msg, msg->format, msg->param_0, msg->param_1, msg->param_2, msg->param_3, msg->param_4 );
    return str;
  }

  if ( msg->index>=ALARM_TXT_NUM ) // if out of range, show msg field
  {
    //swprintf( str, L"ALARM INDEX %d IS OUT OF ALLOWED RANGE", msg->index );
    if ( strlen(msg->msg)>2 )
    {
      mbstowcs_s( &num, str, MAX_TEXT_LEN, msg->msg, _TRUNCATE); // conversione char=>wchar_t
    }
    else
    {
      swprintf( str, MAX_TEXT_LEN, L"index=%d msg=%s frmt=%d p0=%d p1=%d p2=%3.3lf p3=%3.3lf p4=%s", msg->index, msg->msg, msg->format, msg->param_0, msg->param_1, msg->param_2, msg->param_3, msg->param_4 );
    }
    return str;
  }

  switch ( msg->format )
  {
    case 0:  // only string
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng] );
      break;

    case 10:  // string + %d integer (param_0)
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], msg->param_0 );
      break;

    case 11:  // string + %d integer (param_0) + %d integer (param_1)
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], msg->param_0, msg->param_1 );
      break;

    case 20:  // string + %.3lf double (param_2)
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], msg->param_2 );
      break;

    case 21:  // string + %.3lf double (param_2) + %.3lf double (param_3)
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], msg->param_2, msg->param_3 );
      break; 

    case 30:  // string + %s string[20] (param_4)
      wchar_t txt[20];
      mbstowcs_s( &num, txt, 20, msg->param_4, _TRUNCATE); // conversione char=>wchar_t
      swprintf( str, MAX_TEXT_LEN, alarm_msg[msg->index*NLANGS+lng], txt );
      break;

    default:
      swprintf( str, MAX_TEXT_LEN, L"UNHANDLED ALARM FORMAT PARAMETER %d", msg->format );

  }

  index_old = msg->index;
  return str;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++
// +              ALARM-WARNING THREAD              +
// ++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl alarm_warning_3d( LPVOID pParam )
{
  TRACE( "<alarm_warning_3d: thread started>\n" );

  SET_BIT( thread_status, 1 );

	SYSTEMTIME time;
	wchar_t str[200+1];

  //UINT counter = 1234567;

  ZeroMemory( &freezemode_ads, sizeof(ADS_data_type) );

  BYTE freezemodeon_old = freezemode_on;


  CTimer timer;

  if ( ADS_status )
  {

    #if defined TIVS_C40 || defined TIVS_C60  // if combo machine
      if ( IS_PC2 && !ADS_init_parameter( ".F_FREEZEMODE_ON", &freezemode_ads, 1 ) )
      {
        if ( ADS_read_parameter( &freezemode_ads ) ) TRACE( "ERROR WHILE READING <.F_FREEZEMODE_ON>\n" ); // TRACE("ERRORE DURANTE LA LETTURA DELLA VARIABILE <.ALARM>\n");
        else
        {
          if (freezemode_ads.val) freezemode_on = *((BYTE*)freezemode_ads.val);
          else                    freezemode_on = NULL;
          TRACE( "Variable <.F_FREEZEMODE_ON> initialized and connected - size=%d bytes\n", sizeof(BYTE));
          freezemodeon_old = freezemode_on;
          TRACE( "Initial freezemode_on=%d\n", freezemodeon_old );
        }
      }
      else TRACE( "ERROR WHILE INITIALIZING <.ALARM>\n" );
    #endif

    if ( !ADS_init_parameter( ".ALARM", &alarm_ads, sizeof(message_type) ) )
    {
      if ( ADS_read_parameter( &alarm_ads ) ) TRACE( "ERROR WHILE READING <.ALARM>\n" ); // TRACE("ERRORE DURANTE LA LETTURA DELLA VARIABILE <.ALARM>\n");
      else
      {
        if (alarm_ads.val) alarm_data = (message_type*) alarm_ads.val;
        else               alarm_data = NULL;
        TRACE( "Variable <.alarm> initialized and connected - size=%d bytes\n", sizeof(message_type));
      }
    }
    else TRACE( "ERROR WHILE INITIALIZING <.ALARM>\n" );

    if ( !ADS_init_parameter( ".WARNING", &warning_ads, sizeof(message_type) ) )
    {
      if ( ADS_read_parameter( &warning_ads ) ) TRACE( "ERROR WHILE READING <.WARNING>\n" ); // TRACE("ERRORE DURANTE LA LETTURA DELLA VARIABILE <.WARNING>\n");
      else
      {
        if (warning_ads.val) warning_data = (message_type*) warning_ads.val;
        else                 warning_data = NULL;
        TRACE( "<.warning> inizialized and connected - size=%d bytes\n", sizeof(message_type));
      }
    }
    else TRACE( "ERROR WHILE INITIALIZING <.WARNING>\n");

    while ( !alarm_data   || !alarm_ads.handle   ) Sleep(100);
    while ( !warning_data || !warning_ads.handle ) Sleep(100);

  }

  while ( !pView || !pView->container_main || !pView->container_main->date_time ) Sleep(100);

  //ADS_data_type ADS_run_cycles;
  //ADS_init_parameter( ".RUN_CYCLES",  &ADS_run_cycles, 4 );
  //WORD day_old = 0;
  //USHORT index_old = 0;

  Sleep(500);

  while ( !alarm_txt || !alarm_txt->m_hWnd ) Sleep(100);


  //AfxGetMainWnd()->RedrawWindow();

  wchar_t* txt = NULL;
  wchar_t txt_old[MAX_TEXT_LEN+1] = L"";

  int is = 0;


  ASSERT( alarm_txt && alarm_txt->m_hWnd );

  while ( !CHK_BIT(thread_exit, 1) )
  {
    timer.reset();

  	GetLocalTime( &time );
	if( lng == CHINESE)
	swprintf( str, 100, L"%04d/%02d/%02d %02d:%02d:%02d", time.wYear,time.wMonth,time.wDay, time.wHour, time.wMinute, time.wSecond );
	else
    swprintf( str, 100, L"%02d/%02d/%04d %02d:%02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond );
	  pView->container_main->date_time->SetWindowText( str ); 

	  if ( ADS_status )
    {
    
      if ( !ADS_read_parameter( &alarm_ads ) )
      {
        if ( alarm_data->on ) // Allarme attivo!
        {
          txt = prepare_alarm_string(alarm_data);
          alarm_txt->SetText( txt, 0, TXT_RED );

		 // //Pour added on 20201022

		 // if( layout_dsp && layout_dsp->visible ) 
		 // {
			////layout_dsp->Draw_image();
			//pView->RedrawWindow();
		 // }

		 // //Pour added on 20201022


        }
        else
        {
          txt = alarm_msg[lng];  // no pending alarm
          alarm_txt->SetText( txt, 0, TXT_GREEN );

		 // //Pour added on 20201022

		 // if( layout_dsp && layout_dsp->visible ) 
		 // {
		 // //layout_dsp->Draw_image();

		 // //pView->container_main->
			//pView->RedrawWindow();

		 // }

		 // //Pour added on 20201022

        }

        // check if must be logged
        if ( logger && wcscmp( txt, txt_old ) )
        {
          #ifdef ALARMLOG_SEPARATE // log alarm separately and add button to show them
            if ( alarm_data->on ) alarm_logger->Add_string( txt );
            else                  logger->Add_string( txt );
          #else  // normal, log along with other messages
            logger->Add_string( txt );
          #endif
          wcscpy( txt_old, txt );
          //TRACE( L"<%s> added to log\n", txt );
        }
      }

      if ( !ADS_read_parameter( &warning_ads ) )
      {
        if ( warning_data->on ) // Warning attivo!
        {
          warning_txt->SetText( prepare_warning_string(warning_data), 0, TXT_YELLOW );
        }
        else
        {
          warning_txt->SetText( warning_msg[lng], 0, TXT_GREEN );
        }
      } 

      if ( !ADS_read_parameter( &freezemode_ads ) )
      {
        freezemode_on = *((BYTE*)freezemode_ads.val);

        if ( freezemode_on != freezemodeon_old )
        {
          freezemodeon_old = freezemode_on;
          TRACE( "Updated freezemode_on=%d\n", freezemodeon_old );

          if ( IS_PC2 )
          {
            if ( freezemode_on )
            {
              if ( uplist[7].desc_st )
              {
                uplist[7].desc_st->SetWindowTextW( string_sideckrej[lng] ); // Sidecake rej
                uplist[7].desc_txt = string_sideckrej[lng]; // Sidecake rej
              }
              if ( uplist[8].desc_st )
              {

	#ifdef HEEL_INSPECTION
				  //uplist[8].desc_st->SetWindowTextW( string_heelrej  [lng] ); // Heel rej
				  uplist[8].desc_st->SetWindowTextW( string_topckrej  [lng] ); // Heel rej
	#else
				  //uplist[8].desc_st->SetWindowTextW( string_topckrej  [lng] ); // Heel rej
				  uplist[8].desc_st->SetWindowTextW( string_heelrej  [lng] ); // Heel rej
	#endif
                uplist[8].desc_txt = string_heelrej[lng]; // Heel rej

              }
            }
            else
            {
              if ( uplist[7].desc_st ) //uplist[7].desc_st->SetWindowTextW( string_part1rej[lng] ); // Particle1 rej
              {
                uplist[7].desc_st->SetWindowTextW( string_part1rej[lng] ); // Particle1 rej
                uplist[7].desc_txt = string_part1rej[lng]; // Particle1 rej
              }
              if ( uplist[8].desc_st ) //uplist[8].desc_st->SetWindowTextW( string_part2rej[lng] ); // Particle2 rej
              {
                uplist[8].desc_st->SetWindowTextW( string_part2rej[lng] ); // Particle2 rej
                uplist[8].desc_txt = string_part2rej[lng]; // Particle2 rej
              }
            }
          }
        } // if ( !ADS_read_parameter( &freezemode_ads ) )

//    #ifdef TIVS_C40 // MODEL TIVS-C40
//      if ( IS_PC2 && !ADS_init_parameter( ".F_FREEZEMODE_ON", &freezemode_ads, sizeof(BOOL) ) )
//      {
//        if ( ADS_read_parameter( &freezemode_ads ) ) TRACE( "ERROR WHILE READING <.F_FREEZEMODE_ON>\n" ); // TRACE("ERRORE DURANTE LA LETTURA DELLA VARIABILE <.ALARM>\n");
//        else
//        {
//          if (freezemode_ads.val) freezemode_on = *((BOOL*) freezemode_ads.val);
//          else                    freezemode_on = NULL;
//          TRACE( "Variable <.F_FREEZEMODE_ON> initialized and connected - size=%d bytes\n", sizeof(BOOL));
//        }
//      }
//      else TRACE( "ERROR WHILE INITIALIZING <.ALARM>\n" );
//    #endif

      } 


      
      // Update production statistics, if layout is shown, 
      //if ( layout_dsp && layout_dsp->visible ) layout_dsp->Draw_info( 0 );
      //else                                     ADS_update_list();           // else Updatelist
      

      ADS_update_list();
	  
    }

    /*
    swprintf( str, 200, L"%d - %d - %d - %d", counter, counter*2, counter*3, counter*4 );
    machine_txt->SetText( str, 0, TXT_GREEN  );
    alarm_txt->SetText  ( str, 0, TXT_YELLOW );
    warning_txt->SetText( str, 0, TXT_RED    );
    counter = counter * 101 / 100;
    */

//    while ( timer.elapsed_ms()<1000 ) // refresh rate
    while ( timer.elapsed_ms()<refresh_delay ) // refresh rate
    {
      MSG msg; // PeekMessage( &qmsg, NULL, 0, 0, PM_REMOVE );
      while( PeekMessage( &msg, NULL,  0, 0, PM_REMOVE) );
      Sleep(5);
    }

    //TRACE("refresh_delay=%d\n",refresh_delay);

    if ( password_mngr ) password_mngr->check_timeout();



    /*
    // check cameras link speed
    is++;
    if (is==num_tlc) is=0;

    if ( camera[is].device )
    {
      CIntegerPtr lanspd = camera[is].device->GetNodeMap()->GetNode("GevLinkSpeed");
      TRACE("LANSPD[%d] = %d\n",is,lanspd->GetValue());
    }
    */

    //TRACE(".");  // Attesa conclusa\n");
  }

  CLR_BIT( thread_status, 1 );
  CLR_BIT( thread_exit,   1 );


//  ADS_status = 9999; // to signal 3d exited
  //TRACE ("<Exiting alarm_warning_3d[%d]>\n");
  TRACE( L"<Exiting alarm_warning_3d[%d]>\n" );
  return 0;
}


