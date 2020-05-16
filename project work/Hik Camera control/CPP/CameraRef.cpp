#include "CameraRef.h"

MV_CC_DEVICE_INFO_LIST* CcameraRefHikVision::pstDevList = NULL;																			//设备信息结构体
vector< CcameraRefHikVision* > CcameraRefHikVision::vec_p_cam ;
map<unsigned char*, MV_CC_DEVICE_INFO*> CcameraRefHikVision::m_seq;
Image_Process<unsigned char*> CcameraRefHikVision::Image_handle(DEFAULT_IMAGE_BUFFER_SIZE, IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

void formatIpAddress(unsigned int &rawIpAddress)	//格式化打印ip地址
{
	int nIp1 = (rawIpAddress & 0xff000000) >> 24;	//取ip高8位
	int nIp2 = (rawIpAddress & 0x00ff0000) >> 16;	
	int nIp3 = (rawIpAddress & 0x0000ff00) >> 8;
	int nIp4 = (rawIpAddress & 0x000000ff);			//C类地址主机号
	cout << nIp1 << "." << nIp2 << "." << nIp3 << "." << nIp4<<endl;
}


void ShowErrorMsg(int nErrorNum)
{
	string errorMsg = "";
	switch (nErrorNum)
	{
		case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
		case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
		case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
		case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
		case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
		case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
		case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
		case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
		case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
		case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
		case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
		case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
		case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
		case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
		case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
		case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
		case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
	}
	cout << errorMsg.c_str() << endl;
}


void __stdcall ReconnectCamera(unsigned int nMsgType, void* pUser)										//相机重连回调
{
	if (nMsgType == MV_EXCEPTION_DEV_DISCONNECT)
	{
		CcameraRefHikVision * pThis = (CcameraRefHikVision*) pUser;

		
		if (pThis->getCameraStatus())
		{
			int nRet = MV_OK;
			pThis->closeCamera();

			BOOL bConnected = FALSE;
			while (1)
			{
				nRet = pThis->OpenCamera(pThis->num);
				if (MV_OK == nRet)
				{
					pThis->RegisterExceptionCallBack(ReconnectCamera, pUser);
					bConnected = TRUE;
					break;
				}
				else
				{
					Sleep(100);
				}
			}

			for (;;)
			{
				nRet = pThis->initialize(pThis->num);
				if (MV_OK == nRet)
					break;
				else
					Sleep(100);
			}
			
			if (bConnected && pThis->getGrabbingStatus())
			{
				pThis->SetStartGrabbing();
			}
		}
	}
}


CcameraRefHikVision::CcameraRefHikVision(Camera_struct* _pdata): CcameraRefCommon(_pdata), m_bOpenDevice(false), m_bStartGrabbing(false)
{
	pdata_common->width = IMAGE_WIDTH;																							//ROIWIDTH
	pdata_common->height = IMAGE_HEIGHT;																							//ROIHEIGHT
	pdata_common->offsetx = 0;																							//XOFFSET
	pdata_common->offsety = 0;																							//YOFFSET
	pdata_common->shutter = 0;																							//曝光时间
	pdata_common->trigger = TRUE;																						//相机触发模式状态
	m_hDevHandle = NULL;
}


CcameraRefHikVision::~CcameraRefHikVision()
{
	if (m_bOpenDevice) this->closeCamera();
	if(m_hDevHandle) MV_CC_DestroyHandle(m_hDevHandle);
	m_hDevHandle = NULL;
	m_bOpenDevice = false;
	m_bStartGrabbing = false;

	if (im_data.img_acq)
	{
		free(im_data.img_acq);
		im_data.img_acq = NULL;
	}

}


int CcameraRefHikVision::GlobalInitialize(void)
{
	if (NULL == pstDevList)
	{
		pstDevList = new MV_CC_DEVICE_INFO_LIST{ 0 };
		int nRet = 0;
		assert(pstDevList != NULL);		//设备列表不能为空
		nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
		if (MV_OK != nRet)
		{
			throw nRet;
		}

		assert(pstDevList->nDeviceNum > 0);
		
		for (int i = 0; i < (pstDevList->nDeviceNum); i++)
		{
			m_seq[pstDevList->pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber] = 
				pstDevList->pDeviceInfo[i];
		}
		return MV_OK;
	}
	else
		return MV_OK;
}

void CcameraRefHikVision::GlobalTerminate(void)
{
	if (pstDevList)
	{
		delete pstDevList;
		pstDevList = NULL;
	}

	vec_p_cam.clear();
}

bool CcameraRefHikVision::getOneFramTimeOut(int nMsec, MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char ** pData)
{
	if (this->m_bStartGrabbing && m_hDevHandle)
	{
		int nRet;
		nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, *pData, DEFAULT_IMAGE_BUFFER_SIZE, pFrameInfo, nMsec);
		if (MV_OK != nRet)
		{
			try
			{
				throw nRet;
			}
			catch (int &eCorr)
			{
				ShowErrorMsg(eCorr);

			}
			
			return false;
		}
	}

	return true;
}


