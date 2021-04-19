#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;

namespace algoParticleDi {

	enum ParticleType
	{
		particle_di,// �׹�
		particle_bei // ����
	};

struct IsProcRegPD {
	IsProcRegPD() {
		okFindLevel = okFindSide = okRegSide = okFindBottom = okRegBottom=false;
	}
	bool iFindLevel,// Ѱ��Һλ 1
		iFindSide, // Ѱ������   1
		iRegSide, // ��׼����1
		iFindBottom, // �谴�յײ�1
		iRegBottom; // ��׼����1
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
				thr_level_br = 200;
				thr_level_cnt = 10;
				level_shiftUp = 40;
				level_shiftDown = 5;
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
				break;
			case algoParticleDi::particle_bei:
				thr_level_br = 40;
				thr_level_cnt = 30; 
				level_shiftUp = 10;
				level_shiftDown = 5;
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
				break;
			default:
				break;
			}
		}
		int thr_level_br; // Һλ����1
		int thr_level_cnt; // Һλ����1
		int level_shiftUp;// Һλ����ƫ��1
		int	level_shiftDown; // Һλ����ƫ��1
		int thr_side_br; // ��������1
		int thr_bottom_cnt; //�ײ�����1
		int thr_bottom_br; // �ײ�����1
		int btThick; //�ײ����1
		cv::Point ptSft; // λ��ƫ��1
		int thr_locy_br; // �ײ���λ����1
		int thr_locy_cnt; // �ײ���λ����1
		int thr_diff_br;// ֡������1
		int thr_diff_cnt; //֡�����1
		int rangeDetectMin; // ��С��ⷶΧ1
		int lenMax; // ���ƿ����1
		int lenMin; //��Сƿ����1
	};

}