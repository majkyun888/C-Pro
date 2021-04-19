
// ImgProcOfflineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgProcOffline.h"
#include "ImgProcOfflineDlg.h"
#include "afxdialogex.h"
#include "CDParaInter.h"
#include "..//MiddlewareImprocessDll/ParaXml.h"
#include <imagehlp.h>  
#pragma comment(lib,"imagehlp.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 兼容老外软件
#ifdef _DEBUG
#pragma comment(lib,"../IVS/Debug/MiddlewareImprocessDll.lib")
#else
#pragma comment(lib,"../IVS/Release/MiddlewareImprocessDll.lib")
#endif

// 兼容自主软件
//#ifdef _DEBUG
//#pragma comment(lib,"../Debug/MiddlewareImprocessDll.lib")
//#else
//#pragma comment(lib,"../Release/MiddlewareImprocessDll.lib")
//#endif

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


// CImgProcOfflineDlg 对话框

static string str_win_roi_sel = "ROI_SELECT";


CImgProcOfflineDlg::CImgProcOfflineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgProcOfflineDlg::IDD, pParent)
	, delay_frame_ms_true(100)
	, delay_seq_ms_true(0)
	, delay_frame_ms_false(100)
	, delay_seq_ms_false(0)
	, bRtBlinkClear(false)
	, bProcessing(false)
	, m_bright_times (0)
	, m_batch_mode(0)
	, m_batch_st(0)
	, m_batch_end(11)
	, m_cmb_copy_src(-1)
	, m_cmb_copy_dst(-1)
{
	/*para_particle.iVimRltSave = true;*/
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThreadHandle = NULL;
	m_hThreadBatch = NULL;
	para_particle.nozero_frm_sum_thr = 2;
	para_particle.nozero_single_thr = 2;
	para_particle.sensity = 1.;
	para_particle.ratio = 0.85;
	para_particle.minnumimg = 4;
	para_particle.mintotnpt = 6;
	para_particle.roi = cv::Rect();
	cstr_root_path = _T("");
	cam_idx = -1;
	para_particle.iVimRltSave = true;
	para_particle.iVimOnlineSave = 0;
	m_grp_cnt = 0;

	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
}

void CImgProcOfflineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_PARTICLE_PIC_NUM, para_particle.nozero_frm_sum_thr);
	DDX_Text(pDX, IDC_ET_PRTICLE_PT_NUM, para_particle.nozero_single_thr);
	DDX_Text(pDX, IDC_ET_PRTICLE_RATIO, para_particle.ratio);
	DDX_Text(pDX, IDC_ET_PRTICLE_SENSITY, para_particle.sensity);
	//DDX_Text(pDX, IDC_ET_DELAY_TIME_FRAME, delay_frame_ms);
	//DDX_Text(pDX, IDC_ET_DELAY_TIME_SEQ, delay_seq_ms);
}

void CImgProcOfflineDlg::mouseHandler(int event, int x, int y, int, void* pcvim)
{
	static int drag = 0;
	cv::Mat img0 = *(cv::Mat*)pcvim;
	static cv::Mat img1;
	if (event == cv::EVENT_LBUTTONDOWN && !drag)
	{
		img1 = img0.clone();
		drag = 1;
		roiSel = cv::Rect(cv::Point(x, y), cv::Point(x, y));
		imshow(str_win_roi_sel, img1);
	}
	if (event == cv::EVENT_LBUTTONUP && drag)
	{
		imshow(str_win_roi_sel, img1);
		drag = 0;
	}
	if (event == cv::EVENT_MOUSEMOVE && drag) {
		img1 = img0.clone();
		roiSel = cv::Rect(roiSel.tl(), cv::Point(x, y));
		cv::rectangle(img1, roiSel, cv::Scalar(0, 255, 0), 2);
		imshow(str_win_roi_sel, img1);
	}
}

