// MiddlewareImprocessDll.h : MiddlewareImprocessDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CMiddlewareImprocessDllApp
// �йش���ʵ�ֵ���Ϣ������� MiddlewareImprocessDll.cpp
//

class CMiddlewareImprocessDllApp : public CWinApp
{
public:
	CMiddlewareImprocessDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
