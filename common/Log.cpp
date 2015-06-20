#include "Log.h"
#include <time.h>

Log* Log::instance = NULL;
fstream Log::file;
string Log::logFile;
const char* Log::fl;
int Log::line;

Log::Log()
{
	//logFile = "mylog.log";
}

Log::~Log()
{
	if(instance != NULL)
	{
		delete instance;
		instance = NULL;
		file.close();
	}
}

Log* Log::GetInstance(const char *fl, int line)
{
	Log::fl = fl;
	Log::line = line;

	if(instance == NULL)
	{
		instance = new Log;
	}
	return instance;
}

void Log::Init(const string &logFile)
{
	Log::logFile = logFile;
	file.open(logFile,ios::out|ios::app);
}

int Log::WriteLog(const char *fmt, ...)
{
	memset(logBuf,0,LOGBUFSIZE);
	
	time_t t = time(NULL);
	struct tm *curTime = localtime(&t);
	char fmtbuf[255]={0};
	sprintf(fmtbuf,"[%4d-%02d-%02d %02d:%02d:%02d][%s:%d] ",
		curTime->tm_year + 1900,curTime->tm_mon + 1,
		curTime->tm_mday,curTime->tm_hour,
		curTime->tm_min,curTime->tm_sec,fl,line);

	file << fmtbuf;
	
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(logBuf, fmt, argptr);
	va_end(argptr);

	file << logBuf << endl;
	file.flush();
	
	return(cnt);
}

