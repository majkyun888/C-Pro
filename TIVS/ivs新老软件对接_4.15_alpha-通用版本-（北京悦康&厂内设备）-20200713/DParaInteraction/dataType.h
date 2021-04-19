#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;

namespace algoParticleDi {

	enum ParticleType
	{
		particle_di,// 底光
		particle_bei // 背光
	};

struct IsProcRegPD {
	IsProcRegPD() {
		okFindLevel = okFindSide = okRegSide = okFindBottom = okRegBottom=false;
	}
	bool iFindLevel,// 寻找液位 1
		iFindSide, // 寻找两侧   1
		iRegSide, // 配准两侧1
		iFindBottom, // 需按照底部1
		iRegBottom; // 配准两侧1
	bool okFindLevel, okFindSide, okRegSide, okFindBottom, okRegBottom; //不开放界面，都为false
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
		int thr_level_br; // 液位亮度1
		int thr_level_cnt; // 液位点数1
		int level_shiftUp;// 液位向上偏移1
		int	level_shiftDown; // 液位向下偏移1
		int thr_side_br; // 两侧亮度1
		int thr_bottom_cnt; //底部点数1
		int thr_bottom_br; // 底部亮度1
		int btThick; //底部厚度1
		cv::Point ptSft; // 位置偏移1
		int thr_locy_br; // 底部定位亮度1
		int thr_locy_cnt; // 底部定位点数1
		int thr_diff_br;// 帧差亮度1
		int thr_diff_cnt; //帧差点数1
		int rangeDetectMin; // 最小检测范围1
		int lenMax; // 最大瓶身宽度1
		int lenMin; //最小瓶身宽度1
	};

}