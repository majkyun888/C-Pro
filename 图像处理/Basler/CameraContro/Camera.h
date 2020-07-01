// Include files to use the PYLON API
#pragma once

//#pragma pack( 16 )


#include "Globals.h"
//#include "Tipo_timer.h"

#include <pylon/PylonIncludes.h>    // Include files to use the PYLON API.
#include <pylon/gige/BaslerGigECamera.h>

using namespace Pylon;              // Namespace for using pylon objects.
using namespace GenApi;             // Namespace for using GenApi objects.
using namespace std;                // Namespace for using cout.


#define MAXNUMTEXTROWS 5

#define ACE_FREQUENCY  125000000
#define ACE_CFMS       ((1000.0)/(125000000.0))

/*
#ifdef COLOR_DETECTION_P1
int Process_image( CGrabResultPtr ptr_gr, UINT nimg, UINT ind );
#endif
*/

#define BIT_PARTICLE1    (1<<10)  //     1024 - Particle1/SideCake
#define BIT_PARTICLE2    (1<<11)  //     2048 - Particle2/TopCake
#define BIT_FILLINGL1    (1<<12)  //     4096 - Filling 1
#define BIT_FILLINGL2    (1<<13)  //     8192 - Filling 2
#define BIT_COSMETIC     (1<<14)  //    16384 - Cosmetic / SideWall
#define BIT_CAPSEAL1     (1<<15)  //    32768
#define BIT_CAPSEAL2     (1<<16)  //    65536
#define BIT_CAPSEAL3     (1<<17)  //   131072
#define BIT_FOCOLOR      (1<<18)  //   262144
#define BIT_BOTTOMA      (1<<19)  //   524288
#define BIT_BOTTOMB      (1<<20)  //  1048576
#define BIT_DRCOLOR      (1<<21)  //  2097152
#define BIT_WAIST        (1<<22)  //  4194304 - new for AUSIA waist shrink
#define BIT_LAYER        (1<<23)  //  8388608 - layering
#define BIT_SKINNING     (1<<24)  // 16777216 - top-cake skinning

// add for neck inspect
#define BIT_NECK1     (1<<24)  // neck1
#define BIT_NECK2     (1<<25)  // neck1
#define BIT_NECK3     (1<<26)  // neck1
// add end
// add for dot/ring inspect
#define BIT_DRCOLOR1     (1<<24)  // neck1//23
#define BIT_DRCOLOR2     (1<<25)  // neck1//24
#define BIT_DRCOLOR3     (1<<26)  // neck1//25
// add end


/*
  .10 => 0/  1024  Particle1/SideCake  1=Ok 0=Rej
  .11 => 0/  2048  Particle2/TopCake   1=Ok 0=Rej
  .12 => 0/  4096  Filling level 1     1=Ok 0=Rej
  .13 => 0/  8192  Filling level 2     1=Ok 0=Rej
  .14 => 0/ 16384  Cosmetic/SideWall   1=Ok 0=Rej
  .15 => 0/ 32768  Cap/seal TC1        1=Ok 0=Rej
  .16 => 0/ 65536  Cap/seal TC2        1=Ok 0=Rej
  .17 => 0/131072  Cap/seal TC3        1=Ok 0=Rej
  .18 => 0/262144  F.O. color          1=Ok 0=Rej
  .19 => 0/        Bottom1             1=Ok 0=Rej
  .20 => 0/        Bottom2             1=Ok 0=Rej
*/

/* Valid values for AcquisitionMode. 
Enumerator: 
AcquisitionMode_SingleFrame  Sets the acquisition mode to single frame. 
AcquisitionMode_MultiFrame   Sets the acquisition mode to multi frame. 
AcquisitionMode_Continuous   Sets the acquisition mode to continuous. 
*/


