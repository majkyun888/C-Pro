#include "StdAfx.h"
#include "PrintToPDF.h"
#include "childview.h"
#include "LoadPdfFile.h"

extern LoadPdfFile*       pdf_wnd;

#define MAX_LOG_STRING_LENGTH 100

jmp_buf env;


extern CParameter* num_goods;
extern CParameter* num_prod_in;
extern CParameter* num_rejects;
extern CParameter* num_1stpart_rej;
extern CParameter* num_2ndpart_rej;
extern CParameter* num_filllev_rej;
extern CParameter* num_filllev2_rej;
extern CParameter* num_cosmetic_rej;
extern CParameter* num_cap_rej;
extern CParameter* num_carousel_rej;
extern CParameter* num_capcolor_rej;

#ifdef CFR21
extern char User_str[MAX_LOG_STRING_LENGTH];
#endif


void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%d\n", 
      (unsigned int) error_no, (int) detail_no);
    longjmp (env, 1); /* invoke longjmp() on error */
}

CPrintToPDF::CPrintToPDF(void)
{
  /*  pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        TRACE ("error: cannot create PdfDoc object\n");
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        //return 1;
    }
   nRow=1;

   TRACE ("< Creato pdf > \n"); */
}

CPrintToPDF::~CPrintToPDF(void)
{
    /* clean up */
   //if (pdf) HPDF_Free (pdf);  
    
}



void CPrintToPDF::DrawPageNumber ( HPDF_Page page, float x, float y, const char* label )
{
  HPDF_Page_SetLineWidth (page, 0.5);
  HPDF_Page_SetFontAndSize (page, font, 6);

  HPDF_Page_BeginText (page);
  HPDF_Page_MoveTextPos (page, (HPDF_Page_GetWidth(page)/2), y - 10);
  HPDF_Page_ShowText (page, label);
  HPDF_Page_EndText (page);

  HPDF_Page_MoveTo (page, x, y);
  HPDF_Page_LineTo (page, x + 400, y );
  HPDF_Page_Stroke (page);

}

/*
void CPrintToPDF::AddText ( HPDF_Page page, char* text , int dim_char , int posY )
{
  HPDF_Page_BeginText (page);
  HPDF_Page_SetFontAndSize (page, font, (HPDF_REAL)dim_char );
  HPDF_Page_TextOut (page, POS_X_ZERO , (HPDF_REAL)posY, text);
  HPDF_Page_EndText (page);
  nRow++;
 
}
*/

/*
void CPrintToPDF::InsertBatchData( HPDF_Page page, char* txtL, char* txtR, int dim, int nDec, int posY )
{
  int i = 0;
  HPDF_Page_BeginText (page);
  HPDF_Page_SetFontAndSize (page, font, (HPDF_REAL)dim );
  HPDF_Page_TextOut (page, ALIGN_LEFT , (HPDF_REAL)posY, txtL);

  //dopo la virgola
  int lung = strlen (txtR);
  int lenght = lung;
  int posDec=0;
  char* txtR_tmp = (char*) malloc ( lung+nDec+1+(lung/3) );
  int cDec=0;
  BOOL decOK = false;
  for ( i=strlen(txtR)-1; i>=0; i--)
  {
    if ( (txtR[i]=='.') || (txtR[i]==',') )
    {
      posDec=i;
      if ( cDec<nDec )
      {
        int diff = nDec-cDec, j=0;
        while ( j<diff )
        {
          txtR_tmp[lung+j]='0';
          lung++;
          j++;
        }
      }else
        if ( cDec>nDec )
         lung -= cDec-nDec;
    decOK = true;
    }
    if (!decOK) cDec++;
    txtR_tmp[i]=txtR[i];
  }//for
  
  txtR_tmp[strlen(txtR)] = 0;
  if ( !decOK && nDec>0 )
    {
      int j=0;
      txtR_tmp[lung]=',';
      posDec=lung;
      lung++;
      while ( j<nDec )
        {
          txtR_tmp[lung]='0';
          lung++;
          j++;
        }
     decOK = true;
    }

  int nPunt= posDec/3;
  lung += nPunt;

  int Pos1= ( posDec ) % 3;

  char *txt2 = (char*) malloc ( lung );
  int j=0;

  if (nPunt==0) 
  {
    strncpy ( txt2, txtR_tmp, posDec );
    j = posDec;  
  }
  else
  {
    for (int i=0; i<posDec; i++)
    {
      txt2[j]=txtR_tmp[i];
      j++;
    }
  }
  //copio la fine
  for (int i=posDec; i<lenght; i++)
  {
    txt2[j]=txtR[i];
    j++;
  }
 
  if(nDec)
   txt2[lenght] = 0; // +1
  else
    txt2[lenght] = 0;

  int pos = (int)HPDF_Page_TextWidth(page, txtR_tmp);
  
  for (int i=ALIGN_LEFT+(int)HPDF_Page_TextWidth(page, txtL)+2; i<ALIGN_RIGHT-pos-2; i++)
      HPDF_Page_TextOut (page, (HPDF_REAL)i, (HPDF_REAL)posY, "-");

  HPDF_Page_TextOut (page, ALIGN_RIGHT-(HPDF_REAL)pos, (HPDF_REAL)posY, txt2);
  HPDF_Page_EndText (page);
  
  nRow++;
  free (txtR_tmp);
  //free (txt2);
}

*/



