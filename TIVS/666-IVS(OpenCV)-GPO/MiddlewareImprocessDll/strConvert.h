#pragma once
#include <string>
using namespace std;

extern std::string RootImageSaveFolder();
extern std::string Unicode2StdString(CString& cstr);
extern CString StdString2Unicode(std::string str);