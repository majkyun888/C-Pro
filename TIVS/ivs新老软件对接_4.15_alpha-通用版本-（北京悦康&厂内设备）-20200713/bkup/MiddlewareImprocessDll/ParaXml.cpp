#include "stdafx.h"
#include "ParaXml.h"
#include "../LogTxt/AppLog.h"
#include "strConvert.h"

void ParaInter::write(cv::FileStorage& fs) const //Write serialization for this class
{
	fs << "{" << "test" << test;
	fs << "rtMaskBlink" << "[";
	for (size_t idx = 0; idx < rtMaskBlink.size(); idx++)
		fs << rtMaskBlink[idx];
	fs << "]";
	fs << "PrePara" << "{";
	fs << "btThick" << pre_para.btThick;
	fs << "lenMax" << pre_para.lenMax;
	fs << "lenMin" << pre_para.lenMin;
#ifdef _para_test
	fs << "level_shiftDown" << pre_para.level_shiftDown;
	fs << "level_shiftUp" << pre_para.level_shiftUp;
	fs << "thr_level_br" << pre_para.thr_level_br;
	fs << "thr_level_cnt" << pre_para.thr_level_cnt;
#endif // _para_test
	fs << "ptSft" << pre_para.ptSft;
	fs << "ptLightSpot" << pre_para.ptLightSpot;
	fs << "rangeDetectMin" << pre_para.rangeDetectMin;
	fs << "thr_bottom_br" << pre_para.thr_bottom_br;
	fs << "thr_bottom_cnt" << pre_para.thr_bottom_cnt;
	fs << "thr_diff_br" << pre_para.thr_diff_br;
	fs << "thr_diff_cnt" << pre_para.thr_diff_cnt;

	fs << "thr_locy_br" << pre_para.thr_locy_br;
	fs << "thr_locy_cnt" << pre_para.thr_locy_cnt;
	fs << "thr_side_br" << pre_para.thr_side_br;
	fs << "scanUp" << pre_para.scanUp;
	fs << "bottle_blink_filter_rangeX_left" << pre_para.bottle_blink_filter_rangeX_left;
	fs << "bottle_blink_filter_rangeX_right" << pre_para.bottle_blink_filter_rangeX_right;
	fs << "bottle_blink_filter_rangeY_max" << pre_para.bottle_blink_filter_rangeY_max;
	fs << "bottle_blink_filter_rangeY_min" << pre_para.bottle_blink_filter_rangeY_min;
	fs << "bottle_blink_fliter_bright_thr" << pre_para.bottle_blink_fliter_bright_thr;
	fs << "val_dynamic_filter" << pre_para.val_dynamic_filter;
	fs << "DyEordeSize" << pre_para.DyEordeSize;
	fs << "knDyTimes" << pre_para.knDyTimes;
	fs << "}";
	fs << "is_proc_regPD" << "{";
	fs << "iFindBottom" << (int)is_proc_regPD.iFindBottom;
	fs << "iFindLevel" << (int)is_proc_regPD.iFindLevel;
	fs << "iFindSide" << (int)is_proc_regPD.iFindSide;
	fs << "iRegBottom" << (int)is_proc_regPD.iRegBottom;
	fs << "iRegSide" << (int)is_proc_regPD.iRegSide;
	fs << "}";
	fs << "particle_type" << (int)particle_type;
	fs << "}";
}