cv::Rect roiBlinkSel;
string str_win_rlt("");
void CImgProcOfflineDlg::mouseBlinkRectHandler(int event, int x, int y, int, void* _pdlg) {
	static int drag = 0;
	CImgProcOfflineDlg* pdlg=  (CImgProcOfflineDlg*) _pdlg;
	static cv::Mat img1;
	if (event == cv::EVENT_LBUTTONDOWN && !drag)
	{
		img1 = pdlg->imLastShow/*.clone()*/;
		drag = 1;
		roiBlinkSel = cv::Rect(cv::Point(x, y), cv::Point(x, y));
		for (vector<cv::Rect>::iterator it = pdlg->para_inter.rtMaskBlink.begin();
			it != pdlg->para_inter.rtMaskBlink.end(); it++)
			cv::rectangle(img1, *it, cv::Scalar(255, 0, 0), 1);
		imshow(str_win_rlt, img1);
	}
	if (event == cv::EVENT_LBUTTONUP && drag)
	{
		imshow(str_win_rlt, img1);
		pdlg->para_inter.rtMaskBlink.push_back(roiBlinkSel);
		drag = 0;
	}
	if (event == cv::EVENT_MOUSEMOVE && drag) {
		img1 = pdlg->imLastShow.clone();
		roiBlinkSel = cv::Rect(roiBlinkSel.tl(), cv::Point(x, y));
		cv::rectangle(img1, roiBlinkSel, cv::Scalar(0, 255, 0), 1);
		imshow(str_win_rlt, img1);
	}
	if (event == cv::EVENT_MOUSEMOVE && pdlg->bRtBlinkClear) {
		pdlg->bRtBlinkClear = false;
		img1 = pdlg->imLastShow.clone();
		imshow(str_win_rlt, img1);
	}
}

cv::Rect CImgProcOfflineDlg::roiSel = cv::Rect();

BEGIN_MESSAGE_MAP(CImgProcOfflineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PROC_ST, &CImgProcOfflineDlg::OnBnClickedBtnProcSt)
	ON_BN_CLICKED(IDC_BTN_SEL_ROOT_DIR, &CImgProcOfflineDlg::OnBnClickedBtnSelRootDir)
	ON_BN_CLICKED(IDC_BTN_ROI_SEL, &CImgProcOfflineDlg::OnBnClickedBtnRoiSel)
	ON_BN_CLICKED(IDC_BTN_CHECK_FILE_PATH, &CImgProcOfflineDlg::OnBnClickedBtnCheckFilePath)
	ON_BN_CLICKED(IDC_BTN_READ_PARA_FROM_XML, &CImgProcOfflineDlg::OnBnClickedBtnReadParaFromXml)
	ON_BN_CLICKED(IDC_BTN_WRITE_PARA_TO_XML, &CImgProcOfflineDlg::OnBnClickedBtnWriteParaToXml)
	ON_BN_CLICKED(IDC_BTN_PARA_INTER, &CImgProcOfflineDlg::OnBnClickedBtnParaInter)
	ON_BN_CLICKED(IDC_BTN_RT_BLINK_CLEAR_ALL, &CImgProcOfflineDlg::OnBnClickedBtnRtBlinkClearAll)
	ON_BN_CLICKED(IDC_BTN_BATCH_PATH, &CImgProcOfflineDlg::OnBnClickedBtnBatchPath)
	ON_CBN_SELCHANGE(IDC_CMB_PAUSE_MODE, &CImgProcOfflineDlg::OnCbnSelchangeCmbPauseMode)
	ON_CBN_DROPDOWN(IDC_CMB_PAUSE_MODE, &CImgProcOfflineDlg::OnDropdownCmbPauseMode)
	ON_CBN_SELCHANGE(IDC_CMB_BATCH_MODE, &CImgProcOfflineDlg::OnCbnSelchangeCmbBatchMode)
	ON_CBN_SELCHANGE(IDC_CMB_RLT_SAVE, &CImgProcOfflineDlg::OnCbnSelchangeCmbRltSave)
	ON_CBN_SELCHANGE(IDC_CMB_IMG_BRIGHT, &CImgProcOfflineDlg::OnCbnSelchangeCmbImgBright)
	ON_CBN_SELCHANGE(IDC_CMB_COPY_SRC, &CImgProcOfflineDlg::OnCbnSelchangeCmbCopySrc)
	ON_CBN_SELCHANGE(IDC_CMB_COPY_DST, &CImgProcOfflineDlg::OnCbnSelchangeCmbCopyDst)
	ON_BN_CLICKED(IDC_BTN_COPY, &CImgProcOfflineDlg::OnBnClickedBtnCopy)
END_MESSAGE_MAP()


// CImgProcOfflineDlg 消息处理程序



