#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Tip_inspection.h"
//#include "ChildView.h"
#include <stack>//Pour 20200320

using namespace cv;

//CArea_adjust* areainsp=NULL; // inspection/search area

extern Camera_struct    camera   [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;

extern BOOL             learning;
extern BOOL             tipcap_tip;      // used only in combined tip/cap mode, when true = TIP mode / false = CAP mode

#define  xmin    area->buffrect.left  
#define  xmax    area->buffrect.right 
#define  ymin    area->buffrect.top   
#define  ymax    area->buffrect.bottom

extern int ATNLUT[DIMATN][DIMATN];  // array[dx][dy]

int bias8[8][2] = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 },{ -1, -1 },{ -1, 1 },{ 1, -1 },{ 1, 1 } };//Pour added for SANJING on 20200320

// #define AMPATN             6 // semilenght for atan calculation
#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DY pty[i+AMPATN]-pty[i-AMPATN]
#define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]


//  #undef PTD
#define PTD(i)  PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

//#define PTD2(i) PT(ptx2[ptn2]+ofsx[(i)&7],pty2[ptn2]+ofsy[(i)&7])


//#define ANGMARK1(a) (a< 9000?a+36000:a)
//#define ANGMARK2(a) (a>27000?a-36000:a)
//#define DIFANG(a1,a2) (a1-a2>18000?

/*
  #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],col_cross); // inner left end of sealing PAU
  #endif
  */

extern BOOL             datarec_on;
extern Results_manager* cap_result;


#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing
  //UINT __cdecl processing_thread  ( LPVOID pParam );
  extern CEvent* processing_start[2];
  extern CEvent* processing_done[2];
  extern BYTE*   img_ptr;
#endif




CTip_inspection::CTip_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in/*=TIP_INSPECTION*/ )
{
  ASSERT( ind>=0 && ind<num_tlc );

  mode = mode_in; //TIP_INSPECTION or CAP_INSPECTION
  ASSERT( mode==TIP_INSPECTION || mode==CAP_INSPECTION || mode==TOPLIGHT_INSPECTION || mode==TIPCAP_INSPECTION || mode==ORALCAP_INSPECTION );

  file_manager = file_manager_in;

  index = ind;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
  area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =   4; //   dimy/6;
  area->buffrect.bottom = 575; // 5*dimy/6;
  area->buffrect.left   = 276; //   dimx/6;
  area->buffrect.right  = 644; // 5*dimx/6;

  profile      = NULL;
  prosize      =    0;

  contour_thr  =   50;
  min_npt      =  200;

  htot_max     =  700;
  htot_min     =  600;

  maxerr_symm  =    0;
  maxerr_curv  =  100;
  maxerr_bulge =    0;
  maxerr_burns =  100;

  burns_hmax   =  1.0; // distance from top to start (mm)
  burns_hmin   = 10.0; // distance from top to end (mm)
  burns_maxvar =  100;
  burns_border =   14;

  tip_diameter =  7.0;
  scalefct     = 210.0/11.0; // mm to pixel scale factor (calculated on prototype machine)


  if ( mode==ORALCAP_INSPECTION )
  {
    // AD segments
    AD_lenghtmax  =  80;
    AD_lenghtmin  =  40;
    AD_percmax    =  10;

    // DL-DR segment
    DRL_lenghtmax = 200;
    DRL_lenghtmin = 100;

    // BC segments
    BC_lenghtmax  = 200;
    BC_lenghtmin  = 100;
    BC_percmax    =  20;
    BC_angmax     =  12;
    BC_angmin     =  20;
  }


  // Always work by difference!
  if ( mode==CAP_INSPECTION || mode==TOPLIGHT_INSPECTION || mode==TIPCAP_INSPECTION || mode==ORALCAP_INSPECTION )
  {
    scalefct = 395.0/20.5; // mm to pixel scale factor (calculated on AUSIA 2013-003V)
    mm2pix   = scalefct;
    pix2mm   = 1.0/scalefct;

    contour_thr   =  90;
    min_npt       = 200;

    htot_max      = 700;
    htot_min      = 600;

    maxerr_modtot = 100;

    ZeroMemory( model_ref, sizeof(model_ref) );

    CD_lenghtmin = 60;
    CD_lenghtmax = 92;
    CD_percmax   = 10;

    DE_lenghtmin = 38;
    DE_lenghtmax = 44;
    DE_percmax   = 20;
    DE_angmin    = 12;
    DE_angmax    = 20;

  }


  tipthick_height  =  14; // thickness from top to search for dark spots
  tipthick_minlum  =  40; // minium brightness for top thickness 
  tipthick_minnpt  = 400; // minimum acceptable number of dark points 

// NEW DOUBLE CAP PARAMETERS

  tl_contour       =  30;

  // Parameters related to FLIP OFF SIDE analysis
  tl_fohgtofs      = -50;
  tl_fowidth       = 360;
  tl_foheight      =  40;
  tl_fomin         =  10;
  tl_fomax         = 130;
  tl_folenght      =   6;
  tl_foedge        = 100;
  tl_fomaxerr      = 500;
  //Pour added 20180927
  tl_fomaxerr2     = 600;

  // Parameters related to ALUMINIUM SIDE analysis
  tl_aluhgtofs     =  10;
  tl_aluwidth      = 390;
  tl_aluheight     =  50;
  tl_alumin        =   5;
  tl_alumax        = 130;
  tl_alulenght     =   6;
  tl_aluedge       = 100;
  tl_alumaxerr     = 500;
  //Pour added 20180927
  tl_alumaxerr2     = 600;

  // Parameters related to ALUMINIUM CRIMPING analysis
  tl_crihgtofs     =  10;
  tl_criwidth      = 390;
  tl_criheight     =  50;
  tl_crimin        =   5;
  tl_crimax        = 130;
  tl_crilenght     =   6;
  tl_criedge       = 100;
  tl_crimaxerr     = 500;

  // Parameters related to STOPPER SIDE analysis
  tl_stohgtofs     =  80;
  tl_stowidth      = 220;
  tl_stoheight     =  40;
  tl_stomin        =   0;
  tl_stomax        = 130;
  tl_stolenght     =   6;
  tl_stoedge       = 100;
  tl_stomaxerr     = 500;

  //Pour added 20180927
  tl_stomaxerr2    = 600;

  // shoulder check parameters
  shl_height       =    0;
  shl_maxslp       = -900;
  shl_minslp       =  900;
  shl_maxerr       =  500;
  shl_slplen       =   10;

  bl_drawovl = TRUE;
  tl_drawovl = TRUE;

  #ifdef NEWCAP_DOUBLE
    area->buffrect.left   = 300; //   dimx/6;
    area->buffrect.top    =  50; //   dimy/6;
    area->buffrect.right  = 600; // 5*dimx/6;
    area->buffrect.bottom = 510; // 5*dimy/6;
    htot_max              = area->buffrect.right- 10;
    htot_min              = area->buffrect.right-100;
    shl_height            =  50;
  #endif

  #ifdef USE_ACA640_TIP
    area->buffrect.top    =    dimy/6;
    area->buffrect.bottom =  5*dimy/6;
    area->buffrect.left   =    dimx/6;
    area->buffrect.right  =  5*dimx/6;
  #endif

  dxfins = 60;  // x to skip to avoid fins

  edge_minval  = 200; // minimum score for crimping dark rim 
  edge_minwdt  =   3; // minimum width crimping dark rim 
  edge_maxwdt  =  15; // maximum width crimping dark rim 
  edge_npt     =   0; // number of good crimping points found
  edge_minnpt  = 100; // minimum allowed number of good crimping points 
  edge_minside =  50; // minimum left/right heights to validate the found bottom

  edge_minhgt  =  10;  // minimum acceptable height of crimping dark rim 
  edge_maxhgt  =  30;  // maximum acceptable height of crimping dark rim 

  crmparea_w   = 250;  // width of crimping analysis area 
  crmparea_h   =  40;  // height of crimping analysis area 
  crmparea_vo  = -50;  // vertical offset of crimping analysis area 

  pwdrstpr_w      =  200;  // width of powder stopper analysis area 
  pwdrstpr_h      =   45;  // height of powder stopper analysis area 
  pwdrstpr_vo     =  -60;  // vertical offset of  powder stopper analysis area 
  pwdrstpr_minl   =  100; // minimum brightness for powder stopper analysis 
  pwdrstpr_maxl   =  200; // maximum brightness for powder stopper analysis
  pwdrstpr_maxerr = 1000; // maximum allowed error for powder stopper analysis


  #ifdef NEWCAP_V30

  min_npt          =  900;
  max_npt          = 1200;


  // Parameters related to FLIP OFF SIDE analysis
  tl_fohgtofs      =  60;
  tl_fowidth       = 420;
  tl_foheight      =  40;
  tl_fomin         =  10;
  tl_fomax         = 130;
  tl_folenght      =   6;
  tl_foedge        = 100;
  tl_fomaxerr      = 500;

  // Parameters related to ALUMINIUM SIDE analysis
  tl_aluhgtofs     = -10;
  tl_aluwidth      = 400;
  tl_aluheight     =  50;
  tl_alumin        =   5;
  tl_alumax        = 130;
  tl_alulenght     =   6;
  tl_aluedge       = 100;
  tl_alumaxerr     = 500;

  // Parameters related to ALUMINIUM CRIMPING analysis
  tl_crihgtofs     =  18;
  tl_criwidth      = 310;
  tl_criheight     =  18;
  tl_crimin        =   5;
  tl_crimax        = 130;
  tl_crilenght     =   6;
  tl_criedge       = 100;
  tl_crimaxerr     = 500;

  // Parameters related to STOPPER SIDE analysis
  tl_stohgtofs     = -10;
  tl_stowidth      = 270;
  tl_stoheight     =  40;
  tl_stomin        =   0;
  tl_stomax        = 130;
  tl_stolenght     =   6;
  tl_stoedge       = 100;
  tl_stomaxerr     = 500;

  AB_lenghtmin     =  45;
  AB_lenghtmax     =  65;
  AB_percmax       =  20;
  AB_angmin        =  20;
  AB_angmax        =  30;

  BC_lenghtmin     =  80;
  BC_lenghtmax     = 100;
  BC_percmax       =  10;
  BC_angmin        =  85;
  BC_angmax        =  95;

  #endif

  #ifdef NEW_TIP_V30
    contour_thr      =    80;
    min_npt          =   500;
    max_npt          =  1000;

    blgsrk_hmin      =    70; // minimum distance from top (upper) for bulge shrink analysis
    blgsrk_hmax      =   200; // maximum distance from top (lower) for bulge shrink analysis
    blgsrk_hchk      =    10; // height to check variation of width
    blgsrk_maxd      =     4; // max delta over height to check error
    blgsrk_mxer      =    20; // max allowed bulge shrink error 

    innbrn_hmin      =    50; // upper height for analysis of inner burns (0 disabled)
    innbrn_hmax      =   350; // lower height for analysis of inner burns (0 disabled)
    innbrn_minw      =    10; // inner burns minimum width
    innbrn_minb      =   120; // inner burns minimum brightness
    innbrn_fltm      =    50; // inner burns filter threshold
    innbrn_mxer      =   100; // max allowed inner burns error 

    tpcurv_hmin      =     0; // upper height for analysis of tip top curvature (0 disabled)
    tpcurv_hmax      =    30; // lower height for analysis of tip top curvature (0 disabled)
    tpcurv_modw      =   120; // curvature reference width  (height in buffer coords)
    tpcurv_modh      =    89; // curvature reference height (width  in buffer coords)
    tpcurv_mxer      =   100; // max allowed tip curvature error 

//  tpcurv_hmax =   0;
//  tpcurv_hmin = tpcurv_modh/2;
//  tpcurv_hmax += 10;
//  tpcurv_hmin -= 20;


  #endif




	//Pour added on 20200320
#ifdef NECK_CRACK
	
	para_crack.insp_offset = 15;
	para_crack.insp_height = 70;
	para_crack.th_blob_area = 20;
	para_crack.th_blob_ratio = 6.0;
	para_crack.th_blob_width = 6;
	para_crack.th_crack_area = 80 ;
	para_crack.th_crack_brtness = 150;

#endif
	//Pour added on 202003320

}


CTip_inspection::~CTip_inspection(void)
{
  if ( profile ) free( profile );
  profile = NULL;

  if ( area )
  {
    delete area;
    area = NULL;
  }
}




int CTip_inspection::Allocate_profile( void )
{
  if ( profile ) free( profile );
  prosize = xmax-xmin+1;
  profile = (segm_type*) malloc( sizeof(segm_type)*prosize );
  return 0;
}


