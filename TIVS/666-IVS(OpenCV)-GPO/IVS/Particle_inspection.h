

#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

//Pour added 20180601
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>

//// 20190506 foropencv
class MiddlewareParticleDi;

using namespace Pylon;
//Pour added 20180601

//  CArea_adjust* areainsp; // inspection/search area

#define NCRIT_BLOB                   4
#define PARTICLE_MODE_REFLECTION   128
#define PARTICLE_MODE_ADSORPTION   256
#define PARTICLE_MODE_BOTH         512  // selectable 
#define PARTICLE_HORIZONTAL       1024  // camera is horizontal


#define  xmin    area->buffrect.left
#define  xmax    area->buffrect.right
#define  ymin    area->buffrect.top
#define  ymax    area->buffrect.bottom
#define  PT(x,y) img_acq[x+(y)*camera[index].width]


class CParticle_inspection
{

//	DECLARE_DYNAMIC(CParticle_inspection)

  struct learning_filter_type
  {
    BYTE* img_in;
    int   dimx;       // horizontal dimension of input image
    int   dimy;       // vertical   dimension of input image
    int   radius;     // radius of filter range                    - default= 3
    int   threshold;  // threshold to separate high-low brightness - default=20
    int   perc_low;   // filter percentage to use below threshold  - default=30
    int   perc_high;  // filter percentage to use above threshold  - default=80
  };

//20190506 foropencv
private:
MiddlewareParticleDi* p_middleware_particle_di ;

public:
  CParticle_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in=NULL );
  ~CParticle_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );
  int Add_menu_parameters_needle( CContainer_wnd* child );//for needle shield inspection
  static int Set_inspection_area( int ind );

//  int Dilate2H1V_add_sensit_learn_pos( BYTE* base_img, BYTE* img_acq, BYTE* learnt, BYTE* sensit, UINT dimx, UINT dimy );
//  int Dilate2H1V_add_sensit_learn_neg( BYTE* base_img, BYTE* img_acq, BYTE* learnt, BYTE* sensit, UINT dimx, UINT dimy );

  int Calculate_blob( void );
//  int Calculate_blob( BYTE* img1, BYTE* img2 );

  int Process_image( BYTE* img_acq, UINT nimg );
  //Pour added 20180601
  //int Process_image( CGrabResultPtr ptr_gr, UINT nimg );
  //Pour added 20180601

//  int Process_reflections( int inp );
  int Filter_reflections( int inp );
  int Image_filter( learning_filter_type* filter );


//  int Dilate_add_sensit_learn_pos( BYTE* img_acq );
//  int Dilate_add_sensit_learn_neg( BYTE* img_acq );

 // int Dilate2H1V_add_sensit_learn_pos( BYTE* img_acq );
  int Sub_images  ( BYTE* img1, BYTE* img2 );
//  int Dilate_max  ( BYTE* img_inp );
//  int Add_sensit_learn_pos( BYTE* img_acq );


 // int Dilate2H1V_add_sensit_learn_neg( BYTE* img_acq );
  int Dilate2H1V_add_sensit_learn_pos( BYTE* img_acq );
  int Dilate2H1V_add_sensit_learn_neg( BYTE* img_acq );

  int Dilate3H1V_add_sensit_learn_pos( BYTE* img_acq );
  int Dilate3H1V_add_sensit_learn_neg( BYTE* img_acq );

  int Dilate4H1V_add_sensit_learn_pos( BYTE* img_acq );
  int Dilate4H1V_add_sensit_learn_neg( BYTE* img_acq );

  int Dilate6H3V_add_sensit_learn_pos( BYTE* img_acq );
  //int Dilate6H3V_add_sensit_learn_neg( BYTE* img_acq );

  int Dilate8H4V_add_sensit_learn_pos( BYTE* img_acq );

  // versions for horizontal cameras
  int Dilate8H5Vhor_add_sensit_learn_pos( BYTE* img_acq );
  int Dilate8H5Vhor_add_sensit_learn_neg( BYTE* img_acq );

  int Get_diff_neg( BYTE* img_acq );
  int Get_diff_pos( BYTE* img_acq );

  int Get_min     ( BYTE* img_acq );
  int Get_max     ( BYTE* img_acq );

  int Add_learning( void );




  #ifdef NEW_PARTICLE

    #define MAXNBLOB 150

    struct blob_type
    {
      UINT  dim;        // size in pixels
      UINT  cx;         // center coordinate x
      UINT  cy;         // center coordinate y
      UINT  seq;        // sequence number (0..num_img)
      //UINT  score;      // counting correlations
      UINT  xmn;
      UINT  xmx;
      UINT  ymn;
      UINT  ymx;
      //RECT  bbox;
    };

    blob_type blob[MAXNBLOB];
    UINT nblob;

    UINT trace_lev;    // 0 nothing - 1,2,3 different traces

    int Prepare_images( int val, wchar_t* name=NULL );
    int Process_sequence( int val );

    int Terminate( int val );

    int Find_next ( UINT i );
