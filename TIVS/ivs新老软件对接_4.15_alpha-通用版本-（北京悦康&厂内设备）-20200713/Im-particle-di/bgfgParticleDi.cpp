#include  "bgfgParticleDi.h"
#include "..//MiddlewareImprocessDll/dataType.h"

namespace algoParticleDi {



	// 以下是单个背景模型的组成，可以根据需要再调整
	template<typename VT> struct MixData
	{
		uchar occur;
		uchar match_id; // which mixture is match 20180525
		uchar dynamic_num; // mixture is changed times 20180525
		bool ischg;
		//float thres_adjust; // 根据动态变化程度来调节阈值
		/*float sortKey;*/
		float weight;
		VT mean;
		VT var;
		VT varM;
	};

	extern  void _buildmodel(BackgroundSubtrator& obj, const cv::Mat& image, const cv::Mat& _mask, bool _en_dy);
	extern void _justifyUp(BackgroundSubtrator& obj, const cv::Mat& image, cv::Mat& fgmask, const cv::Mat& _mask);
	//extern  inline bool _justifyPix(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata);
	extern  inline uchar _justifyPix(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata, float _varCorrect, ParticleType _particle_type,
		int _ix, int _iy);
	extern void _justifyLevel(BackgroundSubtrator& obj, const cv::Mat& image, cv::Mat& fgmask, const cv::Mat& _mask);
	//extern  inline bool _justifyPix(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata);
	extern  inline uchar _justifyPixLevel(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata, float _varCorrect, ParticleType _particle_type, int _ix, int _iy);
	extern void _justifyDynamic(BackgroundSubtrator& obj, const cv::Mat& image, cv::Mat& fgmask, const cv::Mat& _mask);
	extern  inline bool _justifyPixDynamic(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata);



