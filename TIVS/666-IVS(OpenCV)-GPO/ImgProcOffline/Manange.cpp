#include "StdAfx.h"
#include "Manange.h"
#include <imagehlp.h>  
#pragma comment(lib,"imagehlp.lib") 

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


Manange::Manange()
{
	strExt = _T("*.png");
	p_middleware_particle_di = NULL;
	//numThread = 12;
	//for (int i = 0; i < numThread; i++)
	//	procThreads.push_back(ThreadMange(i));
	for (int i = 0; i < numThread; i++)
		procThreads[i] = new ThreadMange(i);
}


Manange::~Manange()
{
	delete p_middleware_particle_di;
}

void Manange::collectImgPaths(CString _cstrRoot)
{
	// 获取所有的子目录，并确保目录里面的png后缀结尾的文件为空
	/*cur_index = vecPath.size();*/
	vecPath.clear();
	vector<CString> vecTemp = GetFilePaths(_cstrRoot);
	imSampleFullPath = _T("");
	for (vector<CString>::iterator iter = vecTemp.begin();
		iter != vecTemp.end(); iter++)
	{
		CString cstr = scan_img_dir(*iter, imSampleFullPath);
		if (cstr != _T(""))
		{
			vecPath.push_back(cstr);
			::OutputDebugString(cstr);
		}
		else
			continue;
	}
	csv_rlt.init(_cstrRoot);
	itCurPath = vecPath.begin();
}

CString Manange::scan_img_dir(CString imgPath, CString& _imSampleFullPath)
{
	//读取文件夹中的图片；
	CString fileName, wholePath, finalPath(_T(""));
	imgPath += _T("\\");
	wholePath = imgPath;
	//imgPath += _T("\\img_*.png");//只读取.png类型的文件；过滤字符串，*.*是所有文件；为何使用*.*的时候不能遍历文件？
	/*imgPath += _T("\\*.bmp");*/
	/*imgPath += strExt;*/
	//HANDLE hFile;
	//WIN32_FIND_DATA fileData;
	//BOOL hasPic = false;
	//hFile = FindFirstFile(imgPath + _T("*.bmp"), &fileData);
	//do {
	//	fileName = fileData.cFileName;
	//	finalPath = wholePath + fileName;
	//	std::string std_str = Unicode2StdString(finalPath);
	//	cv::Mat  im = cv::imread(std_str, CV_LOAD_IMAGE_GRAYSCALE);
	//	if (im.rows)
	//		return wholePath;
	//	hasPic = FindNextFileW(hFile, &fileData);
	//} while (hasPic);

	HANDLE hFile2;
	WIN32_FIND_DATA fileData2;
	BOOL hasPic2 = false;
	hFile2 = FindFirstFile(imgPath + strExt, &fileData2);
	do {
		fileName = fileData2.cFileName;
		finalPath = wholePath + fileName;
		std::string std_str = Unicode2StdString(finalPath);
		cv::Mat  im = cv::imread(std_str, CV_LOAD_IMAGE_GRAYSCALE);
		if (im.rows) {
			_imSampleFullPath = finalPath;
			return wholePath;
		}
			
		hasPic2 = FindNextFileW(hFile2, &fileData2);
	} while (hasPic2);

	return CString(_T(""));
}


vector<CString> Manange::GetFilePaths(CString rootPath)
{
	CString strFilePath;
	strFilePath += rootPath;
	strFilePath += "//*.*";
	CFileFind finder;
	/*m_childFilePathList.clear();*/
	vector<CString> m_childFilePathList;
	BOOL bFind = finder.FindFile(strFilePath);
	while (bFind)
	{
		bFind = finder.FindNextFile();  //是否会漏一个，如果放到后面为何会异常退出。
		if (!finder.IsDots() && finder.IsDirectory())
		{
			CString strTempPath = finder.GetFilePath();
			m_childFilePathList.push_back(strTempPath);

			CFileFind finder_chilid;
			BOOL bFind_chilid = finder_chilid.FindFile(strTempPath + "//*.*");
			while (bFind_chilid)
			{
				bFind_chilid = finder_chilid.FindNextFile();  //是否会漏一个，如果放到后面为何会异常退出。
				if (!finder_chilid.IsDots() && finder_chilid.IsDirectory())
				{
					CString strTempPath_child = finder_chilid.GetFilePath();
					m_childFilePathList.push_back(strTempPath_child);
				}
			}
		}
	}
	m_childFilePathList.push_back(rootPath);
	finder.Close();
	return  m_childFilePathList;
}

