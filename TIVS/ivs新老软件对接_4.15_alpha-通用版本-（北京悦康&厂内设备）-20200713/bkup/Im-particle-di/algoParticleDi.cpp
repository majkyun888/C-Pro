#include "algoParticleDi.h"
#include "..//MiddlewareImprocessDll/dataType.h"
using namespace cv;

namespace algoParticleDi {


	template<typename T>
	void sortVal(vector<T>& val)
	{
		int n = val.size();
		for (int j = 0; j < n - 1; j++)            // 每次最大元素就像气泡一样"浮"到数组的最后
		{
			for (int i = 0; i < n - 1 - j; i++)    // 依次比较相邻的两个元素,使较小的那个向后移
			{
				if (val[i] < val[i + 1])            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
				{
					std::swap(val[i], val[i + 1]);
				}
			}
		}
	}

	struct DatIdx
	{
		int val;
		int idx;
	};
	void sortDatIdx(vector<DatIdx>& _val) {
		int n = _val.size();
		for (int j = 0; j < n - 1; j++)            // 每次最大元素就像气泡一样"浮"到数组的最后
		{
			for (int i = 0; i < n - 1 - j; i++)    // 依次比较相邻的两个元素,使较小的那个向后移
			{
				if (_val[i].val < _val[i + 1].val)            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
				{
					std::swap(_val[i], _val[i + 1]);
				}
			}
		}
	}

	ProcParticle::ProcParticle() :
		tmset(600)
	{
		palg = NULL;
	}

	ProcParticle::~ProcParticle()
	{
		delete palg;
	}

	// 
	cv::Rect ProcParticle::shrkRect(cv::Rect Rt, int sh)
	{
		if (sh < 0)
			return Rt;
		cv::Rect RtA = Rt + cv::Point(sh, sh);
		RtA -= cv::Size(2 * sh, 2 * sh);
		if (RtA.x <= 0)
			RtA.x = 0;
		if (RtA.y <= 0)
			RtA.y = 0;
		if (RtA.width <= 0)
			Rt.width = 1;
		if (RtA.height <= 0)
			Rt.height = 1;
		return RtA;
	}

	void ProcParticle::fillBorder(cv::Mat & im, int brd, cv::Scalar clr)
	{
		cv::Mat mask = cv::Mat::zeros(im.size(), CV_8UC1);
		mask.setTo(cv::Scalar(255));
		cv::Rect rt = shrkRect(cv::Rect(cv::Point(0, 0), im.size()), brd);
		mask(rt).setTo(cv::Scalar(0));
		im.setTo(clr, mask);
	}

	bool ProcParticle::verifyRect(cv::Rect _rt, cv::Mat _mat)
	{
		if (_rt.tl().x < 0 || _rt.tl().y < 0 || _rt.br().x > _mat.cols || _rt.br().y > _mat.rows || _rt.width < 10 || _rt.height < 10)
			return false;
		else
			return true;
	}


	bool ProcParticle::findLevel(vector<cv::Mat> & __vt_Im) {

		vector< Mat > ims(3); ims.at(0) = __vt_Im.at(0);  ims.at(1) = __vt_Im.at(__vt_Im.size() / 2); ims.at(2) = __vt_Im.at(__vt_Im.size() - 1);
		int level_max(0); int level_min(0);  /*int levelUp(0); int levelDown(0);*/
		for (int i = 0; i < ims.size(); i++) {
			int level(0);
			Mat im = ims[i], imRow;
#ifdef _para_test
			if (m_particle_type == particle_di)
				threshold(im, imRow, m_prePara.thr_level_br, 1, THRESH_BINARY);
			else if (m_particle_type == particle_bei)
				cv::threshold(im, imRow, m_prePara.thr_level_br, 1, THRESH_BINARY_INV);
			else
				m_errLast;
			reduce(imRow, imRow, 0, CV_REDUCE_SUM, CV_32S);
			imRow.convertTo(imRow, CV_8U);
			threshold(imRow, imRow, m_prePara.thr_level_cnt, 255, THRESH_BINARY);
#else
			if (m_particle_type == particle_di)
				threshold(im, imRow, 200, 1, THRESH_BINARY);
			else if (m_particle_type == particle_bei)
				cv::threshold(im, imRow, 40, 1, THRESH_BINARY_INV);
			else
				m_errLast;
			reduce(imRow, imRow, 0, CV_REDUCE_SUM, CV_32S);
			imRow.convertTo(imRow, CV_8U);
			threshold(imRow, imRow, 5, 255, THRESH_BINARY);
#endif // _para_test


			int pyr = 3;
			resize(imRow, imRow, Size(imRow.cols / pyr, imRow.rows));
			uchar * ptr = imRow.ptr(0);
			//int scans(0);
			//scans = m_proBottleSpec.scanBorder / pyr;
			//scans = scans > 0 ? scans : 0;
			//scans = scans < imRow.cols ? scans : imRow.cols - 1;
			//for (int x = scans; x > m_proBottleSpec.level / pyr; x--) {
			//	if (255 == ptr[x]) {
			//		level = x;
			//		break;
			//	}
			//}
			int scMax = m_proBottleSpec.scanBorder / pyr;
			int scMin = m_proBottleSpec.level / pyr;
			for (int scx = scMax; scx > scMin; scx--) {
				if (255 == ptr[scx]) {
					level = scx;
					break;
				}
			}
			if (0 == level)
				continue; // 没有找到液面
			level = pyr * level;
			if (0 == level_max)
				level_max = level;
			if (0 == level_min)
				level_min = level;
			level_max = level > level_max ? level : level_max;
			level_min = level < level_min ? level : level_min;

		}
		if (level_max != 0 && level_min != 0) {
#ifdef _para_test
			m_proBottleSpec.levelDown = level_max + m_prePara.level_shiftDown < m_proBottleSpec.scanBorder ? level_max + m_prePara.level_shiftDown : m_proBottleSpec.scanBorder;
#else
			m_proBottleSpec.levelDown = level_max + 20 < m_proBottleSpec.scanBorder ? level_max + 20 : m_proBottleSpec.scanBorder;
#endif // _para_test

			m_proBottleSpec.level = level_max;
			return true;
		}
		else {
			;
			return false;  // 使用默认的液面值。
			//m_errLast = 1; // 获取液面错误
			//return false;
		}

	}

	void algoParticleDi::ProcParticle::findSideVec(vector<cv::Mat> & __vt_Im, vector<ProBottltBase> & _vtProBase, vector<int> & _vtLen)
	{
		/*_vtProBase.clear();*/
		if (__vt_Im.size() != _vtProBase.size())
			return;
		for (int i = 0; i < __vt_Im.size(); i++) {
			ProBottltBase pro_base;
			memset(&pro_base, 0, sizeof(ProBottltBase));
			Mat imcolrange = __vt_Im.at(i).colRange(m_proBottleSpec.levelUp, m_proBottleSpec.levelDown);
			Mat mbin;
			if (m_particle_type == particle_di) {
				cv::reduce(imcolrange, mbin, 1, CV_REDUCE_MAX);
				cv::threshold(mbin, mbin, m_prePara.thr_side_br, 255, THRESH_BINARY);
			}
			else if (m_particle_type == particle_bei) {
				cv::reduce(imcolrange, mbin, 1, CV_REDUCE_MIN);
				cv::threshold(mbin, mbin, m_prePara.thr_side_br, 255, THRESH_BINARY_INV);  // 不应该是固定值，而是通过直方图可以直接检测出来
			}
			int pyr = 2;
			resize(mbin, mbin, Size(mbin.cols, mbin.rows / pyr));
			int mark(0);
			for (int y = m_proBottleSpec.sideR / pyr; y < m_proBottleSpec.sideL / pyr; y++) {
				uchar* ptr = mbin.ptr(y);
				if (255 == ptr[0]) {
					pro_base.sideR = y * pyr;
					mark++;
					break;
				}
			}
			for (int y = m_proBottleSpec.sideL / pyr; y >= m_proBottleSpec.sideR / pyr; y--) {
				uchar* ptr = mbin.ptr(y);
				if (255 == ptr[0]) {
					pro_base.sideL = y * pyr;
					pro_base.len = pro_base.sideL - pro_base.sideR;
					pro_base.cen = (pro_base.sideL + pro_base.sideR) / 2;
					if (pro_base.len < m_prePara.lenMin || pro_base.len > m_prePara.lenMax)
						break;
					mark++;
					break;
				}
			}
			if (2 == mark) {
				pro_base.valid = true;
				/*_vtProBase.push_back(pro_base);*/
				_vtLen.push_back(pro_base.len);
			}
			else {
				pro_base.valid = false;
				/*_vtProBase.push_back(pro_base);*/
			}
			_vtProBase[i] = pro_base;
		}
	}

	bool ProcParticle::findBottom(vector<cv::Mat> & __vt_Im, vector<ProBottltBase> & _vtProBase)
	{
		int shrk(12);
		for (int i = 0; i < _vtProBase.size(); i++) {
			if (_vtProBase[i].valid == true) {
				int botm(0)/*, botmUp*/; // 测试下边缘所在位置
				cv::Mat imrow;
				cv::Mat im = __vt_Im[i].clone().colRange(m_proBottleSpec.scanBorder, m_proBottleSpec.bottomDown);
				if (m_particle_type == particle_di) 
					threshold(im, im, m_prePara.thr_bottom_br , 1, THRESH_BINARY);
				else if (m_particle_type == particle_bei) 
					cv::threshold(im, im, m_prePara.thr_bottom_br, 1, THRESH_BINARY_INV);
				else
					;
				reduce(im, imrow, 0, CV_REDUCE_SUM, CV_32S);
				imrow.convertTo(imrow, CV_8U);
				threshold(imrow, imrow, m_prePara.thr_bottom_cnt, 255, THRESH_BINARY);
				// 之前通过插值的方法，可能存在偏差
				/*int pyr = 2;*/
				int pyr = 1;
				resize(imrow, imrow, Size(imrow.cols / pyr, imrow.rows));
				uchar* ptr = imrow.ptr(0);
				for (int x = imrow.cols-1; x >= 0; x--) {
					if (255 == ptr[x]) {
						botm = x;
						break;
					}
				}

				int tmpBotm = botm;
				botm = pyr * botm + m_proBottleSpec.scanBorder;

				if (tmpBotm != 0) {
					m_proBottleSpec.bottom = botm;
					m_proBottleSpec.bottomUp = botm - m_prePara.btThick - m_prePara.ptSft.x * 3 > 0 ? botm - m_prePara.btThick - m_prePara.ptSft.x * 3 : 0;
					int btmDwn = botm + m_prePara.ptSft.x + 3 ;  // 防止瓶底不完整
					m_proBottleSpec.bottomDown = btmDwn < __vt_Im[i].cols - 1 ? btmDwn : __vt_Im[i].cols - 1; 
					//m_proBottleSpec.bottomDown = botm + m_prePara.ptSft.x + 3 < __vt_Im[i].cols - 1 ? botm + m_prePara.ptSft.x + 3 : __vt_Im[i].cols - 1; // 20181223 不需要移动那么多 防止瓶底以外有高亮的灰尘
					return true;
				}

			}
		}
		m_errLast = 2;
		return false;

	}

