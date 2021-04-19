#pragma once
#include "dataType.h"

// CDParaInter 对话框

class CDParaInter : public CDialogEx
{
	DECLARE_DYNAMIC(CDParaInter)
	
public:
	CDParaInter(algoParticleDi::PrePara& _pre_para, algoParticleDi::IsProcRegPD& _is_proc_regPD, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDParaInter();
	algoParticleDi::IsProcRegPD& is_proc_regPD;
	algoParticleDi::PrePara& pre_para;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D_PARA_INTER};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnApply();
	BOOL iFindLevel;
	BOOL iFindSide;
	BOOL iRegSide;
	BOOL iFindBottom;
	BOOL iRegBottom;
	afx_msg void OnBnClickedBtnDefaultPara();
	virtual BOOL OnInitDialog();
	CString cstr_ParticleType;
};
