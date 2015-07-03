#ifndef __MY_DEF__
#define __MY_DEF__

/*
	file		:	my_define.h
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
#include "log.h"

using namespace std;

#define LOG(msg) printf("%s:%d %s\n",__FILE__,__LINE__,msg)

//#define MyLogInstance Log::GetInstance(__FILE__,__LINE__)

const int RCVBUFMAX = 409600;

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
	RT_ERR_OPEN_FILE,
	RT_ERR_COMPILE,

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
	"open file error",
	"compile error",
};

enum WORKTYPE
{
	MYWORK,
	COMPILEWORK,
};

int Split(vector<string> &vc, const string &str, const string &sp);

string& ReplaceAll(string &orgStr, const string &toReplaceStr, const string &replaceStr);

int MacToDos(const char *filePath);

#endif
