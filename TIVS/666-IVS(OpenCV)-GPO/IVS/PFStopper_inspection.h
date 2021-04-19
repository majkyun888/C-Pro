

#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"


//#undef   PT
#define  PT2(x,y) img[x+(y)*camera[index].width]


class CPFStopper_inspection
{

//	DECLARE_DYNAMIC(CPFStopper_inspection)

public:
  CPFStopper_inspection( UINT ind, int mode_in, CFile_mngmt* file_manager_in=NULL );
  ~CPFStopper_inspection(void);

  int Add_menu_parameters( CContainer_wnd* child );

  int Process_image    ( BYTE* img_acq, UINT nimg );

//  int Bottom_analysis( void );
//  int Bottom_analysis_circle( void );
//  int Bottom_double_analysis( void );
//  UINT sectbrgth( int xc, int yc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius
//  UINT sectbrgth( int xc, int yc, int rc, int side ); // 0=right 1=bottom 2=left 3=top - uses current radius

  static int Set_inspection_area( int ind );


  // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text

  BOOL    trace;   // if TRUE enable tracing of processing

//  int radius; // radius used for alignment
  int xcnt;   // calculated x of center
  int ycnt;   // calculated y of center


/*
  // parameters/functions involved into search of second (inner cake) center
  int Search_center2( void );
  int xcnt2;   // calculated x of second (inner cake) center
  int ycnt2;   // calculated y of second (inner cake) center
  int src_radius2;   // radius used for alignment search (parameter from GUI)
  int src_radtmp2;   // temporary used during seach process
  // sectbrgth scores in current and other four directions
//  int rv[5]; // 0=center - 1=right - 2=bottom - 3=left - 4=top
//  int bv[5];
//  int lv[5];
//  int tv[5];
//  double score[5]; // overall rating for each position
  int Search_center( void );
  int Calculate_scores( int x, int y );
  int Calculate_scores( int x, int y, int r );
  //  int Calculate_score( int ind );  */

  int Cross( int x, int y, int& v1, int& v2, int& v3, int& v4  );
  int Outer_box ( int x, int y );
  //int Outer_edge( int x, int y, int ang );
  int Radial_offset  ( int ang, int rad, int& x, int& y );
  int Outer_arc      ( int ang, int rad, int hwd );
  int Find_arcedge   ( int ang, int rsta, int rend, int hwd );
  int Insert_average ( int ang, int val );
  int Draw_marker    ( int ang, int rad, UINT col=0xFF00FF00 );
  int Draw_point     ( int ang, int rad, UINT col=0xFF00FF00 );
  int Draw_index     ( int ind, int rad, UINT col=0xFF00FF00 );
  int Draw_petal     ( int ista, int iend, int rad, UINT col=0xFF00FF00 );
  int Variance       ( int x0, int y0, int hlfw );
  int Variance2      ( int x0, int y0, int hlfw );

  int Coords         ( int &x, int &y, int ang, int rad );
  int Flange_analysis( void );


  CFile_mngmt*  file_manager;

  CArea_adjust* area;   // inspection/search area

  BYTE*   img;          // pointer to image to process

  int     nhead;        // used for data collection

  int     index;        // index of camera/image to be bound to
//  int mode;           // TIP_INSPECTION for AMPOULES - CAP_INSPECTION for VIALS

  // inspection parameters
  //int     diam_inn;           // diameter of rubber stopper
  //int     diam_out;           // diameter of outer flange
  BYTE    lmin_inn;           // brightness threshold for inner alignment

  int     radius_inn;                 // inner radius
  int     radius_out;                 // outer radius

  int     cstep;              // step for coarse search
  int     fstep;              // step for fine   search
  int     align_minpt;        // min number of points for fine alignment



  // flange analysis
//  int     flange_avgl;        // moving average lenght for finding 2 maxes (L+1+L)
//  int     flange_rmin;        // minimum allowed flange diameter
//  int     flange_rmax;        // maximum allowed flange diameter
  int     flange_thrs;        // threshold to validate flange petal (9000)

//  int     flange_minang;      // minimum allowed petal angle
//  int     flange_maxang;      // maximum allowed petal angle

  int     petal_minlen;       // minimum lenght of petals


