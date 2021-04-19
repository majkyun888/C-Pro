#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
#include "LQcolor_inspection.h"


extern Camera_struct    camera  [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern circle_type      circle[MAXR_CIRCLES];
extern UINT             thread_status;
//extern BOOL             learning; // TRUE / FALSE for all controls

extern BOOL             datarec_on;
extern Results_manager* cap_result;

#define  xmin    area->buffrect.left  
#define  xmax    area->buffrect.right 
#define  ymin    area->buffrect.top   
#define  ymax    area->buffrect.bottom


#define PTR(x,y) img[3*(x)+(y)*3*dimx  ]  // red   component
#define PTG(x,y) img[3*(x)+(y)*3*dimx+1]  // green component
#define PTB(x,y) img[3*(x)+(y)*3*dimx+2]  // blue  component



CLQcolor_inspection::CLQcolor_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy *3;   // dim in bytes (x3) !

  area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
//  area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 ); //&& (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =      8; //  dimy/6;
  area->buffrect.bottom = dimy-8; // 5*dimy/6;
  area->buffrect.left   =      8; // dimx/6;
  area->buffrect.right  = dimx-8; // 5*dimx/6;

  area->buffrect.top    =    168;
  area->buffrect.bottom =    364;
  area->buffrect.left   =     16;
  area->buffrect.right  =    168; 

  max_redness = 10;

  /*
  ZeroMemory( Lab_ref, sizeof(Lab_ref) );

  src_radius    =  175;
  src_error     =   75;
  
  anl_radius    =  170;

  col_diff      =  150;

  learning_fo   = FALSE;
  */

//  lum_nthr      =   20;

  /*
  maxerr_lab    =    4; // max average color difference ( Lab coordinates )
  maxerr_col    = 1000;

  // basic RGB colors for alignment
  ref_col[0]    =   31; // red
  ref_col[1]    =   65; // green
  ref_col[2]    =  156; // blue
  */

  /*
  path = (BYTE*) malloc(dimx*dimy);
  ASSERT( path && AfxIsValidAddress(path,dimx*dimy) );
  ZeroMemory( path, dimx*dimy );
  */
}


CLQcolor_inspection::~CLQcolor_inspection(void)
{

/*  if ( path )
  {
    delete path;
    path = NULL;
  }  */

  if ( area )
  {
    delete area;
    area = NULL;
  }
}




