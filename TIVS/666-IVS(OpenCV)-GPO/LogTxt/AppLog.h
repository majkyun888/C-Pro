#pragma once

#ifdef LOGTXT_EXPORTS
#define LOGTXT_API __declspec(dllexport)
#else
#define LOGTXT_API __declspec(dllimport)
#endif


#include <afxmt.h>
// CLog3 命令目标
#define		CREATEHEADER_INFOR		_T(	"// Create On: ")



class LOGTXT_API CAppLog
{
public:
    CAppLog();
    ~CAppLog();

protected:
	void CreateLog(CString _csFileName);
	//! must be static function
	static void LogProcThread(void *_data);

public:
	CMutex			m_fMuteX;
	CFile			m_File;

    CString         m_sFileName;

    HANDLE          m_hStopHandle;
	HANDLE			m_hSignalHandle;
	HANDLE			m_hThreadHandle;

	//! simulate the message queue
    CStringList		m_MsgQueue;

	void Initialize(CString _filename);
    void StartLog();
    void StopLog();
	void Write2Log(CString _wrInfor);
};

extern LOGTXT_API CAppLog* pAppLog;

#define	DEF_LOG_PATH				_T("./log11.txt")

//#define PRINT_LOG(INFOR)			pAppLog->Write2Log(INFOR);
#define CLOSE_LOG()					pAppLog->StopLog();


//#define PRINT_LOG(INFOR)			{if(NULL == pAppLog) { pAppLog = new CAppLog; pAppLog->Initialize(DEF_LOG_PATH);pAppLog->StartLog(); }; pAppLog->Write2Log(INFOR);}

#define PRINT_LOG(INFOR)			{	if (NULL == pAppLog)									\
{																								\
	pAppLog = new CAppLog;																		\
	pAppLog->Initialize(DEF_LOG_PATH);															\
	pAppLog->StartLog();																		\
	pAppLog->Write2Log(_T("----------------------------"));										\
	pAppLog->Write2Log(_T("----------------------------"));										\
	CString cstrTime;																			\
	cstrTime.Format(_T("本软件启动后第一次使用OpenCV检测"));		\
	pAppLog->Write2Log(cstrTime);																\
};																								\
pAppLog->Write2Log(INFOR);}

//#define CLOSE_LOG()					pAppLog->StopLog();

//#ifdef IVS_PC1
//#define PRINT_LOG(INFOR)			(((CIVSApp *)AfxGetApp())->GetAppLog()->Write2Log(INFOR))
//#define CLOSE_LOG()					(((CIVSApp *)AfxGetApp())->GetAppLog()->StopLog())
//#endif // IVS_PC1
//
//#ifdef IVS_PC2
//#define PRINT_LOG(INFOR)			(((CDENGJIAN_mfcApp *)AfxGetApp())->GetAppLog()->Write2Log(INFOR))
//#define CLOSE_LOG()					(((CDENGJIAN_mfcApp *)AfxGetApp())->GetAppLog()->StopLog())
//#endif // IVS_PC2