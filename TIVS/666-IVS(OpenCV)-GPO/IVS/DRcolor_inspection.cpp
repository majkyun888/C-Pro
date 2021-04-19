#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
#include "FOcolor_inspection.h"


extern Camera_struct    camera  [NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern circle_type      my_circle[MAXR_CIRCLES];
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



CDRcolor_inspection::CDRcolor_inspection( UINT ind, CFile_mngmt* file_manager_in, int mode_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy *3;   // dim in bytes (x3) !

  area_color = COL_BLUE;  // COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area
//  area_color = 0xFF0000AA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 ); //&& (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =      8; //  dimy/6;
  area->buffrect.bottom = dimy-8; // 5*dimy/6;
  area->buffrect.left   =      8; // dimx/6;
  area->buffrect.right  = dimx-8; // 5*dimx/6;

/*  area->buffrect.top    =     94;
  area->buffrect.bottom =    306;
  area->buffrect.left   =    158;
  area->buffrect.right  =    362; */


//  ZeroMemory( Lab_ref, sizeof(Lab_ref) );

/*  src_radius    =  175;
  src_error     =   75;
  anl_radius    =  170;
  col_diff      =  150;*/

//  learning_fo   = FALSE;

//  lum_nthr      =   20;

//  maxerr_lab    =    4; // max average color difference ( Lab coordinates )
//  maxerr_col    = 1000;

/*  // basic RGB colors for alignment
  ref_col[0]    =   31; // red
  ref_col[1]    =   65; // green
  ref_col[2]    =  156; // blue

  path = (BYTE*) malloc(dimx*dimy);
  ASSERT( path && AfxIsValidAddress(path,dimx*dimy) );
  ZeroMemory( path, dimx*dimy );*/


  H_min = 200;
  H_max = 300;

  S_min =  50;
  S_max = 100;

  V_min =  50;
  V_max = 100;

  npt_min = 100;
}


CDRcolor_inspection::~CDRcolor_inspection(void)
{

/*  if ( path )
  {
    delete path;
    path = NULL;
  } */

  if ( area )
  {
    delete area;
    area = NULL;
  }
}




int CDRcolor_inspection::Set_inspection_area( int ind )
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

  TRACE("CDRcolor_inspection::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CDRcolor_inspection::Add_menu_parameters( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2600[lng], child );
  file_manager->Add_separator( string_2600[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );


  pView->parameter[pView->num_params++] = new CParameter( &H_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3022[lng], string_unit[lng],  100,    0,   360, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &H_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3020[lng], string_unit[lng],  150,    0,   360, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &S_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3026[lng], string_unit[lng],   50,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &S_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3024[lng], string_unit[lng],  100,    0,   100, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &V_min,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3030[lng], string_unit[lng],   50,    0,   100, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &V_max,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3028[lng], string_unit[lng],  100,    0,   100, file_manager );

  pView->parameter[pView->num_params++] = new CParameter( &npt_min,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3032[lng], string_unit[lng],  100,    0, 10000, file_manager );


//  pView->parameter[pView->num_params++] = new CParameter( &learning_fo,   CTYPE_BOOL8, 0, pView->menu_btn[pView->num_menu_btn]->child, string_2408[lng], 0, 0, 0, 1, NULL );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

//  file_manager->Add_data( Lab_ref, sizeof(Lab_ref) );
//  file_manager->Add_data( ref_col, sizeof(ref_col) );

  return 0; // completed successfully
}





int CDRcolor_inspection::Process_image( CGrabResultPtr ptr_gr, UINT nimg )
{
  image_dsp[index]->scale_grabresult( ptr_gr ); // scale for display and prepare RGB buffer from BAYER data
  return Process_image( image_dsp[index]->ptr_buf, nimg );
}



//int CDRcolor_inspection::Process_image( BYTE* img_acq, UINT nimg )
int CDRcolor_inspection::Process_image( BYTE* ptr_in, UINT nimg )
{

  ASSERT( AfxIsValidAddress( ptr_in, dim ) );
  img = ptr_in;

/*
  if ( !npt_min ) // if npt_min=0 completely disabled
  {
    txt1 = L"DISABLED";
    txt2 = L"-";
    good = FALSE;
    col1 = TXT_BLACK;
    col2 = TXT_BLACK;
    return 0;  // return inspection GOOD
  }
  */

  txt1 = L"-";
  txt2 = L"-";
  good = FALSE;
  col1 = TXT_DARKRED;
  col2 = TXT_DARKRED;

  // draw inspection area first
/*
  int x, y;
  for ( x=xmin ; x<=xmax ; x+=4 )
  {
    DRAW_PT( index, x, ymin, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, area_color ); // 0xFFRRGGBB
  }
  for ( y=ymin ; y<=ymax ; y+=4 )
  {
    DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
  }*/ 

  if ( Analyze_area() ) // reject
  {
    return 99;
  }



//  img  = img_acq;
//  SCALE_IMG( index, img_acq );
//  CTimer timer;

/*  npt  = 0;
  totr = 0;
  totg = 0;
  totb = 0;

  // initial search coords
  xcnt = (xmin+xmax)/2;
  ycnt = (ymin+ymax)/2;

  srcrad = src_radius; // lock temporary radius for analysis

  if ( learning_fo ) // Learning - Use center coordinates
  {
    int   r, i;
    BYTE* ofs;
    for ( r=10 ; r<anl_radius-30 ; r++ ) 
    {
      for ( i=0 ; i<my_circle[r].n ; i++ )
      {
        ofs   = img + 3*( xcnt+my_circle[r].x[i] + (ycnt+my_circle[r].y[i])*dimx );
        totr += *ofs++;
        totg += *ofs++;
        totb += *ofs;

        ofs   = img + 3*( xcnt-my_circle[r].x[i] + (ycnt-my_circle[r].y[i])*dimx );
        totr += *ofs++;
        totg += *ofs++;
        totb += *ofs;
      }
      npt += 2*i;
    }

    Calculate_average();

    UINT color_ref = 0xFF000000 | ref_col[0]<<16 | ref_col[1]<<8 | ref_col[2];

    for ( r=10 ; r<anl_radius-30 ; r++ ) for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],color_ref);
      DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],color_ref);
    }

    #ifdef FD_DATAREC
      if ( datarec_on && cap_result ) cap_result->InsertI( -50, 6 ); // insert -50 into 6 cat 
    #endif

    learning_fo = FALSE;
    return 0;
  }


  // NORMAL PROCESSING

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


  #ifdef FD_DATAREC
    if ( datarec_on && cap_result ) cap_result->InsertI( 0, 6 ); // insert 0 into 6 cat = GOOD / ACCEPTED
  #endif  */

  return 0; // inspection ok / good
}










int CDRcolor_inspection::Analyze_area( void )
{
  int x, y;

  npt = 0;

  //TRACE( "(%d,%d)-(%d,%d)\n", xmin, ymin, xmax, ymax );

  int x1 = xmin;
  int y1 = ymin;
  BUF2DSP(index,x1,y1);

  int x2 = xmax;
  int y2 = ymax;
  BUF2DSP(index,x2,y2);

  for ( y=(y1<y2?y1:y2) ; y<=(y1<y2?y2:y1) ; y++ )
  {
    for ( x=(x1<x2?x1:x2) ; x<=(x1<x2?x2:x1) ; x++ )
    {
      image_dsp[index]->ptr_dsp[x+y*image_dsp[index]->dim_dsp.cx] = COL_BLACK;
    }
  }


  for ( y=ymin ; y<=ymax ; y+=3 )
  {
    for ( x=xmin ; x<=xmax ; x+=3 )
    {
   /*   PTR(x,  y  ) = 0;
      PTR(x+1,y  ) = 0;
      PTR(x,  y+1) = 0;
      PTR(x+1,y+1) = 0;

      PTG(x,  y  ) = 255;
      PTG(x+1,y  ) = 255;
      PTG(x,  y+1) = 255;
      PTG(x+1,y+1) = 255;

      PTB(x,  y  ) = 0;
      PTB(x+1,y  ) = 0;
      PTB(x,  y+1) = 0;
      PTB(x+1,y+1) = 0;*/

      //double R = (1.0/1020.0) * ( PTR(x,y) + PTR(x+1,y) + PTR(x,y+1) + PTR(x+1,y+1) );
      //double G = (1.0/1020.0) * ( PTG(x,y) + PTG(x+1,y) + PTG(x,y+1) + PTG(x+1,y+1) );
      //double B = (1.0/1020.0) * ( PTB(x,y) + PTB(x+1,y) + PTB(x,y+1) + PTB(x+1,y+1) );

      #define COEF (1.0/(255.0*9.0))
	    double R = COEF * ( PTR(x-1,y-1) + PTR(x,y-1) + PTR(x+1,y-1) + PTR(x-1,y) + PTR(x,y) + PTR(x+1,y) + PTR(x-1,y+1) + PTR(x,y+1) + PTR(x+1,y+1) );
	    double G = COEF * ( PTG(x-1,y-1) + PTG(x,y-1) + PTG(x+1,y-1) + PTG(x-1,y) + PTG(x,y) + PTG(x+1,y) + PTG(x-1,y+1) + PTG(x,y+1) + PTG(x+1,y+1) );
	    double B = COEF * ( PTB(x-1,y-1) + PTB(x,y-1) + PTB(x+1,y-1) + PTB(x-1,y) + PTB(x,y) + PTB(x+1,y) + PTB(x-1,y+1) + PTB(x,y+1) + PTB(x+1,y+1) );


      // r,g,b values are from 0 to 1 http://www.cs.rit.edu/~ncs/color/t_convert.html
      // h = [0,360], s = [0,1], v = [0,1]
      //		if s == 0, then h = -1 (undefined)
      // void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
      //{
      //double min, max, delta;
      
      //min = MIN( r, g, b );
      //max = MAX( r, g, b );

      double min = R;
      double max = R;

      if ( G < min ) min = G;
      if ( G > max ) max = G;

      if ( B < min ) min = B;
      if ( B > max ) max = B;
            
      double V = max;				// v
      double S = 0.0;
      double H = 0.0;

      double delta = max - min;
      
      if( max != 0 )  S = delta / max;		// s
      else 
      {
        S =  0;
        S = -1;
      }
  
      if        ( R == max )  H =     ( G - B ) / delta;	// between yellow & magenta
      else if   ( G == max )  H = 2 + ( B - R ) / delta;	// between cyan & yellow
           else               H = 4 + ( R - G ) / delta;	// between magenta & cyan

      H *= 60;				// degrees
      if( H < 0 ) H += 360;

      /*  preso da http://www.easyrgb.com/index.php?X=MATH&H=20#text20

      double var_R = 0.0009765625 * ( PTR(x,y) + PTR(x+1,y) + PTR(x,y+1) + PTR(x+1,y+1) );
      double var_G = 0.0009765625 * ( PTG(x,y) + PTG(x+1,y) + PTG(x,y+1) + PTG(x+1,y+1) );
      double var_B = 0.0009765625 * ( PTB(x,y) + PTB(x+1,y) + PTB(x,y+1) + PTB(x+1,y+1) );

      double var_Min = var_R;
      double var_Max = var_R;

      if ( var_G < var_Min ) var_Min = var_G;
      if ( var_G > var_Max ) var_Max = var_G;

      if ( var_B < var_Min ) var_Min = var_B;
      if ( var_B > var_Max ) var_Max = var_B;

      double del_Max = var_Max - var_Min;

      double V = var_Max;

      double H = 0.0;
      double S = 0.0;

      if ( del_Max > 0.0 )                   // If 0 is a gray, no chroma...
      {
        S = del_Max / var_Max;

        double del_R = ( ( ( var_Max - var_R ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
        double del_G = ( ( ( var_Max - var_G ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
        double del_B = ( ( ( var_Max - var_B ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;

        if           ( var_R == var_Max ) H = del_B - del_G;
        else if      ( var_G == var_Max ) H = ( 1.0 / 3.0 ) + del_R - del_B;
             else if ( var_B == var_Max ) H = ( 2.0 / 3.0 ) + del_G - del_R;

        if ( H < 0.0 ) H += 1.0;
        if ( H > 1.0 ) H -= 1.0;
      }

      */

	  int Hi = int( 0.5 +       H );
	  int Si = int( 0.5 + 100.0*S );
	  int Vi = int( 0.5 + 100.0*V );


    //if ( H>100 && H<140 && S>0.5 && V>0.5 )
    //if ( PTG(x,y)>180 )
    if ( Hi>=H_min && Hi<=H_max && Si>=S_min && Si<=S_max && Vi>=V_min && Vi<=V_max )
    {
      npt++;
      DRAW_PT( index, (x-1), (y-1), COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index,  x,    (y-1), COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1), (y-1), COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index, (x-1),  y,    COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index,  x,     y,    COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1),  y,    COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index, (x-1), (y+1), COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index,  x,    (y+1), COL_GREEN ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1), (y+1), COL_GREEN ); // 0xFFRRGGBB
    }
   /* else
    {
      DRAW_PT( index, (x-1), (y-1), COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index,  x,    (y-1), COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1), (y-1), COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index, (x-1),  y,    COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index,  x,     y,    COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1),  y,    COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index, (x-1), (y+1), COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index,  x,    (y+1), COL_BLACK ); // 0xFFRRGGBB
      DRAW_PT( index, (x+1), (y+1), COL_BLACK ); // 0xFFRRGGBB
    } */ 


    //  DRAW_PT( index, xmin, y, area_color ); // 0xFFRRGGBB
    //  DRAW_PT( index, xmax, y, area_color ); // 0xFFRRGGBB
    } 

  }


  npt_cam = npt;

  col1 = 0;
  txt1.Format( L"%d points found", npt );

