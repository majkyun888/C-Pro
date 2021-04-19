#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Ampoule_filling_level.h"
#include "ChildView.h"
//#include "Def_globali.h"


//CArea_adjust* areainsp=NULL; // inspection/search area

extern Camera_struct    camera  [NUM_TLC];
//extern CArea_adjust*    areainsp[NUM_TLC];
extern UINT             lng;
extern CChildView*      pView;
extern CImage_display*  image_dsp[NUM_TLC];
extern UINT             num_tlc;
extern UINT             thread_status;
/*
extern CParameter*      parameter[MAX_NUM_PARAMS];
extern int              num_params; 
extern CMenu_button*    menu_btn[MAX_NUM_MENU_BTN];
extern int              num_menu_btn;
*/




CAmpoule_filling_level::CAmpoule_filling_level( UINT ind, CFile_mngmt* file_manager_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  area = new CArea_adjust( index, file_manager );
  area->buffrect.top    =   image_dsp[index]->dim_buf.cy/3;
  area->buffrect.bottom = 2*image_dsp[index]->dim_buf.cy/3;
  area->buffrect.left   =   image_dsp[index]->dim_buf.cx/3;
  area->buffrect.right  = 2*image_dsp[index]->dim_buf.cx/3;

  necksearch_x   =  535;
  necksearch_th  =  250;
  lum_min        =  123;
  diam           =  7.0;
  min_npt        = 1000;

  mm2pix         = 243.0 / 22.5; // pix = mm2pix * mm
  pix2mm         = 1.0 / mm2pix; // mm  = pix2mm * pix


}


CAmpoule_filling_level::~CAmpoule_filling_level(void)
{

  if ( area )
  {
    delete area;
    area = NULL;
  }


}




int CAmpoule_filling_level::Set_inspection_area( int ind )
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

  TRACE("CAmpoule_filling_level::Set_inspection_area(%d)\n", ind);
  return( 0 );
}




int CAmpoule_filling_level::Add_menu_parameters( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_600[lng], child );
  file_manager->Add_separator( string_600[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 300 );

  pView->parameter[pView->num_params++] = new CParameter( &necksearch_x,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_612[lng], string_pix [lng],    30,   10,   700, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &necksearch_th, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_614[lng], string_log [lng],   150,   10,  9999, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &diam,          CTYPE_LREAL, 0, pView->menu_btn[pView->num_menu_btn]->child, string_616[lng], string_mm  [lng],  17.0,  1.0,  40.0, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &lum_min,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_618[lng], string_log [lng],   100,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,       CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng],  1000,    0, 99999, file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

//	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_area, index );



  return 0; // completed successfully
}





#define PT(x,y) img_acq[x+(y)*camera[index].width]
//#define FLT(x,y) PT(x,y-6)+PT(x,y+6)-3*PT(x,y) // filter for middle dark search (glass edge)


// ORIGINAL FLT USED FOR FIRST SYSTEM (WITH HORIZONTL RING ON THE NECK)
//#define FLT(x,y) PT(x,y-4)+PT(x,y-2)-PT(x,y+2)-PT(x,y+4) // filter for middle dark search (glass edge)

// NEW FLT TO DETECT NECK ALSO WITHOUT HOR RING
#define FLT(x,y) PT(x,y-2)-PT(x,y)-PT(x,y+1)+PT(x,y+3) // filter for middle dark search (glass edge)



