#pragma once



#include "stdafx.h"
#include "File_mngmt.h"
#include "Globals.h"
#include "Def_globali.h"
#include "Container_wnd.h"

//#include "Container_wnd.h"

//#include "Def_globali.h"
//#include "Parameter.h"


#define ID_TIMER    2000


#define MOVE_UP     1000
#define MOVE_DOWN   1100
#define MOVE_LEFT   1200
#define MOVE_RIGHT  1300
#define HOR_ENLARGE 1400
#define HOR_REDUCE  1500
#define VER_ENLARGE 1600
#define VER_REDUCE  1700



class CFile_mngmt;

// CArea_adjust dialog

class CArea_adjust : public CContainer_wnd //: public CDialog
{
	DECLARE_DYNAMIC(CArea_adjust)

public:
//	CArea_adjust(CWnd* pParent = NULL);   // standard constructor
//	CArea_adjust(CWnd* pParent, UINT templatename);   // standard constructor
//  CArea_adjust( CWnd* pParent, int index_in, CFile_mngmt* file_manager=NULL );
  CArea_adjust( int index_in, CFile_mngmt* file_manager=NULL );

	virtual ~CArea_adjust();

//	void Draw_area( int ind, BOOL draw_rect, BOOL delete_flag );
	void Draw_area( void );
//  void Normalize( void );
  void Move_area( int dir );
  void Manage_command( int msg, int cmd );

// Dialog Data
//	enum { IDD = IDD_AREA_DIALOG };


  HWND move_up, move_down, move_left, move_right;
  HWND hor_enlarge, hor_reduce, ver_enlarge, ver_reduce;

	static SIZE     dim_wnd;


  RECT buffrect; // inspection coordinates in buffer space
  RECT limrect;  // limit rect (default=whole original buffer size)
  SIZE minsize;  // minimum size of area

//	int  index;   // selector value (0..NUM_TLC-1)
	UINT index;   // selector value (0..NUM_TLC-1)
  UINT color;   // color to draw inspection area
	int  command; // issued command for automatic repetition
	int  first;

	//CMenu_button* parent;
  int*        ptr_dsp;   // pointer to image dsp
  wchar_t*    parcat_1;  // main category description for cparameter logging (camera/PLC)
  wchar_t*    parcat_2;  // sub  category description for cparameter logging (control)


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedClose();
//	afx_msg void OnBnClickedIndex();
//  afx_msg void OnBnClickedX10();
//  afx_msg void OnBnClickedLevel();
//	afx_msg void OnBnClickedMoveUp();
//	afx_msg void OnBnClickedMoveDown();

protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnPaint();
//  virtual BOOL OnInitDialog();
};