/*  if ( npt < npt_min )
  {
    col1 = TXT_DARKRED;
    txt1.Format( L"REJECT: %d points found", npt );
    return 99;
  }

  col1 = TXT_DARKGREEN;
  txt1.Format( L"GOOD: %d points found", npt ); */
  return 0;
}





/*
var_R = ( R / 255 )                     //RGB from 0 to 255
var_G = ( G / 255 )
var_B = ( B / 255 )

var_Min = min( var_R, var_G, var_B )    //Min. value of RGB
var_Max = max( var_R, var_G, var_B )    //Max. value of RGB
del_Max = var_Max - var_Min             //Delta RGB value 

V = var_Max

if ( del_Max == 0 )                     //This is a gray, no chroma...
{
   H = 0                                //HSV results from 0 to 1
   S = 0
}
else                                    //Chromatic data...
{
   S = del_Max / var_Max

   del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
   del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
   del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max

   if      ( var_R == var_Max ) H = del_B - del_G
   else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B
   else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R

   if ( H < 0 ) H += 1
   if ( H > 1 ) H -= 1
}
*/


/*
int CDRcolor_inspection::Analyze_center( void )
{
  BYTE* ofs;
  int   r, i;
  int err = 0;
  err_col = 0;

  if ( xcnt-anl_radius<2 || xcnt+anl_radius>dimx-2 || ycnt-anl_radius<2 || ycnt>dimy-2 ) return 1999; // center not allowing inspection

  for ( r=1 ; r<anl_radius ; r++ ) 
  {
    for ( i=0 ; i<my_circle[r].n ; i++ )
    {
      ofs   = img + 3*( xcnt+my_circle[r].x[i] + (ycnt+my_circle[r].y[i])*dimx );
      err   = abs( *ofs - ref_col[0] );
      totr += *ofs++;
      err  += abs( *ofs - ref_col[1] );
      totg += *ofs++;
      err  += abs( *ofs - ref_col[2] );
      totb += *ofs;

      if ( err>col_diff )
      {
        err_col += err-col_diff;
        DRAW_PT(index,xcnt+my_circle[r].x[i],ycnt+my_circle[r].y[i],COL_RED);
      }

      ofs   = img + 3*( xcnt-my_circle[r].x[i] + (ycnt-my_circle[r].y[i])*dimx );
      err   = abs( *ofs - ref_col[0] );
      totr += *ofs++;
      err  += abs( *ofs - ref_col[1] );
      totg += *ofs++;
      err  += abs( *ofs - ref_col[2] );
      totb += *ofs;

      if ( err>col_diff )
      {
        err_col += err-col_diff;
        DRAW_PT(index,xcnt-my_circle[r].x[i],ycnt-my_circle[r].y[i],COL_RED);
      }

    }
    npt += 2*i;
  }


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



/*
double CDRcolor_inspection::funz_t( double in )
{
  if ( in > 0.008856 ) return( pow( in , 1.0/3.0 ) );
  else return( 7.787 * in + 16-0 / 116.0 );
}
*/

