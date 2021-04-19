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
	ELEMENT ele_ims; // 所在的类，即MiddlewareImProcess对应的实例不能为局部变量，否则会因为该成员变量反复释放和创建，导致系统不必要的开销
	HANDLE hThreadRun;
	HANDLE g_hevtRequestSubmitted, g_hevtResultReturned;
	HANDLE g_hMutex;
	HANDLE g_hRunMutex;  // 防止run函数被主程序在同一时刻多次执行
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
		// 参数注释 20190514
		int minnumimg;
		int mintotnpt;
		int nozero_single_thr;  // “单帧粒子”。若单张图像中像素个数大于或等于nozero_single_thr， 则该帧图像认为有异物
		int nozero_frm_sum_thr; // “图像张数”。推荐2。最小值1， 最大值不超过序列帧数
		float ratio; // “检测比例”，上限1， 下限0.5。背光工位通常设置0.95，底光工位推荐0.8
		float sensity; // “检测灵敏度”，值越小则越灵敏，下限0.5，上限2,推荐值1

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