/*
void CPrintToPDF::AddDataAndTime( HPDF_Page page, int xPos, int yPos )
{
  HPDF_Page_BeginText (page);
  HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 60);
  nRow++;
  //DATA
  SYSTEMTIME time;
	//GetSystemTime( &time );
  GetLocalTime(&time);
  char SetDataOra[20];
  sprintf( SetDataOra, "%02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute );

  HPDF_Page_SetFontAndSize(page, font, 10);
  HPDF_Page_MoveTextPos ( page,  (HPDF_REAL)xPos - HPDF_Page_TextWidth (page, SetDataOra), (HPDF_REAL)yPos ); 
  HPDF_Page_ShowText (page, SetDataOra);
  HPDF_Page_EndText (page);
}


void CPrintToPDF::AddLogo ( HPDF_Page page, HPDF_Image img )
{
  //LOGO
  HPDF_Point size = HPDF_Image_GetSize(img); 
  while ( (size.x>200) || (size.y>50) ) {
    size.x /=2; size.y /=2;
    }
  //TRACE ( "-----------> dimensioni logo: %d x %d \n", (int)size.x, (int)size.y ); 
  HPDF_Page_DrawImage (page, img, 50, HPDF_Page_GetHeight (page)-50, size.x, size.y);
}
*/











int CPrintToPDF::Add_text( const char* txt, int mode ) 
{
  float x, y;

  switch ( mode )
  {
    case TEXT_MODE_LEFT:
      x = posx;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_CENTER:
//      x = left_margin+(page_dimx-right_margin-HPDF_Page_TextWidth(page,txt))/2;
      x = (left_margin+right_margin)/2 - HPDF_Page_TextWidth(page,txt)/2;
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    case TEXT_MODE_RIGHT:
//      x = page_dimx - right_margin - HPDF_Page_TextWidth( page, txt );
      x = right_margin - HPDF_Page_TextWidth( page, txt ) - (posx-left_margin);
      y = posy;
      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut  (page, x, y, txt );
      HPDF_Page_EndText  (page);
      break;

    default:
      TRACE("CBatch_management::Add_text called with unknown mode\n");

  }
//  posy -= 25; //HPDF_Page_TextHeight( page, txt );

  posy -= row_height; //HPDF_Page_TextHeight( page, txt );

  return 0;

}





