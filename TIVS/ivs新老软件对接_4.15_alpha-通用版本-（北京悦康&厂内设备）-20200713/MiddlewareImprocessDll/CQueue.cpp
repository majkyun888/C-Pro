#include "stdafx.h"
#include "CQueue.h"
#include "strConvert.h"
#include "../LogTxt/AppLog.h"
#include <imagehlp.h>  
#pragma comment(lib,"imagehlp.lib") 

CQueue* p_imsQueue(NULL)/* = new CQueue(20)*/;

ELEMENT::ELEMENT() {
	imdata = (uchar*)HeapAlloc(GetProcessHeap(), 0, sizeof(uchar) * Q_FRAME * PARTICLE_IMAGE_WIDTH * PARTICLE_IMAGE_HEIGHT);
	ptr_im = (uchar*)imdata;
	uchar* ptr_inc = ptr_im;
	for (int n_frm = 0; n_frm < Q_FRAME; n_frm++) {
		ims[n_frm] = cv::Mat(PARTICLE_IMAGE_WIDTH, PARTICLE_IMAGE_HEIGHT, CV_8UC1, ptr_inc);
		ptr_inc += PARTICLE_IMAGE_HEIGHT * PARTICLE_IMAGE_WIDTH;
	}
	b_save = false;

}

ELEMENT::~ELEMENT() {
	for (int n_frm = 0; n_frm < Q_FRAME; n_frm++) {
		ims[n_frm].release();
	}
	HeapFree(GetProcessHeap(), 0, imdata);  //是否要求imdata类型的长度是一致的，应该也不需要
}

bool ELEMENT::copyTo(ELEMENT& ele) {
	MoveMemory(ele.savePath, this->savePath, TLEN);
	MoveMemory(ele.ptr_im, this->ptr_im, sizeof(char) * Q_FRAME * PARTICLE_IMAGE_WIDTH * PARTICLE_IMAGE_HEIGHT);
	ele.numFrm = numFrm;
	ele.b_save = true;
	return true;
}

bool ELEMENT::cloneFrom(string cstr_savePath, vector<cv::Mat> vec_im) {
	if (cstr_savePath.length() > TLEN) {
		PRINT_LOG(_T("online save error :cstr_savePath.length"));
		return false;
	}
	if (vec_im.size() > /*<*/ Q_FRAME) {
		PRINT_LOG(_T("online save error :vec_im.size"));
		return false;
	}
		
	MoveMemory(savePath, cstr_savePath.c_str(), cstr_savePath.length());
	savePath[cstr_savePath.length()] = '\0';
	numFrm = vec_im.size();
	b_save = true;
	uchar* ptr_inc = ptr_im;
	for (int n_frm = 0; n_frm < numFrm/*Q_FRAME*/; n_frm++) {
		if (vec_im.at(n_frm).rows * vec_im.at(n_frm).cols != PARTICLE_IMAGE_WIDTH * PARTICLE_IMAGE_HEIGHT) {
			PRINT_LOG(_T("online save error :vec_im.at"));
			return false;
		}
		MoveMemory(ptr_inc, vec_im.at(n_frm).data, sizeof(char) * PARTICLE_IMAGE_WIDTH * PARTICLE_IMAGE_HEIGHT);
		ptr_inc += PARTICLE_IMAGE_WIDTH * PARTICLE_IMAGE_HEIGHT;
	}
	return true;
}

CQueue::CQueue(int nMaxElements)
	: m_hmtxQ(m_h[0]), m_hsemNumElements(m_h[1]) {

	m_pElements = new ELEMENT[nMaxElements];
	m_nMaxElements = nMaxElements;
	m_hmtxQ = CreateMutex(NULL, FALSE, NULL);
	m_hsemNumElements = CreateSemaphore(NULL, 0, nMaxElements, NULL);
	g_fShutdown = true;
	StartSave();
}

///////////////////////////////////////////////////////////////////////////////


CQueue::~CQueue() {

	g_fShutdown = false;
	Sleep(500);
	CloseHandle(m_hsemNumElements);
	CloseHandle(m_hmtxQ);
	delete[] m_pElements;
}
///////////////////////////////////////////////////////////////////////////////