BOOL CImgProcOfflineDlg::OnInitDialog()
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
	((CComboBox*)GetDlgItem(IDC_CMB_PAUSE_MODE))->InsertString(0, _T("false结果"));
	((CComboBox*)GetDlgItem(IDC_CMB_PAUSE_MODE))->InsertString(1, _T("true结果"));
	((CComboBox*)GetDlgItem(IDC_CMB_PAUSE_MODE))->SetCurSel(0);
	

	
	((CComboBox*)GetDlgItem(IDC_CMB_BATCH_MODE))->InsertString(0, _T("false批次"));
	((CComboBox*)GetDlgItem(IDC_CMB_BATCH_MODE))->InsertString(1, _T("good批次"));
	((CComboBox*)GetDlgItem(IDC_CMB_BATCH_MODE))->InsertString(2, _T("所有序列"));
	((CComboBox*)GetDlgItem(IDC_CMB_BATCH_MODE))->SetCurSel(0);
	OnCbnSelchangeCmbBatchMode();
	

	((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->InsertString(0, _T("不保存"));
	((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->InsertString(1, _T("保存合格"));
	((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->InsertString(2, _T("保存剔除"));
	((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->InsertString(3, _T("保存所有"));
	((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->SetCurSel(0);
	OnCbnSelchangeCmbRltSave();

	((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->InsertString(0, _T("原始亮度"));
	((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->InsertString(1, _T("亮度2倍"));
	((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->InsertString(2, _T("亮度3倍"));
	((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->InsertString(3, _T("亮度4倍"));
	((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->SetCurSel(0);
	OnCbnSelchangeCmbImgBright();

	((CComboBox*)GetDlgItem(IDC_CMB_COPY_SRC))->InsertString(0, _T("当前目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_SRC))->InsertString(1, _T("good目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_SRC))->InsertString(2, _T("false目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_SRC))->SetCurSel(0);
	OnCbnSelchangeCmbCopySrc();

	((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->InsertString(0, _T("当前目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->InsertString(1, _T("good目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->InsertString(2, _T("false目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->InsertString(3, _T("good和flase目录"));
	((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->SetCurSel(0);
	OnCbnSelchangeCmbCopyDst();

	SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, 100);
	SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, 0);
	m_sle_true = 0;

	SetDlgItemInt(IDC_ET_BATCH_ST, m_batch_st);
	SetDlgItemInt(IDC_ET_BATCH_END, m_batch_end);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImgProcOfflineDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImgProcOfflineDlg::OnPaint()
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
HCURSOR CImgProcOfflineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImgProcOfflineDlg::OnBnClickedBtnProcSt()
{
	/*UpdateData(true);*/
	myUpdateData(true);
	
	{
		if (para_particle.nozero_frm_sum_thr <= 0 || para_particle.nozero_frm_sum_thr >= 20) {

			MessageBox(_T("Para setting Err: sequenceNum 2~20"));
			return;
		}

		if (para_particle.nozero_single_thr <= 0 || para_particle.nozero_single_thr >= 5000) {
			MessageBox(_T("Para setting Err: pixelNum 1~49999"));
			return;
		}

		if (para_particle.ratio < 0.1 || para_particle.ratio > 1) {
			MessageBox(_T("Para setting Err: ratio 0.1~1 "));
			return;
		}

		if (para_particle.sensity < 0.5 || para_particle.sensity > 2) {
			MessageBox(_T("Para setting Err: sensity 0.5~2"));
			return;
		}

		char ch[400];
		sprintf(ch, "Para: sequenceNum %d, pixelNum %d, ratio %.2f, sensity %.2f",
			para_particle.nozero_frm_sum_thr, para_particle.nozero_single_thr, 
			para_particle.ratio, para_particle.sensity);
		/*MessageBox( StdString2Unicode(ch) );*/
	}
	if (para_particle.roi.size() == cv::Size()) {
		MessageBox(_T("roi is not selected ,please return"));
		return;
	}
		
	// TODO: 在此添加控件通知处理程序代码
	if (manage.imSampleFullPath == _T("")) {
		MessageBox(_T("the image sequnece PATH is not selected ,please return"));
		return;
	}
		
	if (m_hThreadHandle == NULL) {
		bProcessing = true;
		m_hThreadHandle = HANDLE(_beginthread(CImgProcOfflineDlg::ImgProcThread, 0, this));
		/*GetDlgItem(IDC_BTN_PROC_ST)->EnableWindow(FALSE);*/
	}
	else if (false == bProcessing)
		bProcessing = true;
	else
		MessageBox(_T("processing"));
}


void CImgProcOfflineDlg::ImgProcThread(void* _data)
{
	CImgProcOfflineDlg* dlg = (CImgProcOfflineDlg*)_data;
	while (true) {
		if (dlg->bProcessing == false)
		{
			Sleep(100);
			continue;
		}
		if (str_win_rlt == "") {
			str_win_rlt = "IM_RLT";
			cv::namedWindow(str_win_rlt, cv::WINDOW_NORMAL);
			cv::resizeWindow(str_win_rlt, 1920, 1080);
			cv::moveWindow(str_win_rlt, 0, 0);
			cv::setMouseCallback(str_win_rlt, mouseBlinkRectHandler, dlg);
		}
		bool toNext = false;
		/*int cam_idx = dlg->GetDlgItemInt(IDC_ET_CAM_IDX);*/
		bool rlt(false);
		while (dlg->manage.procOnce(dlg->para_particle, dlg->para_inter, toNext, dlg->cam_idx, rlt))
		{
			CString cstr;
			cstr.Format(_T("总检测次数：%05d, 当前目录：%s"), ++dlg->m_grp_cnt, dlg->cstr_root_path);
			dlg->SetDlgItemTextW(IDC_DIS_TXT, cstr);
			dlg->isBlinkActive = false;
			for (int i = 0; i < dlg->manage.imrlts.size(); i++) {
				cv::Mat im = dlg->manage.imrlts.at(i)* dlg->m_bright_times;
				if (im.empty())
					continue;
				for (vector<cv::Rect>::iterator it = dlg->para_inter.rtMaskBlink.begin();
					it != dlg->para_inter.rtMaskBlink.end(); it++)
					cv::rectangle(im, *it, cv::Scalar(255, 0, 0), 1);
				cv::imshow(str_win_rlt, im);
				dlg->imLastShow = im;
				if (dlg->m_sle_true) {
					dlg->delay_frame_ms_true = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
					dlg->delay_seq_ms_true = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
				}
				else {
					dlg->delay_frame_ms_false = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
					dlg->delay_seq_ms_false = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
				}
				/*dlg->delay_frame_ms = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);*/
				int delay_frame_ms(0);
				if (rlt)
					delay_frame_ms = dlg->delay_frame_ms_true;
				else
					delay_frame_ms = dlg->delay_frame_ms_false;
				cv::waitKey(delay_frame_ms);
				/*cv::waitKey(dlg->delay_frame_ms);*/
			}
			/*dlg->delay_seq_ms = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);*/
			//if (dlg->delay_frame_ms == 0)
			//	dlg->isBlinkActive = true;
			//char ch = cv::waitKey(dlg->delay_seq_ms);
			int delay_seq_ms(0);
			if (rlt)
				delay_seq_ms = dlg->delay_seq_ms_true;
			else
				delay_seq_ms = dlg->delay_seq_ms_false;
			if (delay_seq_ms == 0)
				dlg->isBlinkActive = true;
			char ch = cv::waitKey(delay_seq_ms);
			if (ch == 'j' || ch == 'J')
				toNext = false;
			else
				toNext = true;
			dlg->myUpdateData(true);
		}
		/*cv::destroyWindow(str_win_rlt);*/
		dlg->bProcessing = false;
	}
	dlg->m_hThreadHandle = NULL;
}

void CImgProcOfflineDlg::OnBnClickedBtnSelRootDir()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CString rootPath;*/

	//打开文件框
	BROWSEINFO br;
	ZeroMemory(&br, sizeof(BROWSEINFO));
	br.hwndOwner = GetSafeHwnd();
	br.ulFlags = BIF_RETURNONLYFSDIRS;//只返回文件目录信息；
	LPITEMIDLIST idl;//项目ID列表；
	idl = SHBrowseForFolder(&br);
	//文件的绝对路径；
	TCHAR szPath[MAX_PATH * 2];
	ZeroMemory(szPath, sizeof(TCHAR) * (MAX_PATH * 2));
	//szPath[0]=_T('\0');
	if (idl)
	{
		SHGetPathFromIDList(idl, szPath);//从项目标记符列表转化成CString类型；
		IMalloc* pMalloc;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			pMalloc->Free(idl);
			pMalloc->Release();
		}
		CString strtmp;
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp +_T("正在加载目录")+szPath + _T(".请稍后"));
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(FALSE);
		/*rootPath = szPath;*/
		/*manage.collectImgPaths(szPath);*/
		manage.collectImgPaths(szPath,szPath);
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("目录加载完成") + szPath );
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(TRUE);
		para_particle.roi = cv::Rect();
		((CEdit*)GetDlgItem(IDC_ET_IMGS_PATH))->SetWindowTextW(szPath);
	}
	else
		return;
}


void CImgProcOfflineDlg::OnBnClickedBtnRoiSel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*cv::Mat imForRoiSel = cv::imread(Unicode2StdString(manage.imSampleFullPath), CV_LOAD_IMAGE_ANYCOLOR);*/
	if (imLastShow.empty() == true) {
		CString str;
		/*str.Format(_T("没有加载图像，检测区域选择失败,请确认路径%s"), manage.imSampleFullPath);*/
		str.Format(_T("image is not loaded，the ROI selection failed ,please check the image sequece path %s"), manage.imSampleFullPath);
		imLastShow = cv::imread(Unicode2StdString(manage.imSampleFullPath));
		if (imLastShow.empty()) {
			MessageBox(str);
			return;
		}
	}
	cv::Mat imm;
	if (imLastShow.channels() == 0)
		cv::cvtColor(imLastShow, imm, CV_GRAY2BGR);
	else
		imm = imLastShow.clone();
	cv::Rect roiFlip = cv::Rect(
		cv::Point(para_particle.roi.tl().x, imm.rows - para_particle.roi.br().y),
		cv::Point(para_particle.roi.br().x, imm.rows - para_particle.roi.tl().y));
	cv::rectangle(imm, roiFlip, cv::Scalar(255, 0, 0), 1);
	/*cv::rectangle(imm, para_particle.roi, cv::Scalar(255, 0, 0), 1);*/
	cv::namedWindow(str_win_roi_sel, 1);
	cv::setMouseCallback(str_win_roi_sel, mouseHandler, &imm/*NULL*/);
	cv::imshow(str_win_roi_sel, imm);
	cv::waitKey(0);
	cv::destroyWindow(str_win_roi_sel);
	cv::Size imsz = imm.size();
	imm.release();
	if (roiSel.size() == cv::Size()) {
		/*MessageBox(_T("本次没有设置检测区域，将以上一次设置区域"));*/
		MessageBox(_T(" ROI is not selected, we will use the roi last selected"));
		return;
	}
	else {
		MessageBox(_T("ROI selection is successful"));
		/*para_particle.roi = roiSel;*/
		para_particle.roi =  cv::Rect(
			cv::Point(roiSel.tl().x, imsz.height - roiSel.br().y),
			cv::Point(roiSel.br().x, imsz.height - roiSel.tl().y));
		roiSel = cv::Rect();
		return;
	}
}


int g_cam(-1);
//CString	cstr_root_path_batch;
void CImgProcOfflineDlg::BatchProcThread(void* _data) {	
	CImgProcOfflineDlg* dlg = (CImgProcOfflineDlg*)_data;
	dlg->m_grp_cnt = 0;
	for (g_cam = dlg->m_batch_st; g_cam <= dlg->m_batch_end; g_cam++) {
			/*dlg->para_particle.iVimOnlineSave = 2;*/
			/*dlg->m_cstr_root_path_batch = _T("Z:\\Pic\\2020_05_30_10_18_50\\");*/
			for (int grp = 0; grp < 2 ; grp++) {
				//uchar bmode = 0x1<< grp;
				//bmode &= dlg->m_batch_mode;
				if (grp == 0) {
					if (dlg->m_batch_mode & 0x1)
						dlg->cstr_root_path.Format(dlg->m_cstr_root_path_batch + _T("\\%02d\\false"), g_cam);
					else
						continue;
				}
				if (grp == 1) {
					if (dlg->m_batch_mode & 0x2)
						dlg->cstr_root_path.Format(dlg->m_cstr_root_path_batch + _T("\\%02d\\good"), g_cam);
					else
						continue;
				}

				bool iret = dlg->CheckPath();
				if (iret) {
					((CEdit*)dlg->GetDlgItem(IDC_ET_IMGS_PATH))->SetWindowTextW(dlg->cstr_root_path);
					dlg->cam_idx = g_cam;
					dlg->OnBnClickedBtnReadParaFromXml();
					dlg->OnBnClickedBtnProcSt();
					do {
						Sleep(500);
					} while (dlg->bProcessing);
				}
			}
	}
	g_cam = -1;

}

void CImgProcOfflineDlg::OnBnClickedBtnBatchPath()
{

	GetDlgItemText(IDC_ET_IMGS_PATH, m_cstr_root_path_batch);
	m_batch_st = GetDlgItemInt(IDC_ET_BATCH_ST);
	m_batch_end = GetDlgItemInt(IDC_ET_BATCH_END);
	if (m_hThreadBatch != NULL) {
		DWORD dw = WaitForSingleObject(m_hThreadBatch, 50);
		if (dw == WAIT_OBJECT_0) {
			if (m_hThreadBatch != NULL)
				CloseHandle(m_hThreadBatch);
			m_hThreadBatch = HANDLE(_beginthread(CImgProcOfflineDlg::BatchProcThread, 0, this));
		}
		else {
			MessageBox(_T("批量测试未结束"));
		}
	}else
		m_hThreadBatch = HANDLE(_beginthread(CImgProcOfflineDlg::BatchProcThread, 0, this));


}

bool CImgProcOfflineDlg::CheckPath() {



	DWORD dwAttr = GetFileAttributes(cstr_root_path);
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
		CString strtmp;
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("正在加载目录") + cstr_root_path + _T(".请稍后"));
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(FALSE);
		/*manage.collectImgPaths(cstr_root_path);*/
		if (g_cam == -1)
			manage.collectImgPaths(cstr_root_path, cstr_root_path);
		else
			manage.collectImgPaths(cstr_root_path, m_cstr_root_path_batch);
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("目录加载完成") + cstr_root_path);
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(TRUE);
		para_particle.roi = cv::Rect();
		int itoken(0);
		CString cstr = cstr_root_path;
		CString cstrCamIdx = _T("");
		while (cstr != _T("")) {
			cstrCamIdx = cstr;
			cstr = cstr_root_path.Tokenize(_T("\\"), itoken);
		}
		SetDlgItemTextW(IDC_ET_CAM_IDX, cstrCamIdx);
		cam_idx = _ttoi(cstrCamIdx);
		cstr_root_path += _T("\\");
		return true;
	}
	else {
		CString cstr;
		cstr.Format(_T("%s：目录不存在！"), cstr_root_path);
		MessageBox(cstr);
		cstr_root_path = _T("");
		return false;
	}
	
}

void CImgProcOfflineDlg::OnBnClickedBtnCheckFilePath()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit*)GetDlgItem(IDC_ET_IMGS_PATH))->GetWindowTextW(cstr_root_path);
	m_grp_cnt = 0;
	para_particle.iVimOnlineSave = 0;
	CheckPath();
}


void CImgProcOfflineDlg::OnBnClickedBtnReadParaFromXml()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstr;
	cstr = cstr_root_path;
	//if (g_cam == -1)
	//	cstr = cstr_root_path;
	//else
	//	cstr.Format(cstr_root_path_batch + _T("\\x%d\\"), g_cam);
	bool iret = readParaFromXml(para_particle, Unicode2StdString(cstr ));
	/*iret = iret && MiddleReadInterParaFromXml(vec_rtBlinkMask, cam_idx);*/
	iret = iret && MiddleReadInterParaFromXml(para_inter, Unicode2StdString(cstr) );
	myUpdateData(false);
	if (iret == true)
		/*::MessageBox(NULL, _T("para update from xml is sucessfull"), NULL, MB_OK)*/;
	else
		::MessageBox(NULL, _T("para update from xml is failed"), NULL, MB_OK);
}


void CImgProcOfflineDlg::OnBnClickedBtnWriteParaToXml()
{
	// TODO: 在此添加控件通知处理程序代码
	myUpdateData(true);
	if (para_particle.roi == cv::Rect()) {
		::MessageBox(NULL, _T("roi is empty, para xml file savig is failed"), NULL, MB_OK);
		return;
	}
	/*MiddleWriteInterParaToXml(vec_rtBlinkMask, cam_idx);*/
	MiddleWriteInterParaToXml(para_inter, Unicode2StdString(cstr_root_path) );
	writeParaToXml(para_particle, Unicode2StdString(cstr_root_path /*+ _T("\\para.xml")*/));
	::MessageBox(NULL, _T("para update write to  xml is sucessfull"), NULL, MB_OK);
}

void CImgProcOfflineDlg::myUpdateData(bool _bToVar)
{
	if (true == _bToVar) {

		para_particle.nozero_frm_sum_thr = GetDlgItemInt(IDC_ET_PARTICLE_PIC_NUM);
		para_particle.nozero_single_thr = GetDlgItemInt(IDC_ET_PRTICLE_PT_NUM);
		CString cstrVal;
		GetDlgItem(IDC_ET_PRTICLE_RATIO)->GetWindowTextW(cstrVal);
		para_particle.ratio = atof(Unicode2StdString(cstrVal).c_str() );
		GetDlgItem(IDC_ET_PRTICLE_SENSITY)->GetWindowTextW(cstrVal);
		para_particle.sensity = atof(Unicode2StdString(cstrVal).c_str());
		//delay_frame_ms = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
		//delay_seq_ms = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
		if (m_sle_true) {
			delay_frame_ms_true = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
			delay_seq_ms_true = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
		}
		else {

			delay_frame_ms_false = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
			delay_seq_ms_false = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
		}

	}
	else {
		SetDlgItemInt(IDC_ET_PARTICLE_PIC_NUM, para_particle.nozero_frm_sum_thr);
		SetDlgItemInt( IDC_ET_PRTICLE_PT_NUM, para_particle.nozero_single_thr);
		CString cstrVal;
		cstrVal.Format(_T("%f"), para_particle.ratio);
		GetDlgItem(IDC_ET_PRTICLE_RATIO)->SetWindowTextW(cstrVal);
		cstrVal.Format(_T("%f"), para_particle.sensity);
		GetDlgItem(IDC_ET_PRTICLE_SENSITY)->SetWindowTextW(cstrVal);
		//SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, delay_frame_ms);
		//SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms);
		if (m_sle_true) {
			SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, delay_frame_ms_true);
			SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms_true);
		}
		else {
			SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, delay_frame_ms_false);
			SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms_false);
		}
	}
}


void CImgProcOfflineDlg::OnBnClickedBtnParaInter()
{
	// TODO: 在此添加控件通知处理程序代码
	CDParaInter  dlg_paraInter(para_inter.particle_type, para_inter.pre_para, para_inter.is_proc_regPD);
	dlg_paraInter.DoModal();
}


void CImgProcOfflineDlg::OnBnClickedBtnRtBlinkClearAll()
{
	// TODO: 在此添加控件通知处理程序代码
	para_inter.rtMaskBlink.clear();
	bRtBlinkClear = true;
}




void CImgProcOfflineDlg::OnCbnSelchangeCmbPauseMode()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	int mode_cmb = ((CComboBox*)GetDlgItem(IDC_CMB_PAUSE_MODE))->GetCurSel();
	switch (mode_cmb)
	{
	case 0:
		m_sle_true = 0;
		SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, delay_frame_ms_false);
		SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms_false);
		break;
	case 1:
		m_sle_true = 1;
		SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME,delay_frame_ms_true);
		SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms_true);
		break;
	default:
		break;
	}
}




void CImgProcOfflineDlg::OnDropdownCmbPauseMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode_cmb = ((CComboBox*)GetDlgItem(IDC_CMB_PAUSE_MODE))->GetCurSel();
	switch (mode_cmb)
	{
	case 0:
		delay_frame_ms_false = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME );
		delay_seq_ms_false = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ );
		break;
	case 1:
		delay_frame_ms_true = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME );
		delay_seq_ms_true = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ );
		break;
	default:
		break;
	}
}


void CImgProcOfflineDlg::OnCbnSelchangeCmbBatchMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode_cmb = ((CComboBox*)GetDlgItem(IDC_CMB_BATCH_MODE))->GetCurSel();
	switch (mode_cmb)
	{
	case 0:
		m_batch_mode = 0x1;
		break;
	case 1:
		m_batch_mode = 0x2;
		break;
	case 2:
		m_batch_mode = 0x3;
		break;
	default:
		break;
	}
	
}


void CImgProcOfflineDlg::OnCbnSelchangeCmbRltSave()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode_cmb = ((CComboBox*)GetDlgItem(IDC_CMB_RLT_SAVE))->GetCurSel();
	switch (mode_cmb)
	{
	case 0:
		para_particle.iVimOnlineSave = 0;
		break;
	case 1:
		para_particle.iVimOnlineSave = 1;
		break;
	case 2:
		para_particle.iVimOnlineSave = 2;
		break;
	case 3:
		para_particle.iVimOnlineSave = 3;
		break;
	default:
		break;
	}
	
}


