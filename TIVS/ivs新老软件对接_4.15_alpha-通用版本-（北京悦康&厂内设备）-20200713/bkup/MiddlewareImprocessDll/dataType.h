#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
//#include "middlewareCommon.h"
using namespace std;
#define _para_test

namespace algoParticleDi {

	enum ParticleType
	{
		particle_di = 0,// 底光
		particle_bei = 1 // 背光
	};

struct IsProcRegPD {
	IsProcRegPD() {
		okFindLevel = okFindSide = okRegSide = okFindBottom = okRegBottom=false;
	}
	int/*bool*/ iFindLevel,// 寻找液位
		iFindSide, // 寻找两侧
		iRegSide, // 配准两侧
		iFindBottom, // 需按照底部
		iRegBottom; // 配准两侧
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
				ptLightSpot = cv::Point(50, 50);
				thr_locy_br = 254;
				thr_locy_cnt = 3;
				thr_diff_br = 3;
				thr_diff_cnt = 200000;
				rangeDetectMin = 15;
				lenMax = 500;
				lenMin = 50;
				scanUp = 100;
				bottle_blink_fliter_bright_thr = 50;  //  底部滤光阈值  50
				bottle_blink_filter_rangeY_max = 20;  //  底部Y方向最大边界  ，20
				bottle_blink_filter_rangeY_min = 5;  //  底部Y方向最小边界  ，5
				bottle_blink_filter_rangeX_left = 3;  //  底部X方向左移  ，3
				bottle_blink_filter_rangeX_right = 3; 
				val_dynamic_filter = 1.; // 动态前景滤波次数 ； 1
				DyEordeSize = 5; // 动态小背景腐蚀的次数 ； 5
				knDyTimes = 1; //底部腐蚀迭代的次数
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
				
				//占位，对背光暂时没有处理模块
				bottle_blink_fliter_bright_thr = 50;  //  底部滤光阈值  50
				bottle_blink_filter_rangeY_max = 20;  //  底部Y方向最大边界  ，20
				bottle_blink_filter_rangeY_min = 5;  //  底部Y方向最小边界  ，5
				bottle_blink_filter_rangeX_left = 3;  //  底部X方向左移  ，3
				bottle_blink_filter_rangeX_right = 3;
				val_dynamic_filter = 1.; // 动态前景滤波次数 ； 1
				DyEordeSize = 5; // 动态小背景腐蚀的次数 ； 5
				knDyTimes = 1; //底部腐蚀迭代的次数
				break;
			default:
				break;
			}
		}
#ifdef _para_test
		int thr_level_br; // 液位亮度
		int thr_level_cnt; // 液位点数
		int level_shiftUp;// 液位向上偏移
		int	level_shiftDown; // 液位向下偏移
#endif // _para_test
		int thr_side_br; // 两侧亮度
		int thr_bottom_cnt; //底部点数
		int thr_bottom_br; // 底部亮度
		int btThick; //底部厚度
		cv::Point ptSft; // 位置偏移
		cv::Point ptLightSpot; // 光圈
		int thr_locy_br; // 底部定位亮度
		int thr_locy_cnt; // 底部定位点数
		int thr_diff_br;// 帧差亮度
		int thr_diff_cnt; //帧差点数
		int rangeDetectMin; // 最小检测范围
		int lenMax; // 最大瓶身宽度
		int lenMin; //最小瓶身宽度
		int scanUp; // 搜寻偏移位置

		// 底部反光处理
		int bottle_blink_fliter_bright_thr;  //  底部滤光阈值  50
		int bottle_blink_filter_rangeY_max;  //  底部Y方向最大边界  ，20
		int bottle_blink_filter_rangeY_min;  //  底部Y方向最小边界  ，5
		int bottle_blink_filter_rangeX_left;  //  底部X方向最左移  ，3
		int bottle_blink_filter_rangeX_right;  //  底部X方向最右移  ，3
		//int bottle_blink_filter_rangeX_min;  //  底部X方向最小边界
		float val_dynamic_filter; // 动态前景滤波次数 ； 1
		int DyEordeSize; // 动态小背景腐蚀的次数 ； 5
		int knDyTimes; //底部腐蚀迭代的次数
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