BOOL CQueue::Append(PELEMENT pElement, DWORD dwTimeout) {

	BOOL fOk = FALSE;
	DWORD dw = WaitForSingleObject(m_hmtxQ, dwTimeout);

	if (dw == WAIT_OBJECT_0) {
		// This thread has exclusive access to the queue

		// Increment the number of elements in the queue
		LONG lPrevCount;
		fOk = ReleaseSemaphore(m_hsemNumElements, 1, &lPrevCount);
		if (fOk) {
			// The queue is not full, append the new element
			for (int i = 0; i < m_nMaxElements; i++)
				if (false == m_pElements[i].b_save) {
					pElement->copyTo(m_pElements[i]);
					break;
				}

		}
		else {

			// The queue is full, set the error code and return failure
			SetLastError(ERROR_DATABASE_FULL);
		}

		// Allow other threads to access the queue
		ReleaseMutex(m_hmtxQ);

	}
	else {
		// Timeout, set error code and return failure
		SetLastError(ERROR_TIMEOUT);
	}

	return(fOk);    // Call GetLastError for more info
}

///////////////////////////////////////////////////////////////////////////////
BOOL CQueue::Remove(PELEMENT pElement, DWORD dwTimeout) {

	// Wait for exclusive access to queue and for queue to have element.
	//BOOL fOk = (WaitForMultipleObjects(_countof(m_h), m_h, TRUE, dwTimeout) ==  WAIT_OBJECT_0);

	BOOL fOk = FALSE;
	DWORD dwMtx = WaitForSingleObject(m_hmtxQ, dwTimeout);
	if (dwMtx == WAIT_OBJECT_0) {
		DWORD dwSem = WaitForSingleObject(m_hsemNumElements, 20);
		if (dwSem == WAIT_OBJECT_0) {
			// The queue has an element, pull it from the queue
			for (int i = 0; i < m_nMaxElements; i++) {
				if (true == m_pElements[i].b_save) {
					m_pElements[i].copyTo(*pElement);
					m_pElements[i].b_save = false;
					fOk = true;
					break;
				}
			}
		}
		ReleaseMutex(m_hmtxQ);
	}


	return(fOk);   // Call GetLastError for more info
}

void CQueue::StartSave(void)
{
	HANDLE m_hThreadHandle = HANDLE(_beginthread(CQueue::LogProcThread, 0, this));
	::SetThreadPriority(m_hThreadHandle, THREAD_PRIORITY_LOWEST);
}
//volatile LONG g_fShutdown = true;
void CQueue::LogProcThread(void* _data)
{
	CQueue* pQueue = (CQueue*)_data;
	ELEMENT element;
	while (pQueue->g_fShutdown)
		if (pQueue->Remove(&element, INFINITE)) {
			if (MakeSureDirectoryPathExists(element.savePath)) {
				CString cstr; cstr.Format(_T("保存路径%s，粒子张数%d"),StdString2Unicode(element.savePath), element.numFrm);
				PRINT_LOG(cstr);
				for (int n_frm = 0; n_frm < element.numFrm/*Q_FRAME*/; n_frm++) {
					// 图像路径
					char imName[1024]; memset(imName, '\0', sizeof(imName));
					sprintf(imName, "%s%03d.png", element.savePath, n_frm);
					if (element.ims[n_frm].empty()) {
						PRINT_LOG(_T("one line save: element.ims[n_frm].empty"));
						Sleep(10);
						continue;
					}
					bool isOK = cv::imwrite(imName, element.ims[n_frm]);
					if (isOK == false) {
						CString cstr;
						if (element.ims[n_frm].empty())
							cstr.Format(_T("图像连线保存失败%s，图像为空"), StdString2Unicode(imName));
						else
							cstr.Format(_T("图像连线保存失败%s，路径无效"), StdString2Unicode(imName));
						PRINT_LOG(cstr);
					}
					//Sleep(200);
				}
			}
		}
}

