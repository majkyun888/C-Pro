#pragma once 
#include <opencv2/opencv.hpp>
using namespace std;

#ifdef PARTICLE_DI_EXPORTS
#define PARTICLE_DI_API __declspec(dllexport)
#else
#define PARTICLE_DI_API __declspec(dllimport)
#endif

class ParaInParticleDi
{
public:
	int minnumimg;
	int mintotnpt;
};

class RltOutParticleDi
{
public:
	int nimgpt;
	int totnpt;
};

class PARTICLE_DI_API algParticleDi
{
public:
	algParticleDi(ParaInParticleDi* _p_para_particle_di, RltOutParticleDi* _p_rlt_particle_di);
	~algParticleDi();

	bool proc(vector<cv::Mat>& _vims_src, cv::Mat& _color_rlt) ;
private:
	static bool goodSimulate ;
	ParaInParticleDi* p_para_particle_di;
	RltOutParticleDi* p_rlt_particle_di;
};

algParticleDi::algParticleDi(ParaInParticleDi* _p_para_particle_di, RltOutParticleDi* _p_rlt_particle_di):  p_para_particle_di(_p_para_particle_di), 
						p_rlt_particle_di(_p_rlt_particle_di)
{

}

algParticleDi::~algParticleDi()
{

}