int CPrintToPDF::CreateFile ( const char* fname )
{

  SYSTEMTIME time;
	GetLocalTime(&time);
  char text[200];
  char temp[200];

  int n_page = 0;

  HPDF_Font font;

//  CFileFind findFile;

  pdf = HPDF_New (error_handler, NULL);
  if (!pdf) 
  {
    TRACE ("error: cannot create PdfDoc object\n");
    return 1001;
  }

  if ( setjmp(env) )
  {
    HPDF_Free (pdf);
    return 1002;
  }

  // set compression mode 
  HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

  UINT n_prod_in      =   100000;
  UINT n_goods        =    90000;

  UINT n_rejects      =    10000;
  UINT n_rejects1     =     6000;
  UINT n_rejects2     =     6000;

  UINT n_1stpart_rej  =     1111;
  UINT n_2ndpart_rej  =     2222;
  UINT n_filllev1_rej =     3333;
  UINT n_filllev2_rej =     4444;
  UINT n_cosmetic_rej =     5555;
  UINT n_cap_rej      =     6666;
  UINT n_capcolor_rej =     7777;
//  UINT n_spin_rej     =     8888;

  UINT n_camera_rej[12];
  ZeroMemory( n_camera_rej, sizeof( n_camera_rej ) );

  ADS_data_type  camera_rejects, rejects1, rejects2;


  if ( ADS_status )
  {
    n_prod_in      =  *((UINT*)num_prod_in->val); 
    n_goods        =  *((UINT*)num_goods->val);
    n_rejects      =  *((UINT*)num_rejects->val);
    n_1stpart_rej  =  *((UINT*)num_1stpart_rej->val);
    n_2ndpart_rej  =  *((UINT*)num_2ndpart_rej->val);
    n_filllev1_rej =  *((UINT*)num_filllev_rej->val);
    n_filllev2_rej =  *((UINT*)num_filllev2_rej->val);
    n_cosmetic_rej =  *((UINT*)num_cosmetic_rej->val);
    n_cap_rej      =  *((UINT*)num_cap_rej->val);
    n_capcolor_rej =  *((UINT*)num_capcolor_rej->val);
//    n_spin_rej     =  *((UINT*)num_capcolor_rej->val);

    if ( !ADS_init_parameter( ".num_rejects_cam", &camera_rejects, 12*4 ) && !ADS_read_parameter( &camera_rejects ) )
      CopyMemory( n_camera_rej, camera_rejects.val, camera_rejects.dim );

    if ( !ADS_init_parameter( ".num_rejects1", &rejects1, 4 ) && !ADS_read_parameter( &rejects1 ) )
      n_rejects1 =  *((UINT*)rejects1.val);

    if ( !ADS_init_parameter( ".num_rejects2", &rejects2, 4 ) && !ADS_read_parameter( &rejects2 ) )
      n_rejects2 =  *((UINT*)rejects2.val);
  }

  char dateandtime[100];
  sprintf( dateandtime, "%02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute );
   
  for ( int npage=0 ; npage<2 ; npage++ )
  {

//  const char *page_title =  "Batch Report";

    page = HPDF_AddPage (pdf);
    HPDF_Page_SetSize ( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT ); 

    page_dimx = HPDF_Page_GetWidth (page);
    page_dimy = HPDF_Page_GetHeight(page);

    left_margin   = page_dimx/20;
    right_margin  = page_dimx - left_margin;
    top_margin    = page_dimy*25/26;
    bottom_margin = page_dimy - top_margin;
    row_height    = 30;


    TRACE ("<Page dimension: %f x %f >\n", page_dimx, page_dimy );
    TRACE ("<Page margins: %f,%f %f,%f >\n", left_margin, top_margin, right_margin, bottom_margin );

    HPDF_Page_SetLineWidth (page, 0.5);
    HPDF_Page_Rectangle (page, left_margin, top_margin, right_margin-left_margin, bottom_margin-top_margin );
    HPDF_Page_Rectangle( page, left_margin, top_margin, right_margin-left_margin, -60 );
//  HPDF_Page_Rectangle( page, left_margin, (page_dimy-top_margin-20), page_dimx-right_margin-left_margin, page_dimy );
    HPDF_Page_Stroke (page);

    posx = left_margin + 10;
    posy = top_margin - 24; // Start from top of page

    font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 18 );

    sprintf ( text, "BATCH REPORT" ); 
    Add_text( text, TEXT_MODE_CENTER );

    font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 12 );
    posy += 10;

    sprintf ( text, "NIRMA HEALTHCARE - AUTOMATIC INSPECTION MACHINE ARGO 60-39-9" ); 
    Add_text( text, TEXT_MODE_CENTER );


    font = HPDF_GetFont (pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize (page, font, 12 );
    row_height = 20;
    posy -= 50;

    WideCharToMultiByte( CP_ACP, 0, pView->recipe_name, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Recipe              : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );

    sprintf ( text, "Machine I.D. No.    : %s", "ARGO 60-39-9" ); 
    Add_text( text, TEXT_MODE_LEFT );

    WideCharToMultiByte( CP_ACP, 0, pView->batch_name, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Product name        : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );

    WideCharToMultiByte( CP_ACP, 0, pView->batch_description, -1, temp, 256, NULL, NULL);
    sprintf ( text, "Batch No.           : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );

    sprintf ( text, "No. of containers   : %d", n_prod_in ); 
    Add_text( text, TEXT_MODE_LEFT );

    GetDataAndTimeFromFile( temp );
    sprintf ( text, "Started On          : %s", temp ); 
    Add_text( text, TEXT_MODE_LEFT );

    sprintf ( text, "Ended On            : %02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute); 
    Add_text( text, TEXT_MODE_LEFT );

    sprintf ( text, "User logged in      : %s", User_str );
    Add_text( text, TEXT_MODE_LEFT );

    posy -= 10;
    sprintf ( text, "Pkg. chemist, Name  : _____________________ Sign ____________________" );
    Add_text( text, TEXT_MODE_LEFT );

    posy -= 10;
    sprintf ( text, "Q.A. chemist, Name  : _____________________ Sign ____________________" );
    Add_text( text, TEXT_MODE_LEFT );

    posy -= 70;

    if ( npage==0 )
    {

      sprintf( text, "OVERALL PRODUCTION STATISTICS" );
      Add_text( text, TEXT_MODE_LEFT );
      posy -= 10;

      if ( n_prod_in == 0 )
      {
        sprintf( text, "NO PROCESSED PRODUCT" );
        Add_text( text, TEXT_MODE_LEFT );
      }
      else
      {
        sprintf( text, "Processed products ________________________ %8d pcs   (%8.3lf%%)", n_prod_in,      100.0 );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Accepted products  ________________________ %8d pcs   (%8.3lf%%)", n_goods,        100.0*n_goods/n_prod_in        );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Rejected products  ________________________ %8d pcs   (%8.3lf%%)", n_rejects,      100.0*n_rejects/n_prod_in      );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Products rejected on tray 1 _______________ %8d pcs   (%8.3lf%%)", n_rejects1,     100.0*n_rejects1/n_prod_in     );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Products rejected on tray 2 _______________ %8d pcs   (%8.3lf%%)", n_rejects2,     100.0*n_rejects2/n_prod_in     );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Particle 1 rejects ________________________ %8d pcs   (%8.3lf%%)", n_1stpart_rej,  100.0*n_1stpart_rej/n_prod_in  );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Filling level 1 rejects ___________________ %8d pcs   (%8.3lf%%)", n_filllev1_rej, 100.0*n_filllev2_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Particle 2 rejects ________________________ %8d pcs   (%8.3lf%%)", n_2ndpart_rej,  100.0*n_2ndpart_rej/n_prod_in  );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Filling 2 level rejects ___________________ %8d pcs   (%8.3lf%%)", n_filllev2_rej, 100.0*n_filllev2_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Cosmetic rejects __________________________ %8d pcs   (%8.3lf%%)", n_cosmetic_rej, 100.0*n_cosmetic_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Tip/cap rejects ___________________________ %8d pcs   (%8.3lf%%)", n_cap_rej,      100.0*n_cap_rej/n_prod_in      );
        Add_text( text, TEXT_MODE_LEFT );

        sprintf( text, "Flip-off color rejects ____________________ %8d pcs   (%8.3lf%%)", n_capcolor_rej, 100.0*n_capcolor_rej/n_prod_in );
        Add_text( text, TEXT_MODE_LEFT );

      }

    }

    if ( npage==1 )
    {
      for ( int i=0 ; i<12 ; i++ )
      {

        if ( i==0 )
        {
          posy -= 10;
          Add_text( "PC1 - TIP&CAP INSPECTION STATION", TEXT_MODE_LEFT );
        }
        if ( i==3 )

        {
          posy -= 10;
          Add_text( "PC1 - COSMETIC INSPECTION STATION", TEXT_MODE_LEFT );
        }

        if ( i==6 )
        {
          posy -= 10;
          Add_text( "PC2 - FIRST PARTICLE&FILLING LEVEL INSPECTION STATION", TEXT_MODE_LEFT );
        }

        if ( i==9 )
        {
          posy -= 10;
          Add_text( "PC2 - SECOND PARTICLE&FILLING LEVEL INSPECTION STATION", TEXT_MODE_LEFT );
        }


        sprintf ( text, "Camera no.%2d rejects ____________________________ %6d pcs", i+1, n_camera_rej[i] );
        Add_text( text, TEXT_MODE_LEFT );
      }
    }



    font = HPDF_GetFont (pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize (page, font, 10 );
    posy = bottom_margin-12;
    sprintf ( text, "Page %d/%d", npage+1, 2 );
    Add_text( text, TEXT_MODE_LEFT );
    posy += row_height;
    sprintf ( text, "Date&time %s", dateandtime );
    Add_text( text, TEXT_MODE_RIGHT );


  }





  // save the document to a file 
  HPDF_SaveToFile( pdf, fname );
  
  HPDF_Free (pdf); 
    
  return 0;

}


