#pragma once
#include <opencv2/opencv.hpp>
//typedef unsigned char       BYTE;

#ifdef MIDDLEWARE_IMPROCESS_EXPORT
#define MIDDLEWARE_IMPROCESS_API __declspec(dllexport)
#else
#define MIDDLEWARE_IMPROCESS_API __declspec(dllimport)
#endif


using namespace std;

// --------------------------------- MiddlewareImProcess start -----------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImProcess
{
public:
	MiddlewareImProcess();
	~MiddlewareImProcess();
	void cvMat2Ptr(unsigned int* _ptrDst, int _dimxDst, int _dimyDst, cv::Mat _imsrc){

		cv::Mat im;
		cv::resize(_imsrc, im, cv::Size(_dimxDst, _dimyDst));
		if (im.channels() == 1)
			cv::cvtColor(im, im, CV_GRAY2BGRA);
		else
			cv::cvtColor(im, im, CV_BGR2BGRA);
		CV_Assert(im.isContinuous());
		memcpy(_ptrDst, im.data, im.cols * im.rows * im.channels());
	}
private:

};

MiddlewareImProcess::MiddlewareImProcess()
{
}

MiddlewareImProcess::~MiddlewareImProcess()
{
}
// --------------------------------- MiddlewareImProcess end --------------------------


// --------------------------------- MiddlewareImProcessSingle start ------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImProcessSingle : public MiddlewareImProcess
{
public:
	MiddlewareImProcessSingle();
	~MiddlewareImProcessSingle();

private:

};

MiddlewareImProcessSingle::MiddlewareImProcessSingle()
{
}

MiddlewareImProcessSingle::~MiddlewareImProcessSingle()
{
}
// --------------------------------- MiddlewareImProcessSingle end --------------------


// --------------------------------- MiddlewareImprocessSeq start ---------------------
class MIDDLEWARE_IMPROCESS_API MiddlewareImprocessSeq: public MiddlewareImProcess
{
public:
	MiddlewareImprocessSeq();
	~MiddlewareImprocessSeq();
	void ptrs2cvMats(uchar* _images[], int _dimx, int _dimy, int _seq) {
		// 需要判断取值是否有效，利用try尝试
		for (int i = 0; i < _seq; i++) {
			cv::Mat im = cv::Mat::zeros(_dimy, _dimx, CV_MAKE_TYPE(CV_8U, 1));
			if (im.isContinuous()) {
				memcpy(im.data, _images[i], _dimy * _dimx);
				vec_ims.push_back(im);
			}
			else
				; // 报错
		}
	}
protected:
	vector<cv::Mat> vec_ims;
};

MiddlewareImprocessSeq::MiddlewareImprocessSeq()
{
}

MiddlewareImprocessSeq::~MiddlewareImprocessSeq()
{
}
// ---------------------------------MiddlewareImprocessSeq end --------------------------

//
class MIDDLEWARE_IMPROCESS_API MiddlewareImproessSeqParticleDi: public MiddlewareImprocessSeq
{
public:
	struct ParaIn
	{
		int minnumimg;
		int mintotnpt;
	};
	struct RltOut
	{
		int nimgpt;
		int totnpt;
	};
	MiddlewareImproessSeqParticleDi(ParaIn _para) :para(_para) {};
	~MiddlewareImproessSeqParticleDi();
	bool run(RltOut& _rlt, cv::Mat& _imcolor_rlt);
private:
	ParaIn para;
};



MiddlewareImproessSeqParticleDi::~MiddlewareImproessSeqParticleDi()
{
}


