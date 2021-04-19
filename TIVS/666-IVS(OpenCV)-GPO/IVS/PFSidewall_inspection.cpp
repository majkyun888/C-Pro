#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
//#include "Ampoule_filling_level.h"
//#include "ChildView.h"


//CArea_adjust* areainsp=NULL; // inspection/search area

extern Camera_struct    camera  [NUM_TLC];
//extern CArea_adjust*    areainsp[NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;

//extern BOOL             stn1_part_back;     // In case of selectable TRUE/FALSE BACK/BOTTOM LIGHT particle station 1 

//extern CFilling_level*  fillingl_insp[NUM_TLC]; // filling level inspection
//#define NCRIT_BLOB 4


extern UINT             thread_status;
extern CLogger*         logger;

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif


#define PT(x,y) img_acq[x+(y)*camera[index].width]
//#define FLT(x,y) PT(x,y-6)+PT(x,y+6)-3*PT(x,y)




CPFSidewall_inspection::CPFSidewall_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in/*=NULL*/ )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  mdlresptr = NULL;    // pointer to the Cstring to hold model check result


  mode = mode_in; // PFS_NEEDLESHIELD 128 / PFS_LOWSTOPPER 256

//  ASSERT( mode&PARTICLE_MODE_REFLECTION || mode&PARTICLE_MODE_ADSORPTION || mode&PARTICLE_MODE_BOTH );  // Check AT LEAST one mode selected
//  ASSERT( !( (mode&PARTICLE_MODE_REFLECTION) && (mode&PARTICLE_MODE_ADSORPTION) ) ); // Check NO MORE than one mode selected

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;


//	ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  num_img         =     4;    // number of images for inspection

  // NEEDLE SHIELD INSPECTION PARAMETERS

  learning_needle =     0; // disabled by default
  saving_data     =     0; // disabled by default

  area_color  = COL_BLUE;    // 0xFFRRGGBB assigned color to draw inspection area
  area = new CArea_adjust( index, file_manager );
  swprintf( area->parcat_2, 20, string_5010[lng] );

  xmin           =        4;
  ymin           =       85;
  xmax           =      336;
  ymax           =      316;

  contour_thr    =      0;    // threshold for needle shield
  min_npt        =      100;    // min allowed num of pts for contour
  max_npt        =   NPTCNT;    // max allowed num of pts for contour

  allowance      =      100;    // percentage of stdev allowed for min/max 100=+/-stdev
  maxerr_needle  =     1000;    // maximum allowed error for needle shield


  // STOPPER INSPECTION PARAMETERS
  area2_color     = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
  area2           = new CArea_adjust( index, file_manager );
  swprintf( area2->parcat_2, 20, string_5210[lng] );
//  area2->parcat_2 = string_5210[lng];
  ymin2           =      147;
  ymax2           =      280;
  xmin2           =      486; // dimx/3;
  xmax2           =      598; // 2*dimx/3;

  contour2_thr    =       0;  // threshold for stopper
  min2_npt        =      200;  // min allowed num of pts for contour
  max2_npt        =      600;  // max allowed num of pts for contour

  allowance2      =      100;  // percentage of stdev allowed for min/max 100=+/-stdev
  maxerr_stppr    =     1000;  // maximum allowed error for stopper

  minnptlow       =        0;  // disable orientation check by default

  output          =     NULL;


  maxerr_nedlow   =        0;

#ifdef SIDEWALL_PFS
  if ( mode & PFS_LOWSTOPPER ) // Initialization for low stopper
  {

    //contour_thr     =         0;  // disable needle shield

    // enable orientation analysis
    minnptlow       =       100;       // minimum num of points low, close to bottom to find orientation

    min2_npt        =       500;  // min allowed num of pts for contour
    max2_npt        =      1000;  // max allowed num of pts for contour

    ymin2           =       150;
    ymax2           = ymin2+270;
    xmin2           =       160;
    xmax2           = xmin2+270;

    ymin            = ymin2+ 30;
    ymax            = ymax2- 30;
    xmin            =        50;
    xmax            =       150;

    x1_perc         =        70;    // brightness decrease to detect x1
    x1_band         =         5;    // distance band above/right of x1
    x2_perc         =        70;    // brightness decrease to detect x2
    x2_band         =         5;    // distance band above/right of x2

    min_absl        =        180;   // minimum allowed absolute brightness (area into liquid)

    min_width       =         10;  // minimum width
    min_height      =         10;  // minimum height
    min_dim         =        100;  // minimum dimension of blob
    min_elong       =          1;  // minimum elongation (width/height - height/width on screen)
    min_avgdf       =         10;  // minimum average brightness

    maxerr_cosm     =       1000;  // maximum allowed error for sidewall

    output = (BYTE*) malloc( dimx * dimy );
    ASSERT( output && AfxIsValidAddress(output, dimx*dimy) );

  }
#endif

 // 	//Pour added on 20200320
	//#ifdef KEYPOINTS_ANALYZE
	//
	//para_crack.insp_offset = 15;
	//para_crack.insp_height = 70;
	//para_crack.th_blob_area = 20;
	//para_crack.th_blob_ratio = 6.0;
	//para_crack.th_blob_width = 6;
	//para_crack.th_crack_area = 80 ;
	//para_crack.th_crack_brtness = 150;

	//#endif
	////Pour added on 202003320



  Init_measures(); // Initialize all measure objects



}


CPFSidewall_inspection::~CPFSidewall_inspection(void)
{
  if ( output )
  {
    free( output );
    output = NULL;
  }


  if ( area )
  {
    delete area;
    area = NULL;
  }

  if ( area2 )
  {
    delete area2;
    area2 = NULL;
  }

}


int CPFSidewall_inspection::Set_inspection_area( int ind )
{
//  ASSERT( (UINT)ind>=0 && (UINT)ind<num_tlc );
//  if ( area && area->m_hWnd ) area->ShowWindow( SW_SHOW );

  if ( CHK_BIT( thread_status, 10+((CArea_adjust*)ind)->index ) )
  {
    Message_box( string_2022[lng], MBOX_INFO );
    return( 0 );
  }

  if ( ind==1 ) 
  {


  }

  CArea_adjust* area = (CArea_adjust*) ind; // inspection/search area

  ASSERT( area->index>=0 && area->index<num_tlc ); 
  if ( area && area->m_hWnd && image_dsp[area->index]->visible ) area->ShowWindow( SW_SHOW );

  TRACE("CPFSidewall_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CPFSidewall_inspection::Add_menu_parameters( CContainer_wnd* child )
{

/*  learn_btn = NULL;
	if ( index==12 ) // C2A ONLY
  {
    pView->menu_btn[pView->num_menu_btn+1] = new CMenu_button( string_5020[lng], pView->menu_btn[pView->num_menu_btn]->child, &Learn_needleshield );
    learn_btn = pView->menu_btn[pView->num_menu_btn+1];
    if ( learn_btn ) learn_btn->SetWindowText( L"Start learning" );
  }
  pView->parameter[pView->num_params++] = new CParameter( &num_img,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit[lng], 20,   2, NIMAGES, file_manager ); */


  if ( mode & PFS_NEEDLESHIELD ) // 128    - - - - - -   COSMETIC2 CAMERAS    - - - - - -
  {

    //	if ( index==12 ) pView->parameter[pView->num_params++] = new CParameter( &learning_needle, CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_5020[lng], 0,  0,  0, 1, NULL, &Learn_needleshield );
    learn_btn  = NULL;
    savedt_btn = NULL; // save model data
    if ( /*index==1 || index==2 ||*/ index==12 ) // C3A C3B C2A ONLY
    {
//      pView->menu_btn[pView->num_menu_btn+1] = new CMenu_button( string_5020[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Learn_needleshield, index );
      pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5020[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Learn_needleshield, index );
      learn_btn = pView->menu_btn[pView->num_menu_btn-1];
      //if ( learn_btn ) learn_btn->SetWindowText( L"Start learning" );
	  if ( learn_btn ) learn_btn->SetWindowText( string_5022[lng] );

//      pView->menu_btn[pView->num_menu_btn+2] = new CMenu_button( string_5030[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Datasave_startstop, index );
      pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5030[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Datasave_startstop, index );
      savedt_btn = pView->menu_btn[pView->num_menu_btn-1];
      //if ( savedt_btn ) savedt_btn->SetWindowText( L"Start data saving" );
	  if ( savedt_btn ) savedt_btn->SetWindowText( string_5026[lng] );
    }
#ifdef TIVS_S60
	    if ( /*index==1 || index==2 ||*/ index==15 ) // C3A C3B C2A ONLY
    {
//      pView->menu_btn[pView->num_menu_btn+1] = new CMenu_button( string_5020[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Learn_needleshield, index );
      pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5020[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Learn_needleshield, index );
      learn_btn = pView->menu_btn[pView->num_menu_btn-1];
      //if ( learn_btn ) learn_btn->SetWindowText( L"Start learning" );
	  if ( learn_btn ) learn_btn->SetWindowText( string_5022[lng] );

//      pView->menu_btn[pView->num_menu_btn+2] = new CMenu_button( string_5030[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Datasave_startstop, index );
      pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5030[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Datasave_startstop, index );
      savedt_btn = pView->menu_btn[pView->num_menu_btn-1];
      //if ( savedt_btn ) savedt_btn->SetWindowText( L"Start data saving" );
	  if ( savedt_btn ) savedt_btn->SetWindowText( string_5026[lng] );
    }
#endif


    pView->parameter[pView->num_params++] = new CParameter( &num_img,       CTYPE_DINT,  0, /*pView->menu_btn[pView->num_menu_btn]->*/child, string_2032[lng], string_unit   [lng],   20,   1, NIMAGES, file_manager ); 

    // NEEDLE SHIELD PARAMETERS
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_5010[lng], child );
    file_manager->Add_separator( string_5010[lng], area );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    //	if ( index==12 ) pView->parameter[pView->num_params++] = new CParameter( &learning_needle, CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_5020[lng], 0,  0,  0, 1, NULL, &Learn_needleshield );
/*    learn_btn  = NULL;
    savedt_btn = NULL; // save model data
    if ( index==1 || index==2 || index==12 ) // C3A C3B C2A ONLY
    {
      pView->menu_btn[pView->num_menu_btn+1] = new CMenu_button( string_5020[lng], pView->menu_btn[pView->num_menu_btn]->child, &Learn_needleshield, index );
      learn_btn = pView->menu_btn[pView->num_menu_btn+1];
      if ( learn_btn ) learn_btn->SetWindowText( L"Start learning" );

      pView->menu_btn[pView->num_menu_btn+2] = new CMenu_button( string_5030[lng], pView->menu_btn[pView->num_menu_btn]->child, &Datasave_startstop, index );
      savedt_btn = pView->menu_btn[pView->num_menu_btn+2];
      if ( savedt_btn ) savedt_btn->SetWindowText( L"Start data saving" );
    } */

//    pView->parameter[pView->num_params++] = new CParameter( &num_img,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit   [lng],   20,   1, NIMAGES, file_manager ); 
    pView->parameter[pView->num_params++] = new CParameter( &contour_thr,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702 [lng], string_log    [lng],  0,   0,     255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &allowance,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5106[lng], string_percent[lng], 1000,   0,   99000, file_manager ); 
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_needle, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5130[lng], string_unit   [lng], 1000,   0,   99000, file_manager ); 
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_nedlow, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5140[lng], string_unit   [lng], 1000,   0,   99000, file_manager ); 


 /*   pView->num_menu_btn++;
    pView->num_menu_btn++;
    pView->num_menu_btn++; */

    pView->num_menu_btn++;

//    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-3]->child, &Set_inspection_area, (int) area ); //index );
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );


    // STOPPER PARAMETERS

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_5210[lng], child );
    file_manager->Add_separator( string_5210[lng], area2 );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &contour2_thr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702 [lng], string_log    [lng],  0,  0,     255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &allowance2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5106[lng], string_percent[lng], 1000,   0,   99000, file_manager ); 
    pView->parameter[pView->num_params++] = new CParameter( &minnptlow,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5256[lng], string_unit   [lng], 1000,  0,   99000, file_manager ); 
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_stppr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit   [lng], 1000,  0,   99000, file_manager ); 

    pView->num_menu_btn++;
 //++   pView->num_menu_btn++;