//    int Find_next2( UINT ind );
//    int Find_next2( UINT s, UINT i );
    int Find_next2( void );

    int Draw_blobbox( UINT ind, UINT col ); 
    int Single_blob( UINT ind, BYTE* img );

    int Draw_blobbox2( UINT ind, UINT col ); 

    blob_type tracce[20][100];  // blob values
    UINT      ntracce[20];


    struct sequence_node
    {
      UINT  s1;         // sequence start index
      UINT  i1;         // blob start index
      UINT  s2;         // sequence end index
      UINT  i2;         // blob end index
      int   dx;         // horizontal distance
      int   dy;         // vertical distance
      UINT  sc;         // score for this connection
    };

    sequence_node sequence[20];
    UINT sql; // sequence lenght

    int Subtract_singledil( BYTE* img1, BYTE* img2, BYTE* out, BYTE thr );

    int Subtract       ( BYTE* img1, BYTE* img2, BYTE* out, BYTE thr );
    int Subtract_single( BYTE* img1, BYTE* img2, BYTE* out, BYTE thr );
    int Subtract_SSE   ( BYTE* img1, BYTE* img2, BYTE* out, BYTE thr );

    int Subtract_single_ofs( BYTE* img1, BYTE* img2, BYTE* out, BYTE thr );

    int Subtract_dilate( BYTE* img1, BYTE* img2, BYTE* out, BYTE idx );
    int Subtract_dilate( BYTE* img1, BYTE* img2 );


    int Create_GF( double sigma_in=-1.0 );
    int Gaussian_blur( BYTE* img );
    int Gaussian_blur( BYTE* img, int ind );

    int Finalize( void );
    int Post_process( void );

    int Post_process2( void );

	//Pour added for color to gray for images 20180530
	//BYTE*   img_gray;
	//BYTE*   im_gray_data;
	int Color_gray( BYTE* img, int ind );
	float  max_dist_lab;
	float  dist_lab;
	float  Lab_refl,Lab_refa,Lab_refb;
	//int Process_image( CGrabResultPtr ptr_gr, UINT nimg );
	//Pour added for color to gray for imgaes 20180530

    #define   NIMAGES 30
    BYTE*     images[NIMAGES];

    //BYTE      avglum[NIMAGES];
    //BYTE*     past_img;

    BYTE      threshold;  // difference threshold
    UINT      max_dist;   // max distance               2102
    UINT      max_dist2;  // max distance (squared)
    UINT      min_dim;    // min dimension              2104
    UINT      max_dim;    // max dimension              2106
    UINT      min_depth;  // minimum depth of sequence  2108

    UINT      modenew;    // 0/1 bottom/back   0/1024   singledif/accum

    int Find_offset( int i1, int i2 );
    int       ofsx;
    int       ofsy; 

    #define MAXGFPT 100
    int       gf_ofs[MAXGFPT];
    int       gf_val[MAXGFPT];
    int       gf_num;


    int totnpt; // total num of points found
    int nimgpt; // total num of images with enough points


    int mintotnpt; // min total num of points
    int minnptimg; // min num of pt per image
    int minnumimg; // min num of images with enough points


    CString   fname;
    wchar_t*  path_name;

    int Prepare_thr( void );

    int       threshold1; // = 10;
    int       threshold2; // = 90;
    int       thr_old1; 
    int       thr_old2; 

    double    sigma;

    BYTE      thr[256];

    CString   txt1;
    UINT      col1;

	//Pour added for color detection 20180604
	CString   txt4;
	UINT      col4;
	//Pour added for color detection 20180604
    UINT      proctime;


  #endif // NEW_PARTICLE

#ifdef OPENCV_PARTICLE
	int insp_savemode;
	int nozero_single_thr;//number of particles in single image
	int nozero_frm_sum_thr;//number of iamges
	float ratio; //inspection ratio;
	float sensitivity_part; //inspection sensitivity
	int cstr_camId;
