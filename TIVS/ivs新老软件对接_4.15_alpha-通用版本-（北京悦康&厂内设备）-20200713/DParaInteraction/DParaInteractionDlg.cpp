
// DParaInteractionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DParaInteraction.h"
#include "DParaInteractionDlg.h"
#include "afxdialogex.h"
#include "CDParaInter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDParaInteractionDlg 对话框




CDParaInteractionDlg::CDParaInteractionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDParaInteractionDlg::IDD, pParent)
	, cstr_ParticleType(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDParaInteractionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CMB_DEFAULT_PATTERN, cstr_ParticleType);
}

BEGIN_MESSAGE_MAP(CDParaInteractionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PARA_SET, &CDParaInteractionDlg::OnBnClickedBtnParaSet)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_PARA, &CDParaInteractionDlg::OnBnClickedBtnDefaultPara)
END_MESSAGE_MAP()


// CDParaInteractionDlg 消息处理程序

BOOL CDParaInteractionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//GetDlgItem(IDC_CMB_DEFAULT_PATTERN)
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("底光"));
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("背光"));
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDParaInteractionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDParaInteractionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDParaInteractionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDParaInteractionDlg::OnBnClickedBtnParaSet()
{ 
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	/*if (cstr_ParticleType == _T("背光"))
		pre_para.initParaByParticleType(algoParticleDi::particle_bei);
	else pre_para.initParaByParticleType(algoParticleDi::particle_di);*/
	CDParaInter  dlg_paraInter(pre_para, is_proc_regPD);
	dlg_paraInter.DoModal();
	CString str;
	str.Format(_T("thr_level_br:%d,thr_level_cnt:%d,level_shiftUp:%d,level_shiftDown:%d,thr_side_br:%d,thr_bottom_cnt:%d,thr_bottom_br:%d,btThick:%d,point(%d,%d),thr_locy_br:%d,thr_locy_cnt:%d,thr_diff_br:%d,thr_diff_cnt:%d,rangeDetectMin:%d,lenMax:%d,lenMin:%d;"), pre_para.thr_level_br, pre_para.thr_level_cnt, pre_para.level_shiftUp, pre_para.level_shiftDown, pre_para.thr_side_br, pre_para.thr_bottom_cnt, pre_para.thr_bottom_br, pre_para.btThick, pre_para.ptSft.x, pre_para.ptSft.y, pre_para.thr_locy_br, pre_para.thr_locy_cnt, pre_para.thr_diff_br, pre_para.thr_diff_cnt, pre_para.rangeDetectMin, pre_para.lenMax, pre_para.lenMin);
	MessageBox(str);
}


void CDParaInteractionDlg::OnBnClickedBtnDefaultPara()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (cstr_ParticleType == _T("背光"))
		pre_para.initParaByParticleType(algoParticleDi::particle_bei);
	else pre_para.initParaByParticleType(algoParticleDi::particle_di);
}