int CTip_inspection::Set_inspection_area( int ind )
{
//  ASSERT( (UINT)ind>=0 && (UINT)ind<num_tlc );
//  if ( area && area->m_hWnd ) area->ShowWindow( SW_SHOW );

  if ( CHK_BIT( thread_status, 10+((CArea_adjust*)ind)->index ) )
  {
    Message_box( string_2022[lng], MBOX_INFO );
    return( 0 );
  }

  CArea_adjust* area = (CArea_adjust*) ind; // inspection/search area

  ASSERT( area->index>=0 && area->index<num_tlc ); 
  if ( area && area->m_hWnd && image_dsp[area->index]->visible ) area->ShowWindow( SW_SHOW );

  TRACE("CTip_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CTip_inspection::Add_menu_parameters_v30( CContainer_wnd* child )
{

  // Contour & inspection area
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2910[lng], child );
  file_manager->Add_separator( string_2910[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &contour_thr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702[lng], string_log [lng],   50,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  200,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_npt,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  200,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &htot_max,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_630[lng], string_pix [lng],  720,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &htot_min,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_632[lng], string_pix [lng],  600,    0,   770, file_manager );
  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

  // AB segments
  pView->parameter[pView->num_params++] = new CParameter( &AB_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_560[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &AB_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_561[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &AB_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_562[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &AB_angmax,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_563[lng], string_degrees[lng], 60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &AB_angmin,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_564[lng], string_degrees[lng], 60,    0,   200, file_manager );
  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

  // BC segments
  pView->parameter[pView->num_params++] = new CParameter( &BC_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_687[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &BC_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_688[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &BC_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_689[lng], string_pix[lng],     60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &BC_angmax,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_690[lng], string_degrees[lng], 60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &BC_angmin,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_691[lng], string_degrees[lng], 60,    0,   200, file_manager );

  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  // FLIP-OFF SIDE
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2912[lng], child );
  file_manager->Add_separator( string_2912[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &tl_fohgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   -5, -200,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_fowidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2932[lng], string_pix [lng],  200,   50,   500, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_foheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],   50,   10,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_fomax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2940[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_fomin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2942[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_folenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2944[lng], string_pix [lng],   10,    1,    30, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_foedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2936[lng], string_unit[lng],  100,    1,  3000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946[lng], string_unit[lng], 1000,    0, 99999, file_manager );
  //Pour added 20180927
	#ifdef REJECTION_SEPARATION
    pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#endif
  pView->num_menu_btn++;

  // ALUMINIUM SIDE
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2914[lng], child );
  file_manager->Add_separator( string_2914[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &tl_aluhgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   -5, -200,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_aluwidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2932[lng], string_pix [lng],  200,   50,   500, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_aluheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],   50,   10,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_alumax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2940[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_alumin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2942[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_alulenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2944[lng], string_pix [lng],   10,    1,    30, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_aluedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2936[lng], string_unit[lng],  100,    1,  3000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946[lng], string_unit[lng], 1000,    0, 99999, file_manager );
    //Pour added 20180917
	#ifdef REJECTION_SEPARATION
    pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#endif
  pView->num_menu_btn++;

  // ALUMINIUM CRIMPING
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2916[lng], child );
  file_manager->Add_separator( string_2916[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &tl_crihgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   -5, -200,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_criwidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2932[lng], string_pix [lng],  200,   50,   500, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_criheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],   50,   10,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_crimax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2940[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_crimin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2942[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_crilenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2944[lng], string_pix [lng],   10,    1,    30, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_criedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2936[lng], string_unit[lng],  100,    1,  3000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_crimaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946[lng], string_unit[lng], 1000,    0, 99999, file_manager );
  pView->num_menu_btn++;

  // RUBBER STOPPER
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2918[lng], child );
  file_manager->Add_separator( string_2918[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &tl_stohgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   -5, -200,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stowidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2932[lng], string_pix [lng],  200,   50,   500, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stoheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],   50,   10,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stomax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2940[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stomin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2942[lng], string_log [lng],  120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stolenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2944[lng], string_pix [lng],   10,    1,    30, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stoedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2936[lng], string_unit[lng],  100,    1,  3000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#endif
  pView->num_menu_btn++;


  TRACE("CTip_inspection::Add_menu_parameters_v30(%d)\n", index);
  return( 0 );
}


int CTip_inspection::Add_menu_parameters_tip_v30( CContainer_wnd* child )
{

  // Contour & inspection area
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2910[lng], child );
  file_manager->Add_separator( string_2910[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &contour_thr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702[lng], string_log [lng],   50,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  200,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &max_npt,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  200,    0,  9900, file_manager );
  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->parameter[pView->num_params++] = new CParameter( &htot_max,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_630[lng], string_pix [lng],  720,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &htot_min,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_632[lng], string_pix [lng],  600,    0,   770, file_manager );
  pView->num_menu_btn++;

  // BULGE SHRINK
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3208[lng], child );
  file_manager->Add_separator( string_3208[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &blgsrk_hmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3142[lng], string_pix [lng],  20,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &blgsrk_hmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3140[lng], string_pix [lng], 160,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &blgsrk_hchk,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3216[lng], string_pix [lng],  10,    5,    50, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &blgsrk_maxd,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3218[lng], string_unit[lng],  60,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &blgsrk_mxer,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng],  60,    0, 99999, file_manager );
  pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
  pView->num_menu_btn++;

  // INNER BURNS
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3238[lng], child );
  file_manager->Add_separator( string_3238[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &innbrn_hmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3142[lng], string_pix [lng],  20,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &innbrn_hmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3140[lng], string_pix [lng], 160,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &innbrn_minw,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3248[lng], string_pix [lng],  10,    0,    50, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &innbrn_minb,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3250[lng], string_unit[lng], 120,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &burns_maxvar,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_714[lng], string_unit[lng],  200,    0, 10000, file_manager );
//  pView->parameter[pView->num_params++] = new CParameter( &innbrn_fltm,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3252[lng], string_unit[lng],  60,    0,   800, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &innbrn_mxer,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng],  60,    0, 99999, file_manager );
  pView->num_menu_btn++;

  // TIP TOP CURVATURE
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3280[lng], child );
  file_manager->Add_separator( string_3280[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &tpcurv_hmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3142[lng], string_pix [lng],  20,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tpcurv_hmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3140[lng], string_pix [lng], 160,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tpcurv_modw,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3288[lng], string_pix [lng],  10,    0,   250, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tpcurv_modh,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3290[lng], string_pix [lng], 120,    0,   130, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &tpcurv_mxer,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng],  60,    0, 99999, file_manager );
  pView->num_menu_btn++;

  pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-4]->child, &Set_inspection_area, (int) area ); //index );

  TRACE("CTip_inspection::Add_menu_parameters_tip_v30(%d)\n", index);
  return( 0 );
}



int CTip_inspection::Add_menu_parameters( CContainer_wnd* child )
{

  if ( mode==TIP_INSPECTION )
  {
    #ifdef NEW_TIP_V30
      Add_menu_parameters_tip_v30( child );
      return 0; // completed successfully
    #endif

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_700[lng], child );
    file_manager->Add_separator( string_700[lng], area );
  }

  if ( mode==CAP_INSPECTION )
  {
    #ifdef NEWCAP_V30
      Add_menu_parameters_v30( child );
      return 0; // completed successfully
    #endif

    // normal/old
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_660[lng], child );
    file_manager->Add_separator( string_660[lng], area );
  }

  if ( mode==TOPLIGHT_INSPECTION || mode==ORALCAP_INSPECTION )
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_1406[lng], child );
    file_manager->Add_separator( string_660[lng], area );
  }
  if ( mode==TIPCAP_INSPECTION )
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_1408[lng], child );
    file_manager->Add_separator( string_662[lng], area );
  }

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &contour_thr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702[lng], string_log [lng],   50,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  200,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &htot_max,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_630[lng], string_pix [lng],  720,    0,   770, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &htot_min,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_632[lng], string_pix [lng],  600,    0,   770, file_manager );

  if ( mode==TIP_INSPECTION || mode==TIPCAP_INSPECTION ) // parameters specific for TIP control (or combined)
  {
    pView->parameter[pView->num_params++] = new CParameter( &tip_diameter,    CTYPE_LREAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_704[lng], string_mm  [lng],  7.0,  1.0,  20.0, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_curv,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_706[lng], string_unit[lng],  100,    0,  9000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_symm,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_708[lng], string_unit[lng],  100,    0,  9000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_bulge,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_710[lng], string_unit[lng],  100,    0,  9000, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &burns_border,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_712[lng], string_pix [lng],   10,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &burns_maxvar,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_714[lng], string_unit[lng],  200,    0, 10000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &burns_hmax,      CTYPE_LREAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_715[lng], string_mm  [lng],  1.0,  0.0,  50.0, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &burns_hmin,      CTYPE_LREAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_716[lng], string_mm  [lng],  1.0,  1.0,  50.0, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_burns,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_718[lng], string_unit[lng], 1000,    0, 99000, file_manager );

    #ifdef TIP_THICKNESS
      pView->parameter[pView->num_params++] = new CParameter( &tipthick_height, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2200[lng], string_pix [lng],   14,   1,    50, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tipthick_minlum, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2202[lng], string_log [lng],   40,   0,   255, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tipthick_minnpt, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2204[lng], string_unit[lng],  200,   0,  9999, file_manager );
    #endif

    tipthick_minnpt = 0;

  }


  if ( mode==ORALCAP_INSPECTION ) // parameters specific for ORAL CAP control
  {
//    pView->parameter[pView->num_params++] = new CParameter( &maxerr_modtot, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_664[lng], string_unit[lng],   100,    0,  9000, file_manager );
//    file_manager->Add_data( model_ref, sizeof(model_ref) );

    // AD segments
    pView->parameter[pView->num_params++] = new CParameter( &AD_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_680[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &AD_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_681[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &AD_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_682[lng], string_percent[lng], 10,    0,   200, file_manager );

    // DL-DR segment
    pView->parameter[pView->num_params++] = new CParameter( &DRL_lenghtmax, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_684[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &DRL_lenghtmin, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_685[lng], string_pix[lng],     60,    0,   200, file_manager );

    // BC segments
    pView->parameter[pView->num_params++] = new CParameter( &BC_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_687[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &BC_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_688[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &BC_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_689[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &BC_angmax,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_690[lng], string_degrees[lng], 60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &BC_angmin,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_691[lng], string_degrees[lng], 60,    0,   200, file_manager );

//    #if defined NEWCAP_DOUBLE && defined NEWCAP_V21 // Additional parameters for double cap inspection and shoulder 
    RECT area;
    pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
    area.top -= 60;
    pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
    pView->parameter[pView->num_params++] = new CParameter( &shl_height,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1910[lng], string_pix [lng],     50,    0,   500, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &shl_maxslp,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1916[lng], string_unit[lng],     60,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &shl_minslp,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1914[lng], string_unit[lng],     30,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &shl_slplen,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1918[lng], string_pix [lng],     10,    1,    40, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &shl_maxerr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1920[lng], string_unit[lng],    300,    0,  9999, file_manager );
//    #endif
  }



  if ( mode==CAP_INSPECTION || mode==TOPLIGHT_INSPECTION || mode==TIPCAP_INSPECTION ) // parameters specific for CAP control
  {
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_modtot, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_664[lng], string_unit[lng],   100,    0,  9000, file_manager );
    file_manager->Add_data( model_ref, sizeof(model_ref) );

    #if SERIAL_N==201500002 || SERIAL_N==201900025 ||SERIAL_N==201900030 // Sanjiang skip all parameters related to B&C and add dxfins
      pView->parameter[pView->num_params++] = new CParameter( &dxfins,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_666[lng], string_pix[lng],     60,    0,   200, file_manager );
    #else 
    pView->parameter[pView->num_params++] = new CParameter( &CD_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_670[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &CD_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_671[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &CD_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_672[lng], string_percent[lng], 10,    0,   200, file_manager );
    #endif

    pView->parameter[pView->num_params++] = new CParameter( &DE_lenghtmax,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_674[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &DE_lenghtmin,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_675[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &DE_percmax,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_676[lng], string_pix[lng],     60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &DE_angmax,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_678[lng], string_degrees[lng], 60,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &DE_angmin,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_679[lng], string_degrees[lng], 60,    0,   200, file_manager );

    #if defined NEWCAP_DOUBLE && defined NEWCAP_V21 // Additional parameters for double cap inspection and shoulder 
      RECT area;
      pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
      area.top -= 60;
      pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);

      pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
      pView->parameter[pView->num_params++] = new CParameter( &shl_height,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1910[lng], string_pix [lng],     50,    0,   500, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &shl_maxslp,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1916[lng], string_unit[lng],     60,    0,   100, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &shl_minslp,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1914[lng], string_unit[lng],     30,    0,   100, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &shl_slplen,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1918[lng], string_pix [lng],     10,    1,    40, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &shl_maxerr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1920[lng], string_unit[lng],    300,    0,  9999, file_manager );
    #endif
  }

  if ( mode==TIPCAP_INSPECTION || mode==ORALCAP_INSPECTION ) // parameters specific for CAP control
  {
    RECT area;
    pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
//    area.top -= 290;
    area.top -= 210;
    pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);
  }

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

//  if ( mode==CAP_INSPECTION ) file_manager->Add_data( model, sizeof(mod_type)*CAPMODELSIZE );
//  if ( mode==CAP_INSPECTION ) file_manager->Add_data( model, sizeof(mod_type)*CAPMODELSIZE );

  #ifdef NEWCAP_DOUBLE // Additional parameters for double cap inspection - TOP-LIGHT INSPECTION PARAMETERS

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_1404[lng], child );
    file_manager->Add_separator( string_1404[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &tl_contour,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702[lng], string_log [lng],    50,    0,   255, file_manager );
    
    // ALUMINIUM SIDE
    pView->menu_btn[pView->num_menu_btn]->child->posy += 5; // Set vertical position
    pView->parameter[pView->num_params++] = new CParameter( &tl_aluhgtofs,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1410[lng], string_pix [lng],   -5, -200,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_aluwidth,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1412[lng], string_pix [lng],  200,   50,   500, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_aluheight,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1414[lng], string_pix [lng],   50,   10,   200, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &tl_alumax,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1420[lng], string_log [lng],  120,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_alumin,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1422[lng], string_log [lng],  120,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &tl_alulenght,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1424[lng], string_pix [lng],   10,    1,    30, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_aluedge,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1416[lng], string_unit[lng],  100,    1,  3000, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1426[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1426a[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr2,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1426b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#else
	pView->parameter[pView->num_params++] = new CParameter( &tl_alumaxerr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1426[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#endif

    // FLIP-OFF SIDE
    pView->menu_btn[pView->num_menu_btn]->child->posy += 5; // Set vertical position
    pView->parameter[pView->num_params++] = new CParameter( &tl_fohgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1430[lng], string_pix [lng],   -5, -200,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_fowidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1432[lng], string_pix [lng],  200,   50,   500, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_foheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1434[lng], string_pix [lng],   50,   10,   200, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &tl_fomax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1440[lng], string_log [lng],  120,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_fomin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1442[lng], string_log [lng],  120,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &tl_folenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1444[lng], string_pix [lng],   10,    1,    30, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &tl_foedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1436[lng], string_unit[lng],  100,    1,  3000, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1446[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1446a[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1446b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
#else
	pView->parameter[pView->num_params++] = new CParameter( &tl_fomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1446[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	#endif


    #ifdef NEWCAP_V21
      // STOPPER SIDE
      RECT area;
      pView->menu_btn[pView->num_menu_btn]->child->GetWindowRect(&area);
      //area.top -= 260;
	  area.top -= 355;  //Pour 20180927
	  area.bottom -=35; //Pour 20180927
      pView->menu_btn[pView->num_menu_btn]->child->MoveWindow(&area);

      pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position
      pView->parameter[pView->num_params++] = new CParameter( &tl_stohgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1484[lng], string_pix [lng],   -5, -300,   300, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tl_stowidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1486[lng], string_pix [lng],  200,   50,   500, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tl_stoheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1488[lng], string_pix [lng],   50,   10,   200, file_manager );

      pView->parameter[pView->num_params++] = new CParameter( &tl_stomax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1492[lng], string_log [lng],  120,    0,   255, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tl_stomin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1494[lng], string_log [lng],  120,    0,   255, file_manager );

      pView->parameter[pView->num_params++] = new CParameter( &tl_stolenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1496[lng], string_pix [lng],   10,    1,    30, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tl_stoedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1490[lng], string_unit[lng],  100,    1,  3000, file_manager );
      //pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1498[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	  //Pour added 20180927
	  #ifdef REJECTION_SEPARATION
	  pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1498a[lng], string_unit[lng], 1000,    0, 99999, file_manager );
      pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1498b[lng], string_unit[lng], 1000,    0, 99999, file_manager );
#else
	  pView->parameter[pView->num_params++] = new CParameter( &tl_stomaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_1498[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	  #endif

    #endif

    pView->num_menu_btn++;

  #endif

  #ifdef TEST_BOSEN
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2650[lng], child );
    file_manager->Add_separator( string_2650[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


    pView->parameter[pView->num_params++] = new CParameter( &crmparea_w,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2670[lng], string_pix [lng],   200,   50,   400, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &crmparea_h,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2672[lng], string_pix [lng],    40,   10,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &crmparea_vo,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2674[lng], string_pix [lng],    20, -200,   200, file_manager );

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

    pView->parameter[pView->num_params++] = new CParameter( &edge_minval,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2680[lng], string_log [lng],  400,    0,  9999, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &edge_minside,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2688[lng], string_unit[lng],  200,    0,  9999, file_manager );

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

    pView->parameter[pView->num_params++] = new CParameter( &edge_minwdt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2682[lng], string_pix [lng],   10,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &edge_maxwdt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2684[lng], string_pix [lng],   20,    0,   300, file_manager );

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

    pView->parameter[pView->num_params++] = new CParameter( &edge_minhgt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2692[lng], string_pix [lng],   10,    0,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &edge_maxhgt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2694[lng], string_pix [lng],   30,    0,   300, file_manager );

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

    pView->parameter[pView->num_params++] = new CParameter( &edge_minnpt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2686[lng], string_unit[lng],  100,    0,  9999, file_manager );

    pView->num_menu_btn++;

   // pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );
  #endif

	//Pour added 20180707
	  #ifdef TEST_QILU
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2650[lng], child );
    file_manager->Add_separator( string_2650[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


    //pView->parameter[pView->num_params++] = new CParameter( &crmparea_w,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2670[lng], string_pix [lng],   200,   50,   400, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &crmparea_h,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2672[lng], string_pix [lng],    40,   10,   200, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &crmparea_vo,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2674[lng], string_pix [lng],    20, -200,   200, file_manager );

	//Pour added 20180707
	//pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

	pView->parameter[pView->num_params++] = new CParameter( &tl_crihgtofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   -5, -500,   500, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_criwidth,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2932[lng], string_pix [lng],  200,   50,   500, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_criheight,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],   50,   10,   200, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_crimax,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2940[lng], string_log [lng],  120,    0,   255, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_crimin,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2942[lng], string_log [lng],  120,    0,   255, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_crilenght,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2944[lng], string_pix [lng],   10,    1,    30, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_criedge,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2936[lng], string_unit[lng],  100,    1,  3000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &tl_crimaxerr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2946[lng], string_unit[lng], 1000,    0, 99999, file_manager );
	pView->num_menu_btn++;
	//Pour added 20180707

  #endif//TEST_QILU
	//Pour added 20180707

	
	//Pour added 20200320
	#ifdef NECK_CRACK
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2650a[lng], child );
    file_manager->Add_separator( string_2650a[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

	//pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

	pView->parameter[pView->num_params++] = new CParameter( &para_crack.insp_offset,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2930[lng], string_pix [lng],   15,  0,   200, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.insp_height,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2934[lng], string_pix [lng],  70,   10,   200, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.th_crack_brtness, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2935[lng], string_pix [lng],   150,   0,   255, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.th_blob_area,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2937[lng], string_log [lng],  20,    0,   1000, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.th_blob_ratio,    CTYPE_LREAL,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2939[lng], string_log [lng],  6.0,    1.0,   20.0, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.th_blob_width,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2941[lng], string_pix [lng],   6,    1,    30, file_manager );
	pView->parameter[pView->num_params++] = new CParameter( &para_crack.th_crack_area,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2943[lng], string_unit[lng],  80,    0,  3000, file_manager );
	pView->num_menu_btn++;

  #endif//NECK_CRACK
	//Pour added 20200320
	

  #ifdef POWDER_STOPPER // id defined, test powder on rubber stopper

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2640[lng], child );
    file_manager->Add_separator( string_2640[lng] );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_w,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2670[lng], string_pix [lng],   200,   50,   400, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_h,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2672[lng], string_pix [lng],    40,   10,   200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_vo,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2674[lng], string_pix [lng],    20, -200,   200, file_manager );

    pView->menu_btn[pView->num_menu_btn]->child->posy += 10; // Set vertical position

    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_minl, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_618[lng], string_log[lng],  100,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_maxl, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_619[lng], string_log[lng],  200,    0,   255, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &pwdrstpr_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_622[lng], string_unit[lng],  1000,  0, 99999, file_manager );

    pView->num_menu_btn++;
  #endif


  return 0; // completed successfully
}





#define  PT(x,y) img_acq[x+(y)*camera[index].width]

//#define FLTUP(x,y) (PT(x,y-2)+PT(x+2,y-1)-PT(x,y)-PT(x+1,y+1))
//#define FLTDN(x,y) (PT(x,y+2)+PT(x+2,y+1)-PT(x,y)-PT(x+1,y-1))

/* old, non troppo buono, troppo sensibile
#define FLTUPTIP(x,y) (PT(x-1,y-2)+PT(x+2,y+1)-PT(x-1,y)-PT(x,y+1))
#define FLTDNTIP(x,y) (PT(x-1,y+2)+PT(x+2,y-1)-PT(x-1,y)-PT(x,y-1))
*/

// new messo a punto 20140613
// HORIZONTAL 
#define FLTUPTIPH(x,y) (PT(x-1,y-2)+PT(x,y-2)+PT(x+1,y-2)-PT(x-1,y)-PT(x,y)-PT(x+1,y))
#define FLTDNTIPH(x,y) (PT(x-1,y+2)+PT(x,y+2)+PT(x+1,y+2)-PT(x-1,y)-PT(x,y)-PT(x+1,y))
// DIAGONAL 
#define FLTUPTIPD(x,y) (PT(x+1,y-3)+PT(x+2,y-2)+PT(x+3,y-1)-PT(x-1,y-1)-PT(x,y)-PT(x+1,y+1))
#define FLTDNTIPD(x,y) (PT(x+1,y+3)+PT(x+2,y+2)+PT(x+3,y+1)-PT(x-1,y+1)-PT(x,y)-PT(x+1,y-1))




#define FLTUPCAP(x,y) (PT(x-1,y-1)+PT(x+1,y-1))-(PT(x-1,y+1)+PT(x+1,y+1))
#define FLTDNCAP(x,y) (PT(x-1,y+1)+PT(x+1,y+1))-(PT(x-1,y-1)+PT(x+1,y-1))


int CTip_inspection::Find_line_tip( BYTE* img_acq, int x )
{
  int y1;
  int y2;

  ASSERT( profile );

  profile[x-xmin].y1     = 0;
  profile[x-xmin].y2     = 0;
  //profile[x-xmin].lenght = 0;

  y1 = ymin;
//  while ( y1<ymax && FLTUPTIP(x,y1)<contour_thr ) y1++;
  while ( y1<ymax && FLTUPTIPH(x,y1)<contour_thr && FLTUPTIPD(x,y1)<contour_thr ) y1++;

  if ( y1==ymax ) return 99;

  y2 = ymax;
//  while ( y2>y1 && FLTDNTIP(x,y2)<contour_thr ) y2--;
  while ( y2>y1 && FLTDNTIPH(x,y2)<contour_thr && FLTDNTIPD(x,y2)<contour_thr ) y2--;

  if ( y2==y1 ) return 199;

  profile[x-xmin].y1     = y1;
  profile[x-xmin].y2     = y2;
  //profile[x-xmin].lenght = y2-y1;
  pronpt++;
  yasse += y2+y1;

  xheight = x;

//  DRAW_PT( index, x, y1, 0x000000AA ); // 0xFFRRGGBB
//  DRAW_PT( index, x, y2, 0x000000AA ); // 0xFFRRGGBB

//#define SETPIXEL(nimg,x,y,col) image_dsp[nimg]->ptr_dsp[x+y*dim_dsp.cx]=col
//#define SETPIXEL2(nimg,ofs,col) image_dsp[nimg]->ptr_dsp[ofs]=col


//  BUF2DSP(index,x,y1);
//  SETPIXELXY(index,x,y1,0x000000AA ); // 0xFFRRGGBB
  
  int ofs = BUF2OFS(index,x,y1);
  SETPIXELOFS(index,ofs-1,COL_BLACK ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs  ,COL_CYAN  ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs+1,COL_BLACK ); // 0xFFRRGGBB

  ofs = BUF2OFS(index,x,y2);
  SETPIXELOFS(index,ofs-1,COL_BLACK ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs  ,COL_CYAN  ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs+1,COL_BLACK ); // 0xFFRRGGBB

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp )
    {
      image_dsp[index]->ptr_fulldsp[x+y1*dimx] = COL_CYAN;
      image_dsp[index]->ptr_fulldsp[x+y2*dimx] = COL_CYAN;
    }
  #endif



//  x = xheight;
//  BUF2DSP(index,x,y2);
//  SETPIXELXY(index,x,y2,0x000000AA ); // 0xFFRRGGBB

  return 0;
}




int CTip_inspection::Find_line_cap( BYTE* img_acq, int x )
{
  int y1;
  int y2;

  ASSERT( profile );

  profile[x-xmin].y1     = 0;
  profile[x-xmin].y2     = 0;
  //profile[x-xmin].lenght = 0;

  y1 = ymin;
  while ( y1<ymax && FLTUPCAP(x,y1)<contour_thr ) y1++;

  if ( y1==ymax ) return 99;

  y2 = ymax;
  while ( y2>y1 && FLTDNCAP(x,y2)<contour_thr ) y2--;

  if ( y2==y1 ) return 199;

  profile[pronpt].x  = x;
  profile[pronpt].y1 = y1;
  profile[pronpt].y2 = y2;
  TRACE("%d,%d,%d,%d\n",pronpt,x, y1, y2);
  pronpt++;

  yasse  += y2+y1;
  xheight = x;

  int ofs = BUF2OFS(index,x,y1);
  SETPIXELOFS(index,ofs-1,COL_BLACK ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs  ,COL_CYAN  ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs+1,COL_BLACK ); // 0xFFRRGGBB

  ofs = BUF2OFS(index,x,y2);
  SETPIXELOFS(index,ofs-1,COL_BLACK ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs  ,COL_CYAN  ); // 0xFFRRGGBB
  SETPIXELOFS(index,ofs+1,COL_BLACK ); // 0xFFRRGGBB

  return 0;
}



void CTip_inspection::Draw_height( UINT col )
{
  int ofs = BUF2OFS(index,xheight,yasse);
  for ( int y=20 ; y<=40 ; y++ )
  {
    SETPIXELOFS(index,ofs-y-DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
    SETPIXELOFS(index,ofs-y             ,col       ); // 0xFFRRGGBB
    SETPIXELOFS(index,ofs-y+DSPDX(index),COL_BLACK ); // 0xFFRRGGBB

    SETPIXELOFS(index,ofs+y-DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
    SETPIXELOFS(index,ofs+y             ,col       ); // 0xFFRRGGBB
    SETPIXELOFS(index,ofs+y+DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
  }

  // acceptable height in green, vertical
//  for ( int y=20 ; y<=40 ; y++ )
//  {



}




int CTip_inspection::Process_image( BYTE* img_acq, UINT nimg/*=0*/ )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  img = img_acq;





 // if ( prosize != xmax-xmin+1 ) Allocate_profile();
 // SCALE_IMG( index, img_acq );
  /*
  #ifdef SAVE_FULL_PROC
    image_dsp[index]->scale_fulldsp(img); // contour overlayed
    //ZeroMemory( image_dsp[index]->ptr_fulldsp, image_dsp[index]->buf_size*4 );
    //for ( UINT i=0 ; i<image_dsp[index]->buf_size ; i++ ) image_dsp[index]->ptr_fulldsp[i] = 0xFF000000;
  #endif
  */

  // draw inspection area first
  int x, y;
//  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  if ( ( nimg && tl_drawovl ) || ( !nimg && bl_drawovl ) )
  {
    for ( x=xmin ; x<=xmax ; x+=4 )
    {
      DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
    }
    for ( y=ymin ; y<=ymax ; y+=4 )
    {
      DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    }
  }

  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;
  ptn  = 0;

  #ifdef NEWCAP_V30
    return Process_image_cap_v30( img_acq, nimg );
  #endif

  #ifdef NEW_TIP_V30   // new tip control developed for Kelun

    #ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing

    // to test multithreading split
    if ( index == 2 )
    {

      CTimer timer;

      img_ptr = img_acq;

      if ( 0 )
      {
        Find_contour_cw();
        Find_contour_ccw();
      }
      else
      {
        processing_start[0]->SetEvent(); // Start thread 0 / cw
        processing_start[1]->SetEvent(); // Start thread 1 / ccw
        ::WaitForSingleObject(processing_done[0]->m_hObject, INFINITE); 
        ::WaitForSingleObject(processing_done[1]->m_hObject, INFINITE);  
      }  

      tot_count++;
      tot_time += timer.elapsed_us();

      col1 = 0;
      txt1.Format(L"last %d - average %3.3lf us", timer.elapsed_us(), double(tot_time)/double(tot_count) );


      int diff = 0;
//      ASSERT( ptn==ptn2 );

      if ( ptn==ptn2 )
      {
        for ( x=0 ; x<ptn ; x++ ) if ( ptx[x]!=ptx2[ptn-x-1] || pty[x]!=pty2[ptn-x-1] ) diff++;
      }
      else diff = -999;

      if ( diff )
      {
        col2 = TXT_DARKRED;
        txt2.Format( L"Comparison WRONG - %d points different", diff );
      }
      else
      {
        col2 = TXT_DARKGREEN;
        txt2.Format( L"Comparison Ok - %d points found", ptn );
      }


      //TRACE("diff=%d\n",diff);
      return 0;
    }
    #endif // WORKER_THREAD  // if defined uses worker threads to speed-up processing

    return Process_image_tip_v30( img_acq, nimg );
  #endif // NEW_TIP_V30   // new tip control developed for Kelun



  if ( mode==ORALCAP_INSPECTION )
  {
    if ( nimg ) return Oralcap_toplight ( img_acq, nimg ); // IF NIMG>0 TOPLIGHT  INSPECTION
    else        return Oralcap_backlight( img_acq, nimg ); // IF NIMG=0 BACKLIGHT INSPECTION
  }

  if ( mode==TOPLIGHT_INSPECTION || ( mode==TIPCAP_INSPECTION && !tipcap_tip ) )
  {
    if ( nimg ) return Process_image_toplight( img_acq, nimg ); // IF NIMG>0 TOPLIGHT  INSPECTION
    else        return Process_image_cap     ( img_acq, nimg ); // IF NIMG=0 BACKLIGHT INSPECTION
  }

  if ( mode==TIP_INSPECTION || mode==TIPCAP_INSPECTION ) return Process_image_tip( img_acq, nimg );
  if ( mode==CAP_INSPECTION ) return Process_image_cap( img_acq, nimg );


  return 999; // no inspection mode selected

}


#undef PT

#define PT(x,y) img[(x)+(y)*dimx]






int CTip_inspection::Process_image_cap_v30( BYTE* img_acq, UINT nimg )
{

  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  if ( contour_thr==0 )
  {
    col1 = 0;
    col2 = 0;
    txt1 = string_2510[lng];
    txt2 = L"";
    return 0;
  }

//  TRACE("contour_thr=%d - (%d,%d)-(%d,%d)\n", contour_thr, xmin, ymin, xmax, ymax );

  if ( Find_contour_v30() ) // contour not found/validated
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -105, index+3 ); // insert -105 into 3-4-5 cat 
    #endif 
    col1 = TXT_DARKRED;
    return 9000;
  }

  if ( Analyze_contour_v30() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -115, index+3 ); // insert -115 into 3-4-5 cat 
    #endif
    col1 = TXT_DARKRED;
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints_v30() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -125, index+3 ); // insert -125 into 3-4-5 cat 
    #endif
    col1 = TXT_DARKRED;
    //txt1.Format( string_698[lng] );
    return 9200; // some problem on contour
  }

  // Contour and keypoints OK, move on
  col1 = TXT_DARKGREEN;
//  txt1.Format( string_2562[lng], ptn, min_npt );
  txt1.Format( string_758[lng], ptn, ABL_txt, ABR_txt, BCL_txt, BCR_txt );

  reject_tot = FALSE;

  if ( Flipoff_analysis_v30 () ) reject_tot = TRUE;
  if ( Aluside_analysis_v30 () ) reject_tot = TRUE;
  if ( Crimping_analysis_v30() ) reject_tot = TRUE;
  if ( Stopper_analysis_v30 () ) reject_tot = TRUE;

//  txt2.Format( L"%s %s %s %s", FO_txt, ALU_txt, CRI_txt, STO_txt );
  txt2.Format( string_2927[lng], FO_txt, ALU_txt, CRI_txt, STO_txt );

  if ( reject_tot )
  {
    col2 = TXT_DARKRED;
    return 9600; // reject because of Aluminium crimping area
  }
  
  col2 = TXT_DARKGREEN;
  return 0; ///


  /*
  #ifdef NEWCAP_V21
  if ( shl_height>0 )
  {
    if ( Analyze_shoulder() ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -150, index+3 ); // insert -150 into 3-4-5 cat 
      #endif 
      return( 9400 ); // reject by shoulder analysys
    }
  }
  #endif
  */

  /*
  #if SERIAL_N==201500002 // Sanjing skip B/C points to neglect fins

  if ( Analyze_contour_fins() )
  {
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints_fins() ) return 9200; 

  #else // normal processing

  if ( Analyze_contour_new() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -115, index+3 ); // insert -115 into 3-4-5 cat 
    #endif
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -125, index+3 ); // insert -125 into 3-4-5 cat 
    #endif
    return 9200; // some problem on contour
  }

  #endif // SERIAL_N==201500002 // Sanjing skip B/C points to neglect fins
  */

  /*
  //txt1.Format( L"Cap GOOD" );
  txt1.Format( string_694[lng] );

  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, index+3 ); // insert 0 = GOOD into 3-4-5 cat 
  #endif

  // Everything's OK: GOOD
  col1 = TXT_DARKGREEN;
  col2 = TXT_DARKGREEN;

  //txt1.Format( L"Contour %d pts - %d us" , ptn, timer.elapsed_us() );
  //txt2.Format( L"-" );


  return 0; // completed successfully*/
}










int CTip_inspection::Process_image_tip_v30( BYTE* img_acq, UINT nimg )
{

  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  if ( contour_thr==0 )
  {
    col1 = 0;
    col2 = 0;
    txt1 = string_2510[lng];
    txt2 = L"";
    return 0;
  }

//  TRACE("contour_thr=%d - (%d,%d)-(%d,%d)\n", contour_thr, xmin, ymin, xmax, ymax );

  if ( Find_contour_tip_v30() ) // contour not found/validated
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -105, index+3 ); // insert -105 into 3-4-5 cat 
    #endif 


    //col1 = TXT_DARKRED;
    return 9000;
  }

  //col1 = TXT_DARKGREEN;
  return 0; //


/*  if ( Analyze_contour_v30() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -115, index+3 ); // insert -115 into 3-4-5 cat 
    #endif
    col1 = TXT_DARKRED;
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints_v30() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -125, index+3 ); // insert -125 into 3-4-5 cat 
    #endif
    col1 = TXT_DARKRED;
    //txt1.Format( string_698[lng] );
    return 9200; // some problem on contour
  }

  // Contour and keypoints OK, move on
  col1 = TXT_DARKGREEN;
//  txt1.Format( string_2562[lng], ptn, min_npt );
  txt1.Format( string_758[lng], ptn, ABL_txt, ABR_txt, BCL_txt, BCR_txt ); 

  reject_tot = FALSE;

  if ( Flipoff_analysis_v30 () ) reject_tot = TRUE;
  if ( Aluside_analysis_v30 () ) reject_tot = TRUE;
  if ( Crimping_analysis_v30() ) reject_tot = TRUE;
  if ( Stopper_analysis_v30 () ) reject_tot = TRUE;

//  txt2.Format( L"%s %s %s %s", FO_txt, ALU_txt, CRI_txt, STO_txt );
  txt2.Format( string_2927[lng], FO_txt, ALU_txt, CRI_txt, STO_txt );

  if ( reject_tot )
  {
    col2 = TXT_DARKRED;
    return 9600; // reject because of Aluminium crimping area
  }
  
  col2 = TXT_DARKGREEN;  

  return 0; ///*/


}








///////////////////////////////////////////////////////////////////////////////////////////////



int CTip_inspection::Flipoff_analysis_v30( void )
{
  if ( tl_fomaxerr==0 ) // tl_fomaxerr=0 => FLIPOFF SIDE ANALYSIS DISABLED
  {
    FO_txt = string_2920[lng];
    return 0;
  }

  int x, y;

  // COMPUTE AREA FOR SIDE FLIP-OFF ANALYSIS
  // Area color GREEN -  Attached TOP (right) to PC/PD left&right

  int sa_top    = ( pty[kpts[PC][LEFT]] + pty[kpts[PC][RIGHT]] - tl_fowidth ) / 2;
  int sa_bottom = sa_top + tl_fowidth;
  int sa_right  = ( ptx[kpts[PC][LEFT]] + ptx[kpts[PC][RIGHT]] ) / 2 + tl_fohgtofs;
  int sa_left   = sa_right - tl_foheight;
  int col       = COL_GREEN;

  if ( sa_top   <0 || sa_top   >dimy-1 ||     // sa_top    wrong coordinates     
       sa_bottom<0 || sa_bottom>dimy-1 ||     // sa_bottom wrong coordinates
       sa_left  <0 || sa_left  >dimx-1 ||     // sa_left   wrong coordinates
       sa_right <0 || sa_right >dimx-1 )
  {
    FO_txt = string_2925[lng];
    return 304; // wrong coordinates
  }

  if ( tl_drawovl )
  {
    ASSERT( sa_left <= sa_right );
    for ( x=sa_left ; x<=sa_right ; x+=4 )
    {
      DRAW_PT(index,x,sa_top   ,col); // 
      DRAW_PT(index,x,sa_bottom,col); // 
    }

    ASSERT( sa_top <= sa_bottom );
    for ( y=sa_top ; y<=sa_bottom ; y+=4 )
    {
      DRAW_PT(index,sa_left, y,col); // 
      DRAW_PT(index,sa_right,y,col); // 
    }
  }

  #undef  DST
  #define DST tl_folenght // 6
  #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
  #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

  int err   = 0; // total final error
  int err_a = 0; // absolute error
  int err_r = 0; // relative error

  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
    {
      // edge filter positive error: RED
      if ( FLT1(x,y)-tl_foedge>0 ) 
      {
        err_r += FLT1(x,y)-tl_foedge;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
      }

      // edge filter negative error: CYAN
      if ( FLT2(x,y)-tl_foedge>0 ) 
      {
        err_r += FLT2(x,y)-tl_foedge;
        //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN ); // 
      }

      // positive absolute error: MAGENTA
      if ( PT(x,y)-tl_fomax>0 ) 
      {
        err_a += PT(x,y)-tl_fomax>0;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }

      // negative absolute error: ORANGE
      if ( tl_fomin-PT(x,y)>0 ) 
      {
        err_a += tl_fomin-PT(x,y);
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
      }
    }
  }  

  // Compose final error
  // err_side /= 100; original
  err = err_r/100 + err_a;

  if ( err>tl_fomaxerr )
  {

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==0 && err>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	  //Camera B
	if ( index==1 && err<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	  //Camera C
	  if ( index==2 && err<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

    FO_txt.Format( string_2923[lng], err, tl_fomaxerr );
    return 99;  // return REJECT
  }

  FO_txt.Format( string_2922[lng], err, tl_fomaxerr );
  return 0; // return GOOD
}


int CTip_inspection::Aluside_analysis_v30( void )
{
  if ( tl_alumaxerr==0 ) // ANALYSIS DISABLED
  {
    ALU_txt = string_2920[lng];
    return 0;
  }

  int x, y;

  // COMPUTE AREA FOR ALUMINIUM SIDE ANALYSIS
  // Area color BLUE -  Attached TOP (right) to PC left&right

  int sa_top    = ( pty[kpts[PC][LEFT]] + pty[kpts[PC][RIGHT]] - tl_aluwidth ) / 2;
  int sa_bottom = sa_top + tl_aluwidth;
  int sa_right  = ( ptx[kpts[PC][LEFT]] + ptx[kpts[PC][RIGHT]] ) / 2 + tl_aluhgtofs;
  int sa_left   = sa_right - tl_aluheight;
  int col       = COL_CYAN;

  if ( sa_top   <0 || sa_top   >dimy-1 ||     // sa_top    wrong coordinates     
       sa_bottom<0 || sa_bottom>dimy-1 ||     // sa_bottom wrong coordinates
       sa_left  <0 || sa_left  >dimx-1 ||     // sa_left   wrong coordinates
       sa_right <0 || sa_right >dimx-1 )
  {
    ALU_txt = string_2925[lng];
    return 304; // wrong coordinates
  }

  if ( tl_drawovl )
  {
    ASSERT( sa_left <= sa_right );
    for ( x=sa_left ; x<=sa_right ; x+=4 )
    {
      DRAW_PT(index,x,sa_top   ,col); // 
      DRAW_PT(index,x,sa_bottom,col); // 
    }

    ASSERT( sa_top <= sa_bottom );
    for ( y=sa_top ; y<=sa_bottom ; y+=4 )
    {
      DRAW_PT(index,sa_left, y,col); // 
      DRAW_PT(index,sa_right,y,col); // 
    }
  }

  #undef  DST
  #define DST tl_alulenght // 6
  #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
  #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

  int err   = 0; // total final error
  int err_a = 0; // absolute error
  int err_r = 0; // relative error

  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
    {
      // edge filter positive error: RED
      if ( FLT1(x,y)-tl_aluedge>0 ) 
      {
        err_r += FLT1(x,y)-tl_aluedge;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
      }

      // edge filter negative error: CYAN
      if ( FLT2(x,y)-tl_aluedge>0 ) 
      {
        err_r += FLT2(x,y)-tl_aluedge;
        //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN ); // 
      }

      // positive absolute error: MAGENTA
      if ( PT(x,y)-tl_alumax>0 ) 
      {
        err_a += PT(x,y)-tl_alumax>0;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }

      // negative absolute error: ORANGE
      if ( tl_alumin-PT(x,y)>0 ) 
      {
        err_a += tl_alumin-PT(x,y);
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
      }
    }
  }  

  // Compose final error
  // err_side /= 100; original
  err = err_r/100 + err_a;

  if ( err>tl_alumaxerr )
  {

	//Pour added 20180927
#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if(index==0 && err>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( index==1 && err<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera C
	  if ( index==2 && err<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

    ALU_txt.Format( string_2923[lng], err, tl_alumaxerr );
    return 99;  // return REJECT
  }

  ALU_txt.Format( string_2922[lng], err, tl_alumaxerr );
  return 0; // return GOOD
}


int CTip_inspection::Crimping_analysis_v30( void )
{
  if ( tl_crimaxerr==0 ) // ANALYSIS DISABLED
  {
    CRI_txt = string_2920[lng];
    return 0;
  }

  int x, y;

  // COMPUTE AREA FOR CRIMPING ANALYSIS
  // Area color ORANGE -  Attached TOP (right) to PA/PA left&right

  int sa_top    = ( pty[kpts[PA][LEFT]] + pty[kpts[PA][RIGHT]] - tl_criwidth ) / 2;
  int sa_bottom = sa_top + tl_criwidth;
  int sa_right  = ( ptx[kpts[PA][LEFT]] + ptx[kpts[PA][RIGHT]] ) / 2 + tl_crihgtofs;
  int sa_left   = sa_right - tl_criheight;
  int col       = COL_ORANGE;

  if ( sa_top   <0 || sa_top   >dimy-1 ||     // sa_top    wrong coordinates     
       sa_bottom<0 || sa_bottom>dimy-1 ||     // sa_bottom wrong coordinates
       sa_left  <0 || sa_left  >dimx-1 ||     // sa_left   wrong coordinates
       sa_right <0 || sa_right >dimx-1 )
  {
    CRI_txt = string_2925[lng];
    return 304; // wrong coordinates
  }

  if ( tl_drawovl )
  {
    ASSERT( sa_left <= sa_right );
    for ( x=sa_left ; x<=sa_right ; x+=4 )
    {
      DRAW_PT(index,x,sa_top   ,col); // 
      DRAW_PT(index,x,sa_bottom,col); // 
    }

    ASSERT( sa_top <= sa_bottom );
    for ( y=sa_top ; y<=sa_bottom ; y+=4 )
    {
      DRAW_PT(index,sa_left, y,col); // 
      DRAW_PT(index,sa_right,y,col); // 
    }
  }

  #undef  DST
  #define DST tl_crilenght // 6
  #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
  #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

  int err   = 0; // total final error
  int err_a = 0; // absolute error
  int err_r = 0; // relative error

  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
    {
      // edge filter positive error: RED
      if ( FLT1(x,y)-tl_criedge>0 ) 
      {
        err_r += FLT1(x,y)-tl_criedge;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
      }

      // edge filter negative error: CYAN
      if ( FLT2(x,y)-tl_criedge>0 ) 
      {
        err_r += FLT2(x,y)-tl_criedge;
        //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN ); // 
      }

      // positive absolute error: MAGENTA
      if ( PT(x,y)-tl_crimax>0 ) 
      {
        err_a += PT(x,y)-tl_crimax>0;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }

      // negative absolute error: ORANGE
      if ( tl_crimin-PT(x,y)>0 ) 
      {
        err_a += tl_crimin-PT(x,y);
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
      }
    }
  }  

  // Compose final error
  // err_side /= 100; original
  err = err_r/100 + err_a;

  if ( err>tl_crimaxerr )
  {
    CRI_txt.Format( string_2923[lng], err, tl_crimaxerr );
    return 99;  // return REJECT
  }

  CRI_txt.Format( string_2922[lng], err, tl_crimaxerr );
  return 0; // return GOOD
}


int CTip_inspection::Stopper_analysis_v30( void )
{
  if ( tl_stomaxerr==0 ) // ANALYSIS DISABLED
  {
    STO_txt = string_2920[lng];
    return 0;
  }

  int x, y;

  // COMPUTE AREA FOR SSTOPPER ANALYSIS
  // Area color YELLOW -  Attached TOP (right) to PA/PA left&right

  int sa_top    = ( pty[kpts[PA][LEFT]] + pty[kpts[PA][RIGHT]] - tl_stowidth ) / 2;
  int sa_bottom = sa_top + tl_stowidth;
  int sa_right  = ( ptx[kpts[PA][LEFT]] + ptx[kpts[PA][RIGHT]] ) / 2 + tl_stohgtofs;
  int sa_left   = sa_right - tl_stoheight;
  int col       = COL_YELLOW;

  if ( sa_top   <0 || sa_top   >dimy-1 ||     // sa_top    wrong coordinates     
       sa_bottom<0 || sa_bottom>dimy-1 ||     // sa_bottom wrong coordinates
       sa_left  <0 || sa_left  >dimx-1 ||     // sa_left   wrong coordinates
       sa_right <0 || sa_right >dimx-1 )
  {
    STO_txt = string_2925[lng];
    return 304; // wrong coordinates
  }

  if ( tl_drawovl )
  {
    ASSERT( sa_left <= sa_right );
    for ( x=sa_left ; x<=sa_right ; x+=4 )
    {
      DRAW_PT(index,x,sa_top   ,col); // 
      DRAW_PT(index,x,sa_bottom,col); // 
    }

    ASSERT( sa_top <= sa_bottom );
    for ( y=sa_top ; y<=sa_bottom ; y+=4 )
    {
      DRAW_PT(index,sa_left, y,col); // 
      DRAW_PT(index,sa_right,y,col); // 
    }
  }

  #undef  DST
  #define DST tl_stolenght // 6
  #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
  #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

  int err   = 0; // total final error
  int err_a = 0; // absolute error
  int err_r = 0; // relative error

  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
    {
      // edge filter positive error: RED
      if ( FLT1(x,y)-tl_stoedge>0 ) 
      {
        err_r += FLT1(x,y)-tl_stoedge;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
      }

      // edge filter negative error: CYAN
      if ( FLT2(x,y)-tl_stoedge>0 ) 
      {
        err_r += FLT2(x,y)-tl_stoedge;
        //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN ); // 
      }

      // positive absolute error: MAGENTA
      if ( PT(x,y)-tl_stomax>0 ) 
      {
        err_a += PT(x,y)-tl_stomax>0;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }

      // negative absolute error: ORANGE
      if ( tl_stomin-PT(x,y)>0 ) 
      {
        err_a += tl_stomin-PT(x,y);
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
      }
    }
  }  

  // Compose final error
  // err_side /= 100; original
  err = err_r/100 + err_a;

  if ( err>tl_stomaxerr )
  {
	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if(index==0 && err>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera B
	if ( index==1 && err<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	//Camera C
	  if ( index==2 && err<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

    STO_txt.Format( string_2923[lng], err, tl_stomaxerr );
    return 99;  // return REJECT
  }

  STO_txt.Format( string_2922[lng], err, tl_stomaxerr );
  return 0; // return GOOD
}





///////////////////////////////////////////////////////////////////////////////////////////////











//#define FLT(x,y) PT(x,y-3)+PT(x,y-2)+PT(x,y-1)+2*PT(x,y)-2*PT(x,y+1)-PT(x,y+2)-PT(x,y+3)-PT(x,y+4)
#define FLT(x,y) PT(x,y-3)+PT(x,y-2)+PT(x,y-1)+PT(x,y)-PT(x,y+1)-PT(x,y+2)-PT(x,y+3)-PT(x,y+4)

// function to search edge moving downwards starting from ymin down to ymax (non valid)
// sembra ottima: differenziale ed in grado di trovare (con soglia 60) anche bordi davvero minimi
// attenzione a tenere ben pulito e uniforme lo fondo!
int CTip_inspection::Find_edge_downward( int x )
{
  int y = ymin;

  while ( FLT(x,y)<contour_thr && y<ymax-10 ) y++;
  //TRACE("above th %d,%d,%d\n", x,y,FLT(x,y));

  if ( y<ymax ) // if not out, search max position
  {

    while ( FLT(x,y+1)>FLT(x,y) && y<ymax-10 ) y++;
    
    if ( y<ymax-10 ) // max is validated
    {
      y++;
      //TRACE("max found %d,%d,%d\n", x,y,FLT(x,y));

//      #ifdef SAVE_FULL_PROC
      #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
        if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_MAGENTA;
      #endif
      //TRACE("edge found %d,%d,%d\n", x,y,FLT(x,y));
      return y;
    }
  }

  return 0;
}




int CTip_inspection::Find_contour_toplight( int y0 )
{

  ASSERT( y0>ymin && y0<ymax );

  int i;
  int x, y;

  int   ptnt;
  int   ptxt[NPTCNT/2];
  int   ptyt[NPTCNT/2];

  BOOL  reject = FALSE;


  tl_al.x = 0;
  tl_al.y = 0;
  tl_ar.x = 0;
  tl_ar.y = 0;

  ASSERT( sizeof(LONG)==4 );

  /*
  for ( x=xmin ; x<=xmax ; x++ )
  {
    PT(x,ymin) = 0;
    PT(x,ymax) = 0;
  }

  for ( y=ymin ; y<=ymax ;y++ )
  {
    PT(xmin,y) = 0;
    PT(xmax,y) = 0;
  }
  */

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );

  //ZeroMemory( &cntrbbox, sizeof(cntrbbox) );

/* ***  cntrbbox.left   = xmax;
  cntrbbox.right  = xmin;
  cntrbbox.top    = ymax;
  cntrbbox.bottom = ymin; */

  //ZeroMemory( kpts, sizeof(kpts) );


  // find first point
  x = xmax-1;
  y = y0;
  while ( PT(x,y)<tl_contour && x>xmin ) x--;

  if ( x<=xmin )
  {
    col1 = TXT_DARKRED;
    txt1.Format( string_756[lng], ptn, min_npt );

    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -100, index ); // insert -100 into result file
    #endif

    return 99; // failed to find first point, return error
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptnt    = 0;
  ptxt[0] = x;
  ptyt[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

//  #undef PTD
  #define PTDT(i) PT(ptxt[ptnt]+ofsx[(i)&7],ptyt[ptnt]+ofsy[(i)&7])
  #define PTET(i) PT(ptx [ptn ]+ofsx[(i)&7],pty [ptn ]+ofsy[(i)&7])

  
  i = 0; // start moving upward - CCW search
  while ( ptnt<NPTCNT/2-1 && ptxt[ptnt]>xmin && ptxt[ptnt]<xmax && ptyt[ptnt]>ymin && ptyt[ptnt]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i2;
    while ( i>=i1 && !( PTDT(i+1)<tl_contour && PTDT(i)>=tl_contour ) ) i--;
    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptxt[ptnt+1] = ptxt[ptnt]+ofsx[i&7];
    ptyt[ptnt+1] = ptyt[ptnt]+ofsy[i&7];

    ptnt++;

    if ( ptnt>=2*AMPATN )
    {
      /*
      #define DX ptx[i+AMPATN]-ptx[i-AMPATN]
      #define DY pty[i+AMPATN]-pty[i-AMPATN]
      #define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]
      AMPATN = 6
      DIMATN = 4*AMPATN+1 = 25
      */

      #define DXT (ptxt[ptnt]-ptxt[ptnt-2*AMPATN])
      #define DYT (ptyt[ptnt]-ptyt[ptnt-2*AMPATN])

      int atan = ATNLUT[DXT+AMPATN*2][DYT+AMPATN*2];

      //TRACE("LEFT: %d,%d,%d,%d\n", ptnt, ptxt[ptnt], ptyt[ptnt], atan );

      if ( atan<4500 && atan>0 )
      {
        tl_al.x = ptxt[ptnt];
        tl_al.y = ptyt[ptnt];
        break;
      }

      //TRACE("LEFT: %d,%d,%d\n",    ptnt, ptxt[ptnt], ptyt[ptnt]             );
      //else           TRACE("LEFT: %d,%d,%d,%d\n", ptnt, ptxt[ptnt], ptyt[ptnt], ATAN(ptnt) );
    }

  }

  if ( !tl_al.x || !tl_al.y )
  {
    col1 = TXT_DARKRED;
    txt1.Format( string_764[lng] );

    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -110, index ); // insert -110 into result file
    #endif

    return 109; // failed to find first point, return error
  }




/*
  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  if ( i<=imin ) return 1009; // unable to find point - return error
  kpts[PA][LEFT] = i;
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) return 1019; // unable to find point - return error
  kpts[PB][LEFT] = i;
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;
  */


  
  // copy inverted
  for ( i=0 ; i<ptnt ; i++ )
  {
    ptx[i] = ptxt[ptnt-i-1];
    pty[i] = ptyt[ptnt-i-1];
  }
  ptmid = ptnt; // index of middle/center point of contour
  ptn=ptnt-1;
  

/*  #ifdef SAVE_FULL_PROC
    for ( i=0 ; i<ptnt ; i++ )
    {
      image_dsp[index]->ptr_fulldsp[ptxt[i]+ptyt[i]*dimx] = 0xFFFF00FF;
      TRACE("%d,%d,%d\n", i, ptxt[i], ptyt[i] );
    }
  #endif  */
/*
  ptn    = 0;
  ptx[0] = ptxt[0];
  pty[0] = ptyt[0];
 */

  
  i = 4; // start moving downward - CW search
  while ( ptn<NPTCNT-1 && ptx[ptn]>xmin && ptx[ptn]<xmax && pty[ptn]>ymin && pty[ptn]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i1;
    while ( i<=i2 && !( PTD(i+7)<tl_contour && PTD(i)>=tl_contour ) ) i++;
    //if ( i>i2 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];


    //if ( ptnt>=2*AMPATN )
    //{
      /*
      #define DX ptx[i+AMPATN]-ptx[i-AMPATN]
      #define DY pty[i+AMPATN]-pty[i-AMPATN]
      #define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]
      AMPATN = 6
      DIMATN = 4*AMPATN+1 = 25
      */

    ptn++;


    if ( ptn>=2*AMPATN )
    {

      #undef DXT
      #undef DYT
      #define DXT (ptx[ptn]-ptx[ptn-2*AMPATN])
      #define DYT (pty[ptn]-pty[ptn-2*AMPATN])

      int atan = ATNLUT[DXT+AMPATN*2][DYT+AMPATN*2];

      //TRACE("RIGHT: %d,%d,%d,%d\n", ptn, ptx[ptn], pty[ptn], atan );

      if ( atan>13500 && atan<18000 )
      {
        tl_ar.x = ptx[ptn];
        tl_ar.y = pty[ptn];
        break;
      } 

    }
      //TRACE("LEFT: %d,%d,%d\n",    ptnt, ptxt[ptnt], ptyt[ptnt]             );
      //else           TRACE("LEFT: %d,%d,%d,%d\n", ptnt, ptxt[ptnt], ptyt[ptnt], ATAN(ptnt) );
    //}


  }


  if ( !tl_ar.x || !tl_ar.y )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -120, index ); // insert -120 into result file
    #endif

    col1 = TXT_DARKRED;
    txt1.Format( string_764[lng] );
    return 119; // failed to find right point, return error
  }

 

  /*
  #ifdef SAVE_FULL_PROC
    for ( i=0 ; i<ptn ; i++ )
    {
      image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = 0xFFFF00FF;
      TRACE("%d,%d,%d\n", i, ptx[i], pty[i] );
    }
  #endif
  */


  if ( ptn<min_npt )
  {
    col1 = TXT_DARKRED;
    txt1.Format( string_760[lng], ptn, min_npt );
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -130, index ); // insert -130 into result file
    #endif
    return 129;
  } 

  if ( tl_drawovl )
  {
    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK); // first pass to blank wider trace
  }

  for ( i=0 ; i<ptn ; i++ )
//  for ( i=0 ; i<ptmid ; i++ )
  {
/* ***    if ( ptx[i]>cntrbbox.right  ) cntrbbox.right  = ptx[i];
    if ( ptx[i]<cntrbbox.left   ) cntrbbox.left   = ptx[i];
    if ( pty[i]>cntrbbox.bottom ) cntrbbox.bottom = pty[i];
    if ( pty[i]<cntrbbox.top    ) cntrbbox.top    = pty[i]; */

    if ( tl_drawovl ) DRAW_PT (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
  }

  // DRAW FIRST AND LAST POINTS OF CONTOUR
  if ( tl_drawovl )
  {
    DRAW_SQUARE( index, tl_al.x, tl_al.y, COL_BLUE );
    DRAW_SQUARE( index, tl_ar.x, tl_ar.y, COL_BLUE );
  }

  // COMPUTE AREA FOR SIDE FLIP-OFF ANALYSIS
  if ( tl_fomaxerr>0 )
  {

    int err_side  = 0;
    int err_sidea = 0; // absolute error
    int err_sider = 0; // relative error
    int sa_top    = ( tl_al.y + tl_ar.y - tl_fowidth ) / 2;
    int sa_bottom = ( tl_al.y + tl_ar.y + tl_fowidth ) / 2;
    int sa_right  = ( tl_al.x + tl_ar.x ) / 2 - tl_fohgtofs;
    int sa_left   = sa_right - tl_foheight;

    if ( sa_top   <0 || sa_top   >dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -301, index ); // insert -301 into result file
      #endif
      return 301; // sa_top    wrong coordinates
    }

    if ( sa_bottom<0 || sa_bottom>dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -302, index ); // insert -302 into result file
      #endif
      return 302; // sa_bottom wrong coordinates
    }

    if ( sa_left  <0 || sa_left  >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -303, index ); // insert -303 into result file
      #endif
      return 303; // sa_left   wrong coordinates
    }

    if ( sa_right <0 || sa_right >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -304, index ); // insert -304 into result file
      #endif
      return 304; // sa_right  wrong coordinates
    }


    if ( tl_drawovl )
    {
      ASSERT( sa_left <= sa_right );
      for ( x=sa_left ; x<=sa_right ; x+=4 )
      {
        DRAW_PT(index,x,sa_top   ,COL_YELLOW); // 
        DRAW_PT(index,x,sa_bottom,COL_YELLOW); // 
      }

      ASSERT( sa_top <= sa_bottom );
      for ( y=sa_top ; y<=sa_bottom ; y+=4 )
      {
        DRAW_PT(index,sa_left, y,COL_YELLOW); // 
        DRAW_PT(index,sa_right,y,COL_YELLOW); // 
      }
    }

/*  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top ; y<=sa_bottom ; y++ )
    {
      if ( tl_alumin-PT(x,y)>0 ) 
      {
        err_side += tl_alumin-PT(x,y);
        DRAW_PT( index, x, y, COL_RED ); // 
      }

      if ( PT(x,y)-tl_alumax>0 ) 
      {
        err_side += PT(x,y)-tl_alumax;
        DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }
    }
  }  */

	// add smooth filter by yang 20200611
    #define  AVG(x,y) (PT(x-1,y-1)+ PT(x,y-1)+PT(x+1,y-1)+PT(x-1,y)+ PT(x,y)+PT(x+1,y)+PT(x-1,y+1)+ PT(x,y+1)+PT(x+1,y+1))/9
	// add end by yang 20200611

    #undef  DST
    #define DST tl_folenght // 6
    //#define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
    //#define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))
	#define FLT1(x,y) (AVG(x-2,y-DST)+AVG(x,y-DST)+AVG(x+2,y-DST)-AVG(x-2,y+DST)-AVG(x,y+DST)-AVG(x+2,y+DST))
    #define FLT2(x,y) (AVG(x-2,y+DST)+AVG(x,y+DST)+AVG(x+2,y+DST)-AVG(x-2,y-DST)-AVG(x,y-DST)-AVG(x+2,y-DST))

    for ( x=sa_left ; x<=sa_right ; x++ )
    {
      for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
      {
        // edge filter positive error: RED
        if ( FLT1(x,y)-tl_foedge>0 ) 
        {
          err_sider += FLT1(x,y)-tl_foedge;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
        }

        // edge filter negative error: CYAN
        if ( FLT2(x,y)-tl_foedge>0 ) 
        {
          err_sider += FLT2(x,y)-tl_foedge;
          //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN ); // 
        }

        // positive absolute error: MAGENTA
        if ( PT(x,y)-tl_fomax>0 ) 
        {
          err_sidea += PT(x,y)-tl_fomax>0;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
        }

        // negative absolute error: ORANGE
        if ( tl_fomin-PT(x,y)>0 ) 
        {
          err_sidea += tl_fomin-PT(x,y);
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
        }
      }
    }  

    // err_side /= 100; original
    err_side = err_sider/100 + err_sidea;

    #if SERIAL_N==201400016 // TEST 20150908 FOR XIAN BOSEN
      err_side = err_sider + err_sidea;
    #endif
    

    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( err_side, 7+index ); // insert flip-off side error into result file cat 7-8-9
    #endif

    if ( err_side>tl_fomaxerr )
    {
	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err_side<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if(index==0 && err_side>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	  //Camera B
	if ( index==1 && err_side<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err_side>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	  //Camera C
	  if ( index==2 && err_side<tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err_side>=tl_fomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

      reject = TRUE;
      col1   = TXT_DARKRED;
      txt1.Format( string_1450[lng], err_side, tl_fomaxerr );
    }
    else
    {
      txt1.Format( string_1452[lng], err_side, tl_fomaxerr );
    }

  }
  else // tl_alumaxerr=0 => ALUMINIUM SIDE ANALYSIS DISABLED
  {
    col1 = TXT_DARKBLUE;
    txt1.Format( string_1454[lng] );
  }


  // COMPUTE AREA FOR SIDE ALUMINIUM CAP ANALYSIS
  if ( tl_alumaxerr>0 )
  {

    int err_side  = 0;
    int sa_top    = ( tl_al.y + tl_ar.y - tl_aluwidth ) / 2;
    int sa_bottom = ( tl_al.y + tl_ar.y + tl_aluwidth ) / 2;
    int sa_right  = ( tl_al.x + tl_ar.x ) / 2 - tl_aluhgtofs;
    int sa_left   = sa_right - tl_aluheight;

    if ( sa_top   <0 || sa_top   >dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -201, index ); // insert -201 into result file
      #endif
      return 201; // sa_top    wrong coordinates
    }

    if ( sa_bottom<0 || sa_bottom>dimy-1 )
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -202, index ); // insert -202 into result file
      #endif
      return 202; // sa_bottom wrong coordinates
    }
      
    if ( sa_left  <0 || sa_left  >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -203, index ); // insert -203 into result file
      #endif
      return 203; // sa_left   wrong coordinates
    }

    if ( sa_right <0 || sa_right >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -204, index ); // insert -204 into result file
      #endif
      return 204; // sa_right  wrong coordinates
    }

    if ( tl_drawovl ) 
    {
      ASSERT( sa_left <= sa_right );
      for ( x=sa_left ; x<=sa_right ; x+=4 )
      {
        DRAW_PT(index,x,sa_top   ,COL_ORANGE); // 
        DRAW_PT(index,x,sa_bottom,COL_ORANGE); // 
      }

      ASSERT( sa_top <= sa_bottom );
      for ( y=sa_top ; y<=sa_bottom ; y+=4 )
      {
        DRAW_PT(index,sa_left, y,COL_ORANGE); // 
        DRAW_PT(index,sa_right,y,COL_ORANGE); // 
      }
    }

/*  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top ; y<=sa_bottom ; y++ )
    {
      if ( tl_alumin-PT(x,y)>0 ) 
      {
        err_side += tl_alumin-PT(x,y);
        DRAW_PT( index, x, y, COL_RED ); // 
      }

      if ( PT(x,y)-tl_alumax>0 ) 
      {
        err_side += PT(x,y)-tl_alumax;
        DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }
    }
  }  */

//    int err_side  = 0;
    int err_sider = 0; // rel
    int err_sidea = 0; // abs


    #undef  DST
    #define DST tl_alulenght // 6
    //#define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST)) //Pour marked on 20200611
    //#define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST)) //Pour marked on 20200611
	 
    for ( x=sa_left ; x<=sa_right ; x++ )
    {
      for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
      {
        // edge filter positive error: RED
        if ( FLT1(x,y)-tl_aluedge>0 ) 
        {
          err_sider += FLT1(x,y)-tl_aluedge;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
        }

        // edge filter negative error: CYAN
        if ( FLT2(x,y)-tl_aluedge>0 ) 
        {
          err_sider += FLT2(x,y)-tl_aluedge;
          //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN /*COL_MAGENTA*/ ); // 
        }

        // positive absolute error: MAGENTA
        if ( PT(x,y)-tl_alumax>0 ) 
        {
          err_sidea += PT(x,y)-tl_alumax>0;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
        }

        // negative absolute error: ORANGE
        if ( tl_alumin-PT(x,y)>0 ) 
        {
          err_sidea += tl_alumin-PT(x,y);
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
        }
      }
    }  

//    err_side /= 100;
    #ifdef NEWCAP_ORALCAP
      err_side = err_sider + err_sidea/10; // 100x bigger error for filter error, recompiled for LUNAN after Victor test 20150526
    #else // old, standard evaluation
      err_side = err_sider/100 + err_sidea/100; // 100x bigger error for abs error, recompiled for 201400017 after KONRUS
    #endif

    #if SERIAL_N==201400016 // TEST 20150908 FOR XIAN BOSEN
      err_side = err_sider + err_sidea;
    #endif


    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( err_side, 10+index ); // insert aluminium side error into result file cat 10-11-12
    #endif

    if ( err_side>tl_alumaxerr )
    {

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err_side<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of TIPCAP reject to tray 1 FALSE
	}
	  if(index==0 && err_side>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of TIPCAP reject to tray 1 TRUE
	}
	  //Camera B
	if ( index==1 && err_side<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err_side>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	  //Camera C
	  if ( index==2 && err_side<tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err_side>=tl_alumaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

      reject = TRUE;
      col2   = TXT_DARKRED;
      txt2.Format( string_1460[lng], err_side, tl_alumaxerr );
    }
    else
    {
      txt2.Format( string_1462[lng], err_side, tl_alumaxerr );
    }

  }
  else // tl_alumaxerr=0 => ALUMINIUM SIDE ANALYSIS DISABLED
  {
    col2 = TXT_DARKBLUE;
    txt2.Format( string_1464[lng] );
  }

  	//Pour added 20180709
#ifdef TEST_QILU

	  // COMPUTE AREA FOR CRIMPING CAP ANALYSIS
  if ( tl_crimaxerr>0 )
  {

    int err_side  = 0;
    int sa_top    = ( tl_al.y + tl_ar.y - tl_criwidth ) / 2;
    int sa_bottom = ( tl_al.y + tl_ar.y + tl_criwidth ) / 2;
    int sa_right  = ( tl_al.x + tl_ar.x ) / 2 - tl_crihgtofs;
    int sa_left   = sa_right - tl_criheight;

    if ( sa_top   <0 || sa_top   >dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -201, index ); // insert -201 into result file
      #endif
      return 201; // sa_top    wrong coordinates
    }

    if ( sa_bottom<0 || sa_bottom>dimy-1 )
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -202, index ); // insert -202 into result file
      #endif
      return 202; // sa_bottom wrong coordinates
    }
      
    if ( sa_left  <0 || sa_left  >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -203, index ); // insert -203 into result file
      #endif
      return 203; // sa_left   wrong coordinates
    }

    if ( sa_right <0 || sa_right >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -204, index ); // insert -204 into result file
      #endif
      return 204; // sa_right  wrong coordinates
    }

    if ( tl_drawovl ) 
    {
      ASSERT( sa_left <= sa_right );
      for ( x=sa_left ; x<=sa_right ; x+=4 )
      {
        DRAW_PT(index,x,sa_top   ,COL_ORANGE); // 
        DRAW_PT(index,x,sa_bottom,COL_ORANGE); // 
      }

      ASSERT( sa_top <= sa_bottom );
      for ( y=sa_top ; y<=sa_bottom ; y+=4 )
      {
        DRAW_PT(index,sa_left, y,COL_ORANGE); // 
        DRAW_PT(index,sa_right,y,COL_ORANGE); // 
      }
    }

/*  for ( x=sa_left ; x<=sa_right ; x++ )
  {
    for ( y=sa_top ; y<=sa_bottom ; y++ )
    {
      if ( tl_alumin-PT(x,y)>0 ) 
      {
        err_side += tl_alumin-PT(x,y);
        DRAW_PT( index, x, y, COL_RED ); // 
      }

      if ( PT(x,y)-tl_alumax>0 ) 
      {
        err_side += PT(x,y)-tl_alumax;
        DRAW_PT( index, x, y, COL_MAGENTA ); // 
      }
    }
  }  */

//    int err_side  = 0;
    int err_sider = 0; // rel
    int err_sidea = 0; // abs


    #undef  DST
    #define DST tl_crilenght // 6
    #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
    #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

    for ( x=sa_left ; x<=sa_right ; x++ )
    {
      for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
      {
        // edge filter positive error: RED
        if ( FLT1(x,y)-tl_criedge>0 ) 
        {
          err_sider += FLT1(x,y)-tl_criedge;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
        }

        // edge filter negative error: CYAN
        if ( FLT2(x,y)-tl_criedge>0 ) 
        {
          err_sider += FLT2(x,y)-tl_criedge;
          //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN /*COL_MAGENTA*/ ); // 
        }

        // positive absolute error: MAGENTA
        if ( PT(x,y)-tl_crimax>0 ) 
        {
          err_sidea += PT(x,y)-tl_crimax>0;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
        }

        // negative absolute error: ORANGE
        if ( tl_crimin-PT(x,y)>0 ) 
        {
          err_sidea += tl_crimin-PT(x,y);
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
        }
      }
    }  

//    err_side /= 100;
    #ifdef NEWCAP_ORALCAP
      err_side = err_sider + err_sidea/10; // 100x bigger error for filter error, recompiled for LUNAN after Victor test 20150526
    #else // old, standard evaluation
      //err_side = err_sider/100 + err_sidea/100; // 100x bigger error for abs error, recompiled for 201400017 after KONRUS
	  	//err_side = err_sider/10 + err_sidea/10; // 10x bigger error for abs error, recompiled for 201800001 after XINMA on 20190428
	err_side = err_sider + err_sidea; // 1x bigger error for abs error, recompiled for 201800001 after XINMA on 20190428

    #endif


    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( err_side, 10+index ); // insert aluminium side error into result file cat 10-11-12
    #endif

    if ( err_side>tl_crimaxerr )
    {
      reject = TRUE;
      col3   = TXT_DARKRED;
      txt3.Format( string_1460[lng], err_side, tl_crimaxerr );
    }
    else
    {
      txt3.Format( string_1462[lng], err_side, tl_crimaxerr );
    }

  }
  else // tl_alumaxerr=0 => ALUMINIUM SIDE ANALYSIS DISABLED
  {
    col3 = TXT_DARKBLUE;
    txt3.Format( string_1464[lng] );
  }


