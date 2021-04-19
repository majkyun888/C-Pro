#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Cosmetic_inspection.h"
#include "ChildView.h"



extern Camera_struct    camera  [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;
extern circle_type      circle[MAXR_CIRCLES];

extern BOOL             datarec_on;


extern Results_manager* result;
extern Results_manager* result2;




/*
                                         // SIZE - SCORE
blob_score_type blob_score[NCRIT_BLOB]  = { {   1 ,   1 } ,
                                            {   6 ,   6 } ,  // 6,4
                                            {  10 ,  10 } ,
                                            {  30 ,  40 } }; // 30,50
*/

#define  xmin     area->buffrect.left
#define  xmax     area->buffrect.right
#define  ymin     area->buffrect.top
#define  ymax     area->buffrect.bottom



CCosmetic_inspection::CCosmetic_inspection( UINT ind, CFile_mngmt* file_manager_in/*=NULL*/, int type_in/*=CSMTYPE_SIDEWALL*/ )
{
  ASSERT( ind>=0 && ind<num_tlc );

//  ASSERT( type_in==CSMTYPE_SIDEWALL || type_in==CSMTYPE_SIDECAKE || type_in==CSMTYPE_TOPCAKE );

  file_manager = file_manager_in;

  index = ind;
  type  = type_in;

  nhead = 0;

  txt_desc = string_838[lng];

  area_color = COL_BLUE; // 0x00RRGGBB assigned color to draw inspection area
  
  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  output = (BYTE*) malloc( dimx * dimy );
  ASSERT( output && AfxIsValidAddress(output, dimx*dimy) );

	ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  if ( type & CSMTYPE_SIDECAKE && circle[1].n==0 ) precalc_circles(); // precalculate all circles required for processing


  area = new CArea_adjust( index, file_manager );


/*  area->buffrect.top    =   dimy/3;
  area->buffrect.bottom = 2*dimy/3;
  area->buffrect.left   =   dimx/3;
  area->buffrect.right  = 2*dimx/3; */

  area->buffrect.top    =    5;
  area->buffrect.bottom =  994;
  area->buffrect.left   =  620;
  area->buffrect.right  = 1200;

  blob_score[0].size    =    5;  // GREEN   -   0 points
  blob_score[0].score   =    1;
  blob_score[1].size    =   20;  // YELLOW  -   1 point
  blob_score[1].score   =    5;
  blob_score[2].size    =   80;  // ORANGE  -   5 points
  blob_score[2].score   =   50;
  blob_score[3].size    =  300;  // RED     -  50 points
  blob_score[3].score   =  300;
                                 // MAGENTA - 300 points


  min_dif   =   25;
  max_dif   =   40;
  min_abs   =    0;
  max_abs   =  255;

  radavg    =      6;
  radius    = radavg;
  
  max_err   =  250;
  cosm_err  =    0;
  num_blob  =    0;

  level_th  =   30;
  level_max =  200;
  level_min =  150;

  avglum = (UINT*) malloc( dimx*sizeof(UINT) );
  ASSERT( avglum );
  ZeroMemory( avglum, dimx*sizeof(UINT) );

  extimg   = 0;    // buffer to extended image for processing
  extdim   = 0;    // total dimension of extended image
  extdimx  = 0;    // horizontal dimension of extended image
  extdimy  = 0;    // vertical dimension of extended image
  Buffer_check();

  circ_ofs =   NULL;
  circ_npt =      0;
  circ_rad = radavg;
  Circle_check();

  minbrgt_sw = 140;
  x2ofs_sw   = -20;
  x1ofs_sw   = 600;

  if ( type & CSMTYPE_SIDECAKE )
  {
    minbrgt_sw = 140;
    x1ofs_sw   =   0;
    x2ofs_sw   = 200;
  }


/*  if ( type & CSMTYPE_SIDECAKE ) // allocate for sidecake
  {
    circ_npt = 2*circle[SDCK_AMP].n;
    circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

    int i;
    int j=0;
    for ( i=0 ; i<circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  circle[SDCK_AMP].x[i]+circle[SDCK_AMP].y[i]*dimx,  circle[SDCK_AMP].x[i],  circle[SDCK_AMP].y[i] );
      circ_ofs[j++] =  circle[SDCK_AMP].x[i] + circle[SDCK_AMP].y[i]*dimx;
    }
    for ( i=0 ; i<circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -circle[SDCK_AMP].x[i]-circle[SDCK_AMP].y[i]*dimx, -circle[SDCK_AMP].x[i], -circle[SDCK_AMP].y[i] );
      circ_ofs[j++] = -circle[SDCK_AMP].x[i] - circle[SDCK_AMP].y[i]*dimx;
    }

    //TRACE("circ_ofs prepared - %d points\n", circ_npt );
    //for ( i=0 ; i<circ_npt ; i++ ) TRACE( "ofs[%2d] = %4d (%3d,%3d)\n", i, circ_ofs[i], circ_ofs[i]%dimx, circ_ofs[i]/dimx );
    //TRACE("FINISH\n");
  } */


}


CCosmetic_inspection::~CCosmetic_inspection(void)
{

//  if ( type & CSMTYPE_SIDECAKE && circle[1].n ) precalc_circles(); // precalculate all circles required for processing


  if ( extimg ) 
  {
    free( extimg );
    extimg = NULL;
  }

  if ( circ_ofs )
  {
    delete circ_ofs;
    circ_ofs = NULL;
  }

  if ( area )
  {
    delete area;
    area = NULL;
  }

  if ( avglum )
  {
    free( avglum );
    avglum = NULL;
  }

  if ( output )
  {
    free( output );
    output = NULL;
  }

}


int CCosmetic_inspection::Set_inspection_area( int ind )
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

  TRACE("CCosmetic_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CCosmetic_inspection::Add_menu_parameters( CContainer_wnd* child, wchar_t** name/*=NULL*/ )
{
/*  #ifdef TIVS40_LQ
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  #endif

  #ifdef TIVS40_FD
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  #endif  */

  if ( name && AfxIsValidString( name[lng] ) )
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
    file_manager->Add_separator( name[lng], area );
  }
  else
  {
    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2074[lng], child );
    file_manager->Add_separator( string_2074[lng], area );
  }

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  if ( type & CSMTYPE_SIDECAKE )
  {
    pView->parameter[pView->num_params++] = new CParameter( &radavg,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],    6,    1,    12, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &max_dif,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2762[lng], string_log [lng],   40,    0,   255, file_manager );
  }

  pView->parameter[pView->num_params++] = new CParameter( &min_dif,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2764[lng], string_log [lng],   20,    0,   255, file_manager );
  
  if ( type & CSMTYPE_SIDECAKE )
  {
    pView->parameter[pView->num_params++] = new CParameter( &max_abs,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2776[lng], string_log [lng],    0,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &min_abs,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2778[lng], string_log [lng],  255,    0,   255, file_manager );
  }

  pView->parameter[pView->num_params++] = new CParameter( &max_err,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2080[lng], string_unit[lng], 1000,    0, 99000, file_manager );

  if ( type & CSMTYPE_CAKEVOLUME )
  {
    pView->parameter[pView->num_params++] = new CParameter( &level_th,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_930[lng],  string_log[lng],  30,    0,   255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &level_max,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_932[lng],  string_pix[lng], 200,    0,  1000, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &level_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_934[lng],  string_pix[lng], 150,    0,  1000, file_manager );
  }
  else 
  {
    level_min  = 0;
    level_max  = 0;
  }

  #ifdef NEWCAP_V21
  if ( type & CSMTYPE_SIDEWALL )
  {
    pView->parameter[pView->num_params++] = new CParameter( &minbrgt_sw, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2350[lng],  string_log[lng],   140,    0,  255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2352[lng],  string_pix[lng],     0, -200,  200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2354[lng],  string_pix[lng],   600,  100, 2000, file_manager );
  }
  if ( type & CSMTYPE_SIDECAKE )
  {
    pView->parameter[pView->num_params++] = new CParameter( &minbrgt_sw, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2370[lng],  string_log[lng],   140,    0,  255, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x1ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2372[lng],  string_pix[lng],     0, -200,  600, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &x2ofs_sw,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2354[lng],  string_pix[lng],   200,   50, 2000, file_manager ); // previously minimum was set to 100, reduced to 80 on Victor's request
  }
  #endif

  // Add inspection area
  pView->num_menu_btn++;
	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area );

  return 0; // completed successfully
}





