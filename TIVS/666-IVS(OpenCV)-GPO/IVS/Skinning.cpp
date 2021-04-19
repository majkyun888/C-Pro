#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
#include "Skinning.h"
#include "ChildView.h"

extern Camera_struct         camera  [NUM_TLC];
extern UINT                  lng;
extern CChildView*           pView;
extern CImage_display*       image_dsp[NUM_TLC];
extern UINT                  num_tlc;
extern CParticle_inspection* particle_insp[NUM_TLC];

//extern BOOL                  stn1_part_back;     // In case of selectable TRUE/FALSE BACK/BOTTOM LIGHT particle station 1 


extern UINT                  thread_status;

#define  xmin    area->buffrect.left
#define  xmax    area->buffrect.right
#define  ymin    area->buffrect.top
#define  ymax    area->buffrect.bottom
#define  PT(x,y) img_acq[x+(y)*camera[index].width]
//#define FLT(x,y) PT(x,y-6)+PT(x,y+6)-3*PT(x,y)
//extern BOOL      learn_reflections;




CSkinning::CSkinning( UINT ind, CFile_mngmt* file_manager_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  area_color = COL_BLUE; // 0xFFRRGGBB assigned color to draw inspection area

	//ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =      20; //  dimy/3;
  area->buffrect.bottom = dimy-20; // 2*dimy/3;
  area->buffrect.left   =  dimx/3; //  dimx/3;
  area->buffrect.right  = dimx-20; // 2*dimx/3;



  num_img       =  10;  // number of images for each inspection cycle

  /////////////////////////////////////////////////////////////////////////////////////////////////////

  align_w    =  400; // width used for alignment
  align_h    =  180; // height used for alignment
  gap_w      =   40;
  num_pt     =   35;
  score_min  =   40; // *num_pt;
 
  // currents for alignment
  align_wact =    0;
  align_hact =    0;
  gap_wact   =    0;

  anlys_w    =   70; //*align_w/100; // width  used for analysis
  anlys_h    =   70; //*align_h/100; // height used for analysis

  // currents for analysis
  anlys_wact =    0;
  anlys_hact =    0;

  imgshw     =    1; 

  centr_x    =    0; // x coord of computed center
  centr_y    =    0; // y coord of computed center

  centr_ofsx =    0; // x coord offset of computed center
  centr_ofsy =   30; // y coord offset of computed center

  lum_min    =   70;
  err_max    = 1000;

  nptskin    =    0;
  ptskin     = NULL;
  semiwdt    =    0; // semiwidth of analysis ellipse
  anlwdt     = NULL; // array to store the width of analysis ellipse accordind to x



//  Prepare_ptskin();



}


CSkinning::~CSkinning(void)
{

  if ( area )
  {
    delete area;
    area = NULL;
  }

  if ( ptskin )
  {
    TRACE("CSkinning[%d]::destroying ptskin %d points\n", index, nptskin );
    free( ptskin );
    ptskin  = NULL;
    nptskin = 0;
  }
 // else TRACE("CSkinning[%d]::destroying ptskin not required\n", index );

  if ( anlwdt )
  {
    TRACE("CSkinning[%d]::destroying anlwdt %d points\n", index, semiwdt );
    free( anlwdt );
    anlwdt  = NULL;
    semiwdt = 0;
  }
 // else TRACE("CSkinning[%d]::destroying anlwdt not required\n", index );

}