int CLQcolor_inspection::Set_inspection_area( int ind )
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

  TRACE("CLQcolor_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CLQcolor_inspection::Add_menu_parameters( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2700[lng], child );
  file_manager->Add_separator( string_2400[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


/*  
  pView->parameter[pView->num_params++] = new CParameter( &src_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2766[lng], string_pix [lng],  175,    0,   250, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &src_error,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2800[lng], string_pix [lng],   75,    0,   250, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &anl_radius,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2770[lng], string_pix [lng],  105,    0,   250, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &col_diff,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2414[lng], string_unit[lng],   50,    0,   900, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &maxerr_col,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2412[lng], string_unit[lng],  100,    0,999000, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &maxerr_lab,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2410[lng], string_unit[lng],  100,    0,  1000, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &learning_fo,   CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2408[lng], 0, 0, 0, 1, NULL );
*/

  pView->parameter[pView->num_params++] = new CParameter( &max_redness,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2704[lng], string_unit[lng],  100,   0, 9999, file_manager );


  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

//  file_manager->Add_data( Lab_ref, sizeof(Lab_ref) );
//  file_manager->Add_data( ref_col, sizeof(ref_col) );

  return 0; // completed successfully
}





int CLQcolor_inspection::Process_image( CGrabResultPtr ptr_gr, UINT nimg )
{
  image_dsp[index]->scale_grabresult( ptr_gr ); // scale for display and prepare RGB buffer from BAYER data
  return Process_image( image_dsp[index]->ptr_buf, nimg );
}



//int CLQcolor_inspection::Process_image( BYTE* img_acq, UINT nimg )
int CLQcolor_inspection::Process_image( BYTE* ptr_in, UINT nimg )
{
  int x, y;

  ASSERT( AfxIsValidAddress( ptr_in, dim ) );
  img = ptr_in;

  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;

//  img  = img_acq;
//  SCALE_IMG( index, img_acq );
//  CTimer timer;

  npt  = 0;
  totr = 0;
  totg = 0;
  totb = 0;

  // initial search coords
//  xcnt = (xmin+xmax)/2;
//  ycnt = (ymin+ymax)/2;

  //srcrad = src_radius; // lock temporary radius for analysis

  UINT color = 0xFF0000FF; // 0xAARRGGBB

  for ( y=ymin ; y<=ymax ; y+=3 )
  {
    DRAW_PT(index,xmin,y   ,color);
    DRAW_PT(index,xmax,y   ,color);
  }

  for ( x=xmin ; x<=xmax ; x+=3 )
  {
    DRAW_PT(index,x,ymin,color);
    DRAW_PT(index,x,ymax,color);
  }

  BYTE* ofs;

  for ( y=ymin ; y<=ymax ; y++ )
  {
    for ( x=xmin ; x<=xmax ; x++ )
    {
      ofs   = img + 3*( x + y*dimx );
      totr += *ofs++;
      totg += *ofs++;
      totb += *ofs;
//      DRAW_PT(index,x,y,color);
    }
  }

  npt = (xmax-xmin+1)*(ymax-ymin+1);

  Calculate_average();

  redness = int( 0.5 + a * 10.0 );


  if ( redness > max_redness )
  {
    //txt2.Format( L"Lab = %3.2lf %3.2lf %3.2lf", L, a, b );
    txt2.Format( string_2706[lng], redness, max_redness );
    col2 = TXT_DARKRED;
    return 99; //
  }
  else
  {
    txt2.Format( string_2708[lng], redness, max_redness );
    col2 = TXT_DARKGREEN;
    return 0; // 
  }


  //learning = TRUE;
  /*
  if ( learning_fo ) // Learning - Use center coordinates
  {
    int   r, i;
    BYTE* ofs;
    for ( r=10 ; r<anl_radius-30 ; r++ ) 
    {
      for ( i=0 ; i<circle[r].n ; i++ )
      {
        ofs   = img + 3*( xcnt+circle[r].x[i] + (ycnt+circle[r].y[i])*dimx );
        totr += *ofs++;
        totg += *ofs++;
        totb += *ofs;

        ofs   = img + 3*( xcnt-circle[r].x[i] + (ycnt-circle[r].y[i])*dimx );
        totr += *ofs++;
        totg += *ofs++;
        totb += *ofs;
      }
      npt += 2*i;
    }

    Calculate_average();

    UINT color_ref = 0xFF000000 | ref_col[0]<<16 | ref_col[1]<<8 | ref_col[2];

    for ( r=10 ; r<anl_radius-30 ; r++ ) for ( i=0 ; i<circle[r].n ; i++ )
    {
      DRAW_PT(index,xcnt+circle[r].x[i],ycnt+circle[r].y[i],color_ref);
      DRAW_PT(index,xcnt-circle[r].x[i],ycnt-circle[r].y[i],color_ref);
    }

    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -50, 6 ); // insert -50 into 6 cat 
    #endif

    learning_fo = FALSE;
    return 0;
  }
  */

  // NORMAL PROCESSING
  /*
  // First search alignment
  if ( Search_center() )
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -100, 6 ); // insert -100 into 6 cat 
    #endif
    return 99; // search failed
  }

  // Then analyze inner part
  if ( Analyze_center() ) 
  {
    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -120, 6 ); // insert -120 into 6 cat 
    #endif
    return 199; // analysis failed
  }
  */

  /*
  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, 6 ); // insert 0 into 6 cat = GOOD / ACCEPTED
  #endif
  */

  return 0;
}


/*
int CLQcolor_inspection::Analyze_center( void )
{
  BYTE* ofs;
  int   r, i;
  int err = 0;
  err_col = 0;

  if ( xcnt-anl_radius<2 || xcnt+anl_radius>dimx-2 || ycnt-anl_radius<2 || ycnt>dimy-2 ) return 1999; // center not allowing inspection

  for ( r=1 ; r<anl_radius ; r++ ) 
  {
    for ( i=0 ; i<circle[r].n ; i++ )
    {
      ofs   = img + 3*( xcnt+circle[r].x[i] + (ycnt+circle[r].y[i])*dimx );
      err   = abs( *ofs - ref_col[0] );
      totr += *ofs++;
      err  += abs( *ofs - ref_col[1] );
      totg += *ofs++;
      err  += abs( *ofs - ref_col[2] );
      totb += *ofs;

      if ( err>col_diff )
      {
        err_col += err-col_diff;
        DRAW_PT(index,xcnt+circle[r].x[i],ycnt+circle[r].y[i],COL_RED);
      }

      ofs   = img + 3*( xcnt-circle[r].x[i] + (ycnt-circle[r].y[i])*dimx );
      err   = abs( *ofs - ref_col[0] );
      totr += *ofs++;
      err  += abs( *ofs - ref_col[1] );
      totg += *ofs++;
      err  += abs( *ofs - ref_col[2] );
      totb += *ofs;

      if ( err>col_diff )
      {
        err_col += err-col_diff;
        DRAW_PT(index,xcnt-circle[r].x[i],ycnt-circle[r].y[i],COL_RED);
      }

    }
    npt += 2*i;
  }

/*  #ifdef FD_DATAREC
    if ( datarec_on && cap_result )
    {
      //cap_result->InsertI( maxerr_col, 20 ); // insert spot error into cat 20
      cap_result->InsertI( -100, 6 );        // insert -100 into cat 6 (reject because of spot error inside flipoff)
    }
  #endif */

/*
  // SPOT ANALYSIS
  if ( maxerr_col>0 )
  {
    if ( err_col>maxerr_col )
    {
      txt2.Format( string_2420[lng], err_col, maxerr_col );
      return 99; // result of inspection REJECT
    }
    else
    {
      txt2.Format( string_2422[lng], err_col, maxerr_col );
      col2 = TXT_DARKGREEN;
      //return 0; // result of inspection GOOD
    }
  }
  else
  {
    txt2.Format( string_2424[lng] );
    col2 = 0;
  }

 /* #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) 
    {
      //cap_result->InsertI( maxerr_lab, 21 ); // insert color error into cat 21
      cap_result->InsertI( -120, 6 );        // insert -120 into cat 6 (reject because of color error inside flipoff)
    }
  #endif */
/*
  // COLOR ANALYSIS
  if ( maxerr_lab>0 )
  {
    CString txt;
    dist_lab = 0.0;
    Calculate_average();

    if ( dist_lab>maxerr_lab )
    {
      txt.Format( string_2404[lng], L, a, b, dist_lab );
      txt2.Append( txt );
      col2 = TXT_DARKRED;
      return 99; // result of inspection REJECT
    }
    else
    {
      txt.Format( string_2406[lng], L, a, b, dist_lab );
      txt2.Append( txt );
      col2 = TXT_DARKGREEN;
      return 0; // result of inspection GOOD
    }
  }
  else
  {
    txt2.Append( string_2426[lng] );
  }

  return 0;
}

*/

/*  // draw inspection area first
  for ( x=xmin ; x<=xmax ; x+=4 )
  {
    DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
  }
  for ( y=ymin ; y<=ymax ; y+=4 )
  {
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
  } */
  
  /*
  int dimx = image_dsp[index]->dim_buf.cx;
  int dimy = image_dsp[index]->dim_buf.cy;
  ASSERT( dimx==dimy );
  npt_tot = 0;
  int delta = 25;
  min_col[0] = ref_col[0]>    delta ? ref_col[0]-delta :   0;
  max_col[0] = ref_col[0]<255-delta ? ref_col[0]+delta : 255;
  min_col[1] = ref_col[1]>    delta ? ref_col[1]-delta :   0;
  max_col[1] = ref_col[1]<255-delta ? ref_col[1]+delta : 255;
  min_col[2] = ref_col[2]>    delta ? ref_col[2]-delta :   0;
  max_col[2] = ref_col[2]<255-delta ? ref_col[2]+delta : 255;
  */

//  return ( Calculate_average() );
//  return 1;
//  Calculate_blob();
//  txt1.Format( L"Completed %d points - proc time %d us", npt_tot, timer.elapsed_us() );
//  TRACE("max (%d,%d) completed in %d - average %d/each us\n", xmx, ymx, timer.elapsed_us(), timer.elapsed_us()/((2*amp+1)*(2*amp+1)) );
//  return 999; // no inspection mode selected




double CLQcolor_inspection::funz_t( double in )
{
  if ( in > 0.008856 ) return( pow( in , 1.0/3.0 ) );
  else return( 7.787 * in + 16-0 / 116.0 );
}

int CLQcolor_inspection::Calculate_average( void )
{
/*  int x, y;
  totr = 0;
  totg = 0;
  totb = 0;
  
  for ( y=ymin ; y<=ymax ; y++ ) for ( x=xmin ; x<=xmax ; x++ )
  {
    BYTE* ptr = img + 3*y*dimx;
    for ( x=3*xmin ; x<=3*xmax ; x+=3 )
    {
      totr += ptr[x  ];
      totg += ptr[x+1];
      totb += ptr[x+2];
    }
  }

  npt = (ymax-ymin+1)*(xmax-xmin+1); */

  double R = double(totr)/(255.0*npt);
  double G = double(totg)/(255.0*npt);
  double B = double(totb)/(255.0*npt);

  double X = 0.412453*R + 0.357580*G + 0.180423*B;
  double Y = 0.212671*R + 0.715160*G + 0.072169*B;
  double Z = 0.019334*R + 0.119193*G + 0.950227*B;

  //TRACE("Average RGB %.1lf,%.1lf,%.1lf\n", double(totr)/x, double(totg)/x, double(totb)/x );

  X = X/0.950456;
  Z = Z/1.088754;

  L = 0.0;
  if ( Y>0.008856 ) L = 116.0*pow(Y,1.0/3.0)-16.0;
  else              L = 903.3*Y;

  a = 500.0 * ( funz_t(X) - funz_t(Y) );
  b = 200.0 * ( funz_t(Y) - funz_t(Z) );

  return 0;

/*  if ( learning_fo ) // learning active
  {
    ref_col[0] = int( 0.5 + totr/npt ); // new reference RED   component
    ref_col[1] = int( 0.5 + totg/npt ); // new reference GREEN component
    ref_col[2] = int( 0.5 + totb/npt ); // new reference BLUE  component
    ASSERT( ref_col[0]>=0 && ref_col[0]<=255 );
    ASSERT( ref_col[1]>=0 && ref_col[1]<=255 );
    ASSERT( ref_col[2]>=0 && ref_col[2]<=255 );
 
    Lab_ref[0] = L; // new reference L* component
    Lab_ref[1] = a; // new reference a* component
    Lab_ref[2] = b; // new reference b* component

    txt1.Format( string_2402[lng], L, a, b, ref_col[0], ref_col[1], ref_col[2] );
    col1 = 0;
    return 0;
  } */


  /*
  dist_lab = (L-Lab_ref[0])*(L-Lab_ref[0]) + (a-Lab_ref[1])*(a-Lab_ref[1]) + (b-Lab_ref[2])*(b-Lab_ref[2]);

  if ( dist_lab > 0.0 ) dist_lab = sqrt( dist_lab );
  else                  dist_lab = 0.0;
  return 0;
  */

  /*
  else
  {
    double dist_lab = (L-Lab_ref[0])*(L-Lab_ref[0]) + (a-Lab_ref[1])*(a-Lab_ref[1]) + (b-Lab_ref[2])*(b-Lab_ref[2]);

    if ( dist_lab > 0.0 ) dist_lab = sqrt( dist_lab );
    else                  dist_lab = 0.0;

    if ( dist_lab>maxerr_lab )
    {
      txt1.Format( string_2404[lng], L, a, b, dist_lab );
      return 99; // result of inspection REJECT
    }
    else
    {
      txt1.Format( string_2406[lng], L, a, b, dist_lab );
      col1 = TXT_DARKGREEN;
      return 0; // result of inspection GOOD
    }
    //txt1.Format( L"RGB (%.3lf,%.3lf,%.3lf) - L*a*b* (%.1lf,%.1lf,%.1lf)", R, G, B, L, a, b );
  }
  */


/*  R =  3.240479*X - 1.537150*Y - 0.498535*Z;
  G = -0.969256*X + 1.875991*Y + 0.041556*Z;
  B =  0.055648*X - 0.204043*Y + 1.057311*Z; */

  /*
L* = 116 * (Y/Yn)1/3 - 16    for Y/Yn > 0.008856
L* = 903.3 * Y/Yn             otherwise

a* = 500 * ( f(X/Xn) - f(Y/Yn) )
b* = 200 * ( f(Y/Yn) - f(Z/Zn) )
    where f(t) = t1/3      for t > 0.008856
          f(t) = 7.787 * t + 16/116    otherwise  */

//  txt1.Format( L"RGB (%.3lf,%.3lf,%.3lf) - XYZ (%.3lf,%.3lf,%.3lf)", R, G, B, X, Y, Z );


/*  double K = ( 0.299*totr + 0.587*totg + 0.114*totb ) / x;

  int    Y = DOUBLETOINT( 0.859 *         K  ) +  16;
  int    U = DOUBLETOINT( 0.496 * (totb/x-K) ) + 128;
  int    V = DOUBLETOINT( 0.627 * (totr/x-K) ) + 128;

  totr = (totr+(x>>1))/x;
  totg = (totg+(x>>1))/x;
  totb = (totb+(x>>1))/x; */

  /*
  K = 0.299 * R + 0.587 * G + 0.114 * B 
  Y = Round (0.859 * K) + 16 
  U = Round (0.496 * (B - K)) + 128 
  V = Round (0.627 * (R - K)) + 128
  */


/* [ X ]   [  0.412453  0.357580  0.180423 ]   [ R ]
   [ Y ] = [  0.212671  0.715160  0.072169 ] * [ G ]
   [ Z ]   [  0.019334  0.119193  0.950227 ]   [ B ]   */

//  txt1.Format( L"RGB (%d,%d,%d) - YUV (%d,%d,%d)", totr, totg, totb, Y, U, V );


}




/*

//#define TEST_BLOB(x,y)  PTB(x,y)>200
#define TEST_BLOB(x,y)  ( PTR(x,y)>min_col[0] && PTR(x,y)<max_col[0] && \
                          PTG(x,y)>min_col[1] && PTG(x,y)<max_col[1] && \
                          PTB(x,y)>min_col[2] && PTB(x,y)<max_col[2] )

#define BLOCCO_BLOB( ox , oy )  if ( TEST_BLOB(ox,oy) )          { \
                                   dafx[end] = ox;                 \
                                   dafy[end] = oy;                 \
                                   PTR(dafx[end],dafy[end]) = 0;   \
                                   PTG(dafx[end],dafy[end]) = 0;   \
                                   PTB(dafx[end],dafy[end]) = 0;   \
                                   end++;                        }


int CLQcolor_inspection::Calculate_blob( void )
{
  ASSERT( 2<=xmin && xmin<xmax && xmax<=dimx-2 );
  ASSERT( 2<=ymin && ymin<ymax && ymax<=dimy-2 );

  #define MAX_DIM  10000 // Massimo numero di pixel contati ( 4096 )

  CString msg;
  int x, y, i=0;
  int dafx[MAX_DIM+2], dafy[MAX_DIM+2];
  int start, end;

  // Se serve azzero la lista dei blob
  start = 0;
  end   = 0;

  
  for ( x=xmin ; x<=xmax ; x++ )
  {
    PTR(x,ymin)=0;
    PTG(x,ymin)=0;
    PTB(x,ymin)=0;
    PTR(x,ymax)=0;
    PTG(x,ymax)=0;
    PTB(x,ymax)=0;
  }
  for ( y=ymin ; y<=ymax ; y++ )
  {
    PTR(xmin,y)=0;
    PTG(xmin,y)=0;
    PTB(xmin,y)=0;
    PTR(xmax,y)=0;
    PTG(xmax,y)=0;
    PTB(xmax,y)=0;
  }
  
  for ( y=ymin ; y<=ymax ; y+=10 ) for ( x=xmin ; x<=xmax ; x+=10 )
  {
    if ( TEST_BLOB(x,y) )
    {
        dafx[0]  = x;
        dafy[0]  = y;
        end      = 1;
        //TRACE("PT(%3d,%3d)=(%3d,%3d,%3d)\n", x, y, PTR(x,y), PTG(x,y), PTB(x,y) );
        PTR(x,y) = 0;
        PTG(x,y) = 0;
        PTB(x,y) = 0;
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finchè ci sono punti in lista da fare          
        {
           //Controllo se il punto che sto esaminando ha vicini da marcare
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

		ASSERT( end>0 && end<=MAX_DIM+2 );
        if (end<BLOB_HISTO_DIM) blob_histo[end-1]++;
        else                    blob_histo[BLOB_HISTO_DIM]++;

        //TRACE("new blob %5d big\n", i);
        npt_tot += i;

        for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  COL_RED );  //


        // IN QUESTO PUNTO end è la dimensione del blob
        // Visualizzazione del blob
        if ( end < blob_score[0].size ) // TOO SMALL BLOB - GREEN/NO ERROR SCORE
        {
          for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  0xFF00FF00 );  // GREEN - NO ERROR
        }
        else    // BLOB BIG ENOUGH TO ADD SOME ERROR SCORE
        {

          if ( end < blob_score[1].size ) // 2nd SMALLEST - YELLOW + SCORE[0]
          {
            part_err += blob_score[0].score;
            for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  0xFFFFFF00 );  // YELLOW - SCORE[0]
          }
          else 
          {
            if ( end < blob_score[2].size ) // 3rd SMALLEST - LIGHT ORANGE + SCORE[1]
            {
              part_err += blob_score[1].score;
              for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  0xFFFD9F1A );  // LIGHT ORANGE - SCORE[1]
            }
            else                            
            {
              if ( end < blob_score[3].size ) // 4th SMALLEST - ORANGE + SCORE[2]
              {
                part_err += blob_score[2].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  0xFFFF6600 );  // ORANGE - SCORE[2]
              }
              else                            // BIGGEST - RED + SCORE[3]
              {
                part_err += blob_score[3].score;// + end;
                for ( i = 0 ; i < end ; i++ ) DRAW_PT( index, dafx[i], dafy[i],  0xFFFF0000 );  // RED - SCORE[3]
              }
            }
          } 
       
        }  //BLOG BIG ENOUGH
        
        //TRACE("    end=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", end,     nblob0, nblob1, nblob2, nblob3, part_err );
      } // if TEST
      start = 0;
      end   = 0; 

    }  // ciclo for
   
  //TRACE("numblob=%5d - nblob0=%3d - nblob1=%3d - nblob2=%3d - nblob3=%3d - part_err=%4d\n", numblob, nblob0, nblob1, nblob2, nblob3, part_err );

  return 0;
}
 
 */
 

/*
int CLQcolor_inspection::Search_center( void )
{
  int    i;
  int    x      = xcnt;
  int    y      = ycnt;
  int    niter  = 0; // number of iterations to decide deadlock
  
//  double minval = 0.0;
//  int    mindir = 0;
//  int    olddir = 0;
/*  refr   =  31;
  refg   =  65;
  refb   = 156; */
/*
  ZeroMemory( path, dimx*dimy );

  do
  {
    // HORIZONTAL CHECK

    // next RIGHT sector scores (0=right 1=bottom 2=left 3=top )
    //rv[0] = sectbrgth(x-1,y  ,0); // one to left 
    rv[1] = sectbrgth(x  ,y  ,0); // current position
    //rv[2] = sectbrgth(x+1,y  ,0); // one to right

    // next LEFT sector scores (0=right 1=bottom 2=left 3=top )
    //lv[0] = sectbrgth(x-1,y  ,2); // one to left 
    lv[1] = sectbrgth(x  ,y  ,2); // current position
    //lv[2] = sectbrgth(x+1,y  ,2); // one to right

//    if ( lv[0]+rv[0]<lv[1]+rv[1] && lv[0]+rv[0]<lv[2]+rv[2] ) x--; // MOVE TO LEFT
//    if ( lv[2]+rv[2]<lv[1]+rv[1] && lv[2]+rv[2]<lv[0]+rv[0] ) x++; // MOVE TO RIGHT

    if      ( lv[1]<rv[1] ) x--; // MOVE TO LEFT
    else if ( rv[1]<lv[1] ) x++; // MOVE TO RIGHT

    // VERTICAL CHECK

    // next BOTTOM sector scores (0=right 1=bottom 2=left 3=top )
    //bv[0] = sectbrgth(x  ,y-1,1); // one to top
    bv[1] = sectbrgth(x  ,y  ,1); // current position
    //bv[2] = sectbrgth(x  ,y+1,1); // one to bottom

    // next LEFT sector scores (0=right 1=bottom 2=left 3=top )
    //tv[0] = sectbrgth(x  ,y-1,3); // one to top 
    tv[1] = sectbrgth(x  ,y  ,3); // current position
    //tv[2] = sectbrgth(x  ,y+1,3); // one to bottom

    //if ( tv[0]+bv[0]<tv[1]+bv[1] && tv[0]+bv[0]<tv[2]+bv[2] ) y--; // MOVE TO TOP
    //if ( tv[2]+bv[2]<tv[1]+bv[1] && tv[2]+bv[2]<tv[0]+bv[0] ) y++; // MOVE TO BOTTOM

    if      ( tv[1]<bv[1] ) y--; // MOVE TO TOP
    else if ( bv[1]<tv[1] ) y++; // MOVE TO BOTTOM

    //TRACE( "Iter[%3d] %3d,%3d\n", niter, x, y );
    if ( path[x+y*dimx] ) break; // already passed here!
    path[x+y*dimx]++;
    niter++;

  } while( x>xmin+srcrad && x<xmax-srcrad && y>ymin+srcrad && y<ymax-srcrad && niter<100 );


  int toterr = int ( 0.5*(lv[1]+tv[1]+rv[1]+bv[1])/circle[srcrad].n ); // total average error over full circle
 
  #ifdef FD_DATAREC
    //if ( datarec_on && cap_result ) cap_result->InsertI( toterr, 19 ); // insert alignment error into 19 cat 
  #endif


  if ( toterr < src_error )
  {
    xcnt = x;
    ycnt = y;

//    txt1.Format( string_2752[lng],xcnt,ycnt);
//    txt1.Format( L"Center found (%d,%d) err=%d - niter=%d", xcnt, ycnt, toterr, niter );
    txt1.Format( string_2430[lng], xcnt, ycnt, toterr, niter );
    col1 = TXT_DARKGREEN;

    for ( i=0 ; i<circle[srcrad].n ; i+=8 )  // Draw found circle
    {
      DRAW_CROSS(index,xcnt+circle[srcrad].x[i],ycnt+circle[srcrad].y[i],COL_RED);
      DRAW_CROSS(index,xcnt-circle[srcrad].x[i],ycnt-circle[srcrad].y[i],COL_RED);
    }
    return 0; // search completed successfully
  }
  else
  {
//    txt1.Format( string_2754[lng], rv[1], bv[1], lv[1], tv[1] ); // Center NOT found
//    txt1.Format( L"Center NOT found err=%d - niter=%d", toterr, niter );
    txt1.Format( string_2432[lng], toterr, niter );

    double cf = double(ymax-ymin)/(xmax-xmin);
    for ( x=xmin+50 ; x<=xmax-50 ; x+=4 )
    {
      DRAW_PT( index, x, ymin+int(cf*(x-xmin)), COL_RED );
      DRAW_PT( index, x, ymax-int(cf*(x-xmin)), COL_RED );
    }
    return 99; // too low: not validated / center NOT found
  }




/*
    Calculate_scores( x, y );

    mindir = 0;
    minval = score[0];
    for ( i=1 ; i<5 ; i++ )
    {
      if ( score[i]<minval )
      {
        minval=score[i];
        mindir=i;
      }
    }

    if ( mindir==olddir )
    {
      //TRACE("UGUALI\n");
      mindir = 0;
    }

    switch ( mindir )
    {
      case 1: // max to the right
        x++;
        olddir = 3;
        //TRACE("moving RIGHT to (%d,%d)\n",x,y);
        break;
      case 2: // max to bottom
        y++;
        olddir = 4;
        //TRACE("moving DOWN to (%d,%d)\n",x,y);
        break;
      case 3: // max to the left
        x--;
        olddir = 1;
        //TRACE("moving LEFT to (%d,%d)\n",x,y);
        break;
      case 4: // max to top
        y--;
        olddir = 2;
        //TRACE("moving UP   to (%d,%d)\n",x,y);
        break;
    } 
    
    niter++;

  //    if ( x==xo && y==yo ) TRACE("UGUALI\n");
  } while( mindir>0 && x>xmin+srcrad && x<xmax-srcrad && y>ymin+srcrad && y<ymax-srcrad && niter<500 );
  */

  //TRACE("niter=%d\n",niter);


//  int lummin = 10;
//  lummin = 10;

//  TRACE("Max corrispondenza in %d,%d (%d us)\n", x, y, timer.elapsed_us() );
//  return 0; // search completed successfully

/*
}
*/


/*
// 0=center - 1=right - 2=bottom - 3=left - 4=top
int CLQcolor_inspection::Calculate_scores( int x, int y )
{
  // central position's score
  rv[0] = sectbrgth(x,y,0); // 0=right 1=bottom 2=left 3=top 
  bv[0] = sectbrgth(x,y,1); // 0=right 1=bottom 2=left 3=top 
  lv[0] = sectbrgth(x,y,2); // 0=right 1=bottom 2=left 3=top 
  tv[0] = sectbrgth(x,y,3); // 0=right 1=bottom 2=left 3=top 
  //Calculate_score(0);
  score[0] = 0.0;
  score[1] = lv[0]?double(rv[0])/lv[0]:rv[0];  // score moving toward right
  score[2] = tv[0]?double(bv[0])/tv[0]:bv[0];  // score moving toward bottom
  score[3] = rv[0]?double(lv[0])/rv[0]:lv[0];  // score moving toward left
  score[4] = bv[0]?double(tv[0])/bv[0]:tv[0];  // score moving toward top

  return 0;
}
*/


/*
#define DIFF(x,y) abs(PTR(x,y)-ref_col[0])+abs(PTG(x,y)-ref_col[1])+abs(PTB(x,y)-ref_col[2])

UINT CLQcolor_inspection::sectbrgth( int xc, int yc, int side ) // 0=right 1=bottom 2=left 3=top 
{
  UINT diff = 0;
  int  i;

  ASSERT( img ); // must have been already assigned to img_acq
  ASSERT( circle[srcrad].n && circle[srcrad].x && circle[srcrad].y );

  if ( side==0 ) // right sector
  {
    //for ( i=                   0 ; i<circle[srcrad].n/4 ; i++ ) lum += PT2( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    //for ( i=3*circle[srcrad].n/4 ; i<circle[srcrad].n   ; i++ ) lum += PT2( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    for ( i=                   0 ; i<circle[srcrad].n/4 ; i++ ) diff += DIFF( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    for ( i=3*circle[srcrad].n/4 ; i<circle[srcrad].n   ; i++ ) diff += DIFF( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    //return( 2*diff/circle[srcrad].n );
    return( diff );
  }

  if ( side==1 ) // bottom sector
  {
    //for ( i=circle[srcrad].n/4 ; i<3*circle[srcrad].n/4 ; i++ ) lum += PT2( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    for ( i=circle[srcrad].n/4 ; i<3*circle[srcrad].n/4 ; i++ ) diff += DIFF( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    //return( 2*diff/circle[srcrad].n );
    return( diff );
  }

  if ( side==2 ) // left sector
  {
    //for ( i=3*circle[srcrad].n/4 ; i<circle[srcrad].n   ; i++ ) lum += PT2( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    //for ( i=                   0 ; i<circle[srcrad].n/4 ; i++ ) lum += PT2( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    for ( i=3*circle[srcrad].n/4 ; i<circle[srcrad].n   ; i++ ) diff += DIFF( xc+circle[srcrad].x[i] , yc+circle[srcrad].y[i] );
    for ( i=                   0 ; i<circle[srcrad].n/4 ; i++ ) diff += DIFF( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    //return( 2*diff/circle[srcrad].n );
    return( diff );
  }

  if ( side==3 ) // top sector
  {
    //for ( i=circle[srcrad].n/4 ; i<3*circle[srcrad].n/4 ; i++ ) lum += PT2( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    for ( i=circle[srcrad].n/4 ; i<3*circle[srcrad].n/4 ; i++ ) diff += DIFF( xc-circle[srcrad].x[i] , yc-circle[srcrad].y[i] );
    //return( 2*diff/circle[srcrad].n );
    return( diff );
  }

//  return( diff ); // 2*lum/circle[srcrad].n );
  return 0;
}
*/
