
// DParaInteractionDlg.h : ͷ�ļ�
//

#pragma once

//#include "..//MiddlewareImprocessDll/dataType.h"
#include"dataType.h"
// CDParaInteractionDlg �Ի���
class CDParaInteractionDlg : public CDialogEx
{
// ����
public:
	CDParaInteractionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DPARAINTERACTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	algoParticleDi::PrePara pre_para;
	algoParticleDi::IsProcRegPD is_proc_regPD;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnParaSet();
	afx_msg void OnBnClickedBtnDefaultPara();
	CString cstr_ParticleType;
};