//++    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-2]->child, &Set_inspection_area, (int) area2 ); 
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area2 ); 

    file_manager->Add_data( &ALR_width,  sizeof(measure_type) );
    file_manager->Add_data( &ALR_height, sizeof(measure_type) );
    file_manager->Add_data( &BLR_width,  sizeof(measure_type) );
    file_manager->Add_data( &BLR_height, sizeof(measure_type) );

    file_manager->Add_data( &CLR_width,  sizeof(measure_type) );
    file_manager->Add_data( &DLR_width,  sizeof(measure_type) );
    file_manager->Add_data( &ELR_width,  sizeof(measure_type) );

    file_manager->Add_data( &PAL_PBR,    sizeof(measure_type) );
    file_manager->Add_data( &PAR_PBL,    sizeof(measure_type) );

//    file_manager->Add_data( &AB_offsx,   sizeof(measure_type) );

    file_manager->Add_data( &stppr_width , sizeof(measure_type) );
    file_manager->Add_data( &stppr_height, sizeof(measure_type) );
    file_manager->Add_data( &stppr_vpos  , sizeof(measure_type) );


  } // 128    - - - - - -   COSMETIC2 CAMERAS    - - - - - -


#ifdef SIDEWALL_PFS
  if ( mode & PFS_LOWSTOPPER ) // 256   - - - - - -   COSMETIC3 CAMERAS    - - - - - -
  {

    learn_btn  = NULL;
    savedt_btn = NULL; // save model data
//++      pView->menu_btn[pView->num_menu_btn+1] = new CMenu_button( string_5020[lng], pView->menu_btn[pView->num_menu_btn]->child, &Learn_needleshield, index );
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5020[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Learn_needleshield, index );
    learn_btn = pView->menu_btn[pView->num_menu_btn-1];
    //if ( learn_btn ) learn_btn->SetWindowText( L"Start learning" );
	if ( learn_btn ) learn_btn->SetWindowText( string_5022[lng] );

    //      pView->menu_btn[pView->num_menu_btn+2] = new CMenu_button( string_5030[lng], pView->menu_btn[pView->num_menu_btn]->child, &Datasave_startstop, index );
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_5030[lng], /*pView->menu_btn[pView->num_menu_btn]->*/child, &Datasave_startstop, index );
    savedt_btn = pView->menu_btn[pView->num_menu_btn-1];
    //if ( savedt_btn ) savedt_btn->SetWindowText( L"Start data saving" );
	if ( savedt_btn ) savedt_btn->SetWindowText( string_5026[lng] );


    // STOPPER PARAMETERS ( area2 )
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_5210[lng], child );
    file_manager->Add_separator( string_5210[lng], area2 );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &contour2_thr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702 [lng], string_log    [lng],  0,  0,     255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &allowance2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5106[lng], string_percent[lng], 1000,  0,   99000, file_manager ); 

    pView->parameter[pView->num_params++] = new CParameter( &minnptlow,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5256[lng], string_unit   [lng], 1000,  0,   99000, file_manager ); 

    pView->parameter[pView->num_params++] = new CParameter( &maxerr_stppr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit   [lng], 1000,  0,   99000, file_manager ); 

    pView->num_menu_btn++;
//++    pView->num_menu_btn++;
//++    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-2]->child, &Set_inspection_area, (int) area2 ); 
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area2 ); 



    // SIDEWALL PARAMETERS ( area )
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area2 );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    //pView->parameter[pView->num_params++] = new CParameter( &contour2_thr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_702 [lng], string_log    [lng],  0,  0,     255, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &allowance2,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_5106[lng], string_percent[lng], 1000,   0,   99000, file_manager ); 
    //pView->parameter[pView->num_params++] = new CParameter( &maxerr_stppr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit   [lng], 1000,  0,   99000, file_manager ); 

    /*
    pView->parameter[pView->num_params++] = new CParameter( &x2_perc,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2390[lng], string_percent[lng],  70,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2_band,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2391[lng], string_pix[lng]    ,  20,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1_perc,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2392[lng], string_percent[lng],  70,    0,   100, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1_band,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2393[lng], string_pix[lng]    ,  20,    0,   100, file_manager );
    */

    pView->parameter[pView->num_params++] = new CParameter( &min_absl,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2398[lng], string_log [lng],  160,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_dim,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620 [lng], string_pix [lng], 1000,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_width,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2092[lng], string_pix [lng], 1000,    0, 99000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_height,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2090[lng], string_pix [lng], 1000,    0, 99000, file_manager );
    //pView->parameter[pView->num_params++] = new CParameter( &min_elong,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2094[lng], string_unit[lng],    5,    0,    99, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_avgdf,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2096[lng], string_log [lng],    5,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &maxerr_cosm,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

    pView->num_menu_btn++;
//++    pView->num_menu_btn++;
//++    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-2]->child, &Set_inspection_area, (int) area2 ); 
    pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); 


    file_manager->Add_data( &stppr_width , sizeof(measure_type) );
    file_manager->Add_data( &stppr_height, sizeof(measure_type) );
    file_manager->Add_data( &stppr_vpos  , sizeof(measure_type) );


  } // 256   - - - - - -   COSMETIC3 CAMERAS    - - - - - -

#endif

  return 0; // completed successfully
}


#undef   PT
#define  PT(x,y)  img[x+(y)*dimx]





