//#include"../SocketWarp/socket.h"
//#include <iostream>
//using namespace std;
//
//
//#pragma comment(lib,"ws2_32.lib")
//int main()
//{
//	string OutStraaa;
//	char * Buffer = NULL;
//	Buffer = new char[4096];
//	memset(Buffer, 0, 4096);
//	WSAData wsd;
//	sockaddr_in remotad;
//	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
//	{
//		cout << "WSAStartup Error = " << WSAGetLastError() << endl;
//		return 0;
//	}//Step1：加载库
//	Socket * c_msocket = new Socket;
//	int nRet = 0;
//	if (c_msocket->UDP())
//	{
//		cout << "UDP Open Success" << endl;
//	};//启动UDP
//
//	c_msocket->bind(8006, inet_addr("192.168.20.67"));
//
//	while(true)
//	{
//		//nRet = sendto(c_msocket->)
//		cout << "Input say: 123 mutouren";
//		nRet = c_msocket->sendto("123 mutouren", strlen("123 mutouren"), inet_addr("192.168.20.67"), 8001);
//		c_msocket->recvfrom(Buffer, 4096, &remotad.sin_addr.s_addr, &remotad.sin_port);
//		cout << "Server Said:";
//		cout << Buffer << endl;
//	}
//	
//	c_msocket->closeHandle();
//
//
//
//
//
//
//
//
//	getchar();
//	return 0;
//}