bool Manange::procOnce(aPara _para, ParaInter _para_inter,bool _toNext, int _camIdx)
{
	if (true == _toNext)
		itCurPath++;

	if (itCurPath == vecPath.end()) {
		itCurPath = vecPath.begin();
		return false;
	}
		
	int idx = vecPath.end() - itCurPath;
	CString imPath = *(itCurPath);
	vector<cv::Mat> srcs = readImgFromPath(imPath);
	if(p_middleware_particle_di == NULL)
		p_middleware_particle_di = new MiddlewareParticleDi(_para);
	srcs.pop_back(); // 最后一张是结果，不要使用
	p_middleware_particle_di->copySrcFrom(srcs);
	cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
	p_middleware_particle_di->bDetectionOnline = false;
	p_middleware_particle_di->para = _para;
	*(ParaInter*)p_middleware_particle_di->ppara_inter = _para_inter;
	bool iret = p_middleware_particle_di->run(rlt_particleDi, _camIdx, imcolor);
	p_middleware_particle_di->copyRltTo(imrlts);

	for (int i = 0; i < numThread; i++)
		procThreads[i]->cloneSrcsFrom(srcs, _para);
	for (int i = 0; i < numThread; i++)
		SetEvent(procThreads[i]->g_hSeqSubmitted);
	int curPos(0);
	CString cstrTok = imPath.Tokenize(_T("~"), curPos);
	cstrTok = imPath.Tokenize(_T("~"), curPos);
	if (cstrTok == _T("")) {
		char dstr[20];
		sprintf(dstr, "%03d", idx);
		cstrTok = StdString2Unicode(dstr);
	}
	if(_para.iVimRltSave)
		ims2write(imPath, srcs, imrlts, iret, Unicode2StdString(cstrTok) );
	csv_rlt.write(idx, iret, imPath);
	/*delete p_middleware_particle_di;*/
	return true;

}

vector<cv::Mat> Manange::readImgFromPath(CString _imgpath)
{
	//读取文件夹中的图片；
	vector<cv::Mat> m_picList;
	CString fileName, wholePath, finalPath(_T(""));
	_imgpath += _T("\\");
	wholePath = _imgpath;
	_imgpath += strExt;
	HANDLE hFile;
	WIN32_FIND_DATA fileData;
	BOOL hasPic = false;
	hFile = FindFirstFile(_imgpath, &fileData);
	do {
		fileName = fileData.cFileName;
		finalPath = wholePath + fileName;
		std::string std_str = Unicode2StdString(finalPath);
		cv::Mat  im = cv::imread(std_str, CV_LOAD_IMAGE_GRAYSCALE);
		if (im.rows)
			m_picList.push_back(im);
		else
			continue;
		hasPic = FindNextFileW(hFile, &fileData);
	} while (hasPic);
	return m_picList;
}

