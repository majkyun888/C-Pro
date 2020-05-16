#pragma once
# ifndef _HIKVISION_H_
#define _HIKVISION_H_
#endif

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MvCameraControl.h"
#include "TlFactory.h"
#include "MvErrorDefine.h"
#include "CameraParams.h"
#include "MvGigEDevice.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <set>

using namespace std;
using namespace MvCamCtrl;
using namespace cv;

#define MAX_XML_FILE_SIZE												(1024*1024*1)
#define COMMON_ERROE_CODE												-1
#define	CAMERA_NOT_OPEN													-99
#define DEFAULT_ROI_WIDTH												2048
#define DEFAULT_ROI_HEIGHT												480
#define DEFAULT_ACQUISITION_LINE_RATE									2960
#define DEFAULT_FLOAT_ZERO												0.0
#define DEFAULT_EXPOSURE_TIME											300.0
const int DEFAULT_IMAGE_BUFFER_SIZE					=					(2048 * 992 * 3);
const int IMAGE_HEIGHT								=					992;
const int IMAGE_WIDTH								=					2048;
												

typedef enum _MV_CAM_HEART_BEAT_MODE_							//相机心跳模式设置
{
	MV_CAM_HEART_BEAT_MODE_ON = 0,								//ON
	MV_CAM_HEART_BEAT_MODE_OFF = 1,								// OFF
}MV_CAM_HEART_BEAT_MODE_;



struct Camera_struct
{
	//CInstantCamera* camera;       // pointer of camera object
	//IPylonDevice* device;       // pointer of camera object
	//wchar_t          name[5 + 1];    // description name of camera (4 chars + terminator)
	//char             sn[8 + 1];    // serial number of camera    (8 chars + terminator)
	char             sn[16];    // serial number of camera    (8 chars + terminator)
	int64_t          offsetx;      // acquisition horizontal offset
	int64_t          offsety;      // acquisition vertical offset
	int64_t          width;        // acquisition horizontal size
	int64_t          height;       // acquisition vertical size
	int64_t          width_max;    // max horizontal resolution
	int64_t          height_max;   // max vertical resolution
	//BOOL             sequence;     // true=sequence mode (particle)
	BOOL             trigger;      // true=waiting hardware trigger
	//BOOL             strobe;       // true=enable output strobe on exposure
	//BOOL             output1;      // true=enable output1 control by software
	////CBooleanPtr      userout_ptr;  // pointer to control useroutput if enabled
	float            frame_rate;   // 0=freerun - other tries to set acq rate
	int              acq_mode;     // acquisition mode (enum) AcquisitionMode_Continuous - TriggerMode_On
	int              shutter;      // shutter time (us)
	//int              shutter2;     // second shutter time (us)
	//int              triglab;      // trigger_label index (0..NUMTRIGGER-1)
	//BYTE* ptr;          // pointer to data of last acquired image
	//POINT            dsporg;       // display origin coordinates
	//SIZE             dspdim;       // display windows dimensions
	//double           scale;        // scaling factor
	//int              modedsp;      // rotation and other display options
	//int              textwidth;    // width of lines of text (0=same width as dsp window)
	//int              textnum;      // number of text rows below dsp image
	//char* light_out;    // if != NULL is the PLC parameter to switch light ON when TRIGGER WAIT is disabled
	//CText_box* text[MAXNUMTEXTROWS]; //
	
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
int  Basler_initialize(void);
void Basler_terminate(void);

int  Basler_initialize(UINT ind);
int  Basler_parameters(UINT ind);
int  Basler_display(UINT ind);

void Basler_insp(void);

int  set_camera_output(UINT ind, bool val);

//int  Get_background( int ind );



int Set_camera(int ind);
int Set_area(int ind);
//void Set_averaging( int ind, int val );
//};

