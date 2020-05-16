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
//	void* &m_hDevHandle;																							//ʵ��������
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
	Mat m_image;																								//֡�ṹ��																											//֡��ַ
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
		stParam.pSrcData = buffer;																									//ԭʼͼ������
		stParam.nSrcDataLen = stInfo.nFrameLen;																						//ԭʼͼ�����ݳ���
		stParam.enSrcPixelType = stInfo.enPixelType;																				//ԭʼͼ�����ݵ����ظ�ʽ
		stParam.nWidth = stInfo.nWidth;																								//ͼ���
		cout << stInfo.nWidth << endl;
		stParam.nHeight = stInfo.nHeight;																							//ͼ���     
		cout << stInfo.nHeight << endl;
		stParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;																		//Mono10
		stParam.nDstBufferSize = Size;
		stParam.pDstBuffer = pFrameBuf;																								//������ݻ����������ת��֮�������     
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
	typedef void(__stdcall* cbException)(unsigned int, void*);																			//�쳣�ص�ָ��
	static MV_CC_DEVICE_INFO_LIST* pstDevList;																							//�豸��Ϣ�ṹ��
	static map<unsigned char*, MV_CC_DEVICE_INFO*> m_seq;																				//�洢���к�
	MV_FRAME_OUT_INFO_EX stInfo;																										//֡�ṹ��
	bool m_bOpenDevice;																													//�ж�����Ŀ���״̬
	bool m_bStartGrabbing;																												//�ж������ץͼ״̬
	void* m_hDevHandle;																													//�豸���
	int num;																															//�豸���
	static Image_Process<unsigned char *> Image_handle;
public:
	CcameraRefHikVision(Camera_struct* _pdata);
	~CcameraRefHikVision();
	static int GlobalInitialize(void);																									//ȫ�ֳ�ʼ��
	static void GlobalTerminate(void);																									//ȫ����Դ�ͷ�
	//int CameraTestFunc();																												//��������������ܺ���
	virtual int initialize(UINT ind);																									//�����ʼ��
	static vector< CcameraRefHikVision* > vec_p_cam;																					//������ʵ��
	virtual int parameters(UINT ind, bool& _bInit_light_out);																			//�����������
	virtual int set_camera_output(UINT ind, bool val);
	virtual bool SetAcqFrameCount(int _numimg, bool& _bSucess);																			//����һ�δ����ɼ���֡��
	virtual bool SetStartGrabbing();																									//����ץͼ
	virtual bool SetStaicChunkNodeMapSize();																							//�������ݰ���С
	virtual int GetCurFrameNumber();																									//���ͼ��֡��
	virtual int GetCurTimeStamp();																										//��ȡʱ���
	virtual bool RetrieveResult(unsigned int _timeoutMs);																				//��ȡͼ��buffer
	virtual bool SetSoftwareTrigger();																									//�����������ģʽ
	
	int StopGrabbing();																													//�ر�ץͼ
	
	bool getCameraStatus();																												//���״̬����

	bool getGrabbingStatus();																											//������ץͼ״̬

	int closeCamera();																													//�ر����

	unsigned char* Image_Format();
private:
	CcameraRefHikVision() = delete;
	CcameraRefHikVision(const CcameraRefHikVision&) = delete;
	CcameraRefHikVision& operator= (const CcameraRefHikVision&) = delete;
private:
	int OpenCamera(UINT ind);																											//�����
	int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);																				//ö���豸
	int GetOptimalPacketSize();																											//��ȡ��ǰ������������ݰ���С

	int GetIntValue(const char * strKey, unsigned int *pnValue) ; 																		//���int����
	int SetIntValue(const char * strKey, unsigned int nValue);																			//����int����

	int GetEnumValue(const char * strKey, unsigned int * pnValue);
	int SetEnumValue(const char* strKey, unsigned int nValue);																			//����ö������ֵ

	int GetFloatValue(const char* strKey, float* pfValue);																				//��������ʣ��ع�ʱ��ȸ���ֵ
	int SetFloatValue(const char* strKey, float fValue);

	int GetBoolValue(const char* strKey, bool* pbValue);																				//���BOOL����ֵ
	int SetBoolValue(const char* strKey, bool pbValue);

	int GetStringValue(const char* strKey, char* strValue, unsigned int nSize);															//���String����
	int SetStringValue(const char* strKey, const char* strValue);

	int SetCommandValue(void* handle, const char* strKey);																				//����ICommandֵ
																								
	int  RegisterExceptionCallBack(cbException HikException=NULL, void* pUser=NULL);													// ch:ע����Ϣ�쳣�ص�

	bool getOneFramTimeOut(int nMsec, MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char ** pData);

	friend void __stdcall ReconnectCamera(unsigned int nMsgType, void* pUser);
};