/*  Valid values for AcquisitionStatusSelector. 
Enumerator: 
AcquisitionStatusSelector_AcquisitionTriggerWait  Device is currently waiting for a trigger for the capture of one or many frames. 
AcquisitionStatusSelector_AcquisitionActive  Device is currently doing an acquisition of one or many frames. 
AcquisitionStatusSelector_AcquisitionTransfer  Device is currently transferring an acquisition of one or many frames. 
AcquisitionStatusSelector_FrameTriggerWait  Device is currently waiting for a Frame trigger. 
AcquisitionStatusSelector_FrameActive  Device is currently doing the capture of a frame. 
AcquisitionStatusSelector_FrameTransfer  Device is currently transferring a frame. 
AcquisitionStatusSelector_ExposureActive  Device is doing the exposure of a frame. 
AcquisitionStatusSelector_LineTriggerWait  Device is currently waiting for a line trigger. 
*/

/* Valid values for EventSelector. 
Enumerator: 
EventSelector_ExposureEnd  Selects the end of exposure event for enabling. 
EventSelector_LineStartOvertrigger  Selects the line start overtrigger event for enabling. 
EventSelector_FrameStartOvertrigger  Selects the frame start overtrigger event for enabling. 
EventSelector_AcquisitionStartOvertrigger  Selects the acquisition start overtrigger event for enabling. 
EventSelector_FrameTimeout  Selects the frame timeout event for enabling. 
EventSelector_FrameStart  Selects the frame start trigger event for enabling. 
EventSelector_AcquisitionStart  Selects the acquisition start trigger event for enabling. 
EventSelector_CriticalTemperature  Selects the critical temperature event for enabling. 
EventSelector_OverTemperature  Selects the over temperature event for enabling. 
EventSelector_EventOverrun  Selects the event overrun event for enabling. 
 */

/* Valid values for TriggerActivation. 
Enumerator: 
TriggerActivation_RisingEdge  Sets the selected trigger to become active on the rising edge of the source signal. 
TriggerActivation_FallingEdge  Sets the selected trigger to become active on the falling edge of the source signal. 
TriggerActivation_AnyEdge  Sets the selected trigger to become active on the falling or rising edge of the source signal. 
TriggerActivation_LevelHigh  Sets the selected trigger to become active when the source signal is high. 
TriggerActivation_LevelLow  Sets the selected trigger to become active when the source signal is low. 
*/


/*Valid values for TriggerMode. 
Enumerator: 
TriggerMode_Off  Sets the mode for the selected trigger to off. 
TriggerMode_On  Sets the mode for the selected trigger to on. 
*/

/* Valid values for TriggerSelector. 
Enumerator: 
TriggerSelector_AcquisitionStart  Selects the acquisition start trigger for configuration. 
TriggerSelector_AcquisitionEnd  Selects the acquisition end trigger for configuration. 
TriggerSelector_AcquisitionActive  Selects the acquisition active trigger for configuration. 
TriggerSelector_FrameStart  Selects the frame start trigger for configuration. 
TriggerSelector_FrameEnd  Selects the frame end trigger for configuration. 
TriggerSelector_FrameActive  Selects the frame active trigger for configuration. 
TriggerSelector_LineStart  Selects the line start trigger for configuration. 
TriggerSelector_ExposureStart  Selects the exposure start trigger for configuration. 
TriggerSelector_ExposureEnd  Selects the exposure end trigger for configuration. 
TriggerSelector_ExposureActive  Selects the exposure active trigger for configuration. 
*/

