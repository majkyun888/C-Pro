#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "stdafx.h"
//#include "MiddlewareImprocessDll.h"



using namespace std;
#define Q_FRAME (30+1) // 图像序列最多30张，另外加一张结果图
#define TLEN 1024
#define PARTICLE_IMAGE_WIDTH 400 
#define PARTICLE_IMAGE_HEIGHT 768

class /*MIDDLEWARE_IMPROCESS_API*/ ELEMENT {
public:
	ELEMENT();
	~ELEMENT();
	bool copyTo(ELEMENT& ele);
	bool cloneFrom(string cstr_savePath, vector < cv:: Mat > vec_im);

public:
	int	numFrm; // 改组图像序列的张数，不得大于Q_FRAME;
	char savePath[TLEN];
	uchar* imdata/*[Q_FRAME][PARTICLE_IMAGE_WIDTH][PARTICLE_IMAGE_HEIGHT]*/;
	uchar* ptr_im;
	cv::Mat ims[Q_FRAME];
	bool b_save;
	/*int m_nThreadNum, m_nRequestNum;*/
	// Other element data should go here
};
typedef ELEMENT* PELEMENT;


class /*MIDDLEWARE_IMPROCESS_API*/ CQueue {
public:


private:
	PELEMENT m_pElements;        // Array of elements to be processed
	int      m_nMaxElements;     // Maximum # of elements in the array
	HANDLE   m_h[2];             // Mutex & semaphore handles
	HANDLE& m_hmtxQ;           // Reference to m_h[0]
	HANDLE& m_hsemNumElements; // Reference to m_h[1]

public:
	CQueue(int nMaxElements);
	~CQueue();

	BOOL Append(PELEMENT pElement, DWORD dwMilliseconds);
	BOOL Remove(PELEMENT pElement, DWORD dwMilliseconds);
	void StartSave(void);
	static void LogProcThread(void* _data);
	LONG g_fShutdown;
};

extern CQueue* p_imsQueue;