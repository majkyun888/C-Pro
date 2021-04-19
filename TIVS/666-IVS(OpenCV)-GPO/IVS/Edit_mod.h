#pragma once

#include "stdafx.h"
//#include "Parameter.h"


// CEdit_mod

class CEdit_mod : public CEdit
{
	DECLARE_DYNAMIC(CEdit_mod)

public:
	CEdit_mod();
	virtual ~CEdit_mod();
	//CParameter* parent;
  // CCmdTarget* parent;

	CWnd* parent;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
};


