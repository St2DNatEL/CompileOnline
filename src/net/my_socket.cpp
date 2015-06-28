#include "my_socket.h"

MySocket::MySocket(const string &sip, const int port):mSIP(sip),mPort(port)
{
	myWork = NULL;
}

MySocket::~MySocket(void)
{

}

int MySocket::InitSocket(const string &sip, const int port)
{
	myWork = NULL;
	mSIP = sip;
	if(sip.empty())
		mSIP = "0.0.0.0";
	mPort = port;
	if(port == 0)
		mPort = 8888;
	
	int rt =  InitDLL();
	if(rt != RT_OK)
		return rt;
	
	rt = GenSocket();
	if(rt != RT_OK)
		return rt;
	
	rt = Listen();
	if(rt != RT_OK)
		return rt;

	printf("socket init success ip=%s port=%d\n",mSIP.c_str(),mPort);
	return RT_OK;
}

int MySocket::InitDLL()
{
	#pragma comment(lib, "ws2_32.lib")
	// ����socket��̬���ӿ�(dll)  
    WORD wVersionRequested;  
    WSADATA wsaData;    // ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��  
    int err;  
       
    wVersionRequested = MAKEWORD( 1, 1 );   // ����1.1�汾��WinSock��  
       
    err = WSAStartup( wVersionRequested, &wsaData );  
    if ( err != 0 ) {  
        return RT_ERR_DLL;          // ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup  
    }  
       
    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 ) {  
        // ���������ֽ��ǲ���1�����ֽ��ǲ���1��ȷ���Ƿ������������1.1�汾  
        // ����Ļ�������WSACleanup()�����Ϣ����������  
        WSACleanup( );  
        return RT_ERR_DLL;   
    }  
	return RT_OK;
}


int MySocket::GenSocket()
{
	sockSrv = socket(AF_INET, SOCK_STREAM, 0); 
	if(sockSrv < 0)
		return RT_ERR_SOCKET;
	return RT_OK;
}

int MySocket::Listen()
{
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // ��INADDR_ANYת��Ϊ�����ֽ��򣬵��� htonl(long��)��htons(����)  
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(mPort);  

	int rt = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); // �ڶ�����Ҫǿ������ת��  
	if(rt < 0)
	{
		closesocket(sockSrv);  
		return RT_ERR_BIND;
	}

	rt = listen(sockSrv, 10);
	if(rt < 0)
	{
		closesocket(sockSrv);  
		return RT_ERR_LISTEN;
	}
	
	return RT_OK;
}

void MySocket::Do()
{
	while(1)
	{
		SOCKADDR_IN  addrClient;  
		int len = sizeof(SOCKADDR);  
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  

		char recvBuf[RCVBUFMAX];  
		memset(recvBuf, 0, RCVBUFMAX);
		len = recv(sockConn, recvBuf, RCVBUFMAX, 0);
		if(len < 0)
		{//error
			closesocket(sockConn);  
			continue;
		}
		else if(len == 0)
		{//client closed
			closesocket(sockConn);  
			continue;
		}

		if(myWork == NULL)
			return;
		
		//cout << recvBuf << endl;
		string sendbuf;
		int sendlen = 0;
		
		myWork->DoWork(recvBuf,strlen(recvBuf),sendbuf,sendlen);
		len = send(sockConn, sendbuf.c_str(), sendbuf.length(), 0);
		if(len < 0)
		{
			//
			closesocket(sockConn);  
			continue;
		}
		closesocket(sockConn);  
	}
	
}

int MySocket::SetWork(Work * myWork)
{
	if(myWork == NULL)
	{
		return RT_ERR_POINT;
	}
	this->myWork = myWork;
	return RT_OK;
}


