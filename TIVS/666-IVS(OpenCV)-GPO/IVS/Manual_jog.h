#pragma once

#include "stdafx.h"
#include "resource.h"
#include "strings.h"
#include "globals.h"


// finestra di dialogo CManual_jog



class CManual_jog : public CDialog
{
	DECLARE_DYNAMIC(CManual_jog)

public:
	CManual_jog(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CManual_jog();

  int selected_axis;
  BOOL jogplus_pushed;
  BOOL jogminus_pushed;


  ADS_data_type  ADS_axis_nr;         // m_axis_nr   (W)
  ADS_data_type  ADS_jog_fwd;         // m_jog_fwd   (W)
  ADS_data_type  ADS_jog_rev;         // m_jog_rev   (W)
  ADS_data_type  ADS_homing;          // m_homing    (W)
  ADS_data_type  ADS_stop;            // m_stop      (W)
  ADS_data_type  ADS_homing_pending;  // m_home_search       (R)
  ADS_data_type  ADS_axis_pos;        // m_axis_pos          (R)
  ADS_data_type  ADS_jog_possible;    // m_maintenance_ready (R)
  ADS_data_type  ADS_manual_controls; // m_popup_active      (W)

// Dati della finestra di dialogo
	enum { IDD = IDD_JOGWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnInitDialog();
  virtual BOOL DestroyWindow();
  afx_msg void OnBnClickedAxis();
  afx_msg void OnBnClickedStop();
  afx_msg void OnBnClickedHoming();
};
