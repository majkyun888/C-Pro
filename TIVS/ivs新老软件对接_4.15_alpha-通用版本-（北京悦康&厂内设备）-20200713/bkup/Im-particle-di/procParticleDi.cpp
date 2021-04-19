#include "procParticleDi.h"
#include "algoParticleDi.h"


//void ParaInter::write(cv::FileStorage& fs) const //Write serialization for this class
//{
//	fs << "{" << "test" << test;
//	fs << "rtMaskBlink" << "[";
//	for ( size_t idx = 0; idx < rtMaskBlink.size(); idx++)
//		fs << rtMaskBlink[idx];
//	fs << "]";
//	fs << "PrePara" << "{";
//	fs << "btThick" << pre_para.btThick;
//	fs << "lenMax" << pre_para.lenMax;
//	fs << "lenMin" << pre_para.lenMin;
//	fs << "level_shiftDown" << pre_para.level_shiftDown;
//	fs << "level_shiftUp" << pre_para.level_shiftUp;
//	fs << "ptSft" << pre_para.ptSft;
//	fs << "rangeDetectMin" << pre_para.rangeDetectMin;
//	fs << "thr_bottom_br" << pre_para.thr_bottom_br;
//	fs << "thr_bottom_cnt" << pre_para.thr_bottom_cnt;
//	fs << "thr_diff_br" << pre_para.thr_diff_br;
//	fs << "thr_diff_cnt" << pre_para.thr_diff_cnt;
//	fs << "thr_level_br" << pre_para.thr_level_br;
//	fs << "thr_level_cnt" << pre_para.thr_level_cnt;
//	fs << "thr_locy_br" << pre_para.thr_locy_br;
//	fs << "thr_locy_cnt" << pre_para.thr_locy_cnt;
//	fs << "thr_side_br" << pre_para.thr_side_br;
//	fs << "}";
//	fs << "}";
//}
//void ParaInter::read(const cv::FileNode& node)  //Read serialization for this class
//{
//	test = (int)node["test"];
//	cv::FileNode n = node["rtMaskBlink"];
//	//if (n.type() != cv::FileNode::SEQ)
//	//{
//	//	cerr << "rtMaskBlink is not existed! FAIL" << endl;
//	//	return;
//	//}
//	//rtMaskBlink.clear();
//	//cv::FileNodeIterator it = n.begin(), it_end = n.end();
//	//for (; it != it_end; ++it)
//	//{
//	//	vector<int> rtCor;
//	//	*it >> rtCor;
//	//	rtMaskBlink.push_back(cv::Rect(rtCor[0], rtCor[1], rtCor[2], rtCor[3]));
//	//}
//	//PrePara::btThick = node["btThick"];
//	//PrePara::lenMax = node["lenMax"];
//	//PrePara::lenMin = node["lenMin"];
//	//PrePara::level_shiftDown = node["level_shiftDown"];
//	//PrePara::level_shiftUp = node["level_shiftUp"];
//	//vector<int> pt; node["ptSft"] >> pt;  PrePara::ptSft = cv::Point(pt[0], pt[1]);;
//	//PrePara::rangeDetectMin = node["rangeDetectMin"];
//	//PrePara::thr_bottom_br = node["thr_bottom_br"];
//	//PrePara::thr_bottom_cnt = node["thr_bottom_cnt"];
//	//PrePara::thr_diff_br = node["thr_diff_br"];
//	//PrePara::thr_diff_cnt = node["thr_diff_cnt"];
//	//PrePara::thr_level_br = node["thr_level_br"];
//	//PrePara::thr_level_cnt = node["thr_level_cnt"];
//	//PrePara::thr_locy_br = node["thr_locy_br"];
//	//PrePara::thr_locy_cnt = node["thr_locy_cnt"];
//	//PrePara::thr_side_br = node["thr_side_br"];
//	n = node["PrePara"];
//	if (n.type() != cv::FileNode::MAP)
//	{
//		cerr << "PrePara is not existed! FAIL" << endl;
//		return;
//	}
//	pre_para.btThick = n["btThick"];
//	pre_para.lenMax = n["lenMax"];
//	pre_para.lenMin = n["lenMin"];
//	pre_para.level_shiftDown = n["level_shiftDown"];
//	pre_para.level_shiftUp = n["level_shiftUp"];
//	vector<int> pt; n["ptSft"] >> pt;  pre_para.ptSft = cv::Point(pt[0], pt[1]);;
//	pre_para.rangeDetectMin = n["rangeDetectMin"];
//	pre_para.thr_bottom_br = n["thr_bottom_br"];
//	pre_para.thr_bottom_cnt = n["thr_bottom_cnt"];
//	pre_para.thr_diff_br = n["thr_diff_br"];
//	pre_para.thr_diff_cnt = n["thr_diff_cnt"];
//	pre_para.thr_level_br = n["thr_level_br"];
//	pre_para.thr_level_cnt = n["thr_level_cnt"];
//	pre_para.thr_locy_br = n["thr_locy_br"];
//	pre_para.thr_locy_cnt = n["thr_locy_cnt"];
//	pre_para.thr_side_br = n["thr_side_br"];
//	isRead = true;
//}
////----------------------------------------------------------------------------------------------
////These write and read functions must exist as per the inline functions in operations.hpp
//static void write(cv::FileStorage& fs, const std::string&, const ParaInter& x) {
//	x.write(fs);
//}
//static void read(const cv::FileNode& node, ParaInter& x, const ParaInter& default_value = ParaInter()) {
//	if (node.empty())
//		x = default_value;
//	else
//		x.read(node);
//}
//bool readInterParaFromXml(ParaInter& _para, string _xml_name)
//{
//	cv::FileStorage fs(_xml_name, cv::FileStorage::READ);
//	if (!fs.isOpened())
//	{
//		return false;
//	}
//	cv::FileNode node = fs["ParaInter"];
//	if (node.type() == cv::FileNode::NONE || node.type() == cv::FileNode::EMPTY)
//	{
//		return false;
//	}
//	node >> _para;
//	fs.release();
//	return true;
//}
//bool writeInterParaToXml(ParaInter& _para, string _xml_name)
//{
//	cv::FileStorage fs(_xml_name, cv::FileStorage::WRITE);
//	if (!fs.isOpened())
//	{
//		return false;
//	}
//	fs << "ParaInter" << _para;
//	fs.release();
//	return true;
//}