int CCosmetic_inspection::Process_image( BYTE* img_acq, UINT nimg  )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  cosm_err = 0;
  num_blob = 0;

  xmin = area->buffrect.left;
  xmax = area->buffrect.right;
  ymin = area->buffrect.top;
  ymax = area->buffrect.bottom;

//  SCALE_IMG( index, img_acq );

  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;


  // draw inspection area first
 /* int x, y;
  for ( x=area->buffrect.left ; x<=area->buffrect.right ; x+=4 )
  {
    DRAW_PT( index, x, area->buffrect.top,    0x000000FF ); // 0x00RRGGBB
    DRAW_PT( index, x, area->buffrect.bottom, 0x000000FF ); // 0x00RRGGBB
  }
  for ( y=area->buffrect.top ; y<=area->buffrect.bottom ; y+=4 )
  {
    DRAW_PT( index, area->buffrect.left,  y, 0x000000FF ); // 0x00RRGGBB
    DRAW_PT( index, area->buffrect.right, y, 0x000000FF ); // 0x00RRGGBB
  }*/



  // CSMTYPE_SIDECAKE <=> A60 A40 F40(sidecake&topcake) C20(sidecake&topcake)
  // CSMTYPE_SIDEWALL <=> F40(sidewall) C20(sidewall)

  if ( type & CSMTYPE_SIDECAKE ) return( Sidecake_analysis( img_acq ) ); // sidecake & topcake (F40 PC2)
  return( Sidewall_analysis( img_acq ) ); // sidewall (F40 PC1) 
}




#undef   PT
//#define  PT(x,y)  img_acq[x+(y)*dimx]
#define  PT(x,y)  extimg[x+(y)*extdimx]

#undef   OUT
//#define  OUT(x,y) output [x+(y)*dimx]
#define  OUT(x,y) output[x+(y)*extdimx]




