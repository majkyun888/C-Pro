
// ImgProcOfflineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgProcOffline.h"
#include "ImgProcOfflineDlg.h"
#include "afxdialogex.h"
#include "CDParaInter.h"
#include "..//MiddlewareImprocessDll/ParaXml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib,"../IVS/Debug/MiddlewareImprocessDll.lib")
#else
#pragma comment(lib,"../IVS/Release/MiddlewareImprocessDll.lib")
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


// CImgProcOfflineDlg �Ի���

static string str_win_roi_sel = "ROI_SELECT";


CImgProcOfflineDlg::CImgProcOfflineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgProcOfflineDlg::IDD, pParent)
	, delay_frame_ms(1)
	, delay_seq_ms(1)
{
	/*para_particle.iVimRltSave = true;*/
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThreadHandle = NULL;
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
	para_particle.iVimOnlineSave = 3;
}

void CImgProcOfflineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_PARTICLE_PIC_NUM, para_particle.nozero_frm_sum_thr);
	DDX_Text(pDX, IDC_ET_PRTICLE_PT_NUM, para_particle.nozero_single_thr);
	DDX_Text(pDX, IDC_ET_PRTICLE_RATIO, para_particle.ratio);
	DDX_Text(pDX, IDC_ET_PRTICLE_SENSITY, para_particle.sensity);
	DDX_Text(pDX, IDC_ET_DELAY_TIME_FRAME, delay_frame_ms);
	DDX_Text(pDX, IDC_ET_DELAY_TIME_SEQ, delay_seq_ms);
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
string str_win_rlt("IM_RLT");
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
		//roiBlinkSel = cv::Rect(roiBlinkSel.tl(), cv::Point(x, y));
		//cv::rectangle(img1, roiBlinkSel, cv::Scalar(0, 255, 0), 1);
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
END_MESSAGE_MAP()


// CImgProcOfflineDlg ��Ϣ�������



BOOL CImgProcOfflineDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgProcOfflineDlg::OnPaint()
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
		MessageBox( StdString2Unicode(ch) );
	}
	if (para_particle.roi.size() == cv::Size()) {
		MessageBox(_T("roi is not selected ,please return"));
		return;
	}
		
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (manage.imSampleFullPath == _T("")) {
		MessageBox(_T("the image sequnece PATH is not selected ,please return"));
		return;
	}
		
	if (m_hThreadHandle == NULL) {
		m_hThreadHandle = HANDLE(_beginthread(CImgProcOfflineDlg::ImgProcThread, 0, this));
		/*GetDlgItem(IDC_BTN_PROC_ST)->EnableWindow(FALSE);*/
	}	
	else
		MessageBox(_T("processing"));
}


void CImgProcOfflineDlg::ImgProcThread(void* _data)
{
	CImgProcOfflineDlg* dlg = (CImgProcOfflineDlg*)_data;
	
	/*cv::namedWindow(str_win_rlt, 1);*/
	cv::namedWindow(str_win_rlt, cv::WINDOW_NORMAL );
	/*cv::resizeWindow(str_win_rlt, 1500, 800);*/
	cv::resizeWindow(str_win_rlt, 1920, 1080);
	cv::moveWindow(str_win_rlt, 0, 0);
	cv::setMouseCallback(str_win_rlt, mouseBlinkRectHandler, dlg/*NULL*/);
	bool toNext = false;
	/*int cam_idx = dlg->GetDlgItemInt(IDC_ET_CAM_IDX);*/
	while (dlg->manage.procOnce(dlg->para_particle,dlg->para_inter, toNext, dlg->cam_idx))
	{
		dlg->isBlinkActive = false;
		for (int i = 0; i < dlg->manage.imrlts.size(); i++) {
			cv::Mat im = dlg->manage.imrlts.at(i);
			if (im.empty())
				continue;
			for (vector<cv::Rect>::iterator it = dlg->para_inter.rtMaskBlink.begin();
				it != dlg->para_inter.rtMaskBlink.end(); it++)
				cv::rectangle(im, *it, cv::Scalar(255, 0, 0), 1);
			cv::imshow(str_win_rlt, im);
			dlg->imLastShow = im;
			dlg->delay_frame_ms = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
			cv::waitKey(dlg->delay_frame_ms);
		}
		dlg->delay_seq_ms = dlg->GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
		if (dlg->delay_frame_ms == 0)
			dlg->isBlinkActive = true;
		char ch = cv::waitKey(dlg->delay_seq_ms);
		if (ch == 'j' || ch == 'J')
			toNext = false;
		else
			toNext = true;
		dlg->myUpdateData(true);
		/*dlg->UpdateData(true);*/
	}
	cv::destroyWindow(str_win_rlt);
	dlg->m_hThreadHandle = NULL;
	/*dlg->GetDlgItem(IDC_BTN_PROC_ST)->EnableWindow(TRUE);*/
}