int CSkinning::Prepare_analysis( void )
{

  if ( anlwdt )
  {
    TRACE("CSkinning::Prepare_analysis[%d] destroying anlwdt %d points\n", index, semiwdt );
    free( anlwdt );
    anlwdt  = NULL;
    semiwdt = 0;
  }

  //int wtst = 20;
  //int htst = 20;


  //semiwdt = 1 + anlys_h/2;
  semiwdt = int( 0.5 + anlys_h*align_h/200.0 );
  //semiwdt = int( 0.5 + 0.5*htst );

  if ( anlwdt ) free( anlwdt );
  anlwdt = (int*) malloc( sizeof(int) * (semiwdt+1) );
  ASSERT(anlwdt);
  TRACE("CSkinning::Prepare_analysis[%d]::anlwdt created for %d points\n", index, semiwdt );

  int y;
 
//  for ( int x = 0 ; x <=semiwdt  ; x++ )
  for ( int x = 0 ; x <=semiwdt  ; x++ )
  {
    double w = double( 0.01*anlys_w*align_w );
    double h = double( 0.01*anlys_h*align_h );

    //w = wtst;
    //h = htst;

    if ( 2*x < h ) y = int( 0.5 + 0.5*w*sqrt(h*h-4.0*x*x)/h );
    else           y = 0;

    ASSERT( x>=0 && x<=semiwdt );
    ASSERT( y>=0 && y< w       ); //anlys_w );
    anlwdt[x] = y;

    TRACE("x=%3d - %3d\n", x, y );
//      DRAW_PT( index, x_max + x, y_max + y, COL_BLUE ); // 0xFFRRGGBB
//      DRAW_PT( index, x_max + x, y_max - y, COL_BLUE ); // 0xFFRRGGBB
  }
  
  anlys_wact = anlys_w;
  anlys_hact = anlys_h;

  TRACE("CSkinning::Prepare_analysis[%d] completed %d points created\n", index, semiwdt );
  return( 0 );
}



int CSkinning::Prepare_alignment( void )
{

/*  if ( ptskin )
  {
    TRACE("CSkinning::Prepare_ptskin[%d] not required - exit\n", index );
    return( 0 );
  }*/

  double ang_sta =   0.0 * DEG2RAD;
  double ang_end = 180.0 * DEG2RAD;

  double ang;
  double ang_step = (ang_end-ang_sta)/(num_pt-1.0); // 3.0*DEG2RAD;

  //nptskin = int( 0.5 + (ang_end-ang_sta)/ang_step ) + 1;

  if ( ptskin )
  {
    TRACE("Prepare_alignment[%d]::destroying ptskin %d points\n", index, nptskin );
    free( ptskin );
    ptskin  = NULL;
    nptskin = 0;
  }
//  else TRACE("Prepare_alignment[%d]::destroying ptskin not required\n", index );


  nptskin = num_pt;
  ptskin = (skinpt_type*) malloc( sizeof(skinpt_type) * nptskin );

  ASSERT( ptskin );

  int i=0;


  for ( ang=ang_sta ; ang<=ang_end*1.0001 ; ang+=ang_step )
  {
    ASSERT( i>=0 && i<nptskin );

    double x = 0.5 * align_h * sin(ang);
    double y = 0.5 * align_w * cos(ang);

    ptskin[i].x1 = -int( x );
    ptskin[i].y1 =  int( y );

    x = 0.5 * (align_h-gap_w) * sin(ang);
    y = 0.5 * (align_w-gap_w) * cos(ang);

    ptskin[i].x2 = -int( x );
    ptskin[i].y2 =  int( y );

 //   TRACE("%3d, %3d, %3d\n", i, ptskin[i].x, ptskin[i].y );

    i++;
  }

  ASSERT( i==nptskin );

  align_wact = align_w;
  align_hact = align_h;
  gap_wact   = gap_w;


  TRACE("CSkinning::Prepare_alignment[%d] completed %d points created\n", index, nptskin );
  return( 0 );
}


