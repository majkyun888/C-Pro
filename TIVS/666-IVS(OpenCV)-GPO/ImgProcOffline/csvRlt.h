#pragma once
#include "stdafx.h"
#include <string>
using namespace std;
class csvRlt
{
public:
	csvRlt();
	~csvRlt();
	void write(int _idx, bool good, CString _path);
	void write(CString _cstrIdx, bool good, CString _path);
	void init(CString _rootName);
private:	
	CString csvFileName;
	int count;
};

