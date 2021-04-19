#pragma once
#include <opencv2/opencv.hpp>	
#include "../MiddlewareImprocessDll/dataType.h"
#define THRES_NUM 30
using namespace std;

namespace algoParticleDi {

	//enum ParticleType
	//{
	//	particle_di, particle_bei
	//};



	class BackgroundSubtrator
	{
	public:
		//BackgroundSubtrator(/*float _sensity,*/ ParticleType _particle_type, int _szData);
		BackgroundSubtrator(/*ParaBG _para,*/ ParticleType _particle_type, int _szData);
		~BackgroundSubtrator();
		void buildmodel(cv::InputArray _image, cv::InputArray _mask, bool _en_dy,bool _needToInitialize, ParaBG _para/*, float _sensity*/); // 20180525 监控背景的动态程度
		void justify(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);
		void justifyLevel(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);
		void justifyDynamic(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);	
		void getMaxModel(int _occur,int _thres, cv::Mat& mtMaxModel);

		ParticleType particle_type;
		cv::Mat bgmodel;
		struct ModelData
		{
			ParticleType particle_type;// 回传粒子的类型。
			int fgCntMax; // 前景最多的像素个数，如果超过这个个数，则不再检测，以防检测时间过长导致的超时
			int nmixtures;
			int var0[256], minVar[256];
			float varThrBasic[256];// 根据灰度值动态设置阈值
			float varThrDynamic[THRES_NUM]; // 根据动态程度设置阈值
			int occurBg; //匹配模态的发生次数数目等于或大于 occurBg，则为背景
			cv::Mat mtId; uchar* pdmtId;
			cv::Mat mtDy; uchar* pdmtDy;
			cv::Mat mtSum; uchar* pdmtSum;
			cv::Mat mtTra; uchar* pdmtTra;
			cv::Mat mtDyPost; uchar* pdmtDyPost;
			cv::Mat mtMax;  uchar* pdmtMax;
			uchar* pdBgmodel;
			int szData;

			//uchar* ptrId; // 当前的匹配的id
			//uchar* ptrDy; //  动态变化的次数
			//uchar* ptrSum; // 模态的个数
			//uchar* ptrTra; // 训练的次数
		};
		ParaBG para;
		ModelData mdata;

		static cv::Size szBorder; // 

	private:
		virtual void initialize(cv::Size _frameSize, int _frameType, ParaBG _para/*, float _sensity*/);
		cv::Size frameSize;
		int nTrain;
		float svar0; //起始标准差
		float sminVar; //最大标准差
		double svarThr; // 方差范围


		//int mk_high;
		//int mk_occur;
		//int mk_kel;
		int sftY;
		int sftX;
		//int val_occur_set;

		//float varThresAdjust[THRES_NUM];
		
	};



}