#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
//#include "middlewareCommon.h"
using namespace std;
#define _para_test
enum RltMode
{
	brNone = 0, brGood = 1, brBad = 2, brAll = 3
};

namespace algoParticleDi {




	class VolumesInOut //Һλ������
	{
	public:
		VolumesInOut() {
			paralHigh = 100; paralLow = 0.1; paralCof = 1.; coff = 120.; rltLevel
				= -1; rltBottom = -1; iret = true;
		};
		~VolumesInOut() {};
		float paralHigh; // ��λֵ
		float paralLow;  // ��λֵ
		float paralCof;  // ����ϵ��

		float rltVolumes; // ���ֵ
		float rltLevel; // Һλ��ͼ���еľ�������
		float rltBottom;
		float coff; //
		bool iret; 
	};



	enum ParticleType
	{
		particle_undef = -1, // δ����
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

struct ParaBG
{
	ParaBG() {
		occurBg = 2;
		var0_s = 5, var0_e = 10;
		minvar_s = 4, minvar_e = 8;
		varthres_s = 1, varthres_e = 1.5;
		is = 10, ie = 30;
		sensity = 1.;
	}
	int occurBg;
	float var0_s, var0_e;
	float minvar_s, minvar_e;
	float varthres_s, varthres_e;
	int is, ie;
	float sensity;

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
				level_shiftUp = 20/*40*/;
				level_shiftDown = 5;
#endif // _para_test
				thr_side_br = 25;
				thr_bottom_cnt = 2;
				thr_bottom_br = 100/*200*/;
				btThick = 10;
				ptSft = cv::Point(5, 10); 
				ptLightSpot = cv::Point(10/*50*/, 10/*50*/);
				thr_locy_br = 100/*254*/;
				thr_locy_cnt = 3;
				thr_diff_br = 3;
				thr_diff_cnt = 200000;
				rangeDetectMin = 15;
				lenMax = 500;
				lenMin = 50;
				scanUp = 100;
				bottle_blink_fliter_bright_thr = 50;  //  �ײ��˹���ֵ  50
				bottle_blink_filter_rangeY_max = 20;  //  �ײ�Y�������߽�  ��20
				bottle_blink_filter_rangeY_min = 5;  //  �ײ�Y������С�߽�  ��5
				bottle_blink_filter_rangeX_left = 3;  //  �ײ�X��������  ��3
				bottle_blink_filter_rangeX_right = 3; 
				val_dynamic_filter = 1.; // ��̬ǰ���˲����� �� 1
				DyEordeSize = 1/*5*/; // ��̬С������ʴ�Ĵ��� �� 5
				knDyTimes = 1; //�ײ���ʴ�����Ĵ���
				maskCorner_left_width =1;
				maskCorner_left_height =1;
				maskCorner_right_width =1;
				maskCorner_right_height =1;
				maskCorner_left_x = 1;
				maskCorner_left_y= 1;
				maskCorner_right_x = 1;
				maskCorner_right_y = 1;
				rgl = 55;
				
				bg.var0_s = 2.1;
				bg.var0_e = 6.1;
				bg.minvar_s = 2.1;
				bg.minvar_e = 5.1;
				bg.varthres_s = 1;
				bg.varthres_e = 1.2;
				bg.is = 10;
				bg.ie = 100;
				bg.sensity = 1.;
				bg.occurBg = 2;
				var_weight = 4.;

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
				ptLightSpot = cv::Point(50, 50);
				thr_locy_br = 254;
				thr_locy_cnt = 3;
				thr_diff_br = 3;
				thr_diff_cnt = 200000;
				rangeDetectMin = 15;
				lenMax = 500;
				lenMin = 50;
				scanUp = 100;
				
