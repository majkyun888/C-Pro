#pragma once

#include "stdafx.h"
#include "IVS.h"
#include "Globals.h"
#include "ChildView.h"
#include "Waist_layer.h"
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




CWaist_layer::CWaist_layer( UINT ind, CFile_mngmt* file_manager_in )
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

/*  lum_thr  = 200;
  min_npt  = 200;
  hliv_max = 250;
  hliv_min = 150;
  
  mode = FILLINGL_MODE_REFLECTION;

  hliv_prc = 0; // alignment/correction from bottom initially disabled
  
  #ifdef FILLING_BOTTOMCOMP // compensation of bottom height in backlight images
    hliv_prc = 85;
  #endif  */


  num_img       =  10;  // number of images for each inspection cycle
  bott_thr      =  85;  // percentage of brightness drop to detect bottom of vials

  imgshw        =    1; 

  /////////////////////////////////////////////////////////////////////////////////////////////////////

  waist_lum     =  70;  // threshold brightness for detection of sides of cake
  waist_hmax    = 250;  // max/high height for waist inspection 
  waist_hmin    =  50;  // min/low  height for waist inspection 
  waist_wmax    = 420;  // max width for waist inspection 
  waist_wmin    = 380;  // min width height for waist inspection 
  waist_maxerr  = 100;  // maximum allowed waist shrink total error (0 means control disabled)
  waist_minhgt  =  50;  // minimum continuous height of shrunk waist (continuous width under wmin)
  waist_minnimg =   1;  // minimum number of bad images/inspections to cause reject (default 1)

  /////////////////////////////////////////////////////////////////////////////////////////////////////

  layer_hmax    = 160;  // max/high height for layer inspection 
  layer_hmin    =  80;  // min/low  height for layer inspection 
  layer_width   = 300;  // width of area for layer inspection (search of sides)
  layer_minlum  = 100;  // min brightness to consider layer pixels
  layer_maxerr  = 100;  // maximum allowed layer total error (0 means control disabled)

  /////////////////////////////////////////////////////////////////////////////////////////////////////

}


CWaist_layer::~CWaist_layer(void)
{

  if ( area )
  {
    delete area;
    area = NULL;
  }


}