/*  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow*1.5f;

  font = HPDF_GetFont (pdf, "Helvetica", NULL);
  HPDF_Page_SetFontAndSize (page, font, 10 );

  sprintf ( text, "NIRMA HEALTHCARE - AUTOMATIC INSPECTION MACHINE ARGO 60-39-9", "HPDF_TALIGN_CENTER" ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;
  ypos -= yrow;

  font = HPDF_GetFont (pdf, "Courier", NULL);
  HPDF_Page_SetFontAndSize (page, font, 12 );

  WideCharToMultiByte( CP_ACP, 0, pView->recipe_name, -1, temp, 256, NULL, NULL);
  sprintf ( text, "Recipe                       : %s", temp ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  sprintf ( text, "Machine I.D. No.             : %s", "ARGO 60-39-9" ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  WideCharToMultiByte( CP_ACP, 0, pView->batch_name, -1, temp, 256, NULL, NULL);
  sprintf ( text, "Product name                 : %s", temp ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  WideCharToMultiByte( CP_ACP, 0, pView->batch_description, -1, temp, 256, NULL, NULL);
  sprintf ( text, "Batch No.                    : %s", temp ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  sprintf ( text, "No. of containers            : %d", 123456 ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  
  sprintf ( text, "Started On                   : %02d/%02d/%04d - %02d:%02d", 1, 1, 2012, 12, 34 ); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;

  sprintf ( text, "Ended On                     : %02d/%02d/%04d - %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute); 
  HPDF_Page_BeginText (page);
  HPDF_Page_TextOut (page, xpos, ypos, text);
  HPDF_Page_EndText (page);
  ypos -= yrow;
  

  */


  /*
  strtok(temp, ".");
  strcpy( text1, "Recipe: ");
  strcat( text1, temp);
  AddText ( page, text1,  15, yPos-nRow*VERT_STEP);
   
  strcpy( text1, "Product name: ");
  WideCharToMultiByte( CP_ACP, 0, pView->batch_name, -1, temp, 256, NULL, NULL);
  strcat( text1, temp);
  AddText ( page, text1,  15, yPos-nRow*VERT_STEP);

  strcpy( text1, "Batch no.");
  WideCharToMultiByte( CP_ACP, 0, pView->batch_description, -1, temp, 256, NULL, NULL);
  strcat( text1, temp); 
  AddText ( page, text1,  15, yPos-nRow*VERT_STEP);

    
  #ifdef CFR21

    sprintf( text1, "User: ");
    strcat( text1, User_str);
    AddText ( page, text1,  15, yPos-nRow*VERT_STEP);

    // Data inizio
    sprintf( text1, "Started on: ");
    GetDataAndTimeFromFile( temp );
    strcat( text1, temp );
    AddText ( page, text1,  15, yPos-nRow*VERT_STEP);

    //Data fine
    char SetDataOra[100];
    sprintf( SetDataOra, "%02d/%02d/%04d  %02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute );
    sprintf( text1, "Stopped on: ");
    strcat( text1, SetDataOra );
    AddText ( page, text1 ,  15, yPos-nRow*VERT_STEP);

  #endif

  */


    /*
  num_prod_in->Read_PLC_val();
	UINT prod_in = *((UINT*)num_prod_in->val);
     
	sprintf( text1, "%d", prod_in );
  InsertBatchData( page, "Processed products:", text1, 10, 0, yPos-nRow*VERT_STEP );
	 
  num_goods->Read_PLC_val();
	UINT goods = *((UINT*)num_goods->val);
  
    
  if(prod_in) sprintf( text1, "%d (%4.2lf%%)", goods, 100.0*goods/prod_in );
  else        sprintf( text1, "%d", goods);

  if(goods)   InsertBatchData( page, "Good Products", text1, 10, 1, yPos-nRow*VERT_STEP );
  else        InsertBatchData( page, "Good Products", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_1stpart_rej->Read_PLC_val();
  UINT n1stpart_rej = *((UINT*)num_1stpart_rej->val);
    
  if(prod_in) sprintf( text1, "%d (%4.2lf%%)", n1stpart_rej, 100.0*n1stpart_rej/prod_in );
  else  		  sprintf( text1, "%d", n1stpart_rej );

  if( prod_in ) InsertBatchData( page, "First particle rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else          InsertBatchData( page, "First particle rejects", text1, 10, 0, yPos-nRow*VERT_STEP );
		
	num_2ndpart_rej->Read_PLC_val();
	UINT n2ndpart_rej = *((UINT*)num_2ndpart_rej->val);
  
  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", n2ndpart_rej, 100.0*n2ndpart_rej/prod_in );
  else   		   sprintf( text1, "%d", n2ndpart_rej );
		
  if ( prod_in ) InsertBatchData( page, "Second particle rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else           InsertBatchData( page, "Second particle rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_filllev_rej->Read_PLC_val();
 	UINT nfilllev_rej = *((UINT*)num_filllev_rej->val);

  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", nfilllev_rej, 100.0*nfilllev_rej/prod_in );
  else         sprintf( text1, "%d", nfilllev_rej );
    
  if( prod_in ) InsertBatchData( page, "Fill level 1 rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else          InsertBatchData( page, "Fill level 1 rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_filllev2_rej->Read_PLC_val();
  UINT nfilllev2_rej = *((UINT*)num_filllev2_rej->val);

  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", nfilllev2_rej, 100.0*nfilllev2_rej/prod_in );
  else         sprintf( text1, "%d", nfilllev2_rej );

  if ( prod_in ) InsertBatchData( page, "Fill level 2 rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else           InsertBatchData( page, "Fill level 2 rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_cap_rej->Read_PLC_val();
	UINT ncap_rej = *((UINT*)num_cap_rej->val);  

  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", ncap_rej, 100.0*ncap_rej/prod_in );
  else         sprintf( text1, "%d", ncap_rej );
		
  if( prod_in ) InsertBatchData( page, "Cap rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else          InsertBatchData( page, "Cap rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_cosmetic_rej->Read_PLC_val();
	UINT ncosmetic_rej = *((UINT*)num_cosmetic_rej->val);

  if(prod_in) sprintf( text1, "%d (%4.2lf%%)", ncosmetic_rej, 100.0*ncosmetic_rej/prod_in );
  else     	  sprintf( text1, "%d", ncosmetic_rej );
		
  if ( prod_in ) InsertBatchData( page, "Cosmetic rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else           InsertBatchData( page, "Cosmetic rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_capcolor_rej->Read_PLC_val();
  UINT ncapcolor_rej = *((UINT*)num_capcolor_rej->val);

  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", ncapcolor_rej, 100.0*ncapcolor_rej/prod_in );
  else         sprintf( text1, "%d", ncapcolor_rej );
		
  if( prod_in ) InsertBatchData( page, "Cap Colour rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else          InsertBatchData( page, "Cap Colour rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  num_carousel_rej->Read_PLC_val();
  UINT ncarousel_rej = *((UINT*)num_carousel_rej->val);
    
  if (prod_in) sprintf( text1, "%d (%4.2lf%%)", ncarousel_rej, 100.0*ncarousel_rej/prod_in );
  else         sprintf( text1, "%d", ncarousel_rej );
		
  if ( prod_in ) InsertBatchData( page, "Carousel rejects", text1, 10, 1, yPos-nRow*VERT_STEP );
  else           InsertBatchData( page, "Carousel rejects", text1, 10, 0, yPos-nRow*VERT_STEP );

  */

 // HPDF_Page_EndText (page);


void CPrintToPDF::GetDataAndTimeFromFile( char* deststring )
{
  FILE* fp;
  char temp[100];
  
  fp = fopen( "lotto.txt", "r");
  if(fp == NULL)
    strcpy(deststring, "");
  else
  {
    fgets(temp, 100, fp);
    fgets(temp, 100, fp);
    fgets(temp, 100, fp);
    strcpy(deststring, temp);
    fgets(temp, 100, fp);
    strcat(deststring, " ");
    strcat(deststring, temp);
    fclose( fp );
  }

  return;
}