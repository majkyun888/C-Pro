#pragma once
#include "opencv2\opencv.hpp"
using namespace cv;

// ALGO_base_class
class CAlgo  
{
public:
	int algo_id;
	virtual int process(cv::Mat im_src, cv::Mat im_result) = 0;
    virtual ~CAlgo() {};
};
//========================================================



// ALGO_Flange
//--------------------------------------------------------
#ifdef FLANGEPROC_EXPORTS
#define FLANGEPROC_API __declspec(dllexport)
#else
#define FLANGEPROC_API __declspec(dllimport)
#endif

  typedef	struct 
  {
	bool learning_flange;
	int roi_left;
	int roi_top;
	int roi_right;
	int roi_bottom;
	int th_conv_depth;
	int th_dist;
	int th_sym;
  }Para_Flange ;

  typedef struct 
  {
	int num_conv_defect;
	int dist;
	int sym_perc1;
	int sym_perc2;
  }Result_Flange;


class CFlange : public CAlgo
{
public: 
  Para_Flange* pInput;
  Result_Flange* pOutput;
  CFlange(Para_Flange* _pInput, Result_Flange* _pOutput);
  int process(Mat im_src, Mat im_result);
  ~CFlange() {};
private:
  vector<Point> outline_w, outline_h;
};

extern "C" FLANGEPROC_API CFlange* GetFlange(Para_Flange*, Result_Flange*);

//