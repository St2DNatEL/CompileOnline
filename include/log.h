#ifndef __LOG_H__
#define __LOG_H__

/*
	file		:	log.h
	Description	:	仅支持单线程模式

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	DateTime		:	2015-06-20 22:59
*/


#include <stdarg.h>
#include <string>
#include <fstream>
using namespace std;

#define MyLogInstance Log::GetInstance(__FILE__,__LINE__)

const int LOGBUFSIZE = 204800;

class Log
{
private:
	Log();
	~Log();

public:
	static Log* GetInstance(const char *fl=__FILE__, int line=__LINE__);
	static void Init(const string &logFile);
	int WriteLog(const char *fmt, ...);
	

private:
	char logBuf[LOGBUFSIZE];
	static Log *instance;
	static fstream file;
	static string logFile;
	static const char *fl;
	static int line;
};



#endif