bool CcameraRefHikVision::RetrieveResult(unsigned int _timeoutMs)
{
	if (getGrabbingStatus())
	{
		if (NULL == im_data.img_acq)
			im_data.img_acq = (unsigned char*)malloc(DEFAULT_IMAGE_BUFFER_SIZE);

		if (getOneFramTimeOut(_timeoutMs, &stInfo, &im_data.img_acq))
		{
			if (im_data.img_acq)
			{
				im_data.bGrabSuccess = true;
				im_data.frmnum = stInfo.nFrameNum;
				im_data.tmstamp = stInfo.nHostTimeStamp;

			}
			else
			{
				im_data.bGrabSuccess = false;
				free(im_data.img_acq);
				im_data.img_acq = NULL;
			}
		}
		else
		{
			im_data.bGrabTimeout = true;
			im_data.bGrabSuccess = false;
			free(im_data.img_acq);
			im_data.img_acq = NULL;
		}
	}

	return im_data.bGrabSuccess;
}


int CcameraRefHikVision::closeCamera()
{
	if (m_hDevHandle != NULL && m_bOpenDevice)
	{
		if (m_bStartGrabbing) this->StopGrabbing();
		int nRet = MV_CC_CloseDevice(m_hDevHandle);
		if (MV_OK != nRet)
		{
			throw nRet;
		}
		m_hDevHandle = NULL;
		m_bOpenDevice = false;
		return nRet;
	}
	return MV_OK;
}


int CcameraRefHikVision::StopGrabbing()												//停止抓图
{
	int nRet = 0;
	if (m_bStartGrabbing)
	{
		 nRet = MV_CC_StopGrabbing(m_hDevHandle);
		if (MV_OK != nRet)
		{
			return nRet;
		}
	}
	m_bStartGrabbing = false;

	return nRet;
}


bool CcameraRefHikVision::getCameraStatus()															//返回相机开关状态
{
	return m_bOpenDevice;
}


bool CcameraRefHikVision::getGrabbingStatus()
{
	return m_bStartGrabbing;
}


int CcameraRefHikVision::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)							//枚举设备
{
	int nRet = 0;
	assert(pstDevList != NULL);		//设备列表不能为空
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	for (unsigned int n = 0; n < pstDevList->nDeviceNum; n++)										//输出在线设备的信息
		{
			cout << "[" << n << "] " << pstDevList->pDeviceInfo[n]->SpecialInfo.stGigEInfo.chModelName << endl;
			formatIpAddress(pstDevList->pDeviceInfo[n]->SpecialInfo.stGigEInfo.nCurrentIp);			//打印Ip
		}
	return MV_OK;
}


int CcameraRefHikVision::OpenCamera(UINT ind)																//打开设备
{
	num = ind;
	int nRet = 0;
	if (ind < 0 || ind > pstDevList->nDeviceNum)
		return -99;

	if (NULL == m_hDevHandle)	//句柄不存在创建句柄
	{
		//cout << pstDevList->pDeviceInfo[ind]->SpecialInfo.stGigEInfo.chSerialNumber << endl;
		strcpy_s(pdata_common->sn, (const char *)pstDevList->pDeviceInfo[ind]->SpecialInfo.stGigEInfo.chSerialNumber);
		cout << pdata_common->sn << endl;
		nRet = MV_CC_CreateHandle(&m_hDevHandle, m_seq[pstDevList->pDeviceInfo[ind]->SpecialInfo.stGigEInfo.chSerialNumber]);
		//nRet = 100;
		if (nRet != MV_OK)
		{
			throw nRet;	
		}
		
		assert(m_hDevHandle != NULL);
	}
	
	if (false == m_bOpenDevice)
	{
		nRet = MV_CC_OpenDevice(m_hDevHandle);		//打开设备
		if (nRet != MV_OK)
		{
			MV_CC_DestroyHandle(m_hDevHandle);
			m_hDevHandle = NULL;
			throw nRet;
		}
	}

	m_bOpenDevice = true;
	return MV_OK;
}


