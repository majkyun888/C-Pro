#pragma once
#include <opencv2/core/core.hpp>
#include "../Im-particle-di/procParticleDi.h"
using namespace std;
#include "stdafx.h"
#include "csvRlt.h"

extern std::string Unicode2StdString(CString& cstr);
extern CString StdString2Unicode(std::string str);

class ThreadMange
{
public:
	ThreadMange(int _ID);
	~ThreadMange();
	void cloneSrcsFrom(vector<cv::Mat> _srcs, /*algParticleDi::*/aPara _para);
	HANDLE g_hSeqSubmitted, g_hRltReturned;
private:
	int ID;
	
	HANDLE g_hMutex;
	MiddlewareParticleDi* p_middleware_particle_di;
	vector<cv::Mat> vsrc;
	static DWORD WINAPI runThread(PVOID pvParam);
};

static const int numThread = 0;

class Manange
{
public:
	Manange();
	~Manange();
	void collectImgPaths(CString _cstrRoot, CString _cstrRootCsv);
	
	bool procOnce(aPara _para,ParaInter _para_inter, bool _toNext, int _camIdx, bool& _rlt);
	vector<cv::Mat> imrlts;
	CString imSampleFullPath;
	//int idx;
	//bool iret;
private:
	MiddlewareParticleDi* p_middleware_particle_di;
	csvRlt csv_rlt;
	vector<cv::Mat> readImgFromPath(CString _imgpath);
	void ims2write(CString _path, vector<cv::Mat> _srcs, vector<cv::Mat>& _rlts, bool _good, string _str_index/*, int _idx*/);
	vector<CString> vecPath;
	CString strExt;
	vector<CString>::iterator itCurPath;
	vector<CString> GetFilePaths(CString rootPath);
	CString scan_img_dir(CString imgPath, CString& imSampleFullPath);
	/*vector<ThreadMange> procThreads;*/
	ThreadMange* procThreads[numThread+1];
	/*int cur_index;*/
};



