#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__

/*
	file		:	my_socket.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "../common/my_define.h"
#include "../work/work.h"

#ifdef __LINUX__
#include<stdlib.h>
#include<unistd.h> /*close()*/
#include<netinet/in.h> /*struct sockaddr_in,struct in_addr*/
#include<sys/socket.h> /*socket(),bind(),listen(),accept(),recv()*/
#else
#include <Winsock2.h>
#endif

#ifdef __LINUX__
#define SOCKET int
#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#define closesocket close
#endif

class MySocket
{
public:
	MySocket(const string &sip="0.0.0.0", const int port=8888);
	~MySocket(void);

public:
	int InitSocket(const string &sip="0.0.0.0", const int port=8888);
	int SetWork(Work *myWork);
	void Do();

private:
	int InitDLL();
	int GenSocket();
	int Listen();
	
private:
	SOCKET sockSrv;
	string mSIP;
	int mPort;
	SOCKADDR_IN addrSrv;

	Work *myWork;
};

#endif