int CCosmetic_inspection::Buffer_check( void )
{
//  if ( extdimx == xmax-xmin+1+2*radius ) return 0; // already Ok, just return
  if ( extdimx == x2_sw-x1_sw+1+2*radius ) return 0; // already Ok, just return

//  extdimx = xmax-xmin+1+2*radius; // horizontal size
  extdimx = x2_sw-x1_sw+1+2*radius; // horizontal size
  extdimy = dimy       +2*radius; // vertical size
  extdim  = extdimx * extdimy;    // total size

  if ( extimg ) 
  {
    free( extimg );
    extimg = NULL;
  }
  extimg  = (BYTE*) malloc( extdim );

  if ( output ) 
  {
    free( output );
    output = NULL;
  }
  output  = (BYTE*) malloc( extdim );

  circ_rad = 0; // force recalculate of offsets to keep new extdimx into account!
  TRACE("CCosmetic_inspection::Buffer_check executed - new extimg dimensions %dx%d\n", extdimx, extdimy );

  return 0;
}





/*

    circ_npt = 2*circle[SDCK_AMP].n;
    circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

    int i;
    int j=0;
    for ( i=0 ; i<circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  circle[SDCK_AMP].x[i]+circle[SDCK_AMP].y[i]*dimx,  circle[SDCK_AMP].x[i],  circle[SDCK_AMP].y[i] );
      circ_ofs[j++] =  circle[SDCK_AMP].x[i] + circle[SDCK_AMP].y[i]*dimx;
    }
    for ( i=0 ; i<circle[SDCK_AMP].n ; i++ ) 
    {
      //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -circle[SDCK_AMP].x[i]-circle[SDCK_AMP].y[i]*dimx, -circle[SDCK_AMP].x[i], -circle[SDCK_AMP].y[i] );
      circ_ofs[j++] = -circle[SDCK_AMP].x[i] - circle[SDCK_AMP].y[i]*dimx;
    }

    //TRACE("circ_ofs prepared - %d points\n", circ_npt );
    //for ( i=0 ; i<circ_npt ; i++ ) TRACE( "ofs[%2d] = %4d (%3d,%3d)\n", i, circ_ofs[i], circ_ofs[i]%dimx, circ_ofs[i]/dimx );
    //TRACE("FINISH\n");

    */


int CCosmetic_inspection::Circle_check( void )
{
  if ( radius==circ_rad && circ_ofs && circ_npt ) return 0; // already set, just returns

  if ( circ_ofs )
  {
    delete circ_ofs;
    circ_ofs = NULL;
  }

  circ_npt = 2*circle[radius].n;
  circ_ofs = (int*) malloc( circ_npt * sizeof(int) );

  int i, j=0;
  for ( i=0 ; i<circle[radius].n ; i++ ) 
  {
    //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j,  circle[SDCK_AMP].x[i]+circle[SDCK_AMP].y[i]*dimx,  circle[SDCK_AMP].x[i],  circle[SDCK_AMP].y[i] );
    circ_ofs[j++] =  circle[radius].x[i] + circle[radius].y[i]*extdimx;
  }
  for ( i=0 ; i<circle[radius].n ; i++ ) 
  {
    //TRACE( "ofs[%2d] = %6d (%3d,%3d)\n", j, -circle[SDCK_AMP].x[i]-circle[SDCK_AMP].y[i]*dimx, -circle[SDCK_AMP].x[i], -circle[SDCK_AMP].y[i] );
    circ_ofs[j++] = -circle[radius].x[i] - circle[radius].y[i]*extdimx;
  }

  circ_rad = radius;

  TRACE("CCosmetic_inspection::Circle_check executed - new radius=%d (%d points)\n", circ_rad, circ_npt );

  return 0;
}



