
// ImgProcOffline.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImgProcOfflineApp:
// �йش����ʵ�֣������ ImgProcOffline.cpp
//

class CImgProcOfflineApp : public CWinApp
{
public:
	CImgProcOfflineApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImgProcOfflineApp theApp;