#endif // OPENCV_PARTICLE


  CFile_mngmt* file_manager;

  CArea_adjust* area; // inspection/search area

//  CArea_adjust* areainsp; // inspection/search area

//  CArea_adjust* areainsp; // inspection/search area
  int index;          // index of camera/image to be bound to

  int mode;  // PARTICLE_MODE_REFLECTION / PARTICLE_MODE_ADSORPTION

 // BOOL learning;

  // inspection parameters
  UINT num_img;    // number of images for inspection
  UINT min_dif;    // minimum brightness difference during analysis
  UINT max_err;    // maximum allowed particle error 
  UINT part_err;
  UINT num_blob;

  BYTE LUTBLOB[256];

  int  dim, dimx, dimy;
  UINT area_color;

  //Pour added for color detection 20180530
  int dim_c;
  double L,a,b;
  double funz_t( double in );
  //Pour added for color detection 20180530

  BYTE*  reflections;
  BYTE*  baseimg;
  BYTE*  output;
  BYTE*  minmax;
  //Pour 20180605
  BYTE*  img_gray;


  BYTE*  sensitdata;    // [NUM_TLC] = { NULL, NULL, NULL }; // Pointers to 16bytes arrays containing sensitivity value copies



  struct blob_score_type
  {
    int  size;     // min size of blob to get error score (must be bigger/equal than)
    int  score;    // score added if blob bigger than size
  };

  blob_score_type blob_score[NCRIT_BLOB]; // RESO MEMBRO DI OGNI OGGETTO (NON STATICO) 20141022


/*  int necksearch_x;
  int necksearch_th;
  double diam;
  double mm2pix;  // mm to pixel conversion coefficient
  double pix2mm;  // pixel to mm conversion coefficient  
  int lum_min;    */
//#include "Area_adjust.h"




};




