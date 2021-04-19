#include "middlewareParticleDi.h"
//#include "../IM_particle/procDi.h"
#include "../Im-particle-di/procParticleDi.h"

#ifdef _DEBUG
#pragma comment(lib,"./../Debug/Im-particle-di.lib")
#else
#pragma comment(lib,"./../Release/Im-particle-di.lib")
#endif

bool MiddlewareImproessSeqParticleDi::run(RltOut& _rlt, cv::Mat& _imcolor_rlt)
{
	ParaInParticleDi*	p_para_particle_di = new ParaInParticleDi;
	RltOutParticleDi*	p_rlt_particle_di = new RltOutParticleDi;
	p_para_particle_di->minnumimg = para.minnumimg;
	p_para_particle_di->mintotnpt = para.mintotnpt;
	algParticleDi* alg_particle_di = new algParticleDi(p_para_particle_di, p_rlt_particle_di);
	alg_particle_di->proc(vec_ims, _imcolor_rlt);
	_rlt.nimgpt = p_rlt_particle_di->nimgpt ;
	_rlt.totnpt = p_rlt_particle_di->totnpt ;
	return false;
}

