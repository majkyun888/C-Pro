 
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
 
using namespace std;
 
//************************************
       
// 
int ReadReg(LPCWSTR path, LPCWSTR key, char* value)
{
	HKEY hKey;
	int ret = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		//cout << "failed to open regtable" << endl;
		//Message_box("Failed to open Regtable", MBOX_INFO);
		return 1;
	}
 
	//读取KEY
	DWORD dwType = REG_SZ; //change the type with the context,this is for string
	DWORD cbData = 256;
	ret = RegQueryValueEx(hKey, key, NULL, &dwType, (LPBYTE)value, &cbData);
	if (ret == ERROR_SUCCESS)
	{
		//cout << value << endl;
	}
	else
	{
		cout <<"failed to read the regtable" << endl;
		RegCloseKey(hKey);
		return 1;
	}
	RegCloseKey(hKey);
 
	return 0;
}
 
//************************************

int WriteReg(LPCWSTR path, LPCWSTR key, DWORD value)
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType = REG_DWORD; //type for long unsigned int
    
	
	
	int	ret = RegCreateKeyEx(HKEY_CURRENT_USER, path,0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
		if (ret != ERROR_SUCCESS)
		{
			//cout << "failed to create regtable key" << endl;
			return 1;
		}
	
	ret = RegSetValueEx(hKey, key, 0, dwType, (BYTE*)&value, 4 );
	if (ret != ERROR_SUCCESS)
	{
		//cout << "failed to set the key value" << endl;
		RegCloseKey(hKey);
		return 1;
	}
	RegCloseKey(hKey);
	return 0;
}
 
//************************************

int DelReg(LPCWSTR path)
{
	int ret = RegDeleteKey(HKEY_CURRENT_USER, path);
	if (ret == ERROR_SUCCESS)
	{
		cout << "succeed to delete regtable key" << endl;
	}
	else
	{
		cout << "failed to delete regtable key" << endl;
		return 1;
	}
	return 0;
}
 

 
