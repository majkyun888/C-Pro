#include "procParticleDi.h"

bool algParticleDi::goodSimulate = false;

bool algParticleDi::proc(vector<cv::Mat>& _vims_src, cv::Mat& _color_rlt) {

	// 以下为接口测试代码
	cv::Mat imacc; double cnt(1.0);
	for (vector<cv::Mat>::iterator it = _vims_src.begin(); it != _vims_src.end(); it++) {
		double alpha = 1. / ++cnt;
		cv::accumulateWeighted(*it, imacc, alpha);
	}
	if (imacc.channels() == 1)
		cv::cvtColor(imacc, imacc, CV_GRAY2BGR);
	char str[100];
	cv::Scalar fontColor;
	bool good(false);
	if (!goodSimulate) {
		good = goodSimulate;
		p_rlt_particle_di->nimgpt = 2 * p_para_particle_di->minnumimg;
		p_rlt_particle_di->totnpt = 2 * p_para_particle_di->mintotnpt;
		sprintf(str, "OpenCV_REJECT_err_%d/%d_(%d/%d)",
			p_rlt_particle_di->totnpt, p_para_particle_di->mintotnpt,
			p_rlt_particle_di->nimgpt, p_para_particle_di->minnumimg);
		fontColor = cv::Scalar(0, 0, 255); // RED
	}
	else {
		p_rlt_particle_di->nimgpt = p_para_particle_di->minnumimg / 2;
		p_rlt_particle_di->totnpt = p_para_particle_di->mintotnpt / 2;
		sprintf(str, "OpenCV_GOOD_err_%d/%d_(%d/%d)",
			p_rlt_particle_di->totnpt, p_para_particle_di->mintotnpt,
			p_rlt_particle_di->nimgpt, p_para_particle_di->minnumimg);
		fontColor = cv::Scalar(0, 255, 0); // GREEN
	}

	cv::putText(imacc, str, cv::Point(0, imacc.rows / 2), cv::FONT_HERSHEY_COMPLEX, 3, fontColor);
	_color_rlt = imacc.clone();
	return  good;
};