algParticleDi::algParticleDi(aPara* _p_apara, aRlt* _p_arlt) : p_apara(_p_apara),
p_arlt(_p_arlt)
{
	CV_Assert(_p_apara != NULL && _p_arlt != NULL);
}

algParticleDi::~algParticleDi()
{

}




bool algParticleDi::goodSimulate = false;

bool algParticleDi::proc(vector<cv::Mat>& _vims_src, cv::Mat& _color_rlt, ParaInter _para_inter) {


	bool iret(false);
	cv::Mat maskBlink = cv::Mat::zeros(_vims_src.at(0).size(), CV_8UC1);
	for (vector<cv::Rect>::iterator it = _para_inter.rtMaskBlink.begin();
		it != _para_inter.rtMaskBlink.end(); it++) {
		cv::rectangle(maskBlink, *it, cv::Scalar(255), -1);
	}
	proc_particle.m_prePara = _para_inter.pre_para;
	proc_particle.m_procReg = _para_inter.is_proc_regPD;
	proc_particle.m_particle_type = _para_inter.particle_type;
	proc_particle.m_imsize = _vims_src.at(0).size();
	proc_particle.m_seqNum = _vims_src.size();
	proc_particle.m_para.iVimRltSave = p_apara->iVimRltSave;
	proc_particle.m_para.nozero_frm_sum_thr = p_apara->nozero_frm_sum_thr;  //change 
	proc_particle.m_para.nozero_single_thr = p_apara->nozero_single_thr;
	proc_particle.m_para.sensity = p_apara->sensity;
	proc_particle.m_para.ratio = p_apara->ratio;
	proc_particle.m_para.roi = cv::Rect(
		cv::Point(p_apara->roi.tl().x, proc_particle.m_imsize.height - p_apara->roi.br().y),
		cv::Point(p_apara->roi.br().x, proc_particle.m_imsize.height - p_apara->roi.tl().y));

	proc_particle.m_para.roi &= cv::Rect(cv::Point(), proc_particle.m_imsize);
	memset(&proc_particle.m_rlt, 0, sizeof(proc_particle.m_rlt) );
	proc_particle.ProSpecInit(proc_particle.m_para.roi);
	/*proc_particle.m_particle_type = proc_particle.justfyParticleType(_vims_src[0](proc_particle.m_para.roi));*/
	if( _para_inter.isRead ==false )
		proc_particle.m_prePara.initParaByParticleType(proc_particle.m_particle_type);
	//if (proc_particle.m_particle_type == algoParticleDi::particle_di) {
	//	proc_particle.m_procReg.iFindLevel = true;
	//	proc_particle.m_procReg.iFindSide = true;
	//	proc_particle.m_procReg.iRegSide = true;
	//	proc_particle.m_procReg.iFindBottom = true;
	//	proc_particle.m_procReg.iRegBottom = true;
	//}
	//else {
	//	proc_particle.m_procReg.iFindLevel = true;
	//	proc_particle.m_procReg.iFindSide = true;
	//	proc_particle.m_procReg.iRegSide = true;
	//	proc_particle.m_procReg.iFindBottom = true;
	//	proc_particle.m_procReg.iRegBottom = false;
	//}
	proc_particle.m_procReg.okFindBottom = false;
	proc_particle.m_procReg.okFindLevel = false;
	proc_particle.m_procReg.okFindSide = false;
	proc_particle.m_procReg.okRegBottom = false;
	proc_particle.m_procReg.okRegSide = false;
	proc_particle.paraParticleInit();
	iret = proc_particle.RegisterSeqFuren(_vims_src);
	if (false == iret) {
		p_arlt->nimgpt = 2 * p_apara->minnumimg;
		p_arlt->totnpt = 2 * p_apara->mintotnpt;
		return false;  //如果在中间过程出现问题，则不保存离线序列，直接返回
	}
	proc_particle.VmCollectInit();
	proc_particle.SplitRegionFuren();
	proc_particle.FrmDiff(_vims_src);
	proc_particle.FilterLight(_vims_src);
	iret = proc_particle.ProcGlobal(_vims_src, maskBlink);
	if (iret == false) {
		p_arlt->nimgpt = 2* p_apara->minnumimg  ;
		p_arlt->totnpt = 2 * p_apara->mintotnpt;
	}
	else {
		p_arlt->nimgpt =  p_apara->minnumimg/2;
		p_arlt->totnpt =  p_apara->mintotnpt /2;
	}
	proc_particle.DrawRltIm(_vims_src, iret, _para_inter.rtMaskBlink);
	if (p_apara->iVimRltSave == true) {
		vimrltOffline.assign(proc_particle.m_vmCollect.vmRlt.begin(),
			proc_particle.m_vmCollect.vmRlt.end());
	}
	_color_rlt = proc_particle.m_vmCollect.vmRlt.rbegin()->clone();
	cv::transpose(_color_rlt, _color_rlt);
	cv::flip(_color_rlt, _color_rlt, 0);
	return iret;
};