void markText(cv::Mat& im, string _strIdx /*int _idx*/, int count) {
	char tstr[20];
	sprintf(tstr, "%02d", count);
	//char dstr[20];
	//sprintf(dstr, "%03d", _idx);
	int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 2;
	int thickness = 3;
	int baseline = 0;
	cv::Size textSize = cv::getTextSize(tstr, fontFace,
		fontScale, thickness, &baseline);
	cv::Scalar sclr = cv::Scalar(255, 0, 0);
	
	cv::putText(im, _strIdx/*dstr*/, cv::Point(0, im.rows/2), fontFace, fontScale, sclr, thickness, 8);

	cv::putText(im, tstr, cv::Point(0, textSize.height), fontFace, fontScale, sclr, thickness, 8);
	cv::putText(im, tstr, cv::Point(0, im.rows), fontFace, fontScale, sclr, thickness, 8);
	cv::putText(im, tstr, cv::Point(im.cols - textSize.width, textSize.height), fontFace, fontScale, sclr, thickness, 8);
	cv::putText(im, tstr, cv::Point(im.cols - textSize.width, im.rows), fontFace, fontScale, sclr, thickness, 8);
	cv::putText(im, tstr, cv::Point(im.cols / 2 - textSize.width, textSize.height), fontFace, fontScale, sclr, thickness, 8);
	cv::putText(im, tstr, cv::Point(im.cols / 2 - textSize.width, im.rows), fontFace, fontScale, sclr, thickness, 8);
}


void Manange::ims2write(CString _path, vector<cv::Mat> _srcs, vector<cv::Mat>& _rlts,bool _good, string _strIdx/*, int _idx*/)
{
	CString str;
	//if (_srcs.size() != _rlts.size()-1 ) {
	//	MessageBox(NULL, _T("输入序列和结果序列大小不同，可能图像处理异常，该运算中止"), NULL, NULL);
	//	return;
	//}
	int szRlt = _rlts.size();

	CString pathmiddle = _T("rlt");
	for (int i = 0; i < _srcs.size(); i++) {	

		cv::Mat imsrc = _srcs.at(i);
		if (imsrc.empty() == true)
			continue;
		if (imsrc.channels() == 1)
			cv::cvtColor(imsrc, imsrc, CV_GRAY2BGR);
		markText(imsrc, _strIdx/*, _idx*/, i);
		/*str.Format(_T("%s\\%04do.bmp"), _path, i);*/
		str.Format(_T("%s\\%s\\"), _path, pathmiddle);
		if (MakeSureDirectoryPathExists( Unicode2StdString(str).c_str() ) == FALSE ) {
			MessageBox(NULL, _T("写入目录生成失败"), NULL, NULL);
			return;
		}
		str.Format(_T("%s\\%s\\%04do.bmp"), _path, pathmiddle, i);
		if (imsrc.empty())
			continue;
		cv::imwrite(Unicode2StdString(str), imsrc);

		if ( i > szRlt -1 )
			continue;
		cv::Mat imrlt = _rlts.at(i);
		if (imrlt.empty() == true)
			continue;
		if (imrlt.channels() == 1)
			cv::cvtColor(imrlt, imrlt, CV_GRAY2BGR);
		markText(imrlt, _strIdx/*, _idx*/, i);
		/*str.Format(_T("%s\\%04dr.bmp"), _path, i);*/
		str.Format(_T("%s\\%s\\%04dr.bmp"), _path, pathmiddle, i);
	/*	if (imsrc.empty())
			continue;*/
		cv::imwrite(Unicode2StdString(str), imrlt );
	}
	//if (_good)
	//	str.Format(_T("%s\\rgood.bmp"), _path);
	//else
	//	str.Format(_T("%s\\rfalse.bmp"), _path);
	str.Format(_T("%s\\%s\\finalRlt.bmp"), _path, pathmiddle);
	if (_rlts.end() == _rlts.begin())
		return;
	if (_rlts.rbegin()->empty())
		return;
	cv::Mat& imrlt = *_rlts.rbegin();
	int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 2;
	int thickness = 3;
	cv::Scalar sclr = cv::Scalar(255, 0, 0);
	//char dstr[20];
	//sprintf(dstr, "%03d", _idx);
	cv::putText(imrlt, _strIdx/*dstr*/, cv::Point(0, imrlt.rows / 2), fontFace, fontScale, sclr, thickness, 8);
	cv::imwrite(Unicode2StdString(str), imrlt);

}