int CCosmetic_inspection::Sidecake_analysis( BYTE* img_acq )
{
//  #define NPT (SDCK_AMP<<3) // perimeter é distance*8
  int x, y;
  int i;

  radius = radavg; // set radius (constant during inspection)

//  Buffer_check(); // allocate / reallocate external buffer, if required
//  Circle_check();

  // execute autoalignment, if required
  x1_sw = xmin;
  x2_sw = xmax;

  ASSERT( dimx==2048 ); // if not, change shift below

  #ifdef NEWCAP_V21
  if ( minbrgt_sw>0 )
  {
    x = 0;
    UINT ltot = 0;
//    while ( ltot<minbrgt_sw*dimy && x<dimx ) // ATT.NE CAMBIATO 20151011 per F40 mach 056 
    ltot = minbrgt_sw*dimy*2;
    while ( ltot>minbrgt_sw*dimy && x<dimx )
    {
      ltot = 0;
      for ( y=0 ; y<dimy ; y++ ) ltot+=img_acq[x+(y<<11)]; //PT(x,y);
      //TRACE("x=%3d - ltot=%d\n", x, ltot );
      x++;
    }

    if ( x<dimx ) // bottom found, set inspection coordinates
    {
      //TRACE("index=%d - x=%d\n", index, x );
      x1_sw = x     + x1ofs_sw;
      x2_sw = x1_sw + x2ofs_sw;
    }
    else // autoalignment failed, return
    {
      #ifdef FD_DATAREC
        if ( index<4 ) if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        else           if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 

      //TRACE("index=%d - NOT FOUND\n", index );
      txt1.Format( string_2374[lng] );
      return 69;  // result of inspection REJECT
    }

  }
  #endif

  Buffer_check(); // allocate / reallocate external buffer, if required
  Circle_check();

 
  // copy required part of image to extimg
  ZeroMemory( output, extdimx*extdimy );

//  if ( xmin<      radius ) xmin = radius;
//  if ( xmax>=dimx-radius ) xmax = dimx-radius-1;
  if ( x1_sw<      radius ) x1_sw = radius;
  if ( x2_sw>=dimx-radius ) x1_sw = dimx-radius-1;

  //ASSERT( xmin>=SDCK_AMP && xmax<dimx-SDCK_AMP );

  y = 0;
//  x = xmin-radius; //SDCK_AMP;
  x = x1_sw-radius; //SDCK_AMP;

  // top stripe (minus SDCK_AMP)
  for ( i=dimy-radius ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  } 

  // main, straight block
  for ( i=0 ; i<dimy ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  // bottom stripe (extra SDCK_AMP)
  for ( i=0 ; i<radius ; i++ )
  {
    CopyMemory( extimg+y*extdimx, img_acq+x+i*dimx, extdimx );
    y++;
  }

  //Save_buffer( extimg, extdimx, extdimy, L"extimg" );
  //return 0;

  double coef = 1.0 / circ_npt;

  // START ANALYSIS
  for ( y=radius ; y<radius+dimy ; y++ )
  {
//    for ( x=radius ; x<=radius+xmax-xmin ; x++ )
    for ( x=radius ; x<=radius+x2_sw-x1_sw ; x++ )
    {
      int lum = 0;

      /*for ( i=0 ; i<circ_npt ; i++ )
      {
        //TRACE("(%3d,%3d)[%2d] = %3d\n", x, y, i, extimg[x+y*extdimx+circ_ofs[i]] );
        lum += extimg[x+y*extdimx+circ_ofs[i]];
      }
      lum = ( lum + (circ_npt>>1) ) / circ_npt; */

      BYTE* ptr = extimg+x+y*extdimx;
      for ( i=0 ; i<circ_npt ; i++ ) lum += ptr[circ_ofs[i]];
      lum = int( 0.5 + coef*lum );

      ASSERT( lum>=0 && lum<256 );

      //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
      if ( lum-min_dif-PT(x,y) > 0 ) OUT(x,y) = lum-min_dif-PT(x,y);
 /*     {
        OUT(x,y) = lum-min_dif-PT(x,y);
        //DRAW_PT( index, x, y, COL_RED ); // 0x00RRGGBB
        //TRACE("fine: lum=%d\n", lum );
      } */

      if ( PT(x,y)-lum-max_dif > 0 ) OUT(x,y) = PT(x,y)-lum-max_dif;
/*      {
        OUT(x,y) = PT(x,y)-lum-max_dif;
        //DRAW_PT( index, x, y, COL_RED ); // 0x00RRGGBB
        //TRACE("fine: lum=%d\n", lum );
      } */

      if ( PT(x,y)-max_abs > 0 ) OUT(x,y) = PT(x,y)-max_abs;
      if ( min_abs-PT(x,y) > 0 ) OUT(x,y) = min_abs-PT(x,y);

//      ASSERT( OUT(x,y)==0 );
      //ASSERT( lum == 123*circ_npt );

      /*
      int ofs0 = x + y*dimx;
      for ( i=0 ; i<circ_npt ; i++ ) lum += img_acq[ofs0+circ_ofs[i]];
      lum = ( lum + (circ_npt>>1) ) / circ_npt;

      //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
      if ( lum-min_dif-PT(x,y) > 0 )
      {
        output[x+y*dimx] = lum-min_dif-PT(x,y);
        //DRAW_PT( index, x, y, COL_RED ); // 0x00RRGGBB
        //TRACE("fine: lum=%d\n", lum );
      }
      */

    }
  }



/*  for ( y=0 ; y<dimy ; y++ )
  {
    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0x00RRGGBB

    if ( y>=SDCK_AMP && y<dimy-SDCK_AMP )
    {
      for ( x=xmin+SDCK_AMP ; x<=xmax-SDCK_AMP ; x++ )
      {
        // 
        int lum = 0;
        int ofs0 = x + y*dimx;
        for ( i=0 ; i<circ_npt ; i++ ) lum += img_acq[ofs0+circ_ofs[i]];
        lum = ( lum + (circ_npt>>1) ) / circ_npt;

        //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
        if ( lum-min_dif-PT(x,y) > 0 )
        {
          output[x+y*dimx] = lum-min_dif-PT(x,y);
          //DRAW_PT( index, x, y, COL_RED ); // 0x00RRGGBB
          //TRACE("fine: lum=%d\n", lum );
        }
      }
    }
  }  */

  //Save_buffer( output, extdimx, extdimy, L"output" );

  Calculate_blob_output();

  #ifdef FD_DATAREC
    if ( index<4 )
    {
      if ( datarec_on && result  ) result ->InsertI( cosm_err, nhead ); // insert cosm_err
    }
    else
    {
      if ( datarec_on && result2 ) result2->InsertI( cosm_err, nhead ); // insert cosm_err
    }
  #endif 


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


}


/*
int CCosmetic_inspection::Sidecake_analysis_orig( BYTE* img_acq )
{

  #undef SDCK_AMP
  #define SDCK_AMP       5  // semiamp for inspection
  #define NPT (SDCK_AMP<<3) // perimeter é distance*8

  int x, y;
  int i;

  ZeroMemory( output, dimx*dimy );

  for ( y=0 ; y<dimy ; y++ )
  {
    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0x00RRGGBB


    if ( y>=SDCK_AMP && y<dimy-SDCK_AMP )
    {
      for ( x=xmin+SDCK_AMP ; x<=xmax-SDCK_AMP ; x++ )
      {
        // 
        int lum = 0;
        for ( i=0 ; i<2*SDCK_AMP+1 ; i++ )
        {
          lum += PT(x-SDCK_AMP+i,y-SDCK_AMP); // tratto superiore
          lum += PT(x-SDCK_AMP+i,y+SDCK_AMP); // tratto inferiore
          //TRACE("H(%2d) - (%3d,%3d)-(%3d,%3d)\n", i, x-SDCK_AMP+i, y-SDCK_AMP, x-SDCK_AMP+i, y+SDCK_AMP );
        }

        for ( i=1 ; i<2*SDCK_AMP ; i++ )
        {
          lum += PT(x-SDCK_AMP,y-SDCK_AMP+i); // tratto superiore
          lum += PT(x+SDCK_AMP,y-SDCK_AMP+i); // tratto inferiore
          //TRACE("V(%2d) - (%3d,%3d)-(%3d,%3d)\n", i, x-SDCK_AMP, y-SDCK_AMP+i, x+SDCK_AMP, y-SDCK_AMP+i );
        }


        lum = ( lum + (SDCK_AMP<<2) ) / NPT;

        //TRACE( "%d - %d - %d\n", lum, PT(x,y), lum-min_dif-PT(x,y) );
        if ( lum-min_dif-PT(x,y) > 0 )
        {
          output[x+y*dimx] = lum-min_dif-PT(x,y);
          //DRAW_PT( index, x, y, COL_RED ); // 0x00RRGGBB
          //TRACE("fine: lum=%d\n", lum );
        }


      }
    }


  }  


  Calculate_blob_output();



  if ( cosm_err > max_err )
  {
    txt1.Format( string_2302[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2304[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }

}
*/




#undef BLOCCO_BLOB



#define BLOCCO_BLOB(ox,oy)  if ( OUT(ox,oy) )               { \
                              dafx[end] = ox;                 \
                              dafy[end] = oy;                 \
                              OUT(dafx[end],dafy[end]) = 0;   \
                              end++;                        }