void ParaInter::read(const cv::FileNode& node)  //Read serialization for this class
{
	test = (int)node["test"];
	cv::FileNode n = node["PrePara"];
	if (n.type() != cv::FileNode::MAP)
	{
		cerr << "PrePara is not existed! FAIL" << endl;
		return;
	}
	pre_para.btThick = n["btThick"];
	pre_para.lenMax = n["lenMax"];
	pre_para.lenMin = n["lenMin"];
#ifdef _para_test
	pre_para.level_shiftDown = n["level_shiftDown"];
	pre_para.level_shiftUp = n["level_shiftUp"];
	pre_para.thr_level_br = n["thr_level_br"];
	pre_para.thr_level_cnt = n["thr_level_cnt"];
#endif // _para_test
	vector<int> pt; n["ptSft"] >> pt;  pre_para.ptSft = cv::Point(pt[0], pt[1]);
	n["ptLightSpot"] >> pt; pre_para.ptLightSpot = cv::Point(pt[0], pt[1]);
	pre_para.rangeDetectMin = n["rangeDetectMin"];
	pre_para.thr_bottom_br = n["thr_bottom_br"];
	pre_para.thr_bottom_cnt = n["thr_bottom_cnt"];
	pre_para.thr_diff_br = n["thr_diff_br"];
	pre_para.thr_diff_cnt = n["thr_diff_cnt"];
	pre_para.thr_locy_br = n["thr_locy_br"];
	pre_para.thr_locy_cnt = n["thr_locy_cnt"];
	pre_para.thr_side_br = n["thr_side_br"];
	pre_para.scanUp = n["scanUp"];
	pre_para.bottle_blink_filter_rangeX_left = n["bottle_blink_filter_rangeX_left"];
	pre_para.bottle_blink_filter_rangeX_right = n["bottle_blink_filter_rangeX_right"];
	pre_para.bottle_blink_filter_rangeY_max = n["bottle_blink_filter_rangeY_max"];
	pre_para.bottle_blink_filter_rangeY_min = n["bottle_blink_filter_rangeY_min"];
	pre_para.bottle_blink_fliter_bright_thr = n["bottle_blink_fliter_bright_thr"];
	pre_para.val_dynamic_filter = n["val_dynamic_filter"];
	pre_para.DyEordeSize = n["DyEordeSize"];
	pre_para.knDyTimes = n["knDyTimes"];
	n = node["is_proc_regPD"];
	if (n.type() != cv::FileNode::MAP)
	{
		cerr << "PrePara is not existed! FAIL" << endl;
		return;
	}
	is_proc_regPD.iFindBottom = (int)n["iFindBottom"];
	is_proc_regPD.iFindLevel = (int)n["iFindLevel"];
	is_proc_regPD.iFindSide = (int)n["iFindSide"];
	is_proc_regPD.iRegBottom = (int)n["iRegBottom"];
	is_proc_regPD.iRegSide = (int)n["iRegSide"];
	int val_particle_type(-1);
	val_particle_type = node["particle_type"];
	switch (val_particle_type)
	{
	case 0:
		particle_type = algoParticleDi::particle_di;
		break;
	case 1:
		particle_type = algoParticleDi::particle_bei;
		break;
	default:
		PRINT_LOG(_T("particle_type loading is error"));
		break;
	}
	isRead = true;
	n = node["rtMaskBlink"];
	if (n.type() != cv::FileNode::SEQ)
	{
		cerr << "rtMaskBlink is not existed! FAIL" << endl;
		return;
	}
	rtMaskBlink.clear();
	cv::FileNodeIterator it = n.begin(), it_end = n.end();
	for (; it != it_end; ++it)
	{
		vector<int> rtCor;
		*it >> rtCor;
		rtMaskBlink.push_back(cv::Rect(rtCor[0], rtCor[1], rtCor[2], rtCor[3]));
	}

}

//----------------------------------------------------------------------------------------------
//These write and read functions must exist as per the inline functions in operations.hpp
static void write(cv::FileStorage& fs, const std::string&, const ParaInter& x) {
	x.write(fs);
}
static void read(const cv::FileNode& node, ParaInter& x, const ParaInter& default_value = ParaInter()) {
	if (node.empty())
		x = default_value;
	else
		x.read(node);
}

bool readInterParaFromXml(ParaInter& _para, string _xml_path)
{
	cv::FileStorage fs(_xml_path, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		return false;
	}
	cv::FileNode node = fs["ParaInter"];
	if (node.type() == cv::FileNode::NONE || node.type() == cv::FileNode::EMPTY)
	{
		return false;
	}
	node >> _para;
	fs.release();
	return true;
}

bool writeInterParaToXml(ParaInter& _para, string _xml_path)
{
	cv::FileStorage fs(_xml_path, cv::FileStorage::WRITE);
	if (!fs.isOpened())
	{
		return false;
	}
	fs << "ParaInter" << _para;
	fs.release();
	return true;
}

bool MiddleWriteInterParaToXml(ParaInter& _para_inter, string _path/*, int _camIdx*/) {
	return writeInterParaToXml(_para_inter, _path + "paraInter.xml");
}

bool MiddleReadInterParaFromXml(ParaInter& _para_inter, string _path/*, int _camIdx*/) {
	return readInterParaFromXml(_para_inter, _path + "paraInter.xml");
}

//----------------------------------------------------------------------------------------------
//These write and read functions must exist as per the inline functions in operations.hpp
static void write(cv::FileStorage& fs, const std::string&, const MiddlewareImprocessSeq::ParaIn& x) {
	x.write(fs);
}
static void read(const cv::FileNode& node, MiddlewareImprocessSeq::ParaIn& x, const MiddlewareImprocessSeq::ParaIn& default_value = MiddlewareImprocessSeq::ParaIn()) {
	if (node.empty())
		x = default_value;
	else
		x.read(node);
}

bool readParaFromXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path)
{
	cv::FileStorage fs(_xml_path + "para.xml", cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		CString strCerr;
		strCerr.Format(_T("failed to open xml from %s"), StdString2Unicode(_xml_path));
		PRINT_LOG(strCerr);
		::MessageBox(NULL, strCerr, NULL, MB_OK);
		return false;
	}
	cv::FileNode node = fs["para_particle"];
	if (node.type() == cv::FileNode::NONE || node.type() == cv::FileNode::EMPTY)
	{
		CString strCerr;
		strCerr.Format(_T("failed to read xml Para from %s"), StdString2Unicode(_xml_path));
		PRINT_LOG(strCerr);
		::MessageBox(NULL, strCerr, NULL, MB_OK);
		return false;
	}
	node >> _para;
	fs.release();
	return true;
}

bool writeParaToXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path)
{
	cv::FileStorage fs(_xml_path + "para.xml", cv::FileStorage::WRITE);
	if (!fs.isOpened())
	{
		CString strCerr;
		strCerr.Format(_T("failed to open xml from %s"), StdString2Unicode(_xml_path));
		PRINT_LOG(strCerr);
		::MessageBox(NULL, strCerr, NULL, MB_OK);
		return false;
	}
	fs << "para_particle" << _para;
	fs.release();
	return true;
}