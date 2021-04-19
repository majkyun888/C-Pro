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
		int cen; // ����λ��
		int len; // ��ȣ���roi�޹�
		int sideL; // ����Ե
		int sideR; // �Ҳ��Ե
		int bottom;
		bool valid;
		cv::Point2f m_lines[2];
	};

	class ProBottltSpec : public  ProBottltBase {
	public:
		ProBottltSpec() { roiCorner.resize(4); roiCrackCorner.resize(4); }
		int levelTopLimit;// �ֶ����õ�ƿ���ϱ߽磬��ΪROI����Ķ���
		int level;  //
		int levelUp; //
		int levelDown; //
		int levelDetect;// ���ü�������Ժ󣬼�⿿��Һ����ϱ߽磬��Һ���⣬�Լ�ratio�����й�
		int bottomDown;
		int bottomUp;
		int bottomLocy;
		int scanBorder;

		vector<cv::Point> roiCrackCorner;
		cv::Rect roiCrack;
		vector<cv::Point> roiCorner;
		cv::Rect roiOrigin; // �ӽ���ش�������roi��С����������������ģ������Ļ�������С
		cv::Rect roi;
		cv::Rect roiLevel;
		cv::Rect roiAll;

		int m_passNum;// ����һЩ�Ƚ�����ĳ������Ե���������ͻȻ�ķ���֮��ġ�
	};

	enum ParticleRet
	{
		Good, ErRoi, ErReg, ErDiff, ErTimeout, Bad, NoReason, ErSplit, ErProcFail // �������ݽṹ�����ܵ����䷽����ʧ�� 20180526
	};

	typedef struct _RLT {
		ParticleRet ret;
		int ret_fpmax_id;
		int ret_nozero_max;
		int ret_frm_sum;
		int ret_nozero_sum;
	}RLT;

	typedef struct _IREG {
		int index; // ���б��
		cv::Point pt; // roi�ļ����������
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
		vector<cv::Rect> vRoiLightSpot;// Һ���������˳�����
	}VMCollect;


	typedef struct _ParaParticle {
		//int minnumimg;
		//int mintotnpt;
		int nozero_single_thr;  // ��֡�������ޡ�������ͼ�������ظ������ڻ����nozero_single_thr�� ���֡ͼ����Ϊ������
		int nozero_frm_sum_thr; // �����������ޡ��Ƽ�2����Сֵ1�� ���ֵ����������֡��
		float ratio; // ������������1�� ����0.5�����⹤λͨ������0.95���׹⹤λ�Ƽ�0.8
		float sensity; // ����������ȡ���ֵԽС��Խ����������0.5������2,�Ƽ�ֵ1
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
	//			level_shiftUp = 40 /*10*/; //  ��Կ�����Ʒ1ml�����п��ܽ����������϶��ƶ�һЩ�������Ҳ����߽� 200190721
	//			level_shiftDown = 5;
	//			thr_side_br = 25; // 15 ����ע��
	//			thr_bottom_cnt = 2;
	//			thr_bottom_br = 200;// ? ? ?
	//			btThick = 10;
	//			ptSft = cv::Point(5, 10); // �������ڲ��ԣ��ζ����Ƚϴ�
	//			thr_locy_br = 254;
	//			thr_locy_cnt = 3;
	//			thr_diff_br = 3;
	//			thr_diff_cnt = 200000;
	//			rangeDetectMin = 15;
	//			lenMax = 500;
	//			lenMin = 50 /*50*/; // ���200
	//			break;
	//		case algoParticleDi::particle_bei:
	//			thr_level_br = 40; // ��껳���
	//			thr_level_cnt = 30; // ��껳���
	//			level_shiftUp = 10;
	//			level_shiftDown = 5;
	//			thr_side_br = 60; // ��̩����
	//			thr_bottom_cnt = 2;
	//			thr_bottom_br = 85;// ����
	//			btThick = 10;
	//			ptSft = cv::Point(5, 10); // ��̩���������ƫ�ƿ��Դ�һЩ
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
	//	int thr_level_cnt; // 10�� // ��ҫ140
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
	//	int lenMax; // ƿ�ӿ�ȵ����ֵ
	//	int lenMin; //ƿ�ӿ�ȵ���Сֵ
	//};

}