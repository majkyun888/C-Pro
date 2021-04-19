#pragma once

#include "stdafx.h"
#include "hpdf.h"
#include "Recipe_manager.h"
#include "Globals.h"
#include "Def_globali.h"
#include "File_mngmt.h"

//Pour added 20190328
#include "afx.h"

/*
#include "afx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"
*/



#define TEXT_MODE_LEFT      1
#define TEXT_MODE_CENTER    2
#define TEXT_MODE_RIGHT     4

#define TEXT_MODE_LEFTRIGHT 8 // two texts: one to left, one to right

#define VERT_STEP          15
#define VERT_STEP2         25
#define POS_X_ZERO         80
#define ALIGN_LEFT        150
#define ALIGN_RIGHT       410
#define N_MAX_PAGE         50
#define EDGE              110
#define MAXLEN_BATCH_TXT  100

#define MAX_RECIPE_LEN      50


class CBatch_manager : public CDialog
{
	DECLARE_DYNAMIC(CBatch_manager)



public:
	CBatch_manager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBatch_manager();

// Dialog Data
	enum { IDD = IDD_BATCH_DIALOG };

  int read_string( wchar_t* buffer, wchar_t* caption );

  int Load_data(void);
  int Save_data(void);
  int Create_report(void);
  int Grippers_statistics(void);

   //Pour added 20190328

  //===============
  int  Create_HGA_report(int lang); // make pdf format HGA data  
  int  Create_Log_pdf( int lang ,int type); // make pdf format Log data
  int  Create_Log_pdf( int lang, CTime &save_time); // make pdf format log data,xinchang 
  int  Create_AlarmLog_pdf( int lang, CTime &save_time); // make pdf format log data,xinchang //Pour added 20190222
  int  Txt2pdf();
  //Pour added 20190220 for transfer alarm log
  int  Txt2pdf_alarm();
  //===============

  //Pour added 20190328


  struct batch_data
  {
    int      status;                   // 0=not running - 1=batch active
    wchar_t  recipe[MAX_RECIPE_LEN+1]; // current recipe name
    wchar_t  oper[MAXLEN_BATCH_TXT+1];
    wchar_t  name[MAXLEN_BATCH_TXT+1];
    wchar_t  code[MAXLEN_BATCH_TXT+1]; 
    wchar_t  desc[MAXLEN_BATCH_TXT+1];
    CTime    start_time;
    CTime    end_time;
  } current;

protected:

  HPDF_Doc  pdf;
  HPDF_Font font;
  HPDF_Page page;

  char  current_code[100];

  char  file_name[100];
  char  full_name[100];

  int   num_pages;
  int   pdf_lng;     // pdf language

  // strings to store font name and encoding used for pdf 
//  char  fontname[50];
//  char  encoding[50];
  char* fontname;
  char* encoding;
  UINT  codepage;
//  char  encoding[100];

  float posx;
  float posy;
  float page_dimx;
  float page_dimy;
  float left_margin;
  float right_margin;
  float top_margin;
  float bottom_margin;
  float row_height;

  //Pour added 20180903
  float line_length;


  int   n_prod_in;


  char *time2txt( CTime* time, int format=0 );

  int  Create_pdf( int lang );
  int  Create_grippers_report( int lang );

  int  Add_statistic( char* varname, char**    string );
  int  Add_statistic( char* varname, wchar_t** string );

  int  gripper_read( char* varname,  char* string, UINT* totisp, UINT* totrej );


  int  Add_text( const char* txt,  int mode );
  int  Add_text( const char* txt1, const char* txt2, int mode );

  int  Add_text( const wchar_t* txt,  int mode );
  int  Add_text( const wchar_t* txt1, const char* txt2, int mode );
  int  Add_text( const wchar_t* txt1, const wchar_t* txt2, int mode );

//  int  Add_text( const wchar_t* txt, int mode );
  int  Add_new_page( int npg );

  //Pour added 20190328
  // --------------------
  //new code for HGA and LOG 
  int  Add_HGA_new_page( int npg,int tpg );
  int  Add_Log_new_page( int npg );
  //---------------------

  //Pour added 20190328


  int  Create_page_0   ( void );
//  int  Create_page_0_LQ( void );
//  int  Create_page_0_FD( void );



  void DrawPageNumber ( HPDF_Page page, float x, float y, const char* label );
  //void InsertBatchData( HPDF_Page page, char* txtL, char* txtR, int dim, int nDec, int posY);
  //void GetDataAndTimeFromFile( char* deststring );
  

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual void OnOK();
  virtual void OnCancel();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnStnClickedBatchcodeVal();
  afx_msg void OnStnClickedBatchoperVal();
  afx_msg void OnStnClickedBatchnameVal();
  afx_msg void OnStnClickedBatchdescVal();
};