/*
int CDRcolor_inspection::Calculate_average( void )
{

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


  if ( learning_fo ) // learning active
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
  }

  dist_lab = (L-Lab_ref[0])*(L-Lab_ref[0]) + (a-Lab_ref[1])*(a-Lab_ref[1]) + (b-Lab_ref[2])*(b-Lab_ref[2]);

  if ( dist_lab > 0.0 ) dist_lab = sqrt( dist_lab );
  else                  dist_lab = 0.0;
  return 0;




//  txt1.Format( L"RGB (%.3lf,%.3lf,%.3lf) - XYZ (%.3lf,%.3lf,%.3lf)", R, G, B, X, Y, Z );




//  txt1.Format( L"RGB (%d,%d,%d) - YUV (%d,%d,%d)", totr, totg, totb, Y, U, V );


}

*/


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


int CDRcolor_inspection::Calculate_blob( void )
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
 
        while ( ( end > start ) && ( end < MAX_DIM-10 ) ) // finch?ci sono punti in lista da fare          
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


        // IN QUESTO PUNTO end ?la dimensione del blob
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
int CDRcolor_inspection::Search_center( void )
{
  int    i;
  int    x      = xcnt;
  int    y      = ycnt;
  int    niter  = 0; // number of iterations to decide deadlock
  
//  double minval = 0.0;
//  int    mindir = 0;
//  int    olddir = 0;

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


  int toterr = int ( 0.5*(lv[1]+tv[1]+rv[1]+bv[1])/my_circle[srcrad].n ); // total average error over full circle
 
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

    for ( i=0 ; i<my_circle[srcrad].n ; i+=8 )  // Draw found circle
    {
      DRAW_CROSS(index,xcnt+my_circle[srcrad].x[i],ycnt+my_circle[srcrad].y[i],COL_RED);
      DRAW_CROSS(index,xcnt-my_circle[srcrad].x[i],ycnt-my_circle[srcrad].y[i],COL_RED);
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





  //TRACE("niter=%d\n",niter);


//  int lummin = 10;
//  lummin = 10;

//  TRACE("Max corrispondenza in %d,%d (%d us)\n", x, y, timer.elapsed_us() );
//  return 0; // search completed successfully
}



/*
// 0=center - 1=right - 2=bottom - 3=left - 4=top
int CDRcolor_inspection::Calculate_scores( int x, int y )
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

UINT CDRcolor_inspection::sectbrgth( int xc, int yc, int side ) // 0=right 1=bottom 2=left 3=top 
{
  UINT diff = 0;
  int  i;

  ASSERT( img ); // must have been already assigned to img_acq
  ASSERT( my_circle[srcrad].n && my_circle[srcrad].x && my_circle[srcrad].y );

  if ( side==0 ) // right sector
  {
    //for ( i=                   0 ; i<my_circle[srcrad].n/4 ; i++ ) lum += PT2( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    //for ( i=3*my_circle[srcrad].n/4 ; i<my_circle[srcrad].n   ; i++ ) lum += PT2( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    for ( i=                   0 ; i<my_circle[srcrad].n/4 ; i++ ) diff += DIFF( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    for ( i=3*my_circle[srcrad].n/4 ; i<my_circle[srcrad].n   ; i++ ) diff += DIFF( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    //return( 2*diff/my_circle[srcrad].n );
    return( diff );
  }

  if ( side==1 ) // bottom sector
  {
    //for ( i=my_circle[srcrad].n/4 ; i<3*my_circle[srcrad].n/4 ; i++ ) lum += PT2( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    for ( i=my_circle[srcrad].n/4 ; i<3*my_circle[srcrad].n/4 ; i++ ) diff += DIFF( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    //return( 2*diff/my_circle[srcrad].n );
    return( diff );
  }

  if ( side==2 ) // left sector
  {
    //for ( i=3*my_circle[srcrad].n/4 ; i<my_circle[srcrad].n   ; i++ ) lum += PT2( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    //for ( i=                   0 ; i<my_circle[srcrad].n/4 ; i++ ) lum += PT2( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    for ( i=3*my_circle[srcrad].n/4 ; i<my_circle[srcrad].n   ; i++ ) diff += DIFF( xc+my_circle[srcrad].x[i] , yc+my_circle[srcrad].y[i] );
    for ( i=                   0 ; i<my_circle[srcrad].n/4 ; i++ ) diff += DIFF( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    //return( 2*diff/my_circle[srcrad].n );
    return( diff );
  }

  if ( side==3 ) // top sector
  {
    //for ( i=my_circle[srcrad].n/4 ; i<3*my_circle[srcrad].n/4 ; i++ ) lum += PT2( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    for ( i=my_circle[srcrad].n/4 ; i<3*my_circle[srcrad].n/4 ; i++ ) diff += DIFF( xc-my_circle[srcrad].x[i] , yc-my_circle[srcrad].y[i] );
    //return( 2*diff/my_circle[srcrad].n );
    return( diff );
  }

//  return( diff ); // 2*lum/my_circle[srcrad].n );
  return 0;
}

*/