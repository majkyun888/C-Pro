
// DParaInteractionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DParaInteraction.h"
#include "DParaInteractionDlg.h"
#include "afxdialogex.h"
#include "CDParaInter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDParaInteractionDlg �Ի���




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


// CDParaInteractionDlg ��Ϣ�������

BOOL CDParaInteractionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//GetDlgItem(IDC_CMB_DEFAULT_PATTERN)
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("�׹�"));
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->AddString(_T("����"));
	((CComboBox*)GetDlgItem(IDC_CMB_DEFAULT_PATTERN))->SetCurSel(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDParaInteractionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDParaInteractionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDParaInteractionDlg::OnBnClickedBtnParaSet()
{ 
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if (cstr_ParticleType == _T("����"))
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (cstr_ParticleType == _T("����"))
		pre_para.initParaByParticleType(algoParticleDi::particle_bei);
	else pre_para.initParaByParticleType(algoParticleDi::particle_di);
}