int CcameraRefHikVision::GetOptimalPacketSize()				//获取最佳数据包大小
{
	assert(m_hDevHandle != NULL);
	if (m_hDevHandle != NULL)
	{
		return MV_CC_GetOptimalPacketSize(m_hDevHandle);	
	}

	return COMMON_ERROE_CODE;
}


int CcameraRefHikVision::GetIntValue(const char* strKey, unsigned int* pnValue)					//获得相机INT类型值
{
	assert(strKey != NULL && pnValue != NULL);
	if (NULL == strKey || NULL == pnValue)
	{
		throw MV_E_PARAMETER;
	}

	MVCC_INTVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	*pnValue = stParam.nCurValue;

	return MV_OK;
}


int CcameraRefHikVision::SetIntValue(const char * strKey, unsigned int nValue)			//设置相机Int类型的数据
{
	if (NULL == strKey)
		throw MV_E_PARAMETER;

	int nRet = MV_CC_SetIntValue(m_hDevHandle, strKey, nValue);
	if (MV_OK != nRet)
		throw nRet;

	return nRet;
}


int CcameraRefHikVision::GetEnumValue(const char* strKey, unsigned int* pnValue)
{
	assert(strKey != NULL && pnValue != NULL);
	if(NULL == strKey || NULL == pnValue)
		throw MV_E_PARAMETER;
	MVCC_ENUMVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	int nRet = MV_CC_GetEnumValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	*pnValue = stParam.nCurValue;

	return MV_OK;
}


int CcameraRefHikVision::SetEnumValue(const char* strKey, unsigned int nValue)												//设置枚举属性
{
	assert(strKey != NULL);
	if (NULL == strKey)
	{
		throw MV_E_PARAMETER;
	}
	int nRet = MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
	if (MV_OK != nRet)
		throw nRet;

	return nRet;
}


int CcameraRefHikVision::GetFloatValue(const char* strKey, float* pfValue)
{
	assert(strKey != NULL && pfValue != NULL);
	if(NULL == strKey || NULL == pfValue)
		throw MV_E_PARAMETER;

	MVCC_FLOATVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_FLOATVALUE));
	int nRet = MV_CC_GetFloatValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	*pfValue = stParam.fCurValue;

	return MV_OK;
}


int CcameraRefHikVision::SetFloatValue(const char* strKey, float fValue)													//设置浮点属性值
{
	assert(strKey != NULL);
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}


int CcameraRefHikVision::GetBoolValue(const char* strKey, bool* pbValue)													//获得BOOL属性值
{
	assert(strKey != NULL && pbValue != NULL);
	if (NULL == strKey || NULL == pbValue)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}


int CcameraRefHikVision::SetBoolValue(const char* strKey, bool pbValue)
{
	assert(strKey != NULL);
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return  MV_CC_SetBoolValue(m_hDevHandle, strKey, pbValue);
}


int CcameraRefHikVision::GetStringValue(const char* strKey, char* strValue, unsigned int nSize)								//获得String属性
{
	assert(strKey != NULL && strValue != NULL);
	if (NULL == strKey || NULL == strValue)
	{
		throw MV_E_PARAMETER;
	}

	MVCC_STRINGVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_STRINGVALUE));
	int nRet = MV_CC_GetStringValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	strcpy_s(strValue, nSize, stParam.chCurValue);

	return MV_OK;
}


int CcameraRefHikVision::SetStringValue(const char* strKey, const char* strValue)
{
	assert(strKey != NULL);
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}


int CcameraRefHikVision::SetCommandValue(void* handle, const char* strKey)
{
	assert(strKey != NULL && handle != NULL);
	if (handle != NULL && strKey)
	{
		int nRet = 0;
		nRet = MV_CC_SetCommandValue(handle, strKey);
		if (MV_OK != nRet)
			throw nRet;
	}
	return -1;
}