#endif
	//Pour added 20180709

  #ifdef NEWCAP_V21

  // COMPUTE AREA FOR SIDE STOPPER ANALYSIS
  CString txt;
  if ( tl_stomaxerr>0 )
  {
    int err_side  = 0;
    int err_sider = 0; // rel
    int err_sidea = 0; // abs
    int sa_top    = ( tl_al.y + tl_ar.y - tl_stowidth ) / 2;
    int sa_bottom = ( tl_al.y + tl_ar.y + tl_stowidth ) / 2;
    int sa_right  = ( tl_al.x + tl_ar.x ) / 2 - tl_stohgtofs;
    int sa_left   = sa_right - tl_stoheight;

    if ( sa_top   <0 || sa_top   >dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -401, index ); // insert -401 into result file
      #endif
      return 401; // sa_top    wrong coordinates
    }

    if ( sa_bottom<0 || sa_bottom>dimy-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -402, index ); // insert -402 into result file
      #endif
      return 402; // sa_bottom wrong coordinates
    }

    if ( sa_left  <0 || sa_left  >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -403, index ); // insert -403 into result file
      #endif
      return 403; // sa_left   wrong coordinates
    }

    if ( sa_right <0 || sa_right >dimx-1 ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -404, index ); // insert -404 into result file
      #endif
      return 404; // sa_right  wrong coordinates
    }

    if ( tl_drawovl ) 
    {
      ASSERT( sa_left <= sa_right );
      for ( x=sa_left ; x<=sa_right ; x+=4 )
      {
        DRAW_PT(index,x,sa_top   ,COL_PURPLE); // 
        DRAW_PT(index,x,sa_bottom,COL_PURPLE); // 
      }

      ASSERT( sa_top <= sa_bottom );
      for ( y=sa_top ; y<=sa_bottom ; y+=4 )
      {
        DRAW_PT(index,sa_left, y,COL_PURPLE); // 
        DRAW_PT(index,sa_right,y,COL_PURPLE); // 
      }
    }

    #undef  DST
    #define DST tl_stolenght // 6
    #define FLT1(x,y) (PT(x-2,y-DST)+PT(x,y-DST)+PT(x+2,y-DST)-PT(x-2,y+DST)-PT(x,y+DST)-PT(x+2,y+DST))
    #define FLT2(x,y) (PT(x-2,y+DST)+PT(x,y+DST)+PT(x+2,y+DST)-PT(x-2,y-DST)-PT(x,y-DST)-PT(x+2,y-DST))

    for ( x=sa_left ; x<=sa_right ; x++ )
    {
      for ( y=sa_top+DST ; y<=sa_bottom-DST ; y++ )
      {
        // edge filter positive error: RED
        if ( FLT1(x,y)-tl_stoedge>0 ) 
        {
          err_sider += FLT1(x,y)-tl_stoedge;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 
        }

        // edge filter negative error: CYAN
        if ( FLT2(x,y)-tl_stoedge>0 ) 
        {
          err_sider += FLT2(x,y)-tl_stoedge;
          //err_side += PT(x,y+DST)-PT(x,y-DST)-tl_alumax;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_CYAN /*COL_MAGENTA*/ ); // 
        }

        // positive absolute error: MAGENTA
        if ( PT(x,y)-tl_stomax>0 ) 
        {
          err_sidea += PT(x,y)-tl_stomax>0;
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_MAGENTA ); // 
        }

        // negative absolute error: ORANGE
        if ( tl_stomin-PT(x,y)>0 ) 
        {
          err_sidea += tl_stomin-PT(x,y);
          if ( tl_drawovl ) DRAW_PT( index, x, y, COL_ORANGE ); // 
        }
      }
    }  

//    err_side /= 100;
//    err_side = err_sider/100 + err_sidea/100;
//    err_side = err_sider/100 + err_sidea/10;
    err_side = err_sider/100 + err_sidea; // 100x bigger error for abs error, recompiled for 201400017 after KONRUS

    #if SERIAL_N==201400016 // TEST 20150908 FOR XIAN BOSEN
      err_side = err_sider + err_sidea;
    #endif

     #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( err_side, 13+index ); // insert stopper side error into result file cat 13-14-15
    #endif

    if ( err_side>tl_stomaxerr )
    {

	//Pour added 20180927
	#ifdef REJECTION_SEPARATION
	//Camera A
	if ( index==0 && err_side<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if(index==0 && err_side>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_1", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	 //Camera B
	if ( index==1 && err_side<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==1 && err_side>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_2", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	 //Camera C
	  if ( index==2 && err_side<tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", FALSE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 FALSE
	}
	  if( index==2 && err_side>=tl_stomaxerr2)
	{
	  ADS_write_value(".F_ENABLE_TIPCAP_3", TRUE, CTYPE_BOOL8); // Force enable of BOTTOMA reject to tray 1 TRUE
	}
	#endif
	//Pour added 20180927

      reject = TRUE;
      col2   = TXT_DARKRED;
      txt.Format( string_1470[lng], err_side, tl_stomaxerr );
    }
    else
    {
      txt.Format( string_1472[lng], err_side, tl_stomaxerr );
    }

  }
  else // tl_stomaxerr=0 => ALUMINIUM SIDE ANALYSIS DISABLED
  {
    //col2 = TXT_DARKBLUE;
    txt.Format( string_1474[lng] );
  }
  txt2.Append( L" - " );
  txt2.Append( txt );

  #endif

  #ifdef TEST_BOSEN
    if ( Analyze_crimping() )
    {
      reject = TRUE;
    }
  #endif




  #ifdef POWDER_STOPPER // id defined, test powder on rubber stopper
    if ( pwdrstpr_maxerr>0 ) // check points for powder inside inspection area
    {
      if ( Analyze_powder_stopper() ) reject = TRUE;
    }
    else // disabled
    {
      txt3 = string_1476[lng];
      col3 = TXT_BLACK;
    }

  #endif


  if ( reject )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( 1, index ); // insert 1 = REJECTED into 0-1-2 cat 
    #endif
    return 34999; // some reject during analysis
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp )
    {
      for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    }
  #endif

  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, index ); // insert 0 = GOOD into 0-1-2 cat 
  #endif

  return 0; // ok, return 0
}




/*

int CTip_inspection::Find_contour_newcap( int y0 )
{

  ASSERT( y0>ymin && y0<ymax );

  int i;
  int x, y;

  int   ptnt;
  int   ptxt[NPTCNT/2];
  int   ptyt[NPTCNT/2];

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );

  //ZeroMemory( &cntrbbox, sizeof(cntrbbox) );

  cntrbbox.left   = xmax;
  cntrbbox.right  = xmin;
  cntrbbox.top    = ymax;
  cntrbbox.bottom = ymin;

  ZeroMemory( kpts, sizeof(kpts) );


  // find first point
  x = xmax-1;
  y = y0;
  while ( PT(x,y)<contour_thr && x>xmin ) x--;

  if ( x<=xmin )
  {
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

  #ifdef SAVE_FULL_PROC
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptn    = 0;
  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

  #define PTD(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
 // #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
  i = 0; // start moving upward - CCW search
  while ( ptn<NPTCNT/2-1 && ptx[ptn]>xmin && ptx[ptn]<xmax && pty[ptn]>ymin && pty[ptn]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i2;
    while ( i>=i1 && !( PTD(i+1)<contour_thr && PTD(i)>=contour_thr ) ) i--;
    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;

    if ( ptn>50 && ptx[ptn]>ptx[0]-2 && ptx[ptn]<ptx[0]+2 && pty[ptn]>pty[0]-2 && pty[ptn]<pty[0]+2 ) break;
  }




  if ( ptn<min_npt )
  {
    txt1.Format( string_760[lng], ptn, min_npt );
    return 109;
  } 

  for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK); // first pass to blank wider trace

  for ( i=0 ; i<ptn ; i++ )
  {
    if ( ptx[i]>cntrbbox.right  ) cntrbbox.right  = ptx[i];
    if ( ptx[i]<cntrbbox.left   ) cntrbbox.left   = ptx[i];
    if ( pty[i]>cntrbbox.bottom ) cntrbbox.bottom = pty[i];
    if ( pty[i]<cntrbbox.top    ) cntrbbox.top    = pty[i];

    DRAW_PT (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
  }

  //TRACE("Bounding box: (%d,%d)-(%d,%d)\n", cntrbbox.left, cntrbbox.top, cntrbbox.right, cntrbbox.bottom );

  #ifdef SAVE_FULL_PROC
    if ( image_dsp[index]->ptr_fulldsp )
    {
      for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    }
  #endif

  return 0;
}


*/

#undef PTE



/*
#define PTD(i)  PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

#define PTD2(i) PT(ptx2[ptn2]+ofsx[(i)&7],pty2[ptn2]+ofsy[(i)&7])
*/
//#undef PT


//#ifdef NEW_TIP_V30
#ifdef WORKER_THREAD  // if defined uses worker threads to speed-up processing

