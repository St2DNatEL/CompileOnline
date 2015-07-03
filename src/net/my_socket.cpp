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
#ifdef __WIN__
	#pragma comment(lib, "ws2_32.lib")
	// 加载socket动态链接库(dll)  
    WORD wVersionRequested;  
    WSADATA wsaData;    // 这结构是用于接收Wjndows Socket的结构信息的  
    int err;  
       
    wVersionRequested = MAKEWORD( 1, 1 );   // 请求1.1版本的WinSock库  
       
    err = WSAStartup( wVersionRequested, &wsaData );  
    if ( err != 0 ) {  
        return RT_ERR_DLL;          // 返回值为零的时候是表示成功申请WSAStartup  
    }  
       
    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 ) {  
        // 检查这个低字节是不是1，高字节是不是1以确定是否我们所请求的1.1版本  
        // 否则的话，调用WSACleanup()清除信息，结束函数  
        WSACleanup( );  
        return RT_ERR_DLL;   
    }  
#endif
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
#ifdef __LINUX__
	addrSrv.sin_addr.s_addr = htonl(INADDR_ANY); 
#else
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 将INADDR_ANY转换为网络字节序，调用 htonl(long型)或htons(整型)  
#endif
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(mPort);  

	int rt = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); // 第二参数要强制类型转换  
	if(rt < 0)
	{
#ifdef __LINUX__
		close(sockSrv);
#else
		closesocket(sockSrv);  
#endif
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
#ifdef __LINUX__
		socklen_t len = sizeof(addrClient);
#else
		int len = sizeof(SOCKADDR);  
#endif
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


