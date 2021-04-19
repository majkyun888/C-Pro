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
		void buildmodel(cv::InputArray _image, cv::InputArray _mask, bool _en_dy,bool _needToInitialize, ParaBG _para/*, float _sensity*/); // 20180525 ��ر����Ķ�̬�̶�
		void justify(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);
		void justifyLevel(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);
		void justifyDynamic(cv::InputArray _image, cv::OutputArray _fgmask, cv::InputArray _mask);	
		void getMaxModel(int _occur,int _thres, cv::Mat& mtMaxModel);

		ParticleType particle_type;
		cv::Mat bgmodel;
		struct ModelData
		{
			ParticleType particle_type;// �ش����ӵ����͡�
			int fgCntMax; // ǰ���������ظ������������������������ټ�⣬�Է����ʱ��������µĳ�ʱ
			int nmixtures;
			int var0[256], minVar[256];
			float varThrBasic[256];// ���ݻҶ�ֵ��̬������ֵ
			float varThrDynamic[THRES_NUM]; // ���ݶ�̬�̶�������ֵ
			int occurBg; //ƥ��ģ̬�ķ���������Ŀ���ڻ���� occurBg����Ϊ����
			cv::Mat mtId; uchar* pdmtId;
			cv::Mat mtDy; uchar* pdmtDy;
			cv::Mat mtSum; uchar* pdmtSum;
			cv::Mat mtTra; uchar* pdmtTra;
			cv::Mat mtDyPost; uchar* pdmtDyPost;
			cv::Mat mtMax;  uchar* pdmtMax;
			uchar* pdBgmodel;
			int szData;

			//uchar* ptrId; // ��ǰ��ƥ���id
			//uchar* ptrDy; //  ��̬�仯�Ĵ���
			//uchar* ptrSum; // ģ̬�ĸ���
			//uchar* ptrTra; // ѵ���Ĵ���
		};
		ParaBG para;
		ModelData mdata;

		static cv::Size szBorder; // 

	private:
		virtual void initialize(cv::Size _frameSize, int _frameType, ParaBG _para/*, float _sensity*/);
		cv::Size frameSize;
		int nTrain;
		float svar0; //��ʼ��׼��
		float sminVar; //����׼��
		double svarThr; // ���Χ


		//int mk_high;
		//int mk_occur;
		//int mk_kel;
		int sftY;
		int sftX;
		//int val_occur_set;

		//float varThresAdjust[THRES_NUM];
		
	};



}