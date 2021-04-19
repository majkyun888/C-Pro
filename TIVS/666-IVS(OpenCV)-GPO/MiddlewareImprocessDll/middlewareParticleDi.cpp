#include "stdafx.h"
#include "middlewareParticleDi.h"
#include "../Im-particle-di/procParticleDi.h"
#include "../LogTxt/AppLog.h"
#include "strConvert.h"
#include "ParaXml.h"

#ifdef _DEBUG
#pragma comment(lib,"../IVS/Debug/Im-particle-di.lib")
#else
#pragma comment(lib,"../IVS/Release/Im-particle-di.lib")
#endif


///////////////////////////// chBEGINTHREADEX Macro ///////////////////////////


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// I created this macro to perform the casting.
typedef unsigned(__stdcall* PTHREAD_START) (void*);

#define chBEGINTHREADEX(psa, cbStackSize, pfnStartAddr, \
   pvParam, dwCreateFlags, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                          \
         (void *)        (psa),                         \
         (unsigned)      (cbStackSize),                 \
         (PTHREAD_START) (pfnStartAddr),                \
         (void *)        (pvParam),                     \
         (unsigned)      (dwCreateFlags),               \
         (unsigned *)    (pdwThreadId)))

///////////////////////////////////////////////////////////////////////////////
static bool checkImgValid(cv::Mat im, CString _str_info) {
	if (im.empty() == true) {
		CString cstr;
		cstr.Format(_T("%s is empty"), _str_info);
		PRINT_LOG(cstr);
		return false;
	}
	else
		return true;
}


//void ParaInter::write(cv::FileStorage& fs) const //Write serialization for this class
//{
//	fs << "{" << "test" << test;
//	fs << "rtMaskBlink" << "[";
//	for (size_t idx = 0; idx < rtMaskBlink.size(); idx++)
//		fs << rtMaskBlink[idx];
//	fs << "]";
//	fs << "PrePara" << "{";
//	fs << "btThick" << pre_para.btThick;
//	fs << "lenMax" << pre_para.lenMax;
//	fs << "lenMin" << pre_para.lenMin;
//	fs << "level_shiftDown" << pre_para.level_shiftDown;
//	fs << "level_shiftUp" << pre_para.level_shiftUp;
//	fs << "ptSft" << pre_para.ptSft;
//	fs << "rangeDetectMin" << pre_para.rangeDetectMin;
//	fs << "thr_bottom_br" << pre_para.thr_bottom_br;
//	fs << "thr_bottom_cnt" << pre_para.thr_bottom_cnt;
//	fs << "thr_diff_br" << pre_para.thr_diff_br;
//	fs << "thr_diff_cnt" << pre_para.thr_diff_cnt;
//	fs << "thr_level_br" << pre_para.thr_level_br;
//	fs << "thr_level_cnt" << pre_para.thr_level_cnt;
//	fs << "thr_locy_br" << pre_para.thr_locy_br;
//	fs << "thr_locy_cnt" << pre_para.thr_locy_cnt;
//	fs << "thr_side_br" << pre_para.thr_side_br;
//	fs << "scanUp" << pre_para.scanUp;
//	fs << "}";
//	fs << "is_proc_regPD" << "{";
//	fs << "iFindBottom" << (int)is_proc_regPD.iFindBottom;
//	fs << "iFindLevel"<< (int)is_proc_regPD.iFindLevel;
//	fs << "iFindSide"<< (int)is_proc_regPD.iFindSide;
//	fs << "iRegBottom"<< (int)is_proc_regPD.iRegBottom;
//	fs << "iRegSide"<< (int)is_proc_regPD.iRegSide;
//	fs << "}";
//	fs << "particle_type" << (int)particle_type;
//	fs << "}";
//}
//
//void ParaInter::read(const cv::FileNode& node)  //Read serialization for this class
//{
//	test = (int)node["test"];
//	cv::FileNode n = node["PrePara"];
//	if (n.type() != cv::FileNode::MAP)
//	{
//		cerr << "PrePara is not existed! FAIL" << endl;
//		return;
//	}
//	pre_para.btThick = n["btThick"];
//	pre_para.lenMax = n["lenMax"];
//	pre_para.lenMin = n["lenMin"];
//	pre_para.level_shiftDown = n["level_shiftDown"];
//	pre_para.level_shiftUp = n["level_shiftUp"];
//	vector<int> pt; n["ptSft"] >> pt;  pre_para.ptSft = cv::Point(pt[0], pt[1]);;
//	pre_para.rangeDetectMin = n["rangeDetectMin"];
//	pre_para.thr_bottom_br = n["thr_bottom_br"];
//	pre_para.thr_bottom_cnt = n["thr_bottom_cnt"];
//	pre_para.thr_diff_br = n["thr_diff_br"];
//	pre_para.thr_diff_cnt = n["thr_diff_cnt"];
//	pre_para.thr_level_br = n["thr_level_br"];
//	pre_para.thr_level_cnt = n["thr_level_cnt"];
//	pre_para.thr_locy_br = n["thr_locy_br"];
//	pre_para.thr_locy_cnt = n["thr_locy_cnt"];
//	pre_para.thr_side_br = n["thr_side_br"];
//	pre_para.scanUp = n["scanUp"];
//	n = node["is_proc_regPD"];
//	if (n.type() != cv::FileNode::MAP)
//	{
//		cerr << "PrePara is not existed! FAIL" << endl;
//		return;
//	}
//	is_proc_regPD.iFindBottom =  (int)n["iFindBottom"];
//	is_proc_regPD.iFindLevel = (int)n["iFindLevel"];
//	is_proc_regPD.iFindSide = (int)n["iFindSide"];
//	is_proc_regPD.iRegBottom = (int)n["iRegBottom"];
//	is_proc_regPD.iRegSide = (int)n["iRegSide"];
//	int val_particle_type(-1);
//	val_particle_type = node["particle_type"];
//	switch (val_particle_type)
//	{
//	case 0:
//		particle_type = algoParticleDi::particle_di;
//		break;
//	case 1:
//		particle_type = algoParticleDi::particle_bei;
//		break;
//	default:
//		PRINT_LOG(_T("particle_type loading is error"));
//		break;
//	}
//	isRead = true;
//	n = node["rtMaskBlink"];
//	if (n.type() != cv::FileNode::SEQ)
//	{
//		cerr << "rtMaskBlink is not existed! FAIL" << endl;
//		return;
//	}
//	rtMaskBlink.clear();
//	cv::FileNodeIterator it = n.begin(), it_end = n.end();
//	for (; it != it_end; ++it)
//	{
//		vector<int> rtCor;
//		*it >> rtCor;
//		rtMaskBlink.push_back(cv::Rect(rtCor[0], rtCor[1], rtCor[2], rtCor[3]));
//	}
//
//}

