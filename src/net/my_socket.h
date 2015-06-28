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