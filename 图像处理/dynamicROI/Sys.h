#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <stack>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include<io.h>
#include <conio.h>
#include <ctime>
#include <time.h>
#include <stack>
#include "MouseEvent.h"

using namespace std;
using namespace cv;


#define Liquid_Point  5																				//液位点
#define Bottom_thickness 50																			//瓶底厚度
#define Level_distance 10																			//液位最小宽度
#define AXIS_X 0																					//X轴
#define AXIS_Y 1																					//Y轴
#define KERNEL_SIZE (Size(5, 5))																	//核尺寸
#define GAUSSIAN_KERNEL_SIZE (Size(3, 3))															//高斯核尺寸
#define ANCHOR_POINT (Point(-1, -1))																//锚定点
#define BOTTOMCAPBINARIZATION_THRESHOLD	30															//瓶盖二值化闸值
#define BINARY_POINT_MIN 1																			//二值化映射值
#define CENTERPOINT_BINARY_THRESHOLD	150															//中心点二值化闸值
#define BINARY_POINT_MAX 255																		//最大映射值
#define RECT_COLOR (Scalar(255, 0, 0))
#define BOTTOM_POINT	3																			//底部定位点
#define BRIGHTNESS_SIDE  3																			//两侧亮
#define RECT_OFFSET_X	40
#define BOTTOM_THICKNESS 10																			//瓶底厚
#define SUPPORT_UPPER_BOUND	 -30																	//支持上界
#define SUPPORT_LOWER_BOUND	15																		//支持下界
#define IMAGE_WIDTH		768																			//图像宽
#define IMAGE_HEIGHT	400																			//图像高
#define OFFSET_LEVEL	154																			//液位底部偏移154
#define SAMPLING_POINT_NUM	100																		//采样点个数
#define FITTING_ACCURACY	1e-2																	//拟合精度
#define IMAGE_DELAY			1          																//播放延迟
#define DISTURBANCEOFFSET	2																		//阻力值
#define VERTICAL_RESISTANCE	3																		//垂直阻力
#define SEARCH_STEP			3																		//搜索步长
#define ASPECT_RATIO		0.70000																	//圆宽比
#define MAXIMUM_CENTER_POINT	250																	//最大中心偏离点
#define MINIMUM_CENTER_POINT	60																	//最小中心偏离点
#define MAXIMUM_SEED_VALUE		110																	//最大种子值
#define MINIMUM_SEED_VALUE		15																	//最小种子值
#define ENHANCEMENT_FACTOR	3																		//增强因子
using COLOR = Scalar;
vector<Point2f> points;
#define nima Scalar(3, 3, 3)

typedef enum _IMAGE_ROI_SELECT_
{
	BOTTOM_CAP_ROI = 0,
	BOTTOM_SIDE_ROI = 1,
	LEVEL_SIDE_ROI = 2,
	CAPSULE_ROI = 3
}IMAGE_ROI_SELECT;																					//图片ROI区域选择

typedef struct _targetBox
{
	int label;																						//类别
	int area;
	int radius;
	float AspectRatio;
	Rect boundryBox;																				//外接矩形
	Point center;																					//中心点
}targetBox;																							//候选矩形

typedef struct findBottomPoint
{
	Point centerPoint;																				//液位中心点
	Point bottomUpPoint;																			//瓶子上侧点
	Point bottomDownPoint;																			//瓶子下侧点
	Point bottomCenterPoint;																		//瓶底定位点
	Point BenchmarkNeckPoint;																		//瓶盖定位点
	Point DatumPointY;																				//y坐标定位点
	Point DatumVerticalPoint;																		//垂直基准点
	int BottomRectWidth;																			//底部区域宽
	int BottomRectHeight;																			//底部区域高
	int centerThickness;						
	int bottomSideThick;
}BottomPoint;
	
vector<pair<int, int>> bias = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };	//4邻域
vector<pair<int, int>> bias8 = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };//8邻域