#define PTEN(i) PTN(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
#define PTXT(x,y) img_ptr[x+(y)*(dimx)]

int CTip_inspection::PTN( int x, int y )
{
  int val = 0;

//  val = img[x+(y)*camera[index].width];

/*  val = (  PT(x-2,y-2) +  4*PT(x-2,y-1) +  7*PT(x-2,y) +  4*PT(x-2,y+1) +   PT(x-2,y+2) +
         4*PT(x-2,y-2) + 16*PT(x-2,y-1) + 26*PT(x-2,y) + 16*PT(x-2,y+1) + 4*PT(x-2,y+2) +
         7*PT(x-2,y-2) + 26*PT(x-2,y-1) + 41*PT(x-2,y) + 26*PT(x-2,y+1) + 7*PT(x-2,y+2) +
         4*PT(x-2,y-2) + 16*PT(x-2,y-1) + 26*PT(x-2,y) + 16*PT(x-2,y+1) + 4*PT(x-2,y+2) +
           PT(x-2,y-2) +  4*PT(x-2,y-1) +  7*PT(x-2,y) +  4*PT(x-2,y+1) +   PT(x-2,y+2) ) / 273; */

  val = (  PTXT(x-2,y-2) +  4*PTXT(x-2,y-1) +  7*PTXT(x-2,y) +  4*PTXT(x-2,y+1) +   PTXT(x-2,y+2) +
         4*PTXT(x-2,y-2) + 16*PTXT(x-2,y-1) + 26*PTXT(x-2,y) + 16*PTXT(x-2,y+1) + 4*PTXT(x-2,y+2) +
         7*PTXT(x-2,y-2) + 26*PTXT(x-2,y-1) + 41*PTXT(x-2,y) + 26*PTXT(x-2,y+1) + 7*PTXT(x-2,y+2) +
         4*PTXT(x-2,y-2) + 16*PTXT(x-2,y-1) + 26*PTXT(x-2,y) + 16*PTXT(x-2,y+1) + 4*PTXT(x-2,y+2) +
           PTXT(x-2,y-2) +  4*PTXT(x-2,y-1) +  7*PTXT(x-2,y) +  4*PTXT(x-2,y+1) +   PTXT(x-2,y+2) ) / 273;

  return( val );
}


