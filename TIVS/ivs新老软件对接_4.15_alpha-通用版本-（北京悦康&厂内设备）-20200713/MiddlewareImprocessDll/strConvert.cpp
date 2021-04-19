#include "stdafx.h"
#include "strConvert.h"



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

std::string RootImageSaveFolder()
{
	static std::string strRoot("");
	if ( strRoot.compare(string("") ) == 0) {
		time_t t = time(0);
		char Dir[200];
		memset(Dir, '\0', sizeof(Dir));
		/*strftime(Dir, sizeof(Dir), "C:\\Pic\\%Y_%m_%d_%H_%M_%S", localtime(&t));*/
		strftime(Dir, sizeof(Dir), "D:\\Pic\\%Y_%m_%d_%H_%M_%S", localtime(&t));
		strRoot = Dir;
	}
	return strRoot;
}