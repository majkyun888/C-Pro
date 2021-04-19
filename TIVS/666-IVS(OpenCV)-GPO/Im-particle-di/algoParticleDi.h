#pragma once
#include "bgfgParticleDi.h"
#include "dataTypeParticle.h"
#include "../MiddlewareImprocessDll/dataType.h"


namespace algoParticleDi {



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
		bool ProcGlobal(vector<cv::Mat>& _vmIn/*, VMCollect& _vm_collect*/, cv::Mat _maskBlink);
		void SplitRegionFuren(/*RectS& _rtS*/);
		void DrawRltIm(vector<cv::Mat>& _vmIn, bool iret, vector<cv::Rect>& _rtBlink);
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
		const int tmset;
		bool savetime;

		cv::Rect m_roiBorder; // 图像配准后，必须保留的安全边界
		int m_errLast;		
		ProBottltSpec m_proBottleSpec;
		BackgroundSubtrator* palg;
	};

	 template <typename T>  void sortVal(vector<T>& val);

}