int CWaist_layer::Set_inspection_area( int ind )
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

  TRACE("CWaist_layer::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CWaist_layer::Add_menu_parameters( CContainer_wnd* child )
{


  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3110[lng], child );
  file_manager->Add_separator( string_3110[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &num_img,      CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2032[lng], string_unit   [lng],  20,    0,     200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &imgshw,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2028[lng], string_unit   [lng],   1,    0,     200, NULL         );
  pView->parameter[pView->num_params++] = new CParameter( &bott_thr,     CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3114[lng], string_percent[lng],  85,   10,     200, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_lum,    CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3130[lng], string_log    [lng], 100,    0,     255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_hmax,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3140[lng], string_pix    [lng], 250,    2,  dimx-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_hmin,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3142[lng], string_pix    [lng],  50,    2,  dimx-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_wmax,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3146[lng], string_pix    [lng], 250,    2,  dimy-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_wmin,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3148[lng], string_pix    [lng],  50,    2,  dimy-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_minhgt, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3150[lng], string_pix    [lng], 250,    2,  dimx-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &waist_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3178[lng], string_unit   [lng],  50,    0,    9999, file_manager );

  pView->num_menu_btn++;

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_3160[lng], child );
  file_manager->Add_separator( string_3160[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  pView->parameter[pView->num_params++] = new CParameter( &layer_hmax,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3140[lng], string_pix [lng], 100,    2,  dimx-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &layer_hmin,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3142[lng], string_pix [lng], 250,    2,  dimx-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &layer_width,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3170[lng], string_pix [lng],  50,    2,  dimy-2, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &layer_minlum, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3172[lng], string_log [lng], 100,    0,     255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &layer_maxerr, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_3178[lng], string_unit[lng],  50,    0,    9999, file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  return 0; // completed successfully
}



int CWaist_layer::Process_image( BYTE* img_acq, int nimg )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  //SCALE_IMG( index, img_acq );

  int x,y;

  // draw inspection area first

  if ( nimg==0 ) // only first image
  {
    good        = TRUE;
    //draw_ovl    = TRUE;
    txt1        = L"waist";
    txt2        = L"layer";
    waist_err   = 0;
    layer_err   = 0;
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

  // search bottom
  if  ( Search_bottom( img_acq ) )
  {
/*    txt1 = string_3120[lng];
    txt2 = L""; //string_3120[lng];
    col1 = TXT_DARKRED;
    col2 = TXT_BLACK;
    good = FALSE; // reject */
    return 199;              // Bottom NOT found, return
  }

  // check sides for waist
  if  ( Analyize_sides( img_acq ) )
  {
    return 299; // Reject by waist shrink, return
  }


  // check layer area for bright spots
  if  ( Analyize_layer( img_acq ) )
  {
    
    return 399; // Reject by layer, return
  }
  
  
  return 0; // completed successfully
}


int CWaist_layer::Final_result( void  )
{

  if ( num_aligned < 60*num_img/100 )    // too bad alignment (less than 60%)
  {
    good = FALSE;
    txt1.Format( string_3188[lng], num_aligned, num_img );
    col1 = TXT_DARKRED;
    txt2 = L"";
    col2 = TXT_BLACK;
    return 99;
  }

  if ( waist_err > waist_maxerr )    // waist REJECT
  {
    good = FALSE;
    txt1.Format( string_3182[lng], waist_err, waist_maxerr );
    col1 = TXT_DARKRED;
  }
  else                               // waist GOOD
  {
    txt1.Format( string_3180[lng], waist_err, waist_maxerr );
    col1 = TXT_DARKGREEN;
  }


  if ( layer_err > layer_maxerr )    // layer REJECT
  {
    good = FALSE;
    txt2.Format( string_3186[lng], layer_err, layer_maxerr );
    col2 = TXT_DARKRED;
  }
  else                               // layer GOOD
  {
    txt2.Format( string_3184[lng], layer_err, layer_maxerr );
    col2 = TXT_DARKGREEN;
  }


  return 0; // final result GOOD
}



int CWaist_layer::Search_bottom( BYTE* img_acq )
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


int CWaist_layer::Analyize_sides( BYTE* img_acq )
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

 //      if ( consec_sta==0 ) consec_sta = x;
        
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

//          consec_end = x+edge_lenght; 
          col = COL_RED; 

          consec_end=0; // reset consec_end
          err_frame += waist_wmin-(y2-y1);

          //TRACE("Tratto stretto x=%d\n",x);
        }
        else                      // Good width GREEN
        {
          if ( consec_end==0 ) consec_end = x;

//          if ( consec_sta && consec_end-consec_sta>waist_minhgt ) // just finished BAD SECTOR (high enough)
          if ( consec_sta && consec_end && consec_end-consec_sta>waist_minhgt ) // just finished BAD SECTOR (high enough)
          {
            for ( int i=consec_sta ; i<consec_end ; i++ )
            {
/*              DRAW_PT( index, i, y1-13, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y1-10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y2+10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, y2+13, COL_RED ); // 0xFFRRGGBB */
              DRAW_PT( index, i, ymin+10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, ymin+15, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, ymax-10, COL_RED ); // 0xFFRRGGBB
              DRAW_PT( index, i, ymax-15, COL_RED ); // 0xFFRRGGBB
            }

            waist_err += err_frame;
            //TRACE("Tratto consecutivo x=%d err_frame=%d\n",x,err_frame);

            //consec_end=0; // reset consec_end
          }

          col = COL_GREEN;

          consec_sta=0; // reset consec_sta

          bad_height = 0; // reset consecutive counter

          err_frame = 0;
          //TRACE("Tratto normale x=%d\n",x);
        }

      }

      // check if it's ALL BAD
//      if ( consec_sta==x1 && consec_end-consec_sta>waist_minhgt ) 

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

  if ( consec_sta==x1 && consec_end==0 && x2-x1>waist_minhgt ) 
  {
    for ( int i=x1 ; i<x2 ; i++ )
    {
/*      DRAW_PT( index, i, y1-23, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, y1-20, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, y2+20, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, y2+23, COL_RED ); // 0xFFRRGGBB */

      DRAW_PT( index, i, ymin+10, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, ymin+15, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, ymax-10, COL_RED ); // 0xFFRRGGBB
      DRAW_PT( index, i, ymax-15, COL_RED ); // 0xFFRRGGBB
    }
    waist_err += err_frame;
    //TRACE("TUTTO ERRATO\n");
  }



  if ( ycount<3 )
  {
    return 291; // cannot find the y center of cake
  }

  ycent = int( 0.5 + 0.5*ycent/ycount );

  return 0; // completed successfully
}



int CWaist_layer::Analyize_layer( BYTE* img_acq )
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


/*
int CWaist_layer::Search_bottom( BYTE* img_acq )
{
  return 0; // completed successfully
}
*/