  /*
  int     avg_max;      // maximum allowed average brightness
  int     avg_min;      // minimum allowed average brightness

  int     pos_dif;      // positive threshold brightness difference (calculated on average circle)
  int     neg_dif;      // negative threshold brightness difference (calculated on average circle)
  int     max_lum;      // maximum absolute brightness 0-255
  int     min_lum;      // minimum absolute brightness 0-255

  int     maxerr_pos;   // maximum allowed error for positive points (relative to average)
  int     maxerr_neg;   // maximum allowed error for negative points (relative to average)
  int     maxerr_max;   // maximum allowed error for absolute maximum brightness
  int     maxerr_min;   // maximum allowed error for absolute minimum brightness

  int     err_pos;      // negative difference accumulated error
  int     err_neg;      // positive difference accumulated error
  int     err_max;      // over maximum total accumulated error
  int     err_min;      // below minimum total accumulated error

  int     src_radius;   // radius used for alignment search (parameter from GUI)
  int     anl_radius;   // radius used for analysis

  int     src_radtmp;   // temporary used during seach process
  int     anl_radtmp;   // temporary used during analysis process
  */
  /*
  #if defined DOUBLE_BOTTOM_RING || defined DOUBLE_BOTTOM_ANALYSIS // If external ring analysis required
    int     min2_radius;   // start radius for second analysis
    int     max2_radius;   // end   radius for second analysis
    int     avg2_max;      // maximum allowed average brightness
    int     avg2_min;      // minimum allowed average brightness
    int     pos2_dif;      // positive threshold brightness difference (calculated on average circle)
    int     neg2_dif;      // negative threshold brightness difference (calculated on average circle)
    int     max2_lum;      // maximum absolute brightness 0-255
    int     min2_lum;      // minimum absolute brightness 0-255
    int     maxerr2_pos;   // maximum allowed error for positive points (relative to average)
    int     maxerr2_neg;   // maximum allowed error for negative points (relative to average)
    int     maxerr2_max;   // maximum allowed error for absolute maximum brightness
    int     maxerr2_min;   // maximum allowed error for absolute minimum brightness
    int     err2_pos;      // negative difference accumulated error
    int     err2_neg;      // positive difference accumulated error
    int     err2_max;      // over maximum total accumulated error
    int     err2_min;      // below minimum total accumulated error

    int     min2_radtmp;   // temporary used during analysis process
    int     max2_radtmp;   // temporary used during analysis process
  #endif
  */




  
  
//  int min_npt;        // minimum number of points to validate found profile
//  int htot_max;       // maximum acceptable height
//  int htot_min;       // minimum acceptable height
//  int maxerr_symm;   // maximum allowed error for symmetry      ( 0=disabled )
//  int maxerr_curv;   // maximum allowed error for top curvature ( 0=disabled )
//  double scalefct;
//  double tip_diameter;  // nominal diameter of tip top radius
//  int  curv_err;
//  int  bulge_err;

//  int  dim, dimx, dimy;
  UINT dim, dimx, dimy;

  UINT area_color;

};




/*

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



  }

  return 0;
}


int CPFStopper_inspection::Flange_analysis()
{
  int npt = circle[radius_out].n;
  int i;
  int x, y;
  UINT col;

  for ( i=0 ; i<npt ; i++ )
  {
    x = xcnt+circle[radius_out].x[i];
    y = ycnt+circle[radius_out].y[i];

    //Coords( x, y, ang, radius_out );
    int val = Variance( x, y, 5 );

    if ( val > 9000 ) col = 0xFF00FF00;
    else              col = 0xFFFF0000;

    DRAW_PT( index, x, y, col ); // 0xFFRRGGBB
    //TRACE( "%3d,%3d - var = %6d\n", x, y, val );

  } 

  for ( i=0 ; i<npt ; i++ )
  {
    x = xcnt-circle[radius_out].x[i];
    y = ycnt-circle[radius_out].y[i];

    //Coords( x, y, ang, radius_out );
    int val = Variance( x, y, 5 );

    if ( val > 9000 ) col = 0xFF00FF00;
    else              col = 0xFFFF0000;

    DRAW_PT( index, x, y, col ); // 0xFFRRGGBB
    //TRACE( "%3d,%3d - var = %6d\n", x, y, val );

  } 

*/