// MiddlewareImprocessDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MiddlewareImprocessDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib,"../IVS/Debug/LogTxt.lib")
#else
#pragma comment(lib,"../IVS/Release/LogTxt.lib")
#endif // _DEBUG


//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMiddlewareImprocessDllApp

BEGIN_MESSAGE_MAP(CMiddlewareImprocessDllApp, CWinApp)
END_MESSAGE_MAP()


// CMiddlewareImprocessDllApp ����

CMiddlewareImprocessDllApp::CMiddlewareImprocessDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMiddlewareImprocessDllApp ����

CMiddlewareImprocessDllApp theApp;


// CMiddlewareImprocessDllApp ��ʼ��

BOOL CMiddlewareImprocessDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
