#pragma once

//#include "StdAfx.h"

#include "stdafx.h"
#include "IVS.h"
//#include "Area_adjust.h"
#include "Globals.h"
#include "ChildView.h"
#include "Filling_level.h"
#include "ChildView.h"

//#include "Def_globali.h"

//CArea_adjust* areainsp=NULL; // inspection/search area
extern wchar_t               PC_name[30]; // Computer name read from system configuration

extern Camera_struct         camera  [NUM_TLC];
extern UINT                  lng;
extern CChildView*           pView;
extern CImage_display*       image_dsp[NUM_TLC];
extern UINT                  num_tlc;
extern CParticle_inspection* particle_insp[NUM_TLC];

extern BOOL                  stn1_part_back;     // In case of selectable TRUE/FALSE BACK/BOTTOM LIGHT particle station 1 


extern UINT                  thread_status;

#define  xmin    area->buffrect.left
#define  xmax    area->buffrect.right
#define  ymin    area->buffrect.top
#define  ymax    area->buffrect.bottom

#ifndef COLOR_DETECTION_P1
	#define  PT(x,y) img_acq[x+(y)*camera[index].width]
#else
	#define  PT(x,y) img_gray[x+(y)*camera[index].width]
#endif

//#define  PT(x,y) img_acq[x+(y)*camera[index].width]
//#define FLT(x,y) PT(x,y-6)+PT(x,y+6)-3*PT(x,y)
//extern BOOL      learn_reflections;



CFilling_level::CFilling_level( UINT ind, CFile_mngmt* file_manager_in )
{
  ASSERT( ind>=0 && ind<num_tlc );

  file_manager = file_manager_in;

  index = ind;

  dimx = image_dsp[index]->dim_buf.cx;
  dimy = image_dsp[index]->dim_buf.cy;
  dim  = dimx * dimy;
  
    //Pour added 20180606
	img_gray = (BYTE*) _aligned_malloc( dimx*dimy , 16 ); 
	ASSERT( img_gray && AfxIsValidAddress(img_gray,dimx*dimy) );
	ZeroMemory( img_gray, dimx*dimy );
	
  area_color = COL_MAGENTA; // 0xFFRRGGBB assigned color to draw inspection area

	ASSERT( dimx>0 && (dimx&15) == 0 );
	ASSERT( dimy>0 );

  area = new CArea_adjust( index, file_manager );
  //area->buffrect.top    =   dimy/3;
  //area->buffrect.bottom = 2*dimy/3;
  //area->buffrect.left   =   dimx/3;
  //area->buffrect.right  = 2*dimx/3;

  area->buffrect.top    =   dimy/4;
  area->buffrect.bottom = 2*dimy/4;
  area->buffrect.left   =   dimx/4;
  area->buffrect.right  = 2*dimx/4;

  lum_thr  = 200;
  min_npt  = 200;
  hliv_max = 250;
  hliv_min = 150;
  
  mode = FILLINGL_MODE_REFLECTION; // default mode

  hliv_prc = 0; // alignment/correction from bottom initially disabled
  
  // compensation of bottom height in P2 backlight images enabled without parameter 
  // in menu only for CHANCHUNG machine to keep old recipes
  #if SERIAL_N==201400004 
    hliv_prc = 85;
  #endif

  #ifdef FILLING_BOTTOMCOMP // compensation of bottom height in backlight images
    hliv_prc = 85;
  #endif

	  #ifdef OPACITY_CTRL

    npt_inn = 0;
    lum_inn = 0;
    npt_out = 0;
    lum_out = 0;

    opc_step   =   4; // step for x/y cycles
    opc_vofs   =  60; // distance of outer area compared to lower
    opc_vdim   =  50; // vertical size of opacity area
    opc_mindif = 200; // minimum allowed difference
    opc_maxdif = 600; // maximum allowed difference

  #endif // OPACITY_CTRL

}


CFilling_level::~CFilling_level(void)
{

  if ( area )
  {
    delete area;
    area = NULL;
  }
  
	  if(img_gray)
	  {
	  	_aligned_free(img_gray);
		img_gray = NULL;
	  }

}


int CFilling_level::Set_inspection_area( int ind )
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

  TRACE("CFilling_level::Set_inspection_area(%d)\n", ind);
  return( 0 );
}