	bool ProcParticle::findBtLocY(vector<cv::Mat> & __vt_Im, vector<ProBottltBase> & _vtProBase)
	{
		int shrk(12);
		for (int i = 0; i < _vtProBase.size(); i++) {
			if (_vtProBase[i].valid == true) {
				/*Mat im = __vt_Im[i](__roi + Point(0, _vt_pro_bottleDi[i].cen - _mea_cen)).clone();*/
				Mat imcolrange = __vt_Im[i].colRange(m_proBottleSpec.bottomUp, m_proBottleSpec.bottomDown).clone();
				int thres_type(0);
				if (m_particle_type == particle_di)
					thres_type = THRESH_BINARY;
				else if (m_particle_type == particle_bei)
					thres_type = THRESH_BINARY_INV;
				else
					;
				threshold(imcolrange, imcolrange, m_prePara.thr_locy_br, 1, thres_type);
				reduce(imcolrange, imcolrange, 1, CV_REDUCE_SUM, CV_32SC1);
				imcolrange.convertTo(imcolrange, CV_8U);
				if (imcolrange.rows < 30)
					break;
				imcolrange = imcolrange.rowRange(shrk, imcolrange.rows - shrk);
				double minVal, maxVal;
				Point minLoc, maxLoc;
				minMaxLoc(imcolrange, &minVal, &maxVal, &minLoc, &maxLoc);
				if (maxVal > m_prePara.thr_locy_cnt)
				{
					m_proBottleSpec.bottomLocy = maxLoc.y + shrk;
					return true;
				}
			}
		}
		return false;
	}

	void ProcParticle::findBottomVec(vector<cv::Mat> & __vt_Im, vector<ProBottltBase> & _vtProBase, vector<int> & _vtBtm)
	{
		int shrk(12);
		for (int i = 0; i < _vtProBase.size(); i++) {
			if (_vtProBase[i].valid == true) {
				int btm(0);
				Mat imbtCut = __vt_Im[i].colRange(m_proBottleSpec.bottomUp, m_proBottleSpec.bottomDown).rowRange(m_proBottleSpec.bottomLocy - shrk, m_proBottleSpec.bottomLocy + shrk).clone();
				int thres_type(0);
				if (m_particle_type == particle_di)
					thres_type = THRESH_BINARY;
				else if (m_particle_type == particle_bei)
					thres_type = THRESH_BINARY_INV;
				else
					;
				threshold(imbtCut, imbtCut, m_prePara.thr_locy_br, 255, thres_type);
				reduce(imbtCut, imbtCut, 0, CV_REDUCE_MAX);
				uchar * ptr = imbtCut.ptr(0);
				for (int j = imbtCut.cols - 1; j > 0; j--) {
					if (ptr[j] == 255) {
						/*btm = j + m_proBottleSpec.bottomUp;*/
						btm = j ;
						break;
					}
				}
				if (btm == 0) {
					_vtProBase[i].valid = false;
					continue;
				}
				_vtProBase[i].bottom = btm  +m_proBottleSpec.bottomUp;
				_vtBtm.push_back(_vtProBase[i].bottom);
			}
		}

	}

	ParticleType ProcParticle::justfyParticleType(cv::Mat _im)
	{
		cv::Mat im = _im.clone();
		cv::Mat imbin, imbinH, imbinL;
		cv::threshold(im, imbinH, 100, 255, CV_THRESH_BINARY);
		cv::threshold(im, imbinL, 200, 255, CV_THRESH_BINARY_INV);
		cv::bitwise_and(imbinH, imbinL, imbin);
		double cntBin = cv::countNonZero(imbin);
		double cntTotal = imbin.total();
		double ratioCnt = cntBin / cntTotal;
		if (ratioCnt > 0.4)
			return particle_bei;
		else
			return particle_di;
	}

	void ProcParticle::ProSpecInit(cv::Rect _roiSel)
	{
		m_proBottleSpec.valid = true;
		m_proBottleSpec.bottom = _roiSel.br().x;
		m_proBottleSpec.bottomDown = m_proBottleSpec.bottom + 5;
		m_proBottleSpec.bottomDown = m_proBottleSpec.bottomDown < m_imsize.width ?
			m_proBottleSpec.bottomDown : m_imsize.width - 1;
		m_proBottleSpec.bottomUp = m_proBottleSpec.bottom - 5;
		//m_proBottleSpec.scanBorder = m_proBottleSpec.bottomUp - 55;  // 星昊现场，创泰厂内
		m_proBottleSpec.scanBorder = m_proBottleSpec.bottomUp - m_prePara.scanUp/*175*/;  // 康宝厂内
		m_proBottleSpec.level = _roiSel.tl().x;
		m_proBottleSpec.levelTopLimit = _roiSel.tl().x;
		if (m_procReg.iFindLevel)  // 如果找液位，则往下面平移一点
			m_proBottleSpec.levelDown = m_proBottleSpec.level + 5;
		else
			m_proBottleSpec.levelDown = m_proBottleSpec.level ;

		m_proBottleSpec.levelUp = m_proBottleSpec.level;
		m_proBottleSpec.sideL = _roiSel.br().y;
		m_proBottleSpec.sideR = _roiSel.tl().y;
		m_proBottleSpec.cen = m_proBottleSpec.sideL / 2 + m_proBottleSpec.sideR / 2;
		m_proBottleSpec.len = m_proBottleSpec.sideL - m_proBottleSpec.sideR;
		m_proBottleSpec.bottomLocy = m_proBottleSpec.cen;
		m_proBottleSpec.roi = _roiSel;
		m_proBottleSpec.roiOrigin = _roiSel;
	}

	bool ProcParticle::paraParticleInit()
	{
		m_roiBorder = cv::Rect();
		//if (m_particle_type == particle_di) {
		//	/*m_para.ratio = 0.85;*/
		//	m_prePara.thr_level_br = 200;
		//	m_prePara.thr_level_cnt = 10;
		//	m_prePara.level_shiftUp = 40 /*10*/; //  针对康宝样品1ml，漩涡可能较深，如果不往上多移动一些，可能找不到边界 200190721
		//	m_prePara.level_shiftDown = 5;
		//	m_prePara.thr_side_br = 25; // 15 康宝注释
		//	m_prePara.thr_bottom_cnt = 2;
		//	m_prePara.thr_bottom_br = 200;// ? ? ?
		//	m_prePara.btThick = 10;
		//	/*m_prePara.ptSft = cv::Point(5, 5);*/
		//	m_prePara.ptSft = cv::Point(5, 10); // 康宝厂内测试，晃动幅度较大
		//	m_prePara.thr_locy_br = 254;
		//	m_prePara.thr_locy_cnt = 3;
		//	m_prePara.thr_diff_br = 3;
		//	m_prePara.thr_diff_cnt = 200000;
		//	m_prePara.rangeDetectMin = 15;
		//	m_prePara.lenMax = 500;
		//	m_prePara.lenMin = 50 /*50*/; // 星昊200
		//}

		//if (m_particle_type == particle_bei) {
		//	/*m_para.ratio = 0.95;*/
		//	//m_prePara.thr_level_br = 100; // 东耀
		//	//m_prePara.thr_level_cnt = /*10*/ 140 ; // 东耀
		//	m_prePara.thr_level_br = 40; // 星昊厂内
		//	m_prePara.thr_level_cnt = 30; // 星昊厂内
		//	m_prePara.level_shiftUp = 10;
		//	m_prePara.level_shiftDown = 5;
		//	m_prePara.thr_side_br = 60; // 创泰厂内
		//	m_prePara.thr_bottom_cnt = 2;
		//	m_prePara.thr_bottom_br = 85;// 康宝
		//	m_prePara.btThick = 10;
		//	/*m_prePara.ptSft = cv::Point(5, 5);*/
		//	m_prePara.ptSft = cv::Point(5, 10); // 创泰厂内允许的偏移可以大一些
		//	m_prePara.thr_locy_br = 254;
		//	m_prePara.thr_locy_cnt = 3;
		//	m_prePara.thr_diff_br = 3;
		//	m_prePara.thr_diff_cnt = 200000;
		//	m_prePara.rangeDetectMin = 15;
		//	m_prePara.lenMax = 500;
		//	m_prePara.lenMin = 50;
		//}
		if (m_particle_type != particle_di && m_particle_type != particle_bei) {
			m_errLast;
			return false;
		}
		else
			return true;
	}

	void ProcParticle::VmCollectInit()
	{
		if (m_vmCollect.vRoiLightSpot.size() != m_seqNum + 1) {
			m_vmCollect.vRoiLightSpot.resize(m_seqNum + 1);
			m_vmCollect.vmDebug.resize(m_seqNum + 1);
			m_vmCollect.vmFg.resize(m_seqNum + 1);
			m_vmCollect.vmMask.resize(m_seqNum + 1);
			m_vmCollect.vmRlt.resize(m_seqNum + 1);
			m_vmCollect.vmMogId.resize(m_seqNum + 1);
		}

		for (int i = 0; i < m_seqNum; i++) {
			if (m_vmCollect.vmDebug[i].empty())
				m_vmCollect.vmDebug[i] = cv::Mat::zeros(m_imsize, CV_8UC1);
			else
				m_vmCollect.vmDebug[i] = cv::Scalar::all(0);
			if (m_vmCollect.vmFg[i].empty())
				m_vmCollect.vmFg[i] = cv::Mat::zeros(m_imsize, CV_8UC1);
			else
				m_vmCollect.vmFg[i] = cv::Scalar::all(0);
			if (m_vmCollect.vmMask[i].empty())
				m_vmCollect.vmMask[i] = cv::Mat::zeros(m_imsize, CV_8UC1);
			else
				m_vmCollect.vmMask[i] = cv::Scalar::all(0);
			
			if (m_vmCollect.vmMogId[i].empty())
				m_vmCollect.vmMogId[i] = cv::Mat::zeros(m_imsize, CV_8UC1);
			else
				m_vmCollect.vmMogId[i] = cv::Scalar::all(0);
			m_vmCollect.vRoiLightSpot[i] = cv::Rect();
		}
				//for (int i = 0; i < m_vireg.size(); i++) {
		//	if (m_vmCollect.vmDebug[m_vireg[i].index].empty())
		//		m_vmCollect.vmDebug[m_vireg[i].index] = cv::Mat::zeros(m_imsize, CV_8UC1);
		//	else
		//		m_vmCollect.vmDebug[m_vireg[i].index] = cv::Scalar::all(0);
		//	if (m_vmCollect.vmFg[m_vireg[i].index].empty())
		//		m_vmCollect.vmFg[m_vireg[i].index] = cv::Mat::zeros(m_imsize, CV_8UC1);
		//	else
		//		m_vmCollect.vmFg[m_vireg[i].index] = cv::Scalar::all(0);
		//	if (m_vmCollect.vmMask[m_vireg[i].index].empty())
		//		m_vmCollect.vmMask[m_vireg[i].index] = cv::Mat::zeros(m_imsize, CV_8UC1);
		//	else
		//		m_vmCollect.vmMask[m_vireg[i].index] = cv::Scalar::all(0);
		//	
		//	if (m_vmCollect.vmMogId[m_vireg[i].index].empty())
		//		m_vmCollect.vmMogId[m_vireg[i].index] = cv::Mat::zeros(m_imsize, CV_8UC1);
		//	else
		//		m_vmCollect.vmMogId[m_vireg[i].index] = cv::Scalar::all(0);
		//	m_vmCollect.vRoiLightSpot[m_vireg[i].index] = cv::Rect();
		//}
		if (m_vmCollect.vmDebug.rbegin()->empty())
			* m_vmCollect.vmDebug.rbegin() = cv::Mat::zeros(m_imsize, CV_8UC1);
		else
			*m_vmCollect.vmDebug.rbegin() = cv::Scalar::all(0);

		if (m_vmCollect.vmFg.rbegin()->empty())
			* m_vmCollect.vmFg.rbegin() = cv::Mat::zeros(m_imsize, CV_8UC1);
		else
			*m_vmCollect.vmFg.rbegin() = cv::Scalar::all(0);

		if (m_vmCollect.vmMask.rbegin()->empty())
			* m_vmCollect.vmMask.rbegin() = cv::Mat::zeros(m_imsize, CV_8UC1);
		else
			*m_vmCollect.vmMask.rbegin() = cv::Scalar::all(0);

		if (m_vmCollect.vmMogId.rbegin()->empty())
			* m_vmCollect.vmMogId.rbegin() = cv::Mat::zeros(m_imsize, CV_8UC1);
		else
			*m_vmCollect.vmMogId.rbegin() = cv::Scalar::all(0);

		*m_vmCollect.vRoiLightSpot.rbegin() = cv::Rect();
		if(m_roiBorder.size() == cv::Size())
			m_roiBorder = cv::Rect(cv::Point(), m_imsize);
	}

