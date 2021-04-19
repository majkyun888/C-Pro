#pragma once
#include "middlewareCommon.h"





#define ErrProc(xx) {\
		m_ret = false;\
		_rlt.nimgpt = 2 * para.minnumimg;\
		_rlt.totnpt = 2 * para.mintotnpt;\
		m_imcolorRlt = cv::Scalar::all(0);\
		cv::putText(m_imcolorRlt, string(xx), cv::Point(0, m_imcolorRlt.rows / 2), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 1, 8);\
;\
}

class algParticleDi;
 
//---------------------------------MiddlewareParticleDi start --------------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareParticleDi : public MiddlewareImprocessSeq
{
public:

	MiddlewareParticleDi(ParaIn _para) ;
	~MiddlewareParticleDi();
	bool run(RltOut& _rlt, int _camIdx/*, CString _cstr_camID*/, cv::Mat& _imcolor_rlt);
	bool ParaFromXml(string _xml_path) ;
	bool ParaToXml(string _xml_path);

public:
	ParaIn para;
	/*ParaInter para_inter;*/
	PVOID ppara_inter;
protected:
	algParticleDi* p_alg;
	ParaIn* p_apara;
	RltOut* p_arlt;
	static DWORD WINAPI RunThread(PVOID pvParam);
};

//---------------------------------MiddlewareParticleDi end --------------------------