int CPFSidewall_inspection::Process_image( BYTE* img_acq, UINT nimg )
{
  ASSERT( nimg>=0 && nimg<200 );
  ASSERT( AfxIsValidAddress( img_acq, dim ) );
  ASSERT( nimg>=0 && nimg<NIMAGES );

  CTimer timer;
  CString txt;

  img = img_acq;
  proctime = 0;

  //timer.reset();
  //proctime += timer.elapsed_us();

  if ( mode & PFS_NEEDLESHIELD ) // Cosmetic2 - multiple images Needle shield + stopper
  {
    draw_ovl = 0;

    if ( nimg==0 ) // first image
    {
      good        = 1;   // final inspection result

      err_needle  = 0; // accumulated error for needle shield
      err_nedlow  = 0; // accumulated error for low needle corner
      err_stppr   = 0; // accumulated error for stopper

      col1        = TXT_DARKGREEN;
      col2        = TXT_DARKGREEN;

      errors_txt1 = L"";
      errors_txt2 = L"";

      desc_stppr  = L"Model Match";
      desc_needle = L"Model Match";

      draw_ovl    = 1; // draw only first image

      SCALE_IMG( index, img_acq ); // Scale acq

      // Draw inspection area for NEEDLE SHIELD (if enabled)
      if ( contour_thr>0 )
      {
        for ( int x=xmin ; x<=xmax ; x++ ) //x+=8 ) 
        {
          DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
          DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
        }
        for ( int y=ymin ; y<=ymax ; y++ ) //y+=8 )
        {
          DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
          DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
        }
        //TRACE("NEEDLE SHIELD AREA (%d,%d)-(%d,%d)\n", xmin,ymin,xmax,ymax);
      }

      // Draw inspection area for STOPPER (if enabled)
      if ( contour2_thr>0 )
      {
        for ( int x=xmin2 ; x<=xmax2 ; x++ ) //x+=8 ) 
        {
          DRAW_PT( index, x, ymin2, area2_color ); // 0xFFRRGGBB
          DRAW_PT( index, x, ymax2, area2_color ); // 0xFFRRGGBB
        }
        for ( int y=ymin2 ; y<=ymax2 ; y++ ) //y+=8 )
        {
          DRAW_PT( index, xmin2, y, area2_color ); // 0xFFRRGGBB
          DRAW_PT( index, xmax2, y, area2_color ); // 0xFFRRGGBB
        }
        //TRACE("STOPPER AREA (%d,%d)-(%d,%d)\n", xmin2,ymin2,xmax2,ymax2);
      }

    }

    int err = 0; // error in this image

    // NEEDLE SHIELD ANALYSIS
    if ( contour_thr>0 ) 
    {
      if ( Find_contour_needlesh() ) // some error during contour search
      {
        err = 10000;
        //meas_txt.Format(L"Profile NOT found");
      }
      else // contour found => analyze it
      {
        err = Analyze_contour_needlesh();
      }

      /*   if ( err_needle > maxerr_needle )
      {
      col1 = TXT_DARKRED; // Needle shield REJECT
      good = 0;
      }  */


	//  //Pour added for testing on 20201017

	//  #ifdef KEYPOINTS_ANALYZE

	//cv::Mat needle_img(dimy, dimx, CV_8U, img_acq, dimx);
	//Point P_a_left,P_a_right;
	//P_a_left = Point(ptx[kpts[PB][LEFT]],pty[kpts[PB][LEFT]]);
	//P_a_right = Point(ptx[kpts[PB][RIGHT]],pty[kpts[PB][RIGHT]]);

	//  if (para_crack.th_crack_area)
	//  {
	//	if (detectCrack(needle_img,P_a_left,P_a_right))
	//	{
	//	return 9200; // some problem on neck crack
	//	}
	//  }
	//	#endif

	//  //Pour added for testing on 20201017

      err_needle += err; // accumulate total error

      /*
      // add this image's error
      txt.Format(L"%d ", err );
      errors_txt1 += txt;
      */

      // add maximum allowed at the end
      if ( nimg >= num_img-1 ) // last image - final check 
      {
        if ( learning_needle )
        {
          //txt1.Format( L"REJECT %d/%d %s <%s>", err_needle, maxerr_needle, errors_txt1, meas_txt );
          //col1 = TXT_DARKRED; // Needle shield REJECT
          txt1.Format( L"NEEDLE <%s>", desc_needle );
          col1 = TXT_DARKORANGE; // Needle learning
          good = 0;
        }
        else
        {

          txt1.Format( L"NEEDLE GOOD <%s> %d/%d", desc_needle, err_needle, maxerr_needle );
          col1 = TXT_DARKGREEN; // Needle shield GOOD

          if ( maxerr_needle>0 && err_needle>maxerr_needle )
          {
            txt1.Format( L"NEEDLE REJECT <%s> %d/%d", desc_needle, err_needle, maxerr_needle );
            col1 = TXT_DARKRED; // Needle shield REJECT
            good = 0;
          }

          if ( maxerr_nedlow>0 && err_nedlow>maxerr_nedlow )
          {
            txt1.Format( L"NEEDLE LOW REJECT %d/%d", err_nedlow, maxerr_nedlow );
            col1 = TXT_DARKRED; // Needle shield REJECT
            good = 0;
          }


/*          if ( err_needle > maxerr_needle ) PRECEDENTE SOLO MODELLO NEEDLE
          {
            txt1.Format( L"NEEDLE REJECT <%s> %d/%d", desc_needle, err_needle, maxerr_needle );
            col1 = TXT_DARKRED; // Needle shield REJECT
            good = 0;
          }
          else
          {
            txt1.Format( L"NEEDLE GOOD <%s> %d/%d", desc_needle, err_needle, maxerr_needle );
            col1 = TXT_DARKGREEN; // Needle shield GOOD
          }  */ 


        }
        //      txt.Format(L"/%d", maxerr_needle );
        //      errors_txt1 += txt;
        //      txt1.Append( errors_txt1 );
      }

    }
    else // disabled
    {
      txt1 = string_5006[lng];
      col1 = TXT_BLACK;
    }


    err = 0;

    // STOPPER ANALYSIS
    if ( contour2_thr>0 ) 
    {
      if ( Find_contour_stopper() ) // some error during contour search
      {
        //      err = 1000;
        //      meas_txt.Format(L"Profile NOT found");
        err = 10000;
      }
      else // contour found => analyze it
      {
        err = Analyze_contour_stopper();
      }

      /*    if ( err_stppr > maxerr_stppr )
      {
      col2 = TXT_DARKRED; // Stopper REJECT
      good = 0;
      }  */

      /*
      // add this image's error
      txt.Format(L"%d ", err_stppr );
      errors_txt2 += txt;
      */

      err_stppr += err;

      // add maximum allowed at the end
      if ( nimg >= num_img-1 ) // last image - final check 
      {

        // Final result check
        if ( learning_needle )
        {
          txt2.Format( L"STOPPER %s", desc_stppr );
          col2 = TXT_DARKORANGE;
          good = 0;
        }
        else // Check accumulated error
        {
          if ( err_stppr>maxerr_stppr ) // Reject
          {
            txt2.Format( L"STOPPER REJECT <%s> %d/%d", desc_stppr, err_stppr, maxerr_stppr );
            col2 = TXT_DARKRED;
            good = 0;
          }
          else // GOOD
          {
            txt2.Format( L"STOPPER GOOD <%s> %d/%d", desc_stppr, err_stppr, maxerr_stppr );
            col2 = TXT_DARKGREEN;
          }
        }

        //txt.Format(L"/%d", maxerr_stppr);
        //errors_txt2 += txt;
        //txt2.Append( errors_txt2 );
      }

    }
    else // disabled
    {
      txt2 = string_5206[lng];
      col2 = TXT_BLACK;
    }


  } // 128 - Cosmetic2 - multiple images Needle shield + stopper


  // ---------------------------------------------------------------------------------------------------------

  if ( mode & PFS_LOWSTOPPER ) // 256 - Cosmetic3 - single image stopper + sidewall - area2
  {
    good        = 1;   // final inspection result
    draw_ovl    = 1; // draw only first image
    SCALE_IMG( index, img_acq ); // Scale acq

    // STOPPER ANALYSIS
    if ( contour2_thr>0 ) 
    {
      err_stppr = 0;

      // Draw inspection area for STOPPER (if enabled)
      if ( contour2_thr>0 )
      {
        for ( int x=xmin2 ; x<=xmax2 ; x++ ) //x+=8 ) 
        {
          DRAW_PT( index, x, ymin2, area2_color ); // 0xFFRRGGBB
          DRAW_PT( index, x, ymax2, area2_color ); // 0xFFRRGGBB
        }
        for ( int y=ymin2 ; y<=ymax2 ; y++ ) //y+=8 )
        {
          DRAW_PT( index, xmin2, y, area2_color ); // 0xFFRRGGBB
          DRAW_PT( index, xmax2, y, area2_color ); // 0xFFRRGGBB
        }
        //TRACE("STOPPER AREA (%d,%d)-(%d,%d)\n", xmin2,ymin2,xmax2,ymax2);
      }


      if ( Find_contour_stopper() ) // some error during contour search
      {
        err_stppr = 10000;
      }
      else // contour found => analyze it
      {
        desc_stppr = L"Model Match";
        err_stppr += Analyze_contour_stopper();
      }

      // Final result check
      if ( learning_needle )
      {
        txt1.Format( L"STOPPER %s", desc_stppr );
        col1 = TXT_DARKORANGE;
        good = 0;
      }
      else // Check accumulated error
      {
        if ( err_stppr>maxerr_stppr ) // Reject
        {
          txt1.Format( L"STOPPER REJECT <%s> %d/%d", desc_stppr, err_stppr, maxerr_stppr );
          col1 = TXT_DARKRED;
          good = 0;
        }
        else // GOOD
        {
          txt1.Format( L"STOPPER GOOD <%s> %d/%d", desc_stppr, err_stppr, maxerr_stppr );
          col1 = TXT_DARKGREEN;
        }
      }

    }
    else // disabled
    {
      txt1 = string_5206[lng];
      col1 = TXT_BLACK;
    }


    if ( maxerr_cosm > 0 )
    {
      // Draw inspection area for lower sidewall (if enabled)
      for ( int x=xmin ; x<=xmax ; x++ ) //x+=8 ) 
      {
        DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
      }
      for ( int y=ymin ; y<=ymax ; y++ ) //y+=8 )
      {
        DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
        DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
      }
      //TRACE("NEEDLE SHIELD AREA (%d,%d)-(%d,%d)\n", xmin,ymin,xmax,ymax);

      Sidewall_PFS( img_acq );

      if ( cosm_err>maxerr_cosm ) // Reject
      {
        txt2.Format( L"SIDEWALL REJECT %d/%d", cosm_err, maxerr_cosm );
        col2 = TXT_DARKRED;
        good = 0;
      }
      else // GOOD
      {
        txt2.Format( L"SIDEWALL GOOD %d/%d", cosm_err, maxerr_cosm );
        col2 = TXT_DARKGREEN;
      }
    }
    else // disabled
    {
      txt2 = string_5260[lng];
      col2 = TXT_BLACK;
    }


  }
  

  return 0; // completed successfully  
  
}


extern int ATNLUT[DIMATN][DIMATN];  // array[dx][dy]#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DY pty[i+AMPATN]-pty[i-AMPATN]
#define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]



int CPFSidewall_inspection::Init_measure( wchar_t** desc, measure_type* meas )
{
  ASSERT( meas && AfxIsValidAddress( meas, sizeof( measure_type ) ) );
  ASSERT( AfxIsValidString( desc[lng] ) );

//  ZeroMemory( meas->desc, sizeof(meas->desc) );
  ZeroMemory( meas, sizeof(measure_type) ); // clear everything
//  TRACE("Max desc Lenght=%d\n",sizeof(meas->desc)); // 16 wchar_t = 32 bytes

  swprintf_s( meas->desc, MAXLNGTDESC, desc[lng] ); // copy description string
  //meas->max   =    0;
  meas->min   = int(1E9);  // maxint
  //meas->val   =    0;
  //meas->nsmpl =    0;


//  TRACE(L"Meas <%s> initialized\n", meas->desc[lng] );
  return 0;
}


int CPFSidewall_inspection::Init_measures( int val )
{
  Init_measure( string_5110, &ALR_width  ); // A l-r width
  Init_measure( string_5112, &ALR_height ); // A l-r height
  Init_measure( string_5114, &BLR_width  ); // B l-r width
  Init_measure( string_5116, &BLR_height ); // B l-r height
 // Init_measure( string_5118, &AB_offsx   ); // hor offset/tilt of needlesh

  Init_measure( string_5120, &CLR_width  ); // C l-r width
  Init_measure( string_5122, &DLR_width  ); // D l-r width
  Init_measure( string_5124, &ELR_width  ); // E l-r width

  Init_measure( string_5126, &PAL_PBR    ); // diagonal PAL-PBR
  Init_measure( string_5128, &PAR_PBL    ); // diagonal PAR-PBL

  Init_measure( string_5240, &stppr_width  ); // outer width  of stopper
  Init_measure( string_5242, &stppr_height ); // outer height of stopper
  Init_measure( string_5244, &stppr_vpos   ); // vertical pos of stopper


/*  ALR_width_txt  = string_5110;  // description text pointer
  ALR_heigth_txt = string_5112; // description text pointer
  BLR_width_txt  = string_5114;  // description text pointer
  BLR_heigth_txt = string_5116; // description text pointer */

  return 0;
}


//int CPFSidewall_inspection::Calc_average( wchar_t** txt, measure_type* meas )
int CPFSidewall_inspection::Calc_average( measure_type* meas )
{
  ASSERT( meas && AfxIsValidAddress( meas, sizeof( measure_type ) ) );
  ASSERT( meas->desc && AfxIsValidString( meas->desc ) );

//  if ( meas->nsmpl<10 ) 
  if ( meas->nsmpl<3 ) 
  {
    meas_txt.Format( string_5034[lng], meas->nsmpl, meas->desc );
    Message_box( meas_txt, MBOX_ERROR );
    if ( saving_data ) Save_text( &meas_txt ); //meas_txt.GetBuffer(0) );
    return 99;
  }

  meas->avg = meas->val + double(meas->ex)/meas->nsmpl;
  meas->std = sqrt((meas->ex2-double(meas->ex*meas->ex)/meas->nsmpl)/(meas->nsmpl-1.0));  // (Ex2 - (Ex * Ex) / n) / (n - 1)

  meas->min = int( 0.5 + meas->avg - 0.01*allowance*meas->std );
  meas->max = int( 0.5 + meas->avg + 0.01*allowance*meas->std );

  meas_txt.Format(L"Measure <%s> nsample=%d avg=%.6lf std=%.6lf min=%d max=%d", meas->desc, meas->nsmpl, meas->avg, meas->std, meas->min, meas->max );
  if ( saving_data ) Save_text( &meas_txt ); //meas_txt.GetBuffer(0) );




  /* VECCHIO MIN/MAX
  if ( meas->nsmpl<1 ) 
  {
    meas_txt.Format( L"Error: no samples for <%s>", meas->desc );
    Save_text( meas_txt.GetBuffer(0) );
//    meas_txt.Format( L"Error: no samples for <%s>", txt[lng] );
    return 99;
  }

  meas->val = int( 0.5 + double(meas->val)/double(meas->nsmpl ) );
  meas_txt.Format(L"Measure <%s> nsample=%d avg=%d min=%d max=%d", meas->desc, meas->nsmpl, meas->val, meas->min, meas->max );
  Save_text( meas_txt.GetBuffer(0) );
//  TRACE(L"Average for <%s> calculated %d\n", txt[lng], meas->val );
   */



//  ASSERT( meas->min <= meas->val && meas->val <= meas->max );
  ASSERT( meas->min <= meas->max );
  return 0;
}



int CPFSidewall_inspection::Save_text( CString* txt, int addeol )
{
  Save_text( txt->GetBuffer(0), addeol );
  return 0;
}

//int CPFSidewall_inspection::Save_text( char* txt )
//int CPFSidewall_inspection::Save_text( wchar_t* txt )
int CPFSidewall_inspection::Save_text( wchar_t* txt, int addeol )
{
  wchar_t* EOL = L"\r\n";
  ASSERT( txt ); //&& AfxIsValidString(txt) );
//  if ( saving_data && datafp.m_hFile ) datafp.Write( txt, strlen(txt) );
  if ( saving_data && datafp.m_hFile )
  {
    //datafp.Write( txt, wcslen(txt)*sizeof(wchar_t) );
    datafp.Write( txt, wcslen(txt)*sizeof(wchar_t) );
    if ( addeol ) datafp.Write( EOL, 4 ); //wcslen(EOL)*sizeof(wchar_t) );
    //TRACE( "%d bytes\n", wcslen(EOL)*sizeof(wchar_t) );
  }
  return 0;
}