	//BackgroundSubtrator::BackgroundSubtrator(/*float _sensity,*/ ParticleType _particle_type, int _szData)
	BackgroundSubtrator::BackgroundSubtrator(/*ParaBG _para,*/ ParticleType _particle_type, int _szData)
	{

		// s涉及成员变量使用的，应该在此处初始化
		mdata.nmixtures = 4;
		//mdata.occurBg = 2 /*1*/;  
		/*mdata.occurBg = _para.occurBg;*/
		/*para = _para;*/
		//float var0_s = 5, var0_e = 10;
		//float minvar_s = 4, minvar_e = 8;
		//float varthres_s = 1, varthres_e = 1.5;
		//int is = 10, ie = 30;
		particle_type = _particle_type;

		mdata.pdBgmodel = NULL;
		mdata.pdmtDy = NULL;
		mdata.pdmtDyPost = NULL;
		mdata.pdmtId = NULL;
		mdata.pdmtSum = NULL;
		mdata.pdmtTra = NULL;
		mdata.pdmtMax = NULL;

		mdata.szData = _szData;
		if (mdata.pdBgmodel == NULL)
			mdata.pdBgmodel = (uchar*)malloc(_szData * mdata.nmixtures * (5 + 1) * 4);
		if (!mdata.pdBgmodel)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtDy == NULL)
			mdata.pdmtDy = (uchar*)malloc(_szData);
		if (!mdata.pdmtDy)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtDyPost == NULL)
			mdata.pdmtDyPost = (uchar*)malloc(_szData);
		if (!mdata.pdmtDyPost)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtId == NULL)
			mdata.pdmtId = (uchar*)malloc(_szData);
		if (!mdata.pdmtId)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtSum == NULL)
			mdata.pdmtSum = (uchar*)malloc(_szData);
		if (!mdata.pdmtSum)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtTra == NULL)
			mdata.pdmtTra = (uchar*)malloc(_szData);
		if (!mdata.pdmtTra)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		if (mdata.pdmtMax == NULL)
			mdata.pdmtMax = (uchar*)malloc(_szData);
		if (!mdata.pdmtMax)
			CV_Error(0, "mdata.pdBgmodel malloc error");

		//switch (_particle_type)
		//{
		//case algoParticleDi::particle_di:
		//	/*_sensity -= 0.3;*/
		//	 //var0_s = 5*_sensity, var0_e = 10*_sensity;
		//	 //minvar_s =  4*_sensity, minvar_e = 8*_sensity;
		//	 //varthres_s = 1, varthres_e = 1.5*_sensity;
		//	 //is = 10, ie = float(30)/_sensity;
		//	// 20190614  东耀药业
		//	var0_s = 5 * _sensity, var0_e = 8 * _sensity;
		//	minvar_s = 4 * _sensity, minvar_e = 7 * _sensity;
		//	varthres_s = 1, varthres_e = 1.3*_sensity;
		//	is = 10, ie = float(100) / _sensity;
		//	for (int i = 0; i < 256; i++)
		//	{
		//		if (i < is)
		//		{
		//			mdata.var0[i] = var0_s;
		//			mdata.minVar[i] = minvar_s;
		//			mdata.varThrBasic[i] = varthres_s * varthres_s;
		//		}
		//		else if (i > ie)
		//		{
		//			mdata.var0[i] = var0_e;
		//			mdata.minVar[i] = minvar_e;
		//			mdata.varThrBasic[i] = varthres_e * varthres_e;
		//		}
		//		else
		//		{
		//			double var0t = ((double)var0_e - var0_s) / (ie - is) * (i - is) + var0_s; mdata.var0[i] = var0t;
		//			double minvart = ((double)minvar_e - minvar_s) / (ie - is) * (i - is) + minvar_s; mdata.minVar[i] = minvart;
		//			float varthrest = ((float)varthres_e - varthres_s) / (ie - is) * (i - is) + varthres_s; mdata.varThrBasic[i] = varthrest * varthrest;
		//		}
		//	}
		//	for (int i = 0; i < THRES_NUM; i++) {
		//		int tis = 2; int tend = 5;
		//		float vthrad_s = 1.8, vthrad_e = 2.5;
		//		if (i < tis)
		//		{
		//			mdata.varThrDynamic[i] = vthrad_s * vthrad_s;
		//		}
		//		else if (i > tend)
		//		{
		//			mdata.varThrDynamic[i] = vthrad_e * vthrad_e;
		//		}
		//		else
		//		{
		//			float var0t = ((float)vthrad_e - vthrad_s) / (tend - tis) * (i - tis) + vthrad_s;
		//			mdata.varThrDynamic[i] = var0t * var0t;
		//		}
		//	}
		//	break;
		//case algoParticleDi::particle_bei:
		//	//var0_s = 5, var0_e = 10;
		//	//minvar_s = 4, minvar_e = 8;
		//	//varthres_s = 1, varthres_e = 1.5;
		//	//is = 10, ie = 30;
		//	var0_s = 5 * _sensity, var0_e = 5 * _sensity;
		//	minvar_s = 5 * _sensity, minvar_e = 5 * _sensity;
		//	varthres_s = 1, varthres_e = 1;
		//	is = 10, ie = 30;
		//	for (int i = 0; i < 256; i++)
		//	{
		//		if (i < is)
		//		{
		//			mdata.var0[i] = var0_s;
		//			mdata.minVar[i] = minvar_s;
		//			mdata.varThrBasic[i] = varthres_s * varthres_s;
		//		}
		//		else if (i > ie)
		//		{
		//			mdata.var0[i] = var0_e;
		//			mdata.minVar[i] = minvar_e;
		//			mdata.varThrBasic[i] = varthres_e * varthres_e;
		//		}
		//		else
		//		{
		//			double var0t = ((double)var0_e - var0_s) / (ie - is) * (i - is) + var0_s; mdata.var0[i] = var0t;
		//			double minvart = ((double)minvar_e - minvar_s) / (ie - is) * (i - is) + minvar_s; mdata.minVar[i] = minvart;
		//			float varthrest = ((float)varthres_e - varthres_s) / (ie - is) * (i - is) + varthres_s; mdata.varThrBasic[i] = varthrest * varthrest;
		//		}
		//	}
		//	for (int i = 0; i < THRES_NUM; i++) {
		//		int tis = 2; int tend = 5;
		//		float vthrad_s = 1.8, vthrad_e = 2.5;
		//		if (i < tis)
		//		{
		//			mdata.varThrDynamic[i] = vthrad_s * vthrad_s;
		//		}
		//		else if (i > tend)
		//		{
		//			mdata.varThrDynamic[i] = vthrad_e * vthrad_e;
		//		}
		//		else
		//		{
		//			float var0t = ((float)vthrad_e - vthrad_s) / (tend - tis) * (i - tis) + vthrad_s;
		//			mdata.varThrDynamic[i] = var0t * var0t;
		//		}
		//	}
		//	break;
		//default:
		//	break;
		//}

	}

	BackgroundSubtrator::~BackgroundSubtrator()
	{
		//if (udataBgmodel != NULL)
		//	free(udataBgmodel);
		if (mdata.pdBgmodel != NULL)
			free(mdata.pdBgmodel);
		if (mdata.pdmtDy != NULL)
			free(mdata.pdmtDy);
		if (mdata.pdmtDyPost != NULL)
			free(mdata.pdmtDyPost);
		if (mdata.pdmtId != NULL)
			free(mdata.pdmtId);
		if (mdata.pdmtSum != NULL)
			free(mdata.pdmtSum);
		if (mdata.pdmtTra != NULL)
			free(mdata.pdmtTra);
		if (mdata.pdmtMax != NULL)
			free(mdata.pdmtMax);
	}

	void BackgroundSubtrator::buildmodel(cv::InputArray _image, cv::InputArray _mask, bool _en_dy, bool _needToInitialize, ParaBG _para/*, float _sensity*/)
	{
		cv::Mat mask = _mask.getMat();
		cv::Mat image = _image.getMat();
		CV_Assert(mask.size() == image.size() && mask.type() == CV_8UC1 && image.type() == CV_8UC1);
		/*bool needToInitialize = image.size() != frameSize;*/
		if (_needToInitialize)
			initialize(image.size(), image.type(), _para /*_sensity*/);
		_buildmodel(*this, image, mask, _en_dy);
		nTrain++;
	}

	void BackgroundSubtrator::getMaxModel(int _occur,int _thres, cv::Mat& mtMaxModel) {
		//MixData<float>* mptrBase = (MixData<float>*)bgmodel.data;
		//const int KK = mdata.nmixtures;
		//for (int y = 0; y < frameSize.height; y++) {
		//	for (int x = 0; x < frameSize.width; x++, mptrBase += KK) {
		//		int val(0);
		//		for (int k = 0; k < KK; k++) {
		//			MixData<float>* mptr = mptrBase + k;
		//			if (mptr->mean > _thres) {
		//				mdata.mtMax.at<uchar>(y, x) += mptr->occur	;
		//			}						
		//		}
		//	}
		//}
		cv::threshold(mdata.mtMax, mdata.mtMax, _occur - 1, 255, CV_THRESH_BINARY);
		mtMaxModel = cv::Mat::zeros(mdata.mtMax.size(), CV_8UC1);
		mdata.mtMax.copyTo(mtMaxModel);
	}

	void BackgroundSubtrator::justify(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask)
	{
		cv::Mat image = _image.getMat();
		cv::Mat fgmask = _fgmask.getMat();
		cv::Mat mask = _mask.getMat();
		CV_Assert(	mask.size() == image.size() && mask.size() == fgmask.size( )&& 
					mask.type() == CV_8UC1 && image.type() == CV_8UC1 && 
					_fgmask.type() == CV_8UC1 );
		/*_fgmask.create(image.size(), CV_8U);*/
		_justifyUp(*this, image, fgmask, mask);
	}

	void BackgroundSubtrator::justifyLevel(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask)
	{
		cv::Mat image = _image.getMat();
		cv::Mat fgmask = _fgmask.getMat();
		cv::Mat mask = _mask.getMat();
		CV_Assert(mask.size() == image.size() && mask.size() == fgmask.size() &&
			mask.type() == CV_8UC1 && image.type() == CV_8UC1 &&
			_fgmask.type() == CV_8UC1);
		/*_fgmask.create(image.size(), CV_8U);*/
		_justifyLevel(*this, image, fgmask, mask);
	}

	void BackgroundSubtrator::justifyDynamic(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask)
	{
		cv::Mat image = _image.getMat();
		cv::Mat fgmask = _fgmask.getMat();
		cv::Mat mask = _mask.getMat();
		CV_Assert(mask.size() == image.size() && mask.size() == fgmask.size() &&
			mask.type() == CV_8UC1 && image.type() == CV_8UC1 &&
			_fgmask.type() == CV_8UC1);
		/*_fgmask.create(image.size(), CV_8U);*/
		_justifyDynamic(*this, image, fgmask, mask);
	}

	void BackgroundSubtrator::initialize(cv::Size _frameSize, int _frameType , ParaBG _para/*, float _sensity*/)
	{
		frameSize = _frameSize;
		CV_Assert(CV_MAT_DEPTH(_frameType) == CV_8U);
		CV_Assert(1 == CV_MAT_CN(_frameType));
		bgmodel = cv::Mat();
		mdata.mtDy = cv::Mat();
		mdata.mtDyPost = cv::Mat();
		mdata.mtId = cv::Mat();
		mdata.mtSum = cv::Mat();
		mdata.mtTra = cv::Mat();
		mdata.mtMax = cv::Mat();

		if (frameSize.width * frameSize.height > mdata.szData)
			CV_Error(0,"frameSize.width * frameSize.height small than mdata.szData");
			bgmodel = cv::Mat(1, frameSize.height * frameSize.width * mdata.nmixtures * (5 + 1), CV_32F, mdata.pdBgmodel);
		bgmodel = cv::Scalar::all(0);

		/*if (mdata.mtDy.empty())*/
			mdata.mtDy = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtDy);
		mdata.mtDy = cv::Scalar::all(0);

		/*if (mdata.mtDyPost.empty())*/
			mdata.mtDyPost = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtDyPost);

		/*if (mdata.mtId.empty())*/
			mdata.mtId = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtId);
		mdata.mtId = cv::Scalar::all(0);

		/*if (mdata.mtSum.empty())*/
			mdata.mtSum = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtSum);
		mdata.mtSum = cv::Scalar::all(0);

		/*if (mdata.mtTra.empty())*/
			mdata.mtTra = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtTra);
		mdata.mtTra = cv::Scalar::all(0);

		mdata.mtMax = cv::Mat(frameSize.height, frameSize.width, CV_8UC1, mdata.pdmtMax);
		mdata.mtMax = cv::Scalar::all(0);

		para = _para;
		mdata.occurBg = para.occurBg;
		float var0_s =para.var0_s, var0_e = para.var0_e;
		float minvar_s = para.minvar_s, minvar_e = para.minvar_e;
		float varthres_s = para.varthres_s, varthres_e = para.varthres_e;
		int is = para.is, ie = para.ie;
		
		float _sensity = para.sensity;

		switch (particle_type)
		{
		case algoParticleDi::particle_di:

			if (_sensity == 0) {
				_sensity = 1.;
				var0_s = 2.1 * _sensity, var0_e = 6.1 * _sensity;
				minvar_s = 2.1 * _sensity, minvar_e = 5.1 * _sensity;
				varthres_s = 1, varthres_e = 1.2 * _sensity;
				is = 10, ie = float(100) / _sensity;
				mdata.occurBg = 2;
			}
			else {
				var0_s = var0_s * _sensity, var0_e = var0_e * _sensity;
				minvar_s = minvar_s * _sensity, minvar_e = minvar_e * _sensity;
				varthres_s = varthres_s, varthres_e = varthres_e * _sensity;
				is = is, ie = float(ie) / _sensity;
			}

			for (int i = 0; i < 256; i++)
			{
				if (i < is)
				{
					mdata.var0[i] = var0_s;
					mdata.minVar[i] = minvar_s;
					mdata.varThrBasic[i] = varthres_s * varthres_s;
				}
				else if (i > ie)
				{
					mdata.var0[i] = var0_e;
					mdata.minVar[i] = minvar_e;
					mdata.varThrBasic[i] = varthres_e * varthres_e;
				}
				else
				{
					double var0t = ((double)var0_e - var0_s) / (ie - is) * (i - is) + var0_s; mdata.var0[i] = var0t;
					double minvart = ((double)minvar_e - minvar_s) / (ie - is) * (i - is) + minvar_s; mdata.minVar[i] = minvart;
					float varthrest = ((float)varthres_e - varthres_s) / (ie - is) * (i - is) + varthres_s; mdata.varThrBasic[i] = varthrest * varthrest;
				}
			}
			for (int i = 0; i < THRES_NUM; i++) {
				int tis = 2; int tend = 5;
				float vthrad_s = 1.8, vthrad_e = 2.5;
				if (i < tis)
				{
					mdata.varThrDynamic[i] = vthrad_s * vthrad_s;
				}
				else if (i > tend)
				{
					mdata.varThrDynamic[i] = vthrad_e * vthrad_e;
				}
				else
				{
					float var0t = ((float)vthrad_e - vthrad_s) / (tend - tis) * (i - tis) + vthrad_s;
					mdata.varThrDynamic[i] = var0t * var0t;
				}
			}
			break;
		case algoParticleDi::particle_bei:

			if (_sensity == 0) {
				_sensity = 1.;
				var0_s = 5 * _sensity, var0_e = 5 * _sensity;
				minvar_s = 5 * _sensity, minvar_e = 5 * _sensity;
				varthres_s = 1, varthres_e = 1;
				is = 10, ie = 30;
				mdata.occurBg = 2;
			}

			var0_s = var0_s * _sensity, var0_e = var0_e * _sensity;
			minvar_s = minvar_s * _sensity, minvar_e = minvar_e * _sensity;
			varthres_s = varthres_s, varthres_e = varthres_e;
			is = is, ie = ie;

			for (int i = 0; i < 256; i++)
			{
				if (i < is)
				{
					mdata.var0[i] = var0_s;
					mdata.minVar[i] = minvar_s;
					mdata.varThrBasic[i] = varthres_s * varthres_s;
				}
				else if (i > ie)
				{
					mdata.var0[i] = var0_e;
					mdata.minVar[i] = minvar_e;
					mdata.varThrBasic[i] = varthres_e * varthres_e;
				}
				else
				{
					double var0t = ((double)var0_e - var0_s) / (ie - is) * (i - is) + var0_s; mdata.var0[i] = var0t;
					double minvart = ((double)minvar_e - minvar_s) / (ie - is) * (i - is) + minvar_s; mdata.minVar[i] = minvart;
					float varthrest = ((float)varthres_e - varthres_s) / (ie - is) * (i - is) + varthres_s; mdata.varThrBasic[i] = varthrest * varthrest;
				}
			}
			for (int i = 0; i < THRES_NUM; i++) {
				int tis = 2; int tend = 5;
				float vthrad_s = 1.8, vthrad_e = 2.5;
				if (i < tis)
				{
					mdata.varThrDynamic[i] = vthrad_s * vthrad_s;
				}
				else if (i > tend)
				{
					mdata.varThrDynamic[i] = vthrad_e * vthrad_e;
				}
				else
				{
					float var0t = ((float)vthrad_e - vthrad_s) / (tend - tis) * (i - tis) + vthrad_s;
					mdata.varThrDynamic[i] = var0t * var0t;
				}
			}
			break;
		default:
			break;
		}
		
	}



	static void _buildmodel(BackgroundSubtrator & obj, const cv::Mat & image, const cv::Mat & _mask, bool _en_dy)
	{
		int x, y, k, k1, rows = image.rows, cols = image.cols;
		float  vT;
		int K = obj.mdata.nmixtures;
		MixData<float>* mptr = (MixData<float>*)obj.bgmodel.data;
		float var0;
		float minVar;
		for (y = 0; y < rows; y++)
		{
			const uchar* src = image.ptr<uchar>(y);
			const uchar* mask = _mask.ptr<uchar>(y);
			uchar* ptrMx = obj.mdata.mtMax.ptr<uchar>(y);
			uchar* ptrId = obj.mdata.mtId.ptr<uchar>(y);
			uchar* ptrDy = obj.mdata.mtDy.ptr<uchar>(y);
			uchar* ptrSum = obj.mdata.mtSum.ptr<uchar>(y);
			uchar* ptrTra = obj.mdata.mtTra.ptr<uchar>(y);
			for (x = 0; x < cols; x++, mptr += K) {
				if (mask[x] == 255)
				{
					ptrTra[x]++;
					float pix = src[x];
					if (pix > 100)
						ptrMx[x]++;
					int kHit = -1;
					//if (x == 165 && y == 63)
					//	ptrTra = ptrTra;
					for (k = 0; k < K; k++)
					{
						uchar oc = mptr[k].occur;
						if (oc == 0)
							break;
						float mu = mptr[k].mean;
						float var = mptr[k].var;
						float diff = pix - mu;
						float d2 = diff * diff;
						vT = obj.mdata.varThrBasic[(int)mu];
						minVar = obj.mdata.minVar[(int)mu] * obj.mdata.minVar[(int)mu];
						if (d2 < vT * var)
						{
							if (k != ptrId[x])
								ptrDy[x]++;
							float update = 1. / float(++oc);
							mptr[k].mean = mu + update * diff;
							var = max(var + update * (d2 - var), minVar);
							mptr[k].varM = max(var, d2);
							mptr[k].var = var;
							mptr[k].occur = oc;
							for (k1 = k - 1; k1 >= 0; k1--)
							{
								if (mptr[k1].occur >= mptr[k1 + 1].occur)
									break;
								std::swap(mptr[k1], mptr[k1 + 1]);
							}

							kHit = k1 + 1;
							break;
						}
					}
					if (kHit < 0) // no appropriate gaussian mixture found at all, remove the weakest mixture and create a new one
					{
						kHit = k = min(k, K - 1);
						ptrSum[x]++;
						if (0 != k)
							ptrDy[x]++;
						mptr[k].mean = pix;
						mptr[k].var = obj.mdata.var0[(int)pix] * obj.mdata.var0[(int)pix]; 
						mptr[k].occur = 1;
					}
					ptrId[x] = kHit;

				}
			}
				

		}
	}