int CCosmetic_inspection::Calculate_blob_output( void )
{
//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimx-2 );
  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimx-2 );

  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

  cosm_err = 0;
 
  for ( y=0 ; y<dimy ; y++ )
  {
    //OUT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    //OUT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    //DRAW_PT( index, xmin, y, area_color ); // 0x00RRGGBB
    //DRAW_PT( index, xmax, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, x1_sw, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0x00RRGGBB
  }  


//  for ( y=1 ; y<dimy-1 ; y++ )
//    for ( x=xmin+1; x<=xmax-1 ; x++ )

  for ( y=radius ; y<radius+dimy ; y++ )
    //for ( x=radius ; x<=radius+xmax-xmin ; x++ )
    for ( x=radius ; x<=radius+x2_sw-x1_sw ; x++ )
    {
      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( OUT(x,y) )
      {
        num_blob++;   // new blob found     
        dafx[0]  = x;
        dafy[0]  = y;
        end      = 1;
        OUT(x,y) = 0; // img[x+y*dimx] = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
           //Controllo se il punto che sto esaminando ha vicini da marcare
          /*BLOCCO_BLOB( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] - 1 );*/

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

		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/

        // IN QUESTO PUNTO end è la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_GREEN );  // GREEN - NO ERROR
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_YELLOW );  // YELLOW - SCORE[0]
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_ORANGE );  // LIGHT ORANGE - SCORE[1]
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_ORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_RED );  // ORANGE - SCORE[2]
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_RED );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                //for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+xmin-radius, dafy[i]-radius,  COL_MAGENTA );  // RED - SCORE[3]
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i]+x1_sw-radius, dafy[i]-radius,  COL_MAGENTA );  // RED - SCORE[3]
              }
            }
          } /***--***/
       
        }  //BLOG BIG ENOUGH
        
        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

      } // if TEST
      start = 0;
      end   = 0;

    }  // ciclo for
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  return cosm_err;
}
 







// STANDARD SIDEWALL ANALYSIS (COMPARED TO HORIZONTAL AVERAGE)

#undef   PT
#define  PT(x,y)  img_acq[x+(y)*dimx]

#undef   OUT
#define  OUT(x,y) output [x+(y)*dimx]