int CPFSidewall_inspection::Saving_switch( void ) // Start/stop data saving
{
//  char txt[100];
//  char txt[100];
  wchar_t txt[100];

  if ( saving_data ) /// active => stop it
  {
    swprintf( txt, L"End of saved model data for camera TC[%d]", index );
    if ( saving_data ) Save_text( txt );

    if ( savedt_btn ) savedt_btn->SetWindowText( string_5026[lng] );
    saving_data = 0;

    //if ( datafp.m_hFile ) 
//    txt.Format(L"End of model data for camera TC[%d]", index );
//    datafp.Write( txt.GetBuffer(0), txt.GetLength() );
//    sprintf( txt, "End of model data for camera TC[%d]\r\n", index );
    //datafp.Write( txt, strlen(txt) );

    datafp.Close();
    TRACE("Datasave_startstop: STOPPED - file closed\n");
  }
  else // disabled => start it
  {
    if ( savedt_btn ) savedt_btn->SetWindowText( string_5028[lng] );
    saving_data = 1;

    CTime   time = CTime::GetCurrentTime();   
    CString name;
    name.Format( L"TC[%d]DATA_%04d%02d%02d.txt", index, time.GetYear(), time.GetMonth(), time.GetDay() );

    CFileException ex;

    if ( !datafp.Open( name, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::osWriteThrough|CFile::shareDenyNone, &ex ) )
    {
      ex.ReportError();
      //return 99;
    }

    datafp.SeekToEnd();

    //txt.Format(L"Model data for camera TC[%d]", index );
    //sprintf( txt, "%02d:%02d:%02d - Model data for camera TC[%d]", time.GetHour(), time.GetMinute(), time.GetSecond(), index );
    swprintf( txt, L"%02d:%02d:%02d - Model data for camera TC[%d]", time.GetHour(), time.GetMinute(), time.GetSecond(), index );
    if ( saving_data ) Save_text( txt );

//    datafp.Write( txt, strlen(txt) );


    TRACE(L"Datasave_startstop: STARTED - File <%s> open\n", name);
  }

  return 0;
}


int CPFSidewall_inspection::Learning_switch( void )
{
  wchar_t txt[100];


  if ( learning_needle ) /// active => stop it
  {
    swprintf( txt, L"Stop of learning for camera TC[%d]", index );
    if ( saving_data ) Save_text( txt );

    if ( learn_btn ) learn_btn->SetWindowText( string_5022[lng] );
    learning_needle = 0;

    // calculate all averages
/*    Calc_average( ALR_width_txt,  &ALR_width  ); // A l-r width
    Calc_average( ALR_heigth_txt, &ALR_height ); // A l-r height
    Calc_average( BLR_width_txt,  &BLR_width  ); // B l-r width
    Calc_average( BLR_heigth_txt, &BLR_height ); // B l-r height */

    if ( contour_thr>0 )
    {
      Calc_average( &ALR_width  ); // A l-r width
      Calc_average( &ALR_height ); // A l-r height
      Calc_average( &BLR_width  ); // B l-r width
      Calc_average( &BLR_height ); // B l-r height
      //Calc_average( &AB_offsx   ); // hor offset/tilt of needlesh
      Calc_average( &CLR_width  ); // C l-r width
      Calc_average( &DLR_width  ); // D l-r width
      Calc_average( &ELR_width  ); // E l-r width

      Calc_average( &PAL_PBR    ); // diagonal PAL-PBR
      Calc_average( &PAR_PBL    ); // diagonal PAR-PBL

    }

    if ( contour2_thr>0 )
    {
      Calc_average( &stppr_width  ); // outer width  of stopper 
      Calc_average( &stppr_height ); // outer height of stopper
      Calc_average( &stppr_vpos   ); // vertical position of lower part of stopper 
    }

    //TRACE("CPFSidewall_inspection[%d] LEARNING STOPPED\n",index);

  }
  else // disabled => start it
  {
    swprintf( txt, L"Start of learning for camera TC[%d]", index );
    if ( saving_data ) Save_text( txt );

    if ( learn_btn ) learn_btn->SetWindowText( string_5024[lng] );
    learning_needle = 1;
    Init_measures(); // init all measures
    //TRACE("CPFSidewall_inspection[%d] LEARNING STARTED\n",index);
  }

  return 0;
}



//int CPFSidewall_inspection::Measure_check( int val, wchar_t** desc, measure_type* meas )
int CPFSidewall_inspection::Measure_check( int val, measure_type* meas )
{
  ASSERT( meas );
//  ASSERT( meas->min<=meas->max );
//  ASSERT( meas->nsmpl>=0 && meas->nsmpl<=1000 );
//  ASSERT( desc && AfxIsValidString( desc[lng] ) );
  ASSERT( meas->desc && AfxIsValidString( meas->desc ) );


//  fp.SeekToEnd();
//  fp.Write( buff, wcslen(buff)*sizeof(wchar_t) );

  /*  VECCHIO MIN/MAX
  if ( learning_needle ) // if learning active
  {
    if ( meas->max < val ) meas->max = val;
    if ( meas->min > val ) meas->min = val;
    meas->val += val;
    meas_txt.Format(L"<%s> learning[%d] %d", meas->desc, meas->nsmpl, val );
    Save_text( meas_txt.GetBuffer(0) );
//    meas_txt.Format(L"<%s> learning[%d] %d", desc[lng], meas->nsmpl, val );
    meas->nsmpl++;
    return 0;
  }
  */

  if ( learning_needle ) // if learning active
  {
    if ( meas->nsmpl==0 ) meas->val = val;
    meas->ex  +=  val-meas->val;
    meas->ex2 += (val-meas->val)*(val-meas->val);
    meas->nsmpl++;
    //meas_txt.Format(L"<%s> learning[%d] %d", meas->desc, meas->nsmpl, val );
    meas_txt.Format(L"%d,%d,",meas->nsmpl,val);
    if ( saving_data ) Save_text( &meas_txt, 0 );
    return 6000;
  }



  // check/processing
  if ( meas->nsmpl<1 )
  {
    //desc_stppr.Format(L"<%s> NO MODEL DATA", meas->desc );
    //if ( saving_data ) Save_text( &desc_stppr );

    if ( mdlresptr ) 
    {
      mdlresptr->Format( string_5186[lng], meas->desc );
      if ( saving_data ) Save_text( mdlresptr );
    }
    return 7000; // reject, no model data
  }

//  meas->min = int( 0.5 + meas->avg - 0.01*allowance*meas->std );
//  meas->max = int( 0.5 + meas->avg + 0.01*allowance*meas->std );

//  meas->min = DOUBLETOINT( meas->avg - 0.01*allowance*meas->std );
//  meas->max = DOUBLETOINT( meas->avg + 0.01*allowance*meas->std );

  meas->min = DOUBLETOINT( meas->avg - 0.03*allowance*meas->std );
  meas->max = DOUBLETOINT( meas->avg + 0.03*allowance*meas->std );

  if ( val<meas->min )
  {
//    desc_stppr.Format(L"<%s> too low %d/%d", meas->desc, val, meas->min );
//    if ( saving_data ) Save_text( &desc_stppr );
    if ( mdlresptr ) 
    {
      mdlresptr->Format( string_5184[lng], meas->desc, val, meas->min );
      if ( saving_data ) Save_text( mdlresptr );
    }

    return 100; // reject, value too small
  }

  if ( val>meas->max )
  {
    //desc_stppr.Format(L"<%s> too high %d/%d", meas->desc, val, meas->max );
    //if ( saving_data ) Save_text( &desc_stppr );
    if ( mdlresptr ) 
    {
      mdlresptr->Format( string_5182[lng], meas->desc, val, meas->max );
      if ( saving_data ) Save_text( mdlresptr );
    }

//    meas_txt.Format(L"<%s> too high %d/%d", desc[lng], val, meas->max );
    return 200; // reject, value too small
  }

  //meas_txt.Format(L"<%s> Good %d %d/%d", meas->desc, val, meas->min, meas->max );
  if ( saving_data )
  {
    CString txt;
    txt.Format( string_5180[lng], meas->desc, val, meas->min, meas->max );
    Save_text( &txt );
//    meas_txt.Format(L"<%s> Good %d %d/%d", meas->desc, val, meas->min, meas->max );
  }
  return 0;
}

  
int CPFSidewall_inspection::Calc_distance( int pt1, int pt2 )
{
  if ( pt1<0 || pt1>=ptn ) return 199; // pt1 out of range
  if ( pt2<0 || pt2>=ptn ) return 299; // pt2 out of range

  return( int( 0.5 + sqrt( double(ptx[pt1]-ptx[pt2])*(ptx[pt1]-ptx[pt2]) + (pty[pt1]-pty[pt2])*(pty[pt1]-pty[pt2]) ) ) );
}


