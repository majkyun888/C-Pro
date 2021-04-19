// CDParaInter.cpp: 实现文件
//

//#include "pch.h"
#include "stdafx.h"
#include "ImgProcOffline.h"
#include "CDParaInter.h"
#include "afxdialogex.h"


// CDParaInter 对话框

IMPLEMENT_DYNAMIC(CDParaInter, CDialogEx)

CDParaInter::CDParaInter(algoParticleDi::ParticleType& _particle_type, algoParticleDi::PrePara& _pre_para, algoParticleDi::IsProcRegPD& _is_proc_regPD, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D_PARA_INTER, pParent), pre_para(_pre_para), is_proc_regPD(_is_proc_regPD), particle_type(_particle_type)
{

}

CDParaInter::~CDParaInter()
{
}

void CDParaInter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
#ifdef _para_test
	DDX_Text(pDX, IDC_E_LEVEL_BR, pre_para.thr_level_br);
	DDX_Text(pDX, IDC_E_LEVEL_CNT, pre_para.thr_level_cnt);
	DDX_Text(pDX, IDC_E_LEVEL_SHIFTUP, pre_para.level_shiftUp);
	DDX_Text(pDX, IDC_E_LEVEL_SHIFTDOWN, pre_para.level_shiftDown);
#endif // _para_test
	DDX_Text(pDX, IDC_E_SIDE_BR, pre_para.thr_side_br);
	DDX_Text(pDX, IDC_E_BOTTOM_CNT, pre_para.thr_bottom_cnt);
	DDX_Text(pDX, IDC_E_BOTTOM_BR, pre_para.thr_bottom_br);
	DDX_Text(pDX, IDC_E_BOTTOM_THICK, pre_para.btThick);
	DDX_Text(pDX, IDC_E_POINT_SFT_X, pre_para.ptSft.x);
	DDX_Text(pDX, IDC_E_POINT_SFT_Y, pre_para.ptSft.y);
	DDX_Text(pDX, IDC_E_LOCY_BR, pre_para.thr_locy_br);
	DDX_Text(pDX, IDC_E_LOCY_CNT, pre_para.thr_locy_cnt);
	DDX_Text(pDX, IDC_E_DIFF_BR, pre_para.thr_diff_br);
	DDX_Text(pDX, IDC_E_DIFF_CNT, pre_para.thr_diff_cnt);
	DDX_Text(pDX, IDC_E_RANGEDETECT_MIN, pre_para.rangeDetectMin);
	DDX_Text(pDX, IDC_E_LENMAX, pre_para.lenMax);
	DDX_Text(pDX, IDC_E_LENMIN, pre_para.lenMin);
	DDX_Check(pDX, IDC_CK_FIND_LEVEL, is_proc_regPD.iFindLevel);
	DDX_Check(pDX, IDC_CK_FIND_SIDE, is_proc_regPD.iFindSide);
	DDX_Check(pDX, IDC_CK_REG_SIDE, is_proc_regPD.iRegSide);
	DDX_Check(pDX, IDC_CK_FIND_BOTTOM, is_proc_regPD.iFindBottom);
	DDX_Check(pDX, IDC_CK_REG_BOTTOM, is_proc_regPD.iRegBottom);
	DDX_Text(pDX, IDC_E_SCAN_UP, pre_para.scanUp);
	DDX_Text(pDX, IDC_E_POINT_LIGHT_SPOT_X, pre_para.ptLightSpot.x);
	DDX_Text(pDX, IDC_E_POINT_SPOT_LIGHT_Y, pre_para.ptLightSpot.y);
	DDX_Text(pDX, IDC_E_BLINK_THR, pre_para.bottle_blink_fliter_bright_thr);
	DDX_Text(pDX, IDC_E_BLINK_RANG_X_LEFT, pre_para.bottle_blink_filter_rangeX_left);
	DDX_Text(pDX, IDC_E_BLINK_RANG_X_RIGHT, pre_para.bottle_blink_filter_rangeX_right);
	DDX_Text(pDX, IDC_E_BLINK_RANGE_Y_MAX, pre_para.bottle_blink_filter_rangeY_max);
	DDX_Text(pDX, IDC_E_BLINK_RANGE_Y_MIN, pre_para.bottle_blink_filter_rangeY_min);
	DDX_Text(pDX, IDC_E_val_dynamic_filter, pre_para.val_dynamic_filter);
	DDX_Text(pDX, IDC_E_DyEordeSize, pre_para.DyEordeSize);
	DDX_Text(pDX, IDC_E_knDyTimes, pre_para.knDyTimes);
}


BEGIN_MESSAGE_MAP(CDParaInter, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDParaInter::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDParaInter::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_PARA, &CDParaInter::OnBnClickedBtnDefaultPara)
	ON_CBN_SELCHANGE(IDC_CMB_DEFAULT_PATTERN, &CDParaInter::OnSelchangeCmbDefaultPattern)
END_MESSAGE_MAP()


// CDParaInter 消息处理程序


void CDParaInter::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//is_proc_regPD.iFindLevel = iFindLevel;
	//is_proc_regPD.iFindSide = iFindSide;
	//is_proc_regPD.iRegSide = iRegSide;
	//is_proc_regPD.iFindBottom = iFindBottom;
	//is_proc_regPD.iRegBottom = iRegBottom;
	CDialogEx::OnOK();
}


void CDParaInter::OnBnClickedBtnApply()
{
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	//is_proc_regPD.iFindLevel = iFindLevel;
	//is_proc_regPD.iFindSide = iFindSide;
	//is_proc_regPD.iRegSide = iRegSide;
	//is_proc_regPD.iFindBottom = iFindBottom;
	//is_proc_regPD.iRegBottom = iRegBottom;
}


void CDParaInter::OnBnClickedBtnDefaultPara()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret = MessageBox(_T("是否确认恢复至默认内部参数"), NULL, MB_OKCANCEL);
	if (ret == IDCANCEL)
		return;		
	//if (cstr_ParticleType == _T("背光"))
	//	pre_para.initParaByParticleType(algoParticleDi::particle_bei);
	//else pre_para.initParaByParticleType(algoParticleDi::particle_di);
	pre_para.initParaByParticleType(particle_type);
	UpdateData(false);
}


BOOL CDParaInter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("底光")); // algoParticleDi::particle_di
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("背光")); // algoParticleDi::particle_bei
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->SetCurSel(particle_type);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDParaInter::OnSelchangeCmbDefaultPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode_particle_type = ((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->GetCurSel();
	switch (mode_particle_type)
	{
	case algoParticleDi::particle_di:
		particle_type = algoParticleDi::particle_di;
		break;
	case algoParticleDi::particle_bei:
			particle_type = algoParticleDi::particle_bei;
		break;
	default:
		break;
	}
	
}