////----------------------------------------------------------------------------------------------
////These write and read functions must exist as per the inline functions in operations.hpp
//static void write(cv::FileStorage& fs, const std::string&, const ParaInter& x) {
//	x.write(fs);
//}
//static void read(const cv::FileNode& node, ParaInter& x, const ParaInter& default_value = ParaInter()) {
//	if (node.empty())
//		x = default_value;
//	else
//		x.read(node);
//}
//
//bool readInterParaFromXml(ParaInter& _para, string _xml_path)
//{
//	cv::FileStorage fs(_xml_path, cv::FileStorage::READ);
//	if (!fs.isOpened())
//	{
//		return false;
//	}
//	cv::FileNode node = fs["ParaInter"];
//	if (node.type() == cv::FileNode::NONE || node.type() == cv::FileNode::EMPTY)
//	{
//		return false;
//	}
//	node >> _para;
//	fs.release();
//	return true;
//}
//
//bool writeInterParaToXml(ParaInter& _para, string _xml_path)
//{
//	cv::FileStorage fs(_xml_path, cv::FileStorage::WRITE);
//	if (!fs.isOpened())
//	{
//		return false;
//	}
//	fs << "ParaInter" << _para;
//	fs.release();
//	return true;
//}
//
//bool MiddleWriteInterParaToXml(ParaInter& _para_inter, string _path/*, int _camIdx*/) {
//	return writeInterParaToXml(_para_inter, _path+"paraInter.xml");
//}
//
//bool MiddleReadInterParaFromXml(ParaInter& _para_inter, string _path/*, int _camIdx*/) {
//	return readInterParaFromXml(_para_inter, _path + "paraInter.xml");
//}

bool MiddlewareParticleDi::ParaFromXml(string _xml_path) {
	bool bParaInter = MiddleReadInterParaFromXml(*(ParaInter*)ppara_inter/*para_inter*/, _xml_path);
	if (true == bDetectionOnline)
		return bParaInter;
	bool bPara = readParaFromXml(para, _xml_path);
	return bPara && bParaInter;
}

bool MiddlewareParticleDi::ParaToXml(string _xml_path) {
	bool bParaInter = MiddleWriteInterParaToXml(*(ParaInter*)ppara_inter/*para_inter*/, _xml_path);
	bool bPara = writeParaToXml(para, _xml_path);
	return bPara && bParaInter;
}


////void MiddleWriteInterParaToXml(vector<cv::Rect>& _blinkRect, int _camIdx) {
//void MiddleWriteInterParaToXml(ParaInter& _para_inter, int _camIdx){
//	CString cstrParaInterXmlPath;
//	cstrParaInterXmlPath.Format(_T("C:/ParaXml/%d/paraInter.xml"), _camIdx);
//	ParaInter para_inter;
//	para_inter.rtMaskBlink = _blinkRect;
//	writeInterParaToXml(para_inter, Unicode2StdString(cstrParaInterXmlPath));
//}
//
////bool MiddleReadInterParaFromXml(vector<cv::Rect>& _blinkRect, int _camIdx) {
//bool MiddleReadInterParaFromXml(ParaInter& _para_inter, int _camIdx){
//	CString cstrParaInterXmlPath;
//	cstrParaInterXmlPath.Format(_T("C:/ParaXml/%d/paraInter.xml"), _camIdx);
//	ParaInter para_inter;
//	bool iret =  readInterParaFromXml(para_inter, Unicode2StdString(cstrParaInterXmlPath) );
//	if (false == iret)
//		return false;
//	_blinkRect = para_inter.rtMaskBlink;
//	return true;
//}

