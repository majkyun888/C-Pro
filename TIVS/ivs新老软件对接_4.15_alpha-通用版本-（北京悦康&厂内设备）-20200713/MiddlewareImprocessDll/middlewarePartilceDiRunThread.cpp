#include "stdafx.h"
#include "middlewareParticleDi.h"
#include "../Im-particle-di/procParticleDi.h"
#include "../LogTxt/AppLog.h"
#include "ParaXml.h"
#include "strConvert.h"


typedef algParticleDi* (__stdcall* CREATE_algParticleDi)(aPara* _p_apara, aRlt* _p_arlt);

///////////////////////////////////////////////////////////////////////////////
static bool checkImgValid(cv::Mat im, CString _str_info) {
	if (im.empty() == true) {
		CString cstr;
		cstr.Format(_T("%s is empty"), _str_info);
		PRINT_LOG(cstr);
		return false;
	}
	else
		return true;
}

DWORD WINAPI MiddlewareParticleDi::RunThread(PVOID pvParam) {
	BOOL fShutdown = FALSE;
	MiddlewareParticleDi* p_middleware = (MiddlewareParticleDi*)pvParam;
	/*ParaInter para_inter;*/
	bool bTryReadXml(false);
	PRINT_LOG(_T("RunThread is start"));
	int ii(0);

	while (!fShutdown)
	{
		WaitForSingleObject(p_middleware->g_hevtRequestSubmitted, INFINITE);
		DWORD dwMutex = WaitForSingleObject(p_middleware->g_hMutex, 10);
		switch (dwMutex)
		{
		case WAIT_TIMEOUT:
			PRINT_LOG(_T("g_hMutex timeout in RunThread"));
			break;
		case WAIT_OBJECT_0:
		{
			double tRequestSubmitted = p_middleware->m_tRequestSubmitted;
			MiddlewareImprocessSeq::ParaIn para = p_middleware->para;
			int camIdx = p_middleware->m_camIdx;
			bool ret(false);
			if (p_middleware->vimsrc.size() < 5) {
				PRINT_LOG(_T("vimsrc pic num is less than 5 in  runThrad"));
				ReleaseMutex(p_middleware->g_hMutex);
				continue;
			}
			for (vector<cv::Mat>::iterator it = p_middleware->vimsrc.begin(); it != p_middleware->vimsrc.end(); it++) {
				if (it->empty()) {
					PRINT_LOG(_T("some mat in vimsrc is empty in  mainThrad"));
					ReleaseMutex(p_middleware->g_hMutex);
					continue;
				}
			}
			if (p_middleware->vimsrc.size() != p_middleware->vProcSrc.size())
				p_middleware->vProcSrc.resize(p_middleware->vimsrc.size());

			for (size_t in = 0; in < p_middleware->vimsrc.size(); in++) {
				checkImgValid(p_middleware->vimsrc[in], _T("ms_vimsrc"));
				if (p_middleware->vProcSrc[in].empty())
					p_middleware->vProcSrc[in] = p_middleware->vimsrc[in].clone();
				else
					p_middleware->vimsrc[in].copyTo(p_middleware->vProcSrc[in]);
				
			}
			cv::Mat imcolorRlt;
			MiddlewareImprocessSeq::RltOut rltOut;
			if (p_middleware->p_alg == NULL) {
				p_middleware->p_apara = new MiddlewareImprocessSeq::ParaIn(para);
				p_middleware->p_arlt = new MiddlewareImprocessSeq::RltOut;
				p_middleware->p_alg = new algParticleDi(p_middleware->p_apara, p_middleware->p_arlt);
				
				//// 动态创建动态库
				//CString strDllName = _T("Im-particle-di.dll");
				//HINSTANCE  m_hInstDLL = ::LoadLibrary(strDllName);
				//CREATE_algParticleDi  pfCreate;
				//	if (m_hInstDLL == NULL)
				//	{
				//		AfxMessageBox(_T("Load library: ") + strDllName + _T(" fail!"));
				//	}
				//	else
				//	{
				//		//1.根据地址加载：得到所加载DLL模块中函数的地址
				//		//CREATE_Camera pfCreate = (CREATE_Camera)::GetProcAddress (m_hInstDLL, "?CreateDVPCamera@@YAPAVCCamera@@XZ"); 
				//		//2.根据名字加载:
				//		pfCreate = (CREATE_algParticleDi)::GetProcAddress(m_hInstDLL, "CreateDVPCamera");
				//		if (!pfCreate)
				//		{
				//			AfxMessageBox(_T("Load library: ") + strDllName + _T(",Fail to Get Proc address:") + _T("CreateDVPCamera"));
				//			DWORD dErr = GetLastError();
				//		}
				//		else
				//		{
				//			p_middleware->p_alg =  (*pfCreate)(p_middleware->p_apara, p_middleware->p_arlt);
				//			/*m_Camera->Initialize();*/
				//			AfxMessageBox(_T("success "));
				//		}
				//	}
				// // 动态创建动态库
			}
			else {
				*p_middleware->p_apara = MiddlewareImprocessSeq::ParaIn(para);
			}
				
			p_middleware->m_iProcErr = true;
			p_middleware->m_ret = false;
			p_middleware->m_imcolorRlt = cv::Scalar::all(0);
			__try {
				__try {
					/*if (para_inter.isRead == false) {*/
					if ( ( ((ParaInter*)p_middleware->ppara_inter)->isRead == false  && bTryReadXml == false) && p_middleware->bDetectionOnline == true) {
						CString cstrParaInterXmlPath;
						/*cstrParaInterXmlPath.Format(_T("C:/ParaXml/%d/paraInter.xml"), p_middleware->m_camIdx);*/
						float site_xml = p_middleware->para.sensity * 10;
						site_xml = floor(site_xml);
						site_xml /= 10;
						cstrParaInterXmlPath.Format(_T("C:/ParaXml/%.1f/%02d/paraInter.xml"),site_xml, p_middleware->m_camIdx);
						/*cstrParaInterXmlPath.Format(_T("C:/ParaXml/%d/paraInter.xml"), p_middleware->m_camIdx);*/
						bool iret = readInterParaFromXml(*(ParaInter*)p_middleware->ppara_inter, Unicode2StdString(cstrParaInterXmlPath));
						if (false == iret) {
							((ParaInter*)p_middleware->ppara_inter)->isRead = false;
							PRINT_LOG(_T("readInterParaFromXml try is error	") + cstrParaInterXmlPath );
						}							
						else {
							((ParaInter*)p_middleware->ppara_inter)->isRead = true;
							PRINT_LOG(_T("readInterParaFromXml try is sucess") + cstrParaInterXmlPath );
						}
						bTryReadXml = true;
						/*para_inter.isRead = true;*/
						/*p_middleware->para_inter.isRead = true;*/
					}
					double ts = (double)cv::getTickCount();
					/*cv::setBreakOnError(true);*/
					/*ret = p_middleware->p_alg->proc(p_middleware->vProcSrc, imcolorRlt, para_inter);*/
					ret = p_middleware->p_alg->proc(p_middleware->vProcSrc, imcolorRlt, (ParaInter*)p_middleware->ppara_inter);
					p_middleware->m_iProcErr = false;
				}
				__finally {
					;
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				CString lstr;
				p_middleware->m_iProcErr = true;
				lstr.Format(_T("image process error , program may exit"));
				ret = false;  p_middleware->p_arlt->nimgpt = 2 * p_middleware->p_apara->minnumimg;  p_middleware->p_arlt->totnpt = 2 * p_middleware->p_apara->mintotnpt;
				PRINT_LOG(lstr);
			}

			if (imcolorRlt.empty() == true)
				p_middleware->m_iProcErr = true;
			rltOut = static_cast<MiddlewareImprocessSeq::RltOut>(*p_middleware->p_arlt);
			CString cstr_camID;
			cstr_camID.Format(_T("%02d"), camIdx);

			__try {
				__try {
					if (para.iVimOnlineSave == 1 && ret == true)
						p_middleware->imsOnlineSave(ret, cstr_camID, imcolorRlt);
					else if (para.iVimOnlineSave == 2 && ret == false)
						p_middleware->imsOnlineSave(ret, cstr_camID, imcolorRlt);
					else if (para.iVimOnlineSave == 3)
						p_middleware->imsOnlineSave(ret, cstr_camID, imcolorRlt);
					else if(para.iVimOnlineSave == 0 && true == p_middleware->m_iProcErr)
						p_middleware->imsOnlineSave(ret, cstr_camID, imcolorRlt);
					else
						;
				}
				__finally {
					;
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				CString lstr;
				lstr.Format(_T("image save error, program may exit"));
				PRINT_LOG(lstr);
			}
			int telapse = ((double)cv::getTickCount() - tRequestSubmitted) / cv::getTickFrequency() * 1000;
			int iCheckTimeout(true);
			if (para.iVimRltSave == true) // 如果离线测试，则不保存序列
				iCheckTimeout = false;
			if (telapse < 400 || iCheckTimeout == false) {
				p_middleware->m_ret = ret;
				checkImgValid(imcolorRlt, _T("imcolorRlt"));
				if (!imcolorRlt.empty())
					imcolorRlt.copyTo(p_middleware->m_imcolorRlt);
				if (para.iVimRltSave) {
					p_middleware->vProcRlt.clear();
					for (vector<cv::Mat>::iterator it = p_middleware->p_alg->vimrltOffline.begin(); it != p_middleware->p_alg->vimrltOffline.end(); it++) {
						checkImgValid(*it, _T(" vimrlt"));
						p_middleware->vProcRlt.push_back(it->clone());
					}
				}
				p_middleware->m_rltOut = rltOut;
				SetEvent(p_middleware->g_hevtResultReturned);
			}
			if (p_middleware->m_iProcErr == true) {
				delete p_middleware->p_alg;
				delete p_middleware->p_apara;
				delete p_middleware->p_arlt;
				p_middleware->p_alg = NULL;
				p_middleware->p_apara = NULL;
				p_middleware->p_arlt = NULL;
			}
			//p_middleware->ms_vimsrc.clear();  // 从缓冲区拷贝的序列实用结束后释放，防止下次因为超时而使用上一次的序列	
		}
		ReleaseMutex(p_middleware->g_hMutex);
		break;
		default:
			PRINT_LOG(_T("g_hMutex error in RunThread"));
			break;
		}
	}
	return(0);
}