int CSkinning::Set_inspection_area( int ind )
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

  TRACE("CSkinning::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



/*
  align_w    =  400; // width used for alignment
  align_h    =  180; // height used for alignment
  gap_w      =   40;
  num_pt     =   35;
  score_min  =   40*num_pt;
 
  anlys_w    = 70*align_w/100; // width  used for analysis
  anlys_h    = 70*align_h/100; // height used for analysis

  centr_ofsx =  30; // x coord offset of computed center
  centr_ofsy =   0; // y coord offset of computed center

  lum_min    =   70;
  err_max    = 1000;

*/



int CSkinning::Add_menu_parameters( CContainer_wnd* child )
{


  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2452[lng], child );
  file_manager->Add_separator( string_2452[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &num_img,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit   [lng],   20,    0,   200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &imgshw,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2028[lng], string_unit   [lng],    1,    0,   200, NULL         );
  pView->parameter[pView->num_params++] = new CParameter( &align_w,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2454[lng], string_pix    [lng],  400,   30,   500, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &align_h,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2456[lng], string_pix    [lng],  180,   20,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &gap_w,        CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2458[lng], string_pix    [lng],   40,    2,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &score_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2460[lng], string_unit   [lng],   40,    0,   255, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &anlys_w,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2462[lng], string_percent[lng],   70,    5,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &anlys_h,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2464[lng], string_percent[lng],   70,    5,   300, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &centr_ofsy,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2466[lng], string_pix    [lng],   30,    0,   300, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &lum_min,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2468[lng], string_log    [lng],   70,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &err_max,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3178[lng], string_unit   [lng], 1000,    5, 99999, file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  return 0; // completed successfully
}



int CSkinning::Process_image( BYTE* img_acq, int nimg )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  //SCALE_IMG( index, img_acq );

  int x,y;

  // draw inspection area first

  if ( nimg==0 ) // only first image
  {
    good        = TRUE;
    //draw_ovl    = TRUE;
    txt1        = L"alignment";
    txt2        = L"analysis";
    err_tot     = 0;
    num_aligned = 0;
    //TRACE("START NEW SEQUENCE\n");
  }
  else
  {
    //draw_ovl = FALSE;
  }

  if ( nimg+1==imgshw || imgshw==0 ) draw_ovl =  TRUE;
  else                               draw_ovl = FALSE;

  if ( draw_ovl ) SCALE_IMG( index, img_acq ); // scale first

  err_int = 0; // clear error for this very image

  /*
  if ( draw_ovl ) 
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
  */


  int i;

  int score_max = 0;
  int x_max = 0;
  int y_max = 0;

  // SEARCH ALIGNMENT
  if ( align_w!=align_wact || align_h!=align_hact || gap_w!=gap_wact ) Prepare_alignment(); // need to precalculate

  if ( anlys_wact!=anlys_w || anlys_hact!=anlys_h ) Prepare_analysis();


  int xmax_algn = dimx-1-semiwdt-centr_ofsy; // calculate max x possible for alignment
  //TRACE("Max x for alignment = %d - deltax=%d\n", xmax_algn, semiwdt+centr_ofsy );

  for ( y = align_wact/2 + 1 ; y < dimy-align_wact/2-1 ; y+=4 )
  {
//    for ( x = align_h/2 + 1 ; x <= dimx-1 ; x+=4 )
    for ( x = align_hact/2 + 5 ; x < xmax_algn ; x+=4 )
    {
      int score = 0;

      for ( i=0 ; i<nptskin ; i++ )
      {
        ASSERT( x+ptskin[i].x1 >= 0 && x+ptskin[i].x1 < dimx );
        ASSERT( x+ptskin[i].x2 >= 0 && x+ptskin[i].x2 < dimx );
        ASSERT( y+ptskin[i].y1 >= 0 && y+ptskin[i].y1 < dimy );
        ASSERT( y+ptskin[i].y2 >= 0 && y+ptskin[i].y2 < dimy );

        score += PT(x+ptskin[i].x2,y+ptskin[i].y2)-PT(x+ptskin[i].x1,y+ptskin[i].y1);
      }

      if ( score>score_max )
      {
        score_max = score;
        x_max = x;
        y_max = y;
      }

//      TRACE( "%3d,%3d %6d\n", x,y,score/nptskin );

    }
  }

  score_max /= nptskin;

  if ( score_max < score_min ) // center NOT validated, alignment FAILED
  {
    txt1.Format( string_2482[lng], score_max, score_min );
    col1    = TXT_DARKRED;
    txt2    = L"";
    col2    = TXT_BLACK;
    centr_x = 0;
    centr_y = 0;
    //TRACE( "MAX FOUND IN %3d,%3d BUT SCORE TOO LOW - NOT VALIDATED (%6d)\n", x_max ,y_max ,score_max );
    return 99;
  }

  //TRACE( "MAX FOUND IN %3d,%3d (%6d)\n", x_max ,y_max ,score_max );

  centr_x = x_max + centr_ofsy;
  centr_y = y_max; // + centr_ofsy;
  txt1.Format( string_2484[lng], centr_x, centr_y, score_max, score_min );
  col1 = TXT_DARKGREEN;
  num_aligned++;


  // ANALYSIS INSIDE AREA

  // central line
  for ( y=centr_y-anlwdt[0] ; y<=centr_y+anlwdt[0] ; y++ )
  {
    //DRAW_PT( index, centr_x, y, COL_BLUE ); // 0xFFRRGGBB

    if ( lum_min>PT(centr_x,y) )
    {
      err_int += lum_min-PT( centr_x, y );
      if ( draw_ovl ) DRAW_PT( index, centr_x, y, COL_RED ); // 0xFFRRGGBB
    }
  }

  for ( x=1 ; x<semiwdt ; x++ )
  {
    for ( y=centr_y-anlwdt[x] ; y<=centr_y+anlwdt[x] ; y++ )
    {
      //DRAW_PT( index, centr_x + x, y, COL_CYAN ); // 0xFFRRGGBB
      //DRAW_PT( index, centr_x - x, y, COL_CYAN ); // 0xFFRRGGBB

      if ( lum_min>PT(centr_x+x,y) )
      {
        err_int += lum_min-PT( centr_x+x, y );
        if ( draw_ovl ) DRAW_PT( index, centr_x+x, y, COL_RED ); // 0xFFRRGGBB
      }

      if ( lum_min>PT(centr_x-x,y) )
      {
        err_int += lum_min-PT( centr_x-x, y );
        if ( draw_ovl ) DRAW_PT( index, centr_x-x, y, COL_RED ); // 0xFFRRGGBB
      }

    }

  }

  // Accumulate error
  err_tot += err_int;


/*  if ( err_int > err_max )
  {
    txt2.Format( L"Analysis error REJECT %d/%d", err_int, err_max );
    col2 = TXT_DARKRED;
  }
  else
  {
    txt2.Format( L"Analysis error GOOD %d/%d", err_int, err_max );
    col2 = TXT_DARKGREEN;
  }  */


//  int i;
  if ( draw_ovl ) 
  {

/*    for ( x = -anlys_h/2 ; x <= anlys_h/2 ; x++ )
    {
     // y = int( 0.5 + sqrt( ((anlys_w*anlys_w*anlys_h*anlys_h)-(4.0*anlys_w*anlys_w*x*x))/(4.0*anlys_h*anlys_h) ) );
      double w = double( anlys_w );
      double h = double( anlys_h );
      if ( 2*x < h ) y = int( 0.5 + 0.5*w*sqrt(h*h-4.0*x*x)/h );
      else           y = 0;
      ASSERT( y>=0 && y<=anlys_w );
      DRAW_PT( index, x_max + x, y_max + y, COL_BLUE ); // 0xFFRRGGBB
      DRAW_PT( index, x_max + x, y_max - y, COL_BLUE ); // 0xFFRRGGBB
    }  */


    UINT area_col = COL_CYAN;

    for ( x=1 ; x<=semiwdt ; x++ )
    {
      if ( anlwdt[x-1]-anlwdt[x]>4 )
      {
        for ( i=anlwdt[x] ; i<anlwdt[x-1] ; i+=3 )
        {
          DRAW_PT( index, centr_x + x, centr_y + i, area_col ); // 0xFFRRGGBB
          DRAW_PT( index, centr_x + x, centr_y - i, area_col ); // 0xFFRRGGBB
          DRAW_PT( index, centr_x - x, centr_y + i, area_col ); // 0xFFRRGGBB
          DRAW_PT( index, centr_x - x, centr_y - i, area_col ); // 0xFFRRGGBB
        }
      }
      else
      {
        DRAW_PT( index, centr_x + x, centr_y + anlwdt[x], area_col ); // 0xFFRRGGBB
        DRAW_PT( index, centr_x + x, centr_y - anlwdt[x], area_col ); // 0xFFRRGGBB
        DRAW_PT( index, centr_x - x, centr_y + anlwdt[x], area_col ); // 0xFFRRGGBB
        DRAW_PT( index, centr_x - x, centr_y - anlwdt[x], area_col ); // 0xFFRRGGBB
      }
    }


    for ( i=0 ; i<anlwdt[semiwdt] ; i+=3 )
    {
      DRAW_PT( index, centr_x + semiwdt, centr_y + i, area_col ); // 0xFFRRGGBB
      DRAW_PT( index, centr_x + semiwdt, centr_y - i, area_col ); // 0xFFRRGGBB
      DRAW_PT( index, centr_x - semiwdt, centr_y + i, area_col ); // 0xFFRRGGBB
      DRAW_PT( index, centr_x - semiwdt, centr_y - i, area_col ); // 0xFFRRGGBB
    }  


    // Alignment grid
    for ( i=0 ; i<nptskin ; i++ )
    {
      DRAW_PT( index, x_max + ptskin[i].x1, y_max + ptskin[i].y1, COL_ORANGE ); // 0xFFRRGGBB
      DRAW_PT( index, x_max + ptskin[i].x2, y_max + ptskin[i].y2, COL_ORANGE ); // 0xFFRRGGBB
    }

  }
  
  return 0; // completed successfully
}


int CSkinning::Final_result( void  )
{
  if ( num_aligned <= 3*num_img/4 ) // not enough alignment succeded
  {
    txt1.Format( string_2482[lng], num_aligned, num_img );
    col1 = TXT_DARKRED;
    txt2 = L"";
    col2 = TXT_BLACK;
    good = FALSE;
    return 99;
  }
  
  if ( err_tot > err_max )
  {
    txt2.Format( string_2478[lng], err_tot, err_max );
    good = FALSE;
    col2 = TXT_DARKRED;
  }
  else
  {
    txt2.Format( string_2480[lng], err_tot, err_max );
    col2 = TXT_DARKGREEN;
  }  


  return 0; // final result GOOD
}


/*
int CSkinning::Search_bottom( BYTE* img_acq )
{

  int x,y;

  int y1 = ymin +     ( ymax - ymin ) / 4;
  int y2 = ymin + 3 * ( ymax - ymin ) / 4;

  x = xmax;

  int ltot = 0;

  for ( y=y1 ; y<=y2 ; y+=4 ) ltot += PT(x,y);

  int lthr = int( 0.5 + 0.01 * bott_thr * ltot );

  x--;

  while ( x>xmin && ltot>lthr )
  {
    ltot = 0;
    for ( y=y1 ; y<=y2 ; y+=4 ) ltot += PT(x,y);
    x-=2;
  }

  if ( x<=xmin )
  {
    txt1 = string_3120[lng];
    col1 = TXT_DARKRED;
    return 99; // fail: bottom NOT found
  }

  xbase = x;

  if ( draw_ovl ) 
  {
    for ( y=y1 ; y<=y2 ; y++ ) DRAW_PT( index, xbase, y, COL_RED ); // 0xFFRRGGBB
  }

  num_aligned++;
  return 0; // completed successfully
}


int CSkinning::Analyize_sides( BYTE* img_acq )
{

  int x,y;

  int x1 = xbase - waist_hmax;
  int x2 = xbase - waist_hmin;


  ycent  = 0;
  int ycount = 0;

  int y1 = 0, y2 = 0;

  int edge_lenght =  10;
//  int lum_edge    = 100;

  int lum_lim = waist_lum*edge_lenght;

  int bad_height = 0; // total height consecutive  

  int consec_sta = 0;
  int consec_end = 0;

  //TRACE( "x,y1,y2,wd\n" );

  int err_frame = 0; // waist error for this frame

  for ( x=x1 ; x<=x2 ; x+=edge_lenght )
  {
    y  = (ymin+ymax)/2 + 2; // start from center
    y1 = 0;
    y2 = 0;

    do // search bottom/left
    {
      int lum = 0;
      for ( int i=0 ; i<edge_lenght ; i++ ) lum+=PT(x+i,y);
      if ( lum>lum_lim )
      {
        y2 = y;
        //for ( int i=0 ; i<edge_lenght ; i++ ) DRAW_PT( index, x+i, y, COL_GREEN ); // 0xFFRRGGBB
        break;
      }
      y++;
    } while ( y<ymax );

    y = (ymin+ymax)/2 - 2; // start from center

    do // bottom/right
    {
      int lum = 0;
      for ( int i=0 ; i<edge_lenght ; i++ ) lum+=PT(x+i,y);
      if ( lum>lum_lim )
      {
        y1 = y;
        //for ( int i=0 ; i<edge_lenght ; i++ ) DRAW_PT( index, x+i, y, COL_GREEN ); // 0xFFRRGGBB
        break;
      }
      y--;
    } while ( y>ymin );


    if ( y1 && y2 ) // found both sides, check width
    {
      ycent += y2+y1;
      ycount++;

      if ( y2-y1 > waist_wmax )   // Too wide MAGENTA
      {
//        err_waist += y2-y1 - waist_wmax;
        col = COL_MAGENTA;
      }
      else
      {
        if ( y2-y1 < waist_wmin ) // Too narrow RED
        {
//          err_waist += waist_wmax - y2-y1;
          //bad_height += edge_lenght;
          if ( consec_sta==0 ) consec_sta = x;
          consec_end = x+edge_lenght;
          col = COL_RED;

          err_frame += waist_wmin-(y2-y1);
          //TRACE("Tratto stretto x=%d\n",x);
        }
        else                      // Good width GREEN
        {
          if ( consec_sta && consec_end-consec_sta>waist_minhgt ) // just finished BAD SECTOR (high enough)
          {
            for ( int i=consec_sta ; i<consec_end ; i++ )
            {
              DRAW_PT( index, i, y1-13, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y1-10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y2+10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y2+13, COL_RED ); // 0xFFRRGGBB
            }

            waist_err += err_frame;
            //TRACE("Tratto consecutivo x=%d err_frame=%d\n",x,err_frame);

            consec_sta=0; // reset consec_sta
            consec_end=0; // reset consec_end
          }

          col = COL_GREEN;
          bad_height = 0; // reset consecutive counter

          err_frame = 0;
          //TRACE("Tratto normale x=%d\n",x);
        }

      }

      if ( draw_ovl ) // when required to draw
      {
        for ( int i=0 ; i<edge_lenght ; i++ )
        {
          DRAW_PT( index, x+i, y1, col ); // 0xFFRRGGBB
          DRAW_PT( index, x+i, y2, col ); // 0xFFRRGGBB
        }
      }

      //TRACE( "x=%3d - y1=%3d - y2=%3d - wd=%3d\n", x, y1, y2, y2-1 );
      //TRACE( "%3d,%3d,%3d,%3d\n", x, y1, y2, y2-y1 );
    }

  } // for x

  if ( ycount<3 )
  {
    return 291; // cannot find the y center of cake
  }

  ycent = int( 0.5 + 0.5*ycent/ycount );

  return 0; // completed successfully
}
*/


/*


int CSkinning::Analyize_layer( BYTE* img_acq )
{

  int x,y;

  int x1 = xbase - layer_hmax;
  int x2 = xbase - layer_hmin;

  int y1 = ycent - layer_width/2;
  int y2 = ycent + layer_width/2;

  // draw inspection area first
  if ( draw_ovl ) 
  {
    for ( x=x1 ; x<=x2 ; x+=4 )
    {
      DRAW_PT( index, x, y1, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, x, y2, area_color ); // 0xFFRRGGBB
    }
    for ( y=y1 ; y<=y2 ; y+=4 )
    {
      DRAW_PT( index, x1, y, area_color ); // 0xFFRRGGBB
      DRAW_PT( index, x2, y, area_color ); // 0xFFRRGGBB
    }
  }

  for ( y=y1 ; y<=y2 ; y++ )
  {
    for ( x=x1 ; x<=x2 ; x++ )
    {
      if ( PT(x,y)>layer_minlum )
      {
        layer_err += PT(x,y) - layer_minlum;
        DRAW_PT( index, x, y, 0xFFFF0000 ); // 0xFFRRGGBB
      }
    }
  }

  return 0; // completed successfully
}
*/


/*
int CSkinning::Search_bottom( BYTE* img_acq )
{
  return 0; // completed successfully
}
*/







