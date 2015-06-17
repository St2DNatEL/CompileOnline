#ifndef __MY_DEF__
#define __MY_DEF__

/*
	file		:	MyDef.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include <iostream>  
#include <cstdio>  
#include <Winsock2.h>
#include <string>
#include <vector>

using namespace std;

#define LOG(msg,file,line) printf("%s:%d %s\n",file,line,msg)

const int RCVBUFMAX = 10240;

enum RTIN
{
	RT_ERR = 0,
	RT_OK = 1,
	RT_ERR_SOCKET,
	RT_ERR_POINT,
	RT_ERR_BIND,
	RT_ERR_LISTEN,
	RT_ERR_DLL,
	RT_ERR_PARSE,

	RT_NUM
};

const string RT_INFO[RT_NUM] = {
	"error",
	"OK",
	"socket error",
	"point error",
	"bind error",
	"listen error",
	"init dll error",
	"parese error",
};

enum WORKTYPE
{
	MYWORK,
};

int Split(vector<string> &vc, const string &str, const string &sp);

int MacToDos(char *filePath);

#endif