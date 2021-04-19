#pragma once
#include <opencv2/core/core.hpp>
#include <vector>
using namespace std;

namespace algoParticleDi {

	//enum ParticleType
	//{
	//	particle_di, particle_bei
	//};

	class ProBottltBase {
	public:
		int cen; // 中心位置
		int len; // 宽度，与roi无关
		int sideL; // 左侧边缘
		int sideR; // 右侧边缘
		int bottom;
		bool valid;
		cv::Point2f m_lines[2];
	};

	class ProBottltSpec : public  ProBottltBase {
	public:
		ProBottltSpec() { roiCorner.resize(4); roiCrackCorner.resize(4); }
		int levelTopLimit;// 手动设置的瓶子上边界，即为ROI区域的顶部
		int level;  //
		int levelUp; //
		int levelDown; //
		int levelDetect;// 设置检测区域以后，检测靠近液面的上边界，与液面检测，以及ratio比例有关
		int bottomDown;
		int bottomUp;
		int bottomLocy;
		int scanBorder;

		vector<cv::Point> roiCrackCorner;
		cv::Rect roiCrack;
		vector<cv::Point> roiCorner;
		cv::Rect roiOrigin; // 从界面回传回来的roi大小，用于生产背景建模的所需的缓冲区大小
		cv::Rect roi;
		cv::Rect roiLevel;
		cv::Rect roiAll;

		int m_passNum;// 对于一些比较特殊的场景忽略的张数，如突然的反光之类的。
	};

	enum ParticleRet
	{
		Good, ErRoi, ErReg, ErDiff, ErTimeout, Bad, NoReason, ErSplit, ErProcFail // 更改数据结构，可能导致配方加载失败 20180526
	};

	typedef struct _RLT {
		ParticleRet ret;
		int ret_fpmax_id;
		int ret_nozero_max;
		int ret_frm_sum;
		int ret_nozero_sum;
	}RLT;

	typedef struct _IREG {
		int index; // 序列编号
		cv::Point pt; // roi的检测区域所在
	}IREG;

	//typedef struct _IsProcRegPD {
	//	bool iFindLevel, iFindSide, iRegSide, iFindBottom, iRegBottom;
	//	bool okFindLevel, okFindSide, okRegSide, okFindBottom, okRegBottom;
	//}IsProcRegPD;

	typedef struct _VMCollect {
		vector<cv::Mat> vmFg;
		vector<cv::Mat> vmMask;
		vector<cv::Mat> vmRlt;
		vector<cv::Mat> vmMogId;
		vector<cv::Mat> vmDebug;
		vector<cv::Rect> vRoiLightSpot;// 液面高亮光斑滤除区域
	}VMCollect;


	typedef struct _ParaParticle {
		//int minnumimg;
		//int mintotnpt;
		int nozero_single_thr;  // 单帧粒子上限。若单张图像中像素个数大于或等于nozero_single_thr， 则该帧图像认为有异物
		int nozero_frm_sum_thr; // 粒子张数上限。推荐2。最小值1， 最大值不超过序列帧数
		float ratio; // 检测比例，上限1， 下限0.5。背光工位通常设置0.95，底光工位推荐0.8
		float sensity; // “检测灵敏度”，值越小则越灵敏，下限0.5，上限2,推荐值1
		bool iVimRltSave;
		int iVimOnlineSave;  //  0 good save ; 1 bad save; 2 all save; other no save
		cv::Rect roi;
	}ParaParticle;


	//class PrePara {
	//public:
	//	PrePara() {};
	//	~PrePara() {};
	//	void initParaByParticleType(ParticleType _particle_type) {
	//		switch (_particle_type)
	//		{
	//		case algoParticleDi::particle_di:
	//			thr_level_br = 200;
	//			thr_level_cnt = 10;
	//			level_shiftUp = 40 /*10*/; //  针对康宝样品1ml，漩涡可能较深，如果不往上多移动一些，可能找不到边界 200190721
	//			level_shiftDown = 5;
	//			thr_side_br = 25; // 15 康宝注释
	//			thr_bottom_cnt = 2;
	//			thr_bottom_br = 200;// ? ? ?
	//			btThick = 10;
	//			ptSft = cv::Point(5, 10); // 康宝厂内测试，晃动幅度较大
	//			thr_locy_br = 254;
	//			thr_locy_cnt = 3;
	//			thr_diff_br = 3;
	//			thr_diff_cnt = 200000;
	//			rangeDetectMin = 15;
	//			lenMax = 500;
	//			lenMin = 50 /*50*/; // 星昊200
	//			break;
	//		case algoParticleDi::particle_bei:
	//			thr_level_br = 40; // 星昊厂内
	//			thr_level_cnt = 30; // 星昊厂内
	//			level_shiftUp = 10;
	//			level_shiftDown = 5;
	//			thr_side_br = 60; // 创泰厂内
	//			thr_bottom_cnt = 2;
	//			thr_bottom_br = 85;// 康宝
	//			btThick = 10;
	//			ptSft = cv::Point(5, 10); // 创泰厂内允许的偏移可以大一些
	//			thr_locy_br = 254;
	//			thr_locy_cnt = 3;
	//			thr_diff_br = 3;
	//			thr_diff_cnt = 200000;
	//			rangeDetectMin = 15;
	//			lenMax = 500;
	//			lenMin = 50;
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//	int thr_level_br; // 200
	//	int thr_level_cnt; // 10， // 东耀140
	//	int level_shiftUp;// 10
	//	int	level_shiftDown; // 5
	//	int thr_side_br; // 15
	//	int thr_bottom_cnt; //2
	//	int thr_bottom_br; // 200 ???
	//	int btThick; //10
	//	cv::Point ptSft; // 5,5
	//	int thr_locy_br; // 254
	//	int thr_locy_cnt; // 3
	//	int thr_diff_br;// 3
	//	int thr_diff_cnt; //200000
	//	int rangeDetectMin; // 15 
	//	int lenMax; // 瓶子宽度的最大值
	//	int lenMin; //瓶子宽度的最小值
	//};

}