	void ProcParticle::FilterLight(vector<cv::Mat> & vmIm)
	{
		for (int i = 0; i < m_vireg.size(); i++) {
			int idx = m_vireg[i].index;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			cv::Mat src = vmIm[idx](roi);
			cv::Mat imLight;
			cv::threshold(src, imLight, 254, 255, CV_THRESH_BINARY_INV);
			cv::Mat& mask = m_vmCollect.vmMask[idx](roi);
			//cv::bitwise_and(mask, imLight, mask);
		}
	}


	bool ProcParticle::RegisterSeqFuren(vector<cv::Mat>& _vt_Im/*, vector<IREG>& _vireg, RLT& _rlt*/)
	{
		// --------------------初始化：开始------------------------------------
		m_vireg.clear();
		if (_vt_Im.size() < 10) {
			return false;
		}
		int szims = _vt_Im.size(); // 序列的张数
		int fmin = szims / 2; // 最小应满足的图片张数
		/*bool iret(true);*/
		// ---------------------初始化：结束-----------------------------------


		// --------------------寻找液位：开始------------------------------------
		if (m_procReg.iFindLevel == true)
			m_procReg.okFindLevel = findLevel(_vt_Im);
		// --------------------寻找液位：结束------------------------------------


		// --------------------寻找两侧向量：开始------------------------------------
		vector<ProBottltBase> vt_bottle_base = vector<ProBottltBase>(szims, static_cast<ProBottltBase>(m_proBottleSpec));
		vector<int> vt_len;
		if (m_procReg.iFindSide == true && m_procReg.okFindLevel == true) {
			vector<ProBottltBase> vt_bottle_base_old = vt_bottle_base;
			findSideVec(_vt_Im, vt_bottle_base, vt_len);
			int fpsValid = 0;
			int mea_len = 0;
			int mea_cen = 0;
			if (vt_len.size() > fmin) {
				sortVal(vt_len);
				mea_len = vt_len[vt_len.size() / 2];
				vector<int>  vt_cen;
				for (int i = 0; i < vt_bottle_base.size(); i++) {
					if (abs(vt_bottle_base[i].len - mea_len) <= m_prePara.ptSft.y && true == vt_bottle_base[i].valid)
						vt_cen.push_back(vt_bottle_base[i].cen);
					else
						vt_bottle_base[i].valid = false;
				}				
				if (vt_cen.size() > fmin) {
					sortVal(vt_cen);
					mea_cen = vt_cen[vt_cen.size() / 2];
					for (int i = 0; i < vt_bottle_base.size(); i++) {
						if (abs(vt_bottle_base[i].cen - mea_cen) > m_prePara.ptSft.y &&
							true == vt_bottle_base[i].valid)
							vt_bottle_base[i].valid = false;
					}
					for (int i = 0; i < vt_bottle_base.size(); i++) {
						if (vt_bottle_base[i].valid == true)
							fpsValid++;
					}
				}
			}
			if (fpsValid > fmin) {
				m_proBottleSpec.cen = mea_cen;
				m_proBottleSpec.len = mea_len;
				m_proBottleSpec.sideR = m_proBottleSpec.cen - m_proBottleSpec.len / 2;
				m_proBottleSpec.sideL = m_proBottleSpec.cen + m_proBottleSpec.len / 2;
				m_procReg.okFindSide = true;
				m_procReg.okRegSide = true;
				if (m_procReg.iRegSide == false) {
					m_procReg.okRegSide = false;
					for (int i = 0; i < vt_bottle_base.size(); i++)
						vt_bottle_base[i] = m_proBottleSpec;
				}
			}
			else {
				m_procReg.okFindSide = false;
				m_procReg.okRegSide = false;
				vt_bottle_base = vt_bottle_base_old;
			}
		}
		// ---------------------寻找两侧的有效值：结束------------------------------------


		if (m_procReg.iFindBottom == true )
			m_procReg.okFindBottom = findBottom(_vt_Im, vt_bottle_base);
		for (int i = 0; i < vt_bottle_base.size(); i++)
			vt_bottle_base[i].bottom = m_proBottleSpec.bottom;


		if (/*m_procReg.is_findBtLocY != 0 && m_procReg.is_findBottomVec != 0*/ m_procReg.iRegBottom == true && m_procReg.okFindBottom == true) {
			vector<ProBottltBase> vt_bottle_base_old = vt_bottle_base;
			bool iret = findBtLocY(_vt_Im, vt_bottle_base); 
			if (iret == true) {
				vector<int > vt_bottom;
				findBottomVec(_vt_Im, vt_bottle_base, vt_bottom);
				if (vt_bottom.size() > fmin) {
					sortVal(vt_bottom);
					int mea_bottom = vt_bottom[vt_bottom.size() / 2];// 占位 排序得到中位值
					for (int i = 0; i < vt_bottle_base.size(); i++) {
						if (vt_bottle_base[i].valid == true && abs(vt_bottle_base[i].bottom - mea_bottom) > m_prePara.ptSft.x) {
							vt_bottle_base[i].valid = false;
						}
					}
					int fpsValid = 0;
					for (int i = 0; i < vt_bottle_base.size(); i++) {  // 统计具体有多少是有效的图像张数，如果太少，则转入其他处理机制
						if (vt_bottle_base[i].valid == true) {
							fpsValid++;
						}
					}
					if (fpsValid > fmin)
						m_proBottleSpec.bottom = mea_bottom;
					else
						vt_bottle_base = vt_bottle_base_old;
				}
				else
					vt_bottle_base = vt_bottle_base_old;
			}else
				vt_bottle_base = vt_bottle_base_old;
		}
			
		


		//------------开始： 实用相位相关的方法检测平移，计算量可能比较大-----------------		
		std::vector<cv::Point2d>  shiftCor;
		if (false/*numValid < numValidPreSet*/) {
			cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR),
				Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL));
			//cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR -20),
			//	Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL+20));
			cv::Rect rtIntersect = rtRegOrg & cv::Rect(Point(0, 0), Point(_vt_Im.at(0).cols, _vt_Im.at(0).rows));
			if (rtIntersect != rtRegOrg) {
				//_rlt.ret = ErReg;  //此处错误和选择的框有关，不好重现
				return false;
			}
			cv::Rect rtReg = cv::Rect(rtRegOrg.tl() + cv::Point(m_prePara.ptSft.x, m_prePara.ptSft.y), rtRegOrg.br() - cv::Point(m_prePara.ptSft.x, m_prePara.ptSft.y));

			cv::Mat  hann, curr32f, ref32f, ref;
			ref = _vt_Im.at(0)(rtReg);
			shiftCor.push_back(cv::Point2d(0, 0));
			for (int idx = 1; idx < _vt_Im.size(); idx++) {
				cv::Mat curr = _vt_Im.at(idx)(rtReg);
				if (hann.empty() == true) {
					cv::createHanningWindow(hann, ref.size(), CV_32F);
				}
				ref.convertTo(ref32f, CV_32F);
				curr.convertTo(curr32f, CV_32F);
				cv::Point2d shiftOffset = phaseCorrelate(ref32f, curr32f, hann);
				cv::Point2d shiftAbs = shiftOffset + shiftCor[idx - 1];
				if (abs(shiftAbs.x) < m_prePara.ptSft.x &&
					abs(shiftAbs.y) < m_prePara.ptSft.y
					&& abs(shiftOffset.x) < m_prePara.ptSft.x / 2 &&
					abs(shiftOffset.y) < m_prePara.ptSft.y / 2)
				{
					shiftCor.push_back(shiftAbs);
					ref = curr;
				}
				else {
					shiftCor.push_back(shiftCor[idx - 1]);
				}
			}
		}
		//------------结束： 实用相位相关的方法检测平移，计算量可能比较大-----------------

		//for (int i = 0; i < vt_pro_bottleDi.size(); i++) {
		//	if (vt_pro_bottleDi[i].valid == true) {
		//		//ParaRoi proi = { i, _roi + Point(vt_pro_bottleDi[i].bottomDown - mea_btdown, vt_pro_bottleDi[i].cen - mea_cen) };
				//ParaRoi proi = { i, _roi + Point(vt_pro_bottleDi[i].bottomDown - mea_btdown + shiftCor[i].x, vt_pro_bottleDi[i].cen - mea_cen + shiftCor[i].y) }; // 20181222 国瑞第二台实用相位匹配
		//		_vt_roi_reg.push_back(proi);
		//	}
		//}

		if (false /*numValid < numValidPreSet*/) {
			for (int idx = 0; idx < _vt_Im.size(); idx++) {
				IREG ireg = { idx, Point(shiftCor[idx].x, shiftCor[idx].y) };
				m_vireg.push_back(ireg);
			}
		}
		else {
				for (int i = 0; i < vt_bottle_base.size(); i++) {
					if (vt_bottle_base[i].valid == true) {
						IREG ireg = { i, Point(vt_bottle_base[i].bottom - m_proBottleSpec.bottom, vt_bottle_base[i].cen - m_proBottleSpec.cen) };
						m_vireg.push_back(ireg);
					}
				}

		}

		
		cv::Point ptTl = cv::Point(), ptBr = cv::Point();
		for (int i = 0; i < m_vireg.size(); i++) {
			cv::Point pt = m_vireg[i].pt;		
			ptTl.x = ptTl.x < pt.x ? ptTl.x : pt.x;
			ptTl.y = ptTl.y < pt.y ? ptTl.y : pt.y;
			ptBr.x = ptBr.x > pt.x ? ptBr.x : pt.x;
			ptBr.y = ptBr.y > pt.y ? ptBr.y : pt.y;			
		}
		m_roiBorder = cv::Rect(-ptTl + cv::Point(1,1), cv::Point(m_imsize.width - ptBr.x , m_imsize.height - ptBr.y) - cv::Point(1,1) );
		//if (m_vireg.size() < 10) {
		//	return false;
		//}

		if (m_vireg.size() < _vt_Im.size() / 2) {

		}

		return true;
	}


	//bool ProcParticle::RegisterSeqFuren(vector<cv::Mat> & _vt_Im/*, vector<IREG>& _vireg, RLT& _rlt*/)
	//{
	//	m_vireg.clear();
	//	if (_vt_Im.size() < 10) {
	//		return false;
	//	}
	//	bool iret(true);
	//	if (m_procReg.is_findLevel != 0)
	//		iret = findLevel(_vt_Im);
	//	vector<ProBottltBase> vt_bottle_base;
	//	vector<int> vt_len;
	//	if (m_procReg.is_findSideVec != 0 && iret == true) {
	//		findSideVec(_vt_Im, vt_bottle_base, vt_len);
	//	}
	//	else {
	//		vt_bottle_base = vector<ProBottltBase>(_vt_Im.size(), static_cast<ProBottltBase>(m_proBottleSpec));
	//		vt_len = vector<int>(vt_bottle_base.size(), m_proBottleSpec.len);
	//	}
	//	if (vt_len.size() <= _vt_Im.size() / 2)
	//		iret = false;
	//	int mea_len(0), mea_cen(0);
	//	int numValid(0);
	//	int numValidPreSet(12);
	//	vector<int>  vt_cen;
	//	if (m_procReg.is_findSideConfrim != 0  && true == iret) {
	//		sortVal(vt_len);
	//		mea_len = vt_len[vt_len.size() / 2];
	//		for (int i = 0; i < vt_bottle_base.size(); i++) {
	//			if (abs(vt_bottle_base[i].len - mea_len) <= m_prePara.ptSft.y && true == vt_bottle_base[i].valid)
	//				vt_cen.push_back(vt_bottle_base[i].cen);
	//			else
	//				vt_bottle_base[i].valid = false;
	//		}
	//		sortVal(vt_cen);
	//		mea_cen = vt_cen[vt_cen.size() / 2];
	//		/*int numValid(0);*/
	//		for (int i = 0; i < vt_bottle_base.size(); i++, numValid++) {
	//			if (abs(vt_bottle_base[i].cen - mea_cen) > m_prePara.ptSft.y &&
	//				true == vt_bottle_base[i].valid)
	//				vt_bottle_base[i].valid = false;
	//		}
	//		//if (numValid <= 5) {
	//		//	return false;
	//		//}
	//	}
	//	else {
	//		sortVal(vt_len);
	//		mea_len = vt_len[vt_len.size() / 2];
	//		for (int i = 0; i < vt_bottle_base.size(); i++) {
	//			if (vt_bottle_base[i].valid == true)
	//				vt_cen.push_back(vt_bottle_base[i].cen);
	//		}
	//		if (vt_cen.size() <= 5) {
	//			return false;
	//		}
	//		sortVal(vt_cen);
	//		mea_cen = vt_cen[vt_cen.size() / 2];
	//	}
	//	if (numValid > numValidPreSet) {
	//		m_proBottleSpec.cen = mea_cen;
	//		m_proBottleSpec.len = mea_len;
	//	}
	//	else
	//	{
	//		mea_cen = m_proBottleSpec.cen;
	//		mea_len = m_proBottleSpec.len;
	//	}
	//	m_proBottleSpec.sideR = mea_cen - mea_len / 2;
	//	m_proBottleSpec.sideL = mea_cen + mea_len / 2;
	//	if (m_procReg.is_findBoottom != 0)
	//		iret = findBottom(_vt_Im, vt_bottle_base);
	//	//if (false == iret) {
	//	//	m_errLast;
	//	//	return false;
	//	//}
	//	if (m_procReg.is_findBtLocY != 0 && iret == true)
	//		iret = findBtLocY(_vt_Im, vt_bottle_base);
	//	//if (false == iret) {
	//	//	m_errLast;
	//	//	return false;
	//	//}
	//	vector<int > vt_bottom;
	//	if (m_procReg.is_findBottomVec != 0 && iret == true) {
	//		findBottomVec(_vt_Im, vt_bottle_base, vt_bottom);
	//	}
	//	else {
	//		for (int i = 0; i < vt_bottle_base.size(); i++) {
	//			if (vt_bottle_base[i].valid == true) {
	//				vt_bottle_base[i].bottom = m_proBottleSpec.bottom;
	//				vt_bottom.push_back(m_proBottleSpec.bottom);
	//			}
	//		}
	//	}
	//	
	//	if (vt_bottom.size() < _vt_Im.size() / 2)
	//	{
	//		/*mea_bottom = m_proBottleSpec.bottom;*/
	//		numValid = -1;
	//	}
	//	else
	//	{
	//		int mea_bottom(-1);
	//		sortVal(vt_bottom);		
	//		mea_bottom = vt_bottom[vt_bottom.size() / 2];// 占位 排序得到中位值
	//		m_proBottleSpec.bottom = mea_bottom;
	//		for (int i = 0; i < vt_bottle_base.size(); i++) {
	//			if (vt_bottle_base[i].valid == true && abs(vt_bottle_base[i].bottom - mea_bottom) > m_prePara.ptSft.x) {
	//				vt_bottle_base[i].valid = false;
	//			}
	//		}
	//		numValid = 0;
	//		for (int i = 0; i < vt_bottle_base.size(); i++) {  // 统计具体有多少是有效的图像张数，如果太少，则转入其他处理机制
	//			if (vt_bottle_base[i].valid == true) {
	//				numValid++;
	//			}
	//		}
	//	}
	//	//------------开始： 实用相位相关的方法检测平移，计算量可能比较大-----------------		
	//	std::vector<cv::Point2d>  shiftCor;
	//	if ( false/*numValid < numValidPreSet*/) {
	//		cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR),
	//			Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL));
	//		//cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR -20),
	//		//	Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL+20));
	//		cv::Rect rtIntersect = rtRegOrg & cv::Rect(Point(0, 0), Point(_vt_Im.at(0).cols, _vt_Im.at(0).rows));
	//		if (rtIntersect != rtRegOrg) {
	//			//_rlt.ret = ErReg;  //此处错误和选择的框有关，不好重现
	//			return false;
	//		}
	//		cv::Rect rtReg = cv::Rect(rtRegOrg.tl() + cv::Point(m_prePara.ptSft.x, m_prePara.ptSft.y), rtRegOrg.br() - cv::Point(m_prePara.ptSft.x, m_prePara.ptSft.y));
	//		
	//		cv::Mat  hann, curr32f, ref32f, ref;
	//		ref = _vt_Im.at(0)(rtReg);
	//		shiftCor.push_back(cv::Point2d(0, 0));
	//		for (int idx = 1; idx < _vt_Im.size(); idx++) {
	//			cv::Mat curr = _vt_Im.at(idx)(rtReg);
	//			if (hann.empty() == true) {
	//				cv::createHanningWindow(hann, ref.size(), CV_32F);
	//			}
	//			ref.convertTo(ref32f, CV_32F);
	//			curr.convertTo(curr32f, CV_32F);
	//			cv::Point2d shiftOffset = phaseCorrelate(ref32f, curr32f, hann);
	//			cv::Point2d shiftAbs = shiftOffset + shiftCor[idx - 1];
	//			if (abs(shiftAbs.x) < m_prePara.ptSft.x &&
	//				abs(shiftAbs.y) < m_prePara.ptSft.y
	//				&& abs(shiftOffset.x) < m_prePara.ptSft.x / 2 &&
	//				abs(shiftOffset.y) < m_prePara.ptSft.y / 2)
	//			{
	//				shiftCor.push_back(shiftAbs);
	//				ref = curr;
	//			}
	//			else {
	//				shiftCor.push_back(shiftCor[idx - 1]);
	//			}
	//		}
	//	}
	//	//------------结束： 实用相位相关的方法检测平移，计算量可能比较大-----------------
	//	//for (int i = 0; i < vt_pro_bottleDi.size(); i++) {
	//	//	if (vt_pro_bottleDi[i].valid == true) {
	//	//		//ParaRoi proi = { i, _roi + Point(vt_pro_bottleDi[i].bottomDown - mea_btdown, vt_pro_bottleDi[i].cen - mea_cen) };
	//			//ParaRoi proi = { i, _roi + Point(vt_pro_bottleDi[i].bottomDown - mea_btdown + shiftCor[i].x, vt_pro_bottleDi[i].cen - mea_cen + shiftCor[i].y) }; // 20181222 国瑞第二台实用相位匹配
	//	//		_vt_roi_reg.push_back(proi);
	//	//	}
	//	//}
	//	if ( false /*numValid < numValidPreSet*/) {
	//		for (int idx = 0; idx < _vt_Im.size(); idx++) {
	//			IREG ireg = { idx, Point(shiftCor[idx].x, shiftCor[idx].y) };
	//			m_vireg.push_back(ireg);
	//		}
	//	}
	//	else {
	//		if ( numValid > _vt_Im.size() /2 ) {
	//			for (int i = 0; i < vt_bottle_base.size(); i++) {
	//				if (vt_bottle_base[i].valid == true) {
	//					IREG ireg = { i, Point(vt_bottle_base[i].bottom - m_proBottleSpec.bottom, vt_bottle_base[i].cen - m_proBottleSpec.cen) };
	//					/*IREG ireg = { i, Point(shiftCor[i].x, shiftCor[i].y) };*/
	//					m_vireg.push_back(ireg);
	//				}
	//			}
	//		}
	//		else {
	//			for (int i = 0; i < _vt_Im.size(); i++) {
	//				IREG ireg = { i, Point(0,0) };
	//				m_vireg.push_back(ireg);
	//			}
	//		}
	//	}
	//	//if (m_vireg.size() < 10) {
	//	//	return false;
	//	//}
	//	if (m_vireg.size() < _vt_Im.size() / 2) {
	//	}
	//	return true;
	//}

	bool  ProcParticle::FrmDiff(vector<cv::Mat> & vmIm)
	{
		/*cv::Rect roi = m_proBottleSpec.roi;*/
		cv::Rect roiall = m_proBottleSpec.roiAll;
		cv::Mat mask;

		for (int i = 1; i < m_vireg.size(); i++) {
			cv::Mat imcur = vmIm[m_vireg[i].index](roiall + m_vireg[i].pt);
			cv::Mat imbef = vmIm[m_vireg[i - 1].index](roiall + m_vireg[i - 1].pt);
			cv::Mat difP, difN, dif;
			cv::threshold(imcur - imbef, difP, m_prePara.thr_diff_br, 1, THRESH_BINARY);
			cv::threshold(imbef - imcur, difN, m_prePara.thr_diff_br, 1, THRESH_BINARY);
			cv::bitwise_or(difP, difN, dif);
			if (mask.empty() == true)
				mask = cv::Mat::zeros(dif.size(), CV_8UC1);
			mask = mask + dif;
		}
		cv::threshold(mask, mask, 0, 255, THRESH_BINARY);
		cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		//if (cv::countNonZero(mask) > m_prePara.thr_diff_cnt)
		//	return false;
		cv::Rect roilevel = m_proBottleSpec.roiLevel;
		cv::Mat kn5x5 = cv::getStructuringElement(MORPH_RECT, cv::Size(5, 5));
		for (int i = 0; i < m_vireg.size(); i++) {
			cv::Mat& mkall = m_vmCollect.vmMask[m_vireg[i].index](roiall + m_vireg[i].pt);
			mask.copyTo(mkall);
			cv::Mat imcur = vmIm[m_vireg[i].index](roilevel + m_vireg[i].pt);
			cv::Mat imBin;
			cv::threshold(imcur, imBin, 254, 255, THRESH_BINARY_INV	);
			cv::erode(imBin, imBin, kn5x5);
			cv::Mat& mkLevel = m_vmCollect.vmMask[m_vireg[i].index](roilevel + m_vireg[i].pt);
			cv::bitwise_and(imBin, mkLevel, mkLevel);
			
		}
		mask.copyTo((*m_vmCollect.vmMask.rbegin())(roiall));
		return true;
	}


	//cv::Mat ProcParticle::ShowRltIm(cv::Mat src, cv::Rect roi, cv::Mat fgk, bool iret)
	//{
	//	cv::Scalar color;
	//	if (false == iret)
	//		color = cv::Scalar(0, 0, 255);
	//	else
	//		color = cv::Scalar(0, 255, 0);
	//	cv::rectangle(src, roi, color, 3);
	//	src.setTo(cv::Scalar(0, 0.255), fgk);
	//	return src;
	//}

	// @param _roi: 检测区域
	// @param _iCol:投影方向  true/投影成一列    ; false/ 投影成一行
	// @param _iTl :扫描朝向 true/朝左上方向扫描    ; false/ 朝右下方向扫描
	int ProcParticle::findCannyBorder(cv::Mat _src, cv::Rect _roi,  bool _iCol, bool _iTl) {
		int border(-1);
		cv::Mat mReduce = _src(_roi).clone();
		cv::reduce(mReduce, mReduce, _iCol, CV_REDUCE_SUM, CV_32SC1);
		double minVal; double maxVal = 0; cv::Point minLoc = 0;  cv::Point maxLoc = 0;
		cv::minMaxLoc(mReduce, &minVal, &maxVal, &minLoc, &maxLoc);
		vector<int> vReduce = mReduce;
		sortVal(vReduce); int mea = vReduce.at(vReduce.size() / 2);
		if (2 * mea > maxVal)
			return border;
		if (false == _iCol) {
			int xLocLow = maxLoc.x - 10 > 0  ? maxLoc.x - 10 : 0;
			int xLocHigh = maxLoc.x + 10 < mReduce.cols - 1 ? maxLoc.x + 10 : mReduce.cols - 1;
			if (false == _iTl) {
				for (int x = xLocLow; x <= xLocHigh; x++) {
					if (mReduce.ptr<int>(0)[x] > mea / 2 + maxVal / 2) {
						border = _roi.tl().x +  x + 2;
						break;
					}
				}
			}
			else {
				for (int x = xLocHigh ; x >= xLocLow; x--) {
					if (mReduce.ptr<int>(0)[x] > mea / 2 + maxVal / 2) {
						border = _roi.tl().x + x - 2;
						break;
					}
				}
			}

		}
		else {
			int yLocLow = maxLoc.y - 10 > 0 ? maxLoc.y - 10 : 0;
			int yLocHigh = maxLoc.y + 10 < mReduce.rows - 1 ? maxLoc.y + 10 : mReduce.rows - 1;
			if (false == _iTl) {
				for (int y = yLocLow; y <= yLocHigh; y++) {
					if (mReduce.ptr<int>(y)[0] > mea / 2 + maxVal / 2) {
						border = _roi.tl().y +  y + 2;
						break;
					}
				}
			}
			else {
				for (int y = yLocHigh; y >= yLocLow; y--) {
					if (mReduce.ptr<int>(y)[0] > mea / 2 + maxVal / 2) {
						border = _roi.tl().y + y - 2;
						break;
					}
				}
			}
		}
		return border;
	}

	cv::Rect ProcParticle::CalNewRoi(vector<cv::Mat>& _vmIn) {
		cv::Mat maskCanny;
		cv::Rect roiEn = m_proBottleSpec.roi;

		int widEn = 15, hgtEn = 25;
		roiEn = cv::Rect(cv::Point(roiEn.tl().x, roiEn.tl().y - hgtEn), cv::Point(roiEn.br().x + widEn, roiEn.br().y + hgtEn));
		roiEn &= m_roiBorder;
		maskCanny = cv::Mat::zeros(roiEn.size()/*m_proBottleSpec.roi.size()*/, CV_8UC1);
		cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		int inter(4);

		for (int i = 0; i < m_vireg.size(); i++) {
			if (i % inter != 0)
				continue;
			int idx = m_vireg[i].index;
			cv::Rect roi = roiEn/*m_proBottleSpec.roi*/ + m_vireg[i].pt;
			cv::Mat src = _vmIn[idx](roi /*+ cv::Point(i % 2, 0)*/);
			int lowThreshold(20), ratio(3), kernel_size(3);
			Mat imCanny, row_sum_edges;
			Mat kn;
			blur(src, imCanny, Size(3, 3));
			Canny(imCanny, imCanny, lowThreshold, lowThreshold * ratio, kernel_size);
			cv::dilate(imCanny, imCanny, kn3x3);
			cv::threshold(imCanny, imCanny, 254, 1, THRESH_BINARY);
			maskCanny = maskCanny + imCanny;
		}
		/*cv::threshold(maskCanny, maskCanny, m_vireg.size() / 2, 255, THRESH_BINARY);*/
		cv::threshold(maskCanny, maskCanny, m_vireg.size() / 2 / inter, 255, THRESH_BINARY);
		cv::dilate(maskCanny, maskCanny, kn3x3);
		cv::threshold(maskCanny, maskCanny, 254, 1, CV_THRESH_BINARY);

		int brX = findCannyBorder(maskCanny, cv::Rect(cv::Point(maskCanny.cols * 2 / 3, 0), cv::Point(maskCanny.cols, maskCanny.rows)), false, false);
		if (brX == -1)
			return m_proBottleSpec.roi;

		int tlY = findCannyBorder(maskCanny, cv::Rect(cv::Point(0, 0), cv::Point(brX - 5, maskCanny.rows / 2)), true, true);
		if (tlY == -1)
			return m_proBottleSpec.roi;

		int brY = findCannyBorder(maskCanny, cv::Rect(cv::Point(0, maskCanny.rows / 2), cv::Point(brX - 5, maskCanny.rows)), true, false);
		if (brY == -1)
			return m_proBottleSpec.roi;

		/*cv::Rect roiNew = cv::Rect(m_proBottleSpec.roi.tl() + cv::Point(0, tlY),
			m_proBottleSpec.roi.tl() + cv::Point(brX, brY));*/
		cv::Rect roiNew = cv::Rect(roiEn.tl() + cv::Point(0, tlY),
			roiEn.tl() + cv::Point(brX, brY));
		roiNew &= m_proBottleSpec.roi;
		//if (((roiNew | m_proBottleSpec.roi) == m_proBottleSpec.roi) &&
		//	roiNew.width > 50 && roiNew.height > 50)
		//	return roiNew;
		if (roiNew.width > 50 && roiNew.height > 50)
			return roiNew;
		else
			return m_proBottleSpec.roi;
	}


	cv::Mat ProcParticle::mAccumulateCanny( vector<cv::Mat>& _vsrc, cv::Mat _kn) {
		cv::Mat cannyMask = cv::Mat::zeros(_vsrc[0].size(), CV_8UC1);
		int inter(3);
		for (int i = 0; i < m_vireg.size(); i++) {
			if (i % inter != 0)
				continue;
			int idx = m_vireg[i].index;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			/*cv::Rect roi = m_proBottleSpec.roiAll + m_vireg[i].pt;*/
			cv::Mat src = _vsrc[idx](roi);
			cv::Mat mask = cannyMask(roi);
			cv::Size msz = src.size();
			cv::Range rangTop(0, msz.height / 2);
			cv::Range rangeDown(msz.height / 2, msz.height);
			vector<cv::Mat> vmkCanny;
			cv::Mat mkCannyTop = mask.rowRange(rangTop);
			vmkCanny.push_back(mkCannyTop);
			cv::Mat mkCannyDwn = mask.rowRange(rangeDown);
			vmkCanny.push_back(mkCannyDwn);
			vector<cv::Mat> vsrcCanny;
			cv::Mat srcCannyTop = src.rowRange(rangTop);
			vsrcCanny.push_back(srcCannyTop);
			cv::Mat srcCannyDwn = src.rowRange(rangeDown);
			vsrcCanny.push_back(srcCannyDwn);
			for (int iv = 0; iv < vmkCanny.size(); iv++) {
				cv::Mat detected_edges;
				blur(vsrcCanny[iv], detected_edges, Size(3, 3));
				int lowThreshold = 9; float ratio = 3.; int kernel_size = 3;
				Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
				cv::dilate(detected_edges, detected_edges, _kn);
				cv::threshold(detected_edges, detected_edges, 254, 1, THRESH_BINARY);
				vmkCanny[iv] += detected_edges;
			}
		}
		//cv::threshold(cannyMask, cannyMask, m_vireg.size() * 2 / 3 /inter, 255, THRESH_BINARY_INV);// 星昊现场
		cv::threshold(cannyMask, cannyMask, m_vireg.size() /2 / inter, 255, THRESH_BINARY_INV);// 康宝厂内
		return cannyMask;

	}

	void ProcParticle::findDynamicBorder(cv::Mat _maskDy, int& _dyL, int& _dyR) {
		cv::Mat mRowDy = _maskDy.clone();
		/*cv::bitwise_not(mRowDy, mRowDy);*/
		cv::threshold(mRowDy, mRowDy, 254, 1, cv::THRESH_BINARY);
		cv::reduce(mRowDy, mRowDy, 0, CV_REDUCE_SUM, CV_32SC1);
		mRowDy.convertTo(mRowDy, CV_8U);
		cv::threshold(mRowDy, mRowDy, 50, 255, cv::THRESH_BINARY);
		_dyL = -1; _dyR = -1;
		uchar* ptr = mRowDy.ptr<uchar>(0);
		//for (int i = mRowDy.cols/2; i < mRowDy.cols; i++) // 防止液面干扰
		int scanSt = m_proBottleSpec.scanBorder - m_proBottleSpec.roi.tl().x;
		for (int i = scanSt > 0 ? scanSt : 0;  i < mRowDy.cols; i++) // 防止液面干扰
			if (ptr[i] == 255) {
				_dyL = i;
				for (int j = mRowDy.cols - 1; j >= _dyL; j--) {
					if (ptr[j] == 255) {
						_dyR = j;
						break;
					}
				}
				break;
			}
	}
	
	//int bottle_blink_fliter_bright_thr;
	//int bottle_blink_filter_rangeY_max;
	//int bottle_blink_filter_rangeY_min;
	//int bottle_blink_filter_rangeX_max;
	//int bottle_blink_filter_rangeX_min;

	void ProcParticle::filterBottomBlink(cv::Mat& _fgk, cv::Mat _src, int _dyL) {
		int cntFg(0);
		int cntFgThr(15);  //该操作比较费时，避免频繁操作，一旦前景像素个数大于单帧的像素上限，则没有必要重复检测。
		int cntNzeros = cv::countNonZero(_fgk.colRange(_dyL, _fgk.cols));
		if (0 == cntNzeros)
			return;
		for (int ncl = _dyL; ncl < _fgk.cols; ncl++) {
			cv::Mat mcol = _fgk.col(ncl);
			cv::Mat mmax;
			cv::reduce(mcol, mmax, 0, CV_REDUCE_MAX);
			if (mmax.ptr<uchar>(0)[0] != 0) {
				for (int nrw = 0; nrw < _fgk.rows; nrw++) {
					uchar& pix = _fgk.at<uchar>(nrw, ncl);
					if (pix != 0) {
						m_prePara.bottle_blink_filter_rangeX_left = m_prePara.bottle_blink_filter_rangeX_left > 1 ? m_prePara.bottle_blink_filter_rangeX_left : 1;
						m_prePara.bottle_blink_filter_rangeX_right = m_prePara.bottle_blink_filter_rangeX_right > 1 ? m_prePara.bottle_blink_filter_rangeX_right : 1;
						if (ncl - m_prePara.bottle_blink_filter_rangeX_left< 0 || ncl + m_prePara.bottle_blink_filter_rangeX_right >= _fgk.cols)
							continue;

						cv::Mat mre = _src.colRange(ncl - m_prePara.bottle_blink_filter_rangeX_left, ncl).rowRange(nrw - m_prePara.bottle_blink_filter_rangeY_max > 0 ? nrw - m_prePara.bottle_blink_filter_rangeY_max : 0, nrw - m_prePara.bottle_blink_filter_rangeY_min > 1 ? nrw - m_prePara.bottle_blink_filter_rangeY_min : 1);
						cv::reduce(mre, mre, 0, CV_REDUCE_MAX);
						cv::reduce(mre, mre, 1, CV_REDUCE_MAX);
						if (mre.at<uchar>(0, 0) > m_prePara.bottle_blink_fliter_bright_thr) {
							pix = 0;
							continue;
						}

						mre = _src.colRange(ncl - m_prePara.bottle_blink_filter_rangeX_left, ncl).rowRange(nrw + m_prePara.bottle_blink_filter_rangeY_min < _src.rows - 1 ? nrw + m_prePara.bottle_blink_filter_rangeY_min : _src.rows - 1, nrw + m_prePara.bottle_blink_filter_rangeY_max < _src.rows ? nrw + m_prePara.bottle_blink_filter_rangeY_max : _src.rows);
						cv::reduce(mre, mre, 0, CV_REDUCE_MAX);
						cv::reduce(mre, mre, 1, CV_REDUCE_MAX);
						if (mre.at<uchar>(0, 0) > m_prePara.bottle_blink_fliter_bright_thr) {
							pix = 0;
							continue;
						}

						mre = _src.colRange(ncl, ncl + m_prePara.bottle_blink_filter_rangeX_right).rowRange(nrw - m_prePara.bottle_blink_filter_rangeY_max > 0 ? nrw - m_prePara.bottle_blink_filter_rangeY_max : 0, nrw - m_prePara.bottle_blink_filter_rangeY_min > 1 ? nrw - m_prePara.bottle_blink_filter_rangeY_min : 1);
						cv::reduce(mre, mre, 0, CV_REDUCE_MAX);
						cv::reduce(mre, mre, 1, CV_REDUCE_MAX);
						if (mre.at<uchar>(0, 0) > ( pix + 30 < 200 ? pix+30:200 )) {
							pix = 0;
							continue;
						}

						mre = _src.colRange(ncl , ncl + m_prePara.bottle_blink_filter_rangeX_right).rowRange(nrw + m_prePara.bottle_blink_filter_rangeY_min < _src.rows - 1 ? nrw + m_prePara.bottle_blink_filter_rangeY_min : _src.rows - 1, nrw + m_prePara.bottle_blink_filter_rangeY_max < _src.rows ? nrw + m_prePara.bottle_blink_filter_rangeY_max : _src.rows);
						cv::reduce(mre, mre, 0, CV_REDUCE_MAX);
						cv::reduce(mre, mre, 1, CV_REDUCE_MAX);
						if (mre.at<uchar>(0, 0) > ( pix + 30 < 200 ? pix+30 : 200) ) {
							pix = 0;
							continue;
						}


						if (++cntFg > cntFgThr)
							continue;
					}
				}
				if (cntFg > cntFgThr)
					continue;
			}
		}
	}

	cv::Rect ProcParticle::FilterLigtSpot(cv::Mat src, cv::Rect _roi) {
		cv::Mat im = src(_roi).clone();
		cv::threshold(im, im, 254, 255, THRESH_BINARY);
		cv::Mat kn4x3 = cv::getStructuringElement(MORPH_RECT, cv::Size(4, 3));
		cv::dilate(im, im, kn4x3);
		vector<vector<cv::Point> > contours;
		cv::findContours(im.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		if (contours.size() == 0)
			return  cv::Rect();
		int idxMaxCon(0);
		int szMaxCon(0);
		for (int con = 0; con < contours.size(); con++) {
			if (szMaxCon < contours.at(con).size()) {
				szMaxCon = contours.at(con).size();
				idxMaxCon = con;
			}
		}
		cv::Rect rt = cv::boundingRect(contours.at(idxMaxCon));
		/*int enlargeX(30), enlargeY(10);*/
		
		rt = cv::Rect(	cv::Point(rt.tl().x, rt.tl().y - m_prePara.ptLightSpot.y), 
						cv::Point(rt.br().x + m_prePara.ptLightSpot.x, rt.br().y + m_prePara.ptLightSpot.y));
		rt = rt & cv::Rect(cv::Point() , _roi.size() );
		return rt + _roi.tl();
	}

	bool ProcParticle::ProcGlobal(vector<cv::Mat> & _vmIn/*, VMCollect& _vm_collect*/, cv::Mat _maskBlink)
	{
		bool good(true);
		cv::Size szImWhole = _vmIn.at(0).size();
		Mat kn2x1 = getStructuringElement(MORPH_RECT, Size(2, 1));
		Mat kn1x2 = getStructuringElement(MORPH_RECT, Size(1, 2));
		cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat kn5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::Mat kn3x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
		cv::Mat maskCanny = cv::Mat::zeros(szImWhole,CV_8UC1);
		maskCanny.setTo(255);
		if (m_particle_type == particle_bei)
			m_proBottleSpec.roi = CalNewRoi(_vmIn);
		else if (m_particle_type == particle_di/* && m_para.ratio > 0.65*/) {
			maskCanny = mAccumulateCanny(_vmIn, kn3x3);  // 暂时不适用canny算子，防止超时
			cv::erode(maskCanny, maskCanny, kn3x5);
		}
			
		
		if( NULL == palg)
			palg = new BackgroundSubtrator(m_para.sensity, m_particle_type, (m_proBottleSpec.roiOrigin.width )* m_proBottleSpec.roiOrigin.height); 
		/*BackgroundSubtrator* palg = new BackgroundSubtrator(m_para.sensity, m_particle_type);*/
		/*BackgroundSubtrator* palgLevel = new BackgroundSubtrator(m_para.sensity, m_particle_type);*/

		cv::Mat mtDyLast;
		vector<DatIdx> vecDatIdx;
		int ShiftErrIdx(-1);
		/*vecDatIdx.resize(m_vireg.size());*/
		//int reNum(1);
		//if (m_particle_type == particle_bei)
		//	reNum = 6;
		for (int i = 0; i < m_vireg.size(); i++) {
			int idx = m_vireg[i].index;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			/*roi &= cv::Rect(cv::Point(2, 2), cv::Point(szImWhole.width, szImWhole.height ));*/
			cv::Mat src = _vmIn[idx]( roi + cv::Point(i % 2, 0)  );
			cv::Mat mask = m_vmCollect.vmMask[idx](roi).clone();
			cv::dilate(mask, mask, kn3x3);
			
			if (mtDyLast.empty() == true)
				mtDyLast = cv::Mat::zeros(roi.size(), CV_8UC1);

			bool needToInitialize(false);
			if (0 == i)
				needToInitialize = true;
			palg->buildmodel(src, mask, true, needToInitialize);
			cv::Mat mtDyDiff  = palg->mdata.mtDy - mtDyLast;
			int cntDyDiff = cv::countNonZero(mtDyDiff.colRange(mtDyDiff.cols/2, mtDyDiff.cols) );
			DatIdx mdatIdx = { cntDyDiff, idx };
			vecDatIdx.push_back(mdatIdx);

			mtDyLast = palg->mdata.mtDy.clone();
			/*cv::Mat mmt = m_vmCollect.vmMogId[idx](roi) = palg->mdata.mtId.clone();*/
			palg->mdata.mtId.copyTo(m_vmCollect.vmMogId[idx](roi));

			//液面背景建模，暂时不用
			//cv::Rect roiLevel = m_proBottleSpec.roiLevel + m_vireg[i].pt;
			//cv::Mat	 srcLevel = _vmIn[idx](roiLevel + cv::Point(i%2,0));
			//cv::Mat  maskLevel = m_vmCollect.vmMask[idx](roiLevel).clone();
			//cv::dilate(maskLevel, maskLevel, kn3x3);
			//palgLevel->buildmodel(srcLevel, maskLevel, true);
		}
		sortDatIdx(vecDatIdx);
		ShiftErrIdx = vecDatIdx.begin()->idx -1 ;
		//if (vecDatIdx.begin()->val > (vecDatIdx.begin() + vecDatIdx.size() / 2)->val + 100)
		//	ShiftErrIdx = vecDatIdx.begin()->idx;

		//根据模态的变化程度来调整前背景判决阈值,仅针对主检测区域
		cv::Mat mtWeightDynamic = palg->mdata.mtDy.clone();
		cv::dilate(mtWeightDynamic, mtWeightDynamic, kn5x5);
		cv::threshold(mtWeightDynamic, mtWeightDynamic, 2, 255, THRESH_TOZERO);
		cv::Mat maskDySimple = mtWeightDynamic.clone();
		cv::threshold(maskDySimple, maskDySimple, 2, 255, THRESH_BINARY_INV);
		mtWeightDynamic.convertTo(mtWeightDynamic, CV_32F);
		
		if (m_particle_type == particle_di)
			mtWeightDynamic = mtWeightDynamic / 4. + 0.5;
		else {
			mtWeightDynamic = mtWeightDynamic / 4. + 0.5;
		}

		mtWeightDynamic.setTo(1., maskDySimple);
		//if (m_particle_type == particle_di)
		//	mtWeightDynamic.setTo(1., maskDySimple);
		//else
		//	mtWeightDynamic.setTo(1.);
		mtWeightDynamic.copyTo(palg->mdata.mtDyPost);

		//根据模态的变化程度，一旦变化程度超出阈值，则设置mask区域不检测
		/*float filter_dynamic_thr = (m_vireg.size() / 2 - 1) / (m_para.sensity + 0.2);*/
		/*float val_dynamic_filter = 1;*/
		if (m_particle_type == particle_bei)
			m_prePara.val_dynamic_filter = 1;
		float filter_dynamic_thr = (m_vireg.size() / 2 - 1) / m_prePara.val_dynamic_filter;
		cv::Mat fgDynamic = palg->mdata.mtDy.clone();	
		if (  false /*m_particle_type == particle_di *//*m_para.ratio < 0.65*/ ) {
			cv::threshold(fgDynamic, fgDynamic, 3, 255, CV_THRESH_BINARY);
			cv::dilate(fgDynamic, fgDynamic, kn5x5);
		}
		else
		{
			cv::threshold(fgDynamic, fgDynamic, int(filter_dynamic_thr), 255, CV_THRESH_BINARY);
			cv::dilate(fgDynamic, fgDynamic, kn3x3);
		}
		cv::Mat fgDynamicDown = fgDynamic.clone();
		vector<vector<cv::Point> > contours;
		cv::findContours(fgDynamicDown.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (size_t i = 1; i < contours.size(); i++)
			if (contours[i - 1].size() > contours[i].size())
				std::swap(contours[i - 1], contours[i]);
		cv::Mat fgDynamicDownSmall = cv::Mat::zeros(fgDynamic.size(), CV_8UC1);
		cv::Mat fgDynamicDownBig = cv::Mat::zeros(fgDynamic.size(), CV_8UC1);
		for (int i = 0; i < (int)contours.size() - 1; i++)
			cv::drawContours(fgDynamicDownSmall, contours, i, cv::Scalar::all(255), -1);
		cv::drawContours(fgDynamicDownBig, contours, contours.size() - 1, cv::Scalar::all(255), -1);
		/*int DyEordeSize = 5;*/
		if (m_particle_type == particle_bei)
			m_prePara.DyEordeSize = 1;
		cv::Mat knxn = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_prePara.DyEordeSize, m_prePara.DyEordeSize));
		cv::dilate(fgDynamicDownSmall, fgDynamicDownSmall, knxn);

		int dyL(-1), dyR(-1);
		findDynamicBorder(fgDynamic, dyL, dyR);
		//if (dyL != -1) {
		//	fgDynamic.colRange(dyL, fgDynamic.cols).setTo(255);
		//	palg->mdata.mtDyPost.colRange(dyL, fgDynamic.cols).setTo(255); // 动态区域设置最大变化值
		//}
		fgDynamic.copyTo(palg->mdata.mtDy);
		if(m_particle_type == particle_di)
			cv::bitwise_or(fgDynamic, fgDynamicDownSmall, fgDynamic);
		if(m_particle_type == particle_bei)
			mtWeightDynamic.setTo(2.5, fgDynamic);



		//用于将canny运算后得到的mask区域，在最终结果中显示出来。该区域将不被检测
		if (m_particle_type == particle_di) {
			//cv::Mat& sumDebug = (*m_vmCollect.vmDebug.rbegin())(m_proBottleSpec.roiAll);
			//cv::Mat fgCanny; cv::bitwise_not(maskCanny(m_proBottleSpec.roiAll), fgCanny);
			//fgCanny.copyTo(sumDebug);
			cv::Mat& sumDebug = (*m_vmCollect.vmDebug.rbegin())(m_proBottleSpec.roi);
			fgDynamic.copyTo(sumDebug);
		}

		// 主区域检测
		cv::Mat sumfgk = (*m_vmCollect.vmFg.rbegin())(m_proBottleSpec.roi);
		for (int i = 0; i <  m_vireg.size() ; i++) {
			int idx = m_vireg[i].index;
			//if (ShiftErrIdx == idx)
			//	continue;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			cv::Mat src = _vmIn[idx](roi);
			cv::Mat mkBlink = _maskBlink(roi).clone();
			cv::Mat fgkall = m_vmCollect.vmFg[idx](roi);
			
			cv::Rect roiLightSpot = cv::Rect(
				cv::Point(m_proBottleSpec.levelTopLimit, m_proBottleSpec.sideR),
				cv::Point(m_proBottleSpec.levelDetect + 20 < szImWhole.width ? m_proBottleSpec.levelDetect + 20 : szImWhole.width, m_proBottleSpec.sideL));
			roiLightSpot = FilterLigtSpot(_vmIn[idx], roiLightSpot);
			m_vmCollect.vRoiLightSpot[idx] = roiLightSpot;
			cv::Mat maskWhole = m_vmCollect.vmMask[idx];
			cv::rectangle(maskWhole, roiLightSpot, cv::Scalar::all(0), -1);
			cv::Mat mask = maskWhole(roi).clone();
			
			
			if ( m_particle_type == particle_di ) {
				cv::Mat maskDynamic;
				/*cv::bitwise_not(fgDynamic, maskDynamic);*/
				cv::bitwise_not(fgDynamicDownSmall, maskDynamic);
				/*maskDynamic.colRange(maskDynamic.cols - 50 > 0 ? maskDynamic.cols - 50: 0, maskDynamic.cols).setTo(255);*/
				cv::bitwise_and(maskDynamic, mask, mask);
			}
			
			//cv::bitwise_and(maskDynamic.colRange(0, mask.cols / 3), mask.colRange(0, mask.cols / 3), mask.colRange(0, mask.cols / 3));
			/*cv::Mat mask = m_vmCollect.vmMask[idx](roi).clone();*/
			cv::bitwise_not(mkBlink, mkBlink);
			cv::bitwise_and(mkBlink, mask, mask);
			//if (m_particle_type == particle_di)
			//	cv::bitwise_and(mask, maskCanny(roi), mask);
			fillBorder(mask, 3, 0);
			//if (dyR > -1)
			//	mask.colRange(dyR, mask.cols).setTo(0);
			palg->mdata.particle_type = m_particle_type; // 返回检测的粒子类型
			/*if(i == 12)*/
				palg->justify(src, fgkall, mask);  
			cv::Mat fgkallAbs;
			cv::threshold(fgkall, fgkallAbs, 254, 255, THRESH_BINARY);
			cv::threshold(fgkall, fgkall, 125, 255, THRESH_BINARY);
			cv::Mat fgkx, fgky;
			cv::erode(fgkall, fgkx, kn2x1);
			cv::erode(fgkall, fgky, kn1x2);
			cv::bitwise_or(fgkx, fgky, fgkall);	
			cv::bitwise_or(fgkallAbs, fgkall, fgkall);
			if (m_particle_type == particle_di)
			{	
				cv::Mat fgDynamicBottom = cv::Mat::zeros(mask.size(), CV_8UC1);
				fgkall.copyTo(fgDynamicBottom, fgDynamicDownBig);
				/*int knDyTimes = 1;*/
				for( int knt =0; knt< m_prePara.knDyTimes; knt++)
					cv::erode(fgDynamicBottom, fgDynamicBottom, kn2x1);
				fgDynamicBottom.copyTo(fgkall, fgDynamicDownBig);

				 //暂时注释，防止超时
				if (dyL > -1) {
					cv::Mat im = src;
					cv::Mat fgCur = fgkall.clone();
					filterBottomBlink(fgCur, src , dyL);
					cv::Mat fgBef = fgkall.clone();
					im = _vmIn[m_vireg[i - 3 > 0 ? i - 3 : 0].index](roi);
					filterBottomBlink(fgBef, im, dyL);
					cv::bitwise_or(fgCur, fgBef, fgkall);
				}
				//if (cv::countNonZero(fgkall)) {
				//	LightFliter(src, fgkall.clone(), fgkall, m_para.sensity);
				//}
				//暂时注释防止超时	

				if (cv::countNonZero(fgkall) ) {
					int brdRNoDetect(0);
					cv::Mat mmt = m_vmCollect.vmMogId[idx](roi).clone();
					if (idx == 0 && i+1 < m_vireg.size() ) // 防首张有液面反光，要在第二张才显示出来
						mmt = m_vmCollect.vmMogId[m_vireg[i + 1].index](roi).clone();
					bool bNeedReDetect = LightFliter(src, fgkall.clone(), fgkall, mmt, m_para.sensity, brdRNoDetect,3);
					if(bNeedReDetect == true)
						LightFliter(src, fgkall.clone(), fgkall, mmt, m_para.sensity, brdRNoDetect, 13);
					//cv::Mat fgkallCols = fgkall.colRange(0, fgkall.cols / 3);
					//if( countNonZero( fgkallCols) >  fgkallCols.cols * fgkallCols.rows /6 )
					//	if (i + 1 < m_vireg.size()) {
					//		mmt = m_vmCollect.vmMogId[m_vireg[i + 1].index](roi).clone();
					//		LightFliter(src, fgkall.clone(), fgkall, mmt, m_para.sensity, brdRNoDetect);
					//	}
					/*fgkall.colRange(0, brdRNoDetect < fgkall.cols/2 ? brdRNoDetect: fgkall.cols / 2).setTo(0, maskDynamic.colRange(0, brdRNoDetect < fgkall.cols / 2 ? brdRNoDetect : fgkall.cols / 2));*/
					//cv::Mat fgkCC; 
					//if (brdRNoDetect > 0) {
					//	fgkCC = fgkall.clone();
					//	cv::bitwise_and(maskDynamic.colRange(0, brdRNoDetect),
					//		fgkCC.colRange(0, brdRNoDetect),
					//		fgkCC.colRange(0, brdRNoDetect));
					//	cv::erode(fgkCC.colRange(0, brdRNoDetect), fgkCC.colRange(0, brdRNoDetect), kn2x1);
					//	fgkall.colRange(0, brdRNoDetect).setTo(0, maskDynamic.colRange(0, brdRNoDetect));
					//	cv::bitwise_or(fgkCC.colRange(0, brdRNoDetect), fgkall.colRange(0, brdRNoDetect), fgkall.colRange(0, brdRNoDetect));
					//}
				}
				;
			}
			else if (m_particle_type == particle_bei) {
			}
			else
				;
			int nozero_cur = cv::countNonZero(fgkall);
			if (nozero_cur >= m_para.nozero_single_thr - 1)
			{
				m_rlt.ret_nozero_sum += nozero_cur;
				cv::bitwise_or(fgkall, sumfgk, sumfgk);
				m_rlt.ret_frm_sum++;
				if (m_rlt.ret_nozero_max < nozero_cur)
				{
					m_rlt.ret_nozero_max = nozero_cur;
					m_rlt.ret_fpmax_id = i;
				}
				if (m_rlt.ret_frm_sum >= m_para.nozero_frm_sum_thr) {
					m_rlt.ret = Bad;
					good = false;
					if (m_para.iVimRltSave == false) {
						/*delete palg;*/
						/*delete palgLevel;*/
						return false;
					}
				}
			}
		}

		// 靠近液面的区域检测
		/*
		sumfgk = (*m_vmCollect.vmFg.rbegin())(m_proBottleSpec.roiLevel);
		for (int i = 0; i < m_vireg.size(); i++) {
			int idx = m_vireg[i].index;
			cv::Rect roi = m_proBottleSpec.roiLevel + m_vireg[i].pt;
			cv::Mat src = _vmIn[idx](roi);
			cv::Mat mkBlink = _maskBlink(roi).clone();
			cv::Mat fgkall = m_vmCollect.vmFg[idx](roi);
			cv::Mat mask = m_vmCollect.vmMask[idx](roi).clone();
			if (m_particle_type == particle_di)
				cv::bitwise_and(mask, maskCanny(roi), mask);
			fillBorder(mask, 3, 0);
			cv::bitwise_not(mkBlink, mkBlink);
			cv::bitwise_and(mkBlink, mask, mask);
			palgLevel->justifyLevel(src, fgkall, mask);
			cv::threshold(fgkall, fgkall, 125, 255, THRESH_BINARY);
			cv::Mat fgkx, fgky;
			cv::erode(fgkall, fgkx, kn2x1);
			cv::erode(fgkall, fgky, kn1x2);
			cv::bitwise_or(fgkx, fgky, fgkall);
			if (m_particle_type == particle_di) {
				if (cv::countNonZero(fgkall))
					LightFliter(src, fgkall.clone(), fgkall, m_para.sensity);
			}
			else if (m_particle_type == particle_bei)
				;
			else
				;
			int nozero_cur = cv::countNonZero(fgkall);
			if (nozero_cur >= m_para.nozero_single_thr - 1)
			{
				m_rlt.ret_nozero_sum += nozero_cur;
				bitwise_or(fgkall, sumfgk, sumfgk);
				m_rlt.ret_frm_sum++;
				if (m_rlt.ret_nozero_max < nozero_cur)
				{
					m_rlt.ret_nozero_max = nozero_cur;
					m_rlt.ret_fpmax_id = i;
				}
				if (m_rlt.ret_frm_sum >= m_para.nozero_frm_sum_thr) {
					m_rlt.ret = Bad;
					good = false;
					if (m_para.iVimRltSave == false) {
						delete palg;
						delete palgLevel;
						return false;
					}
				}
			}
		}
		*/

		/*delete palg;*/
		/*delete palgLevel;*/
		return good;
	}

	void ProcParticle::SplitRegionFuren(/*RectS& _rtS*/)
	{
		m_para.ratio = m_para.ratio < 1 ? m_para.ratio : 1;
		m_para.ratio = m_para.ratio > 0.2 ? m_para.ratio : 0.2;
		int rangeDetect = (m_proBottleSpec.bottomDown - m_proBottleSpec.levelDown) * m_para.ratio;
		rangeDetect = rangeDetect > m_prePara.rangeDetectMin ?
			rangeDetect : m_prePara.rangeDetectMin;
		m_proBottleSpec.levelDetect = m_proBottleSpec.bottomDown - rangeDetect;
		m_proBottleSpec.roi = cv::Rect(
			cv::Point(m_proBottleSpec.levelDetect, m_proBottleSpec.sideR),
			cv::Point(m_proBottleSpec.bottomDown, m_proBottleSpec.sideL));
		m_proBottleSpec.roiLevel = cv::Rect(
			cv::Point(m_proBottleSpec.levelUp, m_proBottleSpec.sideR),
			cv::Point(m_proBottleSpec.levelDetect + 5, m_proBottleSpec.sideL));
		m_proBottleSpec.roiAll = cv::Rect(
			cv::Point(m_proBottleSpec.levelUp, m_proBottleSpec.sideR),
			cv::Point(m_proBottleSpec.bottomDown, m_proBottleSpec.sideL)
		);
		//cv::Rect rtShrk = cv::Rect(cv::Point(2, 2), cv::Point(m_imsize.width - 2, m_imsize.width - 2)); // 防止图像配准过程中偏移到其他位置，但需要注意
		/*m_proBottleSpec.roi = cv::Rect(cv::Point(485, 79), cv::Size(240, 269));*/
		m_proBottleSpec.roi &= m_roiBorder;
		m_proBottleSpec.roi &= m_proBottleSpec.roiOrigin;
		m_proBottleSpec.roiLevel &= m_roiBorder;
		m_proBottleSpec.roiAll &= m_roiBorder;
	}


	bool ProcParticle::LightFliter(cv::Mat src, cv::Mat mask, cv::Mat & fg, cv::Mat mtMogId, float _sensity,int&  _brdRNoDetect, float vorg) {
		//  形态学滤波 5*5 
		// 找轮廓，对找到的轮廓逐一处理，
		// 轮廓对应的子图像，按照子图像的大小，加上偏移值；
		//首尾相互处理，统计不等0的个数
		//
		//
		if (fg.size() != src.size())
			fg = cv::Mat();
		if (fg.empty()) {
			fg.create(src.size(), CV_8UC1);
			fg = cv::Scalar_<uchar>(0);
		}

		cv::threshold(mtMogId, mtMogId, 0, 1, THRESH_BINARY);
		cv::dilate(mtMogId, mtMogId, getStructuringElement(MORPH_RECT, cv::Size(6, 1)));
		cv::reduce(mtMogId, mtMogId, 0, CV_REDUCE_SUM, CV_32F);
		mtMogId.convertTo(mtMogId, CV_8U);
		//cv::reduce(mtMogId, mtMogId, 0, CV_REDUCE_MAX, -1);
		//if (mtMogId.at<uchar>(0, 0) == 0)
		//	return;
		int brdR(0);
		_brdRNoDetect = 0;
		bool reT(false);
		for (size_t i = 0; i < mtMogId.cols; i++)
		{
			uchar val = mtMogId.at<uchar>(0, i);
			if (reT == false) {
				if (val > mask.rows / 4)
					_brdRNoDetect = i;
				else
					reT = true;
			}
			if (val == 0) {
				brdR = i;
				break;
			}
		}
		if (_brdRNoDetect > 0 && brdR == 0)
			brdR = mtMogId.cols- 5;
		//if (0 == brdR)
		//	return;
		cv::Mat maskOrg = mask.clone();
		/*mask.colRange(brdR, mask.cols).setTo(0);*/
		int* wgt = new int[brdR+1];
		float scale = _brdRNoDetect < 30 ? 30 : _brdRNoDetect > 60 ? 60 : _brdRNoDetect;
		scale = 30 * 30 / scale;
		/*float vorg = 3 ;*/
		for (size_t i = 0; i < brdR+1 ; i++)
		{
			float vl = float( brdR -i )/ scale;
			vl = vl > 0 ? vl : 0;
			vl = vl < 5 ? vl : 5;
			wgt[i] = vorg + vl;
		}

		cv::Mat maskDialte;
		cv::dilate(mask, maskDialte, cv::getStructuringElement(MORPH_RECT, cv::Size(5, 3)));
		vector<vector<cv::Point> > contours;
		cv::findContours(maskDialte.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		cv::Mat kn3x3 = cv::getStructuringElement(MORPH_RECT, cv::Size(3, 3));
		bool bNeedReDetect(false);
		for (size_t con = 0; con < contours.size(); con++) {
			cv::Rect rt_bound = cv::boundingRect(contours.at(con));
			int wgtVal = wgt[rt_bound.tl().x < brdR ? rt_bound.tl().x : brdR /*- 1*/];
			cv::Mat srcRoi = src(rt_bound).clone();
			srcRoi.convertTo(srcRoi, CV_16U);
			cv::Mat imOffset = cv::Mat(Size(srcRoi.cols, 1),CV_16UC1 /*CV_8UC1*/);
			for (int y = 0; y < imOffset.rows; ++y) {
				unsigned short* p = imOffset.ptr<unsigned short>(y);
				for (int x = 0; x < imOffset.cols; ++x) {
					float pix = imOffset.cols - x;
					pix = pix * wgtVal* (_sensity );
					p[x] = pix;
				}
			}
			cv::repeat(imOffset, srcRoi.rows, 1, imOffset);
			srcRoi += imOffset;
			cv::Mat fgRoi = fg(cv::Rect(rt_bound.tl() + cv::Point(1, 0), rt_bound.br()));
			float cntFgOrg = cv::countNonZero(fgRoi);
			for (int cl = 0; cl < fgRoi.cols; cl++) {
				cv::Mat mcol1 = srcRoi.col(cl);
				cv::Mat mcol2 = srcRoi.col(cl + 1);
				cv::Mat fgRoiCol = fgRoi.col(cl);
				cv::Mat mcoldiff = mcol1 - mcol2;
				mcoldiff.convertTo(mcoldiff, fgRoiCol.type());
				mcoldiff.copyTo(fgRoiCol);
			}
			cv::threshold(fgRoi, fgRoi, 0, 255, CV_THRESH_BINARY_INV);
			if (fgRoi.cols > 10 || fgRoi.rows > 10) {
				float cntFgAft = cv::countNonZero(fgRoi);
				if (cntFgAft / cntFgOrg < 0.3)
					bNeedReDetect = true;
			}
			cv::dilate(fgRoi, fgRoi, kn3x3);
		}
		delete[] wgt;
		cv::bitwise_and(maskOrg/*.colRange(0,brdR)*/, fg/*.colRange(0,brdR)*/, fg/*.colRange(0,brdR)*/);
		if (bNeedReDetect == true) {
			bNeedReDetect = (cv::countNonZero(fg) > 0);
		}
		return bNeedReDetect ;
	}

	void ProcParticle::LightFliter(cv::Mat src, cv::Mat mask, cv::Mat& fg, float _sensity) {
		//  形态学滤波 5*5 
		// 找轮廓，对找到的轮廓逐一处理，
		// 轮廓对应的子图像，按照子图像的大小，加上偏移值；
		//首尾相互处理，统计不等0的个数
		//
		//
		if (fg.size() != src.size())
			fg = cv::Mat();
		if (fg.empty()) {
			fg.create(src.size(), CV_8UC1);
			fg = cv::Scalar_<uchar>(0);
		}
		cv::Mat maskDialte;
		cv::dilate(mask, maskDialte, cv::getStructuringElement(MORPH_RECT, cv::Size(5, 3)));
		vector<vector<cv::Point> > contours;
		cv::findContours(maskDialte.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		cv::Mat kn3x3 = cv::getStructuringElement(MORPH_RECT, cv::Size(3, 3));
		/*cv::Mat imdr = cv::Mat::zeros(src.size(), CV_8UC1);*/
		/*cv::Mat diff = cv::Mat::zeros(src.size(), CV_8UC1);*/
		for (size_t con = 0; con < contours.size(); con++) {
			cv::Rect rt_bound = cv::boundingRect(contours.at(con));
			/*cv::drawContours(imdr, contours, static_cast<int>(con),
				cv::Scalar_<uchar>(255), -1);*/
			cv::Mat srcRoi = src(rt_bound).clone();
			srcRoi.convertTo(srcRoi, CV_16U);
			//cv::Mat imOffset = cv::Mat(Size(srcRoi.cols, 1), CV_16UC1 /*CV_8UC1*/);
			cv::Size isz = Size(srcRoi.cols, 1);
			cv::Mat imOffset = cv::Mat::zeros(isz, CV_16UC1);
			for (int y = 0; y < imOffset.rows; ++y) {
				unsigned short* p = imOffset.ptr<unsigned short>(y);
				for (int x = 0; x < imOffset.cols; ++x) {
					float pix = imOffset.cols - x;
					pix = pix * 4 * (_sensity + 1.1);
					p[x] = pix;
					/*p[x] = (imOffset.cols - x) * 3;*/
				}
			}
			cv::repeat(imOffset, srcRoi.rows, 1, imOffset);
			srcRoi += imOffset;
			/*cv::Mat imDiff = cv::Mat::zeros(cv::Size(srcRoi.cols - 1, srcRoi.rows), CV_8UC1);*/
			cv::Mat fgRoi = fg(cv::Rect(rt_bound.tl() + cv::Point(1, 0), rt_bound.br()));
			for (int cl = 0; cl < fgRoi.cols; cl++) {
				cv::Mat mcol1 = srcRoi.col(cl);
				cv::Mat mcol2 = srcRoi.col(cl + 1);
				cv::Mat fgRoiCol = fgRoi.col(cl);
				cv::Mat mcoldiff = mcol1 - mcol2;
				mcoldiff.convertTo(mcoldiff, fgRoiCol.type());
				mcoldiff.copyTo(fgRoiCol);
				/*fgRoiCol = mcol1 - mcol2;*/
			}
			cv::threshold(fgRoi, fgRoi, 0, 255, CV_THRESH_BINARY_INV);
			cv::dilate(fgRoi, fgRoi, kn3x3);
		}
		cv::bitwise_and(mask, fg, fg);
	}

	//如果漏帧可能会错位。
	void ProcParticle::DrawRltIm(vector<cv::Mat> & _vmIn, bool iret, vector<cv::Rect>& _rtBlink)
	{
		cv::Scalar color;
		if (false == iret)
			color = cv::Scalar(0, 0, 255);
		else
			color = cv::Scalar(0, 255, 0);
		/*cv::Mat & im = *m_vmCollect.vmRlt.rbegin() = _vmIn[m_vireg.rbegin()->index].clone();*/
		cv::Mat& im = *m_vmCollect.vmRlt.rbegin() = _vmIn[m_vireg.rbegin()->index].clone();
		if (im.channels() == 1)
			cv::cvtColor(im, im, CV_GRAY2BGR);
		cv::rectangle(im, m_proBottleSpec.roi, color, 3);
		cv::line(im, cv::Point(m_proBottleSpec.scanBorder, m_proBottleSpec.roi.tl().y), cv::Point(m_proBottleSpec.scanBorder, m_proBottleSpec.roi.br().y), color, 3);
		/*cv::rectangle(im, m_proBottleSpec.roiLevel, color, 3);*/
		im.setTo(cv::Scalar(255, 0, 0), *m_vmCollect.vmDebug.rbegin());
		im.setTo(cv::Scalar(0, 0, 255), *m_vmCollect.vmFg.rbegin());
		for (vector<cv::Rect>::iterator it = _rtBlink.begin(); it != _rtBlink.end(); it++)
			cv::rectangle(im, *it, cv::Scalar(255, 0, 0),2);

		string str;
		if (m_particle_type == particle_bei)
			str = "p_bei";
		else
			str = "p_di";
		cv::putText(im, str, cv::Point(0, im.rows / 4), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 1, 8);
		/*cv::putText(im, "0808Test", cv::Point(im.cols/2, im.rows / 2), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(255, 255, 0), 1, 8);*/

		if (m_para.iVimRltSave) {
			for (int i = 0; i < m_vmCollect.vmRlt.size() - 1; i++) {
				m_vmCollect.vmRlt[i] = cv::Mat();
			}
			for (int i = 0; i < m_vireg.size(); i++) {
				int idx = m_vireg[i].index;
				cv::Mat& imrlt = m_vmCollect.vmRlt[idx] = _vmIn[idx].clone();
				/*cv::Mat& imrlt = m_vmCollect.vmRlt[idx];*/
				if (imrlt.channels() == 1)
					cv::cvtColor(imrlt, imrlt, CV_GRAY2BGR);
				cv::rectangle(imrlt, m_proBottleSpec.roi + m_vireg[i].pt, color);
				cv::rectangle(imrlt, m_vmCollect.vRoiLightSpot[i], cv::Scalar(255, 0, 0));
				/*cv::rectangle(imrlt, m_proBottleSpec.roiLevel + m_vireg[i].pt, color);*/
				cv::Mat fgk = m_vmCollect.vmFg[idx].clone();
				cv::Mat imDebug = m_vmCollect.vmDebug[idx].clone();
				imrlt.setTo(cv::Scalar(255, 0, 0), imDebug);
				Mat kn = getStructuringElement(MORPH_RECT, Size(10, 10));
				dilate(fgk, fgk, kn);
				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;
				findContours(fgk, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				vector<Rect> contourRects(contours.size());
				for (size_t it = 0; it < contours.size(); it++)
				{
					Scalar color = Scalar(255, 0, 255);
					contourRects.at(it) = boundingRect(contours.at(it));
					rectangle(imrlt, contourRects.at(it), color);
				}

			}
		}

	}


}