#define OFFSET_NUM 21
	/*XY*/ cv::Point ptOffset[OFFSET_NUM] = { cv::Point(0,0),// pt index 0
	cv::Point( -1, -1 ),cv::Point( 0,-1 ),cv::Point( 1,-1 ),cv::Point( -1,0 ),cv::Point( 1,0 ),cv::Point( -1,1 ),cv::Point( 0,1 ),cv::Point( 1,1 ), // pt index 1~8
	cv::Point( -2, -2 ),cv::Point( 1,-2 ),cv::Point( 2,-2 ),cv::Point( -2,1 ),cv::Point( 2,1 ),cv::Point( -2,2 ),cv::Point( 1,2 ),cv::Point( 2,2 ), // pt index 9~16
	cv::Point( 0,-2 ) ,cv::Point( 0,2 ) ,cv::Point( 2,0 ) ,cv::Point( -2,0 ) }; // pt index 19~20

	cv::Size BackgroundSubtrator::szBorder = cv::Size(2, 2);

	static void _justifyUp(BackgroundSubtrator & obj, const cv::Mat & image, cv::Mat & fgmask, const cv::Mat & _mask)
	{
		int x, y, rows = image.rows, cols = image.cols;
		int K = obj.mdata.nmixtures;
		MixData<float>* mptrBase = (MixData<float>*)obj.bgmodel.data;
		int bdCol = obj.szBorder.width;
		int bdRow = obj.szBorder.height;
		fgmask = cv::Scalar::all(0);
		obj.mdata.fgCntMax = 100;
		int fgCnt(0); 
		for (y = bdRow; y < rows - bdRow; y++)
		{
			const uchar* src = image.ptr<uchar>(y);
			uchar* dst = fgmask.ptr<uchar>(y);
			float* maskDyPost = obj.mdata.mtDyPost.ptr<float>(y);
			const uchar* mask = _mask.ptr<uchar>(y);
			MixData<float>* mptrScan = mptrBase + K * y * cols + bdCol * K;
			for (x = bdCol; x < cols - bdCol; x++, mptrScan += K)
				if (mask[x] != 0 /*&& x == 73 && y == 145*/)
				{
					float pix = src[x];
					float varCorrect = maskDyPost[x];
					dst[x] = 255;
					for (int i = 0; i < OFFSET_NUM; i++) {
						const MixData<float>* mptrOffset = mptrScan + K * ptOffset[i].y * cols + K * ptOffset[i].x;
						uchar ival = _justifyPix(mptrOffset, pix, obj.mdata, varCorrect, obj.particle_type, x + ptOffset[i].x, y + ptOffset[i].y);
						if (ival == 0) {
							dst[x] = 0;
							break;
						}
						else if( 255 == dst[x] && 128 == ival)
							dst[x] = ival;
					}
					//if ( obj.mdata.particle_type == particle_bei && dst[x] != 0 && ++fgCnt > obj.mdata.fgCntMax)
					//	break; // 前景的像素超过obj.mdata.fgCntMax，则不再检测，避免检测超时

				}
		}
	}

	static inline uchar _justifyPix(const MixData<float> * _mptr, const float _pix, const BackgroundSubtrator::ModelData & _mdata, float _varCorrect, ParticleType _particle_type,
		int _ix, int _iy) {
		int kHit = -1;
		int KK = _mdata.nmixtures;
		uchar comp[10] = { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 };  // 个数必须大于模态个数，否则会异常退出
		uchar mkDynamic = _mdata.mtDy.at<uchar>(_iy, _ix);

		KK = 2;
		for (int k = 0; k < KK; k++) {
			if (_mptr[k].occur == 0)
				break;
			float mu = _mptr[k].mean;
			float var = _mptr[k].var;
			float vthrbs = _mdata.varThrBasic[(int)mu];
			/*float vthrdy = _mdata.varThrDynamic[(int)mu];*/
			//float vthr = vthrbs /** vthrdy*/;
			_varCorrect = std::min(float(4.), _varCorrect);
			/*_varCorrect = 1.;*/
			float vthr = vthrbs * _varCorrect * _varCorrect;
			float diff = _pix - mu;
			float d2 = diff * diff;
			if (d2 < var * vthr) {
			//if (d2 < var * vthr || abs(diff) < 10 ) { //  锦州奥鸿临时20200405
				kHit = k;
				break;
			}
			if (_particle_type == particle_bei && 0 == k) {
				if ( _pix - mu > 0 && d2 > 1.5 * var * vthr) {
					kHit = k;
					break;
				}					
			}
			if (_particle_type == particle_di && _mptr[1].occur > 4 && k == 1) {
				float mx = _mptr[0].mean > _mptr[1].mean ? _mptr[0].mean : _mptr[1].mean;
				float mn = _mptr[0].mean < _mptr[1].mean ? _mptr[0].mean : _mptr[1].mean;
				if (_pix > mn && _pix < mx) {
					kHit = 0;
					break;
				}
			}
			/*if (_varCorrect == 1. &&  d2 > 3 * var * vthr)*/
			if (d2 > 4 * var * _varCorrect * _varCorrect* vthr)
				comp[k] = 255;
		}
		bool iret = kHit < 0 ? false : _mptr[kHit].occur <= _mdata.occurBg ? false : true;
		if (iret == true)
			return 0;
		else {
			if (mkDynamic == 255 &&_mptr[0].occur < 12  ) {
				int mx = std::max(_mptr[0].mean, _mptr[1].mean);
				int mn = std::min(_mptr[0].mean, _mptr[1].mean);
				if (_pix > mn && _pix < mx)
					return 0;
			}
			for (int i = 0; i < KK; i++) {
				if (_mptr[i].occur > _mdata.occurBg && comp[i] == 128)
					return 128;
			}
			return 255;
		}
		/*return iret;*/
	}

	static void _justifyLevel(BackgroundSubtrator& obj, const cv::Mat& image, cv::Mat& fgmask, const cv::Mat& _mask)
	{
		int x, y, rows = image.rows, cols = image.cols;
		int K = obj.mdata.nmixtures;
		MixData<float>* mptrBase = (MixData<float>*)obj.bgmodel.data;
		int bdCol = obj.szBorder.width;
		int bdRow = obj.szBorder.height;
		fgmask = cv::Scalar::all(0);
		for (y = bdRow; y < rows - bdRow; y++)
		{
			const uchar* src = image.ptr<uchar>(y);
			uchar* dst = fgmask.ptr<uchar>(y);
			/*float* maskDyPost = obj.mdata.mtDyPost.ptr<float>(y);*/
			const uchar* mask = _mask.ptr<uchar>(y);
			MixData<float>* mptrScan = mptrBase + K * y * cols + bdCol * K;
			for (x = bdCol; x < cols - bdCol; x++, mptrScan += K)
				if (mask[x] != 0)
				{
					float pix = src[x];
					/*float varCorrect = maskDyPost[x];*/
					float varCorrect = 2.;
					dst[x] = 255;
					for (int i = 0; i < OFFSET_NUM; i++) {
						const MixData<float>* mptrOffset = mptrScan + K * ptOffset[i].y * cols + K * ptOffset[i].x;
						uchar ival = _justifyPixLevel(mptrOffset, pix, obj.mdata, varCorrect, obj.particle_type, x+ ptOffset[i].x, y + ptOffset[i].y);
						if (ival == 0) {
							dst[x] = 0;
							break;
						}
						else if (255 == dst[x] && 128 == ival)
							dst[x] = ival;
					}

				}
		}
	}

	static inline uchar _justifyPixLevel(const MixData<float>* _mptr, const float _pix, const BackgroundSubtrator::ModelData& _mdata, float _varCorrect, ParticleType _particle_type, int _ix, int _iy) {
		int kHit = -1;
		int KK = _mdata.nmixtures;
		uchar comp[10] = { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 };  // 个数必须大于模态个数，否则会异常退出
		uchar valDy = _mdata.mtDy.at<uchar>(_iy,_ix);
		uchar valTra = _mdata.mtTra.at<uchar>(_iy, _ix);
		if (valTra < 3)
			return 0;
		for (int k = 0; k < KK; k++) {
			if (_mptr[k].occur == 0)
				break;
			float mu = _mptr[k].mean;
			float var = _mptr[k].var;
			float vthrbs = _mdata.varThrBasic[(int)mu];
			float vthr = vthrbs * _varCorrect * _varCorrect;
			float diff = _pix - mu;
			float d2 = diff * diff;
			if (d2 < var * vthr) {
				kHit = k;
				break;
			}
			if (_particle_type == particle_bei && 0 == k) {
				if (_pix - mu > 0 && d2 > 1.5 * var * vthr) {
					kHit = k;
					break;
				}
			}
			if (_varCorrect == 1. && d2 > 3 * var * vthr)
				comp[k] = 255;
		}
		bool iret = kHit < 0 ? false : _mptr[kHit].occur <= _mdata.occurBg ? false : true;
		if (iret == true)
			return 0;
		else {
			for (int i = 0; i < KK; i++) {
				if (_mptr[i].occur > _mdata.occurBg && comp[i] == 128)
					return 128;
			}
			return 255;
		}
		/*return iret;*/
	}

	static void _justifyDynamic(BackgroundSubtrator & obj, const cv::Mat & image, cv::Mat & fgmask, const cv::Mat & _mask)
	{
		int x, y, rows = image.rows, cols = image.cols;
		int K = obj.mdata.nmixtures;
		MixData<float>* mptrBase = (MixData<float>*)obj.bgmodel.data;
		int bdCol = obj.szBorder.width;
		int bdRow = obj.szBorder.height;
		fgmask = cv::Scalar::all(0);
		for (y = bdRow; y < rows - bdRow; y++)
		{
			const uchar* src = image.ptr<uchar>(y);
			uchar* dst = fgmask.ptr<uchar>(y);
			const uchar* mask = _mask.ptr<uchar>(y);
			MixData<float>* mptrScan = mptrBase + K * y * cols + bdCol * K;
			for (x = bdCol; x < cols - bdCol; x++, mptrScan += K)
				if (mask[x] != 0)
				{
					float pix = src[x];
					dst[x] = 255;
					for (int i = 0; i <   1/*OFFSET_NUM*/; i++) {
						const MixData<float>* mptrOffset = mptrScan + K * ptOffset[i].y * cols + K * ptOffset[i].x;
						bool ibg = _justifyPixDynamic(mptrOffset, pix, obj.mdata);
						if (ibg == true) {
							dst[x] = 0;
							break;
						}
					}

				}
		}
	}

	static inline bool _justifyPixDynamic(const MixData<float> * _mptr, const float _pix, const BackgroundSubtrator::ModelData & _mdata) {
		int kHit = -1;
		int KK = _mdata.nmixtures;
		for (int k = 0; k < 2 /*KK*/; k++) {
			if (_mptr[k].occur == 0)
				break;
			float mu = _mptr[k].mean;
			float var = _mptr[k].var;
			var = var > 36 ? var : 36 ; // 6*6
			float vthrbs = _mdata.varThrBasic[(int)mu];
			vthrbs = vthrbs > 4 ? vthrbs : 4; // 2*2
			/*float vthrdy = _mdata.varThrDynamic[(int)mu];*/
			float vthr = vthrbs;/** vthrdy*/
			float diff = _pix - mu;
			float d2 = diff * diff;
			if (d2 < var * vthr) {
				kHit = k;
				break;
			}
		}
		bool iret = kHit < 0 ? false : _mptr[kHit].occur <= _mdata.occurBg ? false : true;
		return iret;
	}
}