int CTip_inspection::Find_contour_cw( void )
{
  int i;
  int x, y;

 // ZeroMemory( kpts, sizeof(kpts) );
  ptn    = 0;

  if ( !img_ptr ) return 99;

  int ycntr = (ymin+ymax)/2;

  // find first point
  x = xmin; //xmax-1;
  y = ymin;

  //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

  CTimer timer;

  // if beginning is already below thr, move to above it
  if ( PTN(x,y)<=contour_thr+10 ) while ( PTN(x,y)<contour_thr+10 && y<ycntr ) y++;

  while ( PTN(x,y)>contour_thr && y<ycntr ) y++;   //   while ( PT(x,y)>contour_thr && x>xmin ) x--;

  if ( y>=ycntr ) // if ( x<=xmin )
  {
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
     if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

//  #define PTD(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
//  #define PTE(i) PTN(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
//  i = 0; // start moving upward - CCW search
  i = 2; // start rightward - CW search
  while ( ptn<NPTCNT-1 && ptx[ptn]>=xmin && ptx[ptn]<=xmax && pty[ptn]>=ymin && pty[ptn]<=ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i1; // CW
    while ( i<=i2 && !( PTEN(i+7)>contour_thr && PTEN(i)<=contour_thr ) ) i++;

    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;

  }

 // TRACE("Contour finished maxheights %d,%d - %d,%d\n", xheight1, iheight1, xheight2, iheight2 );

  if ( ptx[ptn-1] != ptx[0] ) //  || ptn<min_npt )
  {
    txt1 = string_764[lng]; // cont_txt; //.Format( L"%s - %s", cont_txt, htot_txt );
    return 199; // failed to validate contour, return error
  }

  // PROFILE CHECK
  if ( ptn<min_npt ) // too few points
  {
    txt1.Format( string_766[lng], ptn, min_npt, max_npt );
    return 299; // failed to validate contour, return error
  }

  if ( ptn>max_npt ) // too many points
  {
    txt1.Format( string_768[lng], ptn, min_npt, max_npt );
    return 399; // failed to validate contour, return error
  }

  //TRACE("Profile found and confirmed %d points\n", ptn );
  // cont_txt.Format( string_759[lng], ptn ); // Good

/*  txt1.Format(L"Countour %d points found in %d us", ptn, timer.elapsed_us() );
  col1 = 0;

  tot_count++;
  tot_time += timer.elapsed_us();

  txt2.Format(L"%d cycles - average time %3.3lf us", tot_count, double(tot_time)/double(tot_count) );
  col2 = 0; */


  if ( bl_drawovl ) // Draw found profile, height and symmetry
  {
    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  ); // first pass to blank wider trace
    for ( i=0 ; i<ptn ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
    UINT col_markers = 0xFF00FFFF;
    DRAW_SQUARE(index,ptx[    0],pty[    0],col_markers); // first and last 
    DRAW_SQUARE(index,ptx[ptn-1],pty[ptn-1],col_markers); // contour points
  } 

  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
  #endif


  return 0; // completed succesfully
}



#define PTD2(i) PTN(ptx2[ptn2]+ofsx[(i)&7],pty2[ptn2]+ofsy[(i)&7])


int CTip_inspection::Find_contour_ccw( void )
{
  int i;
  int x, y;

 // ZeroMemory( kpts2, sizeof(kpts2) );
  if ( !img_ptr ) return 99;

  ptn2    = 0;

  int ycntr = (ymin+ymax)/2;

  // find first point
  x = xmin;
  y = ymax-1;

  //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

  CTimer timer;

  // if beginning is already below thr, move to above it
  if ( PTN(x,y)<=contour_thr+10 ) while ( PTN(x,y)<contour_thr+10 && y>ycntr ) y--;

  while ( PTN(x,y)>contour_thr && y>ycntr ) y--;   //   while ( PT(x,y)>contour_thr && x>xmin ) x--;

  if ( y<=ycntr ) // if ( x<=xmin )
  {
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

/*  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
     if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif */

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptx2[0] = x;
  pty2[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));


  
//  i = 4; // start moving upward - CCW search
  i = 2; // start rightward - CW search
  while ( ptn2<NPTCNT-1 && ptx2[ptn2]>=xmin && ptx2[ptn2]<=xmax && pty2[ptn2]>=ymin && pty2[ptn2]<=ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

//    i = i1; // CW
    i = i2; // CCW
    while ( i>=i1 && !( PTD2(i+1)>contour_thr && PTD2(i)<=contour_thr ) ) i--;

    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx2[ptn2+1] = ptx2[ptn2]+ofsx[i&7];
    pty2[ptn2+1] = pty2[ptn2]+ofsy[i&7];
    ptn2++;

  }

 // TRACE("Contour finished maxheights %d,%d - %d,%d\n", xheight1, iheight1, xheight2, iheight2 );

  if ( ptx2[ptn2-1] != ptx2[0] ) //  || ptn<min_npt )
  {
    txt1 = string_764[lng]; // cont_txt; //.Format( L"%s - %s", cont_txt, htot_txt );
    return 199; // failed to validate contour, return error
  }

  // PROFILE CHECK
  if ( ptn2<min_npt ) // too few points
  {
    txt1.Format( string_766[lng], ptn2, min_npt, max_npt );
    return 299; // failed to validate contour, return error
  }

  if ( ptn2>max_npt ) // too many points
  {
    txt1.Format( string_768[lng], ptn2, min_npt, max_npt );
    return 399; // failed to validate contour, return error
  }

  //TRACE("Profile found and confirmed %d points\n", ptn );
  // cont_txt.Format( string_759[lng], ptn ); // Good

/*  txt1.Format(L"Countour %d points found in %d us", ptn2, timer.elapsed_us() );
  col1 = 0;

  tot_count++;
  tot_time += timer.elapsed_us();

  txt2.Format(L"%d cycles - average time %3.3lf us", tot_count, double(tot_time)/double(tot_count) );
  col2 = 0; */


  if ( bl_drawovl ) // Draw found profile, height and symmetry
  {
    for ( i=0 ; i<ptn2 ; i++ ) DRAW_CROSS(index,ptx2[i],pty2[i],COL_BLACK  ); // first pass to blank wider trace
    for ( i=0 ; i<ptn2 ; i++ ) DRAW_PT   (index,ptx2[i],pty2[i],COL_ORANGE ); // second pass for marking contour
    UINT col_markers = 0xFF00FFFF;
    DRAW_SQUARE(index,ptx2[     0],pty2[     0],col_markers); // first and last 
    DRAW_SQUARE(index,ptx2[ptn2-1],pty2[ptn2-1],col_markers); // contour points
  } 

/*  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
  #endif */


  return 0; // completed succesfully
}



#undef   PT
#define  PT(x,y)  img[x+(y)*dimx]


#endif // WORKER_THREAD  // if defined uses worker threads to speed-up processing


int CTip_inspection::Find_contour_tip_v30( void )
{
  int i;
  int x, y;

  //ZeroMemory( kpts, sizeof(kpts) );

  int ycntr = (ymin+ymax)/2;

  xheight  = 0; // reset maximum height
  iheight  = 0;
  xheight1 = 0; // reset maximum height1
  iheight1 = 0;
  xheight2 = 0; // reset maximum height2
  iheight2 = 0;
  yasse    = 0; // reset central axis

  col1     =  TXT_DARKRED;


  // find first point
  x = xmin; //xmax-1;
  y = ymin;

  //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

  // if beginning is already below thr, move to above it
  if ( PT(x,y)<=contour_thr+10 ) while ( PT(x,y)<contour_thr+10 && y<ycntr ) y++;

  while ( PT(x,y)>contour_thr && y<ycntr ) y++;   //   while ( PT(x,y)>contour_thr && x>xmin ) x--;

  if ( y>=ycntr ) // if ( x<=xmin )
  {
    /*#ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -100, index+3 ); // insert -100 into 3-4-5 cat no container/profile found
    #endif*/
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
     if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptn    = 0;
  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

//  #define PTD(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
  #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
//  i = 0; // start moving upward - CCW search
  i = 2; // start rightward - CW search
  while ( ptn<NPTCNT-1 && ptx[ptn]>=xmin && ptx[ptn]<=xmax && pty[ptn]>=ymin && pty[ptn]<=ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

//    i = i2; // CCW
//    while ( i>=i1 && !( PTD(i+1)>contour_thr && PTD(i)<=contour_thr ) ) i--;

    i = i1; // CW
    while ( i<=i2 && !( PTE(i+7)>contour_thr && PTE(i)<=contour_thr ) ) i++;

    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;

    if ( xheight1<ptx[ptn] )
    {
      xheight1 = ptx[ptn];
      yheight1 = pty[ptn];
      iheight1 = ptn;
    }

    if ( ptx[ptn]==xheight1 ) //==xheight1 )
    {
      xheight2 = xheight1;
      yheight2 = pty[ptn];
      iheight2 = ptn;
    }

    yasse += pty[ptn];
  }

 // TRACE("Contour finished maxheights %d,%d - %d,%d\n", xheight1, iheight1, xheight2, iheight2 );
  ASSERT( xheight1==xheight2 );

  xheight = xheight1;
  yheight = (pty[iheight1]+pty[iheight2])/2;

  yasse = int( 0.5 + double(yasse)/ptn );

  if ( ptx[ptn-1] != ptx[0] ) //  || ptn<min_npt )
  {
    //cont_txt = string_764[lng];
    txt1 = string_764[lng]; // cont_txt; //.Format( L"%s - %s", cont_txt, htot_txt );
    return 199; // failed to validate contour, return error
  }

  // PROFILE CHECK
  if ( ptn<min_npt ) // too few points
  {
    txt1.Format( string_766[lng], ptn, min_npt, max_npt );
    //txt1 = cont_txt;
    return 299; // failed to validate contour, return error
  }

  if ( ptn>max_npt ) // too many points
  {
    txt1.Format( string_768[lng], ptn, min_npt, max_npt );
    //txt1 = cont_txt;
    return 399; // failed to validate contour, return error
  }

  //TRACE("Profile found and confirmed %d points\n", ptn );
  cont_txt.Format( string_759[lng], ptn ); // Good

  // CHECK MAXIMUM HEIGHT
  UINT col_height = COL_GREEN;
  if ( htot_min && htot_max && htot_max>htot_min )
  {
    if ( xheight < htot_min ) // max height too LOW
    {
      htot_txt.Format( string_720[lng], xheight, htot_min, htot_max );
      //txt1.Format( L"%s - %s", cont_txt, htot_txt );
      col_height = COL_RED;
      //return 497;
    }
    else
    {
      if ( xheight > htot_max ) // max height too HIGH
      {
        htot_txt.Format( string_722[lng], xheight, htot_min, htot_max );
        //txt1.Format( L"%s - %s", cont_txt, htot_txt );
        col_height = COL_RED;
        //return 498;
      }
      else // Height Ok
      {
        htot_txt.Format( string_724[lng], xheight, htot_min, htot_max );
      }
    }
  }
  else
  {
    htot_txt = string_3262[lng];
  }

//  txt1.Format( L"%s - %s", cont_txt, htot_txt );


  /*
  // CHECK CURVATURE
  int imin = AMPATN;
  int imax = ptn-AMPATN-1;

  for ( i=imin ; i<imax ; i++ )
  {
//    TRACE("%d %d,%d,%d,%d %d\n", i, ptx[i], pty[i], DX, DY, ATAN(i) );
    TRACE("%d,%d\n", i, ATAN(i) );
  }
  */


  if ( bl_drawovl ) // Draw found profile, height and symmetry
  {
    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  ); // first pass to blank wider trace
    for ( i=0 ; i<ptn ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
    UINT col_markers = 0xFF00FFFF;
    DRAW_SQUARE(index,ptx[    0],pty[    0],col_markers); // first and last 
    DRAW_SQUARE(index,ptx[ptn-1],pty[ptn-1],col_markers); // contour points

    Draw_height( col_height );
    
    //DRAW_SQUARE(index,ptx[iheight1],pty[iheight1],col_markers);
    //DRAW_SQUARE(index,ptx[iheight2],pty[iheight2],col_markers);

    DRAW_SQUARE(index,xheight,yheight,col_markers);

    for ( x=xmin ; x<xheight-4 ; x+=4 ) DRAW_PT(index,x,yasse,COL_MAGENTA); // draw symmetry axis

  } 

  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
  #endif

  txt1.Format( L"%s - %s", cont_txt, htot_txt );

  if ( col_height==COL_RED )
  {
    return 499;
  }

  col1 = TXT_DARKGREEN;

  col2 = TXT_DARKRED;


  //   *******  Bulge shrink analysis  *******
  if ( blgsrk_hmax && blgsrk_hmin && blgsrk_hmax>blgsrk_hmin+10 ) // minimum difference of 10
  {
    int x1 = xheight - blgsrk_hmax; // lower, to the left
    int x2 = xheight - blgsrk_hmin; // upper, to the right

    if ( x1<xmin ) x1 = xmin;

    ASSERT( x2-x1>0 );

    blgsrk_err  =   0;        // bulge shrink error 

    // seconda versione basata su ampiezza in finestra mobile
    int il =     0; // left/up
    int ir = ptn-1; // right/bottom
    int* width = (int*) malloc( (x2-x1+1)*sizeof(int) );
    ASSERT( width );


    for ( x=x1 ; x<=x2 ; x++ )
    {
      while ( ptx[il]<x && il<iheight1 ) il++; // search il (left/top)
      while ( ptx[ir]<x && ir>iheight2 ) ir--; // search ir (right/bottom)

      width[x-x1] = pty[ir]-pty[il];

      if ( x>=x1+blgsrk_hchk )
      {
        ASSERT( x-x1>=0 && x-x1<x2-x1+1 );

        int delta = width[x-x1-blgsrk_hchk]-width[x-x1];
        if ( delta-blgsrk_maxd>0 ) 
        {
          blgsrk_err += delta-blgsrk_maxd;        // accumulate bulge shrink error 
          //DRAW_SQUARE(index,ptx[il],pty[il]-6,COL_RED);
          //DRAW_SQUARE(index,ptx[ir],pty[ir]+6,COL_RED);
          DRAW_SQUARE(index,x,pty[il]-14,COL_RED);
          DRAW_SQUARE(index,x,pty[ir]+14,COL_RED);
        }
        else
        {
          //DRAW_PT(index,ptx[il],pty[il]-6,COL_DARKGREEN);
          //DRAW_PT(index,ptx[ir],pty[ir]+6,COL_DARKGREEN);
          DRAW_PT(index,x,pty[il]-14,COL_DARKGREEN);
          DRAW_PT(index,x,pty[ir]+14,COL_DARKGREEN);
        }


        //TRACE("width[%d]=%d - w[%d]=%d - delta=%d\n", x, width[x-x1],x-blgsrk_hchk,width[x-x1-blgsrk_hchk], width[x-x1-blgsrk_hchk]-width[x-x1] );
      }
      else
      {
        //TRACE("width[%d] = %d\n", x, width[x-x1] );
      }
    }

    free( width );

    if ( blgsrk_err>blgsrk_mxer ) // Bulge shrink REJECT
    {
      //blgsrk_txt.Format( string_3210[lng], blgsrk_err, blgsrk_mxer );
      txt2.Format( string_3210[lng], blgsrk_err, blgsrk_mxer );
      return 899;
    }
    else // Bulge Ok
    {
      blgsrk_txt.Format( string_3212[lng], blgsrk_err, blgsrk_mxer );
    }


    //txt2 = blgsrk_txt;

    /*
    // prima versione basata su pendenza
    // LEFT SIDE
    int i2 = iheight1; // start from top
    while ( ptx[i2]>x2 && i2>20 ) i2--;

    int i1 = i2-10; 
    while ( ptx[i1]>x1 && i1>0 ) i1--;

    //ASSERT( 0<i1 && i1<i2 && i2<iheight1 );

    ASSERT( AMPATN<i1 && i1<i2 && i2<ptn-AMPATN );
    for ( i=i1 ; i<i2 ; i++ ) TRACE("%d,%d %d\n", ptx[i], pty[i], ATAN(i) );

    for ( i=i1 ; i<i2 ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK   ); // first pass to blank wider trace
    for ( i=i1 ; i<i2 ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA ); // second pass for marking contour

    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
      if ( image_dsp[index]->ptr_fulldsp ) for ( i=i1 ; i<=i2 ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    #endif

    // RIGHT SIDE
    i2 = iheight2; // start from top
    while ( ptx[i2]>x2 && i2<ptn-20 ) i2++;

    i1 = i2+10; 
    while ( ptx[i1]>x1 && i1<ptn ) i1++;

    ASSERT( ptn>i1 && i1>i2 && i2>iheight2 );

    for ( i=i2 ; i<i1 ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK   ); // first pass to blank wider trace
    for ( i=i2 ; i<i1 ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA ); // second pass for marking contour

    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
      if ( image_dsp[index]->ptr_fulldsp ) for ( i=i2 ; i<=i1 ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    #endif
    */
  }
  else
  {
    blgsrk_txt = string_3214[lng];
  }



  
  // TIP TOP CURVATURE
  if ( tpcurv_hmax ) // tpcurv_hmin CAN be zero
  {
    //int x1 = xheight - tpcurv_hmax; // upper, to the right
    //int x2 = xheight - tpcurv_hmin; // lower, to the left
    // if ( x1<xmin ) x1 = xmin;
    //  ASSERT( x2-x1>0 );

    tpcurv_err  =   0;        // reset initial error 

    ASSERT( ptx[iheight1]==xheight && ptx[iheight2]==xheight && iheight1<=iheight2 );

    int il = iheight1; // left/up
    int ir = iheight2; // right/bottom

    int x1 = xheight - tpcurv_hmax; // lower
    if ( x1<xmin ) x1 = xmin;

    int x2 = xheight - tpcurv_hmin; // upper 
    if ( x2>xheight ) x2 = xheight;
    if ( x2<=x1     ) x2 = x1;

    //x1 = xheight-tpcurv_modh/2;
    //x2 = xheight;

 /*   for ( y=0 ; y<10 ; y++ )
    {
      DRAW_PT(index, x1, yasse-y, COL_BLUE );
      DRAW_PT(index, x1, yasse+y, COL_BLUE );

      DRAW_PT(index, x2, yasse-y, COL_BLUE );
      DRAW_PT(index, x2, yasse+y, COL_BLUE );

    } */

    //int lastxdrw = 0;
    int x0 = xheight-tpcurv_modh/2;

//    for ( x=x1 ; x<=x2 ; x++ )
    for ( x=x2 ; x>=x1 ; x-- )
    {
      while ( ptx[il]>=x && il>0 ) il--;
      il++;
      //DRAW_PT(index, ptx[il], pty[il], COL_GREEN );

      while ( ptx[ir]>=x && ir<ptn ) ir++;
      ir--;
      //DRAW_PT(index, ptx[ir], pty[ir], COL_GREEN );

      int diam = pty[ir]-pty[il];

      y = int( 0.5 + 0.5*tpcurv_modw*sqrt((0.25*tpcurv_modh*tpcurv_modh-(x-x0)*(x-x0))/(0.25*tpcurv_modh*tpcurv_modh)));

      DRAW_PT(index, x, yheight-y, COL_BLUE );
      DRAW_PT(index, x, yheight+y, COL_BLUE );

      tpcurv_err += (2*y-diam)*(2*y-diam);

//      DRAW_PT(index,x,yasse-y,COL_CYAN);
//      DRAW_PT(index,x,yasse+y,COL_CYAN);
//      DRAW_PT(index,x,yheight-y,COL_CYAN);
//      DRAW_PT(index,x,yheight+y,COL_CYAN);

        //TRACE("width[%d]=%d - w[%d]=%d - delta=%d\n", x, width[x-x1],x-blgsrk_hchk,width[x-x1-blgsrk_hchk], width[x-x1-blgsrk_hchk]-width[x-x1] );
        //TRACE("width[%d] = %d\n", x, width[x-x1] );
    }

    //free( width );

    tpcurv_err /= (x2-x1+1); // normalize against height
    
    if ( tpcurv_err>tpcurv_mxer ) // Tip curvature REJECT
    {
      tpcurv_txt.Format( string_3282[lng], tpcurv_err, tpcurv_mxer );
      txt2.Format( L"%s - %s - %s", blgsrk_txt, innbrn_txt, tpcurv_txt );
      return 899;
    }
    else // curvature Ok
    {
      tpcurv_txt.Format( string_3284[lng], tpcurv_err, tpcurv_mxer );
    } 

  }
  else
  {
    tpcurv_txt = string_3286[lng];
  }


 



  // INNER BURNS CONTROL

  //   *******  Bulge burn analysis  *******
  if ( innbrn_hmax && innbrn_hmin && innbrn_hmax>innbrn_hmin+10 ) // minimum difference of 10
  {
    //  #define FLTBRNS ( PT(x,y-2)-2*PT(x,y)+PT(x,y+2) )
    #define FLTBRNS ( PT(x-2,y-2)+PT(x-2,y+2)-4*PT(x,y)+PT(x+2,y-2)+PT(x+2,y+2) )

    int x1 = xheight - innbrn_hmax; // lower, to the left
    int x2 = xheight - innbrn_hmin; // upper, to the right

    if ( x1<xmin ) x1 = xmin;

    ASSERT( x2-x1>0 );

    innbrn_err  =   0;        // bulge shrink error 

    // versione basata su ampiezza in finestra mobile
    int il =     0; // left/up
    int ir = ptn-1; // right/bottom
    //int* width = (int*) malloc( (x2-x1+1)*sizeof(int) );
    //ASSERT( width );

    for ( x=x1 ; x<=x2 ; x++ )
    {
      while ( ptx[il]<x && il<iheight1 ) il++; // search il (left/top)
      while ( ptx[ir]<x && ir>iheight2 ) ir--; // search ir (right/bottom)

      // search top/left point
      int yl = pty[il]+innbrn_minw;
      while ( PT(x,yl)<innbrn_minb && yl<yasse ) yl++;
   //   DRAW_PT(index,x,yl,COL_CYAN);

      // search bottom/right point
      int yr = pty[ir]-innbrn_minw;
      while ( PT(x,yr)<innbrn_minb && yr>yasse ) yr--;
   //   DRAW_PT(index,x,yr,COL_CYAN);


      // FIRST VERSION WITH DIFFERENTIAL FILTER - From Kelun 20160725 changed back to old variance version
      /* for ( y=yl ; y<=yr ; y++ ) 
      {
        if ( FLTBRNS-innbrn_fltm>0 )
        {
          innbrn_err += FLTBRNS-innbrn_fltm;
          DRAW_PT(index,x,y,COL_RED);
        }

      } */

      // SECOND VERSION BASED ON VARIANCE - First used for Kelun 20160725
      if ( yr-yl>5 )
      {
        UINT sum  = 0;
        UINT sum2 = 0;
        UINT ptn  = 0;

        for ( y=yl ; y<=yr ; y++ ) 
        {
          //DRAW_PT( index, x+xmin, y, area_color ); // 0xFFRRGGBB
          sum  += PT(x,y);
          sum2 += PT(x,y)*PT(x,y);
          ptn++;
        }
        //ASSERT( ptn>9 );

        int varc = ( sum2-(sum*sum)/ptn )/(ptn-1); // compute variance in this line
//        TRACE("%d v=%d\n", x+xmin, burns_err );

        if ( varc>burns_maxvar )
        {
          innbrn_err += varc-burns_maxvar;
          DRAW_PT( index, x, yl, COL_RED ); // 0xFFRRGGBB
          DRAW_PT( index, x, yr, COL_RED ); // 0xFFRRGGBB
        }
        else
        {
          DRAW_PT( index, x, yl, COL_GREEN ); // 0xFFRRGGBB
          DRAW_PT( index, x, yr, COL_GREEN ); // 0xFFRRGGBB
        }
      }

    } // for x

    //free( width );

    // innbrn_err /= 10; old version

    if ( innbrn_err>innbrn_mxer ) // Inner burns REJECT
    {
      innbrn_txt.Format( string_3240[lng], innbrn_err, innbrn_mxer );
      txt2.Format( L"%s - %s", blgsrk_txt, innbrn_txt );
      return 899;
    }
    else // Inner burns Ok
    {
      innbrn_txt.Format( string_3242[lng], innbrn_err, innbrn_mxer );
    } 

  }
  else
  {
    innbrn_txt = string_3244[lng];
  }

  col2 = TXT_DARKGREEN;
  txt2.Format( L"%s - %s - %s", blgsrk_txt, innbrn_txt, tpcurv_txt );




//  txt1.Format( /*string_768[lng]*/ L"Profile OK %d (%d,%d)", ptn, min_npt, max_npt );



  //  - - - -   Search inner profile for new tip   - - - - 
    /*
  #ifdef NEW_TIP_V30 // in tip mode search inner profile

    ycntr = (pty[ptn-1]+pty[0])/2; // center of found external profile

    // find first point
    x = xmin; 
    y = ycntr; // pty[0];

    //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

    // if beginning is already below thr, move to above it
    if ( PT(x,y)<=contour_thr+10 ) while ( PT(x,y)<contour_thr+10 && y>pty[0] ) y--;

    while ( PT(x,y)>contour_thr && y>pty[0] ) y--; 

    if ( y<=pty[0] ) 
    {
      txt1.Format( string_756[lng], ptn2, min_npt );
      return 99; // failed to find first point, return error
    }

    ptn2    = 0;
    ptx2[0] = x;
    pty2[0] = y;

    //  i = 0; // start moving upward - CCW search
    i = 2; // start rightward - CW search
    while ( ptn2<NPTCNT-1 && ptx2[ptn2]>=xmin && ptx2[ptn2]<=xmax && pty2[ptn2]>=ymin && pty2[ptn2]<=ymax )
    {
      int i1 = i-3+8;
      int i2 = i+3+8;

      //    i = i2; // CCW
      //    while ( i>=i1 && !( PTD(i+1)>contour_thr && PTD(i)<=contour_thr ) ) i--;

      i = i1; // CW
      while ( i<=i2 && !( PTE(i+7)>contour_thr && PTE(i)<=contour_thr ) ) i++;

      //if ( i<i1 ) break;

      ASSERT( i%7>=0 && i%7<8 );
      ptx2[ptn2+1] = ptx2[ptn2]+ofsx[i&7];
      pty2[ptn2+1] = pty2[ptn2]+ofsy[i&7];
      ptn2++;
    }

    if ( ptx2[ptn2-1] != ptx2[0] )
    {
      txt1 = string_764[lng];
      return 199; // failed to validate contour, return error
    }

    if ( ptn2<min_npt ) // too few points
    {
      txt1.Format( string_766[lng], ptn2, min_npt, max_npt );
      return 299; // failed to validate contour, return error
    }

    if ( ptn2>max_npt ) // too many points
    {
      txt1.Format( string_768[lng], ptn2, min_npt, max_npt );
      return 399; // failed to validate contour, return error
    }

    if ( bl_drawovl )
    {
      for ( i=0 ; i<ptn2 ; i++ ) DRAW_CROSS(index,ptx2[i],pty2[i],COL_BLACK  ); // first pass to blank wider trace
      for ( i=0 ; i<ptn2 ; i++ ) DRAW_PT   (index,ptx2[i],pty2[i],COL_MAGENTA); // second pass for marking contour

      UINT col_markers = 0xFF00FFFF;
      DRAW_SQUARE(index,ptx2[     0],pty2[     0],col_markers);
      DRAW_SQUARE(index,ptx2[ptn2-1],pty2[ptn2-1],col_markers);
    }

    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
      if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn2 ; i++ ) image_dsp[index]->ptr_fulldsp[ptx2[i]+pty2[i]*dimx] = COL_MAGENTA;
    #endif

  #endif // NEW_TIP_V30 in tip mode search inner profile
  */

  return 0; // completed succesfully
}




int CTip_inspection::Find_contour_v30( void )
{
  int i;
  int x, y;

  ZeroMemory( kpts, sizeof(kpts) );

  int ycntr = (ymin+ymax)/2;

  xheight = 0; // reset maximum height
  iheight = 0;
  yasse   = 0; // reset central axis


  // find first point
  x = xmin; //xmax-1;
  y = ymin;

  //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

  // if beginning is already below thr, move to above it
  if ( PT(x,y)<=contour_thr+10 ) while ( PT(x,y)<contour_thr+10 && y<ycntr ) y++;

  while ( PT(x,y)>contour_thr && y<ycntr ) y++;   //   while ( PT(x,y)>contour_thr && x>xmin ) x--;

   if ( y>=ycntr ) // if ( x<=xmin )
  {
    /*#ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -100, index+3 ); // insert -100 into 3-4-5 cat no container/profile found
    #endif*/
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
     if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptn    = 0;
  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

//  #define PTD(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])
//  #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
//  i = 0; // start moving upward - CCW search
  i = 2; // start rightward - CW search
  while ( ptn<NPTCNT-1 && ptx[ptn]>=xmin && ptx[ptn]<=xmax && pty[ptn]>=ymin && pty[ptn]<=ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

//    i = i2; // CCW
//    while ( i>=i1 && !( PTD(i+1)>contour_thr && PTD(i)<=contour_thr ) ) i--;

    i = i1; // CW
    while ( i<=i2 && !( PTE(i+7)>contour_thr && PTE(i)<=contour_thr ) ) i++;

    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;

  }

  if ( ptx[ptn-1] != ptx[0] ) //  || ptn<min_npt )
  {
    txt1 = string_764[lng];
    return 199; // failed to validate contour, return error
  }

  if ( ptn<min_npt ) // too few points
  {
    txt1.Format( string_766[lng], ptn, min_npt, max_npt );
    return 299; // failed to validate contour, return error
  }

  if ( ptn>max_npt ) // too many points
  {
    txt1.Format( string_768[lng], ptn, min_npt, max_npt );
    return 399; // failed to validate contour, return error
  }

  //TRACE("Profile found and confirmed %d points\n", ptn );


  if ( bl_drawovl ) // Draw found profile
  {
    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  ); // first pass to blank wider trace
    for ( i=0 ; i<ptn ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
    UINT col_markers = 0xFF00FFFF;
    DRAW_SQUARE(index,ptx[    0],pty[    0],col_markers);
    DRAW_SQUARE(index,ptx[ptn-1],pty[ptn-1],col_markers);
  }




  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
  #endif

  txt1.Format( /*string_768[lng]*/ L"Profile OK %d (%d,%d)", ptn, min_npt, max_npt );



  //  - - - -   Search inner profile for new tip   - - - - 
    /*
  #ifdef NEW_TIP_V30 // in tip mode search inner profile

    ycntr = (pty[ptn-1]+pty[0])/2; // center of found external profile

    // find first point
    x = xmin; 
    y = ycntr; // pty[0];

    //for ( y = ymin ; y<ymax ; y++ ) TRACE("%d,%d\n",y,PT(x,y));

    // if beginning is already below thr, move to above it
    if ( PT(x,y)<=contour_thr+10 ) while ( PT(x,y)<contour_thr+10 && y>pty[0] ) y--;

    while ( PT(x,y)>contour_thr && y>pty[0] ) y--; 

    if ( y<=pty[0] ) 
    {
      txt1.Format( string_756[lng], ptn2, min_npt );
      return 99; // failed to find first point, return error
    }

    ptn2    = 0;
    ptx2[0] = x;
    pty2[0] = y;

    //  i = 0; // start moving upward - CCW search
    i = 2; // start rightward - CW search
    while ( ptn2<NPTCNT-1 && ptx2[ptn2]>=xmin && ptx2[ptn2]<=xmax && pty2[ptn2]>=ymin && pty2[ptn2]<=ymax )
    {
      int i1 = i-3+8;
      int i2 = i+3+8;

      //    i = i2; // CCW
      //    while ( i>=i1 && !( PTD(i+1)>contour_thr && PTD(i)<=contour_thr ) ) i--;

      i = i1; // CW
      while ( i<=i2 && !( PTE(i+7)>contour_thr && PTE(i)<=contour_thr ) ) i++;

      //if ( i<i1 ) break;

      ASSERT( i%7>=0 && i%7<8 );
      ptx2[ptn2+1] = ptx2[ptn2]+ofsx[i&7];
      pty2[ptn2+1] = pty2[ptn2]+ofsy[i&7];
      ptn2++;
    }

    if ( ptx2[ptn2-1] != ptx2[0] )
    {
      txt1 = string_764[lng];
      return 199; // failed to validate contour, return error
    }

    if ( ptn2<min_npt ) // too few points
    {
      txt1.Format( string_766[lng], ptn2, min_npt, max_npt );
      return 299; // failed to validate contour, return error
    }

    if ( ptn2>max_npt ) // too many points
    {
      txt1.Format( string_768[lng], ptn2, min_npt, max_npt );
      return 399; // failed to validate contour, return error
    }

    if ( bl_drawovl )
    {
      for ( i=0 ; i<ptn2 ; i++ ) DRAW_CROSS(index,ptx2[i],pty2[i],COL_BLACK  ); // first pass to blank wider trace
      for ( i=0 ; i<ptn2 ; i++ ) DRAW_PT   (index,ptx2[i],pty2[i],COL_MAGENTA); // second pass for marking contour

      UINT col_markers = 0xFF00FFFF;
      DRAW_SQUARE(index,ptx2[     0],pty2[     0],col_markers);
      DRAW_SQUARE(index,ptx2[ptn2-1],pty2[ptn2-1],col_markers);
    }

    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
      if ( image_dsp[index]->ptr_fulldsp ) for ( i=0 ; i<ptn2 ; i++ ) image_dsp[index]->ptr_fulldsp[ptx2[i]+pty2[i]*dimx] = COL_MAGENTA;
    #endif

  #endif // NEW_TIP_V30 in tip mode search inner profile
  */

  return 0; // completed succesfully
}









int CTip_inspection::Find_contour_new( int y0 )
{

  ASSERT( y0>ymin && y0<ymax );

  int i;
  int x, y;

  int   ptnt;
  int   ptxt[NPTCNT/2];
  int   ptyt[NPTCNT/2];

  /*
  for ( x=xmin ; x<=xmax ; x++ )
  {
    PT(x,ymin) = 0;
    PT(x,ymax) = 0;
  }

  for ( y=ymin ; y<=ymax ;y++ )
  {
    PT(xmin,y) = 0;
    PT(xmax,y) = 0;
  }
  */

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );

  //ZeroMemory( &cntrbbox, sizeof(cntrbbox) );

/* ***   cntrbbox.left   = xmax;
  cntrbbox.right  = xmin;
  cntrbbox.top    = ymax;
  cntrbbox.bottom = ymin; */

  ZeroMemory( kpts, sizeof(kpts) );


  // find first point
  x = xmax-1;
  y = y0;
  while ( PT(x,y)>contour_thr && x>xmin ) x--;

  if ( x<=xmin )
  {
    /*#ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -100, index+3 ); // insert -100 into 3-4-5 cat no container/profile found
    #endif*/
    txt1.Format( string_756[lng], ptn, min_npt );
    return 99; // failed to find first point, return error
  }

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptnt    = 0;
  ptxt[0] = x;
  ptyt[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

  #define PTDT(i) PT(ptxt[ptnt]+ofsx[(i)&7],ptyt[ptnt]+ofsy[(i)&7])
//  #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  
  i = 0; // start moving upward - CCW search
  while ( ptnt<NPTCNT/2-1 && ptxt[ptnt]>xmin && ptxt[ptnt]<xmax && ptyt[ptnt]>ymin && ptyt[ptnt]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i2;
    while ( i>=i1 && !( PTDT(i+1)>contour_thr && PTDT(i)<=contour_thr ) ) i--;
    //if ( i<i1 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptxt[ptnt+1] = ptxt[ptnt]+ofsx[i&7];
    ptyt[ptnt+1] = ptyt[ptnt]+ofsy[i&7];
    ptnt++;
  }

  // copy inverted
  for ( i=0 ; i<ptnt ; i++ )
  {
    ptx[i] = ptxt[ptnt-i-1];
    pty[i] = ptyt[ptnt-i-1];
  }
  ptmid = ptnt; // index of middle/center point of contour
  ptn=ptnt-1;


/*  #ifdef SAVE_FULL_PROC
    for ( i=0 ; i<ptnt ; i++ )
    {
      image_dsp[index]->ptr_fulldsp[ptxt[i]+ptyt[i]*dimx] = 0xFFFF00FF;
      TRACE("%d,%d,%d\n", i, ptxt[i], ptyt[i] );
    }
  #endif  */
/*
  ptn    = 0;
  ptx[0] = ptxt[0];
  pty[0] = ptyt[0];
 */


  i = 4; // start moving downward - CW search
  while ( ptn<NPTCNT-1 && ptx[ptn]>xmin && ptx[ptn]<xmax && pty[ptn]>ymin && pty[ptn]<ymax )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i1;
    while ( i<=i2 && !( PTE(i+7)>contour_thr && PTE(i)<=contour_thr ) ) i++;
    //if ( i>i2 ) break;

    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;
  }

  /*
  #ifdef SAVE_FULL_PROC
    for ( i=0 ; i<ptn ; i++ )
    {
      image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = 0xFFFF00FF;
      TRACE("%d,%d,%d\n", i, ptx[i], pty[i] );
    }
  #endif
  */

  if ( ptn<min_npt )
  {
   /* #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -130, index+3 ); // insert -130 into 3-4-5 cat 
    #endif*/
    txt1.Format( string_760[lng], ptn, min_npt );
    return 109;
  } 

  if ( bl_drawovl )
  {
    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK); // first pass to blank wider trace
  }

  for ( i=0 ; i<ptn ; i++ )
  {
/* ***    if ( ptx[i]>cntrbbox.right  ) cntrbbox.right  = ptx[i];
    if ( ptx[i]<cntrbbox.left   ) cntrbbox.left   = ptx[i];
    if ( pty[i]>cntrbbox.bottom ) cntrbbox.bottom = pty[i];
    if ( pty[i]<cntrbbox.top    ) cntrbbox.top    = pty[i]; */

    if ( bl_drawovl ) DRAW_PT (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
  }

  //TRACE("Bounding box: (%d,%d)-(%d,%d)\n", cntrbbox.left, cntrbbox.top, cntrbbox.right, cntrbbox.bottom );

//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp )
    {
      for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    }
  #endif

  return 0;
}






int CTip_inspection::Find_contour( int x0 )
{

  ASSERT( x0>=xmin && x0<=xmax );

  int x, y;
//  int targbrt = 100;
//  int thr = 100;

  //TRACE("Profilo di base x=%d - da %d a %d\n", xmin, ymin, ymax );
  //for ( y=ymin ; y<=ymax ; y++ ) TRACE("%d,%d\n",y,PT(xmin,y));

  for ( x=xmin-20 ; x<xmin+350 ; x++ )
  {
    Find_edge_downward(x);
  }
  return 10;

  for ( x=x0-1 ; x<=xmax ; x++ )
  {
    PT(x,ymin) = 0;
    PT(x,ymax) = 0;
  }

  for ( y=ymin ; y<=ymax ;y++ )
  {
    PT(x0-1,y) = 0;
    PT(xmax,y) = 0;
  }

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );


  // first find top/left point
  x = x0;
  y = ymin+1;
  while ( PT(x,y)>contour_thr && y<(ymin+ymax)/2 ) y++;

  if ( y>=(ymin+ymax)/2 ) return 99; // failed to find first point, return error

  //TRACE("Top/left point found: %d,%d\n", x, y );

  int i;// = 2;


  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6     2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptn    = 0;
  ptx[0] = x;
  pty[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

  //  #define PTD(i) PT(ptx[ptn]+ofsx[(i)%7],pty[ptn]+ofsy[(i)%7])
  //  #define PTD(i) PT(ptx[ptn]+ofsx[i],pty[ptn]+ofsy[i])
  //#undef PTD
  //#define PTD(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

//  ptn = 1;

  i = 2; // start moving rightward

  while ( ptn<NPTCNT-1 && ptx[ptn]>=x0 && ptx[ptn]<=xmax && pty[ptn]>=ymin && pty[ptn]<=ymax )
  {
    int i1 = i+5;
    int i2 = i+3+8;
    i = i1;
//    while ( i<i2 && ( PTD(i)>thr || PTD(i+1)<=thr ) /*&& ptx[ptn]==ptx[ptn-1]+ofsx[i] && pty[ptn]==pty[ptn-1]+ofsy[i]*/ )
    while ( i<i2 && !( PTD(i+7)>contour_thr && PTD(i)<=contour_thr ) /*&& ptx[ptn]==ptx[ptn-1]+ofsx[i] && pty[ptn]==pty[ptn-1]+ofsy[i]*/ )
    {
      //TRACE("%d,%d,%d,%d\n" , i , (i+7)&7 , PTD(i) , PTD((i+7)&7)) ;
     //ASSERT( i>=0 && i<8 );
     // ASSERT( ((i+7)&7)>=0 && ((i+7)&7)<8 );
      i++;
    }
    //TRACE("%3d,%3d - ", PTD(i), PTD(i+1) );
    ASSERT( i%7>=0 && i%7<8 );
    ptx[ptn+1] = ptx[ptn]+ofsx[i&7];
    pty[ptn+1] = pty[ptn]+ofsy[i&7];
    ptn++;

    // TRACE("NPT %3d,%3d,%3d,%3d,%d\n", ptn, ptx[ptn],pty[ptn],PT(ptx[ptn],pty[ptn]),i);
    // DRAW_PT(index,ptx[ptn],pty[ptn],0x00FF00FF);
    // if ( !(ptn&3) ) DRAW_CROSS(index,ptx[ptn],pty[ptn],0x00FF00FF);
  }

//  ptn--;

  if ( ptn<min_npt )
  {
    txt2.Format( string_762[lng], ptn, min_npt );
    return 109;
  }

  // final validation and draw of contour
  if ( ptx[ptn-1]==ptx[0] ) // same start/end x
  {
    //TRACE("PROFILE MADE OF %d POINTS\n",ptn);
    //for ( i=0 ; i<ptn ; i++ ) TRACE("%d,%d,%d\n", i, ptx[i], pty[i] );

    for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  );
    for ( i=0 ; i<ptn ; i++ ) DRAW_PT   (index,ptx[i],pty[i],COL_MAGENTA);


    
//    #ifdef SAVE_FULL_PROC
    #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
      if ( image_dsp[index]->ptr_fulldsp )
      {
        for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
      }
//      image_dsp[index]->Save_fulldsp(L"contour"); //  >img_fulldsp[ptx[ptn]+pty[ptn]*dimx] = 0xFFFF00FF;
      //image_dsp[index]->Save_fulldsp(L"contour"); //  >img_fulldsp[ptx[ptn]+pty[ptn]*dimx] = 0xFFFF00FF;
      //image_dsp[index]->Save_images(L"contour"); //  >img_fulldsp[ptx[ptn]+pty[ptn]*dimx] = 0xFFFF00FF;
    #endif
    

    return 0;
  }

  //TRACE("FINE NPT=%d\n", ptn );
  return 99; // not finally validated
}




int CTip_inspection::Segment_data( int i1, int i2, segment_type* segm )
{
  ASSERT( segm && AfxIsValidAddress(segm,sizeof(segment_type)));

//  ASSERT( i1>10 && i1<ptn-10 );
//  ASSERT( i2>10 && i2<ptn-10 );

  ASSERT( i1>0 && i1<ptn );
  ASSERT( i2>0 && i2<ptn );

  ASSERT( i2 > i1 );

  segm->ind1 = i1;
  segm->ind2 = i2;

  double dx = ptx[i1] - ptx[i2];
  double dy = pty[i1] - pty[i2];

  segm->lenght = int( 0.5 + sqrt( dx*dx+dy*dy ) );

//  segm->ctlenght = i1>i2 ? i1-i2 : i2-i1;

  int i;
  double dist = 0.0;
  for ( i=i1+1 ; i<=i2 ; i++ )
  {
    //sqdst += (ptx[i]-ptx[i-1])*(ptx[i]-ptx[i-1]) + (pty[i]-pty[i-1])*(pty[i]-pty[i-1]);
    if ( (ptx[i]!=ptx[i-1]) && (pty[i]!=pty[i-1]) ) dist += 1.4142;
    else                                            dist += 1.0;
  }
  segm->ctlenght = DOUBLETOINT( dist );

  if ( segm->lenght ) segm->perc = 100 * segm->ctlenght / segm->lenght - 100;
  else                segm->perc = 0;


  if ( dx )
  {
    //segm->angle = DOUBLETOINT(18000.0+18000.0*atan2(dx,-dy)/PIGRECO);
    segm->angle = abs( DOUBLETOINT(180.0*atan2(dx,dy)/PIGRECO) );
  }
  else
  {
    if ( dy>=0) segm->angle =     0;
    else        segm->angle =   180;
//    else        segm->angle = 18000;
  }





//  TRACE("Segment_data %4d,%4d,%4d,%5d\n", i1,i2,segm->lenght,segm->angle);
  //TRACE("Segment_data %4d , %4d , %2.1lf , %3.2lf\n", i1,i2,pix2mm*segm->lenght,0.01*segm->angle);

  return 0;
}






int CTip_inspection::Analyze_powder_stopper( void )
{
  int x, y;

  int x1; 
  int y1; 
  int x2; 
  int y2; 

//    DRAW_SQUARE( index, tl_al.x, tl_al.y, COL_BLUE );
//    DRAW_SQUARE( index, tl_ar.x, tl_ar.y, COL_BLUE );

  x2 = (tl_al.x+tl_ar.x)/2 + pwdrstpr_vo;
  x1 = x2 - pwdrstpr_h;
  y1 = (tl_al.y+tl_ar.y-pwdrstpr_w)/2;
  y2 = y1 + pwdrstpr_w;

  if ( x1<xmin || x2>xmax || x1>=x2 )
  {
    TRACE("Analyze_powder_stopper: something wrong with x1/x2 coords %d,%d\n",x1,x2);
    return 299;
  }

  if ( y1<ymin || y2>ymax || y1>=y2 )
  {
    TRACE("Analyze_powder_stopper: something wrong with y1/y2 coords %d,%d\n",y1,y2);
    return 399;
  }

  UINT area_color = COL_ORANGE;

  int error = 0;

  if ( tl_drawovl )
  {
    for ( x=x1 ; x<=x2 ; x+=4 )
    {
      DRAW_PT( index, x, y1, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2, area_color ); // 0xFFRRGGBB
    }

    for ( y=y1 ; y<=y2 ; y++ )
    {
      DRAW_PT( index, x1, y, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, x2, y, area_color ); // 0xFFRRGGBB
    }
  }

  for ( y=y1 ; y<=y2 ; y++ )
  {
    //DRAW_PT( index, x1, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, x2, y, area_color ); // 0xFFRRGGBB

    for ( x=x1 ; x<=x2 ; x++ )
    {
      if ( PT(x,y)>pwdrstpr_minl && PT(x,y)<pwdrstpr_maxl )
      {
        error++;
        if ( tl_drawovl ) DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
      }
    }
  }

  if ( error>pwdrstpr_maxerr )
  {
    txt3.Format( string_2644[lng], error, pwdrstpr_maxerr );
    col3 = TXT_DARKRED;
    return( 200 );
  }
  txt3.Format( string_2642[lng], error, pwdrstpr_maxerr ); 
  col3 = TXT_DARKGREEN;
  return 0;
}



int CTip_inspection::Analyze_crimping( void )
{
  int x, y;

  int x1; // = 424;
  int y1; // = 127;
  int x2; // = x1 +  60;
  int y2; // = y1 + 282;

//    DRAW_SQUARE( index, tl_al.x, tl_al.y, COL_BLUE );
//    DRAW_SQUARE( index, tl_ar.x, tl_ar.y, COL_BLUE );

  x2 = (tl_al.x+tl_ar.x)/2 + crmparea_vo;
  x1 = x2 - crmparea_h;
//  x1 = (tl_al.x+tl_ar.x)/2 + crmparea_vo;
//  x2 = x1 + crmparea_h;
  y1 = (tl_al.y+tl_ar.y-crmparea_w)/2;
  y2 = y1 + crmparea_w;

  if ( x1<xmin || x2>xmax || x1>=x2 )
  {
    TRACE("Analyze_crimping: something wrong with x1/x2 coords %d,%d\n",x1,x2);
    return 299;
  }

  if ( y1<ymin || y2>ymax || y1>=y2 )
  {
    TRACE("Analyze_crimping: something wrong with y1/y2 coords %d,%d\n",y1,y2);
    return 399;
  }

 // area_color = COL_YELLOW;

//  edge_minh = x2 - edge_maxhgt; // lowest/leftmost
//  edge_maxh = x2 - edge_minhgt; // highest/rightmost
  edge_minh = x1 + edge_minhgt; // lowest/leftmost
  edge_maxh = x1 + edge_maxhgt; // highest/rightmost
  ASSERT( edge_minh < edge_maxh );

  for ( x=x1 ; x<=x2 ; x+=4 )
  {
    DRAW_PT( index, x, y1, COL_RED ); // 0xFFRRGGBB
    DRAW_PT( index, x, y2, COL_RED ); // 0xFFRRGGBB
  }
  for ( y=y1 ; y<=y2 ; y+=4 )
  {
    DRAW_PT( index, x1, y, COL_RED ); // 0xFFRRGGBB
    DRAW_PT( index, x2, y, COL_RED ); // 0xFFRRGGBB

    DRAW_PT( index, edge_maxh, y, COL_LIGHTORANGE ); // 0xFFRRGGBB
    DRAW_PT( index, edge_minh, y, COL_LIGHTORANGE ); // 0xFFRRGGBB

  }

  //y = 147;
  //Analyze_segment_y( y, x1, x2 );
  //Analyze_segment_y( 253, x1, x2 );


  edge_npt = 0;

  for ( y=y1 ; y<=y2 ; y+=2 )
  {
    Analyze_segment_y( y, x1, x2 );
  } 

  if ( edge_npt<edge_minnpt )
  {
    txt3.Format( string_2660[lng], edge_npt, edge_minnpt );
    col3 = TXT_DARKRED;
    return( 200 );
  }
  txt3.Format( string_2662[lng], edge_npt, edge_minnpt ); 
  col3 = TXT_DARKGREEN;



  //{
  /*  for ( x=x1+1 ; x<x2 ; x++ )
    {

      pn = FN(x,y); // present number

      TRACE("%d,%d\n",x,pn);

      po = pn; */
      
      /*int avg = PT(x-1,y-1)+PT(x,y-1)+PT(x+1,y-1)+
                PT(x-1,y  )+PT(x,y  )+PT(x+1,y  )+
                PT(x-1,y+1)+PT(x,y+1)+PT(x+1,y+1);

      TRACE("%d,%d,%d\n",x,PT(x,y),avg/9); */

     /* if ( pn>=on-3 ) // growing
      {
        if ( mode==0 ) // change falling-growing - MINIMUM
        {
          TRACE("%d,%d - %d -  %d,%d MINIMUM\n",x,PT(x,y),mode,ngrw,nred);
          mode = 1;
          ngrw = 1;
          nred = 0;
        }
        else ngrw++;
      }
      else // falling
      {
        if ( mode==1 ) // change growing-falling - MAXIMUM
        {
          TRACE("%d,%d - %d -  %d,%d MAXIMUM\n",x,PT(x,y),mode,ngrw,nred);
          mode = 0;
          nred = 1;
        }
        else nred++;
      } */
     //TRACE("%d,%d - %d -  %d,%d\n",x,PT(x,y),mode,ngrw,nred);
    //}
  //}

  return 0;
}


#define FN(x,y) PT(x-1,y-1)+PT(x,y-1)+PT(x+1,y-1)+ \
                PT(x-1,y  )+PT(x,y  )+PT(x+1,y  )+ \
                PT(x-1,y+1)+PT(x,y+1)+PT(x+1,y+1)



// Analyze one segment at y between x1/x2 coords
// third algorithm based on min first, then before/after max search
int CTip_inspection::Analyze_segment_y( int y, int x1, int x2 )
{
  ASSERT( 0<y && y<dimy );
  ASSERT( 0<x1 && x1<x2 && x2<dimx-1 );

  int trace = 0; // 0=nothing - 1=data - 2=analysis
  //trace = 2;

  if ( trace ) TRACE("\nAnalyze_segment_y  y=%d x1=%d x2=%d\n", y, x1, x2 );

  int ninc = 0; // n. of increasing
  int ndec = 0; // n. of decreasing
  int mode = 0; // 1=increasing / 2=decreasing

  int vn   = 0; // FN(x1+1,y); // new value

  int v1   = FN(x1  ,y); // first  value
  int v2   = FN(x1+1,y); // second value
  int v3   = 0; //FN(x1+2,y); // third  value

  int x    = x1+2;

  int vmin =  0;
  int imin =  0;
  int vmax =  0;
  int imax =  0;

  int smax =  0;

  int vml = 0;
  int vmc = 0;
  int vmr = 0;
  int xml = 0;
  int xmc = 0;
  int xmr = 0;


 // int* val = (int*) malloc( sizeof(int) * x2-x1+1 );
 // ASSERT(val);

  if ( trace==1 ) for ( x=x1 ; x<=x2 ; x++ ) TRACE("%d,%d\n",x, FN(x,y) );


  while ( x<=x2 )
  {
    v3 = FN(x,y);

    if ( trace==2 ) TRACE("%d,%d,%d\n",v1,v2,v3);

    if ( v2>=v1 && v2>v3 ) // MAX
    {
      if ( trace==2 ) TRACE("MAX %d %d,%d,%d\n",x-1,v1,v2,v3);

      // check how good is this max-min-max
      int score = vmax-vmin + v2-vmin; // left/right delta

//      if ( score>smax ) // new best
      if ( score>smax && vmax-vmin>edge_minside && v2-vmin>edge_minside ) // new best
      {
        smax = score;
        vml  = vmax;  // left   max value
        xml  = imax;
        vmc  = vmin;  // center min value
        xmc  = imin;
        vmr  =   v2;  // right  max value
        xmr  =  x-1;
        if ( trace==2 ) TRACE("NEW MAX-MIN-MAX: %d,%d - %d,%d - %d,%d score=%d\n", xml, vml, xmc, vmc, xmr, vmr, smax );
      }

      vmax = v2;
      imax = x-1;

    }

    if ( v2<v1 && v2<=v3 ) // MIN
    {
      if ( trace==2 ) TRACE("MIN %d %d,%d,%d\n",x-1,v1,v2,v3);
      vmin = v2;
      imin = x-1;
    }


    v1 = v2;
    v2 = v3;
    x++;
  }

  if ( trace ) TRACE("BEST MAX-MIN-MAX: %d,%d - %d,%d - %d,%d score=%d\n", xml, vml, xmc, vmc, xmr, vmr, smax );

  if ( smax>edge_minval && xml && xmc && xmr ) // acceptance threshold for max-min-max
  {
    ASSERT( xml && xmc && xmr );
    ASSERT( vml && vmc && vmr );

    ASSERT( xml<xmc && xmc<xmr );
    ASSERT( vml>vmc && vmr>vmc ); 
    

    /*  OLD CODE
    // search right xmr where val reaches/exceeds vml
    x = xmc+1;
    v1 = FN(x,y);
    x++;

    while ( x<xmr && v1<vml )
    {
      v1 = FN(x,y);
      x++;
    }

    vmr = v1;
    xmr = x-1;
    */

    // NEW - SEARCH FROM MIN LEFT/RIGHT UP <edge_minside>
    
    if ( edge_minside ) // if edge_minside is bigger than 0 correct left and right edges, otherwise leave them unchanged
    {
      int thresh = vmc+edge_minside;

      // left side
      x  = xmc-1;
      v1 = FN(x,y);
      while ( x>xml )
      {
        if ( v1>=thresh )
        {
          xml = x;
          vml = v1;
          break;
        }
        x--;
        v1 = FN(x,y);
      }

      // right side
      x  = xmc+1;
      v1 = FN(x,y);
      while ( x<xmr )
      {
        if ( v1>=thresh )
        {
          xmr = x;
          vmr = v1;
          break;
        }
        x++;
        v1 = FN(x,y);
      }

    }

    // check height
    if ( xmr>edge_maxh || xml<edge_minh ) // wrong height
    {
      DRAW_PT(index,xml,y,COL_MAGENTA);
      DRAW_PT(index,xmr,y,COL_MAGENTA);
    }
    else // height ok, check width
    {

      if ( xmr-xml>=edge_minwdt && xmr-xml<=edge_maxwdt ) // widht is acceptable
      {
        DRAW_PT(index,xml,y,COL_GREEN);
        //DRAW_PT(index,xmc,y,COL_BLUE );
        DRAW_PT(index,xmr,y,COL_GREEN);
        edge_npt++;
      }
      else // width is too large or narrow
      {
        DRAW_PT(index,xml,y,COL_RED);
        //DRAW_PT(index,xmc,y,COL_BLUE );
        DRAW_PT(index,xmr,y,COL_RED);
      }

    }

    if ( trace ) TRACE("ADJUSTED MAX-MIN-MAX: %d,%d - %d,%d - %d,%d score=%d\n", xml, vml, xmc, vmc, xmr, vmr, smax );
  }




  /*
  for ( x=x1 ; x<=x2 ; x++ )
  {
    val[x-x1] = FN(x,y);
    TRACE("val[%3d] = %4d\n",x-x1, val[x-x1] );
    TRACE("%d,%d\n",x-x1, val[x-x1] );
  }
  */


//  free( val );



  return 0;
}







/*
// Analyze one segment at y x1/x2 coords
int CTip_inspection::Analyze_segment_y( int y, int x1, int x2 )
{
  ASSERT( 0<y && y<dimy );
  ASSERT( 0<x1 && x1<x2 && x2<dimx-1 );

  int ninc = 0; // n. of increasing
  int ndec = 0; // n. of decreasing
  int mode = 0; // 1=increasing / 2=decreasing

  int vn   = FN(x1+1,y); // new value
  int vo   = FN(x1  ,y); // old value

  int x    = x1;
  int imin =  x;
  int imax =  x;
  int vmin = vo;
  int vmax = vo;



  if ( vn>vo )
  {
    mode = 1; // increasing
    ninc = 1;
    ndec = 0;
  }

  if ( vn<vo )
  {
    mode = 2; // decreasing
    ninc = 0;
    ndec = 1;
  }

  vo = vn;

  for ( x=x1+2 ; x<=x2 ; x++ )
  {

    vn = FN(x,y); // present number

//    TRACE("%d,%d\n",x,vn);

    switch ( mode )
    {
      case 1: // increasing
      if ( vn>=vo ) // still increasing
      {
        ninc++;
      }
      else // reverse: decreasing
      {
        // check if this is the second max in max-min-max sequence
        //if ( vo-vmin > 400 && vmax-vmin>100 )
        // check when value exceeds the previous maximum
        if ( vn>=vmax )
        {
          DRAW_PT(index,imin,y,COL_BLUE );
          DRAW_PT(index,imax,y,COL_GREEN);
          DRAW_PT(index,x-1 ,y,COL_GREEN);
        }


        mode = 2;  // change mode
        vmax = vo;
        imax = x-1;
        //TRACE("MAX %4d found at %3d %d/%d\n", vmax, imax, ninc, ndec);
        ninc = 0;
        ndec = 1;
      }
      break;

      case 2: // decreasing
      if ( vn<=vo ) // still decreasing
      {
        ndec++;
      }
      else // reverse: increasing
      {
        mode = 1;  // change mode
        vmin = vo;
        imin = x-1;
        //TRACE("MIN %4d found at %3d %d/%d\n", vmin, imin, ninc, ndec);
        ninc = 1;
        ndec = 0;
      }
      break;

    }


    vo = vn; 
      
  }


  return 0;
}
*/



/*
// Analyze one segment at y between x1/x2 coords
// second algorithm with sequence max-min-max search
int CTip_inspection::Analyze_segment_y( int y, int x1, int x2 )
{
  ASSERT( 0<y && y<dimy );
  ASSERT( 0<x1 && x1<x2 && x2<dimx-1 );

  int ninc = 0; // n. of increasing
  int ndec = 0; // n. of decreasing
  int mode = 0; // 1=increasing / 2=decreasing

  int vn   = 0; // FN(x1+1,y); // new value

  int vo   = FN(x1  ,y); // old value

  int x    = x1+1;

  int imin =  x;
  int imax =  x;
  int vmin = vo;
  int vmax = vo;


  // phase A - search first maximum
  while ( x<=x2 )
  {
    vn = FN(x,y); // present number

    TRACE("(A) vn/vo %d/%d ninc=%d\n", vn, vo, ninc );

    if ( vn>=vo ) // increasing
    {
      ninc++;
    }
    else // decreasing
    {
      if ( ninc > 6 )
      {
        vmax = vo;
        imax = x-1;
        TRACE("PRIMO MAX %d AT %d\n", vmax, imax);
        break;
      }
      ninc=0; // reset ninc
    }

    vo = vn;
    x++;
  }

  if ( x>x2 ) return 99; // failed


  // phase B - search first minimum
  while ( x<=x2 )
  {
    vn = FN(x,y); // present number

    TRACE("(B) vn/vo %d/%d ndec=%d\n", vn, vo, ndec );

    if ( vn>vo ) // increasing
    {
      if ( ndec > 4 )
      {
        vmin = vo;
        imin = x-1;
        TRACE("PRIMO MIN %d AT %d\n", vmin, imin);
        break;
      }
      ndec=0;
    }
    else // decreasing
    {
      ndec++; // increade ndec
    }

    vo = vn;
    x++;
  }

  if ( x>x2 ) return 99; // failed


  // phase C - search when value reaches again first vmax
  while ( x<=x2 )
  {
    vn = FN(x,y); // present number

    TRACE("(C) vn/vo %d/%d\n", vn, vo );

    if ( vn > vmax )
    {
      //vmax = vo;
      //imax = x-1;
      TRACE("SECONDO MAX %d AT %d\n", vn, x);
      break;
    }

//    vo = vn;
    x++;
  }

  if ( x>x2 ) return 99; // failed

  DRAW_PT(index,imax,y,COL_GREEN);
  DRAW_PT(index,imin,y,COL_BLUE );
  DRAW_PT(index,x   ,y,COL_GREEN);


  return 0;
}

*/







int CTip_inspection::Analyze_shoulder( void )
{
  int i;

  RECT box;
  box.right  =     0;
  box.bottom =     0;
  box.left   = 10000;
  box.top    = 10000;

  //TRACE("SHOULDER %d\n",index);

  int error = 0;

  for ( i=AMPATN ; i<shl_height-AMPATN ; i++ )
  {
    if ( ptx[i]>box.right  ) box.right  = ptx[i];
    if ( ptx[i]<box.left   ) box.left   = ptx[i];
    if ( pty[i]>box.bottom ) box.bottom = pty[i];
    if ( pty[i]<box.top    ) box.top    = pty[i];

    //TRACE( "%d,%d\n", i, ATAN(i) );

//    if ( (36000-ATAN(i))<shl_minslp*100 || (36000-ATAN(i))>shl_maxslp*100 )
    if ( (36000-ATAN(i))<shl_minslp*100 || (36000-ATAN(i))>shl_maxslp*100 )
    {
      error++;
      if ( bl_drawovl ) DRAW_PT( index,ptx[i],pty[i],COL_RED  ); //
    }
    else
    {
      if ( bl_drawovl ) DRAW_PT( index,ptx[i],pty[i],COL_GREEN ); // 
    }

  }


  for ( i=ptn-AMPATN ; i>ptn-shl_height+AMPATN ; i-- )
  {
    if ( ptx[i]>box.right  ) box.right  = ptx[i];
    if ( ptx[i]<box.left   ) box.left   = ptx[i];
    if ( pty[i]>box.bottom ) box.bottom = pty[i];
    if ( pty[i]<box.top    ) box.top    = pty[i];

    //TRACE( "%d,%d\n", i, ATAN(i) );

//    if ( ATAN(i)<shl_minslp*100 || ATAN(i)>shl_maxslp*100 )
    if ( ATAN(i)<shl_minslp*100 || ATAN(i)>shl_maxslp*100 )
    {
      error++;
      if ( bl_drawovl ) DRAW_PT( index,ptx[i],pty[i],COL_RED   ); //
    }
    else
    {
      if ( bl_drawovl ) DRAW_PT( index,ptx[i],pty[i],COL_GREEN ); // 
    }
  }

  #ifdef FD_DATAREC
    //if ( datarec_on && cap_result ) cap_result->InsertI( error, index+16 ); // insert shoulder error into 16-17-18 cat 
  #endif

  if ( error>shl_maxerr )
  {
    txt1.Format( string_1932[lng], error, shl_maxerr );
    return( 200 );
  }

  txt1.Format( string_1930[lng], error, shl_maxerr );
  return 0;
}








int CTip_inspection::Analyze_keypoints_fins( void )
{
  int i;

  for (i=PA ; i<=PE ; i++ )
  {
    if ( i!=PB && i!=PC )
    {
      ASSERT( kpts[i][LEFT ]>10 && kpts[i][LEFT ]<ptn-10 );
      ASSERT( kpts[i][RIGHT]>10 && kpts[i][RIGHT]<ptn-10 );
    }
  }



  /*
  segment_type segment;

  int il = kpts[PA][LEFT ];
  int ir = kpts[PA][RIGHT];
  TRACE("A Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PB][LEFT ];
  ir = kpts[PB][RIGHT];
  TRACE("B Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PC][LEFT ];
  ir = kpts[PC][RIGHT];
  TRACE("C Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PD][LEFT ];
  ir = kpts[PD][RIGHT];
  TRACE("D Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PE][LEFT ];
  ir = kpts[PE][RIGHT];
  TRACE("E Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );
  */

  // first clear found model
  ZeroMemory( model_fnd, sizeof(model_fnd) );


  Segment_data( kpts[PA][LEFT ], kpts[PA][RIGHT], &model_fnd[0] ); // AL-AR
//  Segment_data( kpts[PB][LEFT ], kpts[PB][RIGHT], &model_fnd[1] ); // BL-BR

// SKIP  Segment_data( kpts[PD][LEFT ], kpts[PC][LEFT ], &model_fnd[2] ); // CL-DL
// SKIP  Segment_data( kpts[PC][RIGHT], kpts[PD][RIGHT], &model_fnd[3] ); // CR-DR
  Segment_data( kpts[PE][LEFT ], kpts[PD][LEFT ], &model_fnd[4] ); // DL-EL
  Segment_data( kpts[PD][RIGHT], kpts[PE][RIGHT], &model_fnd[5] ); // DR-ER


  BOOL reject = FALSE;


  // CHECK HEIGHT OF POINTS AL / AR
  if ( htot_max && htot_min ) // IF either==0 height control is disabled
  {
    int y;

    // A LEFT 
    y = ptx[kpts[PA][LEFT]];
    if ( y < htot_min ) // REJECT: A left too low
    {
      reject = TRUE;
      txt1.Format( string_770[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A left too high
    {
      reject = TRUE;
      txt1.Format( string_772[lng], L"A", y, htot_min, htot_max );
    }


    // A RIGHT
    y = ptx[kpts[PA][RIGHT]];
    if ( y < htot_min ) // REJECT: A right too low
    {
      reject = TRUE;
      txt1.Format( string_774[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A right too high
    {
      reject = TRUE;
      txt1.Format( string_776[lng], L"A",  y, htot_min, htot_max );
    }

    UINT color = reject?0xFFAA0000:0xFF00AA00; // 0xAARRGGBB

    if ( bl_drawovl ) 
    {
      for ( y=ymin ; y<=ymax ; y+=4 )
      {
        DRAW_PT( index, htot_max, y, color ); // 0xAARRGGBB
        DRAW_PT( index, htot_min, y, color ); // 0xAARRGGBB
      }
    }

    if ( reject ) return( 88 );

  }



  wchar_t txt[30+1];
  if ( 0 ) // learning )
  {
    // clear reference model
    //ZeroMemory( model_ref, sizeof(model_ref) );
    CopyMemory( model_ref, model_fnd, sizeof(model_ref) );
    txt2.Format( L"Model acquired" );
    TRACE("REFERENCE MODEL LEARNT\n");
  }
  else // processing
  {
    //TRACE("START MODELS COMPARISON\n");
    err_modtot = 0;
    //txt2.Format( L"Model: " );
    txt2 = L""; //.Format( L"Lenghts: " );


    /* SKIP
    // CD LEFT
    if ( model_fnd[2].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too short %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_780[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too long %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_782[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left perc too high %d (%d)",  model_fnd[2].perc, CD_percmax );
      txt1.Format( string_784[lng], L"CD", model_fnd[2].perc, CD_percmax );
    }

    // CD RIGHT
    if ( model_fnd[3].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too short %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_790[lng], L"CD", model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too long %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_792[lng], L"CD",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right perc too high %d (%d)",  model_fnd[3].perc, CD_percmax );
      txt1.Format( string_794[lng], L"CD", model_fnd[3].perc, CD_percmax );
    }

    */



    // DE LEFT
    if ( model_fnd[4].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too short %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_780[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too long %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_782[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left perc too high %d (%d)",  model_fnd[4].perc, DE_percmax );
      txt1.Format( string_784[lng], L"DE", model_fnd[4].perc, DE_percmax );
    }

    if ( model_fnd[4].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too low %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_796[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[4].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too high %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_797[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }


    // DE RIGHT
    if ( model_fnd[5].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too short %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_790[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too long %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_792[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right perc too high %d (%d)",  model_fnd[5].perc, DE_percmax );
      txt1.Format( string_794[lng], L"DE", model_fnd[5].perc, DE_percmax );
    }

    if ( model_fnd[5].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too low %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_798[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[5].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too high %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_799[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }




    for ( i=0 ; i<6 ; i++ ) // compare fnd and ref models
    {
      if ( i!=1 && i!=2 && i!=3 )
      {
        //int perc = 0;
        //if ( model_fnd[i].lenght ) perc = 100 * model_fnd[i].ctlenght / model_fnd[i].lenght - 100;
//        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].ctlenght, model_fnd[i].angle );
        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].perc, model_fnd[i].angle );
        txt2.Append( txt );
      }
    }
     
    if ( reject ) return( 99 );
  } 

  return 0;
}




int CTip_inspection::Analyze_keypoints_v30( void )
{
  int i;

  int i1; 
  int i2; 
  segment_type segment;
  
  i1 = kpts[PA][LEFT ];
  i2 = kpts[PB][LEFT ];
  Segment_data( i1, i2, &segment );
  //TRACE("AB LEFT  Points: %4d/%4d (%3d,%3d)-(%3d,%3d)\n", i1, i2, ptx[i1], pty[i1], ptx[i2], pty[i2] );
  //TRACE("lenght=%4d ctlenght=%4d perc=%4d ang=%4d\n", segment.lenght, segment.ctlenght, segment.perc, segment.angle );

  if ( segment.lenght<AB_lenghtmin ) // LEFT AB TOO SHORT
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"AB", string_tooshort[lng], segment.lenght, AB_lenghtmin, AB_lenghtmax );
    return 1100;
  }

  if ( segment.lenght>AB_lenghtmax ) // LEFT AB TOO LONG
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"AB", string_toolong[lng], segment.lenght, AB_lenghtmin, AB_lenghtmax );
    return 1102;
  }

  if ( segment.perc>AB_percmax ) // LEFT AB PERCENTAGE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"AB", string_perctoohigh[lng], segment.perc, 0, AB_percmax );
    return 1104;
  }

  if ( segment.angle<AB_angmin ) // LEFT AB ANGLE TOO LOW
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"AB", string_angletoolow[lng], segment.angle, AB_angmin, AB_angmax );
    return 1106;
  }

  if ( segment.angle>AB_angmax ) // LEFT AB ANGLE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"AB", string_angletoohigh[lng], segment.angle, AB_angmin, AB_angmax );
    return 1108;
  }

  ABL_txt.Format( L"%d,%d,%d", segment.lenght, segment.perc, segment.angle );


  i1 = kpts[PA][RIGHT];
  i2 = kpts[PB][RIGHT];
  Segment_data( i2, i1, &segment );
  //TRACE("AB RIGHT Points: %4d/%4d (%3d,%3d)-(%3d,%3d)\n", i1, i2, ptx[i1], pty[i1], ptx[i2], pty[i2] );
  //TRACE("lenght=%4d ctlenght=%4d perc=%4d ang=%4d\n", segment.lenght, segment.ctlenght, segment.perc, segment.angle );

  if ( segment.lenght<AB_lenghtmin ) // RIGHT AB TOO SHORT
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"AB", string_tooshort[lng], segment.lenght, AB_lenghtmin, AB_lenghtmax );
    return 1200;
  }

  if ( segment.lenght>AB_lenghtmax ) // RIGHT AB TOO LONG
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"AB", string_toolong[lng], segment.lenght, AB_lenghtmin, AB_lenghtmax );
    return 1202;
  }

  if ( segment.perc>AB_percmax ) // RIGHT AB PERCENTAGE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"AB", string_perctoohigh[lng], segment.perc, 0, AB_percmax );
    return 1204;
  }

  if ( segment.angle<AB_angmin ) // RIGHT AB ANGLE TOO LOW
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"AB", string_angletoolow[lng], segment.angle, AB_angmin, AB_angmax );
    return 1206;
  }

  if ( segment.angle>AB_angmax ) // RIGHT AB ANGLE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"AB", string_angletoohigh[lng], segment.angle, AB_angmin, AB_angmax );
    return 1208;
  }

  ABR_txt.Format( L"%d,%d,%d", segment.lenght, segment.perc, segment.angle );


  i1 = kpts[PB][LEFT ];
  i2 = kpts[PC][LEFT ];
  Segment_data( i1, i2, &segment );
  //TRACE("BC LEFT  Points: %4d/%4d (%3d,%3d)-(%3d,%3d)\n", i1, i2, ptx[i1], pty[i1], ptx[i2], pty[i2] );
  //TRACE("lenght=%4d ctlenght=%4d perc=%4d ang=%4d\n", segment.lenght, segment.ctlenght, segment.perc, segment.angle );

  if ( segment.lenght<BC_lenghtmin ) // LEFT BC TOO SHORT
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"BC", string_tooshort[lng], segment.lenght, BC_lenghtmin, BC_lenghtmax );
    return 1300;
  }

  if ( segment.lenght>BC_lenghtmax ) // LEFT BC TOO LONG
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"BC", string_toolong[lng], segment.lenght, BC_lenghtmin, BC_lenghtmax );
    return 1302;
  }

  if ( segment.perc>BC_percmax ) // LEFT BC PERCENTAGE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"BC", string_perctoohigh[lng], segment.perc, 0, BC_percmax );
    return 1304;
  }

  if ( segment.angle<BC_angmin ) // LEFT BC ANGLE TOO LOW
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"BC", string_angletoolow[lng], segment.angle, BC_angmin, BC_angmax );
    return 1306;
  }

  if ( segment.angle>BC_angmax ) // LEFT BC ANGLE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_left[lng], L"BC", string_angletoohigh[lng], segment.angle, BC_angmin, BC_angmax );
    return 1308;
  }

  BCL_txt.Format( L"%d,%d,%d", segment.lenght, segment.perc, segment.angle );


  i1 = kpts[PB][RIGHT];
  i2 = kpts[PC][RIGHT];
  Segment_data( i2, i1, &segment );
  //TRACE("BC LEFT  Points: %4d/%4d (%3d,%3d)-(%3d,%3d)\n", i1, i2, ptx[i1], pty[i1], ptx[i2], pty[i2] );
  //TRACE("lenght=%4d ctlenght=%4d perc=%4d ang=%4d\n", segment.lenght, segment.ctlenght, segment.perc, segment.angle );

  if ( segment.lenght<BC_lenghtmin ) // RIGHT BC TOO SHORT
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"BC", string_tooshort[lng], segment.lenght, BC_lenghtmin, BC_lenghtmax );
    return 1300;
  }

  if ( segment.lenght>BC_lenghtmax ) // RIGHT BC TOO LONG
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"BC", string_toolong[lng], segment.lenght, BC_lenghtmin, BC_lenghtmax );
    return 1302;
  }

  if ( segment.perc>BC_percmax ) // RIGHT BC PERCENTAGE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"BC", string_perctoohigh[lng], segment.perc, 0, BC_percmax );
    return 1304;
  }

  if ( segment.angle<BC_angmin ) // RIGHT BC ANGLE TOO LOW
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"BC", string_angletoolow[lng], segment.angle, BC_angmin, BC_angmax );
    return 1306;
  }

  if ( segment.angle>BC_angmax ) // RIGHT BC ANGLE TOO HIGH
  {
    txt1.Format( string_rejectv30[lng], string_right[lng], L"BC", string_angletoohigh[lng], segment.angle, BC_angmin, BC_angmax );
    return 1308;
  }

  BCR_txt.Format( L"%d,%d,%d", segment.lenght, segment.perc, segment.angle );

  return( 0 ); // disabled now

  /*
  for (i=PA ; i<=PE ; i++ )
  {
    ASSERT( kpts[i][LEFT ]>10 && kpts[i][LEFT ]<ptn-10 );
    ASSERT( kpts[i][RIGHT]>10 && kpts[i][RIGHT]<ptn-10 );
  }
  */


  /*
  segment_type segment;

  int il = kpts[PA][LEFT ];
  int ir = kpts[PA][RIGHT];
  TRACE("A Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PB][LEFT ];
  ir = kpts[PB][RIGHT];
  TRACE("B Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PC][LEFT ];
  ir = kpts[PC][RIGHT];
  TRACE("C Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PD][LEFT ];
  ir = kpts[PD][RIGHT];
  TRACE("D Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PE][LEFT ];
  ir = kpts[PE][RIGHT];
  TRACE("E Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );
  */

  // first clear found model
  ZeroMemory( model_fnd, sizeof(model_fnd) );


  Segment_data( kpts[PA][LEFT ], kpts[PA][RIGHT], &model_fnd[0] ); // AL-AR
