#include"socket.h"
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
int main()
{
	Socket * p_msock = new Socket;
		
	sockaddr_in siRemote;	//Զ������
	string Input;
	char * pszSend = NULL;
	pszSend = new char[4096];
	memset(pszSend, 0, 4096);
	char * pszRecv = NULL;	//���ջ�����ָ��
	pszRecv = new char[4096];//4096,TCP�����
	memset(pszRecv, 0, 4096);
	int nRet = 0;
	int dwSendSize = 0;
	WSAData wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup Error = " << WSAGetLastError() << endl;
		return 0;
	}//Step0�����ؿ�

	if (!p_msock->TCP())	//Step1:�����׽���
	{
		cout << "TCP Error = " << WSAGetLastError() << endl;
		return -1;
	}
	p_msock->settcp_noblock();
	if (p_msock->bind(8001, inet_addr("192.168.20.67")) == false)	//Step2:�󶨶˿ں�ip
	{
		cout << "bind Error = " << WSAGetLastError() << endl;
		return -1;
	}

	if (!p_msock->listen(1))	//Step3:��ʼ����
	{
		cout << "listen Error = " << WSAGetLastError() << endl;
		return -1;
	}
	while (true)
	{
		SOCKET sockConn = p_msock->accept(0, 0);
		//pszRecv = "��������ʼ��������";
		strcpy(pszSend, "��������ʼ��������");
		/*p_msock->send(pszSend, 4096);
		p_msock->recv(pszRecv, 4096);*/
		if (::send(sockConn, pszSend, 4096, 0) > 0)
		{
		}
		//::send(sockConn, pszSend, 4096, 0);
		if (::recv(sockConn, pszRecv, 4096, 0) > 0)
		{
			cout << "Client Said: ";
			cout << pszRecv << endl;

		}
		//::recv(sockConn, pszRecv, 4096, 0);
		
	}








	getchar();
	return 0;
}