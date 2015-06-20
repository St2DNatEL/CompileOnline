#ifndef __COMPILEWORK_H__
#define __COMPILEWORK_H__

/*
	file		:	Work.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "Work.h"
#include "../common/Mydef.h"
#include "../Http/RequestHttp.h"
#include "../Http/ResponseHttp.h"

const string COMPILEEXE = "E:\\vs2012\\VC\\bin\\cl.exe";
const string COMPILEEVN = "E:\\vs2012\\Common7\\Tools\\vsvars32.bat";
const string TEXTAREANAME = "text_code";

class CompileWork : public Work
{
public:
	CompileWork();
	virtual ~CompileWork();

	virtual int DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen);

	void SetCppPathName(const string &cppPath, const string &cppName);
	void SetCompileResultPathName(const string &filePath, const string &fileName);
	void SetExePathNameResult(const string &filePath, const string &fileName, const string &resultFile);
	
private:
	int GetTextAreaValue();
	int WriteCPPToFile();
	int CompileCPP();
	int ReadCompileResult();
	int ExecuteEXE();
	int ReadExecuteResult();
	string ReturnSendBuf(const string &rtMsg);

private:
	RequestHttp *requestHttp;
	ResponseHttp *responseHttp;

	string cppPathName;
	string cppPath;
	string cppName;

	string compileResultPathName;
	string compileResultPath;
	string compileResultName;
	string compileInfo;

	string exeResultPathName;
	string exePathName;
	string exeName;
	string exePath;
	string exeResult;
	string exeInfo;

	string textAreaValue;
};




#endif