bool CcameraRefHikVision::SetSoftwareTrigger()
{
	assert(NULL != m_hDevHandle);
	int nRet = 0;
	nRet = SetEnumValue("TriggerSource", (unsigned int)MV_TRIGGER_SOURCE_SOFTWARE);
	//nRet = MV_CC_SetTriggerSource(m_hDevHandle, (unsigned int)MV_TRIGGER_SOURCE_SOFTWARE);
	if (MV_OK != nRet)
	{
		return false;
	}

	return true;
}


bool CcameraRefHikVision::SetStartGrabbing()																				//开始抓图
{
	int nRet = MV_OK;
	assert(m_hDevHandle != NULL && m_bOpenDevice == true && m_bStartGrabbing != true);
	if(m_bOpenDevice == true && m_hDevHandle && m_bStartGrabbing != true)
		nRet = MV_CC_StartGrabbing(m_hDevHandle);

	if (nRet != MV_OK)
	{
		MV_CC_StopGrabbing(m_hDevHandle);
		m_bStartGrabbing = false;
		throw nRet;
	}

	m_bStartGrabbing = true;
	return m_bStartGrabbing;
}


//int CcameraRefHikVision::RegisterImageCallBack(ImageCallBackFunc func, void* pUser)										//注册图像抓取回调函数
//{
//	int nRet = 0;
//	assert(func != NULL);
//
//	nRet = MV_CC_RegisterImageCallBackEx(m_hDevHandle, func, m_hDevHandle);
//	if (MV_OK != nRet)
//	{
//		Sleep(100);
//		nRet = MV_CC_RegisterImageCallBackEx(m_hDevHandle, func, m_hDevHandle);
//		if (MV_OK != nRet)
//			cout << "RegisterImageCallBack Failed" << endl;
//		throw nRet;
//	}
//
//	return MV_OK;
//}


int  CcameraRefHikVision::RegisterExceptionCallBack(cbException HikException, void* pUser)									//注册异常回调函数
{
	int nRet = MV_CC_RegisterExceptionCallBack(m_hDevHandle, HikException, pUser);
	if (MV_OK != nRet)
		throw nRet;

	return nRet;
}


int CcameraRefHikVision::GetCurFrameNumber()																				//获得图像帧号
{
	int nFrameNum = 0;
	/*if (m_bStartGrabbing && stImageInfo.nFrameCounter != 0)
	{
		nFrameNum = stImageInfo.nFrameNum;
	}*/

	return nFrameNum;
}


int CcameraRefHikVision::GetCurTimeStamp()																					//获取时间戳
{
	int stmp = 0;
	int low32 = stInfo.nDevTimeStampLow;
	int high32 = stInfo.nDevTimeStampHigh;
	UINT64 time = (high32 << 32);
	time = time | (high32);
	/*if (m_bStartGrabbing && stImageInfo.nFrameCounter != 0)
	{
		stmp = stImageInfo.nHostTimeStamp;
	}*/

	return stmp;
}


int CcameraRefHikVision::initialize(UINT ind)			//相机初始化
{
	try
	{
		//打开设备
		OpenCamera(ind);

		//注册异常回调函数
		RegisterExceptionCallBack(&ReconnectCamera, this);
		SetIntValue("Height", IMAGE_HEIGHT);
		//设置网络数据包大小
		int packageSize = GetOptimalPacketSize();

		if (packageSize > 0)
		{
			SetIntValue("GevSCPSPacketSize", packageSize);
		}

		//关闭触发模式
		SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);

		//打开连续采集模式
		SetEnumValue("AcquisitionMode", MV_ACQ_MODE_CONTINUOUS);

		//打开自动增益
		SetEnumValue("GainAuto", MV_GAIN_MODE_OFF);
		SetFloatValue("Gain", DEFAULT_FLOAT_ZERO);
		
		//白平衡
		SetEnumValue("BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_ONCE);

		//打开连续曝光
		//SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
		SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

		SetFloatValue("ExposureTime", DEFAULT_EXPOSURE_TIME);

		//打开相机心跳模式
		SetEnumValue("DeviceLinkHeartbeatMode", MV_CAM_HEART_BEAT_MODE_OFF);

		//设置相机行频使能
		SetBoolValue("AcquisitionLineRateEnable", true);

		//设置行频
		size_t line_rate = 0;
		//SetIntValue("AcquisitionLineRate", DEFAULT_ACQUISITION_LINE_RATE);
		SetIntValue("AcquisitionLineRate", DEFAULT_ACQUISITION_LINE_RATE);
		GetIntValue("AcquisitionLineRate", &line_rate);
		cout <<"Line Rate:"<<line_rate << endl;

		vec_p_cam.push_back(this);
	}
	catch (int &eCode)
	{
		ShowErrorMsg(eCode);
	}
	
	return MV_OK;
}


