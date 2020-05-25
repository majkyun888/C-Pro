//#include <iostream>
//#include"socket.h"
//#include<string>
//using namespace std;
//#pragma comment(lib,"ws2_32.lib")
//int main()
//{
//	Socket * p_msock = new Socket;
//	
//	sockaddr_in siRemote;	//远端主机
//	string Input;
//	char * pszRecv = NULL;	//接收缓冲区指针
//	int nRet = 0;
//	int dwSendSize = 0;
//	WSAData wsd;
//	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
//	{
//		cout << "WSAStartup Error = " << WSAGetLastError() << endl;
//		return 0;
//	}//Step1：加载库
//
//	//Step2：启动SOCKET
//	if (p_msock->UDP() == false)	//启动UDP
//	{
//		cout << "socket Error = " << WSAGetLastError() << endl;
//		return -1;
//	}
//
//	//Step3：绑定端口和ip
//	if (p_msock->bind(8001, inet_addr("192.168.20.67")) == false)
//	{
//		cout << "bind Error = " << WSAGetLastError() << endl;
//		return -1;
//	}
//
//	pszRecv = new char[4096];	//申请空间
//	memset(pszRecv, 0, 4096);
//	if (pszRecv == NULL)
//	{
//		cout << "pszRecv new char Error " << endl;
//		return 0;
//	}
//	while(true)
//	{
//
//		p_msock->recvfrom(pszRecv, 4096, &siRemote.sin_addr.s_addr, &siRemote.sin_port);
//		cout << "Client Said:" << pszRecv << endl;
//		cin >> Input;
//		p_msock->sendto(Input.c_str(), Input.size(), siRemote.sin_addr.s_addr, siRemote.sin_port);
//
//	}
//	//p_msock->closeHandle();
//
//
//	//delete p_msock;
//
//
//
//	getchar();
//	return 0;
//}
//
