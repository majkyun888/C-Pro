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


#define Liquid_Point  5																				//Һλ��
#define Bottom_thickness 50																			//ƿ�׺��
#define Level_distance 10																			//Һλ��С���
#define AXIS_X 0																					//X��
#define AXIS_Y 1																					//Y��
#define KERNEL_SIZE (Size(5, 5))																	//�˳ߴ�
#define GAUSSIAN_KERNEL_SIZE (Size(3, 3))															//��˹�˳ߴ�
#define ANCHOR_POINT (Point(-1, -1))																//ê����
#define BOTTOMCAPBINARIZATION_THRESHOLD	30															//ƿ�Ƕ�ֵ��բֵ
#define BINARY_POINT_MIN 1																			//��ֵ��ӳ��ֵ
#define CENTERPOINT_BINARY_THRESHOLD	150															//���ĵ��ֵ��բֵ
#define BINARY_POINT_MAX 255																		//���ӳ��ֵ
#define RECT_COLOR (Scalar(255, 0, 0))
#define BOTTOM_POINT	3																			//�ײ���λ��
#define BRIGHTNESS_SIDE  3																			//������
#define RECT_OFFSET_X	40
#define BOTTOM_THICKNESS 10																			//ƿ�׺�
#define SUPPORT_UPPER_BOUND	 -30																	//֧���Ͻ�
#define SUPPORT_LOWER_BOUND	15																		//֧���½�
#define IMAGE_WIDTH		768																			//ͼ���
#define IMAGE_HEIGHT	400																			//ͼ���
#define OFFSET_LEVEL	154																			//Һλ�ײ�ƫ��154
#define SAMPLING_POINT_NUM	100																		//���������
#define FITTING_ACCURACY	1e-2																	//��Ͼ���
#define IMAGE_DELAY			1          																//�����ӳ�
#define DISTURBANCEOFFSET	2																		//����ֵ
#define VERTICAL_RESISTANCE	3																		//��ֱ����
#define SEARCH_STEP			3																		//��������
#define ASPECT_RATIO		0.70000																	//Բ���
#define MAXIMUM_CENTER_POINT	250																	//�������ƫ���
#define MINIMUM_CENTER_POINT	60																	//��С����ƫ���
#define MAXIMUM_SEED_VALUE		110																	//�������ֵ
#define MINIMUM_SEED_VALUE		15																	//��С����ֵ
#define ENHANCEMENT_FACTOR	3																		//��ǿ����
using COLOR = Scalar;
vector<Point2f> points;
#define nima Scalar(3, 3, 3)

typedef enum _IMAGE_ROI_SELECT_
{
	BOTTOM_CAP_ROI = 0,
	BOTTOM_SIDE_ROI = 1,
	LEVEL_SIDE_ROI = 2,
	CAPSULE_ROI = 3
}IMAGE_ROI_SELECT;																					//ͼƬROI����ѡ��

typedef struct _targetBox
{
	int label;																						//���
	int area;
	int radius;
	float AspectRatio;
	Rect boundryBox;																				//��Ӿ���
	Point center;																					//���ĵ�
}targetBox;																							//��ѡ����

typedef struct findBottomPoint
{
	Point centerPoint;																				//Һλ���ĵ�
	Point bottomUpPoint;																			//ƿ���ϲ��
	Point bottomDownPoint;																			//ƿ���²��
	Point bottomCenterPoint;																		//ƿ�׶�λ��
	Point BenchmarkNeckPoint;																		//ƿ�Ƕ�λ��
	Point DatumPointY;																				//y���궨λ��
	Point DatumVerticalPoint;																		//��ֱ��׼��
	int BottomRectWidth;																			//�ײ������
	int BottomRectHeight;																			//�ײ������
	int centerThickness;						
	int bottomSideThick;
}BottomPoint;
	
vector<pair<int, int>> bias = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };	//4����
vector<pair<int, int>> bias8 = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };//8����

