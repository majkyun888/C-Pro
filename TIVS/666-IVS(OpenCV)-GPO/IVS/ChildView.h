// ChildView.h : interface of the CChildView class

#pragma once

#include "stdafx.h"
#include "Def_globali.h"
#include "Parameter.h"
#include "File_mngmt.h"
#include "Menu_button.h"
#include "Container_wnd.h"
#include "Container_main.h"
#include "Text_box.h"
#include "Recipe_manager.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

	CContainer_main* container_main;

	CContainer_wnd* container_wnd[MAX_NUM_CONTAINER];
	int             num_container;

	CMenu_button*   menu_btn[MAX_NUM_MENU_BTN];
	int             num_menu_btn;

//	CParameter*     alarm;
//  CParameter*     warning;



	CParameter*     parameter[MAX_NUM_PARAMS];
	int             num_params; 

	CParameter*     toupdate[MAX_NUM_PARAMS];
	int             num_toupdate;

  wchar_t*        parcat_1;  // main category description for cparameter logging (camera/PLC)
  wchar_t*        parcat_2;  // sub  category description for cparameter logging (control)



// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnLoadImage();
	//afx_msg void On_load_image();
	afx_msg void On_process_image();
	afx_msg void On_test();
//	afx_msg void On_thread_onoff();
  afx_msg LRESULT On_init_prog(WPARAM wParam, LPARAM lParam);

  //Pour added 20180329
  //afx_msg LRESULT On_my_message(WPARAM wParam, LPARAM lParam);
  //Pour added 20180329

	afx_msg void On_ispparameters();
	afx_msg void On_plcparameters();
	afx_msg void On_utilities();
	afx_msg void On_exit_prog();

//	FCObjImage   logo;
//	CStatic      date_time;
	//UINT         app_timer;


  #ifdef BECKHOFF  
  #endif

//	void Create_menu(void);
//	void Create_display(void);
  int  Prepare_dsporgx( int sta, int end );

  void Create_menu_fillev   (void);
	void Create_display_fillev(void);

  void Create_menu_stopper_ctrl   (void);
	void Create_display_stopper_ctrl(void);

	void Create_menu_TIVS_A60   (void);
	void Create_display_TIVS_A60(void);

//	void Create_menu_TIVS40_LQ   (void);
//	void Create_display_TIVS40_LQ(void);
	void Create_menu_TIVS_A40   (void);
	void Create_display_TIVS_A40(void);

	void Create_menu_TIVS_C40   (void);
	void Create_display_TIVS_C40(void);

	void Create_menu_TIVS_C60   (void);
	void Create_display_TIVS_C60(void);

	void Create_menu_TIVS_F40   (void);
	void Create_display_TIVS_F40(void);

	void Create_menu_TIVS_F20   (void);
	void Create_display_TIVS_F20(void);


	void Create_menu_TIVS_C20   (void);
	void Create_display_TIVS_C20(void);

	void Create_menu_TIVS_A20L   (void);
	void Create_display_TIVS_A20L(void);

//Pour added for TIVS-A40L 20180124
	void Create_menu_TIVS_A40L   (void);
	void Create_display_TIVS_A40L(void);

//Pour added for TIVS-A40L 20180124


//Pour added for TIVS-A20 20180523

	void Create_menu_TIVS_A20   (void);
	void Create_display_TIVS_A20(void);

	void Create_menu_TIVS_S40   (void);
	void Create_display_TIVS_S40(void);

	void Create_menu_TIVS_S60   (void);
	void Create_display_TIVS_S60(void);


//Pour added for TIVS-A20 20180523

  
  int Deallocate( void );

  int CChildView::Destroy_menu( void );



//protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

//public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// Create menu for all functions/parameters
  void Close_all_menus(void);
  int Show_logfile( wchar_t* name ); // CString &name );
};