int CCosmetic_inspection::Sidewall_analysis( BYTE* img_acq )
{
//  ASSERT( 0<=data->xmin && data->xmin<data->xmax-10 && data->xmax<data->dimx ); // Check upper/lower limit of inspection area
  //ASSERT( data->dimx && data->dimy );                                           // dimx and dimy have been defined
  //ASSERT( AfxIsValidAddress( img_in, data->dimx*data->dimy ) );                 // Check processing buffer 

/*  if (  <=0)
    data->xmin = 1;
  if(data->xmax >= DIMXLIN_ACQ)
    data->xmax = DIMXLIN_ACQ-2; */

//  BYTE* ptr = img_in;
 // int xmin, xmax;
  //TRACE("%d: difflum = %d\n", data->ind, data->difflum );
  // Image pre-scan to compute average brightness of all columns

  /*
  int x, y;

  UINT* lum_avg = (UINT*) malloc( (xmax-xmin+1)*sizeof(UINT) );
  ASSERT( lum_avg );
  //ZeroMemory( lum_avg, (xmax-xmin+1)*sizeof(int) );

  for ( x=xmin ; x<=xmax ; x++ )
  {
    lum_avg[x-xmin] = 0;
//    for ( y=ymin ; y<=ymax ; y++ ) lum_avg[x-xmin] += PT(x,y);
    for ( y=0 ; y<dimy ; y++ ) lum_avg[x-xmin] += PT(x,y);
    lum_avg[x-xmin] = lum_avg[x-xmin] / (ymax-ymin+1);
    ASSERT( lum_avg[x-xmin]>=0 && lum_avg[x-xmin]<=255 );
    if ( lum_avg[x-xmin]<min_dif ) lum_avg[x-xmin]=min_dif;
  }
  for ( x=xmin+1 ; x<=xmax-1 ; x++ )
  {
    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;
    //for ( y=ymin ; y<=ymax ; y++ )
    for ( y=1 ; y<dimy-1 ; y++ )
    {
      if ( PT(x,y)<lum_avg[x-xmin]-min_dif ) PT(x,y) = lum_avg[x-xmin]-min_dif-PT(x,y);
      else                                   PT(x,y) = 0;
    }
  }
  if ( lum_avg ) free( lum_avg );
  */

  int x, y;
  UINT lum = 0;


  #ifdef NEWCAP_V21 // new version 2.1 with auto-alignment

  // first calculate ALL average values of brightness through dimx
  double coeff = 1.0/dimy;
  for ( x=0 ; x<dimx ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    avglum[x] = UINT( 0.5 + lum*coeff );
    ASSERT( avglum[x]>=0 && avglum[x]<=255 );
  }

  // then calculate top and bottom heights (autoalignment)
  if ( minbrgt_sw>0 )
  {
    x2_sw = dimx-1; // start from rightmost
    while ( avglum[x2_sw]<minbrgt_sw && x2_sw>50 ) x2_sw--;

    if ( x2_sw<=50 ) // upper (right) x coordinate NOT found, fail and return
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 
      txt1.Format( string_2360[lng] );
      return 131;
    }

    // ok, change of brightness found. calculate x1 and x2
    x2_sw = x2_sw + x2ofs_sw; // compute right/top analysis coord
    x1_sw = x2_sw - x1ofs_sw; // compute left/bottom analysis coord

    if ( x1_sw<5 || x2_sw>dimx-5 || x2_sw-x1_sw<20 ) // something wrong with autoaligned sidewall area, error and return
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
        if ( datarec_on && result2 ) result2->InsertI( 99999, nhead ); // insert 99999 unable to find bottom/alignment
      #endif 
      txt1.Format( string_2362[lng] );
      return 132;
    }
  }
  else // autoalignemnt OFF
  {
    x1_sw = xmin+1;
    x2_sw = xmax-1;
  }
  
  // ok, left/bottom and right/top coordinates valid, start analysis
  for ( x=x1_sw ; x<=x2_sw ; x++ )
  {
    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;
    for ( y=1 ; y<dimy-1 ; y++ )
    {
      if ( PT(x,y)<avglum[x]-min_dif ) PT(x,y) = avglum[x]-min_dif-PT(x,y);
      else                             PT(x,y) = 0;
    }
  }

  #else  // older version WITHOUT autoalignment

  for ( x=0 ; x<=xmin ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    avglum[x] = (lum+(dimy>>1)) / dimy;
  }
  
  x1_sw = xmin+1;
  x2_sw = xmax-1;

  for ( x=x1_sw ; x<=x2_sw ; x++ )
  {
    lum = 0;
    for ( y=0 ; y<dimy ; y++ ) lum += PT(x,y);
    lum = (lum+(dimy>>1)) / dimy;
    ASSERT( lum>=0 && lum<=255 );

    avglum[x] = lum;

    PT(x,     0) = 0;
    PT(x,dimy-1) = 0;

    for ( y=1 ; y<dimy-1 ; y++ )
    {
      if ( PT(x,y)<lum-min_dif ) PT(x,y) = lum-min_dif-PT(x,y);
      else                       PT(x,y) = 0;
    }
  }

  #endif


  // SEARCH FOR CAKE VOLUME (IF ENABLED)
  if ( level_min>0 && level_max>level_min )
  {
    level = 0;
    x = xmin; // start from bottom of inspection area
    while ( x>20 && avglum[x]>level_th ) x--;
    if ( x>20 )
    {
      level_high = x;

      x = 10;
      while ( x<level_high-10 && avglum[x]>level_th ) x++;
      if ( x<level_high-10 )
      {
        level_low = x;
        level = level_high - level_low; // calculate filling volume found

        #ifdef FD_DATAREC
          if ( datarec_on && result2 ) result2->InsertI( level, nhead ); // insert level into cake-volume (result2) data
        #endif 

        UINT col = COL_GREEN;
        if ( level < level_min )   // volume TOO LOW
        {
          txt2.Format( string_2312[lng], level, level_min, level_max );
          col = COL_RED;
        }
        else
        {
          if ( level > level_max ) // volume TOO HIGH
          {
            txt2.Format( string_2314[lng], level, level_min, level_max );
            col = COL_RED;
          }
          else                     // Cake volume Ok
          {
            txt2.Format( string_2316[lng], level, level_min, level_max );
            col  = COL_GREEN;
            col2 = TXT_DARKGREEN;
          }
        }

        for ( y=0 ; y<dimy ; y+=8 )
        {
          DRAW_PT( index, level_high, y, col ); // 0x00RRGGBB
          DRAW_PT( index, level_low,  y, col ); // 0x00RRGGBB
        }

        if ( col==COL_RED ) // rejected because TOO LOW or TOO HIGH
        {
          #ifdef FD_DATAREC
            if ( datarec_on && result ) result->InsertI( -1, nhead ); // insert cosm_err into sidewall (result) data
          #endif 
          return 89;
        }
      }
      else // x>=level_high-10 => LOWER cake level NOT FOUND
      {
        #ifdef FD_DATAREC
          if ( datarec_on && result  ) result ->InsertI( -1, nhead ); // insert -1 into sidewall (result) data
          if ( datarec_on && result2 ) result2->InsertI( -1, nhead ); // insert -1 into cake-volume (result2) data
        #endif 
        txt2.Format( string_2320[lng] );
        return 92;
      }
    }
    else // x<20 => UPPER cake level NOT FOUND
    {
      #ifdef FD_DATAREC
        if ( datarec_on && result  ) result ->InsertI( -1, nhead ); // insert -2 into sidewall (result) data
        if ( datarec_on && result2 ) result2->InsertI( -2, nhead ); // insert -2 into cake-volume (result2) data
      #endif 
      txt2.Format( string_2322[lng] );
      return 91;
    }
  }
  else  // cake volume disabled
  {
    #ifdef FD_DATAREC
      if ( datarec_on && result2 ) result2->InsertI( -3, nhead ); // insert -3 into cake-volume (result2) data
    #endif 
    txt2.Format( string_2324[lng] );
    col2 = 0;
  }


  Calculate_blob( img_acq );

  #ifdef FD_DATAREC
    if ( datarec_on && result ) result->InsertI( cosm_err, nhead ); // insert cosm_err into sidewall (result) data
  #endif 

  if ( cosm_err > max_err )
  {
    txt1.Format( string_2330[lng], cosm_err, max_err );
    return 99;  // result of inspection REJECT
  }
  else
  {
    txt1.Format( string_2332[lng], cosm_err, max_err );
    col1 = TXT_DARKGREEN;
    good = TRUE;
    return 0;  // result of inspection GOOD
  }




