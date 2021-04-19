#pragma once
#include <opencv2/opencv.hpp>
//#include "dataType.h"
#include "CQueue.h"

#ifdef MIDDLEWARE_IMPROCESS_DLL_EXPORT
#define MIDDLEWARE_IMPROCESS_API __declspec(dllexport)
#else
#define MIDDLEWARE_IMPROCESS_API __declspec(dllimport)
#endif

#define MsgDebug

#ifdef MsgDebug
#define MsgBox(msgInfo)  ::MessageBox(NULL, msgInfo, NULL, MB_OK);
#endif // MsgDebug


using namespace std;

// --------------------------------- MiddlewareImProcess start -----------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImProcess
{
public:
	MiddlewareImProcess();
	~MiddlewareImProcess();
	void cvMat2Ptr(unsigned int* _ptrDst, int _dimxDst, int _dimyDst, cv::Mat _imsrc);
	void copySrcFrom(vector<cv::Mat> _srcs);
	void copyRltTo( vector<cv::Mat>& _vimrlt);
public:
	vector<cv::Mat> vimsrc , vimrlt;
	ELEMENT ele_ims; // ���ڵ��࣬��MiddlewareImProcess��Ӧ��ʵ������Ϊ�ֲ��������������Ϊ�ó�Ա���������ͷźʹ���������ϵͳ����Ҫ�Ŀ���
	HANDLE hThreadRun;
	HANDLE g_hevtRequestSubmitted, g_hevtResultReturned;
	HANDLE g_hMutex;
	HANDLE g_hRunMutex;  // ��ֹrun��������������ͬһʱ�̶��ִ��
	void imsOnlineSave(bool good, CString _cstr_CamID,cv::Mat imRlt);
	bool bDetectionOnline;
protected:
	int m_camIdx;
	cv::Mat m_imcolorRlt;
	bool m_ret;
	//bool checkImgValid(cv::Mat im, CString _str_info);
public:
	virtual bool ParaFromXml(string _xml_path) = 0;
	virtual bool ParaToXml(string _xml_nmae) = 0;
	/*int ms_camIdx;*/
	/*cv::Mat ms_imcolorRlt;*/
	/*bool ms_ret;*/
	vector<cv::Mat> vProcSrc, vProcRlt;
	/*vector<cv::Mat> ms_vimsrc, ms_vimrlt;*/
	double m_tRequestSubmitted;
	bool m_iProcErr;

};


// --------------------------------- MiddlewareImProcess end --------------------------


// --------------------------------- MiddlewareImProcessSingle start ------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImProcessSingle : public MiddlewareImProcess
{
public:
	MiddlewareImProcessSingle();
	~MiddlewareImProcessSingle();

private:

};


// --------------------------------- MiddlewareImProcessSingle end --------------------



// --------------------------------- MiddlewareImprocessSeq start ---------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImprocessSeq: public MiddlewareImProcess
{
public:
	MiddlewareImprocessSeq();
	~MiddlewareImprocessSeq();
	struct ParaIn
	{
		// ����ע�� 20190514
		int minnumimg;
		int mintotnpt;
		int nozero_single_thr;  // ����֡���ӡ���������ͼ�������ظ������ڻ����nozero_single_thr�� ���֡ͼ����Ϊ������
		int nozero_frm_sum_thr; // ��ͼ�����������Ƽ�2����Сֵ1�� ���ֵ����������֡��
		float ratio; // ����������������1�� ����0.5�����⹤λͨ������0.95���׹⹤λ�Ƽ�0.8
		float sensity; // ����������ȡ���ֵԽС��Խ����������0.5������2,�Ƽ�ֵ1

		bool iVimRltSave;
		int iVimOnlineSave;  //  0 disable ; 1 good save; 2 bad save; 3 all save
		cv::Rect roi;
		ParaIn() {
			nozero_single_thr = 2;
			nozero_frm_sum_thr = 2;
			ratio = 0.8;
			sensity = 1;
			iVimRltSave = false;
			iVimOnlineSave = 0;
			roi = cv::Rect();
		}
		void write(cv::FileStorage& fs) const;//Write serialization for this class

		void read(const cv::FileNode& node); //Read serialization for this class

	};
	struct RltOut
	{
		int nimgpt;
		int totnpt;
	};
	RltOut m_rltOut;
	/*RltOut ms_rltOut;*/
	void ptrs2cvMats(uchar* _images[], int _dimx, int _dimy, int _seq);
	virtual bool ParaFromXml(string _xml_path) =0   ;
	virtual bool ParaToXml(string _xml_nmae) =0;

//public:
//	ParaIn ms_para;

};



// ---------------------------------MiddlewareImprocessSeq end --------------------------