void CImgProcOfflineDlg::OnCbnSelchangeCmbImgBright()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode_cmb = ((CComboBox*)GetDlgItem(IDC_CMB_IMG_BRIGHT))->GetCurSel();
	
	m_bright_times = mode_cmb + 1;
	
}


void CImgProcOfflineDlg::OnCbnSelchangeCmbCopySrc()
{
	// TODO: 在此添加控件通知处理程序代码
	 m_cmb_copy_src = ((CComboBox*)GetDlgItem(IDC_CMB_COPY_SRC))->GetCurSel();
}


void CImgProcOfflineDlg::OnCbnSelchangeCmbCopyDst()
{
	// TODO: 在此添加控件通知处理程序代码
	 m_cmb_copy_dst = ((CComboBox*)GetDlgItem(IDC_CMB_COPY_DST))->GetCurSel();
}


void CImgProcOfflineDlg::OnBnClickedBtnCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	int num_st = GetDlgItemInt(IDC_ET_COPY_ST);
	int num_end = GetDlgItemInt(IDC_ET_COPY_END);
	CString root_src, root_dst;
	GetDlgItemText(IDC_ET_COPY_SRC, root_src);
	GetDlgItemText(IDC_ET_COPY_DST, root_dst);
	for (int num = num_st; num <= num_end; num++) {
		char ch[100];
		sprintf(ch, "%02d", num);
		CString cnum = StdString2Unicode(ch);
		CString cstr_src_path;
		switch (m_cmb_copy_src)
		{
		case 0:
			cstr_src_path = root_src + _T("\\") + cnum + _T("\\");
			break;
		case 1:
			cstr_src_path = root_src + _T("\\") + cnum + _T("\\good\\");
			break;
		case 2:
			cstr_src_path = root_src + _T("\\") + cnum + _T("\\false\\");
			break;
		default:
			break;
		}

		CString cstr_src_para = cstr_src_path + _T("para.xml");
		CString cstr_src_para_inter = cstr_src_path + _T("paraInter.xml");

		CString cstr_dst_path = root_dst + _T("\\") + cnum ;
		vector<CString> vec_dst;
		switch (m_cmb_copy_dst)
		{
		case 0:
		{
			CString dst = cstr_dst_path + _T("\\");
			vec_dst.push_back(dst);
		}
			break;
		case 1:
		{
			CString dst = cstr_dst_path + _T("\\good\\") ;
			vec_dst.push_back(dst);

		}
			break;
		case 2:
		{
			CString dst = cstr_dst_path + _T("\\false\\");
			vec_dst.push_back(dst);
		}
			break;
		case 3:
		{
			CString dst = cstr_dst_path + _T("\\good\\");
			vec_dst.push_back(dst);

			dst = cstr_dst_path + _T("\\false\\");
			vec_dst.push_back(dst);

		}
			break;
		default:
			break;
		}
		for (vector<CString>::iterator it = vec_dst.begin(); it != vec_dst.end(); it++) {

			if (MakeSureDirectoryPathExists(Unicode2StdString(*it).c_str()) == FALSE) {
				MessageBox(NULL, *it + _T(":目录生成失败"), NULL);
				continue;
			}
			BOOL m = CopyFile(cstr_src_para, *it + _T("para.xml"), false);
			if (!m)
				MessageBox(cstr_src_para + *it + _T(":失败!"));//这里是添加消息提示

		    m = CopyFile(cstr_src_para_inter, *it + _T("paraInter.xml"), false);
			if (!m)
				MessageBox(cstr_src_para_inter + *it + _T(":失败!"));//这里是添加消息提示
		}


	}
}