/*
  int filter[DIMXLIN_ACQ];

  ZeroMemory( filter, sizeof(filter) );
  x = 4;
  filter[x] = lum_avg[x+1]+lum_avg[x+2]+lum_avg[x+3]+lum_avg[x+4] - lum_avg[x-4]-lum_avg[x-3]-lum_avg[x-2]-lum_avg[x-1];

  while ( x < dimx-5 )
  {
    x++;
    filter[x] += lum_avg[x-5]+lum_avg[x-1]-lum_avg[x]-lum_avg[x+4];
    //TRACE( "%d,%d\n", x, filter[x] );
  };
  */

  //  ATTENZIONE: RICERCA AUTOMATICA SOLO SE XMIN O XMAX SONO MESSI A ZERO !

/*  if ( xmin )
  {
    xmin = xmin; // Fisso, impostato da operatore
  }
  else // calcolato automaticamente
  {
    // Search left border (negative value for filter)
    int left_threshold = -30;
    x = 500;
    while ( x > 10 && filter[x]>left_threshold ) x--;
    if ( x > 10 ) xmin = x + 10; // Left border found!
    else          xmin =    170; // Left border NOT found - fix value
  } */


/*  if ( data->xmax )
  {
    xmax = data->xmax; // Fisso, impostato da operatore
  }
  else // calcolato automaticamente
  {
    // Search right border (positive value for filter)
    int right_threshold = 30;
    x = 500;
    while ( x < DIMXLIN_ACQ && filter[x]<right_threshold ) x++;
    if ( x < DIMXLIN_ACQ ) 
      xmax = x - 10; // Right border found!
    else            
      xmax =    860; // Right border NOT found - fix value
  } */


  /*
  if ( tot_lum < 80 ) // Whole line is too dark! Mark error!
  {
     // TO DO !!!
  }

  BYTE* blob_img = (BYTE*) _aligned_malloc( dim , 16 );

  ZeroMemory( blob_img, DIMXLIN_ACQ*DIMYLIN_ACQ );

  // SECOND PASS WITH INSPECTION
  for ( x=xmin ; x<=xmax ; x++ )
  {

    BYTE reference;
    if ( lum_avg[x] > min_dif )   reference = BYTE( lum_avg[x]-min_dif );
    else                          reference = 0;

    ptr = img_in+x;
    BYTE* blob_ptr = blob_img+x;

    for ( y=0 ; y<DIMXLIN_ACQ*DIMYLIN_ACQ ; y+=DIMXLIN_ACQ )
    {
      BYTE pippo = ptr[y];
      if ( reference-ptr[y] > 0  )
      {
        blob_ptr[y] = reference-ptr[y];
      }
    }
  }
    
    */
//  return( TRUE );
}




#undef  BLOCCO_BLOB

#define BLOCCO_BLOB(ox,oy)  if ( PT(ox,oy) )                { \
                              dafx[end] = ox;                 \
                              dafy[end] = oy;                 \
                              PT(dafx[end],dafy[end]) = 0;    \
                              end++;                        }