void CImgProcOfflineDlg::OnBnClickedBtnSelRootDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*CString rootPath;*/

	//���ļ���
	BROWSEINFO br;
	ZeroMemory(&br, sizeof(BROWSEINFO));
	br.hwndOwner = GetSafeHwnd();
	br.ulFlags = BIF_RETURNONLYFSDIRS;//ֻ�����ļ�Ŀ¼��Ϣ��
	LPITEMIDLIST idl;//��ĿID�б�
	idl = SHBrowseForFolder(&br);
	//�ļ��ľ���·����
	TCHAR szPath[MAX_PATH * 2];
	ZeroMemory(szPath, sizeof(TCHAR) * (MAX_PATH * 2));
	//szPath[0]=_T('\0');
	if (idl)
	{
		SHGetPathFromIDList(idl, szPath);//����Ŀ��Ƿ��б�ת����CString���ͣ�
		IMalloc* pMalloc;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			pMalloc->Free(idl);
			pMalloc->Release();
		}
		CString strtmp;
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp +_T("���ڼ���Ŀ¼")+szPath + _T(".���Ժ�"));
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(FALSE);
		/*rootPath = szPath;*/
		manage.collectImgPaths(szPath);
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("Ŀ¼�������") + szPath );
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(TRUE);
		para_particle.roi = cv::Rect();
		((CEdit*)GetDlgItem(IDC_ET_IMGS_PATH))->SetWindowTextW(szPath);
	}
	else
		return;
}


void CImgProcOfflineDlg::OnBnClickedBtnRoiSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat imForRoiSel = cv::imread(Unicode2StdString(manage.imSampleFullPath), CV_LOAD_IMAGE_ANYCOLOR);
	if (imForRoiSel.empty() == true) {
		CString str;
		/*str.Format(_T("û�м���ͼ�񣬼������ѡ��ʧ��,��ȷ��·��%s"), manage.imSampleFullPath);*/
		str.Format(_T("image is not loaded��the ROI selection failed ,please check the image sequece path %s"), manage.imSampleFullPath);
		MessageBox( str);
		return;
	}
	cv::Mat imm;
	if (imForRoiSel.channels() == 1)
		cv::cvtColor(imForRoiSel, imm, CV_GRAY2BGR);
	else
		imm = imForRoiSel.clone();
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
		/*MessageBox(_T("����û�����ü�����򣬽�����һ����������"));*/
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



void CImgProcOfflineDlg::OnBnClickedBtnCheckFilePath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*CString cstr_root_path;*/
	((CEdit*)GetDlgItem(IDC_ET_IMGS_PATH))->GetWindowTextW(cstr_root_path);
	DWORD dwAttr = GetFileAttributes(cstr_root_path);
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
		CString strtmp;
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("���ڼ���Ŀ¼") + cstr_root_path + _T(".���Ժ�"));
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(FALSE);
		/*rootPath = szPath;*/
		manage.collectImgPaths(cstr_root_path);
		((CStatic*)GetDlgItem(IDC_DIS_TXT))->SetWindowTextW(strtmp + _T("Ŀ¼�������") + cstr_root_path);
		this->GetDlgItem(IDC_BTN_SEL_ROOT_DIR)->EnableWindow(TRUE);
		para_particle.roi = cv::Rect();
		int itoken(0);
		CString cstr = cstr_root_path;
		CString cstrCamIdx = _T("");
		while (cstr != _T("") ) {
			cstrCamIdx = cstr;
			cstr = cstr_root_path.Tokenize(_T("\\"), itoken);
		}
		SetDlgItemTextW(IDC_ET_CAM_IDX, cstrCamIdx);
		cam_idx = _ttoi(cstrCamIdx);
	}
	else {
		CString cstr;
		cstr.Format(_T("%s��Ŀ¼�����ڣ�"), cstr_root_path);
		MessageBox(cstr);
		cstr_root_path = _T("");
	}
	cstr_root_path += _T("\\");
}


void CImgProcOfflineDlg::OnBnClickedBtnReadParaFromXml()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool iret = readParaFromXml(para_particle, Unicode2StdString(cstr_root_path /*+ _T("\\para.xml")*/));
	/*iret = iret && MiddleReadInterParaFromXml(vec_rtBlinkMask, cam_idx);*/
	iret = iret && MiddleReadInterParaFromXml(para_inter, Unicode2StdString(cstr_root_path) );
	myUpdateData(false);
	if (iret == true)
		::MessageBox(NULL, _T("para update from xml is sucessfull"), NULL, MB_OK);
	else
		::MessageBox(NULL, _T("para update from xml is failed"), NULL, MB_OK);
}


void CImgProcOfflineDlg::OnBnClickedBtnWriteParaToXml()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		delay_frame_ms = GetDlgItemInt(IDC_ET_DELAY_TIME_FRAME);
		delay_seq_ms = GetDlgItemInt(IDC_ET_DELAY_TIME_SEQ);
	}
	else {
		SetDlgItemInt(IDC_ET_PARTICLE_PIC_NUM, para_particle.nozero_frm_sum_thr);
		SetDlgItemInt( IDC_ET_PRTICLE_PT_NUM, para_particle.nozero_single_thr);
		CString cstrVal;
		cstrVal.Format(_T("%f"), para_particle.ratio);
		GetDlgItem(IDC_ET_PRTICLE_RATIO)->SetWindowTextW(cstrVal);
		cstrVal.Format(_T("%f"), para_particle.sensity);
		GetDlgItem(IDC_ET_PRTICLE_SENSITY)->SetWindowTextW(cstrVal);
		SetDlgItemInt(IDC_ET_DELAY_TIME_FRAME, delay_frame_ms);
		SetDlgItemInt(IDC_ET_DELAY_TIME_SEQ, delay_seq_ms);
	}
}


void CImgProcOfflineDlg::OnBnClickedBtnParaInter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDParaInter  dlg_paraInter(para_inter.particle_type, para_inter.pre_para, para_inter.is_proc_regPD);
	dlg_paraInter.DoModal();
}
