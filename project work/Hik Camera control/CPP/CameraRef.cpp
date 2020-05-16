#include "CameraRef.h"

MV_CC_DEVICE_INFO_LIST* CcameraRefHikVision::pstDevList = NULL;																			//�豸��Ϣ�ṹ��
vector< CcameraRefHikVision* > CcameraRefHikVision::vec_p_cam ;
map<unsigned char*, MV_CC_DEVICE_INFO*> CcameraRefHikVision::m_seq;
Image_Process<unsigned char*> CcameraRefHikVision::Image_handle(DEFAULT_IMAGE_BUFFER_SIZE, IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

void formatIpAddress(unsigned int &rawIpAddress)	//��ʽ����ӡip��ַ
{
	int nIp1 = (rawIpAddress & 0xff000000) >> 24;	//ȡip��8λ
	int nIp2 = (rawIpAddress & 0x00ff0000) >> 16;	
	int nIp3 = (rawIpAddress & 0x0000ff00) >> 8;
	int nIp4 = (rawIpAddress & 0x000000ff);			//C���ַ������
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


void __stdcall ReconnectCamera(unsigned int nMsgType, void* pUser)										//��������ص�
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
	pdata_common->shutter = 0;																							//�ع�ʱ��
	pdata_common->trigger = TRUE;																						//�������ģʽ״̬
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
		assert(pstDevList != NULL);		//�豸�б���Ϊ��
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


int CcameraRefHikVision::StopGrabbing()												//ֹͣץͼ
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


bool CcameraRefHikVision::getCameraStatus()															//�����������״̬
{
	return m_bOpenDevice;
}


bool CcameraRefHikVision::getGrabbingStatus()
{
	return m_bStartGrabbing;
}


int CcameraRefHikVision::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)							//ö���豸
{
	int nRet = 0;
	assert(pstDevList != NULL);		//�豸�б���Ϊ��
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
	if (MV_OK != nRet)
	{
		throw nRet;
	}

	for (unsigned int n = 0; n < pstDevList->nDeviceNum; n++)										//��������豸����Ϣ
		{
			cout << "[" << n << "] " << pstDevList->pDeviceInfo[n]->SpecialInfo.stGigEInfo.chModelName << endl;
			formatIpAddress(pstDevList->pDeviceInfo[n]->SpecialInfo.stGigEInfo.nCurrentIp);			//��ӡIp
		}
	return MV_OK;
}


int CcameraRefHikVision::OpenCamera(UINT ind)																//���豸
{
	num = ind;
	int nRet = 0;
	if (ind < 0 || ind > pstDevList->nDeviceNum)
		return -99;

	if (NULL == m_hDevHandle)	//��������ڴ������
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
		nRet = MV_CC_OpenDevice(m_hDevHandle);		//���豸
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


int CcameraRefHikVision::GetOptimalPacketSize()				//��ȡ������ݰ���С
{
	assert(m_hDevHandle != NULL);
	if (m_hDevHandle != NULL)
	{
		return MV_CC_GetOptimalPacketSize(m_hDevHandle);	
	}

	return COMMON_ERROE_CODE;
}


int CcameraRefHikVision::GetIntValue(const char* strKey, unsigned int* pnValue)					//������INT����ֵ
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


int CcameraRefHikVision::SetIntValue(const char * strKey, unsigned int nValue)			//�������Int���͵�����
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


int CcameraRefHikVision::SetEnumValue(const char* strKey, unsigned int nValue)												//����ö������
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


int CcameraRefHikVision::SetFloatValue(const char* strKey, float fValue)													//���ø�������ֵ
{
	assert(strKey != NULL);
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}


int CcameraRefHikVision::GetBoolValue(const char* strKey, bool* pbValue)													//���BOOL����ֵ
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


int CcameraRefHikVision::GetStringValue(const char* strKey, char* strValue, unsigned int nSize)								//���String����
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


bool CcameraRefHikVision::SetStartGrabbing()																				//��ʼץͼ
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


//int CcameraRefHikVision::RegisterImageCallBack(ImageCallBackFunc func, void* pUser)										//ע��ͼ��ץȡ�ص�����
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


int  CcameraRefHikVision::RegisterExceptionCallBack(cbException HikException, void* pUser)									//ע���쳣�ص�����
{
	int nRet = MV_CC_RegisterExceptionCallBack(m_hDevHandle, HikException, pUser);
	if (MV_OK != nRet)
		throw nRet;

	return nRet;
}


int CcameraRefHikVision::GetCurFrameNumber()																				//���ͼ��֡��
{
	int nFrameNum = 0;
	/*if (m_bStartGrabbing && stImageInfo.nFrameCounter != 0)
	{
		nFrameNum = stImageInfo.nFrameNum;
	}*/

	return nFrameNum;
}


int CcameraRefHikVision::GetCurTimeStamp()																					//��ȡʱ���
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


int CcameraRefHikVision::initialize(UINT ind)			//�����ʼ��
{
	try
	{
		//���豸
		OpenCamera(ind);

		//ע���쳣�ص�����
		RegisterExceptionCallBack(&ReconnectCamera, this);
		SetIntValue("Height", IMAGE_HEIGHT);
		//�����������ݰ���С
		int packageSize = GetOptimalPacketSize();

		if (packageSize > 0)
		{
			SetIntValue("GevSCPSPacketSize", packageSize);
		}

		//�رմ���ģʽ
		SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);

		//�������ɼ�ģʽ
		SetEnumValue("AcquisitionMode", MV_ACQ_MODE_CONTINUOUS);

		//���Զ�����
		SetEnumValue("GainAuto", MV_GAIN_MODE_OFF);
		SetFloatValue("Gain", DEFAULT_FLOAT_ZERO);
		
		//��ƽ��
		SetEnumValue("BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_ONCE);

		//�������ع�
		//SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
		SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

		SetFloatValue("ExposureTime", DEFAULT_EXPOSURE_TIME);

		//���������ģʽ
		SetEnumValue("DeviceLinkHeartbeatMode", MV_CAM_HEART_BEAT_MODE_OFF);

		//���������Ƶʹ��
		SetBoolValue("AcquisitionLineRateEnable", true);

		//������Ƶ
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


int  CcameraRefHikVision::parameters(UINT ind, bool& _bInit_light_out) 												//�����������
{
	assert(m_hDevHandle != NULL && getCameraStatus());

	try
	{
		//����ROI��ˮƽ����ƫ����
		size_t offsetX = 0;
		GetIntValue("OffsetX", &offsetX);
		(pdata_common->offsetx == offsetX) ? 1 : SetIntValue("OffsetX", pdata_common->offsetx);


		//����ROI�Ĵ�ֱ����ƫ����
		size_t offsetY = 0;
		GetIntValue("OffsetY", &offsetY);
		(pdata_common->offsety == offsetY) ? 1 : SetIntValue("OffsetY", pdata_common->offsety);


		//����ROI�Ŀ�
		size_t widthMax = 0;
		GetIntValue("WidthMax", &widthMax);
		pdata_common->width_max = widthMax;

		size_t width = 0;
		GetIntValue("Width", &width);
		(pdata_common->width == width) ? 1 : SetIntValue("Width", (size_t)pdata_common->width);
	

		//����ROI�ĸ�
		size_t HeightMax = 0;
		GetIntValue("HeightMax", &HeightMax);
		pdata_common->height_max = HeightMax;

		size_t height = 0;
		GetIntValue("Height", &height);
		(pdata_common->height == height) ? 1 : SetIntValue("Height", (size_t)pdata_common->height);
		

		//��ȡ�������ع�ʱ��
		float ExposureTime = 0;
		GetFloatValue("ExposureTime", &ExposureTime);
		(pdata_common->shutter != ExposureTime) ? SetFloatValue("ExposureTime", pdata_common->shutter) : pdata_common->shutter = ExposureTime;

		size_t lineRate = 0;
		GetIntValue("AcquisitionLineRate", &lineRate);
		(pdata_common->frame_rate != lineRate) ? SetIntValue("AcquisitionLineRate", pdata_common->frame_rate) : pdata_common->frame_rate = lineRate;

		//��ȡ����ģʽ
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


bool CcameraRefHikVision::SetStaicChunkNodeMapSize()																	//û���
{
	return this->GetOptimalPacketSize();
}


bool CcameraRefHikVision::SetAcqFrameCount(int _numimg, bool& _bSucess)													//����һ�δ����ɼ���֡��
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