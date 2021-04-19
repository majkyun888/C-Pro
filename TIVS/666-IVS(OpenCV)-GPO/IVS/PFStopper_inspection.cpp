#pragma once

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "PFStopper_inspection.h"
//#include "ChildView.h"



extern Camera_struct    camera  [NUM_TLC];
//extern CArea_adjust*    areainsp[NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
//extern BOOL             learn_models;
extern circle_type      my_circle[MAXR_CIRCLES];
extern UINT             thread_status;

extern BOOL             datarec_on;
//extern Results_manager* bottomA_result;
//extern Results_manager* bottomB_result;

extern Results_manager*  result;


#define  xmin    area->buffrect.left  
#define  xmax    area->buffrect.right 
#define  ymin    area->buffrect.top   
#define  ymax    area->buffrect.bottom




CPFStopper_inspection::CPFStopper_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  nhead = 0;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;

  area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
  area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 ); //&& (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =      2; //  dimy/6;
  area->buffrect.bottom = dimy-2; // 5*dimy/6;
  area->buffrect.left   =      2; // dimx/6;
  area->buffrect.right  = dimx-2; // 5*dimx/6;

//  src_radius = 183; // src_radius used for alignment matching

//  ZeroMemory( model, sizeof(model) );
//  ZeroMemory( vial,  sizeof(vial)  );

//  model.data = NULL;
//  vial.data  = NULL;

//  Create_model();

//  circle.data = NULL;
  //precalc_circle3( 12, 100, &circle );
//  precalc_circles();


/*  src_radius    =  190;
  anl_radius    =  185;

  avg_max       =  220;
  avg_min       =  160;
  
  pos_dif       =   40;
  neg_dif       =   40;
  max_lum       =  200;
  min_lum       =   20;

  maxerr_pos    = 1000;
  maxerr_neg    = 1000;
  maxerr_max    = 1000;
  maxerr_min    = 1000;


  src_radius2 = 100; */


  radius_inn    =   40; // diameter of rubber stopper (dark circle)
  radius_out    =  130; // diameter of flange for alignment
//  diam_out      =  296; // outer diameter
  lmin_inn      =   80; // brightness threshold for inner alignment

  cstep         =   20; // step for coarse search
  fstep         =    2; // step for fine   search
  align_minpt   = 5000; // min number of points for fine alignment

  flange_thrs   =   50;
  petal_minlen  =   50;

/*  flange_avgl   =    5; 
  flange_rmin   =  140;
  flange_rmax   =  150;
  flange_minang =  100;
  flange_maxang =  120; */



  precalc_circles(); // precalculate all circles required for processing



}


CPFStopper_inspection::~CPFStopper_inspection(void)
{

//  if ( model.data ) free( model.data );
//  if ( vial.data  ) free( vial.data  );

  if ( area )
  {
    delete area;
    area = NULL;
  }
}


/*
int CPFStopper_inspection::Allocate_profile( void )
{
//  if ( profile ) free( profile );
//  prosize = xmax-xmin+1;
//  profile = (segm_type*) malloc( sizeof(segm_type)*prosize );
  return 0;
}
*/