bool MiddlewareParticleDi::run(RltOut& _rlt, int _camIdx, cv::Mat& _imcolor_rlt)
{
	if (m_camIdx == -1)
		m_camIdx = _camIdx;
	if (_camIdx != m_camIdx)
		PRINT_LOG(_T("_camIdx is not equal to last timein  mainThrad"));; // error
	_rlt.nimgpt = 2 * para.minnumimg;
	_rlt.totnpt = 2 * para.mintotnpt;  // 默认强制不合格，防止处理异常导致瓶子放行,临时性模拟写入值，今后须和王工重新对接
	DWORD dwRunMutex = WaitForSingleObject(g_hRunMutex, 10);  // 互斥量好像没有什么用，应该使用事件。
	if (dwRunMutex != WAIT_OBJECT_0)
	{
		PRINT_LOG(_T("g_hRunMutex conflit in  mainThrad "));
		return false;
	}
	vimrlt.clear();
	m_ret = false;	
	double t = m_tRequestSubmitted = cv::getTickCount();
	if (vimsrc.size() < 5) {
		PRINT_LOG(_T("vimsrc pic num is less than 5 in  mainThrad"));
		return false;
	}
	for (vector<cv::Mat>::iterator it = vimsrc.begin(); it != vimsrc.end(); it++) {
		if (it->empty()) {
			PRINT_LOG(_T("some mat in vimsrc is empty in  mainThrad"));
			return false;
		}
	}
	DWORD dwMutex(0);
	if (m_imcolorRlt.empty()) {
		cv::Size szImRlt = cv::Size(vimsrc[0].size().height, vimsrc[0].size().width);
		m_imcolorRlt = cv::Mat::zeros(szImRlt, CV_8UC3);
	}
	m_imcolorRlt = cv::Scalar::all(0);
	ResetEvent(g_hevtResultReturned);
	SetEvent(g_hevtRequestSubmitted/*[m_camIdx]*/);
	int waitMillSec = 430;
	if (para.iVimRltSave == true)
		waitMillSec = INFINITE; // 如果是离线阶段，则不考虑超时问题
	DWORD dwResultReturned = WaitForSingleObject(g_hevtResultReturned, waitMillSec);
	CString cstrTime;
	switch (dwResultReturned)
	{
	case WAIT_TIMEOUT: 
		ErrProc("ms_imcolorRlt is empty");
		PRINT_LOG(_T("g_hevtResultReturned timeout in  mainThrad dwResultReturned"));
		break;
	case WAIT_OBJECT_0:
		dwMutex = WaitForSingleObject(g_hMutex, 10);
		switch (dwMutex)
		{
		case WAIT_TIMEOUT:
			PRINT_LOG(_T("g_hMutex timeout in  mainThrad dwResultReturned"));
			ErrProc("g_hMutex timeout in  mainThrad dwResultReturned");
			break;
		case WAIT_OBJECT_0:
			checkImgValid(m_imcolorRlt, _T("m_imcolorRlt"));
			if (m_iProcErr == true)
				ErrProc("g_hMutex timeout in  mainThrad dwResultReturned")
			else
			{
				_rlt = static_cast<RltOut>(m_rltOut);
			}			
			if (para.iVimRltSave == true) {
				vimrlt.clear();
				for (vector<cv::Mat>::iterator it = vProcRlt.begin(); it != vProcRlt.end(); it++) {
					checkImgValid(*it, _T("ms_vimrlt element"));
					vimrlt.push_back(*it/*->clone()*/);
				}
			}

			ReleaseMutex(g_hMutex);
			t = ((double)cv::getTickCount() - t) / cv::getTickFrequency() * 1000;
			cstrTime.Format(_T("Image process cost time is %d"), (int)t);
			if (t > 250)
				PRINT_LOG(cstrTime);
			break;
		default:
			PRINT_LOG(_T("g_hMutex error in  mainThrad dwResultReturned"));
			break;
		}

		break;
	default:
		ErrProc("ms_imcolorRlt is empty")
		PRINT_LOG(_T("g_hevtResultReturned Err in  mainThrad dwResultReturned"));
		break;
	}
	ResetEvent(g_hevtRequestSubmitted);
	_imcolor_rlt = m_imcolorRlt;
	ReleaseMutex(g_hRunMutex);
	return m_ret;

}

MiddlewareParticleDi::MiddlewareParticleDi(ParaIn _para) :para(_para) 
{
	p_alg = NULL;
	p_apara = NULL;
	p_arlt = NULL;
	ppara_inter = NULL;
	ppara_inter = new ParaInter;
	if (hThreadRun == NULL) {
		DWORD dwThreadID;
		hThreadRun = chBEGINTHREADEX(NULL, 0, MiddlewareParticleDi::RunThread, (void*)this,
			0, &dwThreadID);
	}
}

MiddlewareParticleDi::~MiddlewareParticleDi()
{
	delete (ParaInter*)ppara_inter;
}


