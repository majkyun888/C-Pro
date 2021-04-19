#include "stdafx.h"
#include "middlewareCommon.h"
#include "../LogTxt/AppLog.h"
#include "strConvert.h"
#include <imagehlp.h>  
#pragma comment(lib,"imagehlp.lib") 

CRITICAL_SECTION g_cs;
// ----------------------------------------------------------------------------------------------
MiddlewareImProcess::MiddlewareImProcess()
{
	bDetectionOnline = true;
	if (p_imsQueue == NULL)
		p_imsQueue = new CQueue(5/*20*/);
	if (NULL == pAppLog)
	{
		InitializeCriticalSection(&g_cs);
		//DeleteCriticalSection(&g_cs);
		EnterCriticalSection(&g_cs);
		if (NULL == pAppLog) {
			pAppLog = new CAppLog;
			pAppLog->Initialize(DEF_LOG_PATH);
			pAppLog->StartLog();
			pAppLog->Write2Log(_T("----------------------------"));
			pAppLog->Write2Log(_T("----------------------------"));
			time_t t = time(0);
			char chTime[200];
			memset(chTime, '\0', sizeof(chTime));
			strftime(chTime, sizeof(chTime), "%Y_%m_%d_%H_%M_%S", localtime(&t));
			CString cstrTime;
			cstrTime.Format(_T("本软件启动后第一次使用OpenCV检测%s"), StdString2Unicode(chTime));
			pAppLog->Write2Log(cstrTime);
		}
		LeaveCriticalSection(&g_cs);
	};
	hThreadRun = NULL;
	g_hevtRequestSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hevtResultReturned = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hMutex = CreateMutex(NULL, false, NULL);
	g_hRunMutex = CreateMutex(NULL, false, NULL);
	m_camIdx = -1;
	m_iProcErr = true;
}

MiddlewareImProcess::~MiddlewareImProcess()
{
	// 如果该类对应的实例是局部变量，那么每次都析构，会造成资源浪费
	//if (p_imsQueue != NULL)
	//	delete p_imsQueue;
	CloseHandle(g_hMutex);
	CloseHandle(g_hRunMutex);
	CloseHandle(g_hevtRequestSubmitted);
	CloseHandle(g_hevtResultReturned);
}


// 将检测的结果图转化成可以在主程序中显示的图像
void MiddlewareImProcess::cvMat2Ptr(unsigned int* _ptrDst, int _dimxDst, int _dimyDst, cv::Mat _imsrc) {

	cv::Mat im;
	if (_imsrc.empty())
		return;
	cv::resize(_imsrc, im, cv::Size(_dimxDst, _dimyDst));
	if (im.channels() == 1)
		cv::cvtColor(im, im, CV_GRAY2BGRA);
	else
		cv::cvtColor(im, im, CV_BGR2BGRA);
	CV_Assert(im.isContinuous());
	memcpy(_ptrDst, im.data, im.cols * im.rows * im.channels());
}

void MiddlewareImProcess::copySrcFrom(vector<cv::Mat> _srcs)
{
	vimsrc.assign(_srcs.begin(), _srcs.end());
}

void MiddlewareImProcess::copyRltTo(vector<cv::Mat>& _vimrlt)
{
	_vimrlt.assign(vimrlt.begin(), vimrlt.end());
}

int countImsOnSave(0);
void MiddlewareImProcess::imsOnlineSave(bool good, CString _cstr_CamID, cv::Mat imRlt)
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	CString sTime;
	sTime.Format(_T("%04d-%02d-%02d_%02dH_%02dM_%02dS_%03dms"), tm.wYear,
		tm.wMonth,
		tm.wDay,
		tm.wHour,
		tm.wMinute,
		tm.wSecond,
		tm.wMilliseconds);
	string str_time = Unicode2StdString(sTime);
	string str_good;
	if (true == good)
		str_good = "good";
	else
		str_good = "false";
	char ch[500] , chParaXml[500];
	if (_cstr_CamID == _T("")) {
		//sprintf(ch, "%s\\%s_%s_%04d\\", RootImageSaveFolder().c_str(), str_time.c_str(), str_good.c_str(), countImsOnSave++);
		//sprintf(chParaXml, "%s\\", RootImageSaveFolder().c_str());
		sprintf(ch, "%s\\%s\\%s_%s_%04d\\", RootImageSaveFolder().c_str(), str_good.c_str(), str_time.c_str(), str_good.c_str(), countImsOnSave++ );
		sprintf(chParaXml, "%s\\%s\\", RootImageSaveFolder().c_str(), str_good.c_str());
		MakeSureDirectoryPathExists(RootImageSaveFolder().c_str());
	}		
	else {
		//sprintf(ch, "%s\\%s\\~%s_%04d~%s_%s\\", RootImageSaveFolder().c_str(), Unicode2StdString(_cstr_CamID).c_str(),
		//	Unicode2StdString(_cstr_CamID).c_str(), countImsOnSave++, str_time.c_str(), str_good.c_str());
		//sprintf(chParaXml, "%s\\%s\\", RootImageSaveFolder().c_str(), Unicode2StdString(_cstr_CamID).c_str());
		//string strDir = (RootImageSaveFolder() + "\\" + Unicode2StdString(_cstr_CamID) + "\\");
		sprintf(ch, "%s\\%s\\%s\\~%s_%04d~%s_%s\\", RootImageSaveFolder().c_str(), Unicode2StdString(_cstr_CamID).c_str(), str_good.c_str(),
			Unicode2StdString(_cstr_CamID).c_str(), countImsOnSave++, str_time.c_str(), str_good.c_str());
		sprintf(chParaXml, "%s\\%s\\%s\\", RootImageSaveFolder().c_str(), Unicode2StdString(_cstr_CamID).c_str(), str_good.c_str() );
		string strDir = (RootImageSaveFolder() + "\\" + Unicode2StdString(_cstr_CamID) + "\\" + str_good.c_str() + "\\");
		MakeSureDirectoryPathExists( strDir.c_str()  );
	}
	ParaToXml(chParaXml);
	vector<cv::Mat> vimsave;
	if (vimsrc.size() > Q_FRAME - 1)  // 防止拷贝的内容超出分配堆的大小
		vimsave.assign(vimsrc.begin(), vimsrc.begin() + Q_FRAME -1);
	else
		vimsave.assign(vimsrc.begin(), vimsrc.end());
	//PRINT_LOG(StdString2Unicode(ch));
	
	if (imRlt.empty() != true) {
		cv::Mat imrltGray;
		if (imRlt.channels() == 3)
			cv::cvtColor(imRlt, imrltGray, CV_BGR2GRAY);
		else
			imrltGray = imRlt.clone();
		cv::transpose(imrltGray, imrltGray);
		cv::flip(imrltGray, imrltGray, 1);
		vimsave.push_back(imrltGray);
	}	
	ele_ims.cloneFrom(ch, vimsave);
	p_imsQueue->Append(&ele_ims, 30);
}