//  Segment_data( kpts[PB][LEFT ], kpts[PB][RIGHT], &model_fnd[1] ); // BL-BR

  Segment_data( kpts[PD][LEFT ], kpts[PC][LEFT ], &model_fnd[2] ); // CL-DL
  Segment_data( kpts[PC][RIGHT], kpts[PD][RIGHT], &model_fnd[3] ); // CR-DR
  Segment_data( kpts[PE][LEFT ], kpts[PD][LEFT ], &model_fnd[4] ); // DL-EL
  Segment_data( kpts[PD][RIGHT], kpts[PE][RIGHT], &model_fnd[5] ); // DR-ER


  BOOL reject = FALSE;


  // CHECK HEIGHT OF POINTS AL / AR
  if ( htot_max && htot_min ) // IF either==0 height control is disabled
  {
    int y;

    // A LEFT 
    y = ptx[kpts[PA][LEFT]];
    if ( y < htot_min ) // REJECT: A left too low
    {
      reject = TRUE;
      txt1.Format( string_770[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A left too high
    {
      reject = TRUE;
      txt1.Format( string_772[lng], L"A", y, htot_min, htot_max );
    }


    // A RIGHT
    y = ptx[kpts[PA][RIGHT]];
    if ( y < htot_min ) // REJECT: A right too low
    {
      reject = TRUE;
      txt1.Format( string_774[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A right too high
    {
      reject = TRUE;
      txt1.Format( string_776[lng], L"A",  y, htot_min, htot_max );
    }

    UINT color = reject?0xFFAA0000:0xFF00AA00; // 0xAARRGGBB

    if ( bl_drawovl ) 
    {
      for ( y=ymin ; y<=ymax ; y+=4 )
      {
        DRAW_PT( index, htot_max, y, color ); // 0xAARRGGBB
        DRAW_PT( index, htot_min, y, color ); // 0xAARRGGBB
      }
    }

    if ( reject ) return( 88 );

  }



  wchar_t txt[30+1];
  if ( 0 ) // learning )
  {
    // clear reference model
    //ZeroMemory( model_ref, sizeof(model_ref) );
    CopyMemory( model_ref, model_fnd, sizeof(model_ref) );
    txt2.Format( L"Model acquired" );
    TRACE("REFERENCE MODEL LEARNT\n");
  }
  else // processing
  {
    //TRACE("START MODELS COMPARISON\n");
    err_modtot = 0;
    //txt2.Format( L"Model: " );
    txt2 = L""; //.Format( L"Lenghts: " );


    // CD LEFT
    if ( model_fnd[2].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too short %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_780[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too long %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_782[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left perc too high %d (%d)",  model_fnd[2].perc, CD_percmax );
      txt1.Format( string_784[lng], L"CD", model_fnd[2].perc, CD_percmax );
    }

    // CD RIGHT
    if ( model_fnd[3].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too short %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_790[lng], L"CD", model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too long %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_792[lng], L"CD",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right perc too high %d (%d)",  model_fnd[3].perc, CD_percmax );
      txt1.Format( string_794[lng], L"CD", model_fnd[3].perc, CD_percmax );
    }



    // DE LEFT
    if ( model_fnd[4].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too short %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_780[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too long %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_782[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left perc too high %d (%d)",  model_fnd[4].perc, DE_percmax );
      txt1.Format( string_784[lng], L"DE", model_fnd[4].perc, DE_percmax );
    }

    if ( model_fnd[4].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too low %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_796[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[4].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too high %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_797[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }


    // DE RIGHT
    if ( model_fnd[5].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too short %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_790[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too long %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_792[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right perc too high %d (%d)",  model_fnd[5].perc, DE_percmax );
      txt1.Format( string_794[lng], L"DE", model_fnd[5].perc, DE_percmax );
    }

    if ( model_fnd[5].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too low %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_798[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[5].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too high %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_799[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }




    for ( i=0 ; i<6 ; i++ ) // compare fnd and ref models
    {
      if ( i!=1 )
      {
        //int perc = 0;
        //if ( model_fnd[i].lenght ) perc = 100 * model_fnd[i].ctlenght / model_fnd[i].lenght - 100;
//        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].ctlenght, model_fnd[i].angle );
        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].perc, model_fnd[i].angle );
        txt2.Append( txt );
      }
    }
     

    if ( reject ) return( 99 );
    
/*    if ( model_ref[i].lenght>0 )
      {
        //ASSERT( model_fnd[i].lenght>0 );
        //ASSERT( model_ref[i].lenght>0 );

        /*
        // compare lenght
        int rapp = 100 * model_fnd[i].lenght / model_ref[i].lenght;
        //TRACE("lenght[%d]: fact=%3.1lf\n", i,rapp);
        err_modtot += abs(rapp-100);
        swprintf( txt, 30, L"%d ", rapp );
        txt2.Append( txt );
        */
        // compare angle
 /*     }

    }   */
    //swprintf( txt, 30, L"T=%d", err_modtot );
    //txt2.Append( txt );


  } 

  return 0;
}





int CTip_inspection::Analyze_keypoints( void )
{
  int i;

  for (i=PA ; i<=PE ; i++ )
  {
    ASSERT( kpts[i][LEFT ]>10 && kpts[i][LEFT ]<ptn-10 );
    ASSERT( kpts[i][RIGHT]>10 && kpts[i][RIGHT]<ptn-10 );
  }



  /*
  segment_type segment;

  int il = kpts[PA][LEFT ];
  int ir = kpts[PA][RIGHT];
  TRACE("A Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PB][LEFT ];
  ir = kpts[PB][RIGHT];
  TRACE("B Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PC][LEFT ];
  ir = kpts[PC][RIGHT];
  TRACE("C Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PD][LEFT ];
  ir = kpts[PD][RIGHT];
  TRACE("D Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );

  il = kpts[PE][LEFT ];
  ir = kpts[PE][RIGHT];
  TRACE("E Points: %4d (%3d,%3d) - %4d (%3d,%3d)\n", il, ptx[il], pty[il], ir, ptx[ir], pty[ir] );
  Segment_data( ir, il, &segment );
  */

  // first clear found model
  ZeroMemory( model_fnd, sizeof(model_fnd) );


  Segment_data( kpts[PA][LEFT ], kpts[PA][RIGHT], &model_fnd[0] ); // AL-AR
//  Segment_data( kpts[PB][LEFT ], kpts[PB][RIGHT], &model_fnd[1] ); // BL-BR

  Segment_data( kpts[PD][LEFT ], kpts[PC][LEFT ], &model_fnd[2] ); // CL-DL
  Segment_data( kpts[PC][RIGHT], kpts[PD][RIGHT], &model_fnd[3] ); // CR-DR
  Segment_data( kpts[PE][LEFT ], kpts[PD][LEFT ], &model_fnd[4] ); // DL-EL
  Segment_data( kpts[PD][RIGHT], kpts[PE][RIGHT], &model_fnd[5] ); // DR-ER


  BOOL reject = FALSE;


  // CHECK HEIGHT OF POINTS AL / AR
  if ( htot_max && htot_min ) // IF either==0 height control is disabled
  {
    int y;

    // A LEFT 
    y = ptx[kpts[PA][LEFT]];
    if ( y < htot_min ) // REJECT: A left too low
    {
      reject = TRUE;
      txt1.Format( string_770[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A left too high
    {
      reject = TRUE;
      txt1.Format( string_772[lng], L"A", y, htot_min, htot_max );
    }


    // A RIGHT
    y = ptx[kpts[PA][RIGHT]];
    if ( y < htot_min ) // REJECT: A right too low
    {
      reject = TRUE;
      txt1.Format( string_774[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A right too high
    {
      reject = TRUE;
      txt1.Format( string_776[lng], L"A",  y, htot_min, htot_max );
    }

    UINT color = reject?0xFFAA0000:0xFF00AA00; // 0xAARRGGBB

    if ( bl_drawovl ) 
    {
      for ( y=ymin ; y<=ymax ; y+=4 )
      {
        DRAW_PT( index, htot_max, y, color ); // 0xAARRGGBB
        DRAW_PT( index, htot_min, y, color ); // 0xAARRGGBB
      }
    }

    if ( reject ) return( 88 );

  }



  wchar_t txt[30+1];
  if ( 0 ) // learning )
  {
    // clear reference model
    //ZeroMemory( model_ref, sizeof(model_ref) );
    CopyMemory( model_ref, model_fnd, sizeof(model_ref) );
    txt2.Format( L"Model acquired" );
    TRACE("REFERENCE MODEL LEARNT\n");
  }
  else // processing
  {
    //TRACE("START MODELS COMPARISON\n");
    err_modtot = 0;
    //txt2.Format( L"Model: " );
    txt2 = L""; //.Format( L"Lenghts: " );


    // CD LEFT
    if ( model_fnd[2].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too short %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_780[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left too long %d (%d,%d)",  model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_782[lng], L"CD", model_fnd[2].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[2].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD left perc too high %d (%d)",  model_fnd[2].perc, CD_percmax );
      txt1.Format( string_784[lng], L"CD", model_fnd[2].perc, CD_percmax );
    }

    // CD RIGHT
    if ( model_fnd[3].lenght < CD_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too short %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_790[lng], L"CD", model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].lenght > CD_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right too long %d (%d,%d)",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
      txt1.Format( string_792[lng], L"CD",  model_fnd[3].lenght, CD_lenghtmin, CD_lenghtmax );
    }

    if ( model_fnd[3].perc > CD_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: CD right perc too high %d (%d)",  model_fnd[3].perc, CD_percmax );
      txt1.Format( string_794[lng], L"CD", model_fnd[3].perc, CD_percmax );
    }



    // DE LEFT
    if ( model_fnd[4].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too short %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_780[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left too long %d (%d,%d)",  model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_782[lng], L"DE", model_fnd[4].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[4].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left perc too high %d (%d)",  model_fnd[4].perc, DE_percmax );
      txt1.Format( string_784[lng], L"DE", model_fnd[4].perc, DE_percmax );
    }

    if ( model_fnd[4].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too low %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_796[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[4].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE left angle too high %d (%d,%d)",  model_fnd[4].angle, DE_angmin, DE_angmax );
      txt1.Format( string_797[lng], L"DE", model_fnd[4].angle, DE_angmin, DE_angmax );
    }


    // DE RIGHT
    if ( model_fnd[5].lenght < DE_lenghtmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too short %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_790[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].lenght > DE_lenghtmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right too long %d (%d,%d)",  model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
      txt1.Format( string_792[lng], L"DE", model_fnd[5].lenght, DE_lenghtmin, DE_lenghtmax );
    }

    if ( model_fnd[5].perc > DE_percmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right perc too high %d (%d)",  model_fnd[5].perc, DE_percmax );
      txt1.Format( string_794[lng], L"DE", model_fnd[5].perc, DE_percmax );
    }

    if ( model_fnd[5].angle < DE_angmin )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too low %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_798[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }

    if ( model_fnd[5].angle > DE_angmax )
    {
      reject = TRUE;
      //txt1.Format( L"REJECT: DE right angle too high %d (%d,%d)",  model_fnd[5].angle, DE_angmin, DE_angmax );
      txt1.Format( string_799[lng], L"DE", model_fnd[5].angle, DE_angmin, DE_angmax );
    }




    for ( i=0 ; i<6 ; i++ ) // compare fnd and ref models
    {
      if ( i!=1 )
      {
        //int perc = 0;
        //if ( model_fnd[i].lenght ) perc = 100 * model_fnd[i].ctlenght / model_fnd[i].lenght - 100;
//        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].ctlenght, model_fnd[i].angle );
        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].perc, model_fnd[i].angle );
        txt2.Append( txt );
      }
    }
     

    if ( reject ) return( 99 );
    
/*    if ( model_ref[i].lenght>0 )
      {
        //ASSERT( model_fnd[i].lenght>0 );
        //ASSERT( model_ref[i].lenght>0 );

        /*
        // compare lenght
        int rapp = 100 * model_fnd[i].lenght / model_ref[i].lenght;
        //TRACE("lenght[%d]: fact=%3.1lf\n", i,rapp);
        err_modtot += abs(rapp-100);
        swprintf( txt, 30, L"%d ", rapp );
        txt2.Append( txt );
        */
        // compare angle
 /*     }

    }   */
    //swprintf( txt, 30, L"T=%d", err_modtot );
    //txt2.Append( txt );


  } 

  return 0;
}




int CTip_inspection::Analyze_keypoints_oral( void )
{
  int i;

  for (i=PA ; i<=PD ; i++ )
  {
    ASSERT( kpts[i][LEFT ]>10 && kpts[i][LEFT ]<ptn-10 );
    ASSERT( kpts[i][RIGHT]>10 && kpts[i][RIGHT]<ptn-10 );
  }

  // first clear found model
  ZeroMemory( model_fnd, sizeof(model_fnd) );

  Segment_data( kpts[PA][LEFT ], kpts[PA][RIGHT], &model_fnd[0] ); // AL-AR
  Segment_data( kpts[PD][LEFT ], kpts[PD][RIGHT], &model_fnd[1] ); // DL-DR
  Segment_data( kpts[PD][LEFT ], kpts[PA][LEFT ], &model_fnd[2] ); // AL-DL
  Segment_data( kpts[PA][RIGHT], kpts[PD][RIGHT], &model_fnd[3] ); // AR-DR  
  Segment_data( kpts[PC][LEFT ], kpts[PB][LEFT ], &model_fnd[4] ); // BL-CL
  Segment_data( kpts[PB][RIGHT], kpts[PC][RIGHT], &model_fnd[5] ); // BR-CR

  BOOL reject = FALSE;


  #define ALAR model_fnd[0]
  #define DLDR model_fnd[1]
  #define ALDL model_fnd[2]
  #define ARDR model_fnd[3]
  #define BLCL model_fnd[4]
  #define BRCR model_fnd[5]

  // CHECK HEIGHT OF POINTS AL / AR
  if ( htot_max && htot_min ) // IF either==0 height control is disabled
  {
    int y;

    // A LEFT 
    y = ptx[kpts[PA][LEFT]];
    if ( y < htot_min ) // REJECT: A left too low
    {
      reject = TRUE;
      txt1.Format( string_770[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A left too high
    {
      reject = TRUE;
      txt1.Format( string_772[lng], L"A", y, htot_min, htot_max );
    }

    // A RIGHT
    y = ptx[kpts[PA][RIGHT]];
    if ( y < htot_min ) // REJECT: A right too low
    {
      reject = TRUE;
      txt1.Format( string_774[lng], L"A", y, htot_min, htot_max );
    }

    if ( y > htot_max ) // REJECT: A right too high
    {
      reject = TRUE;
      txt1.Format( string_776[lng], L"A",  y, htot_min, htot_max );
    }

    UINT color = reject?0xFFAA0000:0xFF00AA00; // 0xAARRGGBB

    if ( bl_drawovl ) 
    {
      for ( y=ymin ; y<=ymax ; y+=4 )
      {
        DRAW_PT( index, htot_max, y, color ); // 0xAARRGGBB
        DRAW_PT( index, htot_min, y, color ); // 0xAARRGGBB
      }
    }

    if ( reject ) return( 88 );

  }



  wchar_t txt[30+1];
  if ( 0 ) // learning )
  {
    // clear reference model
    //ZeroMemory( model_ref, sizeof(model_ref) );
    CopyMemory( model_ref, model_fnd, sizeof(model_ref) );
    txt2.Format( L"Model acquired" );
    TRACE("REFERENCE MODEL LEARNT\n");
  }
  else // processing
  {
    //TRACE("START MODELS COMPARISON\n");
    //err_modtot = 0;
    //txt2.Format( L"Model: " );
    txt2 = L""; //.Format( L"Lenghts: " );


    // AD LEFT
    if ( ALDL.lenght < AD_lenghtmin )
    {
      reject = TRUE;
      txt1.Format( string_780[lng], L"ADL", ALDL.lenght, AD_lenghtmin, AD_lenghtmax );
    }

    if ( ALDL.lenght > AD_lenghtmax )
    {
      reject = TRUE;
      txt1.Format( string_782[lng], L"ADL", ALDL.lenght, AD_lenghtmin, AD_lenghtmax );
    }

    if ( ALDL.perc > AD_percmax )
    {
      reject = TRUE;
      txt1.Format( string_784[lng], L"ADL", ALDL.perc, AD_percmax );
    }

    // AD RIGHT
    if ( ARDR.lenght < AD_lenghtmin )
    {
      reject = TRUE;
      txt1.Format( string_790[lng], L"ADR", ARDR.lenght, AD_lenghtmin, AD_lenghtmax );
    }

    if ( ARDR.lenght > AD_lenghtmax )
    {
      reject = TRUE;
      txt1.Format( string_792[lng], L"ADR",  ARDR.lenght, AD_lenghtmin, AD_lenghtmax );
    }

    if ( ARDR.perc > AD_percmax )
    {
      reject = TRUE;
      txt1.Format( string_794[lng], L"ADR", ARDR.perc, AD_percmax );
    }



    // BC LEFT
    if ( BLCL.lenght < BC_lenghtmin )
    {
      reject = TRUE;
      txt1.Format( string_780[lng], L"BCL", BLCL.lenght, BC_lenghtmin, BC_lenghtmax );
    }

    if ( BLCL.lenght > BC_lenghtmax )
    {
      reject = TRUE;
      txt1.Format( string_782[lng], L"BCL", BLCL.lenght, BC_lenghtmin, BC_lenghtmax );
    }

    if ( BLCL.perc > BC_percmax )
    {
      reject = TRUE;
      txt1.Format( string_784[lng], L"BCL", BLCL.perc, BC_percmax );
    }

    if ( BLCL.angle < BC_angmin )
    {
      reject = TRUE;
      txt1.Format( string_796[lng], L"BCL", BLCL.angle, BC_angmin, BC_angmax );
    }

    if ( BLCL.angle > BC_angmax )
    {
      reject = TRUE;
      txt1.Format( string_797[lng], L"BCL", BLCL.angle, BC_angmin, BC_angmax );
    }


    // BC RIGHT
    if ( BRCR.lenght < BC_lenghtmin )
    {
      reject = TRUE;
      txt1.Format( string_790[lng], L"BCR", BRCR.lenght, BC_lenghtmin, BC_lenghtmax );
    }

    if ( BRCR.lenght > BC_lenghtmax )
    {
      reject = TRUE;
      txt1.Format( string_792[lng], L"BCR", BRCR.lenght, BC_lenghtmin, BC_lenghtmax );
    }

    if ( BRCR.perc > BC_percmax )
    {
      reject = TRUE;
      txt1.Format( string_794[lng], L"BCR", BRCR.perc, BC_percmax );
    }

    if ( BRCR.angle < BC_angmin )
    {
      reject = TRUE;
      txt1.Format( string_798[lng], L"BCR", BRCR.angle, BC_angmin, BC_angmax );
    }

    if ( BRCR.angle > BC_angmax )
    {
      reject = TRUE;
      txt1.Format( string_799[lng], L"BCR", BRCR.angle, BC_angmin, BC_angmax );
    }



    
    for ( i=0 ; i<6 ; i++ ) // compare fnd and ref models
    {
      //if ( i!=1 )
      {
        //int perc = 0;
        //if ( model_fnd[i].lenght ) perc = 100 * model_fnd[i].ctlenght / model_fnd[i].lenght - 100;
//        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].ctlenght, model_fnd[i].angle );
        swprintf( txt, 30, L"%d/%d%/%d ", model_fnd[i].lenght, model_fnd[i].perc, model_fnd[i].angle );
        txt2.Append( txt );
      }
    }
     

    if ( reject ) return( 99 );
  } 


  #undef ALAR // model_fnd[0]
  #undef DLDR // model_fnd[1]
  #undef ALBL // model_fnd[2]
  #undef ARBR // model_fnd[3]
  #undef BLCL // model_fnd[4]
  #undef BRCR // model_fnd[5]


  return 0;
}



int CTip_inspection::Analyze_contour_oral( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF;
  UINT col_cross   = 0xFF00FFFF; // on fulldsp image

  if ( ptmid<40 || ptmid>ptn-40 )
  {
    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( -170, index+3 ); // insert -170 into 3-4-5 cat - wrong profile shape
    #endif 
    txt1.Format( string_696[lng] );
    return 1299;
  }

//  for ( i=AMPATN ; i<ptn-AMPATN ; i++ ) TRACE( "%d,%d\n", i, ATAN(i) );
  // SEARCH UPPER POINTS - CCW - LEFT SIDE
  i    = ptmid;
  imin =    20;
  imax = ptmid;

  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -501, index+3 ); // insert -501 into 3-4-5 cat - unable to find PA-LEFT point
    #endif 
    return 1009; // unable to find point - return error
  }
  kpts[PA][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -502, index+3 ); // insert -502 into 3-4-5 cat - unable to find PB-LEFT point
    #endif 
    return 1019; // unable to find point - return error
  }
  kpts[PB][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -503, index+3 ); // insert -503 into 3-4-5 cat - unable to find PC-LEFT point
    #endif 
    return 1029; // unable to find point - return error
  }
  kpts[PC][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;



  /*
  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -504, index+3 ); // insert -504 into 3-4-5 cat - unable to find PD-LEFT point
    #endif 
    return 1039; // unable to find point - return error
  }
  kpts[PD][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -505, index+3 ); // insert -505 into 3-4-5 cat - unable to find PE-LEFT point
    #endif 
    return 1049; // unable to find point - return error
  }
  kpts[PE][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i-=10;  
    while ( ATAN(i)<30000 && i>imin ) i--;
    if ( i<=imin ) 
    {
      #ifdef FD_DATAREC
       // if ( datarec_on && cap_result ) cap_result->InsertI( -506, index+3 ); // insert -506 into 3-4-5 cat - unable to find PF-LEFT point
      #endif 
      return 1059; // unable to find point - return error
    }
    kpts[PF][LEFT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif

  */

/*  TRACE("UPPER/LEFT SIDE KEYPOINTS:\n");
  TRACE("PE[LEFT]: %4d (%3d,%3d)\n", kpts[PE][LEFT], ptx[kpts[PE][LEFT]], pty[kpts[PE][LEFT]] );
  TRACE("PD[LEFT]: %4d (%3d,%3d)\n", kpts[PD][LEFT], ptx[kpts[PD][LEFT]], pty[kpts[PD][LEFT]] );
  TRACE("PC[LEFT]: %4d (%3d,%3d)\n", kpts[PC][LEFT], ptx[kpts[PC][LEFT]], pty[kpts[PC][LEFT]] );
  TRACE("PB[LEFT]: %4d (%3d,%3d)\n", kpts[PB][LEFT], ptx[kpts[PB][LEFT]], pty[kpts[PB][LEFT]] );
  TRACE("PA[LEFT]: %4d (%3d,%3d)\n", kpts[PA][LEFT], ptx[kpts[PA][LEFT]], pty[kpts[PA][LEFT]] );  */


//////////////////////////////////////////////////////

  // SEARCH LOWER POINTS - CW - RIGHT SIDE
  i    =  ptmid;
  imin =     10;
  imax = ptn-20;

  while ( ( ATAN(i)<4500 || ATAN(i)>9000 ) && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -511, index+3 ); // insert -511 into 3-4-5 cat - unable to find PA-RIGHT point
    #endif 
    return 1109; // unable to find point - return error
  }
  kpts[PA][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -512, index+3 ); // insert -512 into 3-4-5 cat - unable to find PB-RIGHT point
    #endif 
    return 1119; // unable to find point - return error
  }
  kpts[PB][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -513, index+3 ); // insert -513 into 3-4-5 cat - unable to find PC-RIGHT point
    #endif 
    return 1129; // unable to find point - return error
  }
  kpts[PC][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  /*
  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -514, index+3 ); // insert -514 into 3-4-5 cat - unable to find PD-RIGHT point
    #endif 
    return 1139; // unable to find point - return error
  }
  kpts[PD][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -515, index+3 ); // insert -515 into 3-4-5 cat - unable to find PE-RIGHT point
    #endif 
    return 1149; // unable to find point - return error
  }
  kpts[PE][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i+=10;  
    while ( ATAN(i)>7000 && i>imin ) i++;
    if ( i>=imax ) 
    {
      #ifdef FD_DATAREC
     //   if ( datarec_on && cap_result ) cap_result->InsertI( -516, index+3 ); // insert -505 into 3-4-5 cat - unable to find PF-RIGHT point
      #endif 
      return 1159; // unable to find point - return error
    }
    kpts[PF][RIGHT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif
  */

  // Search shrink left point 
  kpts[PD][LEFT] = kpts[PB][LEFT]+20;
  ASSERT( kpts[PA][LEFT] > kpts[PB][LEFT] );
  for ( i=kpts[PA][LEFT]-20 ; i>kpts[PB][LEFT]+20 ; i-- )
  {
    if ( pty[i]>pty[kpts[PD][LEFT]] ) kpts[PD][LEFT]=i;
  }

  if ( kpts[PD][LEFT]>kpts[PB][LEFT]+20 && kpts[PA][LEFT]-20>kpts[PD][LEFT] )
  {
    DRAW_SQUARE(index,ptx[kpts[PD][LEFT]],pty[kpts[PD][LEFT]],col_markers);
  }
  else
  {
    //#ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -512, index+3 ); // insert -512 into 3-4-5 cat - unable to find PB-RIGHT point
    //#endif 
    return 1139; // unable to find point - return error
  }

  // Search shrink right point 
  kpts[PD][RIGHT] = kpts[PB][RIGHT]-20;
  ASSERT( kpts[PA][RIGHT] < kpts[PB][RIGHT] );
  for ( i=kpts[PA][RIGHT]+20 ; i<kpts[PB][RIGHT]-20 ; i++ )
  {
    if ( pty[i]<pty[kpts[PD][RIGHT]] ) kpts[PD][RIGHT]=i;
  }

  if ( kpts[PD][RIGHT]>kpts[PA][RIGHT]+20 && kpts[PB][RIGHT]-20>kpts[PD][RIGHT] )
  {
    DRAW_SQUARE(index,ptx[kpts[PD][RIGHT]],pty[kpts[PD][RIGHT]],col_markers);
  }
  else
  {
    //#ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -512, index+3 ); // insert -512 into 3-4-5 cat - unable to find PB-RIGHT point
    //#endif 
    return 1149; // unable to find point - return error
  }





/*  TRACE("LOWER/RIGHT SIDE KEYPOINTS:\n");
  TRACE("PE[RIGHT]: %4d (%3d,%3d)\n", kpts[PE][RIGHT], ptx[kpts[PE][RIGHT]], pty[kpts[PE][RIGHT]] );
  TRACE("PD[RIGHT]: %4d (%3d,%3d)\n", kpts[PD][RIGHT], ptx[kpts[PD][RIGHT]], pty[kpts[PD][RIGHT]] );
  TRACE("PC[RIGHT]: %4d (%3d,%3d)\n", kpts[PC][RIGHT], ptx[kpts[PC][RIGHT]], pty[kpts[PC][RIGHT]] );
  TRACE("PB[RIGHT]: %4d (%3d,%3d)\n", kpts[PB][RIGHT], ptx[kpts[PB][RIGHT]], pty[kpts[PB][RIGHT]] );
  TRACE("PA[RIGHT]: %4d (%3d,%3d)\n", kpts[PA][RIGHT], ptx[kpts[PA][RIGHT]], pty[kpts[PA][RIGHT]] );  */

  return( 0 ); // everything ok
}




// Special version for Sanjing to neglect fins/ B/C keypoints
int CTip_inspection::Analyze_contour_fins( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF;
  UINT col_cross   = 0xFF00FFFF; // on fulldsp image

  if ( ptmid<40 || ptmid>ptn-40 )
  {
    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( -170, index+3 ); // insert -170 into 3-4-5 cat - wrong profile shape
    #endif 
    txt1.Format( string_696[lng] );
    return 1299;
  }

//  for ( i=AMPATN ; i<ptn-AMPATN ; i++ ) TRACE( "%d,%d\n", i, ATAN(i) );
  // SEARCH UPPER POINTS - CCW - LEFT SIDE
  i    = ptmid;
  imin =    20;
  imax = ptmid;

  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -501, index+3 ); // insert -501 into 3-4-5 cat - unable to find PA-LEFT point
    #endif 
    return 1009; // unable to find point - return error
  }
  kpts[PA][LEFT] = i;

  kpts[PB][LEFT] = 0;
  kpts[PC][LEFT] = 0;

  //dxfins = 60;

  int xnew = ptx[i]-dxfins; // x of keypoint A
  if ( xnew<xmin ) xnew = xmin;

  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  // Skip some height to avoid fins (special version Sanjing)
  while ( ptx[i]>xnew && i>imin ) i--;
  if ( i<=imin ) return 1019; // too low - return error
  DRAW_SQUARE(index,ptx[i],pty[i],COL_BLUE);

  /*
  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -502, index+3 ); // insert -502 into 3-4-5 cat - unable to find PB-LEFT point
    #endif 
    return 1019; // unable to find point - return error
  }
  kpts[PB][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -503, index+3 ); // insert -503 into 3-4-5 cat - unable to find PC-LEFT point
    #endif 
    return 1029; // unable to find point - return error
  }
  kpts[PC][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;
  */

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -504, index+3 ); // insert -504 into 3-4-5 cat - unable to find PD-LEFT point
    #endif 
    return 1039; // unable to find point - return error
  }
  kpts[PD][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -505, index+3 ); // insert -505 into 3-4-5 cat - unable to find PE-LEFT point
    #endif 
    return 1049; // unable to find point - return error
  }
  kpts[PE][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i-=10;  
    while ( ATAN(i)<30000 && i>imin ) i--;
    if ( i<=imin ) 
    {
      #ifdef FD_DATAREC
       // if ( datarec_on && cap_result ) cap_result->InsertI( -506, index+3 ); // insert -506 into 3-4-5 cat - unable to find PF-LEFT point
      #endif 
      return 1059; // unable to find point - return error
    }
    kpts[PF][LEFT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif

/*  TRACE("UPPER/LEFT SIDE KEYPOINTS:\n");
  TRACE("PE[LEFT]: %4d (%3d,%3d)\n", kpts[PE][LEFT], ptx[kpts[PE][LEFT]], pty[kpts[PE][LEFT]] );
  TRACE("PD[LEFT]: %4d (%3d,%3d)\n", kpts[PD][LEFT], ptx[kpts[PD][LEFT]], pty[kpts[PD][LEFT]] );
  TRACE("PC[LEFT]: %4d (%3d,%3d)\n", kpts[PC][LEFT], ptx[kpts[PC][LEFT]], pty[kpts[PC][LEFT]] );
  TRACE("PB[LEFT]: %4d (%3d,%3d)\n", kpts[PB][LEFT], ptx[kpts[PB][LEFT]], pty[kpts[PB][LEFT]] );
  TRACE("PA[LEFT]: %4d (%3d,%3d)\n", kpts[PA][LEFT], ptx[kpts[PA][LEFT]], pty[kpts[PA][LEFT]] );  */


//////////////////////////////////////////////////////

  // SEARCH LOWER POINTS - CW - RIGHT SIDE
  i    =  ptmid;
  imin =     10;
  imax = ptn-20;

  while ( ( ATAN(i)<4500 || ATAN(i)>9000 ) && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -511, index+3 ); // insert -511 into 3-4-5 cat - unable to find PA-RIGHT point
    #endif 
    return 1109; // unable to find point - return error
  }
  kpts[PA][RIGHT] = i;
  kpts[PB][RIGHT] = 0;
  kpts[PC][RIGHT] = 0;

  xnew = ptx[i]-dxfins; // x of keypoint A
  if ( xnew<xmin ) xnew = xmin;

  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  // Skip some height to avoid fins (special version Sanjing)
  while ( ptx[i]>xnew && i<imax ) i++;
  if ( i>=imax ) return 1119; // too low - return error
  DRAW_SQUARE(index,ptx[i],pty[i],COL_BLUE);


  /*
  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -512, index+3 ); // insert -512 into 3-4-5 cat - unable to find PB-RIGHT point
    #endif 
    return 1119; // unable to find point - return error
  }
  kpts[PB][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -513, index+3 ); // insert -513 into 3-4-5 cat - unable to find PC-RIGHT point
    #endif 
    return 1129; // unable to find point - return error
  }
  kpts[PC][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;
  */

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -514, index+3 ); // insert -514 into 3-4-5 cat - unable to find PD-RIGHT point
    #endif 
    return 1139; // unable to find point - return error
  }
  kpts[PD][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -515, index+3 ); // insert -515 into 3-4-5 cat - unable to find PE-RIGHT point
    #endif 
    return 1149; // unable to find point - return error
  }
  kpts[PE][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i+=10;  
    while ( ATAN(i)>7000 && i>imin ) i++;
    if ( i>=imax ) 
    {
      #ifdef FD_DATAREC
     //   if ( datarec_on && cap_result ) cap_result->InsertI( -516, index+3 ); // insert -505 into 3-4-5 cat - unable to find PF-RIGHT point
      #endif 
      return 1159; // unable to find point - return error
    }
    kpts[PF][RIGHT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif


/*  TRACE("LOWER/RIGHT SIDE KEYPOINTS:\n");
  TRACE("PE[RIGHT]: %4d (%3d,%3d)\n", kpts[PE][RIGHT], ptx[kpts[PE][RIGHT]], pty[kpts[PE][RIGHT]] );
  TRACE("PD[RIGHT]: %4d (%3d,%3d)\n", kpts[PD][RIGHT], ptx[kpts[PD][RIGHT]], pty[kpts[PD][RIGHT]] );
  TRACE("PC[RIGHT]: %4d (%3d,%3d)\n", kpts[PC][RIGHT], ptx[kpts[PC][RIGHT]], pty[kpts[PC][RIGHT]] );
  TRACE("PB[RIGHT]: %4d (%3d,%3d)\n", kpts[PB][RIGHT], ptx[kpts[PB][RIGHT]], pty[kpts[PB][RIGHT]] );
  TRACE("PA[RIGHT]: %4d (%3d,%3d)\n", kpts[PA][RIGHT], ptx[kpts[PA][RIGHT]], pty[kpts[PA][RIGHT]] );  */

  return( 0 ); // everything ok
}


// #define AMPATN  6 // semilenght for atan calculation
// #define         DX ptx[i+AMPATN]-ptx[i-AMPATN]
// #define         DY pty[i+AMPATN]-pty[i-AMPATN]
// #define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]

// check contour and find keypoints
int CTip_inspection::Analyze_contour_v30( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF;
  UINT col_cross   = 0xFF00FFFF; // on fulldsp image

  //    L E F T   S I D E  

  i    = AMPATN;
  imin = AMPATN;
  imax = ptn / 2;

  // search PA-LEFT
  while ( ATAN(i)>22500 && i<imax ) i++;
  if ( i>=imax ) return 1009; // unable to find point - return error
  kpts[PA][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  // search PB-LEFT
//  while ( ATAN(i)<22500 && i<imax ) i++;
  while ( ATAN(i)<26000 && i<imax ) i++;
  if ( i>=imax ) return 1019; // unable to find point - return error
  kpts[PB][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  // search PC-LEFT
  while ( ATAN(i)>22500 && i<imax ) i++;
  if ( i>=imax ) return 1029; // unable to find point - return error
  kpts[PC][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  // search PD-LEFT
  while ( ATAN(i)<31500 && i<imax ) i++;
  if ( i>=imax ) return 1039; // unable to find point - return error
  kpts[PD][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;


  //    R I G H T   S I D E  

  i    = ptn-AMPATN-1;
  imin = ptn/2;
  imax = i;

  // search PA-RIGHT
  while ( ATAN(i)<13500 && i>imin ) i--;
  if ( i<=imin ) return 1109; // unable to find point - return error
  kpts[PA][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  // search PB-RIGHT
//  while ( ATAN(i)>13500 && i>imin ) i--;
  while ( ATAN(i)>10000 && i>imin ) i--;
  if ( i<=imin ) return 1119; // unable to find point - return error
  kpts[PB][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  // search PC-RIGHT
  while ( ATAN(i)<13500 && i>imin ) i--;
  if ( i<=imin ) return 1129; // unable to find point - return error
  kpts[PC][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  // search PD-RIGHT
  while ( ATAN(i)>4500 && i>imin ) i--;
  if ( i<=imin ) return 1129; // unable to find point - return error
  kpts[PD][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  return( 0 ); // everything ok
}




//Pour marked for current processing on backlight
int CTip_inspection::Analyze_contour_new( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF;
  UINT col_cross   = 0xFF00FFFF; // on fulldsp image

  if ( ptmid<40 || ptmid>ptn-40 )
  {
    #ifdef FD_DATAREC
      //if ( datarec_on && cap_result ) cap_result->InsertI( -170, index+3 ); // insert -170 into 3-4-5 cat - wrong profile shape
    #endif 
    txt1.Format( string_696[lng] );
    return 1299;
  }

//  for ( i=AMPATN ; i<ptn-AMPATN ; i++ ) TRACE( "%d,%d\n", i, ATAN(i) );
  // SEARCH UPPER POINTS - CCW - LEFT SIDE
  i    = ptmid;
  imin =    20;
  imax = ptmid;

  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -501, index+3 ); // insert -501 into 3-4-5 cat - unable to find PA-LEFT point
    #endif 
    return 1009; // unable to find point - return error
  }
  kpts[PA][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -502, index+3 ); // insert -502 into 3-4-5 cat - unable to find PB-LEFT point
    #endif 
    return 1019; // unable to find point - return error
  }
  kpts[PB][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -503, index+3 ); // insert -503 into 3-4-5 cat - unable to find PC-LEFT point
    #endif 
    return 1029; // unable to find point - return error
  }
  kpts[PC][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -504, index+3 ); // insert -504 into 3-4-5 cat - unable to find PD-LEFT point
    #endif 
    return 1039; // unable to find point - return error
  }
  kpts[PD][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -505, index+3 ); // insert -505 into 3-4-5 cat - unable to find PE-LEFT point
    #endif 
    return 1049; // unable to find point - return error
  }
  kpts[PE][LEFT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i-=10;  
    while ( ATAN(i)<30000 && i>imin ) i--;
    if ( i<=imin ) 
    {
      #ifdef FD_DATAREC
       // if ( datarec_on && cap_result ) cap_result->InsertI( -506, index+3 ); // insert -506 into 3-4-5 cat - unable to find PF-LEFT point
      #endif 
      return 1059; // unable to find point - return error
    }
    kpts[PF][LEFT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif

/*  TRACE("UPPER/LEFT SIDE KEYPOINTS:\n");
  TRACE("PE[LEFT]: %4d (%3d,%3d)\n", kpts[PE][LEFT], ptx[kpts[PE][LEFT]], pty[kpts[PE][LEFT]] );
  TRACE("PD[LEFT]: %4d (%3d,%3d)\n", kpts[PD][LEFT], ptx[kpts[PD][LEFT]], pty[kpts[PD][LEFT]] );
  TRACE("PC[LEFT]: %4d (%3d,%3d)\n", kpts[PC][LEFT], ptx[kpts[PC][LEFT]], pty[kpts[PC][LEFT]] );
  TRACE("PB[LEFT]: %4d (%3d,%3d)\n", kpts[PB][LEFT], ptx[kpts[PB][LEFT]], pty[kpts[PB][LEFT]] );
  TRACE("PA[LEFT]: %4d (%3d,%3d)\n", kpts[PA][LEFT], ptx[kpts[PA][LEFT]], pty[kpts[PA][LEFT]] );  */


//////////////////////////////////////////////////////

  // SEARCH LOWER POINTS - CW - RIGHT SIDE
  i    =  ptmid;
  imin =     10;
  imax = ptn-20;

  while ( ( ATAN(i)<4500 || ATAN(i)>9000 ) && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -511, index+3 ); // insert -511 into 3-4-5 cat - unable to find PA-RIGHT point
    #endif 
    return 1109; // unable to find point - return error
  }
  kpts[PA][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -512, index+3 ); // insert -512 into 3-4-5 cat - unable to find PB-RIGHT point
    #endif 
    return 1119; // unable to find point - return error
  }
  kpts[PB][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -513, index+3 ); // insert -513 into 3-4-5 cat - unable to find PC-RIGHT point
    #endif 
    return 1129; // unable to find point - return error
  }
  kpts[PC][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
     // if ( datarec_on && cap_result ) cap_result->InsertI( -514, index+3 ); // insert -514 into 3-4-5 cat - unable to find PD-RIGHT point
    #endif 
    return 1139; // unable to find point - return error
  }
  kpts[PD][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    #ifdef FD_DATAREC
    //  if ( datarec_on && cap_result ) cap_result->InsertI( -515, index+3 ); // insert -515 into 3-4-5 cat - unable to find PE-RIGHT point
    #endif 
    return 1149; // unable to find point - return error
  }
  kpts[PE][RIGHT] = i;
  if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  #ifdef CAP_KEYPOINTS6 // if defined, search one more point
    i+=10;  
    while ( ATAN(i)>7000 && i>imin ) i++;
    if ( i>=imax ) 
    {
      #ifdef FD_DATAREC
     //   if ( datarec_on && cap_result ) cap_result->InsertI( -516, index+3 ); // insert -505 into 3-4-5 cat - unable to find PF-RIGHT point
      #endif 
      return 1159; // unable to find point - return error
    }
    kpts[PF][RIGHT] = i;
    if ( bl_drawovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  #endif


/*  TRACE("LOWER/RIGHT SIDE KEYPOINTS:\n");
  TRACE("PE[RIGHT]: %4d (%3d,%3d)\n", kpts[PE][RIGHT], ptx[kpts[PE][RIGHT]], pty[kpts[PE][RIGHT]] );
  TRACE("PD[RIGHT]: %4d (%3d,%3d)\n", kpts[PD][RIGHT], ptx[kpts[PD][RIGHT]], pty[kpts[PD][RIGHT]] );
  TRACE("PC[RIGHT]: %4d (%3d,%3d)\n", kpts[PC][RIGHT], ptx[kpts[PC][RIGHT]], pty[kpts[PC][RIGHT]] );
  TRACE("PB[RIGHT]: %4d (%3d,%3d)\n", kpts[PB][RIGHT], ptx[kpts[PB][RIGHT]], pty[kpts[PB][RIGHT]] );
  TRACE("PA[RIGHT]: %4d (%3d,%3d)\n", kpts[PA][RIGHT], ptx[kpts[PA][RIGHT]], pty[kpts[PA][RIGHT]] );  */

  return( 0 ); // everything ok
}







int CTip_inspection::Analyze_contour( void )
{
  int i;
  int imin;
  int imax;

  /*
  //  for ( i=10 ; i<ptn-10 ; i++ )
  for ( i=0 ; i<ptn ; i++ )
  {
    if ( i-AMPATN>=0 && i+AMPATN<ptn )
    {
      int dx=ptx[i+AMPATN]-ptx[i-AMPATN];
      int dy=pty[i+AMPATN]-pty[i-AMPATN];
      ASSERT( dx>=-2*AMPATN && dx<=2*AMPATN );
      ASSERT( dy>=-2*AMPATN && dy<=2*AMPATN );
      ASSERT( dx!=0 || dy!=0 ); 
      TRACE("%d,%d,%d,%d\n", i, ptx[i], pty[i], ATNLUT[dx+AMPATN*2][dy+AMPATN*2] );
    }
    else
    {
      TRACE("%d,%d,%d\n", i, ptx[i], pty[i] );
    }

  }
  */

  UINT col_markers = 0xFF00FFFF;

  imin = AMPATN*2;
  imax = ptn/2; // ptn-AMPATN*2;
  i    = imin;

  // upper points
  while ( ATAN(i)>=22500 && i<imax ) i++;
  if ( i>=imax ) return 1009; // unable to find point - return error

  /*#ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)<=22500 && i<imax ) i++;
  if ( i>=imax ) return 1019; // unable to find point - return error
  /*#ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)>=22500 && i<imax ) i++;
  if ( i>=imax ) return 1029; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)<=22500 && i<imax ) i++;
  if ( i>=imax ) return 1039; // unable to find point - return error
  /*#ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif*/
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)<=31500 && i<imax ) i++;
  if ( i>=imax ) return 1049; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

//////////////////////////////////////////////////////

  imin = ptn/2; 
  imax = ptn-AMPATN*2;
  i    = imax;

  // LOWER POINTS (RIGHT)
  while ( ATAN(i)<=13500 && i>imin ) i--;
  if ( i<=imin ) return 1109; // unable to find point - return error
  /*#ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif*/
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)>=13500 && i>imin ) i--;
  if ( i<=imin ) return 1119; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)<=13500 && i>imin ) i--;
  if ( i<=imin ) return 1129; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)>=13500 && i>imin ) i--;
  if ( i<=imin ) return 1139; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

  while ( ATAN(i)>=4500 && i>imin ) i--;
  if ( i<=imin ) return 1149; // unable to find point - return error
  /* #ifdef SAVE_FULL_PROC
    DRAW_FULL(index,ptx[i],pty[i],0xFF0000FF); // inner left end of sealing PAU
  #endif */
  DRAW_SQUARE(index,ptx[i],pty[i],col_markers);


  return( 0 ); // everything ok
}





int CTip_inspection::Oralcap_toplight( BYTE* img_acq, UINT nimg )
{
  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  col1 = TXT_DARKGREEN;
  col2 = TXT_DARKGREEN;

  if ( tl_contour==0 )
  {
    col1 = 0;
    txt1 = string_1402[lng];
    return 0;
  }


//  if ( Find_contour( xmin+1 ) )  // contour not found/validated
  if ( Find_contour_toplight( (ymin+ymax)/2 ) )  // contour not found/validated
  {
    //txt1.Format( L"Contour NOT found/valid (%d pts) - %d us" , ptn, timer.elapsed_us() );
    //txt2.Format( L"-" );
    return 9000;
  }

  return 0; // completed successfully
}



int CTip_inspection::Process_image_toplight( BYTE* img_acq, UINT nimg )
{

  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  col1 = TXT_DARKGREEN;
  col2 = TXT_DARKGREEN;


//  if ( Find_contour( xmin+1 ) )  // contour not found/validated
  if ( Find_contour_toplight( (ymin+ymax)/2 ) )  // contour not found/validated
  {
    //txt1.Format( L"Contour NOT found/valid (%d pts) - %d us" , ptn, timer.elapsed_us() );
    //txt2.Format( L"-" );
    return 9000;
  }


/*  if ( Analyze_contour_new() )
  {
    //txt1.Format( L"Cannot find valid keypoints" );
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints() )
  {
    return 9200; // some problem on contour
  } */

  //txt1.Format( L"Cap GOOD" );
  //txt1.Format( string_694[lng] );


  // Everything's OK: GOOD

  //txt1.Format( L"Contour %d pts - %d us" , ptn, timer.elapsed_us() );
  //txt2.Format( L"-" );


  return 0; // completed successfully
}





int CTip_inspection::Oralcap_backlight( BYTE* img_acq, UINT nimg )
{

  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  // Everything's OK: GOOD
  col1 = TXT_DARKGREEN;
  col2 = TXT_DARKGREEN;

  if ( contour_thr==0 )
  {
    col1 = 0;
    txt1 = string_1402[lng];
    return 0;
  }


//  if ( Find_contour( xmin+1 ) )  // contour not found/validated
  if ( Find_contour_new( (ymin+ymax)/2 ) )  // contour not found/validated
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -105, index+3 ); // insert -105 into 3-4-5 cat 
    #endif 
    col1 = TXT_DARKRED;
    return 9000;
  }

//  #ifdef NEWCAP_V21
  if ( shl_height>0 )
  {
    if ( Analyze_shoulder() ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -150, index+3 ); // insert -150 into 3-4-5 cat 
      #endif 
      col1 = TXT_DARKRED;
      return( 9400 ); // reject by shoulder analysys
    }
  }
//  #endif

  if ( Analyze_contour_oral() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -115, index+3 ); // insert -115 into 3-4-5 cat 
    #endif
    txt1.Format( string_698[lng] );
    col1 = TXT_DARKRED;
    return 9100; // some problem on contour
  } 

  if ( Analyze_keypoints_oral() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -125, index+3 ); // insert -125 into 3-4-5 cat 
    #endif
    col1 = TXT_DARKRED;
    return 9200; // some problem on contour
  } 

  //txt1.Format( L"Cap GOOD" );
  txt1.Format( string_694[lng] );

  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, index+3 ); // insert 0 = GOOD into 3-4-5 cat 
  #endif

  //txt1.Format( L"Contour %d pts - %d us" , ptn, timer.elapsed_us() );
  //txt2.Format( L"-" );
  return 0; // completed successfully
}



