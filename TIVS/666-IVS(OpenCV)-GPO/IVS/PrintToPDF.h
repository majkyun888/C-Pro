#pragma once
#include "afx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"


#define TEXT_MODE_LEFT      1
#define TEXT_MODE_CENTER    2
#define TEXT_MODE_RIGHT     4


#define VERT_STEP     15
#define VERT_STEP2    25
#define POS_X_ZERO    80
#define ALIGN_LEFT    150
#define ALIGN_RIGHT   410
#define N_MAX_PAGE    50
#define EDGE          110


class CPrintToPDF 
{

  private:
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;


    float posx;
    float posy;
    float page_dimx;
    float page_dimy;
    float left_margin;
    float right_margin;
    float top_margin;
    float bottom_margin;
    float row_height;


//    int nRow;

public:
    CPrintToPDF     ( void );
    ~CPrintToPDF    ( void );

    int  CreateFile       ( );
    int  CreateFile ( const char* fname );

private:
    //void AddText          ( HPDF_Page page, char *text , int dim_char , int pos );

    int  Add_text( const char* txt, int mode );

    void DrawPageNumber   ( HPDF_Page page, float x, float y, const char* label );
   // void AddDataAndTime   ( HPDF_Page page, int xPos, int yPos );
   // void AddLogo          ( HPDF_Page page, HPDF_Image img);
    void InsertBatchData( HPDF_Page page, char* txtL, char* txtR, int dim, int nDec, int posY);
    void GetDataAndTimeFromFile( char* deststring );

};