// ----------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------
MiddlewareImProcessSingle::MiddlewareImProcessSingle()
{
}

MiddlewareImProcessSingle::~MiddlewareImProcessSingle()
{
}
// ----------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------
MiddlewareImprocessSeq::MiddlewareImprocessSeq()
{
}

MiddlewareImprocessSeq::~MiddlewareImprocessSeq()
{
}

// 用于将相机图像转化成opencv处理所需的cv::Mat型容器
void MiddlewareImprocessSeq::ptrs2cvMats(uchar* _images[], int _dimx, int _dimy, int _seq) {
	// 需要判断取值是否有效，利用try尝试
	vimsrc.clear();
	for (int i = 0; i < _seq; i++) {
		cv::Mat im = cv::Mat::zeros(_dimy, _dimx, CV_MAKE_TYPE(CV_8U, 1));
		if (im.isContinuous()) {
			memcpy(im.data, _images[i], _dimy * _dimx);
			vimsrc.push_back(im);
		}
		else {
			;
#ifdef MsgDebug
			MsgBox(_T("ptrs2cvMats error"));
#endif // MsgDebug

		}

	}
}


//vector<cv::Rect> vecRoi( 10, cv::Rect( cv::Point(0,0), cv::Size(10,10) ) );

void MiddlewareImprocessSeq::ParaIn::write(cv::FileStorage& fs) const //Write serialization for this class
{
	fs << "{" << "nozero_single_thr" << nozero_single_thr
		<< "nozero_frm_sum_thr" << nozero_frm_sum_thr
		<< "ratio" << ratio
		<< "sensity" << sensity
		<< "roi" << roi;
	fs << "}";
}
void MiddlewareImprocessSeq::ParaIn::read(const cv::FileNode& node)  //Read serialization for this class
{

	nozero_single_thr = (int)node["nozero_single_thr"];
	nozero_frm_sum_thr = (int)node["nozero_frm_sum_thr"];
	ratio = (float)node["ratio"];
	sensity = (float)node["sensity"];
	vector<int> rect;
	node["roi"] >> rect;
	roi = cv::Rect(rect[0], rect[1], rect[2], rect[3]);
}

////----------------------------------------------------------------------------------------------
////These write and read functions must exist as per the inline functions in operations.hpp
//static void write(cv::FileStorage& fs, const std::string&, const MiddlewareImprocessSeq::ParaIn& x) {
//	x.write(fs);
//}
//static void read(const cv::FileNode& node, MiddlewareImprocessSeq::ParaIn& x, const MiddlewareImprocessSeq::ParaIn& default_value = MiddlewareImprocessSeq::ParaIn()) {
//	if (node.empty())
//		x = default_value;
//	else
//		x.read(node);
//}
//
//bool readParaFromXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path)
//{
//	cv::FileStorage fs(_xml_path+"para.xml", cv::FileStorage::READ);
//	if (!fs.isOpened())
//	{
//		CString strCerr;
//		strCerr.Format(_T("failed to open xml from %s"), StdString2Unicode(_xml_path) );
//		PRINT_LOG(strCerr);
//		::MessageBox(NULL, strCerr, NULL, MB_OK);
//		return false;
//	}
//	cv::FileNode node = fs["para_particle"];
//	if (node.type() == cv::FileNode::NONE || node.type() == cv::FileNode::EMPTY)
//	{
//		CString strCerr;
//		strCerr.Format(_T("failed to read xml Para from %s"), StdString2Unicode(_xml_path) );
//		PRINT_LOG(strCerr);
//		::MessageBox(NULL, strCerr, NULL, MB_OK);
//		return false;
//	}
//	node >> _para;
//	fs.release();
//	return true;
//}
//
//bool writeParaToXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path)
//{
//	cv::FileStorage fs(_xml_path+ "para.xml", cv::FileStorage::WRITE);
//	if (!fs.isOpened())
//	{
//		CString strCerr;
//		strCerr.Format(_T("failed to open xml from %s"), StdString2Unicode(_xml_path) );
//		PRINT_LOG(strCerr);
//		::MessageBox(NULL, strCerr, NULL, MB_OK);
//		return false;
//	}
//	fs << "para_particle" << _para;
//	fs.release();
//	return true;
//}