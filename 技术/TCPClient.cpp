#include"../SocketWarp/socket.h"
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


	if (!p_msock->TCP())//Step1:����socket
	{
		cout << "TCP Error = " << WSAGetLastError() << endl;
		return -1;
	}
	if (p_msock->settcp_noblock())
	{
		cout << "TCP Noblock Mode." << endl;
	}
	if (!p_msock->bind(8006, inet_addr("192.168.20.67")))//Step2:�󶨶˿ںź�ip
	{
		cout << "Bind Error = " << WSAGetLastError() << endl;
		return -1;
	}

	if (p_msock->connect(inet_addr("192.168.20.67"), 8001))
	{
		cout << "Connect Error = " << WSAGetLastError() << endl;
		return -1;
	}
	while (true)
	{
		if (p_msock->recv_noblock(pszRecv, 20) > 0)
		{
			
			strcpy(pszSend, "�ͻ���1��ʼ��������");
			p_msock->send(pszSend, 4096);
		}
		//p_msock->recv(pszRecv, 4096);
		cout << "Server Said: ";
		cout << pszRecv << endl;
		if (p_msock->send_noblock(pszSend, 20) > 0)
		{
		}
	}
	p_msock->closeHandle();
	getchar();
	return 0;
}