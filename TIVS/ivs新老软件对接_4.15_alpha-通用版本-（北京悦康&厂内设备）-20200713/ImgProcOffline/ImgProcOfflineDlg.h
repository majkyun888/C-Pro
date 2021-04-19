
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
	bool bRtBlinkClear;
	/*cv::Mat imForRoiSel;*/

	int m_bright_times ;
	/*bool m_rlt_save;*/
	uchar m_batch_mode;
	int m_batch_st, m_batch_end;
	CString m_cstr_root_path_batch;
	int m_cmb_copy_src, m_cmb_copy_dst;

// 实现
protected:
	bool bProcessing;
	HICON m_hIcon;
	HANDLE m_hThreadHandle;
	static void ImgProcThread(void* _data);
	HANDLE m_hThreadBatch;
	static void BatchProcThread(void* _data);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	bool CheckPath();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnProcSt();
	afx_msg void OnBnClickedBtnSelRootDir();
	afx_msg void OnBnClickedBtnRoiSel();
	int m_sle_true;
	int m_grp_cnt;
	int delay_frame_ms_true;
	int delay_frame_ms_false;
	afx_msg void OnBnClickedBtnCheckFilePath();
	int delay_seq_ms_true;
	int delay_seq_ms_false;
	afx_msg void OnBnClickedBtnReadParaFromXml();
	afx_msg void OnBnClickedBtnWriteParaToXml();
	void myUpdateData(bool _bToVar);
	afx_msg void OnBnClickedBtnParaInter();
	afx_msg void OnBnClickedBtnRtBlinkClearAll();
	afx_msg void OnBnClickedBtnBatchPath();
	afx_msg void OnCbnSelchangeCmbPauseMode();
	afx_msg void OnDropdownCmbPauseMode();
	afx_msg void OnCbnSelchangeCmbBatchMode();
	afx_msg void OnCbnSelchangeCmbRltSave();
	afx_msg void OnCbnSelchangeCmbImgBright();
	afx_msg void OnCbnSelchangeCmbCopySrc();
	afx_msg void OnCbnSelchangeCmbCopyDst();
	afx_msg void OnBnClickedBtnCopy();
};


// 1) 确认目录：OnBnClickedBtnCheckFilePath()
// 2）读取xml： OnBnClickedBtnReadParaFromXml()
// 3） 开始检测：OnBnClickedBtnProcSt()