/*



int CParticle_inspection::Dilate4H1V_add_sensit_learn_neg( BYTE* img_acq ) // ca 1300us su N56VZ debug 768x580 process test
{
	ASSERT( ((UINT)baseimg     &15) == 0 );
	ASSERT( ((UINT)img_acq     &15) == 0 );
	ASSERT( ((UINT)reflections &15) == 0 );
	ASSERT( ((UINT)sensitdata  &15) == 0 );

  UINT dim = dimx * dimy;
	ASSERT( (dimx&15)==0 );

	ASSERT( AfxIsValidAddress( baseimg,     dim ) );
	ASSERT( AfxIsValidAddress( img_acq,     dim ) );
	ASSERT( AfxIsValidAddress( reflections, dim ) );
	ASSERT( AfxIsValidAddress( sensitdata,   16 ) );

  BYTE* bas  = baseimg    +4*dimx;
  BYTE* img  = img_acq    +4*dimx;
  BYTE* lnr  = reflections+4*dimx;
  BYTE* sns  = sensitdata;
  UINT  dimr = dimx * (dimy-8);

	__asm
	{
		mov      esi, sns        ; Put address <sensit> into <esi>
    movq     mm5, [esi]      ; load <sensit>   data    into xmm5
		mov		   esi, lnr        ; Load <learnt>   address+dimx into esi
		mov		   edi, bas        ; Load <base_img> address+dimx into edi  
		mov		   eax, img        ; Load <img_acq>  address+dimx into eax
		mov		   ebx, dimr       ; Load dimension into ebx
		xor		   ecx, ecx        ; Clear ecx
		LoopA:                   ; Loop start

    ; seems perfect also in the corners
    movq     mm1, [eax+ecx  -4*PARTICDIMX]; // !
    pminub   mm1, [eax+ecx-1-3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-3*PARTICDIMX];
    pminub   mm1, [eax+ecx-1-2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-2*PARTICDIMX];
    pminub   mm1, [eax+ecx-1-  PARTICDIMX]; 
		pminub   mm1, [eax+ecx  -  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1-  PARTICDIMX];
 		pminub   mm1, [eax+ecx-1             ]; 
    pminub   mm1, [eax+ecx               ]; 
 		pminub   mm1, [eax+ecx+1             ]; 
    pminub   mm1, [eax+ecx-1+  PARTICDIMX]; // positive ofs => dilation toward smaller Y (right)
    pminub   mm1, [eax+ecx  +  PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+  PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+2*PARTICDIMX];
    pminub   mm1, [eax+ecx  +2*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+2*PARTICDIMX]; 
    pminub   mm1, [eax+ecx-1+3*PARTICDIMX];
    pminub   mm1, [eax+ecx  +3*PARTICDIMX]; 
		pminub   mm1, [eax+ecx+1+3*PARTICDIMX]; 
    pminub   mm1, [eax+ecx  +4*PARTICDIMX]; 

		movq     mm0, [esi+ecx]  ; Load next 16 bytes of <learnt> into xmm0
		psubusb	 mm1,      mm0   ; xmm0+max(img_acq) => add dilated img_acq to xmm0
		psubusb	 mm1,      mm5   ; xmm0+xmm5 => xmm0 - add sensitivity
		movq   	[edi+ecx], mm1   ; xmm0 => dest
		add  	   ecx, 8          ; update ecx
		cmp		   ecx, ebx        ; completed?
		jne    	 LoopA           ; if not, run again
		emms                     ; end and terminate MMX mode 
	}

  ZeroMemory( baseimg,               4*dimx );
  ZeroMemory( baseimg+dimx*(dimy-4), 4*dimx );

  return 0; // completed successfully
}























  //TRACE("movie_3d[%d]: STARTGRABBING start mutex waiting...\n", ind);
  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STARTGRABBING mutex allowed to proceed\n", ind);


    ReleaseMutex( mutex );
  //TRACE("movie_3d[%d]: STARTGRABBING completed - mutex released\n", ind);




  //TRACE("movie_3d[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("movie_3d[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

    ReleaseMutex( mutex );
  //TRACE("movie_3d[%d]: STOP GRABBING completed - mutex released\n", ind);




        camera[i].offsetx    =           6;
      camera[i].offsety    =           0;
      camera[i].width      =  PARTICDIMX;
      camera[i].height     =         580;
      camera[i].shutter    =         800;
      camera[i].triglab    =           i; // 3-4-5-6
      camera[i].dsporg.x   =  30 + 100*i;
      camera[i].dsporg.y   = camera[0].dsporg.y;
      camera[i].scale      =         0.5;
      camera[i].acq_mode   = AcquisitionMode_Continuous;
      camera[i].trigger    =       FALSE; // no trigger here
      camera[i].strobe     =       FALSE;
      camera[i].frame_rate =         0.0; 
      camera[i].modedsp    = DISPROT_090; // DISPROT_000 DISPROT_090 DISPROT_180
      camera[i].textnum    =           3;
      camera[i].textwidth  =         300; // can be used to force width to some specified amount




  */


  /* TEST PROCESS PER BACKLIGHT

    UINT index = 3;
  UINT nimg_acq = 0;
  int x, y;

  ASSERT( particle_insp[index] );

  BYTE* img_acq = (BYTE*) _aligned_malloc( image_dsp[index]->buf_size , 16 ); 
  ASSERT( img_acq && AfxIsValidAddress( img_acq, image_dsp[index]->buf_size ) );

 // FillMemory( particle_insp[index]->reflections, image_dsp[index]->buf_size, 13 );

  //particle_insp[index]->min_dif = 24;

  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  for ( y=100 ; y<=120 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 100;


  timer.reset();
  nimg_acq = 0;
  particle_insp[index]->Process_image( img_acq, nimg_acq );
  TRACE("prima immagine: tempo = %d us\n", timer.elapsed_us() );

//  Save_image( particle_insp[index]->baseimg, image_dsp[index]->dim_buf.cx, image_dsp[index]->dim_buf.cy, "baseimg", PNG_FILE );


  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  for ( y=150 ; y<=170 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 100;

  nimg_acq = 1;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  for ( y=180 ; y<=200 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 111;

  nimg_acq = 2;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  FillMemory( img_acq, image_dsp[index]->buf_size, 200 );
  for ( y=210 ; y<=230 ; y++ ) for ( x=100 ; x<=120 ; x++ ) PT(x,y) = 118;

  nimg_acq = 3;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  Save_image( particle_insp[index]->output, image_dsp[index]->dim_buf.cx, image_dsp[index]->dim_buf.cy, "first_sub", PNG_FILE );

  // last
  nimg_acq = particle_insp[index]->num_img-1;
  if ( particle_insp[index] ) particle_insp[index]->Process_image( img_acq, nimg_acq );


  if ( img_acq ) _aligned_free( img_acq );
  return 0;


  */


/*
VA BENE MA E' UN PO' RALLENTATO





// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +            CAMERA INSPECTION PARTICLE THREAD            +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT __cdecl inspect_3d_particle( LPVOID pParam )
{
  UINT ind = (UINT) pParam;
  ASSERT( ind>=0 && ind<num_tlc );

  CString txt;
  //wchar_t txt2[200];
  BYTE* img_acq  = NULL;
  UINT nimg_acq  = 0;
  UINT nimg_lost = 0;
  
  Results_manager* result = NULL;

  CTimer timer;


  if ( datarec_on )
  {
    if ( IS_PC1 ) txt.Format(L"Backlight (adsorption) particle inspection - camera %s",  part1_names[ind-3]);
    if ( IS_PC2 ) txt.Format(L"Bottomlight (reflection) particle inspection - camera %s",part2_names[ind  ]);
    result = new Results_manager( txt.GetBuffer(), 500, 1, L"%.0lf" );
  }

  UINT num_img = 30;
  UINT num_isp =  0; // number of inspection cycles completed

  if ( !camera[ind].camera ) 
  {
    txt.Format( string_802[lng], ind, camera[ind].name );
    camera[ind].text[0]->SetWindowTextW( txt );
    return 0;
  }

  SET_BIT( thread_status, 10+ind );

  txt.Format( string_810[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt );


  //TRACE("inspect_3d_particle[%d]: STARTGRABBING start mutex waiting...\n", ind);
  DWORD mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("inspect_3d_particle[%d]: STARTGRABBING mutex allowed to proceed\n", ind);

  try
  {
    //camera[ind].camera->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete );



    camera[ind].camera->StaticChunkNodeMapPoolSize = camera[ind].camera->MaxNumBuffer.GetValue();
    camera[ind].camera->SetBufferFactory( buffact );
    camera[ind].camera->MaxNumBuffer = 5;
    camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
  }
  catch (GenICam::GenericException &e)
  {
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
    TRACE( L"Genicam exception thrown <%s>\n", msg_err );
    return 99;
  }

  ReleaseMutex( mutex );
  //TRACE("inspect_3d_particle[%d]: STARTGRABBING completed - mutex released\n", ind);

  CGrabResultPtr ptrGrabResult;

  uint64_t tmstamp1 = 0;
  uint32_t frmnum1  = 0;
  uint64_t tmstamp2 = 0;
  uint32_t frmnum2  = 0;

  TRACE("inspect_3d_particle[%d] started - entering main loop\n", ind );
  SET_BIT( thread_status, 10+ind );


  UINT triggerold = trigger_label[camera[ind].triglab];


  timer.reset();

  //double time_first = 0.0;
  //double time_last  = 0.0;
  //double time_old   = 0.0;
  int    last_wait  = 0;

  // endless loop
  while ( !CHK_BIT( thread_exit , 10+ind ) ) // bit high requests to exit
  { 

    // wait sequence start trigger
    timer.reset();
    last_wait = 6;
    while ( triggerold >= trigger_label[camera[ind].triglab] && !F1_PUSHED )
    {
      Sleep( 2 );
      if ( timer.elapsed_sec()>last_wait )
      {
        last_wait = timer.elapsed_sec();
        txt.Format( string_804[lng], last_wait );
        camera[ind].text[1]->SetText( txt, 0 );
        if ( CHK_BIT( thread_exit , 10+ind ) ) goto exit_inspect_3d_particle;

      }
    }

    triggerold = trigger_label[camera[ind].triglab];
    nimg_acq = 0;
    timer.reset();

    try // Genicam exception handling
    {
      //camera[ind].camera->StartGrabbing( GrabStrategy_OneByOne , GrabLoop_ProvidedByUser  );
      camera[ind].camera->ExecuteSoftwareTrigger();

      while ( nimg_acq < particle_insp[ind]->num_img )
      {

        //camera[ind].camera->ExecuteSoftwareTrigger();

        // Wait for an image and then retrieve it. A timeout of 1000 ms is used.
        if ( camera[ind].camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_Return  ) )
        {

          if ( nimg_acq < particle_insp[ind]->num_img-1 ) camera[ind].camera->ExecuteSoftwareTrigger();

          // Image grabbed successfully?
          if ( ptrGrabResult->GrabSucceeded() )
          {
            img_acq = (BYTE*) ptrGrabResult->GetBuffer();

            tmstamp1 = ptrGrabResult->GetTimeStamp();
            frmnum1  = ptrGrabResult->GetFrameNumber();
            TRACE("inspect_3d_particle[%d]: buff=%08X - frm=%I32d - tmst=%I64d (%.3lf)\n", ind, img_acq, frmnum1, tmstamp1, ACE_CFMS*(tmstamp1-tmstamp2)  );
            tmstamp2 = tmstamp1;

            if ( particle_insp[ind] ) particle_insp[ind]->Process_image( img_acq, nimg_acq );
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


        //camera[ind].camera->ExecuteSoftwareTrigger();

      } // while ( nimg_acq < num_img )

      // last image parameters
      //tmstamp2 = ptrGrabResult->GetTimeStamp();
      //frmnum2  = ptrGrabResult->GetFrameNumber();


      if ( learn_reflections )
      {
        txt.Format( string_2046[lng] );
        camera[ind].text[1]->SetText( txt, DARK_BLUE );
      }
      else
      {

        if (  particle_insp[ind]->part_err >  particle_insp[ind]->max_err )
        {
          txt.Format( string_842[lng], particle_insp[ind]->part_err, particle_insp[ind]->max_err );
          camera[ind].text[1]->SetText( txt, DARK_RED );
        }
        else
        {
          txt.Format( string_840[lng], particle_insp[ind]->part_err, particle_insp[ind]->max_err );
          camera[ind].text[1]->SetText( txt, DARK_GREEN );
        }

        if ( datarec_on && result )
        {
          //txt.Format(L"inspect_3d_particle[%d]",ind);
          result->Insert( particle_insp[ind]->part_err ); //   ( txt.GetBuffer(), 200, 3 );
        }


      }

      num_isp++;
      txt.Format( string_832[lng], num_isp, nimg_acq, nimg_lost, timer.elapsed_ms() );
//      txt.Format( string_834[lng], num_isp, nimg_acq, frmnum2-frmnum1, nimg_lost, 1000.0*double(tmstamp2-tmstamp1)/double(ACE_FREQUENCY), timer.elapsed_ms() );
      camera[ind].text[2]->SetWindowTextW( txt );
      nimg_acq = 0;

    }
    catch (GenICam::GenericException &e)
    {
      nimg_lost++;
      size_t convertedChars = 0;
      mbstowcs_s(&convertedChars, msg_err, 200, e.GetDescription(), _TRUNCATE); // conversione char=>wchar_t
      TRACE( L"inspect_3d_particle[%d]: genicam exception thrown <%s>\n", ind, msg_err );
    }


  }
  exit_inspect_3d_particle:

  image_dsp[ind]->ptr_ext = NULL;
  if ( img_acq && AfxIsValidAddress(img_acq,image_dsp[ind]->buf_size) ) CopyMemory( image_dsp[ind]->ptr_buf, img_acq, image_dsp[ind]->buf_size  );

  //TRACE("inspect_3d_particle[%d]: STOPGRABBING start mutex waiting...\n", ind);
  mutex_res = WaitForSingleObject( mutex, INFINITE );   // handle to mutex - no time-out interval
  ASSERT( mutex_res == WAIT_OBJECT_0 ); 
  //TRACE("inspect_3d_particle[%d]: STOPGRABBING mutex allowed to proceed\n", ind);

  camera[ind].camera->StopGrabbing();

  ReleaseMutex( mutex );
  //TRACE("inspect_3d_particle[%d]: STOP GRABBING completed - mutex released\n", ind);

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


  txt.Format( string_800[lng], ind, camera[ind].name );
  camera[ind].text[0]->SetWindowTextW( txt  );
  camera[ind].text[1]->SetWindowTextW( L"-" );
  camera[ind].text[2]->SetWindowTextW( L"-" );

  TRACE("inspect_3d_particle[%d] exiting main loop thread_exit=%d\n", ind, thread_exit );


  CLR_BIT( thread_status , 10+ind ); // confirm thread exit
  CLR_BIT( thread_exit   , 10+ind ); // confirm thread exit
  TRACE("inspect_3d_particle[%d] exiting main loop\n", ind );
  
  return 0;
}






            tmstamp1 = ptrGrabResult->GetTimeStamp();
            frmnum1  = ptrGrabResult->GetFrameNumber();
            TRACE("inspect_3d_particle[%d]: buff=%08X - frm=%I32d - tmst=%I64d (%.3lf)\n", ind, img_acq, frmnum1, tmstamp1, ACE_CFMS*(tmstamp1-tmstamp2)  );
            tmstamp2 = tmstamp1;







*/