/*
#define BLOCCO_BLOB( ox , oy )  if ( img[ox+(oy)*dimx] )               { \
                                   dafx[end] = ox;                       \
                                   dafy[end] = oy;                       \
                                   img[dafx[end]+dafy[end]*dimx] = 0;    \
                                   end++;                              }
                                   */
                                   // TRACE("pt[%3d]=%4d,%4d\n",end,dafx[end],dafy[end]);  \


int CCosmetic_inspection::Calculate_blob( BYTE* img_acq )
{
//  ASSERT( 2<=area->buffrect.left && area->buffrect.left<area->buffrect.right  && area->buffrect.right <=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimx-2 );

  ASSERT( 2<=x1_sw && x1_sw<x2_sw && x2_sw<=dimx-2 );
//  ASSERT( 2<=area->buffrect.top  && area->buffrect.top <area->buffrect.bottom && area->buffrect.bottom<=dimx-2 );


  #define MAX_DIM  5000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;
 

/*  for ( x=xmin ; x<=xmax ; x++ )
  {
    img[x+ymin*dimx] = 0;
    img[x+ymax*dimx] = 0;
    DRAW_PT( index, x, ymin, area_color ); // 0x00RRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0x00RRGGBB
  }  
  for ( y=ymin ; y<=ymax ; y++ )
  {
    img[xmin+y*dimx] = 0;
    img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0x00RRGGBB
  }  */


/*  for ( x=xmin ; x<=xmax ; x++ )  // img[x] = 0;
  {
    img[x+       0*dimx] = 0;
    img[x+(dimy-1)*dimx] = 0;
  }   */


  for ( y=0 ; y<dimy ; y++ )
  {
/*    PT(xmin,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(xmax,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, xmin, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0x00RRGGBB*/
    PT(x1_sw,y) = 0;   //img[xmin+y*dimx] = 0;
    PT(x2_sw,y) = 0;   //img[xmax+y*dimx] = 0;
    DRAW_PT( index, x1_sw, y, area_color ); // 0x00RRGGBB
    DRAW_PT( index, x2_sw, y, area_color ); // 0x00RRGGBB
  }  

//  img[xmin+1] = 0;
  

/*
  for ( y = ymin ; y <= ymax ; y++ )
  {
    DRAW_PT( ind, xmin, y,  COLORE_AREEISP );
    //img_in[xmin+y*(image_dsp[0]->dim_dsp.cx)]=0;
    img_in[xmin+y*DX]=0;
    DRAW_PT( ind, xmax, y,  COLORE_AREEISP );
    //img_in[xmax+y*(image_dsp[0]->dim_dsp.cx)]=0;
    }
    else
    {
    DRAW_PTSHOW( ind, xmin, y,  COLORE_AREEISP );
    //img_in[xmin+y*(image_dsp[0]->dim_dsp.cx)]=0;
    img_in[xmin+y*DX]=0;
    DRAW_PTSHOW( ind, xmax, y,  COLORE_AREEISP );
    }
    img_in[xmax+y*DX]=0;
  } 

  for ( x = xmin ; x <= xmax ; x++ )
  {
    // DISEGNA TRATTI VERTICALI AREA DI ANALISI
    if(!pView->windowsopen[ind])
    {
    DRAW_PT( ind, x, ymin,  COLORE_AREEISP );
    img_in[x+ymax*DX]=0;
    DRAW_PT( ind, x, ymax,  COLORE_AREEISP );
    img_in[x+ymin*DX]=0;
    }
    else
    {
    DRAW_PTSHOW( ind, x, ymin,  COLORE_AREEISP );
    img_in[x+ymax*DX]=0;
    DRAW_PTSHOW( ind, x, ymax,  COLORE_AREEISP );
    img_in[x+ymin*DX]=0;
    }
  }
  */

/*  #ifdef NEWCAP_V21
    int xsta = xmin+1;
    int xend = xmax-1;
  #else
    int xsta = xmin+1;
    int xend = xmax-1;
  #endif */


//  for ( y = ymin ; y <= ymax ; y++ )
  for ( y=1 ; y<dimy-1 ; y++ )
      //    for ( x=xmin+1; x<=xmax-1 ; x++ )
    for ( x=x1_sw+1; x<x2_sw ; x++ )
    {

      // Se il pixel è maggiore di 0 ho un nuovo blob
      if ( PT(x,y) ) // img[x+y*dimx] ) // ( TEST(x+y*DX) )  
      {
        num_blob++;   // new blob found     
        dafx[0] = x;
        dafy[0] = y;
        end     = 1;
        PT(x,y) = 0; // img[x+y*dimx] = 0; 
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
           //Controllo se il punto che sto esaminando ha vicini da marcare
          /*BLOCCO_BLOB( dafx[start] + 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start] - 1 , dafy[start]     );
          BLOCCO_BLOB( dafx[start]     , dafy[start] + 1 );
          BLOCCO_BLOB( dafx[start]     , dafy[start] - 1 );*/

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

		/*ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;*/

        // IN QUESTO PUNTO end è la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_GREEN );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            cosm_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_YELLOW );  // YELLOW - SCORE[0]
          }
          else /***--***/
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              cosm_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_LIGHTORANGE );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                cosm_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_ORANGE );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                cosm_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  // RED - SCORE[3]
              }
            }
          } /***--***/
       
        }  //BLOG BIG ENOUGH
        
        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );

      } // if TEST
      start = 0;
      end   = 0;

    }  // ciclo for
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  return cosm_err;
}
 






