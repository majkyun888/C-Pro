#pragma once

#include "stdafx.h"
#include "File_mngmt.h"


#define NUM_ROWS            10
#define NUM_COLUMNS          3
#define NUM_RECIPES      (NUM_ROWS*NUM_COLUMNS)
#define RECIPES_NID      13500
#define MAX_RECIPE_LEN      50
#define MAX_NUM_RECIPES   1000



class CRecipe_manager : public CDialog
{
	DECLARE_DYNAMIC(CRecipe_manager)

public:
	CRecipe_manager(CWnd* pParent = NULL, CFile_mngmt* file_manager_in=NULL );   // standard constructor
	virtual ~CRecipe_manager();


	CFont       font;
//	CFile_mngmt*  file_mgr;

  CRect       par_area; // parent window area
	CRect       cli_area; // client area

	CButton*    button[NUM_RECIPES];
 // CFile_mngmt* file_manager;

//	static    UINT nID;
	UINT       nID;
	
//  char      name[NUM_RECIPES][MAX_NAME_LENGHT+1]; // Contiene i nomi delle ricette (salvato/caricato)

	int        selected;          // ricetta attualmente selezionata (-1 nessuna!)
  int        selected_old;
//  char      selected_name[MAX_NAME_LENGHT+1];
 
  int        num_page;  // current page number
  int        num_pages; // total pages number
  int        num_files; // total number of files

  wchar_t    recipe_name[MAX_RECIPE_LEN+1];
  wchar_t    full_recipe_name[MAX_RECIPE_LEN+1];


//  CString   filename[NUM_RECIPES];
  CString**  filename;

// Dialog Data
	enum { IDD = IDD_RECIPE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	int Recipe_select(void);
  int Get_filenames(void);
  int Update_filenames(void);

  afx_msg void OnBnClickedLoadRecipe();
	afx_msg void OnBnClickedSaveRecipe();
	afx_msg void OnBnClickedScrollUp();
	afx_msg void OnBnClickedScrollDown();
	afx_msg void OnBnClickedRenameRecipe();
	afx_msg void OnBnClickedDeleteRecipe();
  afx_msg void OnBnClickedCloseRecipe();
  virtual BOOL OnInitDialog();
};