int CAmpoule_filling_level::Process_image( BYTE* img_acq )
{

  int x, y;

  int max1_v = 0;
  int max1_y = 0;
  int max2_v = 0;
  int max2_y = 0;

/*  int xmin = area->buffrect.left;
  int xmax = area->buffrect.right;
  int ymin = area->buffrect.top;
  int ymax = area->buffrect.bottom; */


  CString txt;
  UINT inspareacol = COL_BLUE;    //0xFF0000FF;
  UINT ampfoundcol = COL_MAGENTA; //0xFFFF00FF;


  // DRAW INSPECTION AREA
  for ( x=xmin ; x<=xmax ; x++ )
  {
    DRAW_PT( index, x, ymin, inspareacol ); // 0xFFRRGGBB
    DRAW_PT( index, x, ymax, inspareacol ); // 0xFFRRGGBB
  }

  for ( y=ymin ; y<=ymax ; y++ )
  {
    DRAW_PT( index, necksearch_x, y, inspareacol ); // 0xFFRRGGBB
    DRAW_PT( index, xmin,         y, inspareacol ); // 0xFFRRGGBB
    DRAW_PT( index, xmax,         y, inspareacol ); // 0xFFRRGGBB
  }


  // search neck ycenter first
  x = necksearch_x;
  //y = area->buffrect.top + 3;

  int ysta = area->buffrect.top    + 3;
  int yend = area->buffrect.bottom - 3;

  // calc left avg
  int bavg = (PT(x,ysta-3)+PT(x,ysta-2)+PT(x,ysta-1))/3 - necksearch_th;

  //TRACE("%d,%d,%d - %d\n", PT(x,ysta-3), PT(x,ysta-2), PT(x,ysta-1), bavg );


  if ( bavg<0 || bavg>255 ) // bad average brightness, something wrong
  {
    txt.Format( L"WRONG LEFT AVG BRIGHTNESS %d", bavg );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 191;
  } 
 
  y = ysta;

  // search first point above threshold
  while ( PT(x,y)>necksearch_th && y<yend-30 ) y++;

  if ( y>=yend-30 ) // first point (top in buf, left in image) NOT detected
  {
    txt.Format( L"AMPOULE NECK LEFT POINT NOT FOUND" );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 191;
  } 

  max1_y = y;
  //y += 30; // skip some space

  // calc right avg
  bavg = (PT(x,yend+1)+PT(x,yend+2)+PT(x,yend+3))/3 - necksearch_th;

  if ( bavg<0 || bavg>255 ) // bad average brightness, something wrong
  {
    txt.Format( L"WRONG RIGHT AVG BRIGHTNESS %d", bavg );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 191;
  } 
 
  y = yend;

  // search second point above threshold
  while ( PT(x,y)>necksearch_th && y>max1_y+30 ) y--;

  if ( y<=max1_y+30 ) // second point (top in buf, left in image) NOT detected
  {
    txt.Format( L"AMPOULE NECK RIGHT POINT NOT FOUND" );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 192;
  } 

  max2_y = y;


/*  while ( FLT(x,y)>=necksearch_th && y<area->buffrect.bottom )
  {
    if ( FLT(x,y)>max1_v )
    {
      max1_v = FLT(x,y);
      max1_y = y;
    }
    y++;
  }

  if ( max1_y==0 ) // first point (top in buf, left in image) NOT detected
  {
    txt.Format( L"AMPOULE NECK LEFT POINT NOT FOUND" );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 191;
  }  



  // search second area above neg threshold
  while ( FLT(x,y)> -necksearch_th && y<area->buffrect.bottom ) y++;

  while ( FLT(x,y)<=-necksearch_th && y<area->buffrect.bottom )
  {
    if ( FLT(x,y)<max2_v )
    {
      max2_v = FLT(x,y);
      max2_y = y;
    }
    y++;
  }

  if ( max2_y==0 ) // first point (top in buf, left in image) NOT detected
  {
    txt.Format( L"AMPOULE NECK RIGHT POINT NOT FOUND" );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 192;
  }*/


  /*
  // search second area above threshold
  y=area->buffrect.bottom;
  while ( FLT(x,y)<necksearch_th && y>area->buffrect.top && y>max2_y ) y--;

  while ( FLT(x,y)>=necksearch_th && y>area->buffrect.top && y>max2_y )
  {
    if ( FLT(x,y)>max2_v )
    {
      max2_v = FLT(x,y);
      max2_y = y;
    }
    y--;
  }
  */

  /*
  if ( max1_v<necksearch_th ) // validation failed, return
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of max1_v below threshold (%d/%d)\n", max1_v, necksearch_th );
    txt.Format( L"max1_v too low (%d)", max1_v );
    camera[index].text[1]->SetText( txt, RED_COL );
    return 100;
  }
  */
  
  if ( max1_y && max2_y && max2_y-max1_y<50 ) // validation failed, return
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of max2_v below threshold (%d/%d)\n", max2_v, necksearch_th );
    txt.Format( L"AMPOULE NECK NOT DETECTED (max1-max2 too close %d)", max2_y-max1_y );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 101;
  }


  int ycntr = (max1_y+max2_y)/2; // middle point

  y = ycntr;
  int innbr = PT(x,y-2)+PT(x,y-1)+PT(x,y+1)+PT(x,y+2); // average inner brightness
  int outbr = PT(x,max1_y-7)+PT(x,max1_y-6) + PT(x,max2_y+6)+PT(x,max2_y+7); // average outer brightness

  if ( outbr-innbr<10*4 ) // validation failed, return
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of difference outbr-innbr too low (%d/%d)\n", outbr-innbr, 10*4 );
    txt.Format( L"AMPOULE NECK NOT DETECTED (outer-inner brightness too low %d)", outbr-innbr );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 102;
  }

  // draw horizontal straight segment of found level
  for ( y=max1_y ; y<=max2_y ; y++ )
  {
    DRAW_PT( index, necksearch_x, y, ampfoundcol ); // 0xFFRRGGBB
  }

  // draw vertical straight segment in the center
  for ( x=necksearch_x ; x<=xmax ; x++ )
  {
    DRAW_PT( index, x, ycntr, ampfoundcol ); // 0xFFRRGGBB
  }

  ysta = ycntr - int( 0.5 * diam * mm2pix );
  if ( ysta <= 10 )
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of ymin/ymax (%d/%d)\n", ymin, ymax );
    txt.Format( L"AMPOULE NECK NOT DETECTED (ymin too close to left side %d)", ysta );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 103;
  }

  yend = ycntr + int( 0.5 * diam * mm2pix );
  if ( yend >= camera[index].height )
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of ymin/ymax (%d/%d)\n", ymin, ymax );
    txt.Format( L"AMPOULE NECK NOT DETECTED (ymax too close to right side %d)", yend );
    camera[index].text[1]->SetText( txt, TXT_DARKBLUE );
    return 103;
  }

  // draw dark points search area
  for ( x=xmin ; x<=xmax ; x++ )
  {
    DRAW_PT( index, x, ysta, ampfoundcol ); // 0xFFRRGGBB
    DRAW_PT( index, x, yend, ampfoundcol ); // 0xFFRRGGBB
  }


  // SECOND PART OF INSPECTION: SEARCH OF FILLING LEVEL

  UINT xtot = 0;
  int  npt  = 0;

//  for ( y=areainsp[index]->buffrect.top ; y<=areainsp[index]->buffrect.bottom ; y++ )
  for ( y=ysta ; y<=yend ; y++ )
  {
//    for ( x=areainsp[index]->buffrect.left ; x<=areainsp[index]->buffrect.right ; x++ )
    for ( x=xmin ; x<=xmax ; x++ )
    {
      if ( PT(x,y)<lum_min )
      {
        DRAW_PT( index, x, y, COL_RED ); // 0xFFRRGGBB
        xtot += x;
        npt++;
      }
    }
  }

  if ( npt < min_npt )
  {
    //TRACE("CAmpoule_filling_level: FL search failed because of too few points (%d/%d)\n", npt, 1000 );
    txt.Format( L"REJECT: Too few points found (%d/%d)", npt, min_npt );
    camera[index].text[1]->SetText( txt, TXT_DARKRED );
    return 900;  // REJECT: AMPOULE NECK FOUND BUT FILLING LEVEL IS NOT GOOD/ACCEPTABLE
  }
 

  txt.Format( L"FILLING LEVEL GOOD (%d/%d)", npt, min_npt );
  camera[index].text[1]->SetText( txt, TXT_DARKGREEN );
  return 0; // completed successfully
}
