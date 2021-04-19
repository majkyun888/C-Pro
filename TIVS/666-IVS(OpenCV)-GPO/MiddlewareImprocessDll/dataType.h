#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
//#include "middlewareCommon.h"
using namespace std;
//#define _para_test

namespace algoParticleDi {

	enum ParticleType
	{
		particle_di = 0,// �׹�
		particle_bei = 1 // ����
	};

struct IsProcRegPD {
	IsProcRegPD() {
		okFindLevel = okFindSide = okRegSide = okFindBottom = okRegBottom=false;
	}
	int/*bool*/ iFindLevel,// Ѱ��Һλ
		iFindSide, // Ѱ������
		iRegSide, // ��׼����
		iFindBottom, // �谴�յײ�
		iRegBottom; // ��׼����
	bool okFindLevel, okFindSide, okRegSide, okFindBottom, okRegBottom; //�����Ž��棬��Ϊfalse
};

	class PrePara {
	public:
		PrePara() {};
		~PrePara() {};
		void initParaByParticleType(ParticleType _particle_type) {
			switch (_particle_type)
			{
			case algoParticleDi::particle_di:
#ifdef _para_test
				thr_level_br = 200;
				thr_level_cnt = 10;
				level_shiftUp = 40;
				level_shiftDown = 5;
#endif // _para_test
				thr_side_br = 25;
				thr_bottom_cnt = 2;
				thr_bottom_br = 200;
				btThick = 10;
				ptSft = cv::Point(5, 10); 
				thr_locy_br = 254;
				thr_locy_cnt = 3;
				thr_diff_br = 3;
				thr_diff_cnt = 200000;
				rangeDetectMin = 15;
				lenMax = 500;
				lenMin = 50;
				scanUp = 100;
				break;
			case algoParticleDi::particle_bei:
#ifdef _para_test
				thr_level_br = 40;
				thr_level_cnt = 30;
				level_shiftUp = 10;
				level_shiftDown = 5;
#endif // _para_test
				thr_side_br = 60; 
				thr_bottom_cnt = 2;
				thr_bottom_br = 85;
				btThick = 10;
				ptSft = cv::Point(5, 10); 
				thr_locy_br = 254;
				thr_locy_cnt = 3;
				thr_diff_br = 3;
				thr_diff_cnt = 200000;
				rangeDetectMin = 15;
				lenMax = 500;
				lenMin = 50;
				scanUp = 100;
				break;
			default:
				break;
			}
		}
#ifdef _para_test
		int thr_level_br; // Һλ����
		int thr_level_cnt; // Һλ����
		int level_shiftUp;// Һλ����ƫ��
		int	level_shiftDown; // Һλ����ƫ��
#endif // _para_test
		int thr_side_br; // ��������
		int thr_bottom_cnt; //�ײ�����
		int thr_bottom_br; // �ײ�����
		int btThick; //�ײ����
		cv::Point ptSft; // λ��ƫ��
		int thr_locy_br; // �ײ���λ����
		int thr_locy_cnt; // �ײ���λ����
		int thr_diff_br;// ֡������
		int thr_diff_cnt; //֡�����
		int rangeDetectMin; // ��С��ⷶΧ
		int lenMax; // ���ƿ����
		int lenMin; //��Сƿ����
		int scanUp; // ��Ѱƫ��λ��
	};

}

class ParaInter /*:public  algoParticleDi::PrePara*/
{
public:

	algoParticleDi::ParticleType  particle_type;
	algoParticleDi::PrePara pre_para;
	algoParticleDi::IsProcRegPD is_proc_regPD;
	vector<cv::Rect> rtMaskBlink;
	bool isRead;
	int	test;
	ParaInter() {
		isRead = false;
	};
	void write(cv::FileStorage& fs) const;//Write serialization for this class
	void read(const cv::FileNode& node); //Read serialization for this class
};
//extern MIDDLEWARE_IMPROCESS_API  bool readInterParaFromXml(ParaInter& _para, string _xml_path);
//extern MIDDLEWARE_IMPROCESS_API  bool writeInterParaToXml(ParaInter& _para, string _xml_path);
//extern MIDDLEWARE_IMPROCESS_API  bool MiddleWriteInterParaToXml(ParaInter& _para_inter, string _path);
//extern MIDDLEWARE_IMPROCESS_API bool MiddleReadInterParaFromXml(ParaInter& _para_inter, string _path);