int CTip_inspection::Process_image_cap( BYTE* img_acq, UINT nimg )
{

  CString txt;
  CTimer timer;

  //TRACE("contour_thr=%d\n",contour_thr);

  if ( contour_thr==0 )
  {
    col1 = 0;
    txt1 = string_2510[lng];
    return 0;
  }



//  if ( Find_contour( xmin+1 ) )  // contour not found/validated
  if ( Find_contour_new( (ymin+ymax)/2 ) )  // contour not found/validated
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -105, index+3 ); // insert -105 into 3-4-5 cat 
    #endif 
    return 9000;
  }

  #ifdef NEWCAP_V21
  if ( shl_height>0 )
  {
    if ( Analyze_shoulder() ) 
    {
      #ifdef FD_DATAREC
        if ( datarec_on && cap_result ) cap_result->InsertI( -150, index+3 ); // insert -150 into 3-4-5 cat 
      #endif 
      return( 9400 ); // reject by shoulder analysys
    }
  }
  #endif



  #if SERIAL_N==201500002 /* Sanjing skip B/C points to neglect fins*/ || SERIAL_N==201900097 || SERIAL_N==202000008  || SERIAL_N==202000009 || SERIAL_N==202000010 || SERIAL_N==201900025 || SERIAL_N==201900030 || SERIAL_N == 202000004// Angde to neglect gaps because of FLIP OFF CAP

  if ( Analyze_contour_fins() )
  {
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints_fins() ) return 9200; 

  #else // normal processing

  if ( Analyze_contour_new() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -115, index+3 ); // insert -115 into 3-4-5 cat 
    #endif
    txt1.Format( string_698[lng] );
    return 9100; // some problem on contour
  }

  if ( Analyze_keypoints() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -125, index+3 ); // insert -125 into 3-4-5 cat 
    #endif
    return 9200; // some problem on contour
  }

  #endif // SERIAL_N==201500002 // Sanjing skip B/C points to neglect fins

    //Pour 20200321