int CPFStopper_inspection::Set_inspection_area( int ind )
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

  TRACE("CPFStopper_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CPFStopper_inspection::Add_menu_parameters( CContainer_wnd* child )
{
  wchar_t** name = string_4100; // standard 

  
  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( name[lng], child );
  file_manager->Add_separator( name[lng] );
//  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2760[lng], child );
//  file_manager->Add_separator( string_2760[lng] );

  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


  /*
  diam_inn      =  134; // diameter of rubber stopper (dark circle)
  //diam_out      =  296; // outer flange diameter
  lmin_inn      =   50; // brightness threshold for inner alignment
  cstep         =   20; // step for coarse search
  fstep         =    1; // step for fine   search
  align_minpt   = 5000; // min number of points for fine alignment

  flange_avgl   =    5; 
  flange_rmin   =  140;
  flange_rmax   =  150;
  flange_minang =  100;
  flange_maxang =  120;
  */


  pView->parameter[pView->num_params++] = new CParameter( &radius_inn,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4102[lng], string_pix    [lng],   67,   30,   150, file_manager ); // diameter of rubber stopper (dark circle)
  pView->parameter[pView->num_params++] = new CParameter( &lmin_inn,      CTYPE_BYTE,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4103[lng], string_log    [lng],   50,    0,   255, file_manager ); // brightness threshold for inner alignment
  pView->parameter[pView->num_params++] = new CParameter( &align_minpt,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4104[lng], string_unit   [lng], 5000,    0, 99999, file_manager ); // min number of points for fine alignment
  //pView->parameter[pView->num_params++] = new CParameter( &flange_avgl,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4106[lng], string_degrees[lng],    5,    1,    20, file_manager ); // moving average lenght for finding 2 maxes (L+1+L)
  //pView->parameter[pView->num_params++] = new CParameter( &flange_rmin,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4108[lng], string_pix    [lng],  140,    0,   250, file_manager ); // minimum allowed flange diameter
  //pView->parameter[pView->num_params++] = new CParameter( &flange_rmax,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4110[lng], string_pix    [lng],  150,    0,   250, file_manager ); // maximum allowed flange diameter
  pView->parameter[pView->num_params++] = new CParameter( &radius_out,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4105[lng], string_pix    [lng],  134,   50,   250, file_manager ); // diameter of rubber stopper (dark circle)
  pView->parameter[pView->num_params++] = new CParameter( &flange_thrs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4122[lng], string_degrees[lng],  100,    0, 10000, file_manager ); // minimum allowed petal angle
  pView->parameter[pView->num_params++] = new CParameter( &petal_minlen,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4112[lng], string_unit   [lng],   50,    0, 10000, file_manager ); // minimum allowed petal angle
//  pView->parameter[pView->num_params++] = new CParameter( &flange_minang, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4112[lng], string_degrees[lng],  100,    0,   100, file_manager ); // minimum allowed petal angle
//  pView->parameter[pView->num_params++] = new CParameter( &flange_maxang, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_4114[lng], string_degrees[lng],  120,    0,   120, file_manager ); // maximum allowed petal angle
//  pView->parameter[pView->num_params++] = new CParameter( &maxerr_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2782[lng], string_unit[lng],  100,    0, 99000, file_manager );
  
  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

 
  return 0; // completed successfully
}


 
/*
int CPFStopper_inspection::Cross( int x, int y, int& v1, int& v2, int& v3, int& v4  )
{
  //int lenght = 100;
  int stp =   3;
  int lng = 100;
  UINT col = 0xFF00FF00;

  v1 = 0;
  v2 = 0;
  v3 = 0;
  v4 = 0;

  DRAW_PT( index, x, y, col ); // 0xFFRRGGBB

  for ( int i=1 ; i<=lng ; i+=stp )
  {
    v1 += PT2( x+i, y+i );
    //DRAW_PT( index, x+i, y+i, col ); // 0xFFRRGGBB

    v2 += PT2( x+i, y-i );
    //DRAW_PT( index, x+i, y-i, col ); // 0xFFRRGGBB

    v3 += PT2( x-i, y+i );
    //DRAW_PT( index, x-i, y+i, col ); // 0xFFRRGGBB

    v4 += PT2( x-i, y-i );
    //DRAW_PT( index, x-i, y-i, col ); // 0xFFRRGGBB

  }

  return 0;

}
*/





int CPFStopper_inspection::Outer_box( int x, int y )
{
  //int lenght = 100;
  int delta = 5;
//  int lng = 100;
  UINT col = 0xFF008800;

  DRAW_PT( index, x, y, col ); // 0xFFRRGGBB

/* DRAW A SQUARE 
  for ( int i=-delta ; i<=delta ; i++ )
  {
    DRAW_PT( index, x+i, y+delta, col ); // 0xFFRRGGBB
    DRAW_PT( index, x+i, y-delta, col ); // 0xFFRRGGBB

    DRAW_PT( index, x-delta, y+i, col ); // 0xFFRRGGBB
    DRAW_PT( index, x+delta, y+i, col ); // 0xFFRRGGBB
  } */

  return 0;
}

/*
int CPFStopper_inspection::Outer_edge( int x, int y, int ang )
{
  ASSERT( ang>=0 && ang<360 );

  // 
  UINT col = 0xFF008800;

  int ind, ind2;
  int i, x2, y2;
  int amp =  5; // half-width of arc

  int of = 10; // distance for extern arc

  if ( ang < 180 )
  {
    ind  = int( 0.5 + my_circle[ro].n * ang / 180.0 );
    ind2 = int( 0.5 + my_circle[of].n * ang / 180.0 );

    for ( i=ind-amp ; i<=ind+amp ; i++ )
    {
      x2 = xcnt + my_circle[ro].x[i];
      y2 = ycnt + my_circle[ro].y[i];
      DRAW_PT( index, x2, y2, col ); // 0xFFRRGGBB

      x2 = xcnt + my_circle[of].x[ind2] + my_circle[ro].x[i];
      y2 = ycnt + my_circle[of].y[ind2] + my_circle[ro].y[i];
      DRAW_PT( index, x2, y2, col ); // 0xFFRRGGBB

    }
  }
  else
  {

  }

  return 0;
}
*/




int CPFStopper_inspection::Find_arcedge( int ang, int rsta, int rend, int hwd )
{
  int rad = rsta;

  int len = rsta-rend+1;
  ASSERT( len>0 && len<150 );

  int* val = (int*) malloc( len*sizeof(int) );
  ZeroMemory( val, len*sizeof(int) );

  int dist = 4;  // distance in radius for brigth. comparison

  int thrs = 20; // threshold of brightness difference

  int lum = 0;

  int ind = 0;

  while ( rad>=rend )
  {
    val[ind] = Outer_arc( ang, rad, hwd );

    if ( ind>=dist )
    {
      if ( val[ind]-val[ind-dist]>thrs*(hwd*2+1) )
      {
        //TRACE("Edge for ang=%3d found for r=%3d - delta=%3d\n", ang, ro+rad+dist, (val[ind]-val[ind-dist])/(hwd*2+1) );
        free( val );
        return( radius_out+rad+dist );
      }

    }

    rad--;
    ind++;
  }

  //for ( int i=0 ; i<len ; i++ ) TRACE("%3d - %5d\n", rsta-i, val[i] );

  //TRACE("Edge for ang=%3d NOT found\n");

  free( val );

  return 0;
}






// calculate total brightness on the outer arc at angle <ang>, offset <rad> to center and <hwd> halfwidth
int CPFStopper_inspection::Outer_arc( int ang, int rad, int hwd )
{
  int val = 0;
  int i, ind;
  int x, y;


  ASSERT( ang>=0 && ang<360 );

  // Calculate x and y according to ang and rad
  Radial_offset( ang, rad, x, y );

  if ( ang<180 )
  {
//    ind = int( 0.5 + my_circle[ro].n*ang/180.0 );
    ind = int( my_circle[radius_out].n*ang/180.0 );
    ASSERT( ind>=0 && ind<my_circle[radius_out].n );

    if ( ind-hwd>=0 && ind+hwd<my_circle[radius_out].n )
    {
      for ( i=ind-hwd ; i<=ind+hwd ; i++ )
      {
        //DRAW_PT( index, x+my_circle[ro].x[i], y+my_circle[ro].y[i], 0xFF0000BB ); // 0xFFRRGGBB
        val += PT2( x+my_circle[radius_out].x[i], y+my_circle[radius_out].y[i] );
      }
      //DRAW_PT( index, x+my_circle[ro].x[ind], y+my_circle[ro].y[ind], 0xFFFF0000 ); // 0xFFRRGGBB
    }
    else // rollover on one edge
    {
      for ( i=ind-hwd ; i<=ind+hwd ; i++ )
      {
        if ( i<0 )
        {
          //DRAW_PT( index, x-my_circle[ro].x[i+my_circle[ro].n], y-my_circle[ro].y[i+my_circle[ro].n], 0xFF0000BB ); // 0xFFRRGGBB
          val += PT2( x-my_circle[radius_out].x[i+my_circle[radius_out].n], y-my_circle[radius_out].y[i+my_circle[radius_out].n] );
        }
        else
        {
          if ( i>=my_circle[radius_out].n )
          {
            //DRAW_PT( index, x-my_circle[ro].x[i-my_circle[ro].n], y-my_circle[ro].y[i-my_circle[ro].n], 0xFF0000BB ); // 0xFFRRGGBB
            val += PT2( x-my_circle[radius_out].x[i-my_circle[radius_out].n], y-my_circle[radius_out].y[i-my_circle[radius_out].n] );
          }
          else
          {
            //DRAW_PT( index, x+my_circle[ro].x[i], y+my_circle[ro].y[i], 0xFF0000BB ); // 0xFFRRGGBB
            val += PT2( x+my_circle[radius_out].x[i], y+my_circle[radius_out].y[i] );
          }
        }
      }

      //DRAW_PT( index, x+my_circle[ro].x[ind], y+my_circle[ro].y[ind], 0xFFFF0000 ); // 0xFFRRGGBB
    }
  }
  else // ang>=180
  {
//    ind = int( 0.5 + my_circle[ro].n*ang/180.0 );
    ind = int( my_circle[radius_out].n*(ang-180)/180.0 );
    ASSERT( ind>=0 && ind<my_circle[radius_out].n );

    if ( ind-hwd>=0 && ind+hwd<my_circle[radius_out].n )
    {
      for ( i=ind-hwd ; i<=ind+hwd ; i++ )
      {
        //DRAW_PT( index, x-my_circle[ro].x[i], y-my_circle[ro].y[i], 0xFF0000BB ); // 0xFFRRGGBB
        val += PT2( x-my_circle[radius_out].x[i], y-my_circle[radius_out].y[i] );
      }
      //DRAW_PT( index, x-my_circle[ro].x[ind], y-my_circle[ro].y[ind], 0xFFFF0000 ); // 0xFFRRGGBB
    }
    else // rollover on one edge
    {
      for ( i=ind-hwd ; i<=ind+hwd ; i++ )
      {
        if ( i<0 )
        {
          //DRAW_PT( index, x+my_circle[ro].x[i+my_circle[ro].n], y+my_circle[ro].y[i+my_circle[ro].n], 0xFF0000BB ); // 0xFFRRGGBB
          val += PT2( x+my_circle[radius_out].x[i+my_circle[radius_out].n], y+my_circle[radius_out].y[i+my_circle[radius_out].n] );
        }
        else
        {
          if ( i>=my_circle[radius_out].n )
          {
            //DRAW_PT( index, x+my_circle[ro].x[i-my_circle[ro].n], y+my_circle[ro].y[i-my_circle[ro].n], 0xFF0000BB ); // 0xFFRRGGBB
            val += PT2( x+my_circle[radius_out].x[i-my_circle[radius_out].n], y+my_circle[radius_out].y[i-my_circle[radius_out].n] );
          }
          else
          {
            //DRAW_PT( index, x-my_circle[ro].x[i], y-my_circle[ro].y[i], 0xFF0000BB ); // 0xFFRRGGBB
            val += PT2( x-my_circle[radius_out].x[i], y-my_circle[radius_out].y[i] );
          }
        }
      }

      //DRAW_PT( index, x-my_circle[ro].x[ind], y-my_circle[ro].y[ind], 0xFFFF0000 ); // 0xFFRRGGBB
    }
  }

  return val;

  //return 0;
}





int CPFStopper_inspection::Coords( int &x, int &y, int ang, int rad )
{
  if ( rad==0 )
  {
    x = xcnt;
    y = ycnt;
    return 0;
  }

  int ind;

  if ( ang < 180 )
  {
    ind = int( my_circle[rad].n*ang/180.0 );
    x = xcnt+my_circle[rad].x[ind];
    y = ycnt+my_circle[rad].y[ind];
  }
  else
  {
    ind = int( my_circle[rad].n*(ang-180)/180.0 );
    x = xcnt-my_circle[rad].x[ind];
    y = ycnt-my_circle[rad].y[ind];
  }

  return 0;
}


int CPFStopper_inspection::Draw_marker( int ang, int rad, UINT col/*=0xFF00FF00*/ )
{
  int ind;

  if ( rad==0 ) return 99;


  if ( ang < 180 )
  {
    ind = int( my_circle[rad].n*ang/180.0 );
    DRAW_SQUARE( index, xcnt+my_circle[rad].x[ind], ycnt+my_circle[rad].y[ind], col ); // 0xFFRRGGBB
  }
  else
  {
    ind = int( my_circle[rad].n*(ang-180)/180.0 );
    DRAW_SQUARE( index, xcnt-my_circle[rad].x[ind], ycnt-my_circle[rad].y[ind], col ); // 0xFFRRGGBB
  }

  return 0;
}


int CPFStopper_inspection::Draw_point( int ang, int rad, UINT col/*=0xFF00FF00*/ )
{
  int ind;

  if ( rad==0 ) DRAW_PT( index, xcnt, ycnt, col ); // 0xFFRRGGBB

  if ( ang < 180 )
  {
    ind = int( my_circle[rad].n*ang/180.0 );
    DRAW_PT( index, xcnt+my_circle[rad].x[ind], ycnt+my_circle[rad].y[ind], col ); // 0xFFRRGGBB
  }
  else
  {
    ind = int( my_circle[rad].n*(ang-180)/180.0 );
    DRAW_PT( index, xcnt-my_circle[rad].x[ind], ycnt-my_circle[rad].y[ind], col ); // 0xFFRRGGBB
  }

  return 0;
}


// Draw a point on radius <rad> and index <ind>   0-npt-1=>0-179° / npt-2npt=>180-359°
int CPFStopper_inspection::Draw_index( int ind, int rad, UINT col/*=0xFF00FF00*/ )
{
  #define NPT my_circle[rad].n
  if ( rad==0 ) DRAW_PT( index, xcnt, ycnt, col ); // 0xFFRRGGBB

  ASSERT( ind>=0 && ind<2*NPT );

  if ( ind < NPT )
  {
    DRAW_PT( index, xcnt+my_circle[rad].x[ind], ycnt+my_circle[rad].y[ind], col ); // 0xFFRRGGBB
  }
  else
  {
    DRAW_PT( index, xcnt-my_circle[rad].x[ind-NPT], ycnt-my_circle[rad].y[ind-NPT], col ); // 0xFFRRGGBB
  }

  #undef NPT
  return 0;
}








// Draw a petal with center radius <rad> from <ista> to <iend>
// <ista> and <iend> range 0-npt-1 => 0-359°
int CPFStopper_inspection::Draw_petal( int ista, int iend, int rad, UINT col/*=0xFF00FF00*/ )
{
  //int npt = my_circle[rad].n;

  int r  = rad - 10; // INNER FIRST

  for ( r=rad-10 ; r<=rad+10 ; r+=20 )
  {
    int i1 = int( 0.5 + ista * my_circle[r].n / my_circle[rad].n );
    int i2 = int( 0.5 + iend * my_circle[r].n / my_circle[rad].n );
    ASSERT( i1>=0 && i1<2*my_circle[r].n );
    ASSERT( i2>=0 && i2<2*my_circle[r].n );

    if ( i2>i1 ) // NOT zero crossing
    {
      for ( int i=i1 ; i<=i2 ; i++ )
      {
        Draw_index( i, r, 0xFF00AA00 );
        Draw_index( i, r, 0xFF00AA00 );
      }
    }
    else // Zero crossing
    {
      // i1 to npt-1
      for ( int i=i1 ; i<2*my_circle[r].n ; i++ )
      {
        Draw_index( i, r, 0xFF00AA00 );
        Draw_index( i, r, 0xFF00AA00 );
      }
      // 0 to i2
      for ( int i=0 ; i<=i2 ; i++ )
      {
        Draw_index( i, r, 0xFF00AA00 );
        Draw_index( i, r, 0xFF00AA00 );
      }
    }
  }

  return 0;
}




int CPFStopper_inspection::Radial_offset( int ang, int rad, int& x, int& y )
{
  ASSERT( ang>=   0 && ang< 360 );
  ASSERT( rad>=-100 && rad<=100 );

  if ( rad== 0 )
  {
    x = xcnt;
    y = ycnt;
    return 0;
  }

  int ind;
  if ( ang < 180 )
  {
    if ( rad > 0 )
    {
//      ind = int( 0.5 + my_circle[rad].n*ang/180.0 );
      ind = int( my_circle[rad].n*ang/180.0 );
      ASSERT( ind >= 0 && ind < my_circle[rad].n );
      x = xcnt + my_circle[rad].x[ind];
      y = ycnt + my_circle[rad].y[ind];
    }
    else
    {
      ind = int( my_circle[-rad].n*ang/180.0 );
      ASSERT( ind >= 0 && ind < my_circle[-rad].n );
      x = xcnt - my_circle[-rad].x[ind];
      y = ycnt - my_circle[-rad].y[ind];
    }
  }
  else // ang > 180
  {
    ang -= 180;
    if ( rad > 0 )
    {
      ind = int( my_circle[rad].n*ang/180.0 );
      ASSERT( ind >= 0 && ind < my_circle[rad].n );
      x = xcnt - my_circle[rad].x[ind];
      y = ycnt - my_circle[rad].y[ind];
    }
    else
    {
      ind = int( my_circle[-rad].n*ang/180.0 );
      ASSERT( ind >= 0 && ind < my_circle[-rad].n );
      x = xcnt + my_circle[-rad].x[ind];
      y = ycnt + my_circle[-rad].y[ind];
    }
  }

  return 0;
}


/*
int CPFStopper_inspection::Insert_average( int ang, int val )
{
  static int* buffer = NULL;
  static int  tot;
  static int  vmax1, nmax1;
  static int  vmax2, nmax2;
  static int  vmax3, nmax3;

  int i;

  int inda1, inda2; // start/end index of first petal / if inda1>inda2 accross zero
  int indb1, indb2; // start/end index of second petal


  ASSERT( flange_avgl>=5 && flange_avgl<=100 );
  ASSERT( val>=0 && val<300 );

  if ( ang==0 ) // first - init buffer
  {
    buffer = (int*) malloc( sizeof(int)*360 );
    ASSERT( buffer );
  }

  buffer[ang] = val; // store value
  TRACE("a=%3d - val=%3d\n", ang, val );

  //if ( val>0 ) Draw_marker( ang, val );





  if ( ang==359 ) // last - scan for min/max
  {
    //vmax1 = amax1 = 0;
    //vmax2 = amax2 = 0;

    flange_thrs = flange_rmin*flange_avgl;

    int status = 0;

    inda1 = inda2 = 0;
    indb1 = indb2 = 0;


    vmax1  = vmax2  = vmax3  = 0;
    nmax1  = nmax2  = nmax3  = 0; 


    //status=99;

    // preload
    tot = buffer[0];
    for ( i=1 ; i<=flange_avgl ; i++ ) tot += buffer[360-i]+buffer[i];
    i=0;

    while ( i<360 )
    {

      switch ( status )
      {
        case 0: // wait first part
          if ( tot>=flange_thrs ) 
          {
            status = 1; // entered first part
            vmax1  = i; // start accumulator
            nmax1  = 1; // start counter
            inda1  = i;
            TRACE("i=%3d - status=>1 - vmax1=%5d nmax1=%3d\n", i, vmax1, nmax1 );
          }
          break;

        case 1: // wait end of first part
          if ( tot<flange_thrs ) 
          {
            status = 2; // finished first part
            inda2  = i-1;
            TRACE("i=%3d - status=>2 - %3d-%3d - vmax1=%5d nmax1=%3d angolo1=%3d\n", i, inda1, inda2, vmax1, nmax1, int(0.5+vmax1/nmax1) );
          }
          else
          {
            vmax1 += i; // add accumulator
            nmax1 += 1; // add counter
          }
          break;

        case 2: // wait second part
          if ( tot>=flange_thrs ) 
          {
            status = 3; // entered second part
            vmax2  = i; // start accumulator
            nmax2  = 1; // start counter
            indb1  = i;
            TRACE("i=%3d - status=>3 - vmax2=%5d nmax2=%3d\n", i, vmax2, nmax2 );
          }
          break;

        case 3: // wait end of second part
          if ( tot<flange_thrs ) 
          {
            status = 4; // finished second part
            indb2  = i-1;
            TRACE("i=%3d - status=>4 - %3d-%3d - vmax2=%5d nmax2=%3d angolo2=%3d\n", i, indb1, indb2, vmax2, nmax2, int(0.5+vmax2/nmax2) );
          }
          else
          {
            vmax2 += i; // add accumulator
            nmax2 += 1; // add counter
          }
          break;


        case 4: // third part
          if ( tot>=flange_thrs ) 
          {
            status = 5; // entered third part
            vmax3  = i; // start accumulator
            nmax3  = 1; // start counter
            inda1  = i;
            TRACE("i=%3d - status=>5 - vmax3=%5d nmax3=%3d\n", i, vmax3, nmax3 );
          }
          break;

        case 5: // wait end of third part
          if ( tot<flange_thrs ) 
          {
            status = 6; // finished third part
            TRACE("i=%3d - status=>6 - vmax3=%5d nmax3=%3d angolo3=%3d\n", i, vmax3, nmax3, int(0.5+vmax3/nmax3) );
          }
          else
          {
            vmax3 += i; // add accumulator
            nmax3 += 1; // add counter
          }

          if ( i==359 ) // end of revolution => accross zero
          {

            TRACE("i=%3d - End of revolution - third part connected with first\n", i);
            TRACE("vmax1=%5d nmax1=%3d\n", vmax1, nmax1 );
            TRACE("vmax3=%5d nmax3=%3d\n", vmax3, nmax3 );
            vmax1 += 360*nmax1; 
            TRACE("Corrected vmax1=%5d nmax1=%3d\n", vmax1, nmax1 );
            vmax1 = vmax1 + vmax3;
            nmax1 = nmax1 + nmax3;
            TRACE("Combined - %3d-%3d - vmax1=%5d nmax1=%3d angolo1=%3d\n", inda1, inda2, vmax1, nmax1, int(0.5+vmax1/nmax1) );
          }
          break;


      }

      
      //TRACE( "i=%3d - val=%3d - tot=%6d\n", i, buffer[i], tot );




      // Update moving average
      int ind2 = i-flange_avgl;
      if ( ind2<0 ) ind2+=360;
      i++;
      int ind1 = i+flange_avgl;
      if ( ind1>=360 ) ind1-=360;
      tot += buffer[ind1]-buffer[ind2];

    }


    UINT col;

    // Evaluate found petals
    // FIRST PETAL
    if ( inda2 > inda1 ) // not zero-crossing
    {
      if ( inda2-inda1+1>=flange_minang && inda2-inda1+1<=flange_maxang ) // acceptable
      {
        col = 0xFF00FF00; // green
      }
      else
      {
        col = 0xFFFF0000; // red
      }

      for ( i=inda1 ; i<=inda2 ; i++ ) Draw_marker( i, buffer[i], col );
    }
    else // zero-crossing
    {

      if ( inda1+360-inda2>=flange_minang && inda1+360-inda2<=flange_maxang ) // acceptable
      {
        col = 0xFF00FF00; // green
      }
      else
      {
        col = 0xFFFF0000; // red
      }

      for ( i=inda1 ; i<   360 ; i++ ) Draw_marker( i, buffer[i], col );
      for ( i=    0 ; i<=inda2 ; i++ ) Draw_marker( i, buffer[i], col );
    }


    // SECOND PETAL

    if ( indb2-indb1+1>=flange_minang && indb2-indb1+1<=flange_maxang ) // acceptable
    {
      col = 0xFF00FF00; // green
    }
    else
    {
      col = 0xFFFF0000; // red
    }

    for ( i=indb1 ; i<=indb2 ; i++ ) Draw_marker( i, buffer[i], col );




    if ( buffer ) // clear and free memory
    {
      free( buffer );
      buffer = NULL;
    }
  }

  return 0;
}
*/

// flange analysis, return 0 if GOOD or !=0 if reject
int CPFStopper_inspection::Flange_analysis()
{
  int npt = my_circle[radius_out].n;
  int i;
  int x, y;
  int* buffer;
  int* ptx;
  int* pty;
  int* accum;
  //int var;
  //UINT col;

  int ret; // return value: 0=good, !=0 some reject/error

  #define MAXNUMPET 10  // max number of min/max allowed
  int ista[MAXNUMPET];  // start index of petal
  int iend[MAXNUMPET];  // end   index of petal
  int npet   =   0;     // number of found petals
  int minpos = 100;     // minimum pos width to be acceptable ( width above threshold )
  int minneg =  20;     // minimum gap width to be acceptable ( width below threshold )


  int hwpet = 100;      // typical half-width of petal (in units)  hwpet-0-hwpet


  //flange_thrs = 9000;

  // Allocate buffer
  buffer = (int*) malloc( sizeof(int)*npt*2 );
  ASSERT( buffer );
  ptx    = (int*) malloc( sizeof(int)*npt*2 );
  ASSERT( ptx );
  pty    = (int*) malloc( sizeof(int)*npt*2 );
  ASSERT( ptx );
  accum  = (int*) malloc( sizeof(int)*npt*2 );
  ASSERT( accum );


  // 0-180°
  for ( i=0 ; i<npt ; i++ )
  {
    x = xcnt+my_circle[radius_out].x[i];
    y = ycnt+my_circle[radius_out].y[i];
    ptx   [i] = x;
    pty   [i] = y;

    if ( Variance2( x, y, 9 ) > flange_thrs )
    {
      buffer[i] =  1;
      DRAW_PT( index, x, y, 0xFF00AA00 ); // var above threshold
    }
    else
    {
      buffer[i] =  0;
      DRAW_PT( index, x, y, 0xFFAA0000 ); // 0xFFRRGGBB
    }

//    buffer[i] = Variance( x, y, 5 );
//    buffer[i] = Variance2( x, y, 9 );
//    TRACE("i=%3d (%3d,%3d) var=%5d\n", i, x, y, buffer[i] );
    //var = Variance( x, y, 5 );
    //if ( buffer[i]>flange_thrs ) DRAW_PT( index, x, y, 0xFF00AA00 ); // var above threshold
    //else                         DRAW_PT( index, x, y, 0xFFAA0000 ); // 0xFFRRGGBB
  }

  //TRACE( "%3d,%3d - var = %6d\n", x, y, val ); */

  // 180-360°
  for ( i=0 ; i<npt ; i++ )
  {
    x = xcnt-my_circle[radius_out].x[i];
    y = ycnt-my_circle[radius_out].y[i];
    ptx   [i+npt] = x;
    pty   [i+npt] = y;

    if ( Variance2( x, y, 9 ) > flange_thrs )
    {
      buffer[i+npt] =  1;
      DRAW_PT( index, x, y, 0xFF00AA00 ); // var above threshold
    }
    else
    {
      buffer[i+npt] =  0;
      DRAW_PT( index, x, y, 0xFFAA0000 ); // 0xFFRRGGBB
    }

    //buffer[i+npt] = Variance( x, y, 5 );
    //buffer[i+npt] = Variance2( x, y, 9 );
//    TRACE("i=%3d (%3d,%3d) var=%5d\n", i+npt, x, y, buffer[i+npt] );
    //var = Variance( x, y, 5 );
    //if ( buffer[i+npt]>flange_thrs ) DRAW_PT( index, x, y, 0xFF00AA00 ); // var above threshold
    //else                             DRAW_PT( index, x, y, 0xFFAA0000 ); // 0xFFRRGGBB
  }

  accum[0] = buffer[0];

  for ( i=1 ; i<=hwpet ; i++ ) // preload
  {
    accum[0] += buffer[i] + buffer[2*npt-i];
  }
//  vmax1 = accum[0];
//  imax1 = 0;

  for ( i=1+2*npt ; i<4*npt ; i++ ) // calculate
  {
    int i1 = (i+hwpet  )%(2*npt);
    int i2 = (i-hwpet-1)%(2*npt);

    ASSERT( i1>=0 && i1<2*npt );
    ASSERT( i2>=0 && i2<2*npt );

    accum[i-2*npt] = accum[i-2*npt-1] + buffer[i1] - buffer[i2];

    //if ( accum[i-2*npt]

  }


  //int vmax1=0, imax1=0, vmax2=0, imax2=0;
  int tot = 0;
  int amp = 0;

  int acc_thr = 60*(hwpet*2+1)/100; // threshold on accum

  /*
  for ( i=0 ; i<2*npt ; i++ ) // 
  {
    TRACE( "%d,%d,%d,%3.1lf,%d,%d\n", i, ptx[i], pty[i],180.0*i/npt, buffer[i], accum[i] ); 
  }
  */

  i   =    0;
  ret = 1100; // petal analysis


  int petal1 = 0; // lenght of petal 1 found
  int petal2 = 0; // lenght of petal 2 found


  if ( accum[0]>acc_thr ) // first petal accross zero
  {

    // search first end of first
    while ( accum[i]>acc_thr )
    {
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }
    
    // search for second petal start
    while ( accum[i]<=acc_thr )
    {
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }

    // Inside second petal, search for end
    while ( accum[i]>acc_thr )
    {
      tot += i;
      amp++;
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING END OF FIRST PETAL
    }

    // SECOND PETAL FOUND
    //TRACE("SECOND PETAL: AMP=%d CENT=%d\n", amp, int(0.5+tot/amp) );
    petal2 = amp; // int( 0.5 + 180.0 * amp / npt );

    // search for first petal start (before zero)
    while ( accum[i]<=acc_thr )
    {
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }

    tot = 0;
    amp = 0;

    // Inside first petal, search for end before zero
    while ( accum[i]>acc_thr && i<2*npt )
    {
      tot += i;
      amp++;
      i++;
      //if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING END OF FIRST PETAL
    }

    // then search end of first after zero
    i = 0;
    while ( accum[i]>acc_thr )
    {
      tot += i+2*npt;
      amp++;
      i++;
      //if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }

    tot = int(0.5+tot/amp);
    if ( tot>2*npt ) tot-=2*npt;

    // SECOND PETAL FOUND
    //TRACE("FIRST PETAL: AMP=%d CENT=%d\n", amp, tot );
    petal1 = amp; // int( 0.5 + 180.0 * amp / npt );

    ret = 0;

  }
  else // first petal NOT accross zero
  {

    // search for first petal start
    while ( accum[i]<=acc_thr )
    {
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }

    // Inside first petal, search for end
    while ( accum[i]>acc_thr )
    {
      tot += i;
      amp++;
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING END OF FIRST PETAL
    }

    // FIRST PETAL FOUND
    //TRACE("FIRST PETAL: AMP=%d CENT=%d\n", amp, int(0.5+tot/amp) );
    petal1 = amp; // int( 0.5 + 180.0 * amp / npt );

    // search for second petal start
    while ( accum[i]<=acc_thr )
    {
      i++;
      if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING START OF FIRST PETAL
    }

    tot = 0;
    amp = 0;

    // Inside first petal, search for end
    while ( accum[i]>acc_thr && i<2*npt )
    {
      tot += i;
      amp++;
      i++;
      //if ( i>=2*npt ) goto returnpoint; // ERROR WHILE SEARCHING END OF FIRST PETAL
    }

    // SECOND PETAL FOUND
    //TRACE("SECOND PETAL: AMP=%d CENT=%d\n", amp, int(0.5+tot/amp) );
    petal2 = amp; // int( 0.5 + 180.0 * amp / npt );

    ret = 0;

  }
  
  
  //TRACE("FINITO\n");
  
  







  /*
  while ( i<2*npt )
  {
    //TRACE( "%d,%3.1lf,%d\n", i, 180.0*i/npt, buffer[i] ); 
    //TRACE( "%d,%d,%d,%3.1lf,%d\n", i, ptx[i], pty[i],180.0*i/npt, buffer[i] ); 
    i++;
  }
  */

  ////////////////////////////////////////////
  returnpoint:

  // Clear all buffers
  if ( buffer ) // clear and free memory
  {
    free( buffer );
    buffer = NULL;
  }
  if ( ptx ) // clear and free memory
  {
    free( ptx );
    ptx = NULL;
  }
  if ( pty ) // clear and free memory
  {
    free( pty );
    pty = NULL;
  }
  if ( accum ) // clear and free memory
  {
    free( accum );
    accum = NULL;
  }

  if ( petal1<petal_minlen || petal2<petal_minlen ) ret = 2000; 


  if ( ret )
  {
    txt2.Format(L"PETALS REJECT %d-%d (%d)", petal1, petal2, petal_minlen );
    col2 = TXT_DARKRED;
  }
  else
  {
    txt2.Format(L"PETALS GOOD %d-%d (%d)", petal1, petal2, petal_minlen );
    col2 = TXT_DARKGREEN;
  }


  return ret;








  int status = 0; // waiting for petal beginning
  //  status = 1; // waiting for end

//  int mingap =   5;  
//  int ista   = -1000;
//  int iend   = -1000;

  int lastpn = 0; // index of previous change below=>above threshold -  -1 => uninitialized
  int lastnp = 0; // index of previous change above=>below threshold -  -1 => uninitialized


  ista[0] = 0;

  i = 0;

  //TRACE( "i,x,y,ang,var\n", i, ptx[i], pty[i],180.0*i/npt, buffer[i] ); 
/*
  #define MAXNUMPET 10  // max number of min/max allowed
  int ista[MAXNUMPET];  // start index of petal
  int iend[MAXNUMPET];  // end   index of petal
  int npet   =  0;      // number of found petals
  int minpos = 50;      // minimum pos width to be acceptable ( width above threshold )
  int minneg = 50;      // minimum gap width to be acceptable ( width below threshold )
*/




  while ( i<2*npt )
  {
    //TRACE( "%3d - %3.1lf - %6d\n", i, 180.0*i/npt, buffer[i] ); 
    //TRACE( "%d,%d,%d,%3.1lf,%d\n", i, ptx[i], pty[i],180.0*i/npt, buffer[i] ); 

    switch ( status )
    {
      case 0: // wait beginning of petal
        if ( buffer[i]>=flange_thrs ) // var above threshold
        {
          status     = 1; // inside petal
          lastnp     = i;
          if ( trace ) TRACE("AboveTH %4d (%3d,%3d)\n", i, ptx[i], pty[i] );

          if ( i-lastpn>=minneg )
          {
            ista[npet] =  i;
            iend[npet] = -1; // -1 means unassigned
            if ( trace ) TRACE("Pet[%d] start %4d      (%3d,%3d)\n", npet, i, ptx[ista[npet]], pty[ista[npet]] );
            //TRACE("Pet[%d] start %4d (%3.1lf)\n", npet, i, 180.0*i/npt );
          }
          else
          {
            if ( trace ) TRACE("Pet[%d] neg too small (%4d-%4d), neglect\n",i,lastnp);
          }


          /*
          if ( npet>0 && i-iend[npet-1]<minneg ) // keep previous ista
          {
            npet--;
            TRACE("Pet[%d] start %4d      (%3d,%3d) too small minneg - Keeping previous (%3d,%3d)\n", npet, i, ptx[i], pty[i], ptx[ista[npet]], pty[ista[npet]] );
          }
          else // start new petal
          {
            ista[npet] = i;
            iend[npet] = -1; // -1 means unassigned
            TRACE("Pet[%d] start %4d      (%3d,%3d)\n", npet, i, ptx[ista[npet]], pty[ista[npet]] );
            //TRACE("Pet[%d] start %4d (%3.1lf)\n", npet, i, 180.0*i/npt );
          }*/
          
        }
        break;

      case 1: // wait petal's end
        if ( buffer[i]<flange_thrs ) // var below threshold
        {
          status     =   0; // petal finished
          lastpn     =   i;
          if ( trace ) TRACE("BelowTH %4d (%3d,%3d)\n", i, ptx[i], pty[i] );
          // check for minimum width
          //if ( i-ista[npet]>=minpos )
          if ( i-lastnp>=minpos )
          {
            iend[npet] = i-1;
//          TRACE("Petal found %4d-%4d (%3.1lf,%3.1lf) - lenght %4d (%3.1lf)\n", ista[npet], iend[npet], 180.0*ista[npet]/npt, 180.0*iend[npet]/npt, iend[npet]-ista[npet]+1, 180.0*(iend[npet]-ista[npet]+1)/npt );

            if ( trace ) TRACE("Pet[%d] found %4d-%4d (%3d,%3d)-(%3d,%3d) - lenght %4d (%3.1lf)\n", npet, ista[npet], iend[npet], ptx[ista[npet]], pty[ista[npet]], ptx[iend[npet]], pty[iend[npet]], iend[npet]-ista[npet]+1, 180.0*(iend[npet]-ista[npet]+1)/npt );

            npet++;
            if ( npet>=MAXNUMPET ) i=2*npt; // force to break
          }
          else
          {
            if ( npet==0 && buffer[0]+buffer[1]+buffer[2]>3*flange_thrs ) // keep it for possible connection across zero
            {
              iend[npet] = i-1;
              if ( trace ) TRACE("Pet[%d] found %4d-%4d (%3d,%3d)-(%3d,%3d) - lenght %4d (%3.1lf) TOO SHORT BUT KEEPING\n", npet, ista[npet], iend[npet], ptx[ista[npet]], pty[ista[npet]], ptx[iend[npet]], pty[iend[npet]], iend[npet]-ista[npet]+1, 180.0*(iend[npet]-ista[npet]+1)/npt );
              npet++;
              ista[npet] = lastnp;
            }
            if ( trace ) TRACE("Pet[%d] pos too small (%4d-%4d), neglect\n",i,lastnp);
          }

        }
        break;

    } 

    i++;
  }

  ASSERT( i==2*npt );

  // check accross zero
  if ( npet<MAXNUMPET )
  {
//    if ( ista[0]<1 && iend[npet]==-1 && 2*npt-lastpn>2 ) // pet[0] beggining in 0 or 1 and end of last pet unassigned
    if ( ista[0]<1 && iend[npet]==-1 && ( buffer[i-1]+buffer[i-2]+buffer[i-3]>3*flange_thrs ) ) // pet[0] beggining in 0 or 1 and end of last pet unassigned
    {
      ista[0] = ista[npet];
      //npet--;
      if ( trace ) TRACE("Raccordo pet[0] accross zero %4d-%4d\n", ista[0], iend[0]); 
    }

  }
  else // REJECT, TOO MANY PIECES FOUND!
  {

    if ( trace ) TRACE("REJECT MORE THAN MAXNUMPET PIECES FOUND!\n");
    
  }


  if ( trace ) TRACE("END OF SCAN FOUND %d PETALS\n", npet);
  for ( i=0 ; i<npet ; i++ )
  {
    if ( trace ) TRACE("Pet[%d] %4d-%4d (%3d,%3d)-(%3d,%3d)\n", i, ista[i], iend[i], ptx[ista[i]], pty[ista[i]], ptx[iend[i]], pty[iend[i]] );
    Draw_petal( ista[i], iend[i], radius_out );
  }




  if ( trace ) TRACE("Fine scan\n");

  /*  
  // scan results
  for ( i=0 ; i<2*npt ; i++ )
  {
    if ( var>=flange_thrs ) // var above threshold
    {
      if ( status==0 ) // check new petal
      {
        if ( i-iend>mingap ) // check if it's small gap
        {
          ista   = i; // assign start
          status = 1; 
          TRACE("Beginning of block found %4d (%3.1lf)\n", ista, 180.0*i/npt );
        }
        else // continue
        {
          status = 1; 
          TRACE("Continuation of block found %4d (%3.1lf)\n", ista, 180.0*i/npt );
        }
      }

    }
    else // var below threshold
    {
      DRAW_PT( index, x, y, 0xFFAA0000 ); // 0xFFRRGGBB

      if ( status==1 ) // end of petal
      {
        if ( i-ista>mingap ) // check if it's small gap
        {
          iend   =  i; // assign end
          status =  0;
          TRACE("Block found between %4d-%4d (%3.1lf,%3.1lf) - lenght %4d (%3.1lf)\n", ista, iend, 180.0*ista/npt, 180.0*iend/npt, iend-ista, 180.0*(iend-ista)/npt );

          //int ang1 = 180.0*ista/npt;
          //int ang2 = 180.0*iend/npt;

          int rest = radius_out+10;
          int rint = radius_out-10;

          int ind1 = ista*my_circle[rest].n/npt;
          int ind2 = iend*my_circle[rest].n/npt;

          for ( int i2=ind1 ; i2<ind2 ; i2++ )


        }
        else
        {
          status =  0;
          TRACE("Too small - ignore %4d-%4d (%3.1lf,%3.1lf) - lenght %4d (%3.1lf)\n", ista, i, 180.0*ista/npt, 180.0*i/npt, i-ista, 180.0*(i-ista)/npt );
        }

      }
    }
  }
  */

  if ( buffer ) // clear and free memory
  {
    free( buffer );
    buffer = NULL;
  }

  if ( ptx ) // clear and free memory
  {
    free( ptx );
    ptx = NULL;
  }

  if ( pty ) // clear and free memory
  {
    free( pty );
    pty = NULL;
  }

/*
  for ( i=0 ; i<npet ; i++ )
  {
    if ( trace ) TRACE("Pet[%d] %4d-%4d (%3d,%3d)-(%3d,%3d)\n", i, ista[i], iend[i], ptx[ista[i]], pty[ista[i]], ptx[iend[i]], pty[iend[i]] );
    Draw_petal( ista[i], iend[i], radius_out );
  }
  */





  return 0;
}





int CPFStopper_inspection::Variance( int x0, int y0, int hlfw )
{

  ASSERT( x0-hlfw>=0 && x0+hlfw<int(dimx) );
  ASSERT( y0-hlfw>=0 && y0+hlfw<int(dimy) );
  ASSERT( hlfw>=1 && hlfw<30 );

  int x, y;

  int sum  = 0;
  int sum2 = 0;
  int n    = (hlfw*2+1)*(hlfw*2+1); // number of pixels

  ASSERT( n*n*255 < 2E9 );

  for ( y=y0-hlfw ; y<=y0+hlfw ; y++ )
  {
    for ( x=x0-hlfw ; x<=x0+hlfw ; x++ )
    {
      sum  += PT2(x,y);
      sum2 += PT2(x,y)*PT2(x,y);
    }
  }

  return ( sum2-(sum*sum)/n );
}




int CPFStopper_inspection::Variance2( int x0, int y0, int hlfw )
{

  ASSERT( x0-hlfw>=0 && x0+hlfw<int(dimx) );
  ASSERT( y0-hlfw>=0 && y0+hlfw<int(dimy) );
  ASSERT( hlfw>=1 && hlfw<30 );

  int x, y;

  int    m_n = 0;
  double m_oldM, m_newM, m_oldS, m_newS; 

  //ASSERT( n*n*255 < 2E9 );

  m_oldM = m_newM = PT2(x0-hlfw,y0-hlfw);
  m_oldS = 0;

  for ( y=y0-hlfw ; y<=y0+hlfw ; y+=3 )
  {
    for ( x=x0-hlfw ; x<=x0+hlfw ; x+=3 )
    {
      m_n++;
      m_newM = m_oldM + (PT2(x,y) - m_oldM)/m_n;
      m_newS = m_oldS + (PT2(x,y) - m_oldM)*(PT2(x,y) - m_newM);
      // set up for next iteration
      m_oldM = m_newM;
      m_oldS = m_newS;
      //TRACE("%d,%d,%d\n", x, y, PT2(x,y));
    }
  }

  //TRACE("STD=%lf\n", sqrt(m_newS/(m_n-1) ) );

  return ( int( 0.5 + sqrt( m_newS/(m_n - 1) ) ) );
}




int CPFStopper_inspection::Process_image( BYTE* img_acq, UINT nimg )
{
  int x, y;
  int i;

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  //Results_manager* result = NULL;


  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;

  img  = img_acq;

  //if ( nimg>=0 && nimg<60 ) nhead = nimg;
  //else                      nhead = 0;

  SCALE_IMG( index, img_acq );

  CTimer timer;

  int ang = 20;
  int hwd = 10;
  UINT col = 0xFFFF0000; 


  /*
  #ifdef SAVE_FULL_PROC
    image_dsp[index]->scale_fulldsp(img_acq); // contour overlayed
    //for ( UINT i=0 ; i<image_dsp[index]->buf_size ; i++ ) image_dsp[index]->ptr_fulldsp[i] = 0xFF000000;
  #endif
  */

  int dimx = image_dsp[index]->dim_buf.cx;
  int dimy = image_dsp[index]->dim_buf.cy;
  ASSERT( dimx==dimy );

  // Precalc inspection area
  xmin = dimx * 10 / 100;
  xmax = dimx * 90 / 100;
  ymin = dimy * 10 / 100;
  ymax = dimy * 90 / 100;



  // draw inspection area first
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




//  return 0;

//  int step =  30;
//  int lmin =  50;
//  int diam = 134;


//  ri = (diam_inn+1) / 2;
//  ro = (diam_out+1) / 2;

  cstep = 30; // coarse search

  
  // SIMULATION
  #ifdef SIMULATE
  // prepare buffer
  for ( y=ymin ; y<=ymax ; y++ )
  {
    for ( x=xmin ; x<=xmax ; x++ )
    {
      PT2( x, y ) = 200;
    }
  }

  xcnt = 580/2;
  ycnt = 580/2;

  UINT col = 0xFF080808;
  for ( int i=0 ; i<my_circle[ro-2].n ; i++ )
  {
    PT2( xcnt+my_circle[ro-2].x[i], ycnt+my_circle[ro-2].y[i] ) = 20;
    PT2( xcnt-my_circle[ro-2].x[i], ycnt-my_circle[ro-2].y[i] ) = 20;
//    DRAW_PT( index, xcnt+my_circle[ro].x[i], ycnt+my_circle[ro].y[i], col ); // 0xFFRRGGBB
//    DRAW_PT( index, xcnt-my_circle[ro].x[i], ycnt-my_circle[ro].y[i], col ); // 0xFFRRGGBB
  }

  SCALE_IMG( index, img_acq );



  /*
  for ( ang=0 ; ang<360 ; ang +=4 )
  {
    TRACE("- - - - -  ANG=%3d  - - - - - \n", ang);
    for ( int r=3 ; r>=-3 ; r-- )
    {
      int val = Outer_arc( ang, r, hwd );
      TRACE("r=%3d - val=%5d - avg=%3.2lf\n", r, val, double(val)/(2*hwd+1) );
    }
  }
  */

  for ( ang=0 ; ang<360 ; ang +=2 )
  {
    int val = Find_arcedge( ang, 20, -20, hwd );
   
    if ( val > 0 )
    {
      if ( ang<180 )
      {
        int i = int( my_circle[val].n*ang/180.0 );
        DRAW_PT( index, xcnt+my_circle[val].x[i], ycnt+my_circle[val].y[i], 0xFFFF0000 ); // 0xFFRRGGBB
      }
      else
      {
        int i = int( my_circle[val].n*(ang-180)/180.0 );
        DRAW_PT( index, xcnt-my_circle[val].x[i], ycnt-my_circle[val].y[i], 0xFF0000FF ); // 0xFFRRGGBB
      }
   //     TRACE("Find_arcedge = %d\n", val );
    }

  }
  return 0;

  // end of simulation
  #endif





  // ----- COARSE SEARCH OF CENTER ----- 
  for ( x=xmin+cstep ; x<=xmax-cstep ; x+=cstep )
  {
    for ( y=ymin+cstep ; y<=ymax-cstep ; y+=cstep )
    {
      int ltot = PT2( x-cstep, y-cstep ) + PT2( x, y-cstep ) + PT2( x+cstep, y-cstep ) + \
                 PT2( x-cstep, y       ) + PT2( x, y       ) + PT2( x+cstep, y       ) + \
                 PT2( x-cstep, y+cstep ) + PT2( x, y+cstep ) + PT2( x+cstep, y+cstep );

      if ( ltot < lmin_inn*9 )
      {
        //DRAW_PT( index, x, y, 0xFF00FF00 ); // 0xFFRRGGBB
        //TRACE("BREAK ON COARSE SEARCH %d,%d\n",x ,y );
        //break;
        xcnt = x;
        ycnt = y;

        x = xmax + 1;
        y = ymax + 1;
      }
      //else DRAW_PT( index, x, y, 0xFFFF0000 ); // 0xFFRRGGBB
    }
  }

  //TRACE("END OF COARSE SEARCH %d,%d\n",x ,y );


  // ----- FINE SEARCH OF CENTER ----- 
  int x1 = xcnt-radius_inn*2;
  if ( x1<xmin ) x1 = xmin;

  int x2 = xcnt+radius_inn*2;
  if ( x2>xmax ) x2 = xmax;

  int y1 = ycnt-radius_inn*2;
  if ( y1<ymin ) y1 = ymin;

  int y2 = ycnt+radius_inn*2;
  if ( y2>ymax ) y2 = ymax;

  int xpos = 0;
  int ypos = 0;
  int npt  = 0;


  for ( x=x1 ; x<=x2 ; x+=fstep )
  {
    for ( y=y1 ; y<=y2 ; y+=fstep )
    {
      if ( PT2(x,y)<lmin_inn )
      {
        //if ( !(x&7) && !(y&7) ) DRAW_PT( index, x, y, 0xFFFF0000 ); // 0xFFRRGGBB
        //TRACE("BREAK ON COARSE SEARCH %d,%d\n",x ,y );
        //break;
        xpos += x;
        ypos += y;
        npt++;
      }
      //else DRAW_PT( index, x, y, 0xFFFF0000 ); // 0xFFRRGGBB
    }
  }

  if ( npt >= align_minpt )
  {
    xcnt = int( 0.5 + xpos / npt );
    ycnt = int( 0.5 + ypos / npt );
  }
  else
  {
    xcnt = 0;
    ycnt = 0;
  }


  if ( xcnt-radius_out-10<xmin || xcnt+radius_out+10>xmax || ycnt-radius_out-10<ymin || ycnt+radius_out+10>ymax )
  {
    double coef = double(ymax-ymin)/(xmax-xmin);

    for ( x=xmin+20 ; x<=xmax-20 ; x+=2 )
    {
      DRAW_PT( index, x, ymin+int(coef*(x-xmin)), 0xFFAA0000 ); // 0xFFRRGGBB
      DRAW_PT( index, x, ymax-int(coef*(x-xmin)), 0xFFAA0000 ); // 0xFFRRGGBB
    }

    txt1.Format(L"CENTER ALIGNMENT REJECT %d/%d", npt, align_minpt );
    col1 = TXT_DARKRED;
    return 10;
//    xcnt = (xmin+xmax)/2;
//    ycnt = (ymin+ymax)/2;
  }

  txt1.Format(L"Center aligment Good %d/%d", npt, align_minpt );
  col1 = TXT_DARKGREEN;


  col = 0xFF0000FF; // 0xFFRRGGBB

  // Draw alignment INNER circle
  for ( i=0 ; i<my_circle[radius_inn].n ; i+=2 )
  {
    DRAW_PT( index, xcnt+my_circle[radius_inn].x[i], ycnt+my_circle[radius_inn].y[i], col ); // 0xFFRRGGBB
    DRAW_PT( index, xcnt-my_circle[radius_inn].x[i], ycnt-my_circle[radius_inn].y[i], col ); // 0xFFRRGGBB
  } 



  //TRACE("CENTER FOUND IN %d,%d (%d/%d)\n", xcnt, ycnt, npt, align_minpt );


//  radius_out = 148;

  // Draw alignment red cross
  DRAW_PT( index, xcnt  , ycnt  , 0xFFFF0000 ); // 0xFFRRGGBB
  DRAW_PT( index, xcnt-1, ycnt-1, 0xFFFF0000 ); // 0xFFRRGGBB
  DRAW_PT( index, xcnt+1, ycnt-1, 0xFFFF0000 ); // 0xFFRRGGBB
  DRAW_PT( index, xcnt-1, ycnt+1, 0xFFFF0000 ); // 0xFFRRGGBB
  DRAW_PT( index, xcnt+1, ycnt+1, 0xFFFF0000 ); // 0xFFRRGGBB


/*  for ( i=0 ; i<my_circle[radius_out].n ; i+=2 )
  {
    DRAW_PT( index, xcnt+my_circle[radius_out].x[i], ycnt+my_circle[radius_out].y[i], col ); // 0xFFRRGGBB
    DRAW_PT( index, xcnt-my_circle[radius_out].x[i], ycnt-my_circle[radius_out].y[i], col ); // 0xFFRRGGBB
  }  */

  col = 0xFFFF0000;

/*  for ( ang=0 ; ang<360 ; ang +=1 )
  {
    Coords( x, y, ang, radius_out );
    int val = Variance( x, y, 5 );

    if ( val > 9000 ) col = 0xFF00FF00;
    else              col = 0xFFFF0000;

    DRAW_PT( index, x, y, col ); // 0xFFRRGGBB

    //TRACE( "%3d,%3d - var = %6d\n", x, y, val );

  } */

  return Flange_analysis();


  //return 0;


  /* VECCHIA ANALISI A PUNTI ESTERNI 
  // ----- OUTER FLANGE ANALYSIS ----- 

  int rad =  0;


  //
  for ( rad = -20 ; rad <= 40 ; rad +=4 )
  {
    Radial_offset( ang, rad, x, y );
    DRAW_PT( index, x, y, col ); // 0xFFRRGGBB
  }
  //

  DRAW_PT( index, xcnt, ycnt, 0xFFFF0000 ); // 0xFFRRGGBB


  for ( ang=0 ; ang<360 ; ang +=1 )
  {
    int val = Find_arcedge( ang, 20, -20, hwd );
    Insert_average( ang, val );
   
//    if ( val>0 && val>=140 && val<=150 )
    {
//      Draw_marker( ang, val );
/*      if ( ang<180 )
      {
        int i = int( my_circle[val].n*ang/180.0 );
        DRAW_PT( index, xcnt+my_circle[val].x[i], ycnt+my_circle[val].y[i], 0xFFFF00FF ); // 0xFFRRGGBB
      }
      else
      {
        int i = int( my_circle[val].n*(ang-180)/180.0 );
        DRAW_PT( index, xcnt-my_circle[val].x[i], ycnt-my_circle[val].y[i], 0xFFFF00FF ); // 0xFFRRGGBB
      } 
   //     TRACE("Find_arcedge = %d\n", val );
    }


    //if ( result ) result->InsertI( val, ang );

  }

  return 0;
  */

  /*
  ang = nimg;
  Outer_arc( ang, -10, 10 );
  Outer_arc( ang,   0, 10 );
  Outer_arc( ang,  10, 10 );
  */
  /*
  ang = 45;
  Outer_arc( ang, -10, 10 );
  Outer_arc( ang,   0, 10 );
  Outer_arc( ang,  10, 10 );

  ang = 180;
  Outer_arc( ang, -10, 10 );
  Outer_arc( ang,   0, 10 );
  Outer_arc( ang,  10, 10 );

  ang = 356;
  Outer_arc( ang, -10, 10 );
  Outer_arc( ang,   0, 10 );
  Outer_arc( ang,  10, 10 );
  */



  /*
//  for ( i=0 ; i<my_circle[ro].n ; i+=4 )
//  for ( i=0 ; i<1 ; i++ )
//  for ( i=0 ; i<my_circle[ro].n ; i+=4 )
  for ( i=0 ; i<200 ; i+=30 )
  {
    //DRAW_PT( index, xcnt+my_circle[ro].x[i], ycnt+my_circle[ro].y[i], col ); // 0xFFRRGGBB
    //DRAW_PT( index, xcnt-my_circle[ro].x[i], ycnt-my_circle[ro].y[i], col ); // 0xFFRRGGBB
    Outer_edge( xcnt+my_circle[ro].x[i], ycnt+my_circle[ro].y[i] );
  } 
  */

//  Outer_edge( xcnt, ycnt, 20 );
//  Outer_edge( xcnt, ycnt, 40 );



  
//  return 999;





  /*  H/V PROJECTION
  // calculate h/v projs
  int  profx_dim = sizeof(int) * (xmax-xmin+1);
  int* profx = (int*) malloc( profx_dim );
  ZeroMemory( profx, profx_dim );

  int  profy_dim = sizeof(int) * (ymax-ymin+1);
  int* profy = (int*) malloc( profy_dim );
  ZeroMemory( profy, profy_dim );

  for ( x=xmin ; x<=xmax ; x++ )
  {
    for ( y=ymin ; y<=ymax ; y++ )
    {
      profx[x-xmin] += PT2( x, y );
      profy[y-ymin] += PT2( x, y );
    }
  }

  int profminx = 0;
  int profminy = 0;

  int vmax = 580*255*16;

  TRACE("PROFILEX\n");
  for ( x=xmin+3 ; x<=xmax-3 ; x++ )
  {
    int val = profx[x-xmin-3]+2*profx[x-xmin-2]+3*profx[x-xmin-1]+4*profx[x-xmin]+3*profx[x-xmin+1]+2*profx[x-xmin+2]+profx[x-xmin+3];
//    TRACE( "%3d - %7d - %7d\n", x, profx[x-xmin], val );
    if ( val < vmax ) 
    {
      vmax     = val;
      profminx = x; 
    }
//    TRACE( "%3d - %5d - %3.1lf\n", x, profx[x-xmin], double(profx[x-xmin])/(xmax-xmin+1) );
  }

  TRACE("MINIMUM FOR PROFILEX IN X=%d\n", profminx);

  vmax = 580*255*16;



//  TRACE("\n\n");
  TRACE("PROFILEY\n");
  for ( y=ymin+3 ; y<=ymax-3 ; y++ )
  {
    int val = profy[y-ymin-3]+2*profy[y-ymin-2]+3*profy[y-ymin-1]+4*profy[y-ymin]+3*profy[y-ymin+1]+2*profy[y-ymin+2]+profy[y-ymin+3];
//    TRACE( "%3d - %7d - %7d\n", x, profx[x-xmin], val );
    if ( val < vmax ) 
    {
      vmax     = val;
      profminy = y; 
    }
//    TRACE( "%3d - %5d - %3.1lf\n", y, profy[y-ymin], double(profy[y-ymin])/(ymax-ymin+1) );
  }

  TRACE("MINIMUM FOR PROFILEX IN Y=%d\n", profminy);


  free( profx );
  free( profy );

  xcnt = profminx;
  ycnt = profminy;
  */

  /* DRAW INNER AND OUTER CIRCLES

  int i;
  UINT col = 0xFFFF00FF;

  for ( i=0 ; i<my_circle[ri].n ; i+=4 )
  {
    DRAW_PT( index, xcnt+my_circle[ri].x[i], ycnt+my_circle[ri].y[i], col ); // 0xFFRRGGBB
    DRAW_PT( index, xcnt-my_circle[ri].x[i], ycnt-my_circle[ri].y[i], col ); // 0xFFRRGGBB
  } 


  for ( i=0 ; i<my_circle[ro].n ; i+=4 )
  {
    DRAW_PT( index, xcnt+my_circle[ro].x[i], ycnt+my_circle[ro].y[i], col ); // 0xFFRRGGBB
    DRAW_PT( index, xcnt-my_circle[ro].x[i], ycnt-my_circle[ro].y[i], col ); // 0xFFRRGGBB
  } 

  */

//  return 999; // no inspection mode selected

  /*
  x = (xmin+xmax)/2;
  y = (ymin+ymax)/2;

  int v1, v2, v3, v4;
  
  int cycle = 1;
  int ncycles = 0;
  
  while ( cycle ) 
  {

    Cross( x, y, v1, v2, v3, v4 );
    //TRACE( "(%d,%d) - %d,%d,%d,%d\n", x-inn_diam/2, y-inn_diam/2, v1, v2, v3, v4 );
    TRACE( "(%d,%d)-(%d,%d) - %d,%d,%d,%d - %d cycles\n",x ,y ,x-inn_diam/2, y-inn_diam/2, v1, v2, v3, v4, ncycles );

    cycle = 0;
  
    if ( v1>105*v4/100 ) 
    {
      x -= 2;
      y -= 2;
      cycle = 1;
      TRACE( "Moving V4\n" );
    }
    else
    {
      if ( v4>105*v1/100 ) 
      {
        x += 2;
        y += 2;
        cycle = 1;
        TRACE( "Moving V1\n" );
      }
      else
      {
        if ( v2>105*v3/100 ) 
        {
          x -= 2;
          y += 2;
          cycle = 1;
          TRACE( "Moving V3\n" );
        }
        else
        {
          if ( v3>105*v2/100 ) 
          {
            x += 2;
            y -= 2;
            cycle = 1;
            TRACE( "Moving V2\n" );
          }
        }
      }
    }

    ncycles++;


  };

  TRACE( "COARSE END (%d,%d)-(%d,%d) - %d,%d,%d,%d - %d cycles\n",x ,y ,x-inn_diam/2, y-inn_diam/2, v1, v2, v3, v4, ncycles );


  int i;
  UINT col = 0xFFFF00FF;


  /*
  for ( i=0 ; i<my_circle[ri].n ; i++ )
  {
    DRAW_PT( index, x+my_circle[ri].x[i], y+my_circle[ri].y[i], col ); // 0xFFRRGGBB
    DRAW_PT( index, x-my_circle[ri].x[i], y-my_circle[ri].y[i], col ); // 0xFFRRGGBB
  }
  */

  /*
// fine search
  int xc = x; // coarse search coords
  int yc = y; 

//  lum_int = 40;

  int ltot = 0;
/*  for ( i=0 ; i<my_circle[ri].n ; i++ )
  {
    ltot += PT2(x+my_circle[ri].x[i],y+my_circle[ri].y[i]);
    ltot += PT2(x-my_circle[ri].x[i],y-my_circle[ri].y[i]);
//    DRAW_PT( index, x+my_circle[ri].x[i], y+my_circle[ri].y[i], col ); // 0xFFRRGGBB
//    DRAW_PT( index, x-my_circle[ri].x[i], y-my_circle[ri].y[i], col ); // 0xFFRRGGBB
  } */
  /*
  int vmin = int(1E9);

  // search for min in the surrounding starting from <xc,yc,vmin>
  for ( y=yc-2 ; y<=yc+2 ; y++ )
  {
    for ( x=xc-2 ; x<=xc+2 ; x++ )
    {
      ltot = 0;
      for ( i=0 ; i<my_circle[ri].n ; i++ )
      {
        ltot += PT2(x+my_circle[ri].x[i],y+my_circle[ri].y[i]);
        ltot += PT2(x-my_circle[ri].x[i],y-my_circle[ri].y[i]);
      }

      if ( ltot<vmin ) 
      {
        vmin = ltot;
        xcnt = x;
        ycnt = y;
      } 

//      if ( x=xc && y==yc ) break;
//      TRACE("Fine %d,%d\n",x,y);

      //TRACE("Fine (%d,%d) %d\n",x,y,ltot);


    }

  }


  TRACE("\nFine completed (%d,%d)-(%d,%d) %d\n",xcnt,ycnt,xcnt-inn_diam/2,ycnt-inn_diam/2,vmin);
  */






//  npt_tot += 2*my_circle[r].n;



 // Allocate_serie( &vial, dimx );

//  timer.reset();

 /* #if defined DOUBLE_BOTTOM_ANALYSIS // for F40 machines when double bottom is defined (for now only on B_A camera)
    if ( index==3 ) return( Bottom_double_analysis() ); // includes also double alignment
    if ( index==4 ) return( Bottom_double_analysis() ); // includes also double alignment
  #endif


  if ( !Search_center() ) // if alignment succeded
  {
    /*#ifdef NEWCAP_ORALCAP
      if ( index==5 ) return( Bottom_analysis_circle() );
    #endif */
/*    #if defined NEWCAP_ORALCAP && defined TIVS_F40 // for F40 machines
      if ( index==5 ) return( Bottom_analysis_circle() );
    #endif

    #if defined NEWCAP_ORALCAP && defined TIVS_A40 // for A40 machines
      if ( index==7 ) return( Bottom_analysis_circle() );
    #endif

    return( Bottom_analysis() );
   }
   */

//  TRACE("max (%d,%d) completed in %d - average %d/each us\n", xmx, ymx, timer.elapsed_us(), timer.elapsed_us()/((2*amp+1)*(2*amp+1)) );

//  return 999; // no inspection mode selected
}










