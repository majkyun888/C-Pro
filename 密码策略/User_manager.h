#pragma once

#include "Password_manager_v2.h"
#include "afxwin.h"


// CUser_manager dialog




class CUser_manager : public CDialog
{
	DECLARE_DYNAMIC(CUser_manager)

public:
	CUser_manager(CWnd* pParent = NULL );   // standard constructor
	virtual ~CUser_manager();

// Dialog Data
	enum { IDD = IDD_USERS_MNGR };

  void Refresh( void );
  void UpdateLevels( void );
  BOOL GetFormData( void );
  BOOL CopyUserData( UINT from, UINT to );


  int Check_password( wchar_t* pwd );


  UINT sel_index;  // selection index 0...num_user
  UINT current_level;
  UINT sel_level;

  BOOL hide_password;

  // criteria for acceptance of password
  UINT min_lenght;  // minimun lenght
  UINT max_lenght;  // maximum length
  UINT min_lower;   // minimun number of lower case characters
  UINT min_upper;   // minimun number of upper case characters
  UINT min_digit;   // minimun number of digits

  CString msg_err;


  //wchar_t   txt[PWD_MAXLUNGH_TXT+1];
  wchar_t   usr[PWD_MAXLUNGH_TXT+1];
  wchar_t   pwd[PWD_MAXLUNGH_TXT+1]; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnDeltaposUserSelect(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnEnChangeIndex2();
  afx_msg void OnBnClickedSupervisor();
  afx_msg void OnBnClickedMaintenance();
  afx_msg void OnBnClickedOperator();
  afx_msg void OnBnClickedNolevel();
  afx_msg void OnBnClickedUpdate();
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedDelete();
 // afx_msg void OnEnChangeUsername();
 // afx_msg void OnEnChangePassword();
  afx_msg void OnStnClickedUsername();
  afx_msg void OnStnClickedPassword();
  afx_msg void OnBnClickedCancel();
};
