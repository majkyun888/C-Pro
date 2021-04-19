//#define _AFXDLL
#include "stdafx.h"
#include "AppLog.h"
#include <process.h>

// CLog3
CAppLog* pAppLog(NULL);

CAppLog::CAppLog(){}

CAppLog::~CAppLog(){}

void CAppLog::Initialize(CString _filename)
{
    m_sFileName = _filename;
}

void CAppLog::CreateLog(CString _filename)
{
	if(_filename.GetLength() <= 0 ) return ;

	this->m_fMuteX.Lock();
	//! close the file if file is existed!
    if (m_File.m_hFile != CFile::hFileNull)
    {
        m_File.Close(); 
    }

	CFileFind _find;
	if (!_find.FindFile(_filename,NULL))
    {
        //!	if no find,the file is not existed!
        BOOL isSuccess = m_File.Open(_filename,CFile::modeCreate|CFile::modeReadWrite| CFile::shareDenyNone);
		if(isSuccess)
		{
			WIN32_FIND_DATA FindFileData;
            FindClose(FindFirstFile(_filename, &FindFileData));
            SYSTEMTIME csCreateTime;
            FileTimeToSystemTime(&FindFileData.ftCreationTime,&csCreateTime);

			CString sCreateTime;
			sCreateTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d[%03dms]"),	csCreateTime.wYear,
																			csCreateTime.wMonth,
																			csCreateTime.wDay,
																			csCreateTime.wHour,
																			csCreateTime.wMinute,
																			csCreateTime.wSecond,
																			csCreateTime.wMilliseconds);
			
			CString infor;
			infor =  CREATEHEADER_INFOR;
			infor += sCreateTime;
			infor += _T("\r\n") ;
			DWORD len = WideCharToMultiByte(CP_OEMCP,NULL,infor.AllocSysString(),-1,NULL,0,NULL,FALSE);
            CHAR _wrInfor[1024];
			BSTR bCngInfor = infor.AllocSysString();
            WideCharToMultiByte(CP_OEMCP,NULL,bCngInfor,-1,_wrInfor,len,NULL,FALSE);
			//! must release the BSTR Object!
			SysFreeString(bCngInfor);
			
			m_File.Write(_wrInfor, strlen(_wrInfor));
		}
    }
    else
    {
		//! find the log file, just open the file.
        m_File.Open(_filename, CFile::modeReadWrite | CFile::shareDenyNone);
    }

	this->m_fMuteX.Unlock();
}


void CAppLog::StartLog(void )
{
	CreateLog(m_sFileName);

	m_hStopHandle	= CreateEvent(NULL,TRUE,FALSE,NULL);
    m_hSignalHandle = CreateEvent(NULL,TRUE,FALSE,NULL);

    m_hThreadHandle = HANDLE(_beginthread(CAppLog::LogProcThread, 0, this));
	::SetThreadPriority(m_hThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);
}


void CAppLog::StopLog(void )
{
	//! set the stop event
	SetEvent(this->m_hStopHandle);
	//! must Wait for the log thread to exit.
	WaitForSingleObject(m_hThreadHandle, INFINITE);
}

void CAppLog::LogProcThread(void *_data)
{
	CAppLog *pLog = (CAppLog *)_data;
    HANDLE  hHandleArray[2];

    hHandleArray[0] = pLog->m_hStopHandle;
    hHandleArray[1] = pLog->m_hSignalHandle;

	BOOL    _bStop = FALSE;
    while (!_bStop)
    {
        switch (WaitForMultipleObjects(2, hHandleArray, FALSE, INFINITE))
        {
		//! if it is stop event
        case WAIT_OBJECT_0:
            _bStop = TRUE;
            break;
		//! if it is signal event
        case WAIT_OBJECT_0+1:
            //! Lock the queue to get the count and the next entry
            pLog->m_fMuteX.Lock();
            while (pLog->m_MsgQueue.GetCount())
            {
				//! get msg from tail of queue, then remove it.
                CString _MsgInfor = pLog->m_MsgQueue.RemoveTail();
                SYSTEMTIME tm;
                GetLocalTime(&tm);
                CString sRecord;
				sRecord.Format(_T("#%04d-%02d-%02d %02d:%02d:%02d[%03dms]: %s"),tm.wYear,
																				tm.wMonth,
																				tm.wDay,
																				tm.wHour,
																				tm.wMinute,
																				tm.wSecond,
																				tm.wMilliseconds,
																				_MsgInfor);
                sRecord += _T("\r\n");
                //! seek to the end of the log file.
                pLog->m_File.SeekToEnd();
                
                DWORD len = WideCharToMultiByte(CP_OEMCP,NULL,sRecord.AllocSysString(),-1,NULL,0,NULL,FALSE);
                CHAR _wrInfor[1024];
                WideCharToMultiByte(CP_OEMCP,NULL,sRecord.AllocSysString(),-1,_wrInfor,len,NULL,FALSE);
                pLog->m_File.Write(_wrInfor,strlen(_wrInfor));
            }
            //! output to file on disk
            pLog->m_File.Flush();
            ResetEvent(pLog->m_hSignalHandle);
			//! unlock
            pLog->m_fMuteX.Unlock();
            break;
        }
    }

    pLog->m_File.Close();
	//! end the message processor thread
	_endthread();
}

void CAppLog::Write2Log(CString _wrInfor)
{
	if(_wrInfor.GetLength() <= 0 ) return ;
	//! changle the state
	//  AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //! lock the object for the addhead call
    m_fMuteX.Lock();
    m_MsgQueue.AddHead(_wrInfor);
    m_fMuteX.Unlock();
    //! new message to write
    SetEvent(m_hSignalHandle);
}