/* Valid values for TriggerSource. 
Enumerator: 
TriggerSource_Software  Sets the software trigger as the signal source for the selected trigger. 
TriggerSource_Line1  Sets the signal source for the selected trigger to line 1. 
TriggerSource_Line2  Sets the signal source for the selected trigger to line 2. 
TriggerSource_Line3  Sets the signal source for the selected trigger to line 3. 
TriggerSource_Line4  Sets the signal source for the selected trigger to line 4. 
TriggerSource_Line5  Sets the signal source for the selected trigger to line 5. 
TriggerSource_Line6  Sets the signal source for the selected trigger to line 6. 
TriggerSource_Line7  Sets the signal source for the selected trigger to line 7. 
TriggerSource_Line8  Sets the signal source for the selected trigger to line 8. 
TriggerSource_CC1  Sets the signal source for the selected trigger to CC1. 
TriggerSource_CC2  Sets the signal source for the selected trigger to CC2. 
TriggerSource_CC3  Sets the signal source for the selected trigger to CC3. 
TriggerSource_CC4  Sets the signal source for the selected trigger to CC4. 
TriggerSource_ShaftEncoderModuleOut  Sets the signal source for the selected trigger to the shaft encoder module. 
TriggerSource_FrequencyConverter  Sets the signal source for the selected trigger to the frequency converter module. 
TriggerSource_Timer1Start  Sets the signal source for the selected trigger to timer 1 start. 
TriggerSource_Timer1End  Sets the signal source for the selected trigger to timer 1 end. 
TriggerSource_Counter1Start  Sets the signal source for the selected trigger to counter 1 start. 
TriggerSource_Counter1End  Sets the signal source for the selected trigger to counter 1 end. 
TriggerSource_UserOutput1  Sets the signal source for the selected trigger to user output 1. 
TriggerSource_UserOutput2  Sets the signal source for the selected trigger to user output 2. 
TriggerSource_Action1  Sets the signal source for the selected trigger to action command signal 1. 
TriggerSource_Action2  Sets the signal source for the selected trigger to action command signal 2. 
TriggerSource_Action3  Sets the signal source for the selected trigger to action command signal 3. 
TriggerSource_Action4  Sets the signal source for the selected trigger to action command signal 4. 
*/

struct Camera_struct
{

//  CBaslerGigEInstantCamera* camera;

  CInstantCamera*  camera;       // pointer of camera object
  IPylonDevice*    device;       // pointer of camera object
  wchar_t          name[5+1];    // description name of camera (4 chars + terminator)
  char             sn  [8+1];    // serial number of camera    (8 chars + terminator)
  int64_t          offsetx;      // acquisition horizontal offset
  int64_t          offsety;      // acquisition vertical offset
  int64_t          width;        // acquisition horizontal size
  int64_t          height;       // acquisition vertical size
  int64_t          width_max;    // max horizontal resolution
  int64_t          height_max;   // max vertical resolution
  BOOL             sequence;     // true=sequence mode (particle)
  BOOL             trigger;      // true=waiting hardware trigger
  BOOL             strobe;       // true=enable output strobe on exposure
  BOOL             output1;      // true=enable output1 control by software
  CBooleanPtr      userout_ptr;  // pointer to control useroutput if enabled
  float            frame_rate;   // 0=freerun - other tries to set acq rate
  int              acq_mode;     // acquisition mode (enum) AcquisitionMode_Continuous - TriggerMode_On
  int              shutter;      // shutter time (us)
  int              shutter2;     // second shutter time (us)
  int              triglab;      // trigger_label index (0..NUMTRIGGER-1)
  int              triglab2;     // trigger_label index 2, in case of 2 triggers to one camrea for a vial//Pour 20191118
  BYTE*            ptr;          // pointer to data of last acquired image
  POINT            dsporg;       // display origin coordinates
  SIZE             dspdim;       // display windows dimensions
  double           scale;        // scaling factor
  int              modedsp;      // rotation and other display options
  int              textwidth;    // width of lines of text (0=same width as dsp window)
  int              textnum;      // number of text rows below dsp image
  char*            light_out;    // if != NULL is the PLC parameter to switch light ON when TRIGGER WAIT is disabled
  CText_box*       text[MAXNUMTEXTROWS]; //
};



struct insp_res_type
{
	DWORD     num;      // product label taken as trigger
	DWORD     val;      // inspection result
};



  #define NBUFFER_BASLER          4
  #define BASLER_MINSHUTTER      20 // us
  #define BASLER_MAXSHUTTER   20000 // us

//  int  Basler_init( void );
  int  Basler_initialize( void );
  void Basler_terminate ( void );

  int  Basler_initialize( UINT ind );
  int  Basler_parameters( UINT ind );
  int  Basler_display   ( UINT ind );


  void Basler_insp( void );

  int  set_camera_output( UINT ind, bool val );

  //int  Get_background( int ind );



  int Set_camera( int ind );
  int Set_area  ( int ind );
  //void Set_averaging( int ind, int val );
  int cvt2BW ( BYTE* _im_gray, BYTE* _im_color, UINT dimx, UINT dimy);