				//ռλ���Ա�����ʱû�д���ģ��
				bottle_blink_fliter_bright_thr = 50;  //  �ײ��˹���ֵ  50
				bottle_blink_filter_rangeY_max = 20;  //  �ײ�Y�������߽�  ��20
				bottle_blink_filter_rangeY_min = 5;  //  �ײ�Y������С�߽�  ��5
				bottle_blink_filter_rangeX_left = 3;  //  �ײ�X��������  ��3
				bottle_blink_filter_rangeX_right = 3;
				val_dynamic_filter = 1.; // ��̬ǰ���˲����� �� 1
				DyEordeSize = 5; // ��̬С������ʴ�Ĵ��� �� 5
				knDyTimes = 1; //�ײ���ʴ�����Ĵ���
				maskCorner_left_width = 1;
				maskCorner_left_height = 1;
				maskCorner_left_x = 1;
				maskCorner_left_y = 1;

				maskCorner_right_width = 1;
				maskCorner_right_height = 1;
				maskCorner_right_x = 1;
				maskCorner_right_y = 1;
				rgl = 55;

				bg.var0_s = 5;
				bg.var0_e = 5;
				bg.minvar_s = 5;
				bg.minvar_e = 5;
				bg.varthres_s = 1;
				bg.varthres_e = 1;
				bg.is = 10;
				bg.ie = 30;
				bg.sensity = 1.;
				bg.occurBg = 2;
				var_weight = 4.;

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
		cv::Point ptLightSpot; // ��Ȧ
		int thr_locy_br; // �ײ���λ����
		int thr_locy_cnt; // �ײ���λ����
		int thr_diff_br;// ֡������
		int thr_diff_cnt; //֡�����
		int rangeDetectMin; // ��С��ⷶΧ
		int lenMax; // ���ƿ����
		int lenMin; //��Сƿ����
		int scanUp; // ��Ѱƫ��λ��

		// �ײ����⴦��
		int bottle_blink_fliter_bright_thr;  //  �ײ��˹���ֵ  50
		int bottle_blink_filter_rangeY_max;  //  �ײ�Y�������߽�  ��20
		int bottle_blink_filter_rangeY_min;  //  �ײ�Y������С�߽�  ��5
		int bottle_blink_filter_rangeX_left;  //  �ײ�X����������  ��3
		int bottle_blink_filter_rangeX_right;  //  �ײ�X����������  ��3
		//int bottle_blink_filter_rangeX_min;  //  �ײ�X������С�߽�
		float val_dynamic_filter; // ��̬ǰ���˲����� �� 1
		int DyEordeSize; // ��̬С������ʴ�Ĵ��� �� 5
		int knDyTimes; //�ײ���ʴ�����Ĵ���

		// ����ƿ�����ұ߽ǵķ���
		int maskCorner_left_width;
		int maskCorner_left_height;
		int maskCorner_left_x;
		int maskCorner_left_y;

		int maskCorner_right_width;
		int maskCorner_right_height;
		int maskCorner_right_x;
		int maskCorner_right_y;

		int rgl;  // ����Һ����ţ��Ӽ��������Һ����ŵ�ƽ��ֵ,����Ե׹⹤λ

		ParaBG bg;
		float var_weight;
	};

}

class ParaInter /*:public  algoParticleDi::PrePara*/
{
public:

	algoParticleDi::ParticleType  particle_type;
	algoParticleDi::PrePara pre_para;
	algoParticleDi::IsProcRegPD is_proc_regPD;
	vector<cv::Rect> rtMaskBlink;
	algoParticleDi::VolumesInOut volumes_inout;
	bool isRead;
	int	test;
	ParaInter() {
		particle_type = algoParticleDi::particle_undef;
		isRead = false;
	};
	void write(cv::FileStorage& fs) const;//Write serialization for this class
	void read(const cv::FileNode& node); //Read serialization for this class
};
//extern MIDDLEWARE_IMPROCESS_API  bool readInterParaFromXml(ParaInter& _para, string _xml_path);
//extern MIDDLEWARE_IMPROCESS_API  bool writeInterParaToXml(ParaInter& _para, string _xml_path);
//extern MIDDLEWARE_IMPROCESS_API  bool MiddleWriteInterParaToXml(ParaInter& _para_inter, string _path);
//extern MIDDLEWARE_IMPROCESS_API bool MiddleReadInterParaFromXml(ParaInter& _para_inter, string _path);

