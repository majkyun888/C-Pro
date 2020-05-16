#pragma once

#include "Camera.h"
class CameraRef
{
};

//struct CameraRef_struct_common
//{
//	CameraRef_struct_common(Camera_struct& _cam_struct):frame_rate(_cam_struct.frame_rate), 
//		width_max(_cam_struct.width_max),
//		height_max(_cam_struct.height_max),
//		offsetx(_cam_struct.offsetx),
//		offsety(_cam_struct.offsety),
//		width(_cam_struct.width),
//		height(_cam_struct.height),
//		shutter(_cam_struct.shutter),
//		trigger(_cam_struct.trigger),
//		acq_mode(_cam_struct.acq_mode)
//		/*: name(_cam_struct.name),
//		sn(_cam_struct.sn),
//		sequence(_cam_struct.sequence),
//		strobe(_cam_struct.strobe),
//		output1(_cam_struct.output1),
//		frame_rate(_cam_struct.frame_rate),
//		shutter2(_cam_struct.shutter2),
//		triglab(_cam_struct.triglab),
//		ptr(_cam_struct.ptr),
//		dsporg(_cam_struct.dsporg),
//		dspdim(_cam_struct.dspdim),
//		scale(_cam_struct.scale),
//		modedsp(_cam_struct.modedsp),
//		textwidth(_cam_struct.textwidth),
//		textnum(_cam_struct.textnum),
//		light_out(_cam_struct.light_out),*/
//		//text(_cam_struct.text)
//	{};
//	//CInstantCamera* camera;       // pointer of camera object
//	//IPylonDevice* device;       // pointer of camera object
//	//wchar_t* name;    // description name of camera (4 chars + terminator)
//	//char* sn;    // serial number of camera    (8 chars + terminator)
//	int64_t& offsetx;      // acquisition horizontal offset
//	int64_t& offsety;      // acquisition vertical offset
//	int64_t& width;        // acquisition horizontal size
//	int64_t& height;       // acquisition vertical size
//	int64_t& width_max;    // max horizontal resolution
//	int64_t& height_max;   // max vertical resolution
//	//BOOL& sequence;     // true=sequence mode (particle)
//	BOOL& trigger;      // true=waiting hardware trigger
//	//BOOL& strobe;       // true=enable output strobe on exposure
//	//BOOL& output1;      // true=enable output1 control by software
//	////CBooleanPtr      userout_ptr;  // pointer to control useroutput if enabled
//	float& frame_rate;   // 0=freerun - other tries to set acq rate
//	int& acq_mode;     // acquisition mode (enum) AcquisitionMode_Continuous - TriggerMode_On
//	int& shutter;      // shutter time (us)
//	//int& shutter2;     // second shutter time (us)
//	//int& triglab;      // trigger_label index (0..NUMTRIGGER-1)
//	//BYTE* ptr;          // pointer to data of last acquired image
//	//POINT& dsporg;       // display origin coordinates
//	//SIZE& dspdim;       // display windows dimensions
//	//double& scale;        // scaling factor
//	//int& modedsp;      // rotation and other display options
//	//int& textwidth;    // width of lines of text (0=same width as dsp window)
//	//int& textnum;      // number of text rows below dsp image
//	//char* light_out;    // if != NULL is the PLC parameter to switch light ON when TRIGGER WAIT is disabled
//	//CText_box** text; //
//};
//
//
//struct CameraRef_struct_HikVision : public CameraRef_struct_common
//{
//	CameraRef_struct_HikVision(Camera_struct& _cam_struct) :
//		CameraRef_struct_common(_cam_struct), m_hDevHandle(_cam_struct.m_hDevHandle)
//		/*device(_cam_struct.device),
//		userout_ptr(_cam_struct.userout_ptr)*/
//	{};
//	void* &m_hDevHandle;																							//实例相机句柄
//	//CInstantCamera*& camera;       // pointer of camera object
//	//IPylonDevice*& device;       // pointer of camera object
//	//CBooleanPtr& userout_ptr;  // pointer to control useroutput if enabled
//};


class CcameraRefCommon
{
public:
	CcameraRefCommon(Camera_struct * _pdata_common): pdata_common(_pdata_common)
	{}
	virtual ~CcameraRefCommon()
	{}
	virtual int initialize(UINT ind) = 0;
	virtual int parameters(UINT ind, bool& _bInit_light_out) = 0;
//	virtual int set_camera_output(UINT ind, bool val) = 0;
//	static HANDLE   mutex;
//	string str_exception;
	virtual bool SetAcqFrameCount(int _numimg, bool& _bSucess) = 0;
	virtual bool SetStaicChunkNodeMapSize() = 0;
	virtual bool SetStartGrabbing() = 0;
	virtual bool SetSoftwareTrigger() = 0;
	virtual bool RetrieveResult(unsigned int _timeoutMs) = 0;
	virtual int GetCurFrameNumber() = 0;
	virtual int GetCurTimeStamp() = 0;
	struct ImData
	{
		ImData() {
			img_acq = NULL;  tmstamp = 0;
			frmnum = 0; bGrabSuccess = true;
			bGrabTimeout = false;
		}
		ImData& operator = (ImData _im_data) {
			img_acq = _im_data.img_acq;
			tmstamp = _im_data.tmstamp;
			frmnum = _im_data.frmnum;
			bGrabSuccess = _im_data.bGrabSuccess;
			bGrabTimeout = _im_data.bGrabTimeout;
		}
		BYTE* img_acq;
		uint64_t tmstamp;
		uint32_t frmnum;
		//CString cstr_errDesc;
		bool bGrabTimeout;
		bool bGrabSuccess;

	}im_data;
public:
	Camera_struct* pdata_common;
};

template <typename IMAGE_BUFF>
class Image_Process
{
};

