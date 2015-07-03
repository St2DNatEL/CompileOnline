#ifndef __COMPILEWORK_H__
#define __COMPILEWORK_H__

/*
	file		:	work.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "work.h"
#include "../common/my_define.h"
#include "../http/request_http.h"
#include "../http/response_http.h"


const string COMPILEEXE = "E:\\vs2012\\VC\\bin\\cl.exe";
const string COMPILEEVN = "E:\\vs2012\\Common7\\Tools\\vsvars32.bat";
const string TEXTAREANAME = "text_code";
const string COMPILEWOKRDIR = "F:\\work_nc\\MyProject\\CompileOnline\\src\\html\\";

class CompileWork : public Work
{
public:
	CompileWork();
	virtual ~CompileWork();

	virtual int DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen);

	void InitFilePath();
	void SetCppPathName(const string &cppPath, const string &cppName);
	void SetCompileResultPathName(const string &filePath, const string &fileName);
	void SetExePathNameResult(const string &filePath, const string &fileName, const string &resultFile);
	
private:
	int DetectionTextArea();
	int GetTextAreaValue();
	int WriteCPPToFile();
	int CompileCPP();
	int ReadCompileResult();
	int ExecuteEXE();
	int ReadExecuteResult();
	string ResponseSendBuf(const string &rtMsg);
	int ResponseStaticPage(string &respMsg);
	int ResponseNotFound(string &respMsg);
	string ResponseNotFind(string &respMsg);
	int DoCompile(string &);

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

