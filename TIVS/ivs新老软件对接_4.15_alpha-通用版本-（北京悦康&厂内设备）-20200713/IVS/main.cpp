// ��demo���Ա��룬���������У�����Ϊ�����߼���ʾ
#define _AFXDLL
#include <opencv2\opencv.hpp>
#include  "../MiddlewareImprocessDll/middlewareParticleDi.h"
using namespace std;
#ifdef _DEBUG
#pragma comment(lib,"../IVS/Debug/MiddlewareImprocessDll.lib")
#else
#pragma comment(lib,"../IVS/Release/MiddlewareImprocessDll.lib")
#endif

// ---------------------------��ԭ�������ƶϵı������ͣ�����Ϊ����ͨ��ʹ�ã��������/��ʼ-------------
int mintotnpt; // min total num of points
int minnptimg; // min num of pt per image
int minnumimg; // min num of images with enough points
unsigned int  num_img;    // number of images for inspection
int  dim, dimx, dimy;
uchar* images[1];
int totnpt; // total num of points found
int nimgpt; // total num of images with enough points
struct ImageType
{
	unsigned int* ptr_dsp;
	struct Dim
	{
		int cx, cy;
	}dim_dsp;
};
ImageType* image_dsp[1];
int index(0);
// ---------------------------��ԭ�������ƶϵı������ͣ�����Ϊ����ͨ��ʹ�ã��������/����-------------
MiddlewareParticleDi* p_middleware_particle_di(NULL) ;

void main(int argc, char* argv[]){

	  //MiddlewareParticleDi::ParaIn para_particleDi;
	  //para_particleDi.minnumimg = minnumimg; 
	  //para_particleDi.mintotnpt = mintotnpt;
	  //MiddlewareParticleDi middle_particle_di(para_particleDi);
	  //middle_particle_di.ptrs2cvMats(images, dimx, dimy, num_img);
	  //cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
	  //middle_particle_di.run(rlt_particleDi, imcolor);
	  //nimgpt = rlt_particleDi.nimgpt;
	  //totnpt = rlt_particleDi.totnpt;
	  //middle_particle_di.cvMat2Ptr(image_dsp[index]->ptr_dsp, image_dsp[index]->dim_dsp.cx, image_dsp[index]->dim_dsp.cy, imcolor);
	  //MiddlewareParticleDi* p_middleware_particle_di = new MiddlewareParticleDi(para_particleDi);
	  //p_middleware_particle_di->ptrs2cvMats(images, dimx, dimy, num_img);
	  //cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
	  //p_middleware_particle_di->run(rlt_particleDi, imcolor);
	  //nimgpt = rlt_particleDi.nimgpt;
	  //totnpt = rlt_particleDi.totnpt;
	  //p_middleware_particle_di->cvMat2Ptr(image_dsp[index]->ptr_dsp, image_dsp[index]->dim_dsp.cx, image_dsp[index]->dim_dsp.cy,imcolor );
	  //delete p_middleware_particle_di;

	  // foropencv 20190506
	  {
		  //area->buffrect.left;
		  //cv::Rect rt_roi = cv::Rect(cv::Point(area->buffrect.left, area->buffrect.top), cv::Point(area->buffrect.right, area->buffrect.bottom));
		  MiddlewareParticleDi::ParaIn para_particleDi;
		  para_particleDi.minnumimg = minnumimg;
		  para_particleDi.mintotnpt = mintotnpt;
		  para_particleDi.nozero_frm_sum_thr;
		  para_particleDi.sensity;
		  para_particleDi.nozero_single_thr;
		  para_particleDi.ratio;
		  para_particleDi.iVimRltSave = false;
		  para_particleDi.iVimOnlineSave;
		  /*para_particleDi.roi = rt_roi;*/
		  para_particleDi.roi = cv::Rect(cv::Point(), cv::Point());
		  if (NULL == p_middleware_particle_di)
			  p_middleware_particle_di = new MiddlewareParticleDi(para_particleDi);
		  p_middleware_particle_di->para.iVimOnlineSave = para_particleDi.iVimOnlineSave;
		  p_middleware_particle_di->ptrs2cvMats(images, dimx, dimy, num_img);
		  cv::Mat imcolor; MiddlewareParticleDi::RltOut rlt_particleDi;
		  int camIdx(0);
		  p_middleware_particle_di->run(rlt_particleDi, camIdx, imcolor);
		  nimgpt = rlt_particleDi.nimgpt;
		  totnpt = rlt_particleDi.totnpt;
		  p_middleware_particle_di->cvMat2Ptr(image_dsp[index]->ptr_dsp, image_dsp[index]->dim_dsp.cx, image_dsp[index]->dim_dsp.cy, imcolor);
	  }


}