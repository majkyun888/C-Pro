#pragma once

#include "stdafx.h"
#include "Globals.h"
#include "Def_globali.h"
#include "Area_adjust.h"


class CParameter;
class CArea_adjust;

#define MAX_NUM_DATA    90
#define RECT_TYPE      128


class CFile_mngmt
{
public:

  CFile_mngmt( wchar_t* name_in = L"default.par" );
	~CFile_mngmt(void);

	struct data_type
	{
		void* ptr;  // pointer to data to be loaded/saved
		UINT  size; // data dimensions in bytes
		UINT  type; // data dimensions in bytes
	} data[MAX_NUM_DATA];

	struct separator_type
	{
		wchar_t*      text;  // pointer to description text
		UINT          index; // index in the sequence
    CArea_adjust* area;  // if not null, ptr to inspection area
	} separator[MAX_NUM_DATA];

  CParameter* ptr[MAX_NUM_PARAMS];

  UINT        num_separators;
  UINT        num_data;
	UINT        num_params;
	UINT        dim_file; // Dimensione attesa del file

	wchar_t*    file_name;
  wchar_t     msg_err[255];
  wchar_t     recipe_name[50+1];

    int Clear_all( void );

	int Add_data( CParameter* par_in );
  int Add_data( void* data_in, UINT size_in, UINT type_in=0 );
  
//  int Add_separator( wchar_t* text_in );
  int Add_separator( wchar_t* text_in, CArea_adjust* area=NULL );

	int Save_all( wchar_t* name = NULL );
	int Load_all( wchar_t* name = NULL );
  int Save_text( wchar_t* name, int lang );

};