int CFilling_level::Add_menu_parameters( CContainer_wnd* child )
{
	//if(IS_PC1)//only for PARTICLE 1
	//{

	  #ifdef OPACITY_CTRL

    pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_2240[lng], child );
    file_manager->Add_separator( string_2240[lng], area );
    pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

    pView->parameter[pView->num_params++] = new CParameter( &opc_vofs,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2268[lng], string_pix[lng],    60,    0,  200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &opc_vdim,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2270[lng], string_pix[lng],    60,    5,  200, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &opc_step,   CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2272[lng], string_pix[lng],     8,    1,   16, file_manager );

    pView->parameter[pView->num_params++] = new CParameter( &opc_maxdif, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2262[lng], string_unit[lng],  200,    0, 9999, file_manager );
    pView->parameter[pView->num_params++] = new CParameter( &opc_mindif, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_2260[lng], string_unit[lng],  200,    0, 9999, file_manager );
   
    
    pView->num_menu_btn++;
  	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );


    return 0; // completed successfully

  #endif
	//}
 

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_600[lng], child );
  file_manager->Add_separator( string_600[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  #ifdef FILLING_BOTTOMCOMP // compensation of bottom height in backlight images
    pView->parameter[pView->num_params++] = new CParameter( &hliv_prc,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_604[lng], string_percent[lng],  85,    0,   100, file_manager );
  #endif

  pView->parameter[pView->num_params++] = new CParameter( &lum_thr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_624[lng], string_log [lng],  100,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng], 1000,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &hliv_max, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_630[lng], string_pix [lng],  700,    0,  double(camera[index].width-2), file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &hliv_min, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_632[lng], string_pix [lng],  600,    0,  double(camera[index].width-2), file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  return 0; // completed successfully

}

int CFilling_level::Add_menu_parameters_f( CContainer_wnd* child )
{

  pView->menu_btn[pView->num_menu_btn] = new CMenu_button( string_600[lng], child );
  file_manager->Add_separator( string_600[lng], area );
  pView->menu_btn[pView->num_menu_btn]->child->new_size( 320, 100 );

  #ifdef FILLING_BOTTOMCOMP // compensation of bottom height in backlight images
    pView->parameter[pView->num_params++] = new CParameter( &hliv_prc,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_604[lng], string_percent[lng],  85,    0,   100, file_manager );
  #endif

  pView->parameter[pView->num_params++] = new CParameter( &lum_thr,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_624[lng], string_log [lng],  100,    0,   255, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &min_npt,  CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_620[lng], string_unit[lng], 1000,    0,  9900, file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &hliv_max, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_630[lng], string_pix [lng],  700,    0,  double(camera[index].width-2), file_manager );
  pView->parameter[pView->num_params++] = new CParameter( &hliv_min, CTYPE_DINT,  0, pView->menu_btn[pView->num_menu_btn]->child, string_632[lng], string_pix [lng],  600,    0,  double(camera[index].width-2), file_manager );

  pView->num_menu_btn++;

	pView->menu_btn[pView->num_menu_btn++] = new CMenu_button( string_2020[lng], pView->menu_btn[pView->num_menu_btn-1]->child, &Set_inspection_area, (int) area ); //index );

  return 0; // completed successfully

}



/*
//Pour added for color version 20180605
// yang add for A20 special config
// if it is color image, we need to convert to RGB8 format 
// here is the converter
int CFilling_level::Process_image(CGrabResultPtr ptr_gr, UINT nimg )
{
	image_dsp[index]->scale_grabresult( ptr_gr ); // scale for display and prepare RGB buffer from BAYER data
	 return Process_image( image_dsp[index]->ptr_buf, nimg );
	
}
*/

int CFilling_level::Process_image( BYTE* img_acq, int nimg )
{

  ASSERT( AfxIsValidAddress( img_acq, dim ) );

  //SCALE_IMG( index, img_acq );

  int x,y;

    #ifdef OPACITY_CTRL

   // if( IS_PC1 )
  //{
	//#ifdef OPACITY_CTRL//Pour changed position on 20201118
    ASSERT( nimg>=0 && nimg<NIMAGES );

    if ( !opc_mindif && !opc_mindif )
    {
      txt.Format( string_2258[lng] );
      col  = TXT_DARKRED;
      good = FALSE;
      return 0;
    }

    if ( nimg==0 ) // first - clear all
    {
      good    = FALSE;
      numimg  = particle_insp[index]->num_img;
      lum_inn = 0;
      lum_out = 0;
      npt_inn = 0;
      npt_out = 0;

      xmaxo = xmin  - opc_vofs;

	//Pour added for Taiji density control on P1
	//#if SERIAL_N==201900005 
	//xmaxo = xmin  + 2*opc_vofs;
	//#endif
	//Pour added for Taiji density control on P1

      if ( xmaxo <      1 ) xmaxo =      1;
      if ( xmaxo > dimx-1 ) xmaxo = dimx-1;

      xmino = xmaxo - opc_vdim;

	//Pour added for Taiji density control on P1
	//#if SERIAL_N==201900005 
	//xmino = xmaxo + 2* opc_vdim;
	//#endif
	//Pour added for Taiji density control on P1


      if ( xmino <      1 ) xmino =      1;
      if ( xmino > dimx-1 ) xmino = dimx-1;

      ymino = ymin;
      if ( ymino <      1 ) ymino =      1;
      if ( ymino > dimy-1 ) ymino = dimy-1;

      ymaxo = ymax;
      if ( ymaxo <      1 ) ymaxo =      1;
      if ( ymaxo > dimy-1 ) ymaxo = dimy-1;
    }

    for ( y=ymin ; y<=ymax ; y+=opc_step )
    {
      for ( x=xmin ; x<=xmax ; x+=opc_step )
      {
        lum_inn += PT(x,y);
        npt_inn++;
      }
    }

    for ( y=ymino ; y<=ymaxo ; y+=opc_step )
    {
      for ( x=xmino ; x<=xmaxo ; x+=opc_step )
      {
        lum_out += PT(x,y);
        npt_out++;
      }
    }

  //  TRACE("nimg[%d]=%d/%d\n",index,nimg,numimg);

    if ( nimg >= numimg-1 ) // last
    {
      col = 0xFF994010;

      //draw area outside liquid
      for ( y=ymino ; y<=ymaxo ; y+=3 )
      {
        DRAW_PT( index, xmino, y, col ); // 0xFFRRGGBB
        DRAW_PT( index, xmaxo, y, col ); // 0xFFRRGGBB
      }
        
      for ( x=xmino ; x<=xmaxo ; x+=3 )
      {
        DRAW_PT( index, x, ymino, col ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymaxo, col ); // 0xFFRRGGBB
      }


      col = 0xFF104099;

      //draw area inside liquid
      for ( y=ymin ; y<=ymax ; y+=3 )
      {
        DRAW_PT( index, xmin, y, col ); // 0xFFRRGGBB
        DRAW_PT( index, xmax, y, col ); // 0xFFRRGGBB
      }
        
      for ( x=xmin ; x<=xmax ; x+=3 )
      {
        DRAW_PT( index, x, ymin, col ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax, col ); // 0xFFRRGGBB
      }



      int avg_inn = npt_inn ? 10*lum_inn/npt_inn : 0;
      int avg_out = npt_out ? 10*lum_out/npt_out : 0;

      int opc_dif = avg_out - avg_inn;

/*      double avg_inn = double(lum_inn)/npt_inn;
      double avg_out = double(lum_out)/npt_out;
      TRACE("Opacity[%d] - npt=%d/%d - avg=%3.1lf/%3.1lf diff=%3.1lf\n", index, npt_out, npt_inn, avg_out, avg_inn, avg_out-avg_inn ); */

//CFilling_level*         fillingl_insp[NUM_TLC]; // filling level inspection

      if ( opc_dif > opc_maxdif )  // difference too big => REJECT liquid too much opaque/dense
      {
        txt.Format( string_2250[lng], opc_dif, opc_mindif, opc_maxdif );
        col  = TXT_DARKRED;
        good = FALSE;
      }
      else
      {
        if ( opc_dif < opc_mindif )  // difference too low => REJECT liquid too little opaque/dense
        {
          txt.Format( string_2252[lng], opc_dif, opc_mindif, opc_maxdif );
          col  = TXT_DARKRED;
          good = FALSE;
        }
        else // difference OK - GOOD
        {
          txt.Format( string_2254[lng], opc_dif, opc_mindif, opc_maxdif );
          col  = TXT_DARKGREEN;
          good = TRUE;
        }
      }

      //TRACE(L"thread[%d] - txt=%s - col=%X\n", index, txt, col );

    }

    return 0;  

	//#endif
//}
  //#endif // OPACITY_CTRL
#else     //if not defined OPACITY_CTRL, and if is PC2
//else
//{         //if not defined OPACITY_CTRL, and if is PC2 
	//if(IS_PC2)//to be changed for different types of machine
	//{
  if ( !hliv_max || !hliv_min ) // disabled
  {
//    if ( nimg==0 ) camera[index].text[2]->SetText( string_634[lng], 0 );
    if ( nimg==0 )
    {
      txt = string_634[lng];
      col = 0;
    }
    good = TRUE;
    return 0; // disabled if either == 0
  }

  //Pour added for color to gray 20180604

  #ifdef COLOR_DETECTION_P1
	//BYTE* img_gray = (BYTE*) _aligned_malloc( dimx*dimy , 16 ); 
	//ASSERT( img_gray && AfxIsValidAddress(img_gray,dimx*dimy) );
	//ZeroMemory( img_gray, dimx*dimy );

	for(int y = 0; y < dimy; ++y)
	{
		BYTE* pline = img_acq + 3*( y*dimx );
		for ( int x = 0; x < dimx; x++)
		{
			float R = *(pline + 3*x);
			float G = *(pline + 3*x+1);
			float B = *(pline + 3*x+2);
			*(img_gray + y* dimx + x) = (BYTE)(R*0.25+G*0.625+B*0.125);
		}
	}
	#endif

  // enabled
  if ( nimg==0 )
  {
    good     = FALSE;
    numimg   = particle_insp[index]->num_img;
    hliv     = 0;
    hliv_tot = 0;
    nliv_tot = 0;

    ASSERT( 0<xmin && xmin<xmax && xmax<dimx );
    ASSERT( 0<ymin && ymin<ymax && ymax<dimy );
  }

  //TRACE("FL[%d] - img=%2d/%2d\n", index, nimg, numimg );

  // calculate average height
  UINT ytot = 0;
  npt  = 0;

  if ( mode&FILLINGL_MODE_REFLECTION || ( mode&FILLINGL_MODE_SELECTABLE && !stn1_part_back ) )
  {

    if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
    {
      for ( y=ymin ; y<=ymax ; y++ )
      {
        for ( x=xmin ; x<=xmax ; x++ )
        {
          if ( PT(x,y)>=lum_thr )
          {
            //DRAW_PT( index, x, y, 0xFF007700 ); // 0xFFRRGGBB
            ytot += y; 
            npt++;
          }
        }
      }
    }
    else // normal // vertical camera
    {
      for ( y=ymin ; y<=ymax ; y++ )
      {
        for ( x=xmin ; x<=xmax ; x++ )
        {
          if ( PT(x,y)>=lum_thr )
          {
            //DRAW_PT( index, x, y, 0xFF007700 ); // 0xFFRRGGBB
            ytot += x;
            npt++;
          }
        }
      }
    }
  }

  if ( mode==FILLINGL_MODE_ADSORPTION || ( mode==FILLINGL_MODE_SELECTABLE && stn1_part_back ) )
  {

    if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
    {
      for ( y=ymin ; y<=ymax ; y++ )
      {
        for ( x=xmin ; x<=xmax ; x++ )
        {
          if ( PT(x,y)<=lum_thr )
          {
            //DRAW_PT( index, x, y, 0xFF007700 ); // 0xFFRRGGBB
            ytot += y;
            npt++;
          }
        }
      }
    }
    else // normal // vertical camera
    {
      for ( y=ymin ; y<=ymax ; y++ )
      {
        for ( x=xmin ; x<=xmax ; x++ )
        {
          if ( PT(x,y)<=lum_thr )
          {
            if ( nimg>=numimg-1 ) DRAW_PT( index, x, y, 0xFF0077AA ); // 0xFFRRGGBB
            ytot += x;
            npt++;
          }
        }
      }
    }
  }


//  int hliv = 0;
  if ( npt>min_npt )
  {
    hliv_tot += int( 0.5 + double(ytot)/npt );
    nliv_tot++;
    //TRACE("FL trovato: hliv=%3d - hliv_tot=%5d - nliv=%2d\n", int( 0.5 + double(ytot)/npt ), hliv_tot, nliv_tot );
//    ASSERT( hliv>=xmin && hliv<=xmax );
  }

  //TRACE("hliv=%d - npt=%d - hliv_min=%d - hliv_max=%d\n", hliv, npt, hliv_min, hliv_max );


  // Last
  if ( nimg>=numimg-1 )
  {
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

    // draw minimum and maximum allowed
/*    for ( y=ymin+10 ; y<=ymax-10 ; y+=4 )
    {
      DRAW_PT( index, dimx-hliv_min, y, 0x0000FF00 ); // 0xFFRRGGBB
      DRAW_PT( index, dimx-hliv_max, y, 0x0000FF00 ); // 0xFFRRGGBB
    } */

   // CString txt;
    ninsp++;

    int hliv_x = 0;  // x for drawing found level

    if ( nliv_tot>numimg/2 ) // required to have found level in at least 50% of images
    {

  //    #if defined FILLING_BOTTOMCOMP || defined PARTICLE_BOTH // if defined also search for bottom (right) height of vial

      if ( hliv_prc && ( ( mode==FILLINGL_MODE_SELECTABLE && stn1_part_back ) || ( mode==FILLINGL_MODE_ADSORPTION ) ) ) // hliv_prc=0 => disabled
      {

        int xbase     = 0;
        int lum_start = 0;
        int lum_bott  = 0;

        if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
        {
          y = dimy-2; 
          for ( x=xmin ; x<=xmax ; x+=4 ) lum_start += PT(x,y);

          // set threshold to hliv_prc
          lum_start = int( 0.5 + 0.01*hliv_prc*lum_start );
          y--; // start from bottom

          lum_bott = lum_start*2;
          while ( lum_bott>lum_start && y>ymin )
          {
            lum_bott = 0;
            for ( x=xmin ; x<=xmax ; x+=4 ) lum_bott += PT(x,y);
            y--;
          }
        }
        else // normal // vertical camera
        {
          x = dimx-2; //xmax;
          for ( y=ymin ; y<=ymax ; y+=4 ) lum_start += PT(x,y);

          // set threshold to 85% (hliv_prc)
          //lum_start = int( 0.5 + 0.85*lum_start );
          lum_start = int( 0.5 + 0.01*hliv_prc*lum_start );
          x--; // start from right/bottom

          lum_bott = lum_start*2;
          while ( lum_bott>lum_start && x>xmin ) // x>xmax ) secondo me era sbagliato!
          {
            lum_bott = 0;
            for ( y=ymin ; y<=ymax ; y+=4 ) lum_bott += PT(x,y);
            x--;
          }
        }


        if ( lum_bott<=lum_start ) // bottom found
        {

          if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
          {

            xbase = y;
            for ( x=xmin ; x<=xmax ; x+=4 ) DRAW_PT( index, x, y, area_color ); // 0xFFRRGGBB

            hliv = xbase - int( 0.5 + double(hliv_tot)/nliv_tot);
            ASSERT( hliv>=0 && hliv<dimy );

            hliv_x = xbase - hliv; // dimx - hliv - xbase;
            ASSERT( hliv_x>=0 && hliv_x<dimy );

            int hlivmax_x = xbase - hliv_max; // dimx - hliv_max - xbase;
            int hlivmin_x = xbase - hliv_min; // dimx - hliv_min - xbase;

            ASSERT( hlivmin_x>=0 && hlivmin_x<dimy );
            ASSERT( hlivmax_x>=0 && hlivmax_x<dimy );

            // draw hliv_min and hliv_max referred to baseline
            // if ( dimx-hliv_min>=0 && dimx-hliv_min<dimx && dimx-hliv_max>=0 && dimx-hliv_max<dimx )
            if ( hlivmin_x>=0 && hlivmin_x<dimy && hlivmax_x>=0 && hlivmax_x<dimy )
            {
              if ( xmin-20>=0 && xmin-20<dimx && xmax+20>=0 && xmax+20<dimx )
              {
                for ( x=2 ; x<=20 ; x+=2 )
                {
                  DRAW_PT( index, xmin-x, hlivmin_x, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, xmax+x, hlivmin_x, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, xmin-x, hlivmax_x, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, xmax+x, hlivmax_x, COL_GREEN ); // 0xFFRRGGBB
                }
              }
            }


          }
          else // normal // vertical camera
          {

            xbase = x;
            for ( y=ymin ; y<=ymax ; y+=4 ) DRAW_PT( index, x, y, area_color ); // 0xFFRRGGBB

            hliv = xbase - int( 0.5 + double(hliv_tot)/nliv_tot);
            //ASSERT( hliv>=0 && hliv<dimx );

            hliv_x = xbase - hliv; // dimx - hliv - xbase;
            //ASSERT( hliv_x>=0 && hliv_x<dimx );

            int hlivmax_x = xbase - hliv_max; // dimx - hliv_max - xbase;
            if ( hlivmax_x<     5 ) hlivmax_x =      5;
            if ( hlivmax_x>dimx-5 ) hlivmax_x = dimx-5;

            int hlivmin_x = xbase - hliv_min; // dimx - hliv_min - xbase;
            if ( hlivmin_x<     5 ) hlivmin_x =      5;
            if ( hlivmin_x>dimx-5 ) hlivmin_x = dimx-5;

            ASSERT( hlivmin_x>=0 && hlivmin_x<dimx );
            ASSERT( hlivmax_x>=0 && hlivmax_x<dimx );

            // draw hliv_min and hliv_max referred to baseline
            // if ( dimx-hliv_min>=0 && dimx-hliv_min<dimx && dimx-hliv_max>=0 && dimx-hliv_max<dimx )
            if ( hlivmin_x>=0 && hlivmin_x<dimx && hlivmax_x>=0 && hlivmax_x<dimx )
            {
              if ( ymin-20>=0 && ymin-20<dimy && ymax+20>=0 && ymax+20<dimy )
              {
                for ( y=2 ; y<=20 ; y+=2 )
                {
                  DRAW_PT( index, hlivmin_x, ymin-y, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, hlivmin_x, ymax+y, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, hlivmax_x, ymin-y, COL_GREEN ); // 0xFFRRGGBB
                  DRAW_PT( index, hlivmax_x, ymax+y, COL_GREEN ); // 0xFFRRGGBB
                }
              }
            }

          }

        }
        else // bottom NOT FOUND
        {
          hliv = 0; // NO BASE LEVEL FOUND
        }

      }
      else // normal version WITHOUT compensation for bottom height
      {
      //      #else  // original version

        if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
        {

          if ( dimy-hliv_min>=0 && dimy-hliv_min<dimy && dimy-hliv_max>=0 && dimy-hliv_max<dimy )
          {
            if ( xmin-20>=0 && xmin-20<dimx && xmax+20>=0 && xmax+20<dimx )
            {
              for ( x=2 ; x<=20 ; x+=2 )
              {
                DRAW_PT( index, xmin-x, dimy-hliv_min, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, xmax+x, dimy-hliv_min, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, xmin-x, dimy-hliv_max, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, xmax+x, dimy-hliv_max, COL_GREEN ); // 0xFFRRGGBB
              }
            }
          }

          hliv = dimy - int( 0.5 + double(hliv_tot)/nliv_tot);
          hliv_x = dimy - hliv;

        }
        else // normal // vertical camera
        {

          if ( dimx-hliv_min>=0 && dimx-hliv_min<dimx && dimx-hliv_max>=0 && dimx-hliv_max<dimx )
          {
            if ( ymin-20>=0 && ymin-20<dimy && ymax+20>=0 && ymax+20<dimy )
            {
              for ( y=2 ; y<=20 ; y+=2 )
              {
                DRAW_PT( index, dimx-hliv_min, ymin-y, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, dimx-hliv_min, ymax+y, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, dimx-hliv_max, ymin-y, COL_GREEN ); // 0xFFRRGGBB
                DRAW_PT( index, dimx-hliv_max, ymax+y, COL_GREEN ); // 0xFFRRGGBB
              }
            }
          }

          hliv = dimx - int( 0.5 + double(hliv_tot)/nliv_tot);
          hliv_x = dimx - hliv;

        }
      }
      //#endif


      if ( hliv<hliv_min ) // too low
      {
        nrej++;
        //Draw_level( dimx-hliv, COL_RED );
        Draw_level( hliv_x, COL_RED );
        //txt.Format( string_636[lng], hliv, hliv_min, hliv_max );
        txt.Format( string_650[lng], hliv, hliv_min, hliv_max, nrej, ninsp );
        col = TXT_DARKRED;
        //camera[index].text[2]->SetText( txt, TXT_DARKRED );
        good = FALSE;
        return 29; // not found into enough images
      }

      if ( hliv>hliv_max ) // too high
      {
        nrej++;
        //Draw_level( dimx-hliv, COL_RED );
        Draw_level( hliv_x, COL_RED );
        //for ( y=ymin ; y<=ymax ; y+=2 ) DRAW_PT( index, dimx-hliv, y, 0x00FF0000 ); // 0xFFRRGGBB
        //txt.Format( string_638[lng], hliv, hliv_min, hliv_max );
        txt.Format( string_652[lng], hliv, hliv_min, hliv_max, nrej, ninsp );
        //camera[index].text[2]->SetText( txt, TXT_DARKRED );
        col = TXT_DARKRED;
        good = FALSE;
        return 39; // not found into enough images
      }

      // Filling level acceptable
      //for ( y=ymin ; y<=ymax ; y+=2 ) DRAW_PT( index, dimx-hliv, y, 0x0000FF00 ); // 0xFFRRGGBB
//      Draw_level( dimx-hliv, COL_GREEN );
      Draw_level( hliv_x, COL_GREEN );
      //txt.Format( string_640[lng], hliv, hliv_min, hliv_max );
      txt.Format( string_654[lng], hliv, hliv_min, hliv_max, nrej, ninsp );
      //camera[index].text[2]->SetText( txt, TXT_DARKGREEN );
      col = TXT_DARKGREEN;
      good = TRUE;
      return 0;


    }
    else // filling level NOT found
    {
      //hliv = 0;
      nrej++;

      double cf = double(ymax-ymin)/(xmax-xmin);
      for ( x=xmin+2 ; x<xmax-2 ; x+=8 )
      {
        int dy = int( cf*(x-xmin) );
        DRAW_PT( index, x, ymin+dy, COL_RED ); // 0xFFRRGGBB
        DRAW_PT( index, x, ymax-dy, COL_RED ); // 0xFFRRGGBB
      }

      //camera[index].text[2]->SetText( string_642[lng], TXT_DARKRED );
      //txt = string_642[lng], TXT_DARKRED;
      txt.Format( string_656[lng], nrej, ninsp );
      col = TXT_DARKRED;

      return 19; // not found into enough images
    }

  }
  /*
  //Pour marked 20180606
	#ifdef COLOR_DETECTION_P1
	if(img_gray)
	{
	_aligned_free(img_gray);
	img_gray=NULL;
	}
	#endif
	//Pour marked 20180606
	*/
  return 0; // completed successfully
  //}
  //} // if not defined OPACITY_CTRL, and if is PC2
    #endif // OPACITY_CTRL
  return 0;
}


void CFilling_level::Draw_level( int x, UINT col )
{

  if ( mode&FILLINGL_MODE_HORIZONTAL ) // if camera are horizontal
  {

    if ( x<2 || x>=dimy-2 ) return;

    for ( int y=xmin+2 ; y<=xmax-2 ; y+=2 )
    {
      int ofs = BUF2OFS(index,y,x); // x and y swapped
      SETPIXELOFS(index,ofs-DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs             ,col       ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs+DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
    }
  
  }
  else // normal // vertical camera
  {

    if ( x<2 || x>=dimx-2 ) return;

    for ( int y=ymin+2 ; y<=ymax-2 ; y+=2 )
    {
      int ofs = BUF2OFS(index,x,y);
      SETPIXELOFS(index,ofs-DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs             ,col       ); // 0xFFRRGGBB
      SETPIXELOFS(index,ofs+DSPDX(index),COL_BLACK ); // 0xFFRRGGBB
    }

  }

}