template <>
class Image_Process<unsigned char *>
{
private:
	Mat m_image;																								//帧结构体																											//帧地址
	MV_CC_PIXEL_CONVERT_PARAM stParam;
	unsigned char* pFrameBuf;
	Image_Process() {};
	Image_Process& operator= (const Image_Process&) {};
	Image_Process(const Image_Process&) {};
	int Size;
public:
	Image_Process(int imageSize, int Height, int Width, int mode) {
		Size = imageSize;
		if (NULL == pFrameBuf)
			pFrameBuf = (unsigned char*)malloc(Size);

		m_image = Mat::Mat::ones(Height, Width, CV_8UC3);
		memset(&stParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
	}
	~Image_Process()
	{
		if (pFrameBuf)
		{
			free(pFrameBuf);
			pFrameBuf = NULL;
		}
	}
public:
	unsigned char * FormatImageToBGR(void * handle, unsigned char * buffer, MV_FRAME_OUT_INFO_EX& stInfo)
	{
		stParam.pSrcData = buffer;																									//原始图像数据
		stParam.nSrcDataLen = stInfo.nFrameLen;																						//原始图像数据长度
		stParam.enSrcPixelType = stInfo.enPixelType;																				//原始图像数据的像素格式
		stParam.nWidth = stInfo.nWidth;																								//图像宽
		cout << stInfo.nWidth << endl;
		stParam.nHeight = stInfo.nHeight;																							//图像高     
		cout << stInfo.nHeight << endl;
		stParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;																		//Mono10
		stParam.nDstBufferSize = Size;
		stParam.pDstBuffer = pFrameBuf;																								//输出数据缓冲区，存放转换之后的数据     
		m_image.data = pFrameBuf;
		int nRet = MV_CC_ConvertPixelType(handle, &stParam);
		string str = "./rotary/";
		if (nRet == MV_OK)
		{
			str += to_string(stInfo.nFrameNum);
			str += ".png";
			imwrite(str.c_str(), m_image);
		}
		return pFrameBuf;
	}
};

class CcameraRefHikVision : public CcameraRefCommon
{
private:
	typedef void(__stdcall* cbException)(unsigned int, void*);																			//异常回调指针
	static MV_CC_DEVICE_INFO_LIST* pstDevList;																							//设备信息结构体
	static map<unsigned char*, MV_CC_DEVICE_INFO*> m_seq;																				//存储序列号
	MV_FRAME_OUT_INFO_EX stInfo;																										//帧结构体
	bool m_bOpenDevice;																													//判断相机的开启状态
	bool m_bStartGrabbing;																												//判断相机的抓图状态
	void* m_hDevHandle;																													//设备句柄
	int num;																															//设备编号
	static Image_Process<unsigned char *> Image_handle;
public:
	CcameraRefHikVision(Camera_struct* _pdata);
	~CcameraRefHikVision();
	static int GlobalInitialize(void);																									//全局初始化
	static void GlobalTerminate(void);																									//全局资源释放
	//int CameraTestFunc();																												//测试相机基本功能函数
	virtual int initialize(UINT ind);																									//相机初始化
	static vector< CcameraRefHikVision* > vec_p_cam;																					//存放相机实例
	virtual int parameters(UINT ind, bool& _bInit_light_out);																			//相机参数设置
	virtual int set_camera_output(UINT ind, bool val);
	virtual bool SetAcqFrameCount(int _numimg, bool& _bSucess);																			//设置一次触发采集的帧数
	virtual bool SetStartGrabbing();																									//设置抓图
	virtual bool SetStaicChunkNodeMapSize();																							//设置数据包大小
	virtual int GetCurFrameNumber();																									//获得图像帧号
	virtual int GetCurTimeStamp();																										//获取时间戳
	virtual bool RetrieveResult(unsigned int _timeoutMs);																				//获取图像buffer
	virtual bool SetSoftwareTrigger();																									//设置相机软触发模式
	
	int StopGrabbing();																													//关闭抓图
	
	bool getCameraStatus();																												//相机状态函数

	bool getGrabbingStatus();																											//获得相机抓图状态

	int closeCamera();																													//关闭相机

	unsigned char* Image_Format();
private:
	CcameraRefHikVision() = delete;
	CcameraRefHikVision(const CcameraRefHikVision&) = delete;
	CcameraRefHikVision& operator= (const CcameraRefHikVision&) = delete;
private:
	int OpenCamera(UINT ind);																											//打开相机
	int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);																				//枚举设备
	int GetOptimalPacketSize();																											//获取当前网络下最佳数据包大小

	int GetIntValue(const char * strKey, unsigned int *pnValue) ; 																		//获得int属性
	int SetIntValue(const char * strKey, unsigned int nValue);																			//设置int属性

	int GetEnumValue(const char * strKey, unsigned int * pnValue);
	int SetEnumValue(const char* strKey, unsigned int nValue);																			//设置枚举类型值

	int GetFloatValue(const char* strKey, float* pfValue);																				//获得增益率，曝光时间等浮点值
	int SetFloatValue(const char* strKey, float fValue);

	int GetBoolValue(const char* strKey, bool* pbValue);																				//获得BOOL属性值
	int SetBoolValue(const char* strKey, bool pbValue);

	int GetStringValue(const char* strKey, char* strValue, unsigned int nSize);															//获得String属性
	int SetStringValue(const char* strKey, const char* strValue);

	int SetCommandValue(void* handle, const char* strKey);																				//设置ICommand值
																								
	int  RegisterExceptionCallBack(cbException HikException=NULL, void* pUser=NULL);													// ch:注册消息异常回调

	bool getOneFramTimeOut(int nMsec, MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char ** pData);

	friend void __stdcall ReconnectCamera(unsigned int nMsgType, void* pUser);
};