#ifdef NECK_CRACK

  cv::Mat neck_img(dimy, dimx, CV_8U, img_acq, dimx);
  Point P_a_left,P_a_right;
  P_a_left = Point(ptx[kpts[PE][LEFT]],pty[kpts[PE][LEFT]]);
  P_a_right = Point(ptx[kpts[PE][RIGHT]],pty[kpts[PE][RIGHT]]);

	  if (para_crack.th_crack_area)
	  {
  if (detectCrack(neck_img,P_a_left,P_a_right))
  {
	return 9200; // some problem on neck crack
  }

	  }
#endif	  
	  //Pour 20200321


  //txt1.Format( L"Cap GOOD" );
  txt1.Format( string_694[lng] );

  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, index+3 ); // insert 0 = GOOD into 3-4-5 cat 
  #endif

  // Everything's OK: GOOD
  col1 = TXT_DARKGREEN;
  col2 = TXT_DARKGREEN;

  //txt1.Format( L"Contour %d pts - %d us" , ptn, timer.elapsed_us() );
  //txt2.Format( L"-" );

  return 0;


  /*

  // search profile points through xmin-xmax
  xheight = 0;
  yasse   = 0;
  pronpt  = 0;
  for ( x=xmin ; x<=xmax ; x++ ) Find_line_cap( img_acq, x ); 

  TRACE("finito ricerca profilo - %d segmenti trovati\n", pronpt );

  if ( pronpt>min_npt && xheight>xmin && xheight<xmax )  // minimum n. of points to validate
  {
    yasse = int( 0.5 + 0.5*yasse/pronpt );
    
    // check height first
    if ( xheight<htot_min ) // height too low
    {
      Draw_height( 0x00FF0000 ); // 0xFFRRGGBB
      txt.Format( string_720[lng] , xheight, htot_min, htot_max );
      camera[index].text[1]->SetText( txt, TXT_DARKRED );
      camera[index].text[2]->SetText( L"", 0        );
      return 29; // too LOW top height
    }


    if ( xheight>=htot_max ) // height too high
    {
      Draw_height( 0x00FF0000 ); // 0xFFRRGGBB
      txt.Format( string_722[lng], xheight, htot_min, htot_max );
      camera[index].text[1]->SetText( txt, TXT_DARKRED );
      camera[index].text[2]->SetText( L"", 0        );
      return 39; // too HIGH top height
    }

    // height OK - draw in green
    Draw_height( 0x0000FF00 ); // 0xFFRRGGBB
    txt.Format( string_724[lng], xheight, htot_min, htot_max );
    camera[index].text[1]->SetText( txt, TXT_DARKGREEN );


    // draw calculated axis
    for ( x=xmin ; x<xheight-30 ; x+=4 )
    {
      int ofs = BUF2OFS(index,x,yasse);
      SETPIXELOFS(index,ofs-1,0x00000000 ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs  ,0x0000FFD9 ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs+1,0x00000000 ); // 0xFFRRGGBB
    }


/*
  profile[x-xmin].y1 = y1;
  profile[x-xmin].y2 = y2;
  pronpt++;
  yasse += y2+y1;
  */


  /*
    //search for neck diameter
    #define SRCAMP 5
    int i;
    int val = 0;
    for ( i=0 ; i<SRCAMP ; i++ ) val += profile[i].y2-profile[i].y1;
    int min = val;
    TRACE("%d,%d\n", i, val);

    while ( i<pronpt )
    {
      val += profile[i].y2-profile[i].y1-profile[i-SRCAMP].y2+profile[i-SRCAMP].y1;
      i++;
      TRACE("%d,%d\n", i, val);
    }


//    for ( i=0 ; i<pronpt ; i++ )


    TRACE("finito\n");


    

/*    int err_symm = 0;
      //DRAW_PT( index, x, yasse, 0x0000AA00 ); // 0xFFRRGGBB

    // check symmetry
    if ( maxerr_symm > 0 )
    {
      for ( x=xmin ; x<xheight ; x++ )
      {
        err_symm += ((profile[x-xmin].y2-yasse)-(yasse-profile[x-xmin].y1))*
                    ((profile[x-xmin].y2-yasse)-(yasse-profile[x-xmin].y1));
      }
    }

    err_symm = int( 10.0*sqrt( double(err_symm)/pronpt ) );
    txt.Format( L"Symmetry=%d", err_symm );
    camera[index].text[2]->SetText( txt, 0 );  */

/*    int i=0;
    if ( learn_models )
    {
      // start from TOP height
      while( i<CAPMODELSIZE )
      {
        ASSERT( xheight-xmin-i>=0 && xheight-xmin-i<prosize );
        ASSERT( i>=0 && i<modsize );
        model[i].val = profile[xheight-xmin-i].y2 - yasse;
        TRACE("Model[%3i]=%3d\n",i,model[i].val);
        i++;
      }
    }
    else
    {


    }*/ 
/*
  }
  else  // not enough points, failed
  {
    txt.Format( string_746[lng] , pronpt, min_npt );
    camera[index].text[1]->SetText( txt, TXT_DARKRED );
    camera[index].text[2]->SetText( L"", 0        );
    return 19; // too few points
  }
  */
  return 0; // completed successfully
}



int CTip_inspection::Process_image_tip( BYTE* img_acq, UINT nimg )
{

  CString txt;
  int x, y;

  if ( prosize != xmax-xmin+1 ) Allocate_profile();

/*  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  CString txt;

  SCALE_IMG( index, img_acq );

  // draw inspection area first
  int x, y;
//  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  for ( x=xmin ; x<=xmax ; x+=4 )
  {
    DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
  }
  for ( y=ymin ; y<=ymax ; y+=4 )
  {
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
  }

  */

  // search profile points through xmin-xmax
  xheight = 0;
  yasse   = 0;
  pronpt  = 0;
  for ( x=xmin ; x<=xmax ; x++ ) Find_line_tip( img_acq, x ); 

  if ( pronpt>min_npt && xheight>xmin && xheight<xmax )  // minimum n. of points to validate
  {
    yasse = int( 0.5 + 0.5*yasse/pronpt );
    
    // check height first
    if ( xheight<htot_min ) // height too low
    {
      Draw_height( COL_RED ); // 0xFFRRGGBB
//      txt.Format( string_720[lng] , xheight, htot_min, htot_max );
//      camera[index].text[1]->SetText( txt, TXT_DARKRED );
//      camera[index].text[2]->SetText( L"", 0        );
      txt1.Format( string_720[lng] , xheight, htot_min, htot_max );
      return 29; // too LOW top height
    }

    if ( xheight>=htot_max ) // height too high
    {
      Draw_height( COL_RED ); // 0xFFRRGGBB
 //     txt.Format( string_722[lng], xheight, htot_min, htot_max );
 //     camera[index].text[1]->SetText( txt, TXT_DARKRED );
 //     camera[index].text[2]->SetText( L"", 0        );
      txt1.Format( string_722[lng], xheight, htot_min, htot_max );
      return 39; // too HIGH top height
    }

    // height OK - draw in green
    Draw_height( COL_GREEN ); // 0xFFRRGGBB
//    txt.Format( string_724[lng], xheight, htot_min, htot_max );
//    camera[index].text[1]->SetText( txt, TXT_DARKGREEN );
    txt1.Format( string_724[lng], xheight, htot_min, htot_max );


    // draw calculated axis
    for ( x=xmin ; x<xheight-30 ; x+=4 )
    {
      int ofs = BUF2OFS(index,x,yasse);
      SETPIXELOFS(index,ofs-1,COL_BLACK ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs  ,0xFF00FFD9 ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs+1,COL_BLACK ); // 0xFFRRGGBB
    }  

/*    int err_symm = 0;
      //DRAW_PT( index, x, yasse, 0x0000AA00 ); // 0xFFRRGGBB

    // check symmetry
    if ( maxerr_symm > 0 )
    {
      for ( x=xmin ; x<xheight ; x++ )
      {
        err_symm += ((profile[x-xmin].y2-yasse)-(yasse-profile[x-xmin].y1))*
                    ((profile[x-xmin].y2-yasse)-(yasse-profile[x-xmin].y1));
      }
    }

    err_symm = int( 10.0*sqrt( double(err_symm)/pronpt ) );
    txt.Format( L"Symmetry=%d", err_symm );
    camera[index].text[2]->SetText( txt, 0 );  */



    // tip bulge analysis
    //CString bulge_txt;
    bulge_err = 0;
    if ( maxerr_bulge > 0 )
    {
      //TRACE("NEW BULGE INSPECTION\n");
     // int err_bulge = 0;
      int width_old = 0; // profile[x-xmin].y2-profile[x-xmin].y1; // first width (lower)
      int width_new = 0;

      int left_old  = 0;
      int right_old = 0;
      int left_new  = 0;
      int right_new = 0;

      for ( x=xmin+10 ; x<xheight ; x++ )
      {
        /*
        width_old = profile[x-xmin-10].y2-profile[x-xmin-10].y1; // first width (lower)
        width_new = profile[x-xmin   ].y2-profile[x-xmin   ].y1;
        if ( width_new > width_old ) bulge_err += (width_new-width_old)*(width_new-width_old);
        */
        // TOP/LEFT
        left_old = yasse-profile[x-xmin-10].y1; // first width (lower)
        left_new = yasse-profile[x-xmin   ].y1;
        if ( left_new > left_old ) bulge_err += (left_new-left_old)*(left_new-left_old);

        // BOTTOM/RIGHT
        right_old = profile[x-xmin-10].y2-yasse;
        right_new = profile[x-xmin   ].y2-yasse;
        if ( right_new > right_old ) bulge_err += (right_new-right_old)*(right_new-right_old);


        //if ( width_new >= width_old ) bulge_err += width_new-width_old+1; 
        //width_old = width_new;     
        //TRACE("x=%3d - w=%3d - err=%3d\n", x-xmin, width_new, bulge_err);
      }

      if ( bulge_err > maxerr_bulge )
      {
//        bulge_txt.Format( string_730[lng], bulge_err, maxerr_bulge );
//        camera[index].text[2]->SetText( bulge_txt, TXT_DARKRED );
        txt.Format( string_730[lng], bulge_err, maxerr_bulge );
        txt1.Append( txt );
        return 59; // bulge error
      }
      else // bulge Ok
      {
        txt.Format( string_732[lng], bulge_err, maxerr_bulge );
        txt1.Append( txt );
        col1 = TXT_DARKGREEN;
      }

    }
    else // BULGE disabled
    {
      txt1.Append( string_728[lng] );
      col1 = TXT_DARKGREEN;
    }



    col1 = TXT_DARKGREEN;


    // INNER AREA/BURNS ANALYSIS
    burns_err = 0;
    CString burns_txt;
    if ( maxerr_burns > 0 )
    {
      int bhmax = xheight - int(burns_hmax*scalefct); // compute top height for burns in pixel
      int bhmin = xheight - int(burns_hmin*scalefct); // compute top height for burns in pixel

      ASSERT( 0<bhmin && bhmin<bhmax && bhmax<dimx );

      
      #ifdef NEW_BURNS_INSPECTION // new inner burns analysis (insensible to vertical scratches)
      
      #undef FLT
      //#define FLT(x,y) PT(x,y-4)+PT(x,y-3)+PT(x,y-2)-2*(PT(x,y-1)+PT(x,y)+PT(x,y+1))+PT(x,y+2)+PT(x,y+3)+PT(x,y+4)
      //#define FLT(x,y) PT(x,y) //img_acq[x+y*dimx]

      #define FLT(x,y) (PT(x-4,y)+PT(x-3,y)+PT(x-2,y)-2*(PT(x-1,y)+PT(x,y)+PT(x+1,y))+PT(x+2,y)+PT(x+3,y)+PT(x+4,y))
// new      #define FLT(x,y) ( PT(x-4,y)+PT(x-3,y)+PT(x-2,y)+PT(x-1,y)-PT(x+1,y)-PT(x+2,y)-PT(x+3,y)-PT(x+4,y) )



      for ( x=0 ; x<pronpt ; x++ )
      {

        if ( x+xmin>=bhmin && x+xmin<=bhmax && profile[x].y2-profile[x].y1-2*burns_border>10 ) 
        {

          for ( y=profile[x].y1+burns_border ; y<profile[x].y2-burns_border ; y++ )
          {
            if ( FLT(x+xmin,y)-burns_maxvar>0 ) 
            {
              burns_err += FLT(x+xmin,y)-burns_maxvar;
              DRAW_PT( index, x+xmin, y, COL_RED ); // 0xFFRRGGBB
            }
 /*           if ( -FLT(x+xmin,y)-burns_maxvar>0 ) 
            {
              burns_err += -FLT(x+xmin,y)-burns_maxvar;
              DRAW_PT( index, x+xmin, y, COL_MAGENTA ); // 0xFFRRGGBB
            }*/

           // DRAW_PT( index, x+xmin, y, COL_CYAN ); // 0xFFRRGGBB
          }

          DRAW_PT( index, x+xmin, profile[x].y1+burns_border, COL_CYAN ); // 0xFFRRGGBB
          DRAW_PT( index, x+xmin, profile[x].y2-burns_border, COL_CYAN ); // 0xFFRRGGBB
        
        }
      }




      #else  // first version, horizontal variance analysis
      
      for ( x=0 ; x<pronpt ; x++ )
      {

        if ( x+xmin>=bhmin && x+xmin<=bhmax && profile[x].y2-profile[x].y1-2*burns_border>10 ) 
        {

          UINT sum  = 0;
          UINT sum2 = 0;
          UINT ptn  = 0;

          for ( y=profile[x].y1+burns_border ; y<profile[x].y2-burns_border ; y++ )
          {
            //DRAW_PT( index, x+xmin, y, area_color ); // 0xFFRRGGBB
            sum  += PT((x+xmin),y);
            sum2 += PT((x+xmin),y)*PT(x+xmin,y);
            ptn++;
          }
//          nrows++;
          ASSERT( ptn>9 );

          int varc = ( sum2-(sum*sum)/ptn )/(ptn-1); // compute variance in this line
//        TRACE("%d v=%d\n", x+xmin, burns_err );

    /*      if ( varc>1000 )
          {
            TRACE("x=%d x+xmin=%d\n", x, x+xmin);
            for ( y=profile[x].y1+burns_border ; y<profile[x].y2-burns_border ; y++ )
            {
              TRACE("%d,%d\n", y, PT((x+xmin),y));
            }
            TRACE("check\n");
          }  */

          if ( varc>burns_maxvar )
          {
            burns_err += varc - burns_maxvar;
            DRAW_PT( index, x+xmin, profile[x].y1+burns_border, COL_RED ); // 0xFFRRGGBB
            DRAW_PT( index, x+xmin, profile[x].y2-burns_border, COL_RED ); // 0xFFRRGGBB
          }
          else
          {
            DRAW_PT( index, x+xmin, profile[x].y1+burns_border, COL_GREEN ); // 0xFFRRGGBB
            DRAW_PT( index, x+xmin, profile[x].y2-burns_border, COL_GREEN ); // 0xFFRRGGBB
          }
        
        }
        /*else
        {
          ASSERT( x+xmin > 0 && x+xmin < 780 );
          if ( profile[x].y1 >= 0 && profile[x].y1 < 580 ) DRAW_PT( index, x+xmin, profile[x].y1, 0x00FF00F2 ); // 0xFFRRGGBB
          if ( profile[x].y2 >= 0 && profile[x].y2 < 580 ) DRAW_PT( index, x+xmin, profile[x].y2, 0x00FF00F2 ); // 0xFFRRGGBB
        }*/
        //x--;
      }


      #endif


      if ( burns_err > maxerr_burns )
      {
        txt2.Format( string_734[lng], burns_err, maxerr_burns );
        return 69; // burns error
      }
      else // Burns acceptable
      {
        txt2.Format( string_736[lng], burns_err, maxerr_burns );
      }

    }
    else
    {
      txt2 = string_738[lng];
    }


    // TIP CURVATURE ANALYSIS
    if ( maxerr_curv > 0 )
    {

      int    x2 = xheight-xmin;
      double rg = 0.5*tip_diameter*scalefct; // convert into pixels
      int    x1 = x2 - int( 0.5 + rg );  // calculate points down - radius

      double err = 0.0;
      //int n = 0;

      if ( yasse<int(rg+3.0) || yasse>ymax-int(rg+3-0) )
      {
        txt2.Append( string_748[lng] );
        return 48; // too close to top/up image borders (left/right in the image)
      }

      //TRACE("yasse=%d\n", yasse);
      for ( x=x1 ; x<=x2 ; x++ )
      {
        if ( x-x1 > rg ) break;
        
        double w1 = profile[x].y2 - yasse;
        double w2 = yasse - profile[x].y1;
        double ac = sqrt( rg*rg - (x-x1)*(x-x1) );

        //err += abs( w1/ac - 1.0 );
        //err += abs( w2/ac - 1.0 );

        //err += abs( w1 - ac );
        //err += abs( w2 - ac );
       
        err += (w1-ac)*(w1-ac);
        err += (w2-ac)*(w2-ac);

        if ( !(x&3) )
        {
          DRAW_PT( index, x+xmin, yasse-int(ac), COL_MAGENTA ); // 0xFFRRGGBB
          DRAW_PT( index, x+xmin, yasse+int(ac), COL_MAGENTA ); // 0xFFRRGGBB
        }
      }

      //curv_err = int( 0.5 + abs( 1000.0 * (curv_err/(x-x1)-1.0) ) ); // normalize
      //curv_err = int( 0.5 + 100.0*err/(x-x1) ); // normalize
      curv_err = int( 0.5 + err * 0.1 );

      if ( curv_err > maxerr_curv )
      {
        txt.Format( string_740[lng], curv_err, maxerr_curv );
        txt2.Append( txt );
        return 49; // too HIGH curvature error
      }
      else // curvature OK
      {
        txt.Format( string_742[lng], curv_err, maxerr_curv );
        txt2.Append( txt );
      }
    }
    else // curvature disabled
    {
      txt2.Append( string_744[lng] );
    }
  }
  else  // not enough points, failed
  {
    txt1.Format( string_746[lng] , pronpt, min_npt );
    txt2 = "";
    return 19; // too few points
  }


  #ifdef TIP_THICKNESS
    if ( tipthick_minnpt>0 ) // hthink_minprc && hthink_maxprc )
    {
      int x1  = xheight-tipthick_height; // left/bottom point
      int npt = 0; // number of found points
      while ( x1<=xheight )
      {
        for ( y=profile[x1-xmin].y1 ; y<=profile[x1-xmin].y2 ; y++ )
        {
          if ( PT(x1,y)<tipthick_minlum )
          {
            DRAW_PT( index,x1,y,COL_CYAN );
            npt++;
          }
        }
        x1++;
      }   

      if ( npt<tipthick_minnpt )
      {
        txt.Format( string_2210[lng], npt, tipthick_minnpt );
        txt2.Append( txt );
        return 59; // too FEW thickness dark points found on tip
      }
      else // thickness OK
      {
        txt.Format( string_2212[lng], npt, tipthick_minnpt );
        txt2.Append( txt );
      }

    }
    else // thickness disabled
    {
      txt2.Append( string_2214[lng] );
    }
  #endif

  col2 = TXT_DARKGREEN;
  good = TRUE;
  return 0; // completed successfully
}


//Pour added on 20200320
int CTip_inspection::detectCrack(const Mat &srcImage, Point pt1, Point pt2)
{
	Mat src1 = srcImage.clone();//stretch(srcImage,10);
	Mat src2; //阈值分割
	//cvtColor(srcImage, colorSrc, CV_GRAY2BGR);
    // find the crack area
	int leftx = (pt2.x <= pt1.x ? pt1.x : pt2.x)- para_crack.insp_offset - para_crack.insp_height;//85;
	int topy = pt2.y;
	//int topy = pt2.y-para_crack.th_blob_area;    //for powder stopper detection 20201106
	int rightx = (pt2.x <= pt1.x ? pt1.x : pt2.x) - para_crack.insp_offset;//15;
	int bottomy = pt1.y;
	//int bottomy = pt1.y+para_crack.th_blob_area; //for powder stopper detection 20201106
	Rect roi = Rect(Point(leftx, topy), Point(rightx, bottomy));

	    // Last

    // draw inspection area first
	for ( int x=leftx ; x<=rightx ; x+=4 )
    {
      DRAW_CROSS( index, x, topy,    COL_MAGENTA ); // 0xFFRRGGBB
      DRAW_CROSS( index, x, bottomy, COL_MAGENTA ); // 0xFFRRGGBB
    }
    for ( int y=bottomy ; y<=topy ; y+=4 )
    {
      DRAW_CROSS( index, leftx, y,  COL_MAGENTA ); // 0xFFRRGGBB
      DRAW_CROSS( index, rightx, y, COL_MAGENTA ); // 0xFFRRGGBB
    }


	Mat im_roi = src1(roi);
	//string strwrite = "D:\\IMAGES\\sanjing_toubu\\beikguang\\image0\\";
	//strwrite = strwrite + str + "_roi.png";
	//imwrite(strwrite, im_roi);
	Mat temp,temp1,temp2;
	threshold(im_roi, temp, para_crack.th_crack_brtness, 255, CV_THRESH_BINARY_INV);//150 暂时取180
	//threshold(im_roi, temp, para_crack.th_crack_brtness, 255, CV_THRESH_BINARY);//150 暂时取180      //for powder stopper detection on 20201106
	Mat kernal = getStructuringElement(MORPH_RECT, Size(7, 7));
	morphologyEx( temp, temp1 , CV_MOP_OPEN, kernal);
	temp2 = temp - temp1;
	
// get blobs 
	vector<targetBox> boxBlobs;
	findBoundaryBox(temp2, boxBlobs);


// shape analyse: judge whether it is strip
	//Rect rect_blob = Rect( Point(29,60),width, height );
	//struct b
	//focus_box.push_back(a);

	int crackPix = 0;
	//crackPix= countNonZero(temp);//for powder stopper detection on 20201106
	//	int width = im_roi.cols;  //for powder stopper detection on 20201106
	//	int height = im_roi.rows; //for powder stopper detection on 20201106
	//tintPixel(temp , Rect(leftx , bottomy, width, height));// roi)//20201106
	for (int i = 0; i < boxBlobs.size() ; i++)
	{
		int width = boxBlobs[i].boundryBox.width; // box.right - box.left + 1; // width
		int height = boxBlobs[i].boundryBox.height; // height
		assert(width > 0);
		assert(height > 0);
		Point topleft;
		if (width > height)
			topleft = Point(0, (width - height) / 2);
		else
			topleft = Point((height - width) / 2, 0);

		Rect blob_check;
		int length = (width >= height ? width : height);
		Mat square, square1;
		square = Mat::Mat(Size(length, length), CV_8U, Scalar(0));

		Mat temp4 = square(Rect(topleft.x, topleft.y, width, height));
		Mat temp3 = temp2(Rect(boxBlobs[i].boundryBox.x, boxBlobs[i].boundryBox.y, width, height));
		temp3.copyTo(temp4);

		cv::transpose(square, square1);
		cv::flip(square1, square1, 1);
		cv::bitwise_and(square, square1, square1);
		if (float(countNonZero(square1))/ float(boxBlobs[i].area) < 0.25)
		{	//focus_box.push_back(box);
		//Rect rect1 = Rect(leftx + 29, topy + 60, width, height);
			crackPix = crackPix + boxBlobs[i].area;
			tintPixel(temp3, Rect(leftx + boxBlobs[i].boundryBox.x, bottomy + boxBlobs[i].boundryBox.y, width, height));// roi)
		}
	}
	result_crack.crack_area = crackPix;
	if( crackPix > para_crack.th_crack_area )// REJECT: A left too low
    {
	col1 = TXT_DARKRED;
    txt1.Format( string_761[lng], crackPix, para_crack.th_crack_area );
	return 199; 
    }
	else
	{
	col1 = TXT_DARKGREEN;
    txt1.Format( string_761[lng], crackPix, para_crack.th_crack_area );
	return 0;
	}
}


int CTip_inspection::findBoundaryBox(Mat& src, vector<targetBox>& boxList)
{
  	boxList.clear();
	//dst.release();
	Mat dst = src.clone();


	int numBlob = 0;
	Point cur, next;
	int dimy = src.rows;
	int dimx = src.cols;
	int area = 0;
	int leftBox = 0, rightBox = 0, topBox = 0, bottomBox = 0;
	stack<Point> pointStk;

	Rect box;
	targetBox target;
	for (int i = 0; i< dimx; i++)
	{
		//output[i] = 0;
		//output[x+dimx*(dimy-1)] = 0;
		OUTP(i, 0) = 0;
		OUTP(i, (dimy - 1)) = 0;
	}
	for (int j = 0; j<dimy; j++)
	{
		OUTP(0, j) = 0;
		OUTP((dimx - 1), j) = 0;
	}


	for (int j = 1; j < dimy - 1; j++)
	{
		uchar* p = dst.data + j*dst.step;
		for (int i = 1; i < dimx - 1; i++)
		{
			if (p[i] == 255)
			{
				area = 1;
				numBlob++;
				cur = Point(i, j);
				OUTP(cur.x, cur.y) = 0;
				pointStk.emplace(cur);
				//area++;
				leftBox = cur.x;
				rightBox = cur.x;
				topBox = cur.y;
				bottomBox = cur.y;

				while (!pointStk.empty() && pointStk.size()< MAX_DIM)
				{
					for (int index = 0; index < 8; index++)
					{
						next = Point(cur.x + bias8[index][0], cur.y + bias8[index][1]);
						SPREAD_BLOB(next.x, next.y);
					}
					cur = pointStk.top();
					pointStk.pop();
				}

				//target.label = labelValue;
				// select the box with condition
				int width = rightBox - leftBox + 1; // width
				int height = bottomBox - topBox + 1; // height

				assert(width  > 0 && width  < dimx);
				assert(height > 0 && height < dimy);

				float elong = 0.0;

				//if (width >= height) 
				//	elong = (float)width / (float)height; // veritcal crack
				//else 
				elong = (float)height / (float)width; // horizontal crack
				if (area > para_crack.th_blob_area && elong < para_crack.th_blob_ratio && width > para_crack.th_blob_width) //20
				{
					box = cv::Rect(leftBox, topBox, width, height);
					target.area = area;
					target.boundryBox = box;
					boxList.push_back(target);
				}
			}
		}
	}
	return numBlob;

}

int CTip_inspection::vector2Mat(vector< vector<int> > src, Mat &dst)
{
    Mat temp(src.size(),src.at(0).size(),CV_8U);
    for(int i=0; i<temp.rows; ++i)
         for(int j=0; j<temp.cols; ++j)
		 {
			 if( src.at(i).at(j) > 255 || src.at(i).at(j) < 0)
				 return -1;
             temp.at<uchar>(i, j) = src.at(i).at(j);
		 }
    temp.copyTo(dst);
	return 0;
}
//Pour added on 20200320

// mark color on the source image according to the mask and ROI
 int CTip_inspection::tintPixel(Mat& mask, Rect roi)
{
 int cnt=0;
 //assert(mask.rows <= des.rows && mask.cols <= des.cols);
 assert(roi.width == mask.cols && roi.height == mask.rows);
 for(int i = 0; i< mask.cols; i++) 
 {
  for (int j = 0; j < mask.rows; j++)
  {  
   int x = i + roi.x;
   int y = j + roi.y;
     
   int t = *(mask.data + j*mask.step + i);
   if (t >= 200)
   {
    /*uchar* src = (uchar*)(des.data+ y*des.step);
    *(src+3*x) = color.val[0];
    *(src+3*x+1) = color.val[1];
    *(src+3*x+2) = color.val[2];*/
    DRAW_PT(index, x,y,COL_RED);
    cnt++;
   } 
  }
 }
 return cnt;
}