int CPFSidewall_inspection::Analyze_contour_needlesh( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF; // 0xFFRRGGBB
 // UINT col_cross   = 0xFF00FFFF; // on fulldsp image

/*  if ( ptmid<40 || ptmid>ptn-40 )
  {
    txt1.Format( string_696[lng] );
    return 1299;
  }  */

  ptmid = ptn / 2; // start from middle

  imin =     20;
  imax = ptn-20;

  int xtop = xmax;
  int itop = 0;

  for ( i=imin ; i<imax ; i+= 20 )
  {
    if ( xtop>ptx[i] )
    {
      xtop = ptx[i];
      itop = i;
    }
  }

  mdlresptr = &desc_needle;

//  for ( i=AMPATN ; i<ptn-AMPATN ; i++ ) TRACE( "%d,%d\n", i, ATAN(i) );
  // SEARCH UPPER POINTS - CCW - LEFT SIDE


  int dlt  = 10;
  int xdx  =  0;
  int jump =  8;
//  i        =  ptmid-jump;
  i        =  itop-jump;

  // check if ptmid is on top area
  do 
  {
    i = i+jump;
    if ( jump>0 ) jump = -(jump+8);
    else          jump = -(jump-8);
    xdx = (ptx[i+dlt]-ptx[i-dlt])*(ptx[i+dlt]-ptx[i-dlt]);
    //TRACE("i=%3d - jump=%4d - xdx=%5d\n", i, jump, xdx );
  } 
  while ( xdx>16 && i>imin && i<imax );

  ptmid = i;

  // Draw midpoint
  DRAW_SQUARE(index,ptx[i],pty[i], COL_BLUE ); // 0xFFRRGGBB

  // Draw profile right/left endpoints
  DRAW_SQUARE(index,ptx[    0],pty[    0], COL_DARKGREEN ); // 0xFFRRGGBB
  DRAW_SQUARE(index,ptx[ptn-1],pty[ptn-1], COL_DARKGREEN ); // 0xFFRRGGBB


/*  for ( i=AMPATN ; i<ptn-AMPATN ; i++ ) // while ( i>imin )
  {
    TRACE( "pt[%3d] = %3d,%3d - atan=%5d\n", i, ptx[i], pty[i], ATAN(i) );
    //i--;
  }*/  


  i=ptmid;

  // left/lower side
  while ( ( ATAN(i)>13500 && i>imin ) ) i--;
  if ( i<=imin )
  {
    desc_needle.Format( string_5170[lng], L"PAL" );
    return 1010; // unable to find point PAL - return error
  }
  kpts[PA][LEFT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
//  if ( draw_ovl ) DRAW_PT(index,ptx[i],pty[i],col_markers);

  i-=10;

  while ( ( ATAN(i)>4500 && i>imin ) ) i--;
  if ( i<=imin )
  {
    desc_needle.Format( string_5170[lng], L"PBL" );
    return 1011; // unable to find point PBL - return error
  }
  kpts[PB][LEFT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
//  if ( draw_ovl ) DRAW_PT(index,ptx[i],pty[i],col_markers);
  i-=10;

  i=ptmid; 
  // right/upper side
  while ( ( ATAN(i)<22500 && i<imax ) ) i++;
  if ( i>=imax )
  {
    desc_needle.Format( string_5170[lng], L"PAR" );
    return 1012; // unable to find point PAL - return error
  }
  kpts[PA][RIGHT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
//  if ( draw_ovl ) DRAW_PT(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ( ATAN(i)<31500 && i<imax ) ) i++;
  if ( i<=imin )
  {
    desc_needle.Format( string_5170[lng], L"PBR" );
    return 1013; // unable to find point PAL - return error
  }
  kpts[PB][RIGHT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
//  if ( draw_ovl ) DRAW_PT(index,ptx[i],pty[i],col_markers);
  i+=10;


  TRACE("PAL %3d,%3d,%3d\n", kpts[PA][LEFT ], ptx[kpts[PA][LEFT ]], pty[kpts[PA][LEFT ]] );
  TRACE("PBL %3d,%3d,%3d\n", kpts[PB][LEFT ], ptx[kpts[PB][LEFT ]], pty[kpts[PB][LEFT ]] );
  TRACE("PAR %3d,%3d,%3d\n", kpts[PA][RIGHT], ptx[kpts[PA][RIGHT]], pty[kpts[PA][RIGHT]] );
  TRACE("PBR %3d,%3d,%3d\n", kpts[PB][RIGHT], ptx[kpts[PB][RIGHT]], pty[kpts[PB][RIGHT]] );


  kpts[PC][LEFT ] = kpts[PA][LEFT ] - int( 0.5 + 0.25*(kpts[PA][LEFT ]-kpts[PB][LEFT ]) );
  kpts[PD][LEFT ] = kpts[PA][LEFT ] - int( 0.5 + 0.50*(kpts[PA][LEFT ]-kpts[PB][LEFT ]) );
  kpts[PE][LEFT ] = kpts[PA][LEFT ] - int( 0.5 + 0.75*(kpts[PA][LEFT ]-kpts[PB][LEFT ]) );

  kpts[PC][RIGHT] = kpts[PA][RIGHT] + int( 0.5 + 0.25*(kpts[PB][RIGHT]-kpts[PA][RIGHT]) );
  kpts[PD][RIGHT] = kpts[PA][RIGHT] + int( 0.5 + 0.50*(kpts[PB][RIGHT]-kpts[PA][RIGHT]) );
  kpts[PE][RIGHT] = kpts[PA][RIGHT] + int( 0.5 + 0.75*(kpts[PB][RIGHT]-kpts[PA][RIGHT]) );

  if ( draw_ovl )
  {
/*    DRAW_PT(index,ptx[kpts[PC][LEFT ]],pty[kpts[PC][LEFT ]],col_markers);
    DRAW_PT(index,ptx[kpts[PD][LEFT ]],pty[kpts[PD][LEFT ]],col_markers);
    DRAW_PT(index,ptx[kpts[PE][LEFT ]],pty[kpts[PE][LEFT ]],col_markers);
    DRAW_PT(index,ptx[kpts[PC][RIGHT]],pty[kpts[PC][RIGHT]],col_markers);
    DRAW_PT(index,ptx[kpts[PD][RIGHT]],pty[kpts[PD][RIGHT]],col_markers);
    DRAW_PT(index,ptx[kpts[PE][RIGHT]],pty[kpts[PE][RIGHT]],col_markers); */
    DRAW_SQUARE(index,ptx[kpts[PC][LEFT ]],pty[kpts[PC][LEFT ]],col_markers);
    DRAW_SQUARE(index,ptx[kpts[PD][LEFT ]],pty[kpts[PD][LEFT ]],col_markers);
    DRAW_SQUARE(index,ptx[kpts[PE][LEFT ]],pty[kpts[PE][LEFT ]],col_markers);
    DRAW_SQUARE(index,ptx[kpts[PC][RIGHT]],pty[kpts[PC][RIGHT]],col_markers);
    DRAW_SQUARE(index,ptx[kpts[PD][RIGHT]],pty[kpts[PD][RIGHT]],col_markers);
    DRAW_SQUARE(index,ptx[kpts[PE][RIGHT]],pty[kpts[PE][RIGHT]],col_markers);
  }


  int val;
  int err = 0;


/*  int dia1, dia2;
  dia1 = Calc_distance( kpts[PA][LEFT ], kpts[PB][RIGHT] );
  dia2 = Calc_distance( kpts[PA][RIGHT], kpts[PB][LEFT ] );
  TRACE("Dia1/2 = %3d,%3d\n", dia1, dia2 ); */



//#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
//#define DY pty[i+AMPATN]-pty[i-AMPATN]
//#define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]

//  #define ATAN2(i) ATNLUT[ptx[i+AMPATN]-ptx[i-AMPATN]+AMPATN*2][pty[i+AMPATN]-pty[i-AMPATN]+AMPATN*2]
//  #define ATAND1(i,j) ATNLUT[ptx[i]-ptx[j]+AMPATN*2][pty[i]-pty[j]+AMPATN*2]
//  #undef ATAN2
//  #undef ATAND1 //(i,j)


  // TABELLA ORIENTAMENTO




  if ( maxerr_nedlow>0 ) // If needle bottom check is enabled
  {

    //          DX DY
    int tabl[3][3];
    int tabr[3][3];

    ZeroMemory( tabl, sizeof(tabl) );
    ZeroMemory( tabr, sizeof(tabr) );


    //  for ( i=kpts[PB][LEFT ]-10 ; i<=kpts[PB][LEFT ]+10 ; i++ )
    for ( int j=-10 ; j<=10 ; j++ )
    {
      int il = kpts[PB][LEFT ]-j; // scan left  side downwards
      int ir = kpts[PB][RIGHT]+j; // scan right side downwards


      int dx = ptx[il]-ptx[il+1];
      int dy = pty[il]-pty[il+1];

      tabl[dx+1][dy+1]++;

      dx = ptx[ir]-ptx[ir-1];
      dy = pty[ir]-pty[ir-1];

      tabr[dx+1][dy+1]++;

    }

//  TRACE("COUNTING    %2d %2d %2d   %2d %2d %2d\n", tabl[0][0], tabl[1][0], tabl[2][0], tabr[0][0], tabr[1][0], tabr[2][0] );
//  TRACE("            %2d %2d %2d   %2d %2d %2d\n", tabl[0][1], tabl[1][1], tabl[2][1], tabr[0][1], tabr[1][1], tabr[2][1] );
//  TRACE("            %2d %2d %2d   %2d %2d %2d\n", tabl[0][2], tabl[1][2], tabl[2][2], tabr[0][2], tabr[1][2], tabr[2][2] );

// NORMALE      LEFT       RIGHT     
// COUNTING     0  7  5    0  0  0
//              0  0  9    0  0  8
//              0  0  0    0  8  5

    err_nedlow += tabl[0][0] + tabl[0][1] + tabl[0][2] + tabl[1][2] + tabl[2][2]; // error on the left  side

    err_nedlow += tabr[0][0] + tabr[1][0] + tabr[2][0] + tabr[0][1] + tabr[0][2]; // error on the right side


 /*   if ( nedlow_err>nedlow_maxerr ) // needle low reject
    {

    }
    else // needle low good
    {

    }  */
  }


//    TRACE("i=%3d - LEFT %3d,%3d A=%5d D=%4d - RIGHT %3d,%3d A=%5d D=%4d\n", j, ptx[il], pty[il], ATAN2(il), ATAN2(il)-ATAN2(il-1), 
//                                                                               ptx[ir], pty[ir], ATAN2(ir), ATAN2(ir)-ATAN2(ir-1) );
/*    int dl = ATAN2(il)-ATAN2(il-1);
    int dr = ATAN2(ir)-ATAN2(ir-1);
    if ( dl<0 || dl>1000 ) DRAW_PT(index,ptx[il],pty[il],COL_RED);
    if ( dr<0 || dr>1000 ) DRAW_PT(index,ptx[ir],pty[ir],COL_RED); */
//    int al = ATAND1(il);
//    TRACE("j=%3d LEFT %3d,%3d al=%5d - RIGHT %3d,%3d al=%5d\n", j, ptx[il], pty[il], ATAND1(il,il+1), ptx[ir], pty[ir], ATAND1(ir,ir-1) );
//    TRACE("i=%3d %3d,%3d\n", i, ptx[i], pty[i] );


//  TRACE("COUNTING    %2d %2d %2d\n", tabl[0][0], tabl[1][0], tabl[2][0] );
//  TRACE("            %2d %2d %2d\n", tabl[0][1], tabl[1][1], tabl[2][1] );
//  TRACE("            %2d %2d %2d\n", tabl[0][2], tabl[1][2], tabl[2][2] );




   

  // ALR_width
  val = Calc_distance( kpts[PA][LEFT], kpts[PA][RIGHT] );
  err += Measure_check( val, &ALR_width );
  //TRACE(L"ALR_width=%d - result: %s\n", val, meas_txt );
//  err += Measure_check( val, ALR_width_txt, &ALR_width );
//  TRACE(L"ALR_width=%d - result: %s\n", val, meas_txt );

  // ALR_height
  val = int( 0.5 + 0.5*(2*dimx-ptx[kpts[PA][LEFT]]-ptx[kpts[PA][RIGHT]]));    
  err += Measure_check( val, &ALR_height );
//    err += Measure_check( val, ALR_heigth_txt, &ALR_height );
//    TRACE(L"ALR_height=%d - result: %s\n", val, meas_txt );

  // BLR_width
  val = Calc_distance( kpts[PB][LEFT], kpts[PB][RIGHT] );
  err += Measure_check( val, &BLR_width );
//    err += Measure_check( val, BLR_width_txt, &BLR_width );
//    TRACE(L"BLR_width=%d - result: %s\n", val, meas_txt );

  // BLR_height
  val = int( 0.5 + 0.5*(2*dimx-ptx[kpts[PB][LEFT]]-ptx[kpts[PB][RIGHT]]));
  err += Measure_check( val, &BLR_height );
//    err += Measure_check( val, BLR_heigth_txt, &BLR_height );
//    TRACE(L"BLR_height=%d - result: %s\n", val, meas_txt );


  // CLR_width
  val = Calc_distance( kpts[PC][LEFT], kpts[PC][RIGHT] );
  err += Measure_check( val, &CLR_width );

  // DLR_width
  val = Calc_distance( kpts[PD][LEFT], kpts[PD][RIGHT] );
  err += Measure_check( val, &DLR_width );

  // ELR_width
  val = Calc_distance( kpts[PE][LEFT], kpts[PE][RIGHT] );
  err += Measure_check( val, &ELR_width );

  // diagonal PAL-PBR
  val = Calc_distance( kpts[PA][LEFT ], kpts[PB][RIGHT] );
  err += Measure_check( val, &PAL_PBR );

  // diagonal PAR-PBL
  val = Calc_distance( kpts[PA][RIGHT], kpts[PB][LEFT ] );
  err += Measure_check( val, &PAR_PBL );






  /*
  // AB horizontal offset (=TILT) of needle shield
  //if ( err==0 )
  {
    val = int( 0.5 + 0.5*( pty[kpts[PA][LEFT]]+pty[kpts[PA][RIGHT]]-pty[kpts[PB][LEFT]]-pty[kpts[PB][RIGHT]] ) );
    err += Measure_check( val, &AB_offsx );
//    err += Measure_check( val, BLR_heigth_txt, &BLR_height );
//    TRACE(L"AB_offsx=%d - result: %s\n", val, meas_txt );
  }
  */

//  err_needle += err;

  //meas_txt.Format(L"Learning" );
  if ( saving_data ) Save_text( L"" );



  // IF learning just return
  if ( learning_needle )
  {
    //txt1.Format( string_5014[lng], ALR_width.nsmpl, ALR_height.nsmpl, BLR_width.nsmpl, BLR_height.nsmpl );
    //col1 = TXT_DARKORANGE;
    desc_needle.Format(string_5150[lng], ALR_width.nsmpl, ALR_height.nsmpl, BLR_width.nsmpl, BLR_height.nsmpl, CLR_width.nsmpl );
    return 1000; // reject during learning
  }

  return err;

/*  if ( err==0 ) // All GOOD
  {
    txt1.Format( string_5016[lng] );
    col1 = TXT_DARKGREEN;
    return 0; // reject during learning
  }

  txt1.Format( string_5018[lng], meas_txt);
  col1 = TXT_DARKRED;

  return 100; // SOME REJECT */

}




int CPFSidewall_inspection::Draw_boundingbox( int x1, int y1, int x2, int y2, UINT col )
{
  ASSERT( x1<x2 && y1<y2 );

  // DRAW BOUNDING BOX
  for ( int i=1 ; i<20 ; i++ )
  {
    DRAW_PT(index,x1+i,y1  ,col);
//    DRAW_PT(index,x1+i,y1+3,  0);
    DRAW_PT(index,x1+i,y2  ,col);
//    DRAW_PT(index,x1+i,y2-3,  0);
    DRAW_PT(index,x2-i,y1  ,col);
//    DRAW_PT(index,x2-i,y1+3,  0);
    DRAW_PT(index,x2-i,y2  ,col);
//    DRAW_PT(index,x2-i,y2-3,  0);

    DRAW_PT(index,x1  ,y1+i,col);
//    DRAW_PT(index,x1+3,y1+i,  0);
    DRAW_PT(index,x2  ,y1+i,col);
//    DRAW_PT(index,x2-3,y1+i,  0);
    DRAW_PT(index,x1  ,y2-i,col);
//    DRAW_PT(index,x1+3,y2-i,  0);
    DRAW_PT(index,x2  ,y2-i,col);
//    DRAW_PT(index,x2-3,y2-i,  0);
  }

  return 0;
}



// RES!=0 REJECT - RES==0 GOOD
int CPFSidewall_inspection::Draw_stopper_updown( int pos, int res )
{
  int i;
  if ( res ) // REJECT
  {
    for ( i=stymin+40 ; i<=stymax-40 ; i++ )
    {
      DRAW_PT( index,pos-11,i, COL_RED );
      DRAW_PT( index,pos-10,i, COL_RED );
      DRAW_PT( index,pos+10,i, COL_RED );
      DRAW_PT( index,pos+11,i, COL_RED );
    }
  }
  else // GOOD
  {
    for ( i=stymin+20 ; i<=stymax-20 ; i++ )
    {
      DRAW_PT( index,pos-11,i, COL_GREEN );
      DRAW_PT( index,pos-10,i, COL_GREEN );
      DRAW_PT( index,pos+10,i, COL_GREEN );
      DRAW_PT( index,pos+11,i, COL_GREEN );
    }
  }

  return 0;
}


int CPFSidewall_inspection::Analyze_contour_stopper( void )
{
  int i;
  int imin;
  int imax;
  UINT col_markers = 0xFF00FFFF;
  UINT col_cross   = 0xFF00FFFF; // on fulldsp image

  //ptmid = ptn / 2; // start from middle

  //i    =  ptmid;
  imin =      10;
  imax = ptn2-10;
  i    =    imin;

  mdlresptr = &desc_stppr;

/*  // Analisi punti doppi
  for ( i=0 ; i<ptn2 ; i++ )
  {
    for ( int j=0 ; j<ptn2 ; j++ )
    {
      if ( i!=j && ptx2[i]==ptx2[j] && pty2[i]==pty2[j] )
      {
        TRACE("PUNTI DOPPI TROVATI %d-%d\n", i, j );
        TRACE("(%d,%d)-(%d,%d)\n", ptx2[i], pty2[i], ptx2[j], pty2[j] );
      }
    }
  }    */

/*
extern int ATNLUT[DIMATN][DIMATN];  // array[dx][dy]#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DX ptx[i+AMPATN]-ptx[i-AMPATN]
#define DY pty[i+AMPATN]-pty[i-AMPATN]
#define ATAN(i) ATNLUT[DX+AMPATN*2][DY+AMPATN*2]
*/

  #define DX2 ptx2[i+AMPATN]-ptx2[i-AMPATN]
  #define DY2 pty2[i+AMPATN]-pty2[i-AMPATN]
  #define ATAN2(i) ATNLUT[DX2+AMPATN*2][DY2+AMPATN*2]





  /*
  // ADD AMPATN TO THE END
  for ( i=0 ; i<2*AMPATN ; i++ )
  {
    ptx2[ptn2+i]=ptx2[i];
    pty2[ptn2+i]=pty2[i];
  }

  for ( i=AMPATN ; i<ptn2+AMPATN ; i++ )
  {
    TRACE("pt2[%3d]=(%d,%d) - ATN=%5d\n", i, ptx2[i], pty2[i], ATAN2(i) );
  }
  */

  // FIND BOUNDING BOX
  stxmin = xmax2;
  stxmax = xmin2;
  stymin = ymax2;
  stymax = ymin2;
  for ( i=0 ; i<ptn2 ; i++ )
  {
    //TRACE("pt2[%3d]=(%d,%d)\n", i, ptx2[i], pty2[i] );
    if      ( ptx2[i]>stxmax ) stxmax = ptx2[i];
    else if ( ptx2[i]<stxmin ) stxmin = ptx2[i];

    if      ( pty2[i]>stymax ) stymax = pty2[i];
    else if ( pty2[i]<stymin ) stymin = pty2[i];
  }

  // Something wrong with contour 
  if ( stxmin<=xmin2 || stxmax>=xmax2 || stymin<=ymin2 || stymax>=ymax2 )
  {
    txt2.Format( string_5234[lng] );
    return 99;
  }


  if ( minnptlow ) // stopper orientation analysis
  {
    int nptlow = 0;

    if ( mode & PFS_NEEDLESHIELD ) // Cosmetic2 cameras - needle to left / xmin
    {
      for ( i=0 ; i<ptn2 ; i++ )
      {
        if ( ptx2[i]>stxmax-5 ) nptlow++; // diam pfs = 88 pix
      }

      if ( nptlow>=minnptlow ) // Ok, orientation correct
      {
        //TRACE("STOPPER ORIENTATION CORRECT %d/%d\n",  nptlow, minnptlow );
        if ( draw_ovl ) Draw_stopper_updown( stxmax, 0 );
      }
      else // REJECT, upside down
      {
        if ( draw_ovl ) Draw_stopper_updown( stxmax, 1 );
        desc_stppr.Format( string_5250[lng], nptlow, minnptlow );
        return 5000; // STOPPER INVERTED
      }
    }
    else // Cosmetic3 - needle to right / xmax
    {
      for ( i=0 ; i<ptn2 ; i++ )
      {
        if ( ptx2[i]<stxmin+10 ) nptlow++; // diam pfs = 166 pix
      }

      if ( nptlow>=minnptlow ) // Ok, orientation correct
      {
        //TRACE("STOPPER ORIENTATION CORRECT %d/%d\n",  nptlow, minnptlow );
        if ( draw_ovl ) Draw_stopper_updown( stxmin, 0 );
      }
      else // REJECT, upside down
      {
        if ( draw_ovl ) Draw_stopper_updown( stxmin, 1 );
        desc_stppr.Format( string_5250[lng], nptlow, minnptlow );
        return 5000; // STOPPER INVERTED
      }


    }

  } // if ( minnptlow ) // stopper orientation analysis


    /*
    if ( nptlow>=minnptlow ) // Ok, orientation correct
    {
      //TRACE("STOPPER ORIENTATION CORRECT %d/%d\n",  nptlow, minnptlow );
      for ( i=stymin+20 ; i<=stymax-20 ; i++ )
      {
        DRAW_PT( index,stxmin-11,i, COL_GREEN );
        DRAW_PT( index,stxmin-10,i, COL_GREEN );
        DRAW_PT( index,stxmin+10,i, COL_GREEN );
        DRAW_PT( index,stxmin+11,i, COL_GREEN );
      }
    }
    else // ERROR, orientation incorrect
    {
      //TRACE("STOPPER UPSIDE-DONW %d/%d\n",  nptlow, minnptlow );
      for ( i=stymin+40 ; i<=stymax-40 ; i++ )
      {
        DRAW_PT( index,stxmin-11,i, COL_RED );
        DRAW_PT( index,stxmin-10,i, COL_RED );
        DRAW_PT( index,stxmin+10,i, COL_RED );
        DRAW_PT( index,stxmin+11,i, COL_RED );
      }

//      desc_stppr.Format(L"%s %d/%d", string_5250[lng], nptlow, minnptlow );
      desc_stppr.Format( string_5250[lng], nptlow, minnptlow );

      return 5000; // STOPPER INVERTED
      
    }
    */



  //TRACE("STOPPER BB (%d,%d)-(%d,%d) W=%d H=%d\n",stxmin,stymin,stxmax,stymax,stymax-stymin,stxmax-stxmin);

  int val;
  int err = 0;

  // Stopper width
  //if ( err==0 )
  {
    val = int( 0.5 + stymax - stymin );
    err += Measure_check( val, &stppr_width );
    //TRACE(L"STPR_width=%d - result: %s\n", val, meas_txt );
  }

  // Stopper height
  //if ( err==0 )
  {
    val = int( 0.5 + stxmax - stxmin );
    err += Measure_check( val, &stppr_height );
    //TRACE(L"STPR_height=%d - result: %s\n", val, meas_txt );
  }

  // Stopper vertical position
  //if ( err==0 )
  {
    val = int( 0.5 + dimx - stxmax );
    err += Measure_check( val, &stppr_vpos );
    //TRACE(L"STPR_vpos=%d - result: %s\n", val, meas_txt );
  }

  if ( saving_data ) Save_text( L"" );

  // IF learning just return
  if ( learning_needle )
  {
    desc_stppr.Format(L"LEARNING %d,%d,%d", stppr_width.nsmpl, stppr_height.nsmpl, stppr_vpos.nsmpl );
    //col2 = TXT_DARKORANGE;
    return 99; // reject during learning
  }

  //int ovsc = 5;

  /*
  if ( err==0 ) // All GOOD
  {
    //txt2.Format(L"Stopper GOOD");
    //col2 = TXT_DARKGREEN;
    if ( draw_ovl ) Draw_boundingbox( stxmin-ovsc, stymin-ovsc, stxmax+ovsc, stymax+ovsc, COL_GREEN );
    return 0; // GOOD
  }

  // Some error on the way
  //txt2.Format(L"Stopper REJECT: %s", meas_txt);
  //col2 = TXT_DARKRED;
  if ( draw_ovl ) Draw_boundingbox( stxmin-ovsc, stymin-ovsc, stxmax+ovsc, stymax+ovsc, COL_RED );
  
    return 999; // SOME REJECT */

  return err;




//  txt2.Format( L"Bounding box (%d,%d)-(%d,%d) %dx%d", stxmin, stymin, stxmax, stymax, stxmax-stxmin+1, stymax-stymin+1 );


  return( 0 ); // everything ok

  #undef DX2 
  #undef DY2 
  #undef ATAN2 
 








/*  while ( i<imax )
  {
    TRACE( "pt[%3d] = %3d,%3d - atan=%5d\n", i, ptx[i], pty[i], ATAN(i) );
    i++;
  }  */



  /*

  // left/lower side
  while ( ( ATAN(i)>13500 && i>imin ) ) i--;
  if ( i<=imin ) return 1010; // unable to find point PAL - return error
  kpts[PA][LEFT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ( ATAN(i)>4500 && i>imin ) ) i--;
  if ( i<=imin ) return 1011; // unable to find point PBL - return error
  kpts[PB][LEFT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  i=ptmid; 
  // right/upper side
  while ( ( ATAN(i)<22500 && i<imax ) ) i++;
  if ( i>=imax ) return 1012; // unable to find point PAR - return error
  kpts[PA][RIGHT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ( ATAN(i)<31500 && i<imax ) ) i++;
  if ( i<=imin ) return 1011; // unable to find point PBL - return error
  kpts[PB][RIGHT] = i;
  if ( draw_ovl ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;


  int val;

  int err = 0;

  // ALR_width
  val = Calc_distance( kpts[PA][LEFT], kpts[PA][RIGHT] );
  err += Measure_check( val, &ALR_width );
  TRACE(L"ALR_width=%d - result: %s\n", val, meas_txt );

  // ALR_height
  
  if ( err==0 )
  {
    val = int( 0.5 + 0.5*(2*dimx-ptx[kpts[PA][LEFT]]-ptx[kpts[PA][RIGHT]]));    
    err += Measure_check( val, &ALR_height );
    TRACE(L"ALR_height=%d - result: %s\n", val, meas_txt );
  }

  // BLR_width
  if ( err==0 )
  {
    val = Calc_distance( kpts[PB][LEFT], kpts[PB][RIGHT] );
    err += Measure_check( val, &BLR_width );
    TRACE(L"BLR_width=%d - result: %s\n", val, meas_txt );
  }

  // BLR_height
  if ( err==0 )
  {
    val = int( 0.5 + 0.5*(2*dimx-ptx[kpts[PB][LEFT]]-ptx[kpts[PB][RIGHT]]));
    err += Measure_check( val, &BLR_height );
    TRACE(L"BLR_height=%d - result: %s\n", val, meas_txt );
  }

  // IF learning just return
  if ( learning_needle )
  {
    txt1.Format(L"Needle shield LEARNING %d,%d,%d,%d", ALR_width.nsmpl, ALR_height.nsmpl, BLR_width.nsmpl, BLR_height.nsmpl );
    col1 = col1 = TXT_DARKORANGE;
    return 99; // reject during learning
  }

  if ( err==0 ) // All GOOD
  {
    txt1.Format(L"Needle shield GOOD");
    col1 = col1 = TXT_DARKGREEN;
    return 0; // reject during learning
  }

  txt1.Format(L"Needle shield REJECT: %s", meas_txt);
  col1 = col1 = TXT_DARKRED;

  return 999; // SOME REJECT
  */



  /*
  //  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  while ( ( ATAN(i)>31500 || ATAN(i)<22500 ) && i>imin ) i--;
  if ( i<=imin ) 
  {
    return 1009; // unable to find point - return error
  }
  kpts[PA][LEFT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    return 1019; // unable to find point - return error
  }
  kpts[PB][LEFT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    return 1029; // unable to find point - return error
  }
  kpts[PC][LEFT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)>22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    return 1039; // unable to find point - return error
  }
  kpts[PD][LEFT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i-=10;

  while ( ATAN(i)<22500 && i>imin ) i--;
  if ( i<=imin ) 
  {
    return 1049; // unable to find point - return error
  }
  kpts[PE][LEFT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

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


  /*

//////////////////////////////////////////////////////

  // SEARCH LOWER POINTS - CW - RIGHT SIDE
  i    =  ptmid;
  imin =     10;
  imax = ptn-20;

  while ( ( ATAN(i)<4500 || ATAN(i)>9000 ) && i<imax ) i++;
  if ( i>=imax ) 
  {
    return 1109; // unable to find point - return error
  }
  kpts[PA][RIGHT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    return 1119; // unable to find point - return error
  }
  kpts[PB][RIGHT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    return 1129; // unable to find point - return error
  }
  kpts[PC][RIGHT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)<13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    return 1139; // unable to find point - return error
  }
  kpts[PD][RIGHT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);
  i+=10;

  while ( ATAN(i)>13500 && i<imax ) i++;
  if ( i>=imax ) 
  {
    return 1149; // unable to find point - return error
  }
  kpts[PE][RIGHT] = i;
  if ( 1 ) DRAW_SQUARE(index,ptx[i],pty[i],col_markers);

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

}





int CPFSidewall_inspection::Find_contour_needlesh( int y0 )
{

  //ASSERT( y0>ymin && y0<ymax );

  int i;
  int x, y;

  int   ptnt;
  int   ptxt[NPTCNT/2];
  int   ptyt[NPTCNT/2];

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );
  //ZeroMemory( &cntrbbox, sizeof(cntrbbox) );
  //ZeroMemory( kpts, sizeof(kpts) );

  // start point
  x = (xmin+xmax)>>1;
  y = ymin;
  ptn = 0;

  // search beginning
  while ( PT(x,y)>contour_thr && y<ymax ) y++;

  if ( y>=ymax ) // beginning NOT found
  {
    txt1.Format( string_5230[lng], ptn, min_npt );

    return 99; // failed to find first point, return error
  }

  /*
//  #ifdef SAVE_FULL_PROC
  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp ) image_dsp[index]->ptr_fulldsp[x+y*dimx] = COL_BLUE;
  #endif
  */
  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

  ptnt    = 0;
  ptxt[0] = x;
  ptyt[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

  #define PTDT(i) PT(ptxt[ptnt]+ofsx[(i)&7],ptyt[ptnt]+ofsy[(i)&7])
  #define PTE(i)  PT(ptx [ptn] +ofsx[(i)&7],pty [ptn ]+ofsy[(i)&7])

//  #define PTE(i) PT(ptx[ptn]+ofsx[(i)&7],pty[ptn]+ofsy[(i)&7])

  int ylim = ymin;
  
  i = 6; // start moving leftward - CCW search
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
    if ( ptyt[ptnt]>ylim ) ylim=ptyt[ptnt];
    if ( ylim-ptyt[ptnt] > 35 ) break;
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

  ylim=ymax;

  i = 2; // start moving rightward - CW search
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
    if ( pty[ptn]<ylim ) ylim=pty[ptn];
    if ( pty[ptn]-ylim > 35 ) break;
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

  if ( ptn<min_npt ) // Too few points
  {
    //txt1.Format( string_760[lng], ptn, min_npt );
    //meas_txt.Format( string_760[lng], ptn, min_npt );
    //return 109;
    //txt2.Format(L"Stopper REJECT: %s", meas_txt);
    //col2 = TXT_DARKRED;
    desc_needle.Format( string_5164[lng], ptn, min_npt, max_npt );
    return 108;
  } 

  if ( ptn>max_npt ) // Too many points
  {
    //txt1.Format( string_761[lng], ptn, max_npt );
    desc_needle.Format( string_5162[lng], ptn, min_npt, max_npt );
    return 109;
  } 



//  for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  ); // first pass to blank wider trace

  // draw profile found
  if ( draw_ovl ) for ( i=0 ; i<ptn ; i++ ) DRAW_PT(index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour


  //txt1.Format( L"Profile found %d pt", ptn );
  return 0;
  
//  for ( i=0 ; i<ptn ; i++ )
//  {
/* ***    if ( ptx[i]>cntrbbox.right  ) cntrbbox.right  = ptx[i];
    if ( ptx[i]<cntrbbox.left   ) cntrbbox.left   = ptx[i];
    if ( pty[i]>cntrbbox.bottom ) cntrbbox.bottom = pty[i];
    if ( pty[i]<cntrbbox.top    ) cntrbbox.top    = pty[i]; */

//    if ( 1 ) DRAW_PT (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
//  }

  //TRACE("Bounding box: (%d,%d)-(%d,%d)\n", cntrbbox.left, cntrbbox.top, cntrbbox.right, cntrbbox.bottom );

//  #ifdef SAVE_FULL_PROC
/*  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp )
    {
      for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    }
  #endif */

}







int CPFSidewall_inspection::Find_contour_stopper( int y0 )
{

  //ASSERT( y0>ymin && y0<ymax );

  int i;
  int x, y;

//  int   ptnt;
//  int   ptxt[NPTCNT/2];
//  int   ptyt[NPTCNT/2];

  //TRACE("%d,%d,%d,%d\n", xmin, xmax, ymin, ymax );
  //ZeroMemory( &cntrbbox, sizeof(cntrbbox) );
  //ZeroMemory( kpts, sizeof(kpts) );

  // start point
  x = xmin2;
  y = (ymin2+ymax2)>>1;
  ptn2 = 0;

  // search beginning
  while ( PT(x,y)>contour2_thr && x<xmax2 ) x++;

  if ( x>=xmax2 ) // beginning NOT found
  {
    txt2.Format( string_5230[lng], ptn2, min2_npt );
    return 99; // failed to find first point, return error
  }

  //TRACE("starting point point found: %d,%d\n", x, y );

  //               0    1    2    3    4    5    6    7     //   7  0  1
  int ofsx[8] = {  0 ,  1 ,  1 ,  1 ,  0 , -1 , -1 , -1 };  //   6  #  2
  int ofsy[8] = { -1 , -1 ,  0 ,  1 ,  1 ,  1 ,  0 , -1 };  //   5  4  3

//  ptnt    = 0;
//  ptxt[0] = x;
//  ptyt[0] = y;
  ptn2    = 0;
  ptx2[0] = x;
  pty2[0] = y;
  //TRACE("%d,%d,%d,%d\n", ptn, ptx[ptn],pty[ptn],PT(x,y));

/*  #undef  PTDT
  #undef  PTE
  #define PTDT(i) PT(ptxt[ptnt]+ofsx[(i)&7],ptyt[ptnt]+ofsy[(i)&7])
  #define PTE(i)  PT(ptx [ptn] +ofsx[(i)&7],pty [ptn ]+ofsy[(i)&7])*/

  #undef  PTDT
  #define PTDT(i) PT(ptx2[ptn2]+ofsx[(i)&7],pty2[ptn2]+ofsy[(i)&7])


  i = 4; // start moving downward - CCW search
  while ( ptn2<NPTCNT/2-1 && ptx2[ptn2]>xmin2 && ptx2[ptn2]<xmax2 && pty2[ptn2]>ymin2 && pty2[ptn2]<ymax2 )
  {
    int i1 = i-3+8;
    int i2 = i+3+8;

    i = i2;
    while ( i>=i1 && !( PTDT(i+1)>contour2_thr && PTDT(i)<=contour2_thr ) ) i--;

    ASSERT( i%7>=0 && i%7<8 );
    ptx2[ptn2+1] = ptx2[ptn2]+ofsx[i&7];
    pty2[ptn2+1] = pty2[ptn2]+ofsy[i&7];
    ptn2++;

    if ( ptx2[ptn2]==ptx2[0] && pty2[ptn2]==pty2[0] )
    {
      //TRACE("CLOSED\n");
      break;
    }

  }

  #undef  PTDT

//  if ( ptn2>=NPTCNT/2-1 || ptx2[ptn2]<=xmin2 || ptx2[ptn2]>=xmax2 || pty2[ptn2]<=ymin2 || pty2[ptn2]>=ymax2 ) // cannot find closed contour
  if ( ptn2>=NPTCNT/2-1 ) // cannot find closed contour
  {
    desc_stppr.Format( string_5160[lng], ptn2, min2_npt, max2_npt );
    //col2 = TXT_DARKRED;
    return 199; // failed to find contour
  }


  
  if ( ptn2>max2_npt ) // Too many points
  {
//    txt2.Format( string_761[lng], ptn2, max2_npt );
    desc_stppr.Format( string_5162[lng], ptn2, min2_npt, max2_npt );
    //col2 = TXT_DARKRED;
    return 299;
  } 


  if ( ptn2<min2_npt ) // Too few points
  {
    desc_stppr.Format(string_5164[lng], ptn2, min2_npt, max2_npt );
    //col2 = TXT_DARKRED;
//    txt2.Format( string_760[lng], ptn2, min2_npt );
    return 399;
  } 
  

  //txt2.Format( string_759[lng], ptn2 );


  if ( draw_ovl ) for ( i=0 ; i<ptn2 ; i++ ) DRAW_PT(index,ptx2[i],pty2[i],COL_MAGENTA); // second pass for marking contour

  return 0;

  /*
  
  // copy inverted
  for ( i=0 ; i<ptnt ; i++ )
  {
    ptx[i] = ptxt[ptnt-i-1];
    pty[i] = ptyt[ptnt-i-1];
  }
  ptmid = ptnt; // index of middle/center point of contour
  ptn=ptnt-1;
  
  */

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


  i = 2; // start moving rightward - CW search
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




//  for ( i=0 ; i<ptn ; i++ ) DRAW_CROSS(index,ptx[i],pty[i],COL_BLACK  ); // first pass to blank wider trace

  // draw profile found
  if ( draw_ovl ) for ( i=0 ; i<ptn ; i++ ) DRAW_PT(index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour


  txt1.Format( L"Profile found %d pt", ptn );
  
  for ( i=0 ; i<ptn ; i++ )
  {
/* ***    if ( ptx[i]>cntrbbox.right  ) cntrbbox.right  = ptx[i];
    if ( ptx[i]<cntrbbox.left   ) cntrbbox.left   = ptx[i];
    if ( pty[i]>cntrbbox.bottom ) cntrbbox.bottom = pty[i];
    if ( pty[i]<cntrbbox.top    ) cntrbbox.top    = pty[i]; */

//    if ( 1 ) DRAW_PT (index,ptx[i],pty[i],COL_MAGENTA); // second pass for marking contour
  }

  //TRACE("Bounding box: (%d,%d)-(%d,%d)\n", cntrbbox.left, cntrbbox.top, cntrbbox.right, cntrbbox.bottom );

//  #ifdef SAVE_FULL_PROC
/*  #if defined SAVE_FULL_PROC && defined LOCALDEBUG // only in Localdebug!
    if ( image_dsp[index]->ptr_fulldsp )
    {
      for ( i=0 ; i<ptn ; i++ ) image_dsp[index]->ptr_fulldsp[ptx[i]+pty[i]*dimx] = COL_MAGENTA;
    }
  #endif */

  return 0;
}






#undef   PT
#define  PT(x,y)  img_acq[x+(y)*dimx]

#undef   OUTP
#define  OUTP(x,y) output[x+(y)*dimx]

#undef   BLOCCO_BLOB
#define  BLOCCO_BLOB(ox,oy) if ( OUTP(ox,oy) )               {       \
                              tote     += OUTP(ox,oy);               \
                              dafx[end] = ox;                        \
                              dafy[end] = oy;                        \
                              if ( bbox.left  >ox ) bbox.left  =ox;  \
                              if ( bbox.right <ox ) bbox.right =ox;  \
                              if ( bbox.top   >oy ) bbox.top   =oy;  \
                              if ( bbox.bottom<oy ) bbox.bottom=oy;  \
                              OUTP(dafx[end],dafy[end]) = 0;         \
                              end++;                         }





int CPFSidewall_inspection::Sidewall_PFS( BYTE* img_acq )
{
//  #define NPT (SDCK_AMP<<3) // perimeter é distance*8
  int x, y;

  ASSERT( AfxIsValidAddress( img_acq, dimx*dimy ) );

  //#define NSTRIPES 10
  //int width = dimy / NSTRIPES;
  //ASSERT( NSTRIPES * width == dimy );

  // cycle for stripes 

  //int x1 =     0; // bottom of lower inspection area (into  liquid)
  //int x2 =     0; // top    of lower inspection area (into  liquid)
  //int y1 =     0; // start y of this stripe
  //int y2 = width; // end   y of this stripe

 // for ( x1=0 ; x1<dimx*dimy ; x1++ ) output[x1];

//  bott_err = 0;

  ASSERT( AfxIsValidAddress( output, dim ) );
  ZeroMemory( output, dim );

  cosm_err = 0;

#ifdef SIDEWALL_PFS
  for ( y=ymin ; y<=ymax ; y++ )
  {

    // analysis of LOWER part
    for ( x=xmin ; x<=xmax ; x++ )
    {
      //DRAW_PT( index, x, y, COL_YELLOW ); // 0xFFRRGGBB

      if ( min_absl-PT(x,y)>0 )
      {
        OUTP(x,y) = min_absl-PT(x,y);
        DRAW_PT( index, x, y, COL_ORANGE ); // COL_RED ); // 0xFFRRGGBB
      }
    }

  }
#endif


    // - - - - -    BLOB ANALYSIS    - - - - - 


//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

//  if ( x1_sw<2 || x1_sw>=x2_sw || x2_sw>dimx-2 ) return 99; // something wrong

//  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimy-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int i = 0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

 
  /*
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0xFFRRGGBB
  }  
  */

  // clear outer perimeter
  for ( x=0 ; x<dimx ; x++ )
  {
    OUTP(x,0)      = 0;
    OUTP(x,dimy-1) = 0;
  }
  for ( y=0 ; y<dimy ; y++ )
  {
    OUTP(     0,y) = 0;
    OUTP(dimx-1,y) = 0;
  }
  
  RECT bbox; // bounding box
  int  tote; // total error of each blob
  #ifdef SIDEWALL_PFS
  for ( y=1 ; y<dimy-1 ; y++ )
  {
    for ( x=1 ; x<dimx-1 ; x++ )
    {

      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( OUTP(x,y) )
      {
        tote      = OUTP(x,y);
        num_blob++;   // new blob found     
        bbox.left = bbox.right  = x;
        bbox.top  = bbox.bottom = y;
        dafx[0]   = x;
        dafy[0]   = y;
        end       = 1;
        OUTP(x,y) = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
	    	  BLOCCO_BLOB( dafx[start] - 1 , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start] - 1 );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start] + 1 , dafy[start] + 1 );

          start++; // Ho controllato questo punto, percio' lo tolgo dalla lista

          ASSERT ( (   end >= 0 ) && (   end <= MAX_DIM + 2 ) );
          ASSERT ( ( start >= 0 ) && ( start <= MAX_DIM + 2 ) );

        } // Fine di questo blob


        int width  = bbox.right  - bbox.left + 1; // width
        int height = bbox.bottom - bbox.top  + 1; // height

        ASSERT( width  > 0 && width  < dimx );
        ASSERT( height > 0 && height < dimy );

        int elong = 100;

       // if ( width>height ) elong = width/height;
        if ( width>2 && height>2 ) elong = 100*height/width;
     

        // end    = dimensione in pixel
        // tote   = errore totale accumulato (somma valori pixel)
        // width  = larghezza complessiva
        // height = altezza complessiva
        // elong  = 100 se width O height sono minori di 2, altrimenti è il rapporto moltitplicato 100 E=10 significa largo e stretto, E=1000 significa stretto e alto




		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/


//        if ( end > 100 && elong > 3 )
        if ( end>=min_dim && elong>=min_elong && width>=min_height && height>=min_width && tote>=end*min_avgdf )
        {
          // draw bbox
          for ( x=bbox.left ; x<=bbox.right ; x+=2 )
          {
            DRAW_PT( index, x, bbox.top,    COL_RED );
            DRAW_PT( index, x, bbox.bottom, COL_RED );
          }

          for ( y=bbox.top ; y<=bbox.bottom ; y+=2 )
          {
            DRAW_PT( index, bbox.left,  y, COL_RED );
            DRAW_PT( index, bbox.right, y, COL_RED );
          }

          cosm_err += end;

          //TRACE("Fine blob: bbox %dx%d (%d,%d)-(%d,%d) size=%d tote=%d media=%d\n", bbox.right-bbox.left+1, bbox.bottom-bbox.top+1, bbox.left, bbox.top, bbox.right, bbox.bottom, end, tote, tote/end );
        }

        start = 0;
        end   = 0;

      } // if TEST

    }  // ciclo for x
  }  // ciclo for y
   #endif
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

//  txt1.Format(L"Cosmetic error %d (%d blobs) - %d us", cosm_err, num_blob, timer.elapsed_us() );
  
  return cosm_err;

  /*
  good = TRUE;

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    col1 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }
  */

/*  if ( bott_err > bott_maxerr )
  {
    txt2.Format( string_2380[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKRED;
    good = FALSE;
    //return 99;  // result of inspection REJECT
  }
  else
  {
    txt2.Format( string_2382[lng], bott_err, bott_maxerr );
    col2 = TXT_DARKGREEN;
    //return 0;  // result of inspection GOOD
  }*/


  //if ( good ) return 0;

//  return 99; // reject


/*

  cosm_err = 0;

  if ( cosm_err > max_err )
  {
    //txt1.Format( string_2302[lng], cosm_err, max_err );
    txt1.Format( string_2302[lng], txt_desc, cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    //txt1.Format( string_2304[lng], cosm_err, max_err );
    txt1.Format( string_2304[lng], txt_desc, cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }

  */
}


////Pour added on 20201017
//int CPFSidewall_inspection::detectCrack(const Mat &srcImage, Point pt1, Point pt2)
//{
//	Mat src1 = srcImage.clone();//stretch(srcImage,10);
//	Mat src2; 
//	//cvtColor(srcImage, colorSrc, CV_GRAY2BGR);
//    // find the crack area
//	int leftx = (pt2.x <= pt1.x ? pt1.x : pt2.x)- para_crack.insp_offset - para_crack.insp_height;//85;
//	int topy = pt2.y;
//	int rightx = (pt2.x <= pt1.x ? pt1.x : pt2.x) - para_crack.insp_offset;//15;
//	int bottomy = pt1.y; 
//	Rect roi = Rect(Point(leftx, topy), Point(rightx, bottomy));
//
//	    // Last
//
//    // draw inspection area first
//	for ( int x=leftx ; x<=rightx ; x+=4 )
//    {
//      DRAW_CROSS( index, x, topy,    COL_MAGENTA ); // 0xFFRRGGBB
//      DRAW_CROSS( index, x, bottomy, COL_MAGENTA ); // 0xFFRRGGBB
//    }
//    for ( int y=bottomy ; y<=topy ; y+=4 )
//    {
//      DRAW_CROSS( index, leftx, y,  COL_MAGENTA ); // 0xFFRRGGBB
//      DRAW_CROSS( index, rightx, y, COL_MAGENTA ); // 0xFFRRGGBB
//    }
//
//	return 0;
//}
////Pour added on 20201017

#undef   PT
#undef   OUTP
#undef   BLOCCO_BLOB
