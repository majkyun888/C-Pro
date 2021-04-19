#pragma once
#include "bgfgParticleDi.h"
#include "dataTypeParticle.h"
#include "../MiddlewareImprocessDll/dataType.h"


namespace algoParticleDi {

#define AP_FINDSIDE_MED 0
#define AP_FINDSIDE_FIT 1

	class ProcParticle
	{
	public:
		ProcParticle();
		~ProcParticle();
		/*long int getTime();*/
		void ProSpecInit(cv::Rect _roiSel);
		bool paraParticleInit();
		void VmCollectInit();
		void FilterLight(vector<cv::Mat>& vmIm);
		bool RegisterSeqFuren(vector<cv::Mat>& _vt_Im/*, vector<IREG>& _vireg,  RLT& _rlt*/);
		bool FrmDiff(vector<cv::Mat>& vmIm);
		bool ProcGlobal(vector<cv::Mat>& _vmIn/*, VMCollect& _vm_collect*/,vector<cv::Rect>& _rtMask /*cv::Mat _maskBlink*/);
		bool ProcDefect(vector<cv::Mat>& _vmIn);
		void SplitRegionFuren(/*RectS& _rtS*/);
		void DrawRltIm(vector<cv::Mat>& _vmIn, bool iret, vector<cv::Rect>& _rtBlink);
		bool DetectVolumes(cv::Mat _src, VolumesInOut* _pvolumes_inout);
		ParticleType justfyParticleType(cv::Mat _im);
		int m_seqNum;
		cv::Size m_imsize;
		VMCollect m_vmCollect;
		ParaParticle m_para;
		IsProcRegPD  m_procReg;
		ParticleType m_particle_type;
		RLT m_rlt;
		vector<IREG> m_vireg;
		PrePara m_prePara;
		VolumesInOut* mp_volumes_inout;
		int m_skip_cnt; //用于一些误检测的若干次跳出的机会

	private:
		cv::Rect shrkRect(cv::Rect Rt,  int sh);
		void fillBorder(cv::Mat& im, int brd, cv::Scalar clr);
		bool verifyRect(cv::Rect _rt, cv::Mat _mat);

		bool findLevel(vector<cv::Mat>& __vt_Im);
		bool LightFliter(cv::Mat src, cv::Mat mask, cv::Mat& fg, cv::Mat mtMogId, float _sensity, int& brdRNoDetect,float vorg);
		void LightFliter(cv::Mat src, cv::Mat mask, cv::Mat& fg, float _sensity);
		void findSideVec(vector<cv::Mat>& __vt_Im, vector<ProBottltBase>& _vtProBase, vector<int>& _vtLen);
		bool findBottom(vector<cv::Mat>& __vt_Im, vector<ProBottltBase>& _vtProBase);
		bool findBtLocY(vector<cv::Mat>& __vt_Im, vector<ProBottltBase>& _vtProBase);
		void findBottomVec(vector<cv::Mat>& __vt_Im, vector<ProBottltBase>& _vtProBase, vector<int>& _vtBtm);
		int findCannyBorder(cv::Mat _src, cv::Rect _roi, bool _iCol, bool _iTl);
		cv::Rect CalNewRoi(vector<cv::Mat>& _vmIn);
		cv::Mat mAccumulateCanny(vector<cv::Mat>& _src, cv::Mat _kn);
		void findDynamicBorder(cv::Mat _maskDy, int& _dyL, int& _dyR);
		void filterBottomBlink(cv::Mat& _fgk, cv::Mat _src, int _dyL);
		cv::Rect FilterLigtSpot(cv::Mat src, cv::Rect _roi);
		uchar calMatLocMean(cv::Mat _src, int _brd, int range, bool _bRow, bool _bInc);
		
		//
		void calMatLocMeanRange_beta(cv::Mat _src, cv::Mat& _ref, int _brd, bool _bRow, bool _bInc , int range);

		////
		//int findSide_beta(cv::Mat _src, cv::Mat _ref, int _brd, bool _bInv, bool _bRow, bool _bInc);
		//
		int findSidePoint_beta(cv::Mat _src, cv::Mat _ref, int _brd, bool _bInv, bool _bRow, bool _bInc, int _method, cv::Point2f& _lines);
		vector<cv::Point> findSideVec_beta(cv::Mat _src, cv::Mat _ref, int _brd, bool _bInv, bool _bRow, bool _bInc);

		const int tmset;
		bool savetime;

		cv::Rect m_roiBorder; // 图像配准后，必须保留的安全边界
		int m_errLast;		
		ProBottltSpec m_proBottleSpec;
		BackgroundSubtrator* palg;
	};

	 template <typename T>  void sortVal(vector<T>& val);

}