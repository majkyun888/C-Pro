
// ImgProcOfflineDlg.h : 头文件
//

#pragma once

#include "csvRlt.h"
#include "Manange.h"

// CImgProcOfflineDlg 对话框
class CImgProcOfflineDlg : public CDialogEx
{
// 构造
public:
	CImgProcOfflineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMGPROCOFFLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	Manange manage;
	MiddlewareImprocessSeq::ParaIn para_particle;
	ParaInter para_inter;
	/*vector<cv::Rect> vec_rtBlinkMask;*/
	cv::Mat imLastShow;
	bool isBlinkActive;
	static void  mouseHandler(int event, int x, int y, int, void* pcvim);
	static void  mouseBlinkRectHandler(int event, int x, int y, int, void* _pdlg);
	static cv::Rect roiSel;
	CString cstr_root_path;// 图像序列根目录
	int cam_idx;
	/*cv::Mat imForRoiSel;*/

// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hThreadHandle;
	static void ImgProcThread(void* _data);
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnProcSt();
	afx_msg void OnBnClickedBtnSelRootDir();
	afx_msg void OnBnClickedBtnRoiSel();
	int delay_frame_ms;
	afx_msg void OnBnClickedBtnCheckFilePath();
	int delay_seq_ms;
	afx_msg void OnBnClickedBtnReadParaFromXml();
	afx_msg void OnBnClickedBtnWriteParaToXml();
	void myUpdateData(bool _bToVar);
	afx_msg void OnBnClickedBtnParaInter();
};
