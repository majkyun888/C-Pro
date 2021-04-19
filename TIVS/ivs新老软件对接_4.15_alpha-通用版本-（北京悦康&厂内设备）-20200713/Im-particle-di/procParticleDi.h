#pragma once 
#include "dataTypeParticle.h"
#include "algoParticleDi.h"
#include "../MiddlewareImprocessDll/middlewareParticleDi.h"
#include "..//MiddlewareImprocessDll/dataType.h"
#include <opencv2/opencv.hpp>

using namespace std;

#ifdef PARTICLE_DI_EXPORTS
#define PARTICLE_DI_API __declspec(dllexport)
#else
#define PARTICLE_DI_API __declspec(dllimport)
#endif

//class ParaInter /*:public  algoParticleDi::PrePara*/
//{
//public:
//	algoParticleDi::PrePara pre_para;
//	vector<cv::Rect> rtMaskBlink;
//	bool isRead;
//	int	test;
//	ParaInter() {
//		isRead = false;
//	};
//	void write(cv::FileStorage& fs) const;//Write serialization for this class
//	void read(const cv::FileNode& node); //Read serialization for this class
//};
//PARTICLE_DI_API extern bool readInterParaFromXml(ParaInter& _para, string _xml_name);
//PARTICLE_DI_API extern bool writeInterParaToXml(ParaInter& _para, string _xml_name);

typedef MiddlewareImprocessSeq::ParaIn aPara;
typedef MiddlewareImprocessSeq::RltOut aRlt;

class PARTICLE_DI_API algParticleDi
{
public:
	algParticleDi(aPara* _p_para, aRlt* _p_rlt);
	~algParticleDi();

	bool proc(vector<cv::Mat>& _vims_src, cv::Mat& _color_rlt, ParaInter* para_inter) ;
	vector<cv::Mat> vimrltOffline;
private:
	static bool goodSimulate ;  // ¡Ÿ ±≤‚ ‘”√
	aPara* p_apara;
	aRlt* p_arlt;
	algoParticleDi::ProcParticle proc_particle;
};