std::string Unicode2StdString(CString& cstr)
{


	//计算需要多少个字节才能表示对应的多字节字符串  
	LPCWSTR lpw_str = cstr;
	DWORD num = WideCharToMultiByte(CP_ACP, 0, lpw_str, -1, NULL, 0, NULL, 0);

	//开辟空间  
	char* pChar = NULL;
	pChar = (char*)malloc(num * sizeof(char));
	if (pChar == NULL)
	{
		free(pChar);
	}
	ZeroMemory(pChar, num * sizeof(char));
	//memset(pChar, 0, num * sizeof(char));

	//将宽字节字符串转换为多字节字符串  
	WideCharToMultiByte(CP_ACP, 0, lpw_str, -1, pChar, num, NULL, 0);

	std::string std_str = pChar;
	delete[] pChar;
	return std_str;

}

CString StdString2Unicode(std::string str)
{
	wchar_t* pWCHAR = NULL;
	LPCSTR lp_str = str.c_str();
	DWORD num = MultiByteToWideChar(CP_ACP, 0, lp_str, -1, NULL, 0);
	pWCHAR = (wchar_t*)malloc(num * sizeof(wchar_t));
	if (pWCHAR == NULL)
	{
		free(pWCHAR);
	}

	//memset(pWCHAR, 0, num * sizeof(wchar_t));
	ZeroMemory(pWCHAR, num * sizeof(wchar_t));
	//多字节转换为宽字节  
	MultiByteToWideChar(CP_ACP, 0, lp_str, -1, pWCHAR, num);
	CString cstr = pWCHAR;
	delete[] pWCHAR;
	return cstr;


}



ThreadMange::ThreadMange(int _ID):ID(_ID)
{
 	p_middleware_particle_di = NULL;
	g_hSeqSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hRltReturned = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hMutex = CreateMutex(NULL, false, NULL);
	DWORD dwThreadID;
	chBEGINTHREADEX(NULL, 0, ThreadMange::runThread, (void*)this,
		0, &dwThreadID);
}

ThreadMange::~ThreadMange()
{
	CloseHandle(g_hSeqSubmitted);
	CloseHandle(g_hRltReturned);
	CloseHandle(g_hMutex);
	if (NULL != p_middleware_particle_di)
		delete p_middleware_particle_di;
}

void ThreadMange::cloneSrcsFrom(vector<cv::Mat> _srcs, /*algParticleDi::*/aPara _para)
{

	if (p_middleware_particle_di == NULL)
		p_middleware_particle_di = new MiddlewareParticleDi(_para);
	p_middleware_particle_di->bDetectionOnline = false;
	DWORD dwMutex = WaitForSingleObject(g_hMutex, 10);
	switch (dwMutex)
	{
	case WAIT_TIMEOUT:
		break;
	case WAIT_OBJECT_0: 
	{
		vsrc.clear();
		for (int i = 0; i < _srcs.size(); i++)
			vsrc.push_back(_srcs[i].clone());
		ReleaseMutex(g_hMutex);
	}
		break;
	default:
		break;
	}

}

DWORD WINAPI ThreadMange::runThread(PVOID pvParam)
{
	BOOL fShutdown = FALSE;
	ThreadMange* p_threadManage = (ThreadMange*)pvParam;
	while (!fShutdown)
	{
		WaitForSingleObject(p_threadManage->g_hSeqSubmitted, INFINITE);
		DWORD dwMutex = WaitForSingleObject(p_threadManage->g_hMutex, 10);
		switch (dwMutex)
		{
		case WAIT_TIMEOUT:
			break;
		case WAIT_OBJECT_0:
		{
			cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
			p_threadManage->p_middleware_particle_di->copySrcFrom(p_threadManage->vsrc);
			p_threadManage->p_middleware_particle_di->run(rlt_particleDi, 0, imcolor);
			ReleaseMutex(p_threadManage->g_hMutex);
			SetEvent(p_threadManage->g_hRltReturned);
		}
			break;
		default:
			break;
		}
	}
	return 0;
}