int  CcameraRefHikVision::parameters(UINT ind, bool& _bInit_light_out) 												//相机参数设置
{
	assert(m_hDevHandle != NULL && getCameraStatus());

	try
	{
		//设置ROI的水平方向偏移量
		size_t offsetX = 0;
		GetIntValue("OffsetX", &offsetX);
		(pdata_common->offsetx == offsetX) ? 1 : SetIntValue("OffsetX", pdata_common->offsetx);


		//设置ROI的垂直方向偏移量
		size_t offsetY = 0;
		GetIntValue("OffsetY", &offsetY);
		(pdata_common->offsety == offsetY) ? 1 : SetIntValue("OffsetY", pdata_common->offsety);


		//设置ROI的宽
		size_t widthMax = 0;
		GetIntValue("WidthMax", &widthMax);
		pdata_common->width_max = widthMax;

		size_t width = 0;
		GetIntValue("Width", &width);
		(pdata_common->width == width) ? 1 : SetIntValue("Width", (size_t)pdata_common->width);
	

		//设置ROI的高
		size_t HeightMax = 0;
		GetIntValue("HeightMax", &HeightMax);
		pdata_common->height_max = HeightMax;

		size_t height = 0;
		GetIntValue("Height", &height);
		(pdata_common->height == height) ? 1 : SetIntValue("Height", (size_t)pdata_common->height);
		

		//获取或设置曝光时间
		float ExposureTime = 0;
		GetFloatValue("ExposureTime", &ExposureTime);
		(pdata_common->shutter != ExposureTime) ? SetFloatValue("ExposureTime", pdata_common->shutter) : pdata_common->shutter = ExposureTime;

		size_t lineRate = 0;
		GetIntValue("AcquisitionLineRate", &lineRate);
		(pdata_common->frame_rate != lineRate) ? SetIntValue("AcquisitionLineRate", pdata_common->frame_rate) : pdata_common->frame_rate = lineRate;

		//获取触发模式
		size_t acquisitionMode = 0;
		GetEnumValue("AcquisitionMode", &acquisitionMode);
		pdata_common->acq_mode = acquisitionMode;
		//
		size_t trigger = 0;
		GetEnumValue("TriggerMode", &trigger);
		if (pdata_common->trigger)
		{
			if (trigger == MV_TRIGGER_MODE_OFF)
			{
				SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
				GetEnumValue("TriggerMode", &trigger);
				assert(trigger == MV_TRIGGER_MODE_ON);
			}
		}
		else
		{
			if (trigger == MV_TRIGGER_MODE_ON)
			{
				SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
				GetEnumValue("TriggerMode", &trigger);
				assert(trigger == MV_TRIGGER_MODE_OFF);
			}
		}
	}
	catch (int& eCode)
	{
		ShowErrorMsg(eCode);
		return -1;
	}
	return MV_OK;

}


int CcameraRefHikVision::set_camera_output(UINT ind, bool val)
{
	return 1;
}


bool CcameraRefHikVision::SetStaicChunkNodeMapSize()																	//没理解
{
	return this->GetOptimalPacketSize();
}


bool CcameraRefHikVision::SetAcqFrameCount(int _numimg, bool& _bSucess)													//设置一次触发采集的帧数
{
	_bSucess = true;
	size_t frame_count = 0;
	try
	{
		int nRet = MV_OK;
		size_t frame_count = 0;
		if (_numimg >= 0)
		{
			nRet = SetIntValue("AcquisitionBurstFrameCount ", _numimg);
			assert(GetIntValue("AcquisitionBurstFrameCount ", &frame_count) == _numimg);
		}
	}
	catch (int& eCode)
	{
		_bSucess = false;
		ShowErrorMsg(eCode);
		return false;
	}

	return true;
}

unsigned char * CcameraRefHikVision::Image_Format()
{
	return Image_handle.FormatImageToBGR(this->m_hDevHandle, im_data.img_acq, stInfo);
}