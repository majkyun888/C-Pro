#include "algoParticleDi.h"
#include "..//MiddlewareImprocessDll/dataType.h"
using namespace cv;
const int paramode(0); // 1 广东星昊与楚天pk的定制版本；

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
		m_skip_cnt = 1;
		m_particle_type = particle_undef;
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
		int level_max(-1); int level_min(-1);
		cv::Mat im;
		m_proBottleSpec.levelUp = m_proBottleSpec.level;
		for (int i = 0; i < ims.size(); i++) {
			
			im = ims[i];
			cv::Mat imRow;
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
			const uchar * ptr = imRow.ptr(0);
			int level(0);
			const int scMax = m_proBottleSpec.scanBorder / pyr;
			const int scMin = m_proBottleSpec.level / pyr;
			for (int scx = scMax; scx > scMin; scx--) {
				if (255 == ptr[scx]) {
					level = scx;
					break;
				}
			}

			if (-1 == level_max)
				level_max = 0;
			if (0 != level) {
				level = pyr * level;
				level_max = level > level_max ? level : level_max;
			}

			level = im.cols;
			for (int scx = scMin; scx < scMax; scx++) {
				if (255 == ptr[scx]) {
					level = scx;
					break;
				}
			}

			if (level_min == -1)
				level_min = im.cols;
			if (im.cols != level) {
				level = pyr * level;
				level_min = level < level_min ? level : level_min;
			}

		}

		if (level_min != im.cols) {
			m_proBottleSpec.levelUp = level_min;
		}

		if (level_max != 0) {
#ifdef _para_test
			m_proBottleSpec.levelDown = level_max + m_prePara.level_shiftDown < m_proBottleSpec.scanBorder ? level_max + m_prePara.level_shiftDown : m_proBottleSpec.scanBorder;
#else
			m_proBottleSpec.levelDown = level_max + 20 < m_proBottleSpec.scanBorder ? level_max + 20 : m_proBottleSpec.scanBorder;
#endif // _para_test
			m_proBottleSpec.level = level_max;
			return true;
		}
		else 
			return false;  // 使用默认的液面值。



	}

	int method_type(0);
	void algoParticleDi::ProcParticle::findSideVec(vector<cv::Mat>& __vt_Im, vector<ProBottltBase>& _vtProBase, vector<int>& _vtLen)
	{
		if (m_particle_type == particle_bei)
			method_type = 1;
		else
			method_type = 0;

		if (__vt_Im.size() != _vtProBase.size())
			return;
		cv::Mat refDiff;
		cv::Range scan_range;
		if (m_particle_type == particle_di) {
			if (method_type != 1) {
				scan_range = cv::Range(m_proBottleSpec.levelUp, m_proBottleSpec.levelDown);
			}
			else {
				/*scan_range = cv::Range(m_proBottleSpec.levelDown, m_proBottleSpec.levelDown + 300);*/
				scan_range = cv::Range(m_proBottleSpec.levelDown, m_proBottleSpec.bottomUp);
				refDiff = cv::Mat::zeros(__vt_Im.at(0).colRange(scan_range).size(), CV_8UC1);
				cv::Mat src = __vt_Im.at(0).colRange(scan_range);
				calMatLocMeanRange_beta(src.rowRange(0, src.rows / 2), refDiff.rowRange(0, src.rows / 2), m_proBottleSpec.sideR, true, true, 5);
				calMatLocMeanRange_beta(src.rowRange(src.rows / 2, src.rows), refDiff.rowRange(src.rows / 2, src.rows), m_proBottleSpec.sideL - src.rows / 2, true, false, 5);
			}
			
		}
		else if (m_particle_type == particle_bei) {
			if (method_type != 1) {
				int levelDownShift = m_proBottleSpec.levelDown + 20 < __vt_Im.at(0).cols ? m_proBottleSpec.levelDown + 20 : __vt_Im.at(0).cols;
				scan_range = cv::Range(m_proBottleSpec.levelDown, levelDownShift);
			}
			else {
				scan_range = cv::Range(m_proBottleSpec.levelDown, m_proBottleSpec.bottomUp);
				/*scan_range = cv::Range(m_proBottleSpec.levelUp, m_proBottleSpec.bottomUp- 60);*/
				refDiff = cv::Mat::zeros(__vt_Im.at(0).colRange(scan_range).size(), CV_8UC1);
				cv::Mat src = __vt_Im.at(0).colRange(scan_range);
				calMatLocMeanRange_beta(src.rowRange(0, src.rows / 2), refDiff.rowRange(0, src.rows / 2), m_proBottleSpec.sideR, true, true, 5);
				calMatLocMeanRange_beta(src.rowRange(src.rows / 2, src.rows), refDiff.rowRange(src.rows / 2, src.rows), m_proBottleSpec.sideL - src.rows / 2, true,false, 5);
			}

		}

		for (int i = 0; i < __vt_Im.size(); i++) {
			ProBottltBase pro_base;
			memset(&pro_base, 0, sizeof(ProBottltBase));
			cv::Mat imcolrange;
			if (method_type == 1 && m_particle_type == particle_bei)
			{
				imcolrange = __vt_Im.at(i).colRange(scan_range);
				//pro_base.sideR = findSide_beta(imcolrange.rowRange(0, imcolrange.rows / 2), refDiff.rowRange(0, imcolrange.rows / 2), m_proBottleSpec.sideR , true, true, true);
				//pro_base.sideL = imcolrange.rows / 2 +  findSide_beta(imcolrange.rowRange(imcolrange.rows / 2, imcolrange.rows), refDiff.rowRange(imcolrange.rows / 2, imcolrange.rows), m_proBottleSpec.sideL- imcolrange.rows / 2,true, true, false);
				pro_base.sideR = findSidePoint_beta(imcolrange.rowRange(0, imcolrange.rows / 2), refDiff.rowRange(0, imcolrange.rows / 2), m_proBottleSpec.sideR, true, true, true, AP_FINDSIDE_FIT, pro_base.m_lines[0]);
				pro_base.m_lines[0].y -= pro_base.m_lines[0].x * scan_range.start;
				pro_base.sideL = imcolrange.rows / 2 + findSidePoint_beta(imcolrange.rowRange(imcolrange.rows / 2, imcolrange.rows), refDiff.rowRange(imcolrange.rows / 2, imcolrange.rows), m_proBottleSpec.sideL - imcolrange.rows / 2, true, true, false, AP_FINDSIDE_FIT, pro_base.m_lines[1]);
				pro_base.m_lines[1].y += imcolrange.rows / 2;
				pro_base.m_lines[1].y -= pro_base.m_lines[1].x * scan_range.start;
				pro_base.valid = false;
				if (-1 != pro_base.sideL && -1 != pro_base.sideR) {
					pro_base.len = pro_base.sideL - pro_base.sideR;
					pro_base.cen = (pro_base.sideL + pro_base.sideR) / 2;
					if (pro_base.len > m_prePara.lenMin && pro_base.len < m_prePara.lenMax)
					{
						pro_base.valid = true;
						_vtLen.push_back(pro_base.len);
					}
				}
			}
			else if (method_type == 1 && m_particle_type == particle_di) {
				imcolrange = __vt_Im.at(i).colRange(scan_range);
				pro_base.sideR = findSidePoint_beta(imcolrange.rowRange(0, imcolrange.rows / 2), refDiff.rowRange(0, imcolrange.rows / 2), m_proBottleSpec.sideR, false, true, true, AP_FINDSIDE_FIT, pro_base.m_lines[0]);
				pro_base.m_lines[0].y -= pro_base.m_lines[0].x * scan_range.start;
				pro_base.sideL = imcolrange.rows / 2 + findSidePoint_beta(imcolrange.rowRange(imcolrange.rows / 2, imcolrange.rows), refDiff.rowRange(imcolrange.rows / 2, imcolrange.rows), m_proBottleSpec.sideL - imcolrange.rows / 2, false, true, false, AP_FINDSIDE_FIT, pro_base.m_lines[1]);
				pro_base.m_lines[1].y += imcolrange.rows / 2;
				pro_base.m_lines[1].y -= pro_base.m_lines[1].x * scan_range.start;
				pro_base.valid = false;
				if (-1 != pro_base.sideL && -1 != pro_base.sideR) {
					pro_base.len = pro_base.sideL - pro_base.sideR;
					pro_base.cen = (pro_base.sideL + pro_base.sideR) / 2;
					if (pro_base.len > m_prePara.lenMin && pro_base.len < m_prePara.lenMax)
					{
						pro_base.valid = true;
						_vtLen.push_back(pro_base.len);
					}
				}
			}
			else
			{
				Mat mbin;
				if (m_particle_type == particle_di) {
					/*Mat*/ imcolrange = __vt_Im.at(i).colRange(scan_range/*m_proBottleSpec.levelUp, m_proBottleSpec.levelDown*/);
					cv::reduce(imcolrange, mbin, 1, CV_REDUCE_MAX);
					cv::threshold(mbin, mbin, m_prePara.thr_side_br, 255, THRESH_BINARY);
				}
				else if (m_particle_type == particle_bei) {
					//int levelDownShift = m_proBottleSpec.levelDown + 20 < __vt_Im.at(i).cols ? m_proBottleSpec.levelDown + 20 : __vt_Im.at(i).cols;
					imcolrange = __vt_Im.at(i).colRange(scan_range/*m_proBottleSpec.levelDown, levelDownShift*/);
					int range = 5;
					uchar meaL = calMatLocMean(imcolrange, m_proBottleSpec.sideL, range, true, true);
					uchar meaR = calMatLocMean(imcolrange, m_proBottleSpec.sideR, range, true, false);
					int thr_mea = 15;
					cv::reduce(imcolrange, mbin, 1, CV_REDUCE_MIN);
					cv::threshold(mbin.rowRange(0, mbin.rows / 2), mbin.rowRange(0, mbin.rows / 2), meaR - thr_mea, 255, THRESH_BINARY_INV);
					cv::threshold(mbin.rowRange(mbin.rows / 2, mbin.rows), mbin.rowRange(mbin.rows / 2, mbin.rows), meaL - thr_mea, 255, THRESH_BINARY_INV);
				}
				int mark(0);
				int pyr = 1/*2*/;
				if (pyr != 1)
					resize(mbin, mbin, Size(mbin.cols, mbin.rows / pyr));
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
					_vtLen.push_back(pro_base.len);
				}
				else {
					pro_base.valid = false;
				}
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


				cv::Mat im;
				if (m_particle_type == particle_di) {
					im = __vt_Im[i].clone().colRange(m_proBottleSpec.scanBorder, m_proBottleSpec.bottomDown);
					threshold(im, im, m_prePara.thr_bottom_br, 1, THRESH_BINARY);
				}
				else if (m_particle_type == particle_bei) {
					im = __vt_Im[i].clone().colRange(m_proBottleSpec.scanBorder, m_proBottleSpec.bottomDown).rowRange(m_proBottleSpec.sideR, m_proBottleSpec.sideL);
					uchar mea = calMatLocMean(im, im.cols - 1, 5, false, false);
					int thr_mea =  25  /*15*/;  //锦州奥鸿改成25，否则有时亮度找错了
					mea -= thr_mea;
					m_prePara.thr_locy_br = mea;
					cv::threshold(im, im, mea/*m_prePara.thr_bottom_br*/, 1, THRESH_BINARY_INV);
				}
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
					if (m_particle_type == particle_bei) { // 锦州奥鸿底部有干扰
						btmDwn -= m_prePara.ptSft.x + 3;
					}
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

				if (m_particle_type == particle_di) {
					imcolrange = imcolrange.rowRange(shrk, imcolrange.rows - shrk);
				}
				else if (m_particle_type == particle_bei) {
					int scen = m_proBottleSpec.sideL / 2 + m_proBottleSpec.sideR / 2;
					int slen = m_proBottleSpec.sideL - m_proBottleSpec.sideR;
					slen = slen / 4;
					int shrkL = scen + slen; int shrkR = scen - slen;
					shrkR = shrkR > shrk ? shrkR : shrk;
					imcolrange = imcolrange.rowRange(shrkR, shrkL);
					shrk = shrkR;
				}


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

	uchar ProcParticle::calMatLocMean(cv::Mat _src, int _brd, int _range, bool _bRow, bool _bInc) {
		cv::Mat im;
		CV_Assert(!_src.empty());
		CV_Assert(_brd > 0 && _range > 0);
		if (true == _bRow)
			CV_Assert(_brd < _src.rows);
		else
			CV_Assert(_brd < _src.cols);
		if (true == _bRow && true == _bInc) {
			im = _src.clone().rowRange(_brd, _brd + _range < _src.rows ? _brd + _range : _src.rows - 1);
		}
		else if (true == _bRow && false == _bInc) {
			im = _src.clone().rowRange(_brd - _range > 0 ? _brd - _range : 0, _brd);
		}
		else if (false == _bRow && true == _bInc) {
			im = _src.clone().colRange(_brd, _brd + _range < _src.cols ? _brd + _range : _src.cols - 1);
		}
		else if (false == _bRow && false == _bInc) {
			im = _src.clone().colRange(_brd - _range > 0 ? _brd - _range : 0, _brd);
		}
		cv::reduce(im, im, 0, CV_REDUCE_AVG);
		cv::reduce(im, im, 1, CV_REDUCE_AVG);
		return im.at<uchar>(0, 0);
	}

	void ProcParticle::calMatLocMeanRange_beta(cv::Mat _src, cv::Mat& _ref, int _brd, bool _bRow, bool _bInc, int range)
	{
		CV_Assert(_src.size() == _ref.size());
		cv::Mat rr;
		cv::Mat vref;
		if (_bRow) {
			vref = cv::Mat_<uchar>(1, _src.cols);
			for (int x = 0; x < _src.cols; x++) {
				vref.at<uchar>(0, x) = calMatLocMean(_src.col(x), _brd, range, _bRow, _bInc);
			}
			rr = cv::repeat(vref, _src.rows, 1);
		}
		else {
			vref = cv::Mat_<uchar>(_src.rows, 1);
			for (int y = 0; y < _src.rows; y++) {
				vref.at<uchar>(y, 0) = calMatLocMean(_src.row(y), _brd, range, _bRow, _bInc);
			}
			rr = cv::repeat(vref, 1, _src.cols);
		}
		CV_Assert(rr.size() == _ref.size());
		rr.copyTo(_ref);

	}

	vector<cv::Point> ProcParticle::findSideVec_beta(cv::Mat _src, cv::Mat _ref, int _brd, bool _bInv, bool _bRow, bool _bInc) {
		/*vector<int> vst;*/
		vector<cv::Point> vst;
		cv::Mat diff;
		cv::Mat binary;
		int thres(0);
		if (m_particle_type == particle_di)
			thres = 5;
		else
			thres = 20;

		if (_bInv)
			diff = _ref - _src;
		else
			diff = _src - _ref;
		cv::threshold(diff, binary, thres, 255, CV_THRESH_BINARY);
		if (_bRow) {
			for (int x = 0; x < diff.cols; x++) {
				cv::Mat mdr = diff.col(x);
				if (_bInc) {
					for (int y = _brd; y < diff.rows; y++)
						if (binary.at<uchar>(y, x) == 255) {
							/*vst.push_back(y);*/
							vst.push_back(cv::Point(x, y));
							break;
						}

				}
				else {
					for (int y = _brd; y > 0; y--)
						if (binary.at<uchar>(y, x) == 255) {
							/*vst.push_back(y);*/
							vst.push_back(cv::Point(x, y));
							break;
						}

				}
			}
		}
		else {
			for (int y = 0; y < diff.rows; y++) {
				cv::Mat mdr = diff.row(y);
				if (_bInc) {
					for (int x = _brd; x < diff.cols; x++)
						if (binary.at<uchar>(y, x) == 255) {
							/*vst.push_back(x);*/
							vst.push_back(cv::Point(x, y));
							break;
						}

				}
				else {
					for (int x = _brd; x > 0; x--)
						if (binary.at<uchar>(y, x) == 255) {
							/*vst.push_back(x);*/
							vst.push_back(cv::Point(x, y));
							break;
						}

				}
			}
		}
		return vst;
	}


	int ProcParticle::findSidePoint_beta(cv::Mat _src, cv::Mat _ref, int _brd, bool _bInv, bool _bRow, bool _bInc, int _method, cv::Point2f& _lines)
	{
		vector<cv::Point> vst;
		const size_t nsz = 5;
		vst = findSideVec_beta(_src, _ref, _brd, _bInv, _bRow, _bInc);
		if (vst.size() < nsz)
			return -1;
		switch (_method)
		{
		case AP_FINDSIDE_FIT:
		{
			vector<int > vint;
			for (size_t i = 0; i < vst.size(); i++)
			{
				vint.push_back(vst[i].y);
			}
			sortVal(vint);
			/*int mea =  vint.at(vst.size() / 2);*/
			int mea(0);
			int ran(6); // 边缘倾斜范围
			if (m_particle_type == particle_di) {
				ran = 6;
				if (!_bInc)
					mea = *(vint.begin() + nsz - 1);
				else
					mea = *(vint.rbegin() + nsz - 1);
			}
			else {
				ran = 12;
				mea = vint.at(vst.size() / 2);
			}


			vector<cv::Point> vstN;
			for (size_t i = 0; i < vst.size(); i++)
			{
				if (abs(vst[i].y - mea) < 6)
				{
					vstN.push_back(vst[i]);
				}
			}

			cv::Vec4f sline;
			fitLine(vstN, sline, CV_DIST_L2, 0, 0.01, 0.01);
			_lines.x = sline[1] / sline[0];
			_lines.y = sline[3] - sline[1] * sline[2] / sline[0];
			int xr = _src.cols;
			float yr = sline[1] / sline[0] * (xr - sline[2]) + sline[3];
			int yri = yr;
			return yri;
		}
		break;
		case AP_FINDSIDE_MED:
		{
			vector<int > vint;
			for (size_t i = 0; i < vst.size(); i++)
			{
				vint.push_back(vst[i].y);
			}
			sortVal(vint);
			return vint.at(vst.size() / 2);
		}
		break;
		default:
			break;
		}

	}

	/*int ProcParticle::findSide_beta(cv::Mat _src, cv::Mat _ref, int _brd,bool _bInv, bool _bRow, bool _bInc)
	{

		cv::Mat diff;
		cv::Mat binary;
		int thres(15);
		if (_bInv)
			diff = _ref - _src;			
		else 
			diff = _src - _ref;
		cv::threshold(diff, binary, thres, 255, CV_THRESH_BINARY);
		vector<int> vst;
		if (_bRow  ) {
			for (int x = 0; x < diff.cols; x++) {
				cv::Mat mdr = diff.col(x);
				if (_bInc) {
					for (int y = _brd; y < diff.rows; y++)
						if (binary.at<uchar>(y, x) == 255) {
							vst.push_back(y);
							break;
						}
							
				}
				else {
					for (int y = _brd; y > 0; y--)
						if (binary.at<uchar>(y, x) == 255) {
							vst.push_back(y);
							break;
						}
							
				}
			}
		}
		else  {
			for (int y = 0; y < diff.rows; y++) {
				cv::Mat mdr = diff.row(y);
				if (_bInc) {
					for (int x = _brd; x < diff.cols; x++)
						if (binary.at<uchar>(y, x) == 255) {
							vst.push_back(x);
							break;
						}
							
				}
				else {
					for (int x = _brd; x > 0; x--)
						if (binary.at<uchar>(y, x) == 255) {
							vst.push_back(x);
							break;
						}
							
				}
			}
		}
		if (vst.size() > 3) {
			sortVal(vst);
			return vst.at(vst.size() / 2);
		}
		else
			return -1;
	}*/

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
		cv::threshold(im, imbinH, 80, 255, CV_THRESH_BINARY);
		cv::threshold(im, imbinL, 200, 255, CV_THRESH_BINARY_INV);
		cv::bitwise_and(imbinH, imbinL, imbin);
		double cntBin = cv::countNonZero(imbin);
		double cntTotal = imbin.total();
		double ratioCnt = cntBin / cntTotal;
		if (ratioCnt > 0.3)
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
		m_proBottleSpec.m_passNum = 1;
	}

	bool ProcParticle::paraParticleInit()
	{
		m_roiBorder = cv::Rect();
		m_proBottleSpec.m_lines[0] = cv::Point2f(0, 0);
		m_proBottleSpec.m_lines[1] = cv::Point2f(0, 0);
		m_skip_cnt = 1;
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
					if (abs(true == vt_bottle_base[i].valid && vt_bottle_base[i].len - mea_len) <= m_prePara.ptSft.y)
						vt_cen.push_back(vt_bottle_base[i].cen);
					else
						vt_bottle_base[i].valid = false;
				}				
				if (vt_cen.size() > fmin) {
					sortVal(vt_cen);
					mea_cen = vt_cen[vt_cen.size() / 2];
					for (int i = 0; i < vt_bottle_base.size(); i++) {
						if (true == vt_bottle_base[i].valid &&
							abs(vt_bottle_base[i].cen - mea_cen) > m_prePara.ptSft.y )
							vt_bottle_base[i].valid = false;
					}
					for (int i = 0; i < vt_bottle_base.size(); i++) {
						if (vt_bottle_base[i].valid == true)
							fpsValid++;
					}
				}
				if (m_particle_type == particle_di)  // 锦州奥鸿
				{
					if (mea_len < 50)
						fpsValid = 0; // 间距太小
				}
			}
			if (fpsValid > fmin) {
				m_proBottleSpec.cen = mea_cen;
				m_proBottleSpec.len = mea_len;
				m_proBottleSpec.sideR = m_proBottleSpec.cen - m_proBottleSpec.len / 2;
				m_proBottleSpec.sideL = m_proBottleSpec.cen + m_proBottleSpec.len / 2;
				m_procReg.okFindSide = true;
				m_procReg.okRegSide = true;

				vector< float > kup(0), kdwn(0), bup(0), bdwn(0);
				for (int i = 0; i < vt_bottle_base.size(); i++) {
					if (vt_bottle_base[i].valid == true) {
						kup.push_back(vt_bottle_base[i].m_lines[0].x);
						bup.push_back(vt_bottle_base[i].m_lines[0].y);
						kdwn.push_back(vt_bottle_base[i].m_lines[1].x);
						bdwn.push_back(vt_bottle_base[i].m_lines[1].y);
					}
				}
				sortVal(kup);
				sortVal(kdwn);
				sortVal(bup);
				sortVal(bdwn);
				m_proBottleSpec.m_lines[0] = cv::Point2f(kup[kup.size() / 2], bup[bup.size() / 2]);
				m_proBottleSpec.m_lines[1] = cv::Point2f(kdwn[kdwn.size() / 2], bdwn[bdwn.size() / 2]);

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
		if (false) {
			//cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR),
			//	Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL));
			cv::Rect rtRegOrg = cv::Rect(Point(m_proBottleSpec.bottom - 100, m_proBottleSpec.sideR - 35),
				Point(m_proBottleSpec.bottom + 25, m_proBottleSpec.sideL + 35));
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


		if (false ) {
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
			//dif.setTo(1);// 20200516测试用
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
			int thres_val = m_prePara.thr_level_br;
			int thres_type(-1);
			if (m_particle_type == particle_di) 
				thres_type = THRESH_BINARY_INV;			
			else if (m_particle_type == particle_bei)
				thres_type = THRESH_BINARY;
			else
				;
			cv::Mat imBin;
			cv::threshold(imcur, imBin, thres_val, 255, THRESH_BINARY_INV	);
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
				//for (int y = yLocLow; y <= yLocHigh; y++) {
				//	if (mReduce.ptr<int>(y)[0] > mea / 2 + maxVal / 2) {
				//		border = _roi.tl().y +  y + 2;
				//		break;
				//	}
				//}
				yLocHigh = mReduce.rows - 1;
				for (int y = yLocHigh; y >= yLocLow; y--) {
					if (mReduce.ptr<int>(y)[0] > mea / 2 + maxVal / 2) {
						border = _roi.tl().y + y - 2;
						break;
					}
				}
			}
			else {
				for (int y = yLocHigh; y >= yLocLow; y--) {
					if (mReduce.ptr<int>(y)[0] > mea / 2 + maxVal / 2) {
						border = _roi.tl().y + y + /*-*/ 2;
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
		roiEn = cv::Rect(cv::Point(roiEn.tl().x, roiEn.tl().y - hgtEn), \
			cv::Point(roiEn.br().x + widEn, roiEn.br().y + hgtEn));
		roiEn &= m_roiBorder;
		cv::Mat mtRef;
		maskCanny = cv::Mat::zeros(roiEn.size(), CV_8UC1);
		cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		int inter(4);// 
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
			if (m_particle_type == particle_bei) {
				if(mtRef.empty())
					cv::threshold(imCanny, imCanny, 100, 255, CV_THRESH_BINARY);
				else {
					imCanny = mtRef - imCanny;
					cv::threshold(imCanny, imCanny, 25, 255, CV_THRESH_BINARY);
				}
			}
				
			Canny(imCanny, imCanny, lowThreshold, lowThreshold * ratio, kernel_size);
			cv::dilate(imCanny, imCanny, kn3x3);
			cv::threshold(imCanny, imCanny, 254, 1, THRESH_BINARY);
			maskCanny = maskCanny + imCanny;
		}
		cv::threshold(maskCanny, maskCanny, m_vireg.size() / 2 / inter, 255, THRESH_BINARY);
		cv::dilate(maskCanny, maskCanny, kn3x3);
		cv::threshold(maskCanny, maskCanny, 254, 1, CV_THRESH_BINARY);

		int brX = findCannyBorder(maskCanny,\
			cv::Rect(cv::Point(maskCanny.cols * 2 / 3, 0), cv::Point(maskCanny.cols, maskCanny.rows)),\
			false, false);
		if (brX == -1)
			return m_proBottleSpec.roi;

		int tlY = findCannyBorder(maskCanny,\
			cv::Rect(cv::Point(0, 0), cv::Point(brX - 5, maskCanny.rows / 2)),\
			true, true);
		if (tlY == -1)
			return m_proBottleSpec.roi;

		int brY = findCannyBorder(maskCanny, \
			cv::Rect(cv::Point(0, maskCanny.rows / 2), cv::Point(brX - 5, maskCanny.rows)),\
			true, false);
		if (brY == -1)
			return m_proBottleSpec.roi;

		cv::Rect roiNew = cv::Rect(roiEn.tl() + cv::Point(0, tlY),
			roiEn.tl() + cv::Point(brX, brY));
		roiNew &= m_proBottleSpec.roi;
		if (roiNew.width > 35 /*50*/ && roiNew.height > 50)
			return roiNew;
		else
			return m_proBottleSpec.roi;
	}

	//  通过canny算子得到图像序列中比较复杂区域的mask图像表示，但这样的表示容易将其二值化一分为二。
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
		cv::Rect rt /*= cv::boundingRect(contours.at(idxMaxCon))*/;
		/*int enlargeX(30), enlargeY(10);*/
		int szSecCon = 0;
		int idxSecCon(0);
		for (int con = 0; con < contours.size(); con++) {
			if (szSecCon < contours.at(con).size() && con != idxMaxCon) {
				szSecCon = contours.at(con).size();
				idxSecCon = con;
			}
		}
		cv::Rect rtMax = cv::boundingRect(contours.at(idxMaxCon));
		cv::Rect rtSec = cv::boundingRect(contours.at(idxSecCon));
		if (szSecCon > szMaxCon / 3 && rtSec.br().x > rtMax.br().x)
			rt = rtSec;
		else
			rt = rtMax;
		rt = cv::Rect(	cv::Point(rt.tl().x, rt.tl().y - m_prePara.ptLightSpot.y), 
						cv::Point(rt.br().x + m_prePara.ptLightSpot.x, rt.br().y + m_prePara.ptLightSpot.y));
		rt = rt & cv::Rect(cv::Point() , _roi.size() );
		return rt + _roi.tl();
	}

	bool ProcParticle::ProcGlobal(vector<cv::Mat> & _vmIn/*, VMCollect& _vm_collect*/, vector<cv::Rect>& _rtMask/*cv::Mat _maskBlink*/)
	{
		bool good(true);
		/*return good;*/
		cv::Size szImWhole = _vmIn.at(0).size();
		Mat kn2x1 = getStructuringElement(MORPH_RECT, Size(2, 1));
		Mat kn1x2 = getStructuringElement(MORPH_RECT, Size(1, 2));
		cv::Mat kn3x1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1));
		cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat kn5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::Mat kn3x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
		cv::Mat maskCanny = cv::Mat::zeros(szImWhole,CV_8UC1);
		maskCanny.setTo(255);
		if (false/*m_particle_type == particle_bei*/) {
			/*m_proBottleSpec.roi = CalNewRoi(_vmIn);*/
			//int tlx = m_proBottleSpec.roi.tl().x > m_proBottleSpec.scanBorder ?\
			//	m_proBottleSpec.roi.tl().x : m_proBottleSpec.scanBorder;
			//m_proBottleSpec.roi = cv::Rect(cv::Point(tlx, m_proBottleSpec.roi.tl().y) , m_proBottleSpec.roi.br() ); 
		}			
		else if (false/*m_particle_type == particle_di*/) {
			maskCanny = mAccumulateCanny(_vmIn, kn3x3);  // 暂时不适用canny算子，防止超时
			cv::erode(maskCanny, maskCanny, kn3x5);
		}
			
		
		if( NULL == palg)
			palg = new BackgroundSubtrator(m_particle_type, (m_proBottleSpec.roiOrigin.width )* m_proBottleSpec.roiOrigin.height); 
		/*BackgroundSubtrator* palg = new BackgroundSubtrator(m_para.sensity, m_particle_type);*/
		/*BackgroundSubtrator* palgLevel = new BackgroundSubtrator(m_para.sensity, m_particle_type);*/

		cv::Mat mtDyLast;
		vector<DatIdx> vecDatIdx;
		int ShiftErrIdx(-1);
		/*vecDatIdx.resize(m_vireg.size());*/
		//int reNum(1);
		//if (m_particle_type == particle_bei)
		//	reNum = 6;
		int imNum = 0;
		if (m_particle_type == particle_bei)
			/*imNum = 8;*/imNum = m_vireg.size();
		else
			imNum = m_vireg.size();

		cv::Mat mkcrn = cv::Mat::zeros(m_proBottleSpec.roi.size(), CV_8UC1);
		vector<cv::Point> vpt;
		vpt.push_back(m_proBottleSpec.roiCorner[0] - m_proBottleSpec.roi.tl());
		vpt.push_back(m_proBottleSpec.roiCorner[1] - m_proBottleSpec.roi.tl());
		vpt.push_back(m_proBottleSpec.roiCorner[2] - m_proBottleSpec.roi.tl());
		vpt.push_back(m_proBottleSpec.roiCorner[3] - m_proBottleSpec.roi.tl());
		vector< vector< cv::Point> > vvpt;
		vvpt.push_back(vpt);
		cv::drawContours(mkcrn, vvpt, 0, cv::Scalar::all(255), -1);
		cv::bitwise_not(mkcrn, mkcrn);
		cv::Mat srctmp;
		vector<bool> vb_fg;

		cv::Point ptCornerRight(m_proBottleSpec.roi.br().x, m_proBottleSpec.roi.tl().y);
		cv::Rect rtMaskRight = cv::Rect(
			ptCornerRight + cv::Point(-m_prePara.maskCorner_right_x, m_prePara.maskCorner_right_y) + cv::Point(-m_prePara.maskCorner_right_width, 0),
			cv::Size(m_prePara.maskCorner_right_width, m_prePara.maskCorner_right_height)
		);
		rtMaskRight = rtMaskRight & m_proBottleSpec.roi;
		cv::Point ptCornerLeft = m_proBottleSpec.roi.br();
		cv::Rect rtMaskLeft = cv::Rect(
			ptCornerLeft - cv::Point(m_prePara.maskCorner_left_width, m_prePara.maskCorner_left_height) - cv::Point(m_prePara.maskCorner_left_x, m_prePara.maskCorner_left_y),
			cv::Size(m_prePara.maskCorner_left_width, m_prePara.maskCorner_left_height)
		);
		rtMaskLeft = rtMaskLeft & m_proBottleSpec.roi;
		_rtMask.push_back(rtMaskRight);
		_rtMask.push_back(rtMaskLeft);
		cv::Mat maskBlink = cv::Mat::zeros(szImWhole, CV_8UC1);
		//for (vector<cv::Rect>::iterator it = _rtMask.begin();
		//	it != _rtMask.end(); it++) {
		//	cv::rectangle(maskBlink, *it, cv::Scalar(255), -1);
		//}

		for (int i = 0; i < imNum /*m_vireg.size()*/; i++) {
			int idx = m_vireg[i].index;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			/*roi &= cv::Rect(cv::Point(2, 2), cv::Point(szImWhole.width, szImWhole.height ));*/
			cv::Mat src = _vmIn[idx]( roi + cv::Point(i % 2, 0)  );  //20200516注释
			srctmp = src;
			//cv::Mat src = _vmIn[idx]( roi + cv::Point(i % 3 -1, 0)  );   // 20200516 添加
			m_vmCollect.vmMask[idx](roi).setTo(0, mkcrn);
			cv::Mat mask = m_vmCollect.vmMask[idx](roi).clone();
			cv::dilate(mask, mask, kn3x3);
			
			if (mtDyLast.empty() == true)
				mtDyLast = cv::Mat::zeros(roi.size(), CV_8UC1);

			bool needToInitialize(false);
			if (0 == i)
				needToInitialize = true;
			palg->buildmodel(src, mask, true, needToInitialize, m_prePara.bg /*m_para.sensity*/);
			cv::Mat mtDyDiff  = palg->mdata.mtDy - mtDyLast;
			int cntDyDiff = cv::countNonZero(mtDyDiff.colRange(mtDyDiff.cols/2, mtDyDiff.cols) );
			DatIdx mdatIdx = { cntDyDiff, idx };
			vecDatIdx.push_back(mdatIdx);

			for (vector<cv::Rect>::iterator it = _rtMask.begin();
				it != _rtMask.end(); it++) {
				cv::Rect ri = *it - roi.tl();
				ri = ri & cv::Rect(cv::Point(), roi.size());
				cv::Mat imdf = mtDyDiff( ri );
				if (cv::countNonZero(imdf))
					maskBlink(*it) += 1;
			}

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
		cv::threshold(maskBlink, maskBlink, 0, 255, CV_THRESH_BINARY);
		sortDatIdx(vecDatIdx);
		ShiftErrIdx = vecDatIdx.begin()->idx -1 ;
		//if (vecDatIdx.begin()->val > (vecDatIdx.begin() + vecDatIdx.size() / 2)->val + 100)
		//	ShiftErrIdx = vecDatIdx.begin()->idx;

		//根据模态的变化程度来调整前背景判决阈值,仅针对主检测区域
		/*cv::Mat mtDyFew = palg->mdata.mtDy.clone();*/
		cv::Mat mtWeightDynamic = palg->mdata.mtDy.clone();
		cv::dilate(mtWeightDynamic, mtWeightDynamic, kn5x5);
		cv::threshold(mtWeightDynamic, mtWeightDynamic, 2, 255, THRESH_TOZERO);
		cv::Mat maskDySimple = mtWeightDynamic.clone();
		cv::threshold(maskDySimple, maskDySimple, 2, 255, THRESH_BINARY_INV);
		mtWeightDynamic.convertTo(mtWeightDynamic, CV_32F);
		
		float varWeight =  m_prePara.var_weight/*100*/;
		varWeight = varWeight > 4 ? varWeight : 4;
		float varAdd = 1. - 2. / varWeight;
		if (m_particle_type == particle_di)
			mtWeightDynamic = mtWeightDynamic / varWeight + varAdd;
		else {
			mtWeightDynamic = mtWeightDynamic / varWeight + varAdd;
		}

		//if (m_particle_type == particle_di)
		//	mtWeightDynamic = mtWeightDynamic / 4. + 0.5;
		//else {
		//	mtWeightDynamic = mtWeightDynamic / 4. + 0.5;
		//}

		mtWeightDynamic.setTo(1., maskDySimple);
		//mtWeightDynamic.setTo(1.); // 
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
		cv::Mat mtBgDy = palg->mdata.mtDy.clone();	
		if (  false /*m_particle_type == particle_di *//*m_para.ratio < 0.65*/ ) {
			cv::threshold(mtBgDy, mtBgDy, 3, 255, CV_THRESH_BINARY);
			cv::dilate(mtBgDy, mtBgDy, kn5x5);
		}
		else
		{
			cv::threshold(mtBgDy, mtBgDy, int(filter_dynamic_thr), 255, CV_THRESH_BINARY);
			cv::dilate(mtBgDy, mtBgDy, kn3x3);
		}

		cv::Mat mtBgDyDwn = mtBgDy.clone();
		mtBgDyDwn.colRange(0, mtBgDyDwn.cols / 2).setTo(0);
		vector<vector<cv::Point> > contours;
		cv::findContours(mtBgDyDwn.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (size_t i = 1; i < contours.size(); i++)
			if (contours[i - 1].size() > contours[i].size())
				std::swap(contours[i - 1], contours[i]);
		cv::Mat mtBgDyDwnBig = cv::Mat::zeros(mtBgDy.size(), CV_8UC1);
		cv::drawContours(mtBgDyDwnBig, contours, contours.size() - 1, cv::Scalar::all(255), -1);



		//cv::findContours(mtBgDy.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//for (size_t i = 1; i < contours.size(); i++)
		//	if (contours[i - 1].size() > contours[i].size())
		//		std::swap(contours[i - 1], contours[i]);
		cv::Mat mtBgDySmall = cv::Mat::zeros(mtBgDy.size(), CV_8UC1);
		mtBgDy.copyTo(mtBgDySmall);

		if(paramode == 1 && m_particle_type == particle_di	&& mtBgDyDwnBig.rows > 102	& mtBgDyDwnBig.cols > 42){
			cv::Size mz = mtBgDyDwnBig.size();
			cv::Mat mt = mtBgDyDwnBig.colRange(mz.width - 42, mz.width).rowRange(mz.height / 2 - 50, mz.height / 2 + 50).setTo(255);
		}

		mtBgDySmall.setTo(0, mtBgDyDwnBig);
		palg->mdata.mtDyPost.setTo(4., mtBgDyDwnBig);// 20200607 ceshi
		//if (m_particle_type == particle_bei) {
		//	mtBgDyDwnBig.setTo(0);
		//	mtBgDySmall.setTo(0);
		//	mtBgDyDwnBig.colRange(mtBgDyDwnBig.cols - 45, mtBgDyDwnBig.cols).setTo(255);
		//	palg->mdata.mtDyPost.setTo(1., mtBgDyDwnBig);// 20200607 ceshi
		//}
		//for (int i = 0; i < (int)contours.size() - 1 ; i++)
		//	cv::drawContours(mtBgDySmall, contours, i, cv::Scalar::all(255), -1);
		
		/*cv::Rect rtDynamicBig = cv::boundingRect(contours.at(contours.size() - 1));*/

		// 20200508 commnet
		//if (m_particle_type == particle_di)
		//	palg->mdata.mtDyPost.setTo(1., mtBgDyDwnBig);

		/*int DyEordeSize = 5;*/
		int DyEordeSize = m_prePara.DyEordeSize > 0 ? m_prePara.DyEordeSize : 1;
		if (m_particle_type == particle_bei)
			DyEordeSize = 1;
		//else
		//	DyEordeSize = 1;
		cv::Mat knxn = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(DyEordeSize, DyEordeSize));
		if(m_prePara.DyEordeSize> 0 )
			cv::dilate(mtBgDySmall, mtBgDySmall, knxn);

		int dyL(-1), dyR(-1);
		findDynamicBorder(mtBgDy, dyL, dyR);
		//if (dyL != -1) {
		//	mtBgDy.colRange(dyL, mtBgDy.cols).setTo(255);
		//	palg->mdata.mtDyPost.colRange(dyL, mtBgDy.cols).setTo(255); // 动态区域设置最大变化值
		//}
		mtBgDy.copyTo(palg->mdata.mtDy);
		palg->mdata.mtDy.setTo(0);
		if (m_particle_type == particle_di) {
			// cv::bitwise_or(mtBgDy, mtBgDySmall, mtBgDy);  // 20200508 comment
			mtBgDySmall.copyTo(mtBgDy);// 20200508 comment
		}
		cv::Mat mtMaxdl;
		palg->getMaxModel(4, 100, mtMaxdl);
		cv::threshold(mtMaxdl, mtMaxdl, 100, 255, CV_THRESH_BINARY);
		/*cv::Mat cd1, cd2;*/
		/*cv::dilate(mtMaxdl, cd1, kn3x3, cv::Point(-2,-1) );*/
		cv::dilate(mtMaxdl, mtMaxdl, kn2x1);
		cv::dilate(mtMaxdl, mtMaxdl, kn2x1);
		cv::dilate(mtMaxdl, mtMaxdl, kn3x3);

		// 暂时对背光的底部检测提高检测阈值
		//if (m_particle_type == particle_bei) {
		//	mtWeightDynamic.setTo(2.5, mtBgDy);
		//	palg->mdata.mtDyPost = 1.;
		//}
			



		//用于将canny运算后得到的mask区域，在最终结果中显示出来。该区域将不被检测
		if ( true/*m_particle_type == particle_di*/) {
			//cv::Mat& sumDebug = (*m_vmCollect.vmDebug.rbegin())(m_proBottleSpec.roiAll);
			//cv::Mat fgCanny; cv::bitwise_not(maskCanny(m_proBottleSpec.roiAll), fgCanny);
			//fgCanny.copyTo(sumDebug);

			// comment 20200530 
			cv::Mat& sumDebug = (*m_vmCollect.vmDebug.rbegin())(m_proBottleSpec.roi);
			mtBgDyDwnBig.copyTo(sumDebug);
			cv::bitwise_or(sumDebug, mtBgDySmall, sumDebug);

			// add 20200530
			//cv::Mat& sumDebug = (*m_vmCollect.vmDebug.rbegin());
			//maskBlink.copyTo(sumDebug);


			//CV_Assert(sumDebug.size() == palg->mdata.mtSum.size());
			//palg->mdata.mtSum.copyTo(sumDebug);
			//cv::threshold(sumDebug, sumDebug, 1, 255, CV_THRESH_BINARY);
			/*cv::threshold(mtDyFew, mtDyFew, 2, 255, CV_THRESH_BINARY_INV);
			cv::erode(mtDyFew, mtDyFew, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));*/

		}

		//cv::Point ptCornerRight(m_proBottleSpec.roi.br().x, m_proBottleSpec.roi.tl().y);
		//cv::Rect rtMaskRight = cv::Rect(
		//	ptCornerRight + cv::Point(-m_prePara.maskCorner_right_x, m_prePara.maskCorner_right_y) + cv::Point(-m_prePara.maskCorner_right_width, 0),
		//	cv::Size(m_prePara.maskCorner_right_width, m_prePara.maskCorner_right_height)
		//);
		//rtMaskRight = rtMaskRight & m_proBottleSpec.roi;
		//cv::Point ptCornerLeft = m_proBottleSpec.roi.br();
		//cv::Rect rtMaskLeft = cv::Rect(
		//	ptCornerLeft - cv::Point(m_prePara.maskCorner_left_width, m_prePara.maskCorner_left_height) - cv::Point(m_prePara.maskCorner_left_x, m_prePara.maskCorner_left_y),
		//	cv::Size(m_prePara.maskCorner_left_width,m_prePara.maskCorner_left_height)
		//);
		//rtMaskLeft = rtMaskLeft & m_proBottleSpec.roi;
		//_rtMask.push_back(rtMaskRight);
		//_rtMask.push_back(rtMaskLeft);
		//cv::Mat maskBlink = cv::Mat::zeros(szImWhole, CV_8UC1);
		//for (vector<cv::Rect>::iterator it = _rtMask.begin();
		//	it != _rtMask.end(); it++) {
		//	cv::rectangle(maskBlink, *it, cv::Scalar(255), -1);
		//}


		// 主区域检测
		cv::Mat sumfgk = (*m_vmCollect.vmFg.rbegin())(m_proBottleSpec.roi);
		//if (m_particle_type == particle_bei)
		//	imNum = imNum / 4;
		for (int i = 0; i <  /*m_vireg.size()*/ imNum; i++) {
			int idx = m_vireg[i].index;
			//if (ShiftErrIdx == idx)
			//	continue;
			cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			cv::Mat src = _vmIn[idx](roi);
			/*cv::Mat mkBlink = _maskBlink(roi).clone();*/

			cv::Mat mkBlink = maskBlink(roi).clone();

			cv::Mat fgkall = m_vmCollect.vmFg[idx](roi);
			
			cv::Rect roiLightSpot = cv::Rect(
				cv::Point(m_proBottleSpec.levelTopLimit, m_proBottleSpec.sideR),
				cv::Point(m_proBottleSpec.levelDetect + 20 < szImWhole.width ? m_proBottleSpec.levelDetect + 20 : szImWhole.width, m_proBottleSpec.sideL));
			if (m_particle_type == particle_di)
				roiLightSpot = FilterLigtSpot(_vmIn[idx], roiLightSpot);
			else
				roiLightSpot = cv::Rect();
			m_vmCollect.vRoiLightSpot[idx] = roiLightSpot;
			cv::Mat maskWhole = m_vmCollect.vmMask[idx];
			cv::rectangle(maskWhole, roiLightSpot, cv::Scalar::all(0), -1);
			cv::Mat mask = maskWhole(roi).clone();
			
			if ( true /*m_particle_type == particle_di*/ ) {

				//cv::Mat maskDynamic;
				//if (m_prePara.DyEordeSize > 0)
				//	cv::bitwise_not(mtBgDySmall, maskDynamic);
				//else
				//	cv::bitwise_not(mtBgDy, maskDynamic);
				//cv::bitwise_and(maskDynamic, mask, mask);  // 哈药双黄连暂时注释

				if (m_prePara.DyEordeSize > 0)
					cv::bitwise_and(~mtBgDySmall, mask, mask);
				else
					cv::bitwise_and(~mtBgDy, mask, mask);
			}
			
			//cv::bitwise_and(maskDynamic.colRange(0, mask.cols / 3), mask.colRange(0, mask.cols / 3), mask.colRange(0, mask.cols / 3));
			/*cv::Mat mask = m_vmCollect.vmMask[idx](roi).clone();*/

			if (paramode != 1) {
				cv::bitwise_not(mkBlink, mkBlink);
				cv::bitwise_and(mkBlink, mask, mask);
			}

			//if (m_particle_type == particle_bei) {
			//	cv::bitwise_and(mtBgDyDwnBig, mask, mask);
			//}

			//if (m_particle_type == particle_di)
			//	cv::bitwise_and(mask, maskCanny(roi), mask);
			fillBorder(mask, 3, 0);
			//if (dyR > -1)
			//	mask.colRange(dyR, mask.cols).setTo(0);
			palg->mdata.particle_type = m_particle_type; // 返回检测的粒子类型
			/*if(i == 12)*/
			palg->justify(src, fgkall, mask);  
			//if (m_particle_type == particle_bei) {
			//	fgkall.setTo(0);
			//	if (i > 0 && i < imNum-1) {
			//		int idx = m_vireg[i].index;
			//		cv::Rect roi = m_proBottleSpec.roi + m_vireg[i].pt;
			//		cv::Mat src = _vmIn[idx](roi);
			//		 idx = m_vireg[i-1].index;
			//		 roi = m_proBottleSpec.roi + m_vireg[i-1].pt;
			//		cv::Mat ref1 = _vmIn[idx](roi);
			//		 idx = m_vireg[i+1].index;
			//		 roi = m_proBottleSpec.roi + m_vireg[i+1].pt;
			//		cv::Mat ref2 = _vmIn[idx](roi);
			//		//cv::bitwise_not(mtDyFew, mtDyFew);
			//		cv::Mat dif1 = ref1 - src;
			//		cv::threshold(dif1, dif1, 6, 255, THRESH_BINARY);
			//		cv::bitwise_and(dif1, mtDyFew, dif1);
			//		cv::dilate(dif1, dif1, kn3x3);
			//		cv::Mat dif2 = ref2 - src;
			//		cv::threshold(dif2, dif2, 9, 255 ,THRESH_BINARY);
			//		cv::bitwise_and(dif2, mtDyFew, dif2);
			//		cv::dilate(dif2, dif2, kn3x3);
			//		cv::Mat dif;
			//		cv::bitwise_and(dif1, dif2, dif);
			//		dif.copyTo(fgkall);
			//	}
			//}
			
			//fgkall.setTo(0);
			//int numDiff(1);
			//if (i - numDiff >= 0) {
			//	cv::Mat imRef = _vmIn[m_vireg[i - numDiff].index](m_proBottleSpec.roi)(rtDynamicBig);
			//	cv::Mat imCur = _vmIn[m_vireg[i ].index](m_proBottleSpec.roi)(rtDynamicBig);
			//	cv::Mat diff = imRef - imCur;
			//	cv::threshold(diff, diff, 20, 255, CV_THRESH_BINARY);
			//	cv::bitwise_or(fgkall(rtDynamicBig), diff, fgkall(rtDynamicBig));
			//	diff = imCur - imRef ;
			//	cv::threshold(diff, diff, 20, 255, CV_THRESH_BINARY);
			//	cv::bitwise_or(fgkall(rtDynamicBig), diff, fgkall(rtDynamicBig));
			//}

			cv::Mat fgkallAbs;
			cv::threshold(fgkall, fgkallAbs, 254, 255, THRESH_BINARY);
			cv::threshold(fgkall, fgkall, 125, 255, THRESH_BINARY);

			// 暂时注释，提高检出率， 20200524，为仁和义康做准备，该公司的1ml废品率太高
			cv::Mat fgkx, fgky;
			cv::erode(fgkall, fgkx, kn2x1);
			cv::erode(fgkall, fgky, kn1x2);
			cv::bitwise_or(fgkx, fgky, fgkall);	

			cv::bitwise_or(fgkallAbs, fgkall, fgkall);
			if (m_particle_type == particle_di)
			{	
				cv::Mat fgMtBgDyDwnBig = cv::Mat::zeros(mask.size(), CV_8UC1);
				cv::Mat mask_sumModel = palg->mdata.mtSum;
				cv::threshold(mask_sumModel, mask_sumModel, 12, 255, CV_THRESH_BINARY);
				cv::dilate(mask_sumModel, mask_sumModel, kn3x3);
				cv::Mat fgk_sumModel;
				fgkall.copyTo(fgk_sumModel, mask_sumModel);
				cv::erode(fgk_sumModel, fgk_sumModel, kn2x1);
				fgk_sumModel.copyTo(fgkall, mask_sumModel);
				fgkall.setTo(0, mtMaxdl);
				fgkall.copyTo(fgMtBgDyDwnBig, mtBgDyDwnBig);

				int kncnt(0);
				if (paramode == 1) {
					cv::Mat mtr = fgkall.clone();
					cv::erode(mtr, mtr, kn1x2);
					mtr.copyTo(fgkall, mkBlink);
					if (i < 5)
						kncnt = 1;
				}
				/*int knDyTimes = 1;*/
				/*m_prePara.knDyTimes = 3;*/

				// 20200505 comment 
				for (int knt = 0; knt < m_prePara.knDyTimes + kncnt; knt++) {
					cv::erode(fgMtBgDyDwnBig, fgMtBgDyDwnBig, kn2x1);
				}
					

				fgMtBgDyDwnBig.copyTo(fgkall, mtBgDyDwnBig);

				 //暂时注释，防止超时
				//if (dyL > -1) {
				//	cv::Mat im = src;
				//	cv::Mat fgCur = fgkall.clone();
				//	filterBottomBlink(fgCur, src , dyL);
				//	cv::Mat fgBef = fgkall.clone();
				//	im = _vmIn[m_vireg[i - 3 > 0 ? i - 3 : 0].index](roi);
				//	filterBottomBlink(fgBef, im, dyL);
				//	cv::bitwise_or(fgCur, fgBef, fgkall);
				//}
				//if (cv::countNonZero(fgkall)) {
				//	LightFliter(src, fgkall.clone(), fgkall, m_para.sensity);
				//}
				//暂时注释防止超时	

				if (cv::countNonZero(fgkall) ) {
					int brdRNoDetect(0);
					cv::Mat mmt = m_vmCollect.vmMogId[idx](roi).clone();
					if (idx == 0 && i+1 < m_vireg.size() ) // 防首张有液面反光，要在第二张才显示出来
						mmt = m_vmCollect.vmMogId[m_vireg[i + 1].index](roi).clone();
					
					

					//int rgl(500/*45*/);
					int rgl = m_prePara.rgl;
					rgl = rgl >= src.cols - 1 ? src.cols - 1 : rgl;
					rgl = rgl < 50 ? 50 : rgl;
					if (src.cols > rgl) {
						cv::Mat fkcols = fgkall.colRange(0, rgl);
						vector<vector < cv::Point> > conts;
						cv::findContours(fkcols.clone(), conts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
						int nozr = cv::countNonZero(fkcols);
						if (conts.size() > 3 && nozr > 100 && m_skip_cnt != 0) {
							fgkall.setTo(0);
							m_skip_cnt--;							
						}
						else
							; //bool bNeedReDetect = LightFliter(src.colRange(0, rgl), fgkall.clone().colRange(0, rgl), fgkall.colRange(0, rgl), mmt.colRange(0, rgl), m_para.sensity, brdRNoDetect, 4/*3*/);
					}
					bool bNeedReDetect = LightFliter(src, fgkall.clone(), fgkall, mmt, m_para.sensity, brdRNoDetect,4/*3*/);
					if(bNeedReDetect == true)
						LightFliter(src, fgkall.clone(), fgkall, mmt, m_para.sensity, brdRNoDetect, 13);

					cv::Mat fgkallCols = fgkall.colRange(0, fgkall.cols / 3);
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
				cv::Mat fgMtBgDyDwnBig = cv::Mat::zeros(mask.size(), CV_8UC1);
				fgkall.copyTo(fgMtBgDyDwnBig, mtBgDyDwnBig);
				/*for (int knt = 0; knt < m_prePara.knDyTimes; knt++)
					cv::erode(fgMtBgDyDwnBig, fgMtBgDyDwnBig, kn2x1);*/

				fgMtBgDyDwnBig.copyTo(fgkall, mtBgDyDwnBig);
			}
			else
				;
			int nozero_cur = cv::countNonZero(fgkall);
			
			// 供哈药双黄连测试使用 20200307 ，滤除小规格的误检测
			if (  false /*nozero_cur > 0 && m_particle_type == particle_bei*/) {

				cv::Mat fgbin = cv::Mat::zeros(src.size(), src.type());
				fgbin.setTo(255);
				src.copyTo(fgbin, fgkall);
				cv::threshold(fgbin, fgkall, 254, 255, CV_THRESH_BINARY_INV);
				int colst = fgkall.cols - 50 > 1 ? fgkall.cols - 50 : 1;
				vector<vector<cv::Point> > contours;
				cv::findContours(fgkall.clone()	, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				cv::Mat fgkt = cv::Mat::zeros(fgkall.size(), fgkall.type());
				for (size_t i = 0; i < contours.size(); i++) {
					int brx = cv::boundingRect(contours[i]).br().x;
					int cnz = brx > colst ? 3 : 45;
					if (cv::contourArea(contours[i]) > cnz)
						cv::drawContours(fgkt, contours, i, cv::Scalar::all(255), -1);
				}
				//cv::Mat fgbin = cv::Mat::zeros(src.size(), src.type());
				//fgbin.setTo(255);
				//src.copyTo(fgbin, fgkt);
				//cv::threshold(fgbin, fgbin, 90, 255, CV_THRESH_BINARY_INV);
				
				cv::Range rt = cv::Range(0,colst);
				fgkt.colRange(rt).copyTo(fgkall.colRange(rt) );
				cv::Range re = cv::Range(colst, fgkt.cols);
				cv::erode(fgkall.colRange(re), fgkall.colRange(re), kn3x1);
				cv::bitwise_or(fgkt.colRange(re), fgkall.colRange(re), fgkall.colRange(re));
				//fgkt.copyTo(fgkall);
				/*fgkall.colRange(re).setTo(0);*/
			}

			// 供哈药蓝钙测试使用，该规格异物的检测灵敏度更高
			//if ( nozero_cur > 0 && m_particle_type == particle_bei) {
			//	mtBgDy;
			//	cv::Mat fgbin = cv::Mat::zeros(src.size(), src.type());
			//	fgbin.setTo(255);
			//	src.copyTo(fgbin, fgkall);
			//	cv::threshold(fgbin, fgbin, 254, 255, CV_THRESH_BINARY_INV);
			//	int colst = fgbin.cols - 50 > 1 ? fgbin.cols - 50 : 1;
			//	vector<vector<cv::Point> > contours;
			//	cv::findContours(fgbin.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			//	cv::Mat fgkt = cv::Mat::zeros(fgbin.size(), fgbin.type());
			//	cv::Mat imdr = cv::Mat::zeros(fgbin.size(), fgbin.type());
			//	for (size_t i = 0; i < contours.size(); i++) {
			//		cv::Rect rt = cv::boundingRect(contours[i]);
			//		int brx = rt.br().x;
			//		int nozero = cv::countNonZero(fgbin(rt) & mtBgDy(rt));
			//		int cnz = nozero ? 5 : 1;
			//		if (cv::contourArea(contours[i]) > cnz) {
			//			cv::drawContours(imdr, contours, i, cv::Scalar::all(255), -1);
			//			if (brx > colst && nozero)
			//				cv::erode(imdr(rt), imdr(rt), kn3x1);
			//			cv::bitwise_or(imdr, fgkt, fgkt);
			//			imdr.setTo(0);
			//		}
			//			
			//	}
			//	fgkt.copyTo(fgkall);
			//}

			// 供哈药蓝钙测试使用，现场实用
			if ( false/*nozero_cur > 0 && m_particle_type == particle_bei*/) {
				mtBgDy;
				cv::Mat fgbinH = cv::Mat::zeros(src.size(), src.type());
				cv::Mat fgbinL = cv::Mat::zeros(src.size(), src.type());
				fgbinH.setTo(255); fgbinL.setTo(255);
				src.copyTo(fgbinH, fgkall);
				src.copyTo(fgbinL, fgkall);
				cv::threshold(fgbinL, fgbinL, 90, 255, CV_THRESH_BINARY_INV);
				cv::threshold(fgbinH, fgbinH, 254, 255, CV_THRESH_BINARY_INV);
				int colst = src.cols - 50 > 1 ? src.cols - 50 : 1;

				vector<vector<cv::Point> > contoursH, contoursL;
				cv::findContours(fgbinL.clone(), contoursL, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				cv::findContours(fgbinH.clone(), contoursH, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				cv::Mat fgkt = cv::Mat::zeros(src.size(), src.type());
				cv::Mat imdr = cv::Mat::zeros(src.size(), src.type());
				for (size_t i = 0; i < contoursL.size(); i++) {
					cv::Rect rt = cv::boundingRect(contoursL[i]);
					int brx = rt.br().x;
					int nozero = cv::countNonZero(fgbinL(rt) & mtBgDy(rt));
					int cnz = nozero ? 2+5 : 2;
					int cnar = cv::countNonZero(fgbinL(rt));
					if (cnar > cnz) {
						cv::drawContours(imdr, contoursL, i, cv::Scalar::all(255), -1);
						if (brx > colst && nozero)
							cv::erode(imdr(rt), imdr(rt), kn3x1);
						cv::bitwise_or(imdr, fgkt, fgkt);
						imdr.setTo(0);
					}

				}

				for (size_t i = 0; i < contoursH.size(); i++) {
					cv::Rect rt = cv::boundingRect(contoursH[i]);
					int brx = rt.br().x;
					int nozero = cv::countNonZero(fgbinH(rt) & mtBgDy(rt));
					int cnz = nozero ? 17 + 5 : 17;
					int cnar = cv::countNonZero(fgbinH(rt) );
					if (/*cv::contourArea(contoursH[i])*/ cnar> cnz) {
						cv::drawContours(imdr, contoursH, i, cv::Scalar::all(255), -1);
						if (brx > colst && nozero)
							cv::erode(imdr(rt), imdr(rt), kn3x1);
						cv::bitwise_or(imdr, fgkt, fgkt);
						imdr.setTo(0);
					}

				}
				fgkt.copyTo(fgkall);
			}

			// 20200229 复宏汉霖临时测试，可降低误检测，客户暂时未用，因为他们更关注漏检
			//if (nozero_cur > 0 && m_skip_cnt > 0) {
			//	
			//	cv::Mat im = fgkall.colRange(0, 20<fgkall.cols ? 20:fgkall.cols);
			//	if (!im.empty() && cv::countNonZero(im) ) {
			//		im.setTo(0);
			//		m_skip_cnt--;
			//	}
			//	if (nozero_cur > 400) {
			//		fgkall.setTo(0);
			//		m_skip_cnt--;
			//	}
			//	vector<vector<cv::Point> > contours;
			//	cv::dilate(fgkall, im, kn5x5);
			//	cv::findContours(im, contours , CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			//	if (contours.size() > 2) {
			//		fgkall.setTo(0);
			//		m_skip_cnt--;
			//	}
			//	nozero_cur = cv::countNonZero(fgkall);
			//}
			
			//// 锦州奥鸿更严格的版本暂时将此注释
			//if (m_particle_type == particle_di) {
			//	int colx = fgkall.cols * 0.4;
			//	cv::Mat fgkcol = fgkall.colRange(0, colx);
			//	cv::erode(fgkcol, fgkcol, kn3x1);  // 锦州奥鸿暂时
			//}

			// 锦州奥鸿更严格的版本暂时将此注释，瞿磊在20200407所使用的版本
			// 星昊暂时注释 20200615
			//if (m_particle_type == particle_di) {
			//	int colx = fgkall.cols * 0.3;
			//	cv::Mat fgkcol = fgkall.colRange(0, colx);
			//	cv::erode(fgkcol, fgkcol, kn2x1);  // 锦州奥鸿暂时
			//}

			nozero_cur = cv::countNonZero(fgkall);

			//临时测试 20200616
			if (m_particle_type == particle_di) {
				// cv::bitwise_or(mtBgDyDwnBig)
				float imsz = mtBgDyDwnBig.cols * mtBgDyDwnBig.rows;
				cv::bitwise_or(mtBgDyDwnBig, mtBgDySmall, mtBgDyDwnBig);
				float nosz = cv::countNonZero(mtBgDyDwnBig);
				if (nosz / imsz > 0.8)
					nozero_cur = 100;
				//else
				//	nozero_cur = 0;

			}

			// 20200330 取消注释
			/*m_para.nozero_single_thr = 17;	*/	



			//if (nozero_cur > 25)
			//	m_para.nozero_frm_sum_thr = 1;
			//else
			//	m_para.nozero_frm_sum_thr = 1 /*2*/; // 锦州奥鸿底光更严格版本使用1，其他情况使用2

			vector< vector< cv::Point> > cnts;
			cv::findContours(fgkall.clone(), cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			if (cnts.size() > 2 && m_proBottleSpec.m_passNum != 0 ) {  // 如果大于该值，则认为可能出现误检测，当前帧跳出
				m_proBottleSpec.m_passNum--;
				fgkall.setTo(0);
				nozero_cur = 0;
			}				
			//else
			//	m_para.nozero_frm_sum_thr = 1 /*2*/; // 锦州奥鸿底光更严格版本使用1，其他情况使用2


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

#include <math.h>
	bool ProcParticle::ProcDefect(vector<cv::Mat>& _vmIn)
	{
		//  没有合适的检测区域，跳过
		if (m_proBottleSpec.roiCrackCorner[0] == cv::Point() &&
			m_proBottleSpec.roiCrackCorner[1] == cv::Point() &&
			m_proBottleSpec.roiCrackCorner[2] == cv::Point() &&
			m_proBottleSpec.roiCrackCorner[3] == cv::Point())
			return true;

		cv::Mat mkcrn = cv::Mat::zeros(m_proBottleSpec.roiCrack.size(), CV_8UC1);
		vector<cv::Point> vpt;
		vpt.push_back(m_proBottleSpec.roiCrackCorner[0] - m_proBottleSpec.roiCrack.tl());
		vpt.push_back(m_proBottleSpec.roiCrackCorner[1] - m_proBottleSpec.roiCrack.tl());
		vpt.push_back(m_proBottleSpec.roiCrackCorner[2] - m_proBottleSpec.roiCrack.tl());
		vpt.push_back(m_proBottleSpec.roiCrackCorner[3] - m_proBottleSpec.roiCrack.tl());
		vector< vector< cv::Point> > vvpt;
		vvpt.push_back(vpt);
		cv::drawContours(mkcrn, vvpt, 0, cv::Scalar::all(255), -1);
		cv::Mat kn5x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 7));
		cv::rectangle(mkcrn, cv::Rect(cv::Point(), mkcrn.size()), cv::Scalar::all(0), 3);
		cv::erode(mkcrn, mkcrn, kn5x7);
		cv::bitwise_not(mkcrn, mkcrn);
		cv::Mat sumfgk = (*m_vmCollect.vmFg.rbegin())(m_proBottleSpec.roiCrack);
		for (int ip = m_vireg.size()-1; ip < m_vireg.size(); ip++) {
			int idx = m_vireg[ip].index;
			cv::Rect roi = m_proBottleSpec.roiCrack + m_vireg[ip].pt;
			cv::Mat src = _vmIn[idx](roi).clone(); 
			//static int cnt = 0;
			//char ch[100];
			//sprintf(ch, "./pic/%04d.png", cnt++);
			//cv::imwrite(ch, src);
			int scale = 1;
			int delta = 0;
			int ddepth = CV_16S;
			cv::Mat mclose = cv::Mat::zeros(src.size(), CV_8UC1);
			GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
			cv::Mat grad_y, abs_grad_y;
			cv::Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
			cv::convertScaleAbs(grad_y, abs_grad_y);
			cv::Mat grad;
			grad = abs_grad_y;
			cv::threshold(grad, grad, 35, 255, CV_THRESH_BINARY);
			cv::Mat binary;
			cv::threshold(src, binary, 110, 255, CV_THRESH_BINARY_INV);
			binary.setTo(0, mkcrn);
			cv::Mat kn3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
			cv::dilate(binary, binary, kn3x3);
			cv::bitwise_and(grad, binary, binary);
			cv::Mat kn7x1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 1));
			cv::dilate(binary, binary, kn7x1);
			vector< vector<cv::Point> > contours;
			cv::findContours(binary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			/*cv::cvtColor(binary, binary, CV_GRAY2BGR);*/
			for (int ic = 0; ic < contours.size(); ic++) {
				cv::Rect rect = cv::boundingRect(contours[ic]);
				//cv::RNG rng(12345);
				//cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				cv::rectangle(mclose, rect, cv::Scalar::all(255), -1);
			}
			contours.clear();
			cv::findContours(mclose.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			for (int ic = 0; ic < contours.size(); ic++) {
				cv::Rect rect = cv::boundingRect(contours[ic]);
				if (rect.width > 45) {
					cv::rectangle(src, rect, cv::Scalar::all(255), 2);
					cv::rectangle(sumfgk, rect, cv::Scalar::all(255), 2);
				}					
				//cv::RNG rng(12345);
				//cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			}
			if (cv::countNonZero(sumfgk) > 0)
				return false;
			else
				return true;
			//cv::Mat mbin_org, mbin_proc; int thres_bin(100);
			//cv::threshold(src, mbin_org, thres_bin, 255, CV_THRESH_BINARY_INV);
			//mbin_org.setTo(0, mkcrn);
			//cv::Mat kn1x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5));
			//cv::erode(mbin_org, mbin_proc, kn1x5);
			//cv::dilate(mbin_proc, mbin_proc, kn1x5);
			//cv::Mat kn3x1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1));
			//cv::dilate(mbin_proc, mbin_proc, kn3x1);
			//cv::Mat mcanny; int thres_canny_low(30); int thres_canny_high(70);
			//cv::Canny(src, mcanny, thres_canny_low, thres_canny_high);
			//mcanny.setTo(0, mkcrn);
			//mcanny.setTo(0, mbin_proc);
			//vector<vector<cv::Point> > contours;
			//cv::findContours(mcanny.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			//cv::Mat imdraw;
			//cv::cvtColor(src, imdraw, CV_GRAY2BGR);
			//cv::RNG rng(12345);
			//for (size_t ic = 0; ic < contours.size(); ic++) {
			//	cv::Scalar clr = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//	cv::drawContours(imdraw, contours, ic, clr,3);
			//	int c_size = contours[ic].size();
			//	int c_arc = cv::arcLength(contours[ic], true);
			//	cv::Rect c_rect_bound = cv::boundingRect(contours[ic]);
			//	cv::Point c_rect_bound_center = c_rect_bound.tl() + c_rect_bound.br();
			//	c_rect_bound_center = cv::Point(c_rect_bound_center.x / 2 + c_rect_bound_center.y / 2);
			//	cv::RotatedRect c_rect_min = cv::minAreaRect(contours[ic]);
			//	cv::rectangle(imdraw, c_rect_bound, clr,3);
			//	Point2f vertices[4];
			//	c_rect_min.points(vertices);
			//	for (int i = 0; i < 4; i++)
			//		line(imdraw, vertices[i], vertices[(i + 1) % 4], clr,3);
			//	std::cout << "c_size is" << c_size << std::endl;
			//	std::cout << "c_arc is" << c_arc << std::endl;
			//	std::cout << "c_rect_bound of center is" << c_rect_bound_center << ", and Size is " << c_rect_bound.size() << std::endl;
			//	std::cout << "c_rect_min of center is" << c_rect_min.center << ", and size is" << c_rect_min.size << std::endl << std::endl;
			//}
			// 二值化；mbin
			//找中心线；mthin
			// mthin膨胀后与运算mbin取反值
		}
		return true;
	}

	void ProcParticle::SplitRegionFuren(/*RectS& _rtS*/)
	{
		
		m_para.ratio = m_para.ratio < 1 ? m_para.ratio : 1;
		m_para.ratio = m_para.ratio > 0.2 ? m_para.ratio : 0.2;
		int rangeDetect = (m_proBottleSpec.bottomDown - m_proBottleSpec.levelDown) * m_para.ratio;
		rangeDetect = rangeDetect > m_prePara.rangeDetectMin ?
			rangeDetect : m_prePara.rangeDetectMin;
		m_proBottleSpec.levelDetect = m_proBottleSpec.bottomDown - rangeDetect;
		m_proBottleSpec.levelDetect = m_proBottleSpec.levelDetect < m_proBottleSpec.scanBorder ? m_proBottleSpec.levelDetect : m_proBottleSpec.scanBorder;
		//m_proBottleSpec.levelDetect = m_proBottleSpec.levelTopLimit;  // 哈药三精检测裂缝，临时测试

		int bottomTest = m_proBottleSpec.bottomDown;
		int rangeTest = (m_proBottleSpec.bottomDown - m_proBottleSpec.levelDown) * 0.75;
		rangeTest = rangeTest > m_prePara.rangeDetectMin ?
			rangeTest : m_prePara.rangeDetectMin;
		 bottomTest = m_proBottleSpec.bottomDown - rangeTest;
		 bottomTest = m_proBottleSpec.bottomDown;

		//int range_tmp = (m_proBottleSpec.bottomDown - m_proBottleSpec.levelDown) * 0.5;
		//bottomTest = m_proBottleSpec.bottomDown - range_tmp;
		 /*bottomTest = m_proBottleSpec.bottomUp - 50;*/
		

		m_proBottleSpec.roi = cv::Rect(
			cv::Point(m_proBottleSpec.levelDetect, m_proBottleSpec.sideR),
			cv::Point(/*m_proBottleSpec.bottomDown*/bottomTest, m_proBottleSpec.sideL));
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

		if (m_proBottleSpec.m_lines[0] != cv::Point2f(0, 0) &&
			m_proBottleSpec.m_lines[1] != cv::Point2f(0, 0)) {
			CV_Assert(m_proBottleSpec.roiCorner.size() == 4);
			m_proBottleSpec.m_lines[0].y += 4;
			m_proBottleSpec.m_lines[1].y -= 4;
			m_proBottleSpec.roiCorner[0] = cv::Point(m_proBottleSpec.roi.tl().x, m_proBottleSpec.m_lines[0].x * m_proBottleSpec.roi.tl().x + m_proBottleSpec.m_lines[0].y);
			m_proBottleSpec.roiCorner[1] = cv::Point(m_proBottleSpec.roi.br().x, m_proBottleSpec.m_lines[0].x * m_proBottleSpec.roi.br().x + m_proBottleSpec.m_lines[0].y);
			m_proBottleSpec.roiCorner[2] = cv::Point(m_proBottleSpec.roi.br().x, m_proBottleSpec.m_lines[1].x * m_proBottleSpec.roi.br().x + m_proBottleSpec.m_lines[1].y);
			m_proBottleSpec.roiCorner[3] = cv::Point(m_proBottleSpec.roi.tl().x, m_proBottleSpec.m_lines[1].x * m_proBottleSpec.roi.tl().x + m_proBottleSpec.m_lines[1].y);
			m_proBottleSpec.roi = cv::boundingRect(m_proBottleSpec.roiCorner);
			m_proBottleSpec.roi &= m_roiBorder;
			m_proBottleSpec.roi &= m_proBottleSpec.roiOrigin;
		}
		else {
			m_proBottleSpec.roiCorner[0] = m_proBottleSpec.roi.tl();
			m_proBottleSpec.roiCorner[1] = cv::Point2f(m_proBottleSpec.roi.br().x, m_proBottleSpec.roi.tl().y);
			m_proBottleSpec.roiCorner[2] = m_proBottleSpec.roi.br();
			m_proBottleSpec.roiCorner[3] = cv::Point2f(m_proBottleSpec.roi.tl().x, m_proBottleSpec.roi.br().y);
		}

		{
			int rangeDetect = m_proBottleSpec.bottomDown - m_proBottleSpec.levelDown ;
			int levelDetect = m_proBottleSpec.bottomDown - rangeDetect;
			int bottomTest  = m_proBottleSpec.bottomUp - 50;
			m_proBottleSpec.roiCrack = cv::Rect(
				cv::Point(levelDetect, m_proBottleSpec.sideR),
				cv::Point(bottomTest, m_proBottleSpec.sideL));
			m_proBottleSpec.roiCrack &= m_roiBorder;
			m_proBottleSpec.roiCrack &= m_proBottleSpec.roiOrigin;

			if (m_proBottleSpec.m_lines[0] != cv::Point2f(0, 0) &&
				m_proBottleSpec.m_lines[1] != cv::Point2f(0, 0)) {
				CV_Assert(m_proBottleSpec.roiCrackCorner.size() == 4);
				m_proBottleSpec.m_lines[0].y += 4;
				m_proBottleSpec.m_lines[1].y -= 4;
				m_proBottleSpec.roiCrackCorner[0] = cv::Point(m_proBottleSpec.roiCrack.tl().x, m_proBottleSpec.m_lines[0].x * m_proBottleSpec.roiCrack.tl().x + m_proBottleSpec.m_lines[0].y);
				m_proBottleSpec.roiCrackCorner[1] = cv::Point(m_proBottleSpec.roiCrack.br().x, m_proBottleSpec.m_lines[0].x * m_proBottleSpec.roiCrack.br().x + m_proBottleSpec.m_lines[0].y);
				m_proBottleSpec.roiCrackCorner[2] = cv::Point(m_proBottleSpec.roiCrack.br().x, m_proBottleSpec.m_lines[1].x * m_proBottleSpec.roiCrack.br().x + m_proBottleSpec.m_lines[1].y);
				m_proBottleSpec.roiCrackCorner[3] = cv::Point(m_proBottleSpec.roiCrack.tl().x, m_proBottleSpec.m_lines[1].x * m_proBottleSpec.roiCrack.tl().x + m_proBottleSpec.m_lines[1].y);
				m_proBottleSpec.roiCrack = cv::boundingRect(m_proBottleSpec.roiCrackCorner);
				m_proBottleSpec.roiCrack &= m_roiBorder;
				m_proBottleSpec.roiCrack &= m_proBottleSpec.roiOrigin;
			}
			else {
				m_proBottleSpec.roiCrackCorner[0] = cv::Point();
				m_proBottleSpec.roiCrackCorner[1] = cv::Point();
				m_proBottleSpec.roiCrackCorner[3] = cv::Point();
			}
		}

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
			vl = 0;
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

	bool ProcParticle::DetectVolumes(cv::Mat _src, VolumesInOut* _pvolumes_inout) {
		_pvolumes_inout->rltLevel = -1;
		_pvolumes_inout->rltBottom = -1;
		_pvolumes_inout->rltVolumes = 100000.;  //初始值很大，默认装量不合格
		_pvolumes_inout->iret = false;

		int level(0);
		Mat im = _src, imRow;
		if (m_particle_type == particle_di)
			threshold(im, imRow, m_prePara.thr_level_br, 1, THRESH_BINARY);
		else if (m_particle_type == particle_bei)
			cv::threshold(im, imRow, m_prePara.thr_level_br, 1, THRESH_BINARY_INV);
		else
			m_errLast;
		reduce(imRow, imRow, 0, CV_REDUCE_SUM, CV_32S);
		imRow.convertTo(imRow, CV_8U);
		threshold(imRow, imRow, m_prePara.thr_level_cnt, 255, THRESH_BINARY);
		int pyr = 3;
		resize(imRow, imRow, Size(imRow.cols / pyr, imRow.rows));
		uchar* ptr = imRow.ptr(0);
		int scMax = m_proBottleSpec.scanBorder / pyr;
		int scMin = m_proBottleSpec.levelTopLimit / pyr;
		for (int scx = scMax; scx > scMin; scx--) {
			if (255 == ptr[scx]) {
				level = scx;
				break;
			}
		}
		if (0 == level)
			return _pvolumes_inout->iret;
		else
			level = pyr * level;
		if (m_particle_type == particle_di)
			return _pvolumes_inout->iret;

		int botm(0)/*, botmUp*/; // 测试下边缘所在位置
		cv::Mat imrow;
		im = _src.clone().colRange(m_proBottleSpec.scanBorder, m_proBottleSpec.bottomDown);
		if (m_particle_type == particle_di)
			threshold(im, im, m_prePara.thr_bottom_br, 1, THRESH_BINARY);
		else if (m_particle_type == particle_bei)
			cv::threshold(im, im, m_prePara.thr_bottom_br, 1, THRESH_BINARY_INV);
		else
			;
		reduce(im, imrow, 0, CV_REDUCE_SUM, CV_32S);
		imrow.convertTo(imrow, CV_8U);
		threshold(imrow, imrow, m_prePara.thr_bottom_cnt, 255, THRESH_BINARY);
		pyr = 1;
		resize(imrow, imrow, Size(imrow.cols / pyr, imrow.rows));
		/*uchar* */ptr = imrow.ptr(0);
		for (int x = imrow.cols - 1; x >= 0; x--) {
			if (255 == ptr[x]) {
				botm = x;
				break;
			}
		}
		if (botm == 0)
			return _pvolumes_inout->iret;
		botm = pyr * botm + m_proBottleSpec.scanBorder;

		_pvolumes_inout->rltLevel = level;
		_pvolumes_inout->rltBottom = botm;
		_pvolumes_inout->rltVolumes = float(botm - level)/ _pvolumes_inout->paralCof / _pvolumes_inout->coff;
		if (_pvolumes_inout->rltVolumes <= _pvolumes_inout->paralHigh && _pvolumes_inout->rltVolumes >= _pvolumes_inout->paralLow)
			_pvolumes_inout->iret = true;
		return _pvolumes_inout->iret;
			
	}

	//如果漏帧可能会错位。
	void ProcParticle::DrawRltIm(vector<cv::Mat> & _vmIn, bool iret, vector<cv::Rect>& _rtBlink)
	{
		cv::Scalar color;
		if (false == mp_volumes_inout->iret/*iret*/)
			color = cv::Scalar(0, 0, 255);
		else
			color = cv::Scalar(0, 255, 0);
		/*cv::Mat & im = *m_vmCollect.vmRlt.rbegin() = _vmIn[m_vireg.rbegin()->index].clone();*/
		cv::Mat& im = *m_vmCollect.vmRlt.rbegin() = _vmIn[m_vireg.rbegin()->index].clone();
		if (im.channels() == 1)
			cv::cvtColor(im, im, CV_GRAY2BGR);

		if (mp_volumes_inout->rltBottom > -1 && mp_volumes_inout->rltLevel > -1) {
			cv::line(im, cv::Point(mp_volumes_inout->rltBottom, 0), cv::Point(mp_volumes_inout->rltBottom, im.rows), color);
			cv::line(im, cv::Point(mp_volumes_inout->rltLevel, 0), cv::Point(mp_volumes_inout->rltLevel, im.rows), color);
		}
		if (false == iret)
			color = cv::Scalar(0, 0, 255);
		else
			color = cv::Scalar(0, 255, 0);
		/*cv::rectangle(im, m_proBottleSpec.roi, color, 3);*/
		for (int i = 0; i < m_proBottleSpec.roiCorner.size(); i++) {
			cv::line(im, m_proBottleSpec.roiCorner[i % 4], m_proBottleSpec.roiCorner[(i + 1) % 4], color, 2);
		}
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
				if(m_particle_type == particle_di)
					_vmIn[idx] = _vmIn[idx] /** 3*/; // 图像亮度是之前三倍
				cv::Mat& imrlt = m_vmCollect.vmRlt[idx] = _vmIn[idx].clone();
				/*cv::Mat& imrlt = m_vmCollect.vmRlt[idx];*/
				if (imrlt.channels() == 1)
					cv::cvtColor(imrlt, imrlt, CV_GRAY2BGR);
				/*cv::rectangle(imrlt, m_proBottleSpec.roi + m_vireg[i].pt, color);*/
				for (int ic = 0; ic < m_proBottleSpec.roiCorner.size(); ic++) {
					cv::line(imrlt, m_proBottleSpec.roiCorner[ic % 4] + m_vireg[i].pt, m_proBottleSpec.roiCorner[(ic + 1) % 4] + m_vireg[i].pt, color, 2);
				}
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