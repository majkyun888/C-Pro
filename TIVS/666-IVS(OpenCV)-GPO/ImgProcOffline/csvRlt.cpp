#include "StdAfx.h"
#include "csvRlt.h"
#include "Manange.h"


csvRlt::csvRlt()
{
}


csvRlt::~csvRlt()
{
}

void csvRlt::write(int _idx, bool good, CString _path)
{


	HANDLE hFile = ::CreateFile(csvFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CString Line;
		Line.Format(TEXT("%d,%d,%d,"), count++, _idx, int(good) );
		Line.Append(TEXT("\""));
		Line.Append( _path );
		//Line.Append(TEXT("\",\""));
		//Line.Append(st.posStr);
		Line.Append(TEXT("\"\r\n"));
		DWORD dw;
		::SetFilePointer(hFile, 0, NULL, FILE_END);
		std::string strLine = Unicode2StdString(Line);
		;
		::WriteFile(hFile, strLine.c_str(), strLine.length(), &dw, NULL);
		::CloseHandle(hFile);
	}
}

// 有问题，以后再解决
void csvRlt::write(CString _cstrIdx, bool good, CString _path)
{


	HANDLE hFile = ::CreateFile(csvFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CString Line;
		Line.Format(TEXT("%d,"), count++);
		Line.Append(TEXT("\""));
		Line.Append(_cstrIdx);
		Line.Append(TEXT("\",\""));
		Line.Format(TEXT("%d,"), int(good));
		Line.Append(TEXT("\""));
		//Line.Append(st.posStr);
		Line.Append(TEXT("\"\r\n"));
		DWORD dw;
		::SetFilePointer(hFile, 0, NULL, FILE_END);
		std::string strLine = Unicode2StdString(Line);
		;
		::WriteFile(hFile, strLine.c_str(), strLine.length(), &dw, NULL);
		::CloseHandle(hFile);
	}
}

void csvRlt::init(CString _rootName)
{
	time_t t = time(0);
	char Dir[200];
	memset(Dir, '\0', sizeof(Dir));
	strftime(Dir, sizeof(Dir), "\\Rlt%Y_%m_%d_%H_%M_%S", localtime(&t));
	csvFileName = _rootName + StdString2Unicode(Dir) + _T(".csv");
	::WIN32_FIND_DATA fFi;
	HANDLE hF(::FindFirstFile(csvFileName, &fFi));
	if (hF == INVALID_HANDLE_VALUE)
	{
		HANDLE _hF = ::CreateFile(csvFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (_hF != INVALID_HANDLE_VALUE)
		{
			CStringA str;
			str.Format("\"count\",\"index\",\"Result\",\"Path\"\r\n");

			DWORD dw;
			::WriteFile(_hF, str, str.GetLength() * sizeof(CHAR), &dw, NULL);
			::CloseHandle(_hF);
		}
		count = 0;
	}
	else
	{
		::